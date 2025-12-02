# c-group-project

## Authors: Ryann Mack, Ronan, Hadi, Raji

# Double Pendulum Simulation — Arithmetic Module

## 🎯 Project Overview

This project simulates the motion of a **double pendulum** using C.
The system consists of two pendulums connected in series, producing **chaotic nonlinear motion**.

Our group divided the work into modules:

* Arithmetic / Physics Engine
* Rendering
* User input
* System integration

## 🧠 Mathematical Model

The double pendulum follows two coupled nonlinear differential equations.

Variables:

* θ1, θ2 — angles
* ω1, ω2 — angular velocities
* m1, m2 — masses
* L1, L2 — rod lengths
* g — gravity

Define:

δ = θ1 − θ2
den = 2*m1 + m2 − m2*cos(2*δ)

### Angular Accelerations

θ1¨ =
[ −g(2*m1 + m2) * sin(θ1)
− m2*g * sin(θ1 − 2*θ2)
− 2*sin(δ)*m2*(ω2²*L2 + ω1²*L1*cos(δ)) ]
/ (L1 * den)

θ2¨ =
[ 2*sin(δ) * ( ω1²*L1*(m1 + m2)
+ g*(m1 + m2)*cos(θ1)
+ ω2²*L2*m2*cos(δ) ) ]
/ (L2 * den)

These equations form the core physics engine.

---

## 🧮 Numerical Method — Euler Integration



Given a time step dt:

Velocity updates:
ω1_new = ω1 + θ1¨ * dt
ω2_new = ω2 + θ2¨ * dt

Angle updates:
θ1_new = θ1 + ω1_new * dt
θ2_new = θ2 + ω2_new * dt

This update repeats every frame.

---


### Arithmetic Engine

* The double pendulum equations
* The Euler integration algorithm
* A `compute()` function that updates angles and velocities each step
* All trigonometric math (sin, cos, etc.)

The renderer uses these updated angles to draw the pendulum.

## 🧪 Testing and Validation (QA)

This project includes an automated testing suite using the **Unity** framework to validate the physics engine.

### How to Run Tests
To execute the test suite, run the following commands from the project root:

```bash
mkdir -p build && cd build
cmake ..
make
./tests/run_tests
```
### Validation Strategy
*   **Integration Stability:** The tests ensure the RK4 solver produces valid floating-point numbers (checking for NaN/Infinity) and correctly updates the system state during single steps.
*   **Physics Validation:** We validate the **Law of Conservation of Energy**. Since the double pendulum is a conservative system (no friction), the Total Energy (Hamiltonian) must remain constant.
*   **Results:** Our automated tests simulate 1000 time steps and assert that the total energy (Kinetic + Potential) remains constant within a 5% margin of error. This confirms that the chaotic motion observed is physically accurate and not a result of numerical instability..