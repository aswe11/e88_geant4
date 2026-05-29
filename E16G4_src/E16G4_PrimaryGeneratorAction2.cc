//2016-09-23, uploaded by yokkaich
//2016-08-12, uploaded by nakai
//2015-08-30, uploaded by yokkaich
//2015-03-27, uploaded by yokkaich
//E16G4_PrimaryGeneratorAction2.cc 150223 by S. Yokkaichi
//    Last modified at <2016-09-21 23:41:16 >
//

// to be merged to E16G4_PrimaryGeneratorAction.cc
//

#include <vector>
#include "G4IonTable.hh"//sako
//#include "G4ParticleTable.hh"//sako
using namespace std;


void E16G4_PrimaryGeneratorAction::ParticleTableTest(){
   // pid=0 cannot be find by FindParticle(0), by the code
   // of the class G4ParticleTable

  //dump
	int max = particleTable->entries(); 
	cerr<<"particleTable max="<<max<<endl;
	for(int j=0; j<max; j++){
	  G4ParticleDefinition* p=particleTable->GetParticle(j);
	  cerr<<" "<<j<<" "<<p->GetParticleName();
	  cerr<<" "<<p->GetPDGEncoding() <<endl;
	}
 //try to remove and find

     //     G4ParticleDefinition* g1= particleTable-> FindParticle(22);
     //G4ParticleDefinition* g2= particleTable-> FindParticle("geantino");

     //     cerr<<" "<<g1->GetParticleName() <<endl;
     //     cerr<<" "<<g2->GetParticleName() <<endl;

     G4ParticleDefinition* particle= particleTable-> FindParticle( G4int(0)  );
     //-> cannot find 0(geantino) . return null

     if( particle == NULL ){
       cerr<<"try remove....";
       G4ParticleDefinition* g3= particleTable-> FindParticle("chargedgeantino");
       G4ParticleDefinition* g4=particleTable->Remove(g3);

	int max = particleTable->entries(); 
	cerr<<"particleTable max="<<max<<endl;
	for(int j=0; j<max; j++){
	  G4ParticleDefinition* p=particleTable->GetParticle(j);
	  cerr<<" "<<j<<" "<<p->GetParticleName();
	  cerr<<" "<<p->GetPDGEncoding() <<endl;
	}

	G4ParticleDefinition* p= particleTable-> FindParticle( G4int(0) );
       if( p == NULL ){
	 cerr<<"...removed,  but fail to find ...exit()";
	 cerr<<g3<<" "<<g4<<endl;
	 exit(1);
       }
       else{
	 cerr<<"...get "<<p->GetParticleName()<<endl;
	 particle = p;
       }
     }
  
}

