//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai
//2015-11-02, uploaded by yokkaich
//2015-11-02, uploaded by komatsu
//2015-03-27, uploaded by yokkaich
//2015-01-05, modified by yokkaich
//2015-01-05, modified by yokkaich
//2014-08-27, modified by kawama
//2014-05-07, modified by kawama
//2014-04-30, modified by kawama
//2014-04-24, modified by kawama
//2013-11-14, modified by kawama
//2013-08-16, modified by kawama
//2013-05-15, modified by kawama
//2013-05-13, modified by kawama
#include "E16ANA_OutputData.hh"
#include "TVector3.h"
#include "TRotation.h"
#include <fstream> 
#include <iostream> 
using namespace std;
   
const int E16ANA_OutputData::maxGTR1hit;
const int E16ANA_OutputData::maxGTR2hit;
const int E16ANA_OutputData::maxGTR3hit;
const int E16ANA_OutputData::maxHBDhit;
const int E16ANA_OutputData::maxLGVDhit;
const int E16ANA_OutputData::maxVDCyl1hit;
const int E16ANA_OutputData::maxVDCyl2hit;
const int E16ANA_OutputData::maxVDCyl3hit;
const int E16ANA_OutputData::maxVDCyl4hit;

E16ANA_OutputData::E16ANA_OutputData(char* filename)
:filename_(filename)
{
   //arr=new TClonesArray("TVector3");
   /*TAR1lPos=new TClonesArray("TVector3");
   TAR1lMom=new TClonesArray("TVector3");
   TAR2lPos=new TClonesArray("TVector3");
   TAR2lMom=new TClonesArray("TVector3");
   TAR3lPos=new TClonesArray("TVector3");
   TAR3lMom=new TClonesArray("TVector3");
   GTR1gPos=new TClonesArray("TVector3");
   GTR1lPos=new TClonesArray("TVector3");
   GTR1gMom=new TClonesArray("TVector3");
   GTR1lMom=new TClonesArray("TVector3");
   GTR2gPos=new TClonesArray("TVector3");
   GTR2lPos=new TClonesArray("TVector3");
   GTR2gMom=new TClonesArray("TVector3");
   GTR2lMom=new TClonesArray("TVector3");
   GTR3gPos=new TClonesArray("TVector3");
   GTR3lPos=new TClonesArray("TVector3");
   GTR3gMom=new TClonesArray("TVector3");
   GTR3lMom=new TClonesArray("TVector3");
   HBDgPos=new TClonesArray("TVector3");
   HBDlPos=new TClonesArray("TVector3");
   //HBDgMom=new TClonesArray("TVector3");
   HBDlMom=new TClonesArray("TVector3");
   LGVDgPos=new TClonesArray("TVector3");
   LGVDlPos=new TClonesArray("TVector3");
   LGVDgMom=new TClonesArray("TVector3");
   LGVDlMom=new TClonesArray("TVector3");*/
   file= new TFile(filename_,"recreate");
   file->SetCompressionLevel(1);
   DefineHistograms();
}

E16ANA_OutputData::~E16ANA_OutputData(){
   /*delete TAR1lPos;
   delete TAR1lMom;
   delete TAR2lPos;
   delete TAR2lMom;
   delete TAR3lPos;
   delete TAR3lMom;
   delete GTR1gPos;
   delete GTR1lPos;
   delete GTR1gMom;
   delete GTR1lMom;
   delete GTR2gPos;
   delete GTR2lPos;
   delete GTR2gMom;
   delete GTR2lMom;
   delete GTR3gPos;
   delete GTR3lPos;
   delete GTR3gMom;
   delete GTR3lMom;
   delete HBDgPos;
   delete HBDlPos;
   //delete HBDgMom;
   delete HBDlMom;
   delete LGVDgPos;
   delete LGVDlPos;
   delete LGVDgMom;
   delete LGVDlMom;*/
}

void E16ANA_OutputData::Fill(){
   tree->Fill();
}

void E16ANA_OutputData::WritetoFile(){
   file->Write();
   file->Close();
}

