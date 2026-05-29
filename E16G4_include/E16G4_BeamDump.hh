//2016-08-12, uploaded by nakai
//2015-01-05, uploaded by yokkaich
//2015-01-05, uploaded by yokkaich
//2014-08-27, uploaded by kawama
//2014-08-27, uploaded by kawama
//2014-04-30, uploaded by kawama
//2013-11-14, uploaded by kawama
//2013-05-13, modified by kawama
/*
//E16G4_Bdump.hh 150105 by S. Yokkaichi
//    Last modified at <2015-01-05 13:36:27 >
//

  E16G4_Bdump.hh

   D.Kawama
*/

#ifndef E16G4_BeamDump_HH
#define E16G4_BeamDump_HH 1

#include "E16G4_BeamLineElement.hh"

#include <vector>

class G4Material;
class G4VSolid;
class G4LogicalVolume;
class G4VPhysicalVolume;


class E16G4_BeamDump : public E16G4_BeamLineElement
{
public:
  E16G4_BeamDump( const G4String &name, const G4ThreeVector &gPos,
	  const G4RotationMatrix *rotMtx, int id );
  ~E16G4_BeamDump();

  bool ExistElectricField( void ) const { return false; }
  bool ExistMagneticField( void ) const { return false; }

  bool IsInside( const G4ThreeVector &gPos ) const;

  void SetMaterials(G4Material *mIron, G4Material *mConc)
  { mIron_=mIron;mConc_=mConc;}
  void SetThickness(G4double thick){ thick_=thick;}

  G4LogicalVolume *GetDetectorLV( void ) { return DetLV_; }

  G4VPhysicalVolume *PlaceE16( G4LogicalVolume *pMother );
  G4VPhysicalVolume *PlaceE325( G4LogicalVolume *pMother );

private:
  G4Material *mIron_, *mConc_;
  G4LogicalVolume *DetLV_;
  G4VSolid *vSolid_;
  int Id_;
  G4double thick_;
};

#endif//E16G4_BeamDump_HH

