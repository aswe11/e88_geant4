#!/bin/bash

# KEKCC Setup Script for E88 Simulation
# Based on original CCJ E16setup.csh

# User: Please set this to the actual E16 installation root at KEKCC
# e.g., /group/e16/local
export E16SYS=${E16SYS:-/group/e16/local}

# ROOT Setup (Assumes 'module' system is available at KEKCC)
# If not using modules, source the root-config path manually
if command -v module &> /dev/null; then
    module load root/6.24.06 # Example version
    module load geant4/9.5.1  # Essential for physics consistency
fi

export ROOTSYS=${E16SYS}
export G4_INSTALL=${E16SYS}
export G4_DATA=${G4_INSTALL}/share/Geant4-9.5.1/data

# Geant4 Data Paths
export G4ABLDATA=${G4_DATA}/G4ABLA3.0
export G4LEVELGAMMADATA=${G4_DATA}/PhotonEvaporation2.2
export G4RADIOACTIVEDATA=${G4_DATA}/RadioactiveDecay3.4
export G4LEDATA=${G4_DATA}/G4EMLOW6.23
export G4NEUTRONHPDATA=${G4_DATA}/G4NDL4.0
export G4REALSURFACEDATA=${G4_DATA}/RealSurface1.0
export G4NEUTRONXSDATA=${G4_DATA}/G4NEUTRONXS1.1
export G4PIIDATA=${G4_DATA}/G4PII1.3

# Library Paths
# Using 2020-09-16 versions as in original CCJ setup
export E16ANA_VERSION=2020-09-16
export E16G4_VERSION=2020-11-19

export LD_LIBRARY_PATH=${E16SYS}/lib64:${E16SYS}/E16ANA/${E16ANA_VERSION}/lib64:${E16SYS}/E16G4/${E16G4_VERSION}/lib64:${LD_LIBRARY_PATH}

# Path
export PATH=${E16SYS}/bin:${PATH}

# Database/Calibration Root (Used in KEKCC config files)
export E16_DATABASE=${E16SYS}/database

echo "E88 KEKCC environment set up."
echo "E16SYS: $E16SYS"
