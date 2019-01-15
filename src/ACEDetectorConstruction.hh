#include "G4VUserDetectorConstruction.hh"

class ACEDetectorConstruction : public G4VUserDetectorConstruction {
   public:
    ACEDetectorConstruction();
    ~ACEDetectorConstruction();

    G4VPhysicalVolume *Construct();
    void ConstructSDandField();

   private:
};
