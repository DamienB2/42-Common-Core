class Plant:
    def __init__(
        self, name: str, height: float, days_old: int, rate: float = 0.5
    ) -> None:
        self.name = name
        self.height = height
        self.days_old = days_old
        self.rate = rate

    def show(self) -> str:
        return f"{self.name}: {self.height}cm, {self.days_old} days old"

    def grow(self) -> None:
        self.height = round(self.height + self.rate, 1)

    def age(self) -> None:
        self.days_old += 1


def main() -> None:
    print("=== Plant Factory Output ===")
    rose = Plant("Rose", 25.0, 30)
    oak = Plant("Oak", 200.0, 365)
    cactus = Plant("Cactus", 5.0, 90)
    sunflower = Plant("Sunflower", 80.0, 45, 1.0)
    fern = Plant("Fern", 15.0, 120)
    print("Created: " + rose.show())
    print("Created: " + oak.show())
    print("Created: " + cactus.show())
    print("Created: " + sunflower.show())
    print("Created: " + fern.show())



if __name__ == "__main__":
    main()
