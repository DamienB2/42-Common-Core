class Plant:
    def __init__(self, name: str, height: int, age: int) -> None:
        self.name = name
        self.height = height
        self.age = age

    def show(self) -> None:
        print(f"{self.name}: {self.height}cm, {self.age} days old")

def main() -> None:
    print("=== Garden Plant Registry ===")
    Rose = Plant("Rose", 25, 30)
    Sunflower = Plant("Sunflower", 80, 45)
    Cactus = Plant("Cactus", 15, 120)
    Rose.show()
    Sunflower.show()
    Cactus.show()

if __name__ == "__main__":
    main()
