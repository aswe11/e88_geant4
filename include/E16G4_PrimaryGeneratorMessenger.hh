//2016-09-23, uploaded by yokkaich
//2016-08-12, uploaded by nakai
//2015-09-05, uploaded by nakai
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
// $Id: PrimaryGeneratorMessenger.hh,v 1.1 2010-10-18 15:56:17 maire Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//E16G4_PrimaryGeneratorMessenger.hh 150105 
//    Last modified at <>
//

#ifndef E16G4_PrimaryGeneratorMessenger_h
#define E16G4_PrimaryGeneratorMessenger_h 1

#include <G4UImessenger.hh>
#include <globals.hh>

class E16G4_PrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class E16G4_PrimaryGeneratorMessenger: public G4UImessenger
{
public:
  E16G4_PrimaryGeneratorMessenger(E16G4_PrimaryGeneratorAction*);
  virtual ~E16G4_PrimaryGeneratorMessenger();
    
  void SetNewValue(G4UIcommand*, G4String);
    
private:
  E16G4_PrimaryGeneratorAction* Action;
  G4UIdirectory*          E16G4Dir;
  G4UIdirectory*          gunDir; 
  G4UIcmdWithAnInteger*     RndmSeedCmd;
  G4UIcmdWithAnInteger*     GenIDCmd;
  G4UIcmdWithAnInteger*     MultiplicityCmd;
  G4UIcmdWithAnInteger*     EvOffsetCmd;
  G4UIcmdWithAString*     SeedFileCmd;
  G4UIcmdWithADoubleAndUnit*     XPointCmd;
  G4UIcmdWithADoubleAndUnit*     YPointCmd;
  G4UIcmdWithADoubleAndUnit*     ZPointCmd;
  G4UIcmdWithADoubleAndUnit*     SigmaXCmd;
  G4UIcmdWithADoubleAndUnit*     SigmaYCmd;
  G4UIcmdWithADoubleAndUnit*     SigmaZCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif// E16G4_PrimaryGeneratorMessenger_h

