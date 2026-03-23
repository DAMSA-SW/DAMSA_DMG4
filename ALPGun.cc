#include "ALPGunDetectorConstruction.hh"
#include "ALPGunActionInitialization.hh"
#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "Randomize.hh"
#include "FTFP_BERT_HP.hh" 

int main(int argc,char** argv)
{
  G4UIExecutive* ui = nullptr;
  if (argc == 1) {
    ui = new G4UIExecutive(argc, argv);
  }

  // Random engine
  G4Random::setTheEngine(new CLHEP::RanecuEngine);
  G4Random::setTheSeed(12345);

#ifdef G4MULTITHREADED
  auto runManager = new G4MTRunManager;
  runManager->SetNumberOfThreads(4);
#else
  auto runManager = new G4RunManager;
#endif

  // Initialization
  runManager->SetUserInitialization(new ALPGunDetectorConstruction());
  G4cout << "start main2" << G4endl;
  runManager->SetUserInitialization(new FTFP_BERT_HP());
  runManager->SetUserInitialization(new ALPGunActionInitialization());

  // Visualization only if interactive
  G4VisManager* visManager = nullptr;
  if (ui) {
    visManager = new G4VisExecutive;
    visManager->Initialize();
  }

  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  if (!ui) {
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command + fileName);
  } else {
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
  }

  delete visManager;
  delete runManager;

  return 0;
}
