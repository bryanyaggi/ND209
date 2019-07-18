#include <iostream>

using namespace std;

int main()
{
  // Given P(POS), P(DOOR|POS), and P(DOOR|~POS)
  double a = .0002; // P(POS)
  double b = .6; // P(DOOR|POS)
  double c = .05; // P(DOOR|~POS)

  // Compute P(~POS) and P(POS|DOOR)
  double d = 1 - a; // P(~POS)
  double e = b * a / (b * a + c * d); // P(POS|DOOR)

  cout << "P(POS|DOOR) = " << e << endl;

  return 0;
}
