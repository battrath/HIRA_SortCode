#include "TCanvas.h"


void backgroundSubtract_12O()
{
  TFile *file = new TFile("read_O11.root");
  TFile *out = new TFile("out2.root","RECREATE");
  
  ostringstream outstring;
  string name;

  TCanvas *mycan = new TCanvas("mycan","mycan",1600,900);
  double norm1;
  bool worked;

  norm1 = -0.015;

  outstring.str("");
  outstring << "corr/O12/stripcuts/Erel12O_2p10C_Trans_stripcuts";
  name = outstring.str();

  TH1I * O12 = (TH1I*)file->Get(name.c_str())->Clone();
  TH1I * O12_Copy = (TH1I*)file->Get(name.c_str())->Clone();

  outstring.str("");
  outstring << "corr/O12/stripcuts/JacobiY_12O_2p10C_stripcuts";
  name = outstring.str();

  TH2I * JacobiY = (TH2I*)file->Get(name.c_str())->Clone();

  outstring.str("");
  outstring << "corr/O12/stripcuts/JacobiT_12O_2p10C_stripcuts";
  name = outstring.str();

  TH2I * JacobiT = (TH2I*)file->Get(name.c_str())->Clone();

  outstring.str("");
  outstring << "corr/O12/stripcuts/JacobiY_12O_2p10C_star1_stripcuts";
  name = outstring.str();

  TH2I * JacobiY_star1 = (TH2I*)file->Get(name.c_str())->Clone();

  outstring.str("");
  outstring << "corr/O12/stripcuts/JacobiT_12O_2p10C_star1_stripcuts";
  name = outstring.str();

  TH2I * JacobiT_star1 = (TH2I*)file->Get(name.c_str())->Clone();

  outstring.str("");
  outstring << "corr/O12/stripcuts/JacobiY_12O_2p10C_star2_stripcuts";
  name = outstring.str();

  TH2I * JacobiY_star2 = (TH2I*)file->Get(name.c_str())->Clone();

  outstring.str("");
  outstring << "corr/O12/stripcuts/JacobiT_12O_2p10C_star2_stripcuts";
  name = outstring.str();

  TH2I * JacobiT_star2 = (TH2I*)file->Get(name.c_str())->Clone();




  //fakes

  outstring.str("");
  outstring << "corr/O12/stripcuts/Erel12O_2p10C_Fake_CsIreaction_Trans_stripcuts";
  name = outstring.str();

  TH1I * O12_Fake = (TH1I*)file->Get(name.c_str())->Clone();

  outstring.str("");
  outstring << "corr/O12/stripcuts/JacobiY_12O_2p10C_Fake_stripcuts";
  name = outstring.str();

  TH2I * JacobiY_Fake = (TH2I*)file->Get(name.c_str())->Clone();

  outstring.str("");
  outstring << "corr/O12/stripcuts/JacobiT_12O_2p10C_Fake_stripcuts";
  name = outstring.str();

  TH2I * JacobiT_Fake = (TH2I*)file->Get(name.c_str())->Clone();

  outstring.str("");
  outstring << "corr/O12/stripcuts/JacobiY_12O_2p10C_star1_Fake_stripcuts";
  name = outstring.str();

  TH2I * JacobiY_star1_Fake = (TH2I*)file->Get(name.c_str())->Clone();

  outstring.str("");
  outstring << "corr/O12/stripcuts/JacobiT_12O_2p10C_star1_Fake_stripcuts";
  name = outstring.str();

  TH2I * JacobiT_star1_Fake = (TH2I*)file->Get(name.c_str())->Clone();

  outstring.str("");
  outstring << "corr/O12/stripcuts/JacobiY_12O_2p10C_star2_Fake_stripcuts";
  name = outstring.str();

  TH2I * JacobiY_star2_Fake = (TH2I*)file->Get(name.c_str())->Clone();

  outstring.str("");
  outstring << "corr/O12/stripcuts/JacobiT_12O_2p10C_star2_Fake_stripcuts";
  name = outstring.str();

  TH2I * JacobiT_star2_Fake = (TH2I*)file->Get(name.c_str())->Clone();



  worked = O12->Add(O12, O12_Fake, 1., norm1);

  worked = JacobiT->Add(JacobiT, JacobiT_Fake, 1., norm1);
  worked = JacobiT_star1->Add(JacobiT_star1, JacobiT_star1_Fake, 1., norm1);
  worked = JacobiT_star2->Add(JacobiT_star2, JacobiT_star2_Fake, 1., norm1);
  worked = JacobiY->Add(JacobiY, JacobiY_Fake, 1., norm1);
  worked = JacobiY_star1->Add(JacobiY_star1, JacobiY_star1_Fake, 1., norm1);
  worked = JacobiY_star2->Add(JacobiY_star2, JacobiY_star2_Fake, 1., norm1);


  cout << worked << endl;

  out->cd();
  O12->SetStats(kFALSE);
  O12->GetXaxis()->SetTitle("Relative Energy [MeV]");
  O12->GetYaxis()->SetTitle("Counts");
  O12->Write();

  JacobiT->SetStats(kFALSE);
  JacobiT->GetXaxis()->SetTitle("E_{pp}");
  JacobiT->GetYaxis()->SetTitle("#theta_{k}");
  JacobiT->Write();

  JacobiT_star1->SetStats(kFALSE);
  JacobiT_star1->GetXaxis()->SetTitle("E_{pp}");
  JacobiT_star1->GetYaxis()->SetTitle("#theta_{k}");
  JacobiT_star1->Write();

  JacobiT_star2->SetStats(kFALSE);
  JacobiT_star2->GetXaxis()->SetTitle("E_{pp}");
  JacobiT_star2->GetYaxis()->SetTitle("#theta_{k}");
  JacobiT_star2->Write();

  JacobiY->SetStats(kFALSE);
  JacobiY->GetXaxis()->SetTitle("E_{core-p}");
  JacobiY->GetYaxis()->SetTitle("#theta_{k}");
  JacobiY->Write();

  JacobiY_star1->SetStats(kFALSE);
  JacobiY_star1->GetXaxis()->SetTitle("E_{core-p}");
  JacobiY_star1->GetYaxis()->SetTitle("#theta_{k}");
  JacobiY_star1->Write();

  JacobiY_star2->SetStats(kFALSE);
  JacobiY_star2->GetXaxis()->SetTitle("E_{core-p}");
  JacobiY_star2->GetYaxis()->SetTitle("#theta_{k}");
  JacobiY_star2->Write();

  O12_Copy->SetStats(kFALSE);
  O12_Copy->Draw();
  O12_Copy->GetXaxis()->SetTitle("Relative Energy [MeV]");
  O12_Copy->GetYaxis()->SetTitle("Counts");
  mycan->Update();

  O12_Fake->SetLineColor(kRed);
  O12_Fake->Scale((-1.)*norm1);
  O12_Fake->SetStats(kFALSE);
  O12_Fake->Draw("same");
  mycan->Update();
  mycan->Write();

  O12->Draw();
  JacobiT->Draw();
  JacobiY->Draw();
  JacobiT_star1->Draw();
  JacobiY_star1->Draw();
  JacobiT_star2->Draw();
  JacobiY_star2->Draw();

  mycan->Update();
  
  file->Close();
  out->Close();
}
