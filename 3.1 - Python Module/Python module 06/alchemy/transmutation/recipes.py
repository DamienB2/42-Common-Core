from .. import create_air, strength_potions
from elements import create_fire


def lead_to_gold() -> str:
    return (f"Recipe transmuting Lead to Gold: brew '{create_air()}'"
            f" and '{strength_potions()}' mixed with '{create_fire()}'")
