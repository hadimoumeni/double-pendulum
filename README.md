# Double Pendulum Simulation

Interactive double pendulum simulation written in C. Shows chaotic motion with real-time visualization.

## Authors

Ryann Mack, Ronan, Hadi, Raji

## Overview

This is a double pendulum simulator. Two pendulums connected together - the first one hangs from a fixed point, and the second one hangs from the end of the first. Even though it looks simple, tiny changes in starting position lead to completely different motion patterns.

We split the work into different parts:

- Physics calculations and integration (arithmetic.c)
- Pendulum state and coordinate stuff (pendulum.c)  
- Drawing and user input (sdl_visuals.c)
- UDP networking for sending data (sender/receiver.c)
- Tests to make sure the physics works (tests/)

## How It Works

The double pendulum follows these differential equations. We have four things that change over time: two angles (theta1, theta2) and two angular velocities (omega1, omega2). The physical setup has masses m1 and m2, rod lengths L1 and L2, and gravity g.

The acceleration equations come from Lagrangian mechanics:

```
delta = theta1 - theta2
denominator = 2*m1 + m2 - m2*cos(2*delta)

theta1_double_dot = [-g*(2*m1 + m2)*sin(theta1) 
                     - m2*g*sin(theta1 - 2*theta2)
                     - 2*sin(delta)*m2*(omega2^2*L2 + omega1^2*L1*cos(delta))]
                     / (L1 * denominator)

theta2_double_dot = [2*sin(delta) * (omega1^2*L1*(m1 + m2)
                     + g*(m1 + m2)*cos(theta1)
                     + omega2^2*L2*m2*cos(delta))]
                     / (L2 * denominator)
```

We use RK4 (Runge-Kutta 4th order) to solve these equations. It's more accurate than Euler and runs fast enough for real-time. The `compute()` function in arithmetic.c does one time step - it takes the current angles and velocities and gives you the new ones after dt seconds. We usually use dt = 0.01.

## Files

Main source files:
- `src/arithmetic.c` - physics equations and RK4 integration
- `src/pendulum.c` - pendulum state, initialization, screen coordinates
- `src/sdl_visuals.c` - SDL rendering, input handling, main loop
- `src/main.c` - starts everything up
- `src/sha256.c` - hashing for network stuff

Headers:
- `include/arithmetic.h` - compute() function declaration
- `include/pendulum.h` - Pendulum struct and functions
- `include/sdl_visuals.h` - screen size constants and run_simulation()
- `include/sha256.h` - hashing functions

Other:
- `sender/receiver.c` - UDP receiver program
- `tests/test_suite.c` - Unity tests

## Controls

**Mouse**: Click and drag the second bob to move it. Uses inverse kinematics to figure out the angles. Pauses while dragging.

**Keyboard**:
- W/S - change first pendulum velocity
- A/D - change second pendulum velocity  
- SPACE or P - play/pause
- R - reset to starting position
- ESC - quit

When paused, you'll see red and green lines showing the velocity directions.

## Visual Stuff

The pendulum draws with two rods and two bobs (circles). The second bob leaves a trail showing where it's been - we keep the last 500 positions in a circular buffer. There's also a grid in the background to help see the scale (1 meter spacing).

## Networking

There's UDP code for sending random numbers to other programs. Uses SHA-256 for checksums. The receiver program is in sender/receiver.c.

## Building

Uses CMake. Build it like this:

```bash
mkdir -p build
cd build
cmake ..
make
```

You'll get `build/src/main` for the simulation and `build/tests/run_tests` for tests.

**What you need**:
- CMake
- SDL2
- C compiler
- pthread (usually comes with the compiler)

On Mac:
```bash
brew install sdl2
```

On Linux:
```bash
sudo apt-get install libsdl2-dev
```

## Running

```bash
./build/src/main
```

Starts with both pendulums at 90 degrees. Drag it around or use the keyboard controls.

## Tests

We have Unity tests that check:
- The integration doesn't produce NaN or garbage values
- Energy stays roughly constant (conservative system)

Run them:
```bash
cd build
./tests/run_tests
```

The energy test runs the simulation for a while and makes sure total energy doesn't drift too much.

## Implementation Notes

Angles are measured from vertical (0 = straight down). Screen coordinates use a pixels-per-meter scale to convert from physics space to display.

The Pendulum struct holds everything: physical parameters, current state, color, and the trail buffer.

There's a thread-safe update function using pthread mutexes, though we're not really using multiple threads right now.

## Known Issues

No friction or air resistance - it's a perfect conservative system. Time step is fixed at 0.01s. Could add adaptive stepping later if needed.
