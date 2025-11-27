#ifndef PENDULUM_H
#define PENDULUM_H

// --- Constants ---
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// --- Structs for Physics ---

// Physical parameters of the double pendulum
typedef struct {
    double m1;          // Mass 1 (kg)
    double m2;          // Mass 2 (kg)
    double l1;          // Length 1 (meters)
    double l2;          // Length 2 (meters)
    double g;           // Gravity (m/s^2)
} PendulumParams;

// Dynamic state of the double pendulum
typedef struct {
    double theta1;      // Angle of first rod (rad, 0 is vertical down)
    double theta2;      // Angle of second rod (rad, 0 is vertical down)
    double omega1;      // Angular velocity 1 (rad/s)
    double omega2;      // Angular velocity 2 (rad/s)
} PendulumState;

// --- Function Prototypes ---

// Initialize pendulum parameters and state
void init_pendulum(PendulumParams *params, PendulumState *state,
                   double m1, double m2, double l1, double l2, double g,
                   double initial_theta1, double initial_theta2);

// Wrapper for the RK4 update (calls compute from arithmetic.h)
void update_pendulum(PendulumParams *params, PendulumState *state, double dt);

#endif // PENDULUM_H