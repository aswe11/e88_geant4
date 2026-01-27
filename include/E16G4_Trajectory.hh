//2016-08-12, uploaded by nakai
//2016-05-05, uploaded by yokkaich
//2016-05-02, uploaded by obara
//2015-01-05, uploaded by yokkaich
//2015-01-05, uploaded by yokkaich
//2014-08-27, uploaded by kawama
//2014-08-27, uploaded by kawama
//2014-04-30, uploaded by kawama
//2013-11-14, uploaded by kawama
//2013-05-13, modified by kawama
/*
  E16G4_Trajectory.hh

  D.Kawama
*/

#ifndef E16G4_Trajectory_h

#define E16G4_Trajectory_h 1

#include <globals.hh>
#include <G4VTrajectory.hh>
#include <G4ThreeVector.hh>
#include <G4Allocator.hh>

#include <vector>

class G4Track;
class G4Step;

typedef std::vector <G4VTrajectoryPoint*> TrajectoryPointContainer;

class E16G4_Trajectory : public G4VTrajectory
{
public:
  E16G4_Trajectory();
  E16G4_Trajectory( const G4Track *aTrack );
  ~E16G4_Trajectory();

  inline void* operator new( std::size_t );
  inline void operator delete( void* );
  int operator == ( const E16G4_Trajectory &right ) const
  { return ( this == &right ); }


private:
  TrajectoryPointContainer* positionRecord;
  G4int trackID_;
  G4int parentID_;
  G4String particleName_;
  G4double PDGCharge_;
  G4int PDGEncoding_;
  
  G4int pID;
  
  G4ThreeVector gpos_, dpos_;
  G4ThreeVector gmom_, dmom_;

  G4double genTime;
  G4String genVol;
  
  G4String processName;
  G4int processSubType;

  G4ThreeVector gtr1HitPos1;
  G4int gtr1HitDetID1;
  G4ThreeVector gtr1HitPos2;
  G4int gtr1HitDetID2;

  G4ThreeVector gtr2HitPos1;
  G4int gtr2HitDetID1;
  G4ThreeVector gtr2HitPos2;
  G4int gtr2HitDetID2;

  G4ThreeVector gtr3HitPos1;
  G4int gtr3HitDetID1;
  G4ThreeVector gtr3HitPos2;
  G4int gtr3HitDetID2;
  
  G4ThreeVector hbdFrontHitPos;
  G4int hbdFrontHitDetID;
  G4ThreeVector hbdBackHitPos;
  G4int hbdBackHitDetID;
  
  G4ThreeVector lgHitPos;
  G4int lgHitDetID;
  
public:
  void ShowTrajectory( std::ostream& os = G4cout ) const;
  void DrawTrajectory( G4int i_mode=0 ) const;
  void AppendStep( const G4Step *aStep );
  int GetPointEntries() const
  { return positionRecord-> size(); }
  G4VTrajectoryPoint* GetPoint( int i ) const
  { return (*positionRecord)[i]; }
  void MergeTrajectory( G4VTrajectory* secondTrajectory );

  G4int GetTrackID( void )  const { return trackID_;  }
  G4int GetParentID( void ) const { return parentID_; }
  G4int GetPID(){return pID;}
  G4double GetGenerateTime(){return genTime;}
  
#ifdef GEANT44
  const G4String & GetParticleName( void ) const
  { return particleName_; }
  G4double GetPDGCharge( void ) const { return PDGCharge_; }
#else
  G4String GetParticleName( void ) const { return particleName_; }
  G4double GetCharge( void ) const { return PDGCharge_; }
  G4int GetPDGEncoding( void ) const { return PDGEncoding_; }
  G4ThreeVector GetInitialMomentum() const { return gmom_; }
#endif

  const G4ThreeVector & GetPositionAtFirst( void ) const 
  { return gpos_; }
  const G4ThreeVector & GetPositionAtLast( void ) const 
  { return dpos_; }
  const G4ThreeVector & GetMomentumAtFirst( void ) const 
  { return gmom_; }
  const G4ThreeVector & GetMomentumAtLast( void ) const 
  { return dmom_; }

  void SetPositionAtLast( const G4ThreeVector &pos ){ dpos_=pos; }
  void SetMomentumAtLast( const G4ThreeVector &mom ){ dmom_=mom; }

  G4ThreeVector GetInitialVertex(){ return gpos_;}
  G4String GetProcessName(){return processName;}
  G4String GetGenerateVolume(){return genVol;}
  
  // void SetGTR1HitPos1(const G4ThreeVector& pos){ gtr1HitPos1 = pos;}
  // void SetGTR1HitDetID1(G4int id){ gtr1HitDetID1 = id;}
  // void SetGTR1HitPos2(const G4ThreeVector& pos){ gtr1HitPos2 = pos;}
  // void SetGTR1HitDetID2(G4int id){ gtr1HitDetID2 = id;}

  // G4ThreeVector GetGTR1HitPos1(){return gtr1HitPos1;}
  // G4int GetGTR1HitDetID1(){ return gtr1HitDetID1;}
  // G4ThreeVector GetGTR1HitPos1(){return gtr1HitPos1;}
  // G4int GetGTR1HitDetID1(){ return gtr1HitDetID1;}
  
  // void SetGTR2HitPos1(const G4ThreeVector& pos){ gtr2HitPos1 = pos;}
  // void SetGTR2HitDetID1(G4int id){ gtr2HitDetID1 = id;}
  // void SetGTR2HitPos2(const G4ThreeVector& pos){ gtr2HitPos2 = pos;}
  // void SetGTR2HitDetID2(G4int id){ gtr2HitDetID2 = id;}

  // void SetGTR3HitPos1(const G4ThreeVector& pos){ gtr3HitPos1 = pos;}
  // void SetGTR3HitDetID1(G4int id){ gtr3HitDetID1 = id;}
  // void SetGTR3HitPos2(const G4ThreeVector& pos){ gtr3HitPos2 = pos;}
  // void SetGTR3HitDetID2(G4int id){ gtr3HitDetID2 = id;}

  
};

extern G4Allocator<E16G4_Trajectory> TrajectoryAllocator;

inline void* E16G4_Trajectory::operator new( std::size_t )
{
  return static_cast<void*>( TrajectoryAllocator.MallocSingle() );
}

inline void E16G4_Trajectory::operator delete( void *aHit )
{
  TrajectoryAllocator.FreeSingle
    ( static_cast<E16G4_Trajectory*>( aHit ) );
}

#endif//E16G4_Trajectory_h


