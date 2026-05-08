//2016-08-12, uploaded by nakai
//2015-01-05, uploaded by yokkaich
//2015-01-05, uploaded by yokkaich
//2014-08-27, uploaded by kawama
//2014-08-27, uploaded by kawama
//2014-04-30, uploaded by kawama
//2013-11-14, uploaded by kawama
//2013-05-13, modified by kawama
/*
  E16G4_BeamLineElement.hh

  D.Kawama
*/

#ifndef E16G4_BeamLineElement_h
#define E16G4_BeamLineElement_h 1

#include <G4ThreeVector.hh>
#include <G4RotationMatrix.hh>
#include <G4String.hh>

class G4AffineTransform;
class G4VPhysicalVolume;

class E16G4_BeamLineElement
{
public:
  E16G4_BeamLineElement( const G4String &name,
		   const G4ThreeVector &gPos,
		   const G4RotationMatrix *rotMtx );
  virtual ~E16G4_BeamLineElement();

  virtual bool ExistMagneticField( void ) const = 0;
  virtual bool ExistElectricField( void ) const = 0;
  virtual bool IsInside( const G4ThreeVector &globalPosition ) const = 0;

  virtual G4ThreeVector GetMagneticField( const G4ThreeVector &gPos ) const;
  virtual G4ThreeVector GetElectricField( const G4ThreeVector &gPos ) const;

  const G4String & GetName( void ) const
  { return elemName_; }

  //virtual G4VPhysicalVolume * Place( G4VPhysicalVolume *pMother ) = 0;
  //virtual G4VPhysicalVolume * Place( G4LogicalVolume *pMother ) = 0;

protected:
  G4String elemName_;
  G4ThreeVector gPos_;
  G4RotationMatrix rotMtx_;

  G4AffineTransform *pGtoL, *pLtoG;

public:
  virtual G4ThreeVector GetLocalPosition( const G4ThreeVector &gPos ) const;
  virtual G4ThreeVector GetGlobalPosition( const G4ThreeVector &lPos ) const;
  virtual G4ThreeVector GetLocalDirection( const G4ThreeVector &gDir ) const;
  virtual G4ThreeVector GetGlobalDirection( const G4ThreeVector &lDir ) const;
  const G4ThreeVector & GetCent() const { return gPos_; }
};

#endif//E16G4_BeamLineElement_h


