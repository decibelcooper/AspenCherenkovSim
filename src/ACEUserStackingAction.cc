#include <iostream>
#include <cmath>
using namespace std;

#include "ACEUserStackingAction.hh"
#include "ACEMessenger.hh"
#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"

ACEUserStackingAction::ACEUserStackingAction() { ; }

ACEUserStackingAction::~ACEUserStackingAction() { ; }

G4ClassificationOfNewTrack ACEUserStackingAction::ClassifyNewTrack(const G4Track *) {
    /*	if (aTrack->GetDefinition() ==
     G4OpticalPhoton::OpticalPhotonDefinition()) {
     // particle is optical photon
     if (aTrack->GetParentID() > 0) {
     // particle is secondary
     messenger->AddToPhotonCount();
     }
     } else {
     if (aTrack->GetParentID() > 0) {
     //particle is secondary
     G4ParticleDefinition* def = aTrack->GetDefinition();
     if (def->GetPDGCharge() != 0) {
     double EvsT = aTrack->GetTotalEnergy() / def->GetPDGMass() * 0.304910678;
     if (EvsT >= 1)
     cout << "Secondary non-photon:\t" << def->GetParticleName() << "\tEnergy vs.
     threshold:\t" << EvsT
     << endl;
     }
     } else {
     //			cout << "Incident particle:\t" <<
     aTrack->GetDefinition()->GetParticleName() << endl;
     }
     }*/
    return fUrgent;
}
