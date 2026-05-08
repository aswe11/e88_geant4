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
   E16G4_GTRGEM.cc

   D.Kawama
   */

#define RUN0E // special conf for Id_/3 = 13
#include <G4Material.hh>
#include <G4Box.hh>
#include <G4Trd.hh>
#include <G4SubtractionSolid.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>

#include <G4VisAttributes.hh>
#include <G4Colour.hh>

#include "E16G4_GTRGEM.hh"

G4double tcfrp=4*mm;

///////////////////////////////////////////////
 E16G4_GTRGEM::E16G4_GTRGEM( const G4String &name,
      const G4ThreeVector &gPos,
      const G4RotationMatrix *rotMtx,
      int id )
: E16G4_BeamLineElement(name,gPos,rotMtx),
   mGas_(0),  mMylar_(0), mAl_(0), mKap_(0), 
   mCu_(0), mNi_(0), mEpox_(0),mAcryl_(0),
   DetLV_(0), vSolid_(0), Id_(id)
///////////////////////////////////////////////
{
}

/////////////////
 E16G4_GTRGEM::~E16G4_GTRGEM()
/////////////////
{
}

////////////////////////////////////////////////////////
bool E16G4_GTRGEM::IsInside( const G4ThreeVector &gPos ) const
////////////////////////////////////////////////////////
{
   G4ThreeVector lPos = GetLocalPosition(gPos);
   if( vSolid_-> Inside(lPos) != kOutside ) return true;
   else return false;
}

