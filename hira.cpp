#include "hira.h"


hira::hira(TRandom * ran0, histo_sort * Histo0) {
  type = 0;
  Histo = Histo0;
  ran = ran0;
  init();
}

hira::hira(TRandom * ran0, histo_read * Histo1) {
  type = 1;
  Histo_read = Histo1;
  ran = ran0;
  init();
}

hira::hira(TRandom * ran0, histo_sort * Histo0, histo_read * Histo1, runOptions *opt) {
  type = 2;
  Histo_read = Histo1;
  Histo = Histo0;
  ran = ran0;
  S4RingMap = opt->ringmap;
  S4PieMap = opt->piemap;
  CsIChMap = opt->csimap;
  CsIChTDCMap = opt->csitdcmap;
  RingCal = opt->ringcal;
  PieCal = opt->piecal;
  ProtonCal = opt->pcal;
  dist2Si = opt->siDist;
  Ntele = opt->ntele;
  Nstrip = opt->nring;
  Ncsi = opt->ncsi;
  init();
}

void hira::init() {
  xmarker[0] = 0x1ff3;
  TDC = new TDC1190(3, 20, 128); //registers 3 hits, reference channel 33, 128 total channels
  //make map of chips
  ifstream ifile(S4RingMap.c_str());
  if (!ifile.is_open()) {
    cout << "Ring map not found" << endl;
    abort();
  }
  string name;
  getline(ifile, name);
  int CB, Ch, iT, iR;
  for (;;) {
    ifile >> CB >> Ch >> iT >> iR;
    if (ifile.eof()) break;
    if (ifile.bad()) break;
    RingMap[CB - 5][Ch] = iR;
    RingTeleMap[CB - 5][Ch] = iT;
  }
  ifile.close();
  ifile.clear();

  ifstream ifile2(S4PieMap.c_str());
  if (!ifile2.is_open()) {
    cout << "Pie map not found" << endl;
    abort();
  }
  getline(ifile2, name);
  int iP;
  for (;;) {
    ifile2 >> CB >> Ch >> iT >> iP;
    if (ifile2.eof()) break;
    if (ifile2.bad()) break;
    PieMap[CB - 1][Ch] = iP;
    PieTeleMap[CB - 1][Ch] = iT;
  }
  ifile2.close();
  ifile2.clear();

  ifstream ifile3(CsIChMap.c_str());
  if (!ifile3.is_open()) {
    cout << "CsI map not found" << endl;
    abort();
  }
  getline(ifile3, name);
  int iC;
  for (;;) {
    ifile3 >> Ch >> iT >> iC;
    if (ifile3.eof()) break;
    if (ifile3.bad()) break;
    CsIMap[Ch] = iC;
    CsITeleMap[Ch] = iT;
  }
  ifile3.close();
  ifile3.clear();


  ifstream ifile4(CsIChTDCMap.c_str());
  if (!ifile4.is_open()) {
    cout << "CsI TDC map not found" << endl;
    abort();
  }
  getline(ifile4, name);
  for (;;) {
    ifile4 >> Ch >> iT >> iC;
    if (ifile4.eof()) break;
    if (ifile4.bad()) break;
    CsITDCMap[Ch] = iC;
    CsITDCTeleMap[Ch] = iT;
  }
  ifile4.close();
  ifile4.clear();


  //read in calibrations

  calPies = new calibrate(Ntele, Nstrip, PieCal, 1);
  calRings = new calibrate(Ntele, Nstrip, RingCal, 1);
  calCsi = new calibrate(Ntele, Ncsi, ProtonCal, 1);

  XY_mon = new XYmon(dist2Si);

  NHira = 0;
  Nfiber = 0;

  //high low correlations zero arrays
  for(int i = 0; i < 14; i++) {
    for(int j = 0; j < 32; j++) {
      fsumN[i][j] = 0.;
      fsumx[i][j] = 0.;
      fsumxx[i][j] = 0.;
      fsumyx[i][j] = 0.;
      fsumy[i][j] = 0.;

      bsumN[i][j] = 0.;
      bsumx[i][j] = 0.;
      bsumxx[i][j] = 0.;
      bsumyx[i][j] = 0.;
      bsumy[i][j] = 0.;
    }
  }
}
//*************************************************************//
bool hira::unpack(unsigned short * & point, int runno) {

  bool stat = true;
  stat = unpackSi_HINP4(point);
  if (!stat) {
    // cout << "Bad hira" << endl;
    return stat;
  }
  NHira++;

  //  cout <<"before CsI " << hex << *point << " " << *(point+1) << dec << endl;
  stat = unpackCsi(point, runno);

  if (!stat) {
    //  cout << "Bad CsI" << endl;
    return stat;
  }
  stat = unpackFiber(point);
  if (stat)
    Nfiber++;
  else {
    // cout << "bad Fiber" << endl;
  }

  // cout << hex << *point << " " << *(point+1) << " " << *(point+2) << " "<< stat << endl;
  // abort();
  return stat;
}
//************************************************************//
bool hira::unpackFiber(unsigned short * & point) {
  unsigned short f3 = * point;
  unsigned short f4 = * (point + 1);
  // check to see if there in no data first
  if (f3 == 0xffff && f4 == 0xffff) {
    point += 2;
    return false;
  }
  //suck out qdc info
  ADC.number = 0;
  point = ADC.read(point);
  // cout << "NQDC channels read = " << ADC.number << endl;
  for (int i = 0; i < ADC.number; i++) {
    if (ADC.underflow[i]) continue;
    if (ADC.overflow[i]) continue;
    int id = ADC.channel[i];
    int ienergy = ADC.data[i];
    if (id == 0) XY_mon->horz.dynode = ienergy + ran->Rndm();
    else if (id == 1) XY_mon->horz.A = ienergy + ran->Rndm() - 272.;
    else if (id == 2) XY_mon->horz.B = ienergy + ran->Rndm() - 270.;
    else if (id == 3) XY_mon->horz.C = ienergy + ran->Rndm() - 294.;
    else if (id == 4) XY_mon->horz.D = ienergy + ran->Rndm() - 284.;
    else if (id == 5) XY_mon->vert.dynode = ienergy + ran->Rndm();
    else if (id == 6) XY_mon->vert.A = ienergy + ran->Rndm() - 256.;
    else if (id == 7) XY_mon->vert.B = ienergy + ran->Rndm() - 308.;
    else if (id == 8) XY_mon->vert.C = ienergy + ran->Rndm() - 276.;
    else if (id == 9) XY_mon->vert.D = ienergy + ran->Rndm() - 294.;
  }

  XY_mon->make_2d();
  if (XY_mon->vert.has_data) {
    Histo->VertA->Fill(XY_mon->vert.A);
    Histo->VertB->Fill(XY_mon->vert.B);
    Histo->VertC->Fill(XY_mon->vert.C);
    Histo->VertD->Fill(XY_mon->vert.D);
    Histo->VertDyn->Fill(XY_mon->vert.dynode);
    Histo->VertHitMap->Fill(XY_mon->vert.x, XY_mon->vert.y);
    Histo->Fiber_Y->Fill(XY_mon->y);
  }
  if (XY_mon->horz.has_data) {
    Histo->HorzA->Fill(XY_mon->horz.A);
    Histo->HorzB->Fill(XY_mon->horz.B);
    Histo->HorzC->Fill(XY_mon->horz.C);
    Histo->HorzD->Fill(XY_mon->horz.D);
    Histo->HorzDyn->Fill(XY_mon->horz.dynode);
    Histo->HorzHitMap->Fill(XY_mon->horz.x, XY_mon->horz.y);
    if (XY_mon->horz.dynode > 300.) Histo->HorzHitMap_energygate->Fill(XY_mon->horz.x, XY_mon->horz.y);
    Histo->DynodevsSum->Fill(XY_mon->horz.dynode, XY_mon->horz.total);
    Histo->AvsSum->Fill(XY_mon->horz.A, XY_mon->horz.total);
    Histo->BvsSum->Fill(XY_mon->horz.B, XY_mon->horz.total);
    Histo->CvsSum->Fill(XY_mon->horz.C, XY_mon->horz.total);
    Histo->DvsSum->Fill(XY_mon->horz.D, XY_mon->horz.total);
    Histo->Fiber_X->Fill(XY_mon->x);
  }
  if (XY_mon->has_data) {
    Histo->Fiber_XY->Fill(XY_mon->x, XY_mon->y);
  }

  //check for ffff's
  unsigned short f1 = * point;
  point++;
  unsigned short f2 = * point;
  point++;
  if (f1 != 0xffff && f2 != 0xffff) {
    cout << "didnt read right" << endl;
    return false;
  }
  return true;
}

