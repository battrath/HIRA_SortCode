#include "TCanvas.h"

void project()
{
  TFile *exp = new TFile("home/Oxygen11/sortcode_addback/read.root");
  TFile *sim = new TFile("home/Oxygen11/sortcode_addback/tree/O12/2plus2/sim/sim.root");
 
  TFile *out = new TFile("project.root", "RECREATE");

  TCanvas *mycan1 = new TCanvas("star1","star1", 1000,2000);  


  mycan1->Divide(1,2);
  mycan2->Divide(1,2);

  ostringstream outstring;
  string name;

  outstring.str("");
  outstring << "corr/O12/stripcuts/transverse/JacobiY_12O_2p10C_star1_stripcuts_trans";
  name = outstring.str();
  TH2I *jacobi_exp_1 = (TH2I*)exp->Get(name.c_str())->Clone();

  outstring.str("");
  outstring << "corr/O12/stripcuts/transverse/JacobiY_12O_2p10C_star2_stripcuts_trans";
  name = outstring.str();
  TH2I *jacobi_exp_2 = (TH2I*)exp->Get(name.c_str())->Clone();

  outstring.str("");
  outstring << "JacobiY_xy_s";
  name = outstring.str();
  TH2I *jacobi_sim_1 = (TH2I*)sim1->Get(name.c_str())->Clone();

  outstring.str("");
  outstring << "JacobiY_xy_s";
  name = outstring.str();
  TH2I *jacobi_sim_2 = (TH2I*)sim2->Get(name.c_str())->Clone();

  cout << "got files baby!" << endl; 

  out->cd();

  mycan1->cd();
  mycan1->cd(1);

  jacobi_exp_1->SetTitle("Experimental");
  jacobi_exp_1->Draw("colz");

  mycan1->cd(2);

  jacobi_sim_1->SetTitle("Simulation");
  jacobi_sim_1->Draw("colz");

  mycan2->cd();
  mycan2->cd(1);

  jacobi_exp_2->SetTitle("Experimental");
  jacobi_exp_2->Draw("colz");

  mycan2->cd(2);

  jacobi_sim_2->SetTitle("Simulation");
  jacobi_sim_2->Draw("colz");

  mycan1->Write();
  mycan2->Write();

  exp->Close();
  sim1->Close();
  sim2->Close();
  out->Write();

}
