//2016-08-12, uploaded by nakai
//2015-01-05, modified by yokkaich
//2015-01-05, modified by yokkaich
//2014-08-27, modified by kawama
//2014-08-27, modified by kawama
//2014-04-30, modified by kawama
//2013-11-14, modified by kawama
//2013-05-13, modified by kawama
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"

#include "E16G4_Analysis.hh"
#include "E16G4_EventAction.hh"
#include <time.h>

 E16G4_EventAction::E16G4_EventAction(E16G4_Analysis *anaManager)
   :G4UserEventAction(), anaMan(anaManager)
{
}

 E16G4_EventAction::~E16G4_EventAction(){
}

void E16G4_EventAction::BeginOfEventAction(const G4Event* anEvent){
   clock_t begin_begin, begin_end;
   begin_begin=clock();
   if(anaMan) anaMan->BeginOfEvent(anEvent);
   begin_end=clock();
   //G4cout <<"Begin of EventAction:"<<(double)(begin_end-begin_begin)/CLOCKS_PER_SEC<<G4endl;
   //G4cout <<"Begin of EventAction"<<G4endl;
   //ResetDecayChain();
}

void E16G4_EventAction::EndOfEventAction(const G4Event* anEvent){
   clock_t end_begin, end_end;
   end_begin=clock();
   if(anaMan) anaMan->EndOfEvent(anEvent);
   end_end=clock();
   //G4cout <<"End of EventAction:"<<(double)(end_end-end_begin)/CLOCKS_PER_SEC<<G4endl;
   //G4cout <<" ********** End of EventAction **********"<<G4endl;
   //AddDecayChain("EndOfDecayChain\n");
   //G4cout <<" decay chain:"<<decayChain<<G4endl;
   //G4cout <<" size:"<<decayChain.size()<<G4endl;
}

