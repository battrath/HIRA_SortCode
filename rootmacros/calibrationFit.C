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
  //TCanvas canvas("twoFit","",600,800);
  TCanvas canvas("twoFit","",600,1066);
  //divide up canvas into three pads
  double overlap = .01;
  
  TPad *pad1  = new TPad("pad1","",0.,0.,1.,1./4.+0.07/2.);
  TPad *pad2  = new TPad("pad2","",0.,1./4.- 0.07/2.,1.,2./4.+0.0/2.);
  TPad *pad3  = new TPad("pad3","",0.,2./4.-overlap/2.,1.,3./4.+overlap/2.);
  TPad *pad4  = new TPad("pad4","",0.,3./4.-0.04,1.,1.);

  pad1->SetFillStyle(4000);
  pad2->SetFillStyle(4000);
  pad3->SetFillStyle(4000);
  pad4->SetFillStyle(4000);
  
  gPad->GetFrame()->SetLineWidth(1);
  pad1->Draw();
  pad2->Draw();
  pad3->Draw();
  pad4->Draw();
  
  
  
  pad1->cd();
  //gPad->SetLogy();
  //TH2S frame1("frame1","",10,12.3,14,10,0,7000);
  
  TH2S frame1("frame1","",275,1.,8.5,10,0,1500);
  frame1.GetXaxis()->SetTitle("E_{T} [MeV]");
  frame1.GetYaxis()->SetTitle("Counts");  
  frame1.GetXaxis()->CenterTitle();
  frame1.GetYaxis()->CenterTitle();
  frame1.GetXaxis()->SetLabelSize(0.08);
  frame1.GetYaxis()->SetLabelSize(0.08);
  frame1.SetNdivisions(510);
  frame1.Draw();
  
  TLatex tt;
  tt.SetNDC();
  tt.SetTextSize(0.1);

  TLatex text;
  text.SetNDC();
  text.SetTextSize(.1);

  //O12 low
  
  TFile fileX1("/home/Oxygen11/sortcode_addback/tree/O12/out.root");
  TH1I* hist1 = (TH1I*) fileX1.Get("Ex_trans");
  
  
  
  hist1->SetMarkerStyle(20);
  hist1->SetMarkerColor(1);
  hist1->SetLineColor(1);
  hist1->Sumw2();
  
  hist1->Draw("same PE");
  
  
  double one,two,three,four,five,six,seven;
  
  ifstream fileIn("/home/Oxygen11/sortcode_addback/tree/O12/doublet/fit/fit_low.dat");
 
  double x1[274];
  double ytot1[274];
  double yb1[274];
  double y1_1[274];
  double y2_1[274];
  double y3_1[274];
  double y4_1[274];
  

  double sum_peak_2plus2_gate = 0.;
  double sum_back_2plus2_gate = 0.;

  double sum_peak_2plus2_gate_tight = 0.;
  double sum_back_2plus2_gate_tight = 0.;



  double sum_peak1_2plus1_gate = 0.;
  double sum_peak2_2plus1_gate = 0.;
  double sum_back_2plus1_gate = 0.;


  int N1 = 0;
  for (;;)
    {
      fileIn >> one >> two >> three >> four >> five >> six >> seven;
      if (fileIn.eof()) break;
      if (fileIn.bad())break;
      x1[N1] = one;
      ytot1[N1] = two;
      yb1[N1] = three;
      y1_1[N1] = four;
      y2_1[N1] = five;
      y3_1[N1] = six;
      y4_1[N1] = seven;
      
      if (x1[N1] > 5.79 && x1[N1] < 6.99)
	{
          sum_peak_2plus2_gate += y4_1[N1];
          sum_back_2plus2_gate += yb1[N1];
	}



      if (x1[N1] > 6.23 && x1[N1] < 6.58)
	{
          sum_peak_2plus2_gate_tight += y4_1[N1];
          sum_back_2plus2_gate_tight += yb1[N1];
	}



      if (x1[N1] > 3.54 && x1[N1] < 3.98)
	{
          sum_peak1_2plus1_gate += y2_1[N1];
          sum_peak2_2plus1_gate += y3_1[N1];
          sum_back_2plus1_gate += yb1[N1];
	}


      
      N1++;
    }
  
  cout << "signal 2plus2 = " << sum_peak_2plus2_gate << " back 2plus2 = " << sum_back_2plus2_gate << endl;

  cout << "signal 2plus2 (tight)= " << sum_peak_2plus2_gate_tight << " back 2plus2(tight) = " << sum_back_2plus2_gate_tight << endl;



  cout << "signal 2plus1 peak1 = " << sum_peak1_2plus1_gate << 
