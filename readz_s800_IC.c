// Ar31/S29/28P/27Si
using namespace std;
void readz_s800_IC(string setting = "Cl",TString iso = "29S") {
  int Nlines = 0;
  ostringstream outstring;
  if(setting == "Ar")
    outstring << Form("zline/ArSetting/s800_residue_Ar_%s.zline",iso.Data());
  else if(setting =="Cl")
    outstring << Form("zline/ClSetting/s800_residue_Cl_%s.zline",iso.Data());
  else {
    cout << "you have to choose a setting and isotope" << endl;
    cout << "Example (\"Ar\",\"Ar31/S29/28P/27Si\")" << endl;
    return;
  }
  string sname = outstring.str();
  cout << sname << endl;
  ifstream zFile;
  zFile.open(sname);
//  cout << zFile << endl;
  zFile >> Nlines;
  cout << Nlines << endl;
  TCutG *zline[22];
  char name[10];

  string infilename = "sort_55-86.root";
//  string infilename = "sort_94-385.root";
  TFile * infile = new TFile(infilename.c_str(),"READ");
  TH2I * opid;
  TH2I * pid;
  TH2I * apid;
  TH2I * ppid;
  TH2I * pppid;
  TH2I * dpid;

  char oname[20];
  sprintf(oname,"S800/PID/%s/ObjvsICsum_%s_fiber", iso.Data(), iso.Data());
  opid = (TH2I *) infile->Get(oname);
  opid->Draw("colz");

  char hname[20];
  sprintf(hname,"S800/PID/%s/ObjvsICsum_%s_proton1", iso.Data(), iso.Data());
  pid = (TH2I *) infile->Get(hname);
  pid->Draw("colz");

  char aname[20];
  sprintf(aname,"S800/PID/%s/ObjvsICsum_%s_alpha1", iso.Data(), iso.Data());
  apid = (TH2I *) infile->Get(aname);

  char gname[20];
  sprintf(gname,"S800/PID/%s/ObjvsICsum_%s_proton2", iso.Data(), iso.Data());
  ppid = (TH2I *) infile->Get(gname);
//  ppid->Draw("colz");

  char tname[20];
  sprintf(tname,"S800/PID/%s/ObjvsICsum_%s_proton3", iso.Data(), iso.Data());
  pppid = (TH2I *) infile->Get(tname);
//  pppid->Draw("colz");

  char dname[20];
  sprintf(dname,"S800/PID/%s/ObjvsICsum_%s_deuteron", iso.Data(), iso.Data());
  dpid = (TH2I *) infile->Get(dname);

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
    int pcount = zline[i]->IntegralHist(pid);
    int ppcount = zline[i]->IntegralHist(ppid);
    int pppcount = zline[i]->IntegralHist(pppid);
    int acount = zline[i]->IntegralHist(apid);
    int dcount = zline[i]->IntegralHist(dpid);
    cout << iZ << "\t" << iA << "\t" << ngcount << "\t" << pcount << "\t" << ppcount << "\t" << pppcount << "\t" << dcount << "\t" << acount << endl;
  }
  zFile.close();

}
