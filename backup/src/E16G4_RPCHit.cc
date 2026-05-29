//2016-08-12, uploaded by nakai
//2016-03-18, uploaded by nakai

#include "E16G4_RPCHit.hh"

G4Allocator<E16G4_RPCHit> RPCHitAllocator;

E16G4_RPCHit::E16G4_RPCHit()
 : gpos_(0.,0.,0.), time_(0.){
}

E16G4_RPCHit::E16G4_RPCHit(const G4ThreeVector& axyz, G4double t, G4double acharge)
 : gpos_(axyz), time_(t), charge_(acharge){
}

E16G4_RPCHit::~E16G4_RPCHit(){
}

void E16G4_RPCHit::Draw(){
}

void E16G4_RPCHit::Print(){
   G4cout << "Hit in Counter: " << xyz_*(1./mm) << " mm, "
      << time_/ns << " ns " << charge_ << G4endl;
}

