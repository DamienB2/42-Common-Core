from typing import Callable


def mage_counter() -> Callable:
    count = 0

    def counter() -> int:
        nonlocal count
        count += 1
        return count

    return counter


def spell_accumulator(initial_power: int) -> Callable:
    total_power = initial_power

    def accumulates_power(added_power: int) -> int:
        nonlocal total_power
        total_power += added_power
        return total_power

    return accumulates_power


def enchantment_factory(enchantment_type: str) -> Callable:
    enchantment = enchantment_type

    def apply_to_item(item: str) -> str:
        item = enchantment + " " + item
        return item

    return apply_to_item


def memory_vault() -> dict[str, Callable]:
    vault = {}

    def store(key: str, value: int) -> None:
        vault[key] = value

    def recall(key: str) -> str | intclear:
        if key in vault:
            return vault.pop(key)
        return "Memory not found"

    return {"store": store, "recall": recall}


if __name__ == "__main__":
    counter_a = mage_counter()
    counter_b = mage_counter()

    print("Testing mage counter...")
    print(f"counter_a call 1: {counter_a()}")
    print(f"counter_a call 2: {counter_a()}")
    print(f"counter_b call 1: {counter_b()}")
    print()

    base_power = 100
    added_power = 20
    accumulate_power = spell_accumulator(base_power)

    print("Testing spell accumulator...")
    print(
        f"Base {base_power}, add {added_power}: "
        f"{accumulate_power(added_power)}"
    )
    print(
        f"Base {base_power}, add {added_power}: "
        f"{accumulate_power(added_power)}"
    )
    print()

    item = "Sword"
    enchantment = "Flaming"
    item_enchanter = enchantment_factory(enchantment)

    print("Testing enchantment factory...")
    print(item_enchanter(item))
    print()

    vault = memory_vault()

    print("Testing memory vault...")
    vault["store"]("secret", 42)
    print("Store 'secret' = 42")
    print("Recall 'secret':", vault["recall"]("secret"))
    print("Recall 'unknown':", vault["recall"]("unknown"))
