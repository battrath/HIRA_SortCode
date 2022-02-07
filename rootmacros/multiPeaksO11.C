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
  
  TH2S frame1("frame1","",275,1.,11.,9,0,1450);
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
  
  ifstream fileIn("/home/Oxygen11/sortcode_addback/tree/O11/fit_twostates/fit.dat");
 
  double x1[159];
  double ytot1[159];
  double yb1[159];
  double peak1[159];
  double peak2[159];
  double peak3[159];
  double peak4[159];
 
  
  int N1 = 0;
  for (;;)
    {
      fileIn >> one >> two >> three >> four >> five;
      if (fileIn.eof()) break;
      if (fileIn.bad())break;
      x1[N1] = one;
      ytot1[N1] = two;
      yb1[N1] = three;
      peak1[N1] = four;
      peak2[N1] = five;
      
      N1++;
    }
  
  fileIn.close();
  fileIn.clear();
  TGraph gtot1(N1,x1,ytot1);
  gtot1.SetLineColor(2);
  gtot1.SetLineWidth(2);
  gtot1.Draw("C");
  
  TGraph g11(N1,x1,peak1);
  g11.SetLineColor(3);
  g11.SetLineWidth(2);
  g11.Draw("L");

  TGraph g21(N1,x1,peak2);
  g21.SetLineColor(3);
  g21.SetLineWidth(2);
  g21.Draw("L");
  
  TGraph gb1(N1,x1,yb1);
  gb1.SetLineColor(4);
  gb1.SetLineWidth(2);
  gb1.SetLineStyle(9);
  gb1.Draw("C");

 


  tt.DrawLatex(0.8,0.8,"(b)");

  pad2->cd();
  //gPad->SetLogy();
  //TH2S frame1("frame1","",10,12.3,14,10,0,7000);
  
  TH2S frame2("frame1","",275,1.,11.,9,0,1450);
  
  frame2.GetYaxis()->SetTitle("Counts");  
  frame2.GetXaxis()->CenterTitle();
  frame2.GetYaxis()->CenterTitle();
  frame2.GetXaxis()->SetLabelSize(0.08);
  frame2.GetYaxis()->SetLabelSize(0.08);
  frame2.SetNdivisions(5);
  frame2.Draw();
  




  //O12 low
  

  TH1I* hist2 = (TH1I*) fileX1.Get("Ex");
  
 
  
  hist2->SetMarkerStyle(20);
  hist2->SetMarkerColor(1);
  hist2->SetLineColor(1);
  hist2->Sumw2();
  
  hist2->Draw("same PE");
  
  
  ifstream fileIn2("/home/Oxygen11/sortcode_addback/tree/O11/fit_fourstates/fit.dat");
 

  
  N1 = 0;
  for (;;)
    {
      fileIn2 >> one >> two >> three >> four >> five >> six >> seven;
      if (fileIn2.eof()) break;
      if (fileIn2.bad())break;
      x1[N1] = one;
      ytot1[N1] = two;
      yb1[N1] = three;
      peak1[N1] = four;
      peak2[N1] = five;
      peak3[N1] = six;
      peak4[N1] = seven;

      N1++;
    }
  
  fileIn.close();
  fileIn.clear();
  TGraph gtot2(N1,x1,ytot1);
  gtot2.SetLineColor(2);
  gtot2.SetLineWidth(2);
  gtot2.Draw("C");
  
  TGraph g12(N1,x1,peak1);
  g12.SetLineColor(3);
  g12.SetLineWidth(2);
  g12.Draw("L");

  TGraph g22(N1,x1,peak2);
  g22.SetLineColor(3);
  g22.SetLineWidth(2);
  g22.Draw("L");

  TGraph g32(N1,x1,peak3);
  g32.SetLineColor(3);
  g32.SetLineWidth(2);
  g32.Draw("L");

  TGraph g42(N1,x1,peak4);
  g42.SetLineColor(3);
  g42.SetLineWidth(2);
  g42.Draw("L");
  
  TGraph gb2(N1,x1,yb1);
  gb2.SetLineColor(4);
  gb2.SetLineWidth(2);
  gb2.SetLineStyle(9);
  gb2.Draw("C");

 


  tt.DrawLatex(0.8,0.8,"(a)");


} 
