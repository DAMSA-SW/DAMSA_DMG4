#ifndef ALPGunDetectorConstruction_h
#define ALPGunDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4GenericMessenger.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;

class ALPGunDetectorConstruction : public G4VUserDetectorConstruction
{
  private:
    G4GenericMessenger* messenger;
    G4double targetLength;
    G4double m_target_length;
    G4String m_target_mat;
          
  public:
    ALPGunDetectorConstruction();
    virtual ~ALPGunDetectorConstruction();
    
    virtual G4VPhysicalVolume* Construct();
    
    G4LogicalVolume* GetScoringVolume1() const { return fScoringVolume1; }
    G4LogicalVolume* GetScoringVolume2() const { return fScoringVolume2; }
    G4double GetTargetLength() const { return targetLength;}

  protected: 
    
    G4LogicalVolume*  fScoringVolume1;
    G4LogicalVolume*  fScoringVolume2;

};

#endif