void E16ANA_OutputData::Initialize(){
   evID_JAM=-1000;
   //for (int i=0;i<10;i++){            
   //new((*arr)[i]) TVector3(-1000.,-1000.,-1000.);
   //}
   VTXpos=TVector3(-1000.,-1000.,-1000.);
   VTXmom=TVector3(-1000.,-1000.,-1000.);
   for (int i=0;i<maxTAR1hit;i++){
   //new((*TAR1lPos)[i]) TVector3(-1000.,-1000.,-1000.);
   //new((*TAR1lMom)[i]) TVector3(-1000.,-1000.,-1000.);
   TAR1lPosX[i]=-1000.;
   TAR1lPosY[i]=-1000.;
   TAR1lPosZ[i]=-1000.;
   TAR1lMomX[i]=-1000.;
   TAR1lMomY[i]=-1000.;
   TAR1lMomZ[i]=-1000.;
   TAR1pid[i]=-1000;
      TAR1parentpid[i]=-1000;
   TAR1trkid[i]=-1000;
   }
   for (int i=0;i<maxTAR2hit;i++){
   //new((*TAR2lPos)[i]) TVector3(-1000.,-1000.,-1000.);
   //new((*TAR2lMom)[i]) TVector3(-1000.,-1000.,-1000.);
   TAR2lPosX[i]=-1000.;
   TAR2lPosY[i]=-1000.;
   TAR2lPosZ[i]=-1000.;
   TAR2lMomX[i]=-1000.;
   TAR2lMomY[i]=-1000.;
   TAR2lMomZ[i]=-1000.;
   TAR2pid[i]=-1000;
      TAR2parentpid[i]=-1000;
   TAR2trkid[i]=-1000;
   }
   for (int i=0;i<maxTAR3hit;i++){
   //new((*TAR3lPos)[i]) TVector3(-1000.,-1000.,-1000.);
   //new((*TAR3lMom)[i]) TVector3(-1000.,-1000.,-1000.);
   TAR3lPosX[i]=-1000.;
   TAR3lPosY[i]=-1000.;
   TAR3lPosZ[i]=-1000.;
   TAR3lMomX[i]=-1000.;
   TAR3lMomY[i]=-1000.;
   TAR3lMomZ[i]=-1000.;
   TAR3pid[i]=-1000;
      TAR3parentpid[i]=-1000;
   TAR3trkid[i]=-1000;
   }
   for (int i=0;i<maxGTR1hit;i++){
      /*new((*GTR1gPos)[i]) TVector3(-1000.,-1000.,-1000.);
      new((*GTR1lPos)[i]) TVector3(-1000.,-1000.,-1000.);
      new((*GTR1gMom)[i]) TVector3(-1000.,-1000.,-1000.);
      new((*GTR1lMom)[i]) TVector3(-1000.,-1000.,-1000.);
      */
      GTR1gPosX[i]=-1000.;
      GTR1gPosY[i]=-1000.;
      GTR1gPosZ[i]=-1000.;
      GTR1vPosX[i]=-1000.;
      GTR1vPosY[i]=-1000.;
      GTR1vPosZ[i]=-1000.;
      GTR1lPosX[i]=-1000.;
      GTR1lPosY[i]=-1000.;
      GTR1lPosZ[i]=-1000.;
      GTR1gMomX[i]=-1000.;
      GTR1gMomY[i]=-1000.;
      GTR1gMomZ[i]=-1000.;
      GTR1lMomX[i]=-1000.;
      GTR1lMomY[i]=-1000.;
      GTR1lMomZ[i]=-1000.;
      GTR1id[i]=-1000;
      GTR1pid[i]=-1000;
      GTR1trkid[i]=-1000;
      GTR1parentpid[i]=-1000;
      GTR1originid[i]=-1000;
      sprintf(GTR1process[i],"none");
      sprintf(GTR1vtxvolume[i],"none");
   }
   for (int i=0;i<maxGTR2hit;i++){
      /*new((*GTR2gPos)[i]) TVector3(-1000.,-1000.,-1000.);
      new((*GTR2lPos)[i]) TVector3(-1000.,-1000.,-1000.);
      new((*GTR2gMom)[i]) TVector3(-1000.,-1000.,-1000.);
      new((*GTR2lMom)[i]) TVector3(-1000.,-1000.,-1000.);
      */
      GTR2gPosX[i]=-1000.;
      GTR2gPosY[i]=-1000.;
      GTR2gPosZ[i]=-1000.;
      GTR2vPosX[i]=-1000.;
      GTR2vPosY[i]=-1000.;
      GTR2vPosZ[i]=-1000.;
      GTR2lPosX[i]=-1000.;
      GTR2lPosY[i]=-1000.;
      GTR2lPosZ[i]=-1000.;
      GTR2gMomX[i]=-1000.;
      GTR2gMomY[i]=-1000.;
      GTR2gMomZ[i]=-1000.;
      GTR2lMomX[i]=-1000.;
      GTR2lMomY[i]=-1000.;
      GTR2lMomZ[i]=-1000.;
      GTR2id[i]=-1000;
      GTR2pid[i]=-1000;
      GTR2trkid[i]=-1000;
      GTR2parentpid[i]=-1000;
      GTR2originid[i]=-1000;
      sprintf(GTR2process[i],"none");
      sprintf(GTR2vtxvolume[i],"none");
   }
   for (int i=0;i<maxGTR3hit;i++){
      /*new((*GTR3gPos)[i]) TVector3(-1000.,-1000.,-1000.);
      new((*GTR3lPos)[i]) TVector3(-1000.,-1000.,-1000.);
      new((*GTR3gMom)[i]) TVector3(-1000.,-1000.,-1000.);
      new((*GTR3lMom)[i]) TVector3(-1000.,-1000.,-1000.);
      */
      GTR3gPosX[i]=-1000.;
      GTR3gPosY[i]=-1000.;
      GTR3gPosZ[i]=-1000.;
      GTR3vPosX[i]=-1000.;
      GTR3vPosY[i]=-1000.;
      GTR3vPosZ[i]=-1000.;
      GTR3lPosX[i]=-1000.;
      GTR3lPosY[i]=-1000.;
      GTR3lPosZ[i]=-1000.;
      GTR3gMomX[i]=-1000.;
      GTR3gMomY[i]=-1000.;
      GTR3gMomZ[i]=-1000.;
      GTR3lMomX[i]=-1000.;
      GTR3lMomY[i]=-1000.;
      GTR3lMomZ[i]=-1000.;
      GTR3id[i]=-1000;
      GTR3pid[i]=-1000;
      GTR3trkid[i]=-1000;
      GTR3parentpid[i]=-1000;
      GTR3originid[i]=-1000;
      sprintf(GTR3process[i],"none");
      sprintf(GTR3vtxvolume[i],"none");
   }
   for (int i=0;i<maxHBDhit;i++){
      /*new((*HBDgPos)[i]) TVector3(-1000.,-1000.,-1000.);
      new((*HBDlPos)[i]) TVector3(-1000.,-1000.,-1000.);
      new((*HBDgMom)[i]) TVector3(-1000.,-1000.,-1000.);
      new((*HBDlMom)[i]) TVector3(-1000.,-1000.,-1000.);
      */
      HBDgPosX[i]=-1000.;
      HBDgPosY[i]=-1000.;
      HBDgPosZ[i]=-1000.;
      HBDvPosX[i]=-1000.;
      HBDvPosY[i]=-1000.;
      HBDvPosZ[i]=-1000.;
      HBDlPosX[i]=-1000.;
      HBDlPosY[i]=-1000.;
      HBDlPosZ[i]=-1000.;
      HBDgMomX[i]=-1000.;
      HBDgMomY[i]=-1000.;
      HBDgMomZ[i]=-1000.;
      HBDlMomX[i]=-1000.;
      HBDlMomY[i]=-1000.;
      HBDlMomZ[i]=-1000.;
      HBDfl[i]=-1000;
      HBDid[i]=-1000;
      HBDpid[i]=-1000;
      HBDtrkid[i]=-1000;
      HBDparentpid[i]=-1000;
      HBDoriginid[i]=-1000;
      sprintf(HBDprocess[i],"none");
      sprintf(HBDvtxvolume[i],"none");
   }
   for (int i=0;i<maxLGVDhit;i++){
      LGVDgPosX[i]=-1000.;
      LGVDgPosY[i]=-1000.;
      LGVDgPosZ[i]=-1000.;
      LGVDvPosX[i]=-1000.;
      LGVDvPosY[i]=-1000.;
      LGVDvPosZ[i]=-1000.;
      LGVDlPosX[i]=-1000.;
      LGVDlPosY[i]=-1000.;
      LGVDlPosZ[i]=-1000.;
      LGVDgMomX[i]=-1000.;
      LGVDgMomY[i]=-1000.;
      LGVDgMomZ[i]=-1000.;
      LGVDlMomX[i]=-1000.;
      LGVDlMomY[i]=-1000.;
      LGVDlMomZ[i]=-1000.;
      LGVDid[i]=-1000;
      LGVDpid[i]=-1000;
      LGVDtrkid[i]=-1000;
      LGVDparentpid[i]=-1000;
      sprintf(LGVDprocess[i],"none");
      sprintf(LGVDvtxvolume[i],"none");
   }
   for (int i=0;i<maxVDCyl1hit;i++){
      VDCyl1gPosX[i]=-1000.;
      VDCyl1gPosY[i]=-1000.;
      VDCyl1gPosZ[i]=-1000.;
      VDCyl1vPosX[i]=-1000.;
      VDCyl1vPosY[i]=-1000.;
      VDCyl1vPosZ[i]=-1000.;
      VDCyl1lPosX[i]=-1000.;
      VDCyl1lPosY[i]=-1000.;
      VDCyl1lPosZ[i]=-1000.;
      VDCyl1gMomX[i]=-1000.;
      VDCyl1gMomY[i]=-1000.;
      VDCyl1gMomZ[i]=-1000.;
      VDCyl1lMomX[i]=-1000.;
      VDCyl1lMomY[i]=-1000.;
      VDCyl1lMomZ[i]=-1000.;
      VDCyl1id[i]=-1000;
      VDCyl1pid[i]=-1000;
      VDCyl1trkid[i]=-1000;
      VDCyl1parentpid[i]=-1000;
      sprintf(VDCyl1process[i],"none");
      sprintf(VDCyl1vtxvolume[i],"none");
   }
   for (int i=0;i<maxVDCyl2hit;i++){
      VDCyl2gPosX[i]=-1000.;
      VDCyl2gPosY[i]=-1000.;
      VDCyl2gPosZ[i]=-1000.;
      VDCyl2vPosX[i]=-1000.;
      VDCyl2vPosY[i]=-1000.;
      VDCyl2vPosZ[i]=-1000.;
      VDCyl2lPosX[i]=-1000.;
      VDCyl2lPosY[i]=-1000.;
      VDCyl2lPosZ[i]=-1000.;
      VDCyl2gMomX[i]=-1000.;
      VDCyl2gMomY[i]=-1000.;
      VDCyl2gMomZ[i]=-1000.;
      VDCyl2lMomX[i]=-1000.;
      VDCyl2lMomY[i]=-1000.;
      VDCyl2lMomZ[i]=-1000.;
      VDCyl2id[i]=-1000;
      VDCyl2pid[i]=-1000;
      VDCyl2trkid[i]=-1000;
      VDCyl2parentpid[i]=-1000;
      sprintf(VDCyl2process[i],"none");
      sprintf(VDCyl2vtxvolume[i],"none");
   }
   for (int i=0;i<maxVDCyl3hit;i++){
      VDCyl3gPosX[i]=-1000.;
      VDCyl3gPosY[i]=-1000.;
      VDCyl3gPosZ[i]=-1000.;
      VDCyl3vPosX[i]=-1000.;
      VDCyl3vPosY[i]=-1000.;
      VDCyl3vPosZ[i]=-1000.;
      VDCyl3lPosX[i]=-1000.;
      VDCyl3lPosY[i]=-1000.;
      VDCyl3lPosZ[i]=-1000.;
      VDCyl3gMomX[i]=-1000.;
      VDCyl3gMomY[i]=-1000.;
      VDCyl3gMomZ[i]=-1000.;
      VDCyl3lMomX[i]=-1000.;
      VDCyl3lMomY[i]=-1000.;
      VDCyl3lMomZ[i]=-1000.;
      VDCyl3id[i]=-1000;
      VDCyl3pid[i]=-1000;
      VDCyl3trkid[i]=-1000;
      VDCyl3parentpid[i]=-1000;
      sprintf(VDCyl3process[i],"none");
      sprintf(VDCyl3vtxvolume[i],"none");
   }
   for (int i=0;i<maxVDCyl4hit;i++){
      VDCyl4gPosX[i]=-1000.;
      VDCyl4gPosY[i]=-1000.;
      VDCyl4gPosZ[i]=-1000.;
      VDCyl4vPosX[i]=-1000.;
      VDCyl4vPosY[i]=-1000.;
      VDCyl4vPosZ[i]=-1000.;
      VDCyl4lPosX[i]=-1000.;
      VDCyl4lPosY[i]=-1000.;
      VDCyl4lPosZ[i]=-1000.;
      VDCyl4gMomX[i]=-1000.;
      VDCyl4gMomY[i]=-1000.;
      VDCyl4gMomZ[i]=-1000.;
      VDCyl4lMomX[i]=-1000.;
      VDCyl4lMomY[i]=-1000.;
      VDCyl4lMomZ[i]=-1000.;
      VDCyl4id[i]=-1000;
      VDCyl4pid[i]=-1000;
      VDCyl4trkid[i]=-1000;
      VDCyl4parentpid[i]=-1000;
      sprintf(VDCyl4process[i],"none");
      sprintf(VDCyl4vtxvolume[i],"none");
   }
}
void E16ANA_OutputData::Clear(){
   /*TAR1lPos->Clear();
   TAR1lMom->Clear();
   TAR2lPos->Clear();
   TAR2lMom->Clear();
   TAR3lPos->Clear();
   TAR3lMom->Clear();
      GTR1gPos->Clear();
      GTR1lPos->Clear();
      GTR1gMom->Clear();
      GTR1lMom->Clear();
      GTR2gPos->Clear();
      GTR2lPos->Clear();
      GTR2gMom->Clear();
      GTR2lMom->Clear();
      GTR3gPos->Clear();
      GTR3lPos->Clear();
      GTR3gMom->Clear();
      GTR3lMom->Clear();
      HBDgPos->Clear();
      HBDlPos->Clear();
      HBDgMom->Clear();
      HBDlMom->Clear();
      LGVDgPos->Clear();
      LGVDlPos->Clear();
      LGVDgMom->Clear();
      LGVDlMom->Clear();*/
}
   
