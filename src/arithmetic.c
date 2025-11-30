#include <math.h>
#include "../include/arithmetic.h"


static void accelerations(
    double theta1, double theta2, // angles
    double omega1, double omega2,  // angular velocities
    double m1, double m2, // masses
    double L1, double L2, // rod lengths
    double g, // gravitational acceleration
    double *theta1_dd, double *theta2_dd // angular accelerations
) {
    double delta = theta1 - theta2;
    // NOTE: This term is simplified. The original denominator should be a function of theta1, theta2, m1, m2.
    // The provided equation is standard for the double pendulum.
    double den = 2*m1 + m2 - m2 * cos(2 * delta); 

    *theta1_dd =
        (-g * (2*m1 + m2) * sin(theta1)
        - m2 * g * sin(theta1 - 2 * theta2)
        - 2 * sin(delta) * m2 *
          (omega2*omega2*L2 + omega1*omega1*L1*cos(delta)))
        / (L1 * den);

    *theta2_dd =
        (2 * sin(delta) *
         (omega1*omega1*L1*(m1 + m2)
        + g*(m1 + m2)*cos(theta1)
        + omega2*omega2*L2*m2*cos(delta)))
        / (L2 * den);
}


void compute(
    double theta1, double theta2,
    double omega1, double omega2,
    double m1, double m2,
    double L1, double L2,
    double g, double dt,
    double *new_theta1, double *new_theta2,
    double *new_omega1, double *new_omega2
) {
    // --- k1 ---
    double k1_omega1, k1_omega2, k1_theta1, k1_theta2;
    accelerations(theta1, theta2, omega1, omega2,
                  m1, m2, L1, L2, g,
                  &k1_omega1, &k1_omega2);
    k1_theta1 = omega1;
    k1_theta2 = omega2;

    // --- k2 ---
    double th1_2 = theta1 + 0.5 * dt * k1_theta1;
    double th2_2 = theta2 + 0.5 * dt * k1_theta2;
    double om1_2 = omega1 + 0.5 * dt * k1_omega1;
    double om2_2 = omega2 + 0.5 * dt * k1_omega2;

    double k2_omega1, k2_omega2, k2_theta1, k2_theta2;
    accelerations(th1_2, th2_2, om1_2, om2_2,
                  m1, m2, L1, L2, g,
                  &k2_omega1, &k2_omega2);
    k2_theta1 = om1_2;
    k2_theta2 = om2_2;

    // --- k3 ---
    double th1_3 = theta1 + 0.5 * dt * k2_theta1;
    double th2_3 = theta2 + 0.5 * dt * k2_theta2;
    double om1_3 = omega1 + 0.5 * dt * k2_omega1;
    double om2_3 = omega2 + 0.5 * dt * k2_omega2;

    double k3_omega1, k3_omega2, k3_theta1, k3_theta2;
    accelerations(th1_3, th2_3, om1_3, om2_3,
                  m1, m2, L1, L2, g,
                  &k3_omega1, &k3_omega2);
    k3_theta1 = om1_3;
    k3_theta2 = om2_3;

    // --- k4 ---
    double th1_4 = theta1 + dt * k3_theta1;
    double th2_4 = theta2 + dt * k3_theta2;
    double om1_4 = omega1 + dt * k3_omega1;
    double om2_4 = omega2 + dt * k3_omega2;

    double k4_omega1, k4_omega2, k4_theta1, k4_theta2;
    accelerations(th1_4, th2_4, om1_4, om2_4,
                  m1, m2, L1, L2, g,
                  &k4_omega1, &k4_omega2);
    k4_theta1 = om1_4;
    k4_theta2 = om2_4;


//  Final RK4 weighted average update

    *new_theta1 = theta1 + (dt / 6.0) *
        (k1_theta1 + 2*k2_theta1 + 2*k3_theta1 + k4_theta1);

    *new_theta2 = theta2 + (dt / 6.0) *
        (k1_theta2 + 2*k2_theta2 + 2*k3_theta2 + k4_theta2);

    *new_omega1 = omega1 + (dt / 6.0) *
        (k1_omega1 + 2*k2_omega1 + 2*k3_omega1 + k4_omega1);

    *new_omega2 = omega2 + (dt / 6.0) *
        (k1_omega2 + 2*k2_omega2 + 2*k3_omega2 + k4_omega2);
}
