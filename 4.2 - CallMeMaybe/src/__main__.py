# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    __main__.py                                        :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dbaise <dbaise@student.42belgium.be>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/11/16 16:59:56 by dbaise            #+#    #+#              #
#    Updated: 2026/11/19 11:31:20 by dbaise           ###   #######belgium.be  #
#                                                                              #
# **************************************************************************** #
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
from src.generator import best_allowed, build_string_ids, generate_integer
from src.generator import generate_number, generate_string, generate_boolean


def load_vocab(model: Small_LLM_Model) -> dict[int, str]:
    """Load the tokenizer vocabulary of the model.

    Reads the vocabulary JSON file (token text -> token id) and inverts
    it. The byte-level BPE space marker is replaced by a real space so
    that token texts can be concatenated directly.

    Args:
        model: The loaded LLM, used to locate the vocabulary file.

    Returns:
        A dictionary mapping each token id to its text.
    """
    with open(model.get_path_to_vocab_file()) as file:
        raw = json.load(file)
    return {token_id: token.replace("Ġ", " ")
            for token, token_id in raw.items()}


def build_prompt(functions: list[FunctionDefinition], request: str) -> str:
    """Build the text prompt sent to the LLM.

    The prompt lists every available function with its typed signature
    and description, then gives the user request. It ends with the
    beginning of a JSON answer so the model only has to complete the
    function name.

    Args:
        functions: The validated function definitions.
        request: The natural-language request of the user.

    Returns:
        The full prompt as a single string.
    """
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
    """Write the generated function calls to a JSON file.

    Missing parent directories are created automatically.

    Args:
        path: Destination file path.
        calls: The function calls to serialize.

    Returns:
        True if the file was written, False if an OS error occurred
        (the error is printed on stderr).
    """
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
    """Let the LLM choose a function name with constrained decoding.

    At each step, only the tokens that extend the text generated so far
    into a prefix of one of the known function names are allowed. Once
    the text is exactly a function name, the closing quote becomes
    allowed and ends the generation.

    Args:
        model: The loaded LLM.
        vocab: Mapping of token id to token text.
        names: The names of the available functions.
        prompt: The prompt built by build_prompt.

    Returns:
        The chosen function name (or the text generated so far if the
        limit of 32 tokens is reached).
    """
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
                       ) -> dict[str, str | float | bool | int]:
    """Generate the values of all parameters of a function.

    The JSON is built progressively: for each parameter, the key is
    appended to the running text, then the value is generated by the
    generator matching its declared type (number, string or boolean).
    The generated value is appended back to the text so that the next
    parameters are conditioned on the previous ones.

    Args:
        model: The loaded LLM.
        vocab: Mapping of token id to token text.
        func_def: Definition of the chosen function.
        prompt: The prompt built by build_prompt.
        func_name: The name of the chosen function.
        string_ids: Token ids allowed inside a string value.

    Returns:
        A dictionary mapping each parameter name to its generated value.
    """

    parameters: dict[str, str | float | bool | int] = {}
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

        elif param_def.type == "integer":
            current_json += f'"{param_name}": '
            input_ids = model.encode(current_json)[0].tolist()
            int_val, _ = generate_integer(model, vocab, input_ids, end_char)
            parameters[param_name] = int_val
            current_json += f"{int_val}{end_char} "

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
    """Run the whole pipeline.

    Steps: parse the command line, load and validate the function
    definitions and the prompts, load the model, then for each prompt
    choose a function and extract its parameters. Prompts that fail are
    reported and skipped. On Ctrl+C, the results obtained so far are
    still written to the output file.

    Returns:
        0 on success, 1 if an input, model or output error occurred.
    """
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

    if not validated_functions:
        print("Error: the functions definition file contains no function.",
              file=sys.stderr)
        return 1

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
        for item in tqdm(validated_prompts, desc="Generation process",
                         unit="prompt"):
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
