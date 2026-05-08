//2016-08-12, uploaded by nakai
//2015-01-05, uploaded by yokkaich
//2015-01-05, uploaded by yokkaich
//2014-08-27, uploaded by kawama
//2014-08-27, uploaded by kawama
//2014-04-30, uploaded by kawama
//2013-11-14, uploaded by kawama
//2013-05-13, modified by kawama
/*
  E16G4_VD.hh

  D.Kawama
*/

#ifndef E16G4_VD_h
#define E16G4_VD_h 1

#include "E16G4_BeamLineElement.hh"

class G4Material;
class G4LogicalVolume;
class G4VPhysicalVolume;
class E16ANA_DetectorGeometry;

class E16G4_VD : public E16G4_BeamLineElement
{
   private:
      G4String elemname_;
      G4double Length_, Height_, Width_;
      int Id_;
      G4Material *mVol_;
      G4LogicalVolume *logDetector_;
      const E16ANA_DetectorGeometry *geom_;

   public:
      E16G4_VD( const G4String &name, const G4ThreeVector &gPos,
            const G4RotationMatrix *rotMtx,
            double SizeX, double SizeY, double SizeZ, int id );

      ~E16G4_VD();

      bool ExistMagneticField( void ) const { return false; }
      bool ExistElectricField( void ) const { return false; }

      bool IsInside( const G4ThreeVector &gPos ) const;

      void SetMaterials( G4Material *mVolume )
      { mVol_ = mVolume; }

      G4VPhysicalVolume *Place( G4LogicalVolume *pMother );

      G4LogicalVolume *GetDetectorLV( void ) { return logDetector_; }
      void SetGeometry( const E16ANA_DetectorGeometry *geom ) { geom_ = geom; };
      G4ThreeVector GetLocalPosition( const G4ThreeVector &gPos ) const;
      G4ThreeVector GetGlobalPosition( const G4ThreeVector &lPos ) const;
      G4ThreeVector GetLocalDirection( const G4ThreeVector &gDir ) const;
      G4ThreeVector GetGlobalDirection( const G4ThreeVector &lDir ) const;


};
#endif//E16G4_VD_h

