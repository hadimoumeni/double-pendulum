#ifndef ARITHMETIC_H
#define ARITHMETIC_H

// Computes the next state of the double pendulum using RK4 integration.
void compute(
    double theta1,
    double theta2,
    double omega1,
    double omega2,
    double m1,
    double m2,
    double l1,
    double l2,
    double g,
    double dt,
    double *new_theta1,
    double *new_theta2,
    double *new_omega1,
    double *new_omega2
);

#endif // ARITHMETIC_H