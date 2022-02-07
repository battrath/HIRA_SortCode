#ifndef kinematics_
#define kinematics_
#include <cmath>
#include <iostream>

/***********************************************************/
/* Base class for kinematics *******************************/
/***********************************************************/
class CKinematics {
 public:

  CKinematics(){};
  virtual ~CKinematics(){};
  float theta;
  float phi;
  float energy;
  float velocity;
  float vcm[3];
  float velocitycm;
  float Erel;
};
#endif
