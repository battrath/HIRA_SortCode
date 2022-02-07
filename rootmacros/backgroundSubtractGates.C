#include "TCanvas.h"


void backgroundSubtractGates()
{
  TFile *file = new TFile("completesort.root");
  TFile *out = new TFile("out_gated.root","RECREATE");
  


  ostringstream outstring;
  string name;

  TCanvas *mycan = new TCanvas("mycan","mycan",1700,1000);
  double norm1, norm2;
  bool worked;

  norm1 = -0.0045;
  norm2 = -0.0045;


  outstring.str("");
  outstring << "corr/O11/stripcuts/JacobiY_11O_2p9C_penergies1";
  name = outstring.str();

  TH2I * penergies1 = (TH2I*)file->Get(name.c_str())->Clone();

  outstring.str("");
  outstring << "corr/O11/stripcuts/JacobiY_11O_2p9C_FakeC10_penergies1";
  name = outstring.str();

  TH1I * FakeC10_penergies1 = (TH1I*)file->Get(name.c_str())->Clone();
  cout << "here" << endl;
  outstring.str("");
  outstring << "corr/O11/stripcuts/JacobiY_11O_2p9C_FakeC11_penergies1";
  name = outstring.str();

  TH2I * FakeC11_penergies1 = (TH2I*)file->Get(name.c_str())->Clone();

  outstring.str("");
  outstring << "corr/O11/stripcuts/JacobiY_11O_2p9C_penergies2";
  name = outstring.str();

  TH2I * penergies2 = (TH2I*)file->Get(name.c_str())->Clone();

  outstring.str("");
  outstring << "corr/O11/stripcuts/JacobiY_11O_2p9C_FakeC10_penergies2";
  name = outstring.str();

  TH1I * FakeC10_penergies2 = (TH1I*)file->Get(name.c_str())->Clone();

  outstring.str("");
  outstring << "corr/O11/stripcuts/JacobiY_11O_2p9C_FakeC11_penergies2";
  name = outstring.str();

  TH2I * FakeC11_penergies2 = (TH2I*)file->Get(name.c_str())->Clone();

  outstring.str("");
  outstring << "corr/O11/stripcuts/JacobiY_11O_2p9C_penergies3";
  name = outstring.str();

  TH2I * penergies3 = (TH2I*)file->Get(name.c_str())->Clone();

  outstring.str("");
  outstring << "corr/O11/stripcuts/JacobiY_11O_2p9C_FakeC10_penergies3";
  name = outstring.str();

  TH1I * FakeC10_penergies3 = (TH1I*)file->Get(name.c_str())->Clone();

  outstring.str("");
  outstring << "corr/O11/stripcuts/JacobiY_11O_2p9C_FakeC11_penergies3";
  name = outstring.str();

  TH2I * FakeC11_penergies3 = (TH2I*)file->Get(name.c_str())->Clone();
 
  worked = penergies1->Add(penergies1, FakeC10_penergies1, 1., norm1);
  worked = penergies1->Add(penergies1, FakeC11_penergies1, 1., norm2);
  worked = penergies2->Add(penergies2, FakeC10_penergies2, 1., norm1);
  worked = penergies2->Add(penergies2, FakeC11_penergies2, 1., norm2);
  worked = penergies3->Add(penergies3, FakeC10_penergies3, 1., norm1);
  worked = penergies3->Add(penergies3, FakeC11_penergies3, 1., norm2);

  penergies1->GetXaxis()->SetRangeUser(0,4.5);
  penergies2->GetXaxis()->SetRangeUser(0,6);
  penergies3->GetXaxis()->SetRangeUser(0.5,8);

  out->cd();

  gROOT->Reset();
  TStyle * Sty = new TStyle("MyStyle","MyStyle");
  Sty->SetOptTitle(0);
  Sty->SetOptStat(0);
  Sty->SetLineWidth(5);
  Sty->SetPalette(8,0);
  Sty->SetCanvasColor(10);
  Sty->SetCanvasBorderMode(0);
  Sty->SetFrameLineWidth(3);
  Sty->SetFrameFillColor(10);
  Sty->SetPadColor(10);
  Sty->SetPadTickX(2);
  Sty->SetPadTickY(2);
  Sty->SetPadBottomMargin(.15);
  Sty->SetPadTopMargin(.05);
  Sty->SetPadLeftMargin(.15);
  Sty->SetPadRightMargin(.05);
  Sty->SetHistLineWidth(3);
  Sty->SetFuncWidth(3);
  Sty->SetFuncColor(kGreen);
  Sty->SetLineWidth(3);
  Sty->SetLabelSize(0.03,"xyz");
  Sty->SetLabelOffset(0.02,"y");
  Sty->SetLabelOffset(0.02,"x");
  Sty->SetLabelColor(kBlack,"xyz");
  Sty->SetTitleSize(0.04,"xyz");
  Sty->SetTitleOffset(1.2,"y");
  Sty->SetTitleOffset(1.3,"x");
  Sty->SetTitleFillColor(10);
  Sty->SetTitleTextColor(kBlack);
  Sty->SetTickLength(.05,"xz");
  Sty->SetTickLength(.025,"y");
  Sty->SetNdivisions(10,"xyz");
  Sty->SetEndErrorSize(0);
  gROOT->SetStyle("MyStyle");
  gROOT->ForceStyle();

  TLatex text;
  text.SetTextSize(0.037);
  text.SetTextAlign(10);

  penergies1->SetStats(kFALSE);
  penergies1->GetXaxis()->SetTitle("E_{core-p}");
  penergies1->GetYaxis()->SetTitle("cos(#Theta_{k})");
  penergies1->GetXaxis()->CenterTitle();
  penergies1->GetYaxis()->CenterTitle();
  penergies1->Write();

  penergies2->SetStats(kFALSE);
  penergies2->GetXaxis()->SetTitle("E_{core-p}");
  penergies2->GetYaxis()->SetTitle("cos(#Theta_{k})");
  penergies2->GetXaxis()->CenterTitle();
  penergies2->GetYaxis()->CenterTitle();
  penergies2->Write();

  penergies3->SetStats(kFALSE);
  penergies3->GetXaxis()->SetTitle("E_{core-p}");
  penergies3->GetYaxis()->SetTitle("cos(#Theta_{k})");
  penergies3->GetXaxis()->CenterTitle();
  penergies3->GetYaxis()->CenterTitle();
  penergies3->Write();


  
  file->Close();
  out->Close();
}
