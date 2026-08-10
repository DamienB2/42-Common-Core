import numpy as np
from numpy.typing import NDArray
from typing import Any
from random import seed, choice
from .function_timer import timer
from .maze_parameters import MazeParams


class MazeGenerator:
    """Generator for 2D mazes using randomized DFS and BFS.

    Attributes:
        cells_visited (int): Count of visited cells during generation.
        y (int): Total height of maze grid including walls/borders.
        x (int): Total width of maze grid including walls/borders.
        maze (NDArray[np.bytes_]): Grid representation of the maze.
        all_directions (dict[str, np.ndarray]): Direction vectors.
        path (str): Encoded path string from entry to exit.
    """

    def __init__(self, seed_value: int,
                 params: dict[str, Any]) -> None:
        """Initialize maze generator with a seed and parameters.

        Args:
            seed_value (int): Random seed value.
            params (dict[str, Any]): Dictionary of configuration parameters.
        """
        self._seed = seed_value
        self._params = MazeParams(**params)
        self._params_copy = params
        self.cells_visited = 0
        self.y = self._params.height * 2 + 1
        self.x = self._params.width * 2 + 1
        self.maze = self.init_maze()
        self.all_directions: dict[str, np.ndarray] = {
            'North': np.array([-2, 0]),
            'South': np.array([2, 0]),
            'West': np.array([0, -2]),
            'East': np.array([0, 2])
        }
        self.path = ''

    def _is_move_valid_then_break(self, pos: np.ndarray,
                                  move: np.ndarray) -> bool:
        """Check if move is valid and break the intermediate wall.

        Args:
            pos (np.ndarray): Current coordinate position array [y, x].
            move (np.ndarray): Movement vector array [dy, dx].

        Returns:
            bool: True if move was valid and executed, False otherwise.
        """
        wall_pos = pos + (move // 2)
        moved_pos = pos + move
        wall_y, wall_x = wall_pos
        moved_y, moved_x = moved_pos
        if self.maze[wall_y, wall_x] == b'#' or \
                self.maze[moved_y, moved_x] == b'c':
            return False
        self.maze[moved_y][moved_x] = b'c'
        self.maze[wall_y][wall_x] = b' '
        return True

    def _break_random_wall(self, pos: np.ndarray) -> None:
        """Randomly break an adjacent wall from the given position.

        Args:
            pos (np.ndarray): Current coordinate position array [y, x].
        """
        possible_walls = []
        for delta in self.all_directions.values():
            wall_y, wall_x = pos + (delta // 2)
            if self.maze[wall_y, wall_x] == b'W':
                possible_walls.append((wall_y, wall_x))
        if possible_walls:
            wall_y, wall_x = choice(possible_walls)
            self.maze[wall_y, wall_x] = b' '

    @timer
    def random_dfs(self) -> None:
        """Generate maze layout using randomized Depth-First Search."""
        seed(self._seed)
        direction = self.all_directions.copy()
        log_path: list[str] = []
        entry_y, entry_x = map(lambda x: x * 2 + 1, self._params.entry)
        self.maze[entry_y, entry_x] = b'c'
        self.cells_visited += 1
        pos = np.array([entry_y, entry_x])
        total_cells = self._params.height * self._params.width
        just_arrived = True

        while self.cells_visited < total_cells:
            if pos[0] == entry_y and pos[1] == entry_x and \
                    not self._params.perfect:
                self._break_random_wall(pos)
            if len(direction) == 0:
                if just_arrived and not self._params.perfect:
                    self._break_random_wall(pos)
                direction = self.all_directions.copy()
                pos -= direction[log_path.pop(-1)]
                just_arrived = False
            else:
                move = choice(list(direction.keys()))
            if self._is_move_valid_then_break(pos, direction[move]):
                pos += direction[move]
                log_path.append(move)
                self.cells_visited += 1
                direction = self.all_directions.copy()
                just_arrived = True
            else:
                direction.pop(move)

        if not self._params.perfect:
            self._break_random_wall(pos)

    @timer
    def bfs(self) -> None:
        """Find path from entry to exit using Breadth-First Search.
            path is stored as a list of directions and also updated on
            the maze with b'p' cells."""
        live_cells: list[tuple[int, ...]] = []
        stash: dict[tuple[int, ...], str] = {}

        start_coords = tuple(map(lambda x: x * 2 + 1, self._params.entry))
        live_cells.append(start_coords)

        exit_coords = tuple(
            map(lambda x: x * 2 + 1, self._params.exit))

        while live_cells[0][0] != exit_coords[0] or \
                live_cells[0][1] != exit_coords[1]:
            for direction, delta in self.all_directions.items():
                if self.maze[tuple(live_cells[0] + (delta // 2))] == b' ' and \
                        self.maze[tuple(live_cells[0] + delta)] == b'c':

                    live_cells.append(tuple(live_cells[0] + delta))
                    self.maze[tuple(live_cells[-1])] = b'C'

                    stash[live_cells[-1]] = direction
            live_cells.pop(0)

        path_cell = exit_coords
        while path_cell[0] != start_coords[0] or \
                path_cell[1] != start_coords[1]:
            self.maze[tuple(path_cell)] = b'p'

            cardinal_str = stash[tuple(path_cell)]
            move_vector = self.all_directions[cardinal_str]

            self.path = cardinal_str[0] + self.path

            mid_cell = path_cell - move_vector // 2
            self.maze[tuple(mid_cell)] = b'p'

            path_cell = tuple(path_cell - move_vector)
            self.maze[start_coords] = b'E'
            self.maze[exit_coords] = b'e'

    def init_maze(self) -> NDArray[np.bytes_]:
        """Initialize grid array with wall and cell structures.

        Returns:
            NDArray[np.bytes_]: 2D byte array of initial grid state.
        """
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
        """Embed decorative symbol structures near center of maze."""
        center_y = self._params.height
        center_x = self._params.width
        if center_y < 7 or center_x < 9:
            return
        if center_y % 2 == 0:
            center_y += 1
        if center_x % 2 == 0:
            center_x += 1
        four_list = [[center_y + 2, center_x - 2],
                     [center_y + 4, center_x - 2],
                     [center_y, center_x - 2],
                     [center_y, center_x - 4],
                     [center_y, center_x - 6],
                     [center_y - 2, center_x - 6],
                     [center_y - 4, center_x - 6]]
        two_list = [[center_y + 4, center_x + 6],
                    [center_y + 4, center_x + 4],
                    [center_y + 4, center_x + 2],
                    [center_y + 2, center_x + 2],
                    [center_y, center_x + 2],
                    [center_y, center_x + 4],
                    [center_y, center_x + 6],
                    [center_y - 2, center_x + 6],
                    [center_y - 4, center_x + 6],
                    [center_y - 4, center_x + 4],
                    [center_y - 4, center_x + 2]]
        for cell in four_list + two_list:
            self.cells_visited += 1
            y, x = cell

            for dy in (-1, 0, 1):
                for dx in (-1, 0, 1):
                    if dy != 0 or dx != 0:
                        ny = y + dy
                        nx = x + dx
                        if 0 <= ny < self.maze.shape[0] and \
                                0 <= nx < self.maze.shape[1]:
                            self.maze[ny, nx] = b'#'
            self.maze[y, x] = b'c'

    def create_output(self) -> None:
        """Format maze state into hex grid and write to output file."""
        output = ''
        wall_or_border = (b'W', b'#')
        for y in range(1, self.y, 2):
            for x in range(1, self.x, 2):
                cell_value = 0
                if self.maze[y - 1, x] in wall_or_border:
                    cell_value += 1
                if self.maze[y, x + 1] in wall_or_border:
                    cell_value += 2
                if self.maze[y + 1, x] in wall_or_border:
                    cell_value += 4
                if self.maze[y, x - 1] in wall_or_border:
                    cell_value += 8
                output += format(cell_value, 'x')
            output += '\n'
        output += '\n'
        output += str(self._params.entry[1]) + \
            ',' + str(self._params.entry[0]) + '\n'
        output += str(self._params.exit[1]) + \
            ',' + str(self._params.exit[0]) + '\n'
        output += self.path + '\n'
        with open(self._params.output_file, 'w') as f:
            f.write(output)
