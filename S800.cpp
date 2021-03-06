#include "S800.h"

S800::S800(TRandom * ran0, histo_sort * Histo0) {
  Histo = Histo0;
  ran = ran0;
  Init();
}
S800::S800(TRandom * ran0, histo_read * Histo1) {
  Histo_read = Histo1;
  ran = ran0;
  Init();
}

S800::S800(TRandom * ran0, histo_sort * Histo0, histo_read * Histo1, readBeam *beams0, runOptions *opt0) {
  Histo = Histo0;
  Histo_read = Histo1;
  Beams = beams0;
  ran = ran0;
  opt = opt0;
  mapfile = opt->invmap;
  pidpath = opt->pidpath;
  crdcped = opt->crdcped;
  crdcpar = opt->crdcpar;
  crdccal = opt->crdccal;
  Init();
}

void S800::Reset() {
  mTDC.Reset();
  IC.Reset();
  CRDC[0].Reset();
  CRDC[1].Reset();
  beam_pid->Z = 0;
  beam_pid->A = 0;
  for(unsigned int i = 0; i < Beams->pb.size(); i++) {
    residue_pid[i]->Z = 0;
    residue_pid[i]->A = 0;
  }
  track->Reset();
}

S800::~S800() {
  delete beam_pid;
//  delete residue_pid;
}


void S800::Init() {
fail = 0;
  S800Setting =-99;
  string name = "s800_beam";
  beam_pid = new pid(name);

  residue_pid = new pid*[5];
  for(unsigned int i = 0; i < Beams->pb.size(); i++) {//Loop over beams
    name = Form("%s%s",pidpath.c_str(), Beams->pb.at(i).c_str());
    residue_pid[i] = new pid(name);
  }
  /////////////////////////////////////////////////////////////////////////////////////////
  ifstream infile(crdcped.c_str());
  if(!infile.is_open()) {
    cout << "Couldn't open CRDC Pedestal file" << endl;
    abort();
  }

  int CRDC,pad;
  float ped;
  getline(infile,name);
  for(;;) {
    infile >> CRDC >> pad >> ped;
    CRDCPeds[CRDC-1][pad] = (int)ped;
    if(infile.eof()) break;
    if(infile.bad()) break;
  }
  infile.close();
  infile.clear();


  ifstream infile2(crdcpar.c_str());
  if(!infile2.is_open()) {
    cout << "Couldn't open CRDCpars file" << endl;
    abort();
  }
  getline(infile2,name);
  float slope, intercept;
  for(;;) {
    infile2 >> CRDC >> pad >> slope >> intercept;
    Chargeslope[CRDC][pad] = slope;
    Chargeintercept[CRDC][pad] = intercept;
    if(infile2.eof()) break;
    if(infile2.bad()) break;
  }
  infile2.close();
  infile2.clear();

  ifstream infile3(crdccal.c_str());
  if(!infile3.is_open()) {
    cout << "Couldn't open CRDCcal file" << endl;
    abort();
  }
  getline(infile3,name);
  int run;
  string XY;
  for(;;) {
    infile3 >> CRDC >> XY >> run >> slope >> intercept;
    if(XY == "X") {
      CRDCCalRunsX[CRDC].push_back(run);
      CRDCCalSlopeX[CRDC].push_back(slope);
      CRDCCalOffsetX[CRDC].push_back(intercept);
    } else {
      CRDCCalRunsY[CRDC].push_back(run);
      CRDCCalSlopeY[CRDC].push_back(slope);
      CRDCCalOffsetY[CRDC].push_back(intercept);
    }
    if(infile3.eof()) break;
    if(infile3.bad()) break;
  }
  infile3.close();
  infile3.clear();


  //The inverse map is different for each S800 setting and will
  //need to be chaged.
  char mapname[64];
  sprintf(mapname,"%s",mapfile.c_str());
  map = new S800Map(); //initialize the inverse map
  map->LoadInverseMap(mapname);
  track = new S800FpTrack(map);

  ObjCorr[0] = 90; //obj/mrad
  ObjCorr[1] = 0.01; //obj/mm

  gravity_width = 12; //pads
}


