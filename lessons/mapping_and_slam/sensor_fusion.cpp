#include <iostream>
#include <math.h>

using namespace std;

const int mapWidth = 2;
const int mapHeight = 2;

void sensorFusion(double m1[][mapWidth], double m2[][mapWidth])
{
  for (int y = 0; y < mapHeight; y++)
  {
    for (int x = 0; x < mapWidth; x++)
    {
      double p = 1 - (1 - m1[y][x]) * (1 - m2[y][x]);
      cout << p << " ";
    }
    cout << endl;
  }
}

int main()
{
  double m1[mapHeight][mapWidth] = {{.9, .6}, {.1, .5}};
  double m2[mapHeight][mapWidth] = {{.3, .4}, {.4, .3}};
  sensorFusion(m1, m2);

  return 0;
}
