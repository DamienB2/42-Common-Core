import sys
import json
import argparse
from typing import Any
from pathlib import Path
from pydantic import ValidationError, TypeAdapter
from src.models import PromptInput, FunctionDefinition


def parse_arguments() -> argparse.Namespace:
    """Parse the command-line arguments.

    Options:
        --functions_definition: Path to the function definitions file.
        --input: Path to the file containing the prompts.
        --output: Path of the JSON file where results are written.

    Returns:
        The namespace holding the three paths (with their defaults when
        an option is not given).
    """
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
    """Read and decode a JSON file.

    Args:
        path: Path of the JSON file.

    Returns:
        The decoded JSON content.

    Raises:
        FileNotFoundError: If the file does not exist.
        json.JSONDecodeError: If the content is not valid JSON.
        OSError: If the file cannot be read.
    """
    with path.open("r") as file:
        return json.load(file)


def validate_prompt_and_function(
        function_definitions: Any,
        prompts: Any
        ) -> tuple[list[FunctionDefinition], list[PromptInput]] | None:
    """Validate the content of the two input files with Pydantic.

    Both files are always checked, so that every error is reported at
    once. Errors are printed on stderr.

    Args:
        function_definitions: Decoded content of the functions file.
        prompts: Decoded content of the prompts file.

    Returns:
        A tuple (functions, prompts) of validated models, or None if at
        least one of the files is invalid.
    """
    errors: list[str] = []

    try:
        prompts_ok = TypeAdapter(
            list[PromptInput]).validate_python(prompts)
    except ValidationError as error:
        errors.append(f"invalid prompts file:\n{error}")

    try:
        functions_ok = TypeAdapter(
            list[FunctionDefinition]).validate_python(function_definitions)
    except ValidationError as error:
        errors.append(f"invalid functions file:\n{error}")

    if errors:
        for message in errors:
            print(f"Error: {message}", file=sys.stderr)
        return None

    return functions_ok, prompts_ok