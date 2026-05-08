//2016-08-12, uploaded by nakai
//2015-01-05, uploaded by yokkaich
//2015-01-05, uploaded by yokkaich
//2014-08-27, uploaded by kawama
//2014-08-27, uploaded by kawama
//2014-04-30, uploaded by kawama
//2013-11-14, uploaded by kawama
//2013-05-13, modified by kawama
/*
  E16G4_GTR.hh

   D.Kawama
*/

#ifndef E16G4_GTR_h
#define E16G4_GTR_h 1

#include "E16G4_BeamLineElement.hh"
#include "E16G4_GTRGEM.hh"
#include "E16ANA_Geometry.hh"

#include <vector>

class G4Material;
class G4VSolid;
class G4LogicalVolume;
class G4VPhysicalVolume;


class E16G4_GTR : public E16G4_BeamLineElement
{
public:
  E16G4_GTR( const G4String &name, const G4ThreeVector &gPos,
	  const G4RotationMatrix *rotMtx, const E16ANA_Geometry *geom, int id );

  ~E16G4_GTR();

  bool ExistElectricField( void ) const { return false; }
  bool ExistMagneticField( void ) const { return false; }

  bool IsInside( const G4ThreeVector &gPos ) const;

  void SetMaterials( G4Material *mGas, G4Material *mMylar, 
        G4Material *mAl, G4Material *mKap, G4Material *mCu, 
        G4Material *mNi, G4Material *mEpox, G4Material *mAcryl, 
        G4Material *mFrame, G4Material *mAir)
  { mGas_=mGas; mMylar_=mMylar; mAl_=mAl;
     mKap_=mKap; mCu_=mCu; mNi_=mNi; mEpox_=mEpox; 
     mFrame_=mFrame; mAcryl_=mAcryl; mAir_=mAir;}

  G4LogicalVolume *GetDetectorLV( void ) { return DetLV_; }
  G4LogicalVolume *GetG0LV( void ) { return g0LV_; }
  G4LogicalVolume *GetG1LV( void ) { return g1LV_; }
  G4LogicalVolume *GetG2LV( void ) { return g2LV_; }
  E16G4_GTRGEM *GetGEM0( void ) { return g0_; }
  E16G4_GTRGEM *GetGEM1( void ) { return g1_; }
  E16G4_GTRGEM *GetGEM2( void ) { return g2_; }

  //G4VPhysicalVolume *Place( G4VPhysicalVolume *pMother );
  G4VPhysicalVolume *Place( G4LogicalVolume *pMother );

private:
  G4Material *mGas_, *mMylar_, *mAl_, *mKap_, *mCu_, *mNi_;
  G4Material *mEpox_, *mAcryl_, *mFrame_, *mAir_;
  G4LogicalVolume *DetLV_;
  G4LogicalVolume *g0LV_,*g1LV_,*g2LV_;
  E16G4_GTRGEM *g0_,*g1_,*g2_;
  const E16ANA_Geometry *geom_;
  G4VSolid *vSolid_;
  int Id_;
};

#endif//E16G4_GTR_h

