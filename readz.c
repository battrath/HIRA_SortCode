void readz( int icsi)
{
  int Nlines;
  ostringstream outstring;
  //outstring << "zlineBackground/pid"<<icsi<<".zline";
  //outstring << "zlineC12_cal/pid"<<icsi<<".zline";
  outstring << "zline/pid"<<icsi<<".zline";
  string sname = outstring.str();
  cout << sname << endl;
  ifstream zFile(sname.c_str());
  zFile >> Nlines;
  cout << Nlines << endl;
  TCutG *zline[22];
  char name[10];
for (int i=0;i<Nlines;i++)
{
  int Npoints, iZ, iA;
  zFile >> iZ >> iA;
  zFile >> Npoints;
  cout << i << " " << Npoints << " Z = " << iZ << " A = " << iA << endl;
  sprintf(name,"zline[%d]",i);
  zline[i] = new TCutG(name,Npoints);

  for (int j=0;j<Npoints;j++)
    {
      double x,y;
      zFile >> x >> y;
      zline[i]->SetPoint(j,x,y);
    }
  zline[i]->Draw();
}
}
