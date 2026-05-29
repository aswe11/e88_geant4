//2016-08-12, uploaded by nakai
//2015-01-05, uploaded by yokkaich
//2015-01-05, uploaded by yokkaich
//2014-08-27, uploaded by kawama
//2014-08-27, uploaded by kawama
//2014-04-30, uploaded by kawama
//2013-11-14, uploaded by kawama
//2013-05-13, modified by kawama

//E16G4_TargetSD.hh 150105 
//    Last modified at <2015-01-05 14:10:35 >
//

#ifndef E16G4_TARGET_SD_H
#define E16G4_TARGET_SD_H

#include <G4VSensitiveDetector.hh>
#include "E16G4_TargetHit.hh"

class G4HCofThisEvent;
class G4Step;
class G4TouchableHistory;
class E16G4_BeamLineElement;

class E16G4_TargetSD : public G4VSensitiveDetector{
   private:
      enum{nID=5};
      TargetHitsCollection* hitsCollection[nID];
      //G4THitsCollection<TargetHit>* hitsCollection[nID];
      E16G4_BeamLineElement *elem_[nID];

   public:
      E16G4_TargetSD(const G4String& name);
      virtual ~E16G4_TargetSD();

      virtual G4bool ProcessHits(G4Step* astep, G4TouchableHistory* ROhist);
      virtual void Initialize(G4HCofThisEvent* HCTE);
      virtual void EndOfEvent(G4HCofThisEvent* HCTE);
      virtual void DrawAll();
      virtual void PrintAll();
      void SetElement(int id, E16G4_BeamLineElement *elem);
};


#endif//E16G4_TARGET_SD_H


