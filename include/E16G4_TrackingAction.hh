//2016-08-12, uploaded by nakai
//2015-01-05, uploaded by yokkaich
//2015-01-05, uploaded by yokkaich
//2014-08-27, uploaded by kawama
//2014-08-27, uploaded by kawama
//2014-04-30, uploaded by kawama
//2013-11-14, uploaded by kawama
//2013-05-13, modified by kawama
/*
  E16G4_TrackingAction.hh

  D.Kawama
*/

#ifndef E16G4_TrackingAction_h
#define E16G4_TrackingAction_h

#include <G4UserTrackingAction.hh>

class E16G4_EventAction;
class E16G4_Analysis;

class G4Track;

class E16G4_TrackingAction : public G4UserTrackingAction
{
public:
  E16G4_TrackingAction(E16G4_EventAction*,E16G4_Analysis*);
  E16G4_TrackingAction( const G4Track* aTrack );
  ~E16G4_TrackingAction();

  void PreUserTrackingAction( const G4Track* aTrack );
  void PostUserTrackingAction( const G4Track* aTrack );

  int GetiSec( void ) const { return iSec; }
  
private:
  char secList[100][10];
  int iSec;
  E16G4_EventAction *event;
  E16G4_Analysis *anaMan;

};

#endif//E16G4_TrackingAction_h

