#include "TCanvas.h"


void superImpose()
{
  TFile *file = new TFile("out.root");
  TFile *sim = new TFile("sim.root");
  TFile *out = new TFile("out2.root","RECREATE");
  
  ostringstream outstring;
  string name;

  TCanvas *mycan = new TCanvas("mycan","mycan",1600,900);
  double norm;
  bool worked;

  norm = -4.35/3.;

  outstring.str("");
  outstring << "Erel11O_2p9C";
  name = outstring.str();

  TH1I * O11 = (TH1I*)file->Get(name.c_str())->Clone();

  outstring.str("");
  outstring << "Erel1";
  name = outstring.str();

  TH1I * O11_Sim = (TH1I*)sim->Get(name.c_str())->Clone();




  cout << worked << endl;
  out->cd();


  O11->SetStats(kFALSE);
  O11->GetYaxis()->SetRangeUser(0,450);
  O11->Draw();
  O11->GetXaxis()->SetTitle("Relative Energy [MeV]");
  O11->GetYaxis()->SetTitle("Counts");

  mycan->Update();

  O11_Sim->SetLineColor(kRed);
  O11_Sim->Scale((-1.)*norm);
  O11_Sim->SetStats(kFALSE);
  O11_Sim->Draw("same");
  mycan->Update();
  mycan->Write();

 

  mycan->Update();
  
  file->Close();
  out->Close();
}
