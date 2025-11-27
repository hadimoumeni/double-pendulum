// renderer.h
#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include "pendulum.h"

// Draw the double pendulum for given state and parameters
void render_pendulum(SDL_Renderer *renderer,
                     int width, int height,
                     const PendulumParams *params,
                     const PendulumState *state);

#endif