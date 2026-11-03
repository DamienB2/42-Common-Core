import argparse, json, sys
from typing import Any
from pathlib import Path
from pydantic import ValidationError, TypeAdapter
from src.models import PromptInput, FunctionDefinition


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


def validate_prompt_and_function(function_definitions: Any, prompts: Any) -> tuple[list[FunctionDefinition], list[PromptInput]] | None:
    errors: list[str] = []

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
        return None

    return validated_functions, validated_prompts
        