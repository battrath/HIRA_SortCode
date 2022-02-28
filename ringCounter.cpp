#include "ringCounter.h"

bool const ringCounter::relativity = 1;

void ringCounter::SetDistance(float dist0) {
  distance = dist0;
}
void ringCounter::SetTargetThickness(float thick0) {
  TargetThickness = thick0;
}
void ringCounter::SetAlThickness(float althick0) {
  AlThickness = althick0;
}
float ringCounter::GetTargetThickness() {
  return TargetThickness;
}

ringCounter::ringCounter(TRandom * ran0, histo_sort * Histo1, readMass * Masses0, float dist0, runOptions *opt) {
  ran = ran0;
  Histo = Histo1;
  distance = dist0;
  Masses = Masses0;

  sidet = opt->sidet;
  Ntele = opt->ntele;
  Npie = opt->npie;
  Nstrip = opt->nring;
  Ncsi = opt->ncsi;

  counter = 0;
  string name;
  ostringstream outstring;
  for(int i = 0; i < Ntele; i++) {
    for (int j = 0; j <Ncsi; j++) {
      Pid[i][j] = new pid(Form("CsIpidT%i_%i", i, j));
    }
  }

  //loading energy loss tables for target
  losses = new CLosses(11,".loss");
  //loading energy loss table fo rthe Al plate
  losses_Al = new CLosses(3,"_Al.loss");

  //Proton Calibrations
  calProton = new calibrate(Ntele, Ncsi, opt->pcal, 1);
  calDeuteron = new calibrate(Ntele, Ncsi, opt->dcal, 1);
  calAlpha = new calibrate(Ntele, Ncsi, opt->acal, 1);

  //timw gates for s800 coincidence
  int one,two,three;
  ifstream fileTime(opt->csitime);
  for (int i = 0;i < 20; i++) {
    fileTime >> one >> two >> three;
    csiTimeMin[one]= two;
    csiTimeMax[one]= three;
  }
  fileTime.close();
  Telescope = new telescope(ran, opt);
}
//*****************************************//
void ringCounter::reset() {

  //reset the solutions
  for(int i = 0;i < Nsolution; i++) {
    Solution[i].denergy = 0;
    Solution[i].energy = 0;
    Solution[i].penergy = 0;
    Solution[i].energyR =0;
    Solution[i].energyCsI = 0;
    Solution[i].icsi =-1;
    Solution[i].iPie =-1;
    Solution[i].iRing = -1;
    Solution[i].itele = -1;
    Solution[i].mass = 0;
    Solution[i].Ekin = 0;
    Solution[i].iZ =-1;
    Solution[i].iA = -1;
    Solution[i].energyTot = 0;
    for(int j = 0;j < 3; j++) {
      Solution[i].Vvect[j] = 0;
      Solution[i].Mvect[j] = 0;
      Solution[i].MomCM[j] = 0;
    }
    Solution[i].energyCM = 0;
    Solution[i].momentum = 0;
    Solution[i].momentumCM =0;
    Solution[i].pos.Clear();
  }
  Nsolution = 0;
}

