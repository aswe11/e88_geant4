//2016-09-23, uploaded by yokkaich
//2016-08-12, uploaded by nakai
//2015-09-05, uploaded by nakai
//2015-03-01, uploaded by yokkaich
//2015-01-05, modified by yokkaich
//2015-01-05, modified by yokkaich
//2014-08-27, modified by kawama
//2014-08-27, modified by kawama
//2014-04-30, modified by kawama
//2013-11-14, modified by kawama
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
// $Id: PrimaryGeneratorMessenger.cc,v 1.1 2010-10-18 15:56:17 maire Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include <G4UIdirectory.hh>
#include <G4UIcmdWithAString.hh>
#include <G4UIcmdWithAnInteger.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>

#include "E16G4_PrimaryGeneratorMessenger.hh"
#include "E16G4_PrimaryGeneratorAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

E16G4_PrimaryGeneratorMessenger::E16G4_PrimaryGeneratorMessenger(
                                          E16G4_PrimaryGeneratorAction* Gun)
:Action(Gun)
{
  //E16G4Dir = new G4UIdirectory("/E16G4/");
  //E16G4Dir->SetGuidance("UI commands of E16G4");
  
  gunDir = new G4UIdirectory("/E16G4/gun/");
  gunDir->SetGuidance("PrimaryGenerator control");
   
  RndmSeedCmd = new G4UIcmdWithAnInteger("/E16G4/gun/setRndmSeed",this);
  RndmSeedCmd->SetGuidance("Set random seed for the incident particle.");
  RndmSeedCmd->SetParameterName("RndmSeed",false);
  RndmSeedCmd->SetRange("RndmSeed>0");
  RndmSeedCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  GenIDCmd = new G4UIcmdWithAnInteger("/E16G4/gun/setGenID",this);
  GenIDCmd->SetGuidance("Set Generation ID for the incident particle.");
  GenIDCmd->SetParameterName("GenID",false);
  GenIDCmd->SetRange("GenID>=0");
  GenIDCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  MultiplicityCmd = new G4UIcmdWithAnInteger("/E16G4/gun/setMultiplicity",this);
  MultiplicityCmd->SetGuidance("Set Multiplicity, for GenID=7, 8");
  MultiplicityCmd->SetParameterName("Multiplicity",false);
  MultiplicityCmd->SetRange("Multiplicity>=0");
  MultiplicityCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  EvOffsetCmd = new G4UIcmdWithAnInteger("/E16G4/gun/setEvOffset",this);
  EvOffsetCmd->SetGuidance("Set EventOffset for the incident particle file.");
  EvOffsetCmd->SetParameterName("EvOffset",false);
  EvOffsetCmd->SetRange("EvOffset>=0");
  EvOffsetCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  SeedFileCmd = new G4UIcmdWithAString("/E16G4/gun/setSeedFile",this);
  SeedFileCmd->SetGuidance("Set Seed File for the incident particle.");
  SeedFileCmd->SetParameterName("SeedFile",true);
  SeedFileCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  XPointCmd = new G4UIcmdWithADoubleAndUnit("/E16G4/gun/setXPoint",this);
  XPointCmd->SetGuidance("Set X Coordinate of the particle generation point.");
  XPointCmd->SetParameterName("XPoint",true);
  XPointCmd->SetDefaultValue(0.);
  XPointCmd->SetDefaultUnit("cm");
  XPointCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  YPointCmd = new G4UIcmdWithADoubleAndUnit("/E16G4/gun/setYPoint",this);
  YPointCmd->SetGuidance("Set Y Coordinate of the particle generation point.");
  YPointCmd->SetParameterName("YPoint",true);
  YPointCmd->SetDefaultValue(0.);
  YPointCmd->SetDefaultUnit("cm");
  YPointCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  ZPointCmd = new G4UIcmdWithADoubleAndUnit("/E16G4/gun/setZPoint",this);
  ZPointCmd->SetGuidance("Set Z Coordinate of the particle generation point.");
  ZPointCmd->SetParameterName("ZPoint",true);
  ZPointCmd->SetDefaultValue(0.);
  ZPointCmd->SetDefaultUnit("cm");
  ZPointCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  SigmaXCmd = new G4UIcmdWithADoubleAndUnit("/E16G4/gun/setSigmaX",this);
  SigmaXCmd->SetGuidance("Set beam x width in sigma.[cm]");
  SigmaXCmd->SetParameterName("SigmaX",true);
  SigmaXCmd->SetDefaultValue(0.);
  SigmaXCmd->SetDefaultUnit("cm");
  SigmaXCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  SigmaYCmd = new G4UIcmdWithADoubleAndUnit("/E16G4/gun/setSigmaY",this);
  SigmaYCmd->SetGuidance("Set beam y width in sigma.[cm]");
  SigmaYCmd->SetParameterName("SigmaY",true);
  SigmaYCmd->SetDefaultValue(0.);
  SigmaYCmd->SetDefaultUnit("cm");
  SigmaYCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  SigmaZCmd = new G4UIcmdWithADoubleAndUnit("/E16G4/gun/setSigmaZ",this);
  SigmaZCmd->SetGuidance("Set beam z width in sigma.[cm]");
  SigmaZCmd->SetParameterName("SigmaZ",true);
  SigmaZCmd->SetDefaultValue(0.);
  SigmaZCmd->SetDefaultUnit("cm");
  SigmaZCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

 E16G4_PrimaryGeneratorMessenger::~E16G4_PrimaryGeneratorMessenger()
{
  delete RndmSeedCmd;
  delete GenIDCmd;
  delete EvOffsetCmd;
  delete SeedFileCmd;
  delete XPointCmd;
  delete YPointCmd;
  delete ZPointCmd;
  delete SigmaXCmd;
  delete SigmaYCmd;
  delete SigmaZCmd;
  delete gunDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void E16G4_PrimaryGeneratorMessenger::SetNewValue(
                                        G4UIcommand* command, G4String newValue)
{ 
  if( command == RndmSeedCmd )
   { Action->SetRndmSeed(RndmSeedCmd->GetNewIntValue(newValue));}
  if( command == GenIDCmd )
   { Action->SetGenID(GenIDCmd->GetNewIntValue(newValue));}
  if( command == MultiplicityCmd )
   { Action->SetRandomMulti(MultiplicityCmd->GetNewIntValue(newValue));}
  if( command == EvOffsetCmd )
   { Action->SetEvOffset(EvOffsetCmd->GetNewIntValue(newValue));}
  if( command == SeedFileCmd )
   { Action->SetSeedFile(newValue);}
  if( command == XPointCmd )
   { Action->SetXPoint(XPointCmd->GetNewDoubleValue(newValue));}
  if( command == YPointCmd )
   { Action->SetYPoint(YPointCmd->GetNewDoubleValue(newValue));}
  if( command == ZPointCmd )
   { Action->SetZPoint(ZPointCmd->GetNewDoubleValue(newValue));}
  if( command == SigmaXCmd )
   { Action->SetSigmaX(SigmaXCmd->GetNewDoubleValue(newValue));}
  if( command == SigmaYCmd )
   { Action->SetSigmaY(SigmaYCmd->GetNewDoubleValue(newValue));}
  if( command == SigmaZCmd )
   { Action->SetSigmaZ(SigmaZCmd->GetNewDoubleValue(newValue));}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

