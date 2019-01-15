#include "ACEMessenger.hh"

#include <iostream>
using namespace std;

ACEMessenger::ACEMessenger() {
    photonCount = 0;
    numHits = 0;
    numEvents = 0;
    spectrum = new int[200];
    for (int i = 0; i < 200; i++) spectrum[i] = 0;
    spaceSpecSize = 1;
    spaceSpectrum = new double *[spaceSpecSize];
}

ACEMessenger::~ACEMessenger() { delete spectrum; }

void ACEMessenger::AddToPhotonCount() { photonCount++; }

void ACEMessenger::resetCounts() {
    photonCount = 0;
    numHits = 0;
    numEvents = 0;
    delete[] spectrum;
    spectrum = new int[200];
    for (int i = 0; i < 200; i++) spectrum[i] = 0;
}

int ACEMessenger::PhotonCount() { return photonCount; }

void ACEMessenger::AddHits(int n) {
    numHits += n;
    numEvents += 1;
    if (n < 200) spectrum[n]++;
}

void ACEMessenger::AddHits(double x, double y, int n) {
    if (numEvents == spaceSpecSize) {
        spaceSpecSize *= 2;
        double **newSpec = new double *[spaceSpecSize];
        for (int i = 0; i < numEvents; i++) newSpec[i] = spaceSpectrum[i];
        delete[] spaceSpectrum;
        spaceSpectrum = newSpec;
    }

    spaceSpectrum[numEvents] = new double[3];
    spaceSpectrum[numEvents][0] = x;
    spaceSpectrum[numEvents][1] = y;
    spaceSpectrum[numEvents][2] = n;

    numHits += n;
    numEvents++;
}

double ACEMessenger::GetAverageNumHits() { return ((double)numHits) / ((double)numEvents); }

int *ACEMessenger::getSpectrum() { return spectrum; }

int ACEMessenger::getNumEvents() { return numEvents; }

double **ACEMessenger::getSpaceSpectrum() { return spaceSpectrum; }
