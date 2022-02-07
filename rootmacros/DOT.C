void DOT(){
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
  Sty->SetLabelSize(0.045,"xyz");
  Sty->SetLabelOffset(0.0,"y");
  Sty->SetLabelOffset(0.0,"x");
  Sty->SetLabelColor(kBlack,"xyz");
  Sty->SetTitleSize(0.055,"xyz");
  Sty->SetTitleOffset(0.45,"y");
  Sty->SetTitleOffset(0.45,"x");
  Sty->SetTitleFillColor(10);
  Sty->SetTitleTextColor(kBlack);
  Sty->SetTickLength(.05,"xz");
  Sty->SetTickLength(.025,"y");
  Sty->SetNdivisions(10,"xyz");
  Sty->SetEndErrorSize(0);
   
  gROOT->SetStyle("MyStyle");
  gROOT->ForceStyle();
  TCanvas *canvas = new TCanvas("mycan","mycan",1600,1800);

  double overlap = .07;

  TPad *pad1  = new TPad("pad1","",0.,overlap/2.,1./2.+overlap/2.,1./2.+overlap/2.);
  TPad *pad2  = new TPad("pad2","",1./2.-overlap/2.,overlap/2.,1.,1/2.+overlap/2.);
  TPad *pad3  = new TPad("pad3","",0.,1./2.-overlap/2.,1/2.+overlap/2.,1.);
  TPad *pad4  = new TPad("pad4","",1./2.-overlap/2.,1./2.-overlap/2.,1.,1.);

  pad1->SetFillStyle(4000);
  pad2->SetFillStyle(4000);
  pad3->SetFillStyle(4000);
  pad4->SetFillStyle(4000);
  
  pad1->SetLogz();
  pad2->SetLogz();
  pad3->SetLogz();
  pad4->SetLogz();

  gPad->GetFrame()->SetLineWidth(1);
  pad1->Draw();
  pad2->Draw();
  pad3->Draw();
  pad4->Draw();

  TLatex tt;
  tt.SetNDC();
  tt.SetTextSize(0.07);
  

  TFile *file = new TFile("dot.root");
  TFile *out = new TFile("dot_fig.root","RECREATE");

  ostringstream outstring;
  string name;

  pad3->cd();

  outstring.str("");
  outstring << "rr2D_mean";
  name = outstring.str();
  TH2I * rr2Dmean = (TH2I*)file->Get(name.c_str())->Clone();
  rr2Dmean->GetXaxis()->SetLabelSize(0.);
  rr2Dmean->GetXaxis()->SetTitleSize(0.);
  rr2Dmean->Draw("colz");
  tt.DrawLatex(0.6,0.8,"(a)");

  pad4->cd();

  outstring.str("");
  outstring << "rr2D_contrast";
  name = outstring.str();
  TH2I * rr2Dcontrast = (TH2I*)file->Get(name.c_str())->Clone();
  rr2Dcontrast->GetXaxis()->SetLabelSize(0.);
  rr2Dcontrast->GetYaxis()->SetLabelSize(0.);
  rr2Dcontrast->GetXaxis()->SetTitleSize(0.);
  rr2Dcontrast->GetYaxis()->SetTitleSize(0.);
  rr2Dcontrast->Draw("colz");
  tt.DrawLatex(0.6,0.8,"(b)");

  pad1->cd();

  outstring.str("");
  outstring << "rr2D_lower";
  name = outstring.str();
  TH2I * rr2Dlower = (TH2I*)file->Get(name.c_str())->Clone();
  rr2Dlower->GetXaxis()->SetTitle("R_{Dx} (cm)");
  rr2Dlower->Draw("colz");
  tt.DrawLatex(0.6,0.8,"(c)");

  pad2->cd();

  outstring.str("");
  outstring << "rr2D_upper";
  name = outstring.str();
  TH2I * rr2Dupper = (TH2I*)file->Get(name.c_str())->Clone();
  rr2Dupper->GetYaxis()->SetLabelSize(0.);
  rr2Dupper->GetYaxis()->SetTitleSize(0.);
  rr2Dupper->GetXaxis()->SetTitle("R_{Dx} (cm)");
  rr2Dupper->Draw("colz");
  tt.DrawLatex(0.6,0.8,"(d)");

  canvas->Update();
  canvas->Write();

  file->Close();
  out->Close();
}
