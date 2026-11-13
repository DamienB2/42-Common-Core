*This project has been created as part of the 42 curriculum by dbaise.*

# call me maybe

Introduction to function calling in LLMs.

## Description

Large language models understand natural language but do not naturally
produce reliable, machine-executable output. This project translates natural
language prompts into **structured function calls** using a small model
(`Qwen/Qwen3-0.6B`), with **constrained decoding** guaranteeing that the
output is always valid JSON that respects the schema of the available
functions.

Given the prompt `"What is the sum of 40 and 2?"`, the program does not answer
`42`. It returns the function to call and its typed arguments:

```json
{
  "prompt": "What is the sum of 40 and 2?",
  "name": "fn_add_numbers",
  "parameters": {"a": 40.0, "b": 2.0}
}
```

Key points:

- The **function is chosen by the LLM**, not by heuristics.
- The **JSON structure is never left to the model**: the program writes the
  skeleton, and the model only fills in values under token-level constraints.
- Argument types (`number`, `string`, `boolean`) follow the function
  definitions exactly.
- Input files are validated with **pydantic**, and every error is handled
  with a clear message (no unexpected crash).

## Instructions

### Requirements

- Python 3.10 or later
- [uv](https://docs.astral.sh/uv/)

### Installation

```bash
make install        # runs `uv sync`
```

### Execution

```bash
make run            # uses the default files in data/
```

or, with custom paths:

```bash
uv run python -m src \
    --functions_definition data/input/functions_definition.json \
    --input data/input/function_calling_tests.json \
    --output data/output/function_calls.json
```

| Argument                | Default                                  |
|-------------------------|------------------------------------------|
| `--functions_definition`| `data/input/functions_definition.json`   |
| `--input`               | `data/input/function_calling_tests.json` |
| `--output`              | `data/output/function_calls.json`        |

### Other Makefile rules

| Rule        | Description                                            |
|-------------|--------------------------------------------------------|
| `install`   | Install dependencies with `uv sync`                    |
| `run`       | Run the program                                        |
| `debug`     | Run the program under `pdb`                            |
| `clean`     | Remove `__pycache__`, `.mypy_cache`, `.pytest_cache`   |
| `lint`      | Run `flake8` and `mypy` with the mandatory flags       |

### Project structure

```
.
├── src/
│   ├── __init__.py
│   ├── __main__.py     # entry point, prompt building, pipeline
│   ├── generator.py    # constrained decoding (number, string, boolean)
│   ├── models.py       # pydantic models
│   └── parser.py       # CLI arguments, JSON loading and validation
├── llm_sdk/            # provided wrapper around the LLM
├── data/input/         # functions_definition.json, function_calling_tests.json
├── pyproject.toml
├── uv.lock
└── README.md
```

## Example usage

Input (`function_calling_tests.json`):

```json
[
  {"prompt": "What is the sum of 2 and 3?"},
  {"prompt": "Greet shrek"},
  {"prompt": "Replace all vowels in 'Programming is fun' with asterisks"}
]
```

Run:

```bash
uv run python -m src
```

Output (`data/output/function_calls.json`):

```json
[
  {
    "prompt": "What is the sum of 2 and 3?",
    "name": "fn_add_numbers",
    "parameters": {"a": 2.0, "b": 3.0}
  },
  {
    "prompt": "Greet shrek",
    "name": "fn_greet",
    "parameters": {"name": "shrek"}
  },
  {
    "prompt": "Replace all vowels in 'Programming is fun' with asterisks",
    "name": "fn_substitute_string_with_regex",
    "parameters": {
      "source_string": "Programming is fun",
      "regex": "[aeiouAEIOU]",
      "replacement": "*"
    }
  }
]
```

> TODO: replace this example with the real output of your run.

Error handling examples:

```text
Error: file not found: data/input/missing.json
Error: invalid JSON: Expecting value: line 1 column 1 (char 0)
Error: invalid functions file: ...
```

## Algorithm explanation

### Overview

At each generation step the model produces **logits**, one score per token of
its vocabulary. Normally we pick the highest-scoring token over the whole
vocabulary. With constrained decoding, we first compute the set of tokens that
keep the output valid, then take the highest-scoring token **among those only**.
This is equivalent to setting the logits of invalid tokens to `-inf`.

The pipeline for one prompt:

1. **Vocabulary mapping.** `vocab.json` (via `get_path_to_vocab_file()`) is
   loaded and inverted into `token_id -> text`. The byte-level BPE marker `Ġ`
   is replaced by a space so that token texts can be concatenated and compared
   with plain strings.
2. **Prompt building.** The prompt lists the available functions (name,
   parameters and types, description), then the user request, and ends with
   `Answer: {"name": "`. The model therefore continues directly inside the
   function name.
3. **Function name selection.** See below.
4. **Parameter extraction.** For each parameter, in the order of the function
   definition, the program appends `"param_name": ` to the context and
   generates the value with a type-specific constraint. The JSON keys,
   quotes, braces and commas are written by the program, never by the model.
5. **Validation and output.** The result is validated with pydantic
   (`FunctionCall`) and written with `json.dump`.

### Choosing the function name

- Candidate tokens are those whose text is a substring of at least one
  function name.
- At every step, a token is allowed only if `generated + token` is still a
  **prefix of some function name**.
- Once `generated` is exactly a function name, the closing quote token also
  becomes allowed. If the model picks it, the name is finished.
- The highest-logit token among the allowed ones is selected, then appended to
  the context.

As a result, the model can only output the exact name of an existing
function, and the choice among them is made by the LLM's own scores.

### Generating values

| Type      | Constraint                                                                                                  |
|-----------|-------------------------------------------------------------------------------------------------------------|
| `number`  | Allowed tokens must keep `generated` matching `^-?(\d+\.?\d*)?$`. The end token (`,` or `}`) is allowed only once the number is complete (`^-?\d+(\.\d+)?$`). The result is converted with `float()`. |
| `string`  | Any token except those containing a newline or an escaped quote (`\"`). Generation stops as soon as a token contains `"`, and only the text before the quote is kept. Leading spaces are stripped, and a limit of 64 tokens prevents endless output. |
| `boolean` | Allowed tokens must keep `generated` a prefix of `true` or `false`. Generation stops when one of them is complete. |

Each generated value is appended back to the context before the next
parameter, so the model sees what it has already produced.

### Why the JSON is always valid

The model never writes structural characters. The program builds the object
`{"prompt", "name", "parameters"}` itself, the name is guaranteed to be one of
the defined functions, each value respects its declared type, and the file is
serialised by `json.dump`. Strings containing quotes or backslashes (for
example regular expressions such as `\d+`) are escaped correctly by the
serialiser.

## Design decisions

- **Greedy decoding (argmax) over the allowed tokens.** It is deterministic,
  simple to reason about and reproducible.
- **The program owns the structure, the model owns the values.** This removes
  any dependency on the model "knowing" JSON.
- **Token-level constraints using the vocabulary file** instead of
  character-level ones: every candidate token is checked against the
  constraint before being considered.
- **Numbers are always returned as `float`**, matching the expected output
  (`2.0`, `3.0`).
- **pydantic for all data models** (`PromptInput`, `FunctionDefinition`,
  `FunctionCall`, ...), as required, so malformed input files are rejected
  with precise messages.
- **Per-prompt error isolation.** If one prompt fails (no matching function,
  invalid value), an error is printed and the program continues with the next.
- **Partial results are saved** if the program is interrupted with Ctrl+C.
- **Only public methods of `llm_sdk` are used** (`encode`,
  `get_logits_from_input_ids`, `get_path_to_vocab_file`).
- **Progress bar (bonus).** `tqdm` displays progress and an ETA, since
  generation processes the full context at each token.

## Performance analysis

> TODO: fill with your real measurements. Suggested content:

- **Accuracy:** number of prompts with the correct function and arguments out
  of the total (target: 90%+). Mention which prompts fail, if any (for
  example, regex-based substitutions are the hardest for a 0.6B model).
- **Reliability:** 100% of outputs are valid JSON and schema-compliant by
  construction.
- **Speed:** total time on the 11 provided prompts and on which machine
  (target: under 5 minutes). Each token requires a full forward pass of the
  model, because no KV cache is used.

## Challenges faced

> Adapt this section to what you actually experienced. Points that naturally
> come from this implementation:

- **Leading spaces in tokens.** BPE tokens carry a `Ġ` marker for a preceding
  space. It had to be converted to be able to concatenate and compare token
  texts.
- **Tokens that span a structural character.** A single token can contain a
  closing quote together with other characters, so the string generator
  checks for `"` inside each token and keeps only the text before it.
- **Escaped quotes.** Tokens containing `\"` are excluded from string
  generation so the model cannot produce an ambiguous end of string.
- **Partial numbers.** A number is built token by token, so the constraint
  works on prefixes (`-`, `12`, `12.`) and only allows the end token once the
  number is complete.
- **Regular expressions as arguments.** Backslashes must be escaped correctly
  in the JSON output and in the context re-fed to the model.
- **Speed.** Constraint checks over the whole vocabulary at each step were
  kept cheap by pre-filtering candidates once per value.

## Testing strategy

> TODO: describe what you really did. Suggested content:

- Run the program on the provided `function_calling_tests.json` and check each
  result manually against the expected function and arguments.
- Validate that `function_calls.json` is valid JSON and that every `name`
  exists in the definitions and every parameter has the right type.
- Test edge cases: empty strings, large and negative numbers, special
  characters, ambiguous prompts, functions with several parameters, and
  different function sets than the provided one.
- Test error handling: missing input file, invalid JSON, empty file, wrong
  schema in the definitions, unwritable output path.
- Static checks with `make lint` (`flake8` and `mypy`).

## Resources

### References

- [Qwen3 model card](https://huggingface.co/Qwen/Qwen3-0.6B)
- [Hugging Face Tokenizers documentation](https://huggingface.co/docs/tokenizers)
- [Byte-Pair Encoding (Sennrich et al., 2016)](https://arxiv.org/abs/1508.07909)
- [Efficient Guided Generation for LLMs (Willard & Louf, 2023)](https://arxiv.org/abs/2307.09702)
- [pydantic documentation](https://docs.pydantic.dev/)
- [uv documentation](https://docs.astral.sh/uv/)
- [JSON specification (RFC 8259)](https://www.rfc-editor.org/rfc/rfc8259)

### Use of AI

> TODO: describe honestly how you used AI, as required by the subject:
> which tasks (for example: understanding constrained decoding, reviewing the
> subject against the code, drafting the README or docstrings) and which parts
> of the project. Mention that you reviewed, tested and understood everything
> that was generated.