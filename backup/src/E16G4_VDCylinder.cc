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
  E16G4_VDCylinder.cc

  D.Kawama
*/



#include <G4Material.hh>
#include <G4Box.hh>
#include <G4Tubs.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>

#include <G4VisAttributes.hh>
#include <G4Colour.hh>

#include "E16G4_VDCylinder.hh"

///////////////////////////////////////////////////////////////////////////
E16G4_VDCylinder::E16G4_VDCylinder( const G4String &name,
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
E16G4_VDCylinder::~E16G4_VDCylinder()
   ////////////////////////////////////
{
}

//////////////////////////////////////////////////////////////////
bool E16G4_VDCylinder::IsInside( const G4ThreeVector &gPos ) const
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
G4VPhysicalVolume *E16G4_VDCylinder::Place(G4LogicalVolume *pMother)
   /////////////////////////////////////////////////////////////////////
{
   G4Tubs *VDsolid = new G4Tubs( elemName_, Radius_, Radius_+Thickness_, Height_, SPhi_, DPhi_ );
   //G4cout <<SPhi_/deg<<" "<<DPhi_/deg<<G4endl;
      
   G4LogicalVolume *VDlogical =
      new G4LogicalVolume( VDsolid, mVol_, elemName_, 0, 0, 0 );


   G4RotationMatrix rot
      (-rotMtx_.phi(),rotMtx_.theta(),-rotMtx_.psi());
   rot.rotateX(90*deg);
   rot.rotateY(90*deg);

   G4VPhysicalVolume *VirtualD =
      new G4PVPlacement(G4Transform3D(rot, gPos_), VDlogical, elemname_,
            pMother, false, Id_);

   VDlogical-> SetVisAttributes(G4VisAttributes::Invisible);
   G4VisAttributes *VisAtt =
      //new G4VisAttributes( G4Colour::G4Colour(0.,1.,1.));
      new G4VisAttributes(false);
   VDlogical->SetVisAttributes( VisAtt );
   logDetector_ = VDlogical;

   //delete VisAtt;
   return VirtualD;
}
