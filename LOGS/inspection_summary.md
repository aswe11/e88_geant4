# Phase 1: Inspection Summary

This document summarizes the findings from the inspection of the E88 simulation framework in the CCJ environment.

## 1. CCJ-Specific Assumptions

### Environment Variables
- `E16SYS`: Hardcoded to `/ccj/u/E16`. This is the root for almost all dependencies.
- `ROOTSYS`, `G4_INSTALL`: Both point to `${E16SYS}`.
- `G4_DATA`: Points to `${E16SYS}/share/Geant4-9.5.1/data`.
- `LD_LIBRARY_PATH`: Includes specific versioned directories like `E16ANA/2020-09-16` and `E16G4/2020-11-19`.

### Hardcoded Paths
- **`param/E16CalibFiles-local.cfg`**: Contains many absolute paths to `/ccj/u/E16/database/`.
  - Field Map: `/ccj/u/E16/database/fieldmap/Bmap_201207010_mod.binary`
  - Geometry Data: `/ccj/u/sako/E16G4E88/geometry_202304_run0e.dat`
  - Analyzer Parameters: `/ccj/u/E16/database/analysis/gtr/gtr_parameters.dat`
- **`makefile`**: References `$(E16SYS)` for includes and libraries.

## 2. Dependencies

| Dependency | Version / Path | Note |
| :--- | :--- | :--- |
| **Geant4** | 9.5.1 | Very old version. Essential for physics reproducibility. |
| **ROOT** | In `E16SYS` | Likely an older version (e.g., ROOT 5 or early ROOT 6). |
| **CLHEP** | `G4clhep`, `HepPDT` | Provided within the Geant4/E16 installation. |
| **E16ANA** | `2020-09-16` | Internal analysis library. Must be available at KEKCC. |
| **Garfield++** | `$E16SYS/src/garfpp` | Optional but mentioned in setup. |

## 3. Potential Migration Issues

### Physics Safety (Priority #1)
- The use of Geant4 9.5.1 is a major constraint. If KEKCC only has newer versions, we may need to compile 9.5.1 from source to ensure physics behavior is preserved.
- Static linking (`-static`) in the makefile may fail if KEKCC's system libraries aren't compatible or available in static form.

### Path Resolution
- The absolute paths in `E16CalibFiles-local.cfg` must be abstracted. We should use environment variables (e.g., `$E16_DATABASE`) to point to these files.

### Compiler Differences
- CCJ uses an "old compiler". We need to check if modern `g++` at KEKCC can handle the old code without major modifications (Priority #5).

## 4. Immediate Next Steps (Phase 2)
1.  Verify the available ROOT and Geant4 versions at KEKCC.
2.  Propose a strategy for handling the `E16ANA` internal library.
3.  Design a path abstraction layer for calibration files.
