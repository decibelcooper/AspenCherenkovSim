#include "ACEHit.hh"

ACEHit::ACEHit(double time, double deposit) {
    hitTime = time;
    hitDEnergy = deposit;
}

ACEHit::~ACEHit() { ; }

double ACEHit::GetEnergy() { return hitDEnergy; }
double ACEHit::GetTime() { return hitTime; }
