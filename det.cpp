#include "det.h"
#include <fstream>
#include <iostream>
/***********************************************************/
/* Constructor *********************************************/
/***********************************************************/

det::det(histo_sort * Histo0, histo_read * Histo1, readBeam * Beam0, readMass *Masses0, runOptions *opt0) {
  type = 2;
  Histo_read = Histo1;
  Histo_sort = Histo0;
  Beams = Beam0;
  Opt = opt0;
  NPB.resize(Beams->pb.size());

  for(unsigned int i = 0; i < Beams->pb.size(); i++) {
    NPB.at(i) = 0;
    NSB[i].resize(Beams->sb[i].size());
    NSB_alpha[i].resize(Beams->sb[i].size());
    NSB_deuteron[i].resize(Beams->sb[i].size());
    for(unsigned int j = 0; j < Beams->sb[i].size(); j++) {
      NSB[i].at(j) = 0;
      for(int k = 0; k < 6; k++) {
        NSB_pro[i][k].resize(Beams->sb[i].size());
      }
    }
  }
  Masses = Masses0;
  ran = new TRandom;
  RingCounter = new ringCounter(ran, Histo_sort, Masses, 33.1, Opt);
  Hira = new hira(ran,Histo_sort,Histo_read, Opt);
  s800 = new S800(ran,Histo_sort,Histo_read,Beams, Opt);
  Ceasar = new ceasar(ran,Histo_sort,Histo_read, Opt);
  Doppler = new doppler(0.326477); // beta for 54 MeV/A Ne-17

  losses_target = new CLosses(18,"_target.loss",true);
  losses_fiber = new CLosses(18,"_fiber.loss",true);

  NS800 =0;
  NS800_PB =0;
  NS800_PBpass =0;
  trig[0] = 0;
  trig[1] = 0;
  trig[2] = 0;
}

/***********************************************************/
/* Destructor **********************************************/
/***********************************************************/
det::~det(){
  delete losses_fiber;
  delete losses_target;
  delete Hira;
  delete ran;
  delete s800;
  delete Ceasar;
  if (type == 0) {
    //delete Forest;
  }
  cout << "number of good tree events " << ngood << endl;
  cout << "You made it!" << endl;
}

void det::Reset() {
  Hira->reset();
  RingCounter->reset();
  s800->Reset();
}

/***********************************************************/
/* Unpacks a physics event from the data stream ************/
/* Param point0 - pointer to location in data stream *******/
/***********************************************************/
bool det::unpack(unsigned short *point,int runno,int sourceID) {
  unsigned short  words = *point++;
  Run = runno;
  bool stat = false;
  if(sourceID == SiID) {
    point+=6; //This skips the SIS3820 information which is already in the timestamp
    stat = Hira->unpack(point,runno);
    if(!stat) {
   //   cout << "Didn't read hira right" <<endl;
      return stat;
    }
     stat = Ceasar->unpack(point, runno);
     if(!stat) {
   //   cout << "Didn't read ceasar right" <<endl;
      return stat;
     }
   } else if (sourceID == S800ID) {
//      cout << "unpacking s800" << endl;
      stat = s800->unpack(point,runno);
      if(!stat) cout << "Didn't read s800 right" <<endl;
      if(stat)NS800++;
   } else {
     cout << "found unexpected sourceID = " << endl;
     return stat;
   }
  return stat;
}

