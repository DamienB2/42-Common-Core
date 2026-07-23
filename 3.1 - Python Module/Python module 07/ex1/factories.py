from ex0.factories import CreatureFactory
from .creatures import Creature, Sproutling, Bloomelle, Shiftling, Morphagon

class HealingCreatureFactory(CreatureFactory):

    def create_base() -> Creature:
        return Sproutling()
    
    def create_evolved() -> Creature:
        return Bloomelle()


class TransformCreatureFactory(CreatureFactory):

    def create_base() -> Creature:
        return Shiftling()
        
    def create_evolved() -> Creature:
        return Morphagon()
