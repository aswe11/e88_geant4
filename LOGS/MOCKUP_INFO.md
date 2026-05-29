# E88 Compilation Mockups & Shims Guide

To achieve 100% original code preservation for legacy CCJ simulation (`E16G4`) and analysis (`E16ANA`) libraries, this project bypasses all modern compiler (C++17), Geant4 11, and ROOT 6 compatibility blocks purely at the compile-time level using lightweight shims and mock headers. 

These mocks are located in **`E16G4_include/`** and **`E16ANA_include/`** and are automatically injected by the `makefile` using the compiler `-include` flag.

---

## 1. E16G4 (Simulation) Mockups & Compatibility Shims
Located in: **`E16G4_include/`**

### 1.1 `E16G4_include_shim.h`
An automated redirection shim injected during the compilation of all original `E16G4` simulation sources (`E16G4_src/*.cc`).
*   **Geant4 11 smart-pointer touchable cast bypass**: Redefines the deprecated touchable cast using `#define GetTouchable() GetTouchableHandle()()`, resolving modern Geant4 smart-pointer Cast exceptions.
*   **Active E88 Data Model redirection**: Automatically redirects legacy data objects to E88-specific active targets at preprocessor time:
    ```cpp
    #define E16ANA_G4OutputData E88_G4OutputData
    #define E16ANA_G4Event E88_G4Event
    #define E16ANA_G4Hit E88_G4Hit
    #define E16ANA_GeometryV2 E88_GeometryV2
    ```
*   **Geant4 Header Isolation**: To prevent macro collisions inside Geant4's own headers, the shim pre-includes Geant4's internal `G4StepPoint.hh`, `G4Step.hh`, and `G4Track.hh` before the macro definition is applied.

### 1.2 `G4VReadOutGeometry.hh`
*   **Problem**: Modern Geant4 versions (v10+) removed the legacy readout geometry header entirely.
*   **Solution**: Provided a dummy empty header `G4VReadOutGeometry.hh` inside the include search path to satisfy include statements in legacy files without modifying their code.

---

## 2. E16ANA (Analysis) Mockups & Compatibility Shims
Located in: **`E16ANA_include/`**

### 2.1 `E16ANA_namespace_shim.h`
An automated namespace and standard unit injector used for compiling `E16ANA` analysis files (`E16ANA_src/*.cc`).
*   **C++ Standard streams injection**: Resolves standard library namespace issues by injecting standard qualifiers (`using std::ifstream`, `ofstream`, etc.) where they were omitted in legacy code.
*   **CLHEP units support**: Injects `using namespace CLHEP` and transitively pulls standard units like `mm` and `GeV`.
*   **ROOT 6 `TSpectrum` Deconvolution Wrapper**: ROOT 6 upgraded `TSpectrum::Deconvolution` signature to double pointers (`double*`), causing compiler blocks against the legacy analysis float array types. The shim provides a `MockTSpectrum` class that intercepts and seamlessly translates float arrays to double arrays on the fly.

### 2.2 HepPID and HepPDT Mocking
*   **Problem**: KEKCC lacks the legacy `HepPID`/`HepPDT` package libraries, which are required for particle name conversions.
*   **Solution**: Implemented local mock headers `HepPID/ParticleName.hh` and `HepPDT/ParticleData.hh` inside `E16ANA_include/` that intercept `HepPID::particleName(pID)` and dynamically resolve it using ROOT's built-in `TDatabasePDG` database.
