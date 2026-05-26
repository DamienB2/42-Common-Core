class Plant:
    def __init__(self, name: str, height: int, age: int) -> None:
        self.name = name
        self.height = height
        self.age = age

    def show(self) -> None:
        print(f"{self.name}: {self.height}cm, {self.age} days old")

def community_garden() -> None:
    Rose = Plant("Rose", 25, 30)
    Sunflower = Plant("Rose", 25, 30)
    Rose = Plant("Rose", 25, 30)

if __name__ == "__main__":

