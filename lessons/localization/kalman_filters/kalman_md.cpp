#include <iostream>
#include <math.h>
#include <tuple>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/LU>

using namespace std;
using namespace Eigen;

float measurements[3] = {1, 2, 3};

tuple<MatrixXf, MatrixXf> kalman_filter(MatrixXf x, MatrixXf P, MatrixXf u,
    MatrixXf F, MatrixXf H, MatrixXf R, MatrixXf I)
{
  // Initialize z, y, S, K
  MatrixXf z(1, 1); // measurement
  MatrixXf y(1, 1); // residual
  MatrixXf S(1, 1); // system uncertainty
  MatrixXf K(2, 1); // Kalman gain

  for (int n = 0; n < sizeof(measurements)/sizeof(measurements[0]); n++)
  {
    // Measurement update
    z << measurements[n];
    y << z - H * x;
    S << H * P * H.transpose() + R;
    K << P * H.transpose() * S.inverse();
    x << x + K * y;
    P << (I - K * H) * P;

    // State prediction
    x << F * x;
    P << F * P * F.transpose();
  }

  return make_tuple(x, P);
}

int main()
{
  MatrixXf x(2, 1); // initial state (location and velocity)
  x << 0,
       0;
  MatrixXf P(2, 2); // initial uncertainty
  P << 100, 0,
       0, 100;
  MatrixXf u(2, 1); // external motion
  u << 0,
       0;
  MatrixXf F(2, 2); // next state function
  F << 1, 1,
       0, 1;
  MatrixXf H(1, 2); // measurement function
  H << 1,
       0;
  MatrixXf R(1, 1); // measurement uncertainty
  R << 1;
  MatrixXf I(2, 2); // identity matrix
  I << 1, 0,
       0, 1;

  tie(x, P) = kalman_filter(x, P, u, F, H, R, I);
  cout << "x = " << x << endl;
  cout << "P = " << P << endl;

  return 0;
}
