#include "TCanvas.h"

void JacobiSplit()
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
  
  TFile file("out_gated.root");

  TFile *out = new TFile("jacobi_split.root","RECREATE");

  TLatex tt;
  tt.SetNDC();
  tt.SetTextSize(.1); 
 
  TBox box;

  box.SetFillColor(10);

  double overlap = .03;
  
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
  
  pad1->cd();

  TH2S frame1("frame1","",25,0.,1.,20,-1.,1.);
  frame1.GetXaxis()->SetTitle("E_{core-p} [MeV]");
  //frame1.GetYaxis()->SetTitle("Counts");  
  frame1.GetXaxis()->CenterTitle();
  frame1.GetYaxis()->CenterTitle();
 
  frame1.SetNdivisions(510);
  frame1.Draw();
  
  TH2I * JacobiY = (TH2I*)file.Get("JacobiY_11O_2p9C_penergies1");
  JacobiY->GetXaxis()->SetTitle("");
 
  JacobiY->GetYaxis()->SetTitle("cos(#theta_{k})");
  JacobiY->GetXaxis()->SetTitle("");
  //JacobiY->GetXaxis()->SetTitle("E_{core-p} [MeV]");
  //JacobiY->GetXaxis()->SetLabelSize(0.);
  JacobiY->GetYaxis()->SetTitle("cos(#theta_{k})");
  JacobiY->GetYaxis()->CenterTitle();

  JacobiY->Draw("colz2");

  //tt.SetTextColor(2);

  box.DrawBox(2.75,0.5,4.35,0.85);
  tt.DrawLatex(0.65,0.82,"(a)2.6<E_{T}#leq4.9");


  pad2->cd();

  TH2S frame2("frame2","",25,0.,1.,20,-1.,1.);
  //frame2.GetXaxis()->SetTitle("E* [MeV]");
  frame2.SetStats(kFALSE);
  //frame2.GetYaxis()->SetTitle("Counts");  
  frame2.GetXaxis()->CenterTitle();
  frame2.GetYaxis()->CenterTitle();

  frame2.SetNdivisions(510);
  frame2.Draw();


  TH2I * JacobiY_star1 = (TH2I*)file.Get("JacobiY_11O_2p9C_penergies2");
  JacobiY_star1->GetXaxis()->SetTitle("");

  JacobiY_star1->GetYaxis()->SetTitle("cos(#theta_{k})");
  JacobiY_star1->Draw("colz2");

  box.DrawBox(3.65,0.5,5.8,0.85);
  tt.DrawLatex(0.65,0.82,"(b)4.9<E_{T}#leq7.2");


  pad3->cd();
  TH2S frame3("frame3","",25,0.,1.,20,-1.,1.);
  //frame3.GetXaxis()->SetTitle("E* [MeV]");
  //frame3.GetYaxis()->SetTitle("Counts");  
  //frame3.GetXaxis()->CenterTitle();
  frame3.GetYaxis()->CenterTitle();

  frame3.SetNdivisions(510);
  frame3.Draw();

  TH2I * JacobiY_star2 = (TH2I*)file.Get("JacobiY_11O_2p9C_penergies3");
  JacobiY_star2->GetYaxis()->SetTitle("cos(#theta_{k})");
 

  JacobiY_star2->Draw("colz1");

  //tt.SetTextColor(1);
  box.DrawBox(5.1,0.5,7.8,0.85);
  tt.DrawLatex(0.65,0.82,"(c) 7.2<E_{T}#leq9.5");


  canvas.Write();
  out->Close();
}
