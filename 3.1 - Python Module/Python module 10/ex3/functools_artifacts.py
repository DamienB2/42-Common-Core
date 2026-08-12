from functools import reduce, partial, lru_cache
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
    fire_enchant = partial(base_enchantment, 50, "fire")
    ice_enchant = partial(base_enchantment, 50, "ice")
    shadow_enchant = partial(base_enchantment, 50, "shadow")

    return {
        "fire": fire_enchant,
        "ice": ice_enchant,
        "shadow": shadow_enchant,
    }


def base_enchantment(power: int, element: str, target: str) -> str:
    return f"{element} enchantment hits {target} with power {power}"


@lru_cache
def memoized_fibonacci(n: int) -> int:
    if n < 2:
        return n
    return memoized_fibonacci(n - 1) + memoized_fibonacci(n - 2)






if __name__ == "__main__":
 

    print("Testing spell reducer...")
    print(f"Sum: {spell_reducer([50, 50], 'add')}")
    print(f"Product: {spell_reducer([50, 50], 'multiply')}")
    print(f"max: {spell_reducer([50, 40, 30], 'max')}")
    print(f"min: {spell_reducer([50, 40, 30], 'min')}")
    print()

    print("Testing partial enchanter...")
    enchants = partial_enchanter(base_enchantment)
    print(enchants["fire"]("orc"))
    print(enchants["ice"]("dragon"))
    print(enchants["shadow"]("goblin"))
    print()


    print("Testing memoized fibonacci...")
    print(f"fib(0) = {memoized_fibonacci(0)}")
    print(f"fib(1) = {memoized_fibonacci(1)}")
    print(f"fib(10) = {memoized_fibonacci(10)}")
    print(f"fib(15) = {memoized_fibonacci(15)}")
