//2016-08-12, uploaded by nakai
//2016-03-18, uploaded by nakai
//2015-09-05, uploaded by nakai
//2015-03-01, uploaded by yokkaich
//2015-01-05, modified by yokkaich
//2015-01-05, modified by yokkaich
//2014-08-27, modified by kawama
//2014-08-27, modified by kawama
//2014-04-30, modified by kawama
//2013-11-14, modified by kawama
//2013-05-13, modified by kawama

#include <G4VPhysicalVolume.hh>
#include <G4Step.hh>
#include <G4Track.hh>
#include <G4VTouchable.hh>
#include <G4TouchableHistory.hh>

#include <G4ParticleChange.hh>
#include <G4VProcess.hh>
#include <G4ProcessManager.hh>

#include <TVector3.h>

#include "E16G4_GTRSD.hh"
#include "E16G4_BeamLineElement.hh"
#include "E16ANA_Transform.hh"
#include "E16ANA_GeometryV2.hh"


//////////////////////////////////////////
   E16G4_GTRSD::E16G4_GTRSD(const G4String& name,const E16ANA_GeometryV2* geom)
: G4VSensitiveDetector(name), geom_(geom)
//////////////////////////////////////////
{
   for (int i=0;i<nID;i++){
      std::ostringstream id;
      id << i;
      G4String Name = G4String("GTR")+id.str().c_str()+"-Collection";
      collectionName.insert(Name);
      elem_[i]=0;
   }
   //ptr_ = new E16ANA_Transform();
}

/////////////////////////
 E16G4_GTRSD::~E16G4_GTRSD(){
/////////////////////////
}

/////////////////////////////////////////////////////////
void E16G4_GTRSD::SetElement(int id, E16G4_BeamLineElement *elem)
/////////////////////////////////////////////////////////
{
   elem_[id]=elem;
}

//////////////////////////////////////////////////
void E16G4_GTRSD::Initialize(G4HCofThisEvent* HCTE){
//////////////////////////////////////////////////
   for (int i=0;i<nID;i++){
      hitsCollection[i]= 
         new GTRHitsCollection(SensitiveDetectorName, 
               collectionName[i]);
      G4int hcid= GetCollectionID(i);
      HCTE-> AddHitsCollection(hcid, hitsCollection[i]);
   }
}

