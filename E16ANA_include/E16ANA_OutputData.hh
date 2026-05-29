//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai
//2015-11-02, uploaded by yokkaich
//2015-11-02, uploaded by komatsu
//2015-03-27, uploaded by yokkaich
//2015-01-05, uploaded by yokkaich
//2015-01-05, uploaded by yokkaich
//2014-08-27, uploaded by kawama
//2014-05-07, uploaded by kawama
//2014-04-30, uploaded by kawama
//2014-04-24, uploaded by kawama
//2013-11-14, uploaded by kawama
//2013-05-15, uploaded by kawama
//2013-05-13, modified by kawama
//2013-05-13, modified by kawama
#ifndef OutputData_h

#define OutputData_h 1

#include <fstream> 
#include <iostream> 
//#include <TClonesArray.h>
#include <TVector3.h>
#include <TFile.h>
#include <TTree.h>
//TClonesArray should not be used!

class E16ANA_OutputData 
{
   private:
     char* filename_;
     char* readwrite_;
      int maxTrkID_;
      TFile *file;


   public:
      TTree* tree;

      E16ANA_OutputData(char* filename);
      ~E16ANA_OutputData();
      void DefineHistograms();
      void Initialize();
      void Clear();
      void Fill();
      void WritetoFile();
      TVector3 VTXpos;
      TVector3 VTXmom;
      int VTXpid;
      int evID_JAM;
      
      static const int maxTAR1hit=100;
      //TClonesArray *TAR1lPos;
      //TClonesArray *TAR1lMom;
      double TAR1lPosX[maxTAR1hit];
      double TAR1lPosY[maxTAR1hit];
      double TAR1lPosZ[maxTAR1hit];
      double TAR1lMomX[maxTAR1hit];
      double TAR1lMomY[maxTAR1hit];
      double TAR1lMomZ[maxTAR1hit];
      int TAR1trkid[maxTAR1hit];
      int TAR1pid[maxTAR1hit];
      int TAR1parentpid[maxTAR1hit];
      static const int maxTAR2hit=100;
      //TClonesArray *TAR2lPos;
      //TClonesArray *TAR2lMom;
      double TAR2lPosX[maxTAR2hit];
      double TAR2lPosY[maxTAR2hit];
      double TAR2lPosZ[maxTAR2hit];
      double TAR2lMomX[maxTAR2hit];
      double TAR2lMomY[maxTAR2hit];
      double TAR2lMomZ[maxTAR2hit];
      int TAR2trkid[maxTAR2hit];
      int TAR2pid[maxTAR2hit];
      int TAR2parentpid[maxTAR1hit];
      static const int maxTAR3hit=100;
      //TClonesArray *TAR3lPos;
      //TClonesArray *TAR3lMom;
      double TAR3lPosX[maxTAR3hit];
      double TAR3lPosY[maxTAR3hit];
      double TAR3lPosZ[maxTAR3hit];
      double TAR3lMomX[maxTAR3hit];
      double TAR3lMomY[maxTAR3hit];
      double TAR3lMomZ[maxTAR3hit];
      int TAR3trkid[maxTAR3hit];
      int TAR3pid[maxTAR3hit];
      int TAR3parentpid[maxTAR1hit];
      
