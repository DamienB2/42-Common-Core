class GardenError(Exception):
    def __init__(self, message: str = "Unknown garden error") -> None:
        super().__init__(message)


class PlantError(GardenError):
    def __init__(self, message: str = "Unknown plant error") -> None:
        super().__init__(message)


def water_plant(plant_name: str) -> None:
    if not plant_name.isupper():
        raise PlantError(f"Invalid plant name to water: '{plant_name}'")
    print(f"Watering {plant_name.capitalize()}: [OK]")


def test_watering_system() -> None:
    print("=== Garden Watering System ===")
    print()

    try:
        print("Testing valid plants...")
        print("Opening watering system")
        water_plant("TOMATO")
        water_plant("LETTUCE")
        water_plant("CARROTS")
        print("Closing watering system")
        print()

        print("Testing invalid plants...")
        print("Opening watering system")
        water_plant("TOMATO")
        water_plant("lettuce")
        water_plant("CARROTS")
        print("Closing watering system")
    except PlantError as e:
        print(f"Caught PlantError: {e}")
        print(".. ending tests and returning to main")
        print("Closing watering system")
        return
    finally:
        print()
        print("Cleanup always happens, even with errors!")


if __name__ == "__main__":
    test_watering_system()
