#include "correl2.h"

correl2::correl2() {

}
//*********************************************//
void correl2::Mask(bool mask0, bool mask1) {
  SetMask(0, mask0);
  SetMask(1, mask1);
}
//*********************************************//
void correl2::Mask(bool mask0, bool mask1, bool mask2) {
  SetMask(0, mask0);
  SetMask(1, mask1);
  SetMask(2, mask2);
}
//*********************************************//
int correl2::GetMask(int i) {
  return parPair[i].second;
}
//*********************************************//
void correl2::SetMask(int i, bool mask) {
  parPair[i].second = mask;
}
//*********************************************//
int correl2::GetProtonMult() {
  int mult = 0;
  for(unsigned int i = 0; i < parPair.size(); i++) {
    if(parPair[i].first->iZ == 1 && parPair[i].first->iA == 1) mult++;
  }
  return mult;
}
//*********************************************//
int correl2::GetAlphaMult() {
  int mult = 0;
  for(unsigned int i = 0; i < parPair.size(); i++) {
    if(parPair[i].first->iZ == 2 && parPair[i].first->iA == 4) mult++;
  }
  return mult;
}
//*********************************************//
int correl2::GetDeuteronMult() {
  int mult = 0;
  for(unsigned int i = 0; i < parPair.size(); i++) {
    if(parPair[i].first->iZ == 1 && parPair[i].first->iA == 2) mult++;
  }
  return mult;
}
//*********************************************//
void correl2::addPar(solution *fragment) {
  parPair.push_back( make_pair(fragment,true) );
}

//*********************************************//
void correl2::makeParArray() {
  N = 0;
  for(unsigned int i = 0; i < parPair.size(); i++) {
    if(parPair[i].second) {
      frag[N] = parPair[i].first;
      N++;
    }
  }
}
//*********************************************//
void correl2::reset() {
  parPair.clear();
}

//**************************************************************//
/**
 * Finds the total kinetic energy of the fragments
 * in there center-of-mass frame.
 */
float correl2::findErel() {
  //first find total momentum
  for (int i=0;i<3;i++) Mtot[i] = 0.;
  energyTot = 0.;   // total energy for relativity, total mass for newton
  for (int i = 0; i < N; i++) {
    energyTot += frag[i]->energyTot;
    if(frag[i]->mass > 1000000) abort();
    for (int j = 0; j < 3; j++) {
      Mtot[j] += frag[i]->Mvect[j];
    }
  }

  momentumCM = 0.;
  for (int j = 0; j < 3; j++) momentumCM += pow(Mtot[j],2);
  momentumCM = sqrt(momentumCM);

  //transform to average

  float velC[3]={0.,0.,10.6636}; //20Mg after Be tgt
  Kinematics.transformMomentum(Mtot,velC, energyTot,momC);
  float mmc = 0.;
  for (int i = 0; i < 3; i++) mmc += pow(momC[i],2);
  mmc = sqrt(mmc);
  cosThetaC = momC[2]/mmc;
  PperpC = sqrt(pow(momC[0],2)+pow(momC[1],2));
  PparaC = momC[2];
  PtotC = sqrt(pow(momC[0],2)+pow(momC[1],2)+pow(momC[2],2));

  //velocity of cemter of mass
  velocityCM = momentumCM*Kinematics.c/energyTot;

  float velCM[3]={0.};
  for (int j = 0 ; j < 3; j++) {
    velCM[j] = velocityCM/momentumCM*Mtot[j];
  }
  thetaCM = acos(velCM[2]/velocityCM);
  phiCM = atan2(velCM[1],velCM[0]);

  totalKE = 0.;
  check_mass = 0.;
  check_A = 0;
  check_Z = 0;
  for (int i = 0; i < N; i++) {
    float eKinNew = Kinematics.transformMomentum(frag[i]->Mvect,velCM,frag[i]->energyTot,frag[i]->MomCM);
    frag[i]->energyCM = eKinNew - Kinematics.scale*frag[i]->mass;
    totalKE += eKinNew - Kinematics.scale*frag[i]->mass;
    check_ke = eKinNew - Kinematics.scale*frag[i]->mass;
    check_mass += Kinematics.scale*frag[i]->mass;
    check_A += frag[i]->iA;
    check_Z += frag[i]->iZ;
  }

  if (N == 3) {
    float dot = 0.;
    float mm = 0.;
    for (int j = 0; j < 3; j++) {
      dot += frag[2]->MomCM[j]*momC[j];
      mm += pow(frag[2]->MomCM[j],2);
    }
    mm = sqrt(mm);
    cosAlphaQ = dot/mm/PtotC;
  }
  return totalKE;
}
//***********************************************************
void correl2::getJacobi(){
  for (int i = 0; i < 3; i++) {
    frag[i]->momentumCM = 0.;
    for (int k = 0; k < 3; k++) frag[i]->momentumCM += pow(frag[i]->MomCM[k],2);
    frag[i]->momentumCM = sqrt(frag[i]->momentumCM);
  }

  //alpha is the  third fragment
  //first JacobiT
  float dot = 0.;
  float pp[3] = {0.};
  float PP = 0.;
  for (int k = 0;k < 3; k++) {
    pp[k] = frag[0]->MomCM[k] - frag[1]->MomCM[k];
    PP += pow(pp[k],2);
    dot += pp[k]*frag[2]->MomCM[k];
  }
  PP = sqrt(PP);
  cosThetaT = dot/PP/frag[2]->momentumCM;
  dot = 0;
  double PP1 = 0;
  double pp1[3]={0.};
  for (int k = 0; k < 3; k++) {
    pp1[k] = frag[0]->MomCM[k]/frag[0]->mass - frag[2]->MomCM[k]/frag[2]->mass;
    PP1 += pow(pp1[k],2);
    dot += pp1[k]*frag[1]->MomCM[k];
  }
  PP1 = sqrt(PP1);
  cosThetaY[0] = -dot/PP1/frag[1]->momentumCM;

  dot = 0;
  double PP2 = 0;
  double pp2[3]={0.};
  for (int k = 0; k < 3; k++) {
    pp2[k] = frag[1]->MomCM[k]/frag[1]->mass - frag[2]->MomCM[k]/frag[2]->mass;
    PP2 += pow(pp2[k],2);
    dot += pp2[k]*frag[0]->MomCM[k];
  }
  PP2 = sqrt(PP2);
  cosThetaY[1] = -dot/PP2/frag[0]->momentumCM;
  cosThetaV = (pp1[0]*pp2[0] + pp1[1]*pp2[1] + pp1[2]*pp2[2])/PP1/PP2;
}
