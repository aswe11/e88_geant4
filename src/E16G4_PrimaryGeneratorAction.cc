//2016-09-23, uploaded by yokkaich
//2016-08-12, uploaded by nakai
//2015-09-05, uploaded by nakai
//2015-03-27, uploaded by yokkaich
//2015-01-20, uploaded by yokkaich
//2015-01-05, modified by yokkaich
//2015-01-05, modified by yokkaich
//2014-08-27, modified by kawama
//2014-08-27, modified by kawama
//2014-04-30, modified by kawama
//2013-11-14, modified by kawama
//2013-05-13, modified by kawama
// ====================================================================
//   E16G4_PrimaryGeneratorAction.cc
//
// ====================================================================
#include <G4ParticleGun.hh>
#include <G4ParticleTable.hh>
#include <G4ParticleDefinition.hh>
#include <G4Event.hh>
#include <G4RunManager.hh>

#include <G4LorentzVector.hh>

#include <cmath>
#define PI 3.14159265358979
#include <math.h>


#include <Randomize.hh>

using namespace CLHEP;


#include "E16G4_PrimaryGeneratorAction.hh"
#include "E16G4_PrimaryGeneratorMessenger.hh"
#include "E16G4_Analysis.hh"
#include "E16G4_SanfordWangGenerator.hh"

#include "E16ANA_ErrorMessage.hh"

// ====================================================================
//
// class description
//
// ====================================================================
E16G4_PrimaryGeneratorAction *E16G4_PrimaryGeneratorAction::thisPointer=0;

////////////////////////////////////////////////
 E16G4_PrimaryGeneratorAction::E16G4_PrimaryGeneratorAction
   (E16G4_Analysis* anaManager)
   :anaMan(anaManager)
////////////////////////////////////////////////
{
   //create a messenger for this class
   gunMessenger = new E16G4_PrimaryGeneratorMessenger(this);
   particleTable= G4ParticleTable::GetParticleTable();
   sw_gen = new E16G4_SanfordWangGenerator();

   thisPointer = this;
}

/////////////////////////////////////////////////
E16G4_PrimaryGeneratorAction::~E16G4_PrimaryGeneratorAction()
   /////////////////////////////////////////////////
{
   delete gunMessenger;
   delete sw_gen;
}

////////////////////////////////////////////////////////////////
void E16G4_PrimaryGeneratorAction::SetPrimaryConditions()
   ////////////////////////////////////////////////////////////////
{
  CLHEP::HepRandom::setTheSeed(rndmSeed);
  const char* defaultname= CLHEP::HepRandom::getTheEngine()->name().c_str();

  CLHEP::HepRandom::setTheEngine( new CLHEP::MTwistEngine(rndmSeed) );
  const char* newname= CLHEP::HepRandom::getTheEngine()->name().c_str();

  E16INFO("CLHEP Random Engine default : %s , -> %s is set", 
    defaultname, newname);

  bool flag=true;

  if(genID==1 ){//Obsolete: singletrack phi
    G4cout <<"Reading JAM Data File phi 1..."<<inputFileName<<G4endl; 
    flag=JAMDataRead_Phi(inputFileName);
  }
  else if(genID==11 ){
    G4cout <<"Reading JAM Data File phi 11..."<<inputFileName<<G4endl; 
    flag=JAMDataRead_Phi(inputFileName);//multitrack phi->ee
  }
  else if (genID==3){
   G4cout <<"Reading JAM Data File 3..."<<inputFileName<<G4endl; 
   flag=JAMDataRead(inputFileName);//Obsolete: 
  }
  else if(genID==4 ){//Obsolete: singletrack pion
   G4cout <<"Reading JAM Data File 4..."<<inputFileName<<G4endl; 
   flag=JAMDataRead_Pi0(inputFileName);//Obsolete: singletrack pion
  }
  else if(genID==14 || genID==24){
   G4cout <<"Reading JAM Data File 14/24..."<<inputFileName<<G4endl; 
   flag=JAMDataRead_Pi0_2(inputFileName);//multitrack pion 
  } else if (genID==1014) {//sako add new
   G4cout <<"Reading JAM Data File 1014..."<<inputFileName<<G4endl; 
   flag=JAMDataOpen_Pi0_2_Event(inputFileName);//multitrack pion 
  }
  if (!flag){
    exit(1);
  }

}

