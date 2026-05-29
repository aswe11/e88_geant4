# E88 CCJ → KEKCC Migration Project

This project ports the E88 simulation and analysis framework from RIKEN CCJ to KEKCC.

## KEKCC Environment Setup

Before building or running the simulation, source the setup script appropriate for your shell:

### For Bash/Zsh users:
```bash
source KEKCC_setup.sh
```

### For Csh/Tcsh users:
```csh
source KEKCC_setup.csh
```

## Building the Project

To compile the project, run:
```bash
make clean
make -j2
```

## Running the Simulation

To run the simulation:
```bash
./E16G4.stat ./macro/pion-simple.mac
```

## Compilation Mockups & Shims

To preserve 100% of the original legacy CCJ code, compile-time mockups and compatibility shims are used to bypass Geant4 11, ROOT 6, and C++17 compiler compatibility blocks. 

Detailed documentation regarding these shims and mock files is available here:
- **Mockup & Shim Guide**: [LOGS/MOCKUP_INFO.md](file:///Users/JWS/Desktop/E88/E88_simulation/LOGS/MOCKUP_INFO.md)
