#include <pthread.h>

#include "G4UserEventAction.hh"

class ACEUserEventAction : public G4UserEventAction {
   public:
    ACEUserEventAction();
    ~ACEUserEventAction();

    void BeginOfEventAction(const G4Event *);
    void EndOfEventAction(const G4Event *);
};

