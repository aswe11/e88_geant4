//2016-08-12, uploaded by nakai
//2016-05-05, uploaded by yokkaich
//2015-08-30, uploaded by yokkaich
//2015-03-01, uploaded by yokkaich
//2015-01-20, uploaded by yokkaich
//2015-01-05, modified by yokkaich
//2015-01-05, modified by yokkaich
//2014-08-27, modified by kawama
//2014-08-27, modified by kawama
//2014-04-30, modified by kawama
//2013-11-14, modified by kawama
//2013-05-13, modified by kawama
/*
   E16G4_TrackingAction.cc

   D.Kawama
   */

#include <G4Track.hh>
#include <G4TrackVector.hh>
#include <G4TrackingManager.hh>
#include <G4SteppingManager.hh>
#include <G4Step.hh>
#include <G4StepPoint.hh>
#include <G4VProcess.hh>
#include <G4VParticleChange.hh>

#include "E16G4_TrackingAction.hh"
#include "E16G4_EventAction.hh"
#include "E16G4_Trajectory.hh"
#include "E16G4_Field.hh"
#include "E16G4_Analysis.hh"
#include "E16G4_SteppingAction.hh"

#include "E16ANA_ErrorMessage.hh"

///////////////////////////////////////
   E16G4_TrackingAction::E16G4_TrackingAction(
         E16G4_EventAction* EA, E16G4_Analysis* ANA)
: G4UserTrackingAction(),iSec(0),event(EA), anaMan(ANA)
   ///////////////////////////////////////  
{
}

////////////////////////////////////////
E16G4_TrackingAction::~E16G4_TrackingAction()
   ////////////////////////////////////////
{
}

