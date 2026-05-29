//2016-08-12, uploaded by nakai
//2015-01-05, modified by yokkaich
//2015-01-05, modified by yokkaich
//2014-08-27, modified by kawama
//2014-08-27, modified by kawama
//2014-04-30, modified by kawama
//2013-11-14, modified by kawama
//2013-05-13, modified by kawama
#include "E16G4_LGHit.hh"

G4Allocator<E16G4_LGHit> LGHitAllocator;

 E16G4_LGHit::E16G4_LGHit()
 : gpos_(0.,0.,0.), time_(0.){
}

 E16G4_LGHit::E16G4_LGHit(G4ThreeVector& axyz, G4double t, G4double acharge)
 : gpos_(axyz), time_(t), charge_(acharge){
}

 E16G4_LGHit::~E16G4_LGHit(){
}

void E16G4_LGHit::Draw(){
}

void E16G4_LGHit::Print(){
	G4cout<< "Hit in Counter: "<< xyz_*(1./cm)<< " cm, "
	<< time_/ns<< " ns "<<charge_<<G4endl;
}