//*************************************************************//
bool hira::unpackCsi(unsigned short * & point, int runno) {
  NE = 0;

  for (int iadc = 0; iadc < 1; iadc++) {
    //check for ffff's
    unsigned short f3 = * point;
    unsigned short f4 = * (point + 1);
    if (f3 == 0xffff && f4 == 0xffff) {
      point += 2;
      continue;
    }
    ADC.number = 0;
    point = ADC.read(point); // suck out the info in the qdc
    for (int i = 0; i < ADC.number; i++) {
      if (ADC.underflow[i]) continue;
      if (ADC.overflow[i]) continue;
      int id = ADC.channel[i] + 32 * iadc;
      int ienergy = ADC.data[i];
      if (id < 20) {
        int iCsi = CsIMap[id];
        int itele = CsITeleMap[id];
        float energy = calCsi->getEnergy(itele, id, ienergy + ran->Rndm());
        DataE[NE].id = iCsi;
        DataE[NE].itele = itele;
        DataE[NE].ienergy = ienergy;
        DataE[NE].energy = energy;
        Histo->ECsI[itele][iCsi]->Fill(ienergy);
        Histo->ECsISum->Fill(iCsi, ienergy);
        Histo->ECsICSum->Fill(iCsi, energy);
        NE++;
      }
    }
  }
  //check for ffff's
  unsigned short f1 = * point;
  point++;
  unsigned short f2 = * point;
  point++;
  if (f1 != 0xffff && f2 != 0xffff) {
    cout << "didnt read Ecsi right" << endl;
    return false;
  }

  NT = 0;
  for (int itdc = 0; itdc < 1; itdc++) {
    //check for ffff's
    unsigned short f3 = * point;
    unsigned short f4 = * (point + 1);
    if (f3 == 0xffff && f4 == 0xffff) {
      cout << "no Csi TDC" << endl;
      point += 2;
      continue;
    }
    point = TDC->read(point); // suck out the info in the tdc
    for (int i = 0; i < TDC->Ndata; i++) {
      int id = TDC->dataOut[i].channel;
      int itime = TDC->dataOut[i].time;
      int itele = CsITDCTeleMap[id];
      if (id < 20) {
//KYLE!!! How to give TDC a Telescope Number?
        DataT[NT].id = id;
        DataT[NT].itele = itele;
        DataT[NT].itime = itime;
        Histo->TCsI[itele][id]->Fill(itime / 10.);
        NT++;
      } else if (id == 65) {
        T_RFSCIN = itime;
      } else if (id == 66) {
        T_A1900 = itime;
        Histo->T_A1900->Fill(itime / 10.);
      } else if (id == 67) {
        T_RFCYC = itime;
        Histo->T_RFCYC->Fill(itime / 10.);
      } else if (id == 27) {
        T_CsiTrig = itime;
      }
    }
    //check for ffff's
    unsigned short f1 = * point;
    point++;
    unsigned short f2 = * point;
    point++;
    if (f1 != 0xffff && f2 != 0xffff) {
      cout << "did not read CsI TDC correctly" << endl;
      return false;
    }
  }

  int Nfound = 0;
  int Nnotfound = 0;
  // match up energies to times
  for (int ie = 0; ie < NE; ie++) {
    DataE[ie].itime = -1;
    bool found = false;
    for (int it = 0; it < NT; it++) {
      if ((DataE[ie].itele == DataT[it].itele) &&(DataE[ie].id == DataT[it].id)) { //we have matched
        found = true;
        DataE[ie].itime = DataT[it].itime;
        int icsi = DataE[ie].id;
        int itele = 0;
        if (DataE[ie].energy > Csi_energy_min) { // && DataE[ie].itime > 500 && DataE[ie].itime < 1500)
          Csi.Add(icsi, itele, DataE[ie].energy, 0., DataE[ie].ienergy, DataE[ie].itime);
          Histo->ET_csi[itele][icsi]->Fill(DataE[ie].ienergy, DataE[ie].itime / 10.);
          Nfound++;
        }
      }
    }
    if (!found) {
      if (DataE[ie].energy > Csi_energy_min) {
        int icsi = DataE[ie].id;
        int itele = 0;
        Nnotfound++;
        Csi.Add(icsi, itele, DataE[ie].energy, 0., DataE[ie].ienergy, 0.);
      }
    }
  }
  bool stat = true;
  return stat;
}

