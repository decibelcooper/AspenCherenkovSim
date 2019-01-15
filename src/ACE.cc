#include "ACEDetectorConstruction.hh"
#include "ACEPhysicsList.hh"
#include "ACEMessenger.hh"
#include "ACEActionInitialization.hh"

#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include <G4SystemOfUnits.hh>
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "globals.hh"

#include "fstream"
using namespace std;

ACEMessenger *messenger = new ACEMessenger();

int main(int argc, char **argv) {
    int nThreads = 40;

#ifdef G4MULTITHREADED
    G4MTRunManager *runningMan = new G4MTRunManager;
    runningMan->SetNumberOfThreads(nThreads);
#else
    G4RunManager *runningMan = new G4RunManager;
#endif

    runningMan->SetUserInitialization(new ACEDetectorConstruction);
    runningMan->SetUserInitialization(new ACEPhysicsList);
    runningMan->SetUserInitialization(new ACEActionInitialization);

    G4VisManager *visMan = new G4VisExecutive();
    visMan->SetVerboseLevel(0);
    visMan->Initialize();

    runningMan->Initialize();

    G4UImanager *UIman = G4UImanager::GetUIpointer();

    // Give batch file path as single argument to ACE, and it will execute it,
    // otherwise, it will open a standard UI session

    if (argc > 1) {
        G4String batchPath = argv[1];
        UIman->ApplyCommand("/control/execute " + batchPath);

        if (argc > 2) {
            ofstream file(argv[2]);

            //	cout << messenger->GetAverageNumHits() << endl;

            //	int* spectrum = messenger->getSpectrum();
            //	for(int i = 0;i < 200;i++)
            //		file << i << "\t" << spectrum[i] << endl;

            double **spec = messenger->getSpaceSpectrum();
            for (int i = 0; i < messenger->getNumEvents(); i++) {
                file << spec[i][0] / cm << "\t" << spec[i][1] / cm << "\t" << spec[i][2] << endl;
            }

            file.close();
        }
    } else {
        G4UIExecutive *uiExec = new G4UIExecutive(argc, argv);
        UIman->ApplyCommand("/control/execute macro/QT_Init.mac");
        uiExec->SessionStart();
        delete uiExec;
    }

    delete visMan;
    delete runningMan;

    return 0;
}
