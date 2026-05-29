// E88_PhysicsList.cc
// Geant4 11.2.2 compatible PhysicsList for the E88 simulation.
// Replaces the legacy E16G4_PhysicsList.
//
// Default physics: FTFP_BERT (recommended for high-energy hadron simulations)
// EM: G4EmStandardPhysics_option3 (accurate EM, suitable for tracking detectors)
// Stopping: G4StoppingPhysics
// Ion: G4IonPhysics
//
// 2026 - written for KEKCC porting

// --- EM Physics ---
#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option1.hh"
#include "G4EmStandardPhysics_option2.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4EmExtraPhysics.hh"

// --- Decay ---
#include "G4DecayPhysics.hh"

// --- Hadronic (Geant4 10+ header names, all prefixed G4) ---
#include "G4HadronElasticPhysics.hh"
#include "G4HadronElasticPhysicsXS.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4HadronPhysicsFTFP_BERT.hh"
#include "G4HadronPhysicsFTFP_BERT_HP.hh"
#include "G4HadronPhysicsQGSP_BERT.hh"
#include "G4HadronPhysicsQGSP_BIC.hh"
#include "G4HadronPhysicsQGSP_BIC_AllHP.hh"
#include "G4HadronInelasticQBBC.hh"

// --- Stopping & Ion ---
#include "G4StoppingPhysics.hh"
#include "G4IonPhysics.hh"
#include "G4IonINCLXXPhysics.hh"

// --- Neutron ---
#include "G4NeutronTrackingCut.hh"

// --- Process utilities ---
#include "G4LossTableManager.hh"
#include "G4ProcessManager.hh"
#include "G4StepLimiter.hh"
#include "G4UserSpecialCuts.hh"

// --- Particle definitions ---
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Proton.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "E88_PhysicsList.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

