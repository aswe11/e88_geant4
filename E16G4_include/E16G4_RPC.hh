//2016-12-18, uploaded by yokkaich
//2016-08-12, uploaded by nakai
//2016-03-18, uploaded by nakai

#ifndef E16G4_RPC_hh
#define E16G4_RPC_hh

#include "E16G4_BeamLineElement.hh"

class G4Material;
class G4LogicalVolume;
class G4VSolid;
class G4VPhysicalVolume;

class E16G4_RPC : public E16G4_BeamLineElement {
public:
   E16G4_RPC(const G4String &name, const G4ThreeVector &gPos, const G4RotationMatrix *rotMtx, int id);
   ~E16G4_RPC();
   bool ExistMagneticField() const {return false;};
   bool ExistElectricField() const {return false;};
   bool IsInside(const G4ThreeVector &gPos) const;
  void SetMaterials(G4Material *mat1, G4Material *mat2, G4Material *mat3, G4Material *mat4){
      mGlass_ = mat1;
      mPCB_ = mat2;
      mAir_ = mat3;
      mCu_ = mat4;
   };

   G4LogicalVolume* GetDetectorLV(){return detLV_;};
   G4VPhysicalVolume* Place(G4LogicalVolume *pMother);

private:
   G4Material *mGlass_;
   G4Material *mPCB_;
   G4Material *mAir_;
   G4Material *mCu_;
   G4LogicalVolume *detLV_;
   G4LogicalVolume *pcbLV_;
   G4VSolid *vSolid_;
   int id_;

  //   G4VPhysicalVolume* PlaceSensor(G4LogicalVolume *pMother);
   G4VPhysicalVolume* PlaceRPC(G4LogicalVolume *pMother);

};

#endif // E16G4_RPC_hh

