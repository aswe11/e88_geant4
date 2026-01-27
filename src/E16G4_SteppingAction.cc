//2016-08-12, uploaded by nakai
//2015-01-20, uploaded by yokkaich
//2015-01-05, modified by yokkaich
//2015-01-05, modified by yokkaich
//2014-08-27, modified by kawama
//2014-08-27, modified by kawama
//2014-04-30, modified by kawama
//2013-11-14, modified by kawama
//2013-05-13, modified by kawama
/*
  E16G4_SteppingAction.cc

  D.Kawama
*/

#include <ostream>

#include <G4Step.hh>
#include <G4FieldManager.hh>
#include <G4TransportationManager.hh>
#include <G4VTouchable.hh>
#include <G4VPhysicalVolume.hh>
#include <G4VProcess.hh>

#include "E16G4_PrimaryGeneratorAction.hh"
#include "E16G4_SteppingAction.hh"
#include "E16G4_GlobalVar.hh"


////////////////////////////////////////
E16G4_SteppingAction::E16G4_SteppingAction()
  : G4UserSteppingAction()
///////////////////////////////////////
{
   //fmf.Initialize();
  //  std::cerr<<"yok-steppingaction-init"<<std::endl;
}

///////////////////////////////////////
E16G4_SteppingAction::~E16G4_SteppingAction()
///////////////////////////////////////
{
}

//////////////////////////////////////////////////////////////////
void E16G4_SteppingAction::UserSteppingAction( const G4Step *aStep )
/////////////////////////////////////////////////////////////////
{
  G4ThreeVector pos = aStep->GetPostStepPoint()->GetPosition();
  G4ThreeVector dpos = aStep->GetDeltaPosition(); 
  const G4VTouchable *theTouchable =
    aStep->GetPreStepPoint()->GetTouchable();
  G4VPhysicalVolume *Vol = theTouchable->GetVolume(); 
  G4String volName = Vol->GetName(); 
  G4String matName = Vol->GetLogicalVolume()->GetMaterial()->GetName(); 
  G4Track *track = aStep->GetTrack();
  G4String pname= track->GetDefinition()->GetParticleName();
  //G4double kin=track->GetKineticEnergy();
  G4TrackStatus status = track->GetTrackStatus();
  G4int iStep=track-> GetCurrentStepNumber();
  
  G4double kin=aStep->GetPostStepPoint()->GetKineticEnergy();
  G4double dE=aStep->GetTotalEnergyDeposit();
 
 /***Debug */
  G4int id=track-> GetTrackID();
  const G4VProcess *proc=track-> GetCreatorProcess();
  G4String procName;
  if(proc!=0){
        procName=proc->GetProcessName();
   }
  
  double X[3];
  double Bfield[3];
  X[0] = pos.x()/cm;
  X[1] = pos.y()/cm;
  X[2] = pos.z()/cm;
  //fmf.GetFieldValue(X,Bfield,0); 

  //material check
   /*G4cout << "SteppingAction: "<<" step id="<<iStep
   <<", material="<<matName<<" "<<procName
   <<", Delta Z="<<dpos.z()/cm
   <<", pos Z="<<pos.z()/cm
   <<", dE="<<dE/MeV<< 
   G4endl;*/
  //int iVolumeList=0;
   /*sprintf(VolumeList[iStep],volName);
  if(aStep->IsLastStepInVolume()){ 
  G4cout << "SteppingAction: "<<id<<" "<<iStep<<" "<<pname <<" " << 
   volName<<" "<<aStep->IsLastStepInVolume()<<G4endl;
   //iVolumeList++;
  }*/
   //G4cout << "SteppingAction: "<<id<<" "<<iStep<<" "<<pname <<" " << kin/MeV <<" "<< 
   //pos <<" "<<matName<<" "<<status<<" "<<procName<<" "<<volName<<G4endl;
  
  /*G4cout<<matName<<" "<<
   dpos.x()/cm<<" "<< dpos.x()/cm<<" "<<dpos.x()/cm<<" "<<
   Bfield[0]<<" "<<Bfield[1]<<" "<<Bfield[2]<<" "<< G4endl;*/
  /***/

 E16G4_PrimaryGeneratorAction *pPGA = E16G4_PrimaryGeneratorAction::GetpPGA();
 int genID = pPGA->GetGenID();
 G4double energyco;
  if (genID==2) energyco=1.*keV;
  else energyco=1*MeV;
  if (fabs(pos.x()) > WorldSizeX 
      || fabs(pos.y()) > WorldSizeY 
      || fabs(pos.z()) > WorldSizeZ 
      ||kin<energyco
      ||iStep>1e5
      ||volName=="FMMagnet_Yoke"
      ||volName=="FMMagnet_Pole"
      )
		{
      //track->SetTrackStatus(fKillTrackAndSecondaries);
      track->SetTrackStatus(fStopAndKill);
    }
 
  /*if ( kin==0*MeV)
    {
      //track->SetTrackStatus(fStopButAlive);
      track->SetTrackStatus(fStopAndKill);
      //track->SetTrackStatus(fKillTrackAndSecondaries);
    }*/
  

//#ifdef DEBUG
//   if( !field_ ){
//     G4FieldManager *fieldMan =
//       G4TransportationManager::GetTransportationManager()->GetFieldManager();
//     field_ = dynamic_cast<const E16G4_Field *>(fieldMan->GetDetectorField());
//   }
  //if( field_ ) field_->PushInQueue( 1, pos );
//#endif
}
