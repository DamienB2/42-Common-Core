import json, sys
import numpy as np
import time
from llm_sdk import Small_LLM_Model
from src.models import FunctionDefinition
from src.parser import parse_arguments, load_json, validate_prompt_and_function


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

    # check for errors in the differents files we gonna use
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

    # call for a pydantic check to be sure the files contains all correct value types
    result = validate_prompt_and_function(function_definitions, prompts)
    if result is None:
        return 1
    validated_functions, validated_prompts = result

    llm = Small_LLM_Model()

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
