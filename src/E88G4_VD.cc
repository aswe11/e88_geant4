#include "E88G4_VD.hh"
#include "E88_GeometryV2.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

///////////////////////////////////////////////////////////////////////////
E88G4_VD::E88G4_VD( const G4String &name,
				  const G4ThreeVector &gPos,
				  const G4RotationMatrix *rotMtx,
				  double SizeX, double SizeY, double SizeZ,
				  int id )
  : E16G4_BeamLineElement(name,gPos,rotMtx),
    elemname_(name),Length_(0.5*SizeX), Height_(0.5*SizeY), Width_(0.5*SizeZ),
    Id_(id), mVol_(0), logDetector_(0), geom_(NULL)
//////////////////////////////////////////////////////////////////////////
{
}

////////////////////////////////////
E88G4_VD::~E88G4_VD()
////////////////////////////////////
{
}

//////////////////////////////////////////////////////////////////
bool E88G4_VD::IsInside( const G4ThreeVector &gPos ) const
//////////////////////////////////////////////////////////////////
{
  G4ThreeVector lPos = GetLocalPosition( gPos );

  return ( fabs(lPos.x()) <= Length_ &&
	   fabs(lPos.y()) <= Height_ &&
	   fabs(lPos.z()) <= Width_ );
}

/////////////////////////////////////////////////////////////////////
G4VPhysicalVolume *E88G4_VD::Place(G4LogicalVolume *pMother)
/////////////////////////////////////////////////////////////////////
{
  G4Box *VDsolid = new G4Box( elemName_, Length_, Height_, Width_ );

  G4LogicalVolume *VDlogical =
    new G4LogicalVolume( VDsolid, mVol_, elemName_, 0, 0, 0 );

  G4VPhysicalVolume *VirtualD =
    new G4PVPlacement(G4Transform3D(rotMtx_, gPos_), VDlogical, elemname_,
           pMother, false, Id_);

  VDlogical->SetVisAttributes(G4VisAttributes::GetInvisible());
  G4VisAttributes *VisAtt =
    new G4VisAttributes(G4Colour(0.,1.,1.));
  VDlogical->SetVisAttributes( VisAtt );
  logDetector_ = VDlogical;

  return VirtualD;
}

G4ThreeVector E88G4_VD::GetLocalPosition( const G4ThreeVector &gPos ) const {
   if(geom_){
      return geom_->GetLPos(gPos);
   }
   return E16G4_BeamLineElement::GetLocalPosition(gPos);
}

G4ThreeVector E88G4_VD::GetGlobalPosition( const G4ThreeVector &lPos ) const {
   if(geom_){
      return geom_->GetGPos(lPos);
   }
   return E16G4_BeamLineElement::GetGlobalPosition(lPos);
}

G4ThreeVector E88G4_VD::GetLocalDirection( const G4ThreeVector &gDir ) const {
   if(geom_){
      return geom_->GetLMom(gDir);
   }
   return E16G4_BeamLineElement::GetLocalDirection(gDir);
}

G4ThreeVector E88G4_VD::GetGlobalDirection( const G4ThreeVector &lDir ) const {
   if(geom_){
      return geom_->GetGMom(lDir);
   }
   return E16G4_BeamLineElement::GetGlobalDirection(lDir);
}
