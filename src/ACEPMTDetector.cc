#include <iostream>
using namespace std;

#include <G4SystemOfUnits.hh>
#include "ACEHit.hh"
#include "ACEHitsCollection.hh"
#include "ACEPMTDetector.hh"
#include "G4SDManager.hh"

ACEPMTDetector::ACEPMTDetector(G4String name) : G4VSensitiveDetector(name) {
    hitsCollection = 0;
    collectionName.insert("1");
}

ACEPMTDetector::~ACEPMTDetector() { ; }

void ACEPMTDetector::Initialize(G4HCofThisEvent *hcc) {
    hitsCollection = new ACEHitsCollection(SensitiveDetectorName, collectionName[0]);

    G4SDManager *sdMan = G4SDManager::GetSDMpointer();

    G4int collectionID = sdMan->GetCollectionID(hitsCollection);
    hcc->AddHitsCollection(collectionID, hitsCollection);
}

void ACEPMTDetector::EndOfEvent(G4HCofThisEvent *) { ; }

G4bool ACEPMTDetector::ProcessHits(G4Step *step, G4TouchableHistory *) {
    G4Track *track = step->GetTrack();
    double time = track->GetGlobalTime();
    double deposit = step->GetTotalEnergyDeposit();
    if (deposit == 0.0) return false;

    if (track->GetDefinition()->GetParticleName().compare("opticalphoton")) return false;

    if (deposit < 3.1 * eV || deposit > 4.13 * eV) cout << "deposit (eV): " << deposit / eV << endl;

    ACEHit *hit = new ACEHit(time, deposit);
    hitsCollection->Insert(hit);

    return true;
}
