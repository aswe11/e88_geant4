//2016-08-12, uploaded by nakai
//2015-01-05, uploaded by yokkaich
//2015-01-05, uploaded by yokkaich
//2014-08-27, uploaded by kawama
//2014-08-27, uploaded by kawama
//2014-04-30, uploaded by kawama
//2013-11-14, uploaded by kawama
//2013-05-13, modified by kawama
/*
  E16G4_FMMagnet.hh

   D.Kawama
*/

#ifndef E16G4_FMMagnet_h
#define E16G4_FMMagnet_h 1

#include "E16G4_BeamLineElement.hh"

class E16ANA_MagneticFieldMap;

#include <vector>

class G4Material;
class G4VSolid;
class G4LogicalVolume;
class G4VPhysicalVolume;


class E16G4_FMMagnet : public E16G4_BeamLineElement
{
public:
  E16G4_FMMagnet( const G4String &name, const G4ThreeVector &gPos,
        const G4RotationMatrix *rotMtx, 
        const std::string filename); // magnetic field will be initialized by "filename"
  E16G4_FMMagnet( const G4String &name, const G4ThreeVector &gPos,
        const G4RotationMatrix *rotMtx, 
        G4double Bz); // constant magnetic field

  ~E16G4_FMMagnet();

  bool ExistElectricField( void ) const{ return false;}
  bool ExistMagneticField( void ) const{ return true;}

  bool IsInside( const G4ThreeVector &gPos ) const;

  void SetMaterials( G4Material *mMag, G4Material *mAir)
  { mMag_=mMag; mAir_=mAir;}

  G4LogicalVolume *GetDetectorLV( void ) { return DetLV_; }
  G4ThreeVector GetMagneticField( const G4ThreeVector &gPos ) const;

  //G4VPhysicalVolume *Place( G4VPhysicalVolume *pMother );
  void Place( G4LogicalVolume *pMother );

private:
  G4Material *mMag_, *mAir_;
  G4LogicalVolume *DetLV_;
  G4LogicalVolume *g0LV_,*g1LV_,*g2LV_;
  G4VSolid *vSolid_;
  int Id_;
  G4ThreeVector Field( const G4ThreeVector &pos ) const;
  E16ANA_MagneticFieldMap* fmf;
};

#endif//E16G4_FMMagnet_h

