#include "readMass.h"

/***********************************************************/
/* Constructor *********************************************/
/***********************************************************/
readMass::readMass(string file) {
  ifstream fp;
  fp.open(file.c_str());
  int tmpZ, tmpA;
  double tmpMass;
  while(fp.good()) {
    fp >> tmpZ >> tmpA >> tmpMass;
    if(fp.eof()) break;
    massfile.push_back(make_tuple(tmpZ,tmpA,tmpMass/1000.));
  }
}
/***********************************************************/
/* Destructor **********************************************/
/***********************************************************/

readMass::~readMass() {
  massfile.clear();
}

/***********************************************************/
/* Calculats mass given Z and A ****************************/
/***********************************************************/
double readMass::GetMass(int Z, int A) {
  for(unsigned int i = 0; i < massfile.size(); i++) {
    if(get<0>(massfile[i]) == Z && get<1>(massfile[i]) == A) return A + get<2>(massfile[i])/m0;
  }
  return -1;
}
