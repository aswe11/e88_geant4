//2016-08-12, uploaded by nakai
//2015-01-05, uploaded by yokkaich
//2015-01-05, uploaded by yokkaich
//2014-08-27, uploaded by kawama
//2014-08-27, uploaded by kawama
//2014-04-30, uploaded by kawama
//2013-11-14, uploaded by kawama
//2013-05-13, modified by kawama
/*
  E16G4_HBD.hh

   D.Kawama
*/

#ifndef E16G4_HBD_h
#define E16G4_HBD_h 1

#include "E16G4_BeamLineElement.hh"

#include <vector>

class G4Material;
class G4VSolid;
class G4LogicalVolume;
class G4VPhysicalVolume;


class E16G4_HBD : public E16G4_BeamLineElement
{
public:
  E16G4_HBD( const G4String &name, const G4ThreeVector &gPos,
	  const G4RotationMatrix *rotMtx, int id );

  ~E16G4_HBD();

  bool ExistElectricField( void ) const { return false; }
  bool ExistMagneticField( void ) const { return false; }

  bool IsInside( const G4ThreeVector &gPos ) const;

  void SetMaterials( G4Material *mGas, G4Material *mWind, 
        G4Material *mCath, G4Material *mFoil, G4Material *mBox, G4Material *mReadout)
  { mGas_=mGas; mWind_=mWind; mCath_=mCath; mFoil_=mFoil, mBox_=mBox; mReadout_=mReadout;}

  G4LogicalVolume *GetDetectorLV( void ) { return DetLV_; }
  G4LogicalVolume *GetG0LV( void ) { return g0LV_; }
  G4LogicalVolume *GetG1LV( void ) { return g1LV_; }
  G4LogicalVolume *GetG2LV( void ) { return g2LV_; }

  G4VPhysicalVolume *Place0( G4LogicalVolume *pMother );
  G4VPhysicalVolume *Place1( G4LogicalVolume *pMother );

private:
  G4Material *mGas_, *mWind_, *mCath_, *mFoil_, *mBox_, *mReadout_;
  G4LogicalVolume *DetLV_;
  G4LogicalVolume *g0LV_,*g1LV_,*g2LV_;
  G4VSolid *vSolid_;
  int Id_;
};

#endif//E16G4_HBD_h

