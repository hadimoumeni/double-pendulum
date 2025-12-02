#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#include "../include/sdl_visuals.h"
#include "../include/pendulum.h"
#include "../include/sha256.h"

static SDL_Window *gWindow = NULL;
static SDL_Renderer *gRenderer = NULL;
static const double PIX_PER_M = PIXELS_PER_METER; 
static bool simulation_running = false; 
static bool is_dragging = false; 

static int udp_sock = -1;
static struct sockaddr_in udp_dest;
static int udp_initialized = 0;

const int BOB_RADIUS = 10;
const int PIVOT_X = SCREEN_WIDTH / 2;
const int PIVOT_Y = SCREEN_HEIGHT / 3;

void setup_udp_sender(const char* dest_ip, int dest_port) {
    udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&udp_dest, 0, sizeof(udp_dest));
    udp_dest.sin_family = AF_INET;
    udp_dest.sin_port = htons(dest_port);
    inet_pton(AF_INET, dest_ip, &udp_dest.sin_addr);
    udp_initialized = 1;
}

void send_random_number_udp(uint64_t randnum) {
    if (!udp_initialized) return;
    char msg[64];
    snprintf(msg, sizeof(msg), "%llu", (unsigned long long)randnum);
    sendto(udp_sock, msg, strlen(msg), 0, (struct sockaddr*)&udp_dest, sizeof(udp_dest));
}

static void draw_grid() {
    SDL_SetRenderDrawColor(gRenderer, 50, 50, 60, 255); // Dark blue-gray

    // Horizontal lines
    int h_offset_start = PIVOT_Y % (int)(GRID_SPACING * PIX_PER_M);
    for (int y = h_offset_start; y < SCREEN_HEIGHT; y += (int)(GRID_SPACING * PIX_PER_M)) {
        if (y < PIVOT_Y - 1 || y > PIVOT_Y + 1) {
            SDL_RenderDrawLine(gRenderer, 0, y, SCREEN_WIDTH, y);
        }
    }
    // Vertical lines
    int v_offset_start = PIVOT_X % (int)(GRID_SPACING * PIX_PER_M);
    for (int x = v_offset_start; x < SCREEN_WIDTH; x += (int)(GRID_SPACING * PIX_PER_M)) {
        if (x < PIVOT_X - 1 || x > PIVOT_X + 1) { 
            SDL_RenderDrawLine(gRenderer, x, 0, x, SCREEN_HEIGHT);
        }
    }
}

static void draw_filled_circle(SDL_Renderer *rend, int cx, int cy, int radius, unsigned char r, unsigned char g, unsigned char b) {
    SDL_SetRenderDrawColor(rend, r, g, b, 255);
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w; 
            int dy = radius - h; 
            if ((dx * dx + dy * dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(rend, cx + dx, cy + dy);
            }
        }
    }
}