" signal 2plus1 peak2 = " << sum_peak2_2plus1_gate << 
    " back " << sum_back_2plus1_gate << endl; 



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
  

  tt.DrawLatex(0.6,0.8,"(d)^{12}O#rightarrow2p+^{10}C");

  //O12 
  
  pad2->cd();
  //gPad->SetLogy();
  //TH2S frame1("frame1","",10,12.3,14,10,0,7000);
  
  TH2S frame2("frame1","",275,1.,8.5,10,0,1500);
  //frame2.GetXaxis()->SetTitle("E_{T} [MeV]");
  frame2.GetYaxis()->SetTitle("Counts");  
  frame2.GetXaxis()->CenterTitle();
  frame2.GetYaxis()->CenterTitle();
  frame2.GetXaxis()->SetLabelSize(0.0);
  frame2.GetYaxis()->SetLabelSize(0.08);
  frame2.SetNdivisions(510);
  frame2.Draw();
  

  TFile fileX2("/home/Oxygen11/sortcode_addback/tree/O12/out.root");
  TH1I* hist2 = (TH1I*) fileX2.Get("Ex_trans");
  
  
  
  hist2->SetMarkerStyle(20);
  hist2->SetMarkerColor(1);
  hist2->SetLineColor(1);
  hist2->Sumw2();
  
  hist2->Draw("same PE");
  
  

  
  fileIn.open("/home/Oxygen11/sortcode_addback/tree/O12/doublet/fit/fit_high.dat");
 
  double x2[274];
  double ytot2[274];
  double yb2[274];
  double y1_2[274];
  double y2_2[274];
  double y3_2[274];
  double y4_2[274];
  

  sum_peak_2plus2_gate = 0.;
  sum_back_2plus2_gate = 0.;

  sum_peak_2plus2_gate_tight = 0.;
  sum_back_2plus2_gate_tight = 0.;

  sum_peak1_2plus1_gate = 0.;
  sum_peak2_2plus1_gate = 0.;
  sum_back_2plus1_gate = 0.;

  int N2 = 0;
  for (;;)
    {
      fileIn >> one >> two >> three >> four >> five >> six >> seven;
      if (fileIn.eof()) break;
      if (fileIn.bad())break;
      x2[N2] = one;
      ytot2[N2] = two;
      yb2[N2] = three;
      y1_2[N2] = four;
      y2_2[N2] = five;
      y3_2[N2] = six;
      y4_2[N2] = seven;
      

      if (x2[N2] > 5.79 && x2[N2] < 6.99)
	{
          sum_peak_2plus2_gate += y4_2[N2];
          sum_back_2plus2_gate += yb2[N2];
	}



      if (x2[N2] > 6.23 && x2[N2] < 6.58)
	{
          sum_peak_2plus2_gate_tight += y4_2[N2];
          sum_back_2plus2_gate_tight += yb2[N2];
	}

     
      if (x2[N2] > 3.54 && x2[N2] < 3.98)
	{
          sum_peak1_2plus1_gate += y2_2[N2];
          sum_peak2_2plus1_gate += y3_2[N2];
          sum_back_2plus1_gate += yb2[N2];
	}
 
      N2++;
    }
  

  cout << "signal 2plus2 = " << sum_peak_2plus2_gate << " back 2plus2 = " << sum_back_2plus2_gate << endl;

  cout << "signal 2plus2 (tight)= " << sum_peak_2plus2_gate_tight << " back 2plus2(tight) = " << sum_back_2plus2_gate_tight << endl;


  cout << "signal 2plus1 peak1 = " << sum_peak1_2plus1_gate << 
