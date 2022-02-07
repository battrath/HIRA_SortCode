#include "ZApar.h"

/***********************************************************/
/* Constructor *********************************************/
/***********************************************************/
ZApar::ZApar(std::ifstream& ifile){
  ifile >> Z >> A;
  mass = (float)A;
  ifile >> n;  // number of points
  x = new float [n];
  y = new float [n];
  for (int i = 0; i < n; i++) {
    ifile >> x[i] >> y[i];
  }
}
/***********************************************************/
/* Destructor **********************************************/
/***********************************************************/
ZApar::~ZApar() {
  delete [] x;
  delete [] y;
}
/***********************************************************/
/* Returns true if particle is in banana gate **************/
/***********************************************************/
bool ZApar::inBanana(float xx, float yy) {
  if (TMath::IsInside(xx,yy,n,x,y)) return true;
  else return false;
}
