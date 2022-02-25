#include "telescope.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

void telescope::SetNtele(int ntele0){
  ntele = ntele0;
}
/***********************************************************/
/* Constructor *********************************************/
/***********************************************************/
telescope::telescope(TRandom *ran0, runOptions *opt) {
  ran = ran0;
  ifstream fp;
  xActive = opt->xact;
  yActive = opt->yact;
  SetNtele(opt->ntele);
  fp.open(opt->telegeo.c_str());
  while(fp.good()) {
    fp >> itele >> xcenter >> ycenter >> zcenter >> xhoriz >> yhoriz >> zhoriz >> xdiag >> ydiag >> zdiag;
    findVectors(r_center[itele], r_back[itele], r_diag[itele], r_normal[itele], r_front[itele], xcenter, ycenter, zcenter, xhoriz, yhoriz, zhoriz, xdiag, ydiag, zdiag);
  }
  fp.close();
}

//************************************************************//
//** Calculates vector coordinates ***************************//
//************************************************************//
void telescope::findVectors(float * rcenter, float * rback, float * rdiag, float * rnormal, float * rfront, float xcenter0, float ycenter0, float zcenter0, float xhoriz0, float yhoriz0, float zhoriz0, float xdiag0, float ydiag0, float zdiag0) {
  float m_to_cm = 100.; //to convert from meters to centimeters

  rcenter[0] = m_to_cm * xcenter0;
  rcenter[1] = m_to_cm * ycenter0;
  rcenter[2] = m_to_cm * zcenter0;

  float rfront_mag = sqrt(pow(xhoriz0, 2) + pow(yhoriz0, 2) + pow(zhoriz0, 2));

  rfront[0] = xhoriz0 / rfront_mag; //go left to right (beam right point - beam left point)
  rfront[1] = yhoriz0 / rfront_mag;
  rfront[2] = zhoriz0 / rfront_mag;

  rdiag[0] = -xdiag0; // beam left point  - beam right point (this is wrong, this is a later comment)
  rdiag[1] = -ydiag0;
  rdiag[2] = -zdiag0;

  rnormal[0] = rfront[1] * rdiag[2] - rfront[2] * rdiag[1];
  rnormal[1] = rfront[2] * rdiag[0] - rfront[0] * rdiag[2];
  rnormal[2] = rfront[0] * rdiag[1] - rfront[1] * rdiag[0];

  float rnormal_mag = sqrt(pow(rnormal[0], 2) + pow(rnormal[1], 2) + pow(rnormal[2], 2));

  rnormal[0] = rnormal[0] / rnormal_mag;
  rnormal[1] = rnormal[1] / rnormal_mag;
  rnormal[2] = rnormal[2] / rnormal_mag;

  //rback = rfront x rnormal

  rback[0] = rfront[1] * rnormal[2] - rfront[2] * rnormal[1];
  rback[1] = rfront[2] * rnormal[0] - rfront[0] * rnormal[2];
  rback[2] = rfront[0] * rnormal[1] - rfront[1] * rnormal[0];

  float rback_mag = sqrt(pow(rback[0], 2) + pow(rback[1], 2) + pow(rback[2], 2));

  rback[0] = rback[0] / rback_mag;
  rback[1] = rback[1] / rback_mag;
  rback[2] = rback[2] / rback_mag;
}

//************************************************************//
//** Get Theta of Telescope **********************************//
//************************************************************//
float telescope::getTheta(int itele, int ixStrip, int iyStrip) {
  float Random = ran->Rndm();
  float xRecon = ((float)ixStrip + Random)/32.*xActive - xActive/2.;
  Random = ran->Rndm();
  float yRecon = ((float)iyStrip +Random)/32.*yActive - yActive/2.;
  float rRecon[3];
  float rr = 0.;
  for (int i = 0; i < 3; i++) {
    rRecon[i] = r_center[itele][i] + xRecon*r_front[itele][i] + yRecon*r_back[itele][i];
    rr += pow(rRecon[i],2);
  }
  r = sqrt(rr);
  thetaRecon = acos(rRecon[2]/r);
  phiRecon = atan2(rRecon[1],rRecon[0]);
  return thetaRecon;
}

//************************************************************//
//** Print ***************************************************//
//************************************************************//
void telescope::print() {
  for(int i = 0; i < ntele; i++) {
    for(int j = 0; j < 32; j++) {
      for(int k = 0; k < 32; k++) {
        cout << i << "\t" << j << "\t" << k << "\t" << getTheta(i, j, k) << "\t" << phiRecon << "\t" << r << endl;
      }
    }
  }
}
