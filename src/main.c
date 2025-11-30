#include <stdio.h>
#include <math.h>

#include "../include/pendulum.h"
#include "../include/sdl_visuals.h"

int main(int argc, char *argv[]) {
    
    // Single pendulum instance
    Pendulum pendulum;

    // m1=1.0 kg, m2=1.0 kg, l1=1.5 m, l2=1.5 m, g=9.81 m/s^2
    // Initial angles: 90 degrees (M_PI / 2)
    init_pendulum(&pendulum, 
                  1.0, 1.0, 1.5, 1.5, 9.81, 
                  M_PI / 2.0, M_PI / 2.0,
                  50, 50, 255); // Blue color

    // Run the main SDL simulation loop
    run_simulation(&pendulum);

    return 0;
}