//*************************************//
//void ringCounter::analysis(s800_results S800_results) {
void ringCounter::analysis(s800_results S800_results, elist Pie, elist Ring, elist Csi) {
  counter++;
  proton_present = false;
  Histo->RingsMult->Fill(Ring.Nstore);
  Histo->PiesMult->Fill(Pie.Nstore);
  Histo->CsIMult->Fill(Csi.Nstore);

  Nsolution = 0;
  //must have pie,ring
  if (!(Pie.Nstore >0 && Ring.Nstore > 0)) return;
  Ring.Neighbours();
  //  Pie.NeighboursPies();

  Ring.Threshold(0.4);
  Pie.Threshold(0.4);

  if(!(Pie.Nstore>0 && Ring.Nstore>0)) return;

  Histo->PievsRing->Fill(Pie.Order[0].energy,Ring.Order[0].energy);
  Histo->EdiffvsPie->Fill(Pie.Order[0].energy,Pie.Order[0].energy-Ring.Order[0].energy);

  // add-back histogram fill stuff =============================================
  // ==================================================================
  //added by jinyu may222019
  for(Int_t iorder=0;iorder<Pie.Nstore;iorder++) {
    if(Pie.Order[iorder].neighbours==0) {
      Histo->Pienn->Fill(Pie.Order[iorder].energy);
    }
    if(Pie.Order[iorder].neighbours==1) {
      Histo->Pieabwn1->Fill(Pie.Order[iorder].energy);
      Histo->PieEadd1E0->Fill(Pie.Order[iorder].energyMax,Pie.Order[iorder].energy-Pie.Order[iorder].energyMax);
    }
    if(Pie.Order[iorder].neighbours==2) {
      Histo->Pieabwn2->Fill(Pie.Order[iorder].energy);
      Histo->PieEadd2E0->Fill(Pie.Order[iorder].energyMax,Pie.Order[iorder].energy-Pie.Order[iorder].energyMax);
    }
  }
  for(Int_t iorder=0;iorder<Ring.Nstore;iorder++) {
    if(Ring.Order[iorder].neighbours==0) {
      Histo->Ringnn->Fill(Ring.Order[iorder].energy);
    }
    if(Ring.Order[iorder].neighbours==1) {
      Histo->Ringabwn1->Fill(Ring.Order[iorder].energy);
      Histo->RingEadd1E0->Fill(Ring.Order[iorder].energyMax,Ring.Order[iorder].energy-Ring.Order[iorder].energyMax);
    }
    if(Ring.Order[iorder].neighbours==2) {
      Histo->Ringabwn2->Fill(Ring.Order[iorder].energy);
      Histo->RingEadd2E0->Fill(Ring.Order[iorder].energyMax,Ring.Order[iorder].energy-Ring.Order[iorder].energyMax);
    }
  }
  //end of added by jinyu May222019
  // ==================================================================
  // ==================================================================
  for(int i = 0;i < Pie.Nstore;i++) {
    Histo->PCSum_AfterAddback->Fill(Pie.Order[i].strip,Pie.Order[i].energy);
    Histo->EpiesC_AfterAddback[Pie.Order[i].tele][Pie.Order[i].strip]->Fill(Pie.Order[i].energy);
  }

  for(int i = 0;i < Ring.Nstore; i++) {
    Histo->RCSum_AfterAddback->Fill(Ring.Order[i].strip,Ring.Order[i].energy);
    Histo->EringsC_AfterAddback[Ring.Order[i].tele][Ring.Order[i].strip]->Fill(Ring.Order[i].energy);
  }
  if (!(Pie.Nstore >0 && Ring.Nstore > 0 && Csi.Nstore >0)) return;

  //check mapping
  if (Csi.Nstore == 1) {
    int id_csi = Csi.Order[0].strip;
    int id_tele = Csi.Order[0].tele;
    Histo->csiRing[id_tele]->Fill(id_csi, Ring.Order[0].strip);
    Histo->csiPie[id_tele]->Fill(id_csi, Pie.Order[0].strip);
  }
  int Ncsi_before = Csi.Nstore;
  //remove csi with large times // random S800 Csi coincidences
  for (int i = Csi.Nstore-1 ; i >= 0 ; i--) {
    if (Csi.Order[i].time/10. > csiTimeMax[Csi.Order[i].strip]) Csi.Remove(i);
  }
  if (Csi.Nstore == 0) return;

  //E-De maps for S800 coincidences
  if (Csi.Nstore >=1 && S800_results.trig_coin) {
    int id_csi = Csi.Order[0].strip;
    int id_tele = Csi.Order[0].tele;
    for (int ipi = 0; ipi < 1; ipi++) {
      if (id_csi < 16) {
        int id_csi2 = floor((float)(Pie.Order[ipi].strip+4)/8.) ;
        if (id_csi2 == 16) id_csi2 = 0.;
        if (id_csi == id_csi2) Histo->dee_S800[id_tele][id_csi]->Fill(Csi.Order[0].energyR,Pie.Order[ipi].energy);
      } else {
        int  id_csi2 = floor((float)Pie.Order[ipi].strip/32.) + 16;
        if (id_csi == id_csi2) Histo->dee_S800[id_tele][id_csi]->Fill(Csi.Order[0].energyR,Pie.Order[ipi].energy);
      }
    }
  }

  int NsiHits = min(Pie.Nstore,Ring.Nstore);

  // match pies and strips
  multiHit(&Pie, &Ring);
  // match si solution with Csi possibilities
  int Nmatched = matchWithCsi(&Csi);

  //look for ambiguities, i.e.,  if there were two possible Si solutions
  // and the two Si-Pie energies are very similar, maybe the match ring-pie
  //combinations are swapped by mistake
  if (Nmatched  == 0 && Nsolution == 2 && Csi.Nstore == 2) {
    if (abs(Solution[0].denergy-Solution[1].denergy)< .3) {
      int ring0 = Solution[0].iRing;
      int ring1 = Solution[1].iRing;
      Solution[0].iRing = ring1;
      Solution[1].iRing = ring0;
      Nmatched = matchWithCsi(&Csi);
    }
  }

  for(int i = 0; i < Nsolution; i++) {
    int icsi = Solution[i].icsi;
    int itele = Solution[i].itele;
    if(icsi >=0) Histo->dee[itele][icsi]->Fill(Solution[i].energyR,Solution[i].denergy);
//    if(icsi >=0 && S800_results.trig_coin) Histo->dee_S800[icsi]->Fill(Solution[i].energyR,Solution[i].denergy);
  }

  //Get the kinematics for the detected and identified particles
  getMomentum();
}

