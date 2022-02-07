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
  Sty->SetLabelOffset(0.01,"x");
  Sty->SetLabelColor(kBlack,"xyz");
  Sty->SetTitleSize(0.1,"xyz");
  Sty->SetTitleOffset(0.85,"y");
  Sty->SetTitleOffset(1.,"x");
  Sty->SetTitleFillColor(10);
  Sty->SetTitleTextColor(kBlack);
  Sty->SetTickLength(.05,"xz");
  Sty->SetTickLength(.025,"y");
  Sty->SetNdivisions(2,"xyz");
  Sty->SetEndErrorSize(0);
  Sty->SetErrorX(0.00001);
  
  gROOT->SetStyle("MyStyle");
  gROOT->ForceStyle();
  
  TArrow arrow;
  //TCanvas canvas("twoFit","",800,800);
  TCanvas canvas("twoFit","",900,1066);
  //divide up canvas into three pads
  double overlap = .14;

  TFile *out = new TFile("o11composite.root","RECREATE");
  
  TPad *pad1  = new TPad("pad1","",0.,0.,1.,1./2.+overlap/2.);
  TPad *pad2  = new TPad("pad2","",0.,1./2.- overlap/2.,1.,1.);

  pad1->SetFillStyle(4000);
  pad2->SetFillStyle(4000);

  gPad->GetFrame()->SetLineWidth(1);
  pad1->Draw();
  pad2->Draw();

  
  
  pad1->cd();
  //gPad->SetLogy();
  //TH2S frame1("frame1","",10,12.3,14,10,0,7000);
  
  TH2S frame1("frame1","",275,1.,11.,9,0,750);
  frame1.GetXaxis()->SetTitle("E_{T} [MeV]");
  frame1.GetYaxis()->SetTitle("Counts");  
  frame1.GetXaxis()->CenterTitle();
  frame1.GetYaxis()->CenterTitle();
  frame1.GetXaxis()->SetLabelSize(0.08);
  frame1.GetYaxis()->SetLabelSize(0.08);
  frame1.SetNdivisions(5);
  frame1.Draw();
  
  TLatex tt;
  tt.SetNDC();
  tt.SetTextSize(0.08);



  //O12 low
  
  TFile fileX1("/home/Oxygen11/sortcode_addback/tree/O11/out2.root");
  TH1I* hist1 = (TH1I*) fileX1.Get("Ex");
  
 
  
  hist1->SetMarkerStyle(20);
  hist1->SetMarkerColor(1);
  hist1->SetLineColor(1);
  hist1->Sumw2();
  
  hist1->Draw("same PE");
  
  
  double one,two,three,four,five,six,seven;
  
  ifstream fileIn("/home/Oxygen11/sortcode_addback/tree/O11/fit/fit.dat");
 
  double x1[159];
  double ytot1[159];
  double yb1[159];
 
  
  int N1 = 0;
  for (;;)
    {
      fileIn >> one >> two >> three;
      if (fileIn.eof()) break;
      if (fileIn.bad())break;
      x1[N1] = one;
      ytot1[N1] = two;
      yb1[N1] = three;
      
      
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
  
  
 

  tt.DrawLatex(0.8,0.8,"(b)");

  //O12 
  
  pad2->cd();
 
  double norm1 = -0.0045;
  bool worked;

  TFile fileX2("completesort.root");
  TH1I* raw = (TH1I*)fileX2.Get("corr/O11/stripcuts/Erel11O_2p9C_stripcuts");
  TH1I* o12fake = (TH1I*)fileX2.Get("corr/O11/stripcuts/Erel11O_2p9C_Fake_CsIreaction_stripcuts");
  TH1I* o13fake = (TH1I*)fileX2.Get("corr/O11/stripcuts/Erel11O_2p9C_Fake_CsIreaction_C11_stripcuts");


  cout << worked << endl;

  raw->SetStats(kFALSE);
  raw->GetXaxis()->SetRangeUser(1,11);
  raw->GetXaxis()->SetLabelSize(0.0);
  raw->GetYaxis()->SetRangeUser(0,800);
  raw->SetLineColor(kBlack);
  //raw->SetLineWidth(2);
  raw->SetFillStyle(3001);
  raw->Draw();
 
  raw->GetYaxis()->SetTitle("Counts");

  raw->GetYaxis()->CenterTitle();

  o12fake->SetLineColor(kRed);
  o12fake->Scale((-1.)*norm1);
  //o12fake->SetLineWidth(2);
  o12fake->SetStats(kFALSE);
  o12fake->Draw("same");

  o13fake->SetLineColor(kBlue);
  o13fake->Scale((-1.)*norm1);
  //o13fake->SetLineWidth(2);
  o13fake->SetStats(kFALSE);
  o13fake->Draw("same");

  tt.DrawLatex(0.8,0.8,"(a)");

  tt.SetTextSize(0.05);
  tt.DrawLatex(0.6,0.7,"Raw ^{11}O Spectrum");
  tt.SetTextColor(2);
  tt.DrawLatex(0.62,0.64,"^{13}O Background");
  tt.SetTextColor(4);
  tt.DrawLatex(0.62,0.58,"^{12}O Background");

} 