////////////////////////////////////////////////////////////////
void E16G4_PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
   ////////////////////////////////////////////////////////////////
{
  
   if ( genID == 0 ) {
      return GeneratePie( anEvent );
   } 
   else if (genID ==2) {
      return GeneratePrimaryBeam( anEvent );
   } 
   else if (genID ==7) {
     return GenerateMultiTrackRandom( anEvent );
   } 
   else if (genID ==8) {
     return GenerateMultiTrackRandom45( anEvent );
   } 
   else if (genID>=100 && genID<=132) {
      return GenerateWireTargetEvent(anEvent, genID-100);
   } else if (genID==1014) {//sako
      const G4int eventID=anEvent->GetEventID();
      if (eventID  < nEv-1) {
	bool flag=JAMDataRead_Pi0_2_Event(inputFileName);//multitrack pion 
	if (!flag){
	  exit(1);
	}
	return GenerateMultiTrackJAMEvent_Event( anEvent ); 
      } else {
         G4RunManager::GetRunManager()->AbortRun();
         G4cout<<"No events are remained in the File. AbortRun." <<G4endl;
      }//if eventID
   }
   else {// 1, 3, 4, 11, 14
      const G4int eventID=anEvent->GetEventID();
      if (eventID  < nEv-1) {
         if ( genID == 11 || genID == 14) {
            return GenerateMultiTrackJAMEvent( anEvent ); 
         } else if (genID == 24) {
            return GenerateMultiTrackJAMEvent( anEvent , true ); 
         } else {
            return GenerateJAMEvent( anEvent );
         }
      } else {
         G4RunManager::GetRunManager()->AbortRun();
         G4cout<<"No events are remained in the File. AbortRun." <<G4endl;
      }//if eventID
   }//if genID
}

