//2016-09-23, uploaded by yokkaich
//2016-08-12, uploaded by nakai
//2015-01-05, uploaded by yokkaich
//2015-01-05, uploaded by yokkaich
//2014-08-27, uploaded by kawama
//2014-08-27, uploaded by kawama
//2014-04-30, uploaded by kawama
//2013-11-14, uploaded by kawama
//2013-05-13, modified by kawama
/*
  E16G4_MaterialList.hh

  D.Kawama

*/

#ifndef E16G4_MaterialList_h

#define E16G4_MaterialList_h 1

#include <G4Element.hh>
#include <G4Material.hh>
#include <G4UnitsTable.hh>

class E16G4_MaterialList
{
public:
  E16G4_MaterialList();
  ~E16G4_MaterialList();

  G4Element *elH;
  G4Element *elLi;
  G4Element *elBe;
  G4Element *elB;
  G4Element *elC;
  G4Element *elN;
  G4Element *elO;
  G4Element *elF;
  G4Element *elNa;
  G4Element *elAl;
  G4Element *elSi;
  G4Element *elAr;
  G4Element *elCa;
  G4Element *elCr;
  G4Element *elMn;
  G4Element *elFe;
  G4Element *elCo;
  G4Element *elNi;
  G4Element *elCu;
  G4Element *elMo;
  G4Element *elW;
  G4Element *elSb;
  G4Element *elPb;
  G4Element *elK;

  G4Material *HeGas;
  G4Material *Ar;
  G4Material *ArCO2;
  G4Material *Myl;
  G4Material *Kevlar;
  G4Material *Eval;
  G4Material *Al;
  G4Material *W;
  G4Material *Ti;
  G4Material *Fe;
  G4Material *Ni;
  G4Material *Cu;
  G4Material *C12;
  G4Material *CFRP;
  G4Material *Si28;
  G4Material *V51;
  G4Material *Cr52;
  G4Material *Y89;
  G4Material *Li7;
  G4Material *Be9;
  G4Material *B10;
  G4Material *Ca40;
  G4Material *Pb208;
  G4Material *N2Gas;
  G4Material *O2Gas;
  G4Material *CF4;

  G4Material *Sci;
  G4Material *Aerog;
  G4Material *H2O;
  G4Material *C2H6;
  G4Material *CH2;

  G4Material *Vacuum;
  G4Material *Air;
  G4Material *DCGas;
  G4Material *Heavymet;
  G4Material *Harver;  
  G4Material *sus304;  
  G4Material *Concre;  
  G4Material *SiO2; 
  G4Material *CO2; 
  G4Material *K2O; 
  G4Material *Na2O; 
  G4Material *PbO; 
  G4Material *Sb2O2; 
  G4Material *LG; 
  G4Material *Kapton; 
  G4Material *Acrylic; 
  G4Material *G10; 
  G4Material *Epoxy; 
  G4Material *FR4; 

private:
  E16G4_MaterialList( const E16G4_MaterialList & );
  E16G4_MaterialList & operator = ( const E16G4_MaterialList & );
};

#endif//E16G4_MaterialList_h

