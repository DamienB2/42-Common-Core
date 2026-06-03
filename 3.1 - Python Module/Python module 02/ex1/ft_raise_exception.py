def input_temperature(temp_str: str) -> int:
    temp_int = int(temp_str)
    if int(temp_str) > 40:
        raise ValueError(f"{temp_int}°C is too hot for plants (max 40°C)")
    elif int(temp_str) < 0:
        raise ValueError(f"{temp_int}°C is too cold for plants (min 0°C)")
    else:
        return temp_int


def test_temperature() -> None:
    valid_input = '25'
    invalid_input = 'abc'
    too_hot = '100'
    too_cold = '-50'

    print("=== Garden Temperature Checker ===")
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

    print(f"Input data is '{too_hot}'")
    try:
        print(f"Temperature is now {input_temperature(too_hot)}°C")
    except ValueError as e:
        print(f"Caught input_temperature error: {e}")
    print()

    print(f"Input data is '{too_cold}'")
    try:
        print(f"Temperature is now {input_temperature(too_cold)}°C")
    except ValueError as e:
        print(f"Caught input_temperature error: {e}")
    print()

    print("All tests completed - program didn't crash!")


if __name__ == "__main__":
    test_temperature()
