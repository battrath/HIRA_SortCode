// Ar31/S29/28P/27Si
void readz_s800(string setting = "Ar",TString iso = "P28") {
  int Nlines = 0;
  ostringstream outstring;
  if(setting == "Ar")
    outstring << "zline/s800_beam.zline";
  else if(setting =="Cl")
    outstring << "zline/s800_beam.zline";
//    outstring << Form("zline/ClSetting/s800_residue_Cl_%s.zline",iso.Data());
  else {
    cout << "you have to choose a setting and isotope" << endl;
    cout << "Example (\"Ar\",\"Ar31/S29/28P/27Si\")" << endl;
    return;
  }
  string sname = outstring.str();
  cout << sname << endl;
  ifstream zFile(sname);
  zFile >> Nlines;
  cout << Nlines << endl;
  TCutG *zline[22];
  char name[10];

  string infilename = "sort_R92.root";
  TFile * infile = new TFile(infilename.c_str(),"READ");
  TH2I * opid;
  TH2I * pid;
  TH2I * apid;
  TH2I * ppid;
  TH2I * pppid;
  TH2I * dpid;

  char oname[20] = "S800/PID/ObjvsXFP";
  opid = (TH2I *) infile->Get(oname);
  opid->Draw("colz");

  for (int i = 0; i < Nlines; i++) {
    int Npoints, iZ, iA;
    zFile >> iZ >> iA;
    zFile >> Npoints;
    sprintf(name,"zline_%i_%i",iA,iZ);
    zline[i] = new TCutG(name,Npoints);
    for (int j = 0; j < Npoints; j++) {
      double x,y;
      zFile >> x >> y;
      zline[i]->SetPoint(j,x,y);
    }
    zline[i]->Draw("same");
    int ngcount = zline[i]->IntegralHist(opid);
    cout << iZ << "\t" << iA << "\t" << ngcount << endl;
  }
}
