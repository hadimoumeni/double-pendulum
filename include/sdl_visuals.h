// sdl_visuals.h
#ifndef SDL_VISUALS_H
#define SDL_VISUALS_H

#include <stdbool.h>
#include "../include/pendulum.h"

// Constants for Visuals 
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define PIXELS_PER_METER 150.0 // Scaling factor

// --- Main Simulation Function ---

// Sets up SDL, runs the main loop (input, update, render), and cleans up.
void run_simulation(PendulumParams *params, PendulumState *state);

#endif // VISUALIZER_H