////////////////////////////////////////////////////////////////
void E16G4_PrimaryGeneratorAction::GeneratePie(G4Event* anEvent)
   ////////////////////////////////////////////////////////////////
{
   G4ParticleTable* particleTable= G4ParticleTable::GetParticleTable();
   G4ParticleDefinition* partA;
   G4ParticleDefinition* partB;
   int flag=0;
   const int n_part=2;
   if (RandFlat::shoot(0., 1.)>0.5) flag=1;
   G4ThreeVector gMom[n_part];
   G4ThreeVector gPos[n_part];
   G4int Pdg[n_part];

   switch (flag){
      case 1:
         partA= particleTable-> FindParticle("e-");
         partB= particleTable-> FindParticle("e+");
         break;

      default:
         partA= particleTable-> FindParticle("pi+");
         partB= particleTable-> FindParticle("pi-");
         break;
   }
   const G4String pNameA=partA->GetParticleName();
   const G4String pNameB=partB->GetParticleName();

   G4double mass = partA->GetPDGMass()/MeV;
   Pdg[0] = partA->GetPDGEncoding();
   Pdg[1] = partB->GetPDGEncoding();

   double u= RandFlat::shoot(0., 1.);
   double v= RandFlat::shoot(0., 1.);
   double theta= acos(1.0-2.0*u);
   double phi= 2*PI*v;
   //double E=500.;

   G4double ele_p= RandFlat::shoot(0.5,1.5)*GeV;
   G4double E=sqrt(ele_p*ele_p+mass*mass);
   //G4double ele_p= sqrt((E*E)-(massA*massA))*MeV;
   //G4double ele_p= 200.*MeV;
   /*G4double ele_px= (G4double)(sin(theta)*cos(phi))*ele_p;
     G4double ele_py= (G4double)(sin(theta)*sin(phi))*ele_p;
     G4double ele_pz= (G4double)(cos(theta))*ele_p;*/
   G4double ele_px= (G4double)(sin(theta)*sin(phi))*ele_p;
   G4double ele_py= (G4double)(cos(theta))*ele_p;
   G4double ele_pz= (G4double)(sin(theta)*cos(phi))*ele_p;
   G4double pos_px= -ele_px;
   G4double pos_py= -ele_py;
   G4double pos_pz= -ele_pz;

   G4LorentzVector* rest_4momentum= new G4LorentzVector
      (ele_px, ele_py, ele_pz, E*MeV);
   double gamma= RandFlat::shoot(1., 3.);
   //double gamma= 1.2;
   //G4cout<< ele_px<< " "<< ele_py<< " "<< ele_pz<< G4endl;
   G4LorentzVector* boosted_4momentum= new G4LorentzVector;
   *boosted_4momentum= rest_4momentum-> boostZ(sqrt(1.-1./gamma/gamma));
   ele_px= boosted_4momentum-> getX();
   ele_py= boosted_4momentum-> getY();
   ele_pz= boosted_4momentum-> getZ();

   /*Double_t momentum_e[3];
     momentum_e[0]= ele_px/GeV;
     momentum_e[1]= ele_py/GeV;
     momentum_e[2]= ele_pz/GeV;*/
   //G4cout<< ele_px<< " "<< ele_py<< " "<< ele_pz<< G4endl;

   //G4cout<< pos_px<< " "<< pos_py<< " "<< pos_pz<< G4endl;
   rest_4momentum= new G4LorentzVector
      (pos_px, pos_py, pos_pz, E*MeV);
   *boosted_4momentum= rest_4momentum-> boostZ(sqrt(1.-1./gamma/gamma));
   pos_px= boosted_4momentum-> getX();
   pos_py= boosted_4momentum-> getY();
   pos_pz= boosted_4momentum-> getZ();

   /*Double_t momentum_p[3];
     momentum_p[0]= pos_px/GeV;
     momentum_p[1]= pos_py/GeV;
     momentum_p[2]= pos_pz/GeV;*/
   //G4cout<< pos_px<< " "<< pos_py<< " "<< pos_pz<< G4endl;

   eGun= new G4ParticleGun(1);
   pGun= new G4ParticleGun(1);
   //Electron Generation
   G4double x= RandGauss::shoot(0.,sigmaX/cm);//Generate Position in cm
   G4double y= RandGauss::shoot(0.,sigmaY/cm);//Generate Position in cm
   G4double z= RandGauss::shoot(0.,sigmaZ/cm);//Generate Position in cm
   gPos[0]=G4ThreeVector(x*cm,y*cm,z*cm);
   gMom[0]=G4ThreeVector(ele_px, ele_py, ele_pz);
   eGun-> SetParticleEnergy(0.*MeV);
   eGun-> SetParticleDefinition(partA);
   //eGun-> SetParticleDefinition(piminus);
   eGun-> SetParticlePosition(gPos[0]);
   eGun-> SetParticleMomentum(gMom[0]);
   eGun-> GeneratePrimaryVertex(anEvent);

   //Positron Generation
   gPos[1]=G4ThreeVector(x*cm,y*cm,z*cm);
   gMom[1]=G4ThreeVector(pos_px, pos_py, pos_pz);
   pGun-> SetParticleEnergy(0.*MeV);
   pGun-> SetParticleDefinition(partB);
   //pGun-> SetParticleDefinition(piplus);
   pGun-> SetParticlePosition(gPos[1]);
   pGun-> SetParticleMomentum(gMom[1]);
   pGun-> GeneratePrimaryVertex(anEvent);

   if( anaMan ){
      anaMan->PrimaryGeneration( n_part, Pdg, gPos, gMom, iev );
   }
   delete pGun;
   delete eGun;


}


