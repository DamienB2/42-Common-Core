from functools import wraps
import time
from typing import Callable


def spell_timer(func: Callable) -> Callable:
    @wraps(func)
    def wrapper(*args, **kwargs):
        print(f"Casting {func.__name__}...")

        start_time = time.time()
        result = func(*args, **kwargs)
        end_time = time.time()

        elapsed_time = end_time - start_time
        print(f"Spell completed in {elapsed_time:.3f} seconds")

        return result

    return wrapper


def power_validator(min_power: int) -> Callable:
    def decorator(func: Callable) -> Callable:
        @wraps(func)
        def wrapper(*args, **kwargs):
            power = kwargs.get("power")

            if power is None and args:
                power = args[-1]

            if power is None or power < min_power:
                return "Insufficient power for this spell"

            return func(*args, **kwargs)

        return wrapper

    return decorator


def retry_spell(max_attempts: int) -> Callable:
    def decorator(func: Callable) -> Callable:
        @wraps(func)
        def wrapper(*args, **kwargs):
            attempts = 0
            while attempts < max_attempts:
                try:
                    return func(*args, **kwargs)
                except ValueError:
                    print(
                        "Spell failed, retrying... "
                        f"({attempts}/{max_attempts})"
                    )
                    attempts += 1

            return "Spell casting failed after max_attempts attempts"

        return wrapper

    return decorator


class MageGuild:
    @staticmethod
    def validate_mage_name(name: str) -> bool:
        if len(name) >= 3:
            for char in name:
                if not char.isalpha() and not char.isspace():
                    return False
            return True
        return False

    @power_validator(10)
    def cast_spell(self, spell_name: str, power: int) -> str:
        return f"Successfully cast {spell_name} with {power} power"


# @retry_spell(3)
# @spell_timer
# @power_validator(min_power=50)
def fireball(power: int) -> str:
    if power >= 0:
        return f"Fireball launched with {power} power"
    else:
        raise Exception


if __name__ == "__main__":
    # print(fireball(10))
    print()

    guild = MageGuild()
    spell = "fire bagitll"
    if MageGuild.validate_mage_name(spell):
        print(guild.cast_spell(spell, 80))
