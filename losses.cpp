#include "losses.h"
#include <algorithm>
#include <cmath>

using namespace std;
/***********************************************************/
/* Constructor *********************************************/
/***********************************************************/
CLosses::CLosses(int Zmax0,string suffix) {
  Zmax = Zmax0;
  string filename;
  string path = "LossFiles/";
  loss = new CLoss2 * [Zmax];
  for (int iZ = 1;iZ < Zmax;iZ++) {
    if (iZ ==  1)       filename = path + "Hydrogen" + suffix;
    else if (iZ == 2)   filename = path + "Helium" + suffix;
    else if (iZ == 3)   filename = path + "Lithium" + suffix;
    else if (iZ == 4)   filename = path + "Beryllium" + suffix;
    else if (iZ == 5)   filename = path + "Boron" + suffix;
    else if (iZ == 6)   filename = path + "Carbon" + suffix;
    else if (iZ == 7)   filename = path + "Nitrogen" + suffix;
    else if (iZ == 8)   filename = path + "Oxygen" + suffix;
    else if (iZ == 9)   filename = path + "Fluorine" + suffix;
    else if (iZ == 10)  filename = path + "Neon" + suffix;
    else {
      cout << "loss file for Z = " << iZ << " not found "<< endl;
      break;
    }
    loss[iZ] = new CLoss2 (filename);
  }
}
/***********************************************************/
/* Alternate Constructor ***********************************/
/***********************************************************/
CLosses::CLosses(int Zmax0,string suffix,bool S800) {
  Zmax = Zmax0;
  //  cout << Zmax << endl;
  string filename;
  string path = "LossFiles/";
  loss = new CLoss2 * [Zmax];
  for (int iZ = 1;iZ < Zmax;iZ++) {
    if (iZ == 1)  filename = path + "Hydrogen" + suffix;
    else if (iZ == 2)  filename = path + "Helium" + suffix;
    else if (iZ >= 3 && iZ <= 11)  filename = path + "Sodium" + suffix;
    else if (iZ == 12)  filename = path + "Magnesium" + suffix;
    else if (iZ == 13)  filename = path + "Aluminium" + suffix;
    else if (iZ == 14)  filename = path + "Silicon" + suffix;
    else if (iZ == 15)  filename = path + "Phosphorus" + suffix;
    else if (iZ == 16)  filename = path + "Sulfer" + suffix;
    else if (iZ == 17)  filename = path + "Chlorine" + suffix;   //mira
    else {
      cout << "loss file for Z = " << iZ << " filename = " << filename << " not found "<< endl;
      break;
    }
    loss[iZ] = new CLoss2 (filename);
  }
}
CLosses::~CLosses() {
  for (int iZ = 1;iZ<Zmax;iZ++) delete loss[iZ];
  delete loss;
}


//**********************************************************
float CLosses::getEin(float energy, float thick,int Z,int A) {
  if (Z > Zmax) {
    cout << " no loss info for Z = " << Z << endl;
    abort();
  }
  float energyi = loss[Z]->getEin(energy,thick, A);
  return energyi;
}
//**********************************************************
float CLosses::getEout(float energy, float thick,int Z,int A) {
  if (Z > Zmax) {
    cout << " no loss info for Z = " << Z << endl;
    abort();
  }
  return loss[Z]->getEout(energy,thick, A);
}