////////////////////////////////////////////////////////////////////
void E16G4_PrimaryGeneratorAction::GenerateMultiTrackRandom( 
  G4Event *anEvent,   bool flag45)
   ////////////////////////////////////////////////////////////////////
{
   const G4int eventID=anEvent->GetEventID();
   G4double x=0., y=0., z=0.;
   const int vsize=64;

   vector<int> Pdg;            Pdg.reserve(vsize);
   vector<G4ThreeVector> gMom; gMom.reserve(vsize);
   vector<G4ThreeVector> gPos; gPos.reserve(vsize);

   int n=eventID;
   int multi = randomMulti;

   E16MESSAGE("ev/multi=%5i %3i",eventID, multi);

#if 0
   x= RandGauss::shoot(0.,sigmaX/cm);//Generate Position in cm
   y= RandGauss::shoot(0.,sigmaY/cm);//Generate Position in cm
   z= zPoint/cm + 
     RandFlat::shoot(-sigmaZ/cm, sigmaZ/cm);//Generate Position in cm
#else
   x=0;y=0;z=0;  
#endif

   G4ThreeVector generatePosition( x*cm, y*cm,z*cm);

   double px, py,pz, costheta, sintheta, phi, temp;  
   const double twoPI = PI*2;

   int pid=0;//geantino
   G4ParticleDefinition* geantino= particleTable-> FindParticle("geantino");
   //int pid=11;//e-

   //   ParticleTableTest();
   // pid=0 cannot be find by FindParticle(), by the code
   // of the class G4ParticleTable

   for(int i=0; i< multi ; i++ ){ //track loop in an event
     phi      = RandFlat::shoot(twoPI);
     temp     = RandFlat::shoot(-1, 1);
     costheta = flag45 ? 0.707*temp : temp;
     sintheta = sqrt( 1-costheta*costheta );
     //conversion: z -> y, y->x, x->z
     pz= sintheta*cos(phi);//x
     px= sintheta*sin(phi);//y
     py= costheta;//z  : if flag45 is true, dip angle less than 45 deg.

     //     cerr<<"random "<<pid<<" "<<phi<<" "<<temp<<endl;

     G4ParticleDefinition* particle= geantino;


     gMom.push_back( G4ThreeVector( px*GeV, py*GeV,pz*GeV) );
     gPos.push_back( generatePosition );
     Pdg.push_back(  pid  );

     //     E16MESSAGE("pdg pushint  %i %i %i %i:  %i %i ",i, pid, test[i],
     //		     test.size(),
     //		     Pdg[0],Pdg[1]);
     
     //     particleGun = 

     G4ParticleGun* particleGun=new G4ParticleGun(1);
     particleGun-> SetParticleEnergy(0.0*GeV); //reset
     particleGun-> SetParticleDefinition(particle); 
     particleGun-> SetParticleMomentum(gMom[i]);
     particleGun-> SetParticlePosition(gPos[i]);
     particleGun-> GeneratePrimaryVertex(anEvent);
     delete particleGun;
   }

   if( anaMan ){
      anaMan->PrimaryGenerationMulti( n,multi, Pdg, gPos, gMom, eventID );
   }

   //   delete particleGun;

}
void E16G4_PrimaryGeneratorAction::GenerateMultiTrackJAMEvent( G4Event *anEvent, bool wire)
   ////////////////////////////////////////////////////////////////////
{
   const G4int eventID=anEvent->GetEventID();
   G4double x=0., y=0., z=0.;
   const int vsize=64;

   vector<int> Pdg;            Pdg.reserve(vsize);
   vector<G4ThreeVector> gMom; gMom.reserve(vsize);
   vector<G4ThreeVector> gPos; gPos.reserve(vsize);

   int n=eventID;
   //   int m=multiplicity;
   int multi = nmul_[n];
   cout << "multi = " << multi << endl;
   
      E16MESSAGE("ev/multi=%5i %3i",eventID, multi);

   if(!wire){
      x= RandGauss::shoot(0.,sigmaX/cm);//Generate Position in cm
      y= RandGauss::shoot(0.,sigmaY/cm);//Generate Position in cm
      z= zPoint/cm + 
        RandFlat::shoot(-sigmaZ/cm, sigmaZ/cm);//Generate Position in cm
   }else{
      double u = RandFlat::shoot(0., (sigmaZ/cm)*(sigmaZ/cm));
      G4double theta = RandFlat::shoot(0.0, 360.0)*degree;
      G4double r = sqrt(u);
      x = r*sin(theta/rad)+xPoint/cm; // in cm
      y = RandGauss::shoot(yPoint/cm, sigmaY/cm); // in cm
      z = r*cos(theta/rad)+zPoint/cm; // in cm
   }
   G4ThreeVector generatePosition( x*cm, y*cm,z*cm);

   double px, py,pz;  int pid;

   for(int i=0; i< multi ; i++ ){ //track loop in an event

     //G4cout <<"Generation z:"<< z <<G4endl;
     px=pxJAM_[n][i];
     py=pyJAM_[n][i];
     pz=pzJAM_[n][i];
     //     cout << "px, py, pz = " << px << " " << py << " " << pz << endl;
     //     cout << "pidJAM_["<<n<<"]["<<i<<"] = " << endl;
     //     cout << pidJAM_[n][i] << endl;
     pid = pidJAM_[n][i];

     //     G4IonTable *ionTable = G4ParticleTable::GetParticleTable()->GetIonTable();
     G4IonTable *ionTable = G4IonTable::GetIonTable();

     G4ParticleDefinition* particle= 0;
     
     if (pid>1000000) {

       G4int iz = pid/1000000;
       G4int in = (pid%1000000)/1000;
       G4int iy = pid%1000;
       G4int iA = iz+in+iy;
       G4int lvl=0;
       G4cout << "iz,in,iy,iA = " << iz << " " << in << " " << iy << " " << iA << G4endl;

       if (iy!=0) {
	 G4cout << "iy!=0" << G4endl;
	 particle = ionTable->GetIon(iz,iA,iy,lvl);//this cause a problem for hypernuclei
       } else {
	 particle = ionTable->GetIon(iz,iA,iy,lvl);//this cause a problem for hypernuclei
       }

       //       continue;
     } else {
       particle = particleTable-> FindParticle(pid);
     }
     //def        particle = particleTable-> FindParticle(pid);
     const G4String pName=particle->GetParticleName();

     gMom.push_back( G4ThreeVector( px*GeV, py*GeV,pz*GeV) );
     gPos.push_back( generatePosition );
     Pdg.push_back(  pid  );

     //     E16MESSAGE("pdg pushint  %i %i %i %i:  %i %i ",i, pid, test[i],
     //		     test.size(),
     //		     Pdg[0],Pdg[1]);
     
     //     particleGun = 
     G4ParticleGun* particleGun=new G4ParticleGun(1);
     particleGun-> SetParticleEnergy(0.0*GeV); //reset
     particleGun-> SetParticleDefinition(particle); 
     particleGun-> SetParticleMomentum(gMom[i]);
     particleGun-> SetParticlePosition(gPos[i]);
     particleGun-> GeneratePrimaryVertex(anEvent);
     delete particleGun;
   }

   if( anaMan ){
      anaMan->PrimaryGenerationMulti( n,multi, Pdg, gPos, gMom, eventID );
   }

   //   delete particleGun;

}


bool E16G4_PrimaryGeneratorAction::JAMDataRead_Pi0_2
(G4String inputFilename){
  //JAM kawama format, track-by-track, event infomation is only eventID

   FILE* fpi;
   fpi=fopen(inputFilename,"rb");
   if (fpi==NULL ){
     E16FATAL("cannot open:%s, exit(1) ",inputFilename.c_str());
     exit(1);
      return false;
   }

   int nev_=100000;
   iev_= new int[nev_];
   nmul_= new int[nev_];
   impJAM_ =new double[nev_];
   pxJAM_=new double*[nev_];
   pyJAM_=new double*[nev_];
   pzJAM_=new double*[nev_];
   mJAM_=new double*[nev_];
   pidJAM_=new int*[nev_];

   double buffer[5];//impact param, px,py,pz,mass

   int nevent=0, ntrack=0, prevID=0; 
   //def   const int NtrackMAX=64;
   //sako 2019/10/5
   //   const int NtrackMAX=2000;

   int eventIDJAM=0;
   size_t size1=fread(&eventIDJAM, sizeof(int),  1,fpi);
   //   cerr<<"first evidjam "<<eventIDJAM<<endl;
   int mode=1;//current position
   fseek(fpi, -1*sizeof(int), mode);
   prevID = eventIDJAM;

   for(int i=0;i<nev_;i++){
      pxJAM_[i]=new double[NtrackMAX];
      pyJAM_[i]=new double[NtrackMAX];
      pzJAM_[i]=new double[NtrackMAX];
      mJAM_ [i]=new double[NtrackMAX];
      pidJAM_[i]=new int[NtrackMAX];

      ntrack=0;
      while ( ntrack < NtrackMAX){

	int pidJAM;
	size_t size1=fread(&eventIDJAM, sizeof(int),  1,fpi);
	cerr<<"evidjam, ntrack "<<eventIDJAM<< " " << ntrack <<endl;

	if(size1 != 1) {goto label;}
	if(eventIDJAM != prevID){ //should go next event
	  //	  cerr << "eventIDJAM != prevID = " << eventIDJAM << " " << prevID << endl;
	  prevID=eventIDJAM;
	  nmul_[i]=ntrack;
	  fseek(fpi, -1*sizeof(int), mode);
	  break;
	}

	size_t size2  =fread(&buffer,  sizeof(double),  5,fpi);
	size_t size3=fread(&pidJAM, sizeof(int),  1,fpi);
	//cerr<<"evidjam size2/3" <<eventIDJAM<<" "<<size2<<" "<<size3<<endl;
	if(size2!=5 || size3!=1){goto label;}

	 iev_[i]=eventIDJAM;
	 impJAM_[i]=buffer[0];

	 pxJAM_[i][ntrack]=buffer[1];
	 pyJAM_[i][ntrack]=buffer[2];
	 pzJAM_[i][ntrack]=buffer[3];
	 mJAM_[i][ntrack] =buffer[4];
	pidJAM_[i][ntrack]=pidJAM;
	//	cerr<<"ntrack, iev, b, px, py, pz, m, pid = " << ntrack << " " << eventIDJAM<< " " << buffer[0]
	//	    << " " << buffer[1] << " " << buffer[2] << " " << buffer[3] << " " << buffer[4] << " " << pidJAM << endl;

	ntrack++;
      }//while ntrack :track loop in an event
      if (ntrack>NtrackMAX) {
	cerr << "***ntrack exceeded NtrackMAX!!!! event, ntrack, NtrackMAX = "
	     << nevent << " " << ntrack << " " << NtrackMAX
	     << endl;
      }
      nevent++;
   }//for i :event loop

 label:

   nEv=nevent;
   G4cout <<"Read "<<nEv<<" events"<<G4endl;

   fclose(fpi);

   return true;
}


