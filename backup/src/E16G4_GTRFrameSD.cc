//2016-08-12, uploaded by nakai
//2015-01-05, modified by yokkaich
//2015-01-05, modified by yokkaich
//2014-08-27, modified by kawama
//2014-08-27, modified by kawama
//2014-04-30, modified by kawama
//2013-11-14, modified by kawama
//2013-05-13, modified by kawama
/*
  E16G4_GTRFrameSD.cc

  D.Kawama
*/

#include "G4HCofThisEvent.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VTouchable.hh"
#include "G4VProcess.hh"
#include "G4TouchableHistory.hh"
#include "G4TouchableHandle.hh"
#include "G4VVisManager.hh"

#include "E16G4_GTRFrameSD.hh"
#include "E16G4_BeamLineElement.hh"

#include <sstream>

//////////////////////////////////
E16G4_GTRFrameSD::E16G4_GTRFrameSD( G4String name )
  : G4VSensitiveDetector(name)
/////////////////////////////////
{
  for( int i=0; i<nGTRFrame; i++ ){
    std::ostringstream id;
    id << i;
    G4String Name = name+id.str().c_str()+"-Collection";
    //G4cout <<Name<<G4endl;
    collectionName.insert( Name );
    elem_[i] = 0;
  }
}

////////////////////
E16G4_GTRFrameSD::~E16G4_GTRFrameSD()
///////////////////
{
}

/////////////////////////////////////////////////////////
void E16G4_GTRFrameSD::SetElement( int id, E16G4_BeamLineElement *elem )
/////////////////////////////////////////////////////////
{
  if( id >= 0 && id < nGTRFrame ) elem_[id] = elem;
}

/////////////////////////////////////////////////
void E16G4_GTRFrameSD::Initialize( G4HCofThisEvent *HCE )
/////////////////////////////////////////////////
{
  for( int i=0; i<nGTRFrame ; i++ ){
    hitsCollection[i] = 
      new GTRFrameHitsCollection( SensitiveDetectorName,
			       collectionName[i]);
      //G4cout <<i<<" "<<SensitiveDetectorName<<" "<<
	//		       collectionName[i]<<G4endl;
    G4int HCID = GetCollectionID(i);
    HCE-> AddHitsCollection( HCID, hitsCollection[i] );
  }
}

//////////////////////////////////////////////////////////////
G4bool E16G4_GTRFrameSD::ProcessHits( G4Step* aStep,
				 G4TouchableHistory* )
/////////////////////////////////////////////////////////////	
{
  G4Track* aTrack = aStep-> GetTrack();
  const G4VTouchable* theTouchable =
    aStep-> GetPreStepPoint()-> GetTouchable();
  G4VPhysicalVolume *PV = theTouchable-> GetVolume();
   G4String Name = PV->GetName();
   //G4cout << "ProcessHits:Names="<<Name<<G4endl;
   std::stringstream sid(Name.substr(6,2));
   int id=-1;sid>>id;
  //G4double Charge=aStep->GetTrack()->GetDefinition()->GetPDGCharge();
   const G4ThreeVector vtx = aTrack->GetVertexPosition();

  if( id>=0 && id<nGTRFrame && elem_[id]){
    G4ThreeVector hitPos = aStep-> GetPreStepPoint()-> GetPosition();
    G4ThreeVector hitMom = aTrack-> GetMomentum();
    G4double edep = aStep->GetTotalEnergyDeposit();
    G4ThreeVector vertex = aTrack->GetVertexPosition();
    G4double tLength = aTrack->GetTrackLength();

    G4ThreeVector lPos = elem_[id]-> GetLocalPosition( hitPos );
    G4ThreeVector lMom = elem_[id]-> GetLocalDirection( hitMom );
    //G4double u = lMom.x()/lMom.z(), v = lMom.y()/lMom.z();
    //G4double x = lPos.x()-u*lPos.z(), y = lPos.y()-v*lPos.z();
    
    G4ParticleDefinition *particle =
    aTrack-> GetDynamicParticle()-> GetDefinition();
	 G4String particlename =particle-> GetParticleName();
   G4int PDGCode=particle->GetPDGEncoding();
   G4int tid=aTrack->GetTrackID();
   //G4int ptid=aTrack->GetParentID();
   
   const G4VProcess *proc=aTrack-> GetCreatorProcess();
   G4String procName;
   if(proc!=0){
      procName=proc->GetProcessName();
   }
   else{
      procName="Primary";
   }
   /*if (PDGCode==22){
      G4cout <<"Gamma creation process and momentum:"<<procName
         <<" "<<lMom.mag()/MeV<<G4endl;
   }*/

    E16G4_GTRFrameHit *aHit =
      new E16G4_GTRFrameHit( id, hitPos, hitMom, edep, vertex, particlename );
    aHit-> SetGPos( hitPos );
    aHit-> SetLPos( lPos );
    aHit-> SetLMom( lMom );
    aHit->SetPCode(PDGCode);
    aHit->SetTrackID(tid);
    aHit-> SetEdep( edep );
    aHit-> SetVertex( vertex );
    aHit-> SettLength( tLength );
    aHit->SetProcName(procName);
    /*G4cout << " tid="<<tid<<" id="<<id
       <<" PDG="<<PDGCode<<" procname="<<procName
       <<" Mom="<<lMom.mag()<<" vtx="<<vtx.z()<<
       G4endl;*/
    //G4cout << "Hit GTRFrame"<<id<<G4endl;

    hitsCollection[id]-> insert( aHit );
  }
  return true;
}

/////////////////////////////////////////////////
void E16G4_GTRFrameSD::EndOfEvent( G4HCofThisEvent* )
/////////////////////////////////////////////////
{
}

//////////////////////
void E16G4_GTRFrameSD::clear()
/////////////////////
{
  for( int i=0; i<nGTRFrame; ++i ){
    G4int nHits = hitsCollection[i]-> entries();
    for( G4int j=0; j<nHits; ++j ){
      delete (*hitsCollection[i])[j];
    }
  }
}

///////////////////////
void E16G4_GTRFrameSD::DrawAll()
///////////////////////
{
  G4VVisManager* pVisManager = G4VVisManager::GetConcreteInstance();
  if( pVisManager ){
    for( int j=0; j<nGTRFrame; ++j ){
      G4int nHits = hitsCollection[j]-> entries();
      for( G4int i=0; i<nHits; i++ ){
	(*hitsCollection[j])[i]-> Draw();
      }
    }
  }
}

/////////////////////////
void E16G4_GTRFrameSD::PrintAll() 
/////////////////////////
{
  for( int j=0; j<nGTRFrame; ++j ){
    G4int nHits = hitsCollection[j]-> entries();
    for( G4int i=0; i<nHits; i++ ){
      (*hitsCollection[j])[i]-> Print();
    }
  }
}

