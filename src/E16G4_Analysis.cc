//2016-08-12, uploaded by nakai
//2016-05-05, uploaded by yokkaich
//2016-03-18, uploaded by nakai
//2015-03-01, uploaded by yokkaich
//2015-01-20, uploaded by yokkaich
//2015-01-05, modified by yokkaich
//2015-01-05, modified by yokkaich
//2014-08-27, modified by kawama
//2014-08-27, modified by kawama
//2014-04-30, modified by kawama
//2013-11-14, modified by kawama
//2013-05-13, modified by kawama
/*
   E16G4_Analysis.cc

   D.Kawama
   */



//#include "Randomize.hh"
//#include <CLHEP/Random/RandGaussQ.h>
//#include <CLHEP/Random/RandGauss.h>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <string>
#include <vector>
#include <cstdio>

#include <G4Run.hh>
#include <G4RunManager.hh>
#include <G4Event.hh>
#include <G4Step.hh>
#include <G4String.hh>
#include <G4ios.hh>
#include <G4SDManager.hh>
#include <G4ThreeVector.hh>
#include <G4Poisson.hh>
#include <G4TrajectoryContainer.hh>


#include "E16G4_Analysis.hh"
#include "E16G4_AnalysisMessenger.hh"
#include "E16G4_TargetHit.hh"
#include "E16G4_GTRHit.hh"
#include "E16G4_GTRFrameHit.hh"
#include "E16G4_SSDHit.hh"
#include "E16G4_RPCHit.hh"
#include "E16G4_HBDHit.hh"
#include "E16G4_LGHit.hh"
#include "E16G4_VDHit.hh"
#include "E16G4_VDCylinderHit.hh"
#include "E16G4_TrackingAction.hh"
#include "E16G4_PrimaryGeneratorAction.hh"
#include "E16G4_Trajectory.hh"

#include "E16ANA_G4OutputData.hh"


extern E16ANA_G4OutputData* g4data;

//G4double PI = 4.*atan(1.);
ofstream *dataFile;
//ofstream dcFile;
//TFile* file;
const G4int nTar=5;
const G4int nGTR=33;
const G4int nGTRGEM=3;
const G4int nTrGEM=99;
const G4int nHBD=27;
const G4int nLG=3*9*36;
const G4int nLGVD=27;
const G4int nVDCyl=4;
const int nLayer=15;

const G4int nHBDmodule=27;
const G4int nGTRChamber=99;// (27+6) x 3
const G4int nSSDmodule=33;
const G4int nRPCmodule=81;
const G4int nTarget=3;

const double E16G4_Analysis::cutMomentum=0.001;//GeV

const double HBDcutMomentum = 0.001;//GeV
const double GTRcutMomentum = 0.001;//GeV

using namespace std;

#include "E16G4_Analysis2.cc"


///////////////////////////////////////
   E16G4_Analysis::E16G4_Analysis()
: fTriggered(false), fActive_(true), fOutput(false)
   ///////////////////////////////////////
{
  anaMessenger = new E16G4_AnalysisMessenger(this);

}

///////////////////////////
E16G4_Analysis::~E16G4_Analysis()
   ///////////////////////////
{
   delete anaMessenger;
   G4cout << "E16G4_Analysis destruction"<<G4endl;
}


//////////////////////////////////////////////////
void E16G4_Analysis::BeginOfRun( const G4Run *aRun)
   //////////////////////////////////////////////////
{
   E16G4_PrimaryGeneratorAction *pPGA = E16G4_PrimaryGeneratorAction::GetpPGA();
   genID = pPGA->GetGenID();
   if (genID>0) nGen=1;
   else nGen=2;
   datGEN = new GENData[nGen];
   datTAR1 = new TARData[100];
   datTAR2 = new TARData[100];
   datTAR3 = new TARData[100];
   datGTR1 = new GTRData[100];
   datGTR2 = new GTRData[100];
   datGTR3 = new GTRData[100];
   datGTRFrame1 = new GTRFrameData[100];
   datGTRFrame2 = new GTRFrameData[100];
   datGTRFrame3 = new GTRFrameData[100];
   datHBD = new HBDData[500];
   datLG  = new LGData[nLG];
   datLGVD  = new VDData[100];
   datVD  = new VDData[nVD];
   datVDCyl1  = new VDCylData[100];
   datVDCyl2  = new VDCylData[100];
   datVDCyl3  = new VDCylData[100];
   datVDCyl4  = new VDCylData[100];
   datDC  = new decayChain[maxTrkID];
  
   dETar = new G4double[nTar]; 
   if (filename_!=""){
      //file= new TFile( filename_, "recreate","",0 );
      //dataFile.open(filename_,std::ios::out|std::ios::binary|std::ios::trunc);
      dataFile= new ofstream(filename_,std::ios::out|std::ios::binary|std::ios::trunc);
      dataFile->write((char*)&nLayer,  sizeof(int));
      //dcFile.open("decaychain.dat",std::ios::out|std::ios::binary|std::ios::trunc);
      /*const int bufsize=1000000;
      char buf[bufsize];
      dataFile->rdbuf()->pubsetbuf(buf,bufsize);*/
   }
   //defineHistograms();
   trigNum=0;


}

////////////////////////////////////////////////
void E16G4_Analysis::EndOfRun( const G4Run *aRun )
   ////////////////////////////////////////////////
{
   G4cout << "#events generated = " << aRun->GetNumberOfEvent() << G4endl;
   G4cout << "#events triggered = " << trigNum << G4endl;
   /*if (filename_!=""){
      file->Write();
   }
   if (genID==2){
      for (int i=0;i<nVD;i++) treeVD[i]->Write();
   }
   file->Close();*/
   //dataFile->close();
   //G4cout << "dataFile closed"<<G4endl;
   //dcFile.close();
   
}

/////////////////////////////////////////////////////////
void E16G4_Analysis::BeginOfEvent( const G4Event *)
   /////////////////////////////////////////////////////////
{
   //G4cout << "Begin Of Event" <<G4endl;
}

////////////////////////////////////////////////////////////////
void E16G4_Analysis::PrimaryGeneration(
      int n, int m, const G4int PID[], 
      const G4ThreeVector pos[],const G4ThreeVector mom[],
      const G4int ID)
////////////////////////////////////////////////////////////////
{
   E16G4_PrimaryGeneratorAction *pPGA = E16G4_PrimaryGeneratorAction::GetpPGA();
   evID_JAM=pPGA->GetJAMEventNumber(n);
   mulID_JAM=m;
   mulNum_JAM= pPGA->GetJAMMultiplicity(n);
   //G4cout << "Primary Generation" <<G4endl;
   //G4cout << "n="<<n << "m="<<m <<G4endl;
   //G4cout << "PID="<<PID[0] <<G4endl;
   //G4cout << mom[0].mag()/GeV <<G4endl;
   datGEN[0].x= pos[0].x()/cm; 
   datGEN[0].y= pos[0].y()/cm; 
   datGEN[0].z= pos[0].z()/cm; 
   datGEN[0].px= mom[0].x()/GeV; 
   datGEN[0].py= mom[0].y()/GeV; 
   datGEN[0].pz= mom[0].z()/GeV; 
   datGEN[0].p= mom[0].mag()/GeV;         
   datGEN[0].PID= PID[0];
   evID_G4 = ID;

   //def
   //   g4data->Event().SetHeader(ID, pPGA->GetJAMEventNumber(n), 
   //			     pPGA->GetJAMMultiplicity(n)     );
   //sako 2019/11/18
   g4data->Event().SetHeader(ID, pPGA->GetJAMEventNumber(n), 
			     pPGA->GetJAMMultiplicity(n), pPGA->GetJAMImpact(n));

   g4data->Event().PrimaryTracks(0).SetData(evID_G4,
					    PID[0],
		   pos[0].x()/cm,  pos[0].y()/cm, pos[0].z()/cm,
		   mom[0].x()/GeV,  mom[0].y()/GeV, mom[0].z()/GeV,
		   mom[0].mag()/GeV  );
   g4data->Event().PrimaryTracks().SetNoh(1);

   //   std::cerr<<" track "<<mom[0].x()<<std::endl;

}
////////////////////////////////////////////////////////////////
void E16G4_Analysis::PrimaryGeneration(
      int n, const G4int PID[], 
      const G4ThreeVector pos[],const G4ThreeVector mom[],
      const G4int ID)
////////////////////////////////////////////////////////////////
{
   //G4cout << "Primary Generation" <<G4endl;
   for (int i=0;i<n;i++){
      datGEN[i].x= pos[i].x()/cm; 
      datGEN[i].y= pos[i].y()/cm; 
      datGEN[i].z= pos[i].z()/cm; 
      datGEN[i].px= mom[i].x()/GeV; 
      datGEN[i].py= mom[i].y()/GeV; 
      datGEN[i].pz= mom[i].z()/GeV; 
      datGEN[i].p= mom[i].mag()/GeV; 
      datGEN[i].PID= PID[i];
      /*dcFile<<datGEN[i].x<<" "<<datGEN[i].y<<" "<<datGEN[i].z
         <<" "<<datGEN[i].px <<" "<<datGEN[i].py <<" "<<datGEN[i].pz 
         <<" "<<datGEN[i].p <<" "<<datGEN[i].PID<<G4endl;*/

      g4data->Event().PrimaryTracks(i).SetData(ID, 
					    PID[i],
		   pos[i].x()/cm,  pos[i].y()/cm, pos[i].z()/cm,
		   mom[i].x()/GeV,  mom[i].y()/GeV, mom[i].z()/GeV,
		   mom[i].mag()/GeV  );
   }
   evID_G4 = ID;

   cerr<<"primary n"<<endl;
   g4data->Event().PrimaryTracks().SetNoh(n);
   E16G4_PrimaryGeneratorAction *pPGA = E16G4_PrimaryGeneratorAction::GetpPGA();

   //def
   //   g4data->Event().SetHeader(ID, pPGA->GetJAMEventNumber(n), 
   //			     pPGA->GetJAMMultiplicity(n)     );
   //sako 2019/11/18
   g4data->Event().SetHeader(ID, pPGA->GetJAMEventNumber(n), 
			     pPGA->GetJAMMultiplicity(n), pPGA->GetJAMImpact(n));


   // (n) is questionable; should be checked. by Yok, 150209

}

