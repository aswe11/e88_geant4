//2016-08-12, uploaded by nakai
//2015-01-05, uploaded by yokkaich
//2015-01-05, uploaded by yokkaich
//2014-08-27, uploaded by kawama
//2014-08-27, uploaded by kawama
//2014-04-30, uploaded by kawama
//2013-11-14, uploaded by kawama
//2013-05-13, modified by kawama
/*
   E16G4_Field.hh

   D.Kawama
*/

#ifndef E16G4_Field_h
#define E16G4_Field_h 1

#include <vector>

#include <G4MagneticField.hh>
#include <globals.hh>
#include <G4ThreeVector.hh>


class E16G4_BeamLineElement;
class E16ANA_MagneticFieldMap;

// class G4ChordFinder;
// class G4EqMagElectricField;
// class G4MagIntegratorStepper;



//class Field : public G4ElectroMagneticField
class E16G4_Field : public G4MagneticField
{
public:
  E16G4_Field();
  ~E16G4_Field();

      void GetFieldValue(const double Point[4], double* Bfield) const;

      void cleanupElemList( void );
      void AddElement( E16G4_BeamLineElement *elem );

  void SetFieldMap( E16ANA_MagneticFieldMap* map ){
    fieldmap=map;
  }

      void ResetQueue( void ) const;
      void PushInQueue( int val, const G4ThreeVector &pos ) const;
      void PrintQueue( void ) const;
      

   private:
      E16G4_Field( const E16G4_Field & );
      E16G4_Field & operator = ( const E16G4_Field & );
      
      typedef std::vector <E16G4_BeamLineElement *> BLEContainer;
      typedef std::vector <E16G4_BeamLineElement *>::const_iterator BLEIterator;

      BLEContainer elemList;

      E16ANA_MagneticFieldMap* fieldmap;

      //   G4ChordFinder *fChordFinder;
      //   G4EqMagElectricField *fEquation;
      //   G4MagIntegratorStepper *fStepper;

      void MakeStepper( void );
      void DeleteStepper( void );

      mutable std::vector<int> fFlag;
      mutable std::vector<double> fPointX, fPointY, fPointZ;
      
      static const double FieldSizeX;
      static const double FieldSizeY;
      static const double FieldSizeZ;

};
      

#endif//E16G4_Field_h


