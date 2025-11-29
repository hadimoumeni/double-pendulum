// sdl_visuals.h
#ifndef SDL_VISUALS_H
#define SDL_VISUALS_H

#include <stdbool.h>
#include "../include/pendulum.h"

// Constants for Visuals 
#define SCREEN_WIDTH 1000 
#define SCREEN_HEIGHT 750
#define PIXELS_PER_METER 150.0 // Scaling factor (150 pixels per meter)
#define GRID_SPACING 1.0       // Grid lines every 1.0 meter (150 pixels)

// --- Main Simulation Function ---

// Runs the simulation loop for a single pendulum.
void run_simulation(Pendulum *p);

#endif // SDL_VISUALS_H