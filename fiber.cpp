#include "fiber.h"

void fiber::reset() {
  A = -1.;
  B = -1.;
  C = -1.;
  D = -1.;
  dynode = -1.;
  total = -1.;
  x = -1000.;
  y = -100.;
  position = -100.;
  has_data = false;
  Mult = 0;
}

bool fiber::make_2d() {
  if(A>0.) Mult++;
  if(B>0.) Mult++;
  if(C>0.) Mult++;
  if(D>0.) Mult++;
  if (A > 0. && B > 0. && C > 0. && D> 0.) {
    total = A + B + C + D;
//     x = (B+D)/total*500.;
//     y = (C+D)/total*500.;
    //      x = ((A-B)/(A+B))+((C-D)/(C+D))*500.;
    //y = ((B-D)/(B+D))+((A-C)/(A+C))*500.;
    x = ((A+B)-(C+D))/total*500.;
    y = ((C+A)-(D+B))/total*500.;
    // position goes from 0 to 8000
    int iy = (int)round(y/118. + 3.5);
    if (iy%2) position = (400.-x)+ (float)iy*800.;
    else position = (x+400.) + (float)iy*800;
    has_data = true;
  }
  return has_data;
}
