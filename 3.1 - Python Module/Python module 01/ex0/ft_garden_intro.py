def ft_garden_intro() -> None:
    name = "Carrot"
    height = 12
    age = 15
    print("Plant: " + name)
    print(f"Height: {height}cm")
    print(f"Age: {age} days")


def main() -> None:
    print("=== Welcome to My Garden ===")
    ft_garden_intro()
    print()
    print("=== End of Program")


if __name__ == "__main__":
    main()
