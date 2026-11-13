import json
import sys
import time
import numpy as np
from tqdm import tqdm
from pathlib import Path
from llm_sdk import Small_LLM_Model  # type: ignore[attr-defined]
from pydantic import ValidationError
from src.models import FunctionDefinition, FunctionCall
from src.parser import parse_arguments, load_json, validate_prompt_and_function
from src.generator import best_allowed, build_string_ids
from src.generator import generate_number, generate_string, generate_boolean


def load_vocab(model: Small_LLM_Model) -> dict[int, str]:
    with open(model.get_path_to_vocab_file()) as file:
        raw = json.load(file)
    return {token_id: token.replace("Ġ", " ")
            for token, token_id in raw.items()}


def build_prompt(functions: list[FunctionDefinition], request: str) -> str:
    lines = ["Available functions:"]
    for func in functions:
        params = ", ".join(f"{name}: {definition.type}"
                           for name, definition in func.parameters.items())
        lines.append(f"- {func.name}({params}): {func.description}")
    lines.append("")
    lines.append(f"Request: {request}")
    lines.append('Answer: {"name": "')
    return "\n".join(lines)


def write_output(path: Path, calls: list[FunctionCall]) -> bool:
    try:
        path.parent.mkdir(parents=True, exist_ok=True)
        with path.open("w") as file:
            json.dump([c.model_dump() for c in calls], file, indent=2)
    except OSError as error:
        print(f"Error: cannot write output file: {error}", file=sys.stderr)
        return False
    return True


def choose_function(model: Small_LLM_Model, vocab: dict[int, str],
                    names: list[str], prompt: str) -> str:
    candidates = {token_id: text for token_id, text in vocab.items()
                  if text and any(text in name for name in names)}
    quote_id = int(model.encode('"')[0][0].item())
    input_ids = model.encode(prompt)[0].tolist()
    generated = ""

    for _ in range(32):
        logits = model.get_logits_from_input_ids(input_ids)
        allowed = [token_id for token_id, text in candidates.items()
                   if any(name.startswith(generated + text) for name in names)]

        if generated in names:
            allowed.append(quote_id)

        next_id = best_allowed(logits, np.array(allowed, dtype=np.int64))
        if next_id == quote_id:
            return generated
        generated += candidates[next_id]
        input_ids.append(next_id)

    return generated


def extract_parameters(model: Small_LLM_Model, vocab: dict[int, str],
                       func_def: FunctionDefinition, prompt: str,
                       func_name: str, string_ids: np.ndarray,
                       ) -> dict[str, str | float | bool]:

    parameters: dict[str, str | float | bool] = {}
    current_json = f'{prompt}{func_name}", "parameters": {{'

    param_items = list(func_def.parameters.items())

    for i, (param_name, param_def) in enumerate(param_items):
        is_last = (i == len(param_items) - 1)
        end_char = "}" if is_last else ","

        if param_def.type == "number":
            current_json += f'"{param_name}": '
            input_ids = model.encode(current_json)[0].tolist()
            num_val, _ = generate_number(model, vocab, input_ids, end_char)
            parameters[param_name] = num_val
            current_json += f"{num_val}{end_char} "

        elif param_def.type == "string":
            current_json += f'"{param_name}": "'
            input_ids = model.encode(current_json)[0].tolist()
            str_val = generate_string(model, vocab, input_ids, string_ids)
            parameters[param_name] = str_val
            val_escaped = str_val.replace("\\", "\\\\")
            current_json += f'{val_escaped}"{end_char} '

        elif param_def.type == "boolean":
            current_json += f'"{param_name}": '
            input_ids = model.encode(current_json)[0].tolist()
            bool_val = generate_boolean(model, vocab, input_ids)
            parameters[param_name] = bool_val
            current_json += f'{"true" if bool_val else "false"}{end_char} '

    return parameters


def main() -> int:
    args = parse_arguments()

    try:
        function_definitions = load_json(args.functions_definition)
        prompts = load_json(args.input)
    except FileNotFoundError as error:
        print(f"Error: file not found: {error.filename}", file=sys.stderr)
        return 1
    except json.JSONDecodeError as error:
        print(f"Error: invalid JSON: {error}", file=sys.stderr)
        return 1
    except OSError as error:
        print(f"Error: cannot read file: {error}", file=sys.stderr)
        return 1

    result = validate_prompt_and_function(function_definitions, prompts)
    if result is None:
        return 1
    validated_functions, validated_prompts = result

    try:
        llm = Small_LLM_Model()
        vocab = load_vocab(llm)
    except Exception as error:
        print(f"Error: cannot load the model: {error}", file=sys.stderr)
        return 1

    string_ids = build_string_ids(vocab)
    names = [func.name for func in validated_functions]
    results: list[FunctionCall] = []

    start = time.time()
    try:
        for item in tqdm(validated_prompts, desc="Génération", unit="prompt"):
            prompt = build_prompt(validated_functions, item.prompt)
            func_name = choose_function(llm, vocab, names, prompt)

            func_def = next(
                (f for f in validated_functions if f.name == func_name),
                None,
            )
            if func_def is None:
                tqdm.write(f"Error: no function found for {item.prompt!r}")
                continue

            try:
                params = extract_parameters(
                    llm, vocab, func_def, prompt, func_name, string_ids
                )
                results.append(FunctionCall(
                    prompt=item.prompt, name=func_name, parameters=params
                ))
            except (ValidationError, ValueError) as error:
                tqdm.write(
                    f"Error: invalid result for {item.prompt!r}: {error}"
                    )
                continue
    except KeyboardInterrupt:
        print("\nInterrupted, saving partial results.", file=sys.stderr)
    finally:
        print(f"All done in {time.time() - start:.1f}s")
        if not write_output(args.output, results):
            return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
