#include "G4VUserPrimaryGeneratorAction.hh"

#include <G4Event.hh>
#include <G4ParticleGun.hh>

class ACEPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
   public:
    ACEPrimaryGeneratorAction();
    ~ACEPrimaryGeneratorAction();

    void GeneratePrimaries(G4Event *);

   private:
    double getCosmicMomentum(double randFlat);

    G4ParticleGun *rayGun;
};
