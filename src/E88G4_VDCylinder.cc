#include <G4Material.hh>
#include <G4Box.hh>
#include <G4Tubs.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>

#include <G4VisAttributes.hh>
#include <G4Colour.hh>

#include "E88G4_VDCylinder.hh"
#include <G4SystemOfUnits.hh>

///////////////////////////////////////////////////////////////////////////
E88G4_VDCylinder::E88G4_VDCylinder( const G4String &name,
      const G4ThreeVector &gPos,
      const G4RotationMatrix *rotMtx,
      double Radius, double Thickness, double Height, 
      double SPhi, double DPhi,int id )
: E16G4_BeamLineElement(name,gPos,rotMtx),
   elemname_(name),Radius_(Radius), Thickness_(Thickness), Height_(Height),
   SPhi_(SPhi), DPhi_(DPhi),
   Id_(id), mVol_(0), logDetector_(0)
//////////////////////////////////////////////////////////////////////////
{
}

////////////////////////////////////
E88G4_VDCylinder::~E88G4_VDCylinder()
////////////////////////////////////
{
}

//////////////////////////////////////////////////////////////////
bool E88G4_VDCylinder::IsInside( const G4ThreeVector &gPos ) const
//////////////////////////////////////////////////////////////////
{
   G4ThreeVector lPos = GetLocalPosition( gPos );
   G4double r = sqrt(lPos.x()*lPos.x()+lPos.z()*lPos.z());
   G4double phi = atan2(lPos.x(),lPos.z());

   return ( r >= Radius_ && r<=Radius_+Thickness_ &&
         fabs(lPos.z()) <= Height_ &&
         phi>=SPhi_ && phi<DPhi_ );
}

/////////////////////////////////////////////////////////////////////
G4VPhysicalVolume *E88G4_VDCylinder::Place(G4LogicalVolume *pMother)
/////////////////////////////////////////////////////////////////////
{
   G4Tubs *VDsolid = new G4Tubs( elemName_, Radius_, Radius_+Thickness_, Height_, SPhi_, DPhi_ );
      
   G4LogicalVolume *VDlogical =
      new G4LogicalVolume( VDsolid, mVol_, elemName_, 0, 0, 0 );

   G4RotationMatrix rot
      (-rotMtx_.phi(),rotMtx_.theta(),-rotMtx_.psi());
   rot.rotateX(90*deg);
   rot.rotateY(90*deg);

   G4VPhysicalVolume *VirtualD =
      new G4PVPlacement(G4Transform3D(rot, gPos_), VDlogical, elemname_,
            pMother, false, Id_);

   VDlogical->SetVisAttributes(G4VisAttributes::GetInvisible());
   G4VisAttributes *VisAtt =
      new G4VisAttributes(false);
   VDlogical->SetVisAttributes( VisAtt );
   logDetector_ = VDlogical;

   return VirtualD;
}
