void fourp2alpha()
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

  TFile *file = new TFile("completesort.root");
  TFile *out = new TFile("fourp2alpha.root","RECREATE");

  TLatex tt;

  tt.SetTextSize(0.07);

  ostringstream outstring;
  string name;
  out->cd();

  TCanvas *mycan = new TCanvas("mycan","mycan",1700,1000);

  mycan->cd();
  outstring.str("");
  outstring << "corr/O12/Erel12O_4p2a";
  name = outstring.str();

  TH1I * O12 = (TH1I*)file->Get(name.c_str());
 
  O12->SetStats(kFALSE);
  O12->GetXaxis()->SetTitle("E_{T} [MeV]");
  O12->GetXaxis()->CenterTitle();
  O12->GetYaxis()->SetTitle("Counts");
  O12->GetYaxis()->CenterTitle();
  O12->Draw();

  tt.DrawLatex(2,50,"4p+2#alpha");

  mycan->Update();
  mycan->Write();

  file->Close();
  out->Close();
}
