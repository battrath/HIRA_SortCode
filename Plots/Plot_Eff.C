void Plot_Eff()
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
  Sty->SetNdivisions(5,"x");
  Sty->SetNdivisions(10,"yz");
  Sty->SetEndErrorSize(10);
  gROOT->SetStyle("MyStyle");
  gROOT->ForceStyle();


  TFile *in = new TFile("../sort.root");
  if(!in->IsOpen())
    {
      cout << "Did not open the file properly" << endl;
      return;
    }

  TCanvas * mycan = new TCanvas("mycan","",800,800);
  mycan->Divide(2,2,0.000001,0.0000001);

  TLatex * mytex = new TLatex();

  
  mycan->cd(1);

  TH1I *total = (TH1I*)in->Get("Fiber/CornerMult");
  total->Draw();
  total->GetXaxis()->SetTitle("CornerMultiplicity");
  total->GetXaxis()->CenterTitle();

  float Ntotal = (float)total->GetEntries();
  float Teff = total->GetBinContent(9)/Ntotal*100.;
  mytex->DrawLatexNDC(0.25,0.5,Form("Total Eff = %.2f %%",Teff));
  
  
  mycan->cd(3);
  TH1I *vert = (TH1I*)in->Get("Fiber/CornerMult_Vert");
  vert->Draw();
  vert->GetXaxis()->SetTitle("CornerMultiplicity");
  vert->GetXaxis()->CenterTitle();

  float Veff = vert->GetBinContent(5)/Ntotal*100.;
  mytex->DrawLatexNDC(0.25,0.5,Form("Vertical Eff = %.2f %%",Veff));
  
  mycan->cd(4);
  TH1I *hori = (TH1I*)in->Get("Fiber/CornerMult_Hori");
  hori->Draw();
  hori->GetXaxis()->SetTitle("CornerMultiplicity");
  hori->GetXaxis()->CenterTitle();

  float Heff = hori->GetBinContent(5)/Ntotal*100.;
  mytex->DrawLatexNDC(0.25,0.5,Form("Horizontal Eff = %.2f %%",Heff));
  

  mycan->cd(2);


  TH2I * VvsH = (TH2I*)in->Get("Fiber/CornerMult_VvsH");
  VvsH->Draw("col text");
  VvsH->SetMarkerSize(2.0);
  VvsH->GetXaxis()->SetTitle("Horizontal Multiplicity");
  VvsH->GetXaxis()->CenterTitle();
  VvsH->GetYaxis()->SetTitle("Vertical Multiplicity");
  VvsH->GetYaxis()->CenterTitle();
				
				
  

  return;
}
