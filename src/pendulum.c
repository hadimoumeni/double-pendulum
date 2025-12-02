#include <math.h>
#include <stdio.h>
#include <pthread.h>
#include "pendulum.h"
#include "arithmetic.h"

void init_pendulum(Pendulum *p,
                   double m1_val, double m2_val, double l1_val, double l2_val, double g_val,
                   double initial_theta1, double initial_theta2,
                   unsigned char r_val, unsigned char g_val_color, unsigned char b_val) {
    double m1 = m1_val;
    double m2 = m2_val;
    double l1 = l1_val;
    double l2 = l2_val;
    double g = g_val;
    p->m1 = m1;
    p->m2 = m2;
    p->l1 = l1;
    p->l2 = l2;
    p->g = g;
    double theta1 = initial_theta1;
    double theta2 = initial_theta2;
    double omega1 = 0.0;
    double omega2 = 0.0;
    p->theta1 = theta1;
    p->theta2 = theta2;
    p->omega1 = omega1;
    p->omega2 = omega2;
    p->color_r = r_val;
    p->color_g = g_val_color;
    p->color_b = b_val;
    p->trail_index = 0;
    p->trail_full = false;
    for (int i = 0; i < TRAIL_LENGTH; i++) {
        p->trail_x[i] = 0;
        p->trail_y[i] = 0;
    }
}

static void get_screen_coords(const Pendulum *p, double pixels_per_meter, int screen_width, int screen_height,
                              int pivot_x, int pivot_y, int *x2_screen, int *y2_screen) {
    double l1 = p->l1;
    double l2 = p->l2;
    double theta1 = p->theta1;
    double theta2 = p->theta2;
    double scale = pixels_per_meter;
    int x1_screen = pivot_x + (int)(l1 * sin(theta1) * scale);
    int y1_screen = pivot_y + (int)(l1 * cos(theta1) * scale);
    *x2_screen = x1_screen + (int)(l2 * sin(theta2) * scale);
    *y2_screen = y1_screen + (int)(l2 * cos(theta2) * scale);
}

void update_pendulum(Pendulum *p, double dt, double pixels_per_meter, int screen_width, int screen_height) {
    double theta1 = p->theta1;
    double theta2 = p->theta2;
    double omega1 = p->omega1;
    double omega2 = p->omega2;
    double m1 = p->m1;
    double m2 = p->m2;
    double l1 = p->l1;
    double l2 = p->l2;
    double g = p->g;
    double new_theta1, new_theta2, new_omega1, new_omega2;
    compute(
        theta1, theta2,
        omega1, omega2,
        m1, m2,
        l1, l2,
        g, dt,
        &new_theta1, &new_theta2,
        &new_omega1, &new_omega2
    );
    theta1 = new_theta1;
    theta2 = new_theta2;
    omega1 = new_omega1;
    omega2 = new_omega2;
    p->theta1 = theta1;
    p->theta2 = theta2;
    p->omega1 = omega1;
    p->omega2 = omega2;
    const int pivot_x = screen_width / 2;
    const int pivot_y = screen_height / 3;
    int x2, y2;
    get_screen_coords(p, pixels_per_meter, screen_width, screen_height, pivot_x, pivot_y, &x2, &y2);
    int trail_index = p->trail_index;
    p->trail_x[trail_index] = x2;
    p->trail_y[trail_index] = y2;
    trail_index++;
    if (trail_index >= TRAIL_LENGTH) {
        trail_index = 0;
        p->trail_full = true;
    }
    p->trail_index = trail_index;
}

static pthread_mutex_t pendulum_mutex = PTHREAD_MUTEX_INITIALIZER;

void update_pendulum_threadsafe(Pendulum *p, double dt, double pix_per_m, int screen_w, int screen_h) {
    pthread_mutex_lock(&pendulum_mutex);
    update_pendulum(p, dt, pix_per_m, screen_w, screen_h);
    pthread_mutex_unlock(&pendulum_mutex);
}

void set_pendulum_position_ik(Pendulum *p, double target_x, double target_y,
                              double pivot_x_screen, double pivot_y_screen, double pixels_per_meter) {
    double L1 = p->l1;
    double L2 = p->l2;
    double target_X = (target_x - pivot_x_screen) / pixels_per_meter;
    double target_Y = (target_y - pivot_y_screen) / pixels_per_meter;
    double D_sq = target_X * target_X + target_Y * target_Y;
    double D = sqrt(D_sq);
    if (D > L1 + L2 + 0.01 || D < fabs(L1 - L2) - 0.01) {
        return;
    }
    double cos_alpha = (L1*L1 + D_sq - L2*L2) / (2 * L1 * D);
    cos_alpha = fmax(-1.0, fmin(1.0, cos_alpha));
    double alpha = acos(cos_alpha);
    double beta = atan2(target_X, target_Y);
    double new_th1 = beta - alpha;
    double x1_calc = L1 * sin(new_th1);
    double y1_calc = L1 * cos(new_th1);
    double dx_m1_m2 = target_X - x1_calc;
    double dy_m1_m2 = target_Y - y1_calc;
    double new_th2 = atan2(dx_m1_m2, dy_m1_m2);
    double theta1 = new_th1;
    double theta2 = new_th2;
    p->theta1 = theta1;
    p->theta2 = theta2;
    p->trail_index = 0;
    p->trail_full = false;
}