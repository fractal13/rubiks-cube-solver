# Rubik's Cube Solver

This project provides a C++ application for solving a 3x3x3 Rubik's Cube. It features a robust cube representation, a comprehensive set of cube manipulation moves, and a solver capable of operating in both interactive and pipe-based modes.

## Features

*   **Rubik's Cube Representation:** A detailed internal representation of a 3x3x3 Rubik's Cube state.
*   **Extensive Move Set:** Supports standard face rotations (U, L, F, R, B, D), their inverse (prime) counterparts, slice rotations (M, E, S), and whole-cube rotations (X, Y, Z).
*   **Solving Algorithm:** Implements an algorithm to find a solution for a scrambled Rubik's Cube.
*   **Interactive Mode:** Allows users to input moves one by one and observe the cube's state.
*   **Pipe-based Mode:** Enables automated processing of cube states or move sequences from standard input, suitable for scripting or integration with other tools.

## Building the Project

To build the Rubik's Cube solver, navigate to the project root directory and run `make`:

```bash
make
```

This will first build the `ai-lib` dependency and then compile the Rubik's Cube solver, creating the executable `rubiks-cube-solver` in the project root directory.

## Usage

The solver can be used in two primary modes: interactive and pipe-based.

### Interactive Mode

When run without piping input, the solver enters an interactive mode, prompting the user with `rubiks> `. You can type commands and press Enter.

```bash
./rubiks-cube-solver
```

Example interactive session:

```
rubiks> initial_solved
rubiks> show_line
wwwwwwwwwgggggggggrrrrrrrrrbbbbbbbbboooooooooyyyyyyyyy
rubiks> rotate white cw
rubiks> show_line
wwrwwrwwrgggggggggrrrbbbrrrbbbbbbbbboooooooooyyyyyyyyy
rubiks> help
Rubiks Cube Solver Help
=======================
Available commands:
  echo <text>                      - Display text
  initial_solved                   - Set cube to solved state
  initial_line <state>             - Set cube from one-line display
  initial <file>                   - Set cube from file
  show                             - Display current cube
  show_line                        - Display cube in one-line format
  goal_solved                      - Set goal cube to solved state
  goal_line <state>                - Set goal cube from one-line display
  show_goal_line                   - Display goal cube in one-line format
  goal <file>                      - Set goal cube from file
  rotate <face> <direction>        - Rotate a face (cw, ccw, 180)
  shuffle <count>                  - Shuffle cube with random rotations
  shuffle_range <min> <max>        - Shuffle with random count between min and max
  solve <tree|graph> <frontier>    - Solve the cube using search algorithm
  solve_save <tree|graph> <frontier> - Solve and save solution for later use
  hla_solve                        - Solve using HLA algorithm
  config <param> <value>           - Configure search parameters
  show_config                      - Display current configuration settings
  find <corner|edge>               - Find position of corner or edge piece
  isequal <cube>                   - Check if current cube equals given cube
  generate <depth>                 - Generate all cubes within given depth
  dump_known_cubes [<label>]       - Display known cubes
  save_known_cubes                 - Save known cubes to file
  repeat <count> <filename>        - Repeat commands from a file
  help                             - Display this help message
  quit/exit                        - Terminate the program

Supported faces: white, green, red, blue, orange, yellow
Supported directions: cw, ccw, 180 (if enabled)
Supported frontiers: astar, bfs, dfs, dl, ids, al, greedy, uc

rubiks> quit
```

### Pipe-based Mode

You can pipe a sequence of commands into the solver. Each command should be on a new line. This is useful for scripting or automated testing.

**Example: Applying a sequence of moves and displaying the result**

```bash
echo -e "initial_solved\nrotate white cw\nrotate red ccw\nshow_line" | ./rubiks-cube-solver
```

This will output the one-line representation of the cube after the rotations.

**Example: Solving a scrambled cube**

You can provide a `initial_line` command with a 54-character string representing the facelets of a scrambled cube, followed by a `solve` command. Each character corresponds to a facelet color: 'w' (white), 'g' (green), 'r' (red), 'b' (blue), 'o' (orange), 'y' (yellow). The order of facelets typically follows a standard convention (e.g., URFDLB faces, row by row).

```bash
echo -e "initial_line wwwwwwwwwgggggggggrrrrrrrrrbbbbbbbbboooooooooyyyyyyyyy\nsolve graph astar" | ./rubiks-cube-solver
```

The solver will then attempt to solve this cube state and output the solution (a sequence of `rotate` commands).

