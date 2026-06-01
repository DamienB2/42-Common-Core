class Plant:
    def __init__(
        self, name: str, height: float, age: int, rate: float = 0.5
    ) -> None:
        self._name = name
        self._height = height
        self._age = age
        self._rate = rate

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
        self._bloom = False

    def get_bloom_status(self) -> str:
        if self._bloom == False:
            return f"{self._name} has not bloomed yet" 
        else:
            return f"{self._name} is blooming beautifully!"
    
    def bloom(self) -> None:
        self._bloom = True
        print(f"[asking the {self._name} to bloom]")

    def show(self) -> str:
        return super().show() + f"\n Color: {self._color}\n {self.get_bloom_status()}"


class Tree(Plant):
    def __init__(
        self, name: str, height: float, age: int, trunk_diameter: float, rate: float = 0.5
        ) -> None:
        super().__init__(name, height, age, rate)
        self._trunk_diameter = trunk_diameter

    def produce_shade(self) -> None:
        print(f"[asking the {self._name} to produce shade]")
        print(f"Tree {self._name} now produces a shade of {self._height}cm long and {self._trunk_diameter}cm wide.")
    
    def show(self) -> str:
        return super().show() + f"\n Trunk diameter: {self._trunk_diameter}cm"    


class Vegetable(Plant):
    def __init__(
        self, name: str, height: float, age: int, harvest_season: str, nutritional_value: int, rate: float = 0.5
        ) -> None:
        super().__init__(name, height, age, rate)
        self._harvest_season = harvest_season
        self._nutritional_value = nutritional_value

    def show(self) -> str:
        return super().show() + f"\n Harvest season: {self._harvest_season}\n Nutritional value: {self._nutritional_value}"
    
    def grow(self, time: int) -> None:
        print(f"[make tomato grow and age for {time} days]")
        for i in range(time):
            super().grow()
            super().age()
            self._nutritional_value += 1


def main() -> None:
    print("=== Garden Plant Types ===")
    print("=== Flower")
    rose = Flower("Rose", 15.0, 10, "red")
    print(rose.show())
    rose.bloom()
    print(rose.show())
    print()
    print("=== Tree")
    oak = Tree("Oak", 200.0, 365, 5.0)
    print(oak.show())
    oak.produce_shade()
    print()
    print("=== Vegetable")
    tomato = Vegetable("Tomato", 5.0, 10, "April", 0, 2.1)
    print(tomato.show())
    tomato.grow(20)
    print(tomato.show())


if __name__ == "__main__":
    main()
