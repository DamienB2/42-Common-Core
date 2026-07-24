from .elements import create_air, create_earth
from elements import create_fire, create_water


def healing_potions() -> str:
    return (
        f"Healing potion brewed with '{create_earth()}' and '{create_air()}'"
    )


def strengh_potions() -> str:
    return (
        f"Strengh potion brewed with '{create_fire()}' and '{create_water()}'"
    )
