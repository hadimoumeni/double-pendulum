# c-group-project

## Authors: Ryann Mack, Ronan, Hadi, Raji

# Double Pendulum Simulation — Arithmetic Module

## 🎯 Project Overview

This project simulates the motion of a **double pendulum** using C.
The system consists of two pendulums connected in series, producing **chaotic nonlinear motion**.

Our group divided the work into modules:

* Arithmetic / Physics Engine (my part)
* Rendering
* User input
* System integration

This README explains the **math model** and **numerical method** used in the arithmetic engine.

---

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

We approximate the motion using the **Euler method**.

Given a time step dt:

Velocity updates:
ω1_new = ω1 + θ1¨ * dt
ω2_new = ω2 + θ2¨ * dt

Angle updates:
θ1_new = θ1 + ω1_new * dt
θ2_new = θ2 + ω2_new * dt

This update repeats every frame.

---

## 🧩 Module Responsibilities

### ✔️ My Contribution (Arithmetic Engine)

Raji implemented:

* The double pendulum equations
* The Euler integration algorithm
* A `compute()` function that updates angles and velocities each step
* All trigonometric math (sin, cos, etc.)

The renderer uses these updated angles to draw the pendulum.


