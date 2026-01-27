//2016-08-12, uploaded by nakai
//2015-01-05, uploaded by yokkaich
//2015-01-05, uploaded by yokkaich
//2014-08-27, uploaded by kawama
//2014-08-27, uploaded by kawama
//2014-04-30, uploaded by kawama
//2013-11-14, uploaded by kawama
//2013-05-13, modified by kawama
/*
  E16G4_Transportation.hh

  D.Kawama
*/

#ifndef E16G4_Transportation_h
#define E16G4_Transportation_h 1

#include <G4Transportation.hh>

class E16G4_Transportation : public G4Transportation
{
public:
  E16G4_Transportation()
    : G4Transportation() {}
  ~E16G4_Transportation() {}

  G4double AlongStepGetPhysicalInteractionLength( const G4Track & track, 
						  G4double previousStepSize,
						  G4double currentMinimumStep, 
						  G4double & currentSafety,
						  G4GPILSelection* selection );

};

#endif//E16G4_Transportation_h

