#include "G4UserRunAction.hh"

class ACEUserRunAction : public G4UserRunAction {
   public:
    ACEUserRunAction();
    ~ACEUserRunAction();

    void BeginOfRunAction(const G4Run *);
    void EndOfRunAction(const G4Run *);

   private:
};

extern pthread_mutex_t *eventWriteLock;