////////////////////////////////////////////////////////////////////
void E16G4_PrimaryGeneratorAction::GenerateJAMEvent( G4Event *anEvent )
   ////////////////////////////////////////////////////////////////////
{
   const G4int eid=anEvent->GetEventID();
   G4double x=0., y=0., z=0.,px,py,pz;
   const G4int n_part = 1;
   G4ThreeVector gMom[n_part];
   G4ThreeVector gPos[n_part];
   G4int Pdg[n_part], pid;

   int JAMOffset=evOffset;
   int eventOffset=0;
   int m,n;
   if(JAMOffset>0){
      for (int i=0;i<JAMOffset;i++){
         eventOffset+=nmul_[i];
         //eventOffset+=2;
      }
   }

   int mul_sum=0;
   for (int i=JAMOffset;i<nev_;i++){
      if(eid<mul_sum+nmul_[i]){
         n=i;
         m=eid-mul_sum;
         break;
      }
      mul_sum+=nmul_[i];
   }   

   G4cerr<<"m= "<<m<<" "<<mul_sum<<" "<<iev_[n]<<" "<<mJAM_[n][0]<<G4endl;

   /*G4cout<<"n="<<n<<",m="<<m<<G4endl; 

     G4cout <<
     xJAM_[n]<<" "<<
     yJAM_[n]<<" "<<
     pxJAM_[n][m]<<" "<<
     pyJAM_[n][m]<<" "<<
     pzJAM_[n][m]<<" "<<
     mJAM_[n][m]<<" "<<
     pidJAM_[n][m]<<G4endl;*/

   x=xJAM_[n];
   y=yJAM_[n];
   z=zPoint/cm+RandFlat::shoot(-sigmaZ/cm,sigmaZ/cm);//Generate Position in cm
   //G4cout <<"Generation z:"<< z <<G4endl;
   px=pxJAM_[n][m];
   py=pyJAM_[n][m];
   pz=pzJAM_[n][m];
   pid=pidJAM_[n][m];


   G4ParticleDefinition* particle= particleTable-> FindParticle(pid);
   const G4String pName=particle->GetParticleName();
   //G4cout <<"m="<<m<<" "<< PDG<<" "<<pName<<G4endl;
   gMom[0]=G4ThreeVector( px*GeV, py*GeV,pz*GeV);  
   gPos[0]=G4ThreeVector( x*cm, y*cm,z*cm);  
   Pdg[0]=pid;
   G4ParticleGun* particleGun = new G4ParticleGun(n_part);
   particleGun-> SetParticleEnergy(0.0*GeV); //reset
   particleGun-> SetParticleDefinition(particle); 
   particleGun-> SetParticleMomentum(gMom[0]);
   particleGun-> SetParticlePosition(gPos[0]);
   particleGun-> GeneratePrimaryVertex(anEvent);

   if( anaMan ){
      anaMan->PrimaryGeneration( n,m,Pdg, gPos, gMom, eid );
   }
   delete particleGun;

}


////////////////////////////////////////////////////////////////////
void E16G4_PrimaryGeneratorAction::GeneratePrimaryBeam( G4Event *anEvent )
   ////////////////////////////////////////////////////////////////////
{
   //const G4int eid=anEvent->GetEventID();

   //G4double x=0., y=0., z=-1.;
   G4double x=0., y=0., z=-200.;
   const G4int n_part = 1;
   G4ThreeVector gMom[n_part];
   G4ThreeVector gPos[n_part];
   G4int Pdg[n_part];

   G4ParticleDefinition* particle= particleTable-> FindParticle("proton");
   //G4ParticleDefinition* particle= particleTable-> FindParticle("geantino");
   const G4String pName=particle->GetParticleName();
   //gMom[0]=G4ThreeVector( 0.*GeV, 0.*GeV,12.94*GeV);  
   gMom[0]=G4ThreeVector( 0.*GeV, 0.*GeV,30.94*GeV);  
   x= RandGauss::shoot(0.,sigmaX/cm);//Generate Position in cm
   y= RandGauss::shoot(0.,sigmaY/cm);//Generate Position in cm
   gPos[0]=G4ThreeVector( x*cm, y*cm,z*cm);  
   Pdg[0]=2212;     //proton

   G4ParticleGun* particleGun = new G4ParticleGun(n_part);
   particleGun-> SetParticleEnergy(0.0*GeV); //reset
   particleGun-> SetParticleDefinition(particle); 
   particleGun-> SetParticleMomentum(gMom[0]);
   particleGun-> SetParticlePosition(gPos[0]);
   particleGun-> GeneratePrimaryVertex(anEvent);


   if( anaMan ){
      anaMan->PrimaryGeneration( n_part, Pdg, gPos, gMom, iev );
   }
   delete particleGun;

}