//***************************************************//
//extracts multiple particle from strip data
int ringCounter::multiHit(elist *Pie, elist *Ring) {
  int Ntries = min(Ring->Nstore,Pie->Nstore);
  if (Ntries > 6) Ntries =6;
  Nsolution = 0;
  if (Ntries <= 0) return 0;

  for (NestDim = Ntries; NestDim > 0; NestDim--) {
    dstripMin = 1000;
    deMin = 10000.;
    //look for best solution
    loop(0, Pie, Ring);
    //check to see if best possible solution is reasonable
    int leave = 0;
    for (int i = 0; i < NestDim; i++) {
      pie_energy = Pie->Order[arrayB[i]].energy;
      ring_energy = Ring->Order[i].energy;
      float accept = ring_pie_matching_acceptance;
      if(pie_energy < 10. && pie_energy >0.) accept =1.5/pie_energy;
      if (fabs(pie_energy-ring_energy) >pie_energy*accept) {
        leave = 1;
        break;
      }
    }
    if (leave) continue;
    // now load solution
    for (int i = 0;i < NestDim; i++) {
      pie_energy = Pie->Order[i].energy;
      Solution[i].denergy = pie_energy;
      Solution[i].iRing = Ring->Order[i].strip;
      Solution[i].iPie= Pie->Order[arrayB[i]].strip;
      Solution[i].itele= Pie->Order[arrayB[i]].tele;
    }
    Nsolution = NestDim;
    break;
  }

  int nomatch = 1;
  if(Nsolution != Ntries){
    nomatch = 0;
  }
  Histo->S4SF->Fill(nomatch);
  return Nsolution;
}

//***************************************************//
//recursive subroutine  used for multihit subroutine
void ringCounter::loop(int depth, elist *Pie, elist *Ring) {
  if (depth == NestDim ) {
    // do stuff here
    int dstrip = 0;
    float de = 0.;
    for (int i = 0; i < NestDim; i++) {
      pie_energy = Pie->Order[NestArray[i]].energy;
      ring_energy = Ring->Order[i].energy;
      de += abs(pie_energy-ring_energy);
    }
    if (dstrip < dstripMin) {
      dstripMin = dstrip;
      for (int i = 0; i < NestDim; i++) arrayD[i] = NestArray[i];
    }
    if (de < deMin) {
      deMin = de;
      for (int i = 0;i < NestDim; i++) arrayB[i] = NestArray[i];
    }
    return;
  }
  for (int i = 0;i < NestDim; i++) {
    NestArray[depth] = i;
    int leave = 0;
    for (int j=0;j<depth;j++) {
      if (NestArray[j] == i) {
        leave = 1;
        break;
      }
    }
    if (leave) continue;
    loop(depth+1, Pie, Ring);
  }
}
//********************************************//
ringCounter::~ringCounter() {
  delete losses;
  delete losses_Al;
  delete calProton;
  return;
}


