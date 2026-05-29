//2016-12-18, uploaded by yokkaich
//2016-08-12, uploaded by nakai

#include <G4VisAttributes.hh>
#include <G4Material.hh>
#include <G4Box.hh>
#include <G4SubtractionSolid.hh>

#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4PVPlacement.hh>

#include "E16G4_RPC.hh"

E16G4_RPC::E16G4_RPC(const G4String &name, const G4ThreeVector &gPos, const G4RotationMatrix *rotMtx, int id) : 
   E16G4_BeamLineElement(name, gPos, rotMtx),
   mGlass_(0), // materials
   detLV_(0), // logical volumes
   mPCB_(0),
   pcbLV_(0),
   mAir_(0),
   mCu_(0),
   id_(id)
{
}

E16G4_RPC::~E16G4_RPC(){
}

bool E16G4_RPC::IsInside(const G4ThreeVector &gPos) const {
   G4ThreeVector lPos = GetLocalPosition(gPos);
   if(vSolid_->Inside(lPos) != kOutside) return true;
   else return false;
}

G4VPhysicalVolume* E16G4_RPC::Place(G4LogicalVolume *pMother){
   // place PCB, support frame etc...
  return PlaceRPC(pMother);
  //  return PlaceSensor(pMother);
}

//new sako
G4VPhysicalVolume* E16G4_RPC::PlaceRPC(G4LogicalVolume *pMother){//strip layer position
  G4double sizex_RPC = 246.0*mm;
  G4double sizey_RPC_real = 1162.0*mm;
  G4double sizey_RPC = 819.0*mm;//temporary for current geom file
  G4double sizez_RPC = 24.0*mm;

  G4double sizex_strip = 208.0*mm;
  //sako up to 2023/4/8  G4double sizey_Strip = 819.0*mm;
  //sako 2023/4/9
  G4double sizey_strip = 770.0*mm;
  G4double sizey_strip_real = 1000.0*mm;

  //  if (id_==16) {
  //    sizex_sensor = 400.0*mm;
  //    sizey_sensor = 400.0*mm;
  //  }
  const G4double t_glass = 0.4*6.*mm;//half
  const G4double t_air = 0.24*5.*mm;//half
  const G4double t_PCB = 0.8*mm;//*4
  const G4double t_strip = 0.04*mm;//Cu
  G4String name_RPC = elemName_+"_RPC";

  G4Box *boxRPC = new G4Box(name_RPC, sizex_RPC/2.0, sizey_RPC/2.0, sizez_RPC/2.0);
  G4LogicalVolume *RPCLV = new G4LogicalVolume(boxRPC, mAir_, name_RPC);
  G4VisAttributes *va = new G4VisAttributes(G4Color(1.,1.,1.));
  RPCLV->SetVisAttributes(va);


  //place inner structures
  //Glass
  const G4double z_glass = 2.8*mm;
  const G4double z_Strip0 = 0*mm;
  const G4double z_Strip1 = 5.72*mm;
  const G4double z_PCB1 = 0.5*mm;
  const G4double z_PCB2 = 5.1*mm;

  G4String name_glass = elemName_+"_glass";
  G4Box *boxGlass = new G4Box(name_glass, sizex_RPC/2.0, sizey_RPC/2.0, t_glass/2.0);
  G4LogicalVolume *glassLV = new G4LogicalVolume(boxGlass, mGlass_, name_glass);
  G4VisAttributes *va2 = new G4VisAttributes(G4Color(1.,1.,0.));
  glassLV->SetVisAttributes(va2);
  G4VPhysicalVolume *glass1PV = new G4PVPlacement(0,G4ThreeVector(0,0,z_glass), glassLV, name_glass, RPCLV, false, 0);
  G4VPhysicalVolume *glass2PV = new G4PVPlacement(0,G4ThreeVector(0,0,-z_glass), glassLV, name_glass, RPCLV, false, 1);

  G4String name_PCB = elemName_+"_PCB";
  G4Box *boxPCB = new G4Box(name_PCB, sizex_RPC/2.0, sizey_RPC/2.0, t_PCB/2.0);
  G4LogicalVolume *PCBLV = new G4LogicalVolume(boxPCB, mPCB_, name_PCB);
  G4VisAttributes *va3 = new G4VisAttributes(G4Color(0.,1.,0.));
  PCBLV->SetVisAttributes(va3);
  G4VPhysicalVolume *PCB1PV = new G4PVPlacement(0,G4ThreeVector(0,0,z_PCB1), PCBLV, name_PCB, RPCLV, false, 0);
  G4VPhysicalVolume *PCB2PV = new G4PVPlacement(0,G4ThreeVector(0,0,-z_PCB1), PCBLV, name_PCB, RPCLV, false, 1);
  G4VPhysicalVolume *PCB3PV = new G4PVPlacement(0,G4ThreeVector(0,0,z_PCB2), PCBLV, name_PCB, RPCLV, false, 2);
  G4VPhysicalVolume *PCB4PV = new G4PVPlacement(0,G4ThreeVector(0,0,-z_PCB2), PCBLV, name_PCB, RPCLV, false, 3);

  G4String name_Sensor = elemName_+"_Sensor";
  G4String name_Strip = elemName_+"_Strip";
  //sako up to 2023/4/8
  //  G4Box *boxSensor = new G4Box(name_Sensor, sizex_RPC/2.0, sizey_RPC/2.0, t_Strip*2.0/2.0);//strip 0 is sensor
  //  G4Box *boxStrip1 = new G4Box(name_Strip, sizex_RPC/2.0, sizey_RPC/2.0, t_Strip/2.0);
  //sako new 2023/4/9
  G4Box *boxSensor = new G4Box(name_Sensor, sizex_strip/2.0, sizey_strip/2.0, t_strip*2.0/2.0);//strip 0 is sensor
  G4Box *boxStrip1 = new G4Box(name_Strip, sizex_strip/2.0, sizey_strip/2.0, t_strip/2.0);

  detLV_ = new G4LogicalVolume(boxSensor, mCu_, name_Sensor);

  G4VisAttributes *va1 = new G4VisAttributes(G4Color(0.,0.,1.));
  detLV_->SetVisAttributes(va1);

  G4LogicalVolume *StripLV1 = new G4LogicalVolume(boxStrip1, mCu_, name_Strip);
  G4VPhysicalVolume *SensorPV = new G4PVPlacement(0,G4ThreeVector(0,0,z_Strip0), detLV_, name_Sensor, RPCLV, false, 0);
  G4VPhysicalVolume *Strip1PV = new G4PVPlacement(0,G4ThreeVector(0,0,z_Strip1), StripLV1, name_Strip, RPCLV, false, 1);
  G4VPhysicalVolume *Strip2PV = new G4PVPlacement(0,G4ThreeVector(0,0,-z_Strip1), StripLV1, name_Strip, RPCLV, false, 2);


  G4ThreeVector pos_RPC = gPos_;
  
  G4cout << "E16G4_RPC::PlaceRPC : ID = " << id_ << ", pos = " << pos_RPC/mm << G4endl;
  G4VPhysicalVolume *RPCPV = new G4PVPlacement(&rotMtx_, pos_RPC, RPCLV, name_RPC, pMother, false, id_);
  return RPCPV;
}


/*
G4VPhysicalVolume* E16G4_RPC::PlaceSensor(G4LogicalVolume *pMother){//strip layer position
  G4double sizex_sensor = 250.0*mm;
  G4double sizey_sensor = 1000.0*mm;
  //  if (id_==16) {
  //    sizex_sensor = 400.0*mm;
  //    sizey_sensor = 400.0*mm;
  //  }
   const G4double thickness_sensor = 0.3*mm;
   G4String name_sensor = elemName_+"_Sensor";

   G4Box *boxSi = new G4Box(name_sensor, sizex_sensor/2.0, sizey_sensor/2.0, thickness_sensor/2.0);
   detLV_ = new G4LogicalVolume(boxSi, mSi_, name_sensor);
   G4VisAttributes *va = new G4VisAttributes(G4Color(0.,0.,1.));
   detLV_->SetVisAttributes(va);

   G4ThreeVector pos_sensor = gPos_;

   G4cout << "E16G4_RPC::PlaceSensor : ID = " << id_ << ", pos = " << pos_sensor/mm << G4endl;
   G4VPhysicalVolume *detPV = new G4PVPlacement(&rotMtx_, pos_sensor, detLV_, name_sensor, pMother, false, id_);
   return detPV;
}
*/


