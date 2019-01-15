#include <sstream>

#include <G4RunManager.hh>
#include <G4Threading.hh>

#include "ACEPrimaryGeneratorAction.hh"
#include "ACEUserEventAction.hh"
#include "ACEUserRunAction.hh"
#include "ACEUserStackingAction.hh"

#include "ACEActionInitialization.hh"

ACEActionInitialization::ACEActionInitialization() { ; }

ACEActionInitialization::~ACEActionInitialization() { ; }

void ACEActionInitialization::Build() const {
    G4RunManager::GetRunManager()->SetPrintProgress(1000);

    SetUserAction(new ACEPrimaryGeneratorAction);
#ifndef G4MULTITHREADED
    SetUserAction(new ACEUserRunAction);
#endif
    SetUserAction(new ACEUserEventAction);
}

void ACEActionInitialization::BuildForMaster() const { SetUserAction(new ACEUserRunAction); }
