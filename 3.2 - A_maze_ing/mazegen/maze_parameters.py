from typing import Tuple
from pydantic import BaseModel, Field, model_validator


class MazeParams(BaseModel):
    """Pydantic model representing maze configuration parameters.

    Attributes:
        width (int): Maze width (must be >= 1).
        height (int): Maze height (must be >= 1).
        entry (Tuple[int, int]): Row and column coordinates for entry.
        exit (Tuple[int, int]): Row and column coordinates for exit.
        output_file (str): Destination file path for generated maze.
        perfect (bool): Whether the maze should be perfect.
    """
    width: int = Field(ge=1)
    height: int = Field(ge=1)
    entry: Tuple[int, int]
    exit: Tuple[int, int]
    output_file: str
    perfect: bool

    @model_validator(mode='after')
    def validate_boundaries(self) -> 'MazeParams':
        """Validate boundary constraints and uniqueness for entry/exit points.

        Returns:
            MazeParams: The validated model instance.

        Raises:
            ValueError: If entry or exit is out of bounds or identical.
        """
        if not (0 <= self.entry[0] < self.height and
                0 <= self.entry[1] < self.width):
            raise ValueError(f"Entry coordinates {self.entry} " +
                             f"out of bounds for maze size {self.width} " +
                             f"by {self.height}.")

        if not (0 <= self.exit[0] < self.height and
                0 <= self.exit[1] < self.width):
            raise ValueError(
                f"Exit coordinates {self.exit} " +
                f"out of bounds for maze size {self.width} " +
                f"by {self.height}.")
        if self.entry[0] == self.exit[0] and \
                self.entry[1] == self.exit[1]:
            raise ValueError("Entry and exit coordinates must be different.")

        return self
