class GardenError(Exception):
    def __init__(self, message: str = "Unknown garden error") -> None:
        super().__init__(message)


class PlantError(GardenError):
    def __init__(self, message: str = "Unknown plant error") -> None:
        super().__init__(message)


class WaterError(GardenError):
    def __init__(self, message: str = "Unknown water error") -> None:
        super().__init__(message)


def check_plant() -> None:
    raise PlantError("The tomato plant is wilting!")


def check_water() -> None:
    raise WaterError("Not enough water in the tank!")


def raise_custom_error() -> None:
    print("=== Custom Garden Errors Demo ===")
    print()

    print("Testing PlantError...")
    try:
        check_plant()
    except PlantError as e:
        print(f"Caught PlantError: {e}")
    print()

    print("Testing WaterError...")
    try:
        check_water()
    except WaterError as e:
        print(f"Caught WaterError: {e}")
    print()

    print("Testing catching all garden errors...")
    try:
        check_plant()
    except GardenError as e:
        print(f"Caught GardenError: {e}")
    try:
        check_water()
    except GardenError as e:
        print(f"Caught GardenError: {e}")
    print()

    print("All custom error types work correctly!")


if __name__ == "__main__":
    raise_custom_error()
