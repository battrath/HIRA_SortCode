OBJECTS = det.o hira.o histo_sort.o histo_read.o caen.o calibrate.o TDC1190.o elist.o doppler.o solution.o ZApar.o pid.o einstein.o newton.o correl2.o  loss2.o losses.o S800.o fiber.o XYmon.o ringCounter.o ceasar.o readBeam.o readMass.o
ALLOBJECTS := $(patsubst %.cpp,%.o,$(wildcard *.cpp))
CFLAGS= -c -ggdb -std=c++11 -W -I$(shell root-config --incdir)
COMPILER= g++
LINKOPTION = $(shell root-config --libs)

all: sort ThFront ThBack

sort: sort.o $(OBJECTS)
	$(COMPILER) -o sort sort.o $(OBJECTS) $(LINKOPTION)

readtree: readtree.o $(OBJECTS)
	$(COMPILER) -o readtree readtree.o $(OBJECTS) $(LINKOPTION)

ThFront: ThFront.o $(OBJECTS)
	$(COMPILER) -o ThFront ThFront.o $(OBJECTS) $(LINKOPTION)

ThBack: ThBack.o $(OBJECTS)
	$(COMPILER) -o ThBack ThBack.o $(OBJECTS) $(LINKOPTION)

$(ALLOBJECTS): %.o : %.cpp
	$(COMPILER) $(CFLAGS) $< -o $@


clean:
	rm -f *.o

