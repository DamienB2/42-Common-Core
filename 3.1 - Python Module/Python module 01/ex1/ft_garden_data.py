class Plant:
    def __init__(self, name: str, height: int, days_old: int) -> None:
        self.name = name
        self.height = height
        self.days_old = days_old

    def show(self) -> str:
        return f"{self.name}: {self.height}cm, {self.days_old} days old"


def main() -> None:
    print("=== Garden Plant Registry ===")
    Rose = Plant("Rose", 25, 30)
    Sunflower = Plant("Sunflower", 80, 45)
    Cactus = Plant("Cactus", 15, 120)
    print(Rose.show())
    print(Sunflower.show())
    print(Cactus.show())


if __name__ == "__main__":
    main()
