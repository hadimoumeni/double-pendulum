#include <math.h>


void compute( 
  double theta1, 
  double theta2,
  double omega1,
  double omega2,
  double m1,
  double m2,  
  double l1,
  double l2, 
  double g,
  double dt,
  double *new_theta1,
  double *new_theta2,
  double *new_omega1,
  double *new_omega2
){
  {
    double delta = theta1 - theta2;
    double den = 2*m1 + m2 - m2 * cos(2*delta);

    double theta1_dd = (
        -g*(2*m1 + m2)*sin(theta1)
        - m2*g*sin(theta1 - 2*theta2)
        - 2*sin(delta)*m2*(omega2*omega2*l2 + omega1*omega1*l1*cos(delta))
    ) / (l1 * den);

    double theta2_dd = (
        2*sin(delta) * (
            omega1*omega1*l1*(m1 + m2)
            + g*(m1 + m2)*cos(theta1)
            + omega2*omega2*l2*m2*cos(delta)
        )
    ) / (l2 * den);

    // Euler integration
    *new_omega1 = omega1 + theta1_dd * dt;
    *new_omega2 = omega2 + theta2_dd * dt;

    *new_theta1 = theta1 + (*new_omega1) * dt;
    *new_theta2 = theta2 + (*new_omega2) * dt;
}
}