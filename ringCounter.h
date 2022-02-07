#ifndef ringCounter_
#define ringCounter_

#include "histo_sort.h"
#include "elist.h"
#include "pid.h"
#include <iostream>
#include <sstream>
#include "solution.h"
#include "losses.h"
#include "TRandom.h"
#include "TVector3.h"
#include "pid.h"
#include "parameters.h"
#include "calibrate.h"
#include "constants.h"
#include "elist.h"
#include "readMass.h"
#include "runOptions.h"
struct pieCsiMap {
  int N;
  int NcsiThere;
  int csi[6];
  bool there[6];
};

struct s800_results {
  bool trig_coin;
  bool trig_singles;
  bool trig_s800_singles;
  int Zbeam;
  int Abeam;
  int Zresidue;
  int Aresidue;
  void Reset() {
    trig_coin = false;
    trig_singles = false;
    trig_s800_singles = false;
    Zbeam =-1;
    Abeam=-1;
    Zresidue=-1;
    Aresidue=-1;
  }
};

class ringCounter {
 public:
  ringCounter(TRandom* ran, histo_sort *Histo0, readMass *Masses, float dist2Si, runOptions *opt);
  ~ringCounter();

  static bool const relativity;

  int multProton;
  int multAlpha;
  int multDeuteron;

  int multiHit(elist*,elist*);
  int NestDim;

  void match(elist *, elist *, elist *);
  int NestArray[50];
  int arrayD[50];
  int arrayB[50];
  float deMin;
  int dstripMin;

  void loop(int, elist *, elist *);

  void SetDistance(float);
  void SetTargetThickness(float);
  void SetAlThickness(float);
  float GetTargetThickness();

  void analysis(s800_results,elist,elist,elist);

  void reset();
  int matchWithCsi(elist *);
  void csical(int icsi1, int i2, elist *);

  int Nsolution;
  solution Solution[21];
  pid * Pid[20];

  int csiTimeMin[20];
  int csiTimeMax[20];

  bool proton_present;

  CLosses *losses;
  CLosses *losses_Al;

  float TargetThickness;
  float AlThickness;

 private:
  TRandom *ran;
  histo_sort * Histo;
  readMass * Masses;
  float pie_energy;
  float ring_energy;
  float distance;
  int counter;

  pieCsiMap PieCsiMap[Npie];
  void getMomentum();

  calibrate * calProton;
  calibrate * calDeuteron;
  calibrate * calAlpha;
};
#endif
