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
   E16G4_HBD.cc

   D.Kawama
   */

#include <G4Material.hh>
#include <G4Box.hh>
#include <G4Trd.hh>
#include <G4Trap.hh>
#include <G4SubtractionSolid.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4DisplacedSolid.hh>

#include <G4VisAttributes.hh>
#include <G4Colour.hh>

#include "E16G4_HBD.hh"

   
G4VisAttributes *vaGas= new G4VisAttributes(G4Color(1.,1.,0.));
G4VisAttributes *vaCham= new G4VisAttributes(G4Color(.5,0.5,0.5));
G4VisAttributes *vaGEM= new G4VisAttributes(G4Color(1.,0.,0.));
//G4VisAttributes *vaGas= new G4VisAttributes(false);
//G4VisAttributes *vaCham= new G4VisAttributes(false);
//G4VisAttributes *vaGEM= new G4VisAttributes(false);

///////////////////////////////////////////////
 E16G4_HBD::E16G4_HBD( const G4String &name,
      const G4ThreeVector &gPos,
      const G4RotationMatrix *rotMtx,
      int id )
: E16G4_BeamLineElement(name,gPos,rotMtx),
   mGas_(0), mWind_(0), mCath_(0), mFoil_(0), mBox_(0), mReadout_(0), 
   DetLV_(0), g0LV_(0), g1LV_(0), g2LV_(0), 
   vSolid_(0), Id_(id)
///////////////////////////////////////////////
{
}

/////////////////
 E16G4_HBD::~E16G4_HBD()
/////////////////
{
}

////////////////////////////////////////////////////////
bool E16G4_HBD::IsInside( const G4ThreeVector &gPos ) const
////////////////////////////////////////////////////////
{
   G4ThreeVector lPos = GetLocalPosition(gPos);
   if( vSolid_-> Inside(lPos) != kOutside ) return true;
   else return false;
}

