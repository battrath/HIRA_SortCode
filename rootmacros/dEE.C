#include "TCanvas.h"

void dEE()
{

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
  Sty->SetPadTickX(1);
  Sty->SetPadTickY(1);
  Sty->SetPadBottomMargin(.15);
  Sty->SetPadTopMargin(.05);
  Sty->SetPadLeftMargin(.15);
  Sty->SetPadRightMargin(.05);
  Sty->SetHistLineWidth(3);
  Sty->SetFuncWidth(3);
  Sty->SetFuncColor(kGreen);
  Sty->SetLineWidth(3);
  Sty->SetLabelSize(0.06,"xyz");
  Sty->SetLabelOffset(0.02,"y");
  Sty->SetLabelOffset(0.02,"x");
  Sty->SetLabelColor(kBlack,"xyz");
  Sty->SetTitleSize(0.06,"xyz");
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


  TFile *file = new TFile("read.root");
  TFile *out = new TFile("dEE.root","RECREATE");

  ostringstream outstring;
  string name;

  TCanvas *canvas = new TCanvas("canvas","canvas",1700,1000);

  outstring.str("");
  outstring << "/dEE/dEE_13";
  name = outstring.str();

  TH2I * dEE = (TH2I*)file->Get(name.c_str())->Clone();
  dEE->GetYaxis()->SetRangeUser(0,330);
  dEE->GetXaxis()->SetRangeUser(80,3500);

  out->cd();
  dEE->SetStats(kFALSE);
  dEE->GetXaxis()->SetTitle("CsI Light [arb.]");
  dEE->GetXaxis()->CenterTitle();
  
  dEE->GetYaxis()->SetTitle("dE_{Si} [MeV]");
  dEE->GetYaxis()->CenterTitle();

  gPad->SetLogz();
  dEE->Draw("colz");

  canvas->Write();

  file->Close();
  out->Close();


}
