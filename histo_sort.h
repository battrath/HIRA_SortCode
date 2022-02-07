#ifndef histo_sort_
#define histo_sort_
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include "TH1I.h"
#include "TH2I.h"
#include "TH3I.h"
#include "TFile.h"
#include "TDirectory.h"
#include "readBeam.h"
using namespace std;

class histo_sort {
 protected:

  TFile * file; //!< output root file
  //correlations
  //de
  TDirectoryFile * dirdee; //!< directory for dee maps

  //fiberArray
  TDirectoryFile * dirFiber; //!< directory for fiber array

  //ringCounter
  TDirectoryFile * dirRing;  //!< ring counter stuff
  TDirectory * dirRSum;

  //Summary
  TDirectoryFile * dirSum; //!< directory for summary spectra

  //Pies
  TDirectoryFile * dirPies; //!< directory for front strip info
  TDirectory * dirPiesRaw; //!< directory for raw front strip info
  TDirectory * dirPiesCal; //!< directory for calibrated front strip info
  TDirectory * dirPiesLGR; //!< directory for raw front string low gain
  TDirectory * dirPiesTime; //!< directory for times
  TDirectory * dirTotCal;

  //Rings
  TDirectoryFile * dirRings; //!< directory for rings strip info
  TDirectory * dirRingsRaw; //!< directory for raw rings strip info
  TDirectory * dirRingsCal; //!< directory for calibrated rings strip info
  TDirectory * dirRingsLGR; //!< directory for raw rings string low gain
  TDirectory * dirRingsTime; //!< directory for times

  //CsI
  TDirectoryFile *dirCsI; //!< directory for the CsI info
  TDirectory * dirCsIRaw; //!< CsI energies
  TDirectory * dirCsITime; //!< CsI time
  TDirectory * dirCsISum; //!< CsI Sum
  TDirectory * dirCsIGate; //!< CsI gated on Z
  TDirectory * dirCsIMult; //!< CsI Multiplicity
  TDirectory * dirCsIET; //!< CsI time
   TDirectory * dirCsIpTheta; //CsI theta dependence for proton
  TDirectory * dirCsIpPhi; //CsI phi dependece for proton
  TDirectory * dirCsIpRing; //CsI ring dependence for proton
  TDirectory * dirCsIaTheta; //CsI theta dependence for alpha
  TDirectory * dirCsIaPhi; //CsI phi dependece for alpha
  TDirectory * dirCsIaRing; //CsI ring dependence for alpha

  //CEARAR
  TDirectoryFile *dirC; //!< directory for the CEASAR info
  TDirectory * dirEnergy; //!< CsI energies
  TDirectory * dirTime; //!< CsI time
  TDirectory * dirEcal; //!< CsI Sum
  TDirectory * dirRsum; //!< CsI gated on Z
  TDirectory * dirCSum;

  //S800 directory
  TDirectoryFile *dirS800;
  TDirectory * dirS800Raw;
  TDirectory * dirS800Cal;
  TDirectory * dirS800Cal_ID[10];
  TDirectory * dirS800PID;
  TDirectory * dirS800PID_ID[10];
  TDirectory * dirS800Trig;

  //ThetaPhiMap
  TDirectoryFile *dirThetaPhi;
 public:
  histo_sort(readBeam *beams, string name);                  //!< constructor
  ~histo_sort(){};
  void write(); //!< write the root spectra to file

  int Nstrip;
  int Nceasar;
  int NCsI;
  int Nring;

  //ring counter
  TH2I* protonHitMap;
  TH2I* csi0;
  TH2I* csi4;
  TH2I* csi8;
  TH2I* csi12;
  TH2I* csi16;
  TH2I* csi17;
  TH2I* csi18;
  TH2I* csi19;
  TH1I* csiInner;
  TH1I* csiOuter;
  TH2I* csiPie;
  TH2I** RingSum;

  //fiber
  TH1I* VertA;
  TH1I* VertB;
  TH1I* VertC;
  TH1I* VertD;
  TH1I* VertDyn;
  TH2I* VertHitMap;

  TH1I* HorzA;
  TH1I* HorzB;
  TH1I* HorzC;
  TH1I* HorzD;
  TH1I* HorzDyn;
  TH2I* HorzHitMap;
  TH2I* HorzHitMap_energygate;

  TH2I* DynodevsSum;
  TH2I* AvsSum;
  TH2I* BvsSum;
  TH2I* CvsSum;
  TH2I* DvsSum;

  TH1I * Fiber_X;
  TH1I * Fiber_Y;
  TH2I * Fiber_XY;

  TH1I * CornerMult;
  TH1I * CornerMult_Vert;
  TH1I * CornerMult_Hori;
  TH2I * CornerMult_VvsH;

  TH2I * FiberVyta;

  TH1I * Txfp;
  TH1I * TRF;

  TH2I * PCSum;
  TH2I * PCSum_AfterAddback;
  TH2I * PCLSum;
  TH2I * RCSum;
  TH2I * RCSum_AfterAddback;
  TH2I * PSum;
  TH2I * RSum;

