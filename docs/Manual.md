# Rubik's Cube Solver Manual

This document provides a manual for the Rubik's Cube Solver application.

## Overview

The Rubik's Cube Solver is a command-line application that allows you to manipulate and solve Rubik's Cubes. You can initialize a cube to a specific state, shuffle it, and then use various search algorithms to find a solution. The application supports defining custom moves, cubes, and even planning actions to solve the cube in a step-by-step manner.

The typical workflow is as follows:

1.  **Initialize the cube:** Use the `init cube` command to set the initial state of the cube. You can also set a goal state using `init goal`.
2.  **Define moves and cubes:** Use the `define` command to create custom moves or cube configurations.
3.  **Search for a solution:** Use the `search` or `plan` command to find a sequence of moves to solve the cube.
4.  **Apply the solution:** The solution can be automatically applied to the cube, or you can manually apply moves using the `rotate` command.
5.  **Verify the solution:** Use the `equal` command to check if the cube is solved.

The application also provides commands for managing configuration, running scripts from a file, and getting help on commands.

## Commands

### echo

**Usage:** `echo word*`

**Description:** Displays the words of its line separated by a single space.

---

### config

**Usage:** `config option value`

**Description:** Sets the option to value. Only predefined options are allowed. All values are interpreted as floating point. Changing `enable_half` or `enable_slice` will cause the moves to be reset to basic moves, with application of these configurations.

**Available Options:**

| Option                        | Description                                               | Values                                                              |
|:------------------------------|:----------------------------------------------------------|:--------------------------------------------------------------------|
| `enable_prompt`               | Enable or disable the command prompt.                     | `enable`, `disable`                                                 |
| `enable_half`                 | Enable or disable half-turns (180-degree rotations).      | `enable`, `disable`                                                 |
| `enable_slice`                | Enable or disable slice-turns.                            | `enable`, `disable`                                                 |
| `cube_display`                | Set the display format for the cube.                      | `t`, `one_word`, `one_line`                                         |
| `move_display`                | Set the display format for moves.                         | `name`, `sub_moves`                                                 |
| `depth_limit`                 | Set the depth limit for depth-limited search.             | numeric value                                                       |
| `ids_depth`                   | Set the initial depth for iterative deepening search.     | numeric value                                                       |
| `ids_limit`                   | Set the limit for iterative deepening search.             | numeric value                                                       |
| `astar_limit`                 | Set the limit for A* search.                              | numeric value                                                       |
| `generation_limit`            | Set the generation limit for search algorithms.           | numeric value                                                       |
| `storage_limit`               | Set the storage limit for search algorithms.              | numeric value                                                       |
| `apply_solution`              | Automatically apply the solution to the cube after a search.| `enable`, `disable`                                                 |
| `heuristic`                   | Set the heuristic function for search algorithms.         | `zero`, `facelet`, `edge_facelet`, `corner_facelet`, `facelet_max`, `color_count` |
| `solution_display`            | Set the display format for the solution.                  | `human`, `machine`                                                  |
| `initial_cube_remove_wildcard`| Automatically remove wildcards from the initial cube.     | `enable`, `disable`                                                 |
| `compile_preconditions`       | Compile plan action preconditions for faster execution.   | `enable`, `disable`                                                 |

---

### init

**Usage:** `init cube|goal solved|cube_configuration+`

**Description:** Assigns the current state of either the working cube or the goal cube. Goal cube may contain `*` for wildcards. May use the `solved` keyword instead of an actual configuration. A `cube_configuration` is either 54 facelet symbols, or the name of a defined cube. For goal, more than one configuration may be specified, space separated.

---

### shuffle

**Usage:** `shuffle num1 [num2]?`

**Description:** Randomly applies rotations to the current cube num1 times. If num2 is present, a number in the range [num1, num2] (inclusive) is selected as the number of rotations to apply. Configured moves are selected with equal probability.

---

### generate

**Usage:** `generate depth filename`

**Description:** Create all unique cubes up to 'depth' steps from the current cube. Saves the results in 'filename'.

---

### equal

**Usage:** `equal cube_configuration|solved|goal`

**Description:** Checks if the current state matches the specified cube configuration, a solved cube, or one of the goal cubes. If so, displays the string TRUE, if not, displays the string FALSE. Note that the `cube_configuration` or the goal cube may contain wildcards, which match anything. A `cube_configuration` is either 54 facelet symbols, or the name of a defined cube.

---

### search

**Usage:** `search tree|graph astar|bfs|dfs|dl|ids|bl|al|greedy|uc`

