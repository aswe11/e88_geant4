//2016-08-12, uploaded by nakai
//2015-01-05, modified by yokkaich
//2015-01-05, modified by yokkaich
//2014-08-27, modified by kawama
//2014-08-27, modified by kawama
//2014-04-30, modified by kawama
//2013-11-14, modified by kawama
//2013-05-13, modified by kawama
#include "E16G4_HBDHit.hh"

G4Allocator<E16G4_HBDHit> HBDHitAllocator;

 E16G4_HBDHit::E16G4_HBDHit()
 : gpos_(0.,0.,0.), time_(0.){
}

 E16G4_HBDHit::E16G4_HBDHit(G4ThreeVector& axyz, G4double t, G4double acharge)
 : gpos_(axyz), time_(t), charge_(acharge){
}

 E16G4_HBDHit::~E16G4_HBDHit(){
}

void E16G4_HBDHit::Draw(){
}

void E16G4_HBDHit::Print(){
	G4cout<< "Hit in Counter: "<< xyz_*(1./cm)<< " cm, "
	<< time_/ns<< " ns "<<charge_<<G4endl;
}

