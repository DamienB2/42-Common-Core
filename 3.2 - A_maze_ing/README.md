*This project has been created as part of the 42 curriculum by nlovius, dbaise.*

# A-Maze-ing

An interactive Python terminal application that procedurally generates perfect and imperfect mazes using Randomized Depth-First Search (DFS) and finds optimal solutions using Breadth-First Search (BFS). Powered by **NumPy** byte matrix storage (`dtype="S1"`) for state representation and **Textual** for visual rendering.

---

## Table of Contents

1. [Description](https://www.google.com/search?q=%23description)
2. [Instructions](https://www.google.com/search?q=%23instructions)
3. [Configuration File Format](https://www.google.com/search?q=%23configuration-file-format)
4. [Maze Generation Algorithm](https://www.google.com/search?q=%23maze-generation-algorithm)
5. [Code Reusability](https://www.google.com/search?q=%23code-reusability)
6. [Team and Project Management](https://www.google.com/search?q=%23team-and-project-management)
7. [Resources & AI Usage](https://www.google.com/search?q=%23resources--ai-usage)
8. [Advanced Features](https://www.google.com/search?q=%23advanced-features)

---

## Description

**A-Maze-ing** explores graph theory, array-based grid structures, and TUI architecture. The engine initializes expanded 2D matrix grids using NumPy byte character codes, generates maze layouts via stack-logged Randomized DFS, guarantees path solvability, and finds the shortest path using a queue-driven BFS algorithm.

Key Objectives:

* 2D grid representation using NumPy byte character arrays (`dtype="S1"`).
* Procedural maze generation using Randomized DFS with backtracking logging and dynamic wall-carving.
* Support for both perfect (single-path, spanning tree) and imperfect (loop-containing) mazes via controlled random wall breaking.
* Shortest-path resolution using BFS with path backtracking and cardinal direction tracking.
* Interactive terminal UI built with Textual.

---

## Instructions

### Prerequisites

* Python 3.10 or higher
* `make` (GNU Make utility)

### Installation

Install all required Python dependencies (`numpy`, `textual`, `flake8`, `mypy`) using Makefile:

```bash
make install

```

### Execution Rules

The project includes standard Makefile rules for execution, debugging, linting, and cleanup:

* **Run the program:**
```bash
make run

```


*Executes `python3 a_maze_ing.py config.txt`.*
* **Debug mode (interactive `pdb` stepper):**
```bash
make debug

```


*Launches the program inside Python's built-in debugger (`python3 -m pdb a_maze_ing.py config.txt`).*
* **Code quality checks (Linter & Static Type Checker):**
```bash
make lint

```


*Runs `flake8` and `mypy` across the codebase to enforce style guidelines and type safety.*
* **Clean temporary files:**
```bash
make clean

```


*Removes `__pycache__` directories, `.pyc` compiled files, `.mypy_cache`, and `.pytest_cache`.*

---

## Configuration File Format

The parser processes configuration files (`config.txt`) using a strict key-value format. Lines starting with `#` are ignored as comments.

### Configuration File Example

```ini
WIDTH=90
HEIGHT=70
ENTRY=0, 0
EXIT=89, 69
# this is a comment bro
OUTPUT_FILE=maze.txt
PERFECT=True

```

### Config Rules & Grid Mapping

1. Lines starting with `#` or empty lines are ignored as comments.
2. Key-value pairs follow the `KEY=VALUE` format. Coordinates for `ENTRY` and `EXIT` are specified as comma-separated integers (`X, Y`).
3. `ENTRY` and `EXIT` logical cell coordinates $(x, y)$ are automatically mapped to matrix coordinates $(2y+1, 2x+1)$ to accommodate inner wall grids.
4. The resulting NumPy matrix dimension is initialized to size $(2 \times \text{HEIGHT} + 1, 2 \times \text{WIDTH} + 1)$.
5. `PERFECT` takes a boolean (`True` or `False`) to toggle between perfect spanning-tree mazes and imperfect mazes containing loops.

---

## Maze Generation Algorithm

### Chosen Algorithm: Randomized Depth-First Search (DFS)

Our maze generator uses an array-backed **Randomized Depth-First Search** algorithm with back-tracking step tracking (`log_path`).

#### Matrix State Encoded Symbols:

* `b'#'`: Outer boundary wall
* `b'W'`: Solid internal wall
* `b'C'`: Unvisited cell
* `b'c'`: Visited cell
* `b' '`: Carved passage
* `b'E'` / `b'e'`: Entry and Exit markers
* `b'p'`: Final solution path

```
1. Initialize NumPy matrix grid: set outer borders to b'#', even wall grids to b'W', and cell centers to b'C'.
2. Map logical entry point (x, y) to matrix position (2y + 1, 2x + 1).
3. Mark entry cell as visited (b'c'), reset visited counter, and initialize log_path stack.
4. While visited cells count < total cells:
   a. If valid unvisited neighbor move exists:
      - Pick a direction randomly.
      - Remove wall between cells (b'W' -> b' ').
      - Mark target cell as visited (b'c').
      - Push direction to log_path stack and set position to target cell.
   b. Else (dead end reached):
      - If imperfect maze mode is enabled: trigger _break_random_wall(pos) to remove adjacent b'W' wall.
      - Pop last direction from log_path and backtrack position.
5. If imperfect maze mode: carve additional random walls to introduce loops.

```

### Why We Chose Randomized DFS

1. **Long Corridors & Organic Branching:** DFS produces mazes with long winding paths and high solution complexity, making terminal visualization dynamic.
2. **Explicit Backtracking via Direction Logs:** Storing move history in `log_path` allows deterministic reverse step traversal during maze generation and straightforward visual step rendering.
3. **Controlled Loop Insertion:** By triggering `_break_random_wall` when backtracking from dead ends or at entry points, we can seamlessly transition between perfect tree mazes and multi-path imperfect mazes.

---

## Code Reusability

Our codebase isolates data structures, generation algorithms, pathfinding solvers, and visual components for high reusability:

1. **NumPy Matrix Storage Layout (`init_maze` & Grid Model):**
* The expanded odd-dimension matrix (`b'#'` for outer walls, `b'W'` for inner walls, `b'C'` for cells) isolates grid spatial logic from UI rendering.
* Uses `dtype="S1"` 1-byte elements, providing low memory footprint and high performance slicing for any 2D grid project.


2. **Generic Wall Validation & Carving Engine (`_is_move_valid_then_break`):**
* Encapsulates neighbor calculation `pos + (move // 2)` and cell updates into a single reusable helper function.


3. **BFS Pathfinding Solver (`bfs`):**
* Decoupled search algorithm using a queue (`live_cells`) and parent coordinate map (`stash`).
* Reconstructs shortest path vectors into cardinal directions (`self.path`) while writing `b'p'` path overlays directly onto any matching matrix layout.



---

## Team and Project Management

### Work Repartition & Roles

* **`nlovius` (Core Logic & Data Architecture):**
* Implemented the NumPy matrix initialization (`init_maze`) using `dtype="S1"`.
* Developed the `random_dfs` generation engine with stack-based direction logging and `_break_random_wall` imperfect maze carving.
* Implemented the queue-based `bfs` shortest-path solver and path backtracking logic.


* **`dbaise` (TUI Engineering & Systems Architecture):**
* Designed the visual application wrapper using **Textual**.
* Built the configuration parser handling key-value structures, comma-separated coordinates (`ENTRY=0, 0`), and comment filtering.
* Authored the project `Makefile` targets (`run`, `debug`, `lint`, `clean`) and quality configuration rules (`flake8`, `mypy`).
* Handled interactive keyboard bindings, step animations, and documentation.



### Planning vs. Actual Evolution

| Milestone | Planned Timeline | Actual Completion | Notes / Adjustments |
| --- | --- | --- | --- |
| **Phase 1: Grid Matrix Layout & Makefile Setup** | Days 1–2 | Days 1–2 | Finalized byte character flags (`b'#'`, `b'W'`, `b'C'`) and Makefile rules early. |
| **Phase 2: DFS Generation & Imperfect Wall Carving** | Days 3–4 | Days 3–5 | Integrated `_break_random_wall` dead-end logic for imperfect mazes. |
| **Phase 3: BFS Pathfinding Solver** | Days 6–7 | Days 6–7 | Implemented parent map (`stash`) and string path accumulation. |
| **Phase 4: Textual TUI & Code Linting (`mypy`/`flake8`)** | Days 8–9 | Days 8–10 | Connected NumPy matrix states directly to visual Textual widgets and ran strict linting. |

### Retrospective

* **What Worked Well:**
* **Single-Byte NumPy Arrays (`dtype="S1"`):** Character byte assignments made state transitions efficient and allowed direct string-to-widget rendering.
* **Automated Makefile Workflow:** Having predefined `make lint` and `make debug` rules streamlined debugging with `pdb` and guaranteed standard strict typing compliance across all modules.


* **What Could Be Improved:**
* **Backtracking State Synchronization:** Tying visual step updates to `log_path.pop(-1)` required careful coordinate synchronization between generation and display frames.



### Tools Used

* **Language & Runtime:** Python 3
* **Core Libraries & Tools:** `numpy`, `textual`, `flake8`, `mypy`, `pdb`
* **Build System:** GNU Make
* **Version Control:** Git & GitHub
* **Debugging & Profiling:** `@timer` decorator, `pdb`

---

## Resources & AI Usage

### References

* **NumPy Documentation:** [NumPy Array Slicing and Byte Arrays](https://www.google.com/search?q=https://numpy.org/doc/)
* **Textual Framework:** [Textual Documentation](https://www.google.com/search?q=https://textual.textualize.io/)
* **Graph Algorithms:** *Introduction to Algorithms* (CLRS) - Chapter 22 (BFS & DFS)

### Description of AI Assistance

AI models were used in accordance with curriculum rules for specific tasks:

1. **Refactoring Matrix Operations:** AI assisted in optimizing vector offset calculations.
2. **Generating Test Configurations:** Used AI to build invalid configuration test suites.
3. **Documentation:** AI helped organize Markdown structure according to 42 curriculum requirements.

*All algorithm implementations (`random_dfs`, `bfs`), state transitions, Makefile routines, and TUI wiring were developed and verified by team members.*

---

## Advanced Features

1. **Integrated BFS Path Reconstruction:** The BFS solver tracks movement vectors in a parent dictionary (`stash`), populating `self.path` with cardinal movement steps while marking matrix cells with `b'p'`.
2. **Dynamic Imperfect Maze Generation:** Implements `_break_random_wall` during DFS dead-end backtracking to break walls between visited cells, producing mazes with alternate loops.
3. **Interactive Textual Visualizer:** Real-time TUI rendering maze generation state, wall carving, BFS exploration frontier (`b'C'`), and final shortest path (`b'p'`).