int ringCounter::matchWithCsi(elist *Csi){
  if(sidet == "S4") return matchWithS4Csi(Csi);
  else if(sidet == "HIRA") return matchWithHIRACsi(Csi);
  else {
    cout << "No SI Match Found" << endl;
    return 0;
  }
}
//*********************************************//
int ringCounter::matchWithS4Csi(elist *Csi){
  multProton = 0;
  multAlpha = 0;
  multDeuteron = 0;
  // match Si and Csi
  int Nmatched = 0;
  for (int i = 0; i < Nsolution; i++) {
    float radius = (float)Solution[i].iRing/(float)Nstrip * (Si_r_max-Si_r_min)+Si_r_min;
    float ring_width = (Si_r_max-Si_r_min)/(float) Nstrip;
    float phi_width = (2*TMath::Pi())/Npie;
    double sep = ring_width*0.025;
    double r1 = radius - ring_width*0.5 + sep;
    double r2 = radius - ring_width*0.5 + sep;
    radius = sqrt(gRandom->Uniform(r1*r1, r2*r2));
    double phi = (((float)Solution[i].iPie+ran->Rndm())/(float)Npie)*(acos(-1.)*2.);
    double sepphi = sep/radius;
    phi = gRandom->Uniform(phi - phi_width * 0.5 + sepphi, phi + phi_width * 0.5 - sepphi);
    double theta = atan(radius/distance);
    double mag = (distance*10)/cos(theta);

    Solution[i].theta = theta;
    Solution[i].phi = phi;
    Solution[i].pos.SetMagThetaPhi(mag,theta,phi);
    int id_csi,id_csi_plus,id_csi_minus;

    if (Solution[i].iRing < 51) { //inner ring
      id_csi = floor((float)(Solution[i].iPie+4)/32.) + 16;
      if (id_csi > 19) id_csi -= 4.;
      //in case pies and Csi are not lined up exactly
      // consider Csi if pie is off by plus-minus one
      id_csi_plus = floor((float)(Solution[i].iPie+1+4)/32.) + 16;
      if (id_csi_plus > 19) id_csi_plus -= 4.;
      id_csi_minus = floor((float)(Solution[i].iPie-1+4)/32.) + 16;
      if (id_csi_minus > 19) id_csi_plus -= 4.;
    }
    else { // outer ring
      id_csi = floor((float)(Solution[i].iPie+4)/8.) ;
      if (id_csi == 16) id_csi = 0.;
      id_csi_plus = floor((float)(Solution[i].iPie+1+4)/8.) ;
      if (id_csi_plus == 16) id_csi_plus = 0.;
      id_csi_minus = floor((float)(Solution[i].iPie-1+4)/8.) ;
      if (id_csi_minus == 16) id_csi_minus = 0.;
    }
    bool found = false;
    int Nfound = 0;
    for (int icsi = 0;icsi<Csi->Nstore;icsi++) {
      Solution[i].ipid = 0;
      if (Csi->Order[icsi].strip == id_csi || Csi->Order[icsi].strip == id_csi_plus || Csi->Order[icsi].strip == id_csi_minus ) {
        Solution[i].energyR = Csi->Order[icsi].energyR;
        Solution[i].icsi = Csi->Order[icsi].strip;
        found = true;
        Nfound++;
        Nmatched++;
        bool FoundPid = Pid[Csi->Order[icsi].tele][Csi->Order[icsi].strip]->getPID(Solution[i].energyR, Solution[i].denergy);
        float x = Solution[i].theta*cos(Solution[i].phi)*180./acos(-1.);
        float y = Solution[i].theta*sin(Solution[i].phi)*180./acos(-1.);
        if(!FoundPid) continue;
        int Z = Pid[Csi->Order[icsi].tele][Csi->Order[icsi].strip]->Z;
        int A = Pid[Csi->Order[icsi].tele][Csi->Order[icsi].strip]->A;
        if (Pid[Csi->Order[icsi].tele][Csi->Order[icsi].strip]->Z == 1 && Pid[Csi->Order[icsi].tele][Csi->Order[icsi].strip]->A == 1) {
          proton_present = true;
          multProton++;
          Solution[i].ipid = 1;
          Histo->protonHitMap->Fill(x,y);
        } else if (Pid[Csi->Order[icsi].tele][Csi->Order[icsi].strip]->Z == 2 && Pid[Csi->Order[icsi].tele][Csi->Order[icsi].strip]->A == 4){
   	  multAlpha++;
	} else if (Pid[Csi->Order[icsi].tele][Csi->Order[icsi].strip]->Z == 1 && Pid[Csi->Order[icsi].tele][Csi->Order[icsi].strip]->A == 2) {
	  multDeuteron++;
        }
	csical(icsi,i,Csi);
	float sumEnergy = Solution[i].denergy + Solution[i].energy; //Si + CsI in MeV
        if(1) {
          //calculate energy lost in Al plate
	  float Althick = AlThickness/cos(Solution[i].theta); //602.1 mg/cm2 Al plate (2.23 mm)
	  float Ebeforetarget = losses_Al->getEin(sumEnergy,Althick,Z,A);
	  sumEnergy = Ebeforetarget;
	  //Calculate the energy lost in the target
	  float thick = TargetThickness/2./cos(Solution[i].theta); //0.540 mm Be target -> 99.79 mg/cm2
	  float ein = losses->getEin(sumEnergy,thick,Z,A);
	  Solution[i].Ekin = ein;
	  Solution[i].iZ = Z;
	  Solution[i].iA = A;
	  Solution[i].mass = Masses->GetMass(Z,A);
	}
      } //csi match if
    } //loop over Csi
    if (Nfound > 1) {
      return 0;
    }
    Histo->SiMap->Fill(Solution[i].pos.X(),Solution[i].pos.Y());
    // this can happen if pie is close to edge of
    //CSI and two Csi fired, possibly a particle went in one and then
    //out the other. we are going to reject such events for now
  }//loop over solutions
  return Nmatched;
}