/////////////////////////////////////////////
G4bool E16G4_GTRSD::ProcessHits(G4Step* aStep, 
      G4TouchableHistory* ROhist){
/////////////////////////////////////////////
   const G4StepPoint* aPreStepPoint= aStep-> GetPreStepPoint();
   const G4StepPoint* aPostStepPoint= aStep-> GetPostStepPoint();
   const G4Track* aTrack= aStep-> GetTrack();

   G4double charge= aTrack-> GetDefinition()-> GetPDGCharge();
   /*if(charge == 0.){
      return false;
   }*/
   /*if(aPreStepPoint-> GetStepStatus() != fGeomBoundary){
      G4cout <<"ProcessHits:GeoBound "<<G4endl;
      return false;
   }*/

   //Hit Position
   G4ThreeVector prepos= aPreStepPoint-> GetPosition();
   G4ThreeVector postpos= aPostStepPoint-> GetPosition();
   //Track Momentum
   G4ThreeVector mom= aTrack-> GetMomentum();


   ROhist=(G4TouchableHistory*)aPreStepPoint->GetTouchable();
   G4VPhysicalVolume *PV = ROhist->GetVolume();

   G4String Name = PV->GetName();

   std::stringstream sid(Name.substr(6,2));

   int id=-1;sid>>id;

   //E16ANA_Transform *ptr = new E16ANA_Transform();
   if (id>=0){
      int idTr = id/3;
      int idGEM =id%3;
      /*if(idTr==3 || idTr==4 || idTr==5 || idTr==22 || idTr==23 || idTr==24){
        G4cout << "ProcessHits:idTr, idGEM="<<idTr <<" "<<idGEM<<G4endl;
        G4cout << "ProcessHits:gpos="<<pos/cm<<G4endl;
        }*/
      //double phi=30*(idTr%3-1);
      //double theta=27.315*(4-int(idTr/3));
      //double theta=26.06*(4-int(idTr/3));
      //double theta=geom_->GTRtheta[idTr];

      //TVector3 prelPos_t3=ptr->GetLPos(TVector3(prepos.x()/cm, prepos.y()/cm, prepos.z()/cm), theta,geom_->GTRy[idGEM][idTr],geom_->GTRz[idGEM][idTr],
		//		       0.,0.,0.,0.,0.,0.);
      //TVector3 prelPos_t3=E16ANA_Transform::GetLPos(
      //      TVector3(prepos.x()/cm, prepos.y()/cm, prepos.z()/cm),
      //      geom_,idGEM,idTr);

      //G4ThreeVector prelPos(prelPos_t3.X()*cm,prelPos_t3.Y()*cm,prelPos_t3.Z()*cm);
      G4ThreeVector prelPos = geom_->GTR(idTr,idGEM)->GetLPos(prepos);

      //TVector3 postlPos_t3=ptr->GetLPos(TVector3(postpos.x()/cm,postpos.y()/cm,postpos.z()/cm),
      //      theta,geom_->GTRy[idGEM][idTr],geom_->GTRz[idGEM][idTr],0.,0.,0,0,0,0);
      //TVector3 postlPos_t3=E16ANA_Transform::GetLPos(
      //      TVector3(postpos.x()/cm,postpos.y()/cm,postpos.z()/cm),
      //      geom_,idGEM,idTr);
      //G4ThreeVector postlPos(postlPos_t3.X()*cm,postlPos_t3.Y()*cm,postlPos_t3.Z()*cm);
      G4ThreeVector postlPos = geom_->GTR(idTr,idGEM)->GetLPos(postpos);

      //TVector3 lMom_t3=ptr->GetLMom(TVector3(mom.x()/GeV,mom.y()/GeV,mom.z()/GeV),
      //TVector3 lMom_t3=E16ANA_Transform::GetLMom(TVector3(mom.x()/GeV,mom.y()/GeV,mom.z()/GeV),
      //      theta,0,0,0,0);
      //G4ThreeVector lMom(lMom_t3.X()*GeV,lMom_t3.Y()*GeV,lMom_t3.Z()*GeV);
      G4ThreeVector lMom = geom_->GTR(idTr,idGEM)->GetLMom(mom);

      E16G4_GTRHit* aHit;
      /*if(int(idTr/3)==5 && (prepos.z()/cm>60 || postpos.z()/cm>60)){
         G4cout << "ProcessHits:Name="<<Name<<G4endl;
         G4cout << "ProcessHits:Name_post="<<Name_post<<G4endl;
         G4cout << "ProcessHits:prelPos="<<prelPos/cm<<G4endl;
         G4cout << "ProcessHits:postlPos="<<postlPos/cm<<G4endl;
      }*/

      if(prelPos.z()*postlPos.z()<0){

         G4ThreeVector  lpos_detplane=prelPos+fabs(prelPos.z())/fabs(prelPos.z()-postlPos.z())*(postlPos-prelPos);
         //TVector3 lpos_detplane_t3(lpos_detplane.x()/cm,lpos_detplane.y()/cm,lpos_detplane.z()/cm);
         //TVector3 gpos_detplane_t3=ptr->GetGPos(lpos_detplane_t3,
         //      theta,(double)geom_->GTRy[idGEM][idTr],(double)geom_->GTRz[idGEM][idTr],0,0);
         //TVector3 gpos_detplane_t3=E16ANA_Transform::GetGPos(
         //      lpos_detplane_t3,
         //      geom_,idGEM,idTr);
         //G4ThreeVector gpos_detplane(gpos_detplane_t3.X()*cm,gpos_detplane_t3.Y()*cm,gpos_detplane_t3.Z()*cm);
         G4ThreeVector gpos_detplane = geom_->GTR(idTr,idGEM)->GetGPos(lpos_detplane);

         //G4cout << "ProcessHits:lpos_detplane="<<lpos_detplane/cm<<G4endl;

	 G4double time= aPreStepPoint-> GetGlobalTime();
	 //         G4cerr <<"GTR "<< time <<G4endl;

	 //Particle information
	 G4ParticleDefinition *particle= 
	   aTrack-> GetDynamicParticle()->GetDefinition();

	 const G4VProcess *proc=aTrack-> GetCreatorProcess();
	 G4String procName="Primary";
	 if(proc != NULL){
	   procName=proc->GetProcessName();
	 }

         aHit = new E16G4_GTRHit(lpos_detplane, time, charge);
         aHit->SetGPos(gpos_detplane);
         aHit->SetLPos(lpos_detplane);
         aHit->SetGMom(mom);
         aHit->SetLMom(lMom);
         aHit->SetEdep(  aStep->GetTotalEnergyDeposit() );
	 aHit->SetBeta(   aTrack-> GetVelocity()/c_light );

         aHit->SetLength( aStep->GetStepLength() );
         aHit->SetPName( particle->GetParticleName() );
         aHit->SetPCode( particle->GetPDGEncoding() );
         aHit->SetcrProcName(procName);
         aHit->SetTrackID(  aTrack->GetTrackID() );
         aHit->SetParentID( aTrack->GetParentID() );

         hitsCollection[id]-> insert(aHit);
         //G4cout << "GTRSD::ProcessHits : DetectorID = "<<idTr
         //   << ", ProcessName = " << procName
         //   <<G4endl;
      }
      //TVector3 gPos_t3=trTr->GetGPos(TVector3(lPos.x()/cm,lPos.y()/cm,lPos.z()/cm),idTr,idGEM);
      //G4cout << "ProcessHits:gPos_debug="<<gPos_t3.X()<<", "
      //<<gPos_t3.Y()<<", "<<gPos_t3.Z()<<G4endl;
      //G4ThreeVector lPos=elem_[id]->GetLocalPosition(pos);
      //G4ThreeVector lMom=elem_[id]->GetLocalDirection(mom);
      /*if (id==36){
        G4cout << "ProcessHits:pos="<<pos<<G4endl;
        G4cout << "ProcessHits:lPos="<<lPos<<G4endl;
        }*/

      //const G4int nHits=hitsCollection[id]->entries();
      //G4cout << "ProcessHits:nHits="<<nHits<<G4endl;

      //return true;
   }
   //delete ptr;
   return true;
}

///////////////////////////////////////////////////
void E16G4_GTRSD::EndOfEvent(G4HCofThisEvent* ){
   ///////////////////////////////////////////////////
}

//////////////////////////
void E16G4_GTRSD::DrawAll(){
   //////////////////////////
}

///////////////////////////
void E16G4_GTRSD::PrintAll(){
   ///////////////////////////
   for (int i=0;i<nID;i++){
      hitsCollection[i]-> PrintAllHits();
   }
}

