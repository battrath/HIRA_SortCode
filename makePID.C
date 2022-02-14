#include "TCutG.h"
#include "TROOT.h"
#include "TPad.h"
#include <iostream>
#include <fstream>

using namespace std;
void banana() {
  ofstream outfile;
  outfile.open("PID.dat");
  ofstream beamfile;
  beamfile.open("beam.dat");
  vector<string> data;
  int NPID = 0;
  while(1) {
    TCutG *cut1 = (TCutG*)gPad->WaitPrimitive("CUTG","CutG");
    cut1->SetName("cut1");
    cut1->Print();
    int n = cut1->GetN();
    double *x = cut1->GetX();
    double *y = cut1->GetY();
    int Z, A;
    cout << "Z A?" << endl;
    cin >> Z >> A;
    beamfile << Z << " " << A << endl;
    data.push_back(Form("%i %i",Z,A));
    data.push_back(Form("%i",n));
    for (int i = 0; i < n; i++) {
      data.push_back(Form("%f %f",*x++,*y++));
    }
    delete cut1;
    string con;
    cout << "Continue (y/n)" << endl;
    cin >> con;
    NPID++;
    if(con != "y") break;
  }
  outfile << NPID << endl;
  for(int i = 0; i < data.size();i++) {
    outfile << data.at(i) << endl;
  }
  beamfile.close();
  outfile.close();
  return;
}
