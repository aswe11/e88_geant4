//2016-08-12, uploaded by nakai
//2015-01-05, modified by yokkaich
//2015-01-05, modified by yokkaich
//2014-08-27, modified by kawama
//2014-08-27, modified by kawama
//2014-04-30, modified by kawama
//2013-11-14, modified by kawama
//2013-05-13, modified by kawama
/*
  E16G4_Transportation.cc

  D.Kawama
*/

#include "E16G4_Transportation.hh"

////////////////////////////////////////////////////////////////////
G4double E16G4_Transportation::
AlongStepGetPhysicalInteractionLength( const G4Track & track, 
				       G4double previousStepSize,
				       G4double currentMinimumStep, 
				       G4double & currentSafety,
				       G4GPILSelection *selection )
////////////////////////////////////////////////////////////////////		  
{
  /*if( this-> DoesGlobalFieldExist() &&
      currentMinimumStep>2.*cm){
    currentMinimumStep = 2.*cm;
  }*/
  if( this-> DoesGlobalFieldExist() &&
      currentMinimumStep>1.*cm){
    currentMinimumStep = 1.*cm;
  }

  return G4Transportation::
    AlongStepGetPhysicalInteractionLength( track, previousStepSize, 
					   currentMinimumStep,
					   currentSafety, selection );
  
}
