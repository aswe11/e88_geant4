//2016-08-12, uploaded by nakai
//2015-01-20, uploaded by yokkaich
//2015-01-05, modified by yokkaich
//2015-01-05, modified by yokkaich
//2014-08-27, modified by kawama
//2014-08-27, modified by kawama
//2014-04-30, modified by kawama
//2013-11-14, modified by kawama
//2013-05-13, modified by kawama
/*
  E16G4_BeamLineElement.cc

  D.Kawama
*/
#include <G4AffineTransform.hh>

#include "E16G4_BeamLineElement.hh"


///////////////////////////////////////////////////////////////////
 E16G4_BeamLineElement::E16G4_BeamLineElement( const G4String &name,
				  const G4ThreeVector &pos,
				  const G4RotationMatrix *rotMtx )
  : elemName_(name), gPos_(pos),
    rotMtx_( rotMtx ? (*rotMtx) : G4RotationMatrix() ),
    //rotMtx_( (*rotMtx) ),
    pGtoL( new G4AffineTransform( rotMtx_, gPos_ ) ),
    pLtoG( new G4AffineTransform( rotMtx_, gPos_ ) )
//////////////////////////////////////////////////////////////////
{ 
   /*G4cout <<"elname:"<<elemName_<<G4endl;
   G4cout <<"gPos:"<<gPos_<<G4endl;
   G4cout <<"rotMtx_:"<<rotMtx_<<G4endl;*/
  pGtoL-> Invert();
}

///////////////////////////////////
 E16G4_BeamLineElement::~E16G4_BeamLineElement()
///////////////////////////////////
{
  delete pGtoL;
  delete pLtoG;
}

//////////////////////////////////////////////
G4ThreeVector E16G4_BeamLineElement::
GetMagneticField( const G4ThreeVector &) const
//////////////////////////////////////////////
{
  return G4ThreeVector(0.,0.,0.);
}

///////////////////////////////////////////////
G4ThreeVector E16G4_BeamLineElement::
GetElectricField( const G4ThreeVector &) const
///////////////////////////////////////////////
{
  return G4ThreeVector(0.,0.,0.);
}

/////////////////////////////////////////////////////
G4ThreeVector E16G4_BeamLineElement::
GetGlobalPosition( const G4ThreeVector &lPos ) const
/////////////////////////////////////////////////////
{
  return pLtoG-> TransformPoint(lPos);
}

////////////////////////////////////////////////////
G4ThreeVector E16G4_BeamLineElement::
GetLocalPosition( const G4ThreeVector &gPos ) const
///////////////////////////////////////////////////
{
  return pGtoL-> TransformPoint(gPos);
}

//////////////////////////////////////////////////////
G4ThreeVector E16G4_BeamLineElement::
GetGlobalDirection( const G4ThreeVector &lDir ) const
//////////////////////////////////////////////////////
{
  return pLtoG-> TransformAxis(lDir);
}

/////////////////////////////////////////////////////
G4ThreeVector E16G4_BeamLineElement::
GetLocalDirection( const G4ThreeVector &gDir ) const
/////////////////////////////////////////////////////
{
  return pGtoL-> TransformAxis(gDir);
}
