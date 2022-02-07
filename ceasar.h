#include "histo_sort.h"
#include "histo_read.h"
#include "TRandom.h"
#include "caen.h"
#include "TDC1190.h"
#include "calibrate.h"
#include <cmath>
#include "doppler.h"
#include "TVector3.h"
#include "runOptions.h"
struct dataEC {
  int ienergy;
  int id;
  int itime;
  int iRing;
  int iLoc;
  int iqdc;
  int ichan;
  float theta;
  float energy;
  float Total;
  float dop_energy;
  float phi;
  float time;
  TVector3 pos;
};

struct dataTC {
  int itime;
  int id;
  float time;
  int itdc;
  int ichan;
};

struct mapC {
  int iRing;
  int iLoc;
  int iTDC;
  int iTDCChan;
};

class ceasar {
 private:
  TRandom * ran;
  histo_sort * Histo;
  histo_read * Histo_read;
  doppler *Doppler;
  caen Caen;
  TDC1190 ** tdc;

  mapC MapC[6][32];
  calibrate * calCeasar;
  calibrate * calCeasarT;

 public:
  ceasar(TRandom* ran0, histo_sort * Histo0);
  ceasar(TRandom* ran0, histo_read * Histo1);
  ceasar(TRandom* ran0, histo_sort * Histo0,histo_read * Histo1, runOptions *Opt);
  ~ceasar();
  bool unpack(unsigned short *point, int runno);
  void init();

  string chipmap;
  string qdctdc;
  string calfile;
  string posmap;

  dataEC DataEC[192];
  dataTC DataTC[192];

  dataEC select[192];
  dataEC added[192];
  int Nselect;
  int Nadded;

  float angle[10][24]; //10 rings and 24 max detectors
  float angle2[10][24];
  float mag[10][24];
  float Txfp[3];
  int Nxfp;
  float TRF;
  int NE;
  int NT;
  void reset();
};
