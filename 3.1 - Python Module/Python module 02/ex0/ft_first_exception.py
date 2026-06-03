def input_temperature(temp_str: str) -> int:
    return int(temp_str)


def test_temperature() -> None:
    valid_input = '25'
    invalid_input = 'abc'

    print("=== Garden Temperature ===")
    print()

    print(f"Input data is '{valid_input}'")
    try:
        print(f"Temperature is now {input_temperature(valid_input)}°C")
    except ValueError as e:
        print(f"Caught input_temperature error: {e}")
    print()

    print(f"Input data is '{invalid_input}'")
    try:
        print(f"Temperature is now {input_temperature(invalid_input)}°C")
    except ValueError as e:
        print(f"Caught input_temperature error: {e}")
    print()

    print("All tests completed - program didn't crash!")


if __name__ == "__main__":
    test_temperature()