void E16ANA_OutputData::DefineHistograms(){
   tree= new TTree("tree","E16 pion Data File");
   tree->Branch("evID_JAM",&evID_JAM,"evID_JAM/I");

   //Obtained momentum, chi2
   //Actual target position
   tree->Branch("VTXpos","TVector3",&VTXpos);
   char str[100];
   //Actual momentum
   sprintf(str,"VTXmom");
   tree->Branch(str,"TVector3",&VTXmom);
   //Particle ID
   sprintf(str,"VTXpid");
   tree->Branch(str,&VTXpid,256000,2);
   
   //Track ID @Target
   tree->Branch("TAR1trkid",TAR1trkid,"TAR1trkid[10]/I");
   tree->Branch("TAR2trkid",TAR2trkid,"TAR2trkid[10]/I");
   tree->Branch("TAR3trkid",TAR3trkid,"TAR3trkid[10]/I");
   tree->Branch("TAR1pid",TAR1pid,"TAR1pid[10]/I");
   tree->Branch("TAR2pid",TAR2pid,"TAR2pid[10]/I");
   tree->Branch("TAR3pid",TAR3pid,"TAR3pid[10]/I");
   tree->Branch("TAR1parentpid",&TAR1parentpid,"TAR1parentpid[10]/I");
   tree->Branch("TAR2parentpid",&TAR2parentpid,"TAR2parentpid[10]/I");
   tree->Branch("TAR3parentpid",&TAR3parentpid,"TAR3parentpid[10]/I");
   tree->Branch("TAR1lPosX",&TAR1lPosX,"TAR1lPosX[10]/D");
   tree->Branch("TAR1lPosY",&TAR1lPosY,"TAR1lPosY[10]/D");
   tree->Branch("TAR1lPosZ",&TAR1lPosZ,"TAR1lPosZ[10]/D");
   tree->Branch("TAR2lPosX",&TAR2lPosX,"TAR2lPosX[10]/D");
   tree->Branch("TAR2lPosY",&TAR2lPosY,"TAR2lPosY[10]/D");
   tree->Branch("TAR2lPosZ",&TAR2lPosZ,"TAR2lPosZ[10]/D");
   tree->Branch("TAR3lPosX",&TAR3lPosX,"TAR3lPosX[10]/D");
   tree->Branch("TAR3lPosY",&TAR3lPosY,"TAR3lPosY[10]/D");
   tree->Branch("TAR3lPosZ",&TAR3lPosZ,"TAR3lPosZ[10]/D");
   tree->Branch("TAR1lMomX",&TAR1lMomX,"TAR1lMomX[10]/D");
   tree->Branch("TAR1lMomY",&TAR1lMomY,"TAR1lMomY[10]/D");
   tree->Branch("TAR1lMomZ",&TAR1lMomZ,"TAR1lMomZ[10]/D");
   tree->Branch("TAR2lMomX",&TAR2lMomX,"TAR2lMomX[10]/D");
   tree->Branch("TAR2lMomY",&TAR2lMomY,"TAR2lMomY[10]/D");
   tree->Branch("TAR2lMomZ",&TAR2lMomZ,"TAR2lMomZ[10]/D");
   tree->Branch("TAR3lMomX",&TAR3lMomX,"TAR3lMomX[10]/D");
   tree->Branch("TAR3lMomY",&TAR3lMomY,"TAR3lMomY[10]/D");
   tree->Branch("TAR3lMomZ",&TAR3lMomZ,"TAR3lMomZ[10]/D");
   /*tree->Branch("TAR1pos",&TAR1lPos,32000,0);
   tree->Branch("TAR2pos",&TAR2lPos,32000,0);
   tree->Branch("TAR3pos",&TAR3lPos,32000,0);
   tree->Branch("TAR1mom",&TAR1lMom,32000,0);
   tree->Branch("TAR2mom",&TAR2lMom,32000,0);
   tree->Branch("TAR3mom",&TAR3lMom,32000,0);*/
   
   //Track ID @GTR
   tree->Branch("GTR1trkid",GTR1trkid,"GTR1trkid[20]/I");
   tree->Branch("GTR2trkid",GTR2trkid,"GTR2trkid[20]/I");
   tree->Branch("GTR3trkid",GTR3trkid,"GTR3trkid[20]/I");
   //GTR ID
   tree->Branch("GTR1id",GTR1id,"GTR1id[20]/I");
   tree->Branch("GTR2id",GTR2id,"GTR2id[20]/I");
   tree->Branch("GTR3id",GTR3id,"GTR3id[20]/I");
   //GTR PID
   //tree->Branch("arr",&arr,32000,0);
   //arr->BypassStreamer();
   tree->Branch("GTR1pid",GTR1pid,"GTR1pid[20]/I");
   tree->Branch("GTR2pid",GTR2pid,"GTR2pid[20]/I");
   tree->Branch("GTR3pid",GTR3pid,"GTR3pid[20]/I");
   //GTR local pos
   /*tree->Branch("GTR1pos",&GTR1lPos,32000,0);
   tree->Branch("GTR2pos",&GTR2lPos,32000,0);
   tree->Branch("GTR3pos",&GTR3lPos,32000,0);*/
   tree->Branch("GTR1lPosX",&GTR1lPosX,"GTR1lPosX[20]/D");
   tree->Branch("GTR1lPosY",&GTR1lPosY,"GTR1lPosY[20]/D");
   tree->Branch("GTR1lPosZ",&GTR1lPosZ,"GTR1lPosZ[20]/D");
   tree->Branch("GTR2lPosX",&GTR2lPosX,"GTR2lPosX[20]/D");
   tree->Branch("GTR2lPosY",&GTR2lPosY,"GTR2lPosY[20]/D");
   tree->Branch("GTR2lPosZ",&GTR2lPosZ,"GTR2lPosZ[20]/D");
   tree->Branch("GTR3lPosX",&GTR3lPosX,"GTR3lPosX[20]/D");
   tree->Branch("GTR3lPosY",&GTR3lPosY,"GTR3lPosY[20]/D");
   tree->Branch("GTR3lPosZ",&GTR3lPosZ,"GTR3lPosZ[20]/D");
   //GTR global pos
   /*tree->Branch("GTR1gpos",&GTR1gPos,32000,0);
   tree->Branch("GTR2gpos",&GTR2gPos,32000,0);
   tree->Branch("GTR3gpos",&GTR3gPos,32000,0);*/
   tree->Branch("GTR1gPosX",&GTR1gPosX,"GTR1gPosX[20]/D");
   tree->Branch("GTR1gPosY",&GTR1gPosY,"GTR1gPosY[20]/D");
   tree->Branch("GTR1gPosZ",&GTR1gPosZ,"GTR1gPosZ[20]/D");
   tree->Branch("GTR2gPosX",&GTR2gPosX,"GTR2gPosX[20]/D");
   tree->Branch("GTR2gPosY",&GTR2gPosY,"GTR2gPosY[20]/D");
   tree->Branch("GTR2gPosZ",&GTR2gPosZ,"GTR2gPosZ[20]/D");
   tree->Branch("GTR3gPosX",&GTR3gPosX,"GTR3gPosX[20]/D");
   tree->Branch("GTR3gPosY",&GTR3gPosY,"GTR3gPosY[20]/D");
   tree->Branch("GTR3gPosZ",&GTR3gPosZ,"GTR3gPosZ[20]/D");
   //GTR vertex pos
   tree->Branch("GTR1vPosX",&GTR1vPosX,"GTR1vPosX[20]/D");
   tree->Branch("GTR1vPosY",&GTR1vPosY,"GTR1vPosY[20]/D");
   tree->Branch("GTR1vPosZ",&GTR1vPosZ,"GTR1vPosZ[20]/D");
   tree->Branch("GTR2vPosX",&GTR2vPosX,"GTR2vPosX[20]/D");
   tree->Branch("GTR2vPosY",&GTR2vPosY,"GTR2vPosY[20]/D");
   tree->Branch("GTR2vPosZ",&GTR2vPosZ,"GTR2vPosZ[20]/D");
   tree->Branch("GTR3vPosX",&GTR3vPosX,"GTR3vPosX[20]/D");
   tree->Branch("GTR3vPosY",&GTR3vPosY,"GTR3vPosY[20]/D");
   tree->Branch("GTR3vPosZ",&GTR3vPosZ,"GTR3vPosZ[20]/D");
   //GTR local mom 
   /*tree->Branch("GTR1mom",&GTR1lMom,32000,0);
   tree->Branch("GTR2mom",&GTR2lMom,32000,0);
   tree->Branch("GTR3mom",&GTR3lMom,32000,0);*/
   tree->Branch("GTR1lMomX",&GTR1lMomX,"GTR1lMomX[20]/D");
   tree->Branch("GTR1lMomY",&GTR1lMomY,"GTR1lMomY[20]/D");
   tree->Branch("GTR1lMomZ",&GTR1lMomZ,"GTR1lMomZ[20]/D");
   tree->Branch("GTR2lMomX",&GTR2lMomX,"GTR2lMomX[20]/D");
   tree->Branch("GTR2lMomY",&GTR2lMomY,"GTR2lMomY[20]/D");
   tree->Branch("GTR2lMomZ",&GTR2lMomZ,"GTR2lMomZ[20]/D");
   tree->Branch("GTR3lMomX",&GTR3lMomX,"GTR3lMomX[20]/D");
   tree->Branch("GTR3lMomY",&GTR3lMomY,"GTR3lMomY[20]/D");
   tree->Branch("GTR3lMomZ",&GTR3lMomZ,"GTR3lMomZ[20]/D");

   //Decay information
   tree->Branch("GTR1parentpid",&GTR1parentpid,"GTR1parentpid[20]/I");
   tree->Branch("GTR2parentpid",&GTR2parentpid,"GTR2parentpid[20]/I");
   tree->Branch("GTR3parentpid",&GTR3parentpid,"GTR3parentpid[20]/I");
   tree->Branch("HBDparentpid",&HBDparentpid,"HBDparentpid[20]/I");
   tree->Branch("LGVDparentpid",&LGVDparentpid,"LGVDparentpid[20]/I");
   tree->Branch("VDCyl1parentpid",&VDCyl1parentpid,"VDCyl1parentpid[20]/I");
   tree->Branch("VDCyl2parentpid",&VDCyl2parentpid,"VDCyl2parentpid[20]/I");
   tree->Branch("VDCyl3parentpid",&VDCyl3parentpid,"VDCyl3parentpid[20]/I");
   tree->Branch("VDCyl4parentpid",&VDCyl4parentpid,"VDCyl4parentpid[20]/I");
   //Corresponding track id at target
   tree->Branch("GTR1originID",&GTR1originid,"GTR1originID[20]/I");
   tree->Branch("GTR2originID",&GTR2originid,"GTR2originID[20]/I");
   tree->Branch("GTR3originID",&GTR3originid,"GTR3originID[20]/I");
   tree->Branch("HBDoriginID",&HBDoriginid,"HBDoriginID[20]/I");
   for (int i=0;i<20;i++){
      char str1[100];
      char str2[100];
      sprintf(str1,"GTR1process%d",i);
      sprintf(str2,"GTR1process%d/C",i);
      tree->Branch(str1,&GTR1process[i],str2);
      sprintf(str1,"GTR2process%d",i);
      sprintf(str2,"GTR2process%d/C",i);
      tree->Branch(str1,&GTR2process[i],str2);
      sprintf(str1,"GTR3process%d",i);
      sprintf(str2,"GTR3process%d/C",i);
      tree->Branch(str1,&GTR3process[i],str2);
      sprintf(str1,"GTR1vtxvolume%d",i);
      sprintf(str2,"GTR1vtxvolume%d/C",i);
      tree->Branch(str1,&GTR1vtxvolume[i],str2);
      sprintf(str1,"GTR2vtxvolume%d",i);
      sprintf(str2,"GTR2vtxvolume%d/C",i);
      tree->Branch(str1,&GTR2vtxvolume[i],str2);
      sprintf(str1,"GTR3vtxvolume%d",i);
      sprintf(str2,"GTR3vtxvolume%d/C",i);
      tree->Branch(str1,&GTR3vtxvolume[i],str2);
   }
   for (int i=0;i<20;i++){
      char str1[100];
      char str2[100];
      sprintf(str1,"HBDprocess%d",i);
      sprintf(str2,"HBDprocess%d/C",i);
      tree->Branch(str1,&HBDprocess[i],str2);
      sprintf(str1,"HBDvtxvolume%d",i);
      sprintf(str2,"HBDvtxvolume%d/C",i);
      tree->Branch(str1,&HBDvtxvolume[i],str2);
   }
   for (int i=0;i<20;i++){
      char str1[100];
      char str2[100];
      sprintf(str1,"LGVDprocess%d",i);
      sprintf(str2,"LGVDprocess%d/C",i);
      tree->Branch(str1,&LGVDprocess[i],str2);
      sprintf(str1,"LGVDvtxvolume%d",i);
      sprintf(str2,"LGVDvtxvolume%d/C",i);
      tree->Branch(str1,&LGVDvtxvolume[i],str2);
   }
   for (int i=0;i<20;i++){
      char str1[100];
      char str2[100];
      sprintf(str1,"VDCyl1process%d",i);
      sprintf(str2,"VDCyl1process%d/C",i);
      tree->Branch(str1,&VDCyl1process[i],str2);
      sprintf(str1,"VDCyl1vtxvolume%d",i);
      sprintf(str2,"VDCyl1vtxvolume%d/C",i);
      tree->Branch(str1,&VDCyl1vtxvolume[i],str2);
   }
   for (int i=0;i<20;i++){
      char str1[100];
      char str2[100];
      sprintf(str1,"VDCyl2process%d",i);
      sprintf(str2,"VDCyl2process%d/C",i);
      tree->Branch(str1,&VDCyl2process[i],str2);
      sprintf(str1,"VDCyl2vtxvolume%d",i);
      sprintf(str2,"VDCyl2vtxvolume%d/C",i);
      tree->Branch(str1,&VDCyl2vtxvolume[i],str2);
   }
   for (int i=0;i<20;i++){
      char str1[100];
      char str2[100];
      sprintf(str1,"VDCyl3process%d",i);
      sprintf(str2,"VDCyl3process%d/C",i);
      tree->Branch(str1,&VDCyl3process[i],str2);
      sprintf(str1,"VDCyl3vtxvolume%d",i);
      sprintf(str2,"VDCyl3vtxvolume%d/C",i);
      tree->Branch(str1,&VDCyl3vtxvolume[i],str2);
   }
   for (int i=0;i<20;i++){
      char str1[100];
      char str2[100];
      sprintf(str1,"VDCyl4process%d",i);
      sprintf(str2,"VDCyl4process%d/C",i);
      tree->Branch(str1,&VDCyl4process[i],str2);
      sprintf(str1,"VDCyl4vtxvolume%d",i);
      sprintf(str2,"VDCyl4vtxvolume%d/C",i);
      tree->Branch(str1,&VDCyl4vtxvolume[i],str2);
   }



   //HBD 
   tree->Branch("HBDid",HBDid,"HBDid[20]/I");
   tree->Branch("HBDtrkid",HBDtrkid,"HBDtrkid[20]/I");
   tree->Branch("HBDpid",HBDpid,"HBDpid[20]/I");
   tree->Branch("HBDfl",HBDfl,"HBDfl[20]/D");
   //HBD local pos
   //tree->Branch("HBDpos",&HBDlPos,32000,0);
   tree->Branch("HBDlPosX",&HBDlPosX,"HBDlPosX[20]/D");
   tree->Branch("HBDlPosY",&HBDlPosY,"HBDlPosY[20]/D");
   tree->Branch("HBDlPosZ",&HBDlPosZ,"HBDlPosZ[20]/D");
   //HBD global pos
   //tree->Branch("HBDgpos",&HBDgPos,32000,0);
   tree->Branch("HBDgPosX",&HBDgPosX,"HBDgPosX[20]/D");
   tree->Branch("HBDgPosY",&HBDgPosY,"HBDgPosY[20]/D");
   tree->Branch("HBDgPosZ",&HBDgPosZ,"HBDgPosZ[20]/D");
   //HBD vertex pos
   tree->Branch("HBDvPosX",&HBDvPosX,"HBDvPosX[20]/D");
   tree->Branch("HBDvPosY",&HBDvPosY,"HBDvPosY[20]/D");
   tree->Branch("HBDvPosZ",&HBDvPosZ,"HBDvPosZ[20]/D");
   //HBD local mom 
   //tree->Branch("HBDmom",&HBDlMom,32000,0);
   tree->Branch("HBDlMomX",&HBDlMomX,"HBDlMomX[20]/D");
   tree->Branch("HBDlMomY",&HBDlMomY,"HBDlMomY[20]/D");
   tree->Branch("HBDlMomZ",&HBDlMomZ,"HBDlMomZ[20]/D");
   
   //LGVD 
   tree->Branch("LGVDid",LGVDid,"LGVDid[20]/I");
   tree->Branch("LGVDtrkid",LGVDtrkid,"LGVDtrkid[20]/I");
   tree->Branch("LGVDpid",LGVDpid,"LGVDpid[20]/I");
   //LGVD local pos
   tree->Branch("LGVDlPosX",&LGVDlPosX,"LGVDlPosX[20]/D");
   tree->Branch("LGVDlPosY",&LGVDlPosY,"LGVDlPosY[20]/D");
   tree->Branch("LGVDlPosZ",&LGVDlPosZ,"LGVDlPosZ[20]/D");
   //LGVD global pos
   tree->Branch("LGVDgPosX",&LGVDgPosX,"LGVDgPosX[20]/D");
   tree->Branch("LGVDgPosY",&LGVDgPosY,"LGVDgPosY[20]/D");
   tree->Branch("LGVDgPosZ",&LGVDgPosZ,"LGVDgPosZ[20]/D");
   //LGVD global pos
   tree->Branch("LGVDvPosX",&LGVDvPosX,"LGVDvPosX[20]/D");
   tree->Branch("LGVDvPosY",&LGVDvPosY,"LGVDvPosY[20]/D");
   tree->Branch("LGVDvPosZ",&LGVDvPosZ,"LGVDvPosZ[20]/D");
   //LGVD local mom 
   tree->Branch("LGVDlMomX",&LGVDlMomX,"LGVDlMomX[20]/D");
   tree->Branch("LGVDlMomY",&LGVDlMomY,"LGVDlMomY[20]/D");
   tree->Branch("LGVDlMomZ",&LGVDlMomZ,"LGVDlMomZ[20]/D");
   
   //VDCyl1 
   tree->Branch("VDCyl1id",VDCyl1id,"VDCyl1id[20]/I");
   tree->Branch("VDCyl1trkid",VDCyl1trkid,"VDCyl1trkid[20]/I");
   tree->Branch("VDCyl1pid",VDCyl1pid,"VDCyl1pid[20]/I");
   tree->Branch("VDCyl1lPosZ",&VDCyl1lPosZ,"VDCyl1lPosZ[20]/D");
   //VDCyl1 global pos
   tree->Branch("VDCyl1gPosX",&VDCyl1gPosX,"VDCyl1gPosX[20]/D");
   tree->Branch("VDCyl1gPosY",&VDCyl1gPosY,"VDCyl1gPosY[20]/D");
   tree->Branch("VDCyl1gPosZ",&VDCyl1gPosZ,"VDCyl1gPosZ[20]/D");
   //VDCyl1 global pos
   tree->Branch("VDCyl1vPosX",&VDCyl1vPosX,"VDCyl1vPosX[20]/D");
   tree->Branch("VDCyl1vPosY",&VDCyl1vPosY,"VDCyl1vPosY[20]/D");
   tree->Branch("VDCyl1vPosZ",&VDCyl1vPosZ,"VDCyl1vPosZ[20]/D");
   //VDCyl1 local mom 
   tree->Branch("VDCyl1lMomX",&VDCyl1lMomX,"VDCyl1lMomX[20]/D");
   tree->Branch("VDCyl1lMomY",&VDCyl1lMomY,"VDCyl1lMomY[20]/D");
   tree->Branch("VDCyl1lMomZ",&VDCyl1lMomZ,"VDCyl1lMomZ[20]/D");
   
   //VDCyl2 
   tree->Branch("VDCyl2id",VDCyl2id,"VDCyl2id[20]/I");
   tree->Branch("VDCyl2trkid",VDCyl2trkid,"VDCyl2trkid[20]/I");
   tree->Branch("VDCyl2pid",VDCyl2pid,"VDCyl2pid[20]/I");
   //VDCyl2 global pos
   tree->Branch("VDCyl2gPosX",&VDCyl2gPosX,"VDCyl2gPosX[20]/D");
   tree->Branch("VDCyl2gPosY",&VDCyl2gPosY,"VDCyl2gPosY[20]/D");
   tree->Branch("VDCyl2gPosZ",&VDCyl2gPosZ,"VDCyl2gPosZ[20]/D");
   //VDCyl2 global pos
   tree->Branch("VDCyl2vPosX",&VDCyl2vPosX,"VDCyl2vPosX[20]/D");
   tree->Branch("VDCyl2vPosY",&VDCyl2vPosY,"VDCyl2vPosY[20]/D");
   tree->Branch("VDCyl2vPosZ",&VDCyl2vPosZ,"VDCyl2vPosZ[20]/D");
   //VDCyl2 local mom 
   tree->Branch("VDCyl2lMomX",&VDCyl2lMomX,"VDCyl2lMomX[20]/D");
   tree->Branch("VDCyl2lMomY",&VDCyl2lMomY,"VDCyl2lMomY[20]/D");
   tree->Branch("VDCyl2lMomZ",&VDCyl2lMomZ,"VDCyl2lMomZ[20]/D");
   
   //VDCyl3 
   tree->Branch("VDCyl3id",VDCyl3id,"VDCyl3id[20]/I");
   tree->Branch("VDCyl3trkid",VDCyl3trkid,"VDCyl3trkid[20]/I");
   tree->Branch("VDCyl3pid",VDCyl3pid,"VDCyl3pid[20]/I");
   //VDCyl3 global pos
   tree->Branch("VDCyl3gPosX",&VDCyl3gPosX,"VDCyl3gPosX[20]/D");
   tree->Branch("VDCyl3gPosY",&VDCyl3gPosY,"VDCyl3gPosY[20]/D");
   tree->Branch("VDCyl3gPosZ",&VDCyl3gPosZ,"VDCyl3gPosZ[20]/D");
   //VDCyl3 global pos
   tree->Branch("VDCyl3vPosX",&VDCyl3vPosX,"VDCyl3vPosX[20]/D");
   tree->Branch("VDCyl3vPosY",&VDCyl3vPosY,"VDCyl3vPosY[20]/D");
   tree->Branch("VDCyl3vPosZ",&VDCyl3vPosZ,"VDCyl3vPosZ[20]/D");
   //VDCyl3 local mom 
   tree->Branch("VDCyl3lMomX",&VDCyl3lMomX,"VDCyl3lMomX[20]/D");
   tree->Branch("VDCyl3lMomY",&VDCyl3lMomY,"VDCyl3lMomY[20]/D");
   tree->Branch("VDCyl3lMomZ",&VDCyl3lMomZ,"VDCyl3lMomZ[20]/D");
   
   //VDCyl4 
   tree->Branch("VDCyl4id",VDCyl4id,"VDCyl4id[20]/I");
   tree->Branch("VDCyl4trkid",VDCyl4trkid,"VDCyl4trkid[20]/I");
   tree->Branch("VDCyl4pid",VDCyl4pid,"VDCyl4pid[20]/I");
   //VDCyl4 global pos
   tree->Branch("VDCyl4gPosX",&VDCyl4gPosX,"VDCyl4gPosX[20]/D");
   tree->Branch("VDCyl4gPosY",&VDCyl4gPosY,"VDCyl4gPosY[20]/D");
   tree->Branch("VDCyl4gPosZ",&VDCyl4gPosZ,"VDCyl4gPosZ[20]/D");
   //VDCyl4 global pos
   tree->Branch("VDCyl4vPosX",&VDCyl4vPosX,"VDCyl4vPosX[20]/D");
   tree->Branch("VDCyl4vPosY",&VDCyl4vPosY,"VDCyl4vPosY[20]/D");
   tree->Branch("VDCyl4vPosZ",&VDCyl4vPosZ,"VDCyl4vPosZ[20]/D");
   //VDCyl4 local mom 
   tree->Branch("VDCyl4lMomX",&VDCyl4lMomX,"VDCyl4lMomX[20]/D");
   tree->Branch("VDCyl4lMomY",&VDCyl4lMomY,"VDCyl4lMomY[20]/D");
   tree->Branch("VDCyl4lMomZ",&VDCyl4lMomZ,"VDCyl4lMomZ[20]/D");

   //sprintf(str,"Bdl");
   //tree->Branch(str,&Bdl);

   sprintf(str,"nTAR1hit");
   tree->Branch(str,&nTAR1hit);
   sprintf(str,"nTAR2hit");
   tree->Branch(str,&nTAR2hit);
   sprintf(str,"nTAR3hit");
   tree->Branch(str,&nTAR3hit);
   sprintf(str,"nGTR1hit");
   tree->Branch(str,&nGTR1hit);
   sprintf(str,"nGTR2hit");
   tree->Branch(str,&nGTR2hit);
   sprintf(str,"nGTR3hit");
   tree->Branch(str,&nGTR3hit);
   sprintf(str,"nHBDhit");
   tree->Branch(str,&nHBDhit);
   sprintf(str,"nLGVDhit");
   tree->Branch(str,&nLGVDhit);
   /*for (int i=0;i<nTrack;i++){
     char str[100];
   //Actual momentum
   sprintf(str,"VTXmom%d",i);
   tree->Branch(str,"TVector3",&VTXmom[i]);
   //Particle ID
   sprintf(str,"PDGID%d",i);
   tree->Branch(str,&PDGID[i]);
   //GTR ID
   sprintf(str,"GTR1id%d",i);
   tree->Branch(str,&GTRid[0][i]);
   sprintf(str,"GTR2id%d",i);
   tree->Branch(str,&GTRid[1][i]);
   sprintf(str,"GTR3id%d",i);
   tree->Branch(str,&GTRid[2][i]);
   //GTR local pos
   sprintf(str,"GTR1pos%d",i);
   tree->Branch(str,"TVector3",&GTR1lPos[i]);
   sprintf(str,"GTR2pos%d",i);
   tree->Branch(str,"TVector3",&GTR2lPos[i]);
   sprintf(str,"GTR3pos%d",i);
   tree->Branch(str,"TVector3",&GTR3lPos[i]);
   //GTR global pos
   sprintf(str,"GTR1gpos%d",i);
   tree->Branch(str,"TVector3",&GTR1gPos[i]);
   sprintf(str,"GTR2gpos%d",i);
   tree->Branch(str,"TVector3",&GTR2gPos[i]);
   sprintf(str,"GTR3gpos%d",i);
   tree->Branch(str,"TVector3",&GTR3gPos[i]);
   //GTR local mom 
   sprintf(str,"GTR1mom%d",i);
   tree->Branch(str,"TVector3",&GTR1lMom[i]);
   sprintf(str,"GTR2mom%d",i);
   tree->Branch(str,"TVector3",&GTR2lMom[i]);
   sprintf(str,"GTR3mom%d",i);
   tree->Branch(str,"TVector3",&GTR3lMom[i]);

   sprintf(str,"Bdl%d",i);
   tree->Branch(str,&Bdl[i]);

   sprintf(str,"nHBDhit%d",i);
   tree->Branch(str,&nHBDhit[i]);
   sprintf(str,"nLGVDhit%d",i);
   tree->Branch(str,&nLGVDhit[i]);
   }*/
}