bool S800::unpack(unsigned short *&point, int runno) {
  long long int n;
  unsigned short plength, ptag, ID, nwords, words;
//cout << "begin unpack" << endl;
  nwords = *point++;
  nwords--;
  if(*point++ != S800_PACKET) {
    cout << "Didn't find the S800 marker" << endl;
    return false;
  } else {
    nwords--;
  }

  // Unpack S800 data in tree
  // cout << *point << " " << S800_VERSION << endl;
  if(*point++ != S800_VERSION){
    std::cout
      << "DecodedEvent: " 
      << "Wrong version of S800 sub-event. Aborting ..." 
      << std::endl;
    return 1;
  }

  nwords--;
  while(nwords > 0){
    plength = *point; ++point;
    ptag = *point; ++point;
//cout << hex << ptag << endl;
    switch(ptag){
    case S800_TRIGGER_PACKET:
      //cout << "Trigger: " << hex << S800_TRIGGER_PACKET << dec << endl;
      point = DecodeS800Trigger(point);
      break;

    case S800_TOF_PACKET:
      //cout << "Tofr: " << hex << S800_TOF_PACKET << dec << endl;
      point = DecodeS800TimeOfFlight(point);
      break;

    case S800_FP_SCINT_PACKET:
      //cout << "FP SCINT: " << hex << S800_FP_SCINT_PACKET << dec << endl;
      words = plength-2;
      while (words > 0) {
	ID = ((*point)&0xF000)>>12;
	point = DecodeS800Scintillator(point, ID, ID/2);
	words -= 2;
      }
      break;

    case S800_FP_CRDC_PACKET:
      //cout << "CRDC: " << hex << S800_FP_CRDC_PACKET << dec << endl;
      ID = *point; ++point;
      //std::cout << "ID " << ID << std::endl;
      point = DecodeS800Crdc(point, ID, runno);
      break;

    case S800_FP_IC_PACKET:
      //cout << "FP_IC: " << hex << S800_FP_IC_PACKET << dec << endl;
      point = DecodeS800IonChamber(point);
      break;

    case S800_TIMESTAMP_PACKET:
      //cout << "TIMESTAMP: " << hex << S800_TIMESTAMP_PACKET << dec << endl;
      n = *point++;
      n = (*point++<<16|n);
      n = (*point++<<16|n);
      n = (*point++<<16|n);
      //      this->SetInternalTS(n);
      break;

    case S800_EVENT_NUMBER_PACKET:
      //cout << "EVENT_NUMBER: " << hex << S800_EVENT_NUMBER_PACKET << dec << endl;
      n = *point++;
      n = (*point++<<16|n);
      n = (*point++<<16|n);
      //      this->SetEvtNr(n);
      break;

    case S800_FP_HODO_PACKET:
      //cout << "HODO: " << hex << S800_FP_HODO_PACKET << dec << endl;
      point = DecodeS800HodoScope(point);
      break;
    case S800_VME_TDC_PACKET:
      //cout << "VME_TDC: " << hex << S800_VME_TDC_PACKET << dec << endl;
      point = DecodeS800NewMultiHitTDC(point);
      break;
    default: // S800_II_CRDC_PACKET, S800_II_TRACK_PACKET...
      point += plength - 2;
      break;
    }
    nwords -= plength;
  }
//cout << "finished unpack" << endl;
  return true;
}
unsigned short* S800::DecodeS800TimeOfFlight(unsigned short *p) {

  UShort_t words = (*(p-2))-2, ch, dum;
  Short_t rf = -1;
  Short_t obj = -1;
  Short_t xfp = -1;
  Short_t si = -1;
  Short_t tac_obj = -1;
  Short_t tac_xfp = -1;
  while (words > 0) {
    ch = ((*p)&0xf000)>>12;
    int tmp = *p; ++p;
    if     (ch == 12) rf      = (tmp)&0xfff;
    else if(ch == 13) obj     = (tmp)&0xfff;
    else if(ch == 14) xfp     = (tmp)&0xfff;
    else if(ch == 15) si      = (tmp)&0xfff;
    else if(ch ==  5) tac_obj = (tmp)&0xfff;
    else if(ch ==  4) tac_xfp = (tmp)&0xfff;
    else if(ch>0 && ch<8) dum = (tmp)&0xfff;
    words--;
  }
  //  this->GetTimeOfFlight()->Set(rf, obj, xfp, si);
  //this->GetTimeOfFlight()->SetTAC(tac_obj, tac_xfp);

  ToF.rf = rf;
  ToF.obj = obj;
  ToF.xfp = xfp;
  ToF.si = si;
  ToF.tac_obj = tac_obj;
  ToF.tac_xfp = tac_xfp;
  return p;
}

unsigned short* S800::DecodeS800Trigger(unsigned short *p){
  UShort_t words = (*(p-2))-2, ch;
  int registr = -1;
  int s800 = -1;
  int external1 = -1;
  int external2 = -1;
  int secondary = -1;
  registr = *p++;
  words--;
  while(words > 0){
    ch = ((*p)&0xf000)>>12;
    if (ch ==  8) s800 = (*p++)&0xfff;
    if (ch ==  9) external1 = (*p++)&0xfff;
    if (ch == 10) external2 = (*p++)&0xfff;
    if (ch == 11) secondary = (*p++)&0xfff;
    words--;
  }
  //  this->GetTrigger()->Set(registr, s800, external1, external2, secondary);

  Trig.registr = registr;
  Trig.s800 = s800;
  Trig.external1 = external1;
  Trig.external2 = external2;
  Trig.secondary = secondary;
  return p;
}

unsigned short* S800::DecodeS800Scintillator(unsigned short *p, unsigned short updown, int id) {
  int de_up     = -1;
  int time_up   = -1;
  int de_down   = -1;
  int time_down = -1;

  // Even updown: UP channels.  Odd ID: DOWN channels
  if (updown%2==0) {
    de_up     = (*p++)&0xfff;
    time_up   = (*p++)&0xfff;
  }
  else {
    de_down   = (*p++)&0xfff;
    time_down = (*p++)&0xfff;
  }
  //  this->GetScintillator(id)->SetID(id);
  //this->GetScintillator(id)->Set(de_up, time_up, de_down, time_down);

  Scint[id].de_up = de_up;
  Scint[id].time_up = time_up;
  Scint[id].de_down = de_down;
  Scint[id].time_down = time_down;
  return p;
}

