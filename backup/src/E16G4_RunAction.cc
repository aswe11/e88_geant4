//2016-08-12, uploaded by nakai
//2015-01-20, uploaded by yokkaich
//2015-01-05, modified by yokkaich
//2015-01-05, modified by yokkaich
//2014-08-27, modified by kawama
//2014-08-27, modified by kawama
//2014-04-30, modified by kawama
//2013-11-14, modified by kawama
//2013-05-13, modified by kawama

//E16G4_RunAction.cc 150118 
//    Last modified at <>
//
#include <G4Run.hh>

#include "E16G4_RunAction.hh"
#include "E16G4_Analysis.hh"
#include "E16G4_PrimaryGeneratorAction.hh"

////////////////////////////////////////////
E16G4_RunAction::E16G4_RunAction(E16G4_Analysis *anaManager, 
      E16G4_PrimaryGeneratorAction *primarygeneratoraction)
   :G4UserRunAction(), anaMan(anaManager), PrimaryGeneratorAction(primarygeneratoraction)
////////////////////////////////////////////
{
}

/////////////////////////
 E16G4_RunAction::~E16G4_RunAction()
/////////////////////////
{
}

////////////////////////////////////////////////////
void E16G4_RunAction::BeginOfRunAction(const G4Run* aRun)
////////////////////////////////////////////////////
{
   if(anaMan) anaMan->BeginOfRun(aRun);
   if(PrimaryGeneratorAction) {
      PrimaryGeneratorAction->SetPrimaryConditions();
   }
}

////////////////////////////////////////////////////
void E16G4_RunAction::EndOfRunAction(const G4Run* aRun)
////////////////////////////////////////////////////
{
   if(anaMan) anaMan->EndOfRun(aRun);
   G4cout <<"End of RunAction"<<G4endl;
}

