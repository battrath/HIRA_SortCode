#include <iostream>
#include <math.h>
using namespace std;
class fiber {
 public:
  fiber(){};
  float A;
  float B;
  float C;
  float D;
  float dynode;
  void reset();
  bool has_data;
  float x;
  float y;
  float total;
  int Mult;
  bool make_2d();
  float position;
};
