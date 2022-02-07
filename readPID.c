//void readPID(int pid = 0) {
void readPID() {
//  gROOT->SetBatch(kTRUE);
  TH2I * ppid[20];
  TH1I *h[3];
  for(int i = 0; i < 20; i++) {
    char nname[20];
    sprintf(nname,"h%i",i);
    h[i] = new TH1I(nname,"",20,0,20);
  }
  string infilename = "sort_Ar.root";
  TFile * infile = new TFile(infilename.c_str(),"READ");
  vector<int> pcount[3];

  for(int pid = 0; pid < 20; pid++) {
    int Nlines = 0;
    ostringstream outstring;
    outstring << Form("zline/pid%i.zline",pid);

    string sname = outstring.str();
    ifstream zFile(sname);
    zFile >> Nlines;
    TCutG *zline[20][22];
    char name[10];
    char hname[20];
    sprintf(hname,"dee/DEE_%i", pid);
    ppid[pid] = (TH2I *) infile->Get(hname);

    for (int i=0;i<Nlines;i++) {
      int Npoints, iZ, iA;
      zFile >> iZ >> iA;
      zFile >> Npoints;
      sprintf(name,"zline_%i_%i",iA,iZ);
      zline[pid][i] = new TCutG(name,Npoints);
      for (int j=0;j<Npoints;j++) {
        double x,y;
        zFile >> x >> y;
        zline[pid][i]->SetPoint(j,x,y);
      }
//      zline[pid][i]->Draw("same");
      h[i]->Fill(pid,zline[pid][i]->IntegralHist(ppid[pid]));
      pcount[i].push_back(zline[pid][i]->IntegralHist(ppid[pid]));
    }
  }
  cout << "N_proton    = "<< accumulate(pcount[0].begin(),pcount[0].end(),0) << endl;
  cout << "N_deuteron  = "<< accumulate(pcount[1].begin(),pcount[1].end(),0) << endl;
  cout << "N_alpha     = "<< accumulate(pcount[2].begin(),pcount[2].end(),0) << endl;
/*
  TCanvas *cc1 = new TCanvas();
  cc1->Divide(1,3);
  cc1->cd(1);
  h[0]->Draw();
  cc1->cd(2);
  h[1]->Draw();
  cc1->cd(3);
  h[2]->Draw();*/
}
