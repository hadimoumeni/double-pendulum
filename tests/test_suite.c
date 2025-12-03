#include "unity.h"
#include "arithmetic.h"
#include "pendulum.h"
#include <math.h>

void setUp(void) { }
void tearDown(void) { }

// Assert Raji's math

static double get_total_energy(double m1, double m2, double l1, double l2, 
                               double t1, double t2, double w1, double w2, double g) {
    double K1 = 0.5 * m1 * l1 * l1 * w1 * w1;
    double K2 = 0.5 * m2 * (l1 * l1 * w1 * w1 + l2 * l2 * w2 * w2 + 2 * l1 * l2 * w1 * w2 * cos(t1 - t2));
    double V = -(m1 + m2) * g * l1 * cos(t1) - m2 * g * l2 * cos(t2);
    return K1 + K2 + V;
}

void test_Equilibrium(void) {
    double theta1 = 0.0;
    double theta2 = 0.0;
    double omega1 = 0.0;
    double omega2 = 0.0;
    double m1 = 1.0, m2 = 1.0;
    double l1 = 1.0, l2 = 1.0;
    double g = 9.81;
    double dt = 0.01;

    double nt1, nt2, nw1, nw2;
    
    for (int i = 0; i < 100; i++) {
        compute(theta1, theta2, omega1, omega2, m1, m2, l1, l2, g, dt, &nt1, &nt2, &nw1, &nw2);
        theta1 = nt1;
        theta2 = nt2;
        omega1 = nw1;
        omega2 = nw2;
    }

    TEST_ASSERT_DOUBLE_WITHIN(1e-10, 0.0, theta1);
    TEST_ASSERT_DOUBLE_WITHIN(1e-10, 0.0, theta2);
    TEST_ASSERT_DOUBLE_WITHIN(1e-10, 0.0, omega1);
    TEST_ASSERT_DOUBLE_WITHIN(1e-10, 0.0, omega2);
}

void test_ZeroGravity(void) {
    double theta1 = M_PI / 4;
    double theta2 = M_PI / 3;
    double omega1 = 0.5;
    double omega2 = -0.3;
    double m1 = 1.0, m2 = 1.0;
    double l1 = 1.0, l2 = 1.0;
    double g = 0.0;
    double dt = 0.01;

    double start_E = get_total_energy(m1, m2, l1, l2, theta1, theta2, omega1, omega2, g);
    
    double nt1, nt2, nw1, nw2;

    for (int i = 0; i < 100; i++) {
        compute(theta1, theta2, omega1, omega2, m1, m2, l1, l2, g, dt, &nt1, &nt2, &nw1, &nw2);
        theta1 = nt1;
        theta2 = nt2;
        omega1 = nw1;
        omega2 = nw2;
    }

    double end_E = get_total_energy(m1, m2, l1, l2, theta1, theta2, omega1, omega2, g);

    TEST_ASSERT_DOUBLE_WITHIN(1e-10, start_E, end_E);
}


void test_EnergyConservation(void) {
    double t1 = M_PI / 4;
    double t2 = M_PI / 4;
    double w1 = 0.0;
    double w2 = 0.0;
    double m1 = 1.0, m2 = 1.0;
    double l1 = 1.0, l2 = 1.0;
    double g = 9.81;
    double dt = 0.01;

    double start_E = get_total_energy(m1, m2, l1, l2, t1, t2, w1, w2, g);
    double nt1, nt2, nw1, nw2;

    for (int i = 0; i < 500; i++) {
        compute(t1, t2, w1, w2, m1, m2, l1, l2, g, dt, &nt1, &nt2, &nw1, &nw2);
        t1 = nt1;
        t2 = nt2;
        w1 = nw1;
        w2 = nw2;
    }

    double end_E = get_total_energy(m1, m2, l1, l2, t1, t2, w1, w2, g);
    double diff = fabs(start_E - end_E);
    double tolerance = fabs(start_E) * 0.001;

    TEST_ASSERT_TRUE_MESSAGE(diff < tolerance, "Energy conservation violated");
}

void test_Reversibility(void) {
    double t1_start = M_PI / 3;
    double t2_start = M_PI / 4;
    double w1_start = 0.2;
    double w2_start = -0.1;
    double m1 = 1.0, m2 = 1.0;
    double l1 = 1.0, l2 = 1.0;
    double g = 9.81;
    double dt = 0.01;

    double t1 = t1_start;
    double t2 = t2_start;
    double w1 = w1_start;
    double w2 = w2_start;
    double nt1, nt2, nw1, nw2;

    for (int i = 0; i < 100; i++) {
        compute(t1, t2, w1, w2, m1, m2, l1, l2, g, dt, &nt1, &nt2, &nw1, &nw2);
        t1 = nt1;
        t2 = nt2;
        w1 = nw1;
        w2 = nw2;
    }

    w1 = -w1;
    w2 = -w2;

    for (int i = 0; i < 100; i++) {
        compute(t1, t2, w1, w2, m1, m2, l1, l2, g, dt, &nt1, &nt2, &nw1, &nw2);
        t1 = nt1;
        t2 = nt2;
        w1 = nw1;
        w2 = nw2;
    }

    TEST_ASSERT_DOUBLE_WITHIN(1e-6, t1_start, t1);
    TEST_ASSERT_DOUBLE_WITHIN(1e-6, t2_start, t2);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_Equilibrium);
    RUN_TEST(test_ZeroGravity);
    RUN_TEST(test_EnergyConservation);
    RUN_TEST(test_Reversibility);
    return UNITY_END();
}