bool E16G4_PrimaryGeneratorAction::JAMDataRead_Phi
(G4String inputFilename){
#if 0
   char s[256];
   char proj[10];
   char targ[10];
   char cwin[10];
   char frame[10];
   int iev, imul, nv, nbary, nmeson, mseed, mevent;
   float bmin, bmax, dt, nstep;
   float ptot;
   float p[5],r[5],v[4];
   int k[9];
   int iev, imul;
   int ip,im;
#endif


   int n=0;
   FILE* fpi;
   fpi=fopen(inputFilename.c_str(),"rb");
   if (fpi==NULL ){
     E16FATAL("cannot open:%s, exit(1) ",inputFilename.c_str());
     exit(1);
      return false;
   }
   //Run header
   //fread(&nev_, sizeof(nev_),  1,fpi);
   //G4cout<<nev_<<G4endl;
   nev_=1000000;
   iev_= new int[nev_];
   nmul_= new int[nev_];
   /*nv_= new int[nev_];
     nbary_= new int[nev_];
     nmeson_= new int[nev_];
     mseed_= new int[nev_];
     mevent_= new int[nev_];*/
   xJAM_ =new float[nev_];
   yJAM_ =new float[nev_];
   pxJAM_=new double*[nev_];
   pyJAM_=new double*[nev_];
   pzJAM_=new double*[nev_];
   mJAM_=new double*[nev_];
   pidJAM_=new int*[nev_];
   while ( n<nev_){
      //float x= RandFlat::shoot(-.1,.1);//Generate Position in cm
      //float y= RandFlat::shoot(-.1,.1);//Generate Position in cm
      double x= RandGauss::shoot(0.,sigmaX/cm);//Generate Position in cm
      double y= RandGauss::shoot(0.,sigmaY/cm);//Generate Position in cm
      iev_[n]=n;
      nmul_[n]=2;
      xJAM_[n]=x;
      yJAM_[n]=y;
      //G4cout <<iev_[n]<<" "<< nmul_[n]<<" "<< nv_[n]<<" "<< nbary_[n]<<
      //" "<< nmeson_[n]<< " "<< mseed_[n]<<" "<< mevent_[n]<<G4endl;
      pxJAM_[n]=new double[2];
      pyJAM_[n]=new double[2];
      pzJAM_[n]=new double[2];
      mJAM_[n]=new double[2];
      pidJAM_[n]=new int[2];
      size_t sizepx;
      size_t sizepy;
      size_t sizepz;
      size_t sizem;
      size_t sizeid;
      for (int i=0;i<2;i++){
         sizepx=fread(&pxJAM_[n][i], sizeof(double),  1,fpi);
         sizepy=fread(&pyJAM_[n][i], sizeof(double),  1,fpi);
         sizepz=fread(&pzJAM_[n][i], sizeof(double),  1,fpi);
         sizem=fread(&mJAM_[n][i], sizeof(double),  1,fpi);
         sizeid=fread(&pidJAM_[n][i], sizeof(int),  1,fpi);
         //G4cout <<" n="<<n<<" "<<sizepx<<G4endl;
         /*G4cout <<" n="<<n<<" "<<pxJAM_[n][i]
           <<" "<<pyJAM_[n][i]<<" "<<pzJAM_[n][i]<<" "
           <<mJAM_[n][i]<<" "<<pidJAM_[n][i]<<" "
           <<G4endl;*/
         if(isnan(pxJAM_[n][i]) || isnan(pyJAM_[n][i]) || isnan(pzJAM_[n][i])){
            pxJAM_[n][i]=0;
            pyJAM_[n][i]=0;
            pzJAM_[n][i]=0;
            mJAM_[n][i]=0;
            pidJAM_[n][i]=22;
         }
      
      }
      if(sizepx<1 || sizepy<1 || sizepz<1 || sizem<1 || sizeid<1 ) break;
      n++;
   }
   nEv=n;
   G4cout <<"Read "<<nEv<<" events"<<G4endl;

   fclose(fpi);

   return true;
}