///////////////////////////////////////////////////////////////////////
void E16G4_TrackingAction::PreUserTrackingAction( const G4Track *aTrack )
   ///////////////////////////////////////////////////////////////////////
{
   clock_t begin, end;
   begin=clock();
   G4ParticleDefinition *particle=aTrack->GetDefinition();
   G4String name=particle->GetParticleName();
   G4int PDGCode=particle->GetPDGEncoding();
   G4double kin=aTrack->GetKineticEnergy();
   G4ThreeVector mom=aTrack->GetMomentum();
   G4int ID = aTrack->GetTrackID();
   //G4double sl = aTrack->GetStep()->GetStepLength();
   //G4cout << PDGCode<<" "<<sl/um<<G4endl;
   /*G4Track *theTrack= (G4Track*)aTrack;
   if ( 
      kin<1*MeV
      )
		{
      theTrack->SetTrackStatus(fStopAndKill);
    }*/
   /*G4Step *aStep = (G4Step*)aTrack->GetStep();
   G4StepPoint *presp = aStep->GetPreStepPoint();*/
   //G4cout <<"PreTrackingAction:SteppingNumber:"<<aTrack->GetCurrentStepNumber()<<G4endl;
   //def   if(ID<10000){
   if(ID<100000){//sako
      G4int parID = aTrack->GetParentID();
      G4ThreeVector pos_vtx = aTrack->GetVertexPosition();
      G4String volName = aTrack->GetVolume()->GetName();
      G4String volName_vtx = aTrack->GetLogicalVolumeAtVertex()->GetName();
      const G4VProcess *proc = aTrack->GetCreatorProcess();
      anaMan->datDC[ID].partID=PDGCode;
      
      if(proc!=0){
         //G4cout<<pos_vtx<<G4endl;
         anaMan->datDC[ID].posx_vtx=pos_vtx.x()/cm;
         anaMan->datDC[ID].posy_vtx=pos_vtx.y()/cm;
         anaMan->datDC[ID].posz_vtx=pos_vtx.z()/cm;
         strcpy(anaMan->datDC[ID].procName,proc->GetProcessName().c_str());
         if(parID>=0){
            anaMan->datDC[ID].GEN=anaMan->datDC[parID].GEN+1;
            anaMan->datDC[ID].parentID=parID;
            strcpy(anaMan->datDC[ID].volName,volName.c_str());
            strcpy(anaMan->datDC[ID].volName_vtx,volName_vtx.c_str());
         }
         else{
            G4cout <<"***Error*** TrackingAction::parent ID <0"<<G4endl;
         }
      }
      else{
         strcpy(anaMan->datDC[ID].procName,"Primary");
         anaMan->datDC[ID].GEN=0;
         anaMan->datDC[ID].parentID=0;
      }
      //G4cout <<"TrackingAction:"<<anaMan->datDC[ID].procName<<G4endl;
      //G4cout <<"TrackingAction:"<< volName_vtx<<", "<<anaMan->datDC[ID].volName_vtx<<G4endl;
      //if (PDGCode==111){
      //if (PDGCode<10000 && kin/MeV>1){
      //G4cout <<"Particle:"<< name <<", mom="<<mom/GeV<< G4endl;
      std::ostringstream str_kin;
      str_kin << kin;
      std::ostringstream id;
      id << ID;
      std::ostringstream pid;
      pid << parID;
      std::ostringstream gen;
      gen << anaMan->datDC[ID].GEN;
      if (parID==0){  
         event->AddDecayChain(
               name+", ID="+id.str().c_str()+
               ", pID="+pid.str().c_str()+
               ", GEN="+gen.str().c_str()+
               ", "+anaMan->datDC[ID].procName+
               ", "+volName+"\n");
         anaMan->outputDC(ID,0);
         anaMan->outputDC(ID,1);
      }
      else if(
            //anaMan->datDC[ID].procName!="hIoni" && 
            //anaMan->datDC[ID].procName!="phot" && 
            //anaMan->datDC[ID].procName!="compt" &&
            //anaMan->datDC[ID].procName!="eBrem" && 
            //anaMan->datDC[ID].procName!="conv" && 
            //anaMan->datDC[ID].procName!="eIoni" &&
            //anaMan->datDC[ID].procName!="muIoni" && 
            //anaMan->datDC[ID].procName!="annihil" && 
            //anaMan->datDC[ID].procName!="hadElastic"&& 
            //anaMan->datDC[ID].procName!="NeutronInelastic"&& 
            //anaMan->datDC[ID].procName=="Decay"&&
            volName.substr(0,2)!="LG" &&
            volName!="FMM Yoke" && volName!="FMM Coil" && 
            volName!="FMM Pole" && anaMan->datDC[ID].GEN<10
            )
      { event->AddDecayChain("--->"+
            name+
            ", ID="+id.str().c_str()+
            ", pID="+pid.str().c_str()+
            ", GEN="+gen.str().c_str()+
            ", "+anaMan->datDC[ID].procName+
            ", "+volName+"\n");
      anaMan->outputDC(ID,1);
   //G4cout << ID<<" "<<parID<<" "<<name << " "<<kin/MeV<<G4endl;
      }//elseif
   }
   else{
     E16WARNING("Tracking ID exceed 10000. Decay chain will not be logged.");
   }
   
   G4bool stored = true;
   E16G4_Trajectory *trj = new E16G4_Trajectory( aTrack );
   fpTrackingManager-> SetTrajectory(trj );
   fpTrackingManager-> SetStoreTrajectory( stored );

   /*G4ParticleDefinition *particle=aTrack->GetDefinition();
     G4String name=particle->GetParticleName();
     G4int PDGCode=particle->GetPDGEncoding();
     G4double kin=aTrack->GetKineticEnergy();
     G4ThreeVector mom=aTrack->GetMomentum();
     G4int ID = aTrack->GetTrackID();
     G4int parID = aTrack->GetParentID();
     G4String volName = aTrack->GetVolume()->GetName();
     const G4VProcess *proc = aTrack->GetCreatorProcess();
     G4String procName;
     if(proc!=0){
     procName=proc->GetProcessName();
     }
     else{
     procName="Primary";
     }
   //if (PDGCode==111){
   //if (PDGCode<10000 && kin/MeV>1){
   //G4cout <<"Particle:"<< name <<", mom="<<mom/GeV<< G4endl;
   std::ostringstream str_kin;
   str_kin << kin;
   std::ostringstream id;
   id << ID;
   std::ostringstream pid;
   pid << parID;
   if (parID==0){  
   event->AddDecayChain(
   name+", ID="+id.str().c_str()+
   ", pID="+pid.str().c_str()+
   ", "+procName+
   ", "+volName+"\n");
   }
   else if(
   procName!="hIoni" && procName!="phot" && procName!="compt" &&
   procName!="eBrem" && procName!="conv" && procName!="eIoni" &&
   procName!="muIoni" && procName!="annihil" 
   && procName!="hadElastic"&& 
   //procName=="Decay"&&
   volName!="FMM Yoke" && volName!="FMM Coil" && volName!="FMM Pole")
   { event->AddDecayChain("--->"+
   name+
   ", ID="+id.str().c_str()+
   ", pID="+pid.str().c_str()+
   ", "+procName+
   ", "+volName+"\n");
   }*/
   end=clock();
   //G4cout <<"PreUserTracking Action::"<<(double)(end-begin)/CLOCKS_PER_SEC<<G4endl;
}

