//2016-09-23, uploaded by yokkaich
//2016-08-12, uploaded by nakai
//2015-01-05, uploaded by yokkaich
//2015-01-05, uploaded by yokkaich
//2014-08-27, uploaded by kawama
//2014-08-27, uploaded by kawama
//2014-04-30, uploaded by kawama
//2013-11-14, uploaded by kawama
//2013-05-13, modified by kawama
// ====================================================================
//    E16G4_DetectorConstruction.hh
//
// ====================================================================
#ifndef E16G4_DETECTOR_CONSTRUCTION_H
#define E16G4_DETECTOR_CONSTRUCTION_H

#include <globals.hh>
#include <G4VUserDetectorConstruction.hh>
#include "E16G4_DetectorConstructionMessenger.hh"


// ====================================================================
//
// class definition
//
// ====================================================================

class E16G4_MaterialList;
class E16G4_Field;
class E16G4_DetectorConstructionMessenger;

class E16G4_DetectorConstruction : public G4VUserDetectorConstruction {

public:
  E16G4_DetectorConstruction();
  ~E16G4_DetectorConstruction(); 

  virtual G4VPhysicalVolume* Construct(); 
  void UpdateGeometry();

private:
  E16G4_MaterialList *DefineMaterials(void);
  E16G4_MaterialList *mList_;

  E16G4_Field *MakeField(void);
  E16G4_Field *EMField_;
      
  E16G4_DetectorConstructionMessenger* constMessenger;   //messenger of this class
  
  G4int materialFlag;
  G4String tarMaterial[7];
  G4double tarThick[7];

public:  
  void SetMaterialFlag(G4int val) { materialFlag = val;}
  void SetTarget1(G4String val) { tarMaterial[0] = val;}
  void SetTarget2(G4String val) { tarMaterial[1] = val;}
  void SetTarget3(G4String val) { tarMaterial[2] = val;}
  void SetTarget4(G4String val) { tarMaterial[3] = val;}
  void SetTarget5(G4String val) { tarMaterial[4] = val;}
  void SetTarget6(G4String val) { tarMaterial[5] = val;}
  void SetTarget7(G4String val) { tarMaterial[6] = val;}
  void SetTargetThickness1(G4double val) { tarThick[0] = val;}
  void SetTargetThickness2(G4double val) { tarThick[1] = val;}
  void SetTargetThickness3(G4double val) { tarThick[2] = val;}
  void SetTargetThickness4(G4double val) { tarThick[3] = val;}
  void SetTargetThickness5(G4double val) { tarThick[4] = val;}
  void SetTargetThickness6(G4double val) { tarThick[5] = val;}
  void SetTargetThickness7(G4double val) { tarThick[6] = val;}
  void SetTargetDefaultValue(){
    for(int i=0;i<7;i++){
      tarThick[i]=0;
      tarMaterial[i]="Vacuum";
    }
  }

};

#endif//E16G4_DETECTOR_CONSTRUCTION_H
