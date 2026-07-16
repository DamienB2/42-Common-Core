from .elements import create_air
from .potions import healing_potions, strengh_potions

def heal() -> str:
    return healing_potions()

__all__ = ["create_air", "strengh_potions"]


