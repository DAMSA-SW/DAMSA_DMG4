#include "ALPGunSteppingAction.hh"
#include "ALPGunDetectorConstruction.hh"

#include "G4RootAnalysisManager.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4INCLGlobals.hh"
#include "G4String.hh"
#include "ALPGunTrackingInfo.hh"

ALPGunSteppingAction::ALPGunSteppingAction()
: G4UserSteppingAction(),
  fScoringVolume1(0),
  fScoringVolume2(0)
{}

ALPGunSteppingAction::~ALPGunSteppingAction()
{}

void ALPGunSteppingAction::UserSteppingAction(const G4Step* step)
{
  /*
  fScoringVolume1 = logicWorld;
  fScoringVolume2 = logicDet;
  */

  auto analysisManager = G4RootAnalysisManager::Instance();
  // get volume of the current step
  if (!fScoringVolume1) { 
    const ALPGunDetectorConstruction* detectorConstruction
      = static_cast<const ALPGunDetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    fScoringVolume1 = detectorConstruction->GetScoringVolume1();   
    fScoringVolume2 = detectorConstruction->GetScoringVolume2();   
  }

  G4String preVolume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume()->GetName();
  if (!preVolume) return;  
  if (preVolume == "Target") {
    if (step->GetTrack()->GetParticleDefinition()->GetPDGEncoding() == 22) {
	    auto preStep = step->GetPreStepPoint();
	    analysisManager->FillH3(0, preStep->GetTotalEnergy()/MeV, preStep->GetMomentumDirection().theta(), preStep->GetPosition()[2]/mm);
    }		    
  }
}

