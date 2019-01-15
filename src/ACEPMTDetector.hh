#include "G4VSensitiveDetector.hh"

class G4HCofThisEvent;
class G4Step;
class G4TouchableHistory;
class ACEHitsCollection;

class ACEPMTDetector : public G4VSensitiveDetector {
   public:
    ACEPMTDetector(G4String);
    ~ACEPMTDetector();

    void Initialize(G4HCofThisEvent *);
    void EndOfEvent(G4HCofThisEvent *);

   protected:
    G4bool ProcessHits(G4Step *, G4TouchableHistory *);

   private:
    ACEHitsCollection *hitsCollection;
};