////////////////////////////////////////////////////////
void E16G4_Analysis::EndOfEvent( const G4Event *anEvent  )
   ////////////////////////////////////////////////////////
{
   //fTriggered = false;*/
   //G4cout<<"NormalAnalysis start"<<G4endl;
   NormalAnalysis( anEvent );

   G4OutputDataFill( anEvent );

   if (true){
      fTriggered = true;
      char identifier[3];
      sprintf(identifier,"---");
      //G4cout
         /*<<evID_G4<<" "<<evID_JAM<<" "<<mulID_JAM<<" "<<mulNum_JAM
         <<" "<<datGEN[0].x<<" "<<datGEN[0].y<<" "<<datGEN[0].z
         <<" "<<datGEN[0].px <<" "<<datGEN[0].py <<" "<<datGEN[0].pz 
         <<" "<<datGEN[0].PID
         <<" "<<nTAR1hit <<" "<<nTAR2hit<<" "<<nTAR3hit
         <<" "<<nGTR1hit <<" "<<nGTR2hit<<" "<<nGTR3hit*/
         //<<" "<<nHBDhit 
         /*<<" "<<nLGVDhit
         <<" "<<nVDCylhit[0] <<" "
         <<" "<<nVDCylhit[1] <<" "
         <<" "<<nVDCylhit[2] <<" "
         <<" "<<nVDCylhit[3] <<" "*/
         //<<G4endl;
      dataFile->write((char*)&identifier,  sizeof(char[3]));
      dataFile->write((char*)&evID_G4,  sizeof(int));
      dataFile->write((char*)&evID_JAM, sizeof(int));
      dataFile->write((char*)&mulID_JAM,sizeof(int));
      dataFile->write((char*)&mulNum_JAM,sizeof(int));
      dataFile->write((char*)&datGEN[0].x,sizeof(double));
      dataFile->write((char*)&datGEN[0].y,sizeof(double));
      dataFile->write((char*)&datGEN[0].z,sizeof(double));
      dataFile->write((char*)&datGEN[0].px,sizeof(double));
      dataFile->write((char*)&datGEN[0].py,sizeof(double));
      dataFile->write((char*)&datGEN[0].pz,sizeof(double));
      dataFile->write((char*)&datGEN[0].PID,sizeof(int));
      dataFile->write((char*)&nTAR1hit,sizeof(int));
      dataFile->write((char*)&nTAR2hit,sizeof(int));
      dataFile->write((char*)&nTAR3hit,sizeof(int));
      dataFile->write((char*)&nGTR1hit,sizeof(int));
      dataFile->write((char*)&nGTR2hit,sizeof(int));
      dataFile->write((char*)&nGTR3hit,sizeof(int));
      dataFile->write((char*)&nGTRFrame1hit,sizeof(int));
      dataFile->write((char*)&nGTRFrame2hit,sizeof(int));
      dataFile->write((char*)&nGTRFrame3hit,sizeof(int));
      dataFile->write((char*)&nHBDhit,sizeof(int));
      dataFile->write((char*)&nLGVDhit,sizeof(int));
      dataFile->write((char*)&nVDCylhit[0],sizeof(int));
      dataFile->write((char*)&nVDCylhit[1],sizeof(int));
      dataFile->write((char*)&nVDCylhit[2],sizeof(int));
      dataFile->write((char*)&nVDCylhit[3],sizeof(int));
      
   //The last hit in the targets  
      for (int i=0;i<nTAR1hit;i++){
      dataFile->write((char*)&datTAR1[i].trkid,sizeof(datTAR1[i].trkid));
      dataFile->write((char*)&datTAR1[i].x    ,sizeof(datTAR1[i].x));
      dataFile->write((char*)&datTAR1[i].y    ,sizeof(datTAR1[i].y));
      dataFile->write((char*)&datTAR1[i].z    ,sizeof(datTAR1[i].z));
      dataFile->write((char*)&datTAR1[i].px   ,sizeof(datTAR1[i].px));
      dataFile->write((char*)&datTAR1[i].py   ,sizeof(datTAR1[i].py));
      dataFile->write((char*)&datTAR1[i].pz   ,sizeof(datTAR1[i].pz));
      dataFile->write((char*)&datTAR1[i].PID  ,sizeof(datTAR1[i].PID));
      }
      for (int i=0;i<nTAR2hit;i++){
      dataFile->write((char*)&datTAR2[i].trkid,sizeof(datTAR2[i].trkid));
      dataFile->write((char*)&datTAR2[i].x    ,sizeof(datTAR2[i].x));
      dataFile->write((char*)&datTAR2[i].y    ,sizeof(datTAR2[i].y));
      dataFile->write((char*)&datTAR2[i].z    ,sizeof(datTAR2[i].z));
      dataFile->write((char*)&datTAR2[i].px   ,sizeof(datTAR2[i].px));
      dataFile->write((char*)&datTAR2[i].py   ,sizeof(datTAR2[i].py));
      dataFile->write((char*)&datTAR2[i].pz   ,sizeof(datTAR2[i].pz));
      dataFile->write((char*)&datTAR2[i].PID  ,sizeof(datTAR2[i].PID));
         /*G4cout <<"   TAR2:"<<nTAR2hit<<":"<<
            datTAR2[i].trkid<<" "<<
            datTAR2[i].x<<" "<<    
            datTAR2[i].y<<" "<<    
            datTAR2[i].z<<" "<<    
            datTAR2[i].px<<" "<<   
            datTAR2[i].py<<" "<<   
            datTAR2[i].pz<<" "<<
            sqrt(datTAR2[i].px*datTAR2[i].px+
            datTAR2[i].py*datTAR2[i].py+
            datTAR2[i].pz*datTAR2[i].pz ) <<" "<<
            datTAR2[i].PID<<" "<< G4endl;*/
      }
      for (int i=0;i<nTAR3hit;i++){
      dataFile->write((char*)&datTAR3[i].trkid,sizeof(datTAR3[i].trkid));
      dataFile->write((char*)&datTAR3[i].x    ,sizeof(datTAR3[i].x));
      dataFile->write((char*)&datTAR3[i].y    ,sizeof(datTAR3[i].y));
      dataFile->write((char*)&datTAR3[i].z    ,sizeof(datTAR3[i].z));
      dataFile->write((char*)&datTAR3[i].px   ,sizeof(datTAR3[i].px));
      dataFile->write((char*)&datTAR3[i].py   ,sizeof(datTAR3[i].py));
      dataFile->write((char*)&datTAR3[i].pz   ,sizeof(datTAR3[i].pz));
      dataFile->write((char*)&datTAR3[i].PID  ,sizeof(datTAR3[i].PID));
      }   
      /*G4cout <<"   TAR1:"<<
            datTAR1[0].trkid<<" "<<
            datTAR1[0].x<<" "<<    
            datTAR1[0].y<<" "<<    
            datTAR1[0].z<<" "<<    
            datTAR1[0].px<<" "<<   
            datTAR1[0].py<<" "<<   
            datTAR1[0].pz<<" "<<   
            datTAR1[0].PID<<" "<< G4endl;
         G4cout <<"   TAR2:"<<
            datTAR2[0].trkid<<" "<<
            datTAR2[0].x<<" "<<    
            datTAR2[0].y<<" "<<    
            datTAR2[0].z<<" "<<    
            datTAR2[0].px<<" "<<   
            datTAR2[0].py<<" "<<   
            datTAR2[0].pz<<" "<<   
            datTAR2[0].PID<<" "<< G4endl;*/
      for (int i=0;i<nGTR1hit;i++){
         /*if(int(datGTR1[i].id/3)==5)
         G4cout <<"   GTR1:"<<datGTR1[i].trkid<<" "<<
            datGTR1[i].id<<" "<<   
            datGTR1[i].x<<" "<<    
            datGTR1[i].y<<" "<<    
            datGTR1[i].z<<" "<<   
            datGTR1[i].xg<<" "<<   
            datGTR1[i].yg<<" "<<   
            datGTR1[i].zg<<" "<<   
            datGTR1[i].px<<" "<<   
            datGTR1[i].py<<" "<<   
            datGTR1[i].pz<<" "<<   
            datGTR1[i].PID<<" "<<
            G4endl;*/
         dataFile->write((char*)&datGTR1[i].trkid,sizeof(datGTR1[i].trkid));
         dataFile->write((char*)&datGTR1[i].id   ,sizeof(datGTR1[i].id));
         dataFile->write((char*)&datGTR1[i].x    ,sizeof(datGTR1[i].x));
         dataFile->write((char*)&datGTR1[i].y    ,sizeof(datGTR1[i].y));
         dataFile->write((char*)&datGTR1[i].z    ,sizeof(datGTR1[i].z));
         dataFile->write((char*)&datGTR1[i].xg   ,sizeof(datGTR1[i].xg));
         dataFile->write((char*)&datGTR1[i].yg   ,sizeof(datGTR1[i].yg));
         dataFile->write((char*)&datGTR1[i].zg   ,sizeof(datGTR1[i].zg));
         dataFile->write((char*)&datGTR1[i].px   ,sizeof(datGTR1[i].px));
         dataFile->write((char*)&datGTR1[i].py   ,sizeof(datGTR1[i].py));
         dataFile->write((char*)&datGTR1[i].pz   ,sizeof(datGTR1[i].pz));
         dataFile->write((char*)&datGTR1[i].PID  ,sizeof(datGTR1[i].PID));
      }
      for (int i=0;i<nGTR2hit;i++){
         /*G4cout <<"   GTR2:"<<datGTR2[i].trkid<<" "<<
           datGTR2[i].id<<" "<<   
           datGTR2[i].x<<" "<<    
           datGTR2[i].y<<" "<<    
           datGTR2[i].z<<" "<<    
           datGTR2[i].xg<<" "<<   
           datGTR2[i].yg<<" "<<   
           datGTR2[i].zg<<" "<<   
           datGTR2[i].px<<" "<<   
           datGTR2[i].py<<" "<<   
           datGTR2[i].pz<<" "<<   
           datGTR2[i].PID<<" "<< G4endl;*/
         dataFile->write((char*)&datGTR2[i].trkid,sizeof(datGTR2[i].trkid));
         dataFile->write((char*)&datGTR2[i].id   ,sizeof(datGTR2[i].id));
         dataFile->write((char*)&datGTR2[i].x    ,sizeof(datGTR2[i].x));
         dataFile->write((char*)&datGTR2[i].y    ,sizeof(datGTR2[i].y));
         dataFile->write((char*)&datGTR2[i].z    ,sizeof(datGTR2[i].z));
         dataFile->write((char*)&datGTR2[i].xg   ,sizeof(datGTR2[i].xg));
         dataFile->write((char*)&datGTR2[i].yg   ,sizeof(datGTR2[i].yg));
         dataFile->write((char*)&datGTR2[i].zg   ,sizeof(datGTR2[i].zg));
         dataFile->write((char*)&datGTR2[i].px   ,sizeof(datGTR2[i].px));
         dataFile->write((char*)&datGTR2[i].py   ,sizeof(datGTR2[i].py));
         dataFile->write((char*)&datGTR2[i].pz   ,sizeof(datGTR2[i].pz));
         dataFile->write((char*)&datGTR2[i].PID  ,sizeof(datGTR2[i].PID));
      }
      for (int i=0;i<nGTR3hit;i++){
          /* if(int(datGTR3[i].id/3)==5)   
         G4cout <<"   GTR3:"<<datGTR3[i].trkid<<" "<<
           datGTR3[i].id<<" "<<   
           //datGTR3[i].x<<" "<<    
           //datGTR3[i].y<<" "<<    
           //datGTR3[i].z<<" "<<    
           datGTR3[i].xg<<" "<<   
           datGTR3[i].yg<<" "<<   
           datGTR3[i].zg<<" "<<   
           datGTR3[i].px<<" "<<   
           datGTR3[i].py<<" "<<   
           datGTR3[i].pz<<" "<<   
           datGTR3[i].PID<<" "<<
           G4endl;*/
         dataFile->write((char*)&datGTR3[i].trkid,sizeof(datGTR3[i].trkid));
         dataFile->write((char*)&datGTR3[i].id   ,sizeof(datGTR3[i].id));
         dataFile->write((char*)&datGTR3[i].x    ,sizeof(datGTR3[i].x));
         dataFile->write((char*)&datGTR3[i].y    ,sizeof(datGTR3[i].y));
         dataFile->write((char*)&datGTR3[i].z    ,sizeof(datGTR3[i].z));
         dataFile->write((char*)&datGTR3[i].xg   ,sizeof(datGTR3[i].xg));
         dataFile->write((char*)&datGTR3[i].yg   ,sizeof(datGTR3[i].yg));
         dataFile->write((char*)&datGTR3[i].zg   ,sizeof(datGTR3[i].zg));
         dataFile->write((char*)&datGTR3[i].px   ,sizeof(datGTR3[i].px));
         dataFile->write((char*)&datGTR3[i].py   ,sizeof(datGTR3[i].py));
         dataFile->write((char*)&datGTR3[i].pz   ,sizeof(datGTR3[i].pz));
         dataFile->write((char*)&datGTR3[i].PID  ,sizeof(datGTR3[i].PID));
      }
      double mindis=100;
      for (int i=0;i<nHBDhit;i++){
         /*G4cout <<"   HBD:"<<i<<" "<<
            datHBD[i].trkid<<" "<<
           datHBD[i].id<<" "<<   
           datHBD[i].x<<" "<<    
           datHBD[i].y<<" "<<    
           datHBD[i].z<<" "<<    
           datHBD[i].xg<<" "<<   
           datHBD[i].yg<<" "<<   
           datHBD[i].zg<<" "<<   
           datHBD[i].px<<" "<<   
           datHBD[i].py<<" "<<   
           datHBD[i].pz<<" "<<   
           datHBD[i].npe<<" "<<  
           datHBD[i].fl<<" "<< 
           datHBD[i].PID<<" "<< 
           G4endl;*/
         dataFile->write((char*)&datHBD[i].trkid,sizeof(datHBD[i].trkid));
         dataFile->write((char*)&datHBD[i].id   ,sizeof(datHBD[i].id));
         dataFile->write((char*)&datHBD[i].x    ,sizeof(datHBD[i].x));
         dataFile->write((char*)&datHBD[i].y    ,sizeof(datHBD[i].y));
         dataFile->write((char*)&datHBD[i].z    ,sizeof(datHBD[i].z));
         dataFile->write((char*)&datHBD[i].xg   ,sizeof(datHBD[i].xg));
         dataFile->write((char*)&datHBD[i].yg   ,sizeof(datHBD[i].yg));
         dataFile->write((char*)&datHBD[i].zg   ,sizeof(datHBD[i].zg));
         dataFile->write((char*)&datHBD[i].px   ,sizeof(datHBD[i].px));
         dataFile->write((char*)&datHBD[i].py   ,sizeof(datHBD[i].py));
         dataFile->write((char*)&datHBD[i].pz   ,sizeof(datHBD[i].pz));
         dataFile->write((char*)&datHBD[i].PID  ,sizeof(datHBD[i].PID));
         dataFile->write((char*)&datHBD[i].beta  ,sizeof(datHBD[i].beta));
         dataFile->write((char*)&datHBD[i].npe  ,sizeof(datHBD[i].npe));
         dataFile->write((char*)&datHBD[i].fl  ,sizeof(datHBD[i].fl));
         if(mindis>fabs(datHBD[i].z))mindis=fabs(datHBD[i].z);
      }
      //if(mindis>2)G4cout<<"Stopped in HBD!!!"<<G4endl;
      for (int i=0;i<nLGVDhit;i++){
         /*G4cout <<"LGVD:"<<datLGVD[i].trkid<<" "<<
           datLGVD[i].id<<" "<<   
           datLGVD[i].x<<" "<<    
           datLGVD[i].y<<" "<<    
           datLGVD[i].z<<" "<<    
           datLGVD[i].xg<<" "<<   
           datLGVD[i].yg<<" "<<   
           datLGVD[i].zg<<" "<<   
           datLGVD[i].px<<" "<<   
           datLGVD[i].py<<" "<<   
           datLGVD[i].pz<<" "<<   
           datLGVD[i].PID<<" "<< G4endl;*/
         dataFile->write((char*)&datLGVD[i].trkid,sizeof(datLGVD[i].trkid));
         dataFile->write((char*)&datLGVD[i].id   ,sizeof(datLGVD[i].id));
         dataFile->write((char*)&datLGVD[i].x    ,sizeof(datLGVD[i].x));
         dataFile->write((char*)&datLGVD[i].y    ,sizeof(datLGVD[i].y));
         dataFile->write((char*)&datLGVD[i].z    ,sizeof(datLGVD[i].z));
         dataFile->write((char*)&datLGVD[i].xg   ,sizeof(datLGVD[i].xg));
         dataFile->write((char*)&datLGVD[i].yg   ,sizeof(datLGVD[i].yg));
         dataFile->write((char*)&datLGVD[i].zg   ,sizeof(datLGVD[i].zg));
         dataFile->write((char*)&datLGVD[i].px   ,sizeof(datLGVD[i].px));
         dataFile->write((char*)&datLGVD[i].py   ,sizeof(datLGVD[i].py));
         dataFile->write((char*)&datLGVD[i].pz   ,sizeof(datLGVD[i].pz));
         dataFile->write((char*)&datLGVD[i].PID  ,sizeof(datLGVD[i].PID));
      }
      for (int i=0;i<nVDCylhit[0];i++){
         /*G4cout <<"VDCyl1:"<<datVDCyl1[i].trkid<<" "<<
           datVDCyl1[i].xg<<" "<<   
           datVDCyl1[i].yg<<" "<<   
           datVDCyl1[i].zg<<" "<<   
           datVDCyl1[i].px<<" "<<   
           datVDCyl1[i].py<<" "<<   
           datVDCyl1[i].pz<<" "<<   
           datVDCyl1[i].PID<<" "<< G4endl;*/
         dataFile->write((char*)&datVDCyl1[i].trkid,sizeof(datVDCyl1[i].trkid));
         dataFile->write((char*)&datVDCyl1[i].xg   ,sizeof(datVDCyl1[i].xg));
         dataFile->write((char*)&datVDCyl1[i].yg   ,sizeof(datVDCyl1[i].yg));
         dataFile->write((char*)&datVDCyl1[i].zg   ,sizeof(datVDCyl1[i].zg));
         dataFile->write((char*)&datVDCyl1[i].px   ,sizeof(datVDCyl1[i].px));
         dataFile->write((char*)&datVDCyl1[i].py   ,sizeof(datVDCyl1[i].py));
         dataFile->write((char*)&datVDCyl1[i].pz   ,sizeof(datVDCyl1[i].pz));
         dataFile->write((char*)&datVDCyl1[i].PID  ,sizeof(datVDCyl1[i].PID));
      }
      for (int i=0;i<nVDCylhit[1];i++){
         /*G4cout <<"VDCyl2:"<<datVDCyl2[i].trkid<<" "<<
           datVDCyl2[i].xg<<" "<<   
           datVDCyl2[i].yg<<" "<<   
           datVDCyl2[i].zg<<" "<<   
           datVDCyl2[i].px<<" "<<   
           datVDCyl2[i].py<<" "<<   
           datVDCyl2[i].pz<<" "<<   
           datVDCyl2[i].PID<<" "<< G4endl;*/
         dataFile->write((char*)&datVDCyl2[i].trkid,sizeof(datVDCyl2[i].trkid));
         dataFile->write((char*)&datVDCyl2[i].xg   ,sizeof(datVDCyl2[i].xg));
         dataFile->write((char*)&datVDCyl2[i].yg   ,sizeof(datVDCyl2[i].yg));
         dataFile->write((char*)&datVDCyl2[i].zg   ,sizeof(datVDCyl2[i].zg));
         dataFile->write((char*)&datVDCyl2[i].px   ,sizeof(datVDCyl2[i].px));
         dataFile->write((char*)&datVDCyl2[i].py   ,sizeof(datVDCyl2[i].py));
         dataFile->write((char*)&datVDCyl2[i].pz   ,sizeof(datVDCyl2[i].pz));
         dataFile->write((char*)&datVDCyl2[i].PID  ,sizeof(datVDCyl2[i].PID));
      }
      for (int i=0;i<nVDCylhit[2];i++){
         /*G4cout <<"VDCyl3:"<<datVDCyl3[i].trkid<<" "<<
           datVDCyl3[i].xg<<" "<<   
           datVDCyl3[i].yg<<" "<<   
           datVDCyl3[i].zg<<" "<<   
           datVDCyl3[i].px<<" "<<   
           datVDCyl3[i].py<<" "<<   
           datVDCyl3[i].pz<<" "<<   
           datVDCyl3[i].PID<<" "<< G4endl;*/
         dataFile->write((char*)&datVDCyl3[i].trkid,sizeof(datVDCyl3[i].trkid));
         dataFile->write((char*)&datVDCyl3[i].xg   ,sizeof(datVDCyl3[i].xg));
         dataFile->write((char*)&datVDCyl3[i].yg   ,sizeof(datVDCyl3[i].yg));
         dataFile->write((char*)&datVDCyl3[i].zg   ,sizeof(datVDCyl3[i].zg));
         dataFile->write((char*)&datVDCyl3[i].px   ,sizeof(datVDCyl3[i].px));
         dataFile->write((char*)&datVDCyl3[i].py   ,sizeof(datVDCyl3[i].py));
         dataFile->write((char*)&datVDCyl3[i].pz   ,sizeof(datVDCyl3[i].pz));
         dataFile->write((char*)&datVDCyl3[i].PID  ,sizeof(datVDCyl3[i].PID));
      }
      for (int i=0;i<nVDCylhit[3];i++){
         /*G4cout <<"VDCyl4:"<<datVDCyl4[i].trkid<<" "<<
           datVDCyl4[i].xg<<" "<<   
           datVDCyl4[i].yg<<" "<<   
           datVDCyl4[i].zg<<" "<<   
           datVDCyl4[i].px<<" "<<   
           datVDCyl4[i].py<<" "<<   
           datVDCyl4[i].pz<<" "<<   
           datVDCyl4[i].PID<<" "<< G4endl;*/
         dataFile->write((char*)&datVDCyl4[i].trkid,sizeof(datVDCyl4[i].trkid));
         dataFile->write((char*)&datVDCyl4[i].xg   ,sizeof(datVDCyl4[i].xg));
         dataFile->write((char*)&datVDCyl4[i].yg   ,sizeof(datVDCyl4[i].yg));
         dataFile->write((char*)&datVDCyl4[i].zg   ,sizeof(datVDCyl4[i].zg));
         dataFile->write((char*)&datVDCyl4[i].px   ,sizeof(datVDCyl4[i].px));
         dataFile->write((char*)&datVDCyl4[i].py   ,sizeof(datVDCyl4[i].py));
         dataFile->write((char*)&datVDCyl4[i].pz   ,sizeof(datVDCyl4[i].pz));
         dataFile->write((char*)&datVDCyl4[i].PID  ,sizeof(datVDCyl4[i].PID));
      }
      for (int i=0;i<nGTRFrame1hit;i++){
         /*G4cout <<"GTRFrame1:"<<datGTRFrame1[i].trkid<<" "<<
           datGTRFrame1[i].id<<" "<<   
           datGTRFrame1[i].xg<<" "<<   
           datGTRFrame1[i].yg<<" "<<   
           datGTRFrame1[i].zg<<" "<<   
           datGTRFrame1[i].PID<<" "<< G4endl;*/ 
         dataFile->write((char*)&datGTRFrame1[i].trkid,
               sizeof(datGTRFrame1[i].trkid));
         dataFile->write((char*)&datGTRFrame1[i].id   ,
               sizeof(datGTRFrame1[i].id));
         dataFile->write((char*)&datGTRFrame1[i].xg   ,
               sizeof(datGTRFrame1[i].xg));
         dataFile->write((char*)&datGTRFrame1[i].yg   ,
               sizeof(datGTRFrame1[i].yg));
         dataFile->write((char*)&datGTRFrame1[i].zg   ,
               sizeof(datGTRFrame1[i].zg));
         dataFile->write((char*)&datGTRFrame1[i].PID  ,
               sizeof(datGTRFrame1[i].PID));
      }
      for (int i=0;i<nGTRFrame2hit;i++){
         /*G4cout <<"GTRFrame2:"<<datGTRFrame2[i].trkid<<" "<<
           datGTRFrame2[i].id<<" "<<   
           datGTRFrame2[i].xg<<" "<<   
           datGTRFrame2[i].yg<<" "<<   
           datGTRFrame2[i].zg<<" "<<   
           datGTRFrame2[i].PID<<" "<< G4endl; */
         dataFile->write((char*)&datGTRFrame2[i].trkid,
               sizeof(datGTRFrame2[i].trkid));
         dataFile->write((char*)&datGTRFrame2[i].id   ,
               sizeof(datGTRFrame2[i].id));
         dataFile->write((char*)&datGTRFrame2[i].xg   ,
               sizeof(datGTRFrame2[i].xg));
         dataFile->write((char*)&datGTRFrame2[i].yg   ,
               sizeof(datGTRFrame2[i].yg));
         dataFile->write((char*)&datGTRFrame2[i].zg   ,
               sizeof(datGTRFrame2[i].zg));
         dataFile->write((char*)&datGTRFrame2[i].PID  ,
               sizeof(datGTRFrame2[i].PID));
      }
      for (int i=0;i<nGTRFrame3hit;i++){
         /*G4cout <<"GTRFrame3:"<<datGTRFrame3[i].trkid<<" "<<
           datGTRFrame3[i].id<<" "<<   
           datGTRFrame3[i].xg<<" "<<   
           datGTRFrame3[i].yg<<" "<<   
           datGTRFrame3[i].zg<<" "<<   
           datGTRFrame3[i].PID<<" "<< G4endl; */
         dataFile->write((char*)&datGTRFrame3[i].trkid,
               sizeof(datGTRFrame3[i].trkid));
         dataFile->write((char*)&datGTRFrame3[i].id   ,
               sizeof(datGTRFrame3[i].id));
         dataFile->write((char*)&datGTRFrame3[i].xg   ,
               sizeof(datGTRFrame3[i].xg));
         dataFile->write((char*)&datGTRFrame3[i].yg   ,
               sizeof(datGTRFrame3[i].yg));
         dataFile->write((char*)&datGTRFrame3[i].zg   ,
               sizeof(datGTRFrame3[i].zg));
         dataFile->write((char*)&datGTRFrame3[i].PID  ,
               sizeof(datGTRFrame3[i].PID));
      }
      trigNum++;
   }
   //G4cout << "End Of Event" <<G4endl;
   G4int eventID= anEvent->GetEventID()+1;
   if(eventID%100 ==0){
      G4cout << "Event Number :" << std::setw(10) << eventID <<G4endl;
      G4cout << "Triggered Event :" << std::setw(10) << trigNum <<G4endl;
   G4cout<<"Event ID in JAM :"<<evID_JAM<<G4endl;
   }





   g4data->WriteAnEvent();


}
///////////////////////////////////////////////////////////
void E16G4_Analysis::NormalAnalysis( const G4Event *anEvent )
   ///////////////////////////////////////////////////////////
{
   InitializeEvent();
   G4HCofThisEvent *HCE = anEvent->GetHCofThisEvent();
   G4SDManager *SDMan = G4SDManager::GetSDMpointer();
   if (!HCE){
      return;
   }

   //=== Target ===//
   nTAR1hit=0;
   nTAR2hit=0;
   nTAR3hit=0;
   for (int i=0;i<100;i++){
      datTAR1[i].trkid=0;
      datTAR1[i].x=0;
      datTAR1[i].y=0;
      datTAR1[i].z=0;
      datTAR1[i].p=0;
      datTAR1[i].px=0;
      datTAR1[i].py=0;
      datTAR1[i].pz=0;
      datTAR1[i].PID=0;
      datTAR2[i].trkid=0;
      datTAR2[i].x=0;
      datTAR2[i].y=0;
      datTAR2[i].z=0;
      datTAR2[i].p=0;
      datTAR2[i].px=0;
      datTAR2[i].py=0;
      datTAR2[i].pz=0;
      datTAR2[i].PID=0;
      datTAR3[i].trkid=0;
      datTAR3[i].x=0;
      datTAR3[i].y=0;
      datTAR3[i].z=0;
      datTAR3[i].p=0;
      datTAR3[i].px=0;
      datTAR3[i].py=0;
      datTAR3[i].pz=0;
      datTAR3[i].PID=0;
   }
   int colIdTar[nTar];
   TargetHitsCollection *hcTar[nTar];
   for (int k=0;k<nTar;k++){ 
      std::ostringstream id;
      id << k;
      G4String Name = G4String("Target")+id.str().c_str()+"-Collection";
      colIdTar[k] = SDMan-> GetCollectionID( Name );
      hcTar[k] = (TargetHitsCollection*)(HCE->GetHC(colIdTar[k]));
      if( hcTar[k] ){
         G4int nTarHits = hcTar[k]->entries();
         //Total number of hits (=Number of step point) in Target[k]
         //dETar[k]=0.;
         G4ThreeVector gPos;
         G4ThreeVector lMom;
         G4ThreeVector Vertex;
         //G4double maxEdep=10000;
         G4int ID=0;
         G4int preID=0;
         G4int pid=0;
         G4int nTrkID1=1;
         G4int nTrkID2=1;
         G4int nTrkID3=1;
        //G4cout <<"k,nTarHIts="<<k<<" "<< nTarHits<<G4endl;
         for( G4int i=0; i<nTarHits; i++ ){//Loop of total number of hits
            E16G4_TargetHit *aTargetHit = (*hcTar[k])[i];
            //dETar += (aTargetHit->GetEdep())/(MeV);
            G4String pName= aTargetHit-> GetPName();
            G4String procName= aTargetHit-> GetcrProcName();
            preID=ID;
            ID= aTargetHit-> GetTrackID();
            gPos=  aTargetHit->GetGPos();
            lMom=  aTargetHit->GetLMom();
            //G4ThreeVector gPos= (*hcTr[k])[i]-> GetGPos();
            //G4ThreeVector gMom= (*hcTr[k])[i]-> GetGMom();
            pid= aTargetHit-> GetPCode();
            /*G4cout <<"Target ihit="<< i<<" particle:"<<pName
            <<",lMom.mag(GeV):"<<lMom.mag()/GeV
            <<G4endl;*/
            //G4cout << "xg(mm):"<<gPos.x()/mm<< ",yg(mm):"<<gPos.y()/mm 
            //<< "zg(mm):"<<gPos.z()/mm<<G4endl;
            //G4cout << "lMom.x(GeV):"<<lMom.x()/GeV
            //<< "lMom.y(GeV):"<<lMom.y()/GeV 
            //<< "lMom.z(GeV):"<<lMom.z()/GeV
            //<<",lMom.mag(GeV):"<<lMom.mag()/GeV<<G4endl;
            //if(fabs(pid)==22)
              // G4cout <<"     lMom.mag(GeV):"<<lMom.mag()/GeV
                //<<"     gPos.z(cm):"<<gPos.z()/cm<<G4endl;
            //if(lMom.mag()/GeV>1e-4){ //momentum cut
               if (ID!=preID && preID!=0){//track ID was changed or track ID=0
                  switch (k){
                     case 0:
                        nTrkID1++;break;
                     case 1:
                        nTrkID2++;break;
                     default:
                        nTrkID3++;
                  }
               }
               switch (k){//overwrite if trkid is same, record the last one
                  case 0:
                     datTAR1[nTrkID1-1].trkid=ID;
                     datTAR1[nTrkID1-1].x=gPos.x()/cm;
                     datTAR1[nTrkID1-1].y=gPos.y()/cm;
                     datTAR1[nTrkID1-1].z=gPos.z()/cm;
                     datTAR1[nTrkID1-1].p=lMom.mag()/GeV;
                     datTAR1[nTrkID1-1].px=lMom.x()/GeV;
                     datTAR1[nTrkID1-1].py=lMom.y()/GeV;
                     datTAR1[nTrkID1-1].pz=lMom.z()/GeV;
                     datTAR1[nTrkID1-1].PID=pid;
                     //G4cout <<" trkID="<<ID<<" pre trkID="<<preID<<G4endl;
                     //G4cout <<" lMom.mag(GeV):"<<lMom.mag()/GeV<<G4endl;
                     break;
                  case 1:
                     //G4cout <<"nTar2hit,nTrkID2="<<nTAR2hit<<" "<<nTrkID2<<G4endl;
                     datTAR2[nTrkID2-1].trkid=ID;
                     datTAR2[nTrkID2-1].x=gPos.x()/cm;
                     datTAR2[nTrkID2-1].y=gPos.y()/cm;
                     datTAR2[nTrkID2-1].z=gPos.z()/cm;
                     datTAR2[nTrkID2-1].p=lMom.mag()/GeV;
                     datTAR2[nTrkID2-1].px=lMom.x()/GeV;
                     datTAR2[nTrkID2-1].py=lMom.y()/GeV;
                     datTAR2[nTrkID2-1].pz=lMom.z()/GeV;
                     datTAR2[nTrkID2-1].PID=pid;
                     break;
                  default:
                     datTAR3[nTrkID3-1].trkid=ID;
                     datTAR3[nTrkID3-1].x=gPos.x()/cm;
                     datTAR3[nTrkID3-1].y=gPos.y()/cm;
                     datTAR3[nTrkID3-1].z=gPos.z()/cm;
                     datTAR3[nTrkID3-1].p=lMom.mag()/GeV;
                     datTAR3[nTrkID3-1].px=lMom.x()/GeV;
                     datTAR3[nTrkID3-1].py=lMom.y()/GeV;
                     datTAR3[nTrkID3-1].pz=lMom.z()/GeV;
                     datTAR3[nTrkID3-1].PID=pid;
                     //G4cout <<nGTR3hit+nTrkID3-1<<G4endl;
               }//switch

	       //def               if(nTrkID1>100){
	       //sako 2019/10/7
               if(nTrkID1>maxhit){
		 //def		 G4cout <<"Too many hit on TAR1!!! Exit the loop."<<G4endl;
		 G4cout <<"Too many hit on TAR1!!! Exit the loop. nTrkID1 = "
			<< nTrkID1<<G4endl;
                  break;
               }
	       //def               if(nTrkID2>100){
	       //sako 2019/10/7
               if(nTrkID2>maxhit){
		 //def                  G4cout <<"Too many hit on TAR2!!! Exit the loop."<<G4endl;
                  G4cout <<"Too many hit on TAR2!!! Exit the loop. nTrkID2 = "
			 <<nTrkID2<<G4endl;
                  break;
               }
	       //def               if(nTrkID3>100){
	       //sako 2019/10/7
               if(nTrkID3>maxhit){
		 //                  G4cout <<"Too many hit on TAR3!!! Exit the loop."<<G4endl;
                  G4cout <<"Too many hit on TAR3!!! Exit the loop. nTrkID3 = "
			 <<nTrkID3<<G4endl;
                  break;
               }
            //}
            /*G4cout <<"k="<<k<<G4endl;
              G4cout << "xl(mm):"<<lPos.x()/mm<< ",yl(mm):"<<lPos.y()/mm 
              << "zl(mm):"<<lPos.z()/mm<<G4endl;
              */
         }
         switch (k){
            case 0:
               nTAR1hit+=nTrkID1;
               break;
            case 1:
               nTAR2hit+=nTrkID2;
               //G4cout <<"nTar2hit,nTrkID2="<<nTAR2hit<<" "<<nTrkID2<<G4endl;
               break;
            default:
               nTAR3hit+=nTrkID3;
               break;
         }//switch
         //G4cout << "E16G4_Analysis:TargetEdep="<<TargetEdep<<G4endl;
      }
   }

   //G4cout <<"GTR ANalysis start"<<G4endl;

   G4int nPoint1[1000];
   G4int nPoint2[1000];
   G4int nPoint3[1000];
   nGTR1hit=0;
   nGTR2hit=0;
   nGTR3hit=0;
   for (int i=0;i<100;i++){
      nPoint1[i]=0;
      nPoint2[i]=0;
      nPoint3[i]=0;
      datGTR1[i].trkid=0;
      datGTR1[i].id=0;
      datGTR1[i].x=0;
      datGTR1[i].y=0;
      datGTR1[i].z=0;
      datGTR1[i].xg=0;
      datGTR1[i].yg=0;
      datGTR1[i].zg=0;
      datGTR1[i].p=0;
      datGTR1[i].px=0;
      datGTR1[i].py=0;
      datGTR1[i].pz=0;
      datGTR1[i].PID=0;
      datGTR2[i].trkid=0;
      datGTR2[i].id=0;
      datGTR2[i].x=0;
      datGTR2[i].y=0;
      datGTR2[i].z=0;
      datGTR2[i].xg=0;
      datGTR2[i].yg=0;
      datGTR2[i].zg=0;
      datGTR2[i].p=0;
      datGTR2[i].px=0;
      datGTR2[i].py=0;
      datGTR2[i].pz=0;
      datGTR2[i].PID=0;
      datGTR3[i].trkid=0;
      datGTR3[i].id=0;
      datGTR3[i].x=0;
      datGTR3[i].y=0;
      datGTR3[i].z=0;
      datGTR3[i].xg=0;
      datGTR3[i].yg=0;
      datGTR3[i].zg=0;
      datGTR3[i].p=0;
      datGTR3[i].px=0;
      datGTR3[i].py=0;
      datGTR3[i].pz=0;
      datGTR3[i].PID=0;
   }
   int colIdTr[nTrGEM];
   GTRHitsCollection* hcTr[nTrGEM];
   for( int k=0 ; k<nTrGEM; k++){
      G4int nTrkID1=1;
      G4int nTrkID2=1;
      G4int nTrkID3=1;
      std::ostringstream id;
      id << k;
      int m=k/3; 
      int n=k%3; 
      //G4cout <<"GTR ID="<<m<<", GEMID="<<n<<G4endl;
      G4String Name = G4String("GTR")+id.str().c_str()+"-Collection";
      colIdTr[k] = SDMan-> GetCollectionID( Name );
      //G4cout << "E16G4_Analysis:colLdTr"<<colIdTr[k]<<G4endl ;
      hcTr[k]= (GTRHitsCollection*)(HCE-> GetHC(colIdTr[k]));
      if( hcTr[k] ){
         G4int nhits = hcTr[k]->entries();
         G4ThreeVector lPos;
         G4ThreeVector lMom;
         G4ThreeVector Vertex;
         G4double maxEdep=10000;
         //if (nhits>0 )G4cout << "E16G4_Analysis:nhits"<<nhits<<G4endl ;
         if (nhits>0 && nhits<100){
            G4int ID=0;
            G4int preID=0;
            for(G4int i=0; i< nhits; i++){
               //G4int i=0;
               //G4ThreeVector xyz= (*hcTr[k])[i]-> GetPosition();
               G4String pName= (*hcTr[k])[i]-> GetPName();
               G4String procName= (*hcTr[k])[i]-> GetcrProcName();
               G4double edep= (*hcTr[k])[i]-> GetEdep()/keV;
               preID=ID;
               ID= (*hcTr[k])[i]-> GetTrackID();
               lPos= (*hcTr[k])[i]-> GetLPos();
               lMom= (*hcTr[k])[i]-> GetLMom();
               G4ThreeVector gPos= (*hcTr[k])[i]-> GetGPos();
               G4ThreeVector gMom= (*hcTr[k])[i]-> GetGMom();
               G4int pid= (*hcTr[k])[i]-> GetPCode();
               //G4cout <<"GTR ihit="<< i<<" particle:"<<pName
               //<<" ID:"<<ID<<" mom:"<<lMom.mag()/GeV<<G4endl;
               //<<" Edep(keV):"<<edep/keV<<G4endl;
               //G4cout << "xg(mm):"<<gPos.x()/mm<< ",yg(mm):"<<gPos.y()/mm 
               //<< "zg(mm):"<<gPos.z()/mm<<G4endl;
               /*G4cout << "xl(mm):"<<lPos.x()/mm<< ",yl(mm):"<<lPos.y()/mm 
               << "zl(mm):"<<lPos.z()/mm<<G4endl;
               G4cout << "lMom.x(GeV):"<<lMom.x()/GeV
               << "lMom.y(GeV):"<<lMom.y()/GeV 
               << "lMom.z(GeV):"<<lMom.z()/GeV
               <<",lMom.mag(GeV):"<<lMom.mag()/GeV<<G4endl;*/
               //G4cout <<"     lMom.mag(GeV):"<<lMom.mag()/GeV<<G4endl;
               if(lMom.mag()/GeV>1e-3){ //momentum cut
                  if (ID!=preID && preID!=0){
                     switch (n){
                        case 0:
                           nTrkID1++;break;
                        case 1:
                           nTrkID2++;break;
                        default:
                           nTrkID3++;
                     }
                  }
                  switch (n){
                     case 0:
                        datGTR1[nGTR1hit+nTrkID1-1].trkid=ID;
                        datGTR1[nGTR1hit+nTrkID1-1].id=m;
                        datGTR1[nGTR1hit+nTrkID1-1].x=lPos.x()/cm;
                        datGTR1[nGTR1hit+nTrkID1-1].y=lPos.y()/cm;
                        datGTR1[nGTR1hit+nTrkID1-1].z=lPos.z()/cm;
                        datGTR1[nGTR1hit+nTrkID1-1].xg=gPos.x()/cm;
                        datGTR1[nGTR1hit+nTrkID1-1].yg=gPos.y()/cm;
                        datGTR1[nGTR1hit+nTrkID1-1].zg=gPos.z()/cm;
                        datGTR1[nGTR1hit+nTrkID1-1].p=lMom.mag()/GeV;
                        datGTR1[nGTR1hit+nTrkID1-1].px=lMom.x()/GeV;
                        datGTR1[nGTR1hit+nTrkID1-1].py=lMom.y()/GeV;
                        datGTR1[nGTR1hit+nTrkID1-1].pz=lMom.z()/GeV;
                        datGTR1[nGTR1hit+nTrkID1-1].PID=pid;
                        nPoint1[nGTR1hit+nTrkID1-1]++;
                        //G4cout <<" trkID="<<ID<<" pre trkID="<<preID<<
                          // " x="<<lPos.x()/cm<<" y="<<lPos.y()/cm<<" z="<<lPos.z()/cm<<G4endl;
                        //G4cout <<" lMom.mag(GeV):"<<lMom.mag()/GeV<<G4endl;
                        break;
                     case 1:
                        datGTR2[nGTR2hit+nTrkID2-1].trkid=ID;
                        datGTR2[nGTR2hit+nTrkID2-1].id=m;
                        datGTR2[nGTR2hit+nTrkID2-1].x=lPos.x()/cm;
                        datGTR2[nGTR2hit+nTrkID2-1].y=lPos.y()/cm;
                        datGTR2[nGTR2hit+nTrkID2-1].z=lPos.z()/cm;
                        datGTR2[nGTR2hit+nTrkID2-1].xg=gPos.x()/cm;
                        datGTR2[nGTR2hit+nTrkID2-1].yg=gPos.y()/cm;
                        datGTR2[nGTR2hit+nTrkID2-1].zg=gPos.z()/cm;
                        datGTR2[nGTR2hit+nTrkID2-1].p=lMom.mag()/GeV;
                        datGTR2[nGTR2hit+nTrkID2-1].px=lMom.x()/GeV;
                        datGTR2[nGTR2hit+nTrkID2-1].py=lMom.y()/GeV;
                        datGTR2[nGTR2hit+nTrkID2-1].pz=lMom.z()/GeV;
                        datGTR2[nGTR2hit+nTrkID2-1].PID=pid;
                        nPoint2[nGTR2hit+nTrkID2-1]++;
                        break;
                     default:
                        datGTR3[nGTR3hit+nTrkID3-1].trkid=ID;
                        datGTR3[nGTR3hit+nTrkID3-1].id=m;
                        datGTR3[nGTR3hit+nTrkID3-1].x=lPos.x()/cm;
                        datGTR3[nGTR3hit+nTrkID3-1].y=lPos.y()/cm;
                        datGTR3[nGTR3hit+nTrkID3-1].z=lPos.z()/cm;
                        datGTR3[nGTR3hit+nTrkID3-1].xg=gPos.x()/cm;
                        datGTR3[nGTR3hit+nTrkID3-1].yg=gPos.y()/cm;
                        datGTR3[nGTR3hit+nTrkID3-1].zg=gPos.z()/cm;
                        datGTR3[nGTR3hit+nTrkID3-1].p=lMom.mag()/GeV;
                        datGTR3[nGTR3hit+nTrkID3-1].px=lMom.x()/GeV;
                        datGTR3[nGTR3hit+nTrkID3-1].py=lMom.y()/GeV;
                        datGTR3[nGTR3hit+nTrkID3-1].pz=lMom.z()/GeV;
                        datGTR3[nGTR3hit+nTrkID3-1].PID=pid;
                        nPoint3[nGTR3hit+nTrkID3-1]++;
                        //G4cout <<nGTR3hit+nTrkID3-1<<G4endl;
                  }//switch
                  maxEdep=edep;
		  //def                  if(nTrkID1>100){
		  //sako 2019/10/7
                  if(nTrkID1>maxhit){
		    //def                     G4cout <<"Too many hit on GTR1!!! Exit the loop."<<G4endl;
                     G4cout <<"Too many hit on GTR1!!! Exit the loop. nTrkID1 = "
			    <<nTrkID1 <<G4endl;
                     break;
                  }
		  //def                  if(nTrkID2>100){
		  //sako 2019/10/7
                  if(nTrkID2>maxhit){
		    //def                     G4cout <<"Too many hit on GTR2!!! Exit the loop."<<G4endl;
                     G4cout <<"Too many hit on GTR2!!! Exit the loop. nTrkID2 = "
			    <<nTrkID2 <<G4endl;
                     break;
                  }
		  //def                  if(nTrkID3>100){
		  //sako 2019/10/7
                  if(nTrkID3>maxhit){
		    //def                     G4cout <<"Too many hit on GTR3!!! Exit the loop."<<G4endl;
                     G4cout <<"Too many hit on GTR3!!! Exit the loop. nTrkID3 = "
			    << nTrkID3 <<G4endl;
                     break;
                  }
               }// Energy cutoff
            }//nhits loop
            //G4cout << "GTR loop finished"<<G4endl; 
                  if(n==0)nGTR1hit+=nTrkID1;
                  if(n==1)nGTR2hit+=nTrkID2;
                  if(n==2)nGTR3hit+=nTrkID3;
            /*switch (n){
               case 0:
                  if (nTrkID1>0){
                     for (int i=nGTR1hit;i<nGTR1hit+nTrkID1;i++){
                        if (nPoint1[i]>0){
                           //G4cout << "GTR1 ID:"<<datGTR1[i].id
                           //<<"nPoint="<<nPoint1[i]<<G4endl;
                           datGTR1[i].x =datGTR1[i].x /nPoint1[i];
                           datGTR1[i].y =datGTR1[i].y /nPoint1[i];
                           datGTR1[i].z =datGTR1[i].z /nPoint1[i];
                           datGTR1[i].xg=datGTR1[i].xg/nPoint1[i];
                           datGTR1[i].yg=datGTR1[i].yg/nPoint1[i];
                           datGTR1[i].zg=datGTR1[i].zg/nPoint1[i];
                           datGTR1[i].p =datGTR1[i].p /nPoint1[i];
                           datGTR1[i].px=datGTR1[i].px/nPoint1[i];
                           datGTR1[i].py=datGTR1[i].py/nPoint1[i];
                           datGTR1[i].pz=datGTR1[i].pz/nPoint1[i];
                        }
                        //G4cout <<"mom="<<datGTR1[i].p<<G4endl;
                     }
                  }
                  //G4cout <<"nGTR1hit="<<nGTR1hit<<G4endl;
                  //G4cout <<"nTrkID1="<<nTrkID1<<G4endl;
                  break;
               case 1:
                  //G4cout <<"nTrkID2="<<nTrkID2<<G4endl;
                  if (nTrkID2>0){
                     for (int i=nGTR2hit;i<nGTR2hit+nTrkID2;i++){
                        if (nPoint2[i]>0){
                           //G4cout <<"GTR2 ID:"<<datGTR2[i].id
                           //<<"nPoint="<<nPoint2[i]<<G4endl;
                           datGTR2[i].x =datGTR2[i].x /nPoint2[i];
                           datGTR2[i].y =datGTR2[i].y /nPoint2[i];
                           datGTR2[i].z =datGTR2[i].z /nPoint2[i];
                           datGTR2[i].xg=datGTR2[i].xg/nPoint2[i];
                           datGTR2[i].yg=datGTR2[i].yg/nPoint2[i];
                           datGTR2[i].zg=datGTR2[i].zg/nPoint2[i];
                           datGTR2[i].p =datGTR2[i].p /nPoint2[i];
                           datGTR2[i].px=datGTR2[i].px/nPoint2[i];
                           datGTR2[i].py=datGTR2[i].py/nPoint2[i];
                           datGTR2[i].pz=datGTR2[i].pz/nPoint2[i];
                           //G4cout <<"mom="<<datGTR2[i].p<<G4endl;
                        }
                     }
                  }
                  break;
               default:
                  //G4cout <<"nTrkID3="<<nTrkID3<<G4endl;
                  if (nTrkID3>0){
                     for (int i=nGTR3hit;i<nGTR3hit+nTrkID3;i++){
                        if (nPoint3[i]>0){
                           //G4cout <<"GTR3 ID:"<<datGTR3[i].id
                           //<<"nPoint="<<nPoint3[i]<<G4endl;
                           datGTR3[i].x =datGTR3[i].x /nPoint3[i];
                           datGTR3[i].y =datGTR3[i].y /nPoint3[i];
                           datGTR3[i].z =datGTR3[i].z /nPoint3[i];
                           datGTR3[i].xg=datGTR3[i].xg/nPoint3[i];
                           datGTR3[i].yg=datGTR3[i].yg/nPoint3[i];
                           datGTR3[i].zg=datGTR3[i].zg/nPoint3[i];
                           datGTR3[i].p =datGTR3[i].p /nPoint3[i];
                           datGTR3[i].px=datGTR3[i].px/nPoint3[i];
                           datGTR3[i].py=datGTR3[i].py/nPoint3[i];
                           datGTR3[i].pz=datGTR3[i].pz/nPoint3[i];
                           //G4cout <<"mom="<<datGTR2[i].p<<G4endl;
                        }
                     }
                  }
            }//switch*/
         }//if (nhits>0)
         // else{
         //sprintf(datGTR[m][n].proc,"none");
         //accepted= false;
         //}
      }//if( hcTr[k] )
   }//for loop (GTR)
   //G4cerr<<"End of GTR loop"<<G4endl;

   if(nGTR1hit>0 && nGTR2hit>0 && nGTR3hit>0){      GTRTrig=true;   }
   else{      GTRTrig=false;   }
#if 0
   cerr<<"noh old gtr1 "<<nGTR1hit<<endl;

  for(int ii = 0;ii< nGTR1hit; ii++){
    GTRData& h = datGTR1[ii];
    cerr<<"old gtr1 id/x/p "<<ii <<" "<<h.id<<" "<<h.trkid<<" "<<h.x<<" "<<h.p<<endl;
  }
    
#endif
   //G4cout <<"HBD ANalysis start"<<G4endl;

   G4int nHBDPoint[1000];
   nHBDhit=0;
   for (int i=0;i<500;i++){
      nHBDPoint[i]=0;
      datHBD[i].trkid=0;
      datHBD[i].id=0;
      datHBD[i].x=0;
      datHBD[i].y=0;
      datHBD[i].z=0;
      datHBD[i].xg=0;
      datHBD[i].yg=0;
      datHBD[i].zg=0;
      datHBD[i].p=0;
      datHBD[i].px=0;
      datHBD[i].py=0;
      datHBD[i].pz=0;
      datHBD[i].PID=0;
      datHBD[i].fl=0;
   }
   int colIdHBD[nHBD];
   HBDHitsCollection* hcHBD[nHBD];
   for( int k=0 ; k<nHBD; k++){
      G4int nTrkID=1;
      std::ostringstream id;
      id << k;
      G4String Name = G4String("HBD")+id.str().c_str()+"-Collection";
      colIdHBD[k] = SDMan-> GetCollectionID( Name );
      //G4cout << "E16G4_Analysis:colLdHBD"<<colIdHBD[k]<<G4endl ;
      hcHBD[k]= (HBDHitsCollection*)(HCE-> GetHC(colIdHBD[k]));
      if( hcHBD[k] ){
         G4int nhits = hcHBD[k]->entries();
         G4ThreeVector lPos;
         G4ThreeVector lMom;
         G4ThreeVector Vertex;
         G4double npe=0.,stepl=0.;
         //G4cout << "E16G4_Analysis:nhits"<<nhits<<G4endl ;
         if (nhits>0 && nhits<500){
            G4int ID=0;
            G4int preID=0;
         //G4cout << "E16G4_Analysis:nhits"<<nhits<<G4endl ;
            for(G4int i=0; i<nhits; i++){
               //G4ThreeVector xyz= (*hcTr[k])[i]-> GetPosition();
               G4String pName= (*hcHBD[k])[i]-> GetPName();
               G4String procName= (*hcHBD[k])[i]-> GetProcName();
               lMom= (*hcHBD[k])[i]-> GetLMom();
               lPos= (*hcHBD[k])[i]-> GetLPos();
               G4ThreeVector gPos= (*hcHBD[k])[i]-> GetGPos();
               G4ThreeVector gMom= (*hcHBD[k])[i]-> GetGMom();
               G4double beta= (*hcHBD[k])[i]-> GetBeta();
               npe= (*hcHBD[k])[i]-> GetNpe();
               stepl= (*hcHBD[k])[i]-> GetLength();
               G4int pid= (*hcHBD[k])[i]-> GetPCode();
               preID=ID;
               ID= (*hcHBD[k])[i]-> GetTrackID();
               /*G4cout <<"       HBD ihit="<< i<<" particle:"<<pName
                 <<" procName:"<<procName
                 <<" gMom:"<<gMom.mag()/GeV
                 <<" lPos.x:"<<lPos.x()/cm
                 <<" lPos.y:"<<lPos.y()/cm
                 <<" lPos.z:"<<lPos.z()/cm
                 <<" trkID:"<<ID
                 <<" HBDID:"<<k
                 //<<" preID:"<<preID
                 //<<" sl:"<<stepl
                 <<G4endl;*/
               //G4cout <<"Analysis:nhit="<<i<<" "<<pName<<" "<<npe<<" "<<beta<<" "<<lMom.mag()<<G4endl;
               if(lMom.mag()/GeV>1e-3){ //momentum cut
                  if (ID!=preID && preID!=0){//Not Good
                     nTrkID++;
                  }
                  datHBD[nHBDhit+nTrkID-1].trkid=ID;
                  datHBD[nHBDhit+nTrkID-1].id=k;
                  datHBD[nHBDhit+nTrkID-1].x=lPos.x()/cm;
                  datHBD[nHBDhit+nTrkID-1].y=lPos.y()/cm;
                  datHBD[nHBDhit+nTrkID-1].z=lPos.z()/cm;
                  datHBD[nHBDhit+nTrkID-1].xg=gPos.x()/cm;
                  datHBD[nHBDhit+nTrkID-1].yg=gPos.y()/cm;
                  datHBD[nHBDhit+nTrkID-1].zg=gPos.z()/cm;
                  datHBD[nHBDhit+nTrkID-1].p=lMom.mag()/GeV;
                  datHBD[nHBDhit+nTrkID-1].px=lMom.x()/GeV;
                  datHBD[nHBDhit+nTrkID-1].py=lMom.y()/GeV;
                  datHBD[nHBDhit+nTrkID-1].pz=lMom.z()/GeV;
                  datHBD[nHBDhit+nTrkID-1].beta=beta;
                  datHBD[nHBDhit+nTrkID-1].PID=pid;
                  datHBD[nHBDhit+nTrkID-1].npe+=G4Poisson(npe);
                  datHBD[nHBDhit+nTrkID-1].fl+=stepl/cm;
		  nHBDPoint[nHBDhit+nTrkID-1]++;
                  //G4cout <<" trkID="<<ID<<" pre trkID="<<preID<<G4endl;
                  //G4cout <<" lMom.mag(GeV):"<<lMom.mag()/GeV<<G4endl;
		  //def                  if(nTrkID>100){
		  //sako 2019/10/7
		  if(nTrkID>maxhit){
		    //def                     G4cout <<"Too many hit on HBD!!! Exit the loop."<<G4endl;
                     G4cout <<"Too many hit on HBD!!! Exit the loop. nTrkID = "
			    << nTrkID<<G4endl;
                     break;
                  }

               }// Energy cutoff
            }

            //G4cout <<"HBD nTrkID="<<nTrkID<<G4endl;
            /*if (nTrkID>0){
               for (int i=nHBDhit;i<nHBDhit+nTrkID;i++){
                  if (nHBDPoint[i]>0){
                     //G4cout << "HBD ID:"<<datHBD[i].id
                     //<<"nHBDPoint="<<nHBDPoint[i]<<G4endl;
                     datHBD[i].x =datHBD[i].x /nHBDPoint[i];
                     datHBD[i].y =datHBD[i].y /nHBDPoint[i];
                     datHBD[i].z =datHBD[i].z /nHBDPoint[i];
                     datHBD[i].xg=datHBD[i].xg/nHBDPoint[i];
                     datHBD[i].yg=datHBD[i].yg/nHBDPoint[i];
                     datHBD[i].zg=datHBD[i].zg/nHBDPoint[i];
                     datHBD[i].p =datHBD[i].p /nHBDPoint[i];
                     datHBD[i].px=datHBD[i].px/nHBDPoint[i];
                     datHBD[i].py=datHBD[i].py/nHBDPoint[i];
                     datHBD[i].pz=datHBD[i].pz/nHBDPoint[i];
                     datHBD[i].beta=datHBD[i].beta/nHBDPoint[i];
                  }
                  //G4cout <<"mom="<<datHBD[i].p<<G4endl;
               }
            }*/
            //G4cout <<"nGTR1hit="<<nGTR1hit<<G4endl;
            //G4cout <<"nTrkID1="<<nTrkID1<<G4endl;
            nHBDhit+=nTrkID;
         }
         //G4cout <<"AnalysisHBD:"<<datHBD[k].PID<<" "<<datHBD[k].npe<<" "<<datHBD[k].beta<<" "<<datHBD[k].p<<G4endl;
      }
   }
   //  cerr<<"noh old hbd "<<nHBDhit<<endl;

   //G4cout <<"LG ANalysis start"<<G4endl;

   /*int colIdLG[nLG];
   LGHitsCollection* hcLG[nLG];
   for( int k=0 ; k<nLG; k++){
      std::ostringstream id;
      id << k;
      G4String Name = G4String("LG")+id.str().c_str()+"-Collection";
      colIdLG[k] = SDMan-> GetCollectionID( Name );
      //G4cout << "k="<<k<<",E16G4_Analysis:colIdLG"<<colIdLG[k]<<G4endl ;
      hcLG[k]= (LGHitsCollection*)(HCE-> GetHC(colIdLG[k]));
      if( hcLG[k] ){
         G4int nhits = hcLG[k]->entries();
         G4ThreeVector lPos;
         G4ThreeVector lMom;
         G4ThreeVector Vertex;
         G4double stepl=0.,edep=0.;
         //G4cout << "E16G4_Analysis:nhits"<<nhits<<G4endl ;
         if (nhits>0 && nhits<100){
            for(G4int i=0; i< nhits; i++){
               //G4int i=0;
               //G4ThreeVector xyz= (*hcTr[k])[i]-> GetPosition();
               G4String pName= (*hcLG[k])[i]-> GetPName();
               G4String procName= (*hcLG[k])[i]-> GetProcName();
               if (i==0 || procName=="Primary"){
                  lMom= (*hcLG[k])[i]-> GetLMom();
                  lPos= (*hcLG[k])[i]-> GetLPos();
                  G4ThreeVector gPos= (*hcLG[k])[i]-> GetGPos();
                  G4ThreeVector gMom= (*hcLG[k])[i]-> GetGMom();
                  G4double beta= (*hcLG[k])[i]-> GetBeta();
                  //npe+= (*hcLG[k])[i]-> GetNpe();
                  stepl+= (*hcLG[k])[i]-> GetLength();
                  G4int pid= (*hcLG[k])[i]-> GetPCode();
                  edep+= (*hcLG[k])[i]-> GetEdep();
                  //G4cout <<"Analysis:nhit="<<i<<" "<<pName<<" "<<npe<<" "<<beta<<" "<<lMom.mag()<<G4endl;
                  //datLG[k].x=lPos.x()/cm;
                  //datLG[k].y=lPos.y()/cm;
                  //datLG[k].z=lPos.z()/cm;
                  //datLG[k].xg=gPos.x()/cm;
                  //datLG[k].yg=gPos.y()/cm;
                  //datLG[k].zg=gPos.z()/cm;
                  //datLG[k].p=lMom.mag()/GeV;
                  //datLG[k].px=lMom.x()/GeV;
                  //datLG[k].py=lMom.y()/GeV;
                  //datLG[k].pz=lMom.z()/GeV;
                  datLG[k].beta=beta;
                  datLG[k].PID=pid;
               }
            }
            datLG[k].fl=stepl/cm;
            datLG[k].edep=edep/GeV;
         }else{
            //accepted= false;
         }
      }
   }*/

   //G4cout <<"LGVD ANalysis start"<<G4endl;

   G4int nLGVDPoint[1000];
   nLGVDhit=0;
   for (int i=0;i<100;i++){
      nLGVDPoint[i]=0;
      datLGVD[i].trkid=0;
      datLGVD[i].id=0;
      datLGVD[i].x=0;
      datLGVD[i].y=0;
      datLGVD[i].z=0;
      datLGVD[i].xg=0;
      datLGVD[i].yg=0;
      datLGVD[i].zg=0;
      datLGVD[i].p=0;
      datLGVD[i].px=0;
      datLGVD[i].py=0;
      datLGVD[i].pz=0;
      datLGVD[i].PID=0;
   }
   int colIdLGVD[nLGVD];
   VDHitsCollection *hcLGVD[nLGVD];
   for (int k=0;k<nLGVD;k++){
      G4int nTrkID=1;
      std::ostringstream id;
      id << k;
      G4String Name = G4String("LGVD")+id.str().c_str()+"-Collection";
      colIdLGVD[k] = SDMan-> GetCollectionID( Name );
      //      G4cout << "k="<<k<< endl;
      //      cout << "E16G4_Analysis:colIdLGVD"<<colIdLGVD[k]<<G4endl ;
      hcLGVD[k] = (VDHitsCollection*)(HCE->GetHC(colIdLGVD[k]));
      //G4cout << "k="<<k<<",E16G4_Analysis:colIdLGVD"<<colIdLGVD[k]<<G4endl ;
      if( hcLGVD[k] ){
         G4int nhits = hcLGVD[k]->entries();
         if (nhits>0 && nhits<100){
            G4int ID=0;
            G4int preID=0;
            for( G4int i=0; i<nhits; i++ ){
               G4ThreeVector lPos;
               G4ThreeVector lMom;
               lPos= (*hcLGVD[k])[i]-> GetLPos();
               lMom= (*hcLGVD[k])[i]-> GetLMom();
               G4ThreeVector gPos= (*hcLGVD[k])[i]-> GetGPos();
               G4ThreeVector gMom= (*hcLGVD[k])[i]-> GetGMom();
               G4int pid= (*hcLGVD[k])[i]-> GetPCode();
               preID=ID;
               ID= (*hcLGVD[k])[i]-> GetTrackID();
               //G4cout <<" trkID="<<ID<<" pre trkID="<<preID<<G4endl;
               if(lMom.mag()/GeV>1e-3){ //momentum cut
                  if (ID!=preID && preID!=0){//Not Good
                     nTrkID++;
                  }
                  datLGVD[nLGVDhit+nTrkID-1].trkid=ID;
                  datLGVD[nLGVDhit+nTrkID-1].id=k;
                  datLGVD[nLGVDhit+nTrkID-1].x=lPos.x()/cm;
                  datLGVD[nLGVDhit+nTrkID-1].y=lPos.y()/cm;
                  datLGVD[nLGVDhit+nTrkID-1].z=lPos.z()/cm;
                  datLGVD[nLGVDhit+nTrkID-1].xg=gPos.x()/cm;
                  datLGVD[nLGVDhit+nTrkID-1].yg=gPos.y()/cm;
                  datLGVD[nLGVDhit+nTrkID-1].zg=gPos.z()/cm;
                  datLGVD[nLGVDhit+nTrkID-1].p=lMom.mag()/GeV;
                  datLGVD[nLGVDhit+nTrkID-1].px=lMom.x()/GeV;
                  datLGVD[nLGVDhit+nTrkID-1].py=lMom.y()/GeV;
                  datLGVD[nLGVDhit+nTrkID-1].pz=lMom.z()/GeV;
                  datLGVD[nLGVDhit+nTrkID-1].PID=pid;
                  nLGVDPoint[nLGVDhit+nTrkID-1]++;
                  //G4cout <<" trkID="<<ID<<" pre trkID="<<preID<<G4endl;
                  //G4cout <<" lMom.mag(GeV):"<<lMom.mag()/GeV<<G4endl;
		  //def           if(nTrkID>100){
		  //sako 2019/10/7
                  if(nTrkID>maxhit){
		    //def                     G4cout <<"Too many hit on LGVD!!! Exit the loop."<<G4endl;
                     G4cout <<"Too many hit on LGVD!!! Exit the loop. nTrkID = "
			    << nTrkID <<G4endl;
                     break;
                  }

               }// Energy cutoff
            }
            /*if (nTrkID>0){
               for (int i=nLGVDhit;i<nLGVDhit+nTrkID;i++){
                  if (nLGVDPoint[i]>0){
                     //G4cout << "HBD ID:"<<datHBD[i].id
                     //<<"nHBDPoint="<<nHBDPoint[i]<<G4endl;
                     datLGVD[i].x =datLGVD[i].x /nLGVDPoint[i];
                     datLGVD[i].y =datLGVD[i].y /nLGVDPoint[i];
                     datLGVD[i].z =datLGVD[i].z /nLGVDPoint[i];
                     datLGVD[i].xg=datLGVD[i].xg/nLGVDPoint[i];
                     datLGVD[i].yg=datLGVD[i].yg/nLGVDPoint[i];
                     datLGVD[i].zg=datLGVD[i].zg/nLGVDPoint[i];
                     datLGVD[i].p =datLGVD[i].p /nLGVDPoint[i];
                     datLGVD[i].px=datLGVD[i].px/nLGVDPoint[i];
                     datLGVD[i].py=datLGVD[i].py/nLGVDPoint[i];
                     datLGVD[i].pz=datLGVD[i].pz/nLGVDPoint[i];
                  }
                  //G4cout <<"mom="<<datHBD[i].p<<G4endl;
               }
            }*/
            nLGVDhit+=nTrkID;
            //if(tid_pre==0 || tid_pre!=tid){
            //tree->Fill();
            //}
            //tid_pre=tid;
         }
      }
   }
   //delete hcLGVD[nLGVD];
   
   nVDCylhit[0]=0;
   nVDCylhit[1]=0;
   nVDCylhit[2]=0;
   nVDCylhit[3]=0;
   for (int j=0;j<100;j++){
      datVDCyl1[j].trkid=0;
      datVDCyl1[j].id=0;
      datVDCyl1[j].xg=0;
      datVDCyl1[j].yg=0;
      datVDCyl1[j].zg=0;
      datVDCyl1[j].p=0;
      datVDCyl1[j].px=0;
      datVDCyl1[j].py=0;
      datVDCyl1[j].pz=0;
      datVDCyl1[j].PID=0;
      datVDCyl2[j].trkid=0;
      datVDCyl2[j].id=0;
      datVDCyl2[j].xg=0;
      datVDCyl2[j].yg=0;
      datVDCyl2[j].zg=0;
      datVDCyl2[j].p=0;
      datVDCyl2[j].px=0;
      datVDCyl2[j].py=0;
      datVDCyl2[j].pz=0;
      datVDCyl2[j].PID=0;
      datVDCyl3[j].trkid=0;
      datVDCyl3[j].id=0;
      datVDCyl3[j].xg=0;
      datVDCyl3[j].yg=0;
      datVDCyl3[j].zg=0;
      datVDCyl3[j].p=0;
      datVDCyl3[j].px=0;
      datVDCyl3[j].py=0;
      datVDCyl3[j].pz=0;
      datVDCyl3[j].PID=0;
      datVDCyl4[j].trkid=0;
      datVDCyl4[j].id=0;
      datVDCyl4[j].xg=0;
      datVDCyl4[j].yg=0;
      datVDCyl4[j].zg=0;
      datVDCyl4[j].p=0;
      datVDCyl4[j].px=0;
      datVDCyl4[j].py=0;
      datVDCyl4[j].pz=0;
      datVDCyl4[j].PID=0;
   }
   int colIdVDCyl[nVDCyl];
   VDCylinderHitsCollection *hcVDCyl[nVDCyl];
   for (int k=0;k<nVDCyl;k++){
      G4int nTrkID=1;
      std::ostringstream id;
      id << k;
      G4String Name = G4String("VDCylinder")+id.str().c_str()+"-Collection";
      colIdVDCyl[k] = SDMan-> GetCollectionID( Name );
      hcVDCyl[k] = (VDCylinderHitsCollection*)(HCE->GetHC(colIdVDCyl[k]));
      //G4cout << "k="<<k<<",E16G4_Analysis:colIdVDCyl"<<colIdVDCyl[k]<<G4endl ;
      
      if( hcVDCyl[k] ){
         G4int nhits = hcVDCyl[k]->entries();
         //G4cout << "VDCyl nhits="<<nhits<<G4endl;
         if (nhits>0 && nhits<100){
            G4int ID=0;
            G4int preID=0;
            for( G4int i=0; i<nhits; i++ ){
               G4ThreeVector gPos= (*hcVDCyl[k])[i]-> GetGPos();
               G4ThreeVector gMom= (*hcVDCyl[k])[i]-> GetGMom();
               G4int pid= (*hcVDCyl[k])[i]-> GetPCode();
               preID=ID;
               ID= (*hcVDCyl[k])[i]-> GetTrackID();
               //G4cout <<" trkID="<<ID<<" pre trkID="<<preID<<G4endl;
               if(gMom.mag()/GeV>1e-3){ //momentum cut
                  if (ID!=preID && preID!=0){//Not Good
                     nTrkID++;
                  }
               switch(k){
                  case 0:
                  datVDCyl1[nVDCylhit[0]+nTrkID-1].trkid=ID;
                  datVDCyl1[nVDCylhit[0]+nTrkID-1].id=k;
                  datVDCyl1[nVDCylhit[0]+nTrkID-1].xg=gPos.x()/cm;
                  datVDCyl1[nVDCylhit[0]+nTrkID-1].yg=gPos.y()/cm;
                  datVDCyl1[nVDCylhit[0]+nTrkID-1].zg=gPos.z()/cm;
                  datVDCyl1[nVDCylhit[0]+nTrkID-1].p=gMom.mag()/GeV;
                  datVDCyl1[nVDCylhit[0]+nTrkID-1].px=gMom.x()/GeV;
                  datVDCyl1[nVDCylhit[0]+nTrkID-1].py=gMom.y()/GeV;
                  datVDCyl1[nVDCylhit[0]+nTrkID-1].pz=gMom.z()/GeV;
                  datVDCyl1[nVDCylhit[0]+nTrkID-1].PID=pid;
                  break;
                  case 1:
                  datVDCyl2[nVDCylhit[1]+nTrkID-1].trkid=ID;
                  datVDCyl2[nVDCylhit[1]+nTrkID-1].id=k;
                  datVDCyl2[nVDCylhit[1]+nTrkID-1].xg=gPos.x()/cm;
                  datVDCyl2[nVDCylhit[1]+nTrkID-1].yg=gPos.y()/cm;
                  datVDCyl2[nVDCylhit[1]+nTrkID-1].zg=gPos.z()/cm;
                  datVDCyl2[nVDCylhit[1]+nTrkID-1].p=gMom.mag()/GeV;
                  datVDCyl2[nVDCylhit[1]+nTrkID-1].px=gMom.x()/GeV;
                  datVDCyl2[nVDCylhit[1]+nTrkID-1].py=gMom.y()/GeV;
                  datVDCyl2[nVDCylhit[1]+nTrkID-1].pz=gMom.z()/GeV;
                  datVDCyl2[nVDCylhit[1]+nTrkID-1].PID=pid;
                  break;
                  case 2:
                  datVDCyl3[nVDCylhit[2]+nTrkID-1].trkid=ID;
                  datVDCyl3[nVDCylhit[2]+nTrkID-1].id=k;
                  datVDCyl3[nVDCylhit[2]+nTrkID-1].xg=gPos.x()/cm;
                  datVDCyl3[nVDCylhit[2]+nTrkID-1].yg=gPos.y()/cm;
                  datVDCyl3[nVDCylhit[2]+nTrkID-1].zg=gPos.z()/cm;
                  datVDCyl3[nVDCylhit[2]+nTrkID-1].p=gMom.mag()/GeV;
                  datVDCyl3[nVDCylhit[2]+nTrkID-1].px=gMom.x()/GeV;
                  datVDCyl3[nVDCylhit[2]+nTrkID-1].py=gMom.y()/GeV;
                  datVDCyl3[nVDCylhit[2]+nTrkID-1].pz=gMom.z()/GeV;
                  datVDCyl3[nVDCylhit[2]+nTrkID-1].PID=pid;
                  break;
                  default:
                  datVDCyl4[nVDCylhit[3]+nTrkID-1].trkid=ID;
                  datVDCyl4[nVDCylhit[3]+nTrkID-1].id=k;
                  datVDCyl4[nVDCylhit[3]+nTrkID-1].xg=gPos.x()/cm;
                  datVDCyl4[nVDCylhit[3]+nTrkID-1].yg=gPos.y()/cm;
                  datVDCyl4[nVDCylhit[3]+nTrkID-1].zg=gPos.z()/cm;
                  datVDCyl4[nVDCylhit[3]+nTrkID-1].p=gMom.mag()/GeV;
                  datVDCyl4[nVDCylhit[3]+nTrkID-1].px=gMom.x()/GeV;
                  datVDCyl4[nVDCylhit[3]+nTrkID-1].py=gMom.y()/GeV;
                  datVDCyl4[nVDCylhit[3]+nTrkID-1].pz=gMom.z()/GeV;
                  datVDCyl4[nVDCylhit[3]+nTrkID-1].PID=pid;
                  break;
               }
                  //G4cout <<" trkID="<<ID<<" pre trkID="<<preID<<G4endl;
                  //G4cout <<" lMom.mag(GeV):"<<lMom.mag()/GeV<<G4endl;
	       //def                  if(nTrkID>100){
	       //sako 2019/10/7
                  if(nTrkID>maxhit){
		    //def                     G4cout <<"Too many hit on VDCylinder!!! Exit the loop."<<G4endl;
                     G4cout <<"Too many hit on VDCylinder!!! Exit the loop. nTrkID = "
			    << nTrkID <<G4endl;
                     break;
                  }

               }// Energy cutoff
            }
            nVDCylhit[k]+=nTrkID;
         }
      }
   }
   //delete hcLGVD[nLGVD];
   
   
   int colIdGTRFrameDS[nTrGEM];
   int colIdGTRFrameGEM[nTrGEM];
   int colIdGTRFrameRO[nTrGEM];
   int colIdGTRFrameAl[nTrGEM];
   int colIdGTRFrameCFRP[nTrGEM];
   GTRFrameHitsCollection *hcGTRFrameDS[nTrGEM];
   GTRFrameHitsCollection *hcGTRFrameGEM[nTrGEM];
   GTRFrameHitsCollection *hcGTRFrameRO[nTrGEM];
   GTRFrameHitsCollection *hcGTRFrameAl[nTrGEM];
   GTRFrameHitsCollection *hcGTRFrameCFRP[nTrGEM];
   /*G4int nPoint1[1000];
   G4int nPoint2[1000];
   G4int nPoint3[1000];*/
   nGTRFrame1hit=0;
   nGTRFrame2hit=0;
   nGTRFrame3hit=0;
   for (int i=0;i<100;i++){
      /*nPoint1[i]=0;
      nPoint2[i]=0;
      nPoint3[i]=0;*/
      datGTRFrame1[i].trkid=0;
      datGTRFrame1[i].id=0;
      datGTRFrame1[i].xg=0;
      datGTRFrame1[i].yg=0;
      datGTRFrame1[i].zg=0;
      datGTRFrame1[i].PID=0;
      datGTRFrame2[i].trkid=0;
      datGTRFrame2[i].id=0;
      datGTRFrame2[i].xg=0;
      datGTRFrame2[i].yg=0;
      datGTRFrame2[i].zg=0;
      datGTRFrame2[i].PID=0;
      datGTRFrame3[i].trkid=0;
      datGTRFrame3[i].id=0;
      datGTRFrame3[i].xg=0;
      datGTRFrame3[i].yg=0;
      datGTRFrame3[i].zg=0;
      datGTRFrame3[i].PID=0;
   }
   for (int k=0;k<nTrGEM;k++){
      G4int nTrkID=0;
      G4int TrkIDlist[1000];
      G4int ParticleIDlist[1000];
      G4ThreeVector gPoslist[1000];
      G4ThreeVector gMomlist[1000];
      std::ostringstream id;
      id << k;
      G4String Name;
      Name = G4String("GTRFrameDriftSpacer")+id.str().c_str()+"-Collection";
      colIdGTRFrameDS[k] = SDMan-> GetCollectionID( Name );
      //G4cout << "k="<<k<<",E16G4_Analysis:colIdGTRFrame(DS):"
         //<<colIdGTRFrame[k]<<G4endl ;
      Name = G4String("GTRFrameGEM")+id.str().c_str()+"-Collection";
      colIdGTRFrameGEM[k] = SDMan-> GetCollectionID( Name );
      //G4cout << "k="<<k<<",E16G4_Analysis:colIdGTRFrame(GEM):"
         //<<colIdGTRFrame[k]<<G4endl ;
      Name = G4String("GTRFrameReadOut")+id.str().c_str()+"-Collection";
      colIdGTRFrameRO[k] = SDMan-> GetCollectionID( Name );
      //G4cout << "k="<<k<<",E16G4_Analysis:colIdGTRFrame(RO):"
         //<<colIdGTRFrame[k]<<G4endl ;
      Name = G4String("GTRFrameAlBase")+id.str().c_str()+"-Collection";
      colIdGTRFrameAl[k] = SDMan-> GetCollectionID( Name );
      //G4cout << "k="<<k<<",E16G4_Analysis:colIdGTRFrame(Al):"
         //<<colIdGTRFrame[k]<<G4endl ;
      Name = G4String("GTRFrameCFRP")+id.str().c_str()+"-Collection";
      colIdGTRFrameCFRP[k] = SDMan-> GetCollectionID( Name );
      //G4cout << "k="<<k<<",E16G4_Analysis:colIdGTRFrame(CFRP):"
         //<<colIdGTRFrame[k]<<G4endl ;
      hcGTRFrameDS[k] = (GTRFrameHitsCollection*)
         (HCE->GetHC(colIdGTRFrameDS[k]));
      hcGTRFrameGEM[k] = (GTRFrameHitsCollection*)
         (HCE->GetHC(colIdGTRFrameGEM[k]));
      hcGTRFrameAl[k] = (GTRFrameHitsCollection*)
         (HCE->GetHC(colIdGTRFrameAl[k]));
      hcGTRFrameRO[k] = (GTRFrameHitsCollection*)
         (HCE->GetHC(colIdGTRFrameRO[k]));
      hcGTRFrameCFRP[k] = (GTRFrameHitsCollection*)
         (HCE->GetHC(colIdGTRFrameCFRP[k]));
      if( hcGTRFrameDS[k] || hcGTRFrameGEM[k] || hcGTRFrameAl[k] ||
            hcGTRFrameRO[k] || hcGTRFrameCFRP[k] ){
         G4int nhitDS = hcGTRFrameDS[k] ->entries();
         G4int nhitGEM = hcGTRFrameGEM[k]->entries(); 
         G4int nhitAl = hcGTRFrameAl[k]->entries(); 
         G4int nhitRO = hcGTRFrameRO[k]->entries(); 
         G4int nhitCFRP = hcGTRFrameCFRP[k]->entries(); 
         G4int nhits=nhitDS+nhitGEM+nhitAl+nhitRO+nhitCFRP; 
         if (nhits>0 && nhits<100){
         //G4cout<<"k="<<k<<" "<<nhitDS <<" "<<nhitGEM <<" "<<
         //nhitAl <<" "<<nhitRO <<" "<<nhitCFRP <<G4endl;
            G4int TrkID=0;
	    //            G4int preID=0;
            for( G4int i=0; i<nhitDS; i++ ){
               TrkID= (*hcGTRFrameDS[k])[i]-> GetTrackID();
               int newtrkflag=1;
               int iTrkID=100;
               if(i==0)newtrkflag=1;
               else{
                  for (int j=0;j<nTrkID;j++){
                     if(TrkIDlist[j]==TrkID){
                        newtrkflag*=0;
                        iTrkID=j;
                     }
                  }
               }
               //G4cout <<"DS track:"<< TrkID<<" particle:"<<pid<<G4endl;
               if(newtrkflag==1){
               nTrkID++;
              // G4cout <<"DS track:"<< TrkID<<" trkflag:"<<newtrkflag
                //  <<" nTrkID:"<<nTrkID<<G4endl;
               ParticleIDlist[nTrkID-1]= (*hcGTRFrameDS[k])[i]-> GetPCode();
               TrkIDlist[nTrkID-1]=TrkID;   
               gPoslist[nTrkID-1]= (*hcGTRFrameDS[k])[i]-> GetGPos();
               gMomlist[nTrkID-1]= (*hcGTRFrameDS[k])[i]-> GetGMom();
               }
               else if(iTrkID<nTrkID){
               //G4cout <<"DS track:"<< TrkID<<" trkflag:"<<newtrkflag
                 // <<" nTrkID:"<<nTrkID<<G4endl;
               ParticleIDlist[iTrkID]= (*hcGTRFrameDS[k])[i]-> GetPCode();
               TrkIDlist[iTrkID]=TrkID;   
               gPoslist[iTrkID]= (*hcGTRFrameDS[k])[i]-> GetGPos();
               gMomlist[iTrkID]= (*hcGTRFrameDS[k])[i]-> GetGMom();
               }
            }
            for( G4int i=0; i<nhitGEM; i++ ){
               TrkID= (*hcGTRFrameGEM[k])[i]-> GetTrackID();
               int newtrkflag=1;
               int iTrkID=100;
               if(i==0 && nTrkID==0)newtrkflag=1;
               else{
                  for (int j=0;j<nTrkID;j++){
                     if(TrkIDlist[j]==TrkID){
                        newtrkflag*=0;
                        iTrkID=j;
                     }
                  }
               }
               //G4cout <<"GEM track:"<< TrkID<<" particle:"<<pid<<G4endl;
               if(newtrkflag==1){
               nTrkID++;
               //G4cout <<"GEM track:"<< TrkID<<" trkflag:"<<newtrkflag
                 // <<" nTrkID:"<<nTrkID<<G4endl;
               ParticleIDlist[nTrkID-1]= (*hcGTRFrameGEM[k])[i]-> GetPCode();
               TrkIDlist[nTrkID-1]=TrkID;   
               gPoslist[nTrkID-1]= (*hcGTRFrameGEM[k])[i]-> GetGPos();
               gMomlist[nTrkID-1]= (*hcGTRFrameGEM[k])[i]-> GetGMom();
               }
               else if(iTrkID<nTrkID){
               //G4cout <<"GEM track:"<< TrkID<<" trkflag:"<<newtrkflag
                 // <<" nTrkID:"<<nTrkID<<G4endl;
               ParticleIDlist[iTrkID]= (*hcGTRFrameGEM[k])[i]-> GetPCode();
               TrkIDlist[iTrkID]=TrkID;   
               gPoslist[iTrkID]= (*hcGTRFrameGEM[k])[i]-> GetGPos();
               gMomlist[iTrkID]= (*hcGTRFrameGEM[k])[i]-> GetGMom();
               }
            }
            for( G4int i=0; i<nhitAl; i++ ){
               TrkID= (*hcGTRFrameAl[k])[i]-> GetTrackID();
               int newtrkflag=1;
               int iTrkID=100;
               if(i==0 && nTrkID==0)newtrkflag=1;
               else{
                  for (int j=0;j<nTrkID;j++){
                     if(TrkIDlist[j]==TrkID){
                        newtrkflag*=0;
                        iTrkID=j;
                     }
                  }
               }
               //G4cout <<"Al track:"<< TrkID<<" particle:"<<pid<<G4endl;
               if(newtrkflag==1){
               nTrkID++;
               //G4cout <<"Al track:"<< TrkID<<" trkflag:"<<newtrkflag
                 // <<" nTrkID:"<<nTrkID<<G4endl;
               ParticleIDlist[nTrkID-1]= (*hcGTRFrameAl[k])[i]-> GetPCode();
               TrkIDlist[nTrkID-1]=TrkID;   
               gPoslist[nTrkID-1]= (*hcGTRFrameAl[k])[i]-> GetGPos();
               gMomlist[nTrkID-1]= (*hcGTRFrameAl[k])[i]-> GetGMom();
               }
               else if(iTrkID<nTrkID){
               //G4cout <<"Al track:"<< TrkID<<" trkflag:"<<newtrkflag
                 // <<" nTrkID:"<<nTrkID<<G4endl;
               ParticleIDlist[iTrkID]= (*hcGTRFrameAl[k])[i]-> GetPCode();
               TrkIDlist[iTrkID]=TrkID;   
               gPoslist[iTrkID]= (*hcGTRFrameAl[k])[i]-> GetGPos();
               gMomlist[iTrkID]= (*hcGTRFrameAl[k])[i]-> GetGMom();
               }
            }
            for( G4int i=0; i<nhitCFRP; i++ ){
               TrkID= (*hcGTRFrameCFRP[k])[i]-> GetTrackID();
               int newtrkflag=1;
               int iTrkID=100;
               if(i==0 && nTrkID==0)newtrkflag=1;
               else{
                  for (int j=0;j<nTrkID;j++){
                     if(TrkIDlist[j]==TrkID){
                        newtrkflag*=0;
                        iTrkID=j;
                     }
                  }
               }
               //G4cout <<"CFRP track:"<< TrkID<<" particle:"<<pid<<G4endl;
               if(newtrkflag==1){
               nTrkID++;
               //G4cout <<"CFRP track:"<< TrkID<<" trkflag:"<<newtrkflag
                 // <<" nTrkID:"<<nTrkID<<G4endl;
               ParticleIDlist[nTrkID-1]= (*hcGTRFrameCFRP[k])[i]-> GetPCode();
               TrkIDlist[nTrkID-1]=TrkID;   
               gPoslist[nTrkID-1]= (*hcGTRFrameCFRP[k])[i]-> GetGPos();
               gMomlist[nTrkID-1]= (*hcGTRFrameCFRP[k])[i]-> GetGMom();
               }
               else if(iTrkID<nTrkID){
               //G4cout <<"CFRP track:"<< TrkID<<" trkflag:"<<newtrkflag
                 // <<" nTrkID:"<<nTrkID<<G4endl;
               ParticleIDlist[iTrkID]= (*hcGTRFrameCFRP[k])[i]-> GetPCode();
               TrkIDlist[iTrkID]=TrkID;   
               gPoslist[iTrkID]= (*hcGTRFrameCFRP[k])[i]-> GetGPos();
               gMomlist[iTrkID]= (*hcGTRFrameCFRP[k])[i]-> GetGMom();
               }
            }
            for( G4int i=0; i<nhitRO; i++ ){
               TrkID= (*hcGTRFrameRO[k])[i]-> GetTrackID();
               int newtrkflag=1;
               int iTrkID=100;
               if(i==0 && nTrkID==0)newtrkflag=1;
               else{
                  for (int j=0;j<nTrkID;j++){
                     if(TrkIDlist[j]==TrkID){
                        newtrkflag*=0;
                        iTrkID=j;
                     }
                  }
               }
               //G4cout <<"RO track:"<< TrkID<<" particle:"<<pid<<G4endl;
               if(newtrkflag==1){
               nTrkID++;
               
               //G4cout <<"RO track:"<< TrkID<<" trkflag:"<<newtrkflag
                 // <<" nTrkID:"<<nTrkID<<G4endl;
               ParticleIDlist[nTrkID-1]= (*hcGTRFrameRO[k])[i]-> GetPCode();
               TrkIDlist[nTrkID-1]=TrkID;   
               gPoslist[nTrkID-1]= (*hcGTRFrameRO[k])[i]-> GetGPos();
               gMomlist[nTrkID-1]= (*hcGTRFrameRO[k])[i]-> GetGMom();
               }
               else if(iTrkID<nTrkID){
               //G4cout <<"RO track:"<< TrkID<<" trkflag:"<<newtrkflag
                 // <<" nTrkID:"<<nTrkID<<G4endl;
               ParticleIDlist[iTrkID]= (*hcGTRFrameRO[k])[i]-> GetPCode();
               TrkIDlist[iTrkID]=TrkID;   
               gPoslist[iTrkID]= (*hcGTRFrameRO[k])[i]-> GetGPos();
               gMomlist[iTrkID]= (*hcGTRFrameRO[k])[i]-> GetGMom();
               }
            }
            for (int j=0;j<nTrkID;j++){
               switch(k%3){
                  case 0:
                  datGTRFrame1[j].trkid=TrkIDlist[j];
                  datGTRFrame1[j].id=int(k/3);
                  datGTRFrame1[j].xg=gPoslist[j].x();
                  datGTRFrame1[j].yg=gPoslist[j].y();
                  datGTRFrame1[j].zg=gPoslist[j].z();
                  datGTRFrame1[j].PID=ParticleIDlist[j];
                  nGTRFrame1hit=nTrkID;
                  break;
                  case 1:
                  datGTRFrame2[j].trkid=TrkIDlist[j];
                  datGTRFrame2[j].id=int(k/3);
                  datGTRFrame2[j].xg=gPoslist[j].x();
                  datGTRFrame2[j].yg=gPoslist[j].y();
                  datGTRFrame2[j].zg=gPoslist[j].z();
                  datGTRFrame2[j].PID=ParticleIDlist[j];
                  nGTRFrame2hit=nTrkID;
                  break;
                  default:
                  datGTRFrame3[j].trkid=TrkIDlist[j];
                  datGTRFrame3[j].id=int(k/3);
                  datGTRFrame3[j].xg=gPoslist[j].x();
                  datGTRFrame3[j].yg=gPoslist[j].y();
                  datGTRFrame3[j].zg=gPoslist[j].z();
                  datGTRFrame3[j].PID=ParticleIDlist[j];
                  nGTRFrame3hit=nTrkID;
                  break;
               }
               
               /*G4cout <<"    track:"<< TrkIDlist[j]<<
                  " particle:"<<ParticleIDlist[j]<<
                  " mom:"<<gMomlist[j].mag()
                  <<G4endl;*/
            }
         }
      }
   }

   //G4cout <<"End of normalanalysis"<<G4endl;



   fTriggered = false;
   /*for( int i=0 ; i<nTrGEM/3; i++){
     if(datGTR[i][0].x>-1000 && datGTR[i][1].x>-1000 
     && datGTR[i][2].x>-1000){ 
     datTrig.GTR[i]=1;
     fTriggered=true;
     }
     }*/

   //=== VD ===//
   if (genID==2){
     int colIdVD[nVD];
     VDHitsCollection *hcVD[nVD];
     for (int k=0;k<nVD;k++){
     std::ostringstream id;
     id << k;
     G4String Name = G4String("VD")+id.str().c_str()+"-Collection";
     colIdVD[k] = SDMan-> GetCollectionID( Name );
     hcVD[k] = (VDHitsCollection*)(HCE->GetHC(colIdVD[k]));
     if( hcVD[k] ){
     G4int nVDHits = hcVD[k]->entries();
     G4int tid_pre=0;
   //G4cout <<k<<" "<<nVDHits<<G4endl;
   for( G4int i=0; i<nVDHits; i++ ){
   //E16G4_VDHit *aVDHit = (*hcVD[k])[i];
   G4ThreeVector lPos;
   G4ThreeVector lMom;
   lPos= (*hcVD[k])[i]-> GetLPos();
   lMom= (*hcVD[k])[i]-> GetLMom();
   G4ThreeVector gPos= (*hcVD[k])[i]-> GetGPos();
   G4ThreeVector gMom= (*hcVD[k])[i]-> GetGMom();
   G4int pid= (*hcVD[k])[i]-> GetPCode();
   G4int tid= (*hcVD[k])[i]-> GetTrackID();
   G4String procName= (*hcVD[k])[i]-> GetProcName();
   if (procName!="Primary")datVD[k].proc=1;
   else datVD[k].proc=2;
   datVD[k].x=lPos.x()/cm;
   datVD[k].y=lPos.y()/cm;
   datVD[k].z=lPos.z()/cm;
   datVD[k].p=lMom.mag()/GeV;
   datVD[k].px=lMom.x()/GeV;
   datVD[k].py=lMom.y()/GeV;
   datVD[k].pz=lMom.z()/GeV;
   datVD[k].PID=pid;
   if(tid_pre==0 || tid_pre!=tid){
   //treeVD[k]->Fill();
   }
   tid_pre=tid;
   }
   }
   }
   //delete hcVD[nVD];
   }



   fTriggered = false;

   /*for( int i=0 ; i<nTrGEM/3; i++){
     if(datGTR[i][0].x>-1000 && datGTR[i][1].x>-1000 
     && datGTR[i][2].x>-1000){ 
     datTrig.GTR[i]=1;
     fTriggered=true;
     }
     else{
     datTrig.GTR[i]=0;
     }
     }*/
   //if(accepted){
   //G4cout<< "event accepted"<< G4endl;
   //tree-> Fill();
   //}

   //TrackingAction *trAct = new TrackingAction();
   //G4cout <<trAct->GetiSec()<<G4endl;
   /*G4int n_trj=0; 
     G4TrajectoryContainer *trjCont=anEvent->GetTrajectoryContainer(); 
     if (trjCont) n_trj=trjCont->entries();
   //G4cout << "n_trj:"<<n_trj<<G4endl;
   for (int i=0;i<n_trj;i++){
   G4VTrajectory* trj = (G4VTrajectory*)((*(anEvent->GetTrajectoryContainer()))[i]);
   //G4cout <<trj->GetParticleName()<<G4endl;
   G4String pname_trj=trj->GetParticleName();
   sprintf(track.name[i],pname_trj);
   G4cout <<track.name[i]<<G4endl;
   }*/
   //G4VTrajectory *trCont=(G4VTrajectory*)anEvent->GetTrajectoryContainer()->GetVector(); 
   //trCont->GetParticleName(); 
   //G4String name_test=trCont->GetVector(); 
   //G4cout << name_test<<G4endl;
   /*sprintf(track.name0,"test0");
     sprintf(track.name1,"test1");
     sprintf(track.name2,"test2");*/


   /*for (int i=0;i<nTrGEM;i++){
     G4cout <<"X at GEM"<<i<<": "<< xg[i]<<G4endl;
     G4cout <<"particle at GEM"<<i<<": "<< pnm[i]<<G4endl;
     }*/

   /*for (int i=0;i<nTrGEM;i++){
     delete hcTr[nTrGEM];
     }
     for (int i=0;i<nHBD;i++){
     delete hcHBD[nHBD];
     }
     for (int i=0;i<nLG;i++){
     delete hcLG[nLG];
     }
     for (int i=0;i<nLGVD;i++){
     delete hcLGVD[nLGVD];
     }
     delete HCE;
     delete SDMan;*/

   //if (fTriggered){ 
   //tree->Fill();
   //trigNum++;
   //}
}