static bool init_sdl() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return false;
    }

    gWindow = SDL_CreateWindow("Pendulum: Drag Bob 2. W/S/A/D to adjust velocity. SPACE to Play/Pause.",
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

static void close_sdl() {
    if (gRenderer) SDL_DestroyRenderer(gRenderer);
    if (gWindow) SDL_DestroyWindow(gWindow);
    SDL_Quit();
}

static void render_pendulum(const Pendulum *p) {
    
    // 1. Forward Kinematics
    int x1 = PIVOT_X + (int)(p->l1 * sin(p->theta1) * PIX_PER_M);
    int y1 = PIVOT_Y + (int)(p->l1 * cos(p->theta1) * PIX_PER_M);
    int x2 = x1 + (int)(p->l2 * sin(p->theta2) * PIX_PER_M);
    int y2 = y1 + (int)(p->l2 * cos(p->theta2) * PIX_PER_M);

    // 2. Draw Trail
    int start = p->trail_full ? 0 : p->trail_index;
    int end = p->trail_full ? TRAIL_LENGTH : p->trail_index;
    int length = p->trail_full ? TRAIL_LENGTH : p->trail_index;

    for (int i = start; i < end - 1; i++) {
        int current_idx = (p->trail_index + i) % TRAIL_LENGTH;
        int next_idx = (p->trail_index + i + 1) % TRAIL_LENGTH;
        
        double fade = (double)(i - start) / length; 
        
        unsigned char r = (unsigned char)(p->color_r * fade);
        unsigned char g = (unsigned char)(p->color_g * fade);
        unsigned char b = (unsigned char)(p->color_b * fade);
        
        SDL_SetRenderDrawColor(gRenderer, r, g, b, 255);
        SDL_RenderDrawLine(gRenderer, 
                           p->trail_x[current_idx], p->trail_y[current_idx], 
                           p->trail_x[next_idx], p->trail_y[next_idx]);
    }
    
    // 3. Draw Rods and Bobs

    // Draw Rods (Gray)
    SDL_SetRenderDrawColor(gRenderer, 150, 150, 150, 255); 
    SDL_RenderDrawLine(gRenderer, PIVOT_X, PIVOT_Y, x1, y1); 
    SDL_RenderDrawLine(gRenderer, x1, y1, x2, y2);           

    // Draw Bob 1
    draw_filled_circle(gRenderer, x1, y1, BOB_RADIUS, 
                       (unsigned char)fmin(255, p->color_r * 1.5), 
                       (unsigned char)fmin(255, p->color_g * 1.5), 
                       (unsigned char)fmin(255, p->color_b * 1.5));

    // Draw Bob 2 (Main color)
    draw_filled_circle(gRenderer, x2, y2, BOB_RADIUS, p->color_r, p->color_g, p->color_b);

    // Draw Pivot (White dot)
    draw_filled_circle(gRenderer, PIVOT_X, PIVOT_Y, 5, 255, 255, 255);

    // 4. Draw Velocity Indicators
    if (!simulation_running) {
        // Red line for Omega 1
        SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
        SDL_RenderDrawLine(gRenderer, x1, y1, 
                           x1 + (int)(p->omega1 * 5), y1); 
                           
        // Green line for Omega 2
        SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
        SDL_RenderDrawLine(gRenderer, x2, y2, 
                           x2 + (int)(p->omega2 * 5), y2); 
    }
}


static bool handle_input(Pendulum *p) {
    SDL_Event event;
    const double VEL_STEP = 0.5; // change in angular velocity

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
            return false;
        }

        // Keyboard Input 
        if (event.type == SDL_KEYDOWN) {
            // R: Reset Position and Velocity
            if (event.key.keysym.sym == SDLK_r) {
                init_pendulum(p, p->m1, p->m2, p->l1, p->l2, p->g, M_PI / 2.0, M_PI / 2.0, p->color_r, p->color_g, p->color_b);
                simulation_running = false;
                printf("--- Simulation Reset (SPACE to Play) ---\n");
            }
            
            // SPACE: Toggle Start/Stop (Play/Pause)
            else if (event.key.keysym.sym == SDLK_SPACE || event.key.keysym.sym == SDLK_p) {
                simulation_running = !simulation_running;
                printf("--- Simulation %s ---\n", simulation_running ? "RUNNING" : "PAUSED");
                if (simulation_running) {
                    printf("Starting state: Omega1=%.2f, Omega2=%.2f\n", p->omega1, p->omega2);
                }
            }
            
            // W/S: Adjust Omega 1 (Velocity of first bob)
            else if (event.key.keysym.sym == SDLK_w) p->omega1 += VEL_STEP;
            else if (event.key.keysym.sym == SDLK_s) p->omega1 -= VEL_STEP;
            
            // A/D: Adjust Omega 2 (Velocity of second bob)
            else if (event.key.keysym.sym == SDLK_a) p->omega2 -= VEL_STEP;
            else if (event.key.keysym.sym == SDLK_d) p->omega2 += VEL_STEP;
            
            // Print velocity state if a velocity key was pressed
            if (event.key.keysym.sym == SDLK_w || event.key.keysym.sym == SDLK_s || 
                event.key.keysym.sym == SDLK_a || event.key.keysym.sym == SDLK_d) {
                simulation_running = false; // Pause when adjusting velocity
                printf("Velocity Set (Paused): Omega1=%.2f, Omega2=%.2f. Press SPACE to run.\n", p->omega1, p->omega2);
            }
        }
        
        // Mouse Input: Drag-and-Drop Positioning
            if (event.type == SDL_MOUSEBUTTONDOWN && !simulation_running) {
            is_dragging = true;
            p->omega1 = 0.0;
            p->omega2 = 0.0;
            // Clear trail on drag start
            p->trail_full = false;
            p->trail_index = 0;
        } 
        
        // Mouse Motion: Dragging
        else if (event.type == SDL_MOUSEMOTION && is_dragging) {
            // Update position
            set_pendulum_position_ik(p, 
                                     (double)event.motion.x, (double)event.motion.y, 
                                     (double)PIVOT_X, (double)PIVOT_Y, PIX_PER_M);
        }
        
        // Mouse Up: Stop dragging
        else if (event.type == SDL_MOUSEBUTTONUP) {
            if (is_dragging) {
                is_dragging = false;
                printf("--- Position Set (Paused) ---\n");
                printf("Position: Theta1=%.2f, Theta2=%.2f\n", p->theta1, p->theta2);
                printf("Velocity: Omega1=%.2f, Omega2=%.2f\n", p->omega1, p->omega2);
                printf("Press SPACE to start simulation.\n");
            }
        }
    }
    return true;
}