**Description:** Searches for a path from the current cube to the goal cube, which may contain wildcards. Uses the search algorithm and frontier specified. `dl` and `bl` require the `depth_limit` configuration. `ids` requires the `ids_limit` configuration. `al` requires the `astar_limit` configuration. All algorithms and frontiers require the `generation_limit` and `storage_limit` configurations. The `apply_solution` configuration controls whether a solution is applied to the current cube.

---

### plan

**Usage:** `plan tree|graph astar|bfs|dfs|dl|ids|al|greedy|uc`

**Description:** Searches for a plan from the current cube to the goal cube. Uses only the defined plan actions. Uses the search algorithm and frontier specified. `dl` requires the `depth_limit` configuration. `ids` requires the `ids_limit` configuration. `al` requires the `astar_limit` configuration. All algorithms and frontiers require the `generation_limit` and `storage_limit` configurations. The `apply_solution` configuration controls whether a solution is applied to the current cube.

---

### rotate

**Usage:** `rotate [U|U'|L|L'|F|F'|R|R'|B|B'|D|D']+`

**Description:** Rotates through the given sequence. The moves must be separated by whitespace.

---

### moves

**Usage:** `moves [U|U'|L|L'|F|F'|R|R'|B|B'|D|D']+|basic`

**Description:** Restricts legal moves to those listed, or the basic 12 moves augmented by `enable_half` and `enable_slice` configuration options.

---

### show

**Usage:** `show config|cube|goal|moves|defined_moves|defined_cubes|defined_plan_actions`

**Description:** Displays the requested item. The default display format for cubes and moves can be changed using the `config` command.

---

### define

**Usage:** `define cube|move|plan_action object_name object_configuration`

**Description:** Defines a named object of the specified type. Cubes are defined by the 54 tile characters. Spaces and tabs are skipped. Wildcard characters are allowed. Cubes may also be defined by a list of space separated cubes, 54 tile characters or names of defined cubes. The result is the logical AND of all cubes in the list. Moves are defined by one or more already defined moves, e.g. `F U R U' R' F'`. Plan actions are defined as 'precondition cube1 cube2 ... effect move1 move2 ...' There must be one or more cubes in the precondition, and 1 or more moves in the effect.

---

### help

**Usage:** `help command?`

**Description:** Displays a list of commands. If a command is given, only help on that command is displayed.

---

### run

**Usage:** `run filename`

**Description:** Runs the commands in filename as if they were typed. Turns off the prompt while reading the file.

---

### exit_if

**Usage:** `exit_if not_equal_goal`

**Description:** Stops execution if the condition holds.

---

### quit / exit

**Usage:** `quit` or `exit`

**Description:** Exits the program.

---

## Solution Display Formats

The Rubik's Cube solver supports two display formats for representing the solution, which can be set using the `config solution_display` command.

### human

This is the default format. It displays the solution in a human-readable format.

**Example:**

```
Found solution.
rotate U R F'
Cost: 3
Generated 123 nodes.
Maximum frontier size 456 nodes.
```

### machine

This format displays the solution in a machine-readable format.

**Example:**

```
search tree bfs T 123 456 3 rotate U R F'
```

---

## Move Display Formats

The Rubik's Cube solver supports two display formats for representing moves, which can be set using the `config move_display` command.

### name

This is the default format. It displays the name of the move.

**Example:**

```
U
```

### sub_moves

This format displays the sub-moves that make up the move.

**Example:**

```
U : U
```

---

## Cube Display Formats

The Rubik's Cube solver supports three display formats for representing the cube's state, which can be set using the `config cube_display` command.

### t

This is the default format. It provides a human-readable, multi-line representation of the cube, displaying each face sequentially. Each face is shown as a 3x3 grid of characters, where each character represents a facelet's color.

**Example (`show cube` command output for a solved cube):**

```
     www
     www
     www

ggg  rrr  bbb  ooo
ggg  rrr  bbb  ooo
ggg  rrr  bbb  ooo

     yyy
     yyy
     yyy
```

### one_word

This format represents the entire cube as a single 54-character string. Each face consists of 9 characters, representing its facelets.

**Example (`show cube` command output for a solved cube):**

```
wwwwwwwwwgggggggggrrrrrrrrrbbbbbbbbboooooooooyyyyyyyyy
```

### one_line

This format is similar to `one_word`, but the output is separated by spaces for each face.

**Example (`show cube` command output for a solved cube):**

```
wwwwwwwww ggggggggg rrrrrrrrr bbbbbbbbb ooooooooo yyyyyyyyy
```

---

## See Also

*   [README.md](../README.md) (Note: The `README.md` file may be out of date.)
