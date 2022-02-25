#ifndef _telescope
#define _telescope

#include <iostream>
#include "TRandom.h"
#include "runOptions.h"
/***********************************************************/
/** Define detecotor geometry ************ *****************/
/***********************************************************/
class telescope {
  public:

  telescope(TRandom* ran, runOptions *opt);
  void findVectors(float*,float*,float*,float*,float*,float,float,float,float,float,float,float,float,float);
  float getTheta(int itele, int ixStrip, int iyStrip);
  void SetNtele(int);
  void print();

  float r_center[20][3]; //!<coordinates of center of the front face (cm)
  float r_normal[20][3]; //!<unit vector perpendicular to the front face (cm)
  float r_front[20][3]; //!<unit vector pointing along front strips (cm)
  float r_back[20][3]; //!<unit vector pointing along back strips (cm)
  float r_diag[20][3]; //!<unit vector pointing along diagonal strips (cm)

  int itele;
  float xcenter;
  float ycenter;
  float zcenter;
  float xhoriz;
  float yhoriz;
  float zhoriz;
  float xdiag;
  float ydiag;
  float zdiag;

  float r, thetaRecon, phiRecon;

  private:

  TRandom *ran;
  int ntele;
  float xActive; //!< in-plane dimension of active region of Si detector (cm)
  float yActive; //!< out-of-plane dimension
};

#endif
