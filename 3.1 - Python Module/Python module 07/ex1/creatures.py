from .capabilities import HealCapability, TransformCapability
from ex0.creatures import Creature


class Sproutling(Creature, HealCapability):
    def __init__(self) -> None:
        super().__init__("Sproutling", "Grass")
        self._capacity = "Vine Whip"

    def attack(self) -> str:
        return f"{self._name} uses {self._capacity}!"

    def heal(self) -> str:
        return f"{self._name} heals itself for a small amount"


class Bloomelle(Creature, HealCapability):
    def __init__(self) -> None:
        super().__init__("Bloomelle", "Grass/Fairy")
        self._capacity = "Petal Dance"

    def attack(self) -> str:
        return f"{self._name} uses {self._capacity}!"

    def heal(self) -> str:
        return f"{self._name} heals itself for a small amount"


class Shiftling(Creature, TransformCapability):
    def __init__(self) -> None:
        super().__init__("Shiftling", "Normal")
        self._capacity = "boosted strike"
        self._isTranform = 0

    def attack(self) -> str:
        if self._isTranform:
            return f"{self._name} performs a {self._capacity}!"
        return f"{self._name} attacks normally."

    def transform(self):
        self._isTranform = 1
        return f"{self._name} shifts into a sharper form!"

    def revert(self):
        self._isTranform = 0
        return f"{self._name} returns to normal."


class Morphagon(Creature, TransformCapability):
    def __init__(self) -> None:
        super().__init__("Morphagon", "Normal/Dragon")
        self._capacity = "morph strike"
        self._isTranform = 0

    def attack(self) -> str:
        if self._isTranform:
            return f"{self._name} unleashes a devastating {self._capacity}!"
        return f"{self._name} attacks normally."

    def transform(self):
        self._isTranform = 1
        return f"{self._name} morphs into a dragonic battle form!"

    def revert(self):
        self._isTranform = 0
        return f"{self._name} stabilizes its form."
