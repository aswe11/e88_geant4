//2016-08-12, uploaded by nakai
//2015-01-20, uploaded by yokkaich
//2015-01-05, modified by yokkaich
//2015-01-05, modified by yokkaich
//2014-08-27, modified by kawama
//2014-08-27, modified by kawama
//2014-04-30, modified by kawama
//2013-11-14, modified by kawama
//2013-05-13, modified by kawama
/*
  E16G4_VDHit.cc

  D.Kawama
*/


#include <iomanip>
 
#include <G4VVisManager.hh>
#include <G4Circle.hh>
#include <G4Colour.hh>
#include <G4VisAttributes.hh>

#include "E16G4_VDHit.hh"

////////////////////////////////////////////////////////////////////
E16G4_VDHit::E16G4_VDHit( G4int id, const G4ThreeVector &gPos,
		    const G4ThreeVector &gMom,
		    //G4double x, G4double y, G4double u, G4double v, 
		    G4double edep, G4ThreeVector vertex,G4String &name)
  : VDId_(id), gPos_(gPos), gMom_(gMom), 
    //Xloc(x), Yloc(y), Uloc(u), Vloc(v),
	 Edep_(edep),Vertex_(vertex), name_(name)
////////////////////////////////////////////////////////////////////
{
}

/////////////////////
E16G4_VDHit::~E16G4_VDHit()
/////////////////////
{
}


G4Allocator<E16G4_VDHit> VDHitAllocator;

//////////////////////
void E16G4_VDHit::Draw() 
//////////////////////
{
  G4VVisManager* pVisManager = 
    G4VVisManager::GetConcreteInstance();
  if( pVisManager ){
    G4Circle circle(gPos_);
    circle.SetScreenSize( 3.0 ); // in pixels
    circle.SetFillStyle( G4Circle::filled );
    G4Colour colour( 1., 0., 0. ); //blue
    G4VisAttributes attribs( colour );
    circle.SetVisAttributes( attribs );
    pVisManager-> Draw( circle );
  }
}

///////////////////////
void E16G4_VDHit::Print() 
///////////////////////
{
  G4cout << "VD : " << VDId_ << " : ( "
	 << gPos_.x()/cm << ", " << gPos_.y()/cm << ", "
	 << gPos_.z()/cm << " ) " << G4endl;

}
