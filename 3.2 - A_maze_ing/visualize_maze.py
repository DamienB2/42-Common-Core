from mazegen.maze_generator import MazeGenerator
from textual.app import App, ComposeResult
from textual.color import Color
from textual.containers import Container, Horizontal, Vertical
from textual.reactive import reactive
from textual.widgets import Button, Footer, Header, Static
from textual_canvas import Canvas

THEMES = {
    "Classic": (
        Color.parse("lightgrey"),
        Color.parse("orange"),
        Color.parse("red"),
        Color.parse("grey"),
        Color.parse("green"),      # Entry color
        Color.parse("blue"),       # Exit color
    ),
    "Neon": (
        Color.parse("cyan"),
        Color.parse("magenta"),
        Color.parse("yellow"),
        Color.parse("#111122"),
        Color.parse("#00ff00"),    # Entry color
        Color.parse("#0000ff"),    # Exit color
    ),
    "Forest": (
        Color.parse("darkgreen"),
        Color.parse("saddlebrown"),
        Color.parse("lime"),
        Color.parse("#051505"),
        Color.parse("yellowgreen"),  # Entry color
        Color.parse("teal"),       # Exit color
    ),
}


class Amaze(App):
    """A Textual terminal application for displaying and interacting with maze.

    Attributes:
        CSS (str): Textual stylesheet for screen layout, sidebar
                   and canvas styling.
        BINDINGS (list[tuple[str, str, str]]): Keybindings for app actions.
        show_path (reactive[bool]): Controls whether the solution path
                                    is visible.
        active_theme_name (reactive[str]): Currently active color theme name.
    """

    CSS = """
    Screen {
        layout: horizontal;
    }

    #sidebar {
        width: 24;
        height: 100%;
        background: $surface;
        padding: 1 1;
        border-right: heavy $primary;
    }

    #sidebar Button {
        width: 100%;
        margin-bottom: 1;
    }

    #canvas-container {
        align: center middle;
        height: 100%;
        width: 1fr;
    }

    Canvas {
        width: auto;
        height: auto;
    }

    .section-title {
        text-style: bold;
        margin-top: 1;
        margin-bottom: 1;
    }
    """

    BINDINGS = [
        ("q", "quit", "Quit"),
        ("p", "toggle_path", "Toggle Path"),
    ]

    show_path: reactive[bool] = reactive(True)
    active_theme_name: reactive[str] = reactive("Classic")

    def __init__(self, maze: MazeGenerator) -> None:
        """Initialize the application with a 2D matrix representing the maze.

        Args:
            matrix (NDArray[np.bytes_]): A 2D NumPy array of byte value
                                         defining the maze layout.
        """
        super().__init__()
        self.maze = maze
        self.matrix = maze.maze
        self.y, self.x = self.matrix.shape

    def compose(self) -> ComposeResult:
        """Compose the widget layout for the Textual application interface.

        Yields:
            ComposeResult: The composed hierarchy of Textual widgets.
        """
        yield Header()
        with Horizontal():
            with Vertical(id="sidebar"):
                yield Static("Themes", classes="section-title")
                for theme_name in THEMES.keys():
                    yield Button(
                        theme_name,
                        id=f"theme-{theme_name.lower()}",
                        variant="primary"
                    )

                yield Static("Controls", classes="section-title")
                yield Button(
                    "Toggle Path", id="btn-toggle-path", variant="default"
                )
                yield Button(
                    "Regenerate maze", id="btn-regenerate-maze",
                    variant="warning"
                )
                yield Button("Exit", id="btn-exit", variant="error")

            with Container(id="canvas-container"):
                yield Canvas(self.x, self.y, THEMES[self.active_theme_name][3])
        yield Footer()

    def on_mount(self) -> None:
        """Draw the initial maze state once widgets are mounted."""
        self.draw_maze()

    def watch_show_path(self, show_path: bool) -> None:
        """Called automatically whenever self.show_path changes.

        Args:
            show_path (bool): The value updating path visibility.
        """
        self.draw_maze()

    def watch_active_theme_name(self, theme_name: str) -> None:
        """Called automatically whenever self.active_theme_name changes.

        Args:
            theme_name (str): The newly selected theme key.
        """
        self.draw_maze()

    def draw_maze(self) -> None:
        """Redraws the canvas based on current state and theme."""
        try:
            canvas = self.query_one(Canvas)
        except Exception:
            return  # Canvas not mounted yet

        wall_color, border_color, path_color, \
            bg_color, entry_color, exit_color = THEMES[
                self.active_theme_name
            ]

        if hasattr(canvas, "clear"):
            canvas.clear()

        for row_index, row in enumerate(self.matrix):
            for col_index, cell in enumerate(row):
                if cell == b"W":
                    canvas.set_pixel(col_index, row_index, wall_color)
                elif cell == b"#":
                    canvas.set_pixel(col_index, row_index, border_color)
                elif cell == b"p":
                    if self.show_path:
                        canvas.set_pixel(col_index, row_index, path_color)
                    else:
                        canvas.set_pixel(col_index, row_index, bg_color)
                elif cell == b"E":
                    canvas.set_pixel(col_index, row_index, entry_color)
                elif cell == b"e":
                    canvas.set_pixel(col_index, row_index, exit_color)
                else:
                    canvas.set_pixel(col_index, row_index, bg_color)

        canvas.refresh()

    def on_button_pressed(self, event: Button.Pressed) -> None:
        """Handle click events for all buttons.

        Args:
            event (Button.Pressed): The button press event payload.
        """
        button_id = event.button.id

        if button_id == "btn-exit":
            self.exit()
        elif button_id == "btn-toggle-path":
            self.action_toggle_path()
        elif button_id == "btn-regenerate-maze":
            self.maze._seed += 1
            self.maze = MazeGenerator(self.maze._seed, self.maze._params_copy)
            self.maze.add_symbol()
            self.maze.random_dfs()
            self.maze.bfs()

            self.matrix = self.maze.maze

            self.draw_maze()
        elif button_id and button_id.startswith("theme-"):

            theme_key = button_id.replace("theme-", "").capitalize()
            if theme_key in THEMES:
                self.active_theme_name = theme_key

    def action_toggle_path(self) -> None:
        """Action handler linked to key binding and button."""
        self.show_path = not self.show_path
