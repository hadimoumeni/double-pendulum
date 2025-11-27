#ifndef PHYSICS_H
#define PHYSICS_H

typedef struct {
    double theta1;      // Angle of first pendulum
    double theta2;      // Angle of second pendulum
    double omega1;      // Angular velocity of first pendulum
    double omega2;      // Angular velocity of second pendulum
    double m1;          // Mass of first pendulum bob
    double m2;          // Mass of second pendulum bob
    double l1;          // Length of first pendulum rod
    double l2;          // Length of second pendulum rod
    double g;           // Acceleration due to gravity
} DoublePendulum;

/**
 * Initialize the double pendulum with given parameters.
 */
void init_double_pendulum(DoublePendulum *p, double m1, double m2, double l1, double l2, double theta1, double theta2);

/**
 * Update the state of the double pendulum by delta_time.
 * Uses 4th order Runge-Kutta or simple Euler method (Euler is sufficient for visualization usually, but RK4 is better).
 * Let's stick to a reasonably accurate approximation.
 */
void update_double_pendulum(DoublePendulum *p, double dt);

#endif // PHYSICS_H

