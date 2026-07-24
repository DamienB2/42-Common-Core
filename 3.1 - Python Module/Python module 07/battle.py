from ex0 import FlameFactory, AquaFactory

if __name__ == "__main__":
    print("Testing factory")
    Flameling = FlameFactory().create_base()
    print(Flameling.describe())
    print(Flameling.attack())

    Pyrodon = FlameFactory().create_evolved()
    print(Pyrodon.describe())
    print(Pyrodon.attack())

    print()

    print("Testing factory")
    Aquabub = AquaFactory().create_base()
    print(Aquabub.describe())
    print(Aquabub.attack())

    Torragon = AquaFactory().create_evolved()
    print(Torragon.describe())
    print(Torragon.attack())

    print()

    print("Testing battle")
    print(f"{Flameling.describe()}\n vs.\n{Aquabub.describe()}")
    print(" fight!")
    print(Flameling.attack())
    print(Aquabub.attack())