unsigned short* S800::DecodeS800HodoScope(unsigned short *p){
  UShort_t words = (*(p-2))-2;
  UShort_t id;
  UShort_t ch;
  UShort_t energy;
  int nHodo = 0;
  while (words > 0) {
    id = *p;
    if (id == 0) {
      p++;
      words--;
      while (words > 0) {
	ch = (((*p)&0xF000)>>12);
	energy = ((*p)&0x0FFF);
	//	this->GetHodoscope(ch)->SetEnergy((Int_t)energy);
	Hodo[nHodo].id = ch;
	Hodo[nHodo].energy = (Int_t)energy;
	nHodo++;
	p++;
	words--;
      }
    } else if (id == 1) {
      p++;
      words--;
      while (words > 0) {
	ch = (((*p)&0xF000)>>12) + 16;
	energy = ((*p)&0x0FFF);
	//	this->GetHodoscope(ch)->SetEnergy((Int_t)energy);
	Hodo[nHodo].id = ch;
	Hodo[nHodo].energy = (Int_t)energy;
	nHodo++;
	p++;
	words--;
      }
    } else if (id == 2) {
      p++;
      words--;
      while (words > 0) {
	// coincidence register A (for the first  16 channels)
	p++; words--;
	// coincidence register B (for the second 16 channels)
	p++; words--;
	// TAC time
	p++; words--;
      }
    } else {
      p++; words--;
    }
  }
  return p;
}

unsigned short* S800::DecodeS800Crdc(unsigned short *p, int id, int runno){
  UShort_t anode=-1;
  UShort_t tac  =-1;
  //  this->GetCrdc(id)->SetID(id);

  Int_t tag;
  tag = S800_FP_CRDC_PACKET;
  if (*(p+1) == tag+1) {
    p = DecodeS800CrdcRaw(p,id);
  }
  if (*(p+1) == tag+5) {
    anode = *(p+2);
    tac = *(p+3);
    p += 4;
  }

  int r = 0;
  for(unsigned int i = 0; i < CRDCCalRunsY[id].size(); i++) {
    if(runno < CRDCCalRunsY[id].at(i)) {
      r = i-1;
      break;
    } else if(i == CRDCCalRunsY[id].size() - 1) {
      r = CRDCCalRunsY[id].size() - 1;
      break;
    }
  }
  CRDC[id].anode = anode;
  CRDC[id].tac = tac;
  if(r < (int)CRDCCalRunsY[id].size() - 1) {
    int rdiff = CRDCCalRunsY[id].at(r+1) - CRDCCalRunsY[id].at(r);
    double mix1 = (double)(CRDCCalRunsY[id].at(r+1) - runno)/rdiff;
    double mix2 = (double)(runno - CRDCCalRunsY[id].at(r))/rdiff;
    CRDC[id].calY = (tac*CRDCCalSlopeY[id].at(r) + CRDCCalOffsetY[id].at(r))*mix1 + (tac*CRDCCalSlopeY[id].at(r+1) + CRDCCalOffsetY[id].at(r+1))*mix2;
  } else {
    CRDC[id].calY = tac*CRDCCalSlopeY[id].at(r) + CRDCCalOffsetY[id].at(r);
  }
  return p;
}

unsigned short* S800::DecodeS800IonChamber(unsigned short *p){
  UShort_t ch=-1;
  UShort_t raw=-1;
  if (*(p+1) == S800_FP_IC_ENERGY_PACKET) {
    // IC packet with times
    UShort_t length;
    length = *p++;
    p++;
    length -= 2;
    while (length > 0) {
      ch  = ((*p)&0xf000)>>12;
      raw = (*p++)&0xfff;
      length--;

      IC.raw[ch] = raw;
      IC.sum += raw;
      Histo->ICSummary->Fill(ch,raw);
    }
    IC.sum /= 16.;
  } else {
    // Old style IC packet
    UShort_t words = (*(p-2))-2;
    while(words > 0){
      ch = ((*p)&0xf000)>>12;
      raw = (*p++)&0xfff;
      words--;
      //this->G      cout << ch << " " << raw << endl;etIonChamber()->Set(ch,raw);
      IC.raw[ch] = raw;
      IC.sum += raw;
    }
  }
  //IC.Scaledsum = IC.sum * IC.ScalingFactor;

  return p;
}

unsigned short* S800::DecodeS800CrdcRaw(unsigned short *p, int id){
//   cout << "Begin CRDC "<< id << " decoding" << endl;
  static ULong_t total=0, failed=0;
  Short_t sampleBegin = 0, sampleWidth=0, isample, ichannel, cdata[4], connector, previous_sample = 0, ch;
  Short_t sindex=0;
  Short_t previous_channel=0;
  Short_t maxwidth = S800_CRDC_MAXWIDTH;
  Short_t channels;
  channels = S800_FP_CRDC_CHANNELS;

  unsigned short *pStore = p;
  bool mes1=true, mes2=true, mes3=true, mes4=true;
  bool debug = S800_DEBUG;
  UShort_t length = *p++;
//cout << hex << "Length\t" << *p << "\t" << length << endl;
  short i = length-3;
  p++;	// skip packet id
  UShort_t threshold = *p++;

//    cout << "Decoding CRDC " << id << " " << i << "\t" << hex << *pStore << dec << "\t" <<*pStore << endl;
  while(i > 0){
//cout << dec << "\t" << i << hex << "\t" << *p << endl;
    if ((*p)>>15 != 1) {
      std::cout 
	<< "DecodedEvent: " 
	<< "CRDC data is corrupted!" 
	<< std::endl;
      p++; i--;
      continue;
    } else{
//cout << dec << "Sample\t" << isample << hex << "\t" << *p << endl;
      isample  = ((*p)&0x7FC0)>>6;
      ichannel =  (*p)&0x003F;
      if (i == length-3){
	sampleBegin     = isample;
	previous_sample = isample;
      }
      if(previous_channel > ichannel) sindex++;
      previous_channel = ichannel;
    }
    p++; i--;
    memset(cdata, 0, sizeof(cdata));
    while ((*p)>>15 == 0) {
      connector = ((*p)&0x0C00)>>10;
      cdata[connector] = (*p)&0x3FF;
      p++; i--;
      if (i == 0) break;
    }
    if(isample < sampleBegin || isample > sampleBegin+maxwidth){
      if(debug)
	printf("Warning in Crdc Unpack: inconsistent sample number: %d (first: %d)\n", 
	       isample, sampleBegin);
      mes1 = false;
      //  continue;
    }
    if(isample < previous_sample){
      if(debug)
	printf("Warning in Crdc Unpack: sample number lower than previous: %d (previous: %d)\n", 
	       isample, previous_sample);
      mes2 = false;
      //      continue;
    }
    previous_sample = isample;
    for(int j=0; j<4; j++){
      ch = ichannel+j*64;
      if (cdata[j] != 0 && ch < channels){
	if (cdata[j] < threshold) {
	  if (debug)
	    printf("Warning in Crdc Unpack: data lower than threshold: %d (threshold: %d)\n", cdata[j], threshold);
	  mes3 = false;
	} else {
//	  std::cout << "ch " << ch << " cdata[j]" << cdata[j] << " isample " << isample << std::endl;
	  //	  this->GetCrdc(id)->Set(cdata[j], isample, ch);
	  CRDC[id].cdata[ch][sindex] = cdata[j];
	}
      } 
      else if (cdata[j] != 0 && ch >= channels){
	if (debug) {
	  printf("Warning in Crdc Unpack: channel greater than limit: %d (limit: %d)\n", ch, channels);
	}
	mes4 = false;
      }
    }
    //    sampleWidth = isample - sampleBegin + 1;
    sampleWidth = sindex + 1;
    //    cout << "sample width = " << sampleWidth << " " << isample - sampleBegin+1  << endl;
    //    this->GetCrdc(id)->SetSampleWidth(sampleWidth);
    CRDC[id].samplewidth = sampleWidth;
  }

  CRDCIntegrate(id);
//cout << "End Decode" << endl;
  if (!mes1 || !mes2 || !mes3 || !mes4) failed++;
  total++;
//  if (failed == 1000) {
    if (debug)
      printf ("Errors in Crdc Unpackings: %g%%\n", 1.0*failed/total*100);
 //   total = 0;
 //   failed = 0;
//  }
  return (pStore+length);
}

void S800::CRDCIntegrate(int id) { //////////////// CRDC slope + offset

  int PadMult=0;
  for(int i = 0; i < NPads; i++) {
    float raw = -1;
    float cal = -1;
    int nsamples=0;
    for(int s=0;s<CRDC[id].samplewidth;s++) {
//      cout << "crdc " << id << " pad " << i << " sample "<< CRDC[id].cdata[i][s] << endl;
      if(CRDC[id].cdata[i][s] !=0) {
        nsamples++;
        raw += (CRDC[id].cdata[i][s] - CRDCPeds[id][i]);
        raw = (raw +1)/nsamples;
        cal = raw;
//        cout << "Raw/Cal\t" << raw << " "<< cal << endl;
      }
    }
    if(nsamples>0) {
      CRDC[id].raw[PadMult] = raw;
      CRDC[id].cal[PadMult] = cal;
      CRDC[id].pad[PadMult] = i;
      if(CRDC[id].raw[PadMult] !=-1) PadMult++;
    }
  }
  CRDC[id].PadMult=PadMult;
}

unsigned short* S800::DecodeS800NewMultiHitTDC(unsigned short *p) {
  //Data should be interpreted in 16-bit words 

  //Declare temporary arrays to hold the raw data
  unsigned short data[32][32];
  signed short hits[32];
  unsigned short raw[32];
  for (int i=0;i<32;i++){
    hits[i]=-1;
    raw[i]=0;
    for (int j=0;j<32;j++){
      data[i][j]=0;
    }
  }

  UShort_t length, ch, hit;
  length = *(p-2);
  length -= 2;
  while (length > 0) {
    ch = (*p)&0xFF;
    hit = (*p++)>>8;
    if (hit < 32)
      data[ch][hit] = *p++;
    else
      p++;
    if (hit == 0) raw[ch] = data[ch][0];
    if (hit > hits[ch]) hits[ch] = hit;
    length -= 2;
  }



  if (raw[15] != 0) {
    for (int i=0; i<13; i++) {
      switch(i) {
      case 0: // e1up
	if (hits[0] >= 0){   
	  //	  fMultiHitTOF.fE1Up.push_back( (data[0][0] - raw[15]) * 0.0625);
	  mTDC.e1up.push_back( (data[0][0] - raw[15]) * 0.0625);
	}
	break;
      case 1: // e1down
	if (hits[1] >= 0){
	  //	  fMultiHitTOF.fE1Down.push_back((data[1][0] - raw[15]) * 0.0625);
	  mTDC.e1down.push_back((data[1][0] - raw[15]) * 0.0625);
	}
	break;
      case 2: // xf
	if (hits[2] >= 0) {
	  for (int j=0; j<=hits[2]; j++){
	    //	    fMultiHitTOF.fXf.push_back((data[2][j] - raw[15]) * 0.0625);
	    mTDC.xfp.push_back((data[2][j] - raw[15]) * 0.0625);
	  }
	}
	break;
      case 3: // obj
	if (hits[3] >= 0) {
	  for (int j=0; j<=hits[3]; j++){
	    //	    fMultiHitTOF.fObj.push_back( (data[3][j] - raw[15]) * 0.0625);
	    mTDC.obj.push_back( (data[3][j] - raw[15]) * 0.0625);
	  }
	}
	break;
      case 4: // galotte
	if (hits[4] >= 0) {
	  for (int j=0; j<=hits[4]; j++){
	    // fMultiHitTOF.fGalotte.push_back( (data[4][j] - raw[15]) * 0.0625);
	    mTDC.galotte.push_back( (data[4][j] - raw[15]) * 0.0625);
	  }
	}
	break;
      case 5: // rf
	if (hits[5] >= 0) {
	  for (int j=0; j<=hits[5]; j++){
	    //	    fMultiHitTOF.fRf.push_back( (data[5][j] - raw[15]) * 0.0625);
	    mTDC.rf.push_back((data[5][j] - raw[15]) * 0.0625);
	  }
	}
	break;
      case 12: // hodoscope
	if (hits[12] >= 0) {
	  for (int j=0; j<=hits[12]; j++){
	    //	    fMultiHitTOF.fHodoscope.push_back( (data[12][j] - raw[15]) * 0.0625);
	    mTDC.hodoscope.push_back((data[12][j] - raw[15]) * 0.0625);
	  }
	}
	break;
      default:
	break;
      }//end Switch i
    }//end for i
  }//end if raw[15]!=0

  // cout<<"fNewTOF.fE1Up.size() "<<fNewTOF.fE1Up.size()<<endl;
  // cout<<"fNewTOF.fE1Down.size() "<<fNewTOF.fE1Down.size()<<endl;
  // cout<<"fNewTOF.obj.size() "<<fNewTOF.fObj.size()<<endl;

  return p;
}


void S800::CalculateGravity(int id) {
  int maxpad = 0;
  int padmax = 0;
  for(int i=0;i<CRDC[id].PadMult-1;i++) {
    if(CRDC[id].cal[i] > 0 && CRDC[id].cal[i+1] > 0 && CRDC[id].cal[i] > maxpad) {
      maxpad = CRDC[id].cal[i];
      padmax = CRDC[id].pad[i];
    }
  }
  if(CRDC[id].PadMult%2 != 0) padmax = CRDC[id].pad[CRDC[id].PadMult/2];
  else  padmax = CRDC[id].pad[CRDC[id].PadMult/2+1];
  if(padmax ==0) {
    CRDC[id].padsum = 0;
    CRDC[id].x_gravity = -500;
//    cout << "padmax = 0 for crdc " << id << " mult = " << CRDC[id].PadMult << "\t" << CRDC[id].PadMult << endl;
    return;
  }

  int lowpad = (int)padmax - (int)gravity_width/2;
  int highpad = lowpad + (int)gravity_width -1;
  if(lowpad < 0) {
    lowpad =0;
  }
  if(highpad >= 224) {
    highpad = 223;
  }

  float sum=0.0, mom =0.0;
  for(int i=0;i<CRDC[id].PadMult; i++) {
    if(CRDC[id].pad[i] >=lowpad && CRDC[id].pad[i] <=highpad) {
      sum += CRDC[id].cal[i];
      mom += CRDC[id].pad[i]*CRDC[id].cal[i];
    }
  }
  float pad = mom/sum;
  CRDC[id].padsum = sum;
  CRDC[id].x_gravity = mom/sum * CRDCCalSlopeX[id].at(0) + CRDCCalOffsetX[id].at(0);
}