" signal 2plus1 peak2 = " << sum_peak2_2plus1_gate << 
    " back " << sum_back_2plus1_gate << endl; 


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
  
  
  
  TGraph g1_2(N2,x2,y1_2);
  g1_2.SetLineColor(3);
  g1_2.SetLineWidth(2);
  g1_2.SetLineStyle(1);
  g1_2.Draw("C");
  
  TGraph g2_2(N2,x2,y2_2);
  g2_2.SetLineColor(3);
  g2_2.SetLineWidth(2);
  g2_2.SetLineStyle(1);
  g2_2.Draw("C");
  
  TGraph g3_2(N2,x2,y3_2);
  g3_2.SetLineColor(3);
  g3_2.SetLineWidth(2);
  g3_2.SetLineStyle(1);
  g3_2.Draw("C");
  
  
  TGraph g4_2(N2,x2,y4_2);
  g4_2.SetLineColor(3);
  g4_2.SetLineWidth(2);
  g4_2.SetLineStyle(1);
  g4_2.Draw("C");
  
  
  tt.DrawLatex(0.6,0.8,"(c)^{12}O#rightarrow2p+^{10}C");
  
  text.DrawLatex(0.22,0.85,"0^{+}_{1}");
  text.DrawLatex(0.28,0.53,"(0^{+}_{2},2^{+}_{2})");
  text.DrawLatex(0.50,0.52,"2^{+}_{1}");
  text.DrawLatex(0.72,0.60,"2^{+}_{2}");

  //N12
  
  pad3->cd();
  
  //gPad->SetLogy();
  
  
  TH2S frame3("frame3","",10,0.82,1.43,10,0,42000);
  frame3.SetStats(kFALSE);
  frame3.GetXaxis()->SetTitle("E* [MeV]");
  frame3.GetXaxis()->SetLabelSize(0.085);
  frame3.GetYaxis()->SetLabelSize(0.085);
  frame3.GetYaxis()->SetTitle("Counts");  
  frame3.GetXaxis()->CenterTitle();
  frame3.GetYaxis()->CenterTitle();
  frame3.SetNdivisions(510);
  frame3.Draw();
  
  
  
  
  TFile fileX3("/home/Oxygen11/sortcode_addback/tree/N12/pp10B/wood/out.root");
  TH1I* hist3 = (TH1I*) fileX3.Get("Ex_trans");
  
  
  hist3->SetMarkerStyle(20);
  hist3->SetMarkerColor(1);
  hist3->SetLineColor(1);
  hist3->Sumw2();
  
  
  hist3->Draw("same PE");
  
  
  
  fileIn.open("/home/Oxygen11/sortcode_addback/tree/N12/fit/fit.dat");
  
  double x3[92];
  double ytot3[92];
  double yb3[92];
  
  int N3 = 0;
  for (;;)
    {
      fileIn >> one >> two >> three;
      
      
      if (fileIn.eof()) break;
      if (fileIn.bad())break;
      x3[N3] = one;
      ytot3[N3] = two;
      yb3[N3] = three;
      
      
      N3++;
      if (N3 == 399) break;
    }
  
  cout << N3 << endl;
  fileIn.close();
  fileIn.clear();
  TGraph gtot3(N3,x3,ytot3);
  gtot3.SetLineColor(2);
  gtot3.SetLineWidth(2);
  gtot3.Draw("C");
  
  
  TGraph gb3(N3,x3,yb3);
  gb3.SetLineColor(4);
  gb3.SetLineWidth(2);
  gb3.SetLineStyle(9);
  gb3.Draw("C");
 
  tt.SetTextSize(0.11);
  text.SetTextSize(0.11);
  tt.DrawLatex(0.6,0.8,"(b)^{12}N#rightarrowp+^{11}C");
 
  text.DrawLatex(0.40,0.75,"2^{+}");
  text.DrawLatex(0.63,0.4,"2^{-}");
  
  tt.SetTextSize(0.1);
  text.SetTextSize(0.1);

  //O13
  pad4->cd();
  
  TH2S frame4("frame4","",275,2.7,4.2,10,0,20000);
  frame4.SetStats(kFALSE);
  //frame4GetXaxis()->SetTitle("E* [MeV]");
  frame4.GetXaxis()->SetLabelSize(0.08);
  frame4.GetYaxis()->SetLabelSize(0.08);
  frame4.GetYaxis()->SetTitle("Counts");  
  frame4.GetXaxis()->CenterTitle();
  frame4.GetYaxis()->CenterTitle();
  frame4.SetNdivisions(510);
  frame4.Draw();
  
  
  
  
  TFile fileX4("/home/Oxygen11/sortcode_addback/tree/O13/out.root");
  TH1I* hist4 = (TH1I*) fileX4.Get("Ex_trans");
  
  
  hist4->SetMarkerStyle(20);
  hist4->SetMarkerColor(1);
  hist4->SetLineColor(1);
  hist4->Sumw2();
  hist4->SetLabelOffset(0.);

  hist4->Draw("same PE");
  
  
  fileIn.open("/home/Oxygen11/sortcode_addback/tree/O13/fit/fit.dat");
  
  double x4[55];
  double ytot4[55];
  double yb4[55];
  
  
  int N4 = 0;
  for (;;)
    {
      fileIn >> one >> two >> three;
      
      
      if (fileIn.eof()) break;
      if (fileIn.bad())break;
      x4[N4] = one;
      ytot4[N4] = two;
      yb4[N4] = three;
      
      N4++;
      if (N4 == 399) break;
    }
  
  cout << N4 << endl;
  fileIn.close();
  fileIn.clear();
  TGraph gtot4(N4,x4,ytot4);
  gtot4.SetLineColor(2);
  gtot4.SetLineWidth(2);
  gtot4.Draw("C");
  
  
  TGraph gb4(N4,x4,yb4);
  gb4.SetLineColor(4);
  gb4.SetLineWidth(2);
  gb4.SetLineStyle(9);
  gb4.Draw("C");
  
  tt.DrawLatex(0.2,0.8,"(a)^{13}O#rightarrow2p+^{11}C");
  

} 
