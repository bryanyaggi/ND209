#include <iostream>
#include <iomanip>
#include <string.h>
#include <vector>
#include <algorithm>
#include <deque>
#include <fstream>
#include "matplotlibcpp.h"

using namespace std;
namespace plt = matplotlibcpp;

class Map
{
  public:
    /*
    const static int mapWidth = 6;
    const static int mapHeight = 5;
    vector<vector<int>> grid = {
      {0, 1, 0, 0, 0, 0},
      {0, 1, 0, 0, 0, 0},
      {0, 1, 0, 0, 0, 0},
      {0, 1, 0, 0, 0, 0},
      {0, 0, 0, 1, 1, 0}
    };
    */
    const static int mapHeight = 300;
    const static int mapWidth = 150;
    vector<vector<double>> map = getMap();
    vector<vector<int>> grid = mapToGrid();

  private:

    vector<vector<double>> getMap()
    {
      vector<vector<double>> map(mapHeight, vector<double>(mapWidth));
      ifstream mapFile;
      mapFile.open("../map.txt");

      while (!mapFile.eof())
      {
        for (int i = 0; i < mapHeight; i++)
        {
          for (int j = 0; j < mapWidth; j++)
          {
            mapFile >> map[i][j];
          }
        }
      }
      return map;
    }

    vector<vector<int>> mapToGrid()
    {
      vector<vector<int>> grid(mapHeight, vector<int>(mapWidth));
      for (int i = 0; i < mapHeight; i++)
      {
        for (int j = 0; j < mapWidth; j++)
        {
          if (map[i][j] >= 0) // unknown or occupied space
          {
            grid[i][j] = 1; // obstacle
          }
        }
      }
      return grid;
    }
};

class Planner : Map
{
  public:
    /*
    int start[2] = {0, 0};
    int goal[2] = {mapHeight - 1, mapWidth - 1};
    */
    int start[2] = {230, 145};
    int goal[2] = {60, 50};
    int cost = 1;
    string movements_arrows[4] = {"^", "<", "v", ">"};
    vector<vector<int>> movements = {
      {-1, 0},
      {0, -1},
      {1, 0},
      {0, 1}
    };
    vector<vector<int>> heuristic = generateHeuristic();
    vector<vector<int>> path;

  private:

    vector<vector<int>> generateHeuristic()
    {
      vector<vector<int>> heuristic(mapHeight, vector<int>(mapWidth));
      for (int i = 0; i < mapHeight; i++)
      {
        for (int j = 0; j < mapWidth; j++)
        {
          // Manhattan distance
          heuristic[i][j] = abs(goal[0] - i) + abs(goal[1] - j);
        }
      }
      return heuristic;
    }
};

template <typename T>
void print2DVector(T vec)
{
  for (int i = 0; i < vec.size(); i++)
  {
    for (int j = 0; j < vec[0].size(); j++)
    {
      cout << setw(2) << vec[i][j] << " ";
    }
    cout << endl;
  }
}

/*
 * Prints map and planner information
 */
void printInfo(Map map, Planner planner)
{
  cout << "Map:" << endl;
  print2DVector(map.grid);
  cout << "Start: " << planner.start[0] << " , " << planner.start[1] << endl;
  cout << "Goal: " << planner.goal[0] << " , " << planner.goal[1] << endl;
  cout << "Robot Movements: ";
  for (int i = 0;
      i < sizeof(planner.movements_arrows)/sizeof(planner.movements_arrows[0]);
      i++)
  {
    if (i != 0)
    {
      cout << " , ";
    }
    cout << planner.movements_arrows[i];
  }
  cout << endl;
  cout << "Delta:" << endl;
  print2DVector(planner.movements);
}

void visualization(Map map, Planner planner)
{
  plt::title("Path");
  plt::xlim(0, map.mapHeight);
  plt::ylim(0, map.mapWidth);

  // Plot each grid
  for (double x = 0; x < map.mapHeight; x++)
  {
    cout << "Remaining rows: " << map.mapHeight - x << endl;
    for (double y = 0; y < map.mapWidth; y++)
    {
      if (map.map[x][y] == 0) // unknown
      {
        plt::plot({x}, {y}, "g."); // green
      }
      else if (map.map[x][y] > 0) // occupied
      {
        plt::plot({x}, {y}, "k."); // black
      }
      else // free
      {
        plt::plot({x}, {y}, "r."); // red
      }
    }
  }

  // Plot start and goal locations
  plt::plot({(double) planner.start[0]}, {(double) planner.start[1]}, "bo");
  plt::plot({(double) planner.goal[0]}, {(double) planner.goal[1]}, "b*");

  // Plot path
  for (int i = 0; i < planner.path.size(); i++)
  {
    plt::plot({(double) planner.path[i][0]}, {(double) planner.path[i][1]}, "b.");
  }

  plt::save("../Images/Path.png");
  plt::clf();
}