bool E16G4_PrimaryGeneratorAction::JAMDataRead_Pi0
(G4String inputFilename){
#if 0
   char s[256];
   char proj[10];
   char targ[10];
   char cwin[10];
   char frame[10];
   float bmin, bmax, dt, nstep;
   float ptot;
   float p[5],r[5],v[4];
   int k[9];
   int iev, imul, nv, nbary, nmeson, mseed, mevent;
   int iev, imul;
   int ip,im;
#endif


   int n=0;
   FILE* fpi;
   fpi=fopen(inputFilename.c_str(),"rb");
   if (fpi==NULL ){
     E16FATAL("cannot open:%s, exit(1) ",inputFilename.c_str());
     exit(1);
     //      G4cout <<"Error!!!!! No input file!"<<G4endl;
   }
   //Run header
   //fread(&nev_, sizeof(nev_),  1,fpi);
   //G4cout<<nev_<<G4endl;
   nev_=100000;
   iev_= new int[nev_];
   nmul_= new int[nev_];
   /*nv_= new int[nev_];
     nbary_= new int[nev_];
     nmeson_= new int[nev_];
     mseed_= new int[nev_];
     mevent_= new int[nev_];*/
   xJAM_ =new float[nev_];
   yJAM_ =new float[nev_];
   impJAM_ =new double[nev_];
   pxJAM_=new double*[nev_];
   pyJAM_=new double*[nev_];
   pzJAM_=new double*[nev_];
   mJAM_=new double*[nev_];
   pidJAM_=new int*[nev_];
   while ( n<nev_){
      //float x= RandFlat::shoot(-.1,.1);//Generate Position in cm
      //float y= RandFlat::shoot(-.1,.1);//Generate Position in cm
      double x= RandGauss::shoot(0.,sigmaX/cm);//Generate Position in cm
      double y= RandGauss::shoot(0.,sigmaY/cm);//Generate Position in cm
      /*iev_[n]=n;
      nmul_[n]=1;
      xJAM_[n]=x;
      yJAM_[n]=y;*/
      //G4cout <<iev_[n]<<" "<< nmul_[n]<<" "<< nv_[n]<<" "<< nbary_[n]<<
      //" "<< nmeson_[n]<< " "<< mseed_[n]<<" "<< mevent_[n]<<G4endl;
      int iev,pidJAM;
      double impJAM,pxJAM,pyJAM,pzJAM,mJAM;
      size_t sizeev=fread(&iev, sizeof(int),  1,fpi);
      size_t sizeimp=fread(&impJAM, sizeof(double),  1,fpi);
      size_t sizepx=fread(&pxJAM, sizeof(double),  1,fpi);
      size_t sizepy=fread(&pyJAM, sizeof(double),  1,fpi);
      size_t sizepz=fread(&pzJAM, sizeof(double),  1,fpi);
      size_t sizem=fread(&mJAM, sizeof(double),  1,fpi);
      size_t sizeid=fread(&pidJAM, sizeof(int),  1,fpi);
      /*G4cout 
         <<" "<<iev
         <<" "<<impJAM
         <<" "<<pxJAM
         <<" "<<pxJAM
        <<" "<<pyJAM<<" "<<pzJAM<<" "
        <<mJAM<<" "<<pidJAM<<G4endl;*/
      if(sizepx<1 || sizepy<1 || sizepz<1 || sizem<1 || sizeid<1 ) break;

      //      std::cerr<<"JAM pi "<<n<<" "<<iev<<" "<<impJAM<<std::endl;

      nmul_[n]=1;
      xJAM_[n]=x;
      yJAM_[n]=y;
      pxJAM_[n]=new double[2];
      pyJAM_[n]=new double[2];
      pzJAM_[n]=new double[2];
      mJAM_[n]=new double[2];
      pidJAM_[n]=new int[2];
      iev_[n]=iev;
      impJAM_[n]=impJAM;
      pxJAM_[n][0]=pxJAM;
      pyJAM_[n][0]=pyJAM;
      pzJAM_[n][0]=pzJAM;
      mJAM_[n][0]=mJAM;
      pidJAM_[n][0]=pidJAM;
      n++;
   }
   nEv=n;
   G4cout <<"Read "<<nEv<<" events"<<G4endl;

   fclose(fpi);

   return true;
}

/*void E16G4_PrimaryGeneratorAction::JAMDataWrite
  (char* inputFilename, char* outputFilename){
  }*/
