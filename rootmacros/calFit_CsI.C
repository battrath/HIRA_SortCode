/***************************************************
Click on either side of two peaks (smaller peak first) to fit to gaussian and calculate rough calibration
***************************************************/


void calFit_CsI()
{
  TFile file("../files/H_Be.root");

  ofstream centroids("CsI_centroids.dat");
  

  ostringstream outstring;
  string name;

  TCanvas *mycan = new TCanvas("mycan","mycan",1600,900);
  mycan->SetCrosshair(1);
  mycan->ToggleEventStatus();
  
  int NCsI = 20;

  double energy1, energy2;

  for (int icsi=0;icsi<NCsI;icsi++)
    {
      if (1)//icsi == 25 || icsi == 26 || icsi == 28 || icsi == 31)
	{
      
	  double par[3];
	  double peak1;
		  
	  
	  outstring.str("");
	  outstring<< "CsI/CsIRaw/ECsI_"<<icsi;
	  name = outstring.str();
	  cout << name << endl;
	  TH1I * hist = (TH1I*)file.Get(name.c_str());
	  hist->GetXaxis()->SetRangeUser(200,700);
	  
	  hist->Draw();
	  mycan->Modified();
	  mycan->Update();
	  
	  
	  
	  TMarker * mark;
	  double low1,high1;
	  
	  mark = (TMarker*) mycan->WaitPrimitive("TMarker");
	  low1 = mark->GetX();
	  delete mark;
	  mark = (TMarker*) mycan->WaitPrimitive("TMarker");
	  high1 = mark->GetX();
	  delete mark;
	  
	  
	  TF1 *g1 = new TF1("g1","gaus",low1,high1);

	  
	  
	  hist->Fit(g1,"R");
	  
	  
	  g1->GetParameters(&par[0]);
		  
	  peak1 = par[1];
	  
	  centroids << icsi << " " << peak1 << endl;

	  mark = (TMarker*) mycan->WaitPrimitive("TMarker");
	  
	  delete g1;

	  
	  delete hist;
	}
      else
	{
	  continue;
	}
    }
  
    
  
  centroids.close();

  return;
}
