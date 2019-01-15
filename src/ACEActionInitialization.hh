#ifndef ACE_ACTION_INITIALIZATION_H
#define ACE_ACTION_INITIALIZATION_H

#include <pthread.h>

#include <G4String.hh>
#include <G4VUserActionInitialization.hh>

class ACEActionInitialization : public G4VUserActionInitialization {
   public:
    ACEActionInitialization();
    virtual ~ACEActionInitialization();

    void Build() const;
    void BuildForMaster() const;

   private:
};

#endif