//////////////////////////////////////////////////////////////
G4VPhysicalVolume *E16G4_HBD::Place0( G4LogicalVolume *pMother )
//////////////////////////////////////////////////////////////
{ 

   G4RotationMatrix *zeRot= new G4RotationMatrix();
   zeRot->rotateY(0.*degree); 
   G4String nmSol;
   G4String nmSolGem;
   G4String nmWind;
   G4String nmMesh;
   G4String nmDriftGap;
   G4String nmFoil;
   G4String nmPoly;
   G4String nmKap;
   G4String nmChamber;
   G4String nmReadout;
   G4String name;
   nmSol=elemName_;
   nmWind=elemName_+"_SolGEM";
   nmWind=elemName_+"_Window";
   nmMesh=elemName_+"_Mesh";
   nmDriftGap=elemName_+"_DriftGap";
   nmFoil=elemName_+"_Foil";
   nmPoly=elemName_+"_Poly";
   nmKap=elemName_+"_Kap";
   nmChamber=elemName_+"_Cham";
   nmReadout=elemName_+"_Readout";
   
   G4double tFrontpanelFrame=9*mm;
   G4double wFrontpanelFrame=17.6*mm;
   G4double tRearPanel=8*mm;
   G4double tReadout=1.6*mm;
   G4double tTBPanelFrame=13*mm;
   G4double wTBPanelFrame=15*mm;
   G4double tSidePanel=2*mm;
   
   G4RotationMatrix *rotpos= new G4RotationMatrix
      (-rotMtx_.phi(),rotMtx_.theta(),-rotMtx_.psi());
   G4RotationMatrix *rotint= new G4RotationMatrix
      (rotMtx_.phi(),rotMtx_.theta(),rotMtx_.psi());
   
  
   G4double dz_HBD=50.*cm; 
   G4double dxy1;
   G4double dxy2;
   dxy1=37.644*cm;
   dxy2=65.095*cm;
   G4Trd* Solid0= new G4Trd(nmSol, 
         dxy1/2., dxy2/2., 
         dxy1/2., dxy2/2., 
         dz_HBD/2.);
   G4DisplacedSolid *Solid1 = new G4DisplacedSolid(
                      nmSol, Solid0, zeRot,
		  G4ThreeVector(0.*cm,0.*cm,-dz_HBD/2.-0.5*cm));
   //G4LogicalVolume *BoxLV= new G4LogicalVolume(Solid1, mGas_, nmSol);
   DetLV_= new G4LogicalVolume(Solid1, mGas_, nmSol);
   //G4VisAttributes *va= new G4VisAttributes(false);
   DetLV_-> SetVisAttributes(vaGas);
   //BoxLV-> SetVisAttributes(G4VisAttributes::GetInvisible());

   //va= new G4VisAttributes(false);
   G4Box* solidGem= new G4Box(nmSolGem, dxy2/2., dxy2/2., 1.*cm);
   G4LogicalVolume *LVGem= new G4LogicalVolume
      (solidGem, mGas_, nmSolGem);
   LVGem-> SetVisAttributes(vaGEM);

   //G4double zMesh = 50/2.*cm+1.*um;
   G4double zMesh = -0.4*cm/2.+1.*um;
   G4Box* boxMesh= new G4Box(nmMesh, dxy2/2., dxy2/2., 2.*um);
   G4LogicalVolume* gemMesh1LV= new G4LogicalVolume
      (boxMesh, mCath_, nmMesh);
   gemMesh1LV-> SetVisAttributes(vaGEM);
   new G4PVPlacement(zeRot, G4ThreeVector(0.,0.,zMesh), gemMesh1LV, nmMesh, LVGem, false, 0);
   
   G4double zDriftGap = zMesh+0.4*cm/2.;
   G4Box* boxDriftGap= new G4Box(nmDriftGap, dxy2/2., dxy2/2., 0.4/2.*cm);
   G4LogicalVolume* gemDriftGapLV= new G4LogicalVolume(boxDriftGap, mGas_, nmDriftGap);
   gemDriftGapLV-> SetVisAttributes(vaGEM);
   new G4PVPlacement(zeRot, G4ThreeVector(0.,0.,zDriftGap), gemDriftGapLV, 
         nmDriftGap, LVGem, false, 0);

   G4double zPoly1 = zMesh+0.4*cm+1.*um+25.*um;
   G4double zPoly2 = zPoly1+0.15*cm+2.*um+50.*um;
   G4double zPoly3 = zPoly2+0.15*cm+2.*um+50.*um;
   G4Box* boxGEMPoly= new G4Box(nmPoly, dxy2/2., dxy2/2., 25.*um);
   G4LogicalVolume* gemPolyLV= new G4LogicalVolume
      (boxGEMPoly, mWind_, nmPoly);
   gemPolyLV-> SetVisAttributes(vaGEM);
   new G4PVPlacement(zeRot, G4ThreeVector(0.,0.,zPoly1),
         gemPolyLV, nmPoly, LVGem, false, 0);
   new G4PVPlacement(zeRot, G4ThreeVector(0.,0.,zPoly2),
         gemPolyLV, nmPoly, LVGem, false, 1);
   new G4PVPlacement(zeRot, G4ThreeVector(0.,0.,zPoly3),
         gemPolyLV, nmPoly, LVGem, false, 2);
   
   G4double zFoil1 = zPoly1-25.*um-1.*um;
   G4double zFoil2 = zPoly1+25.*um+1.*um;
   G4double zFoil3 = zPoly2-25.*um-1.*um;
   G4double zFoil4 = zPoly2+25.*um+1.*um;
   G4double zFoil5 = zPoly3-25.*um-1.*um;
   G4double zFoil6 = zPoly3+25.*um+1.*um;
   G4Box* foilBox= new G4Box(nmFoil, dxy2/2., dxy2/2., 2.*um);
   G4LogicalVolume* foilLV= new G4LogicalVolume
      (foilBox, mFoil_, nmFoil);
   foilLV-> SetVisAttributes(vaGEM);
   new G4PVPlacement(zeRot, 
         G4ThreeVector(0.,0.,zFoil1),
         foilLV, nmFoil, LVGem, false, 2);
   new G4PVPlacement(zeRot, 
         G4ThreeVector(0.,0.,zFoil2), 
         foilLV, nmFoil, LVGem, false, 3);
   new G4PVPlacement(zeRot, 
         G4ThreeVector(0.,0.,zFoil3),
         foilLV, nmFoil, LVGem, false, 4);
   new G4PVPlacement(zeRot, 
         G4ThreeVector(0.,0.,zFoil4), 
         foilLV, nmFoil, LVGem, false, 5);
   new G4PVPlacement(zeRot, 
         G4ThreeVector(0.,0.,zFoil5),
         foilLV, nmFoil, LVGem, false, 4);
   new G4PVPlacement(zeRot, 
         G4ThreeVector(0.,0.,zFoil6), 
         foilLV, nmFoil, LVGem, false, 5);


   /*G4double zKap=zPoly2+25./2.*um+2*um+0.2*cm;
   G4Box* boxGEMKap= new G4Box(nmKap, dxy2/2., dxy2/2., 12.5*um);
   G4LogicalVolume *gemKapLV_= new G4LogicalVolume
      (boxGEMKap, mWind_, nmKap);
   gemKapLV_-> SetVisAttributes(vaGEM);
   new G4PVPlacement(zeRot, G4ThreeVector(0.,0.,zKap), 
         gemKapLV_, nmKap, LVGem, false, 0);*/

   new G4PVPlacement
      (zeRot, G4ThreeVector(0.*cm,0.*cm,0.*cm), LVGem, nmSolGem, DetLV_, false, Id_);


   G4ThreeVector posHBD(0.,0.,0.);
   posHBD*=(*rotpos);
   posHBD+=gPos_;
   G4VPhysicalVolume *HBD = new G4PVPlacement
         (rotint, posHBD, DetLV_, nmSol,
           pMother, false, Id_);
   
   //Chamber
   //va= new G4VisAttributes(false);
   name=nmChamber+"FFrame";
   G4Box* FrontpanelFrame_tmp1= new G4Box(name, 
         //dxy1/2., 
         dxy1/2.-wFrontpanelFrame, 
         dxy1/2.-wFrontpanelFrame, 
         tFrontpanelFrame+1*cm);
   G4Box* FrontpanelFrame_tmp2= new G4Box(name, 
         dxy1/2., dxy1/2., tFrontpanelFrame);
   G4SubtractionSolid* FrontpanelFrame=
      new G4SubtractionSolid(name,FrontpanelFrame_tmp2,FrontpanelFrame_tmp1);
   G4LogicalVolume *FrontpanelFrameLV= new G4LogicalVolume
      (FrontpanelFrame, mBox_, name);
   FrontpanelFrameLV-> SetVisAttributes(vaCham);
   G4ThreeVector posFrontpanelFrame(0.,0.,-dz_HBD-tFrontpanelFrame/2.-0.5*cm);
   posFrontpanelFrame*=(*rotpos);
   posFrontpanelFrame+=gPos_;
  new G4PVPlacement(rotint, posFrontpanelFrame, 
        FrontpanelFrameLV, name, pMother, false, 0);
   
  G4double tFrontpanelMylar=50*um;
   name=nmChamber+"Mylar";
   G4Box* FrontpanelMylar= new G4Box(name, dxy1/2., dxy1/2., tFrontpanelMylar/2.);
   G4LogicalVolume *FrontpanelMylarLV= new G4LogicalVolume
      (FrontpanelMylar, mWind_, name);
   FrontpanelMylarLV-> SetVisAttributes(vaCham);
   G4ThreeVector posFrontpanelMylar(0.,0.,-dz_HBD-tFrontpanelMylar/2.-0.5*cm);
   posFrontpanelMylar*=(*rotpos);
   posFrontpanelMylar+=gPos_;
  new G4PVPlacement(rotint, posFrontpanelMylar, 
        FrontpanelMylarLV, name, pMother, false, 0);
  
  //RearPanel
   name=nmChamber+"RPanel";
   G4Box* RearpanelFrame= new G4Box(name, dxy2/2., dxy2/2., tRearPanel);
   G4LogicalVolume *RearpanelFrameLV= new G4LogicalVolume
      (RearpanelFrame, mBox_, name);
   RearpanelFrameLV-> SetVisAttributes(vaCham);
   G4ThreeVector posRearpanelFrame(0.,0.,tRearPanel/2.+0.5*cm);
   posRearpanelFrame*=(*rotpos);
   posRearpanelFrame+=gPos_;
  new G4PVPlacement(rotint, posRearpanelFrame, 
        RearpanelFrameLV, name, pMother, false, 0);
  //RearPanelFrame 
   /*G4double tRearpanelFrame=8*mm;
   G4double wRearpanelFrame=4.27*mm;
   name=nmChamber+"FrontFrame";
   G4Box* RearpanelFrame_tmp1= new G4Box(name, dxy2/2., dxy2/2., tRearpanelFrame+1*cm);
   G4Box* RearpanelFrame_tmp2= new G4Box(name, 
         (dxy2+wRearpanelFrame)/2., (dxy2+wRearpanelFrame)/2., tRearpanelFrame);
   G4SubtractionSolid* RearpanelFrame=
      new G4SubtractionSolid(name,RearpanelFrame_tmp2,RearpanelFrame_tmp1);
   G4LogicalVolume *RearpanelFrameLV= new G4LogicalVolume
      (RearpanelFrame, mBox_, name);
   RearpanelFrameLV-> SetVisAttributes(vaCham);
   G4ThreeVector posRearpanelFrame(0.,0.,tRearpanelFrame/2.+0.5*cm);
   posRearpanelFrame*=(*rotpos);
   posRearpanelFrame+=gPos_;
  new G4PVPlacement(rotint, posRearpanelFrame, 
        RearpanelFrameLV, name, pMother, false, 0);*/
   
   G4Box* boxReadout= new G4Box(nmReadout, dxy2/2., dxy2/2., tReadout);
   G4LogicalVolume *ReadoutLV= new G4LogicalVolume
      (boxReadout, mReadout_, nmReadout);
   ReadoutLV-> SetVisAttributes(vaCham);
   G4ThreeVector posReadout(0.,0.,1/2.*cm+tReadout/2.);
   posReadout*=(*rotpos);
   posReadout+=gPos_;
   new G4PVPlacement(rotint, posReadout, 
         ReadoutLV, nmReadout, pMother, false, 0);
   
   name=nmChamber+"SPanel0";
   G4double SidePanelLength=dz_HBD/cos(15.38*degree);
   G4Trd* SidePanel= new G4Trd(name, dxy1/2., dxy2/2., 
         tSidePanel/2., tSidePanel/2.,SidePanelLength/2.);
   
   G4RotationMatrix *rotSidePanel0= new G4RotationMatrix();
   rotSidePanel0->rotateY(-15.38*degree); 
   rotSidePanel0->rotateZ(90.*degree); 
   G4DisplacedSolid *SidePanel0 = new G4DisplacedSolid(
                      nmSol, SidePanel, rotSidePanel0,
		  G4ThreeVector(0.*cm,0*cm,0*cm));
   G4LogicalVolume *SidePanel0LV= new G4LogicalVolume
      (SidePanel0, mBox_, name);
   SidePanel0LV-> SetVisAttributes(vaCham);
   G4ThreeVector posSidePanel0(
         (dxy1/2.+dxy2/2.)/2.+tSidePanel/2.*cos(15.38*degree),0.,-dz_HBD/2.-0.5*cm);
   posSidePanel0*=(*rotpos);
   posSidePanel0+=gPos_;
   if((int)Id_/3==0 || (int)Id_/3==2 || (int)Id_/3==5 || (int)Id_/3==7  )
   new G4PVPlacement(rotint, posSidePanel0, 
         SidePanel0LV, name, pMother, false, 0);
   
   name=nmChamber+"SPanel1";
   G4RotationMatrix *rotSidePanel1= new G4RotationMatrix();
   rotSidePanel1->rotateY(15.38*degree); 
   rotSidePanel1->rotateZ(90.*degree); 
   G4DisplacedSolid *SidePanel1 = new G4DisplacedSolid(
                      nmSol, SidePanel, rotSidePanel1,
		  G4ThreeVector(0.*cm,0*cm,0*cm));
   G4LogicalVolume *SidePanel1LV= new G4LogicalVolume
      (SidePanel1, mBox_, name);
   SidePanel1LV-> SetVisAttributes(vaCham);
   G4ThreeVector posSidePanel1(
         -(dxy1/2.+dxy2/2.)/2.-tSidePanel/2.*cos(15.38*degree),0.,-dz_HBD/2.-0.5*cm);
   posSidePanel1*=(*rotpos);
   posSidePanel1+=gPos_;
   if((int)Id_/3==1 || (int)Id_/3==3 || (int)Id_/3==6 || (int)Id_/3==8  )
   new G4PVPlacement(rotint, posSidePanel1, 
         SidePanel1LV, name, pMother, false, 0);
  
  //Top and Bottom Panel Frame 
   name=nmChamber+"Frame";
   G4Trd* TBPanelFrame_tmp1= new G4Trd(name, 
         dxy1/2.-wTBPanelFrame, dxy2/2.-wTBPanelFrame, 
         tTBPanelFrame/2.+1*mm, tTBPanelFrame/2.+1*mm,
         SidePanelLength/2.-wTBPanelFrame);
   G4Trd* TBPanelFrame_tmp2= new G4Trd(name, 
         dxy1/2., dxy2/2., 
         tTBPanelFrame/2., tTBPanelFrame/2.,SidePanelLength/2.);
   G4SubtractionSolid* TBPanelFrame=
      new G4SubtractionSolid(name,TBPanelFrame_tmp2,TBPanelFrame_tmp1);
   //G4ThreeVector posTBPanelFrame(0.,0.,-dz_HBD-tTBPanelFrame/2.-0.5*cm);
   /*posTBPanelFrame*=(*rotpos);
   posTBPanelFrame+=gPos_;
  new G4PVPlacement(rotint, posTBPanelFrame, 
        TBPanelFrameLV, name, pMother, false, 0);*/
   
  name=nmChamber+"BPanel";
   G4RotationMatrix *rotSidePane2= new G4RotationMatrix();
   rotSidePane2->rotateX(-15.38*degree); 
   G4DisplacedSolid *SidePanel2 = new G4DisplacedSolid(
                      nmSol, SidePanel, rotSidePane2,
		  G4ThreeVector(0.*cm,0*cm,0*cm));
   G4LogicalVolume *SidePanel2V= new G4LogicalVolume
      (SidePanel2, mBox_, name);
   SidePanel2V-> SetVisAttributes(vaCham);
   G4ThreeVector posSidePanel2(
         0.,-(dxy1/2.+dxy2/2.)/2.-tSidePanel*cos(15.38*degree),-dz_HBD/2.);
   posSidePanel2*=(*rotpos);
   posSidePanel2+=gPos_;
   new G4PVPlacement(rotint, posSidePanel2, 
         SidePanel2V, name, pMother, false, 0);
  name=nmChamber+"BFrame";
   G4DisplacedSolid *TBPanelFrame2 = new G4DisplacedSolid(
                      name, TBPanelFrame, rotSidePane2,
		  G4ThreeVector(0.*cm,0*cm,0*cm));
   G4LogicalVolume *TBPanelFrameLV2= new G4LogicalVolume
      (TBPanelFrame2, mBox_, name);
   TBPanelFrameLV2-> SetVisAttributes(vaCham);
   G4ThreeVector posTBPanelFrame2=posSidePanel2+
      G4ThreeVector(0*mm,-tTBPanelFrame/2.,0.*mm);
  new G4PVPlacement(rotint, posTBPanelFrame2, 
        TBPanelFrameLV2, name, pMother, false, 0);
   
   name=nmChamber+"TPanel";
   G4RotationMatrix *rotSidePane3= new G4RotationMatrix();
   rotSidePane3->rotateX(15.38*degree); 
   G4DisplacedSolid *SidePanel3 = new G4DisplacedSolid(
                      nmSol, SidePanel, rotSidePane3,
		  G4ThreeVector(0.*cm,0*cm,0*cm));
   G4LogicalVolume *SidePanel3LV= new G4LogicalVolume
      (SidePanel3, mBox_, name);
   SidePanel3LV-> SetVisAttributes(vaCham);
   G4ThreeVector posSidePanel3(
         0.,(dxy1/2.+dxy2/2.)/2.+tSidePanel*cos(15.38*degree),-dz_HBD/2.);
   posSidePanel3*=(*rotpos);
   posSidePanel3+=gPos_;
   new G4PVPlacement(rotint, posSidePanel3, 
         SidePanel3LV, name, pMother, false, 0);
  name=nmChamber+"TFrame";
   G4DisplacedSolid *TBPanelFrame3 = new G4DisplacedSolid(
                      name, TBPanelFrame, rotSidePane3,
		  G4ThreeVector(0.*cm,0*cm,0*cm));
   G4LogicalVolume *TBPanelFrameLV3= new G4LogicalVolume
      (TBPanelFrame3, mBox_, name);
   TBPanelFrameLV3-> SetVisAttributes(vaCham);
   G4ThreeVector posTBPanelFrame3=posSidePanel3+
      G4ThreeVector(0*mm,tTBPanelFrame/2.,0.*mm);
  new G4PVPlacement(rotint, posTBPanelFrame3, 
        TBPanelFrameLV3, name, pMother, false, 0);


   
   
   return HBD;
}

