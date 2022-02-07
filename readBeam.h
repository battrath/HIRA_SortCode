#ifndef _readBeam
#define _readBeam
#include "TMath.h"
#include <dirent.h>
#include <fstream>
#include <iostream>
using namespace std;

/***********************************************************/
/** Reads in secondary beams and residuals *****************/
/***********************************************************/

class readBeam {
  public:
  vector<string> pb;        //Primary Beams
  vector<string> sb[10];    //Secondary Beams
  readBeam();
  ~readBeam();
  string isotope(int);
  int BeamID(int, int, vector<string>);
  int GetZ(string);
  int GetA(string);
};
#endif
