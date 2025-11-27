// sdl_visuals.c
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

#include "pendulum.h"
#include "sdl_visuals.h"
#include "renderer.h"
#include "arithmetic.h"   

void run_simulation(const PendulumParams *params, PendulumState *state)
{
    const int width = 800;
    const int height = 600;

    // --- SDL INIT ---
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return;
    }

    SDL_Window *window = SDL_CreateWindow(
        "Double Pendulum",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_SHOWN
    );
    if (!window)
    {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer)
    {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    // Time step in seconds (~60 FPS)
    double dt = 0.016;

    bool running = true;
    SDL_Event event;

    // Copy state locally so we can update
    PendulumState s = *state;
    PendulumParams p = *params;  // if you want to tweak inside later

    while (running)
    {
        // --- EVENTS ---
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    running = false;
                }
                // Reset with R
                if (event.key.keysym.sym == SDLK_r)
                {
                    s.theta1 = M_PI / 2.0;
                    s.theta2 = M_PI / 2.0;
                    s.omega1 = 0.0;
                    s.omega2 = 0.0;
                }
            }
        }

        // --- PHYSICS UPDATE (using compute from arithmetic.h) ---
        double new_theta1, new_theta2, new_omega1, new_omega2;

        compute(
            s.theta1, s.theta2,
            s.omega1, s.omega2,
            p.m1, p.m2,
            p.l1, p.l2,
            p.g, dt,
            &new_theta1, &new_theta2,
            &new_omega1, &new_omega2
        );

        s.theta1 = new_theta1;
        s.theta2 = new_theta2;
        s.omega1 = new_omega1;
        s.omega2 = new_omega2;

        // --- RENDER ---
        render_pendulum(renderer, width, height, &p, &s);

        SDL_Delay(16); // approx 60 FPS
    }

    // Save final state back to caller, if they care
    *state = s;

    // --- CLEANUP ---
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
