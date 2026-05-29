//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai
//2015-11-02, uploaded by yokkaich
//2015-11-02, uploaded by komatsu
//2015-08-20, uploaded by yokkaich
//2015-03-28, uploaded by yokkaich
//2015-03-27, uploaded by yokkaich
//2015-03-01, uploaded by yokkaich
//2015-01-28, uploaded by komatsu
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

//E16ANA_DoubleTrackData.cc
//    Last modified at <>
//

#include <TVector3.h>
#include <TRotation.h>
#include <fstream> 
#include <iostream> 

#include "E16ANA_DoubleTrackData.hh"

using namespace std;
const int MAX_LENGTH=100;

const int E16ANA_DoubleTrackData::maxGTR1hit;
const int E16ANA_DoubleTrackData::maxGTR2hit;
const int E16ANA_DoubleTrackData::maxGTR3hit;
const int E16ANA_DoubleTrackData::maxHBDhit;
const int E16ANA_DoubleTrackData::maxLGVDhit;
const int E16ANA_DoubleTrackData::maxVDCyl1hit;
const int E16ANA_DoubleTrackData::maxVDCyl2hit;
const int E16ANA_DoubleTrackData::maxVDCyl3hit;
const int E16ANA_DoubleTrackData::maxVDCyl4hit;


E16ANA_DoubleTrackData::E16ANA_DoubleTrackData(char* filename, 
      int maxTrkID, char* readwrite)
:filename_(filename),maxTrkID_(maxTrkID),readwrite_(readwrite)
{
   file= new TFile(filename_,"recreate");
   VTXpos=TVector3();
   VTXpos_fitd=TVector3();
   for (int i=0;i<nTrack;i++){
      VTXpos_fits[i]=TVector3();
      VTXmom[i]=TVector3();
      VTXmom_fits[i]=TVector3();
      VTXmom_fitd[i]=TVector3();
      GTR1gPos[i]=TVector3();
      GTR2gPos[i]=TVector3();
      GTR3gPos[i]=TVector3();
      GTR1lPos[i]=TVector3();
      GTR2lPos[i]=TVector3();
      GTR3lPos[i]=TVector3();
      GTR1vPos[i]=TVector3();
      GTR2vPos[i]=TVector3();
      GTR3vPos[i]=TVector3();
      GTR1gMom[i]=TVector3();
      GTR2gMom[i]=TVector3();
      GTR3gMom[i]=TVector3();
      GTR1lMom[i]=TVector3();
      GTR2lMom[i]=TVector3();
      GTR3lMom[i]=TVector3();
      GTR1gPos_fits[i]=TVector3();
      GTR2gPos_fits[i]=TVector3();
      GTR3gPos_fits[i]=TVector3();
      GTR1lPos_fits[i]=TVector3();
      GTR2lPos_fits[i]=TVector3();
      GTR3lPos_fits[i]=TVector3();
      GTR1gMom_fits[i]=TVector3();
      GTR2gMom_fits[i]=TVector3();
      GTR3gMom_fits[i]=TVector3();
      GTR1lMom_fits[i]=TVector3();
      GTR2lMom_fits[i]=TVector3();
      GTR3lMom_fits[i]=TVector3();
      GTR1gPos_fitd[i]=TVector3();
      GTR2gPos_fitd[i]=TVector3();
      GTR3gPos_fitd[i]=TVector3();
      GTR1lPos_fitd[i]=TVector3();
      GTR2lPos_fitd[i]=TVector3();
      GTR3lPos_fitd[i]=TVector3();
      GTR1gMom_fitd[i]=TVector3();
      GTR2gMom_fitd[i]=TVector3();
      GTR3gMom_fitd[i]=TVector3();
      GTR1lMom_fitd[i]=TVector3();
      GTR2lMom_fitd[i]=TVector3();
      GTR3lMom_fitd[i]=TVector3();
      GTRFrame1gPos[i]=TVector3();
      GTRFrame2gPos[i]=TVector3();
      GTRFrame3gPos[i]=TVector3();
      HBDgPos[i]=TVector3();
      HBDlPos[i]=TVector3();
      HBDvPos[i]=TVector3();
      HBDgMom[i]=TVector3();
      HBDlMom[i]=TVector3();
      LGVDgPos[i]=TVector3();
      LGVDlPos[i]=TVector3();
      LGVDvPos[i]=TVector3();
      LGVDgMom[i]=TVector3();
      LGVDlMom[i]=TVector3();
      VDCyl1gPos[i]=TVector3();
      VDCyl1vPos[i]=TVector3();
      VDCyl1gMom[i]=TVector3();
      VDCyl2gPos[i]=TVector3();
      VDCyl2vPos[i]=TVector3();
      VDCyl2gMom[i]=TVector3();
      VDCyl3gPos[i]=TVector3();
      VDCyl3vPos[i]=TVector3();
      VDCyl3gMom[i]=TVector3();
      VDCyl4gPos[i]=TVector3();
      VDCyl4vPos[i]=TVector3();
      VDCyl4gMom[i]=TVector3();
   }
   DefineHistograms();
}

