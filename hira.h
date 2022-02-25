#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "TRandom.h"
#include "histo_sort.h"
#include "histo_read.h"
#include "calibrate.h"
#include "caen.h"
#include "TDC1190.h"
#include "XYmon.h"
#include "ringCounter.h"
#include "parameters.h"
#include "elist.h"
#include "runOptions.h"
using namespace std;

struct dataE {
  int ienergy;
  int id;
  int itele;
  int itime;
  float energy;
};

struct dataT {
  int itime;
  int id;
  int itele;
};

struct mmap {
  bool front;
  int itele;
};

struct cmap {
  int itele;
  int iCsi;
};

class hira {
 public:
  hira(TRandom* ran, histo_sort * Histo0);
  hira(TRandom* ran, histo_read * Histo1);
  hira(TRandom* ran, histo_sort* Histo0, histo_read * Histo1, runOptions *opt);
  int type;
  ~hira();
  void init();
  bool unpack(unsigned short*& point,int runno);
  bool unpackSi_sis(unsigned short*& point);
  bool unpackFiber(unsigned short*& point);
  bool unpackSi_HINP4(unsigned short*& point);
  bool unpackSi_adc(unsigned short*& point);
  bool unpackCsi(unsigned short*& point,int runno);
  void analyze();
  void reset();

  elist Pie;
  elist Ring;
  elist Csi;

  XYmon *   XY_mon;

  int Np;
  int N6;
  int Nfiber;
  int NHira;
  float T_CsiTrig;
  string S4RingMap;
  string S4PieMap;
  string RingCal;
  string PieCal;
  string CsIChMap;
  string ProtonCal;
  float dist2Si;
  int Ntele;
  int Nstrip;
  int Ncsi;
 private:
  TRandom * ran;
  unsigned short xmarker[3];

  int RingMap[4][32];
  int RingTeleMap[4][32];
  int PieMap[4][32];
  int PieTeleMap[4][32];
  int CsIMap[60];
  int CsITeleMap[60];

  unsigned short chanXLM[3][400];
  unsigned short nXLM[3];

  histo_sort * Histo;
  histo_read * Histo_read;
  caen ADC;
  //  caen TDC;
  TDC1190 *TDC;

  dataE DataE[56];
  dataT DataT[56];

  calibrate * calCsi;
  calibrate * calPies;
  calibrate * calRings;

  int NE;
  int NT;

  //  int BlockerESum;
  //bool haveBlocker;

  //high-low correlation
  double fsumN[14][32];
  double fsumx[14][32];
  double fsumxx[14][32];
  double fsumy[14][32];
  double fsumyx[14][32];

  double bsumN[14][32];
  double bsumx[14][32];
  double bsumxx[14][32];
  double bsumy[14][32];
  double bsumyx[14][32];

  bool fred;

  float T_RFSCIN;
  float T_A1900;
  float T_RFCYC;

};
