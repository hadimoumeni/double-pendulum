#include <stdio.h>
#include <math.h>
#include "arithmetic.h"

#define ASSERT_TRUE(expr, msg) \
    if (!(expr)) { \
        printf("TEST FAILED: %s\n", msg); \
        return 1; \
    }

int main() {
    // Initial simple state
    double theta1 = M_PI / 4;   // 45 degrees
    double theta2 = M_PI / 2;   // 90 degrees
    double omega1 = 0.0;
    double omega2 = 0.0;

    // Physical constants
    double m1 = 1.0, m2 = 1.0;
    double L1 = 1.0, L2 = 1.0;
    double g  = 9.81;
    double dt = 0.01;

    // Outputs
    double nt1, nt2, nw1, nw2;

    compute(theta1, theta2, omega1, omega2,
            m1, m2, L1, L2, g, dt,
            &nt1, &nt2, &nw1, &nw2);

    // ----------- TESTS ---------------
    ASSERT_TRUE(!isnan(nt1), "theta1 is NaN");
    ASSERT_TRUE(!isnan(nt2), "theta2 is NaN");
    ASSERT_TRUE(!isnan(nw1), "omega1 is NaN");
    ASSERT_TRUE(!isnan(nw2), "omega2 is NaN");

    ASSERT_TRUE(nt1 != theta1, "theta1 did not update");
    ASSERT_TRUE(nt2 != theta2, "theta2 did not update");

    ASSERT_TRUE(fabs(nt1) < 10.0, "theta1 exploded");
    ASSERT_TRUE(fabs(nt2) < 10.0, "theta2 exploded");

    printf("TEST PASSED — RK4 arithmetic module produces valid output.\n");
    printf("theta1=%f, theta2=%f\n", nt1, nt2);
    printf("omega1=%f, omega2=%f\n", nw1, nw2);

    return 0;
}
