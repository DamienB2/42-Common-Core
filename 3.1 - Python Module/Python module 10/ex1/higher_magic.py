from typing import Callable


def heal(target: str, power: int) -> str:
    return f"Heal restores {target} for {power} HP"


def fireball(target: str, power: int) -> str:
    return f"Fireball burns {target} and deals {power} HP"


def frostbite(target: str, power: int) -> str:
    return f"Frostbite hits {target} and deals {power} HP"


def strong_enough(target: str, power: int) -> bool:
    return power >= 50


def spell_combiner(spell1: Callable, spell2: Callable) -> Callable:
    def combined(target: str, power: int) -> tuple[str, str]:
        res1 = spell1(target, power)
        res2 = spell2(target, power)
        return (res1, res2)

    return combined


def power_amplifier(base_spell: Callable, multiplier: int) -> Callable:
    def amplified_spell(target: str, power: int) -> Callable:
        return base_spell(target, power * multiplier)

    return amplified_spell


def conditional_caster(condition: Callable, spell: Callable) -> Callable:
    def conditional_spell(target: str, power: int) -> str:
        if condition(target, power):
            return spell(target, power)
        return "Spell fizzled"

    return conditional_spell


def spell_sequence(spells: list[Callable]) -> Callable:
    def sequence(target: str, power: int) -> list[str]:
        return [spell(target, power) for spell in spells]

    return sequence


if __name__ == "__main__":
    combined = spell_combiner(fireball, heal)
    print(combined("Dragon", 50))
    print()

    mega_fireball = power_amplifier(fireball, 3)
    print(mega_fireball("Dragon", 50))
    print()

    conditional_fireball = conditional_caster(strong_enough, fireball)
    print(conditional_fireball("Dragon", 45))
    print()

    sequence = spell_sequence([fireball, heal, frostbite])
    print(sequence("Dragon", 50))
