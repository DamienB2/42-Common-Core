from parse_config import parse_config
from maze_visualizer import MazeGenerator
import random


def main() -> None:
    try:
        params = parse_config('config.txt')
        maze = MazeGenerator(random.randint(0, 9999), params)
        # 4257
        maze.random_dfs()
        maze.visualize()

    except Exception as e:
        print(e)


if __name__ == "__main__":
    main()
