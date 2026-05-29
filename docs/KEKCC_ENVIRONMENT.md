# KEKCC Environment

## Purpose
This document records the actual KEKCC runtime/build environment used for the E88 migration.
The KEKCC environment differs from the original CCJ environment, which used older compilers and libraries.

---

## Geant4
- **Version:** 11.2.2
- **Setup Scripts:**
  - `source /sw/packages/geant4/11.2.2/bin/geant4.sh`
  - `source /sw/packages/geant4/11.2.2/share/Geant4/geant4make/geant4make.sh`
- **Working Directory:** `export G4WORKDIR=$HOME/work/geant4`

---

## ROOT
- **Version:** 6.32.04
- **Setup Script:** `source /sw/packages/root/6.32.04/bin/thisroot.sh`
- **Environment Variables:**
  - `ROOTSYS=/sw/packages/root/6.32.04`
  - PATH and LD_LIBRARY_PATH are updated by the setup script.

---

## CMake
- **Path:** `/sw/packages/cmake3`
- **Binary Path:** `/sw/packages/cmake3/bin`

---

## Other Tools
- **Unpacker:** `/group/had/sks/software/unpacker/e42/bin`

---

## Technical Implications
- **Geant4 Jump:** The jump from Geant4 9.5.1 (CCJ) to 11.2.2 (KEKCC) is a major version gap. 
- **Compatibility:** Expect significant API changes in Geant4 classes (e.g., `G4VUserPrimaryGeneratorAction`, `G4VUserDetectorConstruction`, etc.).
- **Build System:** The original `makefile` may need adaptation to handle Geant4 11.x multi-threading or library name changes.
