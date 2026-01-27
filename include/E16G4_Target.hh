//2016-08-12, uploaded by nakai
//2015-01-05, uploaded by yokkaich
//2015-01-05, uploaded by yokkaich
//2014-08-27, uploaded by kawama
//2014-08-27, uploaded by kawama
//2014-04-30, uploaded by kawama
//2013-11-14, uploaded by kawama
//2013-05-13, modified by kawama
/*
  E16G4_Target.hh

   D.Kawama
*/

#ifndef E16G4_Target_h
#define E16G4_Target_h 1

#include "E16G4_BeamLineElement.hh"

#include <vector>

class G4Material;
class G4VSolid;
class G4LogicalVolume;
class G4VPhysicalVolume;


class E16G4_Target : public E16G4_BeamLineElement
{
public:
  E16G4_Target( const G4String &name, const G4ThreeVector &gPos,
	  const G4RotationMatrix *rotMtx, int id );
  ~E16G4_Target();

  bool ExistElectricField( void ) const { return false; }
  bool ExistMagneticField( void ) const { return false; }

  bool IsInside( const G4ThreeVector &gPos ) const;

  void SetMaterials(G4Material *mBox){ mBox_=mBox;}
  void SetThickness(G4double thick){ thick_=thick;}

  G4LogicalVolume *GetDetectorLV( void ) { return DetLV_; }

  //G4VPhysicalVolume *Place( G4VPhysicalVolume *pMother );
  G4VPhysicalVolume *Place( G4LogicalVolume *pMother );

private:
  G4Material *mBox_;
  G4LogicalVolume *DetLV_;
  G4VSolid *vSolid_;
  int Id_;
  G4double thick_;
};

#endif//E16G4_Target_h

