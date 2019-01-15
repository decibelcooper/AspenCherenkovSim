#include "G4VHitsCollection.hh"

class ACEHit;

class ACEHitsCollection : public G4VHitsCollection {
   public:
    ACEHitsCollection(G4String, G4String);
    ~ACEHitsCollection();

    G4VHit *GetHit(size_t) const;
    size_t GetSize() const;

    void Insert(ACEHit *);

   private:
    ACEHit **collection;
    size_t size;
};