      static const int maxGTR1hit=20;
      /*TClonesArray *GTR1gPos;
      TClonesArray *GTR1lPos;
      TClonesArray *GTR1gMom;
      TClonesArray *GTR1lMom;*/
      double GTR1gPosX[maxGTR1hit];
      double GTR1gPosY[maxGTR1hit];
      double GTR1gPosZ[maxGTR1hit];
      double GTR1vPosX[maxGTR1hit];
      double GTR1vPosY[maxGTR1hit];
      double GTR1vPosZ[maxGTR1hit];
      double GTR1lPosX[maxGTR1hit];
      double GTR1lPosY[maxGTR1hit];
      double GTR1lPosZ[maxGTR1hit];
      double GTR1gMomX[maxGTR1hit];
      double GTR1gMomY[maxGTR1hit];
      double GTR1gMomZ[maxGTR1hit];
      double GTR1lMomX[maxGTR1hit];
      double GTR1lMomY[maxGTR1hit];
      double GTR1lMomZ[maxGTR1hit];
      int GTR1id[maxGTR1hit];
      int GTR1trkid[maxGTR1hit];
      int GTR1pid[maxGTR1hit];
      int GTR1parentpid[maxGTR1hit];
      int GTR1originid[maxGTR1hit];
      char GTR1process[maxGTR1hit][20];
      char GTR1vtxvolume[maxGTR1hit][30];
      
      static const int maxGTR2hit=20;
      /*TClonesArray *GTR2gPos;
      TClonesArray *GTR2lPos;
      TClonesArray *GTR2gMom;
      TClonesArray *GTR2lMom;*/
      double GTR2gPosX[maxGTR2hit];
      double GTR2gPosY[maxGTR2hit];
      double GTR2gPosZ[maxGTR2hit];
      double GTR2vPosX[maxGTR2hit];
      double GTR2vPosY[maxGTR2hit];
      double GTR2vPosZ[maxGTR2hit];
      double GTR2lPosX[maxGTR2hit];
      double GTR2lPosY[maxGTR2hit];
      double GTR2lPosZ[maxGTR2hit];
      double GTR2gMomX[maxGTR2hit];
      double GTR2gMomY[maxGTR2hit];
      double GTR2gMomZ[maxGTR2hit];
      double GTR2lMomX[maxGTR2hit];
      double GTR2lMomY[maxGTR2hit];
      double GTR2lMomZ[maxGTR2hit];
      int GTR2id[maxGTR2hit];
      int GTR2trkid[maxGTR2hit];
      int GTR2pid[maxGTR2hit];
      int GTR2parentpid[maxGTR2hit];
      int GTR2originid[maxGTR2hit];
      char GTR2process[maxGTR2hit][20];
      char GTR2vtxvolume[maxGTR2hit][30];
      
      static const int maxGTR3hit=20;
      /*TClonesArray *GTR3gPos;
      TClonesArray *GTR3lPos;
      TClonesArray *GTR3gMom;
      TClonesArray *GTR3lMom;*/
      double GTR3gPosX[maxGTR3hit];
      double GTR3gPosY[maxGTR3hit];
      double GTR3gPosZ[maxGTR3hit];
      double GTR3vPosX[maxGTR3hit];
      double GTR3vPosY[maxGTR3hit];
      double GTR3vPosZ[maxGTR3hit];
      double GTR3lPosX[maxGTR3hit];
      double GTR3lPosY[maxGTR3hit];
      double GTR3lPosZ[maxGTR3hit];
      double GTR3gMomX[maxGTR3hit];
      double GTR3gMomY[maxGTR3hit];
      double GTR3gMomZ[maxGTR3hit];
      double GTR3lMomX[maxGTR3hit];
      double GTR3lMomY[maxGTR3hit];
      double GTR3lMomZ[maxGTR3hit];
      int GTR3id[maxGTR3hit];
      int GTR3trkid[maxGTR3hit];
      int GTR3pid[maxGTR3hit];
      int GTR3parentpid[maxGTR3hit];
      int GTR3originid[maxGTR3hit];
      char GTR3process[maxGTR3hit][20];
      char GTR3vtxvolume[maxGTR3hit][30];
      
