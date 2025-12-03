// testing

#include "unity.h"
#include "arithmetic.h"
#include "pendulum.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

void setUp(void) { }
void tearDown(void) { }

void test_Single_Step_Integration(void) {
    double theta1 = M_PI / 4;
    double theta2 = M_PI / 2;
    double omega1 = 0.0;
    double omega2 = 0.0;
    double m1 = 1.0, m2 = 1.0;
    double l1 = 1.0, l2 = 1.0;
    double g  = 9.81;
    double dt = 0.01;

    double nt1, nt2, nw1, nw2;

    compute(theta1, theta2, omega1, omega2, m1, m2, l1, l2, g, dt, &nt1, &nt2, &nw1, &nw2);

    TEST_ASSERT_FALSE_MESSAGE(isnan(nt1), "theta1 is NaN");
    TEST_ASSERT_TRUE_MESSAGE(theta1 != nt1, "theta1 did not update");
}

static double get_total_energy(double m1, double m2, double l1, double l2, 
                        double t1, double t2, double w1, double w2, double g) {
    double K1 = 0.5 * m1 * l1*l1 * w1*w1;
    double term2 = 0.5 * m2 * (l1*l1*w1*w1 + l2*l2*w2*w2 + 2*l1*l2*w1*w2*cos(t1-t2));
    double V = -(m1+m2)*g*l1*cos(t1) - m2*g*l2*cos(t2);
    return K1 + term2 + V;
}

void test_Energy_Conservation(void) {
    double t1 = M_PI/4;
    double t2 = M_PI/4;
    double w1 = 0, w2 = 0;
    double m1=1.0, m2=1.0, l1=1.0, l2=1.0, g=9.81, dt=0.005;

    double start_E = get_total_energy(m1, m2, l1, l2, t1, t2, w1, w2, g);

    double nt1, nt2, nw1, nw2;
    int i;
    for(i=0; i<1000; i++) {
        compute(t1, t2, w1, w2, m1, m2, l1, l2, g, dt, &nt1, &nt2, &nw1, &nw2);
        t1=nt1; t2=nt2; w1=nw1; w2=nw2;
    }

    double end_E = get_total_energy(m1, m2, l1, l2, t1, t2, w1, w2, g);

    double diff = fabs(start_E - end_E);
    double allowed_error = fabs(start_E) * 0.05;

    char msg[100];
    snprintf(msg, sizeof(msg), "Energy lost! Start: %.4f, End: %.4f", start_E, end_E);

    TEST_ASSERT_TRUE_MESSAGE(diff < allowed_error, msg);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_Single_Step_Integration);
    RUN_TEST(test_Energy_Conservation);
    return UNITY_END();
}