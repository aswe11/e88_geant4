#! /bin/csh -f

# KEKCC Setup Script for E88 Simulation (Csh version)
# Based on original CCJ E16setup.csh

# User: Please set this to the actual E16 installation root at KEKCC
if ( ${?E16SYS} ) then
else
   setenv E16SYS /group/e16/local
endif

# ROOT/Geant4 modules
if ( `where module` != "" ) then
    module load root/6.24.06
    module load geant4/9.5.1
endif

setenv ROOTSYS ${E16SYS}
setenv G4_INSTALL ${E16SYS}
setenv G4_DATA ${G4_INSTALL}/share/Geant4-9.5.1/data
setenv G4SYSTEM Linux-g++

setenv G4ABLDATA ${G4_DATA}/G4ABLA3.0
setenv G4LEVELGAMMADATA ${G4_DATA}/PhotonEvaporation2.2
setenv G4RADIOACTIVEDATA ${G4_DATA}/RadioactiveDecay3.4
setenv G4LEDATA ${G4_DATA}/G4EMLOW6.23
setenv G4NEUTRONHPDATA ${G4_DATA}/G4NDL4.0
setenv G4REALSURFACEDATA ${G4_DATA}/RealSurface1.0
setenv G4NEUTRONXSDATA ${G4_DATA}/G4NEUTRONXS1.1
setenv G4PIIDATA ${G4_DATA}/G4PII1.3

setenv E16ANA_VERSION 2020-09-16
setenv E16G4_VERSION 2020-11-19

if ( ${?LD_LIBRARY_PATH} ) then
   setenv LD_LIBRARY_PATH ${E16SYS}/lib64:${E16SYS}/E16ANA/${E16ANA_VERSION}/lib64:${E16SYS}/E16G4/${E16G4_VERSION}/lib64:${LD_LIBRARY_PATH}
else
   setenv LD_LIBRARY_PATH ${E16SYS}/lib64:${E16SYS}/E16ANA/${E16ANA_VERSION}/lib64:${E16SYS}/E16G4/${E16G4_VERSION}/lib64
endif

if ( ${?PATH} ) then
   setenv PATH ${E16SYS}/bin:${PATH}
else
   setenv PATH ${E16SYS}/bin
endif

setenv E16_DATABASE ${E16SYS}/database

echo "E88 KEKCC environment set up (csh)."
echo "E16SYS: $E16SYS"
