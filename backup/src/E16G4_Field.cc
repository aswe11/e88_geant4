//2016-08-12, uploaded by nakai
//2015-01-05, modified by yokkaich
//2015-01-05, modified by yokkaich
//2014-08-27, modified by kawama
//2014-08-27, modified by kawama
//2014-04-30, modified by kawama
//2013-11-14, modified by kawama
//2013-05-13, modified by kawama
/*
  E16G4_Field.cc

  D.Kawama
*/

#include <string>

#include <G4FieldManager.hh>
#include <G4TransportationManager.hh>
// #include <G4ClassicalRK4.hh>
// #include <G4SimpleHeum.hh>
// #include <G4EqMagElectricField.hh>

#include "E16G4_Field.hh"
#include "E16G4_BeamLineElement.hh"
#include "E16G4_GlobalVar.hh"

#include "E16ANA_MagneticFieldMap.hh"

using namespace std;


const double E16G4_Field::FieldSizeX=WorldSizeX+10.*cm;
const double E16G4_Field::FieldSizeY=WorldSizeY+10.*cm;
const double E16G4_Field::FieldSizeZ=WorldSizeZ+10.*cm;

/////////////////////
 E16G4_Field::E16G4_Field()
////////////////////
{
  MakeStepper();
  elemList.reserve(32);
}

//////////////////////
 E16G4_Field::~E16G4_Field()
/////////////////////
{
  cleanupElemList();
  //  DeleteStepper();
}

//////////////////////////////////////
void E16G4_Field::cleanupElemList( void )
//////////////////////////////////////
{
  elemList.clear();
}

///////////////////////////////////////////////////
void E16G4_Field::AddElement( E16G4_BeamLineElement *elem )
//////////////////////////////////////////////////
{
  elemList.push_back(elem);
}



/////////////////////////////////////////////////////////////////////////
void E16G4_Field::GetFieldValue(const double Point[3], double *Bfield) const
/////////////////////////////////////////////////////////////////////////
{
  //G4cout <<"E16G4_Field::GetFieldValue"<<G4endl; 

  double X[3] = { Point[0]/cm, Point[1]/cm, Point[2]/cm}; 


   G4ThreeVector gPos(Point[0], Point[1], Point[2]);  
#ifdef DEBUG
  fFlag.push_back(0);
  fPointX.push_back(gPos.x());
  fPointY.push_back(gPos.y());
  fPointZ.push_back(gPos.z());

  if( fabs(Point[0]) > FieldSizeX ||
      fabs(Point[1]) > FieldSizeY ||
      fabs(Point[2]) > FieldSizeZ ){
    G4cout << "E16G4_Field::GetFieldValue :" <<""
	  << " Out of World !!" << G4endl;
    int ns = fFlag.size();
    for( int i=0 ; i<ns ; ++i ){
      G4ThreeVector pos(fPointX[i], fPointY[i], fPointZ[i]);
      int flag = fFlag[i];
      G4cout << flag < " : " < pos/cm << G4endl;
    }
    G4Exception( "E16G4_Field::E16G4_Field : Out of World" );
  }
#endif
  G4ThreeVector B(0., 0., 0.);

  BLEIterator end = elemList.end();
  int j=0;
  for( BLEIterator i=elemList.begin(); i!=end; ++i){
    if( (*i)->ExistMagneticField() ){
      B += (*i)->GetMagneticField( gPos );
      j++;
      //      cerr<<"B "<<j<<" "<< B.y()<<endl;//yok
    }
  }
  Bfield[0] = B.x(); Bfield[1] = B.y(); Bfield[2] = B.z();



    //G4cout << gPos << " B=" << B << G4endl;
}

///////////////////////////////////
void E16G4_Field::MakeStepper( void )
//////////////////////////////////
{
  //  fEquation = new G4EqMagElectricField( this );
  //  fStepper = new G4SimpleHeum( fEquation );
  G4FieldManager *fieldMan =
    G4TransportationManager::GetTransportationManager()->GetFieldManager();
  fieldMan->SetDetectorField( this );
  fieldMan->CreateChordFinder( this );
  //  fChordFinder = new G4ChordFinder( this, 1.0e-2*mm, fStepper);

//   fieldMan->SetDeltaOneStep( 0.025*mm );
//   fieldMan->SetDeltaIntersection( 0.01*mm );

//   fieldMan->SetChordFinder( fChordFinder );
}

/////////////////////////////////////
void E16G4_Field::DeleteStepper( void )
/////////////////////////////////////
{
//   delete fChordFinder;
//   delete fStepper;
//   delete fEquation;
}

////////////////////////////////////////
void E16G4_Field::ResetQueue( void ) const
////////////////////////////////////////
{
  fFlag.clear();
  fPointX.clear();
  fPointY.clear();
  fPointZ.clear();
  if( fFlag.capacity()>1024 ){
    std::vector<int>(fFlag).swap(fFlag);
    std::vector<double>(fPointX).swap(fPointX);
    std::vector<double>(fPointY).swap(fPointY);
    std::vector<double>(fPointZ).swap(fPointZ);
  }
}

/////////////////////////////////////////////////////////////////////
void E16G4_Field::PushInQueue( int , const G4ThreeVector & ) const
//////////////////////////////////////////////////////////////////////
{
#ifdef DEBUG
  fFlag.push_back(val);
  fPintX.push_back(pos.x());
  fPintY.push_back(pos.y());
  fPintZ.push_back(pos.z());
#endif
}

////////////////////////////////////////
void E16G4_Field::PrintQueue( void ) const
////////////////////////////////////////
{
  int ns = fFlag.size();
  for( int i=0; i<ns ; ++i ){
    G4ThreeVector pos(fPointX[i], fPointY[i], fPointZ[i]);
    int flag = fFlag[i];
    G4cout << flag << " : " << pos/cm << G4endl;
  }
}
