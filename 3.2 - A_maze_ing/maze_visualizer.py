import numpy as np
from numpy.typing import NDArray
from parse_config import MazeParams
from textual.app import App, ComposeResult
from textual.color import Color
from textual_canvas import Canvas
from random import seed, choice


class Amaze(App):
    def __init__(self, height: int, width: int, matrix: NDArray[np.int_]) -> None:
        super().__init__()
        self.y = height
        self.x = width
        self.matrix = matrix

    CSS = """
    Screen {
        align: center middle;
    }

    Canvas {
        width: auto;
        height: auto;
        background: black;
    }
    """

    def compose(self) -> ComposeResult:
        yield Canvas(self.x, self.y, Color.parse("grey"))

    def on_mount(self) -> None:
        canvas = self.query_one(Canvas)
        wall_color = Color.parse("lightgrey")
        border_color = Color.parse("orange")

        for row_index, row in enumerate(self.matrix):
            for col_index, cell in enumerate(row):
                if cell == b'W':
                    canvas.set_pixel(col_index, row_index, wall_color)
                if cell == b'#':
                    canvas.set_pixel(col_index, row_index, border_color)


class MazeGenerator:
    def __init__(self, seed_value: int, params: MazeParams) -> None:
        self._seed = seed_value
        self._params = params
        self.cells_visited = 0
        self.y = params.height * 2 + 1
        self.x = params.width * 2 + 1
        self.maze = self.init_maze()

    def is_move_valid_then_break(self, pos: np.ndarray, move: np.ndarray) -> bool:
        wall_pos = pos + (move // 2)
        moved_pos = pos + move
        wall_y, wall_x = wall_pos
        moved_y, moved_x = moved_pos
        if self.maze[wall_y, wall_x] == b'#' or self.maze[moved_y, moved_x] == b'c':
            return False
        self.maze[moved_y][moved_x] = b'c'
        self.maze[wall_y][wall_x] = b' '
        return True

    def break_random_wall(self, pos: np.ndarray, all_directions: dict[str, np.ndarray]) -> None:
            possible_walls = []
            for delta in all_directions.values():
                wall_y, wall_x = pos + (delta // 2)
                if self.maze[wall_y, wall_x] == b'W':
                    possible_walls.append((wall_y, wall_x))
            if possible_walls:
                wall_y, wall_x = choice(possible_walls)
                self.maze[wall_y, wall_x] = b' '

    def random_dfs(self) -> None:
        seed(self._seed)
        print(self._seed)
        all_directions: dict[str, np.ndarray] = {
            'North': np.array([-2, 0]),
            'South': np.array([2, 0]),
            'West': np.array([0, -2]),
            'East': np.array([0, 2])
        }
        direction = all_directions.copy()
        log_path: list = []
        entry_y, entry_x = map(lambda x: x * 2 + 1, self._params.entry)
        self.maze[entry_y, entry_x] = b'c'
        self.cells_visited += 1
        pos = np.array([entry_y, entry_x])
        total_cells = self._params.height * self._params.width
        just_arrived = True

        while self.cells_visited < total_cells:
            if pos[0] == entry_y and pos[1] == entry_x and not self._params.perfect:
                self.break_random_wall(pos, all_directions)
            if len(direction) == 0:
                if just_arrived and not self._params.perfect:
                    self.break_random_wall(pos, all_directions)
                direction = all_directions.copy()
                pos -= direction[log_path.pop(-1)]
                just_arrived = False
            else:
                move = choice(list(direction.keys()))
            if self.is_move_valid_then_break(pos, direction[move]):
                pos += direction[move]
                log_path.append(move)
                self.cells_visited += 1
                direction = all_directions.copy()
                just_arrived = True
            else:
                direction.pop(move)

        if not self._params.perfect:
            self.break_random_wall(pos, all_directions)
 
    def init_maze(self) -> NDArray[np.bytes_]:
        maze_map = np.zeros((self.y, self.x), dtype="S1")
        for y in range(self.y):
            for x in range(self.x):
                if x == 0 or y == 0 or x == self.x - 1 or y == self.y - 1:
                    maze_map[y][x] = b'#'
                elif x % 2 == 0 or y % 2 == 0:
                    maze_map[y][x] = b'W'
                else:
                    maze_map[y][x] = b'C'
        return maze_map

    def add_symbol(self) -> None:
        center_y = self._params.height
        center_x = self._params.width
        if center_y % 2 == 0:
            center_y += 1
        if center_x % 2 == 0:
            center_x += 1
        four_list = [[center_y + 2, center_x - 2], [center_y + 4, center_x - 2], [center_y, center_x - 2], [center_y, center_x - 4], [center_y, center_x - 6], [center_y - 2, center_x - 6],  [center_y - 4, center_x - 6]]
        two_list = [[center_y + 4, center_x + 6], [center_y + 4, center_x + 4], [center_y + 4, center_x + 2], [center_y + 2, center_x + 2], [center_y, center_x + 2], [center_y, center_x + 4], [center_y, center_x + 6], [center_y - 2, center_x + 6], [center_y - 4, center_x + 6], [center_y - 4, center_x + 4], [center_y - 4, center_x + 2]]
        for cell in four_list + two_list:
            self.cells_visited += 1
            y, x = cell

            for dy in (-1, 0, 1):
                for dx in (-1, 0, 1):
                    if dy != 0 and dx != 0:
                        ny = y + dy
                        nx = x + dx
                        if 0 <= ny < self.maze.shape[0] and 0 <= nx < self.maze.shape[1]:
                            self.maze[ny, nx] = b'#'
            self.maze[y, x] = b'c'

    def visualize(self) -> None:
        print(self.maze)
        app = Amaze(self.y, self.x, self.maze)
        app.run()
