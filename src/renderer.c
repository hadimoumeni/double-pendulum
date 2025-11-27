// renderer.c
#include "renderer.h"
#include <math.h>

void render_pendulum(SDL_Renderer *renderer,
                     int width, int height,
                     const PendulumParams *params,
                     const PendulumState *state)
{
    // Clear background to black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Pivot point near top center
    int origin_x = width / 2;
    int origin_y = height / 4;

    // Shortcuts
    double theta1 = state->theta1;
    double theta2 = state->theta2;
    double l1_pixels = params->l1_pixels;
    double l2_pixels = params->l2_pixels;

    // First bob position
    int x1 = origin_x + (int)(l1_pixels * sin(theta1));
    int y1 = origin_y + (int)(l1_pixels * cos(theta1));

    // Second bob position
    int x2 = x1 + (int)(l2_pixels * sin(theta2));
    int y2 = y1 + (int)(l2_pixels * cos(theta2));

    // Draw rods and masses in white
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Rod 1
    SDL_RenderDrawLine(renderer, origin_x, origin_y, x1, y1);
    // Rod 2
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);

    // Mass 1 (small square)
    SDL_Rect m1 = { x1 - 6, y1 - 6, 12, 12 };
    SDL_RenderFillRect(renderer, &m1);

    // Mass 2 (small square)
    SDL_Rect m2 = { x2 - 6, y2 - 6, 12, 12 };
    SDL_RenderFillRect(renderer, &m2);

    // Present frame
    SDL_RenderPresent(renderer);
}