      static const int maxHBDhit=20;
      /*TClonesArray *HBDgPos;
      TClonesArray *HBDlPos;
      TClonesArray *HBDgMom;
      TClonesArray *HBDlMom;*/
      double HBDgPosX[maxHBDhit];
      double HBDgPosY[maxHBDhit];
      double HBDgPosZ[maxHBDhit];
      double HBDvPosX[maxHBDhit];
      double HBDvPosY[maxHBDhit];
      double HBDvPosZ[maxHBDhit];
      double HBDlPosX[maxHBDhit];
      double HBDlPosY[maxHBDhit];
      double HBDlPosZ[maxHBDhit];
      double HBDgMomX[maxHBDhit];
      double HBDgMomY[maxHBDhit];
      double HBDgMomZ[maxHBDhit];
      double HBDlMomX[maxHBDhit];
      double HBDlMomY[maxHBDhit];
      double HBDlMomZ[maxHBDhit];
      double HBDfl[maxHBDhit];
      int HBDid[maxHBDhit];
      int HBDtrkid[maxHBDhit];
      int HBDpid[maxHBDhit];
      int HBDparentpid[maxHBDhit];
      int HBDoriginid[maxHBDhit];
      char HBDprocess[maxHBDhit][20];
      char HBDvtxvolume[maxHBDhit][30];
      
      static const int maxLGVDhit=20;
      double LGVDgPosX[maxLGVDhit];
      double LGVDgPosY[maxLGVDhit];
      double LGVDgPosZ[maxLGVDhit];
      double LGVDvPosX[maxLGVDhit];
      double LGVDvPosY[maxLGVDhit];
      double LGVDvPosZ[maxLGVDhit];
      double LGVDlPosX[maxLGVDhit];
      double LGVDlPosY[maxLGVDhit];
      double LGVDlPosZ[maxLGVDhit];
      double LGVDgMomX[maxLGVDhit];
      double LGVDgMomY[maxLGVDhit];
      double LGVDgMomZ[maxLGVDhit];
      double LGVDlMomX[maxLGVDhit];
      double LGVDlMomY[maxLGVDhit];
      double LGVDlMomZ[maxLGVDhit];
      int LGVDid[maxLGVDhit];
      int LGVDtrkid[maxLGVDhit];
      int LGVDpid[maxLGVDhit];
      int LGVDparentpid[maxLGVDhit];
      char LGVDprocess[maxLGVDhit][100];
      char LGVDvtxvolume[maxLGVDhit][100];
      
      static const int maxVDCyl1hit=20;
      double VDCyl1gPosX[maxVDCyl1hit];
      double VDCyl1gPosY[maxVDCyl1hit];
      double VDCyl1gPosZ[maxVDCyl1hit];
      double VDCyl1vPosX[maxVDCyl1hit];
      double VDCyl1vPosY[maxVDCyl1hit];
      double VDCyl1vPosZ[maxVDCyl1hit];
      double VDCyl1lPosX[maxVDCyl1hit];
      double VDCyl1lPosY[maxVDCyl1hit];
      double VDCyl1lPosZ[maxVDCyl1hit];
      double VDCyl1gMomX[maxVDCyl1hit];
      double VDCyl1gMomY[maxVDCyl1hit];
      double VDCyl1gMomZ[maxVDCyl1hit];
      double VDCyl1lMomX[maxVDCyl1hit];
      double VDCyl1lMomY[maxVDCyl1hit];
      double VDCyl1lMomZ[maxVDCyl1hit];
      int VDCyl1id[maxVDCyl1hit];
      int VDCyl1trkid[maxVDCyl1hit];
      int VDCyl1pid[maxVDCyl1hit];
      int VDCyl1parentpid[maxVDCyl1hit];
      char VDCyl1process[maxVDCyl1hit][20];
      char VDCyl1vtxvolume[maxVDCyl1hit][30];
      
