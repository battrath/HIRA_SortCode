#include "TCanvas.h"

void JacobiFig_O12()
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
  Sty->SetTitleOffset(1.2,"t");
  Sty->SetTitleFillColor(10);
  Sty->SetTitleTextColor(kBlack);
  Sty->SetTickLength(.05,"xz");
  Sty->SetTickLength(.025,"y");
  Sty->SetNdivisions(10,"xyz");
  Sty->SetEndErrorSize(0);
  Sty->SetTitleAlign();
  gROOT->SetStyle("MyStyle");
  gROOT->ForceStyle();
  TCanvas* canvas = new TCanvas("canvas","canvas",1200,1600);
  canvas->Divide(2,3,0.01,0.01);

  TFile *file = new TFile("out2.root");
  TFile *out = new TFile("jacobis.root","RECREATE");

  ostringstream outstring;
  string name;

  canvas->cd(1);

  outstring.str("");
  outstring << "JacobiY_12O_2p10C_stripcuts";
  name = outstring.str();
  TH2I * JacobiY = (TH2I*)file->Get(name.c_str())->Clone();
  JacobiY->SetTitle("Jacobi Y, g.s.");
  JacobiY->Draw("colz");

  canvas->cd(2);

  outstring.str("");
  outstring << "JacobiT_12O_2p10C_stripcuts";
  name = outstring.str();
  TH2I * JacobiT = (TH2I*)file->Get(name.c_str())->Clone();
  JacobiT->SetTitle("Jacobi T, g.s");
   JacobiT->Draw("colz");

  canvas->cd(3);

  outstring.str("");
  outstring << "JacobiY_12O_2p10C_star1_stripcuts";
  name = outstring.str();
  TH2I * JacobiY_star1 = (TH2I*)file->Get(name.c_str())->Clone();
  JacobiY_star1->SetTitle("Jacobi Y, 0^{+}, 2^{+}_{1}");
  JacobiY_star1->Draw("colz");

  canvas->cd(4);

  outstring.str("");
  outstring << "JacobiT_12O_2p10C_star1_stripcuts";
  name = outstring.str();

  TH2I * JacobiT_star1 = (TH2I*)file->Get(name.c_str())->Clone();
  JacobiT_star1->SetTitle("Jacobi T, 0^{+}, 2^{+}_{1}");
  JacobiT_star1->Draw("colz");

  canvas->cd(5);

  outstring.str("");
  outstring << "JacobiY_12O_2p10C_star2_stripcuts";
  name = outstring.str();

  TH2I * JacobiY_star2 = (TH2I*)file->Get(name.c_str())->Clone();
  JacobiY_star2->SetTitle("Jacobi Y, 2^{+}_{2}");
  JacobiY_star2->Draw("colz");

  canvas->cd(6);

  outstring.str("");
  outstring << "JacobiT_12O_2p10C_star2_stripcuts";
  name = outstring.str();

  TH2I * JacobiT_star2 = (TH2I*)file->Get(name.c_str())->Clone();
  JacobiT_star2->SetTitle("Jacobi T, 2^{+}_{2}");
  JacobiT_star2->Draw("colz");

  canvas->Update();
  canvas->Write();

  file->Close();
  out->Close();
}
