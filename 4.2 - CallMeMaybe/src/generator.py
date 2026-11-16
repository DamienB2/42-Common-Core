import re
import numpy as np
from typing import Callable
from llm_sdk import Small_LLM_Model  # type: ignore[attr-defined]


def best_allowed(logits: list[float], allowed: np.ndarray) -> int:
    """Return the allowed token with the highest logit.

    Args:
        logits: Logits of the model, one per token of the vocabulary.
        allowed: Array of the token ids that may be chosen.

    Returns:
        The id of the allowed token with the best score.
    """
    scores = np.asarray(logits)[allowed]
    return int(allowed[np.argmax(scores)])


def is_number_prefix(text: str) -> bool:
    """Check whether a text can still become a valid number.

    Accepts the empty string, a lone minus sign, and partial numbers
    such as "-", "12", "12." or "-3.5".

    Args:
        text: The text to test.

    Returns:
        True if the text is a valid (possibly incomplete) number.
    """
    return bool(re.match(r"^-?(\d+\.?\d*)?$", text))


def is_complete_number(text: str) -> bool:
    """Check whether a text is a complete number.

    A complete number is an optional minus sign followed by digits and
    an optional fractional part (for example "42", "-7" or "3.14").

    Args:
        text: The text to test.

    Returns:
        True if the text is a complete number, False otherwise.
    """
    return bool(re.match(r"^-?\d+(\.\d+)?$", text))


def is_integer_prefix(text: str) -> bool:
    """Check whether a text can still become a valid integer.

    Accepts the empty string, a lone minus sign, and digit sequences
    such as "12" or "-3". A decimal point is never allowed.

    Args:
        text: The text to test.

    Returns:
        True if the text is a valid (possibly incomplete) integer.
    """
    return bool(re.match(r"^-?\d*$", text))


def is_complete_integer(text: str) -> bool:
    """Check whether a text is a complete integer.

    A complete integer is an optional minus sign followed by at least
    one digit (for example "42" or "-7").

    Args:
        text: The text to test.

    Returns:
        True if the text is a complete integer, False otherwise.
    """
    return bool(re.match(r"^-?\d+$", text))


def _generate_numeric(model: Small_LLM_Model, vocab: dict[int, str],
                      input_ids: list[int], end_char: str,
                      is_prefix: Callable[[str], bool],
                      is_complete: Callable[[str], bool]) -> str:
    """Generate the text of a numeric value token by token.

    Only tokens that keep the generated text a valid prefix (according
    to is_prefix) are allowed. Once the text is complete (according to
    is_complete), the end character is also allowed and stops the
    generation. The generation is limited to 32 tokens.

    Args:
        model: The loaded LLM.
        vocab: Mapping of token id to token text.
        input_ids: Token ids of the current context (updated in place).
        end_char: Character that ends the value, "," or "}".
        is_prefix: Tells if a text can still become a valid value.
        is_complete: Tells if a text is a complete valid value.

    Returns:
        The generated text, not yet converted to a Python number.
    """
    end_id = int(model.encode(end_char)[0][0].item())
    generated = ""

    candidates = {token_id: text for token_id, text in vocab.items()
                  if text and is_prefix(text)}

    for _ in range(32):
        logits = model.get_logits_from_input_ids(input_ids)

        allowed = [token_id for token_id, text in candidates.items()
                   if is_prefix(generated + text)]

        if is_complete(generated):
            allowed.append(end_id)

        if not allowed:
            break

        next_id = best_allowed(logits, np.array(allowed, dtype=np.int64))

        if next_id == end_id:
            break

        generated += vocab[next_id]
        input_ids.append(next_id)

    return generated


def generate_number(model: Small_LLM_Model, vocab: dict[int, str],
                    input_ids: list[int],
                    end_char: str) -> tuple[float, list[int]]:
    """Generate a float token by token.

    Args:
        model: The loaded LLM.
        vocab: Mapping of token id to token text.
        input_ids: Token ids of the current context (updated in place).
        end_char: Character that ends the value, "," or "}".

    Returns:
        A tuple (value, input_ids) with the generated number and the
        context extended with the generated tokens.

    Raises:
        ValueError: If the generated text is not a valid number.
    """
    text = _generate_numeric(model, vocab, input_ids, end_char,
                             is_number_prefix, is_complete_number)
    return float(text), input_ids


