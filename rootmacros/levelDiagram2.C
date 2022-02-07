void levelDiagram(){
 TStyle * Sty = (TStyle*)gROOT->FindObject("MyStyle");
  if(!Sty)
    { 
      Sty = new TStyle("MyStyle","MyStyle");
    }
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
  Sty->SetPadTopMargin(.1);
  Sty->SetPadLeftMargin(.15);
  Sty->SetPadRightMargin(.07);
  Sty->SetHistLineWidth(3);
  Sty->SetHistLineColor(kRed);
  Sty->SetFuncWidth(3);
  Sty->SetFuncColor(kGreen);
  Sty->SetLineWidth(3);
  Sty->SetLabelSize(0.065,"xyz");
  Sty->SetLabelOffset(0.01,"y");
  Sty->SetLabelOffset(0.02,"x");
  Sty->SetLabelColor(kBlack,"xyz");
  Sty->SetTitleSize(0.07,"xyz");
  Sty->SetTitleOffset(.854,"y");
  Sty->SetTitleOffset(1.3,"x");
  Sty->SetTitleFillColor(10);
  Sty->SetTitleTextColor(kBlack);
  Sty->SetTickLength(.05,"xz");
  Sty->SetTickLength(.025,"y");
  Sty->SetNdivisions(10,"xyz");
  Sty->SetEndErrorSize(0);
   
  gROOT->SetStyle("MyStyle");
  gROOT->ForceStyle();
  TCanvas *mycan = new TCanvas("mycan","mycan",800,800);

  double overlap = 0.04;
  TPad *pad1 = new TPad("pad1","",0.,0.,1./2. + overlap,1.);
  TPad *pad2 = new TPad("pad2","",1./2. - overlap,0.,1.,1.);

  pad1->SetFillStyle(4000);
  pad2->SetFillStyle(4000);

  pad1->Draw();
  pad2->Draw();
 
  pad2->cd();
  TH2S *myframe1= new TH2S("frame1","",10,-1,6.5,10,-0.5,7.0);

  myframe1->GetXaxis()->SetTickLength(0);
  myframe1->GetXaxis()->SetLabelSize(0);
  myframe1->GetXaxis()->SetAxisColor(0);


  myframe1->Draw();

  TLine line;
  line.SetLineWidth(7);

  TAttFill fill;
  fill.SetFillStyle(3004);

  TLatex text;
  text.SetTextSize(.06);
  text.SetTextAlign(10);

  TArrow arrow;
  arrow.SetAngle(20);
  arrow.SetFillColor(1);
  arrow.SetLineWidth(3);
  arrow.SetArrowSize(0.03);
  arrow.SetLineStyle(9);

  float textspacing;
  textspacing = -0.1;

  float Ex = 4.561;
  line.DrawLine(-0.2,Ex,1.25,Ex);
  boxit(-0.2,Ex-1.64,1.25,Ex+1.64,2);
  text.DrawLatex(1.27,Ex+textspacing,"3/2^{-}");

  //arrow.DrawArrow(1.25,Ex,2,2.6);
  //arrow.DrawArrow(1.25,Ex,2,3.44);
  
  

  //arrow.SetLineStyle(1);
  //arrow.DrawArrow(1.25,Ex,3.95,0.);
  //arrow.DrawArrow(1.25,Ex,3.95,3.3);
  //arrow.DrawArrow(1.25,Ex,3.95,2.218);


  Ex = 1.9;
  line.DrawLine(2.725,Ex,3.45,Ex);
  boxit(2.725,Ex-0.4,3.45,Ex+0.4,2);
  text.DrawLatex(3.45,Ex+textspacing,"1^{-}");
  
  //arrow.SetLineStyle(9);

  //arrow.DrawArrow(3.25,Ex,4,2.218);  
  //arrow.DrawArrow(3.25,Ex,4,0.);
  
  Ex = 2.8;
  line.DrawLine(2.0,Ex,2.725,Ex);
  boxit(2.0,Ex-1.0,2.725,Ex+1.0,2);
  text.DrawLatex(1.6,Ex+2*textspacing,"2^{-}");

  //arrow.DrawArrow(3.25,Ex,4,2.218);  
  //arrow.DrawArrow(3.25,Ex,4,0.);
  

  
  //arrow.DrawArrow(3.25,Ex,4,-1.44);

  //arrow.SetLineStyle(1);
  //arrow.DrawArrow(4.625,Ex,4.625,-1.25);

  // text.DrawLatex(4.75,-0.545,"#gamma");
  //text.DrawLatex(2.5,-1,"2p");
  //text.DrawLatex(1.3,1.5,"p");
  //text.DrawLatex(3.4,-0.5,"p");
  
  Ex = 0;
  line.DrawLine(4.2,Ex,5.65,Ex);
  text.DrawLatex(5.7,Ex+textspacing,"3/2^{-}");
 

  Ex = 2.218;
  line.DrawLine(4.2,Ex,5.65,Ex);
  boxit(4.2,Ex-0.05,5.65,Ex+0.050,2);  
  
 
  Ex = 3.3;
  line.DrawLine(4.2,Ex,5.65,Ex);
  


  text.SetTextSize(0.06);
  text.DrawLatex(0,-0.8,"{}^{11}O_{gs}");
  text.DrawLatex(2.05,-0.8,"^{10}N+p");
  text.DrawLatex(4.4,-0.8,"^{9}C+2p");
    
  text.DrawLatex(4.5,6.5,"(b)");
  
  pad1->cd();

  TH2S *myframe2 = new TH2S("frame2","",10,-1,6.5,10,-0.5,7.0);

  myframe2->GetXaxis()->SetTickLength(0);
  myframe2->GetXaxis()->SetLabelSize(0);
  myframe2->GetXaxis()->SetAxisColor(0);

  myframe2->GetYaxis()->SetTitle("E_{T} [MeV]");
  myframe2->GetYaxis()->CenterTitle();

  myframe2->Draw();

  //text.SetTextSize(.035);
  text.SetTextAlign(10);
  Ex = 1.718;
  line.DrawLine(-0.2,Ex,1.25,Ex);
  boxit(-0.2,Ex-0.0255,1.25,Ex+0.0255,2);
  text.DrawLatex(1.27,Ex+textspacing,"0^{+}_{1}");

  Ex = 3.519;
  line.DrawLine(0.525,Ex,1.25,Ex);
  boxit(0.525,Ex-0.490,1.25,Ex+0.490,2);
  text.DrawLatex(1.27,Ex+2*textspacing,"0^{+}_{2}");
  
  Ex = 3.817;
  line.DrawLine(-0.2,Ex,0.525,Ex);
  boxit(-0.2,Ex-0.0775,0.525,Ex+0.0775,2);
  text.DrawLatex(-0.6,Ex+3*textspacing,"2^{+}_{1}");
  
  Ex = 6.493;
  line.DrawLine(-0.2,Ex,1.25,Ex);
  boxit(-0.2,Ex-0.377,1.25,Ex+0.377,2);
  text.DrawLatex(1.27,Ex+textspacing,"2^{+}_{2}");

  Ex = 1.316;
  line.DrawLine(2.0,Ex,3.45,Ex);
  boxit(2.0,Ex-0.415,3.45,Ex+0.415,2);
  text.DrawLatex(1.15,1.1,"1/2^{+}");

  Ex = 2.076;
  line.DrawLine(2.0,Ex,3.45,Ex);
  boxit(2.0,Ex-0.3,3.45,Ex+0.3,2);
  //boxit2(3.45,Ex+0.3,2.0,Ex-0.3,2);
  text.DrawLatex(3.45,2.2,"1/2^{-}");

  Ex = 3.506;
  line.DrawLine(2.0,Ex,3.45,Ex);
  boxit(2.0,Ex-0.25,3.45,Ex+0.25,2);
  text.DrawLatex(2.5,Ex-3.5*textspacing,"5/2^{+}");

  Ex = 0.;
  line.DrawLine(4.2,Ex,5.65,Ex);
  text.DrawLatex(5.7,Ex+textspacing,"0^{+}");

  Ex = 3.353;
  line.DrawLine(4.2,Ex,5.65,Ex);
  text.DrawLatex(5.7,Ex+textspacing,"2^{+}");

  arrow.DrawArrow(1.25,6.493,2.0,2.1);
  arrow.DrawArrow(1.25,6.493,4.2,3.4);
  arrow.DrawArrow(3.45,2.076,4.2,0.);
  text.DrawLatex(3.0,4.7,"?");

  text.SetTextSize(0.06);
  text.DrawLatex(0.,-0.8,"{}^{12}O");
  text.DrawLatex(2.05,-0.8,"^{11}N+p");
  text.DrawLatex(4.25,-0.8,"^{10}C+2p");
  text.DrawLatex(4.5,6.5,"(a)");

  return;

}
