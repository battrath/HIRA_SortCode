{
TStyle * Sty = new TStyle("MyStyle","MyStyle");
Sty->SetOptTitle(0);
Sty->SetOptStat(0);
//Sty->SetPalette(8,0);
Sty->SetCanvasColor(10);
Sty->SetCanvasBorderMode(0);
Sty->SetFrameLineWidth(3);
Sty->SetFrameFillColor(10);
Sty->SetPadColor(10);
Sty->SetPadTickX(1);
Sty->SetPadTickY(1);
Sty->SetPadBottomMargin(.2);
Sty->SetPadLeftMargin(.17);
Sty->SetPadTopMargin(.05);
Sty->SetPadRightMargin(.05);
Sty->SetHistLineWidth(3);
Sty->SetHistLineColor(kRed);
Sty->SetFuncWidth(3);
Sty->SetFuncColor(kGreen);
Sty->SetLineWidth(3);
Sty->SetLabelSize(0.06,"xyz");
Sty->SetLabelOffset(0.02,"y");
Sty->SetLabelOffset(0.02,"x");
Sty->SetLabelColor(kBlack,"xyz");
Sty->SetTitleSize(0.08,"xyz");
Sty->SetTitleOffset(1.,"y");
Sty->SetTitleOffset(1.,"x");
Sty->SetTitleFillColor(10);
Sty->SetTitleTextColor(kBlack);
Sty->SetTickLength(.05,"xz");
Sty->SetTickLength(.025,"y");
Sty->SetNdivisions(5,"xyz");
Sty->SetEndErrorSize(0);
gROOT->SetStyle("MyStyle");
gROOT->ForceStyle();

    TArrow arrow;
 TCanvas canvas("twoFit","",600,800);
//divide up canvas into three pads
 double overlap = .06;

 TPad *pad1  = new TPad("pad1","",0.,0.,1.,1./3.+overlap);
 TPad *pad2  = new TPad("pad2","",0.,1./3.-overlap/2,1.,2./3.+overlap/2.);
 TPad *pad3  = new TPad("pad3","",0.,2./3.-overlap,1.,1.);
pad1->SetFillStyle(4000);
pad2->SetFillStyle(4000);
pad3->SetFillStyle(4000);

gPad->GetFrame()->SetLineWidth(1);
pad1->Draw();
pad2->Draw();
pad3->Draw();

 pad3->cd();
 //gPad->SetLogy();
 //TH2S frame1("frame1","",10,12.3,14,10,0,7000);
  TH2S frame1("frame1","",10,7.5,14.2,10,0,4200);
  //frame1.GetXaxis()->SetTitle("E* [MeV]");
  frame1.GetYaxis()->SetTitle("Counts");  
  frame1.GetXaxis()->CenterTitle();
  frame1.GetYaxis()->CenterTitle();
  frame1.GetXaxis()->SetLabelSize(0.);
  frame1.SetNdivisions(510);
  frame1.Draw();

  TLatex tt;
  tt.SetNDC();
  tt.SetTextSize(.07);
  tt.DrawLatex(.3,.85,"(a) p+^{15}N");


  TFile fileX1("/home/Boron8/Ne17a/tree/O16/p15N/out.root");
  TH1I* hist1 = (TH1I*) fileX1.Get("Ex_trans_narrow_15Obeam");


hist1->SetMarkerStyle(20);
 hist1->SetMarkerColor(1);
 hist1->SetLineColor(1);

 hist1->Draw("same PE");


 double one,two,three,four,five,six,seven,eight,nine,ten,eleven,twelve,thirteen,fourteen,fifteen;

ifstream fileIn("/home/Boron8/Ne17a/tree/O16/p15N/fit/fit.dat");

  double x1[200];
  double ytot1[200];
  double yb1[200];
  double y1_1[200];
  double y2_1[200];
  double y3_1[200];
  double y4_1[200];
   double y5_1[200];
   double y6_1[200];
   double y7_1[200];
   double y8_1[200];
   double y9_1[200];
   double y10_1[200];
   double y11_1[200];
   double y12_1[200];

  int N1;
  for (;;)
    {
     fileIn >> one >> two >> three >> four >> five >> six >> seven >> eight >>
     nine >> ten >> eleven >> twelve >> thirteen >> fourteen >> fifteen;
     if (fileIn.eof()) break;
     if (fileIn.bad())break;
   x1[N1] = one;
   ytot1[N1] = two;
   yb1[N1] = three;
   y1_1[N1] = four;
   y2_1[N1] = five;
   y3_1[N1] = six;
   y4_1[N1] = seven;
   y5_1[N1] = eight;
   y6_1[N1] = nine;
   y7_1[N1] = ten;
   y8_1[N1] = eleven;
   y9_1[N1] = twelve;
   y10_1[N1] = thirteen;
   y11_1[N1] = fourteen;
   y12_1[N1] = fifteen;

  N1++;
}
  fileIn.close();
  fileIn.clear();
TGraph gtot1(N1,x1,ytot1);
gtot1.SetLineColor(2);
gtot1.SetLineWidth(2);
gtot1.Draw("C");


TGraph gb1(N1,x1,yb1);
gb1.SetLineColor(4);
gb1.SetLineWidth(2);
gb1.SetLineStyle(9);
gb1.Draw("C");



TGraph g1_1(N1,x1,y1_1);
g1_1.SetLineColor(3);
g1_1.SetLineWidth(2);
g1_1.SetLineStyle(1);
g1_1.Draw("C");

TGraph g2_1(N1,x1,y2_1);
g2_1.SetLineColor(3);
g2_1.SetLineWidth(2);
g2_1.SetLineStyle(1);
g2_1.Draw("C");

TGraph g3_1(N1,x1,y3_1);
g3_1.SetLineColor(3);
g3_1.SetLineWidth(2);
g3_1.SetLineStyle(1);
g3_1.Draw("C");


TGraph g4_1(N1,x1,y4_1);
g4_1.SetLineColor(3);
g4_1.SetLineWidth(2);
g4_1.SetLineStyle(1);
g4_1.Draw("C");


TGraph g5_1(N1,x1,y5_1);
g5_1.SetLineColor(3);
g5_1.SetLineWidth(2);
g5_1.SetLineStyle(1);
g5_1.Draw("C");


TGraph g6_1(N1,x1,y6_1);
g6_1.SetLineColor(3);
g6_1.SetLineWidth(2);
g6_1.SetLineStyle(1);
g6_1.Draw("C");



TGraph g7_1(N1,x1,y7_1);
g7_1.SetLineColor(3);
g7_1.SetLineWidth(2);
g7_1.SetLineStyle(2);
g7_1.Draw("C");

TGraph g8_1(N1,x1,y8_1);
g8_1.SetLineColor(6);
g8_1.SetLineWidth(2);
g8_1.SetLineStyle(2);
g8_1.Draw("C");


TGraph g9_1(N1,x1,y9_1);
g9_1.SetLineColor(6);
g9_1.SetLineWidth(2);
g9_1.SetLineStyle(2);
g9_1.Draw("C");



TGraph g10_1(N1,x1,y10_1);
g10_1.SetLineColor(6);
g10_1.SetLineWidth(2);
g10_1.SetLineStyle(2);
g10_1.Draw("C");



TGraph g11_1(N1,x1,y11_1);
g11_1.SetLineColor(6);
g11_1.SetLineWidth(2);
g11_1.SetLineStyle(2);
g11_1.Draw("C");


TGraph g12_1(N1,x1,y12_1);
g12_1.SetLineColor(6);
g12_1.SetLineWidth(2);
g12_1.SetLineStyle(2);
//g12_1.Draw("C");



 arrow.SetFillColor(1);
 arrow.SetAngle(25);
 arrow.DrawArrow(12.530,1100,12.530,500,.025,"|>");
 arrow.DrawArrow(13.259,1800,13.259,1200,.025,"|>");
 arrow.DrawArrow(12.969,4100,12.969,3600,.025,"|>");
 TLatex text;
 text.SetTextAngle(90.);
 text.SetTextSize(.06);
 text.DrawLatex(12.530+.05,1200,"12.530");
 text.DrawLatex(13.259+.05,1900,"13.259");
 text.DrawLatex(12.8,3100,"12.969");

 pad2->cd();

 //gPad->SetLogy();

  TH2S frame2("frame2","",10,7.5,14.2,10,0,8000);
  frame2.SetStats(kFALSE);
  //frame2.GetXaxis()->SetTitle("E* [MeV]");
  frame2.GetXaxis()->SetLabelSize(0.);
  frame2.GetYaxis()->SetTitle("Counts");  
  frame2.GetXaxis()->CenterTitle();
  frame2.GetYaxis()->CenterTitle();
  frame2.SetNdivisions(510);
  frame2.Draw();

  tt.DrawLatex(.7,.85,"(b) #alpha+^{12}C");


  TFile fileX2("/home/Boron8/Ne17a/tree/O16/a12C/out.root");
  TH1I* hist2 = (TH1I*) fileX2.Get("Ex_trans_narrow_15Obeam");
  TH1I* histExp_gamma = (TH1I*) fileX2.Get("Ex_trans_narrow_gamma1");
  TH1I* hist_gamma = (TH1I*) fileX2.Get("Egamma");

hist2->SetMarkerStyle(20);
 hist2->SetMarkerColor(1);
 hist2->SetLineColor(1);

 hist2->Draw("same PE");


fileIn.open("/home/Boron8/Ne17a/tree/O16/a12C/fit/fit.dat");

  double x2[400];
  double ytot2[400];
  double yb2[400];
  double y1_2[400];
  double y2_2[400];
  double y3_2[400];
  double y4_2[400];
  double y5_2[400];
  double y6_2[400];
  double y7_2[400];
  double y8_2[400];
  double y9_2[400];
  double y10_2[400];
  double y11_2[400];
  double y12_2[400];
  int N2;
  for (;;)
    {
     fileIn >> one >> two >> three >> four >> five >> six >> seven >> eight >>
     nine >> ten >> eleven >> twelve >> thirteen >> fourteen >> fifteen;


     if (fileIn.eof()) break;
     if (fileIn.bad())break;
   x2[N2] = one;
   ytot2[N2] = two;
   yb2[N2] = three;
   y1_2[N2] = four;
   y2_2[N2] = five;
   y3_2[N2] = six;
   y4_2[N2] = seven;
   y5_2[N2] = eight;
   y6_2[N2] = nine;
   y7_2[N2] = ten;
   y8_2[N2] = eleven;
   y9_2[N2] = twelve;
   y10_2[N2] = thirteen;
   y11_2[N2] = fourteen;
   y12_2[N2] = fifteen;
  N2++;
  if (N2 == 399) break;
}

  cout << N2 << endl;
  fileIn.close();
  fileIn.clear();
TGraph gtot2(N2,x2,ytot2);
gtot2.SetLineColor(2);
gtot2.SetLineWidth(2);
gtot2.Draw("C");


TGraph gb2(N2,x2,yb2);
gb2.SetLineColor(4);
gb2.SetLineWidth(2);
gb2.SetLineStyle(9);
gb2.Draw("C");

 TGraph g1_2(N2,x2,y1_2);   //empty
g1_2.SetLineColor(3);
g1_2.SetLineWidth(2);
g1_2.SetLineStyle(1);
g1_2.Draw("C");

TGraph g2_2(N2,x2,y2_2);
g2_2.SetLineColor(6);
g2_2.SetLineWidth(2);
g2_2.SetLineStyle(2);
g2_2.Draw("C");



 TGraph g3_2(N2,x2,y3_2); //empty
g3_2.SetLineColor(3);
g3_2.SetLineWidth(2);
g3_2.SetLineStyle(1);
g3_2.Draw("C");


TGraph g4_2(N2,x2,y4_2);
g4_2.SetLineColor(6);
g4_2.SetLineWidth(2);
g4_2.SetLineStyle(2);
g4_2.Draw("C");


 TGraph g5_2(N2,x2,y5_2); //empty
g5_2.SetLineColor(3);
g5_2.SetLineWidth(2);
g5_2.SetLineStyle(1);
g5_2.Draw("C");


TGraph g6_2(N2,x2,y6_2);
g6_2.SetLineColor(6);
g6_2.SetLineWidth(2);
g6_2.SetLineStyle(2);
g6_2.Draw("C");



TGraph g7_2(N2,x2,y7_2);
g7_2.SetLineColor(3);
g7_2.SetLineWidth(2);
g7_2.SetLineStyle(1);
g7_2.Draw("C");


 TGraph g8_2(N2,x2,y8_2);  //empty
g8_2.SetLineColor(6);
g8_2.SetLineWidth(2);
g8_2.SetLineStyle(2);
g8_2.Draw("C");


TGraph g9_2(N2,x2,y9_2);
g9_2.SetLineColor(3);
g9_2.SetLineWidth(2);
g9_2.SetLineStyle(1);
g9_2.Draw("C");



TGraph g10_2(N2,x2,y10_2);
g10_2.SetLineColor(6);
g10_2.SetLineWidth(2);
g10_2.SetLineStyle(2);
g10_2.Draw("C");



 TGraph g11_2(N2,x2,y11_2); //empty
g11_2.SetLineColor(6);
g11_2.SetLineWidth(2);
g11_2.SetLineStyle(2);
g11_2.Draw("C");


 TGraph g12_2(N2,x2,y12_2); //empty
g12_2.SetLineColor(6);
g12_2.SetLineWidth(2);
g12_2.SetLineStyle(2);
g12_2.Draw("C");

 arrow.DrawArrow(13.259,4400,13.259,3400,.02,"|>");
 arrow.DrawArrow(12.530-4.438,2200,12.530-4.438,1200,.02,"|>");
  arrow.DrawArrow(13.259-4.438,5400,13.259-4.438,4400,.02,"|>");
  arrow.DrawArrow(12.969-4.438,7850,12.969-4.438,7100,.02,"|>");
 arrow.DrawArrow(11.096,3000,11.096,2000,.02,"|>");


  text.DrawLatex(13.259+.05,4450,"13.259");
  text.DrawLatex(12.530+.05-4.438,2250,"12.530");
text.DrawLatex(13.259+.05-4.438,5450,"13.259");
text.DrawLatex(12.969-.1-4.438,6000,"12.969");
  text.DrawLatex(11.096+.05,3050,"11.096");
 
 pad1->cd();


  TH2S frame3("frame3","",10,7.5,14.2,10,0,1200);
  frame3.SetStats(kFALSE);
  frame3.GetXaxis()->SetTitle("E^{*}_{n#gamma} [MeV]");
  frame3.GetYaxis()->SetTitle("Counts");  
  frame3.GetXaxis()->CenterTitle();
  frame3.GetYaxis()->CenterTitle();
  frame3.SetNdivisions(510);
  frame3.Draw();



  histExp_gamma->SetMarkerStyle(20);
  histExp_gamma->SetMarkerColor(1);
  histExp_gamma->SetLineColor(1);
  histExp_gamma->Draw("same lPE");

  tt.DrawLatex(.6,.85,"(c) #alpha+^{12}C+#gamma events");

  TPad *inset  = new TPad("inset","",0.4,0.25,.95,.85);
   inset->SetFillStyle(4000);

    inset->Draw();
    inset->cd();
    TH2S frameI("frameI","",10,0,5.5,10,0,7200);
  frameI.GetXaxis()->SetTitle("E_{#gamma} [MeV]");
  frameI.GetXaxis()->SetTitleSize(.2);
  frameI.GetYaxis()->SetTitle("Counts");  
  frameI.GetXaxis()->SetTitleSize(.07);
  frameI.GetYaxis()->SetTitleSize(.07);
  frameI.GetXaxis()->SetTitleOffset(1.);
  frameI.GetXaxis()->SetNdivisions(510);
  frameI.GetYaxis()->SetTitleOffset(1.2);    

  frameI.GetXaxis()->CenterTitle();
  frameI.GetYaxis()->CenterTitle();
  frameI.Draw();

  hist_gamma->SetBinContent(1,0);
  hist_gamma->SetBinContent(2,0);
    hist_gamma->Draw("same");

    TLine ll;
    ll.SetLineStyle(2);
    ll.DrawLine(3.06,0,3.06,6000);
    ll.DrawLine(4.9,0,4.9,6000);


    arrow.SetLineColor(1);
    arrow.SetFillColor(1);
    arrow.SetAngle(40);
    double dd=.1;
    arrow.DrawArrow(3.06+dd,5000,4.9-dd,5000,.03,"<|>");
    TLatex ttt;
    ttt.SetTextColor(1);
    ttt.SetTextSize(.09);
    ttt.DrawLatex(3.6,5200,"Gate");


} 
