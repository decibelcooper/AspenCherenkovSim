#include "ACEUserEventAction.hh"
#include "ACEUserRunAction.hh"
#include "ACEHitsCollection.hh"
#include "ACEHit.hh"
#include "ACEMessenger.hh"
#include "G4HCofThisEvent.hh"
#include "G4Event.hh"

#include <iostream>
using namespace std;

ACEUserEventAction::ACEUserEventAction() { ; }

ACEUserEventAction::~ACEUserEventAction() { ; }

void ACEUserEventAction::BeginOfEventAction(const G4Event *) { ; }

void ACEUserEventAction::EndOfEventAction(const G4Event *event) {
    G4HCofThisEvent *hcc = event->GetHCofThisEvent();

    ACEHitsCollection *PMTHC = (ACEHitsCollection *)(hcc->GetHC(0));

    G4PrimaryVertex *priV = event->GetPrimaryVertex();

    pthread_mutex_lock(eventWriteLock);

    messenger->AddHits(priV->GetX0(), priV->GetY0(), PMTHC->GetSize());

    pthread_mutex_unlock(eventWriteLock);
}
