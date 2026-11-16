*This project has been created as part of the 42 curriculum by dbaise.*

# call me maybe

## Description

Large Language Models understand natural language, but small ones are unreliable at producing structured, machine-executable output. **call me maybe** translates natural-language prompts into structured function calls instead of answering them directly.

Given the prompt `"What is the sum of 40 and 2?"`, the program does **not** answer `42`. It outputs:

```json
{
  "prompt": "What is the sum of 40 and 2?",
  "name": "fn_add_numbers",
  "parameters": {"a": 40.0, "b": 2.0}
}
```

The goal is to reach 100% valid, schema-compliant JSON with `Qwen/Qwen3-0.6B` by using **constrained decoding**. We intervene at the token-selection step, so the model can only pick tokens that keep the output valid with respect to the function definitions. We do not rely on prompting alone.

**Overview of the pipeline:**

1. Load and validate the function definitions and the prompts (pydantic).
2. Build a prompt listing the available functions and the user request.
3. Let the LLM **choose the function name**, token by token, restricted to the names that exist.
4. Let the LLM **fill each parameter**, token by token, restricted by the parameter's type (`number`, `string`, `boolean`).
5. Validate each result with pydantic and write everything to a single JSON file.

## Instructions

### Requirements

- Python **3.10+**
- [`uv`](https://docs.astral.sh/uv/)
- The `llm_sdk` package (provided, located at the root of the repository next to `src/`)

### Installation

```bash
make install     # or: uv sync
```

The first run downloads `Qwen/Qwen3-0.6B` from the Hugging Face Hub.

### Execution

```bash
uv run python -m src [--functions_definition <file>] [--input <file>] [--output <file>]
```

| Argument                 | Default                                  | Description                              |
|--------------------------|------------------------------------------|------------------------------------------|
| `--functions_definition` | `data/input/functions_definition.json`   | Available functions (name, params, ...)  |
| `--input`                | `data/input/function_calling_tests.json` | Natural-language prompts to process      |
| `--output`               | `data/output/function_calls.json`        | Generated function calls                 |

### Makefile rules

| Rule          | Description                                                       |
|---------------|-------------------------------------------------------------------|
| `install`     | Install the dependencies with `uv`                                |
| `run`         | Run the program with the default arguments                        |
| `debug`       | Run the program under `pdb`                                       |
| `clean`       | Remove `__pycache__`, `.mypy_cache`, and other temporary files    |
| `lint`        | `flake8 .` and `mypy .` with the flags required by the subject    |

### Project layout

```
.
├── src/
│   ├── __init__.py
│   ├── __main__.py      # entry point, prompt building, function/parameter extraction
│   ├── generator.py     # constrained generation (number, string, boolean)
│   ├── models.py        # pydantic models
│   └── parser.py        # CLI arguments, JSON loading and validation
├── llm_sdk/             # provided wrapper around the LLM
├── data/
│   └── input/           # functions_definition.json, function_calling_tests.json
├── pyproject.toml
├── uv.lock
├── Makefile
└── README.md
```

## Example usage

```bash
# Default paths
uv run python -m src

# Custom paths
uv run python -m src \
    --functions_definition data/input/functions_definition.json \
    --input data/input/function_calling_tests.json \
    --output data/output/function_calls.json
```

**Input** (`function_calling_tests.json`):

```json
[
  {"prompt": "What is the sum of 265 and 345?"},
  {"prompt": "Greet shrek"},
  {"prompt": "Replace all vowels in 'Programming is fun' with asterisks"}
]
```

**Output** (`function_calls.json`):

```json
[
  {
    "prompt": "What is the sum of 265 and 345?",
    "name": "fn_add_numbers",
    "parameters": {
      "a": 265.0,
      "b": 345.0
    }
  },
  {
    "prompt": "Greet shrek",
    "name": "fn_greet",
    "parameters": {
      "name": "shrek"
    }
  },
  {
    "prompt": "Replace all vowels in 'Programming is fun' with asterisks",
    "name": "fn_substitute_string_with_regex",
    "parameters": {
      "source_string": "Programming is fun",
      "regex": "([aeiouAEIOU])",
      "replacement": "*"
    }
  }
]
```

## Algorithm explanation

### Principle

At each step the LLM returns one logit per token of its vocabulary. Instead of taking the argmax over the whole vocabulary, we:

1. compute the set of token IDs that would keep the output valid at this exact position;
2. keep only the logits of those tokens;
3. pick the highest remaining logit;
4. append the chosen token to the input IDs and repeat.

The token ↔ text mapping comes from the model's vocabulary file (`get_path_to_vocab_file()`). It is loaded once into a `dict[int, str]`, where the byte-level BPE marker `Ġ` is replaced by a real space. Everything in the program relies on this dictionary.

### Prompt

The model is given a short prompt listing the functions and the request, ending with a partially written answer:

```
Available functions:
- fn_add_numbers(a: number, b: number): Add two numbers together and return their sum.
- fn_greet(name: string): Generate a greeting message for a person by name.
...

Request: Greet shrek
Answer: {"name": "
```

The JSON skeleton (`{"name": "`, `", "parameters": {`, `"a": `, separators, closing braces) is **written by the program, not by the model**. The model only generates the *values*. This is what guarantees the structure.

### Step 1: function name (`choose_function`)

- Candidate tokens are the ones whose text is a substring of at least one function name.
- At each step, a candidate is allowed only if `generated + token` is still a **prefix of at least one function name**.
- Once `generated` equals a complete function name, the closing quote token `"` is also allowed. If the model picks it, generation stops.
- The result is therefore always the name of an existing function. The choice among functions is made by the LLM (no keyword or heuristic matching).

### Step 2: parameters (`extract_parameters`)

Parameters are generated in the order of the function definition. For each one, the program appends `"<param_name>": ` to the running JSON text, re-encodes it, and calls the generator that matches the declared type:

| Type      | Constraint                                                                                                                                                        |
|-----------|-------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `number`  | Only tokens that keep `generated` a valid number prefix (`^-?(\d+\.?\d*)?$`). The terminator (`,` or `}`) becomes allowed only once the text is a complete number. Converted to `float` (e.g. `2.0`). |
| `integer` | Same mechanism, but with a stricter prefix rule that forbids the decimal point (`^-?\d*$`). The terminator is allowed once the text is a complete integer (`^-?\d+$`). Converted to `int` (e.g. `16`). |
| `boolean` | Only tokens that keep `generated` a prefix of `true` or `false`; stops as soon as one is complete.                                                                |
| `string`  | Any token except those containing a newline or an escaped quote; generation stops when the model emits a closing `"`.                                              |

Numbers declared as `number` are converted to `float`, so the output is always e.g. `2.0` rather than `2`. Parameters declared as `integer` are converted to `int` (e.g. `16`). Both share the same generation loop (`_generate_numeric`), which only differs by the validation rules it receives.
### Step 3: output

Each result is validated with a pydantic `FunctionCall` model and the final list is written with `json.dump`. Since the file is serialized from validated Python objects, it is always valid JSON, with exactly the keys `prompt`, `name` and `parameters`.

## Design decisions

- **The program writes the structure, the model writes the values.** Only the tokens that carry information (function name, argument values) are produced by the LLM. Braces, quotes, commas and key names never depend on it.
- **Pydantic everywhere.** Input files (`PromptInput`, `FunctionDefinition`, `TypeDefinition`) and results (`FunctionCall`) are validated with pydantic, which gives clear error messages for malformed inputs. Parameter types are restricted with `Literal["string", "number", "integer", "boolean"]`.
- **Generic by construction.** Nothing is hardcoded from the example files. Function names, parameter names, parameter types and parameter order are all read from `functions_definition.json`, so the program works with any function set.
- **Public SDK API only.** The code uses only the public methods of `Small_LLM_Model` (`encode`, `get_logits_from_input_ids`, `get_path_to_vocab_file`), as required.
- **Numpy masking.** Restricting the logits is done with a numpy index array (`np.asarray(logits)[allowed]` then `argmax`), which is equivalent to setting the other logits to `-inf` and cheaper to write.
- **Greedy decoding.** The best allowed token is always picked, which makes runs deterministic and reproducible.
- **Graceful degradation.** A prompt that fails (unknown function, validation error) is reported and skipped. Results already computed are still written, including after a `KeyboardInterrupt`.
- **Output written last, in a `finally` block**, with the parent directory created automatically.

## Performance analysis

| Metric                              | Result                                       |
|-------------------------------------|----------------------------------------------|
| Valid JSON                          | 100% (by construction)                       |
| Correct function selection          | `11`/`11` prompts on the provided tests    |
| Correct argument extraction         | `11`/`11` prompts on the provided tests    |
| Total time on the provided tests    | `~120` s on `42 Imac` (limit: 5 min)  |

**Reliability.** Structural validity does not depend on the model: the skeleton is fixed, function names are restricted to the existing ones, and numbers/booleans are constrained by grammar.

**Accuracy.** *Semantic* correctness (the right function, the right values) still depends on the 0.6B model. Constrained decoding guarantees the output is *well-formed*, not that it is *right*. Typical errors come from ambiguous prompts or from arguments that require reformulation (for example, producing a regex from a natural-language description).

**Speed.** Every generated token requires one forward pass over the whole sequence, with no KV-cache exposed by the SDK. Cost grows with the number of generated tokens and with prompt length. Computing the allowed tokens by scanning the vocabulary at each step also costs time on a vocabulary of about 150k tokens; precomputing and caching candidate sets is a natural optimization.

## Challenges faced

- **Tokens and spaces.** Byte-level BPE vocabularies represent a leading space with `Ġ`. We map it back to a real space when loading the vocabulary, otherwise prefix checks against function names or numbers fail. Newline tokens (`Ċ`) are excluded from strings.
- **Multi-character tokens.** A single token may contain several characters (a closing quote glued to other text, for instance). The string generator therefore looks *inside* the chosen token: it keeps only the text before the first `"` and stops there. Tokens with an escaped quote (`\"`) are excluded from the allowed set.
- **Knowing when a number ends.** A number can always be extended with more digits, so the terminator (`,` or `}`) is only allowed once the text so far is a complete number, and the model decides between "one more digit" and "stop".
- **Escaping.** Backslashes are escaped when the generated string is fed back into the running JSON text, so later parameters are generated from a consistent context.
- **Function selection with overlapping names.** When one name is a prefix of another, the closing quote is allowed as soon as a complete name is reached and the model decides whether to stop or continue.
- **Strict constraints vs. flexibility.** The more tightly the output is constrained, the more the prompt must help the model choose well. Prompt design (function list with typed signatures, `Answer: {"name": "` suffix) was iterated on to improve accuracy.

## Testing strategy

- Run the program on the provided `function_calling_tests.json` and manually check the generated function names and values.
- Verify that the output file is valid JSON, that every object has exactly `prompt`, `name` and `parameters`, and that parameter names and types match `functions_definition.json`.
- Edge cases: empty strings, large and negative numbers, decimals, special characters and quotes in strings, ambiguous prompts, functions with several parameters, functions with boolean parameters.
- Error handling: missing input file, invalid JSON, wrong schema (missing keys, unknown types), unwritable output path. In each case the program prints a clear message and exits without a traceback.
- Static checks: `make lint` (`flake8` and `mypy` with the flags required by the subject).
- Peer review: the logic was walked through with other students.

## Limitations

- Only flat functions with `string`, `number`, `integer` and `boolean` parameters are supported.
- Strings are limited to 64 generated tokens, function names to 32.
- Constrained decoding guarantees format, not factual or semantic correctness.

## Resources

### References

- [Qwen3 model card (Hugging Face)](https://huggingface.co/Qwen/Qwen3-0.6B)
- [Pydantic documentation](https://docs.pydantic.dev/)
- [uv documentation](https://docs.astral.sh/uv/)
- [NumPy documentation](https://numpy.org/doc/)
- Differents youtube video on how constrained decoding works

### Use of AI

AI tools were used as a learning and review aid, not to write the project for me. They helped with:

- **Project structure**: getting guidance on how to organize the repository and the modules (`src/`, parsing, generation, models).
- **Understanding how an LLM works**: explanations of tokenization, logits, vocabulary files and token-by-token generation, which are the foundations of constrained decoding.
- **Basic information about Qwen**: general facts about the `Qwen/Qwen3-0.6B` model and its tokenizer.
- **Code review**: opinions and suggestions on the structure and readability of my code, and on possible edge cases.
- **README**: help structuring and drafting this document.
