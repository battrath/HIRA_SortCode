// C++ file to read MSU event files
//file numbers.beam contains runs to sort
//uses class hira to unpack hira data
//write out spectra in file sort.root

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "det.h"
#include <ctime>
#include "histo_sort.h"
#include "histo_read.h"
#include "readBeam.h"
#include "readMass.h"
#include "runOptions.h"
//#include "forest.h"
#include "TString.h"
using namespace std;

Long64_t GetTimeStamp(vector<int> time_stamp) {

  Long64_t time = time_stamp.at(0);
  int shift[4] = {0, 16, 32, 48};
  for(unsigned int i = 1; i < time_stamp.size(); i++) {
    time = static_cast<Long64_t>(time) | (static_cast<Long64_t>(time_stamp.at(i)) << shift[i]);
  }
  return time;
}
bool loadOptions(runOptions *opt, string infile) {
  ifstream fp;
  fp.open(infile);
  if(!fp.good()) return false;
  string OPT;
  string VALUE;
  for(;;) {
    fp >> OPT >> VALUE;
    if(fp.eof()) break;
    if(OPT == "INVMAP") opt->invmap = VALUE;
    if(OPT == "RUNLIST") opt->runfile = VALUE;
    if(OPT == "PIDPATH") opt->pidpath = VALUE;
    if(OPT == "FILEPATH") opt->filepath = VALUE;
    if(OPT == "TARGETTHICK") opt->target = stof(VALUE);
    if(OPT == "FIBERTHICK") opt->fiber = stof(VALUE);
    if(OPT == "ALTHICK") opt->alThick = stof(VALUE);
    if(OPT == "BRHO") opt->brho = stof(VALUE);
    if(OPT == "SIDIST") opt->siDist = stof(VALUE);
    if(OPT == "CAESARCHIP") opt->caechip = VALUE;
    if(OPT == "CAESARQDCTDC") opt->caeqdc = VALUE;
    if(OPT == "CAESARCAL") opt->caecal = VALUE;
    if(OPT == "CAESARPOS") opt->caepos = VALUE;
    if(OPT == "CALPROTON") opt->pcal = VALUE;
    if(OPT == "CALDEUTERON") opt->dcal = VALUE;
    if(OPT == "CALALPHA") opt->acal = VALUE;
    if(OPT == "CSITIME") opt->csitime = VALUE;
    if(OPT == "S4PIEMAP") opt->piemap = VALUE;
    if(OPT == "S4RINGMAP") opt->ringmap = VALUE;
    if(OPT == "S4PIECAL") opt->piecal = VALUE;
    if(OPT == "S4RINGCAL") opt->ringcal = VALUE;
    if(OPT == "CRDCPED") opt->crdcped = VALUE;
    if(OPT == "CRDCPAR") opt->crdcpar = VALUE;
    if(OPT == "CRDCCAL") opt->crdccal = VALUE;
    if(OPT == "MASSFILE") opt->massfile = VALUE;
  }
  fp.close();
  fp.clear();
  return true;
}

