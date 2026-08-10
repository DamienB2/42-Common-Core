from parse_config import parse_config
from mazegen import MazeGenerator
import random
from sys import argv
from visualize_maze import Amaze
from time import sleep


def main() -> None:
    """This is the main function of the a-maze-ing project,
    it executes the maze generation and visualization,
    generates a random seed and takes config filename as program argument.
    """
    try:
        if len(argv) != 2:
            raise ValueError("Usage: python3 a_maze_ing.py 'config filename'\n"
                             "Example: python3 a_maze_ing config.txt")
        params = parse_config(argv[1])
        seed = random.randint(0, 10000)
        print(f"maze seed: {seed}\n")
        maze = MazeGenerator(seed, params)
        maze.add_symbol()
        maze.random_dfs()
        maze.bfs()
        maze.create_output()
        sleep(1)
        app = Amaze(maze)
        app.run()
    except Exception as e:
        print(e)


if __name__ == "__main__":
    main()
