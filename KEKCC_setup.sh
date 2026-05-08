#!/bin/bash

# KEKCC Setup Script for E88 Simulation
# Updated with KEKCC-specific environment (G4 11.2, ROOT 6.32)

# User-specific E16 installation root at KEKCC
export E16SYS=${E16SYS:-/home/had/jws/e88/e88_geant4}

# ROOT Setup
source /sw/packages/root/6.32.04/bin/thisroot.sh

# Geant4 Setup
source /sw/packages/geant4/11.2.2/bin/geant4.sh
source /sw/packages/geant4/11.2.2/share/Geant4/geant4make/geant4make.sh

# G4 Working Directory
export G4WORKDIR=$HOME/work/geant4

# Experiments-specific environment (Placeholder for future porting of sub-libs)
export E16ANA_VERSION=2020-09-16
export E16G4_VERSION=2020-11-19

# Library Paths (Including project-local libs)
export LD_LIBRARY_PATH=${E16SYS}/lib64:${LD_LIBRARY_PATH}

# Project Bin Path
export PATH=${E16SYS}/bin:${PATH}

# Database/Calibration Root
export E16_DATABASE=${E16SYS}/database

# Tools
export PATH=/sw/packages/cmake3/bin:/group/had/sks/software/unpacker/e42/bin:${PATH}

echo "E88 KEKCC environment set up."
echo "ROOT: $(root-config --version)"
echo "Geant4: 11.2.2"
echo "E16SYS: $E16SYS"
