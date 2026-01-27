//2016-08-12, uploaded by nakai
//2016-03-18, uploaded by nakai

#include "E16G4_SSDHit.hh"

G4Allocator<E16G4_SSDHit> SSDHitAllocator;

E16G4_SSDHit::E16G4_SSDHit()
 : gpos_(0.,0.,0.), time_(0.){
}

E16G4_SSDHit::E16G4_SSDHit(const G4ThreeVector& axyz, G4double t, G4double acharge)
 : gpos_(axyz), time_(t), charge_(acharge){
}

E16G4_SSDHit::~E16G4_SSDHit(){
}

void E16G4_SSDHit::Draw(){
}

void E16G4_SSDHit::Print(){
   G4cout << "Hit in Counter: " << xyz_*(1./mm) << " mm, "
      << time_/ns << " ns " << charge_ << G4endl;
}