void det::analyze(int event, int run) {
  Correl.reset();
  bool foundresidue = false;
  s800_results S800_results;
  S800_results.Reset();

  if (s800->Trig.registr & 1) {
    S800_results.trig_s800_singles =true;
    trig[0]++;
  }
  if (s800->Trig.registr & 2){
     S800_results.trig_coin = true;
     trig[1]++;
  }
  if (s800->Trig.registr & 16) {
    S800_results.trig_singles = true;
    trig[2]++;
  }

  S800_results.Zbeam = -1;
  S800_results.Abeam = -1;
  S800_results.Zresidue = -1;
  S800_results.Aresidue = -1;

  bool goods800 = s800->analyze();
  //if(!goods800) return;
  S800_results.Zbeam = s800->beam_pid->Z;
  S800_results.Abeam = s800->beam_pid->A;
  NS800_PB++;
  RingCounter->analysis(S800_results, Hira->Pie, Hira->Ring, Hira->Csi);

  int Mult = 0;
  for(int i = 0;i < RingCounter->Nsolution; i++) {
    if(RingCounter->Solution[i].iZ > 0) {
      Correl.addPar(&RingCounter->Solution[i]);
      Mult++;
    }
  }

  sort(Correl.parPair.begin(),Correl.parPair.end());

  int size = Correl.parPair.size();
  int pmult = Correl.GetProtonMult();
  int amult = Correl.GetAlphaMult();
  int dmult = Correl.GetDeuteronMult();

  Histo_sort->AMult->Fill(amult);
  Histo_sort->DMult->Fill(dmult);
  Histo_sort->PMult->Fill(pmult);

  //Added for Li6;
  if(amult == 1 && dmult == 1 && size == 2) {
    Correl.makeParArray();
    float Erel = Correl.findErel();
    Histo_read->ErelLi6->Fill(Erel);
    Histo_read->ThetaCMLi6->Fill(Correl.thetaCM*180./acos(-1));
    Histo_read->VCMLi6->Fill(Correl.velocityCM);
  }
  if(amult == 2 && size == 2) {
    Correl.makeParArray();
    float Erel = Correl.findErel();
    Histo_read->ErelBe8->Fill(Erel);
    Histo_read->ThetaCMBe8->Fill(Correl.thetaCM*180./acos(-1));
    Histo_read->VCMBe8->Fill(Correl.velocityCM);

  }
  if(amult == 2 && pmult == 1 && size == 3) {
    Correl.makeParArray();
    float Erel = Correl.findErel();
    Histo_read->ErelB9->Fill(Erel);
  }

  if(amult == 3 && size == 3) {
    Correl.makeParArray();
    float Erel = Correl.findErel();
    Histo_read->ErelC12->Fill(Erel);

    Correl.Mask(0,1,1);
    Correl.makeParArray();
    Erel = Correl.findErel();
    Histo_read->ErelC12Be8->Fill(Erel);
    Correl.Mask(1,0,1);
    Correl.makeParArray();
    Erel = Correl.findErel();
    Histo_read->ErelC12Be8->Fill(Erel);
    Correl.Mask(1,1,0);
    Correl.makeParArray();
    Erel = Correl.findErel();
    Histo_read->ErelC12Be8->Fill(Erel);
  }
  Correl.reset();

  int PBidx = Beams->BeamID(S800_results.Zbeam, S800_results.Abeam, Beams->pb);
  if(PBidx == -1) return;
  NS800_PBpass++;
  S800_results.Zresidue = s800->residue_pid[PBidx]->Z;
  S800_results.Aresidue = s800->residue_pid[PBidx]->A;

  int SBidx =  Beams->BeamID(S800_results.Zresidue, S800_results.Aresidue, Beams->sb[PBidx]);
//  if(SBidx == -1) return;
  NPB.at(PBidx)++;
  if(SBidx != -1) NSB[PBidx].at(SBidx)++;

  if (S800_results.trig_coin) Histo_sort->S800_Csi_time->Fill(Hira->T_CsiTrig/10.);
  if (S800_results.trig_singles)Histo_sort->singles_trig_time->Fill(Hira->T_CsiTrig/10.);

//  RingCounter->analysis(S800_results, Hira->Pie, Hira->Ring, Hira->Csi);

  if (S800_results.trig_coin && RingCounter->proton_present)
    Histo_sort->S800_Csi_time_with_proton->Fill(Hira->T_CsiTrig/10.);

  if((int)s800->mTDC.objCorrected.size()>0) {
    if((int)s800->mTDC.xfp.size()>0) {
      Histo_sort->ObjvsICsum->Fill(s800->mTDC.objCorrected.at(0),s800->IC.sum);
      Histo_sort->ObjvsICsum_List.at(PBidx)->Fill(s800->mTDC.objCorrected.at(0), s800->IC.sum);
      if(RingCounter->multAlpha == 1) {
        Histo_sort->ObjvsXFPwithAlpha1->Fill(s800->mTDC.objCorrected.at(0), s800->mTDC.xfp.at(0));
        Histo_sort->ObjvsICsum_alphaList.at(PBidx)->Fill(s800->mTDC.objCorrected.at(0), s800->IC.sum);
      }
      if(RingCounter->multDeuteron == 1) {
        Histo_sort->ObjvsICsum_deuteronList.at(PBidx)->Fill(s800->mTDC.objCorrected.at(0), s800->IC.sum);
      }
      if(RingCounter->multProton == 1) {
        Histo_sort->ObjvsXFPwithProton1->Fill(s800->mTDC.objCorrected.at(0), s800->mTDC.xfp.at(0));
        Histo_sort->ObjvsICsum_proton1List.at(PBidx)->Fill(s800->mTDC.objCorrected.at(0), s800->IC.sum);
      }
      if(RingCounter->multProton == 2) {
        Histo_sort->ObjvsXFPwithProton2->Fill(s800->mTDC.objCorrected.at(0), s800->mTDC.xfp.at(0));
        Histo_sort->ObjvsICsum_proton2List.at(PBidx)->Fill(s800->mTDC.objCorrected.at(0), s800->IC.sum);
      }
      if(RingCounter->multProton == 3) {
        Histo_sort->ObjvsXFPwithProton3->Fill(s800->mTDC.objCorrected.at(0), s800->mTDC.xfp.at(0));
        Histo_sort->ObjvsICsum_proton3List.at(PBidx)->Fill(s800->mTDC.objCorrected.at(0), s800->IC.sum);
      }
      if(Ceasar->Nadded > 0) {
        Histo_sort->ObjvsICsum_gammaList.at(PBidx)->Fill(s800->mTDC.objCorrected.at(0), s800->IC.sum);
      }
      if(Hira->XY_mon->theta !=-999) {
        Histo_sort->ObjvsICsum_fiberList.at(PBidx)->Fill(s800->mTDC.objCorrected.at(0), s800->IC.sum);
      }
    }
  }

  Histo_sort->CornerMult->Fill(Hira->XY_mon->vert.Mult+Hira->XY_mon->horz.Mult);
  Histo_sort->CornerMult_Vert->Fill(Hira->XY_mon->vert.Mult);
  Histo_sort->CornerMult_Hori->Fill(Hira->XY_mon->horz.Mult);
  Histo_sort->CornerMult_VvsH->Fill(Hira->XY_mon->horz.Mult,Hira->XY_mon->vert.Mult);


  ///////////////////////////////////////////// CRDC gain analysis
  int pad_max[2] = {0,0}; //max energy for a given pad
  int max_pad[2] = {-1,-1}; //pad with the max energy
  // Creating CRDC gain spectras
  for(int i = 0; i < 2; i++) {
    pad_max[i] = s800->CRDC[i].cal[s800->CRDC[i].PadMult/2];
    max_pad[i] = s800->CRDC[i].pad[s800->CRDC[i].PadMult/2];
//    for(int j = (s800->CRDC[i].PadMult/2 - 5); j < (s800->CRDC[i].PadMult/2 + 5); j++) {
    for(int j = 0; j < s800->CRDC[i].PadMult; j++) {
      if(s800->CRDC[i].cal[j]>pad_max[i]) {
        pad_max[i] = s800->CRDC[i].cal[j];
        max_pad[i] = s800->CRDC[i].pad[j];
      }
    }
    Histo_sort->CRDCSumList[i][PBidx]->Fill(max_pad[i],pad_max[i]);
    Histo_sort->CRDCPadMultList[i][PBidx]->Fill(max_pad[i],s800->CRDC[i].PadMult);
    if(SBidx != -1) Histo_sort->CRDCList[i][PBidx].at(SBidx)->Fill(max_pad[i],pad_max[i]);
  }

  if(SBidx == -1) return;
  //////////////////////////////////  end of CRDC gain analysis
  float Nsol =0;
  int mult  =0;
  CsImult = 0;
  Run = run;
  if(S800_results.Zbeam > 0 && S800_results.Abeam > 0) {
    Histo_sort->ThetavsPhi->Fill(s800->track->thetadeg, s800->track->phideg);
    //Lets add the S800 to the Solution Class
    if(S800_results.Zresidue > 0 && S800_results.Aresidue > 0) {
      foundresidue=true;
      foundresidue = LoadS800toSolution(PBidx);
    }
  }

  ///////////////////////////////////////////checking for consistency with pid plots
  if(foundresidue) {
    if(RingCounter->multAlpha > 0) NSB_alpha[PBidx].at(SBidx)++;
    if(RingCounter->multDeuteron > 0) NSB_deuteron[PBidx].at(SBidx)++;
    if(RingCounter->multProton < 6) NSB_pro[PBidx][RingCounter->multProton].at(SBidx)++;
    int Nsol = RingCounter->Nsolution;
    float res_Vel = RingCounter->Solution[Nsol-1].velocity;
    for(int i=0;i<Ceasar->Nadded;i++) {
      float angle = RingCounter->Solution[Nsol-1].pos.Angle(Ceasar->added[i].pos);
      float dopp = Doppler->correct(Ceasar->added[i].energy,angle,res_Vel);
      Histo_read->TEClist[PBidx].at(SBidx)->Fill(dopp);
    }
  }

  //////// end of gamma code

  Mult = 0;
  for(int i = 0;i < RingCounter->Nsolution; i++) {
    if(RingCounter->Solution[i].iZ > 0) {
      Correl.addPar(&RingCounter->Solution[i]);
      Mult++;
    }
  }

  sort(Correl.parPair.begin(),Correl.parPair.end());

  size = Correl.parPair.size();
  pmult = Correl.GetProtonMult();
  amult = Correl.GetAlphaMult();
  dmult = Correl.GetDeuteronMult();

  if(!foundresidue) return;
  vector<bool> pass = {false, false, false};
  if(pmult > 0 && pmult < 5 ) {
    pass[0] = true;
  }
  if(amult == 1 && size == 2) {
    pmult = 5;
    pass[1] = true;
  }
  if(dmult == 1 && size == 2) {
    pmult = 6;
    pass[2] = true;
  }
  if (!any_of(pass.begin(), pass.end(), [](bool t){return t == true;}) ) return;
  Correl.makeParArray();
  float Erel = Correl.findErel();
  float QValue = Masses->GetMass(Correl.check_Z, Correl.check_A) * m0 - Correl.check_mass;
  float thetaCM = Correl.thetaCM;
  float Ex = Erel - QValue;

  Histo_read->ErelList[PBidx][pmult-1].at(SBidx)->Fill(Erel);
  Histo_read->ExList[PBidx][pmult-1].at(SBidx)->Fill(Ex);
  Histo_read->TCMList[PBidx][pmult-1].at(SBidx)->Fill(thetaCM*180./acos(-1));
  float vCM = Correl.velocityCM;
  Histo_read->VCMList[PBidx][pmult-1].at(SBidx)->Fill(vCM);

  for(int i=0;i<Ceasar->Nadded;i++) {
    float res_Vel = RingCounter->Solution[size-1].velocity;
    float angle = RingCounter->Solution[size-1].pos.Angle(Ceasar->added[i].pos);
    float dopp = Doppler->correct(Ceasar->added[i].energy,angle,res_Vel);
    Histo_read->EgamList[PBidx][pmult-1].at(SBidx)->Fill(dopp);
    Histo_read->EgamErelList[PBidx][pmult-1].at(SBidx)->Fill(dopp,Erel);
  }
  if(pmult == 2) {
    Correl.Mask(0,1,1);
    Correl.makeParArray();
    float ExY1 = Correl.findErel()/Erel;
    Correl.Mask(1,0,1);
    Correl.makeParArray();
    float ExY2 = Correl.findErel()/Erel;
    Histo_read->ErelVsProList[PBidx].at(SBidx)->Fill(ExY1*Erel,Erel);
    Histo_read->ErelVsProList[PBidx].at(SBidx)->Fill(ExY2*Erel,Erel);
    //Get the relative energy betweeen the protons
    Correl.Mask(1,1,0);
    Correl.makeParArray();
    float ExT  = Correl.findErel()/Erel;
    Correl.Mask(1,1,1);
    Correl.makeParArray();
    Correl.findErel();
    Correl.getJacobi();
    if(Erel < 2.5) {
      Histo_read->Jacobi_T_List[PBidx].at(SBidx)->Fill(ExT,Correl.cosThetaT);
      Histo_read->Jacobi_T_List[PBidx].at(SBidx)->Fill(ExT,-Correl.cosThetaT);
      Histo_read->Jacobi_Y_List[PBidx].at(SBidx)->Fill(ExY1,Correl.cosThetaY[0]);
      Histo_read->Jacobi_Y_List[PBidx].at(SBidx)->Fill(ExY2,Correl.cosThetaY[1]);
    }
  }
}
bool det::LoadS800toSolution(int PB){

  int Nsol = RingCounter->Nsolution;
  RingCounter->Nsolution++;

  int Z = s800->residue_pid[PB]->Z;
  int A = s800->residue_pid[PB]->A;

  RingCounter->Solution[Nsol].iZ = Z;
  RingCounter->Solution[Nsol].iA = A;

  //using the theta phi from the fiber
  float thetaf = 0;
  float phif = 0;

  if(Hira->XY_mon->theta != -999) {
    thetaf =  Hira->XY_mon->theta;
    phif = Hira->XY_mon->phi;
  } else {
    thetaf = s800->track->theta;
    phif = s800->track->phi;
    return false;
  }

  float ekin = s800->track->energy;
  float thickness = Hira->XY_mon->GetFiberThickness()/cos(thetaf); // NB: 0.5mm -> 51.1 mg/cm2
  ekin = losses_fiber->getEin(ekin,thickness,Z,A);
  float targetthickness = RingCounter->GetTargetThickness()/2;
  ekin = losses_target->getEin(ekin,targetthickness,Z,A);
  RingCounter->Solution[Nsol].Ekin = ekin;
  double mass = Masses->GetMass(Z, A);
  RingCounter->Solution[Nsol].mass = mass*m0;
  float momentum = RingCounter->Solution[Nsol].Kinematics.getMomentum(RingCounter->Solution[Nsol].Ekin,RingCounter->Solution[Nsol].mass);

  RingCounter->Solution[Nsol].momentum = momentum;
  RingCounter->Solution[Nsol].Mvect[0] = momentum*sin(thetaf)*cos(phif);
  RingCounter->Solution[Nsol].Mvect[1] = momentum*sin(thetaf)*sin(phif);
  RingCounter->Solution[Nsol].Mvect[2] = momentum*cos(thetaf);

  RingCounter->Solution[Nsol].energyTot = RingCounter->Solution[Nsol].mass + RingCounter->Solution[Nsol].Ekin;
  RingCounter->Solution[Nsol].velocity = momentum/RingCounter->Solution[Nsol].energyTot;
  RingCounter->Solution[Nsol].pos.SetMagThetaPhi(10,thetaf,phif);
  return true;
}
