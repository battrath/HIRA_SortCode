#include <iostream>
#include <fstream>
#include <string>

int main()
{
  ifstream fin("Pies_old.cal");

  float slope[128],intercept[128];

  int one,two,five;
  float three, four;
  for (int i=0;i<128;i++)
    {
      fin >> one >> two >> three >> four;
      
      slope[i] = three;
      intercept[i] = four;
      /*
      cout << three << " " << four << endl; 
      */
    }


  cout << slope[0] << " " << intercept[0] << endl;
  
  ofstream fout ("Pies_new.cal");
  ifstream fmap("/user/e16001/unpacker/datfiles/S4Pies.txt");
  string name;
  getline(fmap,name);
  cout << name << endl;

  if (fmap.is_open()) cout << " open" << endl;
  else abort();


  cout << one << " " <<two << " " << three << endl;
  for (int i=0;i<128;i++)
    {
      fmap >> one >> two >> five;
      if (five == 0) cout << i << endl;
      

      
	fout << 0 << " " << i << " " << slope[five] << " " << intercept[five] << endl;
    }
  
}
