# HIRA_SortCode

## Major Changes
The readBeams class has been created and is used to generate histograms in the histo_sort and histo_read classes for all secondary and residual beam combinations. In addition it provides an indexing system to fill in the histograms in the main code. The RingCounter class, used to match CsI and Si hits, has been changed from a subclass of HIRA to a separate class which takes the output of hira as arguments. In addition changes have been made to the CRDC calibrations in the S800 class and the correl2 class. Filenames and parameters previously hardcoded into the code have beeen removed and are instead included in a single options file which must be prepared before running codes.

## Before Sorting
- An options file must be created which includes experimental variables (target thickness, brho etc.) as well any calibration or configuration files. An example file (options.in) has been included here, with the definitions defined in runOptions.h.
- A list containing the run list should be created and included inthe options file
- A directory "Beams" must exist and contain a number of text files equal to the number of secondary beams. The titles of the files must be in the form AZ.dat (e.g. 31Ar.dat). The contents of this file are the resdiduals as defined in the PID gate files. The file should be formatted as a series of rows contained the Z and A of the residuals separated by a tab space and in the same order as the PID gate files. NOTE these files are used by readBeams to define and fill all the histograms.
- In the directory zlins, PID gate files must be created in the for each secondary beam and the titles of the files must be in the form "AZ.zline" (e.g. 31Ar.zline). The format of this file is:
```
Number of Gates
Z1 A2
Number of Gate Nodes
Gate Node coordinates
...
...
Z1 Z2
...
...

```
## Sorting Data
- A Makefile is included to compile the code which can then be run using the commands
```
make
./sort OptionFile (optional) RunNumber (optional)
```
- OptionFile is the input file containing filepaths and variables and RunNumber is the run to sort. The code can be run without arguments, in which case it attempts to load an options fle with the filename "option.in". The order of the arguments is unimportant and the code can be run with either (or both) of them. 
- If a RunNumber is given as an argument it will only sort this run instead of the list of runs defined in the run options file. In this case if a options file is not given as an argument it defaults to the name "options.in" 

## Making PID Gate files
- Two macros are available to produce PID gate files, banana.C and makePID.C 
- banana.C is used to create a single file containing a PID gate. The process for creating the file is
```
- Open TH2 Partcle ID plot
- .L banana.C
- banana()
- Draw PID plot
- Copy data from file banana.dat into PID file.
```
- makePID.C is a new code which can be used to create a complete PID gate file. The code creates two files which are used for readBeam.cpp and for pid.cpp. The code is run as following;
```
- Open TH2 Partcle ID plot.
- .L makePID.C
- makePID()
- Draw PID plot.
- Enter Z and A of particle.
- Draw another gate or quit (y/n).
- Move and rename file PID.dat into zline directory.
- Move and rename file beam.dat into Beam directory, the filename should be of the form AZ.dat (e.g. 31Ar.dat)
```

## CRDC Calibration 
- In previous iterations of the code the CRDC Calibrations, obtained from Mask Runs, used only single calibration parameters which does not account for gains drifts. The code can now use ultiple calibration parameters, from different Mask Runs, with a linear interpolation used to calibrate the CRDCs. 
- The calibration parameters for both CRDCs (X and Y grids) should be defined in a single text file with the format
```
   CRDC#   X/Y     Run     Slope           Offset
```
- CRDC# is the CRDC number (CRDC1 = 0, CRDC2 = 1), X/Y specifys X or Y grids (at present the X grids are not set for extrapolation), Run is the run number of the mask runs and Slope/Offset re the linear calibration parameters.
- The file contaning these parameters should be defined in the run options file as option "CRDCCAL".

## Masses
- All masses are now read in from a text file with the data taken from the Ame2020 mass evaluation.
- A mass file "masses.txt" exists, but at present only masses less than A = 40 are included in the file.
- The data are formatted in the text file, masses.txt, as
```
   Z	A	Mass Excess (keV)
```
## Correl Histograms
- Erel spectra are drawn for 1-4 proton decay, deuteron decay and alpha decay. For the purposes of histogram filling, alpha decay is treated as a "5p" decay and deuteron decay is a "6p" decay.
