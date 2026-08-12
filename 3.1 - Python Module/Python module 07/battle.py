from ex0 import FlameFactory, AquaFactory, CreatureFactory


def test_factory(factory: CreatureFactory) -> None:
    base = factory.create_base()
    evolved = factory.create_evolved()

    print(base.describe())
    print(base.attack())
    print(evolved.describe())
    print(evolved.attack())
    print()


def battle(factory1: CreatureFactory, factory2: CreatureFactory) -> None:
    base1 = factory1.create_base()
    base2 = factory2.create_base()

    print(f"{base1.describe()}\nvs.\n{base2.describe()}")
    print("fight!")
    print(base1.attack())
    print(base2.attack())
    print()


def main() -> None:
    print("Testing factory")
    flame_factory = FlameFactory()
    test_factory(flame_factory)

    print("Testing factory")
    aqua_factory = AquaFactory()
    test_factory(aqua_factory)

    print("Testing battle")
    battle(flame_factory, aqua_factory)


if __name__ == "__main__":
    main()
