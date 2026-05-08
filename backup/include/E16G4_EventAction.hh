//2016-08-12, uploaded by nakai
//2015-01-05, uploaded by yokkaich
//2015-01-05, uploaded by yokkaich
//2014-08-27, uploaded by kawama
//2014-08-27, uploaded by kawama
//2014-04-30, uploaded by kawama
//2013-11-14, uploaded by kawama
//2013-05-13, modified by kawama

#ifndef E16G4_EVENT_ACTION_HH
#define E16G4_EVENT_ACTION_HH

#include <G4UserEventAction.hh>
#include <globals.hh>

class G4Event;
class E16G4_Analysis;

class E16G4_EventAction : public G4UserEventAction{
   private:
      E16G4_Analysis *anaMan;
      G4String decayChain;
   public:
      E16G4_EventAction(E16G4_Analysis *anaManager=0);
      virtual ~E16G4_EventAction();

      virtual void BeginOfEventAction(const G4Event* anEvent);
      virtual void EndOfEventAction(const G4Event* anEvent);

      void AddDecayChain(G4String val){decayChain+=val;};
      void ResetDecayChain(){decayChain="";};
      

};

#endif//E16G4_EVENT_ACTION_HH


