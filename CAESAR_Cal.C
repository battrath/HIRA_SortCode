void CAESAR_Cal() {

  // Load Previous gains
  double gains[192];
  double offsets[192];
  int tmp, tmpN;
  string calfile = "cal/Caesar.cal";
  ifstream fp;
  fp.open(calfile.c_str());
  int iii = 0;
  while(fp.good()){
    fp >> tmp >> tmpN >> gains[iii] >> offsets[iii];
    iii++;
  }
  Int_t num_peaks[3] = {1,2,4};
  Double_t source_energy[3][4] = {
  {661.657,0,0,0},
  {898.042,1836.063,0,0},
  {276.3989, 302.851, 356.013, 383.848}
  };

  vector<double> centroids[192];
  vector<double> energies[192];
  vector<double> charge[192];
  vector<double> charge_er[192];

  string CalSource[3] = {"137Cs","88Y","133Ba"};
  TH1I *h[3][192];
  TF1 *f[3][192][4];
  TList *list = new TList();
  for(int i = 0; i < 2; i++) {
    char fname[64];
    sprintf(fname,"sort_%s.root",CalSource[i].c_str());
    TFile *hfile = new TFile(fname,"READ");
    for(int j = 0; j < 192; j++) {
      h[i][j] = (TH1I *) hfile->Get(Form("ceasar/cal/ECcal%3.3i",j));
      list->Add(h[i][j]);
      if(h[i][j]->Integral(0,-1) < 100) {
        centroids[j].push_back(0);
	energies[j].push_back(0);
      } else {
        for(int k = 0; k < num_peaks[i]; k++) {
	  Double_t x_guess = source_energy[i][k]/1000.0;
          h[i][j]->SetAxisRange(x_guess - 0.04, x_guess + 0.04,"X");
          Int_t bin = h[i][j]->GetMaximumBin();
          x_guess = h[i][j]->GetXaxis()->GetBinCenter(bin);
          h[i][j]->SetAxisRange(0,2,"X");
	  f[i][j][k] = new TF1("f1","gaus(0)+pol1(3)",x_guess - 0.05, x_guess + 0.05);
          f[i][j][k]->SetParameters(100,x_guess,0.02,0,1);
          f[i][j][k]->SetParLimits(1,x_guess - 0.02, x_guess + 0.02);
          f[i][j][k]->SetParLimits(2,1.0/1000.0,30.0/1000.0);
	  h[i][j]->Fit(f[i][j][k],"QR+");
          centroids[j].push_back(f[i][j][k]->GetParameter(1)*1000);
	  double tmpCharge = (f[i][j][k]->GetParameter(1)-offsets[j])/gains[j];
	  charge[j].push_back(tmpCharge);
	  tmpCharge = f[i][j][k]->GetParError(1)*tmpCharge/f[i][j][k]->GetParameter(1);
	  charge_er[j].push_back(tmpCharge);
          energies[j].push_back(source_energy[i][k]);
        }
      }
    }
  }
  ofstream coeff("cal/Caesar_SG.cal");
  TGraph * gr[192];
  TGraphErrors *gc[192];
  TF1 *fc[192];
  for(int i = 0; i < 192; i++) {
    int counter = 0;
    gr[i] = new TGraph();
    gr[i]->SetName(Form("Residuals_%i",i));
    gc[i] = new TGraphErrors();
    gc[i]->SetName(Form("Calib_%i",i));
    for(int j = 0; j < centroids[i].size(); j++) {
      if(centroids[i].at(j) == 0) continue;
      gr[i]->SetPoint(counter, energies[i].at(j), centroids[i].at(j) - energies[i].at(j));
      gc[i]->SetPoint(counter, charge[i].at(j), energies[i].at(j)/1000.0);
      gc[i]->SetPointError(counter,charge_er[i].at(j),0);
      counter++;
    }
    if(counter != 0) {
      fc[i] = new TF1("fc","pol1");
      fc[i]->SetParameters(offsets[i],gains[i]);
      gc[i]->Fit(fc[i],"Q");
//      cout << fc[i]->GetParameter(1) << "\t" << fc[i]->GetParameter(0) << endl;
      coeff << 0 << "\t" << i << "\t" << fc[i]->GetParameter(1) << "\t" << fc[i]->GetParameter(0) << endl;
      list->Add(gr[i]);
      list->Add(gc[i]);
    } else {
      coeff << 0 << "\t" << i << "\t" << -1 << "\t" << 0 << endl;
    }
  }
  coeff.close();
  TFile *out = new TFile("fits.root","RECREATE");
  out->cd();
  list->Write();
  out->Close();
}

