void draw() {
  TH2I *raw;
  TH2I *cal;
  int dnum = 1;
  TFile *f = new TFile("sort_178-179.root","READ");
  raw = (TH2I *) f->Get(Form("CsI/CsIpTheta/p_ECsI_theta_%i", dnum));
  cal = (TH2I *) f->Get(Form("CsI/CsIpTheta/p_ECsI_thetaCal_%i", dnum));

  raw->SetStats(0);
  raw->SetAxisRange(300,400,"Y");
  cal->SetStats(0);
  cal->SetAxisRange(60,80,"Y");
  TCanvas *c1 = new TCanvas();
  c1->Divide(2,1);
  c1->Draw();
  c1->cd(1);
  raw->Draw("colz");
  c1->cd(2);
  cal->Draw("colz");
}