void E16G4_PrimaryGeneratorAction::GenerateMultiTrackJAMEvent_Event( G4Event *anEvent, bool wire)
   ////////////////////////////////////////////////////////////////////
{
   const G4int eventID=anEvent->GetEventID();
   G4double x=0., y=0., z=0.;
   //   const int vsize=64;
   const int vsize=NtrackMAX;

   vector<int> Pdg;            Pdg.reserve(vsize);
   vector<G4ThreeVector> gMom; gMom.reserve(vsize);
   vector<G4ThreeVector> gPos; gPos.reserve(vsize);

   int n=eventID;
   //   int m=multiplicity;
   int multi = nmul_ev;
   cout << "multi = " << multi << endl;
   
      E16MESSAGE("ev/multi=%5i %3i",eventID, multi);

   if(!wire){
      x= RandGauss::shoot(0.,sigmaX/cm);//Generate Position in cm
      y= RandGauss::shoot(0.,sigmaY/cm);//Generate Position in cm
      z= zPoint/cm + 
        RandFlat::shoot(-sigmaZ/cm, sigmaZ/cm);//Generate Position in cm
   }else{
      double u = RandFlat::shoot(0., (sigmaZ/cm)*(sigmaZ/cm));
      G4double theta = RandFlat::shoot(0.0, 360.0)*degree;
      G4double r = sqrt(u);
      x = r*sin(theta/rad)+xPoint/cm; // in cm
      y = RandGauss::shoot(yPoint/cm, sigmaY/cm); // in cm
      z = r*cos(theta/rad)+zPoint/cm; // in cm
   }
   G4ThreeVector generatePosition( x*cm, y*cm,z*cm);

   double px, py,pz;  int pid;

   for(int i=0; i< multi ; i++ ){ //track loop in an event

     //G4cout <<"Generation z:"<< z <<G4endl;
     px=pxJAM_ev[i];
     py=pyJAM_ev[i];
     pz=pzJAM_ev[i];
     //     cout << "px, py, pz = " << px << " " << py << " " << pz << endl;
     //     cout << "pidJAM_ev["<<i<<"] = " << endl;
     //     cout << pidJAM_ev[i] << endl;
     pid = pidJAM_ev[i];

     //     G4IonTable *ionTable = G4ParticleTable::GetParticleTable()->GetIonTable();
     G4IonTable *ionTable = G4IonTable::GetIonTable();

     G4ParticleDefinition* particle= 0;
     
     if (pid>1000000) {

       G4int iz = pid/1000000;
       G4int in = (pid%1000000)/1000;
       G4int iy = pid%1000;
       G4int iA = iz+in+iy;
       G4int lvl=0;
       G4cout << "iz,in,iy,iA = " << iz << " " << in << " " << iy << " " << iA << G4endl;

       if (iy!=0) {
	 G4cout << "iy!=0" << G4endl;
	 particle = ionTable->GetIon(iz,iA,iy,lvl);//this cause a problem for hypernuclei
       } else {
	 particle = ionTable->GetIon(iz,iA,iy,lvl);//this cause a problem for hypernuclei
       }

       //       continue;
     } else {
       particle = particleTable-> FindParticle(pid);
     }
     //def        particle = particleTable-> FindParticle(pid);
     const G4String pName=particle->GetParticleName();

     gMom.push_back( G4ThreeVector( px*GeV, py*GeV,pz*GeV) );
     gPos.push_back( generatePosition );
     Pdg.push_back(  pid  );

     //     E16MESSAGE("pdg pushint  %i %i %i %i:  %i %i ",i, pid, test[i],
     //		     test.size(),
     //		     Pdg[0],Pdg[1]);
     
     //     particleGun = 
     G4ParticleGun* particleGun=new G4ParticleGun(1);
     particleGun-> SetParticleEnergy(0.0*GeV); //reset
     particleGun-> SetParticleDefinition(particle); 
     particleGun-> SetParticleMomentum(gMom[i]);
     particleGun-> SetParticlePosition(gPos[i]);
     particleGun-> GeneratePrimaryVertex(anEvent);
     delete particleGun;
   }

   if( anaMan ){
      anaMan->PrimaryGenerationMulti( n,multi, Pdg, gPos, gMom, eventID );
   }

   //   delete particleGun;

}


