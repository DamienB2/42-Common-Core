class Plant:
    def __init__(self, name: str, height: float, days_old: int, rate: float) -> None:
        self.name = name
        self.height = height
        self.days_old = days_old
        self.rate = rate

    def show(self) -> None:
        print(f"{self.name}: {self.height}cm, {self.days_old} days old")

    def grow(self) -> None:
        self.height = round(self.height + self.rate, 1)
    
    def age(self) -> None:
        self.days_old += 1

def main() -> None:
    print("=== Garden Plant Growth ===")
    rose = Plant("Rose", 25.0, 30, 0.8)
    rose.show()
    start_height = rose.height
    for i in range(7):
        print(f"=== Day {i + 1} ===")
        rose.grow()
        rose.age()
        rose.show()
    print(f"Growth this week: {round(rose.height - start_height, 1)}cm")

if __name__ == "__main__":
    main()
