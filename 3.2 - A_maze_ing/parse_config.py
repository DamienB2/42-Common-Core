from enum import Enum
from typing import Tuple
from pydantic import BaseModel, Field, model_validator


class ConfigKeys(str, Enum):
    WIDTH = 'WIDTH'
    HEIGHT = 'HEIGHT'
    ENTRY = 'ENTRY'
    EXIT = 'EXIT'
    OUTPUT_FILE = 'OUTPUT_FILE'
    PERFECT = 'PERFECT'


class MazeParams(BaseModel):
    width: int = Field(ge=1)
    height: int = Field(ge=1)
    entry: Tuple[int, int]
    exit: Tuple[int, int]
    output_file: str
    perfect: bool

    @model_validator(mode='after')
    def validate_boundaries(self) -> 'MazeParams':
        if not (0 <= self.entry[0] < self.width and
                0 <= self.entry[1] < self.height):
            raise ValueError(f"Entry coordinates {self.entry} " +
                             f"out of bounds for maze size {self.width} " +
                             f"by {self.height}.")

        if not (0 <= self.exit[0] < self.width and
                0 <= self.exit[1] < self.height):
            raise ValueError(
                f"Exit coordinates {self.exit} " +
                f"out of bounds for maze size {self.width} " +
                f"by {self.height}.")

        return self


def parse_config(filename: str) -> MazeParams:
    config_data = {}

    with open(filename, 'r') as f:
        line_count = 1
        for line in f:
            line = line.strip()

            if not line or line.startswith('#'):
                line_count += 1
                continue

            if '=' not in line:
                raise ValueError(
                    f"KEY=VALUE syntax in config file not respected on line {line_count}."
                )

            key, value = line.split('=', 1)
            key = key.strip()
            value = value.strip()

            try:
                config_enum = ConfigKeys(key)
            except ValueError:
                raise ValueError(
                    f"Config parameter '{key}' is unknown in config file '{filename}' (line {line_count})."
                )

            if config_enum in (ConfigKeys.WIDTH, ConfigKeys.HEIGHT):
                config_data[config_enum.value.lower()] = int(value)
            elif config_enum in (ConfigKeys.ENTRY, ConfigKeys.EXIT):
                coords = tuple(map(int, value.split(',')))
                config_data[config_enum.value.lower()] = coords
            elif config_enum == ConfigKeys.PERFECT:
                if value == 'True':
                    config_data['perfect'] = True
                elif value == 'False':
                    config_data['perfect'] = False
                else:
                    raise ValueError("PERFECT parameter must be string 'True' or 'False'",
                                     f"in config file '{filename}' (line {line_count}).")
            elif config_enum == ConfigKeys.OUTPUT_FILE:
                config_data['output_file'] = value

            line_count += 1

    return MazeParams(**config_data)
