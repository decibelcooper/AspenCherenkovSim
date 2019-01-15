#include "sys/time.h"

#include "Randomize.hh"

#include "ACEMessenger.hh"
#include "ACEUserRunAction.hh"
#include "globals.hh"

pthread_mutex_t *eventWriteLock;

ACEUserRunAction::ACEUserRunAction() {
    eventWriteLock = new pthread_mutex_t;
    pthread_mutex_init(eventWriteLock, NULL);
}

ACEUserRunAction::~ACEUserRunAction() {
    pthread_mutex_destroy(eventWriteLock);
    delete eventWriteLock;
}

void ACEUserRunAction::BeginOfRunAction(const G4Run *) { ; }

void ACEUserRunAction::EndOfRunAction(const G4Run *) { ; }
