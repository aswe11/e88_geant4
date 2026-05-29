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
  E16G4_VDSD.cc

  D.Kawama
*/
#include <sstream>

#include <G4HCofThisEvent.hh>
#include <G4VPhysicalVolume.hh>
#include <G4Step.hh>
#include <G4Track.hh>
#include <G4VTouchable.hh>
#include <G4VProcess.hh>
#include <G4TouchableHistory.hh>
#include <G4TouchableHandle.hh>
#include <G4VVisManager.hh>

#include "E16G4_VDSD.hh"
#include "E16G4_BeamLineElement.hh"



//////////////////////////////////
E16G4_VDSD::E16G4_VDSD( G4String name )
  : G4VSensitiveDetector(name)
/////////////////////////////////
{
  for( int i=0; i<nVD; ++i ){
    std::ostringstream id;
    id << i;
    G4String Name = name+id.str().c_str()+"-Collection";
    collectionName.insert( Name );
    elem_[i] = 0;
  }
}

////////////////////
E16G4_VDSD::~E16G4_VDSD()
///////////////////
{
}

/////////////////////////////////////////////////////////
void E16G4_VDSD::SetElement( int id, E16G4_BeamLineElement *elem )
/////////////////////////////////////////////////////////
{
  if( id >= 0 && id < nVD ) elem_[id] = elem;
}

/////////////////////////////////////////////////
void E16G4_VDSD::Initialize( G4HCofThisEvent *HCE )
/////////////////////////////////////////////////
{
  for( int i=0; i<nVD ; ++i ){
    hitsCollection[i] = 
      new VDHitsCollection( SensitiveDetectorName,
			       collectionName[i]);
    G4int HCID = GetCollectionID(i);
    HCE-> AddHitsCollection( HCID, hitsCollection[i] );
  }
}

//////////////////////////////////////////////////////////////
G4bool E16G4_VDSD::ProcessHits( G4Step* aStep,
				 G4TouchableHistory* )
/////////////////////////////////////////////////////////////	
{
  G4Track* aTrack = aStep-> GetTrack();
  const G4VTouchable* theTouchable =
    aStep-> GetPreStepPoint()-> GetTouchable();
  G4VPhysicalVolume *Vol = theTouchable-> GetVolume();
  G4int id = Vol->GetCopyNo();
  //G4double Charge=aStep->GetTrack()->GetDefinition()->GetPDGCharge();
   const G4ThreeVector vtx = aTrack->GetVertexPosition();

  //if( id>=0 && id<nVD && elem_[id] && Charge!=0){
  if( id>=0 && id<nVD && elem_[id]){
    G4ThreeVector hitPos = aStep-> GetPreStepPoint()-> GetPosition();
    G4ThreeVector hitMom = aTrack-> GetMomentum();
    G4double edep = aStep->GetTotalEnergyDeposit();
    G4ThreeVector vertex = aTrack->GetVertexPosition();
    G4double tLength = aTrack->GetTrackLength();
    G4double time = aStep->GetPreStepPoint()->GetGlobalTime();

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

    E16G4_VDHit *aHit =
      new E16G4_VDHit( id, hitPos, hitMom, edep, vertex, particlename );
    aHit-> SetGPos( hitPos );
    aHit-> SetLPos( lPos );
    aHit-> SetLMom( lMom );
    /*G4cout << " tid="<<tid<<" ptid="<<ptid
       <<" PDG="<<PDGCode<<" pname="<<particlename
       <<" Mom="<<lMom.mag()<<" vtx="<<vtx.z()<<G4endl;*/
    aHit->SetPCode(PDGCode);
    aHit->SetTrackID(tid);
    aHit-> SetEdep( edep );
    aHit-> SetVertex( vertex );
    aHit-> SettLength( tLength );
    aHit->SetProcName(procName);
    aHit->SetTime(time);
    //G4cout << "Hit VD"<<id<<G4endl;

    hitsCollection[id]-> insert( aHit );
  }
  return true;
}

/////////////////////////////////////////////////
void E16G4_VDSD::EndOfEvent( G4HCofThisEvent* )
/////////////////////////////////////////////////
{
}

//////////////////////
void E16G4_VDSD::clear()
/////////////////////
{
  for( int i=0; i<nVD; ++i ){
    G4int nHits = hitsCollection[i]-> entries();
    for( G4int j=0; j<nHits; ++j ){
      delete (*hitsCollection[i])[j];
    }
  }
}

///////////////////////
void E16G4_VDSD::DrawAll()
///////////////////////
{
  G4VVisManager* pVisManager = G4VVisManager::GetConcreteInstance();
  if( pVisManager ){
    for( int j=0; j<nVD; ++j ){
      G4int nHits = hitsCollection[j]-> entries();
      for( G4int i=0; i<nHits; i++ ){
	(*hitsCollection[j])[i]-> Draw();
      }
    }
  }
}

/////////////////////////
void E16G4_VDSD::PrintAll() 
/////////////////////////
{
  for( int j=0; j<nVD; ++j ){
    G4int nHits = hitsCollection[j]-> entries();
    for( G4int i=0; i<nHits; i++ ){
      (*hitsCollection[j])[i]-> Print();
    }
  }
}

