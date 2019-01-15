#include <sstream>
using namespace std;

#include <G4SDManager.hh>
#include <G4SystemOfUnits.hh>
#include "G4Box.hh"
#include "G4Element.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4PVPlacement.hh"
#include "G4VPhysicalVolume.hh"

#include "ACEDetectorConstruction.hh"
#include "ACEPMTDetector.hh"
#include "Cerenkov_Counter2.hh"

ACEDetectorConstruction::ACEDetectorConstruction() { ; }

ACEDetectorConstruction::~ACEDetectorConstruction() { ; }

G4VPhysicalVolume *ACEDetectorConstruction::Construct() {
    G4Element *N = new G4Element("Nitrogen", "N", 7, 14.01 * g / mole);
    G4Element *O = new G4Element("Oxygen", "O", 8, 16.00 * g / mole);

    G4Material *vacuum = new G4Material("vacuum", 15 * g / 6.022 / pow((double)10, 23) / cm3, 2);
    vacuum->AddElement(N, 80 * perCent);
    vacuum->AddElement(O, 20 * perCent);

    G4Box *worldSolid = new G4Box("world", 1.5 * m, 1.5 * m, 1.5 * m);
    G4LogicalVolume *worldLog = new G4LogicalVolume(worldSolid, vacuum, "worldLogical");
    G4VPhysicalVolume *World_Phys =
        new G4PVPlacement(0, G4ThreeVector(0, 0, 0), worldLog, "worldPhysical", 0, false, 0);

    Cerenkov_Counter2 *cc = new Cerenkov_Counter2();
    cc->Build(World_Phys);
    delete cc;

    return World_Phys;
}

void ACEDetectorConstruction::ConstructSDandField() {
    ACEPMTDetector *cSensitiveDetector = new ACEPMTDetector("cerenkov/PMT");
    G4SDManager::GetSDMpointer()->AddNewDetector(cSensitiveDetector);
    SetSensitiveDetector("Cerenkov_photocathodeWindowLog", cSensitiveDetector);
}
