#include "ALPGunRunAction.hh"
#include "ALPGunPrimaryGeneratorAction.hh"
#include "ALPGunDetectorConstruction.hh"
#include "ALPGunRun.hh"

#include "G4RootAnalysisManager.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4GenericMessenger.hh"

#include <math.h>

ALPGunRunAction::ALPGunRunAction()
: G4UserRunAction()
{
  auto analysisManager = G4RootAnalysisManager::Instance();
}

ALPGunRunAction::~ALPGunRunAction()
{}

G4Run* ALPGunRunAction::GenerateRun()
{
  return new ALPGunRun;
}

void ALPGunRunAction::BeginOfRunAction(const G4Run*)
{ 
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);
  auto analysisManager = G4RootAnalysisManager::Instance();
  analysisManager->OpenFile();
  G4cout << "Using " << analysisManager->GetType() << G4endl;
  analysisManager->SetVerboseLevel(1);

  const ALPGunDetectorConstruction* detectorConstruction
   = static_cast<const ALPGunDetectorConstruction*>
     (G4RunManager::GetRunManager()->GetUserDetectorConstruction());


  std::vector<G4double> eAxis;
  for (G4double i = 1E0; i < 10E3; i *= pow(10,0.1)) {eAxis.push_back(i);}
  std::vector<G4double> tAxis;
  for (G4double i = 1E-6; i < 1.5; i *= pow(10,0.1)) {tAxis.push_back(i);}
  G4double targetL = detectorConstruction->GetTargetLength()/mm;
  std::vector<G4double> targetAxis;
  for (G4double i = 0.; i < targetL+1.; i += 1.){targetAxis.push_back(i);}
  analysisManager->CreateH3("t_photon_E_Theta_Z", "#gamma E [MeV], Theta [rad.], and Z [mm]",eAxis, tAxis, targetAxis);


}

void ALPGunRunAction::EndOfRunAction(const G4Run* run)
{
  auto analysisManager = G4RootAnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile(); 
}

