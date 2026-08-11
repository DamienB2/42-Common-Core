from functools import reduce, partial
from typing import Callable
import operator

def spell_reducer(spells: list[int], operation: str) -> int:
    if not spells:
        return 0

    operations = {
        "add": operator.add,
        "multiply": operator.mul,
        "max": lambda a, b: a if a > b else b,
        "min": lambda a, b: a if a < b else b,
    }

    if operation not in operations:
        raise ValueError(f"Unknown operation: {operation}")

    func = operations[operation]
    return reduce(func, spells)


def partial_enchanter(base_enchantment: Callable) -> dict[str, Callable]:
    fire_enchant = partial(base_enchantment, power=50, element="fire")
    ice_enchant = partial(base_enchantment, power=50, element="ice")
    shadow_enchant = partial(base_enchantment, power=50, element="shadow")

    return {
        "fire": fire_enchant,
        "ice": ice_enchant,
        "shadow": shadow_enchant,
    }


def base_enchantment(power: int, element: str, target: str) -> str:
    return f"{element} enchantment hits {target} with power {power}"



if __name__ == "__main__":


    print("Testing spell reducer...")
    print(f"Sum: {spell_reducer([50, 50], 'add')}")
    print(f"Product: {spell_reducer([50, 50], 'multiply')}")
    print(f"max: {spell_reducer([50, 40, 30], 'max')}")
    print(f"min: {spell_reducer([50, 40, 30], 'min')}")
    print()

    enchants = partial_enchanter(base_enchantment())
    print(enchants["fire"]("orc"))
    print(enchants["ice"]("dragon"))


