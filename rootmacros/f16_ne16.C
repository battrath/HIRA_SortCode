{

gROOT->Reset();
TStyle * Sty = new TStyle("MyStyle","MyStyle");
Sty->SetOptTitle(0);
Sty->SetOptStat(0);
Sty->SetPalette(8,0);
Sty->SetCanvasColor(10);
Sty->SetCanvasBorderMode(0);
Sty->SetFrameLineWidth(0);
Sty->SetFrameFillColor(10);
Sty->SetPadColor(10);
Sty->SetPadTickX(1);
Sty->SetPadTickY(1);
Sty->SetPadBottomMargin(.05);
Sty->SetPadTopMargin(.05);
Sty->SetPadLeftMargin(.1);
Sty->SetPadRightMargin(.05);
Sty->SetHistLineWidth(3);
Sty->SetHistLineColor(kRed);
Sty->SetFuncWidth(3);
Sty->SetFuncColor(kGreen);
Sty->SetLineWidth(3);
Sty->SetLabelSize(0.07,"xyz");
Sty->SetLabelOffset(0.02,"y");
Sty->SetLabelOffset(0.02,"x");
Sty->SetLabelColor(kBlack,"xyz");
Sty->SetTitleSize(0.07,"xyz");
Sty->SetTitleOffset(.7,"y");
Sty->SetTitleOffset(1.3,"x");
Sty->SetTitleFillColor(10);
Sty->SetTitleTextColor(kBlack);
Sty->SetTickLength(.05,"xz");
Sty->SetTickLength(.025,"y");
Sty->SetNdivisions(5,"xyz");
Sty->SetEndErrorSize(0);
gROOT->SetStyle("MyStyle");
gROOT->ForceStyle();
TCanvas can("F16_Ne16");

  TH2S frame ("frame","",10,-3,12,10,-1,20);
  frame.GetYaxis()->SetTitle("MeV");
  frame.GetYaxis()->CenterTitle();
  frame.GetXaxis()->SetLabelSize(0.);
  frame.GetXaxis()->SetTickLength(0.);
  frame.GetXaxis()->SetAxisColor(0);
  frame. Draw();

  TLine line;
  line.SetLineWidth(2);
 

  TLatex text;
  text.SetTextSize(.035);
  TArrow arrow;
  arrow.SetAngle(30);

  //16F g.s
  float Ex = 0.;
  line.DrawLine(1,Ex,3,Ex);
  text.DrawLatex(1.,-1.,"^{16}F g.s.(T=1)");

  //16F IAS
  line.SetLineColor(2);
   Ex = 9.928-.2;
  line.DrawLine(1,Ex,3,Ex);
  text.SetTextColor(2);
  text.DrawLatex(1.,Ex+.4,"0^{+} IAS(T=2)?");

  //15F g,s
  Ex = 14.17;
  line.SetLineColor(2);
  line.DrawLine(-2,Ex,0,Ex);
  boxit(-2,Ex-.5,0,Ex+.5,2);
  text.SetTextColor(2);
  text.DrawLatex(-2,Ex-1.3,"n+^{15}F g.s.(T=3/2)");


  text.SetTextColor(1);
  //15O g.s
  float Ex_7Be = -.536;
  Ex = Ex_7Be;
  line.SetLineColor(1);
  line.DrawLine(4,Ex,6,Ex);
  text.DrawLatex(4,Ex-.8,"p+^{15}O(T=1/2)");


  //15O
  Ex = Ex_7Be+ 12.225;
  line.SetLineColor(2);
  line.DrawLine(4,Ex,6,Ex);
  boxit(4,Ex-.135/2.,6,Ex+.135/2.,2);
  text.SetTextColor(2);
  text.DrawLatex(4,Ex+.4,"5/2^{+} (T=3/2)");


  //15O
  Ex = Ex_7Be+ 11.615;
  line.SetLineColor(2);
  line.DrawLine(4,Ex,6,Ex);
  boxit(4,Ex-.202/2.,6,Ex+.202/2.,2);
  text.SetTextColor(2);
  text.DrawLatex(4,Ex-.8,"1/2^{+} IAS(T=3/2)?");

  //14N gs
  float Ex_6Li = 6.761;
  Ex = Ex_6Li;
  line.SetLineColor(1);
  line.DrawLine(7,Ex,9,Ex);
  text.SetTextColor(1);
  text.DrawLatex(7,Ex-.8,"2p+^{14}N(T=0)");


  //14N IAS
  Ex = Ex_6Li + 2.312;
  line.SetLineColor(2);
  line.DrawLine(7,Ex,9,Ex);
  text.SetTextColor(2);
  text.DrawLatex(7,Ex+.4,"0^{+} IAS(T=1)");
  arrow.SetLineColor(2);
  arrow.SetFillColor(2);




  arrow.DrawArrow(3,9.928-.2,7,Ex,.02,"|>");

  arrow.SetFillColor(2);
  arrow.SetLineColor(2);
  arrow.DrawArrow(8,Ex,8,Ex_6Li,.02,"|>");
  //minimum energy 10.619 - 3.562 = 7.057

 
  line.SetLineColor(4);
  text.SetTextColor(4);
  double Eshift = 16.;
  //16Ne g.s
  Ex = 0.+Eshift;
  line.DrawLine(1,Ex,3,Ex);
   boxit(1,Ex-.06,3,Ex+.06,4);
  text.DrawLatex(.1,Ex-.8,"^{16}Ne g.s. (S=0^{+},T=2)");

  //16Ne 1st excited
  Ex = 1.690 + Eshift;
  line.DrawLine(1,Ex,3,Ex);
  text.DrawLatex(1,Ex-.8,"S=2^{+} (T=2)");

  arrow.SetLineColor(4);
  arrow.SetFillColor(4);
  arrow.DrawArrow(3,Ex,4,Ex-1.6801,.02,"|>");

  //15F g.s
  Ex_7Be = .0099+Eshift-.5;
  Ex = Ex_7Be;
  line.SetLineColor(4);
   boxit(4,Ex-.25,6,Ex+.25,4);
  line.DrawLine(4,Ex,6,Ex);
  text.DrawLatex(4,Ex+.6,"p+^{15}F (S=1/2^{+},T=3/2)");

  arrow.SetLineColor(4);
  arrow.SetFillColor(4);
  arrow.DrawArrow(6,Ex,7,Eshift-1.405,.02,"|>");

  //14O
  Ex =  - 1.405 + Eshift;
  line.DrawLine(7,Ex,9,Ex);
  text.DrawLatex(7,Ex+.4,"2p+^{14}O (S=0^{+},T=1)");



  line.SetLineStyle(2);
  line.SetLineColor(1);
  line.DrawLine(-1,15.5,11.5,11);

  arrow.SetLineColor(4);
  arrow.SetFillColor(4);
  arrow.DrawArrow(3,Eshift,7,Eshift-1.405,.02,"|>");

}
