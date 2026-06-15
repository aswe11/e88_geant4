#ifndef E88G4_VDCylinder_h
#define E88G4_VDCylinder_h 1

#include "E16G4_BeamLineElement.hh"

class G4Material;
class G4LogicalVolume;
class G4VPhysicalVolume;

class E88G4_VDCylinder : public E16G4_BeamLineElement
{
   private:
      G4String elemname_;
      G4double Radius_, Thickness_, Height_, SPhi_, DPhi_;
      int Id_;
      G4Material *mVol_;
      G4LogicalVolume *logDetector_;
   public:
   E88G4_VDCylinder( const G4String &name,
      const G4ThreeVector &gPos,
      const G4RotationMatrix *rotMtx,
      double Radius, double Thickness, double Height, 
      double SPhi, double DPhi,int id );

      ~E88G4_VDCylinder();

      bool ExistMagneticField( void ) const { return false; }
      bool ExistElectricField( void ) const { return false; }

      bool IsInside( const G4ThreeVector &gPos ) const;

      void SetMaterials( G4Material *mVolume )
      { mVol_ = mVolume; }

      G4VPhysicalVolume *Place( G4LogicalVolume *pMother );

      G4LogicalVolume *GetDetectorLV( void ) { return logDetector_; }
};
#endif//E88G4_VDCylinder_h