//////////////////////////////////////////////////////////////
G4VPhysicalVolume *E16G4_GTRGEM::PlaceSQ( G4LogicalVolume *pMother )
   //////////////////////////////////////////////////////////////
{
   G4double sizex= 0;
   G4double sizey= 0;
   G4double size1x= 10.*cm;
   G4double size2x= 20.*cm;
   G4double size3x= 30.*cm;
   G4double size1y= 10.*cm;
   G4double size2y= 20.*cm;
   G4double size3y= 30.*cm;
   //sako
   G4double size4x= 90.*cm;
   G4double size4y= 90.*cm;
   G4double framesize;
   //G4double cfrpframesize;
   //G4double cfrpframesize1= 0.7*cm;
   //G4double cfrpframesize2= 0.949*cm;
   G4double framesize1= 0.7*cm;
   G4double framesize2= 0.74*cm;
   //thickness
   G4double tmylar=25.*um;
   G4double tal=1.*um;
   G4double tgem=50.*um*(1-0.0489);
   G4double tcu=4.*um*(1-0.195);
   G4double tdgap=6.*mm;
   G4double ttgap=2.2*mm;
   G4double tcux=2.*um*(1-0.311);
   G4double tcuy=2.*um*(1-0.858);
   G4double tpadkap=25.*um*(1-0.0104);
   G4double tni=2.*um*(1-0.311);
   G4double tepox=100.*um;
   G4double tgas1=tdgap;
   //G4double tgas2=tepox+tni+tpadkap+tcuy+tcux+ttgap*3;
   G4double tgas2=tni+tpadkap+tcuy+tcux+ttgap*3+tgem*3+tcu*3;
   G4double tframedriftspacer=6.4*mm;
   G4double tframereadout=1.7*mm;
   G4double tframegem=1.7*mm;
   G4double tframeseal=0.5*mm;
   G4double tframealbase=3.*mm;
   G4double tframecfrp=tcfrp;
   G4double foiloffset=0;
   G4String nmGas1;
   G4String nmGas2;
   G4String nmWind;
   G4String nmMylar;
   G4String nmAl;
   G4String nmGEM;
   G4String nmCu;
   G4String nmCux;
   G4String nmCuy;
   G4String nmPadKap;
   G4String nmNi;
   G4String nmEpox;
   G4String nmFrameDriftSpacer;
   G4String nmFrameReadOut;
   G4String nmFrameGEM;
   G4String nmFrameSeal;
   G4String nmFrameAlBase;
   G4String nmFrameCFRP;
   G4String nmPoleCFRP;
   nmGas1=elemName_+"_DriftGap";
   nmGas2=elemName_+"_TransferGap";
   nmWind=elemName_+"_Window";
   nmMylar=elemName_+"_Mylar";
   nmAl=elemName_+"_Al";
   nmGEM=elemName_+"_GEM";
   nmCu=elemName_+"_Cu";
   nmCux=elemName_+"_Cux";
   nmCuy=elemName_+"_Cuy";
   nmPadKap=elemName_+"_PadKap";
   nmNi=elemName_+"_Ni";
   nmEpox=elemName_+"_Epox";
   nmFrameDriftSpacer=elemName_+"_DriftSpacerFrame";
   nmFrameReadOut=elemName_+"_ReadOutFrame";
   nmFrameGEM=elemName_+"_GEMFrame";
   nmFrameSeal=elemName_+"_GasSeal";
   nmFrameAlBase=elemName_+"_AlBase";
   nmFrameCFRP=elemName_+"_CFRP";
   nmPoleCFRP=elemName_+"_CFRPpole";
   G4bool poleflag;
   G4double polelength;

   G4RotationMatrix *rot= new G4RotationMatrix
      (-rotMtx_.phi(),rotMtx_.theta(),-rotMtx_.psi());
   G4RotationMatrix *zeRot= new G4RotationMatrix();
   zeRot->rotateY(0.*degree);
   G4VisAttributes* va;
   va= new G4VisAttributes(G4Color(1.,0.,0.));
   //va= new G4VisAttributes(false);
   G4int zsign=1;
   if(int(int(Id_/3)/3)%2!=0) zsign=-1;

   G4cout << "++ Id_, Id_%3 = " << Id_ << " " << Id_%3 << G4endl;
   switch(Id_%3){
      case 0:
         sizex=size1x;
         sizey=size1y;
         foiloffset=0.05*cm;
         //cfrpframesize=cfrpframesize1;
         framesize=framesize1;
         polelength=40.*cm;
         break;
      case 1:
         sizex=size2x;
         sizey=size2y;
         foiloffset=0.25*cm;
         //cfrpframesize=cfrpframesize2;
         framesize=framesize2;
         polelength=75.*cm;
         break;
      default:
	 //def
	 sizex=size3x;
         sizey=size3y;
         foiloffset=0.25*cm;
         polelength=110.*cm;
         break;
   }

   //sako for RPC dummy
   G4cout <<"++ Id_, Id_/3, Id_%3, sizex = " << Id_ << " " << Id_/3 << " " << Id_%3 << " " << sizex <<G4endl;   
   //   if ((Id_==41)||(Id_==59)) {
   //     sizex=size4x;
   //     sizey=size4y;
   //   }
   
   if(int(Id_/3)%3==1)poleflag=true;
   else poleflag=false;

   G4Box* boxGas1= new G4Box(nmGas1, sizex/2., sizey/2., tgas1/2);
   DetLV_= new G4LogicalVolume(boxGas1, mGas_, nmGas1);
   DetLV_-> SetVisAttributes(va);
   G4Box* boxGas2= new G4Box(nmGas2, sizex/2., sizey/2., tgas2/2);
   G4LogicalVolume* Gas2LV= new G4LogicalVolume(boxGas2, mGas_, nmGas2);
   Gas2LV-> SetVisAttributes(va);


   G4Box* boxGEM= new G4Box(nmGEM, sizex/2., sizey/2., tgem/2.);
   G4LogicalVolume* GEMLV= new G4LogicalVolume(boxGEM, mKap_, nmGEM);
   GEMLV-> SetVisAttributes(va);
   G4Box* boxCu= new G4Box(nmCu, sizex/2., sizey/2., tcu/2.);
   G4LogicalVolume* CuLV= new G4LogicalVolume(boxCu, mCu_, nmCu);
   CuLV-> SetVisAttributes(va);
   for(G4int i=0; i<3; i++){
      new G4PVPlacement(zeRot, G4ThreeVector(0.,0.,
               zsign*(-tgas2/2.+ttgap*i+tgem*(i+1)/2.+tcu*(2*i+1))),
            GEMLV, nmGEM, Gas2LV, false, 3*i);
      new G4PVPlacement(zeRot, G4ThreeVector(0.,0.,
               zsign*(-tgas2/2.+ttgap*i+tgem*(i+1)/2.+tcu*(2*i+1)+tgem/2.+tcu/2.)),
            CuLV, nmCu, Gas2LV, false, 3*i+1);
      new G4PVPlacement(zeRot, G4ThreeVector(0.,0.,
               zsign*(-tgas2/2.+ttgap*i+tgem*(i+1)/2.+tcu*(2*i+1)-tgem/2.-tcu/2.)),
            CuLV, nmCu, Gas2LV, false, 3*i+2);
   }

   G4Box* boxCux= new G4Box(nmCux, sizex/2., sizey/2., tcux/2.);
   G4LogicalVolume* CuxLV= new G4LogicalVolume(boxCux, mCu_, nmCux);
   CuxLV-> SetVisAttributes(va);
   new G4PVPlacement(zeRot, G4ThreeVector(0.,0.,
            zsign*(tgas2/2.-tni-tcuy-tpadkap-tcux/2.)),
         CuxLV, nmCux, Gas2LV, false, 0);
   
   G4Box* boxPadKap= new G4Box(nmPadKap, sizex/2., sizey/2., tpadkap/2.);
   G4LogicalVolume* PadKapLV= new G4LogicalVolume(boxPadKap, mKap_, nmPadKap);
   PadKapLV-> SetVisAttributes(va);
   new G4PVPlacement(zeRot, G4ThreeVector(0.,0.,
            zsign*(tgas2/2.-tni-tcuy-tpadkap/2.)),
         PadKapLV, nmPadKap, Gas2LV, false, 0);
   
   G4Box* boxCuy= new G4Box(nmCuy, sizex/2., sizey/2., tcuy/2.);
   G4LogicalVolume* CuyLV= new G4LogicalVolume(boxCuy, mCu_, nmCuy);
   CuyLV-> SetVisAttributes(va);
   new G4PVPlacement(zeRot, G4ThreeVector(0.,0.,
            zsign*(tgas2/2.-tni-tcuy/2.)),
         CuyLV, nmCuy, Gas2LV, false, 0);
   
   G4Box* boxNi= new G4Box(nmNi, sizex/2., sizey/2., tni/2.);
   G4LogicalVolume* NiLV= new G4LogicalVolume(boxNi, mNi_, nmNi);
   NiLV-> SetVisAttributes(va);
   new G4PVPlacement(zeRot, G4ThreeVector(0.,0.,
            zsign*(tgas2/2.-tni/2.)),
         NiLV, nmNi, Gas2LV, false, 0);
   
   /*G4Box* boxEpox= new G4Box(nmEpox, size/2., size/2., tepox/2.);
   G4LogicalVolume* EpoxLV= new G4LogicalVolume(boxEpox, mEpox_, nmEpox);
   EpoxLV-> SetVisAttributes(va);
   new G4PVPlacement(zeRot, G4ThreeVector(0.,0.,
            tgas2/2.-tepox/2.),
         EpoxLV, nmEpox, Gas2LV, false, 0);*/

   //G4ThreeVector posGas1(0,0,tmylar+tal);
   G4ThreeVector posGas1(0,0,0);
   posGas1+=gPos_;
   G4ThreeVector posGas2(0,0,zsign*(tgas2/2.+tgas1/2.));
   posGas2*=(*rot);
   posGas2+=gPos_;
   /*G4VPhysicalVolume *gemPV = new G4PVPlacement
      (&rotMtx_, gPos_, DetLV_, nmBox, 
       pMother, false, Id_);*/
   G4VPhysicalVolume *gemPV = new G4PVPlacement
      (&rotMtx_, posGas1, DetLV_, nmGas1, 
       pMother, false, Id_);
   new G4PVPlacement
      (&rotMtx_, posGas2, Gas2LV, nmGas2, pMother, false, Id_);
   
   G4ThreeVector posMylar(0,0,zsign*(-tgas1/2.-tmylar/2.-tal));
   posMylar*=(*rot);
   posMylar+=gPos_;
   G4Box* boxMylar= new G4Box(nmMylar, sizex/2., sizey/2., tmylar/2.);
   G4LogicalVolume* MylarLV= new G4LogicalVolume(boxMylar, mMylar_, nmMylar);
   va= new G4VisAttributes(G4Color(1.,1.,1.));
   MylarLV-> SetVisAttributes(va);
   new G4PVPlacement
      (&rotMtx_, posMylar, MylarLV, nmMylar, pMother, false, 0);
   G4Box* boxAl= new G4Box(nmAl, sizex/2., sizey/2., tal/2.);
   G4LogicalVolume* AlLV= new G4LogicalVolume(boxAl, mAl_, nmAl);
   AlLV-> SetVisAttributes(va);
   G4ThreeVector posAl1(0,0,zsign*(-tgas1/2.-tal/2.));
   posAl1*=(*rot);
   posAl1+=gPos_;
   new G4PVPlacement
      (&rotMtx_, posAl1, AlLV, nmAl, pMother, false, 0);
   G4ThreeVector posAl2(0,0,zsign*(-tgas1/2.-tmylar-tal*3/2.));
   posAl2*=(*rot);
   posAl2+=gPos_;
   new G4PVPlacement
      (&rotMtx_, posAl2, AlLV, nmAl, pMother, false, 1);
   
   //GEM Frame 
   G4Box* frameDriftSpacer_tmp1= new G4Box(nmFrameDriftSpacer, 
         sizex/2.+framesize, sizey/2.+framesize, tframedriftspacer/2.);
   G4Box* frameDriftSpacer_tmp2= new G4Box(nmFrameDriftSpacer, 
         sizex/2., sizey/2., tframedriftspacer/2.+1*mm);
   G4SubtractionSolid *frameDriftSpacer= new
   G4SubtractionSolid(nmFrameDriftSpacer,frameDriftSpacer_tmp1,frameDriftSpacer_tmp2);
   frameDriftSpacerLV_= new G4LogicalVolume(frameDriftSpacer, 
         mAcryl_, nmFrameDriftSpacer);
   frameDriftSpacerLV_-> SetVisAttributes(va);
   G4ThreeVector posFrameDriftSpacer(0,0,zsign*-tframedriftspacer/2.);
   posFrameDriftSpacer*=(*rot);
   posFrameDriftSpacer+=gPos_;
   new G4PVPlacement
      (&rotMtx_, posFrameDriftSpacer, frameDriftSpacerLV_, 
       nmFrameDriftSpacer, pMother, false, 0);
   
   G4Box* frameSeal_tmp1= new G4Box(nmFrameSeal, 
         sizex/2.+framesize, sizey/2.+framesize, tframeseal/2.);
   G4Box* frameSeal_tmp2= new G4Box(nmFrameSeal, 
         sizex/2., sizey/2., tframeseal/2.+1*mm);
   G4SubtractionSolid *frameSeal= new
   G4SubtractionSolid(nmFrameSeal,frameSeal_tmp1,frameSeal_tmp2);
   G4LogicalVolume* frameSealLV= new G4LogicalVolume(frameSeal, mAcryl_, nmFrameSeal);
   frameSealLV-> SetVisAttributes(va);
   
   G4Box* frameGEM_tmp1= new G4Box(nmFrameGEM, 
         sizex/2.+framesize, sizey/2.+framesize, tframegem/2.);
   G4Box* frameGEM_tmp2= new G4Box(nmFrameGEM, 
         sizex/2., sizey/2., tframegem/2.+1*mm);
   G4SubtractionSolid *frameGEM= new
   G4SubtractionSolid(nmFrameGEM,frameGEM_tmp1,frameGEM_tmp2);
   frameGEMLV_= new G4LogicalVolume(frameGEM, mAcryl_, nmFrameGEM);
   frameGEMLV_-> SetVisAttributes(va);
   for (int i=0;i<4;i++){
      G4ThreeVector posFrameSeal(0,0,zsign*(tframeseal*(i+0.5)+tframegem*i));
      posFrameSeal*=(*rot);
      posFrameSeal+=gPos_;
      new G4PVPlacement
         (&rotMtx_, posFrameSeal, frameSealLV, nmFrameSeal, pMother, false, 0);
      if (i<3){
         G4ThreeVector posFrameGEM(0,0,zsign*(tframeseal*(i+1)+tframegem*(i+0.5)));
         posFrameGEM*=(*rot);
         posFrameGEM+=gPos_;
         new G4PVPlacement
            (&rotMtx_, posFrameGEM, frameGEMLV_, nmFrameGEM, pMother, false, 0);
      }
   }
   
   G4Box* frameReadOut_tmp1= new G4Box(nmFrameReadOut, 
         sizex/2.+framesize, sizey/2.+framesize, tframereadout/2.);
   G4Box* frameReadOut_tmp2= new G4Box(nmFrameReadOut, 
         sizex/2., sizey/2., tframereadout/2.+1*mm);
   G4SubtractionSolid *frameReadOut= new
   G4SubtractionSolid(nmFrameReadOut,frameReadOut_tmp1,frameReadOut_tmp2);
   frameReadOutLV_= new G4LogicalVolume(frameReadOut, 
         mAcryl_, nmFrameReadOut);
   frameReadOutLV_-> SetVisAttributes(va);
   G4ThreeVector posFrameReadOut(0,0,
         zsign*(tframeseal*4+tframegem*3+tframereadout/2.));
   posFrameReadOut*=(*rot);
   posFrameReadOut+=gPos_;
   new G4PVPlacement
      (&rotMtx_, posFrameReadOut, frameReadOutLV_, 
       nmFrameReadOut, pMother, false, 0);
   
   G4Box* frameAlBase_tmp1= new G4Box(nmFrameAlBase, 
         sizex/2.+framesize, sizey/2.+framesize, tframealbase/2.);
   G4Box* frameAlBase_tmp2= new G4Box(nmFrameAlBase, 
         sizex/2., sizey/2., tframealbase/2.+1*mm);
   G4SubtractionSolid *frameAlBase= new
   G4SubtractionSolid(nmFrameAlBase,frameAlBase_tmp1,frameAlBase_tmp2);
   frameAlBaseLV_= new G4LogicalVolume(frameAlBase, 
         mAl_, nmFrameAlBase);
   frameAlBaseLV_-> SetVisAttributes(va);
   G4ThreeVector posFrameAlBase(0,0,
         zsign*(tframeseal*4+tframegem*3+tframereadout+tframealbase/2.));
   posFrameAlBase*=(*rot);
   posFrameAlBase+=gPos_;
   new G4PVPlacement
      (&rotMtx_, posFrameAlBase, frameAlBaseLV_, 
       nmFrameAlBase, pMother, false, 0);
   
      G4Box* poleCFRP_tmp1= new G4Box(nmPoleCFRP, 
            //sizex/2.+cfrpframesize, polelength/2., tframecfrp/2.);
            sizex/2.+framesize, polelength/2., tframecfrp/2.);
      G4Box* poleCFRP_tmp2= new G4Box(nmPoleCFRP, 
            sizex/2., polelength/2.+0.1*mm, tframecfrp/2.+0.1*mm);
      G4SubtractionSolid *poleCFRP= new
         G4SubtractionSolid(nmPoleCFRP,poleCFRP_tmp1,poleCFRP_tmp2);
      frameCFRPLV_= new G4LogicalVolume(poleCFRP, 
            mCFRP_, nmPoleCFRP);
      frameCFRPLV_-> SetVisAttributes(va);
      G4ThreeVector posPoleCFRP(0,0,
            zsign*(tframeseal*4+tframegem*3+tframereadout+tframealbase
            +tframecfrp/2.));
            //+tframecfrp+tframecfrp/2.);
      posPoleCFRP*=(*rot);
      posPoleCFRP+=gPos_;
   if(poleflag){//Place poles
      new G4PVPlacement(&rotMtx_, posPoleCFRP, frameCFRPLV_, 
            nmPoleCFRP, pMother, false, 0);
   }
   return gemPV;
}

