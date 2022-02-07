#include "TCanvas.h"

void O11BackgroundFig()
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
  TFile *out = new TFile("O11backgroundfig.root","RECREATE");

  ostringstream outstring;
  string name;
  double norm1, norm2;

  norm1 = 0.0045;
  norm2 = 0.0045;

  TCanvas *canvas = new TCanvas("canvas","canvas",1700,1000);

  outstring.str("");
  outstring << "corr/O11/stripcuts/Erel11O_2p9C_stripcuts";
  name = outstring.str();

  TH1I * O11 = (TH1I*)file->Get(name.c_str())->Clone();
  
  outstring.str("");
  outstring << "corr/O11/stripcuts/Erel11O_2p9C_Fake_CsIreaction_stripcuts";
  name = outstring.str();

  TH1I * O11_12Background = (TH1I*)file->Get(name.c_str())->Clone();

  outstring.str("");
  outstring << "corr/O11/stripcuts/Erel11O_2p9C_Fake_CsIreaction_C11_stripcuts";
  name = outstring.str();

  TH1I * O11_13Background = (TH1I*)file->Get(name.c_str())->Clone();

  O11->GetYaxis()->SetRangeUser(0,900);
  O11->GetXaxis()->SetRangeUser(0,20);

  out->cd();
  O11->SetStats(kFALSE);
  O11->GetXaxis()->SetTitle("E_{T} [MeV]");
  O11->GetYaxis()->SetTitle("Counts");
  O11->GetXaxis()->CenterTitle();
  O11->GetYaxis()->CenterTitle();
  O11->SetFillStyle(3001);
  O11->SetLineColor(kBlack);
  O11->Draw();
  O11_12Background->SetLineColor(kRed);
  O11_12Background->Scale(norm1);
  O11_12Background->Draw("same");
  O11_13Background->SetLineColor(kBlue);
  O11_13Background->Scale(norm2);
  O11_13Background->Draw("same");
  canvas->Update();

  
  
  TLatex text;
  text.SetTextSize(.06);
  text.SetTextAlign(10);

  text.DrawLatex(8,550,"Raw ^{11}O Spectrum");
  text.SetTextColor(2);
  text.DrawLatex(8,450,"^{12}O #rightarrow ^{10}C+2p Background");
  text.SetTextColor(4);
  text.DrawLatex(8,350,"^{13}O #rightarrow ^{11}C+2p Background");



  canvas->Write();

  file->Close();
  out->Close();
}
