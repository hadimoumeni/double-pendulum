#include "physics.h"
#include <math.h>

void init_double_pendulum(DoublePendulum *p, double m1, double m2, double l1, double l2, double theta1, double theta2) {
    p->m1 = m1;
    p->m2 = m2;
    p->l1 = l1;
    p->l2 = l2;
    p->theta1 = theta1;
    p->theta2 = theta2;
    p->omega1 = 0.0;
    p->omega2 = 0.0;
    p->g = 9.81; // Standard gravity
}

// Helper to calculate angular accelerations
static void calculate_accelerations(const DoublePendulum *p, double *alpha1, double *alpha2) {
    double mu = 1 + p->m1 / p->m2;
    double dtheta = p->theta1 - p->theta2;
    double s_dtheta = sin(dtheta);
    double c_dtheta = cos(dtheta);
    
    // Equations of motion for double pendulum
    // Numerator for alpha1
    double num1 = p->g * (sin(p->theta2) * c_dtheta - mu * sin(p->theta1)) 
                - (p->l2 * p->omega2 * p->omega2 + p->l1 * p->omega1 * p->omega1 * c_dtheta) * s_dtheta;
    double den1 = p->l1 * (mu - c_dtheta * c_dtheta);
    
    *alpha1 = num1 / den1;

    // Numerator for alpha2
    double num2 = p->g * mu * (sin(p->theta1) * c_dtheta - sin(p->theta2)) 
                + (mu * p->l1 * p->omega1 * p->omega1 + p->l2 * p->omega2 * p->omega2 * c_dtheta) * s_dtheta;
    double den2 = p->l2 * (mu - c_dtheta * c_dtheta);
    
    *alpha2 = num2 / den2;
}

void update_double_pendulum(DoublePendulum *p, double dt) {
    // Using simple semi-implicit Euler for stability or RK4. 
    // Given the chaotic nature, RK4 is better, but for a visualizer, semi-implicit Euler often works well enough if dt is small.
    // However, the equations are coupled second-order ODEs.
    // Let's implement basic Euler integration for simplicity first, or Runge-Kutta if needed.
    // The previous acceleration calculation is correct.
    
    double alpha1, alpha2;
    calculate_accelerations(p, &alpha1, &alpha2);

    p->omega1 += alpha1 * dt;
    p->omega2 += alpha2 * dt;
    
    p->theta1 += p->omega1 * dt;
    p->theta2 += p->omega2 * dt;

    // Damping (optional, to keep it from spinning forever if we want)
    // p->omega1 *= 0.999;
    // p->omega2 *= 0.999;
}

