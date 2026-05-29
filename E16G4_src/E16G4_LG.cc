//2016-08-12, uploaded by nakai
//2015-03-01, uploaded by yokkaich
//2015-01-05, modified by yokkaich
//2015-01-05, modified by yokkaich
//2014-08-27, modified by kawama
//2014-08-27, modified by kawama
//2014-04-30, modified by kawama
//2013-11-14, modified by kawama
//2013-05-13, modified by kawama
/*
   E16G4_LG.cc

   D.Kawama
   */

#include <G4Material.hh>
#include <G4Box.hh>
#include <G4Trd.hh>
#include <G4UnionSolid.hh>
#include <G4SubtractionSolid.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>

#include <G4VisAttributes.hh>
#include <G4Colour.hh>

#include "E16G4_LG.hh"


///////////////////////////////////////////////
 E16G4_LG::E16G4_LG( const G4String &name,
      const G4ThreeVector &gPos,
      const G4RotationMatrix *rotMtx,
      int id )
: E16G4_BeamLineElement(name,gPos,rotMtx),
   mRad_(0), DetLV_(0),
   vSolid_(0), Id_(id)
///////////////////////////////////////////////
{
}

/////////////////
 E16G4_LG::~E16G4_LG()
/////////////////
{
}

////////////////////////////////////////////////////////
bool E16G4_LG::IsInside( const G4ThreeVector &gPos ) const
////////////////////////////////////////////////////////
{
   G4ThreeVector lPos = GetLocalPosition(gPos);
   if( vSolid_-> Inside(lPos) != kOutside ) return true;
   else return false;
}

#if 0
//////////////////////////////////////////////////////////////
G4VPhysicalVolume *E16G4_LG::Place( G4LogicalVolume *pMother )
//////////////////////////////////////////////////////////////
{ 
   G4RotationMatrix *zeRot= new G4RotationMatrix();
   zeRot->rotateY(0.*degree); 
   
   /*G4String nmBox=elemName_+"Box";
   G4Box* boxSolid = new G4Box(nmBox,12.2/2.*6.*cm, 15.3/2.*6.*cm,34./2.*cm);
   DetLV_ = new G4LogicalVolume(boxSolid, mRad_, nmBox);
   G4VisAttributes *va= new G4VisAttributes(G4Color(1.,1.,1.));
   DetLV_-> SetVisAttributes(G4VisAttributes::GetInvisible());
   //boxLV-> SetVisAttributes(va);*/
   
   G4String nmLG=elemName_;
   G4Trd* trd= new G4Trd(nmLG+"trd", 
         12.2*0.5*cm, 12.2*0.5*cm, 11.3*0.5*cm, 13.337*0.5*cm, 34.*0.5*cm);
   G4Box* box_tmp = new G4Box(nmLG+"tmp",12.2*0.5*cm, 2.2*0.5*cm,34.*0.5*cm);
   G4DisplacedSolid *box_sub = new G4DisplacedSolid(
                      nmLG+"sub", box_tmp, zeRot,
		  G4ThreeVector(0.*cm,-(11.3+2.2)*0.5*cm,0.*cm));
   G4SubtractionSolid* solidLG=
      new G4SubtractionSolid(nmLG+"solid",trd,box_sub);
   /*G4LogicalVolume *solidLV= new G4LogicalVolume(solidLG, mRad_, nmLG+"solidLV");
   va= new G4VisAttributes(G4Color(0.,1.,0.));
   solidLV-> SetVisAttributes(va);*/
 
   /*G4double x[6]={24.57,12.31,0,-12.31,-24.57,-36.74};
   G4double z[6]={-1.47,-0.37,0,-0.37,-1.47,-3.31};
   int k=0;
   G4DisplacedSolid *disLG[36]; 
   for (int i=0;i<6;i++){
      for (int j=0;j<6;j++){
         G4double theta=3.428*degree;
         G4double y=12.2*(2-i)*cm;
         G4RotationMatrix *rotLG= new G4RotationMatrix();
         rotLG->rotateX(theta); 
         G4ThreeVector pos = G4ThreeVector(x[j]*cm,y,z[j]*cm);
         //new G4PVPlacement(rotLG, pos, 
           //    solidLV, nmLG+"crystal",DetLV_, false, Id_);
         disLG[k] = new G4DisplacedSolid(
                      nmLG+"solid", solidLG, rotLG,pos);
         k++;
      }
   }
   G4UnionSolid *uniLG= new G4UnionSolid(nmLG+"solid",disLG[0],disLG[1]);
   for (int i=2;i<36;i++){
         uniLG = new G4UnionSolid(nmLG+"solid",uniLG,disLG[i]);
   }*/
   DetLV_= new G4LogicalVolume(solidLG, mRad_, nmLG+"LV");
   G4VisAttributes *va= new G4VisAttributes(G4Color(0.,1.,0.));
   //G4VisAttributes *va= new G4VisAttributes(false);
   DetLV_-> SetVisAttributes(va);
   //DetLV_-> SetVisAttributes(G4VisAttributes::GetInvisible());

   G4VPhysicalVolume *box = new G4PVPlacement
         (G4Transform3D(rotMtx_, gPos_), DetLV_, nmLG,
           pMother, false, Id_);
   return box;

}
#else
//////////////////////////////////////////////////////////////
G4VPhysicalVolume *E16G4_LG::Place( G4LogicalVolume *pMother )
//////////////////////////////////////////////////////////////
{ 
   G4RotationMatrix *zeRot= new G4RotationMatrix();
   zeRot->rotateY(0.*degree); 
   
   G4String nmLG=elemName_;

   G4Trd* trd= new G4Trd(nmLG+"trd", 
         122*0.5*mm, 122*0.5*mm, 134*0.5*mm, 134*0.5*mm, 161*0.5*mm);
 //         122*0.5*mm, 122*0.5*mm, 113*0.5*mm, 133.37*0.5*mm, 340*0.5*mm);

   G4Box* box_tmp = new G4Box(nmLG+"tmp",122*0.5*mm, 22*0.5*mm,340*0.5*mm);

   G4DisplacedSolid *box_sub = new G4DisplacedSolid(
                      nmLG+"sub", box_tmp, zeRot,
		  G4ThreeVector(0.*mm,-(113+22)*0.5*mm,0.*mm));
   G4SubtractionSolid* solidLG=
      new G4SubtractionSolid(nmLG+"solid",trd,box_sub);


   DetLV_= new G4LogicalVolume(solidLG, mRad_, nmLG+"LV");
   G4VisAttributes *va= new G4VisAttributes(G4Color(0.,1.,0.));

   DetLV_-> SetVisAttributes(va);
   //DetLV_-> SetVisAttributes(G4VisAttributes::GetInvisible());

   G4VPhysicalVolume *box = new G4PVPlacement
         (G4Transform3D(rotMtx_, gPos_), DetLV_, nmLG,
           pMother, false, Id_);
   return box;

}
#endif
