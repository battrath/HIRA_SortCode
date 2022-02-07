#include "fiber.h"
#include <cmath>
#include "TVector3.h"
class XYmon {
 public:
  XYmon(float);
  void setDistance(float);
  void SetFiberThickness(float);
  float GetFiberThickness();

  fiber vert;
  fiber horz;
  bool has_data;
  float x,y,r,theta,phi;
  float thetadeg,phideg;
  TVector3 pos;
  float distance;
  float FiberThickness;
  void reset();
  bool make_2d();
};
