#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <stdbool.h>
#include "physics.h"

bool init_visualizer(int width, int height);

void render_pendulum(const DoublePendulum *p);

bool handle_input(DoublePendulum *p);

void close_visualizer();

#endif

