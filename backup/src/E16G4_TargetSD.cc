//2016-08-12, uploaded by nakai
//2015-03-01, uploaded by yokkaich
//2015-01-20, uploaded by yokkaich
//2015-01-05, modified by yokkaich
//2015-01-05, modified by yokkaich
//2014-08-27, modified by kawama
//2014-08-27, modified by kawama
//2014-04-30, modified by kawama
//2013-11-14, modified by kawama
//2013-05-13, modified by kawama

//E16G4_TargetSD.cc 150118 by S. Yokkaichi
//    Last modified at <>
//

#include <G4VPhysicalVolume.hh>
#include <G4Step.hh>
#include <G4Track.hh>
#include <G4VTouchable.hh>
#include <G4TouchableHistory.hh>
#include <G4VProcess.hh>

#include "E16G4_TargetSD.hh"
#include "E16G4_BeamLineElement.hh"



//////////////////////////////////////////
   E16G4_TargetSD::E16G4_TargetSD(const G4String& name)
: G4VSensitiveDetector(name)
//////////////////////////////////////////
{
   for (int i=0;i<nID;i++){
      std::ostringstream id;
      id << i;
      G4String Name = G4String("Target")+id.str().c_str()+"-Collection";
      collectionName.insert(Name);
      elem_[i]=0;
   }
}

/////////////////////////
 E16G4_TargetSD::~E16G4_TargetSD(){
/////////////////////////
}

/////////////////////////////////////////////////////////
void E16G4_TargetSD::SetElement(int id, E16G4_BeamLineElement *elem)
/////////////////////////////////////////////////////////
{
   elem_[id]=elem;
}

//////////////////////////////////////////////////
void E16G4_TargetSD::Initialize(G4HCofThisEvent* HCTE){
//////////////////////////////////////////////////
   for (int i=0;i<nID;i++){
      hitsCollection[i]= 
         new TargetHitsCollection(SensitiveDetectorName, 
               collectionName[i]);
      G4int hcid= GetCollectionID(i);
      HCTE-> AddHitsCollection(hcid, hitsCollection[i]);
   }
}

/////////////////////////////////////////////
G4bool E16G4_TargetSD::ProcessHits(G4Step* aStep, 
      G4TouchableHistory* ROhist){
/////////////////////////////////////////////
   const G4StepPoint* aStepPoint= aStep-> GetPreStepPoint();
   //const G4StepPoint* aStepPoint= aStep-> GetPostStepPoint();
   const G4Track* aTrack= aStep-> GetTrack();

   G4double charge= aStep-> GetTrack()-> GetDefinition()-> GetPDGCharge();
   /*if(charge == 0.){
      return false;
   }*/
   /*if(aStepPoint-> GetStepStatus() != fGeomBoundary){
      G4cout <<"ProcessHits:GeoBound "<<G4endl;
      return false;
   }*/

   //Hit Position
   G4ThreeVector pos= aStepPoint-> GetPosition();
   //Hit Time
   G4double time= aStepPoint-> GetGlobalTime();
   //   G4cerr <<"target "<< time <<G4endl;

   //Track Momentum
   G4ThreeVector mom= aTrack-> GetMomentum();
   //Track Momentum
   G4double ekin= aTrack-> GetKineticEnergy();
   //Particle information
   G4ParticleDefinition *particle= 
      aTrack-> GetDynamicParticle()->GetDefinition();
   G4String pName=particle->GetParticleName();
   G4int PDGCode=particle->GetPDGEncoding();
   G4double steplength=aStep->GetStepLength();
   G4double edep=aStep->GetTotalEnergyDeposit();
   G4int trkid = aTrack->GetTrackID();
   G4int ptrkid = aTrack->GetParentID();

   const G4VProcess *proc=aTrack-> GetCreatorProcess();
   G4String procName;
   if(proc!=0){
      procName=proc->GetProcessName();
   }
   else{
      procName="Primary";
   }



   ROhist=(G4TouchableHistory*)aStepPoint->GetTouchable();
   G4VPhysicalVolume *PV = ROhist->GetVolume();
   G4String Name = PV->GetName();
   /*G4cout << " "<<G4endl;
   G4cout << "trkid:"<<trkid<<G4endl;
   G4cout << "ProcessHits:Names="<<Name<<G4endl;
   G4cout<< "steplength:"<<steplength/cm<< G4endl;*/
   std::stringstream sid(Name.substr(6));
   int id=-1;sid>>id;
   //G4cout << "ProcessHits:ID="<<id<<G4endl;
   //transTarget *trTr = new transTarget();
   if (id>=0){
      //G4ThreeVector cent=elem_[id]->GetCent();
      //G4ThreeVector orig=elem_[id]->GetLocalPosition(gPos);
      //G4cout << "ProcessHits:cent="<<cent<<G4endl;
      //pos+=cent;
      /*G4cout << "ProcessHits:pid="<<PDGCode<<G4endl;
      G4cout << "ProcessHits:ekin="<<ekin/GeV<<G4endl;
      G4cout << "ProcessHits:pos:"<<pos*(1./cm)<< G4endl;*/
      //if(PDGCode==22)
      //G4cout << "ProcessHits:x,y,z="<<pos.x()<<" "<<pos.y()<<" "<<pos.z()<<G4endl;
      //G4ThreeVector lPos(lPos_t3.X(),lPos_t3.Y(),lPos_t3.Z());
      G4ThreeVector lPos=elem_[id]->GetLocalPosition(pos);
      G4ThreeVector lMom=elem_[id]->GetLocalDirection(mom);
      /*if (id==36){
        G4cout << "ProcessHits:pos="<<pos<<G4endl;
        G4cout << "ProcessHits:lPos="<<lPos<<G4endl;
        }*/

      //const G4int nHits=hitsCollection[id]->entries();
      //G4cout << "ProcessHits:nHits="<<nHits<<G4endl;
      E16G4_TargetHit* aHit;
      /*for (int i=0;i<nHits;i++){
         aHit =(*hitsCollection[id])[i];
         if(id==aHit->GetID()){
            aHit->AddEdep(edep);
            aHit->AddLength(steplength);
            //return true;
         }
      }*/
      aHit = new E16G4_TargetHit(pos, time, charge);
      aHit->SetGPos(pos);
      aHit->SetLPos(lPos);
      aHit->SetLMom(lMom);
      aHit->SetEdep(edep);
      aHit->SetLength(steplength);
      aHit->SetPName(pName);
      aHit->SetPCode(PDGCode);
      aHit->SetcrProcName(procName);
      aHit->SetTrackID(trkid);
      aHit->SetParentID(ptrkid);

      hitsCollection[id]-> insert(aHit);

      //return true;
   }
   return true;
}

///////////////////////////////////////////////////
void E16G4_TargetSD::EndOfEvent(G4HCofThisEvent* ){
   ///////////////////////////////////////////////////
}

//////////////////////////
void E16G4_TargetSD::DrawAll(){
   //////////////////////////
}

///////////////////////////
void E16G4_TargetSD::PrintAll(){
   ///////////////////////////
   for (int i=0;i<nID;i++){
      hitsCollection[i]-> PrintAllHits();
   }
}

