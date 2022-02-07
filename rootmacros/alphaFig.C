void alphaFig()
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

  TFile *file = new TFile("/home/Oxygen11/sortcode_addback/sort.root");\
  TFile *alphas = new TFile("alphas.root","RECREATE");
  ostringstream outstring;
  string name;

  TCanvas *mycan = new TCanvas("mycan","mycan", 1700,1000);
  mycan->cd();
  outstring.str("");
  outstring << "front/cal/EFC4_29";
  name = outstring.str();

  TH1I * alpha = (TH1I*)file->Get(name.c_str())->Clone();

  alpha->GetXaxis()->SetRangeUser(4,10);
  alpha->GetXaxis()->SetTitle("Energy [MeV]");
  alpha->GetXaxis()->CenterTitle();
  alpha->GetYaxis()->SetTitle("Counts");
  alpha->GetYaxis()->CenterTitle();

  alpha->SetStats(kFALSE);
  alpha->SetLineColor(kBlack);
  alpha->SetLineWidth(2);
  alpha->SetFillStyle(3001);
  alpha->Draw();

  mycan->Update();
  mycan->Write();
  alphas->Write();

  file->Close();

}
