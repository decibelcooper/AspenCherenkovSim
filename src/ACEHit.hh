#include "G4VHit.hh"

class ACEHit : public G4VHit {
   public:
    ACEHit(double, double);
    ~ACEHit();

    double GetEnergy();
    double GetTime();

   private:
    double hitTime;
    double hitDEnergy;
};
