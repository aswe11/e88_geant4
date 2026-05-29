//2016-08-12, uploaded by nakai
//2016-03-18, uploaded by nakai
//2015-01-05, uploaded by yokkaich
//2015-01-05, uploaded by yokkaich
//2014-08-27, uploaded by kawama
//2014-08-27, uploaded by kawama
//2014-04-30, uploaded by kawama
//2013-11-14, uploaded by kawama
//2013-05-13, modified by kawama

//E16G4_HBDSD.hh 150105 
//    Last modified at <2015-01-05 14:13:11 >

#ifndef E16G4_HBD_SD_HH
#define E16G4_HBD_SD_HH

#include <G4VSensitiveDetector.hh>

#include "E16ANA_GeometryV2.hh"
#include  "E16G4_HBDHit.hh"

class G4HCofThisEvent;
class G4Step;
class G4TouchableHistory;
class E16G4_BeamLineElement;
class E16ANA_GeometryV2;

class E16G4_HBDSD : public G4VSensitiveDetector{
   

   public:
      E16G4_HBDSD(const G4String& name, const E16ANA_GeometryV2* geom);
      virtual ~E16G4_HBDSD();

      virtual G4bool ProcessHits(G4Step* astep, G4TouchableHistory* ROhist);
      virtual void Initialize(G4HCofThisEvent* HCTE);
      virtual void EndOfEvent(G4HCofThisEvent* HCTE);
      virtual void DrawAll();
      virtual void PrintAll();
      void SetElement(int id, E16G4_BeamLineElement *elem);
      enum{nHBD=27};
   
   private:
      HBDHitsCollection* hitsCollection[nHBD];
      E16G4_BeamLineElement *elem_[nHBD];
      const E16ANA_GeometryV2 *geom_;
};


#endif//E16G4_HBD_SD_HH


