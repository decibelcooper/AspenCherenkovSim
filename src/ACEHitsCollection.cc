#include "ACEHitsCollection.hh"
#include "ACEHit.hh"

ACEHitsCollection::ACEHitsCollection(G4String detName, G4String colName)
    : G4VHitsCollection(detName, colName) {
    size = 0;
    collection = new ACEHit *[0];
}

ACEHitsCollection::~ACEHitsCollection() {
    for (size_t l = 0; l < size; l++) delete collection[l];
    delete collection;
}

G4VHit *ACEHitsCollection::GetHit(size_t index) const { return collection[index]; }

size_t ACEHitsCollection::GetSize() const { return size; }

void ACEHitsCollection::Insert(ACEHit *newHit) {
    ACEHit **collectionOld = collection;

    collection = new ACEHit *[size + 1];

    for (size_t l = 0; l < size; l++) collection[l] = collectionOld[l];
    delete collectionOld;

    size++;

    collection[size - 1] = newHit;
}