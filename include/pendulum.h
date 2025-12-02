// include/pendulum.h
#ifndef PENDULUM_H
#define PENDULUM_H

#include <stdbool.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define TRAIL_LENGTH 500 

typedef struct {
    double m1;          
    double m2;          
    double l1;          
    double l2;       
    double g;          

    double theta1;      
    double theta2;      
    double omega1;      
    double omega2;      

    
    unsigned char color_r, color_g, color_b; 
    
    int trail_x[TRAIL_LENGTH];
    int trail_y[TRAIL_LENGTH];
    int trail_index;   
    bool trail_full;    
    
} Pendulum;


void init_pendulum(Pendulum *p, 
                   double m1, double m2, double l1, double l2, double g_val,
                   double initial_theta1, double initial_theta2,
                   unsigned char r_val, unsigned char g_val_color, unsigned char b_val);

void update_pendulum(Pendulum *p, double dt, double pixels_per_meter, int screen_width, int screen_height);

void set_pendulum_position_ik(Pendulum *p, double target_x, double target_y, 
                              double pivot_x_screen, double pivot_y_screen, double pixels_per_meter);

#endif 