### Output Format

When a solution is found, the solver will output a sequence of `rotate` commands, each on a new line, representing the solution. For other commands, the output will vary as described in the `help` command output.

## Cube Display Formats

The Rubik's Cube solver supports two primary display formats for representing the cube's state:

### Multi-line Display (`show` command)

This format provides a human-readable, multi-line representation of the cube, displaying each face sequentially. Each face is shown as a 3x3 grid of characters, where each word represents a facelet's color.

Example (`show` command output for a solved cube):

```
white white white
white white white
white white white

green green green
green green green
green green green

red red red
red red red
red red red

blue blue blue
blue blue blue
blue blue blue

orange orange orange
orange orange orange
orange orange orange

yellow yellow yellow
yellow yellow yellow
yellow yellow yellow
```

### Single-line Display (`show_line`, `initial_line`, `goal_line` commands)

This format represents the entire cube as a single 54-character string. Each face consists of 9 characters, representing its facelets. This format is used for both input (e.g., with `initial_line` and `goal_line`) and output (with `show_line`).

The order of faces in the string is typically: White (Up), Green (Left), Red (Front), Blue (Right), Orange (Back), Yellow (Down). Within each face, facelets are read row by row.

Example (`show_line` command output for a solved cube):

```
wwwwwwwwwgggggggggrrrrrrrrrbbbbbbbbboooooooooyyyyyyyyy
```

## Cube Notation

The `rotate` command uses the following notation:

*   **Faces:** `white`, `green`, `red`, `blue`, `orange`, `yellow`
*   **Directions:**
    *   `cw`: Clockwise 90-degree turn
    *   `ccw`: Counter-clockwise 90-degree turn
    *   `180`: 180-degree turn (if `ALLOW_180` is enabled during compilation)

The standard Rubik's Cube notation (U, L, F, R, B, D, etc.) is mapped to these internal commands. For example, `U` corresponds to `rotate white cw`. Middle slice turns (M, E, S) and whole-cube rotations (x, y, z) are also supported if `ALLOW_MIDDLE_ROTATE` is enabled during compilation, with corresponding `rotate` commands (e.g., `rotate red up` for M).

## Configuration Options

The solver's behavior can be configured using the `config` command. This allows you to adjust various parameters related to the search algorithm and cube manipulation.

To set a configuration parameter, use the command: `config <param> <value>`

To view the current configuration, use the command: `show_config`

Here are the available configuration parameters:

*   **`generation_limit`**: Sets the maximum number of nodes the search algorithm will generate. (Default: 0, meaning no limit)
*   **`storage_limit`**: Sets the maximum number of nodes the search algorithm will store in memory. (Default: 0, meaning no limit)
*   **`depth_limit`**: Sets the maximum depth for depth-limited search algorithms (e.g., `dl`, `ids`). (Default: 0)
*   **`astar_limit`**: Sets a limit for the A* search algorithm, potentially related to the f-cost. (Default: 0)
*   **`allow_180`**: A boolean flag (0 or 1) to enable or disable 180-degree rotations as basic moves. (Default: 0)
*   **`allow_middle_rotate`**: A boolean flag (0 or 1) to enable or disable middle slice rotations (M, E, S). (Default: 1, as enabled by default in `cglCubeSolver.cpp`)
*   **`edge_heuristic`**: A boolean flag (0 or 1) to enable the edge heuristic for the solver.
*   **`corner_heuristic`**: A boolean flag (0 or 1) to enable the corner heuristic for the solver.
*   **`tile_heuristic`**: A boolean flag (0 or 1) to enable the misplaced tile heuristic for the solver.
*   **`end_game_heuristic`**: A boolean flag (0 or 1) to enable the end-game heuristic for the solver.
*   **`edge_corner_heuristic`**: A boolean flag (0 or 1) to enable a combined edge and corner heuristic.
*   **`all_heuristic`**: A boolean flag (0 or 1) to enable all available heuristics.
*   **`zero_heuristic`**: A boolean flag (0 or 1) to use a zero heuristic (effectively a uniform-cost search if step cost is 1).
*   **`known_cube_minimum`**: Sets a minimum value for known cube states, used in some heuristics. (Default: 0.0)
*   **`known_cube_report_count`**: Sets the frequency for reporting saved known cube states during generation. (Default: 0.0, meaning no reporting)
*   **`apply_solution`**: A boolean flag (0 or 1) to automatically apply the found solution to the current cube state. (Default: 0)
