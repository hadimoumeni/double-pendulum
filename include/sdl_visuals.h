// sdl_visuals.h
#ifndef SDL_VISUALS_H
#define SDL_VISUALS_H

#include <stdbool.h>
#include "pendulum.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 750
#define PIXELS_PER_METER 150.0 
#define GRID_SPACING 1.0     


void run_simulation(Pendulum *p);

#endif