bool E16G4_PrimaryGeneratorAction::JAMDataOpen_Pi0_2_Event //read each event
(G4String inputFilename){
  FILE* fpi=NULL;
  nEv=0;
   fpi=fopen(inputFilename,"rb");
   if (fpi==NULL ){
     E16FATAL("cannot open:%s, exit(1) ",inputFilename.c_str());
     exit(1);
      return false;
   }

   int nevent=0, prevID=0;
   int eventIDJAM=0;
   size_t size1=fread(&eventIDJAM, sizeof(int),  1,fpi);
   int mode=1;//current position
   fseek(fpi, -1*sizeof(int), mode);
   prevID = eventIDJAM;

   int pidJAM;
   double buffer[5];//impact param, px,py,pz,mass
   while (true) {

     size_t size1=fread(&eventIDJAM, sizeof(int),  1,fpi);

     if (size1 != 1) {break;}
     if (eventIDJAM != prevID){ //should go next event
       //       cerr << "eventIDJAM != prevID = " << eventIDJAM << " " << prevID << endl;
       prevID=eventIDJAM;
       fseek(fpi, -1*sizeof(int), mode);
       nevent++;
       continue;
     }
     
     size_t size2 = fread(&buffer, sizeof(double),  5,fpi);
     size_t size3 = fread(&pidJAM, sizeof(int),     1,fpi);
     if (size2!=5 || size3!=1){
       break;
     }
   }

   nEv=nevent;
   cerr << "end of JAMDataOpen_Pi0_2_Event, nEV = " << nEv << endl;
   fclose(fpi);
   
   fpi_event=fopen(inputFilename,"rb");
   if (fpi_event==NULL ){
     E16FATAL("cannot open:%s, exit(1) ",inputFilename.c_str());
     exit(1);
     return false;
   }

   return true;
}
bool E16G4_PrimaryGeneratorAction::JAMDataRead_Pi0_2_Event //read each event
(G4String inputFilename){
  //JAM kawama format, track-by-track, event infomation is only eventID

  /*
   FILE* fpi;
   fpi=fopen(inputFilename,"rb");
   if (fpi==NULL ){
     E16FATAL("cannot open:%s, exit(1) ",inputFilename.c_str());
     exit(1);
      return false;
   }
  */

   //   int nev_=1;
   int iev_ev;
   //   int nmul_ev;
   //   impJAM_ =new double[nev_];
   //   pxJAM_=new double*[nev_];
   //   pyJAM_=new double*[nev_];
   //   pzJAM_=new double*[nev_];
   //   mJAM_=new double*[nev_];
   //   pidJAM_=new int*[nev_];

   double buffer[5];//impact param, px,py,pz,mass

   int nevent=0, ntrack=0, prevID=0; 
   //def   const int NtrackMAX=64;
   //sako 2019/10/5
   const int NtrackMAX=2000;

   int eventIDJAM=0;
   size_t size1=fread(&eventIDJAM, sizeof(int),  1,fpi_event);
   cerr<<"first evidjam "<<eventIDJAM<<endl;
   int mode=1;//current position
   fseek(fpi_event, -1*sizeof(int), mode);
   prevID = eventIDJAM;

   //   for(int i=0;i<nev_;i++){
   for(int i=0;i<1;i++){
     //      pxJAMev_=new double[NtrackMAX];
     //      pyJAMev_=new double[NtrackMAX];
     //      pzJAMev_=new double[NtrackMAX];
     //      mJAMev_=new double[NtrackMAX];
     //      pidJAMev_=new int[NtrackMAX];

      ntrack=0;
      while ( ntrack < NtrackMAX){

	int pidJAM;
	size_t size1=fread(&eventIDJAM, sizeof(int),  1,fpi_event);
	cerr<<"evidjam, ntrack "<<eventIDJAM<< " " << ntrack <<endl;

	if(size1 != 1) {goto label;}
	if(eventIDJAM != prevID){ //should go next event
	  cerr << "eventIDJAM != prevID = " << eventIDJAM << " " << prevID << endl;
	  prevID=eventIDJAM;
	  nmul_ev=ntrack;
	  fseek(fpi_event, -1*sizeof(int), mode);
	  cerr << "Now exiting JAMDataRead_Pi0_2_Event" << endl;
	  break;
	  //	  goto label;
	}

	size_t size2  =fread(&buffer,  sizeof(double),  5,fpi_event);
	size_t size3=fread(&pidJAM, sizeof(int),  1,fpi_event);
	//cerr<<"evidjam size2/3" <<eventIDJAM<<" "<<size2<<" "<<size3<<endl;
	if(size2!=5 || size3!=1){goto label;}

	 iev_ev=eventIDJAM;
	 impJAM_ev=buffer[0];

	 pxJAM_ev[ntrack]=buffer[1];
	 pyJAM_ev[ntrack]=buffer[2];
	 pzJAM_ev[ntrack]=buffer[3];
	 mJAM_ev[ntrack] =buffer[4];
	 pidJAM_ev[ntrack]=pidJAM;
	 cerr<<"ntrack, iev, b, px, py, pz, m, pid = " << ntrack << " " << eventIDJAM<< " " << buffer[0]
	     << " " << buffer[1] << " " << buffer[2] << " " << buffer[3] << " " << buffer[4] << " " << pidJAM << endl;

	ntrack++;
     }//while ntrack :track loop in an event
      if (ntrack>NtrackMAX) {
	cerr << "***ntrack exceeded NtrackMAX!!!! prevID, eventIDJAM, ntrack, NtrackMAX = "
	     << eventIDJAM << prevID << " " << " " << ntrack << " " << NtrackMAX
	     << endl;
	nmul_ev = NtrackMAX;
	break;
      }
      nevent++;
   }//for i :event loop

 label:

   cerr << "Now end of JAMDataRead_Pi0_2_Event" << endl;
   //   nEv=nevent;
   //   G4cout <<"Read "<<nEv<<" events"<<G4endl;

   //   fclose(fpi);

   return true;
}
