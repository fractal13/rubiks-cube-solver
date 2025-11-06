# Rubik's Cube Solver

This project provides a C++ application for solving a 3x3x3 Rubik's Cube. It features a robust cube representation, a comprehensive set of cube manipulation moves, and a solver capable of operating in both interactive and pipe-based modes. The solver implements classic search algorithms to find solutions for scrambled cubes and supports custom goal states for partial solutions.

## Building the Project

To build the Rubik's Cube solver, navigate to the project root directory and run `make`:

```bash
make
```

This will first build the `ai-lib` dependency and then compile the Rubik's Cube solver, creating the executable `rubiks-cube-solver` in the `bin/` directory.

## Example Session

This example demonstrates how to configure the solver, set up a checkerboard pattern as a goal, and use an Iterative Deepening Search (IDS) algorithm to find a solution.

```
cube> init cube solved
cube> config apply_solution enable
cube> config solution_display machine
cube> config cube_display t
cube> moves R2 L2 F2 B2 U2 D2
cube> init goal wywywywywgbgbgbgbgrororororbgbgbgbgbororororoywywywywy
cube> show cube
Cube: 
     www
     www
     www

ggg  rrr  bbb  ooo
ggg  rrr  bbb  ooo
ggg  rrr  bbb  ooo

     yyy
     yyy
     yyy

cube> show goal
Goal:  
     wyw
     ywy
     wyw

gbg  ror  bgb  oro
bgb  oro  gbg  ror
gbg  ror  bgb  oro

     ywy
     wyw
     ywy

cube> config ids_limit 10
cube> search tree ids
search tree ids T 19026 31 12 rotate B B F F L L R R D D U U
cube> show cube
Cube: 
     wyw
     ywy
     wyw

gbg  ror  bgb  oro
bgb  oro  gbg  ror
gbg  ror  bgb  oro

     ywy
     wyw
     ywy

```

## Manual Rotations: 6 Dots Pattern

This example demonstrates how to manually apply a sequence of rotations to achieve the "6 dots" pattern.

```
cube> init cube solved
cube> config cube_display t
cube> show cube
Cube: 
     www
     www
     www

ggg  rrr  bbb  ooo
ggg  rrr  bbb  ooo
ggg  rrr  bbb  ooo

     yyy
     yyy
     yyy

cube> rotate U
cube> rotate D'
cube> rotate R
cube> show cube
Cube: 
     wwb
     wwr
     wwb

rrr  bby  obo  wgg
ggg  rry  obo  woo
rrr  bby  obo  wgg

     yyg
     yyo
     yyg

cube> rotate L'
cube> rotate F
cube> rotate B'
cube> show cube
Cube: 
     rrr
     rwr
     rrr

ggg  yyy  bbb  www
ygy  brb  wbw  gog
ggg  yyy  bbb  www

     ooo
     oyo
     ooo

cube> rotate U
cube> rotate D'
cube> show cube
Cube: 
     rrr
     rwr
     rrr

yyy  bbb  www  ggg
ygy  brb  wbw  gog
yyy  bbb  www  ggg

     ooo
     oyo
     ooo

```
