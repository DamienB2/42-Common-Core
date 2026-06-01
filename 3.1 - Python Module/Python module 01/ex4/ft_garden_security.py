class Plant:
    def __init__(
        self, name: str, height: float, age: int, rate: float = 0.5
    ) -> None:
        self._name = name
        self._height = height
        self._age = age
        self._rate = rate

    def set_height(self, height: int) -> None:
        if height > 0:
            self._height = height
        else:
            print(f"{self._name}: Error, height can't be negative")
            print("Height update rejected")

    def get_height(self) -> None:
        print(f"Height updated: {self._height}cm")

    def set_age(self, age: int) -> None:
        if age > 0:
            self._age = age
        else:
            print(f"{self._name}: Error, age can't be negative")
            print("Age update rejected")

    def get_age(self) -> None:
        print(f"Age updated: {self._height}cm")

    def show(self) -> str:
        return f"{self._name}: {self._height}cm, {self._age} days old"

    def grow(self) -> None:
        self._height = round(self._height + self._rate, 1)

    def age(self) -> None:
        self._age += 1


def main() -> None:
    print("=== Garden Security System ===")
    rose = Plant("Rose", 15.0, 10)
    print("Plant created: " + rose.show())
    print()
    rose.set_height(25)
    rose.get_height()
    rose.set_age(30)
    rose.get_age()
    print()
    rose.set_height(-25)
    rose.set_age(-30)
    print()
    print("Current state: " + rose.show())


if __name__ == "__main__":
    main()
