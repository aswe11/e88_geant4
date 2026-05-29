//2016-08-12, uploaded by nakai
//2016-05-09, uploaded by yokkaich
//2016-05-05, uploaded by yokkaich
//2016-05-02, uploaded by obara
//2015-01-05, modified by yokkaich
//2015-01-05, modified by yokkaich
//2014-08-27, modified by kawama
//2014-08-27, modified by kawama
//2014-04-30, modified by kawama
//2013-11-14, modified by kawama
//2013-05-13, modified by kawama
/*
  E16G4_Trajectiry.cc

 
  D.Kawama
*/

#include "E16G4_Trajectory.hh"

#include <algorithm>
//#include "TemplateLib.hh"

#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4TrajectoryPoint.hh"
#include "G4ios.hh"
#include "G4VVisManager.hh"
#include "G4Polyline.hh"
#include "G4Colour.hh"
#include "G4Circle.hh"
#include "G4VisAttributes.hh"
#include <vector> 
#include "G4VProcess.hh"

G4Allocator<E16G4_Trajectory> TrajectoryAllocator;

///////////////////////////////////////////////////////
E16G4_Trajectory::E16G4_Trajectory( const G4Track* aTrack)
  : positionRecord( new TrajectoryPointContainer ),
    trackID_( aTrack-> GetTrackID() ),
    parentID_( aTrack-> GetParentID() ),
    gpos_( aTrack-> GetPosition() ),
    gmom_( aTrack-> GetMomentum() ),
    gtr1HitPos1(G4ThreeVector(-10000., -10000., -10000.)),
    gtr1HitDetID1(-1),
    gtr1HitPos2(G4ThreeVector(-10000., -10000., -10000.)),
    gtr1HitDetID2(-1),
    gtr2HitPos1(G4ThreeVector(-10000., -10000., -10000.)),
    gtr2HitDetID1(-1),
    gtr2HitPos2(G4ThreeVector(-10000., -10000., -10000.)),
    gtr2HitDetID2(-1),
    gtr3HitPos1(G4ThreeVector(-10000., -10000., -10000.)),
    gtr3HitDetID1(-1),
    gtr3HitPos2(G4ThreeVector(-10000., -10000., -10000.)),
    gtr3HitDetID2(-1),
    hbdFrontHitPos(G4ThreeVector(-10000., -10000., -10000.)),
    hbdFrontHitDetID(-1),
    hbdBackHitPos(G4ThreeVector(-10000., -10000., -10000.)),
    hbdBackHitDetID(-1),
    lgHitPos(G4ThreeVector(-10000., -10000., -10000.)),
    lgHitDetID(-1)
//////////////////////////////////////////////////////
{
  G4ParticleDefinition* fpPD = aTrack-> GetDefinition();
  particleName_ = fpPD-> GetParticleName();
  PDGCharge_ = fpPD-> GetPDGCharge();
  pID = fpPD->GetPDGEncoding();
  genTime = aTrack->GetGlobalTime();
  
  const G4VProcess *proc = aTrack->GetCreatorProcess();

  if(proc!=0){
    processName = proc->GetProcessName();
    processSubType = proc->GetProcessSubType();
  }
  else{
    processName = "Primary";
    processSubType = -1;
  }
  

}

////////////////////////////////
 E16G4_Trajectory::~E16G4_Trajectory()
////////////////////////////////
{
  //for_each( positionRecord-> begin(),
	//    positionRecord-> end(), DeleteObject());
  if( positionRecord ){
    for(int i=0;i< (int)positionRecord->size();i++){
      delete (*positionRecord)[i];
    }
    positionRecord->clear();
    delete positionRecord;
  }


}

/////////////////////////////////////////////////////////
void E16G4_Trajectory::ShowTrajectory( std::ostream& ) const
////////////////////////////////////////////////////////
{
  G4cout << G4endl << "TrackID = " << trackID_
	 << " : ParentID = " << parentID_ << G4endl;

  G4cout << "ParticleName : " << particleName_ << G4endl;
  G4cout << "Process Name : " << processName << G4endl;
  G4cout << "Process Sub Type : " << processSubType << G4endl;
  
  G4cout << "Current trajectory has " << positionRecord-> size()
	 << " point" << G4endl;

  G4cout << "Initial Pos. = " << gpos_ << G4endl;
  G4cout << "Initial Mom. = " << gmom_ << G4endl;
  
  // for( G4int i=0; G4int(positionRecord-> size()); ++i ){
  //   G4TrajectoryPoint* tp = 
  //     dynamic_cast<G4TrajectoryPoint*> ((*positionRecord)[i]);
    
  //   G4cout << "Point[" << i << "]"
  // 	   << "Position = " << tp-> GetPosition() << G4endl;
  // }
}

//////////////////////////////////////////////////////////
void E16G4_Trajectory::DrawTrajectory( G4int i_mode ) const
//////////////////////////////////////////////////////////
{
  G4VVisManager* pVisManager = 
    G4VVisManager::GetConcreteInstance();

  if( pVisManager ){
    if(i_mode >= 0 ){
      G4Polyline pPolyline;
      for( G4int i=0 ; i<G4int(positionRecord-> size());++i){
	G4TrajectoryPoint* tp = 
	  dynamic_cast<G4TrajectoryPoint*> ((*positionRecord)[i]);
	pPolyline.push_back( tp-> GetPosition() );
      }
      G4Colour colour;
      if( PDGCharge_<0. )      colour = G4Colour( 1.0, 0.0, 0.0 );
      else if( PDGCharge_>0. ) colour = G4Colour( 0.0, 0.0, 1.0 );
      else                     colour = G4Colour( 0.0, 1.0, 0.0 );
      //colour = G4Colour( 1.0, 0.0, 0.0 );

      pPolyline.SetVisAttributes( G4VisAttributes(colour) );
      pVisManager-> Draw( pPolyline );
    }

    if( i_mode != 0 ){
      for( G4int i=0 ; i<G4int(positionRecord-> size());++i){
	G4TrajectoryPoint* tp = 
	  dynamic_cast<G4TrajectoryPoint*> ((*positionRecord)[i]);
	G4Circle circle( tp-> GetPosition() );
	circle.SetScreenSize( 0.001*i_mode );
	circle.SetFillStyle( G4Circle::filled );
	circle.SetVisAttributes( G4VisAttributes(G4Colour(0.,0.,0.)) );
	pVisManager-> Draw( circle );
      }
    }
  }
}

//////////////////////////////////////////////////////
void E16G4_Trajectory::AppendStep( const G4Step* aStep )
//////////////////////////////////////////////////////
{
  positionRecord-> push_back
    ( new G4TrajectoryPoint
      ( aStep-> GetPostStepPoint()-> GetPosition() ) );
}

///////////////////////////////////////////////////////////////////////
void E16G4_Trajectory::MergeTrajectory( G4VTrajectory* secondTrajectory )
///////////////////////////////////////////////////////////////////////
{
  E16G4_Trajectory* second = 
    dynamic_cast<E16G4_Trajectory*>( secondTrajectory );
  if( second ){
    G4int ent = second-> GetPointEntries();
    for( G4int i=0; i<ent; ++i ){
      positionRecord-> push_back((*(second-> positionRecord))[i]);
    }
    delete (*(second-> positionRecord))[0];
    second-> positionRecord-> clear();
    dpos_ = second-> dpos_;
    second-> dpos_ = second-> gpos_;
    dmom_ = second-> dmom_;
    second-> dmom_ = second-> gmom_;
  }
}