      static const int maxVDCyl2hit=20;
      double VDCyl2gPosX[maxVDCyl2hit];
      double VDCyl2gPosY[maxVDCyl2hit];
      double VDCyl2gPosZ[maxVDCyl2hit];
      double VDCyl2vPosX[maxVDCyl2hit];
      double VDCyl2vPosY[maxVDCyl2hit];
      double VDCyl2vPosZ[maxVDCyl2hit];
      double VDCyl2lPosX[maxVDCyl2hit];
      double VDCyl2lPosY[maxVDCyl2hit];
      double VDCyl2lPosZ[maxVDCyl2hit];
      double VDCyl2gMomX[maxVDCyl2hit];
      double VDCyl2gMomY[maxVDCyl2hit];
      double VDCyl2gMomZ[maxVDCyl2hit];
      double VDCyl2lMomX[maxVDCyl2hit];
      double VDCyl2lMomY[maxVDCyl2hit];
      double VDCyl2lMomZ[maxVDCyl2hit];
      int VDCyl2id[maxVDCyl2hit];
      int VDCyl2trkid[maxVDCyl2hit];
      int VDCyl2pid[maxVDCyl2hit];
      int VDCyl2parentpid[maxVDCyl2hit];
      char VDCyl2process[maxVDCyl2hit][20];
      char VDCyl2vtxvolume[maxVDCyl2hit][30];
      
      static const int maxVDCyl3hit=20;
      double VDCyl3gPosX[maxVDCyl3hit];
      double VDCyl3gPosY[maxVDCyl3hit];
      double VDCyl3gPosZ[maxVDCyl3hit];
      double VDCyl3vPosX[maxVDCyl3hit];
      double VDCyl3vPosY[maxVDCyl3hit];
      double VDCyl3vPosZ[maxVDCyl3hit];
      double VDCyl3lPosX[maxVDCyl3hit];
      double VDCyl3lPosY[maxVDCyl3hit];
      double VDCyl3lPosZ[maxVDCyl3hit];
      double VDCyl3gMomX[maxVDCyl3hit];
      double VDCyl3gMomY[maxVDCyl3hit];
      double VDCyl3gMomZ[maxVDCyl3hit];
      double VDCyl3lMomX[maxVDCyl3hit];
      double VDCyl3lMomY[maxVDCyl3hit];
      double VDCyl3lMomZ[maxVDCyl3hit];
      int VDCyl3id[maxVDCyl3hit];
      int VDCyl3trkid[maxVDCyl3hit];
      int VDCyl3pid[maxVDCyl3hit];
      int VDCyl3parentpid[maxVDCyl3hit];
      char VDCyl3process[maxVDCyl3hit][20];
      char VDCyl3vtxvolume[maxVDCyl3hit][30];
      
      static const int maxVDCyl4hit=20;
      double VDCyl4gPosX[maxVDCyl4hit];
      double VDCyl4gPosY[maxVDCyl4hit];
      double VDCyl4gPosZ[maxVDCyl4hit];
      double VDCyl4vPosX[maxVDCyl4hit];
      double VDCyl4vPosY[maxVDCyl4hit];
      double VDCyl4vPosZ[maxVDCyl4hit];
      double VDCyl4lPosX[maxVDCyl4hit];
      double VDCyl4lPosY[maxVDCyl4hit];
      double VDCyl4lPosZ[maxVDCyl4hit];
      double VDCyl4gMomX[maxVDCyl4hit];
      double VDCyl4gMomY[maxVDCyl4hit];
      double VDCyl4gMomZ[maxVDCyl4hit];
      double VDCyl4lMomX[maxVDCyl4hit];
      double VDCyl4lMomY[maxVDCyl4hit];
      double VDCyl4lMomZ[maxVDCyl4hit];
      int VDCyl4id[maxVDCyl4hit];
      int VDCyl4trkid[maxVDCyl4hit];
      int VDCyl4pid[maxVDCyl4hit];
      int VDCyl4parentpid[maxVDCyl4hit];
      char VDCyl4process[maxVDCyl4hit][20];
      char VDCyl4vtxvolume[maxVDCyl4hit][30];
      
      double Bdl;
      int nTAR1hit;
      int nTAR2hit;
      int nTAR3hit;
      int nGTR1hit;
      int nGTR2hit;
      int nGTR3hit;
      int nHBDhit;
      int nLGVDhit;

};
#endif
