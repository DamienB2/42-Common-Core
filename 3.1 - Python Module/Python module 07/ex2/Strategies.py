from abc import ABC, abstractmethod
from ex1.creatures import Creature
from ex1.capabilities import HealCapability, TransformCapability

class BattleStrategy(ABC):

    @abstractmethod
    def act(self, creature: Creature) -> None:
        pass

    @abstractmethod
    def is_valid(self, creature: Creature) -> bool:
        pass


class NormalStrategy(BattleStrategy):

    def act(self, creature: Creature) -> None:
        if self.is_valid(creature):
            print(creature.attack())

    def is_valid(self, creature: Creature) -> bool:
        return True


class AggressiveStrategy(BattleStrategy):

    def act(self, creature) -> None:
        if self.is_valid(creature):
            print(creature.transform())
            print(creature.attack())
            print(creature.revert())
        else:
            raise ValueError(f"Invalid Creature '{creature._name}' for this aggressive strategy")

    def is_valid(self, creature) -> bool:
        return isinstance(creature, TransformCapability)


class DefensiveStrategy(BattleStrategy):

    def act(self, creature) -> None:
        if self.is_valid(creature):
            print(creature.attack())
            print(creature.heal())
        else:
            raise ValueError(f"Invalid Creature '{creature._name}' for this defensive strategy")

    def is_valid(self, creature) -> bool:
        return isinstance(creature, HealCapability)