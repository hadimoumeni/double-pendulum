// include/pendulum.h
#ifndef PENDULUM_H
#define PENDULUM_H

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct {
    double m1, m2;      // masses
    double l1, l2;      // rod lengths (physics units)
    double g;           // gravity
    double l1_pixels;   // screen length in pixels for rod 1
    double l2_pixels;   // screen length in pixels for rod 2
} PendulumParams;

typedef struct {
    double theta1, theta2; // angles (radians)
    double omega1, omega2; // angular velocities
} PendulumState;

#endif // PENDULUM_H