bool S800::analyze()  {
  for(int i=0;i<(int)mTDC.e1up.size();i++) {
    Histo->Te1up->Fill(mTDC.e1up.at(i));
  }
  for(int i=0;i<(int)mTDC.e1down.size();i++) {
    Histo->Te1down->Fill(mTDC.e1down.at(i));
  }
  Histo->CRDC1PadMult->Fill(CRDC[0].PadMult);
  Histo->CRDC2PadMult->Fill(CRDC[1].PadMult);
  Histo->CRDC1Tac->Fill(CRDC[0].tac);
  Histo->CRDC2Tac->Fill(CRDC[1].tac);
  Histo->CRDC1Y->Fill(CRDC[0].calY);
  Histo->CRDC2Y->Fill(CRDC[1].calY);
  Histo->CRDC1AnodevsTac->Fill(CRDC[0].anode,CRDC[0].tac);
  Histo->CRDC2AnodevsTac->Fill(CRDC[1].anode,CRDC[1].tac);

  for(int i = 0;i < 2; i++) {
    CalculateGravity(i);
    for(int p = 0;p < CRDC[i].PadMult; p++) {
      if(i==0) {
        Histo->CRDC1raw->Fill(CRDC[i].raw[p]);
        Histo->CRDC1Summary->Fill(CRDC[i].pad[p],CRDC[i].raw[p]);
        Histo->CRDC1Summary_cal->Fill(CRDC[i].pad[p],CRDC[i].cal[p]);
      } else {
	Histo->CRDC2raw->Fill(CRDC[i].raw[p]);
	Histo->CRDC2Summary->Fill(CRDC[i].pad[p],CRDC[i].raw[p]);
	Histo->CRDC2Summary_cal->Fill(CRDC[i].pad[p],CRDC[i].cal[p]);
      }
    }
  }

  if(CRDC[0].x_gravity == -500 || CRDC[1].x_gravity ==-500) {
//cout << "Fail x_gravity\t" << CRDC[0].x_gravity << "\t" << CRDC[1].x_gravity << "\t" << fail << endl;
fail++;
    return false;
  }
  Histo->CRDC1X->Fill(CRDC[0].x_gravity);
  Histo->CRDC2X->Fill(CRDC[1].x_gravity);
  Histo->CRDC1XY->Fill(CRDC[0].x_gravity,CRDC[0].tac);
  Histo->CRDC2XY->Fill(CRDC[1].x_gravity,CRDC[1].tac);
  Histo->CRDC1XCalY->Fill(CRDC[0].x_gravity,CRDC[0].calY);
  Histo->CRDC2XCalY->Fill(CRDC[1].x_gravity,CRDC[1].calY);

  if((int)mTDC.obj.size()>0) {
    if((int)mTDC.xfp.size()>0) {
      bool stat = beam_pid->getPID(mTDC.obj.at(0),mTDC.xfp.at(0));
      if(!stat) {
//	cout << "Beam PID" << endl;
        return false;
      }
      int PBidx = Beams->BeamID(beam_pid->Z, beam_pid->A, Beams->pb);
      if(PBidx == -1) return false;
      track->CRDC[0] = CRDC[0];
      track->CRDC[1] = CRDC[1];
      Histo->ObjvsXFP->Fill(mTDC.obj.at(0),mTDC.xfp.at(0));
      //Tracking through the focal plane
      track->CalculateTracking(beam_pid->A, beam_pid->Z);
      float correctedObj = mTDC.obj.at(0) + ObjCorr[0]*track->afp + ObjCorr[1]*CRDC[0].x_gravity;
      mTDC.objCorrected.push_back(correctedObj);
      if(beam_pid->Z >0 && beam_pid->A >0) {
        bool stat2 = residue_pid[PBidx]->getPID(mTDC.objCorrected.at(0),IC.sum);
//        if(!stat2) return false;
        track->CRDC[0] = CRDC[0];
        track->CRDC[1] = CRDC[1];
        track->CalculateTracking(residue_pid[PBidx]->A, residue_pid[PBidx]->Z);
        track->GetThetaPhi(track->ata, track->bta);
        //Looking at corrections
        Histo->Objvsafp->Fill(mTDC.objCorrected.at(0),track->afp);
        Histo->ObjvsCRDC1X->Fill(mTDC.objCorrected.at(0),CRDC[0].x_gravity);
      } else {
//        cout << "\nBAD PID" << endl;
        return false;
      }
    } else {
//cout << "XFP" << endl;
      return false;
    }
  } else {
//cout << "Fail obj" << endl;
    return false;
  }
  return true;
}

/************************************************************/
/* S800Map Class - Functions                                */
/************************************************************/

S800Map::S800Map() {
  /* Parameters */
  //  m_top = s800;

  /* Variables */
  maxcoefficients = S800_TRACK_COEFFICIENTS;
  maxparameters = S800_TRACK_PARAMETERS;
  maxorder = 0.0;

  /* Internal use values */
  for (int i=0; i<S800_TRACK_PARAMETERS; i++) {
    maxcoefficient[i] = 0;
    for (int j=0; j<S800_TRACK_COEFFICIENTS; j++) {
      order[i][j] = 0;
      coefficient[i][j] = 0;
      for (int k=0; k<S800_TRACK_PARAMETERS; k++) {
	exponent[i][k][j] = 0;
      }
    }
  }

  // LoadInverseMap(filename);

}

