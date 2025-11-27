#include "../include/pendulum.h"
#include "../include/arithmetic.h"

// Initialize pendulum parameters and states
void init_pendulum(PendulumParams *params, PendulumState *state,
                   double m1, double m2, double l1, double l2, double g,
                   double initial_theta1, double initial_theta2) {
    
    // Set parameters
    params->m1 = m1;
    params->m2 = m2;
    params->l1 = l1;
    params->l2 = l2;
    params->g = g;

    // Set initial state
    state->theta1 = initial_theta1;
    state->theta2 = initial_theta2;
    state->omega1 = 0.0;
    state->omega2 = 0.0;
}

// Wrapper for the RK4 update (calls compute from arithmetic.h)
void update_pendulum(PendulumParams *params, PendulumState *state, double dt) {
    double new_theta1, new_theta2, new_omega1, new_omega2;

    compute(
        state->theta1, state->theta2,
        state->omega1, state->omega2,
        params->m1, params->m2,
        params->l1, params->l2,
        params->g, dt,
        &new_theta1, &new_theta2,
        &new_omega1, &new_omega2
    );

    state->theta1 = new_theta1;
    state->theta2 = new_theta2;
    state->omega1 = new_omega1;
    state->omega2 = new_omega2;
}