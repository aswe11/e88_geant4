#ifndef E88G4_DETECTOR_CONSTRUCTION_H
#define E88G4_DETECTOR_CONSTRUCTION_H

#include "E16G4_DetectorConstruction.hh"
#include <globals.hh>


class G4LogicalVolume;
class G4VPhysicalVolume;

class E88G4_DetectorConstruction : public E16G4_DetectorConstruction {
public:
  E88G4_DetectorConstruction();
  virtual ~E88G4_DetectorConstruction();

  virtual G4VPhysicalVolume* Construct();

private:
  void ConstructRPC(G4LogicalVolume* worldLV);
};

#endif // E88G4_DETECTOR_CONSTRUCTION_H
