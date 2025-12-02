# Double Pendulum Simulation

A real-time interactive simulation of a double pendulum system implemented in C. This project demonstrates chaotic dynamics through a physics-accurate simulation with visual feedback and user interaction capabilities.

## Authors

Ryann Mack, Ronan, Hadi, Raji

## Project Overview

This simulation models a double pendulum, a classic example of a chaotic system in physics. The system consists of two pendulums connected in series, where the first pendulum is attached to a fixed pivot point and the second pendulum is attached to the end of the first. Despite its simple construction, the double pendulum exhibits highly sensitive behavior where small changes in initial conditions lead to dramatically different trajectories over time.

The project is divided into several modules that handle different aspects of the simulation:

- **Physics Engine**: Implements the mathematical model and numerical integration
- **State Management**: Handles pendulum state, initialization, and coordinate transformations
- **Visualization**: Renders the simulation using SDL2 with interactive controls
- **Networking**: Provides UDP communication for data sharing between instances
- **Testing**: Automated test suite for validating physics accuracy

## Mathematical Model

The double pendulum is governed by a system of coupled nonlinear differential equations. The system has four state variables: two angles (theta1, theta2) and two angular velocities (omega1, omega2). The physical parameters include the masses of both bobs (m1, m2), the lengths of both rods (l1, l2), and gravitational acceleration (g).

The angular accelerations are computed from the Lagrangian formulation of the system. Defining delta as the difference between the two angles, the equations are:

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

These equations capture the complex interactions between the two pendulums, including gravitational forces, centripetal effects, and the coupling between the two masses.

## Numerical Integration

The simulation uses the fourth-order Runge-Kutta (RK4) method to integrate the equations of motion. This method provides a good balance between accuracy and computational efficiency for this type of system. RK4 evaluates the system at four intermediate points within each time step and combines them using a weighted average to produce the final state update.

The integration is performed in the `compute()` function in `arithmetic.c`, which takes the current state and physical parameters and returns the new state after one time step. The time step size (dt) is typically set to 0.01 seconds, which provides smooth animation while maintaining numerical stability.

## Project Structure

The codebase is organized into several modules:

### Source Files

- `src/arithmetic.c`: Contains the physics engine with the acceleration calculations and RK4 integration
- `src/pendulum.c`: Manages the pendulum state structure, initialization, and coordinate transformations
- `src/sdl_visuals.c`: Handles all SDL2 rendering, user input, and the main simulation loop
- `src/main.c`: Entry point that initializes a pendulum and starts the simulation
- `src/sha256.c`: SHA-256 hashing implementation for network data integrity

### Header Files

- `include/arithmetic.h`: Interface for the physics computation function
- `include/pendulum.h`: Definition of the Pendulum structure and state management functions
- `include/sdl_visuals.h`: Constants and function declarations for the visualization system
- `include/sha256.h`: SHA-256 hashing interface

### Additional Components

- `sender/receiver.c`: Standalone UDP receiver program for receiving random numbers from the simulation
- `tests/test_suite.c`: Automated test suite using the Unity testing framework

## Features

### Interactive Controls

The simulation provides several ways to interact with the pendulum:

- **Mouse Drag**: Click and drag the second bob to position the pendulum. The system uses inverse kinematics to calculate the required angles. The simulation pauses automatically during dragging.

- **Keyboard Controls**:
  - W/S: Increase/decrease the angular velocity of the first pendulum
  - A/D: Decrease/increase the angular velocity of the second pendulum
  - SPACE or P: Toggle play/pause
  - R: Reset the pendulum to its initial position
  - ESC: Quit the simulation

- **Visual Feedback**: When paused, red and green lines indicate the direction and magnitude of the angular velocities for each pendulum.

### Visual Elements

The visualization includes:

- **Pendulum Rendering**: Two rods connecting the pivot point to the first bob, and from the first bob to the second bob. The bobs are rendered as filled circles with customizable colors.

- **Trail System**: The path of the second bob is traced with a fading trail that shows the last 500 positions. The trail uses a circular buffer to efficiently manage memory while providing visual feedback about the pendulum's motion.

- **Grid Overlay**: A background grid helps visualize the scale and motion of the system. The grid spacing corresponds to one meter in the physical simulation.

### Networking

The simulation includes UDP networking capabilities for sending random numbers to other processes. This feature uses SHA-256 hashing for data integrity. A separate receiver program is provided in the `sender/` directory that can listen for these UDP messages.

## Building the Project

The project uses CMake for build configuration. To build:

```bash
mkdir -p build
cd build
cmake ..
make
```

This will create the main simulation executable in `build/src/main` and the test executable in `build/tests/run_tests`.

### Dependencies

- CMake 3.21 or later
- SDL2 development libraries
- C compiler with C11 standard support
- pthread library (for thread-safe pendulum updates)

On macOS, SDL2 can be installed via Homebrew:
```bash
brew install sdl2
```

On Linux, install the SDL2 development package:
```bash
sudo apt-get install libsdl2-dev  # Debian/Ubuntu
```

## Running the Simulation

After building, run the simulation with:

```bash
./build/src/main
```

The simulation window will open with a double pendulum initialized at 90 degrees for both angles. You can immediately start interacting with it using the mouse or keyboard controls.

## Testing

The project includes an automated test suite using the Unity testing framework. The tests validate the physics engine by checking:

- **Integration Stability**: Verifies that the RK4 solver produces valid floating-point numbers and correctly updates the system state
- **Energy Conservation**: Validates that the total energy (kinetic plus potential) remains constant within acceptable tolerances, confirming the simulation maintains physical accuracy

To run the tests:

```bash
cd build
./tests/run_tests
```

The test suite simulates the pendulum for extended periods and verifies that energy conservation holds, which is a critical property of conservative systems like the double pendulum.

## Technical Details

### Coordinate System

The simulation uses a coordinate system where angles are measured from the vertical, with zero representing the downward position. The screen coordinates are calculated from the physical coordinates using a scaling factor (pixels per meter) to map the simulation space to the display.

### State Management

The pendulum state is stored in a structure that includes all physical parameters (masses, lengths, gravity) and dynamic state variables (angles and angular velocities). The structure also maintains visual properties like color and the trail history for rendering.

### Thread Safety

The code includes thread-safe versions of the update functions using pthread mutexes, allowing the simulation to be safely updated from multiple threads if needed.

## Limitations and Future Work

The current implementation assumes no friction or air resistance, making it a purely conservative system. The simulation also uses a fixed time step, which could be extended to adaptive time stepping for better accuracy in extreme conditions.

Potential enhancements could include:
- Multiple pendulum instances running simultaneously
- Configurable physical parameters through a configuration file
- Data logging and export capabilities
- Performance optimizations for real-time rendering at higher frame rates
