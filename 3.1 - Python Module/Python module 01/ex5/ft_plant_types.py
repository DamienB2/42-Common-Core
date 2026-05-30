class Plant:
    def __init__(
        self, name: str, height: float, age: int, rate: float = 0.5
    ) -> None:
        self._name = name
        self._height = height
        self._age = age
        self._rate = rate

    def set_height(self, height) -> None:
        if height > 0:
            self._height = height
        else:
            print(f"{self._name}: Error, height can't be negative")
            print("Height update rejected")
    
    def get_height(self):
        print(f"Height updated: {self._height}cm")

    def set_age(self, age) -> None:
        if age > 0:
            self._age = age
        else:
            print(f"{self._name}: Error, age can't be negative")
            print("Age update rejected")

    def get_age(self):
        print(f"Age updated: {self._height}cm")

    def show(self) -> str:
        return f"{self._name}: {self._height}cm, {self._age} days old"

    def grow(self) -> None:
        self._height = round(self._height + self._rate, 1)

    def age(self) -> None:
        self._age += 1


class Flower(Plant):
    def __init__(
        self, name: str, height: float, age: int, color: str , rate: float = 0.5
        ) -> None:
        super().__init__(name, height, age, rate)
        self._color = color

    def bloom(self) -> None:
        print(f"{self._name} is blooming beatifully!")



class Tree(Plant):
    def __init__(
        self, name: str, height: float, age: int, trun_diameter: int, rate: float = 0.5
        ) -> None:
        super().__init__(name, height, age, rate)
        self._trunk_diameter = trun_diameter

    def produce_shade(self) -> None:
        print(f"Tree {self._name} now produces a shade of {self._height}cm long and {self._trunk_diameter}cm wide.")


class Vegetable(Plant):
    def __init__(
        self, name: str, height: float, age: int, harvest_season: str, nutritional_value: str, rate: float = 0.5
        ) -> None:
        super().__init__(name, height, age, rate)
        self._harvest_season = harvest_season
        self._nutritional_value = nutritional_value


def main() -> None:
    print("=== Garden Plant Type ===")
    rose = Flower("Rose", 15.0, 10, 0.5, "red")
    print("Plant created: " + rose.show())
    print()

if __name__ == "__main__":
    main()