int main(int argc, char* argv[]) {

  string optionfile;
  int number;
  bool singlerun;
  vector<int> runnumbers;
  string sortfile;
  string readfile;

  if(argc == 1) {
    optionfile = "options.in";
  } else if(argc == 2) {
    if(strstr(argv[1], ".in") != 0) {
      optionfile = argv[1];
      singlerun = false;
    } else {
      optionfile = "options.in";
      number = stoi(argv[1]);
      singlerun = true;
    }
  } else if(argc == 3) {
    if(strstr(argv[1], ".in") != 0) {
      optionfile = argv[1];
      number = stoi(argv[2]);
    } else {
      optionfile = argv[2];
      number = stoi(argv[1]);
    }
    singlerun = true;
  } else {
    printf("Too many arguments max two");
    return 0;
  }

  runOptions *opt = new runOptions;
  if(!loadOptions(opt, optionfile)) {
    cout <<  "Inputfile " << optionfile << " not found aborting" << endl;
    return 0;
  }

  if(!singlerun) {
    ifstream runFile;
    runFile.open(opt->runfile);
    //check if this file exists
    if (runFile.is_open() == 0 ) {
      cout << "Could not open runfile " << opt->runfile << endl;
      return 0;
    }
    while(runFile.good()) {
      runFile >> number;
      //check to see if at end of file
      if (runFile.eof()) break;
      runnumbers.push_back(number);
    }
    char fname[64];
    sprintf(fname,"sort_%i-%i.root",runnumbers.front(), runnumbers.back());
    sortfile = fname;
    sprintf(fname,"read_%i-%i.root",runnumbers.front(), runnumbers.back());
    readfile = fname;
  } else {
    char fname[64];
    sprintf(fname,"sort_%i.root",number);
    sortfile = fname;
    sprintf(fname,"read_%i.root",number);
    readfile = fname;
  }

  readBeam * beams = new readBeam();
  readMass * masses = new readMass(opt->massfile);
  histo_sort * Histo_sort = new histo_sort(beams, sortfile);
  histo_read * Histo_read = new histo_read(beams, readfile);

  //forest * Forest = new forest();
  unsigned short *point,*fragmentstart;
  int unsigned words;
  int unsigned type;

  int physicsEvent = 0;
  int physicsEventGood = 0;
  int physicsEventCounter = 0;
  int scalerBuffer = 0;
  int Npauses = 0;
  int Nresumes = 0;
  int runno = 0;
  int NS800 = 0;
  int NGoodS800 = 0;
  int NSecondary = 0;
  int NGoodSecondary =0;
  float transport_efficiency = 0.62;
  det Det(Histo_sort, Histo_read, beams, masses, opt);
  vector<int> tstamp;
  vector<int> tstamp_init;
  //Set the sourceID for the S800 (normally 2) and secondary DAQ
  //This will need to be revisited
  Det.SiID = 1;
  Det.S800ID = 2;

  //open file with run numbers to sort
  int live_Rand, raw_Rand, RF_Scint;
  int runcounter = 0;
  ifstream evtfile;
  FILE *pFile;
  bool fileProblem = false;
  bool endOfRun = false;
  bool first = true;
  int argcounter = 1;
  for (;;) {  // loop over run numbers
    if (evtfile.is_open()) cout << "problem previous file not closed" << endl;
    if(!singlerun) {
      if(runcounter == (int)runnumbers.size()) break;
      number = runnumbers.at(runcounter);
    }
    //Forest->newTree(number);
    for (int iExtra=0;iExtra<3;iExtra++) { //loop over split evtfiles
      //the following loop accounts for files that were split
      endOfRun=false;
      fileProblem = 0;
      string name = Form("%srun%i/run-%4.4i-%2.2i.evt",opt->filepath.c_str(), number, number, iExtra);
      evtfile.clear();
      evtfile.open(name.c_str(),ios::binary);
      //check to see if there are extra files
      if (iExtra>0 && !fileProblem && !evtfile) {
        break;
      }
      cout << '\n'<<name << endl;
      if (evtfile.bad()) cout << "bad" << endl;
      if (evtfile.fail()) cout << "fail" << endl;
      if (!evtfile) {
        cout << "could not open event file" << endl;
        return 1;
      }
      /////////////////////////////////////////////////////////////////////////////////////////
      //Set the distance for the Si and Fibers for different settings
      Det.RingCounter->SetDistance(opt->siDist);
      Det.RingCounter->SetTargetThickness(opt->target);
      Det.RingCounter->SetAlThickness(opt->alThick);
      Det.Hira->XY_mon->setDistance(opt->siDist);
      Det.Hira->XY_mon->SetFiberThickness(opt->fiber);
      Det.s800->track->brho = opt->brho;
      ////////////////////////////////////////////////////////////////////////////////////////
      for(;;) { // loop over items in a evtfile
        //Ring items have the following structure
        //High-Level Description	Lower-Level Description	Size (bytes)
        //Header	Inclusive Size	4
        //        Type	4
        // Body Header	Size = 20	4
        //        Timestamp	8
        //        Source ID	4
        //        Barrier Type	4
        // Body	Data...	>=0
        //I am modifing here to convert this into a sort code that will read built events
        //KB Oct 2018
        //The body of built evens has the following structure
        /////////////////////////////////////////////////
        //High-Level Description	Lower-Level Description	Size (bytes)
        //Body size	        Number of bytes in body	4
        ///////////////////////////////////////////////////
        //Fragment #0	         Fragment Header	20
        //                         Ring Item Header	8
        //                         Ring Item Body Header	4 or 20
        //                         Ring Item Body	Determined by Readout program and user
        ///////////////////////////////////////////////////
        //Fragment #1	         Fragment Header	20
        //                         Ring Item Header	8
        //                         Ring Item Body Header	4 or 20
        //                         Ring Item Body	Determinable by Readout program and user
        /////////////////////////////////////////////////////////////////////////////////////
        int const hBufferWords = 4;
        int const hBufferBytes = hBufferWords*2;
        unsigned short hBuffer[hBufferWords];
        evtfile.read((char*)hBuffer,hBufferBytes);
        if(evtfile.eof()) {
          cout << "eof found" << endl;
          fileProblem = true;
          break;
        }
        if(evtfile.bad()) {
          cout << " bad found" << endl;
          fileProblem = true;
          break;
        }
        point = hBuffer;
        int offset = 0; //Where we are in the buffer
        int nbytes = *point++; //inclusive ring item size
        int nbytes2 = *point++;
        int type = *point++; //ring item type
        int type2 = *point;
        offset = 4;
        int dBufferBytes = nbytes - 8; //skipping the inclusive size and data type
        int dBufferWords = dBufferBytes/2; //calculating 16 bit words from bytes
        unsigned short dBuffer[dBufferWords];
        evtfile.read((char*)dBuffer,dBufferBytes);
        point = dBuffer;
        int BHsize = *point++;
        int BHsize2 = *point++;
        offset +=2;
        int64_t eventTstamp;
        int eventsourceID = 0;
        int eventBarrierType = 0;
        int FragmentSize =0;
        //This assumes that we are dealing with NSCLDAQ-11.0 or later which
        //contains a body header of size 20.
        //If not we are dealing with something older and modifications are necessary
        // KB, Sept 2018
        if(*point != 0xFFFF) tstamp.clear();
        if(BHsize == 20) {
          if(*point != 0xFFFF) for(int i = 0; i < 4; i++) tstamp.push_back(*(point+i));
          point +=4; //skipping the timestamp for now -- need to come back to this later Kyle
	  eventsourceID = *point++;
	  point++;
	  eventBarrierType = *point++;
	  point++;
  	  offset +=8;
	} else {
          //Buffers with no body header, for use with NSCLDAQ older than 11.0
	}
        int64_t fragmentTimestamp[5]={0};
        int fragmentsourceID[5] ={-1};
        int fragmentsize[5] ={0};
        int fragmentBarrierType[5] = {0};
        int nFragment = 0;
        int fragmentcounter=0;
        int BuiltSize = 0;
        int BuiltSize2 = 0;

        bool foundS800 = false;
        bool foundSecondary = false;
        Det.Reset();
        while (offset != nbytes/2) {
          if (type == 1) {
            runno = *point;
            cout << "run number = " << runno << endl;
            offset = nbytes/2;
  	  } else if (type == 30) {
	    if(tstamp_init.empty()) for(int k = 0; k < 4; k++) tstamp_init.push_back(tstamp.at(k));
            nFragment=fragmentcounter;
            fragmentcounter++;
            //Reads the body size, which is only present at the start of a built event
            if(nFragment == 0)	{
   	      BuiltSize = *point++;
	      BuiltSize2 = *point++;
	      offset +=2;
	    }
            int nwordsring = BuiltSize/2;
            //Now to read the fragment header
            fragmentstart=point;
	    point+=4; //Skipping the time stamp for now KB
	    fragmentsourceID[nFragment]=*point++;
	    point++;
	    fragmentsize[nFragment] = *point++;
	    point++;
	    fragmentBarrierType[nFragment]=*point++;
	    point++;
            offset+=10;
            //We may want to modify this so that it checks the position of point
            //when the data returns from the unpackers
	    //I will think about implementing this at a later date
	    //KB Oct 2018
	    offset+=fragmentsize[nFragment]/2; //Skipping the #words of the payload;
            point +=14;
            if (physicsEvent%1000 == 0) cout << '\xd'<< physicsEvent << flush;
            physicsEvent++;
            if(fragmentsourceID[nFragment] == Det.S800ID) {
              NS800++;
	    } else if(fragmentsourceID[nFragment] == Det.SiID) {
	      NSecondary++;
            }
	    bool stat = Det.unpack(point,runno,fragmentsourceID[nFragment]);
//cout << dec << fragmentsourceID[nFragment]  << "\t" << GetTimeStamp(tstamp) << endl;
	    if(stat) {
	      if(fragmentsourceID[nFragment] == Det.S800ID) {
  	        foundS800 = true;
	        physicsEventGood++;
	        NGoodS800++;
              } else if(fragmentsourceID[nFragment] == Det.SiID) {
   	        foundSecondary = true;
	        NGoodSecondary++;
	        physicsEventGood++;
              }
	    }
            //Let move to the next fragment
            point = fragmentstart +(fragmentsize[nFragment]/2)+10;
            //Turning off trees for now, I will return to this later, not needed for online
            //KB Oct 2018
            //Det.treeGrow();
          } else if (type == 31) {
            physicsEventCounter++;
            offset = nbytes/2;
          } else if (type == 2) {
            endOfRun = true;
            break;
          } else if (type == 20) {
            scalerBuffer++;
            offset = nbytes/2;
          } else if (type == 3) {
            Npauses++;
            offset = nbytes/2;
          } else if (type == 4) {
            Nresumes++;
            offset = nbytes/2;
          } else {
            offset = nbytes/2;
          }
	  nFragment++;
   	} //loop until the end of the the ring item
        if (foundS800 || foundSecondary) {
 	  Det.analyze(physicsEvent,number);
	}
      } //loop over items in a evtfile
      evtfile.close();
      evtfile.clear(); // clear event status in case we had a bad file
    } //end loop over file subsections
    if(!tstamp_init.empty() && !tstamp.empty()) {
      Long64_t runtime = GetTimeStamp(tstamp);
      tstamp_init.clear();
      tstamp.clear();
    }
    //Forest->writeTree();
    Histo_sort->RvPass->Fill(number,(float)Det.NS800_PBpass/(float)Det.NS800_PB);
    Det.NS800_PBpass = 0;
    Det.NS800_PB  = 0;
    runcounter++;
    if(singlerun) break;
  } //end loop of run file numbers

  for(unsigned int p = 0; p < beams->pb.size(); p++) {
    cout << "Secondary Beam \t" <<  beams->pb.at(p) << " = " << Det.NPB.at(p) << endl;
    cout << "Resid \tTotal \t0p \t1p \t2p \t3p \t4p \td \t\u03B1" << endl;
    for(unsigned int s = 0; s < beams->sb[p].size(); s++) {
      cout  << beams->sb[p].at(s) << " = \t" << Det.NSB[p].at(s) << "\t";
      for(int pm = 0; pm < 5; pm++) cout << Det.NSB_pro[p][pm].at(s) << "\t";
      cout << Det.NSB_deuteron[p].at(s) << "\t";
      cout << Det.NSB_alpha[p].at(s) << endl;
    }
  }

  cout << '\n'<<"physics Events = " << physicsEvent << endl;
  cout << "Good physics Events = " << physicsEventGood << endl;
  if (physicsEvent > 0) cout << "bad/total = " << (1.-(double)physicsEventGood/(double)physicsEvent)*100. << " %"<< endl;
  cout << "physics Event Counters = " << physicsEventCounter << endl;
  cout << "scaler buffers = " << scalerBuffer << endl;
  cout << "Numbers of pauses = " << Npauses << endl;
  cout << "Number of resumes = " << Nresumes << endl;

  cout << "S800 events that we unpacked correctly = " << (float)NGoodS800/(float)NS800*100. << "%" <<endl;
  cout << NGoodSecondary << " "<< NSecondary << endl;
  cout << "RingCounter events that we unpacked correctly = " << (float)NGoodSecondary/(float)NSecondary*100. << "%" <<endl;
  cout << "Number of good HiRA events = " << Det.Hira->NHira << endl;
  cout << "Number of good fiber events = " << Det.Hira->Nfiber << endl;
  cout << "Number of good S800 events = " << Det.NS800 << endl;


  cout << "Number of Unpacked S800 events = " << Det.NS800_PB << endl;
  cout << "Number of PBidx pass = " << Det.NS800_PBpass << endl;
  cout << "Percent " << (float)Det.NS800_PBpass/(float)Det.NS800_PB << endl;

  cout << "S800 Singles Trigger =" << Det.trig[0] << endl;
  cout << "Coincidence  Trigger =" << Det.trig[1] << endl;
  cout << "Singles Trigger =" << Det.trig[2] << endl;

  cout << "Writing Histo_sort " << endl;
  Histo_sort->write(); // this forces the histrograms to be read out to file
  cout << "Histo_sort written" << endl;
  cout << "Writing Histo_read " << endl;
  Histo_read->write();
  cout << "Histo_read written" << endl;
}
