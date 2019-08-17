#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Print 2D vector coordinate values
void print2DVector(vector<vector<int>> vec)
{
  sort(vec.begin(), vec.end());
  for (int i = 0; i < vec.size(); i++)
  {
    for (int j = 0; j < vec[0].size(); j++)
    {
      cout << vec[i][j] << " ";
    }
    cout << endl;
  }
}

vector<vector<int>> minkowski_sum(vector<vector<int>> A, vector<vector<int>> B)
{
  vector<vector<int>> C;
  for (int i = 0; i < A.size(); i++)
  {
    for (int j = 0; j < B.size(); j++)
    {
      vector<int> newPoint = {A[i][0] + B[j][0], A[i][1] + B[j][1]};
      if (find(C.begin(), C.end(), newPoint) == C.end())
      {
        C.push_back(newPoint);
      }
    }
  }

  return C;
}

int main()
{
  // Define coordinates of triangle A and B using 2D vectors
  vector<vector<int>> A; // robot triangle
  A.push_back(vector<int> {0, -1});
  A.push_back(vector<int> {0, 1});
  A.push_back(vector<int> {1, 0});

  vector<vector<int>> B; // obstacle triangle
  B.push_back(vector<int> {0, 0});
  B.push_back(vector<int> {1, -1});
  B.push_back(vector<int> {1, 1});

  // Compute the Minkowski sum of triangle A and B
  vector<vector<int>> C;
  C = minkowski_sum(A, B);

  // Print resulting vector
  print2DVector(C);

  return 0;
}
