#include "TCanvas.h"

void angPlot()
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
  Sty->SetLabelSize(0.09,"xyz");
  Sty->SetLabelOffset(0.02,"y");
  Sty->SetLabelOffset(0.0,"x");
  Sty->SetLabelColor(kBlack,"xyz");
  Sty->SetTitleSize(0.11,"xyz");
  Sty->SetTitleOffset(0.8,"y");
  Sty->SetTitleOffset(0.85,"x");
  Sty->SetTitleFillColor(10);
  Sty->SetTitleTextColor(kBlack);
  Sty->SetTickLength(.05,"xz");
  Sty->SetTickLength(.025,"y");
  Sty->SetNdivisions(5,"xyz");
  Sty->SetEndErrorSize(0);
  gROOT->SetStyle("MyStyle");
  gROOT->ForceStyle();
  
  TCanvas canvas("angplot","",800,600);

  TFile file("completesort.root");
  TFile out("angplot.root","RECREATE");

  canvas.cd();

  TH1I * angles = (TH1I*)file.Get("/corr/O12/stripcuts/transverse/JacobiYsloton");

  angles->GetYaxis()->SetRangeUser(0,3000);
  //angles->GetYaxis()->SetTitle("Counts");
  angles->GetXaxis()->SetTitle("cos(#theta_{k})");
  angles->GetYaxis()->CenterTitle();
  angles->GetXaxis()->CenterTitle();
  angles->Draw();
  canvas.Update();
  canvas.Write();
  out.Write();
  file.Close();

}
