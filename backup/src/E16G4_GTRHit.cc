//2016-08-12, uploaded by nakai
//2015-01-05, modified by yokkaich
//2015-01-05, modified by yokkaich
//2014-08-27, modified by kawama
//2014-08-27, modified by kawama
//2014-04-30, modified by kawama
//2013-11-14, modified by kawama
//2013-05-13, modified by kawama
#include "E16G4_GTRHit.hh"

G4Allocator<E16G4_GTRHit> GTRHitAllocator;

 E16G4_GTRHit::E16G4_GTRHit()
 : gpos_(0.,0.,0.), time_(0.){
}

 E16G4_GTRHit::E16G4_GTRHit(const G4ThreeVector& axyz, G4double t, G4double acharge)
 : gpos_(axyz), time_(t), charge_(acharge){
}

 E16G4_GTRHit::~E16G4_GTRHit(){
}

void E16G4_GTRHit::Draw(){
}

void E16G4_GTRHit::Print(){
	G4cout<< "Hit in Counter: "<< xyz_*(1./cm)<< " cm, "
	<< time_/ns<< " ns "<<charge_<<G4endl;
}

