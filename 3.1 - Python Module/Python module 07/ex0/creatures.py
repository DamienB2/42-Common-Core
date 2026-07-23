from abc import ABC, abstractmethod

class Creature(ABC):
    def __init__(self, name: str, type: str) -> None:
        self._name = name
        self._type = type

    @abstractmethod
    def attack(self) -> str:
        pass

    def describe(self) -> str:
        return f"{self._name} is a {self._type} type Creature"


class Flameling(Creature):
    def __init__(self) -> None:
        super().__init__("Flameling", "Fire")
        self._capacity = "Ember"

    def attack(self) -> str:
        return f"{self._name} uses {self._capacity}!"


class Pyrodon(Creature):
    def __init__(self) -> None:
        super().__init__("Pyrodon", "Fire/Flying")
        self._capacity = "Flamethrower"

    def attack(self) -> str:
        return f"{self._name} uses {self._capacity}!"


class Aquabub(Creature):
    def __init__(self) -> None:
        super().__init__("Aquabub", "Water")
        self._capacity = "Water Gun"

    def attack(self) -> str:
        return f"{self._name} uses {self._capacity}!"


class Torragon(Creature):
    def __init__(self) -> None:
        super().__init__("Torragon", "Water")
        self._capacity = "Hydro Pump"

    def attack(self) -> str:
        return f"{self._name} uses {self._capacity}!"