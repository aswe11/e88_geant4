//2016-08-12, uploaded by nakai
//2015-01-05, modified by yokkaich
//2015-01-05, modified by yokkaich
//2014-08-27, modified by kawama
//2014-08-27, modified by kawama
//2014-04-30, modified by kawama
//2013-11-14, modified by kawama
//2013-05-13, modified by kawama
/*
//E16G4_BeamPipe.cc 150105 by S. Yokkaichi
//    Last modified at <2015-01-05 13:45:54 >
//
   E16G4_Bpipe.cc

   D.Kawama
   */


#include <G4Material.hh>
#include <G4Box.hh>
#include <G4Trd.hh>
#include <G4Tubs.hh>
#include <G4SubtractionSolid.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>

#include <G4VisAttributes.hh>
#include <G4Colour.hh>


#include "E16G4_BeamPipe.hh"

///////////////////////////////////////////////
 E16G4_BeamPipe::E16G4_BeamPipe( const G4String &name,
      const G4ThreeVector &gPos,
      const G4RotationMatrix *rotMtx,
      int id )
: E16G4_BeamLineElement(name,gPos,rotMtx),
   mBagGas_(0), mPipeGas_(0), mMyl_(0), DetLV_(0), vSolid_(0), Id_(id)
///////////////////////////////////////////////
{
}

/////////////////
 E16G4_BeamPipe::~E16G4_BeamPipe()
/////////////////
{
}

////////////////////////////////////////////////////////
bool E16G4_BeamPipe::IsInside( const G4ThreeVector &gPos ) const
////////////////////////////////////////////////////////
{
   G4ThreeVector lPos = GetLocalPosition(gPos);
   if( vSolid_-> Inside(lPos) != kOutside ) return true;
   else return false;
}

//////////////////////////////////////////////////////////////
G4VPhysicalVolume *E16G4_BeamPipe::Place( G4LogicalVolume *pMother )
//////////////////////////////////////////////////////////////
{ 
   G4VPhysicalVolume *BeamPipePV; 
   G4RotationMatrix *zeRot= new G4RotationMatrix();
   zeRot->rotateY(0.*degree); 
   std::ostringstream id;
   id<<Id_;
   G4String nm;
   G4VisAttributes *va= new G4VisAttributes(G4Color::Grey);
   nm=G4String("Gas_HeBag")+id.str().c_str();;
   //---He Bag---// 
   G4Trd* HeSolid0= new G4Trd(nm, 
         5.6/2.*cm, (50.*cm)/2., 
         5.6/2.*cm, (50.*cm)/2., 
         160/2.*cm);
   G4DisplacedSolid *HeSolid1 = new G4DisplacedSolid(
         nm, HeSolid0, 0,
         G4ThreeVector(0.*cm,0.*cm,(20.+160/2.)*cm));
   G4LogicalVolume *HeLV= new G4LogicalVolume(HeSolid1, mBagGas_, 
         "HeLV_HeBag");
   HeLV-> SetVisAttributes(va);

   //---Beam Pipe---// 
   nm=G4String("BeamPipe_Gas")+id.str().c_str();;
   G4Tubs* tubSolid0= new G4Tubs(nm, 0.*cm, 25.*cm,280*cm,
         0.,2*3.141592);
   G4DisplacedSolid *tubSolid1 = new G4DisplacedSolid(
         nm, tubSolid0, 0,
         G4ThreeVector(0.*cm,0.*cm,460.*cm));
   nm=G4String("LV_BeamPipe_Gas")+id.str().c_str();;
   G4LogicalVolume *tubLV= 
      new G4LogicalVolume(tubSolid1, mPipeGas_, nm);
   tubLV-> SetVisAttributes(va);
   //new G4PVPlacement(G4Transform3D(*zeRot, orig), tubLV, "tube",
   //   worldLV, false, 2);

   //---Beam Pipe Mylar---// 
   nm=G4String("BeamPipe_Mylar")+id.str().c_str();;
   G4Tubs* tubmylSolid0= new G4Tubs(nm, 
         0.*cm, 25.*cm,0.01*cm,0.,2*3.141592);
   G4DisplacedSolid *tubmylSolid1 = new G4DisplacedSolid(
         nm, tubmylSolid0, 0,
         G4ThreeVector(0.*cm,0.*cm,(460.-280.)*cm));
   G4DisplacedSolid *tubmylSolid2 = new G4DisplacedSolid(
         nm, tubmylSolid0, 0,
         G4ThreeVector(0.*cm,0.*cm,(460.+280.)*cm));
   nm=G4String("LV_BeamPipe_Mylar")+id.str().c_str();;
   G4LogicalVolume *tubmylLV1= new G4LogicalVolume(tubmylSolid1, 
         mMyl_, nm);
   G4LogicalVolume *tubmylLV2= new G4LogicalVolume(tubmylSolid2, 
         mMyl_, nm);
   tubmylLV1-> SetVisAttributes(va);
   tubmylLV2-> SetVisAttributes(va);
   //new G4PVPlacement(G4Transform3D(*zeRot, orig), tubmylLV1, "tube_myler1",
   //   worldLV, false, 3);
   //new G4PVPlacement(G4Transform3D(*zeRot, orig), tubmylLV2, "tube_myler2",
   //   worldLV, false, 4);

   nm=G4String("PV_BeamPipe")+id.str().c_str();;
   BeamPipePV = new G4PVPlacement(G4Transform3D(rotMtx_, gPos_), 
         HeLV, nm,pMother, false, Id_);
   BeamPipePV = new G4PVPlacement(G4Transform3D(rotMtx_, gPos_), 
         tubLV, nm, pMother, false, Id_);
   BeamPipePV = new G4PVPlacement(G4Transform3D(rotMtx_, gPos_), 
         tubmylLV1, nm,pMother, false, Id_);
   BeamPipePV = new G4PVPlacement(G4Transform3D(rotMtx_, gPos_), 
         tubmylLV2, nm,pMother, false, Id_);
   return BeamPipePV;

}
