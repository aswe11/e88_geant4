//2016-08-12, uploaded by nakai
//2016-03-18, uploaded by nakai
//2015-01-05, uploaded by yokkaich
//2015-01-05, uploaded by yokkaich
//2014-08-27, uploaded by kawama
//2014-08-27, uploaded by kawama
//2014-04-30, uploaded by kawama
//2013-11-14, uploaded by kawama
//2013-05-13, modified by kawama

//E16G4_GTRSD.hh 150105 
//    Last modified at <2015-01-05 13:55:42 >


#ifndef GTR_SD_H
#define GTR_SD_H

#include <G4VSensitiveDetector.hh>
#include "E16G4_GTRHit.hh"
#include "E16ANA_GeometryV2.hh"
#include "E16ANA_Transform.hh"

class G4HCofThisEvent;
class G4Step;
class G4TouchableHistory;
class E16G4_BeamLineElement;
class E16ANA_GeometryV2;
class E16ANA_Transform;

class E16G4_GTRSD : public G4VSensitiveDetector{
   private:
      enum{nID=33*3};
      GTRHitsCollection* hitsCollection[nID];
      //G4THitsCollection<GTRHit>* hitsCollection[nID];
      E16G4_BeamLineElement *elem_[nID];
      const E16ANA_GeometryV2 *geom_;
   const E16ANA_Transform *ptr_;

   public:
      E16G4_GTRSD(const G4String& name, const E16ANA_GeometryV2* geom);
      virtual ~E16G4_GTRSD();

      virtual G4bool ProcessHits(G4Step* astep, G4TouchableHistory* ROhist);
      virtual void Initialize(G4HCofThisEvent* HCTE);
      virtual void EndOfEvent(G4HCofThisEvent* HCTE);
      virtual void DrawAll();
      virtual void PrintAll();
      void SetElement(int id, E16G4_BeamLineElement *elem);
};


#endif//GTR_SD_H