E16ANA_DoubleTrackData::~E16ANA_DoubleTrackData(){
}

void E16ANA_DoubleTrackData::Fill(){
   tree->Fill();
}

void E16ANA_DoubleTrackData::Initialize(){
      dMinuitStat=-10;
   for (int i=0;i<nTrack;i++){
      GTR1gPos[i].SetXYZ(-1000.,-1000.,-1000.);
      GTR1lPos[i].SetXYZ(-1000.,-1000.,-1000.);
      GTR1vPos[i].SetXYZ(-1000.,-1000.,-1000.);
      GTR1gMom[i].SetXYZ(-1000.,-1000.,-1000.);
      GTR1lMom[i].SetXYZ(-1000.,-1000.,-1000.);
      GTRid[0][i]=-1000;
      GTR1parentpid[i]=-1000;
      sprintf(GTR1process[i],"none");
      sprintf(GTR1vtxvolume[i],"none");
   }
   for (int i=0;i<nTrack;i++){
      GTR2gPos[i].SetXYZ(-1000.,-1000.,-1000.);
      GTR2lPos[i].SetXYZ(-1000.,-1000.,-1000.);
      GTR2vPos[i].SetXYZ(-1000.,-1000.,-1000.);
      GTR2gMom[i].SetXYZ(-1000.,-1000.,-1000.);
      GTR2lMom[i].SetXYZ(-1000.,-1000.,-1000.);
      GTRid[1][i]=-1000;
      GTR2parentpid[i]=-1000;
      sprintf(GTR2process[i],"none");
      sprintf(GTR2vtxvolume[i],"none");
   }
   for (int i=0;i<nTrack;i++){
      GTR3gPos[i].SetXYZ(-1000.,-1000.,-1000.);
      GTR3lPos[i].SetXYZ(-1000.,-1000.,-1000.);
      GTR3vPos[i].SetXYZ(-1000.,-1000.,-1000.);
      GTR3gMom[i].SetXYZ(-1000.,-1000.,-1000.);
      GTR3lMom[i].SetXYZ(-1000.,-1000.,-1000.);
      GTRid[2][i]=-1000;
      GTR3parentpid[i]=-1000;
      sprintf(GTR3process[i],"none");
      sprintf(GTR3vtxvolume[i],"none");
   }
   for (int i=0;i<nTrack;i++){
      HBDgPos[i].SetXYZ(-1000.,-1000.,-1000.);
      HBDlPos[i].SetXYZ(-1000.,-1000.,-1000.);
      HBDvPos[i].SetXYZ(-1000.,-1000.,-1000.);
      HBDgMom[i].SetXYZ(-1000.,-1000.,-1000.);
      HBDlMom[i].SetXYZ(-1000.,-1000.,-1000.);
      HBDid[i]=-1000;
      HBDpid[i]=-1000;
      HBDtrkid[i]=-1000;
      HBDparentpid[i]=-1000;
      HBDfl[i]=-1000;
      sprintf(HBDprocess[i],"none");
      sprintf(HBDvtxvolume[i],"none");
      LGVDgPos[i].SetXYZ(-1000.,-1000.,-1000.);
      LGVDlPos[i].SetXYZ(-1000.,-1000.,-1000.);
      LGVDvPos[i].SetXYZ(-1000.,-1000.,-1000.);
      LGVDgMom[i].SetXYZ(-1000.,-1000.,-1000.);
      LGVDlMom[i].SetXYZ(-1000.,-1000.,-1000.);
      LGVDid[i]=-1000;
      LGVDpid[i]=-1000;
      LGVDtrkid[i]=-1000;
      LGVDparentpid[i]=-1000;
      sprintf(LGVDprocess[i],"none");
      sprintf(LGVDvtxvolume[i],"none");
      
      VDCyl1gPos[i].SetXYZ(-1000.,-1000.,-1000.);
      VDCyl1vPos[i].SetXYZ(-1000.,-1000.,-1000.);
      VDCyl1gMom[i].SetXYZ(-1000.,-1000.,-1000.);
      VDCyl2gPos[i].SetXYZ(-1000.,-1000.,-1000.);
      VDCyl2vPos[i].SetXYZ(-1000.,-1000.,-1000.);
      VDCyl2gMom[i].SetXYZ(-1000.,-1000.,-1000.);
      VDCyl3gPos[i].SetXYZ(-1000.,-1000.,-1000.);
      VDCyl3vPos[i].SetXYZ(-1000.,-1000.,-1000.);
      VDCyl3gMom[i].SetXYZ(-1000.,-1000.,-1000.);
      VDCyl4gPos[i].SetXYZ(-1000.,-1000.,-1000.);
      VDCyl4vPos[i].SetXYZ(-1000.,-1000.,-1000.);
      VDCyl4gMom[i].SetXYZ(-1000.,-1000.,-1000.);
      sprintf(VDCyl1process[i],"none");
      sprintf(VDCyl1vtxvolume[i],"none");
      sprintf(VDCyl2process[i],"none");
      sprintf(VDCyl2vtxvolume[i],"none");
      sprintf(VDCyl3process[i],"none");
      sprintf(VDCyl3vtxvolume[i],"none");
      sprintf(VDCyl4process[i],"none");
      sprintf(VDCyl4vtxvolume[i],"none");
      sMinuitStat[i]=-10;
   }
   for (int i=0;i<20;i++){
      sprintf(GTR1hitlist[0][i],"none");
      sprintf(GTR2hitlist[0][i],"none");
      sprintf(GTR3hitlist[0][i],"none");
      sprintf(HBDhitlist[0][i],"none");
      sprintf(GTR1hitlist[1][i],"none");
      sprintf(GTR2hitlist[1][i],"none");
      sprintf(GTR3hitlist[1][i],"none");
      sprintf(HBDhitlist[1][i],"none");
   }
}

