// main.c
#include <math.h>

#include "pendulum.h"
#include "sdl_visuals.h"
#include "arithmetic.h"

int main(void)
{
    // Pendulum physical parameters
    PendulumParams params;
    params.m1 = 1.0;
    params.m2 = 1.0;
    params.l1 = 1.0;          // meters
    params.l2 = 1.0;
    params.g  = 9.81;

    // How those lengths appear on screen
    params.l1_pixels = 200.0;
    params.l2_pixels = 200.0;

    // Initial state
    PendulumState state;
    state.theta1 = M_PI / 2.0;  // 90° from vertical
    state.theta2 = M_PI / 2.0;
    state.omega1 = 0.0;
    state.omega2 = 0.0;

    // Run the SDL window + simulation loop
    run_simulation(&params, &state);

    return 0;
}