//********************************
int ringCounter::matchWithHIRACsi(elist *Csi){
  multProton = 0;
  multAlpha = 0;
  multDeuteron = 0;
  // match Si and Csi
  int Nmatched = 0;
  for (int i = 0; i < Nsolution; i++) {
// !!!!KYLE!!!! Check Order of Pie ring (front vs back)
    double theta = Telescope->getTheta(Solution[i].itele, Solution[i].iRing, Solution[i].iPie);
    double phi = Telescope->phiRecon;
    double mag = Telescope->r;

    Solution[i].theta = theta;
    Solution[i].phi = phi;
    Solution[i].pos.SetMagThetaPhi(mag,theta,phi);
    int id_csi;

// !!!!KYLE!!!! Check definitions of CsI crystal number
    if(Solution[i].iPie <= 16 && Solution[i].iPie >= 0) {
      if(Solution[i].iRing <= 16 && Solution[i].iRing >= 0) {
        id_csi = 0;
      } else {
        id_csi = 1;
      }
    } else if(Solution[i].iPie <= 31 && Solution[i].iPie >= 15) {
      if(Solution[i].iRing <= 16 && Solution[i].iRing >= 0) {
        id_csi = 2;
      } else {
        id_csi = 3;
      }
    }
    bool found = false;
    int Nfound = 0;
    for (int icsi = 0;icsi<Csi->Nstore;icsi++) {
      Solution[i].ipid = 0;
      if (Csi->Order[icsi].strip == id_csi && Csi->Order[icsi].tele == Solution[i].itele ) {
        Solution[i].energyR = Csi->Order[icsi].energyR;
        Solution[i].icsi = Csi->Order[icsi].strip;
        found = true;
        Nfound++;
        Nmatched++;
        bool FoundPid = Pid[Csi->Order[icsi].tele][Csi->Order[icsi].strip]->getPID(Solution[i].energyR, Solution[i].denergy);
        float x = Solution[i].theta*cos(Solution[i].phi)*180./acos(-1.);
        float y = Solution[i].theta*sin(Solution[i].phi)*180./acos(-1.);
        if(!FoundPid) continue;
        int Z = Pid[Csi->Order[icsi].tele][Csi->Order[icsi].strip]->Z;
        int A = Pid[Csi->Order[icsi].tele][Csi->Order[icsi].strip]->A;
        if (Pid[Csi->Order[icsi].tele][Csi->Order[icsi].strip]->Z == 1 && Pid[Csi->Order[icsi].tele][Csi->Order[icsi].strip]->A == 1) {
          proton_present = true;
          multProton++;
          Solution[i].ipid = 1;
          Histo->protonHitMap->Fill(x,y);
        } else if (Pid[Csi->Order[icsi].tele][Csi->Order[icsi].strip]->Z == 2 && Pid[Csi->Order[icsi].tele][Csi->Order[icsi].strip]->A == 4){
   	  multAlpha++;
	} else if (Pid[Csi->Order[icsi].tele][Csi->Order[icsi].strip]->Z == 1 && Pid[Csi->Order[icsi].tele][Csi->Order[icsi].strip]->A == 2) {
	  multDeuteron++;
        }

	csical(icsi,i,Csi);
	float sumEnergy = Solution[i].denergy + Solution[i].energy; //Si + CsI in MeV
        if(1) {
          //calculate energy lost in Al plate
	  float Althick = AlThickness/cos(Solution[i].theta); //602.1 mg/cm2 Al plate (2.23 mm)
	  float Ebeforetarget = losses_Al->getEin(sumEnergy,Althick,Z,A);
	  sumEnergy = Ebeforetarget;
	  //Calculate the energy lost in the target
	  float thick = TargetThickness/2./cos(Solution[i].theta); //0.540 mm Be target -> 99.79 mg/cm2
	  float ein = losses->getEin(sumEnergy,thick,Z,A);
	  Solution[i].Ekin = ein;
	  Solution[i].iZ = Z;
	  Solution[i].iA = A;
	  Solution[i].mass = Masses->GetMass(Z,A);
	}
      } //csi match if
    } //loop over Csi
    if (Nfound > 1) {
      return 0;
    }
    Histo->SiMap->Fill(Solution[i].pos.X(),Solution[i].pos.Y());
    // this can happen if pie is close to edge of
    //CSI and two Csi fired, possibly a particle went in one and then
    //out the other. we are going to reject such events for now
  }//loop over solutions
  return Nmatched;
}

