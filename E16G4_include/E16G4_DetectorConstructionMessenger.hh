//2016-09-23, uploaded by yokkaich
//2016-08-12, uploaded by nakai
//2015-01-05, uploaded by yokkaich
//2015-01-05, uploaded by yokkaich
//2014-08-27, uploaded by kawama
//2014-08-27, uploaded by kawama
//2014-04-30, uploaded by kawama
//2013-11-14, uploaded by kawama
//2013-05-13, modified by kawama
//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: DetectorConstructionMessenger.hh,v 1.1 2010-10-18 15:56:17 maire Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//E16G4_DetectorConstructionMessenger.hh 150105 
//    Last modified at <>
//

#ifndef E16G4_DetectorConstructionMessenger_h
#define E16G4_DetectorConstructionMessenger_h 1

#include <G4UImessenger.hh>
#include <globals.hh>

class E16G4_DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class E16G4_DetectorConstructionMessenger: public G4UImessenger
{
public:
  E16G4_DetectorConstructionMessenger(E16G4_DetectorConstruction*);
  virtual ~E16G4_DetectorConstructionMessenger();
    
  void SetNewValue(G4UIcommand*, G4String);
    
private:
  E16G4_DetectorConstruction* Const;
  G4UIdirectory*          E16G4Dir; 
  G4UIdirectory*          detDir; 
  G4UIcmdWithAnInteger*     MaterialFlagCmd;
  G4UIcmdWithAString*     Tar1Cmd;
  G4UIcmdWithAString*     Tar2Cmd;
  G4UIcmdWithAString*     Tar3Cmd;
  G4UIcmdWithAString*     Tar4Cmd;
  G4UIcmdWithAString*     Tar5Cmd;
  G4UIcmdWithAString*     Tar6Cmd;
  G4UIcmdWithAString*     Tar7Cmd;
  G4UIcmdWithADoubleAndUnit*     TarThick1Cmd;
  G4UIcmdWithADoubleAndUnit*     TarThick2Cmd;
  G4UIcmdWithADoubleAndUnit*     TarThick3Cmd;
  G4UIcmdWithADoubleAndUnit*     TarThick4Cmd;
  G4UIcmdWithADoubleAndUnit*     TarThick5Cmd;
  G4UIcmdWithADoubleAndUnit*     TarThick6Cmd;
  G4UIcmdWithADoubleAndUnit*     TarThick7Cmd;
  G4UIcmdWithoutParameter* UpdateCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif//E16G4_DetectorConstructionMessenger_h


