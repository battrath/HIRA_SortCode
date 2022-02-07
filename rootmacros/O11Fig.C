#include "TCanvas.h"

void O11Fig()
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


  TFile *file = new TFile("out.root");
  TFile *out = new TFile("O11fig.root","RECREATE");

  ostringstream outstring;
  string name;

  TCanvas *canvas = new TCanvas("canvas","canvas",1700,1000);

  outstring.str("");
  outstring << "Erel11O_2p9C_stripcuts";
  name = outstring.str();

  TH1I * O11 = (TH1I*)file->Get(name.c_str())->Clone();
  O11->GetYaxis()->SetRangeUser(0,900);
  O11->GetXaxis()->SetRangeUser(0,20);

  out->cd();
  O11->SetStats(kFALSE);
  O11->GetXaxis()->SetTitle("E_{T} [MeV]");
  O11->GetXaxis()->CenterTitle();
  O11->GetYaxis()->CenterTitle();
  O11->SetFillStyle(3001);
  O11->SetLineColor(kBlack);
  O11->Draw();
  canvas->Update();
  
  TLine line;
  line.SetLineWidth(5);

  TLatex text;
  text.SetTextSize(.06);
  text.SetTextAlign(10);

  float sigma = 0.84;
  float mean = 3.21;

  text.DrawLatex(2.3,765,"IMME");
  line.DrawLine(mean,730,mean,760);
  line.DrawLine(mean-sigma,745,mean+sigma,745);

  sigma = 0.17;
  mean = 4.46;

  line.SetLineColor(2);
  text.SetTextColor(2);
  text.DrawLatex(4.8,655,"Fortune and Sherr");
  line.DrawLine(mean,665,mean,695);
  line.DrawLine(mean-sigma,680,mean+sigma,680);

  sigma = 0.;
  mean = 4.3897;


  text.SetTextColor(4);

  TArrow * arrow = new TArrow(mean,100,mean,230,0.007,"|>");
  arrow->SetLineWidth(5);
  arrow->SetFillColor(4);
  arrow->SetLineColor(4);


  text.DrawLatex(2.3,40,"Hagino and Sagawa");
  arrow->Draw();



  canvas->Write();

  file->Close();
  out->Close();
}