// Main Simulation Loop
void run_simulation(Pendulum *p) {
    if (!init_sdl()) {
        close_sdl();
        return;
    }

    printf("\n--- Double Pendulum Simulator Controls ---\n");
    printf("SPACE (or P): Play/Pause Simulation\n");
    printf("R: Reset to default 90/90 position\n");
    printf("W/S: Increase/Decrease Angular Velocity 1 (omega1)\n");
    printf("A/D: Decrease/Increase Angular Velocity 2 (omega2)\n");
    printf("Mouse Drag: Set Position of Bob 2 (IK) while paused\n");
    printf("ESC: Quit\n");
    printf("------------------------------------------\n");
    printf("Position: Theta1=%.2f, Theta2=%.2f\n", p->theta1, p->theta2);
    printf("Velocity: Omega1=%.2f, Omega2=%.2f\n", p->omega1, p->omega2);
    printf("Press SPACE to start simulation.\n\n");

    const double PHYS_STEP = 0.01; 
    bool running = true;
    Uint32 last_time = SDL_GetTicks();
    double accumulator = 0.0; 
    double sim_time = 0.0;
    double next_log_time = 2.0;

    while (running) {
        // 1. Frame Timing 
        Uint32 current_time = SDL_GetTicks();
        double frame_time = (current_time - last_time) / 1000.0; 
        last_time = current_time;
        if (frame_time > 0.25) frame_time = 0.25; 
        accumulator += frame_time;

        // 2. Input 
        running = handle_input(p); 
        if (!running) break;

        // 3. Physics Update
        if (simulation_running) {
            while (accumulator >= PHYS_STEP) {
                update_pendulum(p, PHYS_STEP, PIX_PER_M, SCREEN_WIDTH, SCREEN_HEIGHT);
                accumulator -= PHYS_STEP;
                sim_time += PHYS_STEP;
                if (sim_time >= next_log_time) {
                    // Calculate positions
                    double x1 = p->l1 * sin(p->theta1);
                    double y1 = -p->l1 * cos(p->theta1);
                    double x2 = x1 + p->l2 * sin(p->theta2);
                    double y2 = y1 - p->l2 * cos(p->theta2);
                    printf("[t=%.2fs] Mass1: (%.3f, %.3f)  Mass2: (%.3f, %.3f)", sim_time, x1, y1, x2, y2);

                    // SHA-256 hash of the product of positions
                    double product = x1 * y1 * x2 * y2;
                    char prod_str[64];
                    snprintf(prod_str, sizeof(prod_str), "%.16f", product);
                    uint8_t hash[SHA256_BLOCK_SIZE];
                    SHA256_CTX ctx;
                    sha256_init(&ctx);
                    sha256_update(&ctx, (uint8_t*)prod_str, strlen(prod_str));
                    sha256_final(&ctx, hash);
                    printf(" | SHA256(product): ");
                    for (int i = 0; i < SHA256_BLOCK_SIZE; i++) printf("%02x", hash[i]);
                    // For a random number, just use the first 8 bytes as a uint64_t
                    uint64_t randnum = 0;
                    for (int i = 0; i < 8; i++) randnum = (randnum << 8) | hash[i];
                    printf(" | Random: %llu\n", (unsigned long long)randnum);
                    // End SHA-256 

                    if (!udp_initialized) {
                        // Set your friend's IP and port here
                        setup_udp_sender("FRIEND_IP_HERE", 12345); // <-- REPLACE FRIEND_IP_HERE
                    }

                    send_random_number_udp(randnum);

                    next_log_time += 2.0;
                }
            }
        }

        // 4. Render 
        SDL_SetRenderDrawColor(gRenderer, 30, 30, 40, 255); 
        SDL_RenderClear(gRenderer);
        
        draw_grid(); 
        render_pendulum(p);
        
        SDL_RenderPresent(gRenderer);
    }

    close_sdl();
}
