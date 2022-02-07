#include "doppler.h"

/***********************************************************/
/* Costructor **********************************************/
/***********************************************************/
doppler::doppler(float velSource0){
  velSource = velSource0;
  gamma = 1./sqrt(1.-pow(velSource,2));
  // read();
}
/***********************************************************/
/* Alternate Constructor ***********************************/
/***********************************************************/
doppler::doppler() {
  read();
}
/***********************************************************/
/* Reads in the detector angles ****************************/
/***********************************************************/
void doppler::read() {
  ifstream f("cal/Nai.ang");
  if (!f.is_open()) {
    cout << "NaI angles not found " << endl;
    abort();
  }
  int k;
  Nnai = 0;
  float thetaDeg;
  for (int i = 0; i < 10; i++) {
    f >> k >> thetaDeg;
    if (f.eof()) break;
    if (f.bad()) break;
    if (k >= 10) break;
    cosTheta[k] = cos(thetaDeg*0.01745);
    Nnai++;
  }
}

/***********************************************************/
/* Returns the gamma energy in the frame of the source *****/
/* specified in the constructor ****************************/
/* Param energy of detected gamma ray **********************/
/* Param theta is angle in radians of detected gamma ray ***/
/***********************************************************/
float doppler::correct(float energy, float theta) {
  return gamma*energy*(1.-velSource*cos(theta));
}

/***********************************************************/
/* Returns the gamma energy in the frame specified by  *****/
/* the velocity *************** ****************************/
/* Param energy of detected gamma ray **********************/
/* Param theta is angle in radians of detected gamma ray ***/
/* Param velocity is the velocity in units of c ************/
/***********************************************************/
float doppler::correct(float energy, float theta, float velocity) {
  float gamma_v = 1./sqrt(1.-pow(velocity,2));
  return gamma_v*energy*(1.-velocity*cos(theta));
}

/***********************************************************/
/* Returns the gamma energy in the frame of the source *****/
/* specified in the constructor ****************************/
/* Param energy of detected gamma ray **********************/
/* Param id is the detector number *************************/
/***********************************************************/
float doppler::correct(float energy, int id) {
  return gamma*energy*(1.-velSource*cosTheta[id]);
}

/***********************************************************/
/* Returns the gamma energy in the frame specified by  *****/
/* the velocity *************** ****************************/
/* Param energy of detected gamma ray **********************/
/* Param id is the detector number *************************/
/* Param velocity is the velocity in units of c ************/
/***********************************************************/
float doppler::correct(float energy, int id, float velocity) {
  float gamma_v = 1./sqrt(1.-pow(velocity,2));
  return gamma_v*energy*(1.-velocity*cosTheta[id]);
}
