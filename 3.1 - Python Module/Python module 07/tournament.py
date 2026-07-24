from ex0 import FlameFactory, AquaFactory
from ex0.factories import CreatureFactory
from ex1 import HealingCreatureFactory, TransformCreatureFactory
from ex2 import NormalStrategy, AggressiveStrategy, DefensiveStrategy
from ex2.Strategies import BattleStrategy


def single_battle(
    opponents: list[tuple[CreatureFactory, BattleStrategy]],
) -> None:
    print("*** Tournament ***")
    print(f"{len(opponents)} opponents involved\n")

    try:
        for i in range(len(opponents)):
            for j in range(i + 1, len(opponents)):
                print("* Battle *")
                opponent_factory_1, opponent_strategy_1 = opponents[i]
                opponent_factory_2, opponent_strategy_2 = opponents[j]

                opponent1 = opponent_factory_1.create_base()
                opponent2 = opponent_factory_2.create_base()
                print(f"{opponent1.describe()}\n vs.\n{opponent2.describe()}")
                print(" now fight!")
                opponent_strategy_1.act(opponent1)
                opponent_strategy_2.act(opponent2)
                print()

    except ValueError as e:
        print(f"Battle error, aborting tournament: {e}")
        print()


if __name__ == "__main__":
    flame_factory = FlameFactory()
    aqua_factory = AquaFactory()
    healing_factory = HealingCreatureFactory()
    transform_factory = TransformCreatureFactory()

    normal_strategy = NormalStrategy()
    aggressive_strategy = AggressiveStrategy()
    defensive_strategy = DefensiveStrategy()

    print("Tournament 0 (basic)")
    print(" [ (Flameling+Normal), (Healing+Defensive) ]")
    tournament = [
        (flame_factory, normal_strategy),
        (healing_factory, defensive_strategy),
    ]
    single_battle(tournament)

    print("Tournament 1 (error)")
    print(" [ (Flameling+Aggressive), (Healing+Defensive) ]")
    tournament = [
        (flame_factory, aggressive_strategy),
        (healing_factory, defensive_strategy),
    ]
    single_battle(tournament)

    print("Tournament 2 (multiple)")
    print(" [ (Aquabub+Normal), (Healing+Defensive), (Transform+Aggressive) ]")
    tournament = [
        (aqua_factory, normal_strategy),
        (healing_factory, defensive_strategy),
        (transform_factory, aggressive_strategy),
    ]
    single_battle(tournament)
