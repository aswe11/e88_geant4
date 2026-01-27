//2016-09-23, uploaded by yokkaich
//2016-08-12, uploaded by nakai
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
// $Id: DetectorConstructionMessenger.cc,v 1.1 2010-10-18 15:56:17 maire Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "E16G4_DetectorConstructionMessenger.hh"
#include "E16G4_DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

E16G4_DetectorConstructionMessenger::E16G4_DetectorConstructionMessenger(
                                          E16G4_DetectorConstruction* Con)
:Const(Con)
{
  E16G4Dir = new G4UIdirectory("/E16G4/");
  E16G4Dir->SetGuidance("UI commands of E16G4");
  
  detDir = new G4UIdirectory("/E16G4/det/");
  detDir->SetGuidance("DetectorConstruction control");
   
  /*RndmSeedCmd = new G4UIcmdWithAnInteger("/E16G4/gun/setRndmSeed",this);
  RndmSeedCmd->SetGuidance("Set random seed for the incident particle.");
  RndmSeedCmd->SetParameterName("RndmSeed",false);
  RndmSeedCmd->SetRange("RndmSeed>0");
  RndmSeedCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  GenIDCmd = new G4UIcmdWithAnInteger("/E16G4/gun/setGenID",this);
  GenIDCmd->SetGuidance("Set Generation ID for the incident particle.");
  GenIDCmd->SetParameterName("GenID",false);
  GenIDCmd->SetRange("GenID>=0");
  GenIDCmd->AvailableForStates(G4State_PreInit,G4State_Idle);*/
  
  MaterialFlagCmd = new G4UIcmdWithAnInteger("/E16G4/det/setMaterialFlag",this);
  MaterialFlagCmd->SetGuidance("Set MaterialFlag for the spectrometer.");
  MaterialFlagCmd->SetParameterName("MaterialFlag",false);
  MaterialFlagCmd->SetRange("MaterialFlag>=0");
  MaterialFlagCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  Tar1Cmd = new G4UIcmdWithAString("/E16G4/det/setTarget1",this);
  Tar1Cmd->SetGuidance("Set Target1 Material.");
  Tar1Cmd->SetParameterName("Tar1Mat",true);
  Tar1Cmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  Tar2Cmd = new G4UIcmdWithAString("/E16G4/det/setTarget2",this);
  Tar2Cmd->SetGuidance("Set Target2 Material.");
  Tar2Cmd->SetParameterName("Tar2Mat",true);
  Tar2Cmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  Tar3Cmd = new G4UIcmdWithAString("/E16G4/det/setTarget3",this);
  Tar3Cmd->SetGuidance("Set Target3 Material.");
  Tar3Cmd->SetParameterName("Tar3Mat",true);
  Tar3Cmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  Tar4Cmd = new G4UIcmdWithAString("/E16G4/det/setTarget4",this);
  Tar4Cmd->SetGuidance("Set Target4 Material.");
  Tar4Cmd->SetParameterName("Tar4Mat",true);
  Tar4Cmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  Tar5Cmd = new G4UIcmdWithAString("/E16G4/det/setTarget5",this);
  Tar5Cmd->SetGuidance("Set Target5 Material.");
  Tar5Cmd->SetParameterName("Tar5Mat",true);
  Tar5Cmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  Tar6Cmd = new G4UIcmdWithAString("/E16G4/det/setTarget6",this);
  Tar6Cmd->SetGuidance("Set Target6 Material(pipe end window)");
  Tar6Cmd->SetParameterName("Tar6Mat",true);
  Tar6Cmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  Tar7Cmd = new G4UIcmdWithAString("/E16G4/det/setTarget7",this);
  Tar7Cmd->SetGuidance("Set Target7 Material.(TargetChamber input window)");
  Tar7Cmd->SetParameterName("Tar7Mat",true);
  Tar7Cmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  TarThick1Cmd = new G4UIcmdWithADoubleAndUnit("/E16G4/det/setTargetThickness1",this);
  TarThick1Cmd->SetGuidance("Set Target1 Thickness.");
  TarThick1Cmd->SetParameterName("Tar1Thick",true);
  TarThick1Cmd->SetDefaultValue(0.);
  TarThick1Cmd->SetDefaultUnit("um");
  TarThick1Cmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  TarThick2Cmd = new G4UIcmdWithADoubleAndUnit("/E16G4/det/setTargetThickness2",this);
  TarThick2Cmd->SetGuidance("Set Target2 Thickness.");
  TarThick2Cmd->SetParameterName("Tar2Thick",true);
  TarThick2Cmd->SetDefaultValue(0.);
  TarThick2Cmd->SetDefaultUnit("um");
  TarThick2Cmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  TarThick3Cmd = new G4UIcmdWithADoubleAndUnit("/E16G4/det/setTargetThickness3",this);
  TarThick3Cmd->SetGuidance("Set Target3 Thickness.");
  TarThick3Cmd->SetParameterName("Tar3Thick",true);
  TarThick3Cmd->SetDefaultValue(0.);
  TarThick3Cmd->SetDefaultUnit("um");
  TarThick3Cmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  TarThick4Cmd = new G4UIcmdWithADoubleAndUnit("/E16G4/det/setTargetThickness4",this);
  TarThick4Cmd->SetGuidance("Set Target4 Thickness.");
  TarThick4Cmd->SetParameterName("Tar4Thick",true);
  TarThick4Cmd->SetDefaultValue(0.);
  TarThick4Cmd->SetDefaultUnit("um");
  TarThick4Cmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  TarThick5Cmd = new G4UIcmdWithADoubleAndUnit("/E16G4/det/setTargetThickness5",this);
  TarThick5Cmd->SetGuidance("Set Target5 Thickness.");
  TarThick5Cmd->SetParameterName("Tar5Thick",true);
  TarThick5Cmd->SetDefaultValue(0.);
  TarThick5Cmd->SetDefaultUnit("um");
  TarThick5Cmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  TarThick6Cmd = new G4UIcmdWithADoubleAndUnit("/E16G4/det/setTargetThickness6",this);
  TarThick6Cmd->SetGuidance("Set Target6 Thickness.(pipe end window)");
  TarThick6Cmd->SetParameterName("Tar6Thick",true);
  TarThick6Cmd->SetDefaultValue(0.);
  TarThick6Cmd->SetDefaultUnit("um");
  TarThick6Cmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  TarThick7Cmd = new G4UIcmdWithADoubleAndUnit("/E16G4/det/setTargetThickness7",this);
  TarThick7Cmd->SetGuidance("Set Target7 Thickness.(TargetChamber input window)");
  TarThick7Cmd->SetParameterName("Tar7Thick",true);
  TarThick7Cmd->SetDefaultValue(0.);
  TarThick7Cmd->SetDefaultUnit("um");
  TarThick7Cmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  /*UpdateCmd = new G4UIcmdWithoutParameter("/E16G4/det/update",this);
  UpdateCmd->SetGuidance("Update calorimeter geometry.");
  UpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
  UpdateCmd->SetGuidance("if you changed geometrical value(s).");
  UpdateCmd->AvailableForStates(G4State_Idle);
  
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
  SigmaYCmd->AvailableForStates(G4State_PreInit,G4State_Idle);*/
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

 E16G4_DetectorConstructionMessenger::~E16G4_DetectorConstructionMessenger()
{
  delete MaterialFlagCmd;
  delete Tar1Cmd;
  delete Tar2Cmd;
  delete Tar3Cmd;
  delete Tar4Cmd;
  delete Tar5Cmd;
  delete Tar6Cmd;
  delete Tar7Cmd;
  delete TarThick1Cmd;
  delete TarThick2Cmd;
  delete TarThick3Cmd;
  delete TarThick4Cmd;
  delete TarThick5Cmd;
  delete TarThick6Cmd;
  delete TarThick7Cmd;
  //delete UpdateCmd;
  delete detDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void E16G4_DetectorConstructionMessenger::SetNewValue(
                                        G4UIcommand* command, G4String newValue)
{ 
  if( command == MaterialFlagCmd )
   { Const->SetMaterialFlag(MaterialFlagCmd->GetNewIntValue(newValue));}
  if( command == Tar1Cmd )
   { Const->SetTarget1(newValue);}
  if( command == Tar2Cmd )
   { Const->SetTarget2(newValue);}
  if( command == Tar3Cmd )
   { Const->SetTarget3(newValue);}
  if( command == Tar4Cmd )
   { Const->SetTarget4(newValue);}
  if( command == Tar5Cmd )
   { Const->SetTarget5(newValue);}
  if( command == Tar6Cmd )
   { Const->SetTarget6(newValue);}
  if( command == Tar7Cmd )
   { Const->SetTarget7(newValue);}

  if( command == TarThick1Cmd )
   { Const->SetTargetThickness1(TarThick1Cmd->GetNewDoubleValue(newValue));}
  if( command == TarThick2Cmd )
   { Const->SetTargetThickness2(TarThick2Cmd->GetNewDoubleValue(newValue));}
  if( command == TarThick3Cmd )
   { Const->SetTargetThickness3(TarThick3Cmd->GetNewDoubleValue(newValue));}
  if( command == TarThick4Cmd )
   { Const->SetTargetThickness4(TarThick4Cmd->GetNewDoubleValue(newValue));}
  if( command == TarThick5Cmd )
   { Const->SetTargetThickness5(TarThick5Cmd->GetNewDoubleValue(newValue));}
  if( command == TarThick6Cmd )
   { Const->SetTargetThickness6(TarThick6Cmd->GetNewDoubleValue(newValue));}
  if( command == TarThick7Cmd )
   { Const->SetTargetThickness7(TarThick7Cmd->GetNewDoubleValue(newValue));}
  //if( command == UpdateCmd )
  // { Const->UpdateGeometry(); }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

