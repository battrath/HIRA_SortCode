#include "TCanvas.h"

void JacobiSim()
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


 ostringstream outstring;
  string name;

  TCanvas canvas("jacobis","",600,800);
  
  TFile file("completesort.root");
  TFile file2("/home/Oxygen11/sortcode_addback/tree/O12/2plus2/sim/sim.root");
  TFile file3("/home/Oxygen11/sortcode_addback/tree/O12/2plus2/sim/swave.root");

  TFile *out = new TFile("jacobis_O12sim.root","RECREATE");

  TLatex tt;
  tt.SetNDC();
  tt.SetTextSize(.1); 
 
  double overlap = .047;
  
  TPad *pad1  = new TPad("pad1","",0.,0.,1.,1./3.+overlap);
  TPad *pad2  = new TPad("pad2","",0.,1./3.-overlap/2,1.,2./3.+overlap/2.);
  TPad *pad3  = new TPad("pad3","",0.,2./3.-overlap,1.,1.);
  pad1->SetFillStyle(4000);
  pad2->SetFillStyle(4000);
  pad3->SetFillStyle(4000);
  
  gPad->GetFrame()->SetLineWidth(1);
  pad1->Draw();
  pad2->Draw();
  pad3->Draw();
  
  pad3->cd();

  TH2S frame1("frame1","",25,0.,1.,20,-1.,1.);
  //frame1.GetXaxis()->SetTitle("E* [MeV]");
  frame1.GetYaxis()->SetTitle("Counts");  
  frame1.GetXaxis()->CenterTitle();
  frame1.GetYaxis()->CenterTitle();
  frame1.GetXaxis()->SetLabelSize(0.);
  frame1.SetNdivisions(510);
  frame1.Draw();
  
  TH2I * JacobiY = (TH2I*)file.Get("/corr/O12/stripcuts/transverse/JacobiY_12O_2p10C_star2_stripcuts_trans");
  JacobiY->GetXaxis()->SetTitle("");
  JacobiY->GetXaxis()->SetLabelSize(0.);
  JacobiY->GetYaxis()->SetTitle("cos(#theta_{k})");
  JacobiY->GetYaxis()->CenterTitle();
  for(int i=0; i<13; i++)
    {
      JacobiY->SetBinContent(i,19,0);
      JacobiY->SetBinContent(i,18,0);
      JacobiY->SetBinContent(i,17,0);
    }

  JacobiY->Draw("colz2");



  tt.DrawLatex(0.18,0.82,"(a) Experimental");


  pad2->cd();

  TH2S frame2("frame2","",25,0.,1.,20,-1.,1.);
  //frame2.GetXaxis()->SetTitle("E* [MeV]");
  frame2.SetStats(kFALSE);
  frame2.GetYaxis()->SetTitle("Counts");  
  frame2.GetXaxis()->CenterTitle();
  frame2.GetYaxis()->CenterTitle();
  frame2.GetXaxis()->SetLabelSize(0.);
  frame2.SetNdivisions(510);
  frame2.Draw();


  TH2I * JacobiY_star1 = (TH2I*)file2.Get("JacobiY_xy_s");
  JacobiY_star1->GetXaxis()->SetTitle("");
  JacobiY_star1->GetXaxis()->SetLabelSize(0.);
  JacobiY_star1->GetYaxis()->SetTitle("cos(#theta_{k})");
  JacobiY_star1->GetYaxis()->CenterTitle();
  for(int i=0; i<15; i++)
    {
      JacobiY_star1->SetBinContent(i,19,0);
      JacobiY_star1->SetBinContent(i,18,0);
      JacobiY_star1->SetBinContent(i,17,0);
    }

  JacobiY_star1->Draw("colz2");

  tt.DrawLatex(0.18,0.82,"(b) 3/2^{-} Intermediate");


  pad1->cd();
  TH2S frame3("frame3","",25,0.,1.,20,-1.,1.);
  frame3.GetXaxis()->SetTitle("E_{core-p}/E_{T}");
  frame3.GetYaxis()->SetTitle("Counts");  
  frame3.GetXaxis()->CenterTitle();
  frame3.GetYaxis()->CenterTitle();
  frame3.GetXaxis()->SetLabelSize(0.);
  frame3.SetNdivisions(510);
  frame3.Draw();

  TH2I * JacobiY_star2 = (TH2I*)file3.Get("JacobiY_xy_s");
  JacobiY_star2->GetYaxis()->SetTitle("cos(#theta_{k})");
  JacobiY_star2->GetYaxis()->CenterTitle();
  JacobiY_star2->GetXaxis()->SetTitle("E_{core-p}/E_{T}");
  JacobiY_star2->GetXaxis()->CenterTitle();

  for(int i=0; i<15; i++)
    {
      JacobiY_star2->SetBinContent(i,19,0);
      JacobiY_star2->SetBinContent(i,18,0);
      JacobiY_star2->SetBinContent(i,17,0);
    }


  JacobiY_star2->Draw("colz2");
  tt.DrawLatex(0.18,0.82,"(c) 1/2^{+} Intermediate");


  canvas.Write();
  out->Close();
}