//////////////////////////////////////////
void E16G4_Analysis::InitializeEvent( void )
   //////////////////////////////////////////
{
   for (int i=0;i<100;i++){   
      datGTR1[i].x =datGTR1[i].y =datGTR1[i].z=-1000.;
      datGTR1[i].xg=datGTR1[i].yg=datGTR1[i].zg=-1000.;
      datGTR1[i].p =datGTR1[i].px=datGTR1[i].py=datGTR1[i].pz=-1000.;
      datGTR1[i].PID=0;
      datGTR2[i].x =datGTR2[i].y =datGTR2[i].z=-1000.;
      datGTR2[i].xg=datGTR2[i].yg=datGTR2[i].zg=-1000.;
      datGTR2[i].p =datGTR2[i].px=datGTR2[i].py=datGTR2[i].pz=-1000.;
      datGTR2[i].PID=0;
      datGTR3[i].x =datGTR3[i].y =datGTR3[i].z=-1000.;
      datGTR3[i].xg=datGTR3[i].yg=datGTR3[i].zg=-1000.;
      datGTR3[i].p =datGTR3[i].px=datGTR3[i].py=datGTR3[i].pz=-1000.;
      datGTR3[i].PID=0;
      //datGTR1[m].acc=0;
   }
   for (int i=0;i<nHBD;i++){   
      datHBD[i].x=datHBD[i].y=datHBD[i].z=-1000.;
      datHBD[i].xg=datHBD[i].yg=datHBD[i].zg=-1000.;
      datHBD[i].p=datHBD[i].px=datHBD[i].py=datHBD[i].pz=-1000.;
      datHBD[i].npe=-1000.;
      datHBD[i].beta=-1000.;
      datHBD[i].PID=0;
   }
   for (int i=0;i<nLG;i++){   
      /*datLG[i].x=datLG[i].y=datLG[i].z=-1000.;
        datLG[i].xg=datLG[i].yg=datLG[i].zg=-1000.;
        datLG[i].p=datLG[i].px=datLG[i].py=datLG[i].pz=-1000.;*/
      datLG[i].beta=-1000.;
      datLG[i].edep=-1000.;
      datLG[i].PID=0;
   }
   for (int i=0;i<nLGVD;i++){
      datLGVD[i].x=datLGVD[i].y=datLGVD[i].z=-1000.;
      datLGVD[i].p=datLGVD[i].px=datLGVD[i].py=datLGVD[i].pz=-1000.;
      datLGVD[i].PID=datLGVD[i].proc=0;
   }
   for (int i=0;i<nVD;i++){
      datVD[i].x=datVD[i].y=datVD[i].z=-1000.;
      datVD[i].p=datVD[i].px=datVD[i].py=datVD[i].pz=-1000.;
      datVD[i].PID=datVD[i].proc=0;
   }
   /*for (int i=0;i<nVDCyl;i++){
   for (int j=0;j<100;j++){
      datVDCyl[i][j].xg=datVDCyl[i][j].yg=datVDCyl[i][j].zg=-1000.;
      datVDCyl[i][j].p=datVDCyl[i][j].px=datVDCyl[i][j].py=datVDCyl[i][j].pz=-1000.;
      datVDCyl[i][j].PID=datVDCyl[i][j].proc=0;
   }
   }*/

}