void E16ANA_DoubleTrackData::WritetoFile(){
   file->Write();
   file->Close();
}

void E16ANA_DoubleTrackData::DefineHistograms(){
   tree= new TTree("tree","E16 Double Track Data File");
   //Obtained momentum, chi2
   //Actual target position
   tree->Branch("VTXpos","TVector3",&VTXpos);
   tree->Branch("VTXpos_cp","TVector3",&VTXpos_cp);
   tree->Branch("VTXpos_fitd","TVector3",&VTXpos_fitd);
   tree->Branch("chisq_fitd",&chisq_fitd,"chisq_fitd/F");
   tree->Branch("dMinuitStat",&dMinuitStat);
   for (int i=0;i<nTrack;i++){
      char str[100];
      sprintf(str,"VTXmom%d_fits",i);
      tree->Branch(str,"TVector3",&VTXmom_fits[i]);
      sprintf(str,"VTXmom%d_fitd",i);
      tree->Branch(str,"TVector3",&VTXmom_fitd[i]);
      sprintf(str,"chisq%d_fits",i);
      tree->Branch(str,&chisq_fits[i]);
      /*tree->Branch("chi2fit_0",&chi2_fit0,"chi2fit_0/F");
        tree->Branch("chi2fit_1",&chi2_fit1,"chi2fit_1/F");
        tree->Branch("chi2fit_d",&chi2_fitd,"chi2fit_d/F");*/
      //Obtained target position
      sprintf(str,"VTXpos%d_fits",i);
      tree->Branch(str,"TVector3",&VTXpos_fits[i]);
      //Actual momentum
      sprintf(str,"VTXmom%d",i);
      tree->Branch(str,"TVector3",&VTXmom[i]);
      //Particle ID
      sprintf(str,"VTXpid%d",i);
      tree->Branch(str,&PDGID[i]);
      
      //GTR ID
      sprintf(str,"GTR1id%d",i);
      tree->Branch(str,&GTRid[0][i]);
      sprintf(str,"GTR2id%d",i);
      tree->Branch(str,&GTRid[1][i]);
      sprintf(str,"GTR3id%d",i);
      tree->Branch(str,&GTRid[2][i]);
      //GTR original local pos
      sprintf(str,"GTR1pos%d_org",i);
      tree->Branch(str,"TVector3",&GTR1lPos_org[i]);
      sprintf(str,"GTR2pos%d_org",i);
      tree->Branch(str,"TVector3",&GTR2lPos_org[i]);
      sprintf(str,"GTR3pos%d_org",i);
      tree->Branch(str,"TVector3",&GTR3lPos_org[i]);
      //GTR global pos
      sprintf(str,"GTR1gpos%d_org",i);
      tree->Branch(str,"TVector3",&GTR1gPos_org[i]);
      sprintf(str,"GTR2gpos%d_org",i);
      tree->Branch(str,"TVector3",&GTR2gPos_org[i]);
      sprintf(str,"GTR3gpos%d_org",i);
      tree->Branch(str,"TVector3",&GTR3gPos_org[i]);
      //GTR local pos
      sprintf(str,"GTR1pos%d",i);
      tree->Branch(str,"TVector3",&GTR1lPos[i]);
      sprintf(str,"GTR2pos%d",i);
      tree->Branch(str,"TVector3",&GTR2lPos[i]);
      sprintf(str,"GTR3pos%d",i);
      tree->Branch(str,"TVector3",&GTR3lPos[i]);
      //GTR vertex pos
      sprintf(str,"GTR1vpos%d",i);
      tree->Branch(str,"TVector3",&GTR1vPos[i]);
      sprintf(str,"GTR2vpos%d",i);
      tree->Branch(str,"TVector3",&GTR2vPos[i]);
      sprintf(str,"GTR3vpos%d",i);
      tree->Branch(str,"TVector3",&GTR3vPos[i]);
      //GTR global pos
      sprintf(str,"GTR1gpos%d",i);
      tree->Branch(str,"TVector3",&GTR1gPos[i]);
      sprintf(str,"GTR2gpos%d",i);
      tree->Branch(str,"TVector3",&GTR2gPos[i]);
      sprintf(str,"GTR3gpos%d",i);
      tree->Branch(str,"TVector3",&GTR3gPos[i]);
      //GTR localposition after single fit 
      sprintf(str,"GTR1pos%d_fits",i);
      tree->Branch(str,"TVector3",&GTR1lPos_fits[i]);
      sprintf(str,"GTR2pos%d_fits",i);
      tree->Branch(str,"TVector3",&GTR2lPos_fits[i]);
      sprintf(str,"GTR3pos%d_fits",i);
      tree->Branch(str,"TVector3",&GTR3lPos_fits[i]);
      //GTR localposition after single fit 
      sprintf(str,"GTR1pos%d_fitd",i);
      tree->Branch(str,"TVector3",&GTR1lPos_fitd[i]);
      sprintf(str,"GTR2pos%d_fitd",i);
      tree->Branch(str,"TVector3",&GTR2lPos_fitd[i]);
      sprintf(str,"GTR3pos%d_fitd",i);
      tree->Branch(str,"TVector3",&GTR3lPos_fitd[i]);
      //GTR local mom 
      sprintf(str,"GTR1mom%d",i);
      tree->Branch(str,"TVector3",&GTR1lMom[i]);
      sprintf(str,"GTR2mom%d",i);
      tree->Branch(str,"TVector3",&GTR2lMom[i]);
      sprintf(str,"GTR3mom%d",i);
      tree->Branch(str,"TVector3",&GTR3lMom[i]);
      //Step Length
      sprintf(str,"GTR1sl%d",i);
      tree->Branch(str,&GTR1SL[i]);
      sprintf(str,"GTR2sl%d",i);
      tree->Branch(str,&GTR2SL[i]);
      sprintf(str,"GTR3sl%d",i);
      tree->Branch(str,&GTR3SL[i]);
      
      //GTR Frame global pos
      sprintf(str,"GTRFrame1gpos%d",i);
      tree->Branch(str,"TVector3",&GTRFrame1gPos[i]);
      sprintf(str,"GTRFrame2gpos%d",i);
      tree->Branch(str,"TVector3",&GTRFrame2gPos[i]);
      sprintf(str,"GTRFrame3gpos%d",i);
      tree->Branch(str,"TVector3",&GTRFrame3gPos[i]);
      sprintf(str,"nGTRFrame1hit%d",i);
      tree->Branch(str,&nGTRFrame1hit[i]);
      sprintf(str,"nGTRFrame2hit%d",i);
      tree->Branch(str,&nGTRFrame2hit[i]);
      sprintf(str,"nGTRFrame3hit%d",i);
      tree->Branch(str,&nGTRFrame3hit[i]);

      sprintf(str,"Bdl%d",i);
      tree->Branch(str,&Bdl[i]);
      
      //HBD trkID
      sprintf(str,"HBDtrkid%d",i);
      tree->Branch(str,&HBDtrkid[i]);
      //HBD ID
      sprintf(str,"HBDid%d",i);
      tree->Branch(str,&HBDid[i]);
      //HBD pID
      sprintf(str,"HBDpid%d",i);
      tree->Branch(str,&HBDpid[i]);
      //HBD local pos
      sprintf(str,"HBDpos%d",i);
      tree->Branch(str,"TVector3",&HBDlPos[i]);
      //HBD vertex pos
      sprintf(str,"HBDvpos%d",i);
      tree->Branch(str,"TVector3",&HBDvPos[i]);
      //HBD global pos
      sprintf(str,"HBDgpos%d",i);
      tree->Branch(str,"TVector3",&HBDgPos[i]);
      //HBD local mom 
      sprintf(str,"HBDmom%d",i);
      tree->Branch(str,"TVector3",&HBDlMom[i]);
      //HBD Flight Length
      sprintf(str,"HBDfl%d",i);
      tree->Branch(str,&HBDfl[i]);
      
      //LGVD trkID
      sprintf(str,"LGVDtrkid%d",i);
      tree->Branch(str,&LGVDtrkid[i]);
      //LGVD ID
      sprintf(str,"LGVDid%d",i);
      tree->Branch(str,&LGVDid[i]);
      //LGVD pID
      sprintf(str,"LGVDpid%d",i);
      tree->Branch(str,&LGVDpid[i]);
      //LGVD local pos
      sprintf(str,"LGVDpos%d",i);
      tree->Branch(str,"TVector3",&LGVDlPos[i]);
      //LGVD vertex pos
      sprintf(str,"LGVDvpos%d",i);
      tree->Branch(str,"TVector3",&LGVDvPos[i]);
      //LGVD global pos
      sprintf(str,"LGVDgpos%d",i);
      tree->Branch(str,"TVector3",&LGVDgPos[i]);
      //LGVD local mom 
      sprintf(str,"LGVDmom%d",i);
      tree->Branch(str,"TVector3",&LGVDlMom[i]);
      
      //VDCyl1 global pos
      sprintf(str,"VDCyl1gpos%d",i);
      tree->Branch(str,"TVector3",&VDCyl1gPos[i]);
      sprintf(str,"VDCyl1vpos%d",i);
      tree->Branch(str,"TVector3",&VDCyl1vPos[i]);
      sprintf(str,"VDCyl1gmom%d",i);
      tree->Branch(str,"TVector3",&VDCyl1gMom[i]);
      sprintf(str,"VDCyl2gpos%d",i);
      tree->Branch(str,"TVector3",&VDCyl2gPos[i]);
      sprintf(str,"VDCyl2vpos%d",i);
      tree->Branch(str,"TVector3",&VDCyl2vPos[i]);
      sprintf(str,"VDCyl2gmom%d",i);
      tree->Branch(str,"TVector3",&VDCyl2gMom[i]);
      sprintf(str,"VDCyl3gpos%d",i);
      tree->Branch(str,"TVector3",&VDCyl3gPos[i]);
      sprintf(str,"VDCyl3vpos%d",i);
      tree->Branch(str,"TVector3",&VDCyl3vPos[i]);
      sprintf(str,"VDCyl3gmom%d",i);
      tree->Branch(str,"TVector3",&VDCyl3gMom[i]);
      sprintf(str,"VDCyl4gpos%d",i);
      tree->Branch(str,"TVector3",&VDCyl4gPos[i]);
      sprintf(str,"VDCyl4vpos%d",i);
      tree->Branch(str,"TVector3",&VDCyl4vPos[i]);
      sprintf(str,"VDCyl4gmom%d",i);
      tree->Branch(str,"TVector3",&VDCyl4gMom[i]);


      sprintf(str,"nHBDhit%d",i);
      tree->Branch(str,&nHBDhit[i]);
      sprintf(str,"nLGVDhit%d",i);
      tree->Branch(str,&nLGVDhit[i]);
      
      sprintf(str,"GTR1parentPID%d",i);
      tree->Branch(str,&GTR1parentpid[i]);
      sprintf(str,"GTR2parentPID%d",i);
      tree->Branch(str,&GTR2parentpid[i]);
      sprintf(str,"GTR3parentPID%d",i);
      tree->Branch(str,&GTR3parentpid[i]);
      sprintf(str,"HBDparentPID%d",i);
      tree->Branch(str,&HBDparentpid[i]);
      sprintf(str,"VDCyl1parentPID%d",i);
      tree->Branch(str,&VDCyl1parentpid[i]);
      sprintf(str,"VDCyl2parentPID%d",i);
      tree->Branch(str,&VDCyl2parentpid[i]);
      sprintf(str,"VDCyl3parentPID%d",i);
      tree->Branch(str,&VDCyl3parentpid[i]);
      sprintf(str,"VDCyl4parentPID%d",i);
      tree->Branch(str,&VDCyl4parentpid[i]);
      
      
      
      sprintf(str,"sMinuitStat%d",i);
      tree->Branch(str,&sMinuitStat[i]);
   }
   //Decay information
   /*tree->Branch("GTR1parentPID",&GTR1parentpid,"GTR1parentPID[2]/I");
   tree->Branch("GTR2parentPID",&GTR2parentpid,"GTR2parentPID[2]/I");
   tree->Branch("GTR3parentPID",&GTR3parentpid,"GTR3parentPID[2]/I");
   tree->Branch("HBDparentPID",&HBDparentpid,"HBDparentPID[2]/I");*/
   for (int i=0;i<nTrack;i++){
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
      sprintf(str1,"HBDprocess%d",i);
      sprintf(str2,"HBDprocess%d/C",i);
      tree->Branch(str1,&HBDprocess[i],str2);
      sprintf(str1,"GTR1vtxvolume%d",i);
      sprintf(str2,"GTR1vtxvolume%d/C",i);
      tree->Branch(str1,&GTR1vtxvolume[i],str2);
      sprintf(str1,"GTR2vtxvolume%d",i);
      sprintf(str2,"GTR2vtxvolume%d/C",i);
      tree->Branch(str1,&GTR2vtxvolume[i],str2);
      sprintf(str1,"GTR3vtxvolume%d",i);
      sprintf(str2,"GTR3vtxvolume%d/C",i);
      tree->Branch(str1,&GTR3vtxvolume[i],str2);
      sprintf(str1,"HBDvtxvolume%d",i);
      sprintf(str2,"HBDvtxvolume%d/C",i);
      tree->Branch(str1,&HBDvtxvolume[i],str2);
      sprintf(str1,"LGVDvtxvolume%d",i);
      sprintf(str2,"LGVDvtxvolume%d/C",i);
      tree->Branch(str1,&LGVDvtxvolume[i],str2);
      
      sprintf(str1,"VDCyl1process%d",i);
      sprintf(str2,"VDCyl1process%d/C",i);
      tree->Branch(str1,&VDCyl1process[i],str2);
      sprintf(str1,"VDCyl2process%d",i);
      sprintf(str2,"VDCyl2process%d/C",i);
      tree->Branch(str1,&VDCyl2process[i],str2);
      sprintf(str1,"VDCyl3process%d",i);
      sprintf(str2,"VDCyl3process%d/C",i);
      tree->Branch(str1,&VDCyl3process[i],str2);
      sprintf(str1,"VDCyl4process%d",i);
      sprintf(str2,"VDCyl4process%d/C",i);
      tree->Branch(str1,&VDCyl4process[i],str2);
      
      sprintf(str1,"VDCyl1vtxvolume%d",i);
      sprintf(str2,"VDCyl1vtxvolume%d/C",i);
      tree->Branch(str1,&VDCyl1vtxvolume[i],str2);
      sprintf(str1,"VDCyl2vtxvolume%d",i);
      sprintf(str2,"VDCyl2vtxvolume%d/C",i);
      tree->Branch(str1,&VDCyl2vtxvolume[i],str2);
      sprintf(str1,"VDCyl3vtxvolume%d",i);
      sprintf(str2,"VDCyl3vtxvolume%d/C",i);
      tree->Branch(str1,&VDCyl3vtxvolume[i],str2);
      sprintf(str1,"VDCyl4vtxvolume%d",i);
      sprintf(str2,"VDCyl4vtxvolume%d/C",i);
      tree->Branch(str1,&VDCyl4vtxvolume[i],str2);
   }
   for (int i=0;i<20;i++){
      char str1[100];
      char str2[100];
      sprintf(str1,"GTR1hitlist0_%d",i);
      sprintf(str2,"GTR1hitlist0_%d/C",i);
      tree->Branch(str1,&GTR1hitlist[0][i],str2);
      sprintf(str1,"GTR2hitlist0_%d",i);
      sprintf(str2,"GTR2hitlist0_%d/C",i);
      tree->Branch(str1,&GTR2hitlist[0][i],str2);
      sprintf(str1,"GTR3hitlist0_%d",i);
      sprintf(str2,"GTR3hitlist0_%d/C",i);
      tree->Branch(str1,&GTR3hitlist[0][i],str2);
      sprintf(str1,"HBDhitlist0_%d",i);
      sprintf(str2,"HBDhitlist0_%d/C",i);
      tree->Branch(str1,&HBDhitlist[0][i],str2);
      sprintf(str1,"GTR1hitlist1_%d",i);
      sprintf(str2,"GTR1hitlist1_%d/C",i);
      tree->Branch(str1,&GTR1hitlist[1][i],str2);
      sprintf(str1,"GTR2hitlist1_%d",i);
      sprintf(str2,"GTR2hitlist1_%d/C",i);
      tree->Branch(str1,&GTR2hitlist[1][i],str2);
      sprintf(str1,"GTR3hitlist1_%d",i);
      sprintf(str2,"GTR3hitlist1_%d/C",i);
      tree->Branch(str1,&GTR3hitlist[1][i],str2);
      sprintf(str1,"HBDhitlist1_%d",i);
      sprintf(str2,"HBDhitlist1_%d/C",i);
      tree->Branch(str1,&HBDhitlist[1][i],str2);
   }
   
cout <<"Define histogram ..."<<endl;   
   hbg=new TH1F("hbg0","bg generated",100,0.,5.);
      hrap=new TH1F("hrap0","rapidity generated",100,0.,3.);
      hpt=new TH1F("hpt0","pt generated",100,0.,3.);
      hpx[0]=new TH1F("hpx0","px generated",100,-2.,2.);
      hpy[0]=new TH1F("hpy0","py generated",100,-2.,2.);
      hpz[0]=new TH1F("hpz0","pz generated",100,-1.,3.);
      hpx[1]=new TH1F("hpx1","px generated",100,-2.,2.);
      hpy[1]=new TH1F("hpy1","py generated",100,-2.,2.);
      hpz[1]=new TH1F("hpz1","pz generated",100,-1.,3.);
      hphipx=new TH1F("hphipx","phi px generated",100,-2.,2.);
      hphipy=new TH1F("hphipy","phi py generated",100,-2.,2.);
      hphipz=new TH1F("hphipz","phi pz generated",100,-1.,5.);
      
      hgtr1x=new TH1F("hgtr1x","GTR1 local x",100,-10.,10.);
      hgtr2x=new TH1F("hgtr2x","GTR2 local x",100,-20.,20.);
      hgtr3x=new TH1F("hgtr3x","GTR3 local x",100,-30.,30.);
      hgtr1y=new TH1F("hgtr1y","GTR1 local y",100,-10.,10.);
      hgtr2y=new TH1F("hgtr2y","GTR2 local y",100,-20.,20.);
      hgtr3y=new TH1F("hgtr3y","GTR3 local y",100,-30.,30.);

      //Added by Komatsu 2015/1/26
      h_phimass_gen= new TH1F("h_phimass_gen","phi mass generated",100,0.97,1.07);
      h_phimass_trkfit= new TH1F("h_phimass_trkfit","phi mass measured",100,0.97,1.07); //20150126


}