//////////////////////////////////////////////////////////////
G4VPhysicalVolume *E16G4_GTRGEM::PlaceSQ_Epoxy( G4LogicalVolume *pMother )
   //////////////////////////////////////////////////////////////
{
   G4double sizex= 0;
   G4double sizey= 0;
   G4double size1x= 10.*cm;
   G4double size2x= 20.*cm;
   G4double size3x= 30.*cm;
   G4double size1y= 10.*cm;
   G4double size2y= 20.*cm;
   G4double size3y= 30.*cm;
   //sako
   G4double size4x= 90.*cm;
   G4double size4y= 90.*cm;
   //sako
   G4double sizeACx= 60.*cm;
   G4double sizeACy= 60.*cm;
   //sako
   G4double sizeTSCx= 40.*cm;
   G4double sizeTSCy= 40.*cm;

   G4double framesize= 0.78*cm;
   //G4double cfrpframesize= 1.36*cm;//from 5segment.itp
   G4double cfrpframesize= 0.78*cm;//from 5segment.itp
   //thickness
   G4double tmylar=25.*um;
   G4double tal=1.*um;
   G4double tgem=50.*um*(1-0.0489);
   G4double tcu=4.*um*(1-0.195);
   G4double tdgap=6.*mm;
   G4double ttgap=2.2*mm;
   G4double tcux=2.*um*(1-0.80);
   G4double tcuy=2.*um*(1-0.171);
   G4double tpadkap=25.*um*(1-0.8);
   G4double tni=2.*um*(1-0.171);
   G4double tepox=100.*um;
   G4double tgas1=tdgap;
   G4double tgas2=tepox+tni+tpadkap+tcuy+tcux+ttgap*3+tgem*3+tcu*3;
   G4double tframedriftspacer=6.4*mm;
   G4double tframereadout=1.7*mm;
   G4double tframegem=1.7*mm;
   G4double tframeseal=0.5*mm;
   G4double tframealbase=3.*mm;
   G4double tframecfrp=tcfrp;
   G4double foiloffset=0;
   G4String nmGas1;
   G4String nmGas2;
   G4String nmWind;
   G4String nmMylar;
   G4String nmAl;
   G4String nmGEM;
   G4String nmCu;
   G4String nmCux;
   G4String nmCuy;
   G4String nmPadKap;
   G4String nmNi;
   G4String nmEpox;
   G4String nmFrameDriftSpacer;
   G4String nmFrameReadOut;
   G4String nmFrameGEM;
   G4String nmFrameSeal;
   G4String nmFrameAlBase;
   G4String nmFrameCFRP;
   G4String nmPoleCFRP;
   G4String nmCable;
   nmGas1=elemName_+"_DriftGap";
   nmGas2=elemName_+"_TransferGap";
   nmWind=elemName_+"_Window";
   nmMylar=elemName_+"_Mylar";
   nmAl=elemName_+"_Al";
   nmGEM=elemName_+"_GEM";
   nmCu=elemName_+"_Cu";
   nmCux=elemName_+"_Cux";
   nmCuy=elemName_+"_Cuy";
   nmPadKap=elemName_+"_PadKap";
   nmNi=elemName_+"_Ni";
   nmEpox=elemName_+"_Epox";
   nmFrameDriftSpacer=elemName_+"_DriftSpacerFrame";
   nmFrameReadOut=elemName_+"_ReadOutFrame";
   nmFrameGEM=elemName_+"_GEMFrame";
   nmFrameSeal=elemName_+"_GasSeal";
   nmFrameAlBase=elemName_+"_AlBase";
   nmFrameCFRP=elemName_+"_CFRP";
   nmPoleCFRP=elemName_+"_CFRPpole";
   nmCable=elemName_+"_Cable";
   G4bool poleflag;
   G4double polelength;

   G4RotationMatrix *rot= new G4RotationMatrix
      (-rotMtx_.phi(),rotMtx_.theta(),-rotMtx_.psi());
   G4RotationMatrix *zeRot= new G4RotationMatrix();
   zeRot->rotateY(0.*degree);
   G4VisAttributes* va;
   va= new G4VisAttributes(G4Color(1.,0.,0.));
   //va= new G4VisAttributes(false);

   sizex=size3x;
   sizey=size3y;
   //sako
   if ((Id_/3)==12) {//AC
     sizex = sizeACx;
     sizey = sizeACy;
     G4cout << "*AC Id_/3, sizex, sizey = " << Id_/3 << " " << sizex << " " << sizey << G4endl;
   } else if ((Id_/3)==14) {//TSC
     sizex = sizeTSCx;
     sizey = sizeTSCy;
     G4cout << "*TSC Id_/3, sizex, sizey = " << Id_/3 << " " << sizex << " " << sizey << G4endl;
   } else {
     //     G4cout << "*Id_, Id_/3 = " << Id_ << " " << Id_/3 << " " << sizex << G4endl;
   }

   //sako for temporary RPC
   //   if ((Id_==41)||(Id_==59)) {
   //     sizex=size4x;
   //     sizey=size4y;
   //   }
   foiloffset=0.25*cm;
   polelength=110.*cm;
   
   G4int zsign=1;
   if(int(int(Id_/3)/3)%2!=0) zsign=-1;
   //if Id_/3 == 12, (12/3)%2 = 0
   //if Id_/3 == 13, (13/3)%2 = 0
   //if Id_/3 == 13, (14/3)%2 = 0
   /*switch(Id_%3){
      case 0:
         sizex=size1x;
         sizey=size1y;
         foiloffset=0.05*cm;
         break;
      case 1:
         sizex=size2x;
         sizey=size2y;
         foiloffset=0.25*cm;
         break;
      default:
         sizex=size3x;
         sizey=size3y;
         foiloffset=0.25*cm;
         polelength=110.*cm;
         break;
   }*/
   
   if(int(Id_/3)%3==1)poleflag=true;
   else poleflag=false;
#ifdef RUN0E
   if (Id_/3==13) {//treat as poleflag == false
     poleflag=false;
   }
#endif

   // if Id_3 = 12, 12%3 == 0
   // if Id_3 = 13, 13%3 == 1
   // if Id_3 = 14, 14%3 == 2

   //Z-Center of GTR=Center of DriftGap 
   G4Box* boxGas1= new G4Box(nmGas1, sizex/2., sizey/2., tgas1/2);
   DetLV_= new G4LogicalVolume(boxGas1, mGas_, nmGas1);
   DetLV_-> SetVisAttributes(va);
   G4Box* boxGas2= new G4Box(nmGas2, sizex/2., sizey/2., tgas2/2);
   G4LogicalVolume* Gas2LV= new G4LogicalVolume(boxGas2, mGas_, nmGas2);
   Gas2LV-> SetVisAttributes(va);


   G4Box* boxGEM= new G4Box(nmGEM, sizex/2., sizey/2., tgem/2.);
   G4LogicalVolume* GEMLV= new G4LogicalVolume(boxGEM, mKap_, nmGEM);
   GEMLV-> SetVisAttributes(va);
   G4Box* boxCu= new G4Box(nmCu, sizex/2., sizey/2., tcu/2.);
   G4LogicalVolume* CuLV= new G4LogicalVolume(boxCu, mCu_, nmCu);
   CuLV-> SetVisAttributes(va);
   for(G4int i=0; i<3; i++){
      new G4PVPlacement(zeRot, G4ThreeVector(0.,0.,
               zsign*(-tgas2/2.+ttgap*i+tgem*(i+1)/2.+tcu*(2*i+1))),
            GEMLV, nmGEM, Gas2LV, false, 3*i);
      new G4PVPlacement(zeRot, G4ThreeVector(0.,0.,
               zsign*(-tgas2/2.+ttgap*i+tgem*(i+1)/2.+tcu*(2*i+1)+tgem/2.+tcu/2.)),
            CuLV, nmCu, Gas2LV, false, 3*i+1);
      new G4PVPlacement(zeRot, G4ThreeVector(0.,0.,
               zsign*(-tgas2/2.+ttgap*i+tgem*(i+1)/2.+tcu*(2*i+1)-tgem/2.-tcu/2.)),
            CuLV, nmCu, Gas2LV, false, 3*i+2);
   }

   G4Box* boxCux= new G4Box(nmCux, sizex/2., sizey/2., tcux/2.);
   G4LogicalVolume* CuxLV= new G4LogicalVolume(boxCux, mCu_, nmCux);
   CuxLV-> SetVisAttributes(va);
   new G4PVPlacement(zeRot, G4ThreeVector(0.,0.,
            zsign*(tgas2/2.-tepox-tni-tcuy-tpadkap-tcux/2.)),
         CuxLV, nmCux, Gas2LV, false, 0);

   G4Box* boxPadKap= new G4Box(nmPadKap, sizex/2., sizey/2., tpadkap/2.);
   G4LogicalVolume* PadKapLV= new G4LogicalVolume(boxPadKap, mKap_, nmPadKap);
   PadKapLV-> SetVisAttributes(va);
   new G4PVPlacement(zeRot, G4ThreeVector(0.,0.,
            zsign*(tgas2/2.-tepox-tni-tcuy-tpadkap/2.)),
         PadKapLV, nmPadKap, Gas2LV, false, 0);

   G4Box* boxCuy= new G4Box(nmCuy, sizex/2., sizey/2., tcuy/2.);
   G4LogicalVolume* CuyLV= new G4LogicalVolume(boxCuy, mCu_, nmCuy);
   CuyLV-> SetVisAttributes(va);
   new G4PVPlacement(zeRot, G4ThreeVector(0.,0.,
            zsign*(tgas2/2.-tepox-tni-tcuy/2.)),
         CuyLV, nmCuy, Gas2LV, false, 0);

   G4Box* boxNi= new G4Box(nmNi, sizex/2., sizey/2., tni/2.);
   G4LogicalVolume* NiLV= new G4LogicalVolume(boxNi, mNi_, nmNi);
   NiLV-> SetVisAttributes(va);
   new G4PVPlacement(zeRot, G4ThreeVector(0.,0.,
            zsign*(tgas2/2.-tepox-tni/2.)),
         NiLV, nmNi, Gas2LV, false, 0);

   G4Box* boxEpox= new G4Box(nmEpox, sizex/2., sizey/2., tepox/2.);
   G4LogicalVolume* EpoxLV= new G4LogicalVolume(boxEpox, mEpox_, nmEpox);
   EpoxLV-> SetVisAttributes(va);
   new G4PVPlacement(zeRot, G4ThreeVector(0.,0.,
            zsign*(tgas2/2.-tepox/2.)),
         EpoxLV, nmEpox, Gas2LV, false, 0);

   //G4ThreeVector posGas1(0,0,tmylar+tal);
   G4ThreeVector posGas1(0,0,0);
   posGas1+=gPos_;
   G4ThreeVector posGas2(0,0,zsign*(tgas2/2.+tgas1/2.));
   posGas2*=(*rot);
   posGas2+=gPos_;
   /*G4VPhysicalVolume *gemPV = new G4PVPlacement
     (&rotMtx_, gPos_, DetLV_, nmBox, 
     pMother, false, Id_);*/
   G4VPhysicalVolume *gemPV = new G4PVPlacement
      (&rotMtx_, posGas1, DetLV_, nmGas1, 
       pMother, false, Id_);
   new G4PVPlacement
      (&rotMtx_, posGas2, Gas2LV, nmGas2, pMother, false, Id_);

   G4ThreeVector posMylar(0,0,zsign*(-tgas1/2.-tmylar/2.-tal));
   posMylar*=(*rot);
   posMylar+=gPos_;
   G4Box* boxMylar= new G4Box(nmMylar, sizex/2., sizey/2., tmylar/2.);
   G4LogicalVolume* MylarLV= new G4LogicalVolume(boxMylar, mMylar_, nmMylar);
   va= new G4VisAttributes(G4Color(1.,1.,1.));
   MylarLV-> SetVisAttributes(va);
   new G4PVPlacement
      (&rotMtx_, posMylar, MylarLV, nmMylar, pMother, false, 0);
   //new G4PVPlacement(zeRot, G4ThreeVector(0.,0.,-tgas1/2.+tmylar/2.+tal/2.), 
   //MylarLV, nmMylar, pMother, false, 0);
   G4Box* boxAl= new G4Box(nmAl, sizex/2., sizey/2., tal/2.);
   G4LogicalVolume* AlLV= new G4LogicalVolume(boxAl, mAl_, nmAl);
   AlLV-> SetVisAttributes(va);
   G4ThreeVector posAl1(0,0,zsign*(-tgas1/2.-tal/2.));
   posAl1*=(*rot);
   posAl1+=gPos_;
   new G4PVPlacement
      (&rotMtx_, posAl1, AlLV, nmAl, pMother, false, 0);
   G4ThreeVector posAl2(0,0,zsign*(-tgas1/2.-tmylar-tal*3/2.));
   posAl2*=(*rot);
   posAl2+=gPos_;
   new G4PVPlacement
      (&rotMtx_, posAl2, AlLV, nmAl, pMother, false, 1);
   //new G4PVPlacement(zeRot, G4ThreeVector(0.,0.,-tgas1/2.+tal/2.), 
   //    AlLV, nmAl, pMother, false, 0);
   //new G4PVPlacement(zeRot, G4ThreeVector(0.,0.,-tgas1/2.+tmylar+tal/2.), 
   //    AlLV, nmAl, pMother, false, 1);

   //GEM Frame
   // Geometory 
   // ------------
   // -driftspacer 6.4mm
   // ------------
   // -Seal 0.5mm 
   // ------------
   // -FrameGEM 1.7mm
   // ------------
   // -Seal 0.5mm 
   // ------------
   // -FrameGEM 1.7mm
   // ------------
   // -Seal 0.5mm 
   // ------------
   // -FrameGEM 1.7mm
   // ------------
   // -Seal 0.5mm 
   // ------------
   // -FrameReadout 1.7mm
   // ------------
   // -AL Base 3mm
   // ------------
   // -CFRP 4mm
   // ------------
   G4Box* frameDriftSpacer_tmp1= new G4Box(nmFrameDriftSpacer, 
         sizex/2.+framesize, sizey/2.+framesize, tframedriftspacer/2.);
   G4Box* frameDriftSpacer_tmp2= new G4Box(nmFrameDriftSpacer, 
         sizex/2., sizey/2., tframedriftspacer/2.+0.1*mm);
   G4SubtractionSolid *frameDriftSpacer= new
      G4SubtractionSolid(nmFrameDriftSpacer,frameDriftSpacer_tmp1,frameDriftSpacer_tmp2);
   frameDriftSpacerLV_= new G4LogicalVolume(frameDriftSpacer, 
         mEpox_, nmFrameDriftSpacer);
   frameDriftSpacerLV_-> SetVisAttributes(va);
   G4ThreeVector posFrameDriftSpacer(0,0,zsign*-tframedriftspacer/2.);
   posFrameDriftSpacer*=(*rot);
   posFrameDriftSpacer+=gPos_;
   new G4PVPlacement
      (&rotMtx_, posFrameDriftSpacer, frameDriftSpacerLV_, 
       nmFrameDriftSpacer, pMother, false, 0);

   G4Box* frameSeal_tmp1= new G4Box(nmFrameSeal, 
         sizex/2.+framesize, sizey/2.+framesize, tframeseal/2.);
   G4Box* frameSeal_tmp2= new G4Box(nmFrameSeal, 
         sizex/2., sizey/2., tframeseal/2.+0.1*mm);
   G4SubtractionSolid *frameSeal= new
      G4SubtractionSolid(nmFrameSeal,frameSeal_tmp1,frameSeal_tmp2);
   G4LogicalVolume* frameSealLV= new G4LogicalVolume(frameSeal, mAcryl_, nmFrameSeal);
   frameSealLV-> SetVisAttributes(va);

   G4Box* frameGEM_tmp1= new G4Box(nmFrameGEM, 
         sizex/2.+framesize, sizey/2.+framesize, tframegem/2.);
   G4Box* frameGEM_tmp2= new G4Box(nmFrameGEM, 
         sizex/2., sizey/2., tframegem/2.+0.1*mm);
   G4SubtractionSolid *frameGEM= new
      G4SubtractionSolid(nmFrameGEM,frameGEM_tmp1,frameGEM_tmp2);
   frameGEMLV_= new G4LogicalVolume(frameGEM, mEpox_, nmFrameGEM);
   frameGEMLV_-> SetVisAttributes(va);
   for (int i=0;i<4;i++){
      G4ThreeVector posFrameSeal(0,0,zsign*(tframeseal*(i+0.5)+tframegem*i));
      posFrameSeal*=(*rot);
      posFrameSeal+=gPos_;
      new G4PVPlacement
         (&rotMtx_, posFrameSeal, frameSealLV, nmFrameSeal, pMother, false, 0);
      if (i<3){
         G4ThreeVector posFrameGEM(0,0,zsign*(tframeseal*(i+1)+tframegem*(i+0.5)));
         posFrameGEM*=(*rot);
         posFrameGEM+=gPos_;
         new G4PVPlacement
            (&rotMtx_, posFrameGEM, frameGEMLV_, nmFrameGEM, pMother, false, 0);
      }
   }

   G4Box* frameReadOut_tmp1= new G4Box(nmFrameReadOut, 
         sizex/2.+framesize, sizey/2.+framesize, tframereadout/2.);
   G4Box* frameReadOut_tmp2= new G4Box(nmFrameReadOut, 
         sizex/2., sizey/2., tframereadout/2.+0.1*mm);
   G4SubtractionSolid *frameReadOut= new
      G4SubtractionSolid(nmFrameReadOut,frameReadOut_tmp1,frameReadOut_tmp2);
   frameReadOutLV_= new G4LogicalVolume(frameReadOut, 
         mAcryl_, nmFrameReadOut);
   frameReadOutLV_-> SetVisAttributes(va);
   G4ThreeVector posFrameReadOut(0,0,
         zsign*(tframeseal*4+tframegem*3+tframereadout/2.));
   posFrameReadOut*=(*rot);
   posFrameReadOut+=gPos_;
   new G4PVPlacement
      (&rotMtx_, posFrameReadOut, frameReadOutLV_, 
       nmFrameReadOut, pMother, false, 0);

   G4Box* frameAlBase_tmp1= new G4Box(nmFrameAlBase, 
         sizex/2.+framesize, sizey/2.+framesize, tframealbase/2.);
   G4Box* frameAlBase_tmp2= new G4Box(nmFrameAlBase, 
         sizex/2., sizey/2., tframealbase/2.+0.1*mm);
   G4SubtractionSolid *frameAlBase= new
      G4SubtractionSolid(nmFrameAlBase,frameAlBase_tmp1,frameAlBase_tmp2);
   frameAlBaseLV_= new G4LogicalVolume(frameAlBase, 
         mAl_, nmFrameAlBase);
   frameAlBaseLV_-> SetVisAttributes(va);
   G4ThreeVector posFrameAlBase(0,0,
         zsign*(tframeseal*4+tframegem*3+tframereadout+tframealbase/2.));
   posFrameAlBase*=(*rot);
   posFrameAlBase+=gPos_;
   new G4PVPlacement
      (&rotMtx_, posFrameAlBase, frameAlBaseLV_, 
       nmFrameAlBase, pMother, false, 0);

   /*G4Box* frameCFRP_tmp1= new G4Box(nmFrameCFRP, 
         sizex/2.+cfrpframesize, sizey/2.+cfrpframesize, tframecfrp/2.);
   G4Box* frameCFRP_tmp2= new G4Box(nmFrameCFRP, 
         sizex/2., sizey/2., tframecfrp/2.);
   G4SubtractionSolid *frameCFRP= new
      G4SubtractionSolid(nmFrameCFRP,frameCFRP_tmp1,frameCFRP_tmp2);
   G4LogicalVolume* frameCFRPLV= new G4LogicalVolume(frameCFRP, 
         mCFRP_, nmFrameCFRP);
   frameCFRPLV-> SetVisAttributes(va);
   G4ThreeVector posFrameCFRP(0,0,
         tframeseal*4+tframegem*3+tframereadout+tframealbase+tframecfrp/2.);
   posFrameCFRP*=(*rot);
   posFrameCFRP+=gPos_;
   new G4PVPlacement
      (&rotMtx_, posFrameCFRP, frameCFRPLV, 
       nmFrameCFRP, pMother, false, 0);*/
   
   if(poleflag){//Place poles
      G4Box* poleCFRP_tmp1= new G4Box(nmPoleCFRP, 
            sizex/2.+cfrpframesize, polelength/2., tframecfrp/2.);
      G4Box* poleCFRP_tmp2= new G4Box(nmPoleCFRP, 
            sizex/2., polelength/2.+0.1*mm, tframecfrp/2.+0.1*mm);
      G4SubtractionSolid *poleCFRP= new
         G4SubtractionSolid(nmPoleCFRP,poleCFRP_tmp1,poleCFRP_tmp2);
      frameCFRPLV_= new G4LogicalVolume(poleCFRP, 
            mCFRP_, nmPoleCFRP);
      frameCFRPLV_-> SetVisAttributes(va);
      G4ThreeVector posPoleCFRP(0,0,
            zsign*(tframeseal*4+tframegem*3+tframereadout+tframealbase
            +tframecfrp/2.));
            //+tframecfrp+tframecfrp/2.);
      posPoleCFRP*=(*rot);
      posPoleCFRP+=gPos_;
      new G4PVPlacement(&rotMtx_, posPoleCFRP, frameCFRPLV_, 
            nmPoleCFRP, pMother, false, 0);
   }


   return gemPV;
}

