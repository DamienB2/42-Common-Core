import numpy as np
import re
from llm_sdk import Small_LLM_Model


def best_allowed(logits: list[float], allowed: np.ndarray) -> int:
    scores = np.asarray(logits)[allowed]
    return int(allowed[np.argmax(scores)])


def is_number_prefix(text: str) -> bool:
    return bool(re.match(r"^-?(\d+\.?\d*)?$", text))


def is_complete_number(text: str) -> bool:
    return bool(re.match(r"^-?\d+(\.\d+)?$", text))


def generate_number(model: Small_LLM_Model, vocab: dict[int, str], input_ids: list[int], end_char: str) -> tuple[float, list[int]]:
    end_id = int(model.encode(end_char)[0][0].item())
    generated = ""

    number_candidate = {token_id: text for token_id, text in vocab.items() if text and is_number_prefix(text)}

    for _ in range(32):
        logits = model.get_logits_from_input_ids(input_ids)

        allowed = [token_id for token_id, text in number_candidate.items() if is_number_prefix(generated + text)]

        if is_complete_number(generated):
            allowed.append(end_id)

        if not allowed:
            break

        next_id = best_allowed(logits, np.array(allowed, dtype=np.int64))

        if next_id == end_id:
            break

        generated += vocab[next_id]
        input_ids.append(next_id)

    return float(generated), input_ids


def generate_boolean(model: Small_LLM_Model, vocab: dict[int, str], input_ids: list[int]) -> bool:
    generated = ""
    valid_targets = ["true", "false"]

    for _ in range(10):
        logits = model.get_logits_from_input_ids(input_ids)
        allowed = [token_id for token_id, text in vocab.items() if text and any(target.startswith(generated + text) for target in valid_targets)]

        if not allowed:
            break

        next_id = best_allowed(logits, np.array(allowed, dtype=np.int64))
        generated += vocab[next_id]
        input_ids.append(next_id)

        if generated in valid_targets:
            break

    return generated == "true"


def build_string_ids(vocab: dict[int, str]) -> np.ndarray:
    """Return ids of tokens usable inside a JSON string value."""
    ids = []
    for token_id, text in vocab.items():
        if not text or "\n" in text or "Ċ" in text:
            continue
        
        if '"' in text and text.split('"')[0].endswith("\\"):
            continue
        ids.append(token_id)
    return np.array(ids, dtype=np.int64)


def generate_string(model: Small_LLM_Model, vocab: dict[int, str], input_ids: list[int], allowed: np.ndarray, max_tokens: int = 64,
) -> str:
    generated = ""
    for _ in range(max_tokens):
        logits = np.asarray(model.get_logits_from_input_ids(input_ids))
        next_id = int(allowed[np.argmax(logits[allowed])])
        text = vocab[next_id]
        if '"' in text:
            generated += text.split('"')[0]
            break
        generated += text
        input_ids.append(next_id)
    return generated