E88_PhysicsList::E88_PhysicsList()
  : G4VModularPhysicsList(),
    cutForGamma(0.7*mm), cutForElectron(0.7*mm),
    cutForPositron(0.7*mm), cutForProton(0.7*mm)
{
  G4LossTableManager::Instance();
  defaultCutValue = 0.7*mm;
  SetVerboseLevel(1);

  // Decay physics (particles + decays)
  decayPhysicsList = new G4DecayPhysics("decays");

  // Default EM: option3 is the recommended standard for tracking detectors
  emPhysicsList = new G4EmStandardPhysics_option3();

  // Default hadronic: FTFP_BERT is the Geant4 collaboration recommended
  // list for high-energy physics (>= Geant4 10.x)
  hadronPhys.push_back(new G4EmExtraPhysics());
  hadronPhys.push_back(new G4HadronElasticPhysics());
  hadronPhys.push_back(new G4StoppingPhysics());
  hadronPhys.push_back(new G4IonPhysics());
  hadronPhys.push_back(new G4NeutronTrackingCut());
  hadronPhys.push_back(new G4HadronPhysicsFTFP_BERT());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

E88_PhysicsList::~E88_PhysicsList()
{
  delete decayPhysicsList;
  delete emPhysicsList;
  for (auto& p : hadronPhys) { delete p; }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void E88_PhysicsList::ConstructParticle()
{
  decayPhysicsList->ConstructParticle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void E88_PhysicsList::ConstructProcess()
{
  AddTransportation();

  emPhysicsList->ConstructProcess();
  decayPhysicsList->ConstructProcess();

  for (auto& p : hadronPhys) { p->ConstructProcess(); }

  // Step limiter for all charged particles
  // Geant4 11: use GetParticleIterator() instead of theParticleIterator
  auto particleIterator = GetParticleIterator();
  particleIterator->reset();
  while ((*particleIterator)()) {
    G4ParticleDefinition* particle = particleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    if (particle->GetPDGCharge() != 0.0) {
      pmanager->AddDiscreteProcess(new G4StepLimiter());
      pmanager->AddDiscreteProcess(new G4UserSpecialCuts());
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void E88_PhysicsList::SetCuts()
{
  if (verboseLevel > 0) {
    G4cout << "E88_PhysicsList::SetCuts: CutLength = "
           << G4BestUnit(defaultCutValue, "Length") << G4endl;
  }
  SetCutValue(cutForGamma,    "gamma");
  SetCutValue(cutForElectron, "e-");
  SetCutValue(cutForPositron, "e+");
  SetCutValue(cutForProton,   "proton");

  if (verboseLevel > 0) { DumpCutValuesTable(); }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void E88_PhysicsList::SetCutForGamma(G4double cut)
{
  cutForGamma = cut;
  SetParticleCuts(cutForGamma, G4Gamma::Gamma());
}

void E88_PhysicsList::SetCutForElectron(G4double cut)
{
  cutForElectron = cut;
  SetParticleCuts(cutForElectron, G4Electron::Electron());
}

void E88_PhysicsList::SetCutForPositron(G4double cut)
{
  cutForPositron = cut;
  SetParticleCuts(cutForPositron, G4Positron::Positron());
}

void E88_PhysicsList::SetCutForProton(G4double cut)
{
  cutForProton = cut;
  SetParticleCuts(cutForProton, G4Proton::Proton());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void E88_PhysicsList::AddPhysicsList(const G4String& name)
{
  if (verboseLevel > 0) {
    G4cout << "E88_PhysicsList::AddPhysicsList: <" << name << ">" << G4endl;
  }

  // EM physics switching
  if (name == "emstandard" || name == "emstandard_opt0") {
    delete emPhysicsList;
    emPhysicsList = new G4EmStandardPhysics();
  } else if (name == "emstandard_opt1") {
    delete emPhysicsList;
    emPhysicsList = new G4EmStandardPhysics_option1();
  } else if (name == "emstandard_opt2") {
    delete emPhysicsList;
    emPhysicsList = new G4EmStandardPhysics_option2();
  } else if (name == "emstandard_opt3") {
    delete emPhysicsList;
    emPhysicsList = new G4EmStandardPhysics_option3();
  } else if (name == "emstandard_opt4") {
    delete emPhysicsList;
    emPhysicsList = new G4EmStandardPhysics_option4();
  } else if (name == "emlivermore") {
    delete emPhysicsList;
    emPhysicsList = new G4EmLivermorePhysics();
  } else if (name == "empenelope") {
    delete emPhysicsList;
    emPhysicsList = new G4EmPenelopePhysics();

  // Hadronic physics switching
  // (clears existing hadronic list and rebuilds)
  } else if (name == "FTFP_BERT") {
    for (auto& p : hadronPhys) { delete p; }
    hadronPhys.clear();
    hadronPhys.push_back(new G4EmExtraPhysics());
    hadronPhys.push_back(new G4HadronElasticPhysics());
    hadronPhys.push_back(new G4StoppingPhysics());
    hadronPhys.push_back(new G4IonPhysics());
    hadronPhys.push_back(new G4NeutronTrackingCut());
    hadronPhys.push_back(new G4HadronPhysicsFTFP_BERT());
  } else if (name == "FTFP_BERT_HP") {
    for (auto& p : hadronPhys) { delete p; }
    hadronPhys.clear();
    hadronPhys.push_back(new G4EmExtraPhysics());
    hadronPhys.push_back(new G4HadronElasticPhysicsHP());
    hadronPhys.push_back(new G4StoppingPhysics());
    hadronPhys.push_back(new G4IonPhysics());
    hadronPhys.push_back(new G4NeutronTrackingCut());
    hadronPhys.push_back(new G4HadronPhysicsFTFP_BERT_HP());
  } else if (name == "QGSP_BERT") {
    for (auto& p : hadronPhys) { delete p; }
    hadronPhys.clear();
    hadronPhys.push_back(new G4EmExtraPhysics());
    hadronPhys.push_back(new G4HadronElasticPhysics());
    hadronPhys.push_back(new G4StoppingPhysics());
    hadronPhys.push_back(new G4IonPhysics());
    hadronPhys.push_back(new G4NeutronTrackingCut());
    hadronPhys.push_back(new G4HadronPhysicsQGSP_BERT());
  } else if (name == "QGSP_BIC") {
    for (auto& p : hadronPhys) { delete p; }
    hadronPhys.clear();
    hadronPhys.push_back(new G4EmExtraPhysics());
    hadronPhys.push_back(new G4HadronElasticPhysicsXS());
    hadronPhys.push_back(new G4StoppingPhysics());
    hadronPhys.push_back(new G4IonPhysics());
    hadronPhys.push_back(new G4NeutronTrackingCut());
    hadronPhys.push_back(new G4HadronPhysicsQGSP_BIC());
  } else {
    G4cout << "E88_PhysicsList::AddPhysicsList: <" << name
           << "> is not defined." << G4endl;
    List();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void E88_PhysicsList::List()
{
  G4cout << "### Available Physics Lists for E88_PhysicsList:" << G4endl;
  G4cout << "  EM  : emstandard emstandard_opt1-4 emlivermore empenelope" << G4endl;
  G4cout << "  HAD : FTFP_BERT FTFP_BERT_HP QGSP_BERT QGSP_BIC" << G4endl;
}