//////////////////////////////////////////////////////////////
void E16G4_GTRGEM::PlaceFrame( G4LogicalVolume *pMother )
//////////////////////////////////////////////////////////////
{
   G4double xfront1 = 6.*cm;
   G4double xrear1 = 18.*cm;
   G4double z1,z2;
   //G4double z1 = 53.1461*cm;
   //G4double z1 = 48.6289*cm;
   G4double tcfrp = 4*mm;
   G4double ypos1,ypos2,zpos=0*cm;
   G4String nmAFrame1;
   nmAFrame1=elemName_+"_AFrame";
   G4VisAttributes* va;
   va= new G4VisAttributes(G4Color(.5,0.5,0.5));
   G4double angle1,angle2;
   switch(int(Id_/3)%3){
      case 0:
         switch(int(int(Id_/3)/3)%2){
            case 1://far (typeB)
               angle1=-17.*degree;
               angle2=-39.*degree;
               zpos=0*cm;
               ypos1=11*cm;
               ypos2=-12.5*cm;
               z1=41.64*cm;
               z2=52.26*cm;
               break;
            default://near (typeA)
               angle1=-21.*degree;
               angle2=-42.*degree;
               zpos=0*cm;
               ypos1=11*cm;
               ypos2=-11*cm;
               z1=41.74*cm;
               z2=51.44*cm;
               break;
         }
         break;
      case 1:
         switch(int(int(Id_/3)/3)%2){
            case 1:
               angle1=14.*degree;
               angle2=-14.*degree;
               zpos=0*cm;
               ypos1=11*cm;
               ypos2=-11*cm;
               z1=41.*cm;
               z2=41.*cm;
               break;
            default://near (typeA)
               angle1=14.*degree;
               angle2=-14.*degree;
               zpos=-0*cm;
               ypos1=11*cm;
               ypos2=-11*cm;
               z1=41.*cm;
               z2=41.*cm;
               break;
         }
         break;
      default: // case 2
         switch(int(int(Id_/3)/3)%2){
            case 1:
               angle1=39*degree;
               angle2=17*degree;
               zpos=0*cm;
               ypos1=12.5*cm;
               ypos2=-11*cm;
               z1=52.26*cm;
               z2=41.64*cm;
               break;
            default://near (typeA)
               angle1=42*degree;
               angle2=21.*degree;
               ypos1=11*cm;
               ypos2=-11*cm;
               z1=51.44*cm;
               z2=41.74*cm;
               zpos=0*cm;
               break;
         }
         /*angle1=40.06*degree;
         angle2=19.55*degree;
         //ypos1=11*cm;
         //ypos2=-11*cm;*/
         break;
   }
#ifdef RUN0E
   if (Id_/3==13) {//treat as case 2
     switch(int(int(Id_/3)/3)%2){
     case 1:
       angle1=39*degree;
       angle2=17*degree;
       zpos=0*cm;
       ypos1=12.5*cm;
       ypos2=-11*cm;
       z1=52.26*cm;
       z2=41.64*cm;
       break;
     default://near (typeA)
       angle1=42*degree;
       angle2=21.*degree;
       ypos1=11*cm;
       ypos2=-11*cm;
       z1=51.44*cm;
       z2=41.74*cm;
       zpos=0*cm;
       break;
     }
   }
#endif

   G4RotationMatrix *rotpos= new G4RotationMatrix
      (-rotMtx_.phi(),rotMtx_.theta(),-rotMtx_.psi());
   G4RotationMatrix *rotint1= new G4RotationMatrix
      (rotMtx_.phi(),rotMtx_.theta(),rotMtx_.psi());
   rotint1->rotateX(angle1);
   G4RotationMatrix *rotint2= new G4RotationMatrix
      (rotMtx_.phi(),rotMtx_.theta(),rotMtx_.psi());
   rotint2->rotateX(angle2);

   G4Trd* AFrame1 = new G4Trd(nmAFrame1, xfront1/2., xrear1/2., 
         tcfrp/2.,tcfrp/2.,z1/2.);
   G4LogicalVolume* AFrame1LV= new G4LogicalVolume(AFrame1, 
         mCFRP_, nmAFrame1);
   AFrame1LV-> SetVisAttributes(va);
   
   G4Trd* AFrame2 = new G4Trd(nmAFrame1, xfront1/2., xrear1/2., 
         tcfrp/2.,tcfrp/2.,z2/2.);
   G4LogicalVolume* AFrame2LV= new G4LogicalVolume(AFrame2, 
         mCFRP_, nmAFrame1);
   AFrame2LV-> SetVisAttributes(va);

   G4ThreeVector posAFrame1(0,ypos1+tcfrp/2.,zpos);
   posAFrame1*=(*rotpos);
   posAFrame1+=gPos_;
   G4ThreeVector posAFrame2(0,ypos2-tcfrp/2.,zpos);
   posAFrame2*=(*rotpos);
   posAFrame2+=gPos_;
   new G4PVPlacement(rotint1, posAFrame1, AFrame1LV, 
         nmAFrame1, pMother, false, 0);
   new G4PVPlacement(rotint2, posAFrame2, AFrame2LV, 
         nmAFrame1, pMother, false, 0);

}