  TH2I * PievsRing;
  TH2I * EdiffvsPie;
  TH1I * PiesMult;
  TH1I * RingsMult;
  TH1I * S4SF;
  TH1I * S4CsI;
  TH2I * S4Map;
  TH1I * AMult;
  TH1I * DMult;
  TH1I * PMult;

  //neighbour, added by jinyu Mar222019
  TH1I * PieCalSum;
  TH1I * Pienn;
  TH1I * Pieabwn1;
  TH1I * Pieabwn2;
  TH1I * RingCalSum;
  TH1I * Ringnn;
  TH1I * Ringabwn1;
  TH1I * Ringabwn2;

  TH2I * PieEadd1E0;
  TH2I * PieEadd2E0;
  TH2I * RingEadd1E0;
  TH2I * RingEadd2E0;

  TH2I * PTSum;
  TH2I * RTSum;
  TH2I * PHiLo;
  TH2I * RHiLo;

  TH1I * SiPTime;
  TH1I * SiRTime;

  //Pies spectra
  TH1I ** EpiesR; //high gain raw
  TH1I ** EpiesC; //high gain calib
  TH1I ** EpiesC_AfterAddback; //high gain calib
  TH1I ** TpiesR; // time raw

  //rings spectra
  TH1I ** EringsR;
  TH1I ** EringsC;
  TH1I ** EringsC_AfterAddback;
  TH1I ** TringsR;

  TH1I ** ECsI;
  TH1I ** TCsI;
  TH2I * ECsISum;
  TH2I * ECsICSum;
  TH1I ** CsIOver;
  TH1I ** ECsI_Zgate;
  TH1I * CsIMult;
  TH1I * Etot;
  TH2I ** ET_csi;

  TH2I ** p_ECsI_theta;
  TH2I ** p_ECsI_thetaCal;

  TH2I ** p_ECsI_phi;
  TH2I ** p_ECsI_ring;
  TH2I ** alpha_ECsI_theta;
  TH2I ** alpha_ECsI_phi;
  TH2I ** alpha_ECsI_ring;


  TH1I* S800_Csi_time;
  TH1I* S800_Csi_time_with_proton;
  TH1I* singles_trig_time;

  TH2I ** CaldEE;



  TH1I * T_RFSCIN;
  TH1I * T_RFCYC;
  TH1I * T_A1900;

  TH2I **  dee;
  TH2I** dee_S800;

  //CEASAR
  TH1I** ECeasar;
  TH1I** TCeasar;
  TH1I** ECCeasar;
  TH1I* TCtotal;
  TH1I* TECeasar;
  TH1I* TEC_Dop;
  TH1I* TEC_NDop;
  TH1I* CEMult;
  TH1I* CTMult;
  TH1I* TCeasarCal;

  TH1I* CETMult;
  TH2I* CETMult2D;
  TH2I* CESMult2D;
  TH1I * CHitmap;
  TH1I * CHitmap1;
  TH1I * CHitmapNM;
  TH1I * THitmap;

  TH2I* ETOF_Ceasar;


  //S800

  //Raw
  TH1I * Te1up;
  TH1I * Te1down;
  TH2I * ICSummary;
  TH2I * CRDC1Summary;
  TH2I * CRDC2Summary;
  TH2I * CRDC1Summary_cal;
  TH2I * CRDC2Summary_cal;
  TH1I * CRDC1raw;
  TH1I * CRDC2raw;
  TH1I * CRDC1PadMult;
  TH1I * CRDC2PadMult;
  TH1I * CRDC1Tac;
  TH1I * CRDC2Tac;
  TH2I * CRDC1AnodevsTac;
  TH2I * CRDC2AnodevsTac;

  //Cal
  TH1I * CRDC1X;
  TH1I * CRDC1Y;
  TH1I * CRDC2X;
  TH1I * CRDC2Y;
  TH2I * CRDC1XY;
  TH2I * CRDC2XY;

  TH2I * CRDC1XCalY;
  TH2I * CRDC2XCalY;

  TH2I * CRDCSumList[2][10];
  TH2I * CRDCPadMultList[2][10];
  vector<TH2I *> CRDCList[2][10];

  TH2I * ThetavsPhi;

  //PID
  TH2I * ObjvsXFP;
  TH2I * ObjvsXFPwithAlpha1;
  TH2I * ObjvsXFPwithProton1;
  TH2I * ObjvsXFPwithProton2;
  TH2I * ObjvsXFPwithProton3;
  TH2I * ObjvsICsum;
  TH2I * ObjvsICsum_Ar31_uncor;

  vector<TH2I *> ObjvsICsum_List;
  vector<TH2I *> ObjvsICsum_alphaList;
  vector<TH2I *> ObjvsICsum_deuteronList;
  vector<TH2I *> ObjvsICsum_proton1List;
  vector<TH2I *> ObjvsICsum_proton2List;
  vector<TH2I *> ObjvsICsum_proton3List;
  vector<TH2I *> ObjvsICsum_gammaList;
  vector<TH2I *> ObjvsICsum_fiberList;

  TH2I * Objvsafp;
  TH2I * ObjvsCRDC1X;
  TH1F * RvPass;
};
#endif
