#include <iostream>
using namespace std;

#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include <G4SystemOfUnits.hh>
#include "G4ThreeVector.hh"
#include "Randomize.hh"

#include "ACEMessenger.hh"
#include "ACEPrimaryGeneratorAction.hh"

ACEPrimaryGeneratorAction::ACEPrimaryGeneratorAction() {
    rayGun = new G4ParticleGun(1);
    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition *particle = particleTable->FindParticle("e-");
    rayGun->SetParticleDefinition(particle);
    rayGun->SetParticleTime(0);
    rayGun->SetParticlePosition(G4ThreeVector(5 * cm, -5 * cm, -1.49 * m));
    rayGun->SetParticleMomentumDirection(G4ThreeVector(0, 0, 1));
    rayGun->SetParticleEnergy(2.0 * GeV);
}

ACEPrimaryGeneratorAction::~ACEPrimaryGeneratorAction() { delete rayGun; }

void ACEPrimaryGeneratorAction::GeneratePrimaries(G4Event *event) {
    G4ThreeVector beamPos = rayGun->GetParticlePosition();
    rayGun->SetParticlePosition(G4ThreeVector(beamPos.x() + 6 * mm * (CLHEP::RandFlat::shoot() - .5),
                                              beamPos.y() + 6 * mm * (CLHEP::RandFlat::shoot() - .5),
                                              beamPos.z()));
    rayGun->GeneratePrimaryVertex(event);
    rayGun->SetParticlePosition(beamPos);
}

double ACEPrimaryGeneratorAction::getCosmicMomentum(double randFlat) {
    double a = 0.00559394, b = 0.54543, c = 0.561472, d = 0.00936388, D = -0.402605, z = 0.140012,
           pi = 3.14159265;

    double arg = d / a * (randFlat + D);
    double gamma = log(1 - arg * arg);
    double alpha = 2 / pi / z + gamma / 2;

    return exp(sin(arg) / b * sqrt(sqrt(alpha * alpha - gamma / z) - alpha) - c / b) * GeV;
}