//////////////////////////////////////////////////////////////
G4VPhysicalVolume *E16G4_GTRGEM::PlaceTRD( G4LogicalVolume *pMother, G4int TorB )
   //////////////////////////////////////////////////////////////
{
   G4double aaa= 0;
   G4double aaa1= 7.2973*cm;
   G4double aaa2= 12.7239*cm;
   G4double aaa3= 15.8054*cm;
   G4double bbb= 0;
   G4double bbb1= 6.2263*cm;
   G4double bbb2= 11.6529*cm;
   G4double bbb3= 14.7344*cm;
   G4double ccc= 0;
   G4double ccc1= 12.0539*cm;
   G4double ccc2= 22.4711*cm;
   G4double ccc3= 27.8700*cm;
   G4double ddd= 0;
   G4double ddd1= 13.8839*cm;
   G4double ddd2= 24.3011*cm;
   G4double ddd3= 31.7000*cm;
   G4double eee= 0;
   G4double eee1= 10.7497*cm;
   G4double eee2= 19.9555*cm;
   G4double eee3= 27.9195*cm;
   G4double fff= 0;
   G4double fff1= 12.1497*cm;
   G4double fff2= 21.3555*cm;
   G4double fff3= 29.3195*cm;
   G4double framesize= 0;
   G4double framesize1= 1.2*cm;
   G4double framesize2= 1.2*cm;
   G4double framesize3= 1.2*cm;
   //thickness
   G4double tmylar=25.*um;
   G4double tal=1.*um;
   G4double tgem=50.*um;
   G4double tcu=4.*um;
   G4double tdgap=6.*mm;
   G4double ttgap=2.2*mm;
   G4double tcux=4.*um;
   G4double tcuy=4.*um;
   G4double tpadkap=25.*um;
   G4double tni=2.*um;
   G4double tepox=100.*um;
   G4double tgas1=tdgap;
   //G4double tgas2=tni+tpadkap+tcuy+tcux+ttgap*3;
   G4double tgas2=tpadkap+tcuy+tcux+ttgap*3;
   G4double tframedriftspacer=6.4*mm;
   G4double tframereadout=1.7*mm;
   G4double tframegem=1.7*mm;
   G4double tframeseal=0.5*mm;
   G4double tframealbase=3.*mm;
   G4double tframecfrp=tcfrp;
   G4double foiloffset=0;
   G4String nmGas1;
   G4String nmGas2;
   G4String nmWind;
   G4String nmMylar;
   G4String nmAl;
   G4String nmGEM;
   G4String nmCu;
   G4String nmCux;
   G4String nmCuy;
   G4String nmPadKap;
   G4String nmNi;
   G4String nmEpox;
   G4String nmFrameDriftSpacer;
   G4String nmFrameReadOut;
   G4String nmFrameGEM;
   G4String nmFrameSeal;
   G4String nmFrameAlBase;
   G4String nmFrameCFRP;
   nmGas1=elemName_+"_DriftGap";
   nmGas2=elemName_+"_TransferGap";
   nmWind=elemName_+"_Window";
   nmMylar=elemName_+"_Mylar";
   nmAl=elemName_+"_Al";
   nmGEM=elemName_+"_GEM";
   nmCu=elemName_+"_Cu";
   nmCux=elemName_+"_Cux";
   nmCuy=elemName_+"_Cuy";
   nmPadKap=elemName_+"_PadKap";
   nmNi=elemName_+"_Ni";
   nmEpox=elemName_+"_Epox";
   nmFrameDriftSpacer=elemName_+"_DriftSpacerFrame";
   nmFrameReadOut=elemName_+"_ReadOutFrame";
   nmFrameGEM=elemName_+"_GEMFrame";
   nmFrameSeal=elemName_+"_GasSeal";
   nmFrameAlBase=elemName_+"_AlBase";
   nmFrameCFRP=elemName_+"_CFRP";

   G4RotationMatrix *zeRot= new G4RotationMatrix();
   zeRot->rotateY(0.*degree);
   G4RotationMatrix *rot90= new G4RotationMatrix();
   if (TorB==1)rot90->rotateX(-90.*degree);
   else if (TorB==0)rot90->rotateX(-90.*degree);
   else{
      G4cout <<"TorB flag must be 0 or 1!"<<G4endl;
   }
   G4VisAttributes* va;
   va= new G4VisAttributes(G4Color(1.,0.,0.));
   //va= new G4VisAttributes(false);



   switch(Id_%3){
      case 0:
         aaa=aaa1;
         bbb=bbb1;
         ccc=ccc1;
         ddd=ddd1;
         eee=eee1;
         fff=fff1;
         framesize=framesize1;
         break;
      case 1:
         aaa=aaa2;
         bbb=bbb2;
         ccc=ccc2;
         ddd=ddd2;
         eee=eee2;
         fff=fff2;
         framesize=framesize2;
         break;
      default:
         aaa=aaa3;
         bbb=bbb3;
         ccc=ccc3;
         ddd=ddd3;
         eee=eee3;
         fff=fff3;
         framesize=framesize3;
         break;
   }

   G4Trd* Gas1_pre= new G4Trd(nmGas1, bbb/2., ccc/2., tgas1/2.,tgas1/2.,eee/2.);
   G4DisplacedSolid *Gas1 = new G4DisplacedSolid( 
         nmGas1, Gas1_pre, rot90,
         G4ThreeVector(0.*cm,0.*cm,0.*cm));
   DetLV_= new G4LogicalVolume(Gas1, mGas_, nmGas1);
   DetLV_-> SetVisAttributes(va);
   //G4Box* boxGas2= new G4Box(nmGas2, size/2., size/2., tgas2/2);
   G4Trd* Gas2_pre= new G4Trd(nmGas2, bbb/2., ccc/2., tgas2/2.,tgas2/2.,eee/2.);
   G4DisplacedSolid *Gas2 = new G4DisplacedSolid( 
         nmGas2, Gas2_pre, rot90,
         G4ThreeVector(0.*cm,0.*cm,0.*cm));
   G4LogicalVolume* Gas2LV= new G4LogicalVolume(Gas2, mGas_, nmGas2);
   Gas2LV-> SetVisAttributes(va);


   //G4Trd* boxGEM= new G4Box(nmGEM, size/2., size/2., tgem/2.);
   G4Trd* GEM_pre= new G4Trd(nmGEM, bbb/2., ccc/2., tgem/2.,tgem/2.,eee/2.);
   G4DisplacedSolid *GEM = new G4DisplacedSolid( 
         nmGEM, GEM_pre, rot90,
         G4ThreeVector(0.*cm,0.*cm,0.*cm));
   G4LogicalVolume* GEMLV= new G4LogicalVolume(GEM, mKap_, nmGEM);
   GEMLV-> SetVisAttributes(va);
   //G4Box* boxCu= new G4Box(nmCu, size/2., size/2., tcu/2.);
   G4Trd* Cu_pre= new G4Trd(nmCu, bbb/2., ccc/2., tcu/2.,tcu/2.,eee/2.);
   G4DisplacedSolid *Cu = new G4DisplacedSolid( 
         nmCu, Cu_pre, rot90,
         G4ThreeVector(0.*cm,0.*cm,0.*cm));
   G4LogicalVolume* CuLV= new G4LogicalVolume(Cu, mCu_, nmCu);
   CuLV-> SetVisAttributes(va);
   for(G4int i=0; i<3; i++){
      new G4PVPlacement(zeRot, G4ThreeVector(0.,0.,
               -tgas2/2.+ttgap*i),
            GEMLV, nmGEM, Gas2LV, false, 3*i);
      new G4PVPlacement(zeRot, G4ThreeVector(0.,0.,
               -tgas2/2.+ttgap*i-tcu/2.),
            CuLV, nmCu, Gas2LV, false, 3*i+1);
      new G4PVPlacement(zeRot, G4ThreeVector(0.,0.,
               -tgas2/2.+ttgap*i+tcu/2.),
            CuLV, nmCu, Gas2LV, false, 3*i+2);
   }

   //G4Box* boxCux= new G4Box(nmCux, size/2., size/2., tcux/2.);
   G4Trd* Cux_pre= new G4Trd(nmCu, bbb/2., ccc/2., tcux/2.,tcux/2.,eee/2.);
   G4DisplacedSolid *Cux = new G4DisplacedSolid( 
         nmCux, Cux_pre, rot90,
         G4ThreeVector(0.*cm,0.*cm,0.*cm));
   G4LogicalVolume* CuxLV= new G4LogicalVolume(Cux, mCu_, nmCux);
   CuxLV-> SetVisAttributes(va);
   new G4PVPlacement(zeRot, G4ThreeVector(0.,0.,
            tgas2/2.-tepox-tni-tcuy-tpadkap-tcux/2.),
         CuxLV, nmCux, Gas2LV, false, 0);

   //G4Box* boxPadKap= new G4Box(nmPadKap, size/2., size/2., tpadkap/2.);
   G4Trd* PadKap_pre= new G4Trd(nmPadKap, bbb/2., ccc/2., tpadkap/2.,tpadkap/2.,eee/2.);
   G4DisplacedSolid *PadKap = new G4DisplacedSolid( 
         nmPadKap, PadKap_pre, rot90,
         G4ThreeVector(0.*cm,0.*cm,0.*cm));
   G4LogicalVolume* PadKapLV= new G4LogicalVolume(PadKap, mKap_, nmPadKap);
   PadKapLV-> SetVisAttributes(va);
   new G4PVPlacement(zeRot, G4ThreeVector(0.,0.,
            tgas2/2.-tni-tcuy-tpadkap/2.),
         PadKapLV, nmPadKap, Gas2LV, false, 0);

   //G4Box* boxCuy= new G4Box(nmCuy, size/2., size/2., tcuy/2.);
   G4Trd* Cuy_pre= new G4Trd(nmCu, bbb/2., ccc/2., tcuy/2.,tcuy/2.,eee/2.);
   G4DisplacedSolid *Cuy = new G4DisplacedSolid( 
         nmCuy, Cuy_pre, rot90,
         G4ThreeVector(0.*cm,0.*cm,0.*cm));
   G4LogicalVolume* CuyLV= new G4LogicalVolume(Cuy, mCu_, nmCuy);
   CuyLV-> SetVisAttributes(va);
   new G4PVPlacement(zeRot, G4ThreeVector(0.,0.,
            tgas2/2.-tni-tcuy/2.),
         CuyLV, nmCuy, Gas2LV, false, 0);

   //G4Box* boxNi= new G4Box(nmNi, size/2., size/2., tni/2.);
   G4Trd* Ni_pre= new G4Trd(nmNi, bbb/2., ccc/2., tni/2.,tni/2.,eee/2.);
   G4DisplacedSolid *Ni = new G4DisplacedSolid( 
         nmNi, Ni_pre, rot90,
         G4ThreeVector(0.*cm,0.*cm,0.*cm));
   G4LogicalVolume* NiLV= new G4LogicalVolume(Ni, mNi_, nmNi);
   NiLV-> SetVisAttributes(va);
   new G4PVPlacement(zeRot, G4ThreeVector(0.,0.,
            tgas2/2.-tni/2.),
         NiLV, nmNi, Gas2LV, false, 0);

   //G4Box* boxEpox= new G4Box(nmEpox, size/2., size/2., tepox/2.);
   /*G4Trd* Epox_pre= new G4Trd(nmEpox, bbb/2., ccc/2., tepox/2.,tepox/2.,eee/2.);
     G4DisplacedSolid *Epox = new G4DisplacedSolid( 
     nmEpox, Epox_pre, rot90,
     G4ThreeVector(0.*cm,0.*cm,0.*cm));
     G4LogicalVolume* EpoxLV= new G4LogicalVolume(Epox, mEpox_, nmEpox);
     EpoxLV-> SetVisAttributes(va);
     new G4PVPlacement(zeRot, G4ThreeVector(0.,0.,
     tgas2/2.-tepox/2.),
     EpoxLV, nmEpox, Gas2LV, false, 0);*/

   //G4ThreeVector posGas1(0,0,tmylar+tal);
   G4ThreeVector posGas1(0,0,0);
   posGas1+=gPos_;
   posGas1*=rotMtx_;
   G4ThreeVector posGas2(0,0,tgas2/2.+tgas1/2.);
   posGas2+=gPos_;
   posGas2*=rotMtx_;
   /*G4VPhysicalVolume *gemPV = new G4PVPlacement
     (&rotMtx_, gPos_, DetLV_, nmBox, 
     pMother, false, Id_);*/
   G4VPhysicalVolume *gemPV = new G4PVPlacement
      (&rotMtx_, posGas1, DetLV_, nmGas1, 
       pMother, false, Id_);
   new G4PVPlacement
      (&rotMtx_, posGas2, Gas2LV, nmGas2, pMother, false, Id_);

   G4ThreeVector posMylar(0,0,-tgas1/2.-tmylar/2.-tal);
   posMylar+=gPos_;
   posMylar*=rotMtx_;
   //G4Box* boxMylar= new G4Box(nmMylar, size/2., size/2., tmylar/2.);
   G4Trd* Mylar_pre= new G4Trd(nmMylar, bbb/2., ccc/2., tmylar/2.,tmylar/2.,eee/2.);
   G4DisplacedSolid *Mylar = new G4DisplacedSolid( 
         nmMylar, Mylar_pre, rot90,
         G4ThreeVector(0.*cm,0.*cm,0.*cm));
   G4LogicalVolume* MylarLV= new G4LogicalVolume(Mylar, mMylar_, nmMylar);
   MylarLV-> SetVisAttributes(va);
   new G4PVPlacement
      (&rotMtx_, posMylar, MylarLV, nmMylar, pMother, false, 0);
   //new G4PVPlacement(zeRot, G4ThreeVector(0.,0.,-tgas1/2.+tmylar/2.+tal/2.), 
   //MylarLV, nmMylar, pMother, false, 0);
   //G4Box* boxAl= new G4Box(nmAl, size/2., size/2., tal/2.);
   G4Trd* Al_pre= new G4Trd(nmAl, bbb/2., ccc/2., tmylar/2.,tmylar/2.,eee/2.);
   G4DisplacedSolid *Al = new G4DisplacedSolid( 
         nmAl, Al_pre, rot90,
         G4ThreeVector(0.*cm,0.*cm,0.*cm));
   G4LogicalVolume* AlLV= new G4LogicalVolume(Al, mAl_, nmAl);
   AlLV-> SetVisAttributes(va);
   G4ThreeVector posAl1(0,0,-tgas1/2.-tal/2.);
   posAl1+=gPos_;
   posAl1*=rotMtx_;
   new G4PVPlacement
      (&rotMtx_, posAl1, AlLV, nmAl, pMother, false, 0);
   G4ThreeVector posAl2(0,0,-tgas1/2.-tmylar-tal*3/2.);
   posAl2+=gPos_;
   posAl2*=rotMtx_;
   new G4PVPlacement
      (&rotMtx_, posAl2, AlLV, nmAl, pMother, false, 1);
   //new G4PVPlacement(zeRot, G4ThreeVector(0.,0.,-tgas1/2.+tal/2.), 
   //    AlLV, nmAl, pMother, false, 0);
   //new G4PVPlacement(zeRot, G4ThreeVector(0.,0.,-tgas1/2.+tmylar+tal/2.), 
   //    AlLV, nmAl, pMother, false, 1);

   //GEM Frame 
   //va= new G4VisAttributes(G4Color(1.,1.,0.));
   //G4Box* frameDriftSpacer_tmp1= new G4Box(nmFrameDriftSpacer, 
   //    size/2.+framesize/2., size/2.+framesize/2., tframedriftspacer/2.);
   //G4Box* frameDriftSpacer_tmp2= new G4Box(nmFrameDriftSpacer, 
   //    size/2., size/2., tframedriftspacer/2.);
   G4Trd* frameDriftSpacer_tmp1_pre= new G4Trd(nmFrameDriftSpacer, aaa/2., ddd/2., 
         tframedriftspacer/2.,tframedriftspacer/2.,fff/2.);
   G4Trd* frameDriftSpacer_tmp2_pre= new G4Trd(nmFrameDriftSpacer, bbb/2., ccc/2., 
         tframedriftspacer/2.,tframedriftspacer/2.,eee/2.);
   G4DisplacedSolid *frameDriftSpacer_tmp1 = new G4DisplacedSolid( 
         nmFrameDriftSpacer, frameDriftSpacer_tmp1_pre, rot90,
         G4ThreeVector(0.*cm,0.*cm,0.*cm));
   G4DisplacedSolid *frameDriftSpacer_tmp2 = new G4DisplacedSolid( 
         nmFrameDriftSpacer, frameDriftSpacer_tmp2_pre, rot90,
         G4ThreeVector(0.*cm,0.*cm,0.*cm));
   G4SubtractionSolid *frameDriftSpacer= new G4SubtractionSolid(
         nmFrameDriftSpacer,frameDriftSpacer_tmp1,frameDriftSpacer_tmp2);
   frameDriftSpacerLV_= new G4LogicalVolume(frameDriftSpacer, 
         mAcryl_, nmFrameDriftSpacer);
   frameDriftSpacerLV_-> SetVisAttributes(va);
   G4ThreeVector posFrameDriftSpacer(0,0,-tframedriftspacer/2.);
   posFrameDriftSpacer+=gPos_;
   posFrameDriftSpacer*=rotMtx_;
   new G4PVPlacement
      (&rotMtx_, posFrameDriftSpacer, frameDriftSpacerLV_, 
       nmFrameDriftSpacer, pMother, false, 0);

   //G4Box* frameSeal_tmp1= new G4Box(nmFrameSeal, 
   //    size/2.+framesize/2., size/2.+framesize/2., tframeseal/2.);
   //G4Box* frameSeal_tmp2= new G4Box(nmFrameSeal, 
   //    size/2., size/2., tframeseal/2.);
   G4Trd* frameSeal_tmp1_pre= new G4Trd(nmFrameSeal, aaa/2., ddd/2., 
         tframeseal/2.,tframeseal/2.,fff/2.);
   G4Trd* frameSeal_tmp2_pre= new G4Trd(nmFrameSeal, bbb/2., ccc/2., 
         tframeseal/2.,tframeseal/2.,eee/2.);
   G4DisplacedSolid *frameSeal_tmp1 = new G4DisplacedSolid( 
         nmFrameSeal, frameSeal_tmp1_pre, rot90,
         G4ThreeVector(0.*cm,0.*cm,0.*cm));
   G4DisplacedSolid *frameSeal_tmp2 = new G4DisplacedSolid( 
         nmFrameSeal, frameSeal_tmp2_pre, rot90,
         G4ThreeVector(0.*cm,0.*cm,0.*cm));
   G4SubtractionSolid *frameSeal= new
      G4SubtractionSolid(nmFrameSeal,frameSeal_tmp1,frameSeal_tmp2);
   G4LogicalVolume* frameSealLV= new G4LogicalVolume(frameSeal, mAcryl_, nmFrameSeal);
   frameSealLV-> SetVisAttributes(va);

   //G4Box* frameGEM_tmp1= new G4Box(nmFrameGEM, 
   //    size/2.+framesize/2., size/2.+framesize/2., tframegem/2.);
   //G4Box* frameGEM_tmp2= new G4Box(nmFrameGEM, 
   //    size/2., size/2., tframegem/2.);
   G4Trd* frameGEM_tmp1_pre= new G4Trd(nmFrameGEM, aaa/2., ddd/2., 
         tframegem/2.,tframegem/2.,fff/2.);
   G4Trd* frameGEM_tmp2_pre= new G4Trd(nmFrameGEM, bbb/2., ccc/2., 
         tframegem/2.,tframegem/2.,eee/2.);
   G4DisplacedSolid *frameGEM_tmp1 = new G4DisplacedSolid( 
         nmFrameGEM, frameGEM_tmp1_pre, rot90,
         G4ThreeVector(0.*cm,0.*cm,0.*cm));
   G4DisplacedSolid *frameGEM_tmp2 = new G4DisplacedSolid( 
         nmFrameGEM, frameGEM_tmp2_pre, rot90,
         G4ThreeVector(0.*cm,0.*cm,0.*cm));
   G4SubtractionSolid *frameGEM= new
      G4SubtractionSolid(nmFrameGEM,frameGEM_tmp1,frameGEM_tmp2);
   frameGEMLV_= new G4LogicalVolume(frameGEM, mAcryl_, nmFrameGEM);
   frameGEMLV_-> SetVisAttributes(va);
   for (int i=0;i<4;i++){
      G4ThreeVector posFrameSeal(0,0,tframeseal*(i+0.5)+tframegem*i);
      posFrameSeal+=gPos_;
      posFrameSeal*=rotMtx_;
      new G4PVPlacement
         (&rotMtx_, posFrameSeal, frameSealLV, nmFrameSeal, pMother, false, 0);
      if (i<3){
         G4ThreeVector posFrameGEM(0,0,tframeseal*(i+1)+tframegem*(i+0.5));
         posFrameGEM+=gPos_;
         posFrameGEM*=rotMtx_;
         new G4PVPlacement
            (&rotMtx_, posFrameGEM, frameGEMLV_, nmFrameGEM, pMother, false, 0);
      }
   }

   //G4Box* frameReadOut_tmp1= new G4Box(nmFrameReadOut, 
   //    size/2.+framesize/2., size/2.+framesize/2., tframereadout/2.);
   //G4Box* frameReadOut_tmp2= new G4Box(nmFrameReadOut, 
   //    size/2., size/2., tframereadout/2.);
   G4Trd* frameReadOut_tmp1_pre= new G4Trd(nmFrameReadOut, aaa/2., ddd/2., 
         tframereadout/2.,tframereadout/2.,fff/2.);
   G4Trd* frameReadOut_tmp2_pre= new G4Trd(nmFrameReadOut, bbb/2., ccc/2., 
         tframereadout/2.,tframereadout/2.,eee/2.);
   G4DisplacedSolid *frameReadOut_tmp1 = new G4DisplacedSolid( 
         nmFrameReadOut, frameReadOut_tmp1_pre, rot90,
         G4ThreeVector(0.*cm,0.*cm,0.*cm));
   G4DisplacedSolid *frameReadOut_tmp2 = new G4DisplacedSolid( 
         nmFrameReadOut, frameReadOut_tmp2_pre, rot90,
         G4ThreeVector(0.*cm,0.*cm,0.*cm));
   G4SubtractionSolid *frameReadOut= new
      G4SubtractionSolid(nmFrameReadOut,frameReadOut_tmp1,frameReadOut_tmp2);
   frameReadOutLV_= new G4LogicalVolume(frameReadOut, 
         mAcryl_, nmFrameReadOut);
   frameReadOutLV_-> SetVisAttributes(va);
   G4ThreeVector posFrameReadOut(0,0,tframeseal*4+tframegem*3+tframereadout/2.);
   posFrameReadOut+=gPos_;
   posFrameReadOut*=rotMtx_;
   new G4PVPlacement
      (&rotMtx_, posFrameReadOut, frameReadOutLV_, 
       nmFrameReadOut, pMother, false, 0);

   //G4Box* frameAlBase_tmp1= new G4Box(nmFrameAlBase, 
   //    size/2.+framesize/2., size/2.+framesize/2., tframealbase/2.);
   //G4Box* frameAlBase_tmp2= new G4Box(nmFrameAlBase, 
   //    size/2., size/2., tframealbase/2.);
   G4Trd* frameAlBase_tmp1_pre= new G4Trd(nmFrameAlBase, aaa/2., ddd/2., 
         tframealbase/2.,tframealbase/2.,fff/2.);
   G4Trd* frameAlBase_tmp2_pre= new G4Trd(nmFrameAlBase, bbb/2., ccc/2., 
         tframealbase/2.,tframealbase/2.,eee/2.);
   G4DisplacedSolid *frameAlBase_tmp1 = new G4DisplacedSolid( 
         nmFrameAlBase, frameAlBase_tmp1_pre, rot90,
         G4ThreeVector(0.*cm,0.*cm,0.*cm));
   G4DisplacedSolid *frameAlBase_tmp2 = new G4DisplacedSolid( 
         nmFrameAlBase, frameAlBase_tmp2_pre, rot90,
         G4ThreeVector(0.*cm,0.*cm,0.*cm));
   G4SubtractionSolid *frameAlBase= new
      G4SubtractionSolid(nmFrameAlBase,frameAlBase_tmp1,frameAlBase_tmp2);
   frameAlBaseLV_= new G4LogicalVolume(frameAlBase, 
         mAl_, nmFrameAlBase);
   frameAlBaseLV_-> SetVisAttributes(va);
   G4ThreeVector posFrameAlBase(0,0,tframeseal*4+tframegem*3+tframereadout+tframealbase/2.);
   posFrameAlBase+=gPos_;
   posFrameAlBase*=rotMtx_;
   new G4PVPlacement
      (&rotMtx_, posFrameAlBase, frameAlBaseLV_, 
       nmFrameAlBase, pMother, false, 0);

   return gemPV;
}
