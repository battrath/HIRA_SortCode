#include "histo_sort.h"
/***********************************************************/
histo_sort::histo_sort(readBeam * beams, string outfile) {
  ostringstream outstring;
  string name;
  file = new TFile (outfile.c_str(), "RECREATE");
  file->cd();
  NCsI = 20;
  Nstrip = 128;
  Nceasar = 192;
  Nring = 8;
  dirdee = new TDirectoryFile("dee","dee");
  dirdee->cd();
  dee = new TH2I*[NCsI];
  dee_S800 = new TH2I*[NCsI];
  for(int icsi = 0; icsi < NCsI; icsi++) {
    dirdee->cd();
    dee[icsi] = new TH2I(Form("DEE_%i",icsi),"",1024,0,4095,1024,0,100);//4096
    dee_S800[icsi] = new TH2I(Form("DEE_S800_%i",icsi),"",1024,0,4095,1024,0,100);//4096
  }
  file->cd();
  //directory for Summary Spectra
  dirSum = new TDirectoryFile("Sum","Hira_Sum");

  //Pies of the S4
  dirPies = new TDirectoryFile("pies","pies");
  dirPiesRaw = dirPies->mkdir("raw","raw");
  dirPiesCal = dirPies->mkdir("cal","cal");
  dirPiesLGR = dirPies->mkdir("lgr","lgr");
  dirPiesTime = dirPies->mkdir("time","Time");
  dirTotCal = dirPies->mkdir("TotCal","TotCal");

  //Rings of the S4
  dirRings = new TDirectoryFile("rings","rings");
  dirRingsRaw = dirRings->mkdir("raw","raw");
  dirRingsCal = dirRings->mkdir("cal","cal");
  dirRingsLGR = dirRings->mkdir("lgr","lgr");
  dirRingsTime = dirRings->mkdir("time","Time");

  //directories for Ceasar
  dirC = new TDirectoryFile("ceasar","ceasar");
  dirEnergy = dirC->mkdir("raw","energyC");
  dirTime = dirC->mkdir("timeC","timeC");
  dirEcal = dirC->mkdir("cal","calc");
  dirRSum = dirC->mkdir("RSum","Rsum");
  dirCSum = dirC->mkdir("CSum","Rsum");

  //fiber
  dirFiber = new TDirectoryFile("Fiber","Fiber");
  dirS800 = new TDirectoryFile("S800","S800");
  dirS800Raw = dirS800->mkdir("Raw","Raw");
  dirS800Cal = dirS800->mkdir("Cal","Cal");
  dirS800PID = dirS800->mkdir("PID","PID");
  dirS800Trig = dirS800->mkdir("Trig","Trig");
  dirThetaPhi = new TDirectoryFile("ThetaPhi","ThetaPhi");

  dirFiber->cd();
  VertA = new TH1I("VertA","",2048,0,2048);
  VertB = new TH1I("VertB","",2048,0,2048);
  VertC = new TH1I("VertC","",2048,0,2048);
  VertD = new TH1I("VertD","",2048,0,2048);
  VertDyn = new TH1I("VertDyn","",2048,0,2048);
  VertHitMap = new TH2I("VertHitMap","",500,-500,500,500,-500,500);

  HorzA = new TH1I("HorzA","",2048,0,2048);
  HorzB = new TH1I("HorzB","",2048,0,2048);
  HorzC = new TH1I("HorzC","",2048,0,2048);
  HorzD = new TH1I("HorzD","",2048,0,2048);
  HorzDyn = new TH1I("HorzDyn","",2048,0,2048);
  HorzHitMap = new TH2I("HorzHitMap","",500,-500,500,500,-500,500);
  HorzHitMap_energygate = new TH2I("HorzHitMap_energygate","",2000,-500,500,2000,-500,500);

  DynodevsSum = new TH2I("DynodevsSum","",1024,0,4095,1024,0,4095);
  AvsSum = new TH2I("ASum","",1024,0,4095,1024,0,4095);
  BvsSum = new TH2I("BvsSum","",1024,0,4095,1024,0,4095);
  CvsSum = new TH2I("CvsSum","",1024,0,4095,1024,0,4095);
  DvsSum = new TH2I("DvsSum","",1024,0,4095,1024,0,4095);

  Fiber_X = new TH1I("Fiber_X","",200,-10,10);
  Fiber_Y = new TH1I("Fiber_Y","",200,-10,10);
  Fiber_XY = new TH2I("Fiber_XY","",200,-10,10,200,-10,10);

  CornerMult = new TH1I("CornerMult","",9,-0.5,8.5);
  CornerMult_Vert = new TH1I("CornerMult_Vert","",5,-0.5,4.5);
  CornerMult_Hori = new TH1I("CornerMult_Hori","",5,-0.5,4.5);
  CornerMult_VvsH = new TH2I("CornerMult_VvsH","",5,-0.5,4.5,5,-0.5,4.5);

  file->cd();

  dirRing = new TDirectoryFile("Ring","Ring");
  dirRing->cd();
  protonHitMap = new TH2I("protonHitMap","",200,-12,12,200,-12,12);
  csi0 = new TH2I("csi0","",128,0,128,256,0,256);
  csi4 = new TH2I("csi4","",128,0,128,256,0,256);
  csi8 = new TH2I("csi8","",128,0,128,256,0,256);
  csi12 = new TH2I("csi12","",128,0,128,256,0,256);
  csi16 = new TH2I("csi16","",128,0,128,256,0,256);
  csi17 = new TH2I("csi17","",128,0,128,256,0,256);
  csi18 = new TH2I("csi18","",128,0,128,256,0,256);
  csi19 = new TH2I("csi19","",128,0,128,256,0,256);
  csiInner = new TH1I("csiInner","",256,0,256);
  csiOuter = new TH1I("csiOuter","",256,0,256);
  csiPie = new TH2I("csiPie","",20,0,20,128,0,128);
  file->cd();

  //CsI
  dirCsI = new TDirectoryFile("CsI","CsI");
  dirCsIRaw = dirCsI->mkdir("CsIRaw","raw");
  dirCsITime = dirCsI->mkdir("CsITime","time");
  dirCsISum = dirCsI->mkdir("CsISum","sum");
  dirCsIGate = dirCsI->mkdir("CsIGate","CsIGate");
  dirCsIMult = dirCsI->mkdir("CsIMult","CsIMult");
  dirCsIET = dirCsI->mkdir("CsIET","CsIET");
  dirCsIpTheta = dirCsI->mkdir("CsIpTheta","proton_theta_dependentce");
  dirCsIpPhi = dirCsI->mkdir("CsIpPhi","proton_phi_dependece");
  dirCsIpRing = dirCsI->mkdir("CsIpRing","proton_ring_dependece");
  dirCsIaTheta = dirCsI->mkdir("CsIaTheta","alpha_theta_dependence");
  dirCsIaPhi = dirCsI->mkdir("CsIaPhi","alpha_phi_dependece");
  dirCsIaRing = dirCsI->mkdir("CsIaRing","alpha_ring_dependece");

  dirS800Trig->cd();
  singles_trig_time = new TH1I("singles_trig_time","",1024,0,2000);
  S800_Csi_time = new TH1I("S800_CsI_time","",1024,0,2000);
  S800_Csi_time_with_proton = new TH1I("S800_CsI_time_with_proton","",1024,0,2000);

  ECsI = new TH1I*[NCsI];
  TCsI = new TH1I*[NCsI];
  ET_csi = new TH2I*[NCsI];

  p_ECsI_theta = new TH2I*[NCsI];
  p_ECsI_thetaCal = new TH2I*[NCsI];

  p_ECsI_phi = new TH2I*[NCsI];
  p_ECsI_ring = new TH2I*[NCsI];
  alpha_ECsI_theta = new TH2I*[NCsI];
  alpha_ECsI_ring = new TH2I*[NCsI];
  alpha_ECsI_phi = new TH2I*[NCsI];

  dirCsIMult->cd();
  CsIMult = new TH1I("CsIMult","",10,0,10);
  ECsI_Zgate = new TH1I*[NCsI];
  CaldEE = new TH2I*[NCsI];
  CsIOver = new TH1I*[NCsI];
  dirCsISum->cd();
  ECsISum = new TH2I("ECsISum","",56,-0.5,55.5,1024,0,4095);
  ECsICSum = new TH2I("ECsICSum","",56,-0.5,55.5,1024,0,500);
  Etot = new TH1I("Etot","",10000,0,1000);

  for(int icsi = 0; icsi < NCsI; icsi++) {
    dirCsIRaw->cd();
    ECsI[icsi] = new TH1I(Form("ECsI_%i",icsi),"",4096,0,4095);

    dirCsIGate->cd();
    ECsI_Zgate[icsi] = new TH1I(Form("ECsI_%i_Gate",icsi),"",1024,0,4095);

    dirCsISum->cd();
    CsIOver[icsi] = new TH1I(Form("CsIOver_%i",icsi),"",1024,0,4095);

    dirCsITime->cd();
    TCsI[icsi] = new TH1I(Form("TCsI_%i",icsi),"",1500,-1000,500);

    dirCsIET->cd();
    ET_csi[icsi] = new TH2I(Form("ET_CsI_%i",icsi),"",1024,0,2000,1500,-1000,500);

    dirCsIpTheta->cd();
    p_ECsI_theta[icsi] = new TH2I(Form("p_ECsI_theta_%i",icsi),"",400,0.,0.4,4000,0,4000);
    p_ECsI_thetaCal[icsi] = new TH2I(Form("p_ECsI_thetaCal_%i",icsi),"",400,0.,0.4,4000,0,4000);

    dirCsIpPhi->cd();
    p_ECsI_phi[icsi] = new TH2I(Form("p_ECsI_phi_%i",icsi),"",128,0,6.4,4000,0,4000);

    dirCsIpRing->cd();
    if(icsi<16)	p_ECsI_ring[icsi] = new TH2I(Form("p_ECsI_ring_%i",icsi),"",80,50,130,4000,0,4000);
    else p_ECsI_ring[icsi] = new TH2I(Form("p_ECsI_ring_%i",icsi),"",70,0,70,4000,0,4000);

    dirCsIaTheta->cd();
    alpha_ECsI_theta[icsi] = new TH2I(Form("alpha_ECsI_theta_%i",icsi),"",400,0.0,0.4,4000,0,4000);

    dirCsIaPhi->cd();
    alpha_ECsI_phi[icsi] = new TH2I(Form("alpha_ECsI_phi_%i",icsi),"",128,0,6.4,4000,0,4000);

    outstring.str("");
    outstring << "alpha_ECsI_ring_" << icsi;
    name = outstring.str();
    dirCsIaRing->cd();
    if(icsi<16)	alpha_ECsI_ring[icsi] = new TH2I(Form("alpha_ECsI_ring_%i",icsi),"",80,50,130,4000,0,4000);
    else alpha_ECsI_ring[icsi] = new TH2I(Form("alpha_ECsI_ring_%i",icsi),"",70,0,70,4000,0,4000);
  }

/***********************************************************/
/* CAESAR **************************************************/
/***********************************************************/

  ECeasar = new TH1I*[Nceasar];
  TCeasar = new TH1I*[Nceasar];
  ECCeasar = new TH1I*[Nceasar];
  dirCSum->cd();
  TCtotal = new TH1I("TCtotal","",150,-500,1000);
  TECeasar = new TH1I("TECeasar","",512,0,10);
  TECeasar->GetXaxis()->SetTitle("E_{#gamma} [MeV]");
  TEC_Dop = new TH1I("TEC_Dop","",4095,0,10);
  TEC_Dop->GetXaxis()->SetTitle("E_{#gamma} [MeV]");
  TEC_Dop->SetTitle("Dopler corrected");

  TEC_NDop = new TH1I("TEC_NDop","",4095,0,10);
  TEC_NDop->GetXaxis()->SetTitle("E_{#gamma} [MeV]");
  TEC_NDop->SetTitle("Not Dopler corrected");
  CHitmap = new TH1I("CHitmap","", 200, -8, 192);
  CHitmap1 = new TH1I("CHitmap1","", 200, -8, 192);
  CHitmapNM = new TH1I("CHitmapNM","", 200, -8, 192);
  THitmap = new TH1I("THitmap", "",200, -8, 192);

  CEMult = new TH1I("CEMult","",10,-0.5,9.5);
  CTMult = new TH1I("CTMult","",10,-0.5,9.5);
  CETMult = new TH1I("CETMult","",10,-0.5,9.5);
  CETMult2D = new TH2I("CETMult2D", "", 20,-0.5,19.5,20,-0.5,19.5);
  CESMult2D = new TH2I("CESMult2D", "", 20,-0.5,19.5,20,-0.5,19.5);

  ETOF_Ceasar = new TH2I("ETOF_Ceasar","",250,0,1000,1024,-0.5,9.5);
  TCeasarCal = new TH1I("TCeasarCal","",250,0,1000);
  RingSum = new TH2I*[Nring];

  for(int ir = 0; ir < Nring; ir++) {
    dirRSum->cd();
    outstring.str("");
    outstring << "Sum_Ring_" << ir;
    name = outstring.str();
    RingSum[ir] = new TH2I(name.c_str(),"",24,-0.5,23.5,1024,0,4095);
  }

  for(int ic = 0; ic < Nceasar; ic++) {
    outstring.str("");
    if (ic < 10) outstring << "EC00" << ic;
    else if (ic < 100) outstring << "EC0" << ic;
    else outstring << "EC" << ic;
    name = outstring.str();
    dirEnergy->cd();
    ECeasar[ic] = new TH1I(name.c_str(),"",1024,0,4095);
    ECeasar[ic]->GetXaxis()->SetTitle("Ceasar Energy [channel]");
    outstring.str("");
    if (ic < 10) outstring << "ECcal00" << ic;
    else if (ic < 100) outstring << "ECcal0" << ic;
    else outstring << "ECcal" << ic;
    name = outstring.str();
    dirEcal->cd();
    ECCeasar[ic] = new TH1I(name.c_str(),"",500,0,10);
    ECCeasar[ic]->GetXaxis()->SetTitle("Ceasar Energy [MeV]");

    outstring.str("");
    if (ic < 10) outstring << "TC00" << ic;
    else if (ic < 100) outstring << "TC0" << ic;
    else outstring << "TC" << ic;

    name = outstring.str();
    dirTime->cd();
    TCeasar[ic] = new TH1I(name.c_str(),"",250,0,1000);
    TCeasar[ic]->GetXaxis()->SetTitle("Ceasar Time [ns]");
  }

/***********************************************************/
/* S4 ******************************************************/
/***********************************************************/
  file->cd();
  RvPass = new TH1F("RvPass","",400, 0 ,400);
  dirSum->cd();
  SiRTime = new TH1I("Si Time Rings","",2000,0,16000);
  SiPTime = new TH1I("Si Time Pies","",2000,0,16000);

  EpiesR = new TH1I*[Nstrip];
  EpiesC = new TH1I*[Nstrip];
  EpiesC_AfterAddback = new TH1I*[Nstrip];
  TpiesR = new TH1I*[Nstrip];

  EringsR  = new TH1I*[Nstrip];
  EringsC  = new TH1I*[Nstrip];
  EringsC_AfterAddback  = new TH1I*[Nstrip];
  TringsR  = new TH1I*[Nstrip];

  PTSum = new TH2I("PTSum","",Nstrip,-0.5,Nstrip-0.5,4000,0,16000);
  PTSum->GetXaxis()->SetTitle("Strip Num");
  PTSum->GetYaxis()->SetTitle("Time [channel]");

  PievsRing = new TH2I("PievsRing","",1000,0,100,1000,0,100);
  EdiffvsPie = new TH2I("EdiffvsPie","",1000,0,100,1000,-50,50);
  S4SF = new TH1I("S4SF","",5,0,5);
  S4CsI = new TH1I("S4CsI","",5,0,5);

  PieCalSum = new TH1I("PiesCalibratedSum","",1000,0,20);
  PieCalSum->GetXaxis()->SetTitle("pies energy [MeV]");

  Pienn = new TH1I("PiesNoNeighbourSum","",1000,0,20);
  Pienn->GetXaxis()->SetTitle("pies energy [MeV]");

  Pieabwn1 = new TH1I("Pies1NeighbourAddbackSum","",1000,0,20);
  Pieabwn1->GetXaxis()->SetTitle("pies energy [MeV]");

  Pieabwn2 = new TH1I("Pies2NeighboursAddbackSum","",1000,0,20);
  Pieabwn2->GetXaxis()->SetTitle("pies energy [MeV]");

  RingCalSum = new TH1I("RingsCalibratedSum","",1000,0,20);
  RingCalSum->GetXaxis()->SetTitle("rings energy [MeV]");

  Ringnn = new TH1I("RingsNoNeighbourSum","",1000,0,20);
  Ringnn->GetXaxis()->SetTitle("rings energy [MeV]");

  Ringabwn1 = new TH1I("Rings1NeighbourAddbackSum","",1000,0,20);
  Ringabwn1->GetXaxis()->SetTitle("rings energy [MeV]");

  Ringabwn2 = new TH1I("Rings2NeighboursAddbackSum","",1000,0,20);
  Ringabwn2->GetXaxis()->SetTitle("rings energy [MeV]");

  PieEadd1E0= new TH2I("PieaddE1E0","",1000,0,20,1000,0,20);
  PieEadd1E0->GetXaxis()->SetTitle("original energy");
  PieEadd1E0->GetYaxis()->SetTitle("neighbour energy");

  PieEadd2E0= new TH2I("PieaddE2E0","",1000,0,20,1000,0,20);
  PieEadd2E0->GetXaxis()->SetTitle("original energy");
  PieEadd2E0->GetYaxis()->SetTitle("neighbour energy");

  RingEadd1E0= new TH2I("RingaddE1E0","",1000,0,20,1000,0,20);
  RingEadd1E0->GetXaxis()->SetTitle("original energy");
  RingEadd1E0->GetYaxis()->SetTitle("neighbour energy");

  RingEadd2E0= new TH2I("RingaddE2E0","",1000,0,20,1000,0,20);
  RingEadd2E0->GetXaxis()->SetTitle("original energy");
  RingEadd2E0->GetYaxis()->SetTitle("neighbour energy");

  RTSum = new TH2I("RTSum","",Nstrip,-0.5,Nstrip-0.5,4000,0,16000);
  RTSum->GetXaxis()->SetTitle("Strip Num");
  RTSum->GetYaxis()->SetTitle("Time [channel]");

  PCSum = new TH2I("PCSum","",Nstrip,-0.5,Nstrip-0.5,10000,0,10);
  PCSum->GetXaxis()->SetTitle("Strip Num");
  PCSum->GetYaxis()->SetTitle("Energy [channel]");

  PCSum_AfterAddback = new TH2I("PCSum_AfterAddback","",Nstrip,-0.5,Nstrip-0.5,10000,0,10);
  PCSum_AfterAddback->GetXaxis()->SetTitle("Strip Num");
  PCSum_AfterAddback->GetYaxis()->SetTitle("Energy [channel]");

  PCLSum = new TH2I("PCLSum","",Nstrip,-0.5,Nstrip-0.5,10000,0,10);
  PCLSum->GetXaxis()->SetTitle("Strip Num");
  PCLSum->GetYaxis()->SetTitle("Energy [channel]");

  PSum = new TH2I("PSum","",Nstrip,-0.5,Nstrip-0.5,80000,0,16000);
  PSum->GetXaxis()->SetTitle("Strip Num");
  PSum->GetYaxis()->SetTitle("Energy [channel]");

  RCSum = new TH2I("RCSum","",Nstrip,-0.5,Nstrip-0.5,10000,0,10);
  RCSum->GetXaxis()->SetTitle("Strip Num");
  RCSum->GetYaxis()->SetTitle("Energy [channel]");

  RCSum_AfterAddback = new TH2I("RCSum_AfterAddback","",Nstrip,-0.5,Nstrip-0.5,10000,0,10);
  RCSum_AfterAddback->GetXaxis()->SetTitle("Strip Num");
  RCSum_AfterAddback->GetYaxis()->SetTitle("Energy [channel]");

  RSum = new TH2I("RSum","",Nstrip,-0.5,Nstrip-0.5,80000,0,16000);
  RSum->GetXaxis()->SetTitle("Strip Num");
  RSum->GetYaxis()->SetTitle("Energy [channel]");

  S4Map = new TH2I("S4Map","",2800,-70,70,2800,-70,70);
  AMult = new TH1I("AMult","",20,0,20);
  DMult = new TH1I("DMult","",20,0,20);
  PMult = new TH1I("PMult","",20,0,20);

  dirRings->cd();
  RingsMult = new TH1I("RingsMult","",128,-0.5,127.5);

  dirPies->cd();
  PiesMult = new TH1I("PiesMult","",128,-0.5,127.5);

  for (int istrip = 0; istrip < Nstrip; istrip++) {

      dirPiesRaw->cd();
      EpiesR[istrip] = new TH1I(Form("P_%i",istrip),"",4000,0,16000);
      EpiesR[istrip]->GetXaxis()->SetTitle("Energy High Gain[channel]");

      dirPiesCal->cd();
      EpiesC[istrip] = new TH1I(Form("PC_%i",istrip),"",10000,0,100);
      EpiesC[istrip]->GetXaxis()->SetTitle("Energy [MeV]");

      dirPiesCal->cd();
      EpiesC_AfterAddback[istrip] = new TH1I(Form("PC_%i_AfterAddback",istrip),"",10000,0,100);
      EpiesC_AfterAddback[istrip]->GetXaxis()->SetTitle("Energy [MeV]");

      dirPiesTime->cd();
      TpiesR[istrip] = new TH1I(Form("TF_%i",istrip),"",2000,0,16000);
      TpiesR[istrip]->GetXaxis()->SetTitle("time [channels]");

      dirRingsRaw->cd();
      EringsR[istrip] = new TH1I(Form("R_%i",istrip),"",4000,0,16000);
      EringsR[istrip]->GetXaxis()->SetTitle("Energy High Gain[channel]");

      dirRingsCal->cd();
      EringsC[istrip] = new TH1I(Form("RC_%i",istrip),"",10000,0,100);
      EringsC[istrip]->GetXaxis()->SetTitle("Energy [MeV]");

      dirRingsCal->cd();
      EringsC_AfterAddback[istrip] = new TH1I(Form("RC_%i_AfterAddback",istrip),"",10000,0,100);
      EringsC_AfterAddback[istrip]->GetXaxis()->SetTitle("Energy [MeV]");

      dirRingsTime->cd();
      TringsR[istrip] = new TH1I(Form("TB_%i",istrip),"",2000,0,16000);
      TringsR[istrip]->GetXaxis()->SetTitle("time [channels]");
    }


/***********************************************************/
/* S800 Spectra ********************************************/
/***********************************************************/

  dirS800Raw->cd();
  Te1up = new TH1I("Te1up","",501,-500,500);
  Te1down = new TH1I("Te1down","",501,-500,500);

  ICSummary = new TH2I("ICSummary","",16,-0.5,15.5,1024,0,4095);

  CRDC1Summary = new TH2I("CRDC1Summary","",224,-0.5,223.5,2000,0,2000);
  CRDC2Summary = new TH2I("CRDC2Summary","",224,-0.5,223.5,2000,0,2000);
  CRDC1Summary_cal = new TH2I("CRDC1Summary_cal","",224,-0.5,223.5,2000,0,2000);
  CRDC2Summary_cal = new TH2I("CRDC2Summary_cal","",224,-0.5,223.5,2000,0,2000);
  CRDC1raw = new TH1I("CRCD1raw","",2000,0,2000);
  CRDC2raw = new TH1I("CRCD2raw","",2000,0,2000);
  CRDC1PadMult = new TH1I("CRDC1PadMult","",224,-0.5,223.5);
  CRDC2PadMult = new TH1I("CRDC2PadMult","",224,-0.5,223.5);
  CRDC1Tac = new TH1I("CRDC1Tac","",2000,0,2000);
  CRDC2Tac = new TH1I("CRDC2Tac","",2000,0,2000);
  CRDC1AnodevsTac = new TH2I("CRDC1AnodevsTac","",512,0,4096,512,0,4096);
  CRDC2AnodevsTac = new TH2I("CRDC2AnodevsTac","",512,0,4096,512,0,4096);

  dirS800Cal->cd();
  CRDC1X = new TH1I("CRDC1X","",6000,-300,300);
  CRDC1Y = new TH1I("CRDC1Y","",2000,-100,100);
  CRDC2X = new TH1I("CRDC2X","",6000,-300,300);
  CRDC2Y = new TH1I("CRDC2Y","",2000,-100,100);

  CRDC1XCalY = new TH2I("CRDC1XCalY","",1024,-300,300,512,-100,100);
  CRDC2XCalY = new TH2I("CRDC2XCalY","",1024,-300,300,512,-100,100);

  CRDC1XY = new TH2I("CRDC1XY","",512,-300,300,2000,0,2000);
  CRDC2XY = new TH2I("CRDC2XY","",512,-300,300,2000,0,2000);


  for(unsigned int j = 0; j < beams->pb.size(); j++) {
    char iso[16];
    int tmpZ = beams->GetZ(beams->pb.at(j));
    int tmpA = beams->GetA(beams->pb.at(j));
    sprintf(iso,"%i%s",tmpA,beams->isotope(tmpZ).c_str());
    dirS800Cal_ID[j] = dirS800Cal->mkdir(Form("%s",iso));
    dirS800Cal_ID[j]->cd();
    for(int i = 0; i < 2; i++) {
      CRDCSumList[i][j] = new TH2I(Form("CRDC%i_%s_Sum",i+1,beams->pb.at(j).c_str()),"",224,0,224,2000,0,2000);
      CRDCPadMultList[i][j] = new TH2I(Form("CRDC%i_%s_PadMult",i+1,beams->pb.at(j).c_str()),"",224,0,224,50,0,50);
      for(unsigned int k = 0; k < beams->sb[j].size(); k++) {
	TH2I *hh1 = new TH2I(Form("CRDC%i_%s_%s",i+1,beams->pb.at(j).c_str(),beams->sb[j].at(k).c_str()),"",224,0,224,2000,0,2000); CRDCList[i][j].push_back(hh1);
      }
    }
    dirS800Cal->cd();
  }

  ThetavsPhi = new TH2I("ThetavsPhi","",1000,0,100,720,-180,180);

  dirS800PID->cd();
  ObjvsXFP = new TH2I("ObjvsXFP","",600,-300,0,600,0,300);
  ObjvsXFP->GetXaxis()->SetTitle("T Object");
  ObjvsXFP->GetYaxis()->SetTitle("T XFP");

  ObjvsXFPwithAlpha1 = new TH2I("ObjvsXFPwithAlpha1","",600,-300,0,600,0,300);
  ObjvsXFPwithAlpha1->GetXaxis()->SetTitle("T Object");
  ObjvsXFPwithAlpha1->GetYaxis()->SetTitle("T XFP");

  ObjvsXFPwithProton1 = new TH2I("ObjvsXFPwithProton1","",600,-300,0,600,0,300);
  ObjvsXFPwithProton1->GetXaxis()->SetTitle("T Object");
  ObjvsXFPwithProton1->GetYaxis()->SetTitle("T XFP");

  ObjvsXFPwithProton2 = new TH2I("ObjvsXFPwithProton2","",600,-300,0,600,0,300);
  ObjvsXFPwithProton2->GetXaxis()->SetTitle("T Object");
  ObjvsXFPwithProton2->GetYaxis()->SetTitle("T XFP");

  ObjvsXFPwithProton3 = new TH2I("ObjvsXFPwithProton3","",600,-300,0,600,0,300);
  ObjvsXFPwithProton3->GetXaxis()->SetTitle("T Object");
  ObjvsXFPwithProton3->GetYaxis()->SetTitle("T XFP");

  for(unsigned int j = 0; j < beams->pb.size(); j++) {
    char iso[16];
    int tmpZ = beams->GetZ(beams->pb.at(j));
    int tmpA = beams->GetA(beams->pb.at(j));
    sprintf(iso,"%i%s",tmpA,beams->isotope(tmpZ).c_str());
    dirS800PID_ID[j] = dirS800PID->mkdir(Form("%s",iso));
    dirS800PID_ID[j]->cd();
    TH2I * hh = new TH2I(Form("ObjvsICsum_%s",beams->pb.at(j).c_str()),"",600,-300,0,1024,0,4095); ObjvsICsum_List.push_back(hh);
    TH2I * hh1 = new TH2I(Form("ObjvsICsum_%s_alpha1",beams->pb.at(j).c_str()),"",600,-300,0,1024,0,4095); ObjvsICsum_alphaList.push_back(hh1);
    TH2I * hh2 = new TH2I(Form("ObjvsICsum_%s_deuteron1",beams->pb.at(j).c_str()),"",600,-300,0,1024,0,4095); ObjvsICsum_deuteronList.push_back(hh2);
    TH2I * hh3 = new TH2I(Form("ObjvsICsum_%s_proton1",beams->pb.at(j).c_str()),"",600,-300,0,1024,0,4095); ObjvsICsum_proton1List.push_back(hh3);
    TH2I * hh4 = new TH2I(Form("ObjvsICsum_%s_proton2",beams->pb.at(j).c_str()),"",600,-300,0,1024,0,4095); ObjvsICsum_proton2List.push_back(hh4);
    TH2I * hh5 = new TH2I(Form("ObjvsICsum_%s_proton3",beams->pb.at(j).c_str()),"",600,-300,0,1024,0,4095); ObjvsICsum_proton3List.push_back(hh5);
    TH2I * hh6 = new TH2I(Form("ObjvsICsum_%s_gamma",beams->pb.at(j).c_str()),"",600,-300,0,1024,0,4095); ObjvsICsum_gammaList.push_back(hh6);
    TH2I * hh7 = new TH2I(Form("ObjvsICsum_%s_fiber",beams->pb.at(j).c_str()),"",600,-300,0,1024,0,4095); ObjvsICsum_fiberList.push_back(hh7);
    dirS800PID->cd();
  }

  ObjvsICsum = new TH2I("ObjvsICsum","",600,-300,0,1024,0,4095);
  ObjvsICsum->GetXaxis()->SetTitle("T Object");
  ObjvsICsum->GetYaxis()->SetTitle("IC Sum");

  ObjvsICsum_Ar31_uncor = new TH2I("ObjvsICsum_Ar31_uncor","",600,-300,0,1024,0,4095);

  Objvsafp = new TH2I("Objvsafp","",600,-300,0,1000,-1,1);
  Objvsafp->GetXaxis()->SetTitle("Obj Time");
  Objvsafp->GetYaxis()->SetTitle("afp");

  ObjvsCRDC1X = new TH2I("ObjvsCRDC1X","",600,-300,0,512,-300,300);
  ObjvsCRDC1X->GetXaxis()->SetTitle("Obj Time");
  ObjvsCRDC1X->GetYaxis()->SetTitle("CRDC1 X");

}
/***********************************************************/
void histo_sort::write() {
  file->Write();
  cout << "histo written" << endl;
  file->Close();
}
