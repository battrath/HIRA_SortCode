#ifndef _readMass
#define _readMass
#include<vector>
#include<tuple> // for tuple
#include<iostream>
#include<fstream>
#include "constants.h"
using namespace std;

/***********************************************************/
/** Reads in mass excess lists and calculates mass *********/
/***********************************************************/

class readMass {
  public:
  readMass(string);
  ~readMass();
  int tmpA;
  int tmpZ;
  double tmpMass;
  vector<tuple<int,int,double> > massfile;
  double GetMass(int, int);
};
#endif
