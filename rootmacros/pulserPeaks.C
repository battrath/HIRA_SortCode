/***************************************************
Click on either side of pulser peaks to find the channel value of the peak's center
***************************************************/


void pulserPeaks()
{
  TFile file("sort.root");

  //ofstream ofile ("onestrip.dat");
  ofstream ofile("backpeaks_lo_13.dat"); //for if you're doing a front
  //ofstream ofile("tele13backpeaks_lo.dat"); //for if you're doing a back

  ostringstream outstring;
  string name;

  int Nstrip = 32;
  int Npeaks;
  int range;
  int peak_num;
  int Nbins;
  int i;

  double peak_height;
  double bin_width;

  //int ramp_points = 18; //for back
  int ramp_points = 21; //for fronts
  //ramp_points = 20; //for fronts on 6-8, where we missed the lowest voltage on the ramp
	  
  double peaks[ramp_points];



  double voltages[ramp_points];

  double lowest_voltage = 0.1;
  double highest_voltage = 6.5; //for fronts

  double stepsize = (highest_voltage - lowest_voltage)/(double)(ramp_points - 1);
  //lowest_voltage = lowest_voltage + stepsize;

  for(i=0;i<ramp_points;i++)
    {
      peaks[i] = 0;
      voltages[i] = lowest_voltage + i*stepsize;
    }




  TCanvas *mycan = new TCanvas("mycan","mycan",1800,950);
  //TSpectrum *peakfinder = new TSpectrum();
  mycan->SetCrosshair(1);
  mycan->ToggleEventStatus();
  
  for (int itele=13;itele<14;itele++)
    {
      for (int istrip=0;istrip<Nstrip;istrip++)
	{

	  outstring.str("");
	  //outstring<< "back/raw/EB"<<itele<<"_"<<istrip;
	  outstring<< "back/lgr/EBLG"<<itele<<"_"<<istrip;
	  name = outstring.str();
          TH1I * hist = (TH1I*)file.Get(name.c_str());
	  hist->GetXaxis()->SetRangeUser(0,range);
	  // Nbins = hist->GetXaxis()->GetNbins();
	  // bin_width = hist->GetXaxis()->GetBinWidth(2);
          hist->Draw();
          mycan->Modified();
 	  mycan->Update();

	  // peak_num = 0;

	  // 	  i = 1;

//  	  while (i<Nbins){
//  	    peak_height = hist->GetBinContent(i);
//  	    if (peak_height > 25)
//  	      {
//  		hist->GetXaxis()->SetRangeUser(bin_width*(i-10),bin_width*(i+10));
//  		peaks[peak_num] = hist->GetMean(1);
//  		peak_num++;
 
//  		hist->GetXaxis()->SetRangeUser(0,range);
//  		i = i+10;
//  	      }
//  	    i++;
//  	  }
	  
	  
	  
	  TMarker * mark;
	  double low[ramp_points];
	  double high[ramp_points];
	  //Npeaks = peakfinder->Search(hist,.05,"",0.1);
	  //ofile << Npeaks << endl;
	  for (int i = 0; i<ramp_points; i++)
	    {
	      mark = (TMarker*) mycan->WaitPrimitive("TMarker");
	      low[i] = mark->GetX();
	      delete mark;
	      mark = (TMarker*) mycan->WaitPrimitive("TMarker");
	      high[i] = mark->GetX();
	      delete mark;
	    }
	  for (int j = 0; j<ramp_points; j++)
	    {
	      hist->GetXaxis()->SetRangeUser(low[j],high[j]);
	      peaks[j] = hist->GetMean(1);
	      ofile << itele << " " << istrip << " " << peaks[j] << " " << voltages[j] << endl;
	    }
	  
	  delete hist;
	}
    }
  
  ofile.close(); 
  return;
}