//***************************************************************//

//unpacking the XLM with ADC on the CHIP BOARDS (HINP 4)
bool hira::unpackSi_HINP4(unsigned short * & point) {
  unsigned short marker;
  for (int iMB = 0; iMB < 1; iMB++) {
    marker = * point++;
    if (marker != xmarker[iMB]) {
      cout << "Did not read the proper XLM marker. Was " << hex << marker << " expected " << xmarker[iMB] << dec << endl;
      return false;
    }
    int NstripsRead = 0;
    unsigned short chipWords = * point;
    int NWords = * point;
    unsigned short * endHINP = point;
    if (NWords > 2048) return false;
    endHINP += NWords + 2;
    point += 2;
    NstripsRead = * point;
    if (NstripsRead % 4 != 0) return false;
    NstripsRead /= 4;
    if (NstripsRead > 512) {
      point += 8;
      return false; // bad buffer
    }
    point += 5;
    for (int istrip = 0; istrip < NstripsRead; istrip++) {
      unsigned short id = * point++;
      unsigned short chipNum = (id & 0x1FE0) >> 5;
      unsigned short chanNum = id & 0x1F;
      unsigned short ienergy = * point++;
      unsigned short ilowenergy = * point++;
      unsigned short itime = * point++;
      unsigned short underOver = 0; //No under or overflow in HINP4
      if (chipNum % 2 == 0) {
        chanNum = 2 * chanNum + 1; //For S4
        chipNum /= 2;
      } else {
        chipNum = chipNum / 2 + 1;
        chanNum = 2 * chanNum; //For S4
      }
      if (chanNum > 31) {
        cout << "chanNum too big" << endl;
        return false;
      }
      if (chipNum > 8) {
        cout << "chipNum too big " << chipNum << endl;
        return false;
      }
      //Filling the Pies
      if (chipNum >= 1 && chipNum <= 4) {
        int idum = chanNum;
        int iPie = -1;
        iPie = PieMap[chipNum - 1][chanNum];
        int iTele = -1;
        iTele = PieTeleMap[chipNum - 1][chanNum];
        float energy = calPies->getEnergy(iTele, iPie, ienergy + ran->Rndm());
        float time = 0.; //calPiesT->getEnergy(0,chanNum,itime+ran->Rndm());
        float lowenergy = 0.;
        Histo->PTSum[iTele]->Fill(iPie, time);
        //Pies raw spectra
        Histo->EpiesR[iTele][iPie]->Fill(ienergy); //high gain
        Histo->TpiesR[iTele][iPie]->Fill(itime); //time
        Histo->PSum[iTele]->Fill(iPie, ienergy);
        //Pies calibrated spectra
        Histo->EpiesC[iTele][iPie]->Fill(energy); //high gain
        Histo->PCSum[iTele]->Fill(iPie, energy);
        Histo->PCLSum[iTele]->Fill(iPie, lowenergy);
        if (energy > Si_energy_min) {
          Pie.Add(iPie, iTele, energy, 0., ienergy, time);
        }
      }
      else if (chipNum >= 5 && chipNum <= 8) {
        int iRing = -1;
        iRing = RingMap[chipNum - 5][chanNum];
        int iTele = -1;
        iTele = RingTeleMap[chipNum - 5][chanNum];
        if (iRing > 127 || iRing < 0) {
          cout << " RingMap error!!" << endl;
          return false;
        }
        float energy = calRings->getEnergy(iTele, iRing, ienergy + ran->Rndm());
        float lowenergy = 0.; //calRings->getEnergy(0,iRing,voltage_lo_HL);
        float time = 0.; //calRingsT->getEnergy(0,iRing,itime+ran->Rndm());
        Histo->RTSum[iTele]->Fill(iRing, time);
        //Rings raw spectra
        Histo->EringsR[iTele][iRing]->Fill(ienergy); //high gain
        Histo->TringsR[iTele][iRing]->Fill(itime); //time
        Histo->RSum[iTele]->Fill(iRing, ienergy);

        //Rings calibrated spectra
        Histo->EringsC[iTele][iRing]->Fill(energy); //high gain
        Histo->RCSum[iTele]->Fill(iRing, energy);
        if (energy > Si_energy_min) {
          Ring.Add(iRing, iTele, energy, 0., ienergy, time);
        }
      }
    }
    point = endHINP;
  }
  return true;
}
//***************************************************************
hira::~hira() {
  cout << "start Hira destr" << endl;
  //high-low correlations
  delete calCsi;
  delete calRings;
  delete calPies;
  delete XY_mon;
  cout << "stop Hira destr" << endl;
}
//**********************************************
void hira::reset() {
  XY_mon->reset();
  Csi.reset();
  Pie.reset();
  Ring.reset();
  fred = false;
}
