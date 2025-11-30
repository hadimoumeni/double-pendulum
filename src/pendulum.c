#include <math.h>
#include <stdio.h> 

#include "../include/pendulum.h"
#include "../include/arithmetic.h"


// Initialize a pendulum object with physical parameters and state
void init_pendulum(Pendulum *p, 
                   double m1, double m2, double l1, double l2, double g_val,
                   double initial_theta1, double initial_theta2,
                   unsigned char r_val, unsigned char g_val_color, unsigned char b_val) {
    
    // Set parameters
    p->m1 = m1;
    p->m2 = m2;
    p->l1 = l1;
    p->l2 = l2;
    p->g = g_val; 

    // Set initial state
    p->theta1 = initial_theta1;
    p->theta2 = initial_theta2;
    p->omega1 = 0.0;
    p->omega2 = 0.0;
    
    // Set color
    p->color_r = r_val;
    p->color_g = g_val_color;
    p->color_b = b_val;

    // Initialize trail history
    p->trail_index = 0;
    p->trail_full = false;
    for (int i = 0; i < TRAIL_LENGTH; i++) {
        p->trail_x[i] = 0;
        p->trail_y[i] = 0;
    }
}

// Helper function to convert physics state to screen coordinates
static void get_screen_coords(const Pendulum *p, double pixels_per_meter, int screen_width, int screen_height,
                              int pivot_x, int pivot_y, int *x2_screen, int *y2_screen) {
    
    int x1_screen = pivot_x + (int)(p->l1 * sin(p->theta1) * pixels_per_meter);
    int y1_screen = pivot_y + (int)(p->l1 * cos(p->theta1) * pixels_per_meter);

    *x2_screen = x1_screen + (int)(p->l2 * sin(p->theta2) * pixels_per_meter);
    *y2_screen = y1_screen + (int)(p->l2 * cos(p->theta2) * pixels_per_meter);
}


// Update the pendulum's state and its visual trail history. 
void update_pendulum(Pendulum *p, double dt, double pixels_per_meter, int screen_width, int screen_height) {
    // ... (unchanged)
    double new_theta1, new_theta2, new_omega1, new_omega2;

    // 1. Physics Update (RK4)
    compute(
        p->theta1, p->theta2,
        p->omega1, p->omega2,
        p->m1, p->m2,
        p->l1, p->l2,
        p->g, dt,
        &new_theta1, &new_theta2,
        &new_omega1, &new_omega2
    );

    p->theta1 = new_theta1;
    p->theta2 = new_theta2;
    p->omega1 = new_omega1;
    p->omega2 = new_omega2;
    
    // 2. Trail History Update
    const int pivot_x = screen_width / 2;
    const int pivot_y = screen_height / 3;
    
    int x2, y2;
    // Note: Passed NULL for x1/y1 as we only need x2/y2 for the trail.
    get_screen_coords(p, pixels_per_meter, screen_width, screen_height, pivot_x, pivot_y, &x2, &y2); 
    
    p->trail_x[p->trail_index] = x2;
    p->trail_y[p->trail_index] = y2;
    
    p->trail_index++;
    
    if (p->trail_index >= TRAIL_LENGTH) {
        p->trail_index = 0;
        p->trail_full = true;
    }
}


// Wrapper for Inverse Kinematics (IK)
void set_pendulum_position_ik(Pendulum *p, double target_x, double target_y, 
                              double pivot_x_screen, double pivot_y_screen, double pixels_per_meter) {
    
    // 1. Convert mouse coords to physical meters (relative to pivot)
    double target_X = (target_x - pivot_x_screen) / pixels_per_meter;
    double target_Y = (target_y - pivot_y_screen) / pixels_per_meter;

    double L1 = p->l1;
    double L2 = p->l2;

    // 2. Calculate Distance (D) from pivot to target
    double D_sq = target_X * target_X + target_Y * target_Y;
    double D = sqrt(D_sq);

    // 3. Check Reachability (Error Handling/Sanity Check)
    if (D > L1 + L2 + 0.01 || D < fabs(L1 - L2) - 0.01) { 
        // DO NOT PRINT ERROR ON EVERY MOUSE MOVE. 
        // The calling function (sdl_visuals.c) handles drag limits.
        return; 
    }

    // 4. Solve IK for theta1 (Angle of first rod from vertical)
    double cos_alpha = (L1*L1 + D_sq - L2*L2) / (2 * L1 * D);
    cos_alpha = fmax(-1.0, fmin(1.0, cos_alpha));
    double alpha = acos(cos_alpha); 
    double beta = atan2(target_X, target_Y); 
    double new_th1 = beta - alpha;

    // 5. Solve IK for theta2 (Angle of second rod from vertical)
    double x1_calc = L1 * sin(new_th1);
    double y1_calc = L1 * cos(new_th1);
    double dx_m1_m2 = target_X - x1_calc;
    double dy_m1_m2 = target_Y - y1_calc;
    double new_th2 = atan2(dx_m1_m2, dy_m1_m2);

    // --- 6. Set Final State & Reset Velocities ---
    p->theta1 = new_th1;
    p->theta2 = new_th2;
    // NOTE: Velocities are left as is, but are reset to 0 in sdl_visuals.c 
    // when dragging starts to ensure a clean rest start.
    
    // Reset trail
    p->trail_index = 0;
    p->trail_full = false;
}