//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai
//2015-11-02, uploaded by yokkaich
//2015-11-02, uploaded by komatsu
//2015-03-27, uploaded by yokkaich
//2015-01-28, uploaded by komatsu
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
#ifndef DoubleTrackData_h

#define DoubleTrackData_h 1

#include <fstream> 
#include <iostream> 
#include <TVector3.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>


class E16ANA_DoubleTrackData 
{
   private:
     char* filename_;
     char* readwrite_;
      int maxTrkID_;
      TFile *file;
      TTree* tree;


   public:

      E16ANA_DoubleTrackData(char* filename,int maxTrkID, 
            char* ReadorWrite);
      ~E16ANA_DoubleTrackData();
      void DefineHistograms();
      void Fill();
      void Initialize();
      void WritetoFile();
      enum{nTrack=2};
      float chisq_fits[nTrack];
      float chisq_fitd;
      int dMinuitStat;
      int sMinuitStat[nTrack];
      TVector3 VTXpos;
      TVector3 VTXpos_cp;
      TVector3 VTXpos_fitd;
      TVector3 VTXpos_fits[nTrack];
      TVector3 VTXmom[nTrack];
      TVector3 VTXmom_fits[nTrack];
      TVector3 VTXmom_fitd[nTrack];
      TVector3 GTR1gPos_org[nTrack];
      TVector3 GTR2gPos_org[nTrack];
      TVector3 GTR3gPos_org[nTrack];
      TVector3 GTR1lPos_org[nTrack];
      TVector3 GTR2lPos_org[nTrack];
      TVector3 GTR3lPos_org[nTrack];
      TVector3 GTR1gPos[nTrack];
      TVector3 GTR2gPos[nTrack];
      TVector3 GTR3gPos[nTrack];
      TVector3 GTR1lPos[nTrack];
      TVector3 GTR2lPos[nTrack];
      TVector3 GTR3lPos[nTrack];
      TVector3 GTR1gMom[nTrack];
      TVector3 GTR2gMom[nTrack];
      TVector3 GTR3gMom[nTrack];
      TVector3 GTR1lMom[nTrack];
      TVector3 GTR2lMom[nTrack];
      TVector3 GTR3lMom[nTrack];
      TVector3 GTR1gPos_fits[nTrack];
      TVector3 GTR2gPos_fits[nTrack];
      TVector3 GTR3gPos_fits[nTrack];
      TVector3 GTR1lPos_fits[nTrack];
      TVector3 GTR2lPos_fits[nTrack];
      TVector3 GTR3lPos_fits[nTrack];
      TVector3 GTR1gMom_fits[nTrack];
      TVector3 GTR2gMom_fits[nTrack];
      TVector3 GTR3gMom_fits[nTrack];
      TVector3 GTR1lMom_fits[nTrack];
      TVector3 GTR2lMom_fits[nTrack];
      TVector3 GTR3lMom_fits[nTrack];
      TVector3 GTR1gPos_fitd[nTrack];
      TVector3 GTR2gPos_fitd[nTrack];
      TVector3 GTR3gPos_fitd[nTrack];
      TVector3 GTR1lPos_fitd[nTrack];
      TVector3 GTR2lPos_fitd[nTrack];
      TVector3 GTR3lPos_fitd[nTrack];
      TVector3 GTR1gMom_fitd[nTrack];
      TVector3 GTR2gMom_fitd[nTrack];
      TVector3 GTR3gMom_fitd[nTrack];
      TVector3 GTR1lMom_fitd[nTrack];
      TVector3 GTR2lMom_fitd[nTrack];
      TVector3 GTR3lMom_fitd[nTrack];
      TVector3 GTR1vPos[nTrack];
      TVector3 GTR2vPos[nTrack];
      TVector3 GTR3vPos[nTrack];
      TVector3 GTRFrame1gPos[nTrack];
      TVector3 GTRFrame2gPos[nTrack];
      TVector3 GTRFrame3gPos[nTrack];
      int GTRhit[27][3], GTRPDG[3]; 
      int GTRid[3][nTrack];
      int PDGID[nTrack];
      double GTR1SL[nTrack];
      double GTR2SL[nTrack];
      double GTR3SL[nTrack];
      double Bdl[nTrack];
      TVector3 gPos[3];//[GEM1,2,3][x,y,z]
      float GTRxl0[3];//[GEM1,2,3]
      float GTRyl0[3];//[GEM1,2,3]
      float GTRzl0[3];//[GEM1,2,3]
      float GTRxl1[3];//[GEM1,2,3]
      float GTRyl1[3];//[GEM1,2,3]
      float GTRzl1[3];//[GEM1,2,3]
      float GTRlxyz_s0[3][3];//[GEM1,2,3][x,y,z]
      double GTRlxyz_s1[3][3];//[GEM1,2,3][x,y,z]
      float GTRanglex0[3];
      float GTRangley0[3];
      float GTRanglex1[3];
      float GTRangley1[3];
      float d[3];
      bool fitflag0;
      bool fitflag1;
      int nHBDhit[nTrack];
      int nLGVDhit[nTrack];
      
      static const int maxGTR1hit=20;
      /*TClonesArray *GTR1gPos;
      TClonesArray *GTR1lPos;
      TClonesArray *GTR1gMom;
      TClonesArray *GTR1lMom;
      int GTR1id[maxGTR1hit];
      int GTR1trkid[maxGTR1hit];
      int GTR1pid[maxGTR1hit];*/
      int GTR1parentpid[nTrack];
      char GTR1process[nTrack][20];
      char GTR1vtxvolume[nTrack][30];
      char GTR1hitlist[nTrack][30][100];
      
      static const int maxGTR2hit=20;
      /*TClonesArray *GTR2gPos;
      TClonesArray *GTR2lPos;
      TClonesArray *GTR2gMom;
      TClonesArray *GTR2lMom;
      int GTR2id[maxGTR2hit];
      int GTR2trkid[maxGTR2hit];
      int GTR2pid[maxGTR2hit];*/
      int GTR2parentpid[nTrack];
      char GTR2process[nTrack][20];
      char GTR2vtxvolume[nTrack][30];
      char GTR2hitlist[nTrack][30][100];
      
      static const int maxGTR3hit=20;
      /*TClonesArray *GTR3gPos;
      TClonesArray *GTR3lPos;
      TClonesArray *GTR3gMom;
      TClonesArray *GTR3lMom;
      int GTR3id[maxGTR3hit];
      int GTR3trkid[maxGTR3hit];
      int GTR3pid[maxGTR3hit];*/
      int GTR3parentpid[nTrack];
      char GTR3process[nTrack][20];
      char GTR3vtxvolume[nTrack][30];
      char GTR3hitlist[nTrack][30][100];
      
      int nGTRFrame1hit[nTrack];
      int nGTRFrame2hit[nTrack];
      int nGTRFrame3hit[nTrack];
      
      static const int maxHBDhit=20;
      TVector3 HBDgPos[nTrack];
      TVector3 HBDlPos[nTrack];
      TVector3 HBDgMom[nTrack];
      TVector3 HBDlMom[nTrack];
      TVector3 HBDvPos[nTrack];
      int HBDid[nTrack];
      int HBDtrkid[nTrack];
      int HBDpid[nTrack];
      int HBDparentpid[nTrack];
      double HBDfl[nTrack];
      char HBDprocess[nTrack][20];
      char HBDvtxvolume[nTrack][30];
      char HBDhitlist[nTrack][30][100];
      
      static const int maxLGVDhit=20;
      TVector3 LGVDgPos[nTrack];
      TVector3 LGVDlPos[nTrack];
      TVector3 LGVDgMom[nTrack];
      TVector3 LGVDlMom[nTrack];
      TVector3 LGVDvPos[nTrack];
      int LGVDid[nTrack];
      int LGVDtrkid[nTrack];
      int LGVDpid[nTrack];
      int LGVDparentpid[nTrack];
      double LGVDfl[nTrack];
      char LGVDprocess[nTrack][20];
      char LGVDvtxvolume[nTrack][30];
      char LGVDhitlist[nTrack][30][100];
      
      static const int maxVDCyl1hit=20;
      TVector3 VDCyl1gPos[nTrack];
      TVector3 VDCyl1vPos[nTrack];
      TVector3 VDCyl1gMom[nTrack];
      int VDCyl1parentpid[nTrack];
      char VDCyl1process[nTrack][20];
      char VDCyl1vtxvolume[nTrack][30];
      static const int maxVDCyl2hit=20;
      TVector3 VDCyl2gPos[nTrack];
      TVector3 VDCyl2vPos[nTrack];
      TVector3 VDCyl2gMom[nTrack];
      int VDCyl2parentpid[nTrack];
      char VDCyl2process[nTrack][20];
      char VDCyl2vtxvolume[nTrack][30];
      static const int maxVDCyl3hit=20;
      TVector3 VDCyl3gPos[nTrack];
      TVector3 VDCyl3vPos[nTrack];
      TVector3 VDCyl3gMom[nTrack];
      int VDCyl3parentpid[nTrack];
      char VDCyl3process[nTrack][20];
      char VDCyl3vtxvolume[nTrack][30];
      static const int maxVDCyl4hit=20;
      TVector3 VDCyl4gPos[nTrack];
      TVector3 VDCyl4vPos[nTrack];
      TVector3 VDCyl4gMom[nTrack];
      int VDCyl4parentpid[nTrack];
      char VDCyl4process[nTrack][20];
      char VDCyl4vtxvolume[nTrack][30];

      TH1F* hbg;
      TH1F* hrap;
      TH1F* hpt;
      TH1F* hpx[nTrack];
      TH1F* hpy[nTrack];
      TH1F* hpz[nTrack];
      TH1F* hphipx;
      TH1F* hphipy;
      TH1F* hphipz;

  //Added by Komatsu 2015/1/26
  TH1F* h_phimass_gen;
  TH1F* h_phimass_trkfit;//20150126

      TH1F* hgtr1x;
      TH1F* hgtr2x;
      TH1F* hgtr3x;
      TH1F* hgtr1y;
      TH1F* hgtr2y;
      TH1F* hgtr3y;

};
#endif
