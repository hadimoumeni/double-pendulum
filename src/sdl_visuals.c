#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

#include "../include/sdl_visuals.h"
#include "../include/pendulum.h"

// --- Global SDL Pointers ---
static SDL_Window *gWindow = NULL;
static SDL_Renderer *gRenderer = NULL;
static const double PIX_PER_M = PIXELS_PER_METER; // Shorter alias for scale

// Helper Functions

// Draws a filled circle at screen coordinates (cx, cy)
static void draw_filled_circle(SDL_Renderer *rend, int cx, int cy, int radius) {
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            // Calculate offsets from the circle center
            int dx = radius - w; 
            int dy = radius - h; 
            
            // Check if the point (dx, dy) is inside the circle (r^2 = x^2 + y^2)
            if ((dx * dx + dy * dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(rend, cx + dx, cy + dy);
            }
        }
    }
}

// Initializes SDL and creates window/renderer
static bool init_sdl() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return false;
    }

    gWindow = SDL_CreateWindow("Double Pendulum Simulator (Click to Set Blue Bob)",
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!gWindow) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        return false;
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!gRenderer) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

// Cleans up SDL resources
static void close_sdl() {
    if (gRenderer) SDL_DestroyRenderer(gRenderer);
    if (gWindow) SDL_DestroyWindow(gWindow);
    SDL_Quit();
}

// Renders the current state of the pendulum
static void render_pendulum(const PendulumParams *params, const PendulumState *state) {
    // 1. Clear Screen
    SDL_SetRenderDrawColor(gRenderer, 30, 30, 30, 255); // Dark background
    SDL_RenderClear(gRenderer);

    // Pivot location in screen coordinates (pixels)
    const int pivot_x = SCREEN_WIDTH / 2;
    const int pivot_y = SCREEN_HEIGHT / 3;

    // --- Forward Kinematics (Physics to Screen Coordinates) ---
    // Physical coordinates are relative to the pivot.
    // X = L * sin(theta), Y = L * cos(theta) (because theta=0 is vertical down)

    // First bob (m1) position
    int x1 = pivot_x + (int)(params->l1 * sin(state->theta1) * PIX_PER_M);
    int y1 = pivot_y + (int)(params->l1 * cos(state->theta1) * PIX_PER_M);

    // Second bob (m2) position
    int x2 = x1 + (int)(params->l2 * sin(state->theta2) * PIX_PER_M);
    int y2 = y1 + (int)(params->l2 * cos(state->theta2) * PIX_PER_M);

    // --- 2. Draw Rods and Bobs ---

    // Draw Rod 1 (Pivot to m1)
    SDL_SetRenderDrawColor(gRenderer, 200, 200, 200, 255); // Light Gray
    SDL_RenderDrawLine(gRenderer, pivot_x, pivot_y, x1, y1);
    
    // Draw Rod 2 (m1 to m2)
    SDL_RenderDrawLine(gRenderer, x1, y1, x2, y2);

    // Draw Bob 1 (Red)
    SDL_SetRenderDrawColor(gRenderer, 255, 100, 100, 255); 
    draw_filled_circle(gRenderer, x1, y1, 10);

    // Draw Bob 2 (Blue)
    SDL_SetRenderDrawColor(gRenderer, 100, 100, 255, 255); 
    draw_filled_circle(gRenderer, x2, y2, 10);

    // Draw Pivot (White dot)
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    draw_filled_circle(gRenderer, pivot_x, pivot_y, 5);

    // 3. Present Frame
    SDL_RenderPresent(gRenderer);
}

// Handles user input (Quit, Reset, Mouse Click)
static bool handle_input(PendulumParams *params, PendulumState *state) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || 
            (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
            return false; // Exit condition
        }
        
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_r) {
            // Reset to default initial state (90 degrees, 0 velocity)
            init_pendulum(params, state,
                          params->m1, params->m2,
                          params->l1, params->l2,
                          params->g, M_PI / 2.0, M_PI / 2.0);
        }
        
        // --- Inverse Kinematics (IK) for Mouse Click (Set Blue Bob) ---
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            
            // 1. Convert mouse coords to physical meters (relative to pivot)
            const int pivot_x = SCREEN_WIDTH / 2;
            const int pivot_y = SCREEN_HEIGHT / 3;
            
            // Target coordinates in meters (X and Y are relative to the pivot)
            double target_X = (event.button.x - pivot_x) / PIX_PER_M;
            double target_Y = (event.button.y - pivot_y) / PIX_PER_M;

            double L1 = params->l1;
            double L2 = params->l2;

            // 2. Calculate Distance (D) from pivot to target
            double D_sq = target_X * target_X + target_Y * target_Y;
            double D = sqrt(D_sq);

            // 3. Check Reachability
            if (D > L1 + L2 || D < fabs(L1 - L2)) {
                printf("Target unreachable (D=%.2f). Max Range: %.2f\n", D, L1 + L2);
                continue; 
            }

            // 4. Solve IK for theta1 (Angle of first rod from vertical)
            // Use Law of Cosines to find alpha (angle at the pivot)
            // L2^2 = L1^2 + D^2 - 2*L1*D*cos(alpha)
            double cos_alpha = (L1*L1 + D_sq - L2*L2) / (2 * L1 * D);
            
            // Clamp value to [-1, 1] for acos safety
            cos_alpha = fmax(-1.0, fmin(1.0, cos_alpha));
            double alpha = acos(cos_alpha); 
            
            // Angle of the target vector from the vertical (y-axis)
            double beta = atan2(target_X, target_Y); 
            
            // Elbow-down solution for theta1
            double new_th1 = beta - alpha;

            // 5. Solve IK for theta2 (Angle of second rod from vertical)
            // Calculate the position of the first bob (m1)
            double x1_calc = L1 * sin(new_th1);
            double y1_calc = L1 * cos(new_th1);

            // Vector from m1 to the target m2
            double dx_m1_m2 = target_X - x1_calc;
            double dy_m1_m2 = target_Y - y1_calc;

            // theta2 is the angle of this vector from the vertical
            double new_th2 = atan2(dx_m1_m2, dy_m1_m2);


            // --- 6. Set Final State ---
            state->theta1 = new_th1;
            state->theta2 = new_th2;
            
            // Must reset velocities to start from rest
            state->omega1 = 0.0;
            state->omega2 = 0.0;
        }
    }
    return true;
}

// Main Simulation Loop
void run_simulation(PendulumParams *params, PendulumState *state) {
    if (!init_sdl()) {
        close_sdl();
        return;
    }

    const double PHYS_STEP = 0.01; // Fixed time step for physics (e.g., 100 Hz)
    bool running = true;
    Uint32 last_time = SDL_GetTicks();
    double accumulator = 0.0; // Time remaining for physics updates

    while (running) {
        // --- 1. Frame Timing ---
        Uint32 current_time = SDL_GetTicks();
        double frame_time = (current_time - last_time) / 1000.0; // Convert to seconds
        last_time = current_time;

        // Cap frame time to prevent errors during lag spikes
        if (frame_time > 0.25) frame_time = 0.25;

        accumulator += frame_time;

        // --- 2. Input ---
        running = handle_input(params, state);
        if (!running) break;

        // --- 3. Physics Update (Fixed Timestep Loop) ---
        // Ensures physics runs at a constant rate regardless of frame rate
        while (accumulator >= PHYS_STEP) {
            update_pendulum(params, state, PHYS_STEP);
            accumulator -= PHYS_STEP;
        }

        // --- 4. Render ---
        render_pendulum(params, state);
    }

    close_sdl();
}