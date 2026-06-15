#ifndef E16G4_INCLUDE_SHIM_H
#define E16G4_INCLUDE_SHIM_H

// 1. C++ Standard namespace and streams injection
#include <fstream>
#include <iostream>
using std::ifstream;
using std::ofstream;
using std::ios;

// 2. ROOT Form and standard string support
#include <TString.h>

// Geant4 system units support
#include <G4SystemOfUnits.hh>

// 3. Geant4 11 smart-pointer touchable cast bypass
// Pre-include headers that declare GetTouchable() before defining the macro to avoid compiler declaration conflicts.
#include <G4StepPoint.hh>
#include <G4Step.hh>
#include <G4Track.hh>

#define GetTouchable() GetTouchableHandle()()

// 4. E88 Data Model and Geometry preprocessor redirections
// Pre-include modern E88 headers first
#include "E88_GeometryV2.hh"
#include "E88_G4OutputData.hh"
#include "E88G4_VD.hh"
#include "E88G4_VDCylinder.hh"

// Block legacy E16 headers from loading via their include guards
#define E16ANA_GeometryV2_hh
#define E16ANA_G4OutputData_HH
#define E16G4_VD_h
#define E16G4_VDCylinder_h

// This maps legacy E16 types to modern E88 subclasses during E16G4 compilation.
#define E16ANA_G4OutputData E88_G4OutputData
#define E16ANA_G4Event E88_G4Event
#define E16ANA_G4Hit E88_G4Hit
#define E16ANA_GeometryV2 E88_GeometryV2
#define E16ANA_G4Trajectory E88_G4Trajectory
#define E16G4_VD E88G4_VD
#define E16G4_VDCylinder E88G4_VDCylinder

// 5. Compatibility shims to build legacy E16 sources without modifications
#include <G4MaterialPropertiesTable.hh>
class E16G4_MaterialPropertyVectorWrapper {
private:
  G4MaterialPropertyVector* vec;
public:
  E16G4_MaterialPropertyVectorWrapper(G4MaterialPropertyVector* v) : vec(v) {}
  E16G4_MaterialPropertyVectorWrapper* operator->() { return this; }
  void SetSpline(bool val) {}
  operator G4MaterialPropertyVector*() const { return vec; }
};

class E16G4_MaterialPropertiesTable : public G4MaterialPropertiesTable {
public:
  E16G4_MaterialPropertyVectorWrapper AddProperty(const G4String& key, G4double* energies, G4double* values, G4int numEntries) {
    G4MaterialPropertyVector* v = G4MaterialPropertiesTable::AddProperty(key, energies, values, numEntries, true);
    return E16G4_MaterialPropertyVectorWrapper(v);
  }
  E16G4_MaterialPropertyVectorWrapper AddProperty(const G4String& key, const std::vector<G4double>& energies, const std::vector<G4double>& values) {
    G4MaterialPropertyVector* v = G4MaterialPropertiesTable::AddProperty(key, energies, values, true);
    return E16G4_MaterialPropertyVectorWrapper(v);
  }
};
#define G4MaterialPropertiesTable E16G4_MaterialPropertiesTable

#endif // E16G4_INCLUDE_SHIM_H
