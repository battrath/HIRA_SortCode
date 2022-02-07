//creates energy order (descending) lists of strips

#include "elist.h"
#include <algorithm>
#include <iostream>
using namespace std;

//places a new strip energy in an order list from max to min energy
void elist::Add(int StripNum, int underOver, float energy,  float time) {
  //first find place in list
  int i = 0;
  for (;;) {
    if (i == Nstore) break;
    if (energy > Order[i].energy)break;
    i++;
  }
  if (i == nnn) return; // not enougth room in list

  //new list length
  int N = min(nnn,Nstore+1);

  // move those in energy below the new value down the list
  for (int j=N-1;j>i;j--) Order[j] = Order[j-1];

  //add present energy to list

  Order[i].energy = energy;
  Order[i].overflow = underOver;
  Order[i].strip = StripNum;
  Order[i].time = time;
  Order[i].energyRlow = 0.;
  // increase list length
  Nstore = N;
  mult = N;
}
//***********************************************************************
//places a new strip energy in an order list from max to min energy
//updated for high/low gain (HINP4) chips
void elist::Add(int StripNum, float energy, int energyRlow, int energyR, float time) {
  //first find place in list
  int i = 0;
  for (;;) {
    if (i == Nstore) break;
    if(energy >0) {
      if (energy > Order[i].energy)break;
    } else {
      if(energyR > Order[i].energyR)break;
    }
    i++;
  }
  if (i == nnn) return; // not enougth room in list

  //new list length
  int N = min(nnn,Nstore+1);
  // move those in energy below the new value down the list
  for (int j = N-1; j > i; j--) Order[j] = Order[j-1];

  //add present energy to list

  Order[i].energy = energy;
  Order[i].energyR = energyR;
  Order[i].energyRlow = energyRlow;
  Order[i].strip = StripNum;
  Order[i].time = time;

  // increase list length
  Nstore = N;
  mult = N;
}

void elist::Remove(int entry) {
  if (entry >= Nstore) {
      cout << "entry does not exist" << endl;
      return;
  }
  if (entry == Nstore -1) {
    Nstore--;
    return;
  }
  for (int i = entry; i < Nstore; i++) {
    Order[i] = Order[i+1];
  }
  Nstore--;
  return;
}

void elist::Add(int StripNum, float energy, int rawenergy, int time)  { //for use with tree
  Order[Nstore].energy = energy;
  Order[Nstore].strip = StripNum;
  Order[Nstore].time = time;
  Order[Nstore].energylow = 0;
  Order[Nstore].energyR = rawenergy;
  Nstore++;
}

//**********************************************************************
//remove silicon strip cross talk from list
int elist::Reduce(char*face) {

  if (Nstore <= 1) return Nstore;
  for (int ii = Nstore-1;ii > 0;ii--) { // ii location in list of interest
    int xtalk = 0;
    if (Nstore <= 1) return Nstore;
    if (Nstore > nnn) {
      cout << "problem in Reduce" << endl;
      return 0;
     }
    if (Nstore < 0) {
      cout << "problem in Reduce()" << endl;
      return 0;
    }
    for (int i = ii-1; i >= 0; i--) {
      //look for neighboring strips
      if (abs(Order[ii].strip-Order[i].strip) == 1) {
        if (*face == 'F') {
          if (Order[ii].energy < Order[i].energy*0.044 && Order[i].energy > 10.) {
            xtalk = 1; // signal xtalk
            break;
          }
        } else if (*face == 'B') {
          if (Order[ii].energy < Order[i].energy*0.30 && Order[i].energy > 10.) {
            xtalk = 1; // signal xtalk
            break;
          }
        }
      }
    }
     if (xtalk == 0) continue;
     //remove from list
     if (ii+1 < Nstore) {
       for (int j = ii; j < Nstore; j++)Order[j] = Order[j+1];
     }
     Nstore--;
  }
  return Nstore;
}
//*********************************************************************
void elist::reset() {
  for(int i =0;i<Nstore;i++) {
    Order[i].energy = 0;
    Order[i].energyR = 0;
    Order[i].energyRlow = 0;
    Order[i].strip = 0;
    Order[i].time = 0;
    Order[i].neighbours=0;
    Order[i].energyMax=0;
  }
  Nstore = 0;
}
//*********************************************************************
  //looks for cross talks events
void elist::Neighbours() {
  if (Nstore < 1) return; // nothing to look at
  if (Nstore == 1) { // no neighbors to look for
    Order[0].energyMax = Order[0].energy;
    Order[0].neighbours = 0;
    return ;
  }
  int i=-1;
  for (;;) {
    i++;
    if (i >= Nstore)break;
    Order[i].energyMax = Order[i].energy;
    Order[i].neighbours = 0;
    int j = i;
    for (;;) {
      j++;
      if (j >= Nstore) break;
      if (abs(Order[i].strip - Order[j].strip) == 1) { //neighboring strips
        if(Order[j].energy > Order[i].energy) cout << Order[i].energy << "\t" << Order[j].energy << endl;
        Order[i].energy += Order[j].energy; // add energy from adjacent strip
        Order[i].neighbours++;
        //remove this strip from the list
        if (j != Nstore-1) {
          for (int k = j+1; k < Nstore; k++) Order[k-1] = Order[k];
 	}
	Nstore--;
        j--;
      }
    }
  }
}
//looks for cross talks events
void elist::NeighboursPies(){
  if (Nstore <1) return; // nothing to look at
  if (Nstore ==1) { // no neighbors to look for
    Order[0].energyMax = Order[0].energy;
    Order[0].neighbours = 0;
    return ;
  }
  int i =- 1;
  for (;;) {
    i++;
    if (i >= Nstore)break;
    Order[i].energyMax = Order[i].energy;
    Order[i].neighbours = 0;
    int j = i;
    for (;;) {
      j++;
      if (j >= Nstore) break;
      //The pies need to wrap around for 0 and 127
      if(Order[i].strip ==0) {
        if (Order[j].strip == 1 || Order[j].strip ==127) { //neighboring strips
          Order[i].energy += Order[j].energy; // add energy from
          //adjacent strip
	  Order[i].neighbours++;
          //remove this strip from the list
          if (j != Nstore-1) {
            for (int k = j+1; k < Nstore; k++)Order[k-1] = Order[k];
          }
          Nstore--;
          j--;
        }
      } else if(Order[i].strip == 127) {
        if (Order[j].strip == 126 || Order[j].strip ==0) { //neighboring strips
          Order[i].energy += Order[j].energy; // add energy from
	  // adjacent strip
	  Order[i].neighbours++;
          //remove this strip from the list
          if (j != Nstore-1) {
           for (int k = j+1; k < Nstore; k++)Order[k-1] = Order[k];
          }
          Nstore--;
          j--;
        }
      } else {
        if (abs(Order[i].strip - Order[j].strip) == 1) { //neighboring strips
          Order[i].energy += Order[j].energy; // add energy from
          // adjacent strip
          Order[i].neighbours++;
          //remove this strip from the list
          if (j != Nstore-1) {
            for (int k=j+1;k<Nstore;k++)Order[k-1] = Order[k];
          }
          Nstore--;
          j--;
        }
      }
    }
  }
}

//cut threshold
void elist::Threshold(float threshold) {
  for(int i=0;i<Nstore;i++) {
    threshold0 = threshold;
    if(Order[i].energy<threshold0) {
      Remove(i);
      i--;
    }
  }
}
