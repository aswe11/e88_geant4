//2016-08-12, uploaded by nakai
//2016-03-18, uploaded by nakai

#ifndef E16G4_SSDSD_hh
#define E16G4_SSDSD_hh

#include <G4VSensitiveDetector.hh>

#include "E16G4_SSDHit.hh"
#include "E16ANA_GeometryV2.hh"

class G4HCofThisEvent;
class G4Step;
class G4TouchableHistory;
class E16G4_BeamLineElement;

class E16G4_SSDSD : public G4VSensitiveDetector {
public:
   E16G4_SSDSD(const G4String &name, const E16ANA_GeometryV2 *geom);
   ~E16G4_SSDSD();
   G4bool ProcessHits(G4Step *astep, G4TouchableHistory *ROhist);
   void Initialize(G4HCofThisEvent *HCTE);
   void EndOfEvent(G4HCofThisEvent *HCTE);
   void DrawAll();
   void PrintAll();
   void SetElement(int id, E16G4_BeamLineElement *elem);

private:
   enum{
     //      n_ssd = 27
      n_ssd = 33
   };
   SSDHitsCollection *hitsCollection[n_ssd];
   E16G4_BeamLineElement *elem_[n_ssd];
   const E16ANA_GeometryV2 *geom_;

};

#endif // E16G4_SSDSD_hh