Planner bfs(Map map, Planner planner)
{
  // explored 2D vector
  vector<vector<int>> explored(map.mapHeight, vector<int>(map.mapWidth));
  explored[planner.start[0]][planner.start[1]] = 1;

  // frontier data for starting location
  int x = planner.start[0];
  int y = planner.start[1];
  int g = 0;
  int expandCount = 0;

  // frontier queue
  deque<vector<int>> frontier;
  frontier.push_back(vector<int> {g, x, y});

  // expansion 2D vector
  vector<vector<int>> expansion(map.mapHeight, vector<int>(map.mapWidth, -1));

  // action 2D vector
  vector<vector<int>> action(map.mapHeight, vector<int>(map.mapWidth, -1));
  
  bool done = false;

  while (!done)
  {
    if (frontier.size() == 0)
    {
      done = true;
      cout << "Failed to reach goal." << endl;
    }
    else
    {
      // Pop next frontier location
      vector<int> location = frontier.front();
      frontier.pop_front();
      x = location[1];
      y = location[2];
      g = location[0];
      expansion[x][y] = expandCount;
      expandCount++;

      // Check if goal reached
      if (x == planner.goal[0] && y == planner.goal[1])
      {
        done = true;
        cout << "[" << g << ", " << x << ", " << y << "]" << endl;
      }
      // Add locations to frontier
      else
      {
        for (int i = 0; i < planner.movements.size(); i++)
        {
          int x_new = x + planner.movements[i][0];
          int y_new = y + planner.movements[i][1];
          if (x_new >= 0 && x_new < map.mapHeight
              && y_new >= 0 && y_new < map.mapWidth)
          {
            if (explored[x_new][y_new] == 0 && map.grid[x_new][y_new] == 0)
            {
              int g_new = g + planner.cost;
              frontier.push_back(vector<int> {g_new, x_new, y_new});
              explored[x_new][y_new] = 1;
              action[x_new][y_new] = i;
            }
          }
        }
      }
    }
  }
  cout << "Expansion:" << endl;
  print2DVector(expansion);

  // policy 2D vector
  vector<vector<string>> policy(map.mapHeight, vector<string>(map.mapWidth, "-"));
  if (x == planner.goal[0] && y == planner.goal[1])
  {
    policy[x][y] = "*";
  }

  planner.path.clear();
  while (!(x == planner.start[0] && y == planner.start[1]))
  {
    int x_prev = x - planner.movements[action[x][y]][0];
    int y_prev = y - planner.movements[action[x][y]][1];
    planner.path.push_back(vector<int> {x_prev, y_prev});
    policy[x_prev][y_prev] = planner.movements_arrows[action[x][y]];
    x = x_prev;
    y = y_prev;
  }
  cout << "Policy:" << endl;
  print2DVector(policy);

  return planner;
}

Planner aStar(Map map, Planner planner)
{
  // explored 2D vector
  vector<vector<int>> explored(map.mapHeight, vector<int>(map.mapWidth));
  explored[planner.start[0]][planner.start[1]] = 1;

  // frontier data for starting location
  int x = planner.start[0];
  int y = planner.start[1];
  int g = 0;
  int f = g + planner.heuristic[x][y];
  int expandCount = 0;

  // frontier queue
  deque<vector<int>> frontier;
  frontier.push_back(vector<int> {f, g, x, y});

  // expansion 2D vector
  vector<vector<int>> expansion(map.mapHeight, vector<int>(map.mapWidth, -1));

  // action 2D vector
  vector<vector<int>> action(map.mapHeight, vector<int>(map.mapWidth, -1));
  
  bool done = false;

  while (!done)
  {
    if (frontier.size() == 0)
    {
      done = true;
      cout << "Failed to reach goal." << endl;
    }
    else
    {
      // Pop next frontier location
      sort(frontier.begin(), frontier.end());
      vector<int> location = frontier.front();
      frontier.pop_front();
      x = location[2];
      y = location[3];
      g = location[1];
      f = location[0];
      expansion[x][y] = expandCount;
      expandCount++;

      // Check if goal reached
      if (x == planner.goal[0] && y == planner.goal[1])
      {
        done = true;
        cout << "[" << f << ", " << g << ", " << x << ", " << y << "]" << endl;
      }
      // Add locations to frontier
      else
      {
        for (int i = 0; i < planner.movements.size(); i++)
        {
          int x_new = x + planner.movements[i][0];
          int y_new = y + planner.movements[i][1];
          if (x_new >= 0 && x_new < map.mapHeight
              && y_new >= 0 && y_new < map.mapWidth)
          {
            if (explored[x_new][y_new] == 0 && map.grid[x_new][y_new] == 0)
            {
              int g_new = g + planner.cost;
              int f_new = g_new + planner.heuristic[x_new][y_new];
              frontier.push_back(vector<int> {f_new, g_new, x_new, y_new});
              explored[x_new][y_new] = 1;
              action[x_new][y_new] = i;
            }
          }
        }
      }
    }
  }
  cout << "Expansion:" << endl;
  print2DVector(expansion);

  // policy 2D vector
  vector<vector<string>> policy(map.mapHeight, vector<string>(map.mapWidth, "-"));
  if (x == planner.goal[0] && y == planner.goal[1])
  {
    policy[x][y] = "*";
  }

  planner.path.clear();
  while (!(x == planner.start[0] && y == planner.start[1]))
  {
    int x_prev = x - planner.movements[action[x][y]][0];
    int y_prev = y - planner.movements[action[x][y]][1];
    planner.path.push_back(vector<int> {x_prev, y_prev});
    policy[x_prev][y_prev] = planner.movements_arrows[action[x][y]];
    x = x_prev;
    y = y_prev;
  }
  cout << "Policy:" << endl;
  print2DVector(policy);

  return planner;
}

int main()
{
  Map map;
  Planner planner;

  planner = aStar(map, planner);

  visualization(map, planner);

  return 0;
}
