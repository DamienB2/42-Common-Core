from enum import Enum
from typing import Any


class ConfigKeys(str, Enum):
    """Enumeration of valid configuration key names."""
    WIDTH = 'WIDTH'
    HEIGHT = 'HEIGHT'
    ENTRY = 'ENTRY'
    EXIT = 'EXIT'
    OUTPUT_FILE = 'OUTPUT_FILE'
    PERFECT = 'PERFECT'


def parse_config(filename: str) -> dict[str, Any]:
    """Parse a configuration file into a dictionary.

    Validates KEY=VALUE syntax, line formatting, and type constraints for
    all supported configuration keys.

    Args:
        filename (str): The path to the configuration file.

    Returns:
        dict[str, Any]: A dictionary containing parsed configuration values.

    Raises:
        ValueError: If syntax is invalid, keys are unknown, or values
            cannot be converted to expected types.
    """
    config_data: dict[str, Any] = {}

    with open(filename, 'r') as f:
        line_count = 1
        for line in f:
            line = line.strip()

            if not line or line.startswith('#'):
                line_count += 1
                continue

            if '=' not in line:
                raise ValueError(f"KEY=VALUE syntax in config file not "
                                 f"respected on line {line_count}.")

            key, value = line.split('=', 1)
            key = key.strip()
            value = value.strip()

            try:
                config_enum = ConfigKeys(key)
            except ValueError:
                raise ValueError(f"Config parameter '{key}' is unknown in "
                                 f"config file '{filename}' "
                                 f"(line {line_count}).")

            if config_enum in (ConfigKeys.WIDTH, ConfigKeys.HEIGHT):
                try:
                    config_data[config_enum.value.lower()] = int(value)
                except ValueError:
                    raise ValueError(f"{config_enum} parameter must be integer"
                                     f" in config file '{filename}' "
                                     f"(line {line_count}).")
            elif config_enum in (ConfigKeys.ENTRY, ConfigKeys.EXIT):
                try:
                    inverted_coords = tuple(map(int, value.split(',', 1)))
                except ValueError:
                    raise ValueError(f"{config_enum} parameter must be 'row, "
                                     "column' format with row "
                                     "and column as integers.")
                coords = inverted_coords[1], inverted_coords[0]
                config_data[config_enum.value.lower()] = coords
            elif config_enum == ConfigKeys.PERFECT:
                if value == 'True':
                    config_data['perfect'] = True
                elif value == 'False':
                    config_data['perfect'] = False
                else:
                    raise ValueError("PERFECT parameter must be string 'True' "
                                     f"or 'False' in config file '{filename}' "
                                     f"(line {line_count}).")
            elif config_enum == ConfigKeys.OUTPUT_FILE:
                config_data['output_file'] = value

            line_count += 1

    return config_data
