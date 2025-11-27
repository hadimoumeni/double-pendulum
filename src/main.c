#include "physics.h"
#include "visualizer.h"
#include <SDL2/SDL.h>
#include <math.h>

#define PHYSICS_TIME_STEP 0.01

int main(int argc, char *argv[]) {

    DoublePendulum pendulum;
    // m1=1.0, m2=1.0, l1=1.5, l2=1.5, theta1=PI/2, theta2=PI/2
    init_double_pendulum(&pendulum, 1.0, 1.0, 1.5, 1.5, M_PI / 2, M_PI / 2);


    if (!init_visualizer(800, 600)) {
        return 1;
    }

    // Main Loop
    bool running = true;
    Uint32 last_time = SDL_GetTicks();
    double accumulator = 0.0;

    while (running) {
        Uint32 current_time = SDL_GetTicks();
        double frame_time = (current_time - last_time) / 1000.0;
        last_time = current_time;

        // Cap frame time to avoid spiral of death DOOOOOOOOOOOMMMM
        if (frame_time > 0.25) frame_time = 0.25;

        accumulator += frame_time;

        running = handle_input(&pendulum);

        while (accumulator >= PHYSICS_TIME_STEP) {
            update_double_pendulum(&pendulum, PHYSICS_TIME_STEP);
            accumulator -= PHYSICS_TIME_STEP;
        }

        render_pendulum(&pendulum);
    }

    close_visualizer();
    return 0;
}

