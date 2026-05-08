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
   E16G4_Target.cc

   D.Kawama
   */



#include <G4Material.hh>
#include <G4Box.hh>
#include <G4SubtractionSolid.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>

#include <G4VisAttributes.hh>
#include <G4Colour.hh>


#include "E16G4_Target.hh"

///////////////////////////////////////////////
 E16G4_Target::E16G4_Target( const G4String &name,
      const G4ThreeVector &gPos,
      const G4RotationMatrix *rotMtx,
      int id )
: E16G4_BeamLineElement(name,gPos,rotMtx),
   mBox_(0), DetLV_(0), vSolid_(0), Id_(id), thick_(0)
///////////////////////////////////////////////
{
}

/////////////////
 E16G4_Target::~E16G4_Target()
/////////////////
{
}

////////////////////////////////////////////////////////
bool E16G4_Target::IsInside( const G4ThreeVector &gPos ) const
////////////////////////////////////////////////////////
{
   G4ThreeVector lPos = GetLocalPosition(gPos);
   if( vSolid_-> Inside(lPos) != kOutside ) return true;
   else return false;
}

//////////////////////////////////////////////////////////////
G4VPhysicalVolume *E16G4_Target::Place( G4LogicalVolume *pMother )
//////////////////////////////////////////////////////////////
{ 
   std::ostringstream id;
   id<<Id_;
   G4String nmbox=G4String("Target")+id.str().c_str();;
   //G4Box* boxSolid= new G4Box(nmbox, 2.5/2.*cm, 2.5/2.*cm, thick_/2.);
   G4Box* boxSolid= new G4Box(nmbox, 1/2.*cm, 1/2.*cm, thick_/2.);
   DetLV_= new G4LogicalVolume
        (boxSolid, mBox_, nmbox);
   G4VisAttributes *va= new G4VisAttributes(G4Color(1.,1.,1.));
   DetLV_-> SetVisAttributes(va);

   G4VPhysicalVolume *tracker = new G4PVPlacement
         (G4Transform3D(rotMtx_, gPos_), DetLV_, nmbox,
           pMother, false, Id_);
   return tracker;

}
