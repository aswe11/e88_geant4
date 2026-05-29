//2016-09-23, uploaded by yokkaich
//2016-08-12, uploaded by nakai
//2015-01-05, modified by yokkaich
//2015-01-05, modified by yokkaich
//2014-08-27, modified by kawama
//2014-08-27, modified by kawama
//2014-04-30, modified by kawama
//2013-11-14, modified by kawama
//2013-05-13, modified by kawama
/*
   E16G4_MaterialList.cc

   D.Kawama 
*/

#include "E16G4_MaterialList.hh"

 E16G4_MaterialList::E16G4_MaterialList()
{
   G4double a, iz, z, density;
   G4String name, symbol;
   G4int nel, nAtoms;
   G4double fractionMass;

   //=== Elements ===//
   name = "Hydrogen"; symbol = "H";
   a = 1.00794*g/mole;
   elH = new G4Element( name, symbol, iz=1., a);

   name = "Lithium"; symbol = "Li";
   a = 6.941*g/mole;
   elLi = new G4Element( name, symbol, iz=3., a);

   name = "Beryllium"; symbol = "Be";
   a = 9.012182*g/mole;
   elBe = new G4Element( name, symbol, iz=4., a);

   name = "Boron"; symbol = "B";
   a = 10.811*g/mole;
   elB = new G4Element( name, symbol, iz=5., a);

   name = "Carbon"; symbol = "C";
   a = 12.011*g/mole;
   elC = new G4Element( name, symbol, iz=6., a);

   name = "Nitrogen"; symbol = "N";
   a = 14.00674*g/mole;
   elN = new G4Element( name, symbol, iz=7., a);

   name = "Oxygen"; symbol = "O";
   a = 15.9994*g/mole;
   elO = new G4Element( name, symbol, iz=8., a);
   
   name = "Fluorine"; symbol = "F";
   a = 18.9984*g/mole;
   elF = new G4Element( name, symbol, iz=9., a);

   name = "Sodium"; symbol = "Na"; 
   a = 22.989*g/mole;
   elNa = new G4Element( name, symbol, iz=11., a);

   name = "Aluminum"; symbol = "Al"; 
   a = 26.982*g/mole;
   elAl = new G4Element( name, symbol, iz=13., a);

   name = "Silicon"; symbol = "Si";
   a = 28.0855*g/mole;
   elSi = new G4Element( name, symbol, iz=14., a);

   name = "Argon"; symbol = "Ar";
   a = 39.948*g/mole;
   elAr = new G4Element( name, symbol, iz=18., a);

   name = "Potassium"; symbol = "K";
   a = 39.0983*g/mole;
   elK = new G4Element( name, symbol, iz=19., a);

   name = "Calcium"; symbol = "Ca";
   a = 40.078*g/mole;
   elCa = new G4Element( name, symbol, iz=20., a);
   
   name = "Chromium"; symbol = "Cr"; 
   a = 51.996*g/mole;
   elCr = new G4Element( name, symbol, iz=24., a);

   name = "Manganese"; symbol = "Mn";
   a = 54.938*g/mole;
   elMn = new G4Element( name, symbol, iz=25., a);

   name = "Iron"; symbol = "Fe";  
   a = 55.845*g/mole;
   elFe = new G4Element( name, symbol, iz=26., a);

   name = "Cobalt"; symbol = "Co";
   a = 58.933*g/mole;
   elCo = new G4Element( name, symbol, iz=27., a);

   name = "Nickel"; symbol = "Ni";
   a = 58.69*g/mole;
   elNi = new G4Element( name, symbol, iz=28., a);

   name = "Copper"; symbol = "Cu";
   a = 63.546*g/mole;
   elCu= new G4Element( name, symbol, iz=29., a);

   name = "Molybd"; symbol = "Mo";
   a = 95.94*g/mole;
   elMo = new G4Element( name, symbol, iz=42., a);
   
   name = "Antimony"; symbol = "Sb";
   a = 121.760*g/mole;
   elSb = new G4Element( name, symbol, iz=51., a );

   name = "Tungsten"; symbol = "W";
   a = 183.84*g/mole;
   elW = new G4Element( name, symbol, iz=74., a );

   name = "Lead"; symbol = "Pb";
   a = 207.2*g/mole;
   elPb = new G4Element( name, symbol, iz=82., a );

   //=== Simple Materials ==//
   name = "Tungsten";
   a = 183.84*g/mole;
   density = 19.3*g/cm3;
   W = new G4Material( name, z=74., a, density);

   name = "Aluminuium";
   a = 26.98*g/mole;
   density = 2.70*g/cm3;
   Al = new G4Material( name, z=13., a, density);

   name = "Titanium";
   a = 47.867*g/mole;
   density = 4.540*g/cm3;
   Ti = new G4Material( name, z=22., a, density);

   name = "Iron";
   a = 55.843*g/mole;
   density = 7.86*g/cm3;
   Fe = new G4Material( name, z=26., a, density);

   name = "Nikkel";
   a = 58.6934*g/mole;
   density = 8.908*g/cm3;
   Ni = new G4Material( name, z=28., a, density);

   name = "Copper";
   a = 63.546*g/mole;
   density = 8.93*g/cm3;
   Cu = new G4Material( name, z=29., a, density);

   name = "Carbon";
   a = 12.0*g/mole;
   density = 2.25*g/cm3;
   C12 = new G4Material( name, z=6., a, density);
   
   name = "CFRP";
   a = 12.0*g/mole;
   //   density = 1.53*g/cm3;
   density = 1.70*g/cm3;             //conservative 160922
   CFRP = new G4Material( name, z=6., a, density);

   name = "Silicon";
   a = 28.0*g/mole;
   density = 2.33*g/cm3;
   Si28 = new G4Material( name, z=14., a, density);

   name = "Vanadium";
   a = 51.0*g/mole;
   density = 6.11*g/cm3;
   V51 = new G4Material( name, z=23., a, density);

   name = "Cromium";
   a = 52.0*g/mole;
   density = 7.15*g/cm3;
   Cr52 = new G4Material( name, z=24., a, density);

   name = "Yttrium";
   a = 89.0*g/mole;
   density = 4.469*g/cm3;
   Y89 = new G4Material( name, z=39., a, density);

   name = "Lithium";
   a = 7.0*g/mole;
   density = 0.534*g/cm3;
   Li7 = new G4Material( name, z=3., a, density);

   name = "Beryllium"; 
   a = 9.012*g/mole;
   density = 1.848*g/cm3;
   Be9 = new G4Material( name, z=4., a, density);

   name = "Boron";
   a = 10.0*g/mole;
   density = 2.340*g/cm3;
   B10 = new G4Material( name, z=5., a, density);

   name = "Calcium";
   a = 40.0*g/mole;
   density = 1.55*g/cm3;
   Ca40 = new G4Material( name, z=20., a, density);

   name = "Lead";
   a = 207.19*g/mole;
   density = 11.35*g/cm3;
   Pb208 = new G4Material( name, z=82., a, density);

   name = "HeliumGas";
   a = 4.002602*g/mole;
   //  density = 0.167*g/cm3;
   density = 0.1786*mg/cm3;
   HeGas = new G4Material( name, z=2., a, density );

   name = "ArgonGas";
   a = 39.948*g/mole;
   density = 1.6723*mg/cm3;
   Ar = new G4Material( name, z=18., a, density,
        kStateGas, 300.*kelvin );

   name = "Nitrogen Gas";
   a = 28.01*g/mole;
   density = 1.251*mg/cm3;
   N2Gas = new G4Material( name, z=7., a, density );

   name = "Oxygen Gas";
   a = 32.00*g/mole;
   density = 1.429*mg/cm3;
   O2Gas = new G4Material( name, z=8., a, density );

   //=== Componds Materials ===//
   name = "Mylar";
   density = 1.39*g/cm3;
   Myl = new G4Material( name, density, nel=3 );
   Myl->AddElement( elH, nAtoms=8 );
   Myl->AddElement( elC, nAtoms=10 );
   Myl->AddElement( elO, nAtoms=4 );

   name = "Kevlar";
   density = 0.74*g/cm3;
   Kevlar = new G4Material( name, density, nel=4 );
   Kevlar->AddElement( elH, nAtoms=10 );
   Kevlar->AddElement( elC, nAtoms=14 );
   Kevlar->AddElement( elO, nAtoms=2 );
   Kevlar->AddElement( elN, nAtoms=2 );

   name = "Eval";//EVOH, by Yokkaichi
   density = 1.25*g/cm3; // 1.15-1.25 in the datasheet
   Eval = new G4Material( name, density, nel=3 );
   Eval->AddElement( elH, nAtoms=400 );
   Eval->AddElement( elC, nAtoms=200 );
   Eval->AddElement( elO, nAtoms=78 );
   // (CH2-CH2)m - (CH2-CHOH)n,  m~22-49mol%
   // thus, most heavy case  C2H4 x 22 + C2H4O x 78 = C2+H4+O_0.78


   name = "Scintillator";
   density = 1.032*g/cm3;
   Sci = new G4Material( name, density, nel=2);
   Sci->AddElement(elH, nAtoms=10);
   Sci->AddElement(elC, nAtoms=9);

   name = "Water";
   density = 1.*g/cm3;
   H2O = new G4Material( name, density, nel=2);
   H2O->AddElement(elH, nAtoms=2);
   H2O->AddElement(elO, nAtoms=1);
   H2O-> GetIonisation()->SetMeanExcitationEnergy(75.0*eV);
//
// ------------ Generate & Add Material Properties Table ------------
//
  const G4int nEntries = 32;

  G4double PhotonEnergy[nEntries] =
            { 2.034*eV, 2.068*eV, 2.103*eV, 2.139*eV,
              2.177*eV, 2.216*eV, 2.256*eV, 2.298*eV,
              2.341*eV, 2.386*eV, 2.433*eV, 2.481*eV,
              2.532*eV, 2.585*eV, 2.640*eV, 2.697*eV,
              2.757*eV, 2.820*eV, 2.885*eV, 2.954*eV,
              3.026*eV, 3.102*eV, 3.181*eV, 3.265*eV,
              3.353*eV, 3.446*eV, 3.545*eV, 3.649*eV,
              3.760*eV, 3.877*eV, 4.002*eV, 4.136*eV };
//
// Water
//	      
  G4double RefractiveIndex1[nEntries] =
            { 1.3435, 1.344,  1.3445, 1.345,  1.3455,
              1.346,  1.3465, 1.347,  1.3475, 1.348,
              1.3485, 1.3492, 1.35,   1.3505, 1.351,
              1.3518, 1.3522, 1.3530, 1.3535, 1.354,
              1.3545, 1.355,  1.3555, 1.356,  1.3568,
              1.3572, 1.358,  1.3585, 1.359,  1.3595,
              1.36,   1.3608};

  G4double Absorption1[nEntries] =
           {3.448*m,  4.082*m,  6.329*m,  9.174*m, 12.346*m, 13.889*m,
           15.152*m, 17.241*m, 18.868*m, 20.000*m, 26.316*m, 35.714*m,
           45.455*m, 47.619*m, 52.632*m, 52.632*m, 55.556*m, 52.632*m,
           52.632*m, 47.619*m, 45.455*m, 41.667*m, 37.037*m, 33.333*m,
           30.000*m, 28.500*m, 27.000*m, 24.500*m, 22.000*m, 19.500*m,
           17.500*m, 14.500*m };

  G4double ScintilFast[nEntries] =
            { 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
              1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
              1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
              1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
              1.00, 1.00, 1.00, 1.00 };
  G4double ScintilSlow[nEntries] =
            { 0.01, 1.00, 2.00, 3.00, 4.00, 5.00, 6.00,
              7.00, 8.00, 9.00, 8.00, 7.00, 6.00, 4.00,
              3.00, 2.00, 1.00, 0.01, 1.00, 2.00, 3.00,
              4.00, 5.00, 6.00, 7.00, 8.00, 9.00, 8.00,
              7.00, 6.00, 5.00, 4.00 };

  G4MaterialPropertiesTable* myMPT1 = new G4MaterialPropertiesTable();

  myMPT1->AddProperty("RINDEX",       PhotonEnergy, RefractiveIndex1,nEntries)
        ->SetSpline(true);
  myMPT1->AddProperty("ABSLENGTH",    PhotonEnergy, Absorption1,     nEntries)
        ->SetSpline(true);
  myMPT1->AddProperty("FASTCOMPONENT",PhotonEnergy, ScintilFast,     nEntries)
        ->SetSpline(true);
  myMPT1->AddProperty("SLOWCOMPONENT",PhotonEnergy, ScintilSlow,     nEntries)
        ->SetSpline(true);
  
  
  myMPT1->AddConstProperty("SCINTILLATIONYIELD",50./MeV);
  myMPT1->AddConstProperty("RESOLUTIONSCALE",1.0);
  myMPT1->AddConstProperty("FASTTIMECONSTANT", 1.*ns);
  myMPT1->AddConstProperty("SLOWTIMECONSTANT",10.*ns);
  myMPT1->AddConstProperty("YIELDRATIO",0.8);

  const G4int NUMENTRIES_water = 60;

  G4double ENERGY_water[NUMENTRIES_water] = {
     1.56962*eV, 1.58974*eV, 1.61039*eV, 1.63157*eV,
     1.65333*eV, 1.67567*eV, 1.69863*eV, 1.72222*eV,
     1.74647*eV, 1.77142*eV, 1.7971 *eV, 1.82352*eV,
     1.85074*eV, 1.87878*eV, 1.90769*eV, 1.93749*eV,
     1.96825*eV, 1.99999*eV, 2.03278*eV, 2.06666*eV,
     2.10169*eV, 2.13793*eV, 2.17543*eV, 2.21428*eV,
     2.25454*eV, 2.29629*eV, 2.33962*eV, 2.38461*eV,
     2.43137*eV, 2.47999*eV, 2.53061*eV, 2.58333*eV,
     2.63829*eV, 2.69565*eV, 2.75555*eV, 2.81817*eV,
     2.88371*eV, 2.95237*eV, 3.02438*eV, 3.09999*eV,
     3.17948*eV, 3.26315*eV, 3.35134*eV, 3.44444*eV,
     3.54285*eV, 3.64705*eV, 3.75757*eV, 3.87499*eV,
     3.99999*eV, 4.13332*eV, 4.27585*eV, 4.42856*eV,
     4.59258*eV, 4.76922*eV, 4.95999*eV, 5.16665*eV,
     5.39129*eV, 5.63635*eV, 5.90475*eV, 6.19998*eV
  };

  //assume 100 times larger than the rayleigh scattering for now.
  G4double MIE_water[NUMENTRIES_water] = {
     167024.4*m, 158726.7*m, 150742  *m,
     143062.5*m, 135680.2*m, 128587.4*m,
     121776.3*m, 115239.5*m, 108969.5*m,
     102958.8*m, 97200.35*m, 91686.86*m,
     86411.33*m, 81366.79*m, 76546.42*m,
     71943.46*m, 67551.29*m, 63363.36*m,
     59373.25*m, 55574.61*m, 51961.24*m,
     48527.00*m, 45265.87*m, 42171.94*m,
     39239.39*m, 36462.50*m, 33835.68*m,
     31353.41*m, 29010.30*m, 26801.03*m,
     24720.42*m, 22763.36*m, 20924.88*m,
     19200.07*m, 17584.16*m, 16072.45*m,
     14660.38*m, 13343.46*m, 12117.33*m,
     10977.70*m, 9920.416*m, 8941.407*m,
     8036.711*m, 7202.470*m, 6434.927*m,
     5730.429*m, 5085.425*m, 4496.467*m,
     3960.210*m, 3473.413*m, 3032.937*m,
     2635.746*m, 2278.907*m, 1959.588*m,
     1675.064*m, 1422.710*m, 1200.004*m,
     1004.528*m, 833.9666*m, 686.1063*m
  };

  // gforward, gbackward, forward backward ratio
  G4double MIE_water_const[3]={0.99,0.99,0.8};

  myMPT1->AddProperty("MIEHG",ENERGY_water,MIE_water,NUMENTRIES_water)
        ->SetSpline(true);
  myMPT1->AddConstProperty("MIEHG_FORWARD",MIE_water_const[0]);
  myMPT1->AddConstProperty("MIEHG_BACKWARD",MIE_water_const[1]);
  myMPT1->AddConstProperty("MIEHG_FORWARD_RATIO",MIE_water_const[2]);

  H2O->SetMaterialPropertiesTable(myMPT1);

  // Set the Birks Constant for the Water scintillator

  H2O->GetIonisation()->SetBirksConstant(0.126*mm/MeV);


   name = "Etane";
   //  density = 0.509*g/cm3;
   density = 1.356*mg/cm3;
   C2H6 = new G4Material( name, density, nel=2);
   C2H6->AddElement(elC, nAtoms=2);
   C2H6->AddElement(elH, nAtoms=6);

   name = "Polyethylene";
   density = 0.93*g/cm3;
   CH2 = new G4Material( name, density, nel=2);
   CH2->AddElement(elC, nAtoms=1);
   CH2->AddElement(elH, nAtoms=2);

   density= universe_mean_density;
   Vacuum= new G4Material(name="Vacuum", z=1, a=1.01*g/mole, density, kStateGas, 300*kelvin, 3.e-18*pascal);
   //Vacuum= new G4Material(name="Vacuum", density, nel=2);
   //   Vacuum-> AddElement(elN, .7);
   //   Vacuum-> AddElement(elO, .3);
   //Vacuum->AddMaterial( N2Gas, .78 );
   //Vacuum->AddMaterial( O2Gas, .22 );

   name = "Air";
   density = 1.293*mg/cm3;
   Air = new G4Material( name, density, nel=2);
   Air->AddMaterial( N2Gas, .78 );
   Air->AddMaterial( O2Gas, .22 );
   

   name = "Ar+C2H6";
   density = (0.5*39.948+0.5*(2*12.011+6*1.00794))/22.3*mg/cm3;
   DCGas = new G4Material( name, density, nel=2);
   fractionMass = 39.948/(39.948+2*12.011+6*1.00794);
   DCGas->AddMaterial(Ar, fractionMass);
   fractionMass = (2*12.011+6*1.00794)/(39.948+2*12.011+6*1.00794);
   DCGas->AddMaterial(C2H6, fractionMass);


   name = "SUS 304"; 
   density = 8.03*g/cm3;
   sus304 = new G4Material( name, density, nel=3 );
   sus304->AddElement(elFe, .74 );
   sus304->AddElement(elCr, .18 );
   sus304->AddElement(elNi, .08 );

   name = "Concrete"; 
   density = 2.30*g/cm3;
   Concre = new G4Material( name, density, nel=6 );
   Concre->AddElement(elO,  .500 );
   Concre->AddElement(elNa, .011 );
   Concre->AddElement(elAl, .057 );
   Concre->AddElement(elSi, .273 );
   Concre->AddElement(elCa, .136 );
   Concre->AddElement(elFe, .023 );
   
   name = "quartz"; 
   density = 2.200*g/cm3;
   SiO2 = new G4Material( name, density, nel=2 );
   SiO2->AddElement(elSi, 1 );
   SiO2->AddElement(elO, 2 );
   
   name = "Aerogel";
   density = 0.200*mg/cm3;
   Aerog = new G4Material( name, density, nel=3);
   Aerog->AddMaterial( SiO2,.625 );
   Aerog->AddMaterial( H2O, .374 );
   Aerog->AddMaterial( C12,   .001 );
   
   name = "CarbonicGas";
   density = 1.98*mg/cm3;
   CO2 = new G4Material( name, density, nel=2,
         kStateGas, 300.*kelvin, 1.*atmosphere);
   CO2->AddElement( elC, 1 );
   CO2->AddElement( elO, 2 );

   name = "ArCO2Gas";
   density = 1.6723*mg/cm3;
   ArCO2 = new G4Material( name, density, nel=2,
         kStateGas, 300*kelvin, 1*atmosphere);
   ArCO2->AddMaterial( Ar,.7 );
   ArCO2->AddMaterial( CO2, .3 );
   
   name = "CF4Gas";
   density = 3.65*mg/cm3;
   CF4 = new G4Material( name, density, nel=2,
         kStateGas, 300*kelvin, 1*atmosphere);
   CF4->AddElement( elC, 1 );
   CF4->AddElement( elF, 4 );
  
   name = "PbO"; 
   //name = "LeadOxide"; 
   density = 9.53*g/cm3;
   PbO = new G4Material( name, density, nel=2 );
   PbO->AddElement(elPb, 1 );
   PbO->AddElement(elO, 1 );
   
   name = "K2O"; 
   //name = "PotassiumOxide"; 
   density = 2.35*g/cm3;
   K2O = new G4Material( name, density, nel=2 );
   K2O->AddElement(elK, 2 );
   K2O->AddElement(elO, 1 );
   
   name = "Na2O"; 
   //name = "SodiumOxide"; 
   density = 2.27*g/cm3;
   Na2O = new G4Material( name, density, nel=2 );
   Na2O->AddElement(elNa, 2 );
   Na2O->AddElement(elO, 1 );
  
   name = "Sb2O2"; 
   //name = "AntimonyOxide"; 
   density = 5.2*g/cm3;
   Sb2O2 = new G4Material( name, density, nel=2 );
   Sb2O2->AddElement(elSb, 2 );
   Sb2O2->AddElement(elO, 2 );

   name = "LG";
   density = 5.2*g/cm3;
   LG = new G4Material(name, density, nel=5);
   LG->AddMaterial(SiO2,0.273);
   LG->AddMaterial(PbO,0.709);
   LG->AddMaterial(K2O,0.009);
   LG->AddMaterial(Na2O,0.006);
   LG->AddMaterial(Sb2O2,0.003);

   name = "Kapton";//From advanced/lAr_calorimeter/src/FCALMaterialConsultant.cc
   density = 1.42*g/cm3;
   Kapton = new G4Material(name, density, nel=4);
   Kapton->AddElement(elH,0.0273);
   Kapton->AddElement(elC,0.7213);
   Kapton->AddElement(elN,0.0765);
   Kapton->AddElement(elO,0.1749);
   
   name = "Acrylic";
   density = 1.18*g/cm3;
   Acrylic = new G4Material(name , density, nel=3);
   Acrylic->AddElement(elO, 2);
   Acrylic->AddElement(elH, 8);
   Acrylic->AddElement(elC, 5);
   
   name = "G10";
   density = 1.70*g/cm3;
   G10 = new G4Material(name , density, nel=4);
   G10->AddElement(elSi, 1);
   G10->AddElement(elO, 2);
   G10->AddElement(elC, 3);
   G10->AddElement(elH, 3);

/***From http://www.phenix.bnl.gov/~suhanov/ncc/geant/rad-source/src/ExN03DetectorConstruction.cc***/
   //Epoxy (for FR4 )
   name = "Epoxy";//From http://www.physi.uni-heidelberg.de/~adler/TRD/TRDunterlagen/RadiatonLength/tgc2.htm 
   density = 1.2*g/cm3;
   Epoxy = new G4Material(name , density, nel=2);
   Epoxy->AddElement(elH, 2);
   Epoxy->AddElement(elC, 2);

   //FR4 (Glass + Epoxy)
   name = "FR4";
   density = 1.86*g/cm3;
   FR4 = new G4Material(name  , density, nel=2);
   FR4->AddMaterial(SiO2, 0.528);
   FR4->AddMaterial(Epoxy, 0.472);

}

