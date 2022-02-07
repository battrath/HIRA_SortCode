{
  TFile file("read_O13.root");
  TH1I* really = (TH1I*) file.Get("corr/O11/Erel11O_2p9C");
  TH1I* bad = (TH1I*) file.Get("corr/O11/Erel11O_2p9C_Fake");
  bad->Scale(.1);

  TCanvas canvas;
  canvas.Divide(1,2);
  canvas.cd(1);
  really->Draw();
  bad->SetLineColor(2);
  bad->Draw("same");

  
  canvas.cd(2);
  TH2S frame ("frame","",10,0,15,10,-2,30);
  frame.Draw();
  TH1I * final = (TH1I*) really->Clone();
  final->Add(bad,-1.);
  final->Draw("same");
  TLine ll;
  ll.SetLineStyle(2);
  ll.DrawLine(0.,0.,15,0.);
  
}
