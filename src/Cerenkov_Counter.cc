/*
 * Cerenkov_Counter.cc
 *
 *  Created on: Apr 4, 2012
 *      Author: dblyth
 */

#include <cmath>
#include <sstream>
using namespace std;

#include <G4Box.hh>
#include <G4Color.hh>
#include <G4Cons.hh>
#include <G4LogicalBorderSurface.hh>
#include <G4LogicalSkinSurface.hh>
#include <G4LogicalVolume.hh>
#include <G4Material.hh>
#include <G4NistManager.hh>
#include <G4OpticalSurface.hh>
#include <G4PVPlacement.hh>
#include <G4SDManager.hh>
#include <G4SubtractionSolid.hh>
#include <G4SystemOfUnits.hh>
#include <G4Tubs.hh>
#include <G4UnionSolid.hh>
#include <G4VisAttributes.hh>

#include "ACEPMTDetector.hh"
#include "Cerenkov_Counter.hh"

Cerenkov_Counter::Cerenkov_Counter() { ; }

Cerenkov_Counter::~Cerenkov_Counter() { ; }

void Cerenkov_Counter::Build(G4VPhysicalVolume *World_Phys) {
    //	Start setting up materials to be involved in the counter's construction
    G4NistManager *nistMan = G4NistManager::Instance();
    G4SDManager *SDMan = G4SDManager::GetSDMpointer();

    G4Material *air = nistMan->FindOrBuildMaterial("G4_AIR");
    G4MaterialPropertiesTable *airMPT = new G4MaterialPropertiesTable();
    double airEnergy[2] = {2.08 * eV, 5.90 * eV};
    double airRIndex[2] = {1.0003, 1.0003};
    double airAbsorp[2] = {1000 * m, 1000 * m};
    airMPT->AddProperty("RINDEX", airEnergy, airRIndex, 2);
    airMPT->AddProperty("ABSLENGTH", airEnergy, airAbsorp, 2);
    air->SetMaterialPropertiesTable(airMPT);

    // TODO:	Aerogel properties
    G4Material *SiO2 = nistMan->FindOrBuildMaterial("G4_SILICON_DIOXIDE");
    G4Material *aerogel = new G4Material("Aerogel", 0.2 * g / cm3, 2);
    aerogel->AddMaterial(SiO2, 99.65 * perCent);
    aerogel->AddMaterial(air, 0.35 * perCent);
    G4MaterialPropertiesTable *aerogelMPT = new G4MaterialPropertiesTable();
    double aerogelEnergy[2] = {2.08 * eV, 5.90 * eV};
    double aerogelRIndex[2] = {1.025, 1.025};
    double aerogelAbsorp[2] = {0.15 * m, 0.15 * m};
    double aerogelRayleigh[2] = {6 * cm, 6 * cm};
    aerogelMPT->AddProperty("RINDEX", aerogelEnergy, aerogelRIndex, 2);
    aerogelMPT->AddProperty("ABSLENGTH", aerogelEnergy, aerogelAbsorp, 2);
    aerogelMPT->AddProperty("RAYLEIGH", aerogelEnergy, aerogelRayleigh, 2);
    aerogel->SetMaterialPropertiesTable(aerogelMPT);

    //	Aluminum for main construction material
    G4Material *Al = nistMan->FindOrBuildMaterial("G4_Al");

    //	Glass for PMT window, just to add the ability to reflect at oblique
    //	angles
    G4Material *glass = nistMan->FindOrBuildMaterial("G4_Pyrex_Glass");
    G4MaterialPropertiesTable *glassMPT = new G4MaterialPropertiesTable();
    double glassEnergy[2] = {2.08 * eV, 5.90 * eV};
    double glassRIndex[2] = {1.5, 1.5};
    double glassAbsorp[2] = {1000 * m, 1000 * m};
    glassMPT->AddProperty("RINDEX", glassEnergy, glassRIndex, 2);
    glassMPT->AddProperty("ABSLENGTH", glassEnergy, glassAbsorp, 2);
    glass->SetMaterialPropertiesTable(glassMPT);

    //	Steel for the PMT shielding
    G4Element *C = nistMan->FindOrBuildElement("C");
    G4Element *Si = nistMan->FindOrBuildElement("Si");
    G4Element *Cr = nistMan->FindOrBuildElement("Cr");
    G4Element *Mn = nistMan->FindOrBuildElement("Mn");
    G4Element *Fe = nistMan->FindOrBuildElement("Fe");
    G4Element *Ni = nistMan->FindOrBuildElement("Ni");

    G4double density;
    G4int ncomponents;
    G4double fractionmass;
    G4Material *steel = new G4Material("Cerenkov_steel", density = 8.06 * g / cm3, ncomponents = 6);
    steel->AddElement(C, fractionmass = 0.001);
    steel->AddElement(Si, fractionmass = 0.007);
    steel->AddElement(Cr, fractionmass = 0.18);
    steel->AddElement(Mn, fractionmass = 0.01);
    steel->AddElement(Fe, fractionmass = 0.712);
    steel->AddElement(Ni, fractionmass = 0.09);

    //	Set up optical surfaces
    G4OpticalSurface *diffuseReflectorOptical =
        new G4OpticalSurface("Cerenkov_diffuseReflectorOptical", glisur, ground, dielectric_metal);
    double diffuseReflectorEnergy[2] = {2.08 * eV, 5.90 * eV};
    double diffuseReflectorRefl[2] = {0.95, 0.95};
    G4MaterialPropertiesTable *diffuseReflectorOpticalMPT = new G4MaterialPropertiesTable();
    diffuseReflectorOpticalMPT->AddProperty("REFLECTIVITY", diffuseReflectorEnergy, diffuseReflectorRefl, 2);
    diffuseReflectorOptical->SetMaterialPropertiesTable(diffuseReflectorOpticalMPT);
    diffuseReflectorOptical->SetPolish(0.0);

    G4OpticalSurface *aerogelAirOptical = new G4OpticalSurface("Cerenkov_groundOptical");

    G4OpticalSurface *specularReflectorOptical =
        new G4OpticalSurface("Cerenkov_specularReflectorOptical", glisur, polished, dielectric_metal);
    double specularReflectorEnergy[2] = {2.08 * eV, 5.90 * eV};
    double specularReflectorRefl[2] = {0.90, 0.90};
    G4MaterialPropertiesTable *specularReflectorOpticalMPT = new G4MaterialPropertiesTable();
    specularReflectorOpticalMPT->AddProperty("REFLECTIVITY", specularReflectorEnergy, specularReflectorRefl,
                                             2);
    specularReflectorOptical->SetMaterialPropertiesTable(specularReflectorOpticalMPT);

    G4OpticalSurface *photocathodeOptical =
        new G4OpticalSurface("Cerenkov_photocathodeOptical", glisur, polished, dielectric_metal);
    double photocathodeOpticalEnergy[15] = {2.08 * eV, 2.18 * eV, 2.23 * eV, 2.27 * eV, 2.36 * eV,
                                            2.41 * eV, 2.61 * eV, 2.95 * eV, 3.54 * eV, 3.87 * eV,
                                            4.35 * eV, 4.59 * eV, 5.17 * eV, 5.39 * eV, 5.90 * eV};
    double photocathodeOpticalRefl[15] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                          0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    double photocathodeOpticalEff[15] = {.03, .06, .09, .12, .15, .18, .21, .24,
                                         .22, .18, .15, .12, .09, .06, .03};
    G4MaterialPropertiesTable *photocathodeOpticalMPT = new G4MaterialPropertiesTable();
    photocathodeOpticalMPT->AddProperty("REFLECTIVITY", photocathodeOpticalEnergy, photocathodeOpticalRefl,
                                        15);
    photocathodeOpticalMPT->AddProperty("EFFICIENCY", photocathodeOpticalEnergy, photocathodeOpticalEff, 15);
    photocathodeOptical->SetMaterialPropertiesTable(photocathodeOpticalMPT);

    // TODO:	Begin geometry
    double aerogelWidth = 20 * cm;
    double aerogelHeight = 80 * cm;
    double aerogelDepth = 6 * cm;
    double windowThickness = 0.762 * mm;
    double wallThickness = 9.525 * 2.0 / 3.0 * mm;
    double tubeWallThickness = 15.875 * mm;
    double photocathodeWindowThickness = 1 * mm;
    double uniformityExtensionLength = 6 * cm;
    double airBoxDepth = 15.25 * cm;
    double shieldingLength = 47 * cm;
    double shieldingID = 13.6525 * cm;
    double shieldingThickness = 9.525 * mm;
    double lightGuideLength = 15 * cm;
    double lightGuideID = shieldingID - 2 * mm;
    double wedgeAngle = 35 * deg;
    double wedgeProtrusion = -5 * cm;  // tan(17.7527902 * deg) * airBoxDepth;
    bool showShielding = true;

    //	Make enclosing volume
    G4Box *cerenkovSolid = new G4Box(
        "Cerenkov_Solid",
        0.5 * aerogelWidth + uniformityExtensionLength + tubeWallThickness + shieldingLength + 10 * cm,
        0.5 * aerogelHeight + wallThickness + 10 * cm, 0.5 * (aerogelDepth + airBoxDepth) + wallThickness);

    G4LogicalVolume *cerenkovLog = new G4LogicalVolume(cerenkovSolid, air, "Cerenkov_Log");
    cerenkovLog->SetVisAttributes(G4VisAttributes(false, G4Color()));

    G4VPhysicalVolume *cerenkovPhys =
        new G4PVPlacement(0, G4ThreeVector(0, 0, 0), "Cerenkov_Phys", cerenkovLog, World_Phys, false, 0);
    (void)cerenkovPhys;

    //	Make aluminum housing
    G4Box *aerogelBoxSolid =
        new G4Box("Cerenkov_aerogelBoxSolid", 0.5 * aerogelWidth + wallThickness,
                  0.5 * aerogelHeight + wallThickness, 0.5 * (aerogelDepth + windowThickness));

    G4VSolid *airBoxSolid =
        new G4Box("Cerenkov_lightBoxSolid",
                  0.5 * (aerogelWidth + wallThickness + tubeWallThickness) + uniformityExtensionLength * 0.5,
                  0.5 * aerogelHeight + wallThickness, 0.5 * airBoxDepth + wallThickness);

    {
        G4Box *cornerCutSolid = new G4Box(
            "Cerenkov_cornerCutSolid", (aerogelWidth + wedgeProtrusion + wallThickness) / sin(wedgeAngle),
            aerogelHeight + 2.0 * wallThickness + 1 * cm,
            sin(wedgeAngle) * (aerogelWidth + wedgeProtrusion + wallThickness) - wallThickness);
        airBoxSolid =
            new G4SubtractionSolid("Cerenkov_airBoxSolid", airBoxSolid, cornerCutSolid,
                                   new G4RotationMatrix(90 * deg, wedgeAngle, -90 * deg),
                                   G4ThreeVector(0.5 * (aerogelWidth + wallThickness + tubeWallThickness) +
                                                     uniformityExtensionLength * 0.5,
                                                 0, airBoxDepth * 0.5));
    }

    G4Tubs *lightGuideSolid =
        new G4Tubs("Cerenkov_lightGuideSolid", 0, 0.5 * shieldingID,
                   0.5 * (lightGuideLength + photocathodeWindowThickness) + mm, 0, 360 * deg);

    G4BooleanSolid *mainBoxSolid;

    {
        mainBoxSolid = new G4UnionSolid(
            "Cerenkov_mainBoxSolid", airBoxSolid, aerogelBoxSolid, 0,
            G4ThreeVector(0.5 * (uniformityExtensionLength + tubeWallThickness - wallThickness), 0,
                          -0.5 * (aerogelDepth + airBoxDepth + windowThickness) - wallThickness + 0.05 * mm));

        for (int i = 0; i < 4; i++) {
            stringstream name;
            name << "Cerenkov_mainBoxSolid" << i;
            mainBoxSolid = new G4UnionSolid(
                name.str().c_str(), mainBoxSolid, lightGuideSolid,
                new G4RotationMatrix(90 * deg, 90 * deg, 0),
                G4ThreeVector(-0.5 * (aerogelWidth + lightGuideLength + photocathodeWindowThickness +
                                      wallThickness + tubeWallThickness + uniformityExtensionLength),
                              (i - 1.5) * aerogelHeight / 4.0, 0));
        }
    }

    G4LogicalVolume *mainBoxLog = new G4LogicalVolume(mainBoxSolid, Al, "Cerenkov_mainBoxLog");
    G4VisAttributes attr(true, G4Color::Grey());
    attr.SetForceSolid(false);
    mainBoxLog->SetVisAttributes(attr);

    G4VPhysicalVolume *mainBoxPhys;

    {
        mainBoxPhys = new G4PVPlacement(
            0, G4ThreeVector(-0.5 * (uniformityExtensionLength + tubeWallThickness - wallThickness), 0,
                             0.5 * aerogelDepth),
            mainBoxLog, "Cerenkov_mainBoxPhys", cerenkovLog, false, 0);
        (void)mainBoxPhys;
    }

    // Fill in aerogel
    G4Box *aerogelSolid =
        new G4Box("Cerenkov_aerogelSolid", 0.5 * aerogelWidth, 0.5 * aerogelHeight, 0.5 * aerogelDepth);

    G4LogicalVolume *aerogelLog = new G4LogicalVolume(aerogelSolid, aerogel, "Cerenkov_aerogelLog");
    G4VisAttributes attr2(true, G4Color::Cyan());
    attr2.SetForceSolid(true);
    aerogelLog->SetVisAttributes(attr2);

    G4VPhysicalVolume *aerogelPhys;

    {
        aerogelPhys = new G4PVPlacement(
            0, G4ThreeVector(0.5 * (uniformityExtensionLength + tubeWallThickness - wallThickness), 0,
                             -0.5 * (airBoxDepth + aerogelDepth) - wallThickness),
            aerogelLog, "Cerenkov_aerogelPhys", mainBoxLog, false, 0);
        (void)aerogelPhys;
    }

    G4LogicalBorderSurface *aerogelBoxSurface = new G4LogicalBorderSurface(
        "Cerenkov_aerogelBoxSurface", aerogelPhys, mainBoxPhys, diffuseReflectorOptical);
    (void)aerogelBoxSurface;

    // Fill in air inside counter
    G4Box *airConnectorSolid = new G4Box("Cerenkov_airConnectorSolid", 0.5 * aerogelWidth,
                                         0.5 * aerogelHeight, 0.5 * wallThickness + .05 * mm);

    G4VSolid *mainAirSolid =
        new G4Box("Cerenkov_mainAirSolid", 0.5 * aerogelWidth + uniformityExtensionLength * 0.5,
                  0.5 * aerogelHeight, 0.5 * airBoxDepth);
    {
        G4Box *cornerCutSolid =
            new G4Box("Cerenkov_cornerCutSolid", (aerogelWidth + wedgeProtrusion) / sin(wedgeAngle),
                      aerogelHeight + 1 * cm, sin(wedgeAngle) * (aerogelWidth + wedgeProtrusion));
        mainAirSolid = new G4SubtractionSolid(
            "Cerenkov_mainAirSolid", mainAirSolid, cornerCutSolid,
            new G4RotationMatrix(90 * deg, wedgeAngle, -90 * deg),
            G4ThreeVector(aerogelWidth * 0.5 + uniformityExtensionLength * 0.5, 0, airBoxDepth * 0.5));
    }

    G4Cons *airBevelSolid =
        new G4Cons("Cerenkov_airBevelSolid", 0, 0.5 * (lightGuideID + tubeWallThickness) + mm, 0,
                   0.5 * lightGuideID, 0.25 * tubeWallThickness + 0.5 * mm, 0, 360 * deg);

    G4BooleanSolid *airSolid;

    {
        airSolid = new G4UnionSolid(
            "Cerenkov_airSolid", mainAirSolid, airConnectorSolid, 0,
            G4ThreeVector(uniformityExtensionLength * 0.5, 0, -0.5 * (airBoxDepth + wallThickness)));

        for (int i = 0; i < /*6*/ 4; i++) {
            stringstream name;
            name << "Cerenkov_airSolid" << i;
            airSolid = new G4UnionSolid(
                name.str().c_str(), airSolid, airBevelSolid, new G4RotationMatrix(-90 * deg, 90 * deg, 0),
                G4ThreeVector(
                    -0.5 * (aerogelWidth + 0.5 * tubeWallThickness + uniformityExtensionLength - mm),
                    (i - 1.5) * aerogelHeight / 4.0, 0));
        }
    }

    G4LogicalVolume *airLog = new G4LogicalVolume(airSolid, air, "Cerenkov_airLog");
    attr2.SetColor(G4Color::White());
    airLog->SetVisAttributes(attr2);

    G4VPhysicalVolume *airPhys =
        new G4PVPlacement(0, G4ThreeVector(0.5 * (tubeWallThickness - wallThickness), 0, 0), airLog,
                          "Cerenkov_airPhys", mainBoxLog, false, 0);
    (void)airPhys;

    G4LogicalBorderSurface *airBoxSurface =
        new G4LogicalBorderSurface("Cerenkov_airBoxSurface", airPhys, mainBoxPhys, diffuseReflectorOptical);
    (void)airBoxSurface;

    G4LogicalBorderSurface *aerogelAirSurface =
        new G4LogicalBorderSurface("Cerenkov_aerogelAirSurface", aerogelPhys, airPhys, aerogelAirOptical);
    (void)aerogelAirSurface;

    // Make light guide air pockets / PMTs
    G4Tubs *lightGuideAirSolid = new G4Tubs(
        "Cerenkov_lightGuideAirSolid", 0, 0.5 * lightGuideID,
        0.5 * (lightGuideLength + photocathodeWindowThickness + 0.5 * tubeWallThickness + 1.05 * mm), 0,
        360 * deg);

    G4LogicalVolume *lightGuideAirLog =
        new G4LogicalVolume(lightGuideAirSolid, air, "Cerenkov_lightGuideAirLog");
    attr2.SetColor(G4Color::Grey());
    lightGuideAirLog->SetVisAttributes(attr2);

    G4Tubs *photocathodeWindowSolid = new G4Tubs("Cerenkov_photocathodeWindowSolid", 0, 0.5 * lightGuideID,
                                                 0.5 * (photocathodeWindowThickness + mm), 0, 360 * deg);

    ACEPMTDetector *cSensitiveDetector = new ACEPMTDetector("cerenkov/PMT");
    SDMan->AddNewDetector(cSensitiveDetector);

    G4LogicalVolume *photocathodeWindowLog = new G4LogicalVolume(
        photocathodeWindowSolid, glass, "Cerenkov_photocathodeWindowLog", 0, cSensitiveDetector);
    photocathodeWindowLog->SetVisAttributes(G4VisAttributes(true, G4Color(1.0, 0.7, 0.063)));

    G4Tubs *photocathodeSolid =
        new G4Tubs("Cerenkov_photocathodeSolid", 0, 0.5 * lightGuideID, 0.5 * mm, 0, 360 * deg);

    G4LogicalVolume *photocathodeLog = new G4LogicalVolume(photocathodeSolid, Al, "Cerenkov_photocathodeLog");
    G4VisAttributes photocathodeAttr(true, G4Color(0.6, 0.0, 0.2));
    photocathodeAttr.SetForceSolid(true);
    photocathodeLog->SetVisAttributes(photocathodeAttr);

    G4LogicalSkinSurface *photocathodeSurface =
        new G4LogicalSkinSurface("Cerenkov_photocathodeSurface", photocathodeLog, photocathodeOptical);
    (void)photocathodeSurface;

    G4VPhysicalVolume *photocathodePhys =
        new G4PVPlacement(0, G4ThreeVector(0, 0, 0.5 * photocathodeWindowThickness), photocathodeLog,
                          "Cerenkov_photocathodePhys", photocathodeWindowLog, false, 0);
    (void)photocathodePhys;

    G4VPhysicalVolume *photocathodeWindowPhys =
        new G4PVPlacement(0, G4ThreeVector(0, 0, 0.5 * lightGuideLength + 0.25 * tubeWallThickness),
                          photocathodeWindowLog, "Cerenkov_photocathodePhys", lightGuideAirLog, false, 0);
    (void)photocathodeWindowPhys;

    // Place light guide air pockets / PMTs
    {
        for (int i = 0; i < /*6*/ 4; i++) {
            stringstream name;
            name << "Cerenkov_lightGuideAirPhys" << i;
            G4VPhysicalVolume *lightGuideAirPhys = new G4PVPlacement(
                new G4RotationMatrix(-90 * deg, 90 * deg, 0),
                G4ThreeVector(
                    -0.5 * (aerogelWidth + 0.5 * tubeWallThickness + wallThickness + lightGuideLength +
                            photocathodeWindowThickness + uniformityExtensionLength + mm),
                    (i - 1.5) * aerogelHeight / 4.0, 0),
                lightGuideAirLog, name.str().c_str(), mainBoxLog, false, 0);

            name.str("");
            name << "Cerenkov_lightGuideAirBoxSurface" << i;
            G4LogicalBorderSurface *lightGuideAirBoxSurface = new G4LogicalBorderSurface(
                name.str().c_str(), lightGuideAirPhys, mainBoxPhys, specularReflectorOptical);
            (void)lightGuideAirBoxSurface;
        }
    }

    //	Make and place PMT shielding
    G4Tubs *shieldingTubeSolid =
        new G4Tubs("Cerenkov_shieldingTubeSolid", 0.5 * shieldingID, 0.5 * shieldingID + shieldingThickness,
                   0.5 * shieldingLength, 0, 360 * deg);

    G4Tubs *shieldingCapSolid =
        new G4Tubs("Cerenkov_shieldingCapSolid", 0, 0.5 * shieldingID + shieldingThickness,
                   0.5 * shieldingThickness, 0, 360 * deg);

    G4UnionSolid *shieldingSolid =
        new G4UnionSolid("Cerenkov_shieldingSolid", shieldingTubeSolid, shieldingCapSolid, 0,
                         G4ThreeVector(0, 0, 0.5 * (shieldingLength - shieldingThickness)));

    G4LogicalVolume *shieldingLog = new G4LogicalVolume(shieldingSolid, steel, "Cerenkov_shieldingLog");
    attr.SetColor(0.337254902, 0.647058824, 0.925490196);
    attr.SetVisibility(showShielding);
    shieldingLog->SetVisAttributes(attr);

    {
        for (int i = 0; i < 4; i++) {
            stringstream name;
            name << "Cerenkov_shieldingPhys" << i;
            G4VPhysicalVolume *shieldingPhys = new G4PVPlacement(
                new G4RotationMatrix(-90 * deg, 90 * deg, 0),
                G4ThreeVector(
                    -(0.5 * (aerogelWidth + shieldingLength) + uniformityExtensionLength + tubeWallThickness),
                    (i - 1.5) * aerogelHeight / 4.0, 0.5 * aerogelDepth),
                shieldingLog, name.str().c_str(), cerenkovLog, false, 0);
            (void)shieldingPhys;
        }
    }

    // Faux PMTs
    G4Tubs *PMTHeadSolid = new G4Tubs("Cerenkov_PMTHeadSolid", 0, 6.35 * cm, 3 * cm, 0, 360 * deg);

    G4Cons *PMTConeSolid =
        new G4Cons("Cerenkov_PMTConeSolid", 0, 4 * cm, 0, 6.35 * cm, 2 * cm + 0.5 * mm, 0, 360 * deg);

    G4Tubs *PMTNeckSolid = new G4Tubs("Cerenkov_PMTNeckSolid", 0, 4 * cm, 9 * cm + 0.5 * mm, 0, 360 * deg);

    G4UnionSolid *PMTHeadAndConeSolid = new G4UnionSolid("Cerenkov_PMTHeadAndConeSolid", PMTHeadSolid,
                                                         PMTConeSolid, 0, G4ThreeVector(0, 0, -(3 + 2) * cm));

    G4UnionSolid *PMTCompleteSolid =
        new G4UnionSolid("Cerenkov_PMTCompleteSolid", PMTHeadAndConeSolid, PMTNeckSolid, 0,
                         G4ThreeVector(0, 0, -(3 + 2 + 9) * cm));

    G4LogicalVolume *PMTLog = new G4LogicalVolume(PMTCompleteSolid, steel, "Cerenkov_PMTLog");
    attr2.SetColor(0.0823529412, 0.0196078431, 0.0901960784);
    PMTLog->SetVisAttributes(attr2);

    {
        for (int i = 0; i < 4; i++) {
            stringstream name;
            name << "Cerenkov_PMTPhys" << i;
            G4VPhysicalVolume *PMTPhys = new G4PVPlacement(
                new G4RotationMatrix(90 * deg, 90 * deg, 0),
                G4ThreeVector(-(0.5 * aerogelWidth + tubeWallThickness + uniformityExtensionLength +
                                lightGuideLength + 2 * mm + 3 * cm),
                              (i - 1.5) * aerogelHeight / 4.0, 0.5 * aerogelDepth),
                PMTLog, name.str().c_str(), cerenkovLog, false, 0);
            (void)PMTPhys;
        }
    }
}
