#! /bin/csh -f

setenv CCJ_JOBTMP sako

if(${?E16SYS})then
else
   setenv E16SYS /ccj/u/E16
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

if(${?PATH})then
   setenv PATH ${E16SYS}/bin:/opt/ccj/bin:${PATH}
else
   setenv PATH ${E16SYS}/bin:/opt/ccj/bin
endif

if(${?LD_LIBRARY_PATH})then
   setenv LD_LIBRARY_PATH ${E16SYS}/lib64:${E16SYS}/E16ANA/2020-09-16/lib64:${E16SYS}/E16G4/2020-11-19/lib64:${LD_LIBRARY_PATH}
#   setenv LD_LIBRARY_PATH ${E16SYS}/lib64:${E16SYS}/E16ANA/2021-02-27/lib64:${E16SYS}/E16G4/pro/lib64:${LD_LIBRARY_PATH}
#   setenv LD_LIBRARY_PATH ${E16SYS}/lib64:${E16SYS}/E16ANA/pro/lib64:${E16SYS}/E16G4/pro/lib64:${LD_LIBRARY_PATH}
#   setenv LD_LIBRARY_PATH ${E16SYS}/lib64:${E16SYS}/E16ANA/2020-08-30/lib64:${E16SYS}/E16G4/pro/lib64:${LD_LIBRARY_PATH}
else
   setenv LD_LIBRARY_PATH ${E16SYS}/lib64:${E16SYS}/E16ANA/2020-09-16/lib64:${E16SYS}/E16G4/2020-11-19/lib64
#   setenv LD_LIBRARY_PATH ${E16SYS}/lib64:${E16SYS}/E16ANA/2021-02-27/lib64:${E16SYS}/E16G4/pro/lib64
#   setenv LD_LIBRARY_PATH ${E16SYS}/lib64:${E16SYS}/E16ANA/pro/lib64:${E16SYS}/E16G4/pro/lib64
#   setenv LD_LIBRARY_PATH ${E16SYS}/lib64:${E16SYS}/E16ANA/2020-08-30/lib64:${E16SYS}/E16G4/pro/lib64
endif

setenv DAWN_PS_PREVIEWER "NONE"

setenv SVN_EDITOR vim
setenv E16REPO file://$E16SYS/svnroot

setenv GARFIELD_HOME $E16SYS/src/garfpp