////////////////////////////////////////
void E16G4_Analysis::Terminate( void ) const
////////////////////////////////////////
{
  //   if( fActive_ )
  //      gFile->Write();
  //   gFile->Close();
}


///////////////////////////////////////////
void E16G4_Analysis::defineHistograms( void )
   ///////////////////////////////////////////
{
#if 0
    tree=new TTree("tree","tree of E16 Simulation");

   tree->Branch("evID",    &evID_G4, "evID/I"  );
   for (int i=0;i<nGen;i++){   
      char str1[10];
      sprintf(str1,"GEN%d",i);
      tree->Branch(str1,&datGEN[i], "x/D:y:z:p:px:py:pz:PID/I");
   }
#endif
   //tree->Branch("dEt",    &dETar[0], "dEt/D"  );
   /*char str[20000];
     sprintf(str,"");
     for (int i=0;i<1000;i++){   
     char buff[20];
     if (i!=999) sprintf(buff,"name[%d]/C:",i);
     else sprintf(buff,"name[%d]/C",i);
     strcat(str,buff);
     }
     G4cout << str << G4endl;
     tree->Branch("trackname",   &track, str  );*/
   //tree->Branch("trackname",   track.name, "name[30][10]/C"  );


   //if (genID<2){
   /*for (int i=0;i<nTrGEM;i++){   
     int m=i/3; 
     int n=i%3; 
     char str1[10];
     sprintf(str1,"GTR%d_%d",m,n);
     tree->Branch(str1,&datGTR[m][n], "x/D:y:z:xg:yg:zg:p:px:py:pz:PID/I");
     }*/

   for (int i=0;i<nHBD;i++){   
      char str1[10];
      sprintf(str1,"HBD%d",i);
      //      tree->Branch(str1,&datHBD[i], "x/D:y:z:xg:yg:zg:p:px:py:pz:npe:fl:beta:PID/I");
   }

   /*for (int i=0;i<nLG;i++){   
     char str1[10];
     sprintf(str1,"LG%d",i);
   //tree->Branch(str1,&datLG[i], "x/D:y:z:xg:yg:zg:p:px:py:pz:fl:beta:edep:PID/I");
   tree->Branch(str1,&datLG[i], "fl/D:beta:edep:PID/I");
   }*/

   for (int i=0;i<nLGVD;i++){   
      char str1[10];
      sprintf(str1,"LGVD%d",i);
      //tree->Branch(str1,&datLGVD[i], 
      //"x/D:y:z:xg:yg:zg:p:px:py:pz:PID/I:proc");
   }

   //tree->Branch("trigGTR",datTrig.GTR,"GTR[27]/I");
   //}

   if(genID==2){
      for (int i=0;i<nVD;i++){   
         char str1[10];
         sprintf(str1,"treeVD%d",i);
	 //         treeVD[i]=new TTree(str1,"tree of E16 Background Simulation");
         sprintf(str1,"VD%d",i);
         //treeVD[i]->Branch(str1,&datVD[i], "x/D:y:z:xg:yg:zg:p:px:py:pz:PID/I:proc");
      }
   }
   /*for (int i=0;i<27;i++){   
     char str1[10];
     char str2[10];
     sprintf(str1,"trig.GTR%d",i);
     sprintf(str2,"GTR[%d]/I",i);
     tree->Branch(str1,&datTrig.GTR[i],str2);
     }*/
}

