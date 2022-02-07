#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include <vector>

using namespace std;

int main()
{

  ifstream fin("Cpeaks/halfteles/tele11_Cpeaks_16to31.dat");
  ifstream cal_in("cal/tele_cals/tele11.cal");

  ofstream cal_out("cal/tele_cals/adjusted/tele11_adjusted_16to31.cal");

  std::vector<double> centroids;
  std::vector<int> strips;

  double average = 0.;
  double number_read =  0.;
  double ratio;

  int itele, istrip, j, starting_strip, max_strip;
  double centroid, distance;
  double slope, intercept;

  starting_strip = 16;
  max_strip = 31;

  while(fin  >> itele >> istrip >> centroid >> distance)
    {
      average += centroid;
      centroids.push_back(centroid);
      strips.push_back(istrip);
      number_read += 1.;
    }
  average = average/number_read;
  cout << average << endl;

  for (int i=0; i<(int)centroids.size(); i++)
    {
      ratio = centroids[i]/average;
      cout << ratio << endl;
    }

  j = 0;
 
  while(cal_in >> itele >> istrip >> slope >> intercept && starting_strip <= max_strip)
    {
      if (strips[j] == istrip)
	{
	  slope = slope/(centroids[j]/average);
	  j++;
	}
      else
	{
	  slope = slope;
	}
      if (istrip >= starting_strip)
	{
	  cal_out << itele << " " <<  istrip << " " << slope << " " << intercept << endl;
	  starting_strip++;
	}
    }
  cal_out.close();
}


