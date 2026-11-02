import argparse, json, sys
import numpy as np
import time
from typing import Any
from pathlib import Path
from pydantic import ValidationError, TypeAdapter
from src.models import PromptInput, FunctionDefinition
from llm_sdk import Small_LLM_Model


def parse_arguments() -> argparse.Namespace:
    parser = argparse.ArgumentParser()

    parser.add_argument(
        "--functions_definition",
        type=Path,
        default=Path("data/input/functions_definition.json")
    )

    parser.add_argument(
        "--input",
        type=Path,
        default=Path("data/input/function_calling_tests.json")
    )

    parser.add_argument(
        "--output",
        type=Path,
        default=Path("data/output/function_calls.json")
    )

    return parser.parse_args()


def load_json(path: Path) -> Any:
    with path.open("r") as file:
        return json.load(file)


def generate_string_value(model: Small_LLM_Model, prefix_text: str, max_tokens: int = 64) -> str:
    input_ids = model.encode(prefix_text)[0].tolist()
    generated = []

    for _ in range(max_tokens):
        logits = model.get_logits_from_input_ids(input_ids)
        next_id = int(np.argmax(logits))
        token_text = model.decode([next_id])
        
        if '"' in token_text:
            last_token = token_text.split('"')[0]
            return model.decode(generated) + last_token
            
        input_ids.append(next_id)
        generated.append(next_id)
    return model.decode(generated)


def load_vocab(model: Small_LLM_Model) -> dict[int, str]:
    with open(model.get_path_to_vocab_file()) as file:
        raw = json.load(file)
    return {token_id: token.replace("Ġ", " ") for token, token_id in raw.items()}


def best_allowed(logits: list[float], allowed: np.ndarray) -> int:
    scores = np.asarray(logits)[allowed]
    return int(allowed[np.argmax(scores)])


def build_prompt(functions: list[FunctionDefinition], request: str) -> str:
    lines = ["Available functions:"]
    for func in functions:
        params = ", ".join(f"{name}: {definition.type}" for name, definition in func.parameters.items())
        lines.append(f"- {func.name}({params}): {func.description}")
    lines.append("")
    lines.append(f"Request: {request}")
    lines.append('Answer: {"name": "')
    return "\n".join(lines)


def build_params_prefix(func: FunctionDefinition) -> str:
    return f'{func.name}", "parameters": {{'


def choose_function(model: Small_LLM_Model, vocab: dict[int, str], names: list[str], prompt: str) -> str:
    candidates = {token_id: text for token_id, text in vocab.items() if text and any(text in name for name in names)}
    quote_id = int(model.encode('"')[0][0].item())
    input_ids = model.encode(prompt)[0].tolist()
    generated = ""

    for _ in range(32):
        logits = model.get_logits_from_input_ids(input_ids)
        allowed = [token_id for token_id, text in candidates.items() if any(name.startswith(generated + text) for name in names)]

        if generated in names:
            allowed.append(quote_id)

        next_id = best_allowed(logits, np.array(allowed, dtype=np.int64))
        if next_id == quote_id:
            return generated
        generated += candidates[next_id]
        input_ids.append(next_id)

    return generated

def main() -> int:
    args = parse_arguments()
    errors: list[str] = []

    try:
        function_definitions = load_json(args.functions_definition)
        prompts = load_json(args.input)
    except FileNotFoundError as error:
        print(f"Error: file not found: {error.filename}", file=sys.stderr)
        return 1
    except json.JSONDecodeError as error:
        print(f"Error: invalid JSON {error}", file=sys.stderr)
        return 1
    except OSError as error:
        print(f"Error: cannot read file: {error}", file=sys.stderr)
        return 1

    try:
        validated_prompts = TypeAdapter(list[PromptInput]).validate_python(prompts)
    except ValidationError as error:
        errors.append(f"invalid prompts file:\n{error}")

    try:
        validated_functions = TypeAdapter(list[FunctionDefinition]).validate_python(function_definitions)
    except ValidationError as error:
        errors.append(f"invalid functions file:\n{error}")

    if errors:
        for message in errors:
            print(f"Error: {message}", file=sys.stderr)
        return 1

    llm = Small_LLM_Model()

    print(f"Loaded {len(validated_functions)} functions"
          f" and {len(validated_prompts)} prompts.")

    vocab = load_vocab(llm)
    names = [func.name for func in validated_functions]

    start = time.time()
    for item in validated_prompts:
        prompt = build_prompt(validated_functions, item.prompt)
        chosen = choose_function(llm, vocab, names, prompt)
        print(f"{item.prompt!r} -> {chosen}")
    print(f"Done in {time.time() - start:.1f}s")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
