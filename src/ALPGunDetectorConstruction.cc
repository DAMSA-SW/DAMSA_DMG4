#include "ALPGunDetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "ALPGunRunAction.hh"
ALPGunDetectorConstruction::ALPGunDetectorConstruction()
: G4VUserDetectorConstruction(),
  fScoringVolume1(0),
  fScoringVolume2(0),
  targetLength(0.)
{
  messenger = new G4GenericMessenger(this, "/detector/", "Detector properties");
  messenger->DeclareProperty("targetMaterial", m_target_mat)
        .SetGuidance("Set target material")
        .SetStates(G4State_PreInit, G4State_Idle);

  messenger->DeclarePropertyWithUnit("targetLength","cm", m_target_length)
        .SetGuidance("Set target length")
        .SetStates(G4State_PreInit, G4State_Idle);
}

ALPGunDetectorConstruction::~ALPGunDetectorConstruction()
{
  delete messenger;
}

G4VPhysicalVolume* ALPGunDetectorConstruction::Construct()
{  
    G4cout << "build Geometry" << G4endl; 
    // === Parameters ===
    targetLength = m_target_length;

    // === Materials ===
    G4NistManager* nist = G4NistManager::Instance();
    G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
    G4Material* target_mat = nist->FindOrBuildMaterial(m_target_mat);

    // === World Volume ===
    G4double worldSizeZ = targetLength+0.1 * mm;
    G4Box* solidWorld = new G4Box("World", 2.51 * cm, 2.51 * cm, worldSizeZ);
    G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, world_mat, "World");
    G4VPhysicalVolume* physWorld = new G4PVPlacement(0,
                                                     G4ThreeVector(),
                                                     logicWorld,
                                                     "World",
                                                     0,
                                                     false,
                                                     0,
                                                     true);

 
    G4Box* solidTarget =
      new G4Box("Target",                    //its name
          2.5*cm, 2.5*cm, targetLength*0.5); //its size
  
    G4LogicalVolume* logicTarget =
      new G4LogicalVolume(solidTarget,            //its solid
                          target_mat,             //its material
                          "Target");         //its name
  
    G4VPhysicalVolume* phyTarget =
      new G4PVPlacement(0,                       //no rotation
                      G4ThreeVector(0,0, targetLength*0.5),         //at (0,0,0)
                      logicTarget,                //its logical volume
                      "Target",              //its name
                      logicWorld,              //its mother  volume
                      false,                   //no boolean operation
                      0,                       //copy number
                      true);          //overlaps checking
  
    fScoringVolume1 = logicWorld;
    fScoringVolume2 = logicTarget;
 
    return physWorld;
}