////////////////////////////////////////////////////////////////////////
void E16G4_TrackingAction::PostUserTrackingAction( const G4Track *aTrack )
   ////////////////////////////////////////////////////////////////////////
{
   G4ParticleDefinition *particle=aTrack->GetDefinition();
   G4String name=particle->GetParticleName();
   G4int PDGCode=particle->GetPDGEncoding();
   G4double kin=aTrack->GetKineticEnergy();
   G4ThreeVector mom=aTrack->GetMomentum();
   G4int ID = aTrack->GetTrackID();
   G4int nStep=aTrack->GetCurrentStepNumber();
   //G4cout <<"PostTrackingAction:SteppingNumber:"<<aTrack->GetCurrentStepNumber()<<G4endl;
      G4String volName = aTrack->GetVolume()->GetName();
   G4Step *aStep = (G4Step*)aTrack->GetStep();
   //E16G4_SteppingAction *pSteppingAction = new E16G4_SteppingAction();
      /*if(strstr(volName,"LG")){
   G4double deltaE = aStep->GetTotalEnergyDeposit();
         G4cout <<volName<<" "<< ID<<" "<<PDGCode<<" "<<kin<<" "<<deltaE/MeV<<G4endl;
      }*/
   /*for (int i=0;i<nStep;i++){
      G4cout << "iStep="<<i<<" Volume:"<<pSteppingAction->VolumeList[i]<<G4endl;
   }*/
   /*if(ID<10000){
      G4int parID = aTrack->GetParentID();
      G4String volName = aTrack->GetVolume()->GetName();
      G4String volName_vtx = aTrack->GetLogicalVolumeAtVertex()->GetName();
      const G4VProcess *proc = aTrack->GetCreatorProcess();
      anaMan->datDC[ID].partID=PDGCode;
      if(proc!=0){
         anaMan->datDC[ID].procName=proc->GetProcessName();
         if(parID>=0){
            anaMan->datDC[ID].GEN=anaMan->datDC[parID].GEN+1;
            anaMan->datDC[ID].parentID=parID;
            anaMan->datDC[ID].volName=volName;
            anaMan->datDC[ID].volName_vtx=volName_vtx;
         }
         else{
            G4cout <<"***Error*** TrackingAction::parent ID <0"<<G4endl;
         }
      }
      else{
         anaMan->datDC[ID].procName="Primary";
         anaMan->datDC[ID].GEN=0;
         anaMan->datDC[ID].parentID=0;
      }
      //if (PDGCode==111){
      //if (PDGCode<10000 && kin/MeV>1){
      //G4cout <<"Particle:"<< name <<", mom="<<mom/GeV<< G4endl;
      std::ostringstream str_kin;
      str_kin << kin;
      std::ostringstream id;
      id << ID;
      std::ostringstream pid;
      pid << parID;
      std::ostringstream gen;
      gen << anaMan->datDC[ID].GEN;
   G4cout << ID<<" "<<parID<<" "<<name << " "<<kin<<G4endl;
      if (parID==0){  
         event->AddDecayChain(
               name+", ID="+id.str().c_str()+
               ", pID="+pid.str().c_str()+
               ", GEN="+gen.str().c_str()+
               ", "+anaMan->datDC[ID].procName+
               ", "+volName+"\n");
         anaMan->outputDC(ID,0);
         anaMan->outputDC(ID,1);
      }
      else if(
            anaMan->datDC[ID].procName!="hIoni" && 
            //anaMan->datDC[ID].procName!="phot" && 
            //anaMan->datDC[ID].procName!="compt" &&
            //anaMan->datDC[ID].procName!="eBrem" && 
            //anaMan->datDC[ID].procName!="conv" && 
            anaMan->datDC[ID].procName!="eIoni" &&
            anaMan->datDC[ID].procName!="muIoni" && 
            anaMan->datDC[ID].procName!="annihil" && 
            anaMan->datDC[ID].procName!="hadElastic"&& 
            anaMan->datDC[ID].procName!="NeutronInelastic"&& 
            //anaMan->datDC[ID].procName=="Decay"&&
            volName.substr(0,2)!="LG" &&
            volName!="FMM Yoke" && volName!="FMM Coil" && 
            volName!="FMM Pole" && anaMan->datDC[ID].GEN<5
            )
      { event->AddDecayChain("--->"+
            name+
            ", ID="+id.str().c_str()+
            ", pID="+pid.str().c_str()+
            ", GEN="+gen.str().c_str()+
            ", "+anaMan->datDC[ID].procName+
            ", "+volName+"\n");
      anaMan->outputDC(ID,1);
   G4cout << ID<<" "<<parID<<" "<<name << " "<<kin<<G4endl;
      }
   }
      else{
         G4cerr<<"Tracking ID exceed 10000. Decay chain will not be logged."<<G4endl;
      }*/
      //Trajectory *trj = dynamic_cast<Trajectory*>
      //  ( fpTrackingManager-> GimmeTrajectory() );
      /*E16G4_Trajectory *trj = (E16G4_Trajectory*)fpTrackingManager-> GimmeTrajectory();
        if( trj ){
        trj-> SetPositionAtLast( aTrack-> GetPosition() );
        trj-> SetMomentumAtLast( aTrack-> GetMomentum() );
        }*/
      /*G4SteppingManager *stMan = fpTrackingManager-> GetSteppingManager();
        G4VProcess *curProc= stMan->GetfCurrentProcess();
        G4String procName=curProc->GetProcessName();

      //G4cout <<"Priomary:"<<aTrack->GetDefinition()->GetParticleName()<<G4endl;
      G4TrackVector *secondaries = fpTrackingManager->GimmeSecondaries();
      size_t nbtrk = (*secondaries).size();
      if(nbtrk && procName!="hIoni" && procName!="Transportation"){
      //there are secondaries --> it is a decay
      //
      //force 'single' decay
      //if ((!fullChain)&&(ID > 1)) G4RunManager::GetRunManager()->AbortEvent();
      //
      //balance    
      //if (procName=="Decay"){
      //G4cout << "ID:"<<aTrack->GetTrackID()<<G4endl;
      //G4cout << "pName:"<<aTrack->GetDefinition()->GetParticleName()<<G4endl;
      //}

      G4cout <<"Parent ID:"<<aTrack->GetParentID()<<", "
      << "ID:"<<aTrack->GetTrackID()<<G4endl;
      G4cout << "pName:"<<aTrack->GetDefinition()->GetParticleName()<<G4endl;
      G4double EkinTot = 0.;
      G4ThreeVector Pbalance = - aTrack->GetMomentum();
      for (size_t itr=0; itr<nbtrk; itr++) {
      G4Track* trk = (*secondaries)[itr];
      EkinTot += trk->GetKineticEnergy();
      const G4VProcess* s_proc=trk->GetCreatorProcess();
      G4String s_procName=s_proc->GetProcessName();
      //exclude gamma desexcitation from momentum balance
      if(s_procName!="hIoni"){
      G4String secName=trk->GetDefinition()->GetParticleName();
      G4cout <<"Secondaries Name:"<<secName<<G4endl;
      G4cout <<"Secondaries Creator Process Name:"<<s_procName<<G4endl;
      sprintf(secList[iSec],secName.c_str());
      //if (trk->GetDefinition() != G4Gamma::Gamma())	 
      //Pbalance += trk->GetMomentum();
      iSec++;
      }  
      }
      G4cout <<"iSec="<< iSec<<G4endl;
      G4double Pbal = Pbalance.mag();  
      //run->Balance(EkinTot,Pbal);  
      //histoManager->FillHisto(6,EkinTot);
      //histoManager->FillHisto(7,Pbal);
      }*/
      //delete trj;
   }
