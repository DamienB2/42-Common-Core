from functools import reduce, partial, lru_cache, singledispatch
from collections.abc import Callable
from typing import Any
import operator


def spell_reducer(spells: list[int], operation: str) -> int:
    if not spells:
        return 0

    operations: dict[str, Callable[[int, int], int]] = {
        "add": operator.add,
        "multiply": operator.mul,
        "max": lambda a, b: a if a > b else b,
        "min": lambda a, b: a if a < b else b,
    }

    if operation not in operations:
        raise ValueError(f"Unknown operation: {operation}")

    func = operations[operation]
    return reduce(func, spells)


def base_enchantment(power: int, element: str, target: str) -> str:
    return f"{element} enchantment hits {target} with power {power}"


def partial_enchanter(
    base_enchantment: Callable[[int, str, str], str],
) -> dict[str, Callable[[str], str]]:
    fire_enchant = partial(base_enchantment, 50, "fire")
    ice_enchant = partial(base_enchantment, 50, "ice")
    shadow_enchant = partial(base_enchantment, 50, "shadow")

    return {
        "fire": fire_enchant,
        "ice": ice_enchant,
        "shadow": shadow_enchant,
    }


@lru_cache
def memoized_fibonacci(n: int) -> int:
    if n < 2:
        return n
    return memoized_fibonacci(n - 1) + memoized_fibonacci(n - 2)


def spell_dispatcher() -> Callable[[Any], str]:
    @singledispatch
    def cast(spell: Any) -> str:
        return f"Unknown spell type: {type(spell).__name__}"

    @cast.register(int)
    def _(spell: int) -> str:
        return f"Damage spell deals {spell} damage!"

    @cast.register(str)
    def _(spell: str) -> str:
        return f"Enchantment '{spell}' is cast!"

    @cast.register(list)
    def _(spell: list[Any]) -> str:
        results = [cast(s) for s in spell]
        return "Multi-cast!\n" + "\n".join(f" - {r}" for r in results)

    return cast


if __name__ == "__main__":
    print("Testing spell reducer...")
    print(f"Sum: {spell_reducer([50, 50], 'add')}")
    print(f"Product: {spell_reducer([50, 50], 'multiply')}")
    print(f"Max: {spell_reducer([50, 40, 30], 'max')}")
    print()

    print("Testing partial enchanter...")
    enchants = partial_enchanter(base_enchantment)
    print(enchants["fire"]("orc"))
    print(enchants["ice"]("dragon"))
    print(enchants["shadow"]("goblin"))
    print()

    print("Testing memoized fibonacci...")
    print(f"Fib(0): {memoized_fibonacci(0)}")
    print(f"Fib(1): {memoized_fibonacci(1)}")
    print(f"Fib(10): {memoized_fibonacci(10)}")
    print(f"Fib(15): {memoized_fibonacci(15)}")
    print()

    print("Testing spell dispatcher...")
    dispatcher = spell_dispatcher()
    print(dispatcher(42))
    print(dispatcher("fireball"))
    print(dispatcher([10, "ice", 30]))
    print(dispatcher(3.14))
