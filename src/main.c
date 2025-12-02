#include <stdio.h>
#include <math.h>

#include "pendulum.h"
#include "sdl_visuals.h"

int main(int argc, char *argv[]) {
    
    // Single pendulum instance
    Pendulum pendulum;

    init_pendulum(&pendulum, 
                  1.0, 1.0, 1.5, 1.5, 9.81, 
                  M_PI / 2.0, M_PI / 2.0,
                  50, 50, 255); // Blue color

    // run SDL loop
    run_simulation(&pendulum);

    return 0;
}