// include/pendulum.h
#ifndef PENDULUM_H
#define PENDULUM_H

#include <stdbool.h>

// Constants
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Max number of points to store for the trail history
#define TRAIL_LENGTH 500 

typedef struct {
    double m1;          // Mass 1 (kg)
    double m2;          // Mass 2 (kg)
    double l1;          // Length 1 (meters)
    double l2;          // Length 2 (meters)
    double g;           // Gravity (m/s^2)

    // Pendulum swining state
    double theta1;      // Angle of first rod (rad)
    double theta2;      // Angle of second rod (rad)
    double omega1;      // Angular velocity 1 (rad/s)
    double omega2;      // Angular velocity 2 (rad/s)

    // Visuals
    unsigned char color_r, color_g, color_b; 
    
    // Circular buffer to store the screen coordinates (x, y) of the second bob (m2)
    int trail_x[TRAIL_LENGTH];
    int trail_y[TRAIL_LENGTH];
    int trail_index;    // Current write position
    bool trail_full;    // True after the first loop through the buffer
    
} Pendulum;


// Initialize a pendulum object with physical parameters and state
void init_pendulum(Pendulum *p, 
                   double m1, double m2, double l1, double l2, double g_val,
                   double initial_theta1, double initial_theta2,
                   unsigned char r_val, unsigned char g_val_color, unsigned char b_val);

// Update the pendulum's state and its visual trail history.
void update_pendulum(Pendulum *p, double dt, double pixels_per_meter, int screen_width, int screen_height);

// Wrapper for Inverse Kinematics (IK) to set the initial position via mouse click.
void set_pendulum_position_ik(Pendulum *p, double target_x, double target_y, 
                              double pivot_x_screen, double pivot_y_screen, double pixels_per_meter);

#endif // PENDULUM_H