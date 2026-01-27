//2016-08-12, uploaded by nakai
//2015-01-05, modified by yokkaich
//2015-01-05, modified by yokkaich
//2014-08-27, modified by kawama
//2014-08-27, modified by kawama
//2014-04-30, modified by kawama
//2013-11-14, modified by kawama
//2013-05-13, modified by kawama

/*
   E16G4_BeamDump.cc

   D.Kawama
   */


#include <G4Material.hh>
#include <G4Box.hh>
#include <G4SubtractionSolid.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>

#include <G4VisAttributes.hh>
#include <G4Colour.hh>

#include "E16G4_BeamDump.hh"

///////////////////////////////////////////////
 E16G4_BeamDump::E16G4_BeamDump( const G4String &name,
      const G4ThreeVector &gPos,
      const G4RotationMatrix *rotMtx,
      int id )
: E16G4_BeamLineElement(name,gPos,rotMtx),
   mIron_(0), mConc_(0), DetLV_(0), vSolid_(0), Id_(id), thick_(0)
///////////////////////////////////////////////
{
}

/////////////////
 E16G4_BeamDump::~E16G4_BeamDump()
/////////////////
{
}

////////////////////////////////////////////////////////
bool E16G4_BeamDump::IsInside( const G4ThreeVector &gPos ) const
////////////////////////////////////////////////////////
{
   G4ThreeVector lPos = GetLocalPosition(gPos);
   if( vSolid_-> Inside(lPos) != kOutside ) return true;
   else return false;
}

//////////////////////////////////////////////////////////////
G4VPhysicalVolume *E16G4_BeamDump::PlaceE16( G4LogicalVolume *pMother )
//////////////////////////////////////////////////////////////
{ 
   G4RotationMatrix *zeRot= new G4RotationMatrix();
   zeRot->rotateY(0.*degree); 
   std::ostringstream id;
   id<<Id_;
   G4String nm;
   G4VisAttributes *va;
   G4double ironCent=150*cm;
   G4double holeSize=80*cm;
   G4double doorThickness=15.;
   nm=G4String("IronSolid_BeamDump")+id.str().c_str();;
   G4Box* ironSolid0= new G4Box(nm, 300/2.*cm, 300/2.*cm, 300/2.*cm);
   G4DisplacedSolid *ironSolid1 = new G4DisplacedSolid(
                      nm, ironSolid0, 0,
		  G4ThreeVector(0.*cm,0.*cm,ironCent));
   nm=G4String("HoleSolid_BeamDump")+id.str().c_str();;
   G4Box* holeSolid0= new G4Box(nm, holeSize/2., holeSize/2., 300*cm/2.);
   G4DisplacedSolid *holeSolid1 = new G4DisplacedSolid(
                      nm, holeSolid0, 0,
		  G4ThreeVector(0.*cm,0.*cm,-(300/2.*cm)));
   nm=G4String("ConcSolid_BeamDump")+id.str().c_str();;
   G4Box* concSolid0= new G4Box(nm, 600/2.*cm, 600/2.*cm, 600/2.*cm);
   G4SubtractionSolid* concSolid1=
      new G4SubtractionSolid(nm,concSolid0,ironSolid1);
   G4SubtractionSolid* concSolid2=
      new G4SubtractionSolid(nm,concSolid1,holeSolid1);
   
   nm=G4String("IronLV_BeamDump")+id.str().c_str();;
   G4LogicalVolume *ironLV= new G4LogicalVolume(ironSolid1, mIron_, nm);
   va= new G4VisAttributes(G4Color::Red());
   ironLV-> SetVisAttributes(va);

   //DetLV_= new G4LogicalVolume(concSolid0, mBox_, "LV_BeamDump");
   /*nm=G4String("ConcPV_BeamDump")+id.str().c_str();;
   new G4PVPlacement(zeRot, G4ThreeVector(0.*cm,0.*cm,0.*cm), concLV, nm,
           DetLV_, false, 0);*/
   /*nm=G4String("IronPV_BeamDump")+id.str().c_str();;
   new G4PVPlacement(zeRot, G4ThreeVector(0.*cm,0.*cm,0.*cm), ironLV, nm,
           concLV, false, 1);*/
   nm=G4String("doorSolid_BeamDump")+id.str().c_str();;
   G4Box* doorSolid0= new G4Box(nm, holeSize/2., holeSize/2., 
         doorThickness/2.*cm);
   G4Box* tmpSolid= new G4Box(nm, 30./2.*cm, 30./2.*cm, 
         doorThickness/2.*cm);
   G4SubtractionSolid* doorSolid1=
      new G4SubtractionSolid(nm,doorSolid0,tmpSolid);
   G4DisplacedSolid *doorSolid2 = new G4DisplacedSolid(
                      nm, doorSolid1, 0,
		  G4ThreeVector(0.*cm,0.*cm,
                     -(200.+100-doorThickness/2.)*cm));
   nm=G4String("doorLV_BeamDump")+id.str().c_str();;
   G4LogicalVolume *doorLV= new G4LogicalVolume(doorSolid2, mIron_, nm);
   va= new G4VisAttributes(G4Color::Red());
   doorLV-> SetVisAttributes(va);
  
   G4SubtractionSolid* concSolid3=
      new G4SubtractionSolid(nm,concSolid2,doorSolid2);
   
   nm=G4String("ConcLV_BeamDump")+id.str().c_str();;
   G4LogicalVolume *concLV= new G4LogicalVolume
        (concSolid3, mConc_, nm);
   va= new G4VisAttributes(G4Color::Gray());
   concLV-> SetVisAttributes(va);
  
   //Shield 
   G4double shieldThickness=50.;
   G4double shieldHoleSize=30.;
   G4double shieldSize=100.*cm;
   G4double shieldPos=700.;
   nm=G4String("shieldSolid_BeamDump")+id.str().c_str();;
   G4Box* shieldSolid0= new G4Box(nm, shieldSize/2., shieldSize/2., 
         shieldThickness/2.*cm);
   tmpSolid= new G4Box(nm, shieldHoleSize/2.*cm, shieldHoleSize/2.*cm, 
         shieldThickness/2.*cm);
   G4SubtractionSolid* shieldSolid1=
      new G4SubtractionSolid(nm,shieldSolid0,tmpSolid);
   G4DisplacedSolid *shieldSolid2 = new G4DisplacedSolid(
                      nm, shieldSolid1, 0,
		  G4ThreeVector(0.*cm,0.*cm,-(shieldPos+25)*cm));
   G4DisplacedSolid *shieldSolid3 = new G4DisplacedSolid(
                      nm, shieldSolid1, 0,
		  G4ThreeVector(0.*cm,0.*cm,-(shieldPos-25)*cm));
   nm=G4String("shieldLV_BeamDump")+id.str().c_str();;
   G4LogicalVolume *shieldLV0= new G4LogicalVolume(shieldSolid2, mIron_, nm);
   G4LogicalVolume *shieldLV1= new G4LogicalVolume(shieldSolid3, mIron_, nm);
   va= new G4VisAttributes(G4Color::Red());
   shieldLV0-> SetVisAttributes(va);
   va= new G4VisAttributes(G4Color::Red());
   shieldLV1-> SetVisAttributes(va);
   
   nm=G4String("PV_BeamDump")+id.str().c_str();;
   G4VPhysicalVolume *BeamDumpPV = 
   new G4PVPlacement(G4Transform3D(rotMtx_, gPos_), concLV, nm,
           pMother, false, Id_);
   BeamDumpPV = 
   new G4PVPlacement(G4Transform3D(rotMtx_, gPos_), doorLV, nm,
           pMother, false, Id_);
   BeamDumpPV = 
   new G4PVPlacement(G4Transform3D(rotMtx_, gPos_), ironLV, nm,
           pMother, false, Id_);
   BeamDumpPV = 
   new G4PVPlacement(G4Transform3D(rotMtx_, gPos_), shieldLV0, nm,
           pMother, false, Id_);
   BeamDumpPV = 
   new G4PVPlacement(G4Transform3D(rotMtx_, gPos_), shieldLV1, nm,
           pMother, false, Id_);
   return BeamDumpPV;

}

//////////////////////////////////////////////////////////////
G4VPhysicalVolume *E16G4_BeamDump::PlaceE325( G4LogicalVolume *pMother )
//////////////////////////////////////////////////////////////
{ 
   G4RotationMatrix *zeRot= new G4RotationMatrix();
   zeRot->rotateY(0.*degree); 
   std::ostringstream id;
   id<<Id_;
   G4String nm;
   G4VisAttributes *va;
   G4double ironCent=-100*cm;
   G4double holeSize=80*cm;
   nm=G4String("IronSolid_BeamDump")+id.str().c_str();;
   //G4Box* ironSolid0= new G4Box(nm, 300/2.*cm, 300/2.*cm, 300/2.*cm);
   G4Box* ironSolid0= new G4Box(nm, 100/2.*cm, 100/2.*cm, 200/2.*cm);
   G4DisplacedSolid *ironSolid1 = new G4DisplacedSolid(
                      nm, ironSolid0, 0,
		  G4ThreeVector(0.*cm,0.*cm,ironCent));
   nm=G4String("HoleSolid_BeamDump")+id.str().c_str();;
   G4Box* holeSolid0= new G4Box(nm, holeSize/2., holeSize/2., 300*cm/2.);
   G4DisplacedSolid *holeSolid1 = new G4DisplacedSolid(
                      nm, holeSolid0, 0,
		  G4ThreeVector(0.*cm,0.*cm,-(300/2.*cm)));
   nm=G4String("ConcSolid_BeamDump")+id.str().c_str();;
   G4Box* concSolid0= new G4Box(nm, 600/2.*cm, 600/2.*cm, 600/2.*cm);
   G4SubtractionSolid* concSolid1=
      new G4SubtractionSolid(nm,concSolid0,ironSolid1);
   G4SubtractionSolid* concSolid2=
      new G4SubtractionSolid(nm,concSolid1,holeSolid1);
   
   nm=G4String("IronLV_BeamDump")+id.str().c_str();;
   G4LogicalVolume *ironLV= new G4LogicalVolume(ironSolid1, mIron_, nm);
   va= new G4VisAttributes(G4Color::Red());
   ironLV-> SetVisAttributes(va);

   //DetLV_= new G4LogicalVolume(concSolid0, mBox_, "LV_BeamDump");
   /*nm=G4String("ConcPV_BeamDump")+id.str().c_str();;
   new G4PVPlacement(zeRot, G4ThreeVector(0.*cm,0.*cm,0.*cm), concLV, nm,
           DetLV_, false, 0);*/
   /*nm=G4String("IronPV_BeamDump")+id.str().c_str();;
   new G4PVPlacement(zeRot, G4ThreeVector(0.*cm,0.*cm,0.*cm), ironLV, nm,
           concLV, false, 1);*/
   G4double doorThickness=15.;
   nm=G4String("doorSolid_BeamDump")+id.str().c_str();;
   G4Box* doorSolid0= new G4Box(nm, holeSize/2., holeSize/2., 
         doorThickness/2.*cm);
   G4Box* tmpSolid= new G4Box(nm, 30./2.*cm, 30./2.*cm, 
         doorThickness/2.*cm);
   G4SubtractionSolid* doorSolid1=
      new G4SubtractionSolid(nm,doorSolid0,tmpSolid);
   G4DisplacedSolid *doorSolid2 = new G4DisplacedSolid(
                      nm, doorSolid1, 0,
		  G4ThreeVector(0.*cm,0.*cm,
                     -(200.+100-doorThickness/2.)*cm));
   nm=G4String("doorLV_BeamDump")+id.str().c_str();;
   G4LogicalVolume *doorLV= new G4LogicalVolume(doorSolid2, mIron_, nm);
   va= new G4VisAttributes(G4Color::Red());
   doorLV-> SetVisAttributes(va);
  
   G4SubtractionSolid* concSolid3=
      new G4SubtractionSolid(nm,concSolid2,doorSolid2);
   
   nm=G4String("ConcLV_BeamDump")+id.str().c_str();;
   G4LogicalVolume *concLV= new G4LogicalVolume
        (concSolid3, mConc_, nm);
   va= new G4VisAttributes(G4Color::Gray());
   concLV-> SetVisAttributes(va);
  
   //Shield 
   G4double shieldThickness=50.;
   G4double shieldHoleSize=30.;
   G4double shieldPos=800.;
   nm=G4String("shieldSolid_BeamDump")+id.str().c_str();;
   G4Box* shieldSolid0= new G4Box(nm, 300./2.*cm, 300./2.*cm, 
         shieldThickness/2.*cm);
   tmpSolid= new G4Box(nm, shieldHoleSize/2.*cm, shieldHoleSize/2.*cm, 
         shieldThickness/2.*cm);
   G4SubtractionSolid* shieldSolid1=
      new G4SubtractionSolid(nm,shieldSolid0,tmpSolid);
   G4DisplacedSolid *shieldSolid2 = new G4DisplacedSolid(
                      nm, shieldSolid1, 0,
		  G4ThreeVector(0.*cm,0.*cm,-(shieldPos+25)*cm));
   G4DisplacedSolid *shieldSolid3 = new G4DisplacedSolid(
                      nm, shieldSolid1, 0,
		  G4ThreeVector(0.*cm,0.*cm,-(shieldPos-25)*cm));
   nm=G4String("shieldLV_BeamDump")+id.str().c_str();;
   G4LogicalVolume *shieldLV0= new G4LogicalVolume(shieldSolid2, mIron_, nm);
   G4LogicalVolume *shieldLV1= new G4LogicalVolume(shieldSolid3, mIron_, nm);
   va= new G4VisAttributes(G4Color::Red());
   shieldLV0-> SetVisAttributes(va);
   va= new G4VisAttributes(G4Color::Red());
   shieldLV1-> SetVisAttributes(va);
   
   nm=G4String("PV_BeamDump")+id.str().c_str();;
   G4VPhysicalVolume *BeamDumpPV = 
   new G4PVPlacement(G4Transform3D(rotMtx_, gPos_), concLV, nm,
           pMother, false, Id_);
   BeamDumpPV = 
   new G4PVPlacement(G4Transform3D(rotMtx_, gPos_), doorLV, nm,
           pMother, false, Id_);
   BeamDumpPV = 
   new G4PVPlacement(G4Transform3D(rotMtx_, gPos_), ironLV, nm,
           pMother, false, Id_);
   /*BeamDumpPV = 
   new G4PVPlacement(G4Transform3D(rotMtx_, gPos_), shieldLV0, nm,
           pMother, false, Id_);
   BeamDumpPV = 
   new G4PVPlacement(G4Transform3D(rotMtx_, gPos_), shieldLV1, nm,
           pMother, false, Id_);*/
   return BeamDumpPV;

}
