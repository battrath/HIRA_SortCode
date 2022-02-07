 #include "ceasar.h"

ceasar::ceasar(TRandom *ran0, histo_sort * Histo0){
  ran = ran0;
  Histo = Histo0;
  init();
}
ceasar::ceasar(TRandom *ran0, histo_read * Histo1) {
  ran = ran0;
  Histo_read = Histo1;
  init();
}
ceasar::ceasar(TRandom *ran0, histo_sort * Histo0, histo_read * Histo1, runOptions *opt) {
  ran = ran0;
  Histo = Histo0;
  Histo_read = Histo1;
  chipmap = opt->caechip;
  qdctdc = opt->caeqdc;
  calfile = opt->caecal;
  posmap = opt->caepos;
  init();
}
/***********************************************************/
void ceasar::init() {
  Doppler = new doppler(0.326477); // beta for 54 MeV/A Ne-17
  tdc = new TDC1190*[2];
  tdc[0] = new TDC1190(3,0,128);
  tdc[1] = new TDC1190(3,0,128);

  //make map of chips
  ifstream ifile(chipmap.c_str());

  if (!ifile.is_open()) {
    cout << "ceasar map not found" << endl;
    abort();
  }

  int iring,iloc,iqdc,ichan;
  for(int iqdc = 0; iqdc < 6; iqdc++) {
    for(int ich = 0; ich< 32; ich++) {
      MapC[iqdc][ich].iRing = -1;
      MapC[iqdc][ich].iLoc = -1;
    }
  }

  for (;;) {
    ifile >> iring >> iloc >> iqdc >> ichan;
    if (ifile.eof()) break;
    if (ifile.bad()) break;
    iloc-=1; //The map starts at 1 rather than 0
    MapC[iqdc][ichan].iRing = iring;
    MapC[iqdc][ichan].iLoc = iloc;
  }

  ifile.close();
  ifile.clear();
  //Read in QDC to TDC mapping
  int it,itc;
  ifile.open(qdctdc.c_str());
  if(!ifile.is_open()) {
    cout << "Couldn't open QDC to TDC map" << endl;
    abort();
  }
  for(;;) {
    ifile >> it >> itc >> iqdc >> ichan;
    if(ifile.eof()) break;
    if(ifile.bad()) break;
    MapC[iqdc][ichan].iTDC = it;
    MapC[iqdc][ichan].iTDCChan = itc;
  }
  ifile.close();
  ifile.clear();

  //read in calibrations
  int Ntele = 1;
  int Nstrip = 192;

  calCeasar = new calibrate(Ntele,Nstrip,calfile,1);

  // read in detector angles
  float x,y,z;

  for(int ir = 0; ir < 10; ir++) {
    for(int il = 0; il < 24; il++) {
      angle[ir][il] = -1000.;
      angle2[ir][il] = -1000.;
    }
  }
  ifile.open(posmap.c_str());
  if(!ifile.is_open()) {
    cout << "Couldn't open CAESAR position files" << endl;
    abort();
  }
  for (;;) {
    ifile >> iring >>  iloc >>  x >> y >> z;
    if (ifile.eof()) break;
    if (ifile.bad()) break;
    float r = sqrt(pow(x,2)+pow(y,2)+pow(z,2));
    float theta = acos(z/r);
    float phi = atan2(y,-x);
    mag[iring-1][iloc-1] = r;
    angle[iring-1][iloc-1] = theta;
    angle2[iring-1][iloc-1] = phi;
  }
}
/***********************************************************/
bool ceasar::unpack(unsigned short * point, int runno) {

  Nxfp = 0;
  NE = 0;
  int Ring = 0;
  int QDC =0;
  int Loc =0;
  int chan = 0;

  NT = 0;
  for (int itdc = 0; itdc < 2; itdc++) {
    //check for ffff's
    unsigned short f3 = *point;
    unsigned short f4 = *(point+1);
    if (f3 == 0xffff && f4 == 0xffff) {
      point += 2;
      continue;
    }
    point = tdc[itdc]->read(point);
    for (int i = 0; i < tdc[itdc]->Ndata; i++) {
      int id = -1;
      //Throwing away all but the CAESAR data
      if(itdc == 0 && tdc[itdc]->dataOut[i].channel < 16) continue; // was 48
      if(itdc == 1 && tdc[itdc]->dataOut[i].channel < 16)  continue;
      //mapping into unique ID number
      if(itdc==0) id = tdc[itdc]->dataOut[i].channel - 16; // was 48
      else  id = tdc[itdc]->dataOut[i].channel+64;
      int itime = tdc[itdc]->dataOut[i].time;
      if (tdc[itdc]->dataOut[i].order > 0) continue;
      DataTC[NT].id  = id;
      DataTC[NT].itime = itime;
      DataTC[NT].itdc = itdc;
      DataTC[NT].ichan = tdc[itdc]->dataOut[i].channel;
      float time = 0.;//calCeasarT->getEnergy(0,id,itime/10.+ran->Rndm());
      DataTC[NT].time = time;
      Histo->TCeasarCal->Fill(time);
      Histo->TCeasar[id]->Fill(itime/10.);
      Histo->TCtotal->Fill(itime/10.);
      NT++;
    }
    unsigned short f1 = *point;
    point++;
    unsigned short f2 = *point;
    point++;
    if(f1 != 0xffff && f2 != 0xffff) return false;
  }
  Histo->CTMult->Fill(NT);
  for (int iqdc  = 0; iqdc < 6 ; iqdc++) {
    //check for ffff's
    unsigned short f3 = *point;
    unsigned short f4 = *(point+1);
    if (f3 == 0xffff && f4 == 0xffff) {
      point+=2;
      continue;
    }
    Caen.number = 0;
    point = Caen.read(point);  // suck out the info in the qdc
    for (int i = 0; i < Caen.number; i++) {
      bool overflow = 0.;
      bool underflow = 0.;
      if (Caen.underflow[i]) {
        underflow  =1;
      }
      if (Caen.overflow[i]) {
        overflow =1;
      }
      int id = Caen.channel[i] + 32*iqdc;
      int ienergy = Caen.data[i];
      chan = Caen.channel[i];
      if(overflow) {
        cout << "overflow " << id << endl;
        ienergy = 5000;
      }
      if(underflow) {
        cout << "underflow " << id << endl;
        ienergy = 5100;
      }

      DataEC[NE].id = id;
      DataEC[NE].ienergy = ienergy;
      DataEC[NE].iqdc = iqdc;
      DataEC[NE].ichan = chan;
      Ring = MapC[iqdc][chan].iRing;
      Loc = MapC[iqdc][chan].iLoc;
      DataEC[NE].iRing = Ring;
      DataEC[NE].iLoc = Loc;
      DataEC[NE].theta = angle[Ring][Loc];
      DataEC[NE].phi= angle2[Ring][Loc];
      DataEC[NE].pos.SetMagThetaPhi(mag[Ring][Loc], angle[Ring][Loc], angle2[Ring][Loc]);
      if(Ring == -1 || Loc == -1) continue;
      float energy = calCeasar->getEnergy(0,id,ienergy+ran->Rndm());
      Histo->RingSum[Ring]->Fill(Loc,1000*energy);
      DataEC[NE].energy = energy;
      Histo->ECeasar[id]->Fill(ienergy);
      Histo->ECCeasar[id]->Fill(energy);
      Histo->TECeasar->Fill(energy);
      DataEC[NE].Total += energy;
      NE++;
    }
    //check for ffff's
    unsigned short f1 = *point;
    point++;
    unsigned short f2 = *point;
    point++;
    if(f1 != 0xffff && f2 != 0xffff) return false;
  }
  Histo->CEMult->Fill(NE);
  Histo->CETMult2D->Fill(NE, NT);
  Nselect = 0;
  bool match = true;
  // match up energies to times
  for (int it = 0; it < NT; it++) Histo->THitmap->Fill(DataTC[it].id);
  for (int ie = 0; ie < NE; ie++) {
    Histo->TEC_NDop->Fill(DataEC[ie].energy);
    DataEC[ie].itime = -1;
    DataEC[ie].time = -1;
    match = false;
    Histo->CHitmapNM->Fill(DataEC[ie].id);
    for (int it = 0; it < NT; it++) {
      if (DataEC[ie].id == DataTC[it].id) { 	      //we have matched
        int iQDC = DataEC[ie].iqdc;
        int iChan = DataEC[ie].ichan;
        if(DataTC[it].itdc != MapC[iQDC][iChan].iTDC || DataTC[it].ichan != MapC[iQDC][iChan].iTDCChan) {
          //cout << "You've done a bad job mapping" << endl;
         //abort();
        }
        DataEC[ie].itime = DataTC[it].itime;
        DataEC[ie].time = DataTC[it].time;
        if(DataEC[ie].iRing !=-1) {
          float dop_energy = Doppler->correct(DataEC[ie].energy,DataEC[ie].theta);
          Histo->TEC_Dop->Fill(DataEC[ie].energy);
          //Histo->TEC_Dop->Fill(dop_energy);
	  select[Nselect] = DataEC[ie];
	  select[Nselect].dop_energy = dop_energy;
	  Nselect++;
	  match = true;
	}  else cout << "Bad Ring" << endl;
      }
      //else if (DataEC[ie].id < DataTC[it].id) break; // no match found
      else if( !match && it == NT - 1) {
        Histo->CHitmap->Fill(DataEC[ie].id);
        if(DataEC[ie].energy > 0.6) Histo->CHitmap1->Fill(DataEC[ie].id);
      }
    }
  }
  Histo->CESMult2D->Fill(NE, Nselect);

  Nadded = 0;
  std::vector<float> sumgamma;
  std::vector<int> myvector;
  for(int i = 0; i < Nselect;i++) {
    bool addedback =0;
    bool supp = 0;
    float sum = 0.;
    sum = select[i].energy;
    if(i != 0) {
      for(int v = 0; v < (int)myvector.size(); v++) {
        if(i == myvector.at(v)) addedback = 1;
      }
    }
    if(addedback) continue;
    else {
      for(int j = i+1;j<Nselect;j++) {
        if(abs(select[i].iRing - select[j].iRing) <= 1) {
          if(abs(select[i].phi - select[j].phi) < 0.5) {
            sum += select[j].energy;
            myvector.push_back(j);
          }
        }
      }
      added[Nadded] = select[i];
      added[Nadded].energy = sum;
      Nadded++;
    }
  }
  Histo->CETMult->Fill(Nselect);
  return true;
}
/***********************************************************/
ceasar::~ceasar() {
  cout << "start ceasar destr" << endl;

  delete tdc[0];
  delete tdc[1];
  delete [] tdc;
  delete Doppler;
  cout << " stop ceasar destr" << endl;
}
/***********************************************************/
void ceasar::reset() {
  Nselect = 0;
}
