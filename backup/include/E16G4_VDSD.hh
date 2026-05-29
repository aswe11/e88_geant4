//2016-08-12, uploaded by nakai
//2015-01-05, uploaded by yokkaich
//2015-01-05, uploaded by yokkaich
//2014-08-27, uploaded by kawama
//2014-08-27, uploaded by kawama
//2014-04-30, uploaded by kawama
//2013-11-14, uploaded by kawama
//2013-05-13, modified by kawama
/*
  E16G4_VDSD.hh

  D.Kawama
*/

#ifndef E16G4_VDSD_h
#define E16G4_VDSD_h 1

#include <G4VSensitiveDetector.hh>
#include "E16G4_VDHit.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;
class E16G4_BeamLineElement;

class E16G4_VDSD : public G4VSensitiveDetector
{
public:
  E16G4_VDSD( G4String name );
  virtual ~E16G4_VDSD();

  void Initialize( G4HCofThisEvent *HCE );
  G4bool ProcessHits( G4Step* aStep, G4TouchableHistory* ROhist );
   void EndOfEvent( G4HCofThisEvent* HCE );

   void DrawAll() ;
   void PrintAll();
  void clear();

  void SetElement( int id, E16G4_BeamLineElement *elem);

private:
  enum{nVD=27};
  VDHitsCollection* hitsCollection[nVD];
  E16G4_BeamLineElement *elem_[nVD];
};

#endif//E16G4_VDSD_h