def generate_integer(model: Small_LLM_Model, vocab: dict[int, str],
                     input_ids: list[int],
                     end_char: str) -> tuple[int, list[int]]:
    """Generate an integer token by token.

    Same as generate_number, but no decimal point is ever allowed, so
    the result is always a whole number.

    Args:
        model: The loaded LLM.
        vocab: Mapping of token id to token text.
        input_ids: Token ids of the current context (updated in place).
        end_char: Character that ends the value, "," or "}".

    Returns:
        A tuple (value, input_ids) with the generated integer and the
        context extended with the generated tokens.

    Raises:
        ValueError: If the generated text is not a valid integer.
    """
    text = _generate_numeric(model, vocab, input_ids, end_char,
                             is_integer_prefix, is_complete_integer)
    return int(text), input_ids


def generate_boolean(model: Small_LLM_Model, vocab: dict[int, str],
                     input_ids: list[int]) -> bool:
    """Generate a boolean by forcing the text "true" or "false".

    Only tokens that extend the generated text into a prefix of "true"
    or "false" are allowed. The generation stops as soon as one of the
    two words is complete.

    Args:
        model: The loaded LLM.
        vocab: Mapping of token id to token text.
        input_ids: Token ids of the current context (updated in place).

    Returns:
        True if the model generated "true", False otherwise.
    """
    generated = ""
    valid_targets = ["true", "false"]

    for _ in range(10):
        logits = model.get_logits_from_input_ids(input_ids)
        allowed = [token_id for token_id, text in vocab.items()
                   if text and any(target.startswith(generated + text)
                                   for target in valid_targets)]

        if not allowed:
            break

        next_id = best_allowed(logits, np.array(allowed, dtype=np.int64))
        generated += vocab[next_id]
        input_ids.append(next_id)

        if generated in valid_targets:
            break

    return generated == "true"


def build_string_ids(vocab: dict[int, str]) -> np.ndarray:
    """Select the token ids that may appear inside a JSON string value.

    Empty tokens, tokens containing a line break, and tokens containing
    a quote preceded by a backslash (an escaped quote) are excluded.
    Tokens with a plain quote are kept: they are used to detect the end
    of the string.

    Args:
        vocab: Mapping of token id to token text.

    Returns:
        A numpy array of the allowed token ids.
    """
    ids = []
    for token_id, text in vocab.items():
        if not text or "\n" in text or "Ċ" in text:
            continue

        if '"' in text and text.split('"')[0].endswith("\\"):
            continue
        ids.append(token_id)
    return np.array(ids, dtype=np.int64)


def generate_string(model: Small_LLM_Model, vocab: dict[int, str],
                    input_ids: list[int], allowed: np.ndarray,
                    max_tokens: int = 64,) -> str:
    """Generate a string value token by token.

    At each step the best token among the allowed ones is chosen. Leading
    spaces are dropped at the start of the string. When the chosen token
    contains a quote, the text before the quote is kept and the
    generation stops.

    Args:
        model: The loaded LLM.
        vocab: Mapping of token id to token text.
        input_ids: Token ids of the current context (updated in place).
        allowed: Token ids allowed inside a string (see
            build_string_ids).
        max_tokens: Maximum number of tokens to generate.

    Returns:
        The generated string, without the closing quote.
    """
    generated = ""
    for _ in range(max_tokens):
        logits = np.asarray(model.get_logits_from_input_ids(input_ids))
        next_id = int(allowed[np.argmax(logits[allowed])])
        text = vocab[next_id]
        if not generated:
            text = text.lstrip(" ")
            if not text:
                input_ids.append(next_id)
                continue
        if '"' in text:
            before = text.split('"')[0]
            if before.strip():
                generated += before
            break
        generated += text
        input_ids.append(next_id)
    return generated