#include "G4UserStackingAction.hh"

class G4Track;

class ACEUserStackingAction : public G4UserStackingAction {
   public:
    ACEUserStackingAction();
    ~ACEUserStackingAction();

    G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track *);
};
