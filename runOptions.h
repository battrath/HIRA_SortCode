#ifndef runOptions_
#define runOptions_

#include<iostream>
#include<fstream>
#include<string>

using namespace std;

struct runOptions {
  string invmap;	//S800 Inverse Mapfile
  string filepath;	//Data directory for .evt files
  string pidpath;	//PID gate directory
  string runfile;	//List of runs
  string caechip;	//CAESAR Chip map
  string caeqdc;	//CAESAR QDC-TDC map
  string caecal;	//CAESAR Calibration File
  string caepos;	//CAESAR Positions
  string pcal;		//CsI Proton Calibration
  string dcal;		//CsI Deuteron Calibration
  string acal;		//CsI Alpha Calibration
  string csitime;	//Si-CsI Time Gates
  string piemap;	//S4 Pie Chip map
  string piecal;	//S4 Pie Calibrations
  string ringmap;	//S4 Ring Chip map
  string ringcal;	//S4 Ring Calibrations
  string csimap;	//CsI Map
  string csitdcmap;	//CsI TDC Map
  string crdcped;	//CRDC Pedestal File
  string crdcpar;	//CRDC Charge Calibrations
  string crdccal;	//CRDC Calibrations
  string massfile;	//Mass File
  float target;		//Target thickness mg/cm2
  float fiber;		//Fiber thickness mg/cm2
  float brho;		//S800 BRHO
  float siDist;		//Si-Target Distance
  float alThick;	//Al plate thickness mg/cm2
  string sidet;		//Silicon type, S4 or HIRA Telescope
  float xact;	        //Active size of HIRA silicon X side
  float yact;	        //Active size of HIRA silicon Y side
  string telegeo;	//File containing HIRA telescope geometry
  int ntele;		//Number of telescopes
  int npie;		//Number of Si Pie per telescope
  int nring;		//Number of Si Rings per telescope
  int ncsi;		//Number of CsI per telescope
};
#endif
