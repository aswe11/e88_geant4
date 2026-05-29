//2016-09-23, uploaded by yokkaich
//2016-08-12, uploaded by nakai
//2016-03-18, uploaded by nakai
//2015-01-05, uploaded by yokkaich
//2015-01-05, uploaded by yokkaich
//2014-08-27, uploaded by kawama
//2014-08-27, uploaded by kawama
//2014-04-30, uploaded by kawama
//2013-11-14, uploaded by kawama
//2013-05-13, modified by kawama
/*
  E16G4_TargetChamber.hh

   D.Kawama
*/

#ifndef E16G4_TargetChamber_h
#define E16G4_TargetChamber_h 1

#include "E16G4_BeamLineElement.hh"
#include "E16G4_Target.hh"

#include <vector>

class G4Material;
class G4VSolid;
class G4LogicalVolume;
class G4VPhysicalVolume;


class E16G4_TargetChamber : public E16G4_BeamLineElement
{
public:
  E16G4_TargetChamber( const G4String &name, const G4ThreeVector &gPos,
	  const G4RotationMatrix *rotMtx, int id );
  ~E16G4_TargetChamber();

  bool ExistElectricField( void ) const { return false; }
  bool ExistMagneticField( void ) const { return false; }

  bool IsInside( const G4ThreeVector &gPos ) const;

  void SetMaterials(G4Material *mCFRP,G4Material *mMylar,
        G4Material *mGas,
    G4Material *mTar1,G4Material *mTar2,G4Material *mTar3, 
    G4Material *mTar4, G4Material *mTar5, G4Material *mTar6, G4Material *mTar7)
  { 
     mCFRP_=mCFRP;mMylar_=mMylar;
     mGas_=mGas;mTar1_=mTar1;mTar2_=mTar2;mTar3_=mTar3;mTar4_=mTar4;
     mTar5_=mTar5; mTar6_=mTar6; mTar7_=mTar7;}
  void SetMaterials(G4Material *m)
  { mCFRP_=m;mMylar_=m;mGas_=m;mTar1_=m;mTar2_=m;mTar3_=m;
    mTar4_=m,mTar5_=m;mTar6_=m,mTar7_=m;}

  void SetThickness(G4double t1,G4double t2,G4double t3,
    G4double t4,G4double t5,G4double t6,G4double t7){
   tthick1_=t1;tthick2_=t2;tthick3_=t3;
   tthick4_=t4;tthick5_=t5;tthick6_=t6;tthick7_=t7;}

  G4LogicalVolume *GetDetectorLV( void ) { return DetLV_; }
  G4LogicalVolume *GetChamberGasLV( void ) { return gasLV_; }
  G4ThreeVector GetChamberCenter(){return posGas1_;};

  //G4VPhysicalVolume *Place( G4VPhysicalVolume *pMother );
  G4VPhysicalVolume *Place( G4LogicalVolume *pMother ); // (for SSD 09)
  G4VPhysicalVolume *Place2( G4LogicalVolume *pMother ); // cylindrical target chamber (for SSD 33)
  E16G4_Target* GetTarget1(){return tar1_;};
  E16G4_Target* GetTarget2(){return tar2_;};
  E16G4_Target* GetTarget3(){return tar3_;};
  E16G4_Target* GetTarget4(){return tar4_;};
  E16G4_Target* GetTarget5(){return tar5_;};
  E16G4_Target* GetTarget6(){return tar6_;};//beam-pipe end window
  E16G4_Target* GetTarget7(){return tar7_;};//chamber input window

private:
  G4Material *mCFRP_;
  G4Material *mMylar_;
  G4Material *mGas_;
  G4Material *mTar1_;
  G4Material *mTar2_;
  G4Material *mTar3_;
  G4Material *mTar4_;
  G4Material *mTar5_;
  G4Material *mTar6_;
  G4Material *mTar7_;
  G4LogicalVolume *DetLV_;
  G4LogicalVolume *gasLV_;
  G4VSolid *vSolid_;
  int Id_;
  G4ThreeVector posGas1_;
  G4double gascenter_;
  G4double tthick1_;
  G4double tthick2_;
  G4double tthick3_;
  G4double tthick4_;
  G4double tthick5_;
  G4double tthick6_;
  G4double tthick7_;
  E16G4_Target *tar1_;
  E16G4_Target *tar2_;
  E16G4_Target *tar3_;
  E16G4_Target *tar4_;
  E16G4_Target *tar5_;
  E16G4_Target *tar6_;//beampipe end window: for the background study
  E16G4_Target *tar7_;//chamber input window: for the background study
  E16G4_Target* PlaceTarget(int target_id, G4double pos_z, G4double tthick, G4Material *mTar);
};

#endif//E16G4_TargetChamber_h