/* destructor */
S800Map::~S800Map()
{
}

void S800Map::LoadInverseMap(TString filename) {
  cout << "Loading S800 inverse map " << filename.Data() << "...";

  FILE *file;
  if ( (file = fopen(filename.Data(), "r")) == NULL ) {
    printf("\n Inverse map file %s was not found!\n", filename.Data());
    return;
  }

  char line[400];

  unsigned short index;
  double icoefficient;
  unsigned short iorder;
  unsigned short iexp0, iexp1, iexp2, iexp3, iexp4, iexp5;
  int parameter = 0;
  char* retVal;

  retVal = fgets(line, 400, file);
  while (strncmp(line, "     I", 6) != 0) { 
    retVal = fgets(line, 400, file);
  }
  parameter = 0;
  while (!feof(file)) {
    retVal = fgets(line, 400, file);
    while (strncmp(line, "    ---", 7) != 0) {
      sscanf(line, "%hd %lf %hd %hd %hd %hd %hd %hd %hd", &index, 
	     &icoefficient, &iorder, &iexp0, &iexp1, &iexp2, &iexp3, 
	     &iexp4, &iexp5);
      if (iorder > maxorder) { maxorder = (double)iorder; }
      if (index > maxcoefficients) {
	cout << "Error: too many coefficients in map" << endl;
	cout << " Please increase S800_TRACK_COEFFICIENTS and "<< endl
	     << " recompile. " << endl;
	break;
      }

      if (parameter >= S800_TRACK_PARAMETERS) {
	cout << "Invalid parameter number, must be between 0 and " 
	     << S800_TRACK_PARAMETERS-1 << endl;
      }
      if (index >= S800_TRACK_COEFFICIENTS) {
	cout << "Too many coefficients - maximum is " 
	     << S800_TRACK_COEFFICIENTS << endl;
      }
      if (index >= maxcoefficient[parameter]) { 
	maxcoefficient[parameter] = index+1;
      }
      order[parameter][index] = iorder;
      exponent[parameter][0][index] = iexp0;
      exponent[parameter][1][index] = iexp1;
      exponent[parameter][2][index] = iexp2;
      exponent[parameter][3][index] = iexp3;
      exponent[parameter][4][index] = iexp4;
      exponent[parameter][5][index] = iexp5;
      coefficient[parameter][index] = icoefficient;
      retVal = fgets(line, 400, file);
    }
    retVal = fgets(line, 400, file);
    parameter++;
    if (parameter > maxparameters) {
      cout << "Error: too many parameters in map" << endl;
      cout << " Please increase S800_TRACK_PARAMETERS and " << endl
	   << " recompile. " << endl;
      break;
    }
  }
  fclose(file);
  cout << "Done!" << endl;
}

void S800Map::Reset() {
  /* Internal use values */
  for (int i=0; i<S800_TRACK_PARAMETERS; i++) {
    //maxcoefficient[i] = 0;
    for (int j=0; j<S800_TRACK_COEFFICIENTS; j++) {
      //order[i][j] = 0;
      //coefficient[i][j] = 0;
      for (int k=0; k<S800_TRACK_PARAMETERS; k++) {
	//exponent[i][k][j] = 0;
      }
    }
  }
}

double S800Map::Calculate(int calcorder, int parameter, double *input) {
  double cumul = 0;
  double multiplicator;
  for (int index=0; index<maxcoefficient[parameter]; index++) {
    if (calcorder < order[parameter][index]) { break; }
    multiplicator = 1;

    for (int nex=0; nex<S800_TRACK_PARAMETERS; nex++) {
      if (exponent[parameter][nex][index] != 0) {
	multiplicator *= pow(input[nex], exponent[parameter][nex][index]);
      }
    }
    cumul += multiplicator * coefficient[parameter][index];
  }
  return cumul;
}
/************************************************************/
/* S800FpTrack Class - Functions                            */
/************************************************************/
S800FpTrack::S800FpTrack(S800Map *map0) {

  map = map0;

  /* Parameters */
  xfp = -65000.;
  afp = -65000.;
  yfp = -65000.;
  bfp = -65000.;
  ata = -65000.;
  yta = -65000.;
  bta = -65000.;
  dta = -65000.;
  azita = -65000.;
  scatter = -65000.;
  energy = -65000.;
  ptot = -65000.;
  ppar = -65000.;
  ptra = -65000.;

  ata_cor = -65000.;
  bta_cor = -65000.;
  azita_cor = -65000.;
  scatter_cor = -65000.;

  /* Variables */
  anglea = 0; /* degree */
  angleb = 0; /* degree */

  anglea_cor = 0; /* degree */
  angleb_cor = 0; /* degree */
  bta_ytacor = 0; /* degree/yta */
  ata_dtacor = 0; /* degree/dta */

  brho = 2.426; /* Tm */
  mass = 17; /* amu */
  charge = 10; /* q */
  order = 5; /* order */
  zfp = 0; /* m */
  gecorr = 0; /* keV/dta */
  gap = 1073.0; /* mm */
}
S800FpTrack::~S800FpTrack() {
}

