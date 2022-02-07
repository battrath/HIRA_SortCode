#ifndef det_
#define det_
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
#include <memory>
#include "TRandom.h"
#include "TTree.h"
#include "TFile.h"
#include "TBranch.h"
#include "TVector3.h"
#include "hira.h"
#include "histo_sort.h"
#include "histo_read.h"
#include "readBeam.h"
#include "readMass.h"
#include "runOptions.h"
#include "correl2.h"
#include "doppler.h"
//#include "forest.h"
//#include "event.h"
#include "constants.h"
#include "S800.h"
#include "ceasar.h"
#include "losses.h"
using namespace std;
/***********************************************************/
/* Brief detector readout **********************************/
/* Unpacks the data stream for physics events **************/
/***********************************************************/
class det {
 public:
  //  det(histo_sort * Histo_sort, forest * Forest);
  det(histo_sort* Histo_sort,histo_read * Histo_read, readBeam *Beams, readMass *Masses, runOptions *Opt);
  int type;
  ~det();
  bool unpack(unsigned short *point,int runno,int sourceID);
  void Reset();
  hira *Hira;
  S800 *s800;
  ringCounter * RingCounter;

  CLosses *losses_fiber;
  CLosses *losses_target;

  ceasar * Ceasar;
  //forest * Forest;
  TRandom * ran;
  void analyze(int event, int run);
  void timehist(int run, float time);

  int Eventt;
  int treelevel;
  int ngood = 0;
  int NS800;

  int Run;
  int NS800_PB;
  int NS800_PBpass;
  int trig[3];
  vector<int> NPB;
  vector<int> NSB[10];
  vector<int> NSB_pro[10][6];
  vector<int> NSB_deuteron[10];
  vector<int> NSB_alpha[10];
  correl2 Correl;

  //void treeGrow();
  //void loadTree(Event * event);

  int CsImult;
  int count = 0;

  int S800ID;
  int SiID;

  bool LoadS800toSolution(int);

 private:
  doppler *Doppler;
  histo_sort * Histo_sort;
  histo_read * Histo_read;
  readBeam * Beams;
  readMass * Masses;
  runOptions *Opt;
};
#endif
