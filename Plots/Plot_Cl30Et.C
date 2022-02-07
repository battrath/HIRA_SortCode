void Plot_Cl30Et()
{



 gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);
  TStyle * Sty = (TStyle*)gROOT->FindObject("MyStyle");
  if(!Sty)
    {
      Sty = new TStyle("MyStyle","MyStyle");
    }
  Sty->SetOptTitle(0);
  Sty->SetOptStat(0);
  Sty->SetPalette(1,0);
  Sty->SetCanvasColor(10);
  Sty->SetCanvasBorderMode(0);
  Sty->SetFrameLineWidth(3);
  Sty->SetFrameFillColor(10);
  Sty->SetPadColor(10);
  Sty->SetPadTickX(1);
  Sty->SetPadTickY(1);
  Sty->SetPadBottomMargin(.17);
  Sty->SetPadTopMargin(.03);
  Sty->SetPadLeftMargin(.17);
  Sty->SetPadRightMargin(.05);
  Sty->SetHistLineWidth(3);
  Sty->SetHistLineColor(kBlack);
  Sty->SetFuncWidth(3);
  Sty->SetFuncColor(kRed);
  Sty->SetLineWidth(3);
  Sty->SetLabelSize(0.06,"xyz");
  Sty->SetLabelOffset(0.02,"y");
  Sty->SetLabelOffset(0.02,"x");
  Sty->SetLabelColor(kBlack,"xyz");
  Sty->SetTitleSize(0.06,"xyz");
  Sty->SetTitleOffset(1.35,"y");
  Sty->SetTitleOffset(1.1,"x");
  Sty->SetTitleFillColor(10);
  Sty->SetTitleTextColor(kBlack);
  Sty->SetTickLength(.03,"xz");
  Sty->SetTickLength(.02,"y");
  Sty->SetNdivisions(10,"x");
  Sty->SetNdivisions(10,"yz");
  Sty->SetEndErrorSize(10);
  gROOT->SetStyle("MyStyle");
  gROOT->ForceStyle();


  TFile *in = new TFile("../read.root");
  if(!in->IsOpen())
    {
      cout << "Did not open the file properly" << endl;
      return;
    }

  TCanvas *mycan = new TCanvas("mycan","mycan");


  TH1I * Cl = (TH1I*)in->Get("corr/Cl30/Erel_30Cl_p29S")->Clone("Cl");

  Cl->Draw();

  Cl->GetXaxis()->SetTitle("Erel [MeV]");
  Cl->GetXaxis()->CenterTitle();
  Cl->GetXaxis()->SetRangeUser(0,10);


  Cl->GetYaxis()->SetTitle("Counts");
  Cl->GetYaxis()->CenterTitle();

  TLatex *mytex = new TLatex();
  mytex->DrawLatexNDC(0.5,0.5,"^{30}Cl #rightarrow p+^{29}S");

  return;
  
}
