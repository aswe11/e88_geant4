// E88_PhysicsList.hh
// Geant4 11.2.2 compatible PhysicsList for the E88 simulation.
// Replaces the legacy E16G4_PhysicsList which depended on
// headers removed in Geant4 10.x and later.
//
// Physics: EM standard + decay + FTFP_BERT hadronic (default)
// with a runtime /physics/addPhysics messenger command for switching.
//
// 2026 - written for KEKCC porting

#ifndef E88_PhysicsList_HH
#define E88_PhysicsList_HH

#include <G4VModularPhysicsList.hh>
#include <globals.hh>
#include <vector>

class G4VPhysicsConstructor;

class E88_PhysicsList : public G4VModularPhysicsList
{
public:
  E88_PhysicsList();
  virtual ~E88_PhysicsList();

  void ConstructParticle() override;
  void ConstructProcess() override;
  void SetCuts() override;

  // Optionally switch EM or hadronic sub-list at runtime
  void AddPhysicsList(const G4String& name);
  void List();

  void SetCutForGamma(G4double);
  void SetCutForElectron(G4double);
  void SetCutForPositron(G4double);
  void SetCutForProton(G4double);

private:
  G4double cutForGamma;
  G4double cutForElectron;
  G4double cutForPositron;
  G4double cutForProton;

  G4VPhysicsConstructor* emPhysicsList;
  G4VPhysicsConstructor* decayPhysicsList;
  std::vector<G4VPhysicsConstructor*> hadronPhys;
};

#endif // E88_PhysicsList_HH
