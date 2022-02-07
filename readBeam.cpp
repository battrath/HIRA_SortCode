#include "readBeam.h"

/***********************************************************/
/* Constructor *********************************************/
/***********************************************************/
readBeam::readBeam() {
  //Finds Secondary Beam Files in Beam directory
  string directory = "Beam";
  DIR * pDIR;
  struct dirent * entry;
  if ((pDIR = opendir(directory.c_str()))) {
    while ((entry = readdir(pDIR))) {
      if (strstr(entry->d_name, ".dat")) {
        string file = entry->d_name;
        int num = file.find_last_of(".");
        file = file.substr(0,num);
        pb.push_back(file);
      }
    }
    closedir(pDIR);
  }
  //Reads in residuals from beam files
  for(unsigned int i = 0; i < pb.size(); i++) {
    char file[64];
    sprintf(file,"%s/%s.dat",directory.c_str(), pb.at(i).c_str());
    ifstream fp;
    fp.open(file);
    int tmpA;
    int tmpZ;
    while(fp.good()){
      fp >> tmpZ >> tmpA;
      if(fp.eof()) break;
      char iso[16];
      sprintf(iso,"%i%s",tmpA,isotope(tmpZ).c_str());
      sb[i].push_back(iso);
    }
    fp.close();
  }
}

/***********************************************************/
/* Destructor **********************************************/
/***********************************************************/
readBeam::~readBeam() {
  for(int i = 0; i < 10; i++) sb[i].clear();
  pb.clear();
}
/***********************************************************/
/* Finds beam index to fill histogram correctly ************/
/***********************************************************/
int readBeam::BeamID(int Z, int A, vector<string> beam) {
  char iso[16];
  for(unsigned int i = 0; i < beam.size(); i++){
    sprintf(iso,"%i%s",A,isotope(Z).c_str());
    if(beam.at(i) == iso) return i;
  }
  return -1;
}

/***********************************************************/
/* Gets atomic symbol for given Z **************************/
/***********************************************************/
string readBeam::isotope(int Z) {
  string ID;
  if(Z == 1) ID = "H";
  else if(Z == 2) ID = "He";
  else if(Z == 3) ID = "Li";
  else if(Z == 4) ID = "Be";
  else if(Z == 5) ID = "B";
  else if(Z == 6) ID = "C";
  else if(Z == 7) ID = "N";
  else if(Z == 8) ID = "O";
  else if(Z == 9) ID = "F";
  else if(Z == 10) ID = "Ne";
  else if(Z == 11) ID = "Na";
  else if(Z == 12) ID = "Mg";
  else if(Z == 13) ID = "Al";
  else if(Z == 14) ID = "Si";
  else if(Z == 15) ID = "P";
  else if(Z == 16) ID = "S";
  else if(Z == 17) ID = "Cl";
  else if(Z == 18) ID = "Ar";
  else if(Z == 19) ID = "K";
  else if(Z == 20) ID = "Ca";
  else if(Z == 21) ID = "Sc";
  else if(Z == 22) ID = "Ti";
  else ID = "V";
  return ID;
}

/***********************************************************/
/* Gets atomic number for given isotope ********************/
/***********************************************************/
int readBeam::GetZ(string A) {
  int ID;
  int size = A.size();
  int pos = A.find_last_of("0123456789");
  A = A.substr(pos+1);
  if(A == "H") ID = 1;
  else if(A == "He") ID = 2;
  else if(A == "Li") ID = 3;
  else if(A == "Be") ID = 4;
  else if(A == "B")  ID = 5;
  else if(A == "C")  ID = 6;
  else if(A == "N")  ID = 7;
  else if(A == "O")  ID = 8;
  else if(A == "F")  ID = 9;
  else if(A == "Ne") ID = 10;
  else if(A == "Na") ID = 11;
  else if(A == "Mg") ID = 12;
  else if(A == "Al") ID = 13;
  else if(A == "Si") ID = 14;
  else if(A == "P")  ID = 15;
  else if(A == "S")  ID = 16;
  else if(A == "Cl") ID = 17;
  else if(A == "Ar") ID = 18;
  else if(A == "K")  ID = 19;
  else if(A == "Ca") ID = 20;
  else if(A == "Sc") ID = 21;
  else if(A == "Ti") ID = 22;
  else if(A == "V")  ID = 23;
  else ID = -1;
  return ID;
}

/***********************************************************/
/* Gets mass number for given isotope **********************/
/***********************************************************/
int readBeam::GetA(string A) {
  int tmpA = stoi(A.substr(0,A.find_last_of("0123456789")+1));
  return tmpA;
}