E16G4_MaterialList::~E16G4_MaterialList()
{
   delete elH;
   delete  elLi;
   delete  elBe;
   delete  elB;
   delete  elC;
   delete  elN;
   delete  elO;
   delete  elF;
   delete  elNa;
   delete  elAl;
   delete  elSi;
   delete  elAr;
   delete  elCa;
   delete  elCr;
   delete  elMn;
   delete  elFe;
   delete  elCo;
   delete  elNi;
   delete  elCu;
   delete  elMo;
   delete  elW;
   delete  elSb;
   delete  elPb;
   delete  elK;

   delete  HeGas;
   delete  Ar;
   delete  ArCO2;
   delete  Myl;
   delete  Kevlar;
   delete  Eval;
   delete  Al;
   delete  W;
   delete  Ti;
   delete  Fe;
   delete  Ni;
   delete  Cu;
   delete  C12;
   delete  CFRP;
   delete  Si28;
   delete  V51;
   delete  Cr52;
   delete  Y89;
   delete  Li7;
   delete  Be9;
   delete  B10;
   delete  Ca40;
   delete  Pb208;
   delete  N2Gas;
   delete  O2Gas;
   delete  CF4;

   delete  Sci;
   delete  Aerog;
   delete  H2O;
   delete  C2H6;
   delete  CH2;

   delete  Vacuum;
   delete  Air;
   delete  DCGas;
   delete  Heavymet;
   delete  Harver;  
   delete  sus304;  
   delete  Concre;  
   delete  SiO2; 
   delete  CO2; 
   delete  K2O; 
   delete  Na2O; 
   delete  PbO; 
   delete  Sb2O2; 
   delete  LG; 
   delete  Kapton; 
   delete  Acrylic; 
   delete  G10; 
   delete  Epoxy; 
   delete  FR4; 
}
