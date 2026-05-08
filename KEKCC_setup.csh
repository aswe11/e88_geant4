#! /bin/csh -f

# KEKCC Setup Script for E88 Simulation (Csh version)
# Updated with KEKCC-specific environment (G4 11.2, ROOT 6.32)

# User-specific E16 installation root at KEKCC
if ( ${?E16SYS} ) then
else
   setenv E16SYS /home/had/jws/e88/e88_geant4
endif

# ROOT Setup
source /sw/packages/root/6.32.04/bin/thisroot.csh

# Geant4 Setup
source /sw/packages/geant4/11.2.2/bin/geant4.csh
source /sw/packages/geant4/11.2.2/share/Geant4/geant4make/geant4make.csh

# G4 Working Directory
setenv G4WORKDIR $HOME/work/geant4

# Experiment-specific environment
setenv E16ANA_VERSION 2020-09-16
setenv E16G4_VERSION 2020-11-19

# Library Paths
if ( ${?LD_LIBRARY_PATH} ) then
   setenv LD_LIBRARY_PATH ${E16SYS}/lib64:${LD_LIBRARY_PATH}
else
   setenv LD_LIBRARY_PATH ${E16SYS}/lib64
endif

# Path
if ( ${?PATH} ) then
   setenv PATH ${E16SYS}/bin:${PATH}
else
   setenv PATH ${E16SYS}/bin
endif

# Database/Calibration Root
setenv E16_DATABASE ${E16SYS}/database

# Tools
setenv PATH /sw/packages/cmake3/bin:/group/had/sks/software/unpacker/e42/bin:${PATH}

echo "E88 KEKCC environment set up (csh)."
echo "ROOT: `root-config --version`"
echo "Geant4: 11.2.2"
echo "E16SYS: $E16SYS"
