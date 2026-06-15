#include "E88G4_DetectorConstruction.hh"
#include "E16G4_MaterialList.hh"
#include "E88G4_RPC.hh"
#include "E88G4_RPCSD.hh"
#include "E88_GeometryV2.hh"
#include "E16ANA_ParamManager.hh"
#include "E16G4_PrimaryGeneratorAction.hh"

#include <G4VPhysicalVolume.hh>
#include <G4LogicalVolume.hh>
#include <G4SDManager.hh>
#include <G4ThreeVector.hh>
#include <G4RotationMatrix.hh>
#include <G4PVPlacement.hh>
#include <G4UserLimits.hh>
#include <G4SystemOfUnits.hh>
#include <sstream>

E88G4_DetectorConstruction::E88G4_DetectorConstruction()
  : E16G4_DetectorConstruction()
{}

E88G4_DetectorConstruction::~E88G4_DetectorConstruction()
{}

G4VPhysicalVolume* E88G4_DetectorConstruction::Construct() {
  // 1. Construct base E16 spectrometer
  G4VPhysicalVolume* world = E16G4_DetectorConstruction::Construct();
  if (!world) return nullptr;

  // 2. Get the world logical volume
  G4LogicalVolume* worldLV = world->GetLogicalVolume();

  // 3. Construct E88-specific RPC detectors inside the world volume
  ConstructRPC(worldLV);

  return world;
}

// Accessor struct to safely read private fields of E16G4_DetectorConstruction
// by mirroring the exact member variable layout.
struct E16G4_DetectorConstruction_Accessor : public G4VUserDetectorConstruction {
  E16G4_MaterialList *mList_;
  E16G4_Field *EMField_;
  void* constMessenger; // Messenger pointer
  G4int materialFlag;
};

void E88G4_DetectorConstruction::ConstructRPC(G4LogicalVolume* worldLV) {
  // Access private fields of base class via layout cast
  E16G4_DetectorConstruction_Accessor* accessor = 
      reinterpret_cast<E16G4_DetectorConstruction_Accessor*>(static_cast<E16G4_DetectorConstruction*>(this));

  // Sensitive Detector Manager
  G4SDManager *SDMan = G4SDManager::GetSDMpointer();

  // Geometry data
  E16ANA_ParamManager *paramMgr = E16ANA_ParamManager::GetParamManager();
  char* geomfile = paramMgr->GetGeomFileName();
  E88_GeometryV2 *geom = new E88_GeometryV2(geomfile);

  // Instantiate sensitive detector for RPC
  E88G4_RPCSD* pRPCSD = new E88G4_RPCSD("rpc", geom);
  SDMan->AddNewDetector(pRPCSD);

  E88G4_RPC *rpc[81];
  for (G4int ix = 0; ix < 3; ix++) { // x
    for(G4int j = 0; j < 3; j++) { // y
      for(G4int i = 0; i < 9; i++) { // phi
        std::ostringstream id;
        G4int index = ix + j * 3 + i * 3 * 3;
        id << index;
        G4String Name = G4String("RPC") + id.str().c_str();

        G4ThreeVector RPCpos = geom->RPC(index)->GetDetectorCenterG4();
        G4RotationMatrix *armRot = new G4RotationMatrix(geom->RPC(index)->GetRotationG4());
        armRot->invert();

        rpc[index] = new E88G4_RPC(Name, RPCpos, armRot, index);
        if (accessor->materialFlag == 0) {
          rpc[index]->SetMaterials(accessor->mList_->Vacuum, accessor->mList_->Vacuum,
                                   accessor->mList_->Vacuum, accessor->mList_->Vacuum);
        } else {
          rpc[index]->SetMaterials(accessor->mList_->SiO2, accessor->mList_->G10,
                                   accessor->mList_->Air, accessor->mList_->Cu);
        }
        rpc[index]->Place(worldLV);
        rpc[index]->GetDetectorLV()->SetSensitiveDetector(pRPCSD);
        G4UserLimits* stepLimit = new G4UserLimits(1*cm);
        rpc[index]->GetDetectorLV()->SetUserLimits(stepLimit);
        pRPCSD->SetElement(index, rpc[index]);
      }
    }
  }
}
