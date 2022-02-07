#include "XYmon.h"


XYmon::XYmon(float dist2Si) {
  distance = dist2Si + 7.6; //cm
}

void XYmon::setDistance(float dist2Si) {
  distance = dist2Si + 7.6; //cm
}

void XYmon::SetFiberThickness(float thick0) {
  FiberThickness = thick0;
}

float XYmon::GetFiberThickness() {
  return FiberThickness;
}

void XYmon::reset() {
  vert.reset();
  horz.reset();
  x = -999;
  y = -999;
  theta = -999;
  phi = -999;
  thetadeg = -999;
  phideg = -999;
  has_data = false;
  pos.Clear();
  //  bool make_2d();
}


bool XYmon::make_2d(){

  vert.make_2d();
  horz.make_2d();
  has_data = vert.has_data && horz.has_data;

  //position in mm
  x = horz.position/6400.*16.-8.;
  y = vert.position/6400.*16.-8.;

  x = x;
  y = y;

  r = sqrt(pow(x,2) + pow(y,2));
  r /= 10.;  //cm
  theta = atan(r/distance);
  phi = atan2(y,x);
  pos.SetMagThetaPhi(r*10, theta, phi);
  thetadeg = theta*180./acos(-1);
  phideg = phi*180./acos(-1);
  return has_data;
}
