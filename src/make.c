#include <SDL2/SDL.h>
#include <stdbool.h>
#include <math.h>

// Pendulum structure
typedef struct {
    double angle;           // Current angle in radians
    double angular_velocity; // Angular velocity
    double length;          // Length of pendulum string
    double gravity;         // Gravity constant
    double damping;         // Damping factor (friction)
    int pivot_x;            // Pivot point x coordinate
    int pivot_y;            // Pivot point y coordinate
    int bob_radius;         // Radius of the bob (weight)
} Pendulum;

// Function to draw a filled circle
void draw_filled_circle(SDL_Renderer *renderer, int center_x, int center_y, int radius)
{
    for (int x = -radius; x <= radius; x++)
    {
        for (int y = -radius; y <= radius; y++)
        {
            if (x * x + y * y <= radius * radius)
            {
                SDL_RenderDrawPoint(renderer, center_x + x, center_y + y);
            }
        }
    }
}

// Function to draw an outlined circle
void draw_circle(SDL_Renderer *renderer, int center_x, int center_y, int radius)
{
    for (int angle = 0; angle < 360; angle++)
    {
        double radian = angle * M_PI / 180.0;
        int x = center_x + (int)(radius * cos(radian));
        int y = center_y + (int)(radius * sin(radian));
        SDL_RenderDrawPoint(renderer, x, y);
    }
}

// Update pendulum physics
void update_pendulum(Pendulum *pendulum, double delta_time)
{
    // Pendulum equation: angular_acceleration = -(gravity/length) * sin(angle) - damping * angular_velocity
    double angular_acceleration = -(pendulum->gravity / pendulum->length) * sin(pendulum->angle) 
                                   - pendulum->damping * pendulum->angular_velocity;
    pendulum->angular_velocity += angular_acceleration * delta_time;
    pendulum->angle += pendulum->angular_velocity * delta_time;
}

void draw_pendulum(SDL_Renderer *renderer, Pendulum *pendulum)
{
    int bob_x = pendulum->pivot_x + (int)(pendulum->length * sin(pendulum->angle));
    int bob_y = pendulum->pivot_y + (int)(pendulum->length * cos(pendulum->angle));
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255); 
    SDL_RenderDrawLine(renderer, pendulum->pivot_x, pendulum->pivot_y, bob_x, bob_y);
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255); 
    draw_filled_circle(renderer, pendulum->pivot_x, pendulum->pivot_y, 5);
    SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255); 
    draw_filled_circle(renderer, bob_x, bob_y, pendulum->bob_radius);
    SDL_SetRenderDrawColor(renderer, 200, 50, 50, 255); 
    draw_circle(renderer, bob_x, bob_y, pendulum->bob_radius);
}

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow(
        "SDL2 Pendulum Simulation",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    Pendulum pendulum = {
        .angle = M_PI / 4.0,        // Start at 45 degrees
        .angular_velocity = 0.0,     // Start with no velocity
        .length = 200.0,             // 200 pixels long
        .gravity = 500.0,            // Gravity constant (adjusted for pixels)
        .damping = 0.01,             // Small damping for realistic motion
        .pivot_x = 400,              // Center of window horizontally
        .pivot_y = 100,              // Near top of window
        .bob_radius = 15             // Radius of the bob
    };
    
    bool running = true;
    SDL_Event event;
    Uint32 last_time = SDL_GetTicks();
    
    while (running)
    {
        // Calculate delta time
        Uint32 current_time = SDL_GetTicks();
        double delta_time = (current_time - last_time) / 1000.0; // Convert to seconds
        last_time = current_time;
        
        // Cap delta_time to prevent large jumps
        if (delta_time > 0.1) delta_time = 0.1;
        
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
            {
                running = false;
            }
            // Reset pendulum on spacebar
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
            {
                pendulum.angle = M_PI / 4.0;
                pendulum.angular_velocity = 0.0;
            }
        }
        
        // Update pendulum physics
        update_pendulum(&pendulum, delta_time);
        
        // Clear with dark background
        SDL_SetRenderDrawColor(renderer, 20, 20, 30, 255);
        SDL_RenderClear(renderer);
        
        // Draw the pendulum
        draw_pendulum(renderer, &pendulum);
        
        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}