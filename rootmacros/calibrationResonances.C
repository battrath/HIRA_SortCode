void calibrationResonances()
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

  TFile *calibrations = new TFile("calibrations.root","RECREATE");

  TFile *N12 = new TFile("/home/Oxygen11/sortcode_addback/tree/N12/fit/fit.root");
  TFile *O13 = new TFile("/home/Oxygen11/sortcode_addback/tree/O13/fit/fit.root");
  TFile *O12 = new TFile("/home/Oxygen11/sortcode_addback/tree/O12/doublet/fit/fit.root");

  calibrations->cd();

  TCanvas *composite = new TCanvas("composite","composite",800,800);

  TLatex *t = new TLatex();
  t->SetTextSize(0.15);

  double overlap = 0.0;

  composite->cd();

  TPad *pad3  = new TPad("pad1","",0.,0.,1.,1./3.+overlap);
  TPad *pad2  = new TPad("pad2","",0.,1./3.-overlap/2.,1.,2./3.+overlap/2.);
  TPad *pad1  = new TPad("pad3","",0.,2./3.-overlap,1.,1.);

  pad1->SetFillStyle(4000);
  pad2->SetFillStyle(4000);
  pad3->SetFillStyle(4000);
  
  gPad->GetFrame()->SetLineWidth(1);
  pad1->Draw();
  pad2->Draw();
  pad3->Draw();
  
  TCanvas *N12fit = (TCanvas *)N12->Get("fit")->Clone("col");
  TCanvas *O13fit = (TCanvas *)O13->Get("fit")->Clone();
  TCanvas *O12fit = (TCanvas *)O12->Get("fit")->Clone();

  pad1->cd();
  t->DrawLatex(0.1,0.4,"my wife");
  N12fit->DrawClonePad();
  pad2->cd();
  O13fit->DrawClonePad();
  pad3->cd();
  O12fit->DrawClonePad();
 

  composite->Update();
  composite->Draw("col");
  composite->Write("col");
 
  calibrations->Write();
}
