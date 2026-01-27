//2016-08-12, uploaded by nakai
//2015-01-05, uploaded by yokkaich
//2015-01-05, uploaded by yokkaich
//2014-08-27, uploaded by kawama
//2014-08-27, uploaded by kawama
//2014-04-30, uploaded by kawama
//2013-11-14, uploaded by kawama
//2013-05-13, modified by kawama


//E16G4_LGSD.hh 150105 
//    Last modified at <>


#ifndef E16G4_LG_SD_HH
#define E16G4_LG_SD_HH

#include <G4VSensitiveDetector.hh>

//#include "E16G4_LGHit.hh"

class G4HCofThisEvent;
class G4Step;
class G4TouchableHistory;
class E16G4_BeamLineElement;

class E16G4_LGHit;

class E16G4_LGSD : public G4VSensitiveDetector{
   private:
      enum{nID=3*9*36};
      LGHitsCollection* hitsCollection[nID];
      E16G4_BeamLineElement *elem_[nID];
      E16G4_LGHit* aHit;

   public:
      E16G4_LGSD(const G4String& name);
      virtual ~E16G4_LGSD();

      virtual G4bool ProcessHits(G4Step* astep, G4TouchableHistory* ROhist);
      virtual void Initialize(G4HCofThisEvent* HCTE);
      virtual void EndOfEvent(G4HCofThisEvent* HCTE);
      virtual void DrawAll();
      virtual void PrintAll();
      void SetElement(int id, E16G4_BeamLineElement *elem);
};


#endif//E16G4_LG_SD_HH


