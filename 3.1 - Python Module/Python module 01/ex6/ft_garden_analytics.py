class Plant:
    class _Statistics:
        def __init__(self) -> None:
            self._grow_calls = 0
            self._age_calls = 0
            self._show_calls = 0

        def inc_grow(self) -> None:
            self._grow_calls += 1

        def inc_age(self) -> None:
            self._age_calls += 1

        def inc_show(self) -> None:
            self._show_calls += 1

        def display(self) -> str:
            return (
                f"Stats: {self._grow_calls} grow,"
                f" {self._age_calls} age,"
                f" {self._show_calls} show"
            )

    def __init__(
        self, name: str, height: float, age: int, rate: float = 0.5
    ) -> None:
        self._stats = Plant._Statistics()
        self._name = name
        self._height = height
        self._age = age
        self._rate = rate

    @classmethod
    def anonymous(cls) -> "Plant":
        return cls("Unknown plant", 0.0, 0)

    def set_height(self, height: float) -> None:
        if height > 0:
            self._height = height
        else:
            print(f"{self._name}: Error, height can't be negative")
            print("Height update rejected")

    def get_height(self) -> None:
        print(f"Height updated: {int(self._height)}cm")

    def set_age(self, age: int) -> None:
        if age > 0:
            self._age = age
        else:
            print(f"{self._name}: Error, age can't be negative")
            print("Age update rejected")

    def get_age(self) -> None:
        print(f"Age updated: {self._age} days")

    def get_name(self) -> str:
        return self._name

    def get_statistics(self) -> str:
        return self._stats.display()

    def show(self) -> str:
        self._stats.inc_show()
        return f"{self._name}: {self._height}cm, {self._age} days old"

    def grow(self) -> None:
        self._height = round(self._height + self._rate, 1)
        self._stats.inc_grow()

    def age(self) -> None:
        self._age += 1
        self._stats.inc_age()

    @staticmethod
    def is_older_than_year(age: int) -> bool:
        return age > 365


class Flower(Plant):
    def __init__(
        self, name: str, height: float, age: int, color: str, rate: float = 0.5
    ) -> None:
        super().__init__(name, height, age, rate)
        self._color = color
        self._bloom = False

    def get_bloom_status(self) -> str:
        if self._bloom is False:
            return f"{self._name} has not bloomed yet"
        return f"{self._name} is blooming beautifully!"

    def bloom(self) -> None:
        self._bloom = True
        print(f"[asking the {self._name.lower()} to bloom]")

    def grow(self) -> None:
        print(f"[asking the {self._name.lower()} to grow and bloom]")
        super().grow()
        self._bloom = True

    def show(self) -> str:
        return (
            super().show()
            + f"\nColor: {self._color}\n{self.get_bloom_status()}"
        )


class Seed(Flower):
    def __init__(
        self, name: str, height: float, age: int, color: str,
        seed_rate: int = 2, rate: float = 0.5
    ) -> None:
        super().__init__(name, height, age, color, rate)
        self._seed_number = 0
        self._seed_rate = seed_rate

    def show(self) -> str:
        return super().show() + f"\nSeeds: {self._seed_number}"

    def grow(self, time: int = 1) -> None:
        print(f"[make {self._name.lower()} grow, age and bloom]")

        self._height = round(self._height + time * self._rate, 1)
        self._age += time
        self._seed_number += time * self._seed_rate
        self._bloom = True
        self._stats.inc_grow()
        self._stats.inc_age()


class Tree(Plant):
    class _TreeStatistics(Plant._Statistics):
        def __init__(self) -> None:
            super().__init__()
            self._produce_shade_calls = 0

        def inc_produce_shade(self) -> None:
            self._produce_shade_calls += 1

        def display(self) -> str:
            return (
                super().display()
                + f"\n{self._produce_shade_calls} shade"
            )

    def __init__(
        self, name: str, height: float, age: int,
        trunk_diameter: float, rate: float = 0.5
    ) -> None:
        super().__init__(name, height, age, rate)
        self._trunk_diameter = trunk_diameter
        self._stats: Tree._TreeStatistics = Tree._TreeStatistics()

    def produce_shade(self) -> None:
        self._stats.inc_produce_shade()
        print(f"[asking the {self._name.lower()} to produce shade]")
        print(
            f"Tree {self._name} now produces a shade of {self._height}cm long "
            f"and {self._trunk_diameter}cm wide."
        )

    def show(self) -> str:
        return super().show() + f"\nTrunk diameter: {self._trunk_diameter}cm"


def display_statistics(plant: Plant) -> None:
    print(f"[statistics for {plant.get_name()}]")
    print(plant.get_statistics())


def main() -> None:
    print("=== Garden statistics ===")
    print("=== Check year-old")
    print(f"Is 30 days more than a year? -> {Plant.is_older_than_year(30)}")
    print(f"Is 400 days more than a year? -> {Plant.is_older_than_year(400)}")
    print()

    print("=== Flower")
    rose = Flower("Rose", 15.0, 10, "red", 8.0)
    print(rose.show())
    display_statistics(rose)
    rose.grow()
    print(rose.show())
    display_statistics(rose)
    print()

    print("=== Tree")
    oak = Tree("Oak", 200.0, 365, 5.0)
    print(oak.show())
    display_statistics(oak)
    oak.produce_shade()
    display_statistics(oak)
    print()

    print("=== Seed")
    sunflower = Seed("Sunflower", 80.0, 45, "yellow", 2, 1.5)
    print(sunflower.show())
    sunflower.grow(20)
    print(sunflower.show())
    display_statistics(sunflower)
    print()

    print("=== Anonymous")
    unknown = Plant.anonymous()
    print(unknown.show())
    display_statistics(unknown)


if __name__ == "__main__":
    main()