void S800FpTrack::Reset() {
  /* Parameters */
  xfp = -65000.;
  afp = -65000.;
  yfp = -65000.;
  bfp = -65000.;
  ata = -65000.;
  yta = -65000.;
  bta = -65000.;
  dta = -65000.;
  azita = -65000.;
  scatter = -65000.;
  energy = -65000.;
  ptot = -65000.;
  ppar = -65000.;
  ptra = -65000.;

  ata_cor = -65000.;
  bta_cor = -65000.;
  azita_cor = -65000.;
  scatter_cor = -65000.;

  CRDC[0].Reset();
  CRDC[1].Reset();

  theta = -999;
  phi = -999;
  thetadeg = -999;
  phideg = -999;
  //  map.Reset();
}

void S800FpTrack::CalculateTracking(int A, int Z) {
  double input[S800_TRACK_PARAMETERS];
  double pi = 3.14159265;
  double amu = 931.5016;
  double beta, betagamma0, gamma0, gamma, energy0, ptot0;
  if (!map->WasLoaded()) { cout << "\nNo Map Loaded" << endl;return; }

  afp = atan((CRDC[1].x_gravity - CRDC[0].x_gravity) / gap);
  bfp = atan((CRDC[1].calY - CRDC[0].calY) / gap);

  xfp = CRDC[0].x_gravity / 1000. + zfp * tan(afp);
  yfp = CRDC[0].calY / 1000. + zfp * tan(bfp);

  /* x (dispersive) axis is reversed between COSY, and the
     local coordinate system, where x>0 is down. */

  input[0] = -xfp;
  input[1] = -afp;
  input[2] = yfp;
  input[3] = bfp;

  if ((double)order > (double)map->maxorder) { order = map->maxorder; }
  ata = map->Calculate((int)order, 0, input);
  yta = map->Calculate((int)order, 1, input);
  bta = map->Calculate((int)order, 2, input);
  dta = map->Calculate((int)order, 3, input);
  ata += anglea; /* Add theta offset in radians */
  bta += angleb; /* Add phi offset in radians */

  ata_cor = ata + dta*ata_dtacor;
  bta_cor = bta + yta*bta_ytacor;

  ata_cor += anglea_cor/180.*pi;
  bta_cor += angleb_cor/180.*pi;

  double xsin = sin(ata);
  double ysin = sin(bta);

  if ( (xsin > 0) && (ysin > 0) ) {
    azita = atan(ysin/xsin);
  } else if ( (xsin < 0) && (ysin > 0) ) {
    azita = pi - atan(ysin/fabs(xsin));
  } else if ( (xsin < 0) && (ysin < 0) ) {
    azita = pi + atan(fabs(ysin)/fabs(xsin));
  } else if ( (xsin > 0) && (ysin < 0) ) {
    azita = 2*pi - atan(fabs(ysin)/xsin);
  } else {
    azita = 0.0;
  }

  scatter = asin(sqrt( (xsin*xsin) + (ysin*ysin) ))*1000; /* mrad */
  /* Same for corrected ata_cor and bta_cor */
  xsin = sin(ata_cor);
  ysin = sin(bta_cor);

  if ( (xsin > 0) && (ysin > 0) ) {
    azita_cor = atan(ysin/xsin);
  } else if ( (xsin < 0) && (ysin > 0) ) {
    azita_cor = pi - atan(ysin/fabs(xsin));
  } else if ( (xsin < 0) && (ysin < 0) ) {
    azita_cor = pi + atan(fabs(ysin)/fabs(xsin));
  } else if ( (xsin > 0) && (ysin < 0) ) {
    azita_cor = 2*pi - atan(fabs(ysin)/xsin);
  } else {
    azita_cor = 0.0;
  }
  scatter_cor = asin(sqrt( (xsin*xsin) + (ysin*ysin) ))*1000; /* mrad */
  mass = A;
  charge = Z;
  betagamma0 = brho / 3.107 * charge / mass;
  gamma0 = sqrt( (betagamma0 * betagamma0) + 1);
  beta0 = betagamma0 / gamma0;
  energy0 = mass * amu * (gamma0 - 1); /* MeV */
  ptot0 = energy0 * sqrt(1 + 2 * mass * amu / energy0); /* MeV/c */
  energy = (1 + dta) * energy0; /* MeV */
  gamma = 1 + energy / mass / amu;
  beta = sqrt(1 - 1 / gamma / gamma);
  deltabeta = beta;// - beta0;
  ptot = sqrt(energy*energy+2*mass*amu*energy);
  ppar = ptot * cos(scatter/1000.);
  ptra = ptot * sin(scatter/1000.);
}

void S800FpTrack::GetThetaPhi(double ata0, double bta0) {
  double sineata = sin(ata0);
  double sinebta = sin(bta0);
  double result = sqrt(sineata*sineata + sinebta*sinebta);
  if(result >1) {
    result = 1;
  } else if(result < -1) {
    result =-1;
  }
  phi = atan2(sinebta,sineata);
  theta = asin(result);
  phideg = phi*180./acos(-1);
  thetadeg = theta*180./acos(-1);
}
