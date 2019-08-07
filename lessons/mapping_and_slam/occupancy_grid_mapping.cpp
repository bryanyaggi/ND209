#include <iostream>
#include <math.h>
#include <vector>
#include "matplotlibcpp.h"

using namespace std;
namespace plt = matplotlibcpp;

// sensor characteristic: max and min ranges of beams
double Zmax = 5000, Zmin = 170;

// log odd values for unknown (l0), occupied (locc), and free (lfree) cells
double l0 = 0, locc = 0.4, lfree = -0.4;

// grid dimensions
double gridWidth = 100, gridHeight = 100;

// map dimensions
double mapWidth = 30000, mapHeight = 15000;

// robot size with respect to map size
double robotXOffset = mapWidth / 5, robotYOffset = mapHeight / 3;

vector<vector<double>> l(mapWidth / gridWidth, vector<double>(mapHeight / gridHeight));

double inverseSensorModel(double x, double y, double theta, double xi, double yi,
    double sensorData[])
{
  double Zk, thetaK, sensorTheta;
  double minDelta = -1;
  double alpha = 200, beta = 20;

  double r = sqrt(pow(xi - x, 2) + pow(yi - y, 2));
  double phi = atan2(yi - y, xi - x) - theta;

  double sensorThetas[] = {-90, -37.5, -22.5, -7.5, 7.5, 22.5, 37.5, 90};
  int k = -1;
  double minAngle = 360;
  for (int i = 0; i < sizeof(sensorThetas)/sizeof(sensorThetas[0]); i++)
  {
    // convert to radians
    sensorThetas[i] *= M_PI/180;
    if (fabs(phi - sensorThetas[i]) < minAngle)
    {
      minAngle = fabs(phi - sensorThetas[i]);
      k = i;
    }
  }

  if (r > min(Zmax, sensorData[k] + alpha/2) ||
      fabs(phi - sensorThetas[k]) > beta/2 || sensorData[k] > Zmax ||
      sensorData[k] < Zmin)
  {
    return l0;
  }
  if (sensorData[k] < Zmax && fabs(r - sensorData[k]) < alpha/2)
  {
    return locc;
  }
  return lfree;
}

void occupancyGridMapping(double Robotx, double Roboty, double Robottheta,
    double sensorData[])
{
  for (int x = 0; x < mapWidth / gridWidth; x++)
  {
    for (int y = 0; y < mapHeight / gridHeight; y++)
    {
      double xi = x * gridWidth + gridWidth / 2.0 - robotXOffset;
      double yi = -(y * gridHeight + gridHeight / 2.0) + robotYOffset;
      if (sqrt(pow(xi - Robotx, 2) + pow(yi - Roboty, 2)) <= Zmax)
      {
        l[x][y] = l[x][y] + inverseSensorModel(Robotx, Roboty, Robottheta, xi, yi,
            sensorData) - l0;
      }
    }
  }
}

void visualization()
{
  plt::title("Map");
  plt::xlim(0, (int) (mapWidth/gridWidth));
  plt::ylim(0, (int) (mapHeight/gridHeight));

  for (double x = 0; x < mapWidth/gridWidth; x++)
  {
    cout << "Remaining rows: " << mapWidth/gridWidth - x << endl;
    for (double y = 0; y < mapHeight/gridHeight; y++)
    {
      if (l[x][y] == l0) // unknown
      {
        plt::plot({x}, {y}, "g."); // green
      }
      else if (l[x][y] > l0) // occupied
      {
        plt::plot({x}, {y}, "k."); // black
      }
      else // free
      {
        plt::plot({x}, {y}, "r."); // red
      }
    }
  }

  plt::save("../Images/Map.png");
  plt::clf();
}

int main()
{
  double timeStamp;
  double measurementData[8];
  double robotX, robotY, robotTheta;

  FILE * posesFile = fopen("../poses.txt", "r");
  FILE * measurementFile = fopen("../measurement.txt", "r");

  while (fscanf(posesFile, "%lf %lf %lf %lf", &timeStamp, &robotX, &robotY, &robotTheta) != EOF)
  {
    fscanf(measurementFile, "%lf", &timeStamp);
    for (int i = 0; i < 8; i++)
    {
      fscanf(measurementFile, "%lf", &measurementData[i]);
    }
    occupancyGridMapping(robotX, robotY, (robotTheta / 10) * (M_PI / 180),
        measurementData);
  }

  cout << "Wait for the image to generate" << endl;
  visualization();
  cout << "Done!" << endl;

  return 0;
}
