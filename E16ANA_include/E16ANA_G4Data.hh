//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai
//2015-11-02, uploaded by yokkaich
//2015-11-02, uploaded by komatsu
//2015-03-27, uploaded by yokkaich
//2015-03-01, uploaded by yokkaich
//2015-01-20, uploaded by yokkaich
//2015-01-20, uploaded by yokkaich
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

//E16ANA_G4Data.hh 150118 by S. Yokkaichi
//    Last modified at <>
//

#ifndef G4Data_h
#define G4Data_h 1

#include <fstream> 
#include <iostream> 
#include <TVector3.h>


class E16ANA_G4Data 
{
   private:
      char* filename_;
      ifstream *ifs_;
      ofstream *ofs_;
      //max track ID
      const int maxTrkID_;
      bool fexists_;
      int ndet_;

   public:
      typedef struct {
         char projectile[100];
         char target[100];
         float Win, bmin, bmax;
         char magfield[100];
         char physlist[100];
      } RUNData;
      RUNData *datRUN;

      typedef struct {
         int evID_G4;
         int evID_JAM;
         int mulID_JAM;
         int mulNum_JAM;
         double x, y, z;
         double px,py,pz;
         int PDG;
         int nTAR1hit;
         int nTAR2hit;
         int nTAR3hit;
         int nGTR1hit;
         int nGTR2hit;
         int nGTR3hit;
         int nGTRFrame1hit;
         int nGTRFrame2hit;
         int nGTRFrame3hit;
         int nHBDhit;
         int nLGVDhit;
         int nVDCyl1hit;
         int nVDCyl2hit;
         int nVDCyl3hit;
         int nVDCyl4hit;
         int nDC;
      } EVData;
      EVData *datEV;

      typedef struct {
         int trkID;
         double x, y, z;
         double px,py,pz;
         int PDG;
      } TARData;
      TARData *datTAR1;
      TARData *datTAR2;
      TARData *datTAR3;
      
      typedef struct {
         int trkID;
         int GTRID;
	double x, y, z;   //local coordinate
	double xg, yg, zg;;//global coordinate
         double px,py,pz;
	double dE;       //energy deposit
         int PDG;
      } GTRData;
      GTRData *datGTR1;
      GTRData *datGTR2;
      GTRData *datGTR3;
      
      typedef struct {
         int trkID;
         int GTRID;
         double xg, yg, zg;
         int PDG;
      } GTRFrameData;
      GTRFrameData *datGTRFrame1;
      GTRFrameData *datGTRFrame2;
      GTRFrameData *datGTRFrame3;

      typedef struct {
         int ihit;
         int trkID;
         int DetID;
         double x, y, z;
         double xg, yg, zg;
         double px,py,pz;
         double dE;
         int PDG;
	double fl;//flight length?
	double npe,beta;
      } HBDData;
      HBDData *datHBD;

      typedef struct {
         /*float x, y, z;
           float xg, yg, zg;
           float p;
           float px,py,pz;*/
         float fl,beta,edep;
         int PDG;
      } LGData;
      LGData *datLG;

      typedef struct {
         int trkID;
         int DetID;
         double x, y, z;
         double xg, yg, zg;
         double p;
         double px,py,pz;
         int PDG, proc;
      } VDData;
      VDData *datVD;
      VDData *datLGVD;
      
      typedef struct {
         int trkID;
         int DetID;
         double xg, yg, zg;
         double p;
         double px,py,pz;
         int PDG, proc;
      } VDCylData;
      VDCylData *datVDCyl1;
      VDCylData *datVDCyl2;
      VDCylData *datVDCyl3;
      VDCylData *datVDCyl4;

      typedef struct {
         int trkID;
         int partID;
         int parentID;
         int GEN;
         double posx_vtx;
         double posy_vtx;
         double posz_vtx;
         char procName[20];
         char volName[30];
         char volName_vtx[30];
      } decayChain;
      decayChain *datDC;
      
      double *dETar;
   
      E16ANA_G4Data(char* filename, int ReadorWrite,int maxTrkID);
      ~E16ANA_G4Data();
      bool FileExists(){return fexists_;}
      void Read();
      bool ReadAnEvent();
      void MoveToEvent(int n);
      void WriteRUN();
      void WriteEV();
      void WriteHBD();

      void FillRUN(char* projectile,char* target,
         float Win, float bmin, float bmax,
         char* magfield,char* physlist);
      
      void FillEV(
         int evJAM,int multiplicity,int evG4,
         float x, float y, float z,
         float px,float py,float pz,
         int PDG,int nGTRhit,int nHBDhit,int nLGhit
         );
         
      void FillGTR(
         int ihit,int trkID,int GTRID,
         float x, float y, float z,
         float px, float py, float pz,
         float dE,int PDG
         );
      
      void FillHBD(
         int ihit,int trkID,int GTRID,
         float x, float y, float z,
         float px, float py, float pz,
         float dE,int PDG
         );

};
#endif
