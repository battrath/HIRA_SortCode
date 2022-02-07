#include "TCanvas.h"

void JacobiFig_O11()
{

TStyle * Sty = new TStyle("MyStyle","MyStyle");
Sty->SetOptTitle(0);
Sty->SetOptStat(0);
//Sty->SetPalette(8,0);
Sty->SetCanvasColor(10);
Sty->SetCanvasBorderMode(0);
Sty->SetFrameLineWidth(3);
Sty->SetFrameFillColor(10);
Sty->SetPadColor(10);
Sty->SetPadTickX(1);
Sty->SetPadTickY(1);
Sty->SetPadBottomMargin(.2);
Sty->SetPadLeftMargin(.17);
Sty->SetPadTopMargin(.05);
Sty->SetPadRightMargin(.05);
Sty->SetHistLineWidth(3);
Sty->SetHistLineColor(kRed);
Sty->SetFuncWidth(3);
Sty->SetFuncColor(kGreen);
Sty->SetLineWidth(3);
Sty->SetLabelSize(0.05,"xyz");
Sty->SetLabelOffset(0.02,"y");
Sty->SetLabelOffset(0.02,"x");
Sty->SetLabelColor(kBlack,"xyz");
Sty->SetTitleSize(0.08,"xyz");
Sty->SetTitleOffset(1.,"y");
Sty->SetTitleOffset(1.,"x");
Sty->SetTitleFillColor(10);
Sty->SetTitleTextColor(kBlack);
Sty->SetTickLength(.05,"xz");
Sty->SetTickLength(.025,"y");
Sty->SetNdivisions(5,"xyz");
Sty->SetEndErrorSize(0);
gROOT->SetStyle("MyStyle");
gROOT->ForceStyle();


  TCanvas* canvas = new TCanvas("canvas","canvas",900,900);

  TFile *file = new TFile("subtract.root");
  TFile *out = new TFile("jacobis_o11.root","RECREATE");

  ostringstream outstring;
  string name;



  outstring.str("");
  outstring << "JacobiY_11O_2p9C_stripcuts";
  name = outstring.str();
  TH2I * JacobiY = (TH2I*)file->Get(name.c_str())->Clone();
  JacobiY->GetYaxis()->SetTitle("cos(#theta_{k})");
  JacobiY->Draw("colz");




  canvas->Update();
  canvas->Write();

  file->Close();
  out->Close();
}
