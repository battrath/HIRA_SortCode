#include "TCanvas.h"


void backgroundSubtract()
{
  TFile *file = new TFile("completesort.root");
  TFile *out = new TFile("subtract.root","RECREATE");
  
  ostringstream outstring;
  string name;

  TCanvas *mycan = new TCanvas("mycan","mycan",1700,1000);
  double norm1, norm2;
  bool worked;

  norm1 = -0.0045;
  norm2 = -0.0045;


  outstring.str("");
  outstring << "corr/O11/stripcuts/Erel11O_2p9C_stripcuts";
  name = outstring.str();

  TH1I * O11 = (TH1I*)file->Get(name.c_str())->Clone();
  TH1I * O11_Copy = (TH1I*)file->Get(name.c_str())->Clone();

  outstring.str("");
  outstring << "corr/O11/stripcuts/JacobiY_11O_2p9C_stripcuts";
  name = outstring.str();

  TH2I * JacobiY = (TH2I*)file->Get(name.c_str())->Clone();

  outstring.str("");
  outstring << "corr/O11/stripcuts/JacobiT_11O_2p9C_stripcuts";
  name = outstring.str();

  TH2I * JacobiT = (TH2I*)file->Get(name.c_str())->Clone();

  outstring.str("");
  outstring << "corr/O11/stripcuts/Erel11O_2p9C_Fake_CsIreaction_C11_stripcuts";
  name = outstring.str();

  TH1I * O11_Fake = (TH1I*)file->Get(name.c_str())->Clone();

  outstring.str("");
  //outstring << "corr/O11/stripcuts/Erel11O_2p9C_Fake_Channeling_stripcuts";
  outstring << "corr/O11/stripcuts/Erel11O_2p9C_Fake_CsIreaction_stripcuts";
  name = outstring.str();

  TH1I * O11_Channeling = (TH1I*)file->Get(name.c_str())->Clone();

  outstring.str("");
  outstring << "corr/O11/stripcuts/JacobiY_11O_2p9C_Fake_CsIreaction_stripcuts";
  name = outstring.str();

  TH2I * JacobiY_CsIReaction = (TH2I*)file->Get(name.c_str())->Clone();

  outstring.str("");
  outstring << "corr/O11/stripcuts/JacobiT_11O_2p9C_Fake_CsIreaction_stripcuts";
  name = outstring.str();

  TH2I * JacobiT_CsIReaction = (TH2I*)file->Get(name.c_str())->Clone();

  outstring.str("");
  outstring << "corr/O11/stripcuts/JacobiY_11O_2p9C_Fake_CsIreaction_C11_stripcuts";
  name = outstring.str();

  TH2I * JacobiY_CsIReaction_C11 = (TH2I*)file->Get(name.c_str())->Clone();

  outstring.str("");
  outstring << "corr/O11/stripcuts/JacobiT_11O_2p9C_Fake_CsIreaction_C11_stripcuts";
  name = outstring.str();

  TH2I * JacobiT_CsIReaction_C11 = (TH2I*)file->Get(name.c_str())->Clone();

  worked = O11->Add(O11, O11_Fake, 1., norm1);
  worked = O11->Add(O11, O11_Channeling, 1., norm2);
  worked = JacobiT->Add(JacobiT, JacobiT_CsIReaction, 1., norm1);
  worked = JacobiT->Add(JacobiT, JacobiT_CsIReaction_C11, 1., norm2);
  worked = JacobiY->Add(JacobiY, JacobiY_CsIReaction, 1., norm1);
  worked = JacobiY->Add(JacobiY, JacobiY_CsIReaction_C11, 1., norm2);

  cout << worked << endl;

  out->cd();
  O11->SetStats(kFALSE);
  O11->GetXaxis()->SetTitle("Relative Energy [MeV]");
  O11->GetYaxis()->SetTitle("Counts");
  O11->Write();


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
  Sty->SetPadTickX(2);
  Sty->SetPadTickY(2);
  Sty->SetPadBottomMargin(.15);
  Sty->SetPadTopMargin(.05);
  Sty->SetPadLeftMargin(.15);
  Sty->SetPadRightMargin(.05);
  Sty->SetHistLineWidth(3);
  Sty->SetFuncWidth(3);
  Sty->SetFuncColor(kGreen);
  Sty->SetLineWidth(3);
  Sty->SetLabelSize(0.03,"xyz");
  Sty->SetLabelOffset(0.02,"y");
  Sty->SetLabelOffset(0.02,"x");
  Sty->SetLabelColor(kBlack,"xyz");
  Sty->SetTitleSize(0.04,"xyz");
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

  TLatex text;
  text.SetTextSize(0.037);
  text.SetTextAlign(10);

  JacobiT->SetStats(kFALSE);
  JacobiT->GetXaxis()->SetTitle("E_{pp}/E_{T}");
  JacobiT->GetYaxis()->SetTitle("#theta_{k}");
  JacobiT->GetXaxis()->CenterTitle();
  JacobiT->GetYaxis()->CenterTitle();
  JacobiT->Write();

  JacobiY->SetStats(kFALSE);
  JacobiY->GetXaxis()->SetTitle("E_{core-p}/E_{T}");
  JacobiY->GetYaxis()->SetTitle("#theta_{k}");
  JacobiY->GetXaxis()->CenterTitle();
  JacobiY->GetYaxis()->CenterTitle();
  JacobiY->Write();

  O11_Copy->GetXaxis()->SetRangeUser(0,12);
  O11_Copy->GetYaxis()->SetRangeUser(0,800);

  O11_Copy->SetStats(kFALSE);
  O11_Copy->SetLineColor(kBlack);
  O11_Copy->SetLineWidth(2);
  O11_Copy->SetFillStyle(3001);
  O11_Copy->Draw();
  O11_Copy->GetXaxis()->SetTitle("E_{T} [MeV]");
  O11_Copy->GetYaxis()->SetTitle("Counts");
  O11_Copy->GetXaxis()->CenterTitle();
  O11_Copy->GetYaxis()->CenterTitle();
  mycan->Update();

  O11_Fake->SetLineColor(kRed);
  O11_Fake->Scale((-1.)*norm1);
  O11_Fake->SetLineWidth(2);
  O11_Fake->SetStats(kFALSE);
  O11_Fake->Draw("same");
  mycan->Update();

  O11_Channeling->SetLineColor(kBlue);
  O11_Channeling->Scale((-1.)*norm2);
  O11_Channeling->SetLineWidth(2);
  O11_Channeling->SetStats(kFALSE);
  O11_Channeling->Draw("same");
  mycan->Update();

  text.DrawLatex(8,500,"Raw ^{11}O Spectrum");
  text.SetTextColor(2);
  text.DrawLatex(8,450,"^{13}O Background");
  text.SetTextColor(4);
  text.DrawLatex(8,400,"^{12}O Background");
  mycan->Write();

  O11->Draw();
  JacobiT->Draw();
  JacobiY->Draw();

  mycan->Update();
  
  file->Close();
  out->Close();
}
