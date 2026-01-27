//2016-12-18, uploaded by yokkaich
//2016-08-12, uploaded by nakai
//2016-03-18, uploaded by nakai

#ifndef E16G4_SSD_hh
#define E16G4_SSD_hh

#include "E16G4_BeamLineElement.hh"

class G4Material;
class G4LogicalVolume;
class G4VSolid;
class G4VPhysicalVolume;

class E16G4_SSD : public E16G4_BeamLineElement {
public:
   E16G4_SSD(const G4String &name, const G4ThreeVector &gPos, const G4RotationMatrix *rotMtx, int id);
   ~E16G4_SSD();
   bool ExistMagneticField() const {return false;};
   bool ExistElectricField() const {return false;};
   bool IsInside(const G4ThreeVector &gPos) const;
  void SetMaterials(G4Material *mat1,G4Material *mat2 ){
      mSi_ = mat1;
      mPCB_ = mat2;
   };

   G4LogicalVolume* GetDetectorLV(){return detLV_;};
   G4VPhysicalVolume* Place(G4LogicalVolume *pMother);

private:
   G4Material *mSi_;
   G4Material *mPCB_;
   G4LogicalVolume *detLV_;
   G4LogicalVolume *pcbLV_;
   G4VSolid *vSolid_;
   int id_;

   G4VPhysicalVolume* PlaceSiSensor(G4LogicalVolume *pMother);
   G4VPhysicalVolume* PlacePCB(G4LogicalVolume *pMother);

};

#endif // E16G4_SSD_hh

