//2016-08-12, uploaded by nakai
//2015-01-05, uploaded by yokkaich
//2015-01-05, uploaded by yokkaich
//2014-08-27, uploaded by kawama
//2014-08-27, uploaded by kawama
//2014-04-30, uploaded by kawama
//2013-11-14, uploaded by kawama
//2013-05-13, modified by kawama
/*
  E16G4_LG.hh

   D.Kawama
*/

#ifndef E16G4_LG_h
#define E16G4_LG_h 1

#include "E16G4_BeamLineElement.hh"

#include <vector>

class G4Material;
class G4VSolid;
class G4LogicalVolume;
class G4VPhysicalVolume;


class E16G4_LG : public E16G4_BeamLineElement
{
public:
  E16G4_LG( const G4String &name, const G4ThreeVector &gPos,
	  const G4RotationMatrix *rotMtx, int id );

  ~E16G4_LG();

  bool ExistElectricField( void ) const { return false; }
  bool ExistMagneticField( void ) const { return false; }

  bool IsInside( const G4ThreeVector &gPos ) const;

  void SetMaterials( G4Material *mRad)
  { mRad_=mRad;}

  G4LogicalVolume *GetDetectorLV( void ) { return DetLV_; }

  //G4VPhysicalVolume *Place( G4VPhysicalVolume *pMother );
  G4VPhysicalVolume *Place( G4LogicalVolume *pMother );

private:
  G4Material *mRad_;
  G4LogicalVolume *DetLV_;
  G4VSolid *vSolid_;
  int Id_;
};

#endif//E16G4_LG_h

