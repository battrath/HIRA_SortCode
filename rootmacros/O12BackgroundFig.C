#include "TCanvas.h"

void O12BackgroundFig()
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


  TFile *file = new TFile("tree/O12/out2.root");
  TFile *out = new TFile("O12backgroundfig.root","RECREATE");

  ostringstream outstring;
  string name;
 
  TCanvas *canvas = new TCanvas("canvas","canvas",1700,1000);

  outstring.str("");
  outstring << "Ex_trans";
  name = outstring.str();

  TH1I * O12 = (TH1I*)file->Get(name.c_str())->Clone();
  

  O12->GetYaxis()->SetRangeUser(0,1700);
  O12->GetXaxis()->SetRangeUser(0,10);

  out->cd();
  O12->SetStats(kFALSE);
  O12->GetXaxis()->SetTitle("E_{T} [MeV]");
  O12->GetYaxis()->SetTitle("Counts");
  O12->GetXaxis()->CenterTitle();
  O12->GetYaxis()->CenterTitle();
  O12->SetFillStyle(3001);
  O12->SetLineColor(kBlack);
  O12->Draw();
 
  canvas->Update();

  
  
  TLatex text;
  text.SetTextSize(.06);
  text.SetTextAlign(10);

  text.DrawLatex(1.5,1100,"0^{+}_{1}");
  text.DrawLatex(2.7,400,"0^{+}_{2}");
  text.DrawLatex(3.5,1400,"2^{+}_{1}");
  text.DrawLatex(6.0,700,"2^{+}_{2}");

  canvas->Write();

  file->Close();
  out->Close();
}