bool E16G4_PrimaryGeneratorAction::JAMDataRead
(G4String inputFilename){
#if 0
   char s[256];
   char frame[10];
   int iev, imul, nv, nbary, nmeson, mseed, mevent;
   int ip,im;
   float bmin, bmax, dt, nstep;
   float ptot;
   float p[5],r[5],v[4];
   int k[9];
#endif
   char proj[10];
   char targ[10];
   char cwin[10];

   int n=0;
   FILE* fpi;
   fpi=fopen(inputFilename.c_str(), "rb");
   if (fpi==NULL ){
     E16FATAL("cannot open:%s, exit(1) ",inputFilename.c_str());
     exit(1);
      return false;
   }
   //Run header
   fread(&nev_, sizeof(nev_),  1,fpi);
   fread(proj, sizeof(proj),  1,fpi);
   fread(targ, sizeof(targ),  1,fpi);
   fread(cwin, sizeof(cwin),  1,fpi);
   fread(&bmin_, sizeof(bmin_),  1,fpi);
   fread(&bmax_, sizeof(bmax_),  1,fpi);
   fread(&nevtotal_, sizeof(nevtotal_),  1,fpi);
   projectile_=G4String(proj);
   target_=G4String(targ);
   cwin_=G4String(cwin);
   G4cout<<nev_<<" "<<projectile_<<" "<<target_<<" "<<cwin_<<" "<<bmin_
      <<" "<<bmax_<<" "<<nevtotal_<<G4endl;
   iev_= new int[nev_];
   nmul_= new int[nev_];
   nv_= new int[nev_];
   nbary_= new int[nev_];
   nmeson_= new int[nev_];
   mseed_= new int[nev_];
   mevent_= new int[nev_];
   xJAM_ =new float[nev_];
   yJAM_ =new float[nev_];
   pxJAM_=new double*[nev_];
   pyJAM_=new double*[nev_];
   pzJAM_=new double*[nev_];
   //EkJAM_=new float*[nev_];
   mJAM_=new double*[nev_];
   pidJAM_=new int*[nev_];
   while ( n<nev_){
      //float x= RandFlat::shoot(-.1,.1);//Generate Position in cm
      //float y= RandFlat::shoot(-.1,.1);//Generate Position in cm
      float x= RandGauss::shoot(0.,.1);//Generate Position in cm
      float y= RandGauss::shoot(0.,.1);//Generate Position in cm
      //Event header
      fread(&iev_[n], sizeof(iev_[n]),  1,fpi);
      fread(&nmul_[n], sizeof(nmul_[n]),  1,fpi);
      /*fread(&nv_[n], sizeof(nv_[n]),  1,fpi);
        fread(&nbary_[n], sizeof(nbary_[n]),  1,fpi);
        fread(&nmeson_[n], sizeof(nmeson_[n]),  1,fpi);
        fread(&mseed_[n], sizeof(mseed_[n]),  1,fpi);
        fread(&mevent_[n], sizeof(mevent_[n]),  1,fpi);*/
      xJAM_[n]=x;
      yJAM_[n]=y;
      //G4cout <<iev_[n]<<" "<< nmul_[n]<<G4endl;
      pxJAM_[n]=new double[nmul_[n]];
      pyJAM_[n]=new double[nmul_[n]];
      pzJAM_[n]=new double[nmul_[n]];
      mJAM_[n]=new double[nmul_[n]];
      pidJAM_[n]=new int[nmul_[n]];
      for (int i=0;i<nmul_[n];i++){
         fread(&pxJAM_[n][i], sizeof(double),  1,fpi);
         fread(&pyJAM_[n][i], sizeof(pyJAM_[n][i]),  1,fpi);
         fread(&pzJAM_[n][i], sizeof(pzJAM_[n][i]),  1,fpi);
         fread(&mJAM_[n][i], sizeof(mJAM_[n][i]),  1,fpi);
         fread(&pidJAM_[n][i], sizeof(pidJAM_[n][i]),  1,fpi);
         /*G4cerr <<"iev_[n]="<<iev_[n]<<" "<<pxJAM_[n][i]
           <<" "<<pyJAM_[n][i]<<" "<<pzJAM_[n][i]<<" "
           <<mJAM_[n][i]<<" "<<pidJAM_[n][i]<<G4endl;*/
      }
      n++;
   }


   fclose(fpi);

   return true;
}
void E16G4_PrimaryGeneratorAction::JAMDataWrite
(char* inputFilename, char* outputFilename){
}


#include "E16G4_PrimaryGeneratorAction2.cc"
#include "E16G4_PrimaryGeneratorAction3.cc"