//////////////////////////////////////////////////////////////
G4VPhysicalVolume *E16G4_HBD::Place1( G4LogicalVolume *pMother )
//////////////////////////////////////////////////////////////
{ 

   G4RotationMatrix *zeRot= new G4RotationMatrix();
   zeRot->rotateY(0.*degree); 
   G4String nmSol;
   G4String nmSolGem;
   G4String nmWind;
   G4String nmMesh;
   G4String nmDriftGap;
   G4String nmFoil;
   G4String nmPoly;
   G4String nmKap;
   G4String nmChamber;
   G4String nmReadout;
   G4String name;
   nmSol=elemName_;
   nmWind=elemName_+"_SolGEM";
   nmWind=elemName_+"_Window";
   nmMesh=elemName_+"_Mesh";
   nmDriftGap=elemName_+"_DriftGap";
   nmFoil=elemName_+"_Foil";
   nmPoly=elemName_+"_Poly";
   nmKap=elemName_+"_Kap";
   nmChamber=elemName_+"_Cham";
   nmReadout=elemName_+"_Readout";
   
   G4double tFrontpanelFrame=9*mm;
   G4double wFrontpanelFrame=17.6*mm;
   G4double tRearPanel=8*mm;
   G4double tReadout=1.6*mm;
   G4double tTBPanelFrame=13*mm;
   G4double wTBPanelFrame=15*mm;
   G4double tSidePanel=2*mm;
   
  
   G4double dz_HBD=50.*cm; 
   G4double dxy1;
   G4double dxy2;
   dxy1=37.644*cm;
   dxy2=65.095*cm;
   G4double dy1;
   G4double dy2;
   G4double dy3;
   G4double dy4;
   //G4double angle=(40.06+19.55)/2.*degree;
   //G4double angle1=19.55*degree;
   //G4double angle=(39.814+15.38)/2.*degree;
   //G4double angle1=15.38*degree;
   G4double angle1=22*degree;
   G4double angle2=42*degree;
   G4double angle=(angle1+angle2)/2.;
   dy1=dxy1;
   dy2=dxy2+dz_HBD*tan(angle)*2.;
   //dy4=dy2-dxy2;
   //dy3=dy4-2*dz_HBD*tan(angle1);
   dy3=30*cm;//arbitrary value
   dy4=dy3+2*dz_HBD*tan(angle1);
   
   G4RotationMatrix *rotpos= new G4RotationMatrix
      (-rotMtx_.phi(),rotMtx_.theta(),-rotMtx_.psi());
   G4RotationMatrix *rotint= new G4RotationMatrix
      (rotMtx_.phi(),rotMtx_.theta(),rotMtx_.psi());
   
   G4Trd* Solid1= new G4Trd(nmSol, 
         dxy1/2., dxy2/2., 
         dy1/2., dy2/2., 
         dz_HBD/2.);
   G4Trd* Solid2= new G4Trd(nmSol, 
         dxy1/2.+0.1*mm, dxy2/2.+0.1*mm, 
         dy3/2.+0.1*mm, dy4/2.+0.1*mm, 
         dz_HBD/2.);
   G4DisplacedSolid *Solid3 = new G4DisplacedSolid(
                      nmSol, Solid2, zeRot,
		  G4ThreeVector(0.*cm,(-dz_HBD*tan(angle)-dy1/2.-dy3/2.)*int(Id_%3-1),-dz_HBD/2.-0.5*cm));
   G4DisplacedSolid *Solid4 = new G4DisplacedSolid(
                      nmSol, Solid1, zeRot,
		  G4ThreeVector(0.*cm,-dz_HBD*tan(angle)*int(Id_%3-1),-dz_HBD/2.-0.5*cm));
   G4SubtractionSolid* Solid5=
      new G4SubtractionSolid(nmSol,Solid4,Solid3);
   //G4LogicalVolume *BoxLV= new G4LogicalVolume(Solid5, mGas_, nmSol);
   DetLV_= new G4LogicalVolume(Solid5, mGas_, nmSol);
   DetLV_-> SetVisAttributes(vaGas);

   G4Box* solidGem= new G4Box(nmSolGem, dxy2/2., dxy2/2., 1.*cm);
   G4LogicalVolume *LVGem= new G4LogicalVolume
      (solidGem, mGas_, nmSolGem);
   LVGem-> SetVisAttributes(vaGEM);

   /*G4Box* boxWindow= new G4Box(nmWind, dxy2/2., dxy2/2., 10.*um);
   G4LogicalVolume* window1LV= new G4LogicalVolume
      (boxWindow, mWind_, nmWind);
   window1LV-> SetVisAttributes(vaGEM);
   new G4PVPlacement(zeRot, G4ThreeVector(0.,0.,-0.95*cm), 
         window1LV, nmWind, LVGem, false, 0);*/

   //G4double zMesh = 50/2.*cm+1.*um;
   G4double zMesh = -0.2*cm+1.*um;
   G4Box* boxMesh= new G4Box(nmMesh, dxy2/2., dxy2/2., 2.*um);
   G4LogicalVolume* gemMesh1LV= new G4LogicalVolume
      (boxMesh, mCath_, nmMesh);
   gemMesh1LV-> SetVisAttributes(vaGEM);
   new G4PVPlacement(zeRot, G4ThreeVector(0.,0.,zMesh), gemMesh1LV, nmMesh, LVGem, false, 0);
   
   G4double zDriftGap = zMesh+0.4*cm/2.;//~0
   G4Box* boxDriftGap= new G4Box(nmDriftGap, dxy2/2., dxy2/2., 0.4/2.*cm);
   G4LogicalVolume *gemDriftGapLV= new G4LogicalVolume(boxDriftGap, mGas_, nmDriftGap);
   gemDriftGapLV-> SetVisAttributes(vaGEM);
   new G4PVPlacement(zeRot, G4ThreeVector(0.,0.,zDriftGap), gemDriftGapLV, 
         nmDriftGap, LVGem, false, 0);

   G4double zPoly1 = zMesh+0.4*cm+1.*um+25.*um;
   G4double zPoly2 = zPoly1+0.15*cm+2.*um+50.*um;
   G4double zPoly3 = zPoly1+0.15*cm+2.*um+50.*um;
   G4Box* boxGEMPoly= new G4Box(nmPoly, dxy2/2., dxy2/2., 25.*um);
   G4LogicalVolume* gemPolyLV= new G4LogicalVolume
      (boxGEMPoly, mWind_, nmPoly);
   gemPolyLV-> SetVisAttributes(vaGEM);
   new G4PVPlacement(zeRot, G4ThreeVector(0.,0.,zPoly1),
         gemPolyLV, nmPoly, LVGem, false, 0);
   new G4PVPlacement(zeRot, G4ThreeVector(0.,0.,zPoly2),
         gemPolyLV, nmPoly, LVGem, false, 1);
   new G4PVPlacement(zeRot, G4ThreeVector(0.,0.,zPoly3),
         gemPolyLV, nmPoly, LVGem, false, 2);
   
   G4double zFoil1 = zPoly1-25.*um-1.*um;
   G4double zFoil2 = zPoly1+25.*um+1.*um;
   G4double zFoil3 = zPoly2-25.*um-1.*um;
   G4double zFoil4 = zPoly2+25.*um+1.*um;
   G4double zFoil5 = zPoly3-25.*um-1.*um;
   G4double zFoil6 = zPoly3+25.*um+1.*um;
   G4Box* foilBox= new G4Box(nmFoil, dxy2/2., dxy2/2., 2.*um);
   G4LogicalVolume* foilLV= new G4LogicalVolume
      (foilBox, mFoil_, nmFoil);
   foilLV-> SetVisAttributes(vaGEM);
   new G4PVPlacement(zeRot, 
         G4ThreeVector(0.,0.,zFoil1),
         foilLV, nmFoil, LVGem, false, 0);
   new G4PVPlacement(zeRot, 
         G4ThreeVector(0.,0.,zFoil2), 
         foilLV, nmFoil, LVGem, false, 1);
   new G4PVPlacement(zeRot, 
         G4ThreeVector(0.,0.,zFoil3),
         foilLV, nmFoil, LVGem, false, 2);
   new G4PVPlacement(zeRot, 
         G4ThreeVector(0.,0.,zFoil4), 
         foilLV, nmFoil, LVGem, false, 3);
   new G4PVPlacement(zeRot, 
         G4ThreeVector(0.,0.,zFoil5),
         foilLV, nmFoil, LVGem, false, 4);
   new G4PVPlacement(zeRot, 
         G4ThreeVector(0.,0.,zFoil6), 
         foilLV, nmFoil, LVGem, false, 5);


   /*G4double zKap=zPoly2+25./2.*um+2*um+0.2*cm;
   G4Box* boxGEMKap= new G4Box(nmKap, dxy2/2., dxy2/2., 12.5*um);
   G4LogicalVolume *gemKapLV_= new G4LogicalVolume
      (boxGEMKap, mWind_, nmKap);
   gemKapLV_-> SetVisAttributes(vaGEM);
   new G4PVPlacement(zeRot, G4ThreeVector(0.,0.,zKap), gemKapLV_, nmKap, LVGem, false, 0);
*/
   new G4PVPlacement
      (zeRot, G4ThreeVector(0.*cm,0.*cm,0.*cm), LVGem, nmSolGem, DetLV_, false, Id_);


   G4ThreeVector posHBD(0.,0.,0.);
   posHBD*=(*rotpos);
   posHBD+=gPos_;
   G4VPhysicalVolume *HBD = new G4PVPlacement
         (rotint, posHBD, DetLV_, nmSol,
           pMother, false, Id_);
   /*G4VPhysicalVolume *HBD = new G4PVPlacement
         (G4Transform3D(rotMtx_, gPos_), DetLV_, nmSol,
           pMother, false, Id_);*/
   
   
   //Chamber
   name=nmChamber+"FFrame";
   G4Box* FrontpanelFrame_tmp1= new G4Box(name, 
         //dxy1/2., 
         dxy1/2.-wFrontpanelFrame, 
         dxy1/2.-wFrontpanelFrame, 
         tFrontpanelFrame+1*cm);
   G4Box* FrontpanelFrame_tmp2= new G4Box(name, 
         dxy1/2., dxy1/2., tFrontpanelFrame);
   G4SubtractionSolid* FrontpanelFrame=
      new G4SubtractionSolid(name,FrontpanelFrame_tmp2,FrontpanelFrame_tmp1);
   G4LogicalVolume *FrontpanelFrameLV= new G4LogicalVolume
      (FrontpanelFrame, mBox_, name);
   FrontpanelFrameLV-> SetVisAttributes(vaCham);
   G4ThreeVector posFrontpanelFrame(0.,(-dz_HBD*tan(angle))*int(Id_%3-1),-dz_HBD-tFrontpanelFrame/2.);
   posFrontpanelFrame*=(*rotpos);
   posFrontpanelFrame+=gPos_;
   new G4PVPlacement(rotint, posFrontpanelFrame, 
         FrontpanelFrameLV, name, pMother, false, 0);
  
   G4double tFrontpanelMylar=25*um;
   name=nmChamber+"Mylar";
   G4Box* FrontpanelMylar= new G4Box(name, dxy1/2., dxy1/2., tFrontpanelMylar);
   G4LogicalVolume *FrontpanelMylarLV= new G4LogicalVolume
      (FrontpanelMylar, mWind_, name);
   FrontpanelMylarLV-> SetVisAttributes(vaCham);
   G4ThreeVector posFrontpanelMylar(0.,(-dz_HBD*tan(angle))*int(Id_%3-1),-dz_HBD-tFrontpanelMylar/2.-0.5*cm);
   posFrontpanelMylar*=(*rotpos);
   posFrontpanelMylar+=gPos_;
  new G4PVPlacement(rotint, posFrontpanelMylar, 
        FrontpanelMylarLV, name, pMother, false, 0);
  
 //RearPanel 
   name=nmChamber+"RPanel";
   G4Box* boxRearpanel= new G4Box(name, dxy2/2., dxy2/2., tRearPanel);
   G4LogicalVolume *RearpanelLV= new G4LogicalVolume
      (boxRearpanel, mBox_, name);
   RearpanelLV-> SetVisAttributes(vaCham);
   G4ThreeVector posRearpanel(0.,0.,1/2.*cm+tRearPanel/2.);
   posRearpanel*=(*rotpos);
   posRearpanel+=gPos_;
   new G4PVPlacement(rotint, posRearpanel, 
         RearpanelLV, name, pMother, false, 0);
  //RearPanelFrame
   /*G4double tRearpanelFrame=8*mm;
   G4double wRearpanelFrame=4.27*mm;
   name=nmChamber+"Frame1";
   G4Box* RearpanelFrame_tmp1= new G4Box(name, dxy2/2., dxy2/2., tRearpanelFrame+1*cm);
   G4Box* RearpanelFrame_tmp2= new G4Box(name, 
         (dxy2+wRearpanelFrame)/2., (dxy2+wRearpanelFrame)/2., tRearpanelFrame);
   G4SubtractionSolid* RearpanelFrame=
      new G4SubtractionSolid(name,RearpanelFrame_tmp2,RearpanelFrame_tmp1);
   G4LogicalVolume *RearpanelFrameLV= new G4LogicalVolume
      (RearpanelFrame, mBox_, name);
   RearpanelFrameLV-> SetVisAttributes(vaCham);
   G4ThreeVector posRearpanelFrame(0.,0.,tRearpanelFrame/2.+0.5*cm);
   posRearpanelFrame*=(*rotpos);
   posRearpanelFrame+=gPos_;
  new G4PVPlacement(rotint, posRearpanelFrame, 
        RearpanelFrameLV, name, pMother, false, 0);*/
   
   G4Box* boxReadout= new G4Box(nmReadout, dxy2/2., dxy2/2., tReadout);
   G4LogicalVolume *ReadoutLV= new G4LogicalVolume
      (boxReadout, mReadout_, nmReadout);
   ReadoutLV-> SetVisAttributes(vaCham);
   G4ThreeVector posReadout(0.,0.,1/2.*cm+tReadout/2.);
   posReadout*=(*rotpos);
   posReadout+=gPos_;
   new G4PVPlacement(rotint, posReadout, 
         ReadoutLV, nmReadout, pMother, false, 0);
  
  //Solid of SidePanel 
   name=nmChamber+"Panel";
   G4double angle3=15.38*degree;
   G4Trd* SidePanel_tmp1= new G4Trd(name, 
         tSidePanel/2., tSidePanel/2., dy1/2., dy2/2.,dz_HBD/2./cos(angle3));
   G4Trd* SidePanel_tmp2= new G4Trd(name, 
         tSidePanel/2.+0.1*mm, tSidePanel/2.+0.1*mm, dy3/2.+0.1*mm, dy4/2.+0.1*mm, 
         dz_HBD/2./cos(angle3));
   G4DisplacedSolid *SidePanel_tmp3 = new G4DisplacedSolid(
                      name, SidePanel_tmp2, zeRot,
		  G4ThreeVector(0.*cm,(-dz_HBD*tan(angle)-dy1/2.-dy3/2.)*int(Id_%3-1),-dz_HBD/2.-0.5*cm));
   G4DisplacedSolid *SidePanel_tmp4 = new G4DisplacedSolid(
                      name, SidePanel_tmp1, zeRot,
		  G4ThreeVector(0.*cm,-dz_HBD*tan(angle)*int(Id_%3-1),-dz_HBD/2.-0.5*cm));
   G4SubtractionSolid* SidePanel=
      new G4SubtractionSolid(name,SidePanel_tmp4,SidePanel_tmp3);
  
  //SidePanel0 
   name=nmChamber+"SPanel0";
   G4RotationMatrix *rotSidePanel0= new G4RotationMatrix();
   rotSidePanel0->rotateY(-angle3); 
   G4DisplacedSolid *SidePanel0 = new G4DisplacedSolid(
                      name, SidePanel, rotSidePanel0,
		  G4ThreeVector(0.,0.,0.));
   G4LogicalVolume *SidePanel0LV= new G4LogicalVolume
      (SidePanel0, mBox_, name);
   SidePanel0LV-> SetVisAttributes(vaCham);
   G4ThreeVector posSidePanel0(
         (dxy2)/2.+tSidePanel*cos(angle3),0.,0.);
   posSidePanel0*=(*rotpos);
   posSidePanel0+=gPos_;
   if((int)Id_/3==0 || (int)Id_/3==2 || (int)Id_/3==5 || (int)Id_/3==7  )
   new G4PVPlacement(rotint, posSidePanel0, 
         SidePanel0LV, name, pMother, false, 0);
  
  //SidePanel1 
   name=nmChamber+"SPanel1";
   G4RotationMatrix *rotSidePane1= new G4RotationMatrix();
   rotSidePane1->rotateY(angle3); 
   G4DisplacedSolid *SidePanel1 = new G4DisplacedSolid(
                      name, SidePanel, rotSidePane1,
		  G4ThreeVector(0.,0.,0.));
   G4LogicalVolume *SidePanel1LV= new G4LogicalVolume
      (SidePanel1, mBox_, name);
   SidePanel1LV-> SetVisAttributes(vaCham);
   G4ThreeVector posSidePanel1(
         -(dxy2)/2.-tSidePanel*cos(angle3),0.,0.);
   posSidePanel1*=(*rotpos);
   posSidePanel1+=gPos_;
   if((int)Id_/3==1 || (int)Id_/3==3 || (int)Id_/3==6 || (int)Id_/3==8  )
   new G4PVPlacement(rotint, posSidePanel1, 
         SidePanel1LV, name, pMother, false, 0);
   
  //---Top Panel Frame 
   G4double TopPanelLength=dz_HBD/cos(angle1);
   name=nmChamber+"Frame";
   G4Trd* TopPanelFrame_tmp1= new G4Trd(name, 
         dxy1/2.-wTBPanelFrame, dxy2/2.-wTBPanelFrame, 
         tTBPanelFrame/2.+1*mm, tTBPanelFrame/2.+1*mm,
         TopPanelLength/2.-wTBPanelFrame);
   G4Trd* TopPanelFrame_tmp2= new G4Trd(name, 
         dxy1/2., dxy2/2., 
         tTBPanelFrame/2., tTBPanelFrame/2.,TopPanelLength/2.);
   G4SubtractionSolid* TopPanelFrame=
      new G4SubtractionSolid(name,TopPanelFrame_tmp2,TopPanelFrame_tmp1);
   //---
  //---Bottom Panel Frame 
   G4double BotPanelLength=dz_HBD/cos(angle2);
   name=nmChamber+"Frame";
   G4Trd* BotPanelFrame_tmp1= new G4Trd(name, 
         dxy1/2.-wTBPanelFrame, dxy2/2.-wTBPanelFrame, 
         tTBPanelFrame/2.+1*mm, tTBPanelFrame/2.+1*mm,
         BotPanelLength/2.-wTBPanelFrame);
   G4Trd* BotPanelFrame_tmp2= new G4Trd(name, 
         dxy1/2., dxy2/2., 
         tTBPanelFrame/2., tTBPanelFrame/2.,BotPanelLength/2.);
   G4SubtractionSolid* BotPanelFrame=
      new G4SubtractionSolid(name,BotPanelFrame_tmp2,BotPanelFrame_tmp1);
   //---

   if(int(Id_%3-1)==1)name=nmChamber+"BPanel";
   else name=nmChamber+"TPanel";
   G4Trd* Toppanel= new G4Trd(name, dxy1/2., dxy2/2., 
         tSidePanel/2., tSidePanel/2.,TopPanelLength/2.);
   G4RotationMatrix *rotSidePanel2= new G4RotationMatrix();
   rotSidePanel2->rotateX(angle1*int(Id_%3-1)); 
   G4DisplacedSolid *SidePanel2 = new G4DisplacedSolid(
                      name, Toppanel, rotSidePanel2,
		  G4ThreeVector(0.,0.,0.));
   G4LogicalVolume *SidePanel2LV= new G4LogicalVolume
      (SidePanel2, mBox_, name);
   SidePanel2LV-> SetVisAttributes(vaCham);
   G4ThreeVector posSidePanel2(
         0,
         (-(dxy2)/2.-dz_HBD/2.*tan(angle1)+tSidePanel/2.*cos(angle1))*int(Id_%3-1),
         -dz_HBD/2.-0.5*cm);
         //0,(-(dxy2)/2.-dz_HBD/2.*tan(16.5*degree)-tSidePanel*cos(15.38*degree))*int(Id_%3-1),-dz_HBD/2.);
   posSidePanel2*=(*rotpos);
   posSidePanel2+=gPos_;
   new G4PVPlacement(rotint, posSidePanel2, 
         SidePanel2LV, name, pMother, false, 0);
  //Frame
   if(int(Id_%3-1)==1)name=nmChamber+"BFrame";
   else name=nmChamber+"TFrame";
   G4DisplacedSolid *TopPanelFrame_disp = new G4DisplacedSolid(
                      name, TopPanelFrame, rotSidePanel2,
		  G4ThreeVector(0.*cm,0*cm,0*cm));
   G4LogicalVolume *TopPanelFrameLV= new G4LogicalVolume
      (TopPanelFrame_disp, mBox_, name);
   TopPanelFrameLV-> SetVisAttributes(vaCham);
   G4ThreeVector posTopPanelFrame=posSidePanel2+
      G4ThreeVector(0*mm,tTBPanelFrame/2.,0.*mm);
  new G4PVPlacement(rotint, posTopPanelFrame, 
        TopPanelFrameLV, name, pMother, false, 0);
  
 //Panel 
   if(int(Id_%3-1)==-1)name=nmChamber+"BPanel";
   else name=nmChamber+"TPanel";
   G4Trd* Botpanel= new G4Trd(name, dxy1/2., dxy2/2., 
         tSidePanel/2., tSidePanel/2.,BotPanelLength/2.);
   
   G4RotationMatrix *rotSidePanel3= new G4RotationMatrix();
   rotSidePanel3->rotateX(angle2*int(Id_%3-1)); 
   G4DisplacedSolid *SidePanel3 = new G4DisplacedSolid(
                      name, Botpanel, rotSidePanel3,
		  G4ThreeVector(0.,0.,0.));
   G4LogicalVolume *SidePanel3LV= new G4LogicalVolume
      (SidePanel3, mBox_, name);
   SidePanel3LV-> SetVisAttributes(vaCham);
   G4ThreeVector posSidePanel3(
         0,
         ((dxy2)/2.-dz_HBD/2.*tan(angle2)+tSidePanel/2.*cos(angle2))*int(Id_%3-1),
         //((dxy2)/2.-dz_HBD/2.*tan(angle2)+tSidePanel*cos(angle2)+1*cm)*int(Id_%3-1),
         //((dxy2)/2.-dz_HBD/2.*tan(39.814*degree)-tSidePanel*cos(15.38*degree))*int(Id_%3-1),
         -dz_HBD/2.-0.5*cm);
         //0,((dxy2)/2.-dz_HBD/2.*tan(39.814*degree)+tSidePanel*cos(15.38*degree))*int(Id_%3-1),-dz_HBD/2.);
   posSidePanel3*=(*rotpos);
   posSidePanel3+=gPos_;
   new G4PVPlacement(rotint, posSidePanel3, 
         SidePanel3LV, name, pMother, false, 0);
  //Frame
   if(int(Id_%3-1)==-1)name=nmChamber+"BFrame";
   else name=nmChamber+"TFrame";
   G4DisplacedSolid *BotPanelFrame_disp = new G4DisplacedSolid(
                      name, BotPanelFrame, rotSidePanel3,
		  G4ThreeVector(0.*cm,0*cm,0*cm));
   G4LogicalVolume *BotPanelFrameLV= new G4LogicalVolume
      (BotPanelFrame_disp, mBox_, name);
   BotPanelFrameLV-> SetVisAttributes(vaCham);
   G4ThreeVector posBotPanelFrame=posSidePanel3+
      G4ThreeVector(0*mm,tTBPanelFrame/2.,0.*mm);
  new G4PVPlacement(rotint, posBotPanelFrame, 
        BotPanelFrameLV, name, pMother, false, 0);
   
   
   
   return HBD;
}
