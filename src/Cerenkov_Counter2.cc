/*
 * Cerenkov_Counter2.cc
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

#include "Cerenkov_Counter2.hh"

Cerenkov_Counter2::Cerenkov_Counter2() { ; }

Cerenkov_Counter2::~Cerenkov_Counter2() { ; }

void Cerenkov_Counter2::Build(G4VPhysicalVolume *World_Phys) {
    //	Start setting up materials to be involved in the counter's construction
    G4NistManager *nistMan = G4NistManager::Instance();

    G4Material *air = nistMan->FindOrBuildMaterial("G4_AIR");
    G4MaterialPropertiesTable *airMPT = new G4MaterialPropertiesTable();
    double airEnergy[2] = {3.10 * eV, 4.13 * eV};
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
    double aerogelAbsLen = 100 * cm;
    double aerogelScatLen = 7 * cm;
    double aerogelIndex = 1.026;
    double aerogelEnergy[2] = {3.10 * eV, 4.13 * eV};
    double aerogelRIndex[2] = {aerogelIndex, aerogelIndex};
    double aerogelAbsorp[2] = {aerogelAbsLen, aerogelAbsLen};
    double aerogelRayleigh[2] = {aerogelScatLen, aerogelScatLen};
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
    double glassEnergy[2] = {3.10 * eV, 4.13 * eV};
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
    double reflectorRefl = 0.92;
    double diffuseReflectorEnergy[2] = {3.10 * eV, 4.13 * eV};
    double diffuseReflectorRefl[2] = {reflectorRefl, reflectorRefl};
    G4MaterialPropertiesTable *diffuseReflectorOpticalMPT = new G4MaterialPropertiesTable();
    diffuseReflectorOpticalMPT->AddProperty("REFLECTIVITY", diffuseReflectorEnergy, diffuseReflectorRefl, 2);
    diffuseReflectorOptical->SetMaterialPropertiesTable(diffuseReflectorOpticalMPT);
    diffuseReflectorOptical->SetPolish(0.0);

    G4OpticalSurface *aerogelAirOptical = new G4OpticalSurface("Cerenkov_groundOptical");

    G4OpticalSurface *specularReflectorOptical =
        new G4OpticalSurface("Cerenkov_specularReflectorOptical", glisur, polished, dielectric_metal);
    double specularReflectorEnergy[2] = {3.10 * eV, 4.13 * eV};
    double specularReflectorRefl[2] = {0.92, 0.92};
    G4MaterialPropertiesTable *specularReflectorOpticalMPT = new G4MaterialPropertiesTable();
    specularReflectorOpticalMPT->AddProperty("REFLECTIVITY", specularReflectorEnergy, specularReflectorRefl,
                                             2);
    specularReflectorOptical->SetMaterialPropertiesTable(specularReflectorOpticalMPT);

    G4OpticalSurface *photocathodeOptical =
        new G4OpticalSurface("Cerenkov_photocathodeOptical", glisur, polished, dielectric_metal);
    double detectorEff = 0.22;
    double photocathodeOpticalEnergy[2] = {3.10 * eV, 4.13 * eV};
    double photocathodeOpticalRefl[2] = {0.0, 0.0};
    double photocathodeOpticalEff[2] = {detectorEff, detectorEff};

    G4OpticalSurface *perfectReflectorOptical =
        new G4OpticalSurface("Cerenkov_perfectReflectorOptical", glisur, polished, dielectric_metal);

    G4MaterialPropertiesTable *photocathodeOpticalMPT = new G4MaterialPropertiesTable();
    photocathodeOpticalMPT->AddProperty("REFLECTIVITY", photocathodeOpticalEnergy, photocathodeOpticalRefl,
                                        2);
    photocathodeOpticalMPT->AddProperty("EFFICIENCY", photocathodeOpticalEnergy, photocathodeOpticalEff, 2);
    photocathodeOptical->SetMaterialPropertiesTable(photocathodeOpticalMPT);

    // TODO:	Begin geometry
    double aerogelWidth = 20.32 * cm;
    double aerogelHeight = 20.32 * cm;
    double aerogelDepth = 6 * cm;
    double aerogelBoxDepth = 6.2 * cm;
    double windowThickness = 0.762 * mm;
    double wallThickness = 9.525 * mm;
    double tubeWallThickness = 9.525 * 5 / 3 * mm;
    double photocathodeWindowThickness = 1 * mm;
    double uniformityExtensionLength = 8 * cm;
    double airBoxDepth = 20 * cm;
    double shieldingLength = 43.18 * cm;
    double tubeID = 13.6525 * cm;
    double tubeThickness = 9.525 * mm;
    double shieldingThickness = 1.016 * mm;
    double lightGuideLength = 15 * cm;
    double lightGuideID = tubeID - 1 * mm;
    bool showShielding = true;

    //	Make enclosing volume
    G4Box *cerenkovSolid = new G4Box(
        "Cerenkov_Solid",
        0.5 * aerogelWidth + uniformityExtensionLength + tubeWallThickness + shieldingLength + 10 * cm,
        0.5 * aerogelHeight + wallThickness + 10 * cm, 0.5 * (aerogelBoxDepth + airBoxDepth) + wallThickness);

    G4LogicalVolume *cerenkovLog = new G4LogicalVolume(cerenkovSolid, air, "Cerenkov_Log");
    cerenkovLog->SetVisAttributes(G4VisAttributes(false, G4Color()));

    G4VPhysicalVolume *cerenkovPhys =
        new G4PVPlacement(0, G4ThreeVector(0, 0, 0), "Cerenkov_Phys", cerenkovLog, World_Phys, false, 0);
    (void)cerenkovPhys;

    //	Make aluminum housing
    G4Box *aerogelBoxSolid = new G4Box("Cerenkov_aerogelBoxSolid", 0.5 * aerogelWidth + wallThickness,
                                       0.5 * aerogelHeight + wallThickness,
                                       0.5 * (aerogelBoxDepth - wallThickness + windowThickness));

    G4VSolid *airBoxSolid = new G4Box(
        "Cerenkov_lightBoxSolid", 0.5 * aerogelWidth + tubeWallThickness + uniformityExtensionLength,
        0.5 * aerogelHeight + wallThickness, 0.5 * (airBoxDepth + wallThickness + windowThickness));

    G4Tubs *lightGuideSolid =
        new G4Tubs("Cerenkov_lightGuideSolid", 0, 0.5 * tubeID,
                   0.5 * (lightGuideLength + photocathodeWindowThickness) + mm, 0, 360 * deg);

    G4BooleanSolid *mainBoxSolid;

    {
        mainBoxSolid = new G4UnionSolid(
            "Cerenkov_mainBoxSolid", airBoxSolid, aerogelBoxSolid, 0,
            G4ThreeVector(0, 0, -0.5 * (aerogelBoxDepth + airBoxDepth + 2 * windowThickness) + 0.05 * mm));

        for (int i = 0; i < 2; i++) {
            stringstream name;
            name << "Cerenkov_mainBoxSolid" << i;
            mainBoxSolid = new G4UnionSolid(
                name.str().c_str(), mainBoxSolid, lightGuideSolid,
                new G4RotationMatrix(90 * deg, 90 * deg, 0),
                G4ThreeVector(pow((double)-1, i) * (0.5 * (aerogelWidth + lightGuideLength +
                                                           photocathodeWindowThickness + mm) +
                                                    uniformityExtensionLength + tubeWallThickness),
                              0, 0.5 * (wallThickness - windowThickness)));
        }
    }

    G4LogicalVolume *mainBoxLog = new G4LogicalVolume(mainBoxSolid, Al, "Cerenkov_mainBoxLog");

    G4VisAttributes attr(true, G4Color::Grey());
    attr.SetForceSolid(false);
    mainBoxLog->SetVisAttributes(attr);

    G4VPhysicalVolume *mainBoxPhys;

    {
        mainBoxPhys = new G4PVPlacement(0, G4ThreeVector(0, 0, 0.5 * aerogelBoxDepth), mainBoxLog,
                                        "Cerenkov_mainBoxPhys", cerenkovLog, false, 0);
        (void)mainBoxPhys;
    }

    // Fill in air inside counter
    G4Box *airConnectorSolid = new G4Box("Cerenkov_airConnectorSolid", 0.5 * aerogelWidth,
                                         0.5 * aerogelHeight, 0.5 * aerogelBoxDepth);

    G4VSolid *mainAirSolid =
        new G4Box("Cerenkov_mainAirSolid", 0.5 * aerogelWidth + uniformityExtensionLength,
                  0.5 * aerogelHeight, 0.5 * airBoxDepth);

    G4Cons *airBevelSolid =
        new G4Cons("Cerenkov_airBevelSolid", 0, 0.5 * (lightGuideID + tubeWallThickness) + mm, 0,
                   0.5 * lightGuideID, 0.25 * tubeWallThickness + 0.5 * mm, 0, 360 * deg);

    G4BooleanSolid *airSolid;

    {
        airSolid = new G4UnionSolid("Cerenkov_airSolid", mainAirSolid, airConnectorSolid, 0,
                                    G4ThreeVector(0, 0, -0.5 * (airBoxDepth + aerogelBoxDepth)));

        for (int i = 0; i < 2; i++) {
            stringstream name;
            name << "Cerenkov_airSolid" << i;
            airSolid = new G4UnionSolid(
                name.str().c_str(), airSolid, airBevelSolid,
                new G4RotationMatrix(pow((double)-1, i) * 90 * deg, 90 * deg, 0),
                G4ThreeVector(pow((double)-1, i) * (0.5 * aerogelWidth + 0.25 * tubeWallThickness +
                                                    uniformityExtensionLength - 0.5 * mm),
                              0, 0));
        }
    }

    G4LogicalVolume *airLog = new G4LogicalVolume(airSolid, air, "Cerenkov_airLog");
    G4VisAttributes attr2(true, G4Color::Grey());
    attr2.SetForceSolid(false);
    airLog->SetVisAttributes(attr2);

    G4VPhysicalVolume *airPhys =
        new G4PVPlacement(0, G4ThreeVector(0, 0, 0.5 * (wallThickness - windowThickness)), airLog,
                          "Cerenkov_airPhys", mainBoxLog, false, 0);
    (void)airPhys;

    G4LogicalBorderSurface *airBoxSurface =
        new G4LogicalBorderSurface("Cerenkov_airBoxSurface", airPhys, mainBoxPhys, diffuseReflectorOptical);
    (void)airBoxSurface;

    // Fill in aerogel
    G4Box *aerogelSolid =
        new G4Box("Cerenkov_aerogelSolid", 0.5 * aerogelWidth, 0.5 * aerogelHeight, 0.5 * aerogelDepth);

    G4LogicalVolume *aerogelLog = new G4LogicalVolume(aerogelSolid, aerogel, "Cerenkov_aerogelLog");
    attr2.SetColor(G4Color::Cyan());
    attr2.SetForceSolid(false);
    aerogelLog->SetVisAttributes(attr2);

    G4VPhysicalVolume *aerogelPhys;

    {
        aerogelPhys = new G4PVPlacement(0, G4ThreeVector(0, 0, -0.5 * (airBoxDepth + aerogelDepth)),
                                        aerogelLog, "Cerenkov_aerogelPhys", airLog, false, 0);
        (void)aerogelPhys;
    }

    G4Box *outlineSolid = new G4Box("Cerenkov_outlineSolid", 0.5 * 17.5 * cm, 0.5 * 17.5 * cm, 0.5 * 1 * mm);

    G4LogicalVolume *outlineLog = new G4LogicalVolume(outlineSolid, aerogel, "Cerenkov_outlineLog");
    attr2.SetColor(G4Color::Red());
    attr2.SetForceSolid(false);
    outlineLog->SetVisAttributes(attr2);

    G4LogicalBorderSurface *aerogelBoxSurface = new G4LogicalBorderSurface(
        "Cerenkov_aerogelBoxSurface", aerogelPhys, mainBoxPhys, diffuseReflectorOptical);
    (void)aerogelBoxSurface;

    G4LogicalBorderSurface *aerogelAirSurface =
        new G4LogicalBorderSurface("Cerenkov_aerogelAirSurface", aerogelPhys, airPhys, aerogelAirOptical);
    (void)aerogelAirSurface;

    //  Make Al obstacle
    G4Box *obstacleMainSolid =
        new G4Box("Cerenkov_obstacleMainSolid", .5 * aerogelWidth, .5 * aerogelHeight, .5 * mm);
    G4Box *obstacleSubSolid = new G4Box("Cerenkov_obstacleSubSolid", .5 * (.5 * aerogelWidth - .5 * cm),
                                        .5 * (aerogelHeight - .5 * cm), mm);

    G4BooleanSolid *obstacleSolid =
        new G4SubtractionSolid("Cerenkov_obstacleSolid", obstacleMainSolid, obstacleSubSolid, 0,
                               G4ThreeVector(-.25 * aerogelWidth, 0, 0));
    obstacleSolid = new G4SubtractionSolid("Cerenkov_obstacleSolid", obstacleSolid, obstacleSubSolid, 0,
                                           G4ThreeVector(.25 * aerogelWidth, 0, 0));

    G4LogicalVolume *obstacleLog = new G4LogicalVolume(obstacleSolid, Al, "Cerenkov_obstacleLog");

    G4VisAttributes attr3(true, G4Color::Grey());
    attr3.SetForceSolid(true);
    obstacleLog->SetVisAttributes(attr3);

    G4VPhysicalVolume *obstaclePhys =
        new G4PVPlacement(0, G4ThreeVector(0, 0, -0.5 * (airBoxDepth - mm)), "Cerenkov_obstaclePhys",
                          obstacleLog, airPhys, false, 0);
    (void)obstaclePhys;

    G4LogicalBorderSurface *obstacleAirSurface = new G4LogicalBorderSurface(
        "Cerenkov_obstacleAirSurface", airPhys, obstaclePhys, specularReflectorOptical);
    (void)obstacleAirSurface;

    G4LogicalBorderSurface *obstacleAerogelSurface = new G4LogicalBorderSurface(
        "Cerenkov_obstacleAerogelSurface", aerogelPhys, obstaclePhys, specularReflectorOptical);
    (void)obstacleAerogelSurface;

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

    G4LogicalVolume *photocathodeWindowLog =
        new G4LogicalVolume(photocathodeWindowSolid, glass, "Cerenkov_photocathodeWindowLog");
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

    G4LogicalBorderSurface *windowBoxSurface = new G4LogicalBorderSurface(
        "Cerenkov_windowBoxSurface", photocathodeWindowPhys, mainBoxPhys, perfectReflectorOptical);
    (void)windowBoxSurface;

    // Place light guide air pockets / PMTs
    {
        for (int i = 0; i < 2; i++) {
            stringstream name;
            name << "Cerenkov_lightGuideAirPhys" << i;
            G4VPhysicalVolume *lightGuideAirPhys = new G4PVPlacement(
                new G4RotationMatrix(pow((double)-1, i) * 90 * deg, 90 * deg, 0),
                G4ThreeVector(
                    pow((double)-1, i) * (0.5 * (aerogelWidth + 1.5 * tubeWallThickness + lightGuideLength +
                                                 photocathodeWindowThickness + mm) +
                                          uniformityExtensionLength),
                    0, 0.5 * (wallThickness - windowThickness)),
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
        new G4Tubs("Cerenkov_shieldingTubeSolid", 0.5 * tubeID + tubeThickness,
                   0.5 * tubeID + tubeThickness + shieldingThickness, 0.5 * shieldingLength, 0, 360 * deg);

    G4UnionSolid *shieldingSolid = (G4UnionSolid *)shieldingTubeSolid;

    G4LogicalVolume *shieldingLog = new G4LogicalVolume(shieldingSolid, steel, "Cerenkov_shieldingLog");
    attr.SetColor(0.337254902, 0.647058824, 0.925490196);
    attr.SetVisibility(showShielding);
    shieldingLog->SetVisAttributes(attr);

    {
        for (int i = 0; i < 2; i++) {
            stringstream name;
            name << "Cerenkov_shieldingPhys" << i;
            G4VPhysicalVolume *shieldingPhys = new G4PVPlacement(
                new G4RotationMatrix(pow((double)-1, i) * 90 * deg, 90 * deg, 0),
                G4ThreeVector(pow((double)-1, i) * (0.5 * (aerogelWidth + shieldingLength) +
                                                    uniformityExtensionLength + tubeWallThickness),
                              0, 0.5 * (aerogelBoxDepth + wallThickness - windowThickness)),
                shieldingLog, name.str().c_str(), cerenkovLog, false, 0);
            (void)shieldingPhys;
        }
    }

    // Faux PMTs
    G4Tubs *PMTHeadSolid = new G4Tubs("Cerenkov_PMTHeadSolid", 0, 6.5 * cm, 3.4 * cm, 0, 360 * deg);

    G4Cons *PMTConeSolid =
        new G4Cons("Cerenkov_PMTConeSolid", 0, 4.1 * cm, 0, 6.5 * cm, 2.2 * cm + 0.5 * mm, 0, 360 * deg);

    G4Tubs *PMTNeckSolid =
        new G4Tubs("Cerenkov_PMTNeckSolid", 0, 4.1 * cm, 5.4 * cm + 0.5 * mm, 0, 360 * deg);

    G4UnionSolid *PMTHeadAndConeSolid =
        new G4UnionSolid("Cerenkov_PMTHeadAndConeSolid", PMTHeadSolid, PMTConeSolid, 0,
                         G4ThreeVector(0, 0, -(3.4 + 2.2) * cm));

    G4UnionSolid *PMTCompleteSolid =
        new G4UnionSolid("Cerenkov_PMTCompleteSolid", PMTHeadAndConeSolid, PMTNeckSolid, 0,
                         G4ThreeVector(0, 0, -(3.4 + 2.2 + 5.4) * cm));

    G4LogicalVolume *PMTLog = new G4LogicalVolume(PMTCompleteSolid, steel, "Cerenkov_PMTLog");
    attr2.SetColor(0.0823529412, 0.0196078431, 0.0901960784);
    PMTLog->SetVisAttributes(attr2);

    {
        for (int i = 0; i < 2; i++) {
            stringstream name;
            name << "Cerenkov_PMTPhys" << i;
            G4VPhysicalVolume *PMTPhys =
                new G4PVPlacement(new G4RotationMatrix(pow((double)-1, i + 1) * 90 * deg, 90 * deg, 0),
                                  G4ThreeVector(pow((double)-1, i) * (0.5 * aerogelWidth + tubeWallThickness +
                                                                      uniformityExtensionLength +
                                                                      lightGuideLength + 2 * mm + 3 * cm),
                                                0, 0.5 * (aerogelBoxDepth + wallThickness - windowThickness)),
                                  PMTLog, name.str().c_str(), cerenkovLog, false, 0);
            (void)PMTPhys;
        }
    }
}