void ringCounter::getMomentum() {
  for(int i = 0; i < Nsolution; i++) {
    if(Solution[i].iZ ==-1) continue;
    float theta = Solution[i].theta;
    float phi = Solution[i].phi;
    float momentum;
    if (relativity) {
      Solution[i].mass *= 931.478;
      momentum = Solution[i].Kinematics.getMomentum(Solution[i].Ekin,Solution[i].mass);
      Solution[i].energyTot = Solution[i].Ekin + Solution[i].mass;
    } else {
      momentum = sqrt(2.*Solution[i].mass*Solution[i].Ekin);
      Solution[i].mass = 0.;
    }
    Solution[i].Mvect[0] = momentum*sin(theta)*cos(phi);
    Solution[i].Mvect[1] = momentum*sin(theta)*sin(phi);
    Solution[i].Mvect[2] = momentum*cos(theta);
    Solution[i].momentum = sqrt(pow(Solution[i].Mvect[0],2) + pow(Solution[i].Mvect[1],2) + pow(Solution[i].Mvect[2],2));
    Solution[i].velocity = Solution[i].momentum/Solution[i].energyTot;
  }
}

void ringCounter::csical(int icsi1, int i2, elist *Csi) {
  int iCsi = Csi->Order[icsi1].strip;
  int iTele = Csi->Order[icsi1].tele;
  //Everything is treated as a proton by default
  //energies is changed for known particles
  float energy = calProton->getEnergy(iTele,iCsi,Csi->Order[icsi1].energyR);

  if (Pid[iTele][iCsi]->Z == 1  && Pid[iTele][iCsi]->A == 1) {
    Histo->p_ECsI_theta[iTele][iCsi]->Fill(Solution[i2].theta,Solution[i2].energyR);
    Histo->p_ECsI_thetaCal[iTele][iCsi]->Fill(Solution[i2].theta,energy);
    Histo->p_ECsI_phi[iTele][iCsi]->Fill(Solution[i2].phi,Solution[i2].energyR);
    Histo->p_ECsI_ring[iTele][iCsi]->Fill(Solution[i2].iRing,Solution[i2].energyR);
  } else if(Pid[iTele][iCsi]->Z == 1 && Pid[iTele][iCsi]->A == 2) {
    energy = calDeuteron->getEnergy(iTele,iCsi,Csi->Order[icsi1].energyR);
  } else if (Pid[iTele][iCsi]->Z == 2 && Pid[iTele][iCsi]->A == 4) {
    energy = calAlpha->getEnergy(iTele,iCsi,Csi->Order[icsi1].energyR);
    Histo->alpha_ECsI_theta[iTele][iCsi]->Fill(Solution[i2].theta,Solution[i2].energyR);
    Histo->alpha_ECsI_phi[iTele][iCsi]->Fill(Solution[i2].phi,Solution[i2].energyR);
    Histo->alpha_ECsI_ring[iTele][iCsi]->Fill(Solution[i2].iRing, Solution[i2].energyR);
  }
  Solution[i2].energy = energy;
}
