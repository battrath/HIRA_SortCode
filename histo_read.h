#ifndef histo_read_
#define histo_read_
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include "TH1F.h"
#include "TH1I.h"
#include "TH2I.h"
#include "TH3I.h"
#include "TFile.h"
#include "TDirectory.h"
#include "readBeam.h"

using namespace std;

class histo_read {
protected:

  TFile * file_read; //!< output root file

  TDirectory *dirCorrPB[10];
  TDirectory *dirCorrI[10][7][20];
  TDirectory *dirCorrP[10][7];

  //gammas
  TDirectory * dirGammas[10];

public:
    histo_read(readBeam * beams, string name);                  //!< constructor
    ~histo_read(){};
    void write(); //!< write the root spectra to file

    //Corr spectra
    vector<TH1I *> ErelList[10][7];
    vector<TH1I *> ExList[10][7];
    vector<TH1I *> TCMList[10][7];
    vector<TH1I *> VCMList[10][7];
    vector<TH1I *> EgamList[10][7];
    vector<TH2I *> EgamErelList[10][7];
    vector<TH2I *> EthetaList[10][7];
    vector<TH2I *> ErelCosList[10][7];
    vector<TH1I *> MomList[10][7];
    //2P Specific
    vector<TH2I *> Jacobi_Y_List[10];
    vector<TH2I *> Jacobi_T_List[10];
    vector<TH2I *> ErelVsProList[10];
    vector<TH1I *> ppRelList[10];
    vector<TH1I *> ppRelList2[10];

    TH1I *ErelLi6;
    TH1I *ThetaCMLi6;
    TH1I *VCMLi6;
    TH1I *ErelBe8;
    TH1I *ThetaCMBe8;
    TH1I *VCMBe8;
    TH1I *ErelB9;
    TH1I *ErelC12;
    TH1I *ErelC12Be8;
    //TEC spectra
    vector<TH1I *> TEClist[10];
};

#endif
