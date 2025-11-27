// sdl_visuals.h
#ifndef SDL_VISUALS_H
#define SDL_VISUALS_H

#include "pendulum.h"

// Run the SDL window + simulation loop.
// This will update `state` over time until the user quits.
void run_simulation(const PendulumParams *params, PendulumState *state);

#endif