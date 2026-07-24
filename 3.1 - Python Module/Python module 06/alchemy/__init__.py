from .elements import create_air
from .potions import healing_potions, strengh_potions
from .transmutation import lead_to_gold


def heal() -> str:
    return healing_potions()


__all__ = ["create_air", "strengh_potions", "lead_to_gold"]
