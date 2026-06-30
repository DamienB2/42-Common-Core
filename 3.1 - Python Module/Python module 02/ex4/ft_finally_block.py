class GardenError(Exception):
    def __init__(self, message: str = "Unknown garden error") -> None:
        super().__init__(message)


class PlantError(GardenError):
    def __init__(self, message: str = "Unknown plant error") -> None:
        super().__init__(message)


def water_plant(plant_name: str) -> None:
    if plant_name.capitalize() != plant_name:
        raise PlantError(f"Invalid plant name to water: '{plant_name}'")
    print(f"Watering {plant_name}: [OK]")


def test_watering_system() -> None:
    print("=== Garden Watering System ===")
    print()

    try:
        print("Testing valid plants...")
        print("Opening watering system")
        water_plant("Tomato")
        water_plant("Lettuce")
        water_plant("Carrots")
        print("Closing watering system")
        print()

        print("Testing invalid plants...")
        print("Opening watering system")
        water_plant("Tomato")
        water_plant("lettuce")
        water_plant("Carrots")
        print("Closing watering system")
    except PlantError as e:
        print(f"Caught PlantError: {e}")
        print(".. ending tests and returning to main")
        print("Closing watering system")
    finally:
        print()
        print("Cleanup always happens, even with errors!")


if __name__ == "__main__":
    test_watering_system()
