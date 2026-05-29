//2016-08-12, uploaded by nakai
//2015-01-05, uploaded by yokkaich
//2015-01-05, uploaded by yokkaich
//2014-08-27, uploaded by kawama
//2014-08-27, uploaded by kawama
//2014-04-30, uploaded by kawama
//2013-11-14, uploaded by kawama
//2013-05-13, modified by kawama
/*
  E16G4_GTRGEM.hh

   D.Kawama
*/

#ifndef E16G4_GTRGEM_h
#define E16G4_GTRGEM_h 1

#include "E16G4_BeamLineElement.hh"

#include <vector>

class G4Material;
class G4VSolid;
class G4LogicalVolume;
class G4VPhysicalVolume;

class E16G4_GTRGEM : public E16G4_BeamLineElement
{
public:
  E16G4_GTRGEM( const G4String &name, const G4ThreeVector &gPos,
	  const G4RotationMatrix *rotMtx, int id );

  ~E16G4_GTRGEM();

  bool ExistElectricField( void ) const { return false; }
  bool ExistMagneticField( void ) const { return false; }

  bool IsInside( const G4ThreeVector &gPos ) const;

  void SetMaterials(G4Material* mat)
  { mGas_=mat; mMylar_=mat; mAl_=mat;
     mKap_=mat; mCu_=mat; mNi_=mat; mEpox_=mat; mAcryl_=mat; mCFRP_=mat;}
  void SetMaterials( G4Material *mGas, G4Material *mMylar, 
        G4Material *mAl, G4Material *mKap, G4Material *mCu, 
        G4Material *mNi, G4Material *mEpox, G4Material *mAcryl, 
        G4Material *mCFRP)
  { mGas_=mGas; mMylar_=mMylar; mAl_=mAl;
     mKap_=mKap; mCu_=mCu; mNi_=mNi; mEpox_=mEpox; mAcryl_=mAcryl; 
     mCFRP_=mCFRP;}

  G4LogicalVolume *GetDetectorLV( void ) { return DetLV_; }
  G4LogicalVolume *GetframeDriftSpacerLV( void ) 
  { return frameDriftSpacerLV_; }
  G4LogicalVolume *GetframeGEMLV( void ) 
  { return frameGEMLV_; }
  G4LogicalVolume *GetframeReadOutLV( void ) 
  { return frameReadOutLV_; }
  G4LogicalVolume *GetframeAlBaseLV( void ) 
  { return frameAlBaseLV_; }
  G4LogicalVolume *GetframeCFRPLV( void ) 
  { return frameCFRPLV_; }

  //G4VPhysicalVolume *Place( G4VPhysicalVolume *pMother );
  G4VPhysicalVolume *PlaceTRD( G4LogicalVolume *pMother, G4int TorB );
  G4VPhysicalVolume *PlaceSQ( G4LogicalVolume *pMother );
  G4VPhysicalVolume *PlaceSQ_Epoxy( G4LogicalVolume *pMother );
  void PlaceFrame( G4LogicalVolume *pMother );

private:
  G4Material *mGas_, *mMylar_, *mAl_, *mKap_, *mCu_, *mNi_, *mEpox_, *mAcryl_, *mCFRP_;
  G4LogicalVolume *DetLV_;
  G4LogicalVolume *frameDriftSpacerLV_;
  G4LogicalVolume *frameGEMLV_;
  G4LogicalVolume *frameReadOutLV_;
  G4LogicalVolume *frameAlBaseLV_;
  G4LogicalVolume *frameCFRPLV_;
  G4LogicalVolume *gemKapLV_;
  G4VSolid *vSolid_;
  int Id_;
};

#endif//E16G4_GTRGEM_h

