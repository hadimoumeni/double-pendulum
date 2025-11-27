#include "visualizer.h"
#include <SDL2/SDL.h>
#include <math.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static int screen_width = 800;
static int screen_height = 600;
static const double SCALE = 100.0; // Pixels per meter (assuming lengths are around 1-2m)

bool init_visualizer(int width, int height) {
    screen_width = width;
    screen_height = height;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return false;
    }

    window = SDL_CreateWindow("Double Pendulum Visualizer",
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              width, height, SDL_WINDOW_SHOWN);
    if (!window) return false;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) return false;

    return true;
}

static void draw_filled_circle(SDL_Renderer *rend, int cx, int cy, int radius) {
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w; // horizontal offset
            int dy = radius - h; // vertical offset
            if ((dx*dx + dy*dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(rend, cx + dx, cy + dy);
            }
        }
    }
}

void render_pendulum(const DoublePendulum *p) {
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255); // Dark background
    SDL_RenderClear(renderer);

    int center_x = screen_width / 2;
    int center_y = screen_height / 3;

    // Calculate positions
    // x1 = l1 * sin(theta1)
    // y1 = l1 * cos(theta1)
    int x1 = center_x + (int)(p->l1 * sin(p->theta1) * SCALE);
    int y1 = center_y + (int)(p->l1 * cos(p->theta1) * SCALE);

    // x2 = x1 + l2 * sin(theta2)
    // y2 = y1 + l2 * cos(theta2)
    int x2 = x1 + (int)(p->l2 * sin(p->theta2) * SCALE);
    int y2 = y1 + (int)(p->l2 * cos(p->theta2) * SCALE);

    // Draw first arm
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderDrawLine(renderer, center_x, center_y, x1, y1);
    
    // Draw first bob
    SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);
    draw_filled_circle(renderer, x1, y1, 10);

    // Draw second arm
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);

    // Draw second bob
    SDL_SetRenderDrawColor(renderer, 100, 100, 255, 255);
    draw_filled_circle(renderer, x2, y2, 10);

    // Draw pivot
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    draw_filled_circle(renderer, center_x, center_y, 5);

    SDL_RenderPresent(renderer);
}

bool handle_input(DoublePendulum *p) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            return false;
        }
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                return false;
            }
            if (event.key.keysym.sym == SDLK_r) {
                // Reset
                p->theta1 = M_PI / 2;
                p->theta2 = M_PI / 2;
                p->omega1 = 0;
                p->omega2 = 0;
            }
        }
    }
    return true;
}

void close_visualizer() {
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}