#if 0
/////////////////////////////////////////////////////////////////
G4double E16G4_Analysis::RandGauss( G4double center, G4double dev )
   /////////////////////////////////////////////////////////////////
{
   G4double rand1 = G4UniformRand();
   G4double rand2 = G4UniformRand();

   G4double a = sqrt(-2.0*log(rand1)) * cos(2.0*M_PI*rand2);

   return dev*a + center;
}
#endif

void E16G4_Analysis::outputDC(int trkID, int flag){
  //const G4Event *evt = G4RunManager::GetRunManager()->GetCurrentEvent();
  //   int parentID=datDC[trkID].parentID;

   if(flag==0){
      //dcFile<<evt->GetEventID()<<G4endl;
   }
   else{
      /*G4cout<< "trkID="<<trkID
        <<" "<<datDC[trkID].posx_vtx
        <<" "<<datDC[trkID].posy_vtx
        <<" "<<datDC[trkID].posz_vtx
        <<G4endl;*/
      /*G4cout<< "trkID="<<trkID
        <<" "<<datDC[trkID].partID 
        <<" "<<datDC[trkID].GEN 
        <<" "<<datDC[parentID].partID 
        <<" "<<datDC[trkID].procName 
        <<" "<<datDC[trkID].volName_vtx<<G4endl;*/
      dataFile->write((char*)&trkID,sizeof(int)); 
      dataFile->write((char*)&datDC[trkID].partID ,sizeof(int));//particle ID
      dataFile->write((char*)&datDC[trkID].GEN ,sizeof(int));//Generation
      dataFile->write((char*)&datDC[trkID].parentID ,sizeof(int));//Parent trkID
      dataFile->write((char*)&datDC[trkID].procName ,sizeof(char[10]));//process name
      dataFile->write((char*)&datDC[trkID].volName_vtx ,sizeof(char[20]));//vtx colume name
      dataFile->write((char*)&datDC[trkID].posx_vtx,sizeof(double));
      dataFile->write((char*)&datDC[trkID].posy_vtx,sizeof(double));
      dataFile->write((char*)&datDC[trkID].posz_vtx,sizeof(double));
   
   }
}
