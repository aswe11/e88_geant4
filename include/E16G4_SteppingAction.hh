//2016-08-12, uploaded by nakai
//2015-01-20, uploaded by yokkaich
//2015-01-05, uploaded by yokkaich
//2015-01-05, uploaded by yokkaich
//2014-08-27, uploaded by kawama
//2014-08-27, uploaded by kawama
//2014-04-30, uploaded by kawama
//2013-11-14, uploaded by kawama
//2013-05-13, modified by kawama
/*
  E16G4_SteppingAction.hh

*/

#ifndef E16G4_SteppingAction_h
#define E16G4_SteppingAction_h 1

#include <G4UserSteppingAction.hh>

class G4Step;

class E16G4_SteppingAction : public G4UserSteppingAction
{
public:
  E16G4_SteppingAction();
  ~E16G4_SteppingAction();

  void UserSteppingAction( const G4Step *aStep );
  //  E16ANA_MagneticFieldMap fmf;

  char VolumeList[10000][100];

};
#endif// E16G4_SteppingAction_h

 
