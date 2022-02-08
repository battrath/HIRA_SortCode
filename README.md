# HIRA_SortCode
The major changes to the code is the new readBeams class which is used to generate histograms for all secondary and residual beam combinations without having to individually define them in the histo_sort and histo_read classes. In addition it provides an indexing system to fill in the histograms in such a way that if statements are not required to fill in different histograms. The RingCounter class, used to match CsI and Si hits, has been changed from a subclass of hira to a separate class which takes the output of hira as arguments. In addition changes have been made to the CRDC calibrations in the S800 class, the correl2 class.

```
make
./sort OptionFile (optional) RunNumber (optional)
```

Running the Code
 - The locations of all input files (calibrations/channel maps etc.) and other variables (target thickness/distance etc.) have been removed from the individual codes and instead should be defined in an input file "options.in (default name)". The code reads this file and sets all the input file locations and variables. 
 - Also contained within this file should be the name of a text file containing the numbers of all runs to be sorted (numbers.beam)
 - A directory "Beams" must exist and contain a number of text files equal to the number of secondary beams. The titles of the file must be in the form AZ.dat (e.g. 31Ar.dat). The contents of this file are the resdiduals as defined in the PID gate files. The file should be formatted as a series of rows contained the Z and A of the residuals separated by a tab space. NOTE these files are used by readBeams to define and fill all the histograms. 
 - After compilation the code can be run with the following command
   ./sort OptionFile (optional) RunNumber (optional) 
   where OptionFile is the input file containing filepaths and variables and RunNumber is the run to sort. The code can be run without arguments, in which case it attempts to load an options fle with the filename "option.in". The order of the arguments is unimportant and the code can be run with either (or both) of them. If a RunNumber is given as an argument it will only sort this run instead of the list of runs defined in the run options file. In this case if a options file is not given as an argument it defaults to the name "options.in"  


CRDC Calibration 
 - In previous iterations of the code the CRDC Calibrations, obtained from Mask Runs, used only single calibration parameters which does not account for gains drifts. The code can now use ultiple calibration parameters, from different Mask Runs, with a linear interpolation used to calibrate the CRDCs. 
 - The calibration parameters for both CRDCs (X and Y grids) should be defined in a single text file with the format
   CRDC#   X/Y     Run     Slope           Offset
   where CRDC# is the CRDC number (CRDC1 = 0, CRDC2 = 1), X/Y specifys X or Y grids (at present the X grids are not set for extrapolation), Run is the run number of the mask runs and Slope/Offset re the linear calibration parameters.
 - The file contaning these parameters should be defined in the run options file as option "CRDCCAL".

Masses
 - All masses are now read in from a text file with the data taken from the Ame2020 mass evaluation.
 - So far I only masses less than A = 40 are included in the file.
 - The data are formatted in the text file, masses.txt, as
   Z	A	Mass Excess (keV)

Correl Histograms
 - Histogram are defined and filled according to the files read in by the readBeam.cpp class
 - Erel spectra are drawn for 1-4 proton decay, deuteron decay and alpha decay. For the purposes of histogram filling, alpha decay is treated as a "5p" decay and deuteron decay is a "6p" decay.
