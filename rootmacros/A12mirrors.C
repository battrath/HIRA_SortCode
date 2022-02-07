void A12mirrors(){
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
  Sty->SetLabelSize(0.06,"xyz");
  Sty->SetLabelOffset(0.01,"y");
  Sty->SetLabelOffset(0.02,"x");
  Sty->SetLabelColor(kBlack,"xyz");
  Sty->SetTitleSize(0.05,"xyz");
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
  TH2S *myframe = new TH2S("frame","",10,-1,5,10,-2.,6.0);

  myframe->GetXaxis()->SetTickLength(0);
  myframe->GetXaxis()->SetLabelSize(0);
  myframe->GetXaxis()->SetAxisColor(0);

  myframe->GetYaxis()->SetTitle("E^{*} [MeV]");
  myframe->GetYaxis()->CenterTitle();

  myframe->Draw();

  TLine line;
  line.SetLineWidth(7);

  TAttFill fill;
  fill.SetFillStyle(3004);

  TLatex text;
  text.SetTextSize(.035);
  text.SetTextAlign(10);

  float textspacing;
  textspacing = -0.1;

  float Ex = 0.;
  line.DrawLine(-0.2,Ex,1.25,Ex);
  boxit(-0.2,Ex-0.0255,1.25,Ex+0.0255,2);
  text.DrawLatex(1.27,Ex+textspacing,"0^{+}_{1}");

  Ex = 1.801;
  line.DrawLine(-0.2,Ex,0.725,Ex);
  boxit(-0.2,Ex-0.490,0.725,Ex+0.490,2);
  text.DrawLatex(0.727,Ex+2*textspacing,"0^{+}_{2}");
  
  Ex = 2.099;
  line.DrawLine(0.725,Ex,1.25,Ex);
  boxit(0.725,Ex-0.0775,1.25,Ex+0.0775,2);
  text.DrawLatex(1.27,Ex,"2^{+}_{1}");
  
  Ex = 4.775;
  line.DrawLine(-0.2,Ex,1.25,Ex);
  boxit(-0.2,Ex-0.377,1.25,Ex+0.377,2);
  text.DrawLatex(1.27,Ex+textspacing,"2^{+}_{2}");

  Ex = 0.;
  line.DrawLine(2.9,Ex,4.25,Ex);
  text.DrawLatex(4.27,Ex+textspacing,"0^{+}_{1}");

  Ex = 2.25;
  line.DrawLine(2.9,Ex,4.25,Ex);
  text.DrawLatex(4.27,Ex,"0^{+}_{2}");
  
  Ex = 2.11;
  line.DrawLine(2.9,Ex,4.25,Ex);
  boxit(2.9,Ex-0.0535,4.25,Ex+0.0535,2);
  text.DrawLatex(4.27,Ex+2*textspacing,"2^{+}_{1}");

  Ex = 4.414;
  line.DrawLine(2.9,Ex,4.25,Ex);
  boxit(2.9,Ex-0.317,4.25,Ex+0.317,2);
  text.DrawLatex(4.27,Ex+textspacing,"2^{+}_{2}");

  text.SetTextSize(0.05);
  text.DrawLatex(0.,-0.5,"{}^{12}O");
  text.DrawLatex(3.,-0.5,"{}^{12}Be");

  line.SetLineStyle(6);
  line.SetLineWidth(4);
  line.SetLineColor(2);

  Ex = -1.718;
  line.DrawLine(-0.5,Ex,1.55,Ex);

  Ex = 3.3171;
  line.DrawLine(2.6,Ex,4.55,Ex);

  return;

}
