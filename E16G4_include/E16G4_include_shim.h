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

// 3. Geant4 11 smart-pointer touchable cast bypass
// Pre-include headers that declare GetTouchable() before defining the macro to avoid compiler declaration conflicts.
#include <G4StepPoint.hh>
#include <G4Step.hh>
#include <G4Track.hh>

#define GetTouchable() GetTouchableHandle()()

// 4. E88 Data Model and Geometry preprocessor redirections
// This maps legacy E16 types to modern E88 subclasses during E16G4 compilation.
#define E16ANA_G4OutputData E88_G4OutputData
#define E16ANA_G4Event E88_G4Event
#define E16ANA_G4Hit E88_G4Hit
#define E16ANA_GeometryV2 E88_GeometryV2

#endif // E16G4_INCLUDE_SHIM_H
