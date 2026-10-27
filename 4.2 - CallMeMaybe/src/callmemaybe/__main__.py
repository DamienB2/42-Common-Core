import argparse, json, sys
import numpy as np
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

    print(validated_functions[0].description)
    print(generate_string_value(llm, validated_prompts[0].prompt))

    #uv run --env-file .env python -m src
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
