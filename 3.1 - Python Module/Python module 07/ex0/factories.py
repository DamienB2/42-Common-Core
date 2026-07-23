from abc import ABC, abstractmethod
from .creatures import Creature, Flameling, Pyrodon, Aquabub, Torragon

class CreatureFactory(ABC):

    @abstractmethod
    def create_base() -> Creature:
        pass


    @abstractmethod
    def create_evolved() -> Creature:
        pass


class FlameFactory(CreatureFactory):

    def create_base() -> Creature:
        return Flameling()

    def create_evolved() -> Creature:
        return Pyrodon()


class AquaFactory(CreatureFactory):

    def create_base() -> Creature:
        return Aquabub()
    
    def create_evolved() -> Creature:
        return Torragon()