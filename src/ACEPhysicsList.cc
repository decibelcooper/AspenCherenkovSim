#include "ACEPhysicsList.hh"
// ConstructParticle()
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
// ConstructProcess()
#include "G4ProcessManager.hh"
// ConstructDecay()
#include "G4Decay.hh"
// ConstrucEM()
#include "G4Cerenkov.hh"
#include "G4Scintillation.hh"
#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4eMultipleScattering.hh"
#include "G4hIonisation.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"
#include "G4MuMultipleScattering.hh"
#include "G4hMultipleScattering.hh"
#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"
#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpBoundaryProcess.hh"
// Birks Correction to Scintillation
#include "G4LossTableManager.hh"
#include "G4EmSaturation.hh"

ACEPhysicsList::ACEPhysicsList() : QGSP_BERT() { ; }

ACEPhysicsList::~ACEPhysicsList() { ; }

void ACEPhysicsList::ConstructParticle() { QGSP_BERT::ConstructParticle(); }

void ACEPhysicsList::ConstructProcess() {
    QGSP_BERT::ConstructProcess();
    ConstructOp();
}

void ACEPhysicsList::ConstructOp() {
    G4Cerenkov *cerenkovProcess = new G4Cerenkov("Cerenkov");
    G4Scintillation *scintillationProcess = new G4Scintillation("Scintillation");
    G4OpAbsorption *absorptionProcess = new G4OpAbsorption();
    G4OpRayleigh *rayleighProcess = new G4OpRayleigh();
    G4OpBoundaryProcess *boundProcess = new G4OpBoundaryProcess();

    // Cerenkov Processs
    //	cerenkovProcess->SetMaxNumPhotonsPerStep(1);
    //	cerenkovProcess->SetMaxBetaChangePerStep(.001);
    //	cerenkovProcess->SetTrackSecondariesFirst(false);

    // Scintillation
    scintillationProcess->SetScintillationYieldFactor(1.);
    scintillationProcess->SetTrackSecondariesFirst(true);

    // Birks Correction to Scintillation Process
    G4EmSaturation *emSaturation = G4LossTableManager::Instance()->EmSaturation();
    scintillationProcess->AddSaturation(emSaturation);

    // Boundary Process
    //	G4OpticalSurfaceModel themodel = unified;
    //	boundProcess->SetModel(themodel);

    GetParticleIterator()->reset();
    while ((*GetParticleIterator())()) {
        G4ParticleDefinition *particle = GetParticleIterator()->value();
        G4ProcessManager *pmanager = particle->GetProcessManager();
        G4String particleName = particle->GetParticleName();
        if (cerenkovProcess->IsApplicable(*particle)) {
            pmanager->AddProcess(cerenkovProcess);
            pmanager->SetProcessOrdering(cerenkovProcess, idxPostStep);
        }
        if (scintillationProcess->IsApplicable(*particle)) {
            pmanager->AddProcess(scintillationProcess);
            pmanager->SetProcessOrderingToLast(scintillationProcess, idxAtRest);
            pmanager->SetProcessOrderingToLast(scintillationProcess, idxPostStep);
        }
        if (particleName == "opticalphoton") {
            pmanager->AddDiscreteProcess(absorptionProcess);
            pmanager->AddDiscreteProcess(rayleighProcess);
            pmanager->AddDiscreteProcess(boundProcess);
        }
    }
}
