GEANT4LIBS = $(shell geant4-config --libs)
GEANT4FLAGS = $(shell geant4-config --cflags)
BASE = ACEDetectorConstruction ACEHit ACEHitsCollection ACEMessenger\
       ACEPMTDetector ACEPhysicsList ACEPrimaryGeneratorAction\
       ACEUserEventAction ACEUserRunAction ACEUserStackingAction\
       Cerenkov_Counter Cerenkov_Counter2 ACEActionInitialization
OBJECTS = $(addprefix src/,$(BASE:=.o))
HEADERS = $(addprefix src/,$(BASE:=.hh))

all: bin/ACE

clean:
	rm -f $(OBJECTS)
	rm -f bin/ACE

bin/ACE: src/ACE.cc $(OBJECTS)
	$(CXX) -g -o $@ $(CPPFLAGS) $(CXXFLAGS) $(GEANT4LIBS) $(GEANT4FLAGS) $^

src/%.o: src/%.cc $(HEADERS)
	$(CXX) -g -c -o $@ $(CPPFLAGS) $(CXXFLAGS) $(GEANT4FLAGS) $<

