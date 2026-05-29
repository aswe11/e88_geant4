//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai
//2015-11-02, uploaded by yokkaich
//2015-11-02, uploaded by komatsu
//2015-03-27, uploaded by yokkaich
//2015-03-01, uploaded by yokkaich
//2015-01-20, uploaded by yokkaich
//2015-01-20, uploaded by yokkaich
//2015-01-05, modified by yokkaich
//2015-01-05, modified by yokkaich
//2014-08-27, modified by kawama
//2014-04-30, modified by kawama
//2014-04-24, modified by kawama
//2013-11-14, modified by kawama
//2013-08-16, modified by kawama
//2013-05-15, modified by kawama
//2013-05-13, modified by kawama

#include "E16ANA_G4Data.hh"
#include "TVector3.h"
#include "TRotation.h"
#include <fstream> 
#include <iostream> 
//#include <math.h> 

using namespace std;
//const double PI=3.14159265358979;
//const int MAX_LENGTH=100;

E16ANA_G4Data::E16ANA_G4Data(char* filename, int ReadorWrite,int maxTrkID)
:filename_(filename),maxTrkID_(maxTrkID)
{
   datRUN = new RUNData;
   datEV  = new EVData;
   datDC  = new decayChain[maxTrkID_];
   datTAR1 = new TARData[maxTrkID_];
   datTAR2 = new TARData[maxTrkID_];
   datTAR3 = new TARData[maxTrkID_];
   datGTR1 = new GTRData[maxTrkID_];
   datGTR2 = new GTRData[maxTrkID_];
   datGTR3 = new GTRData[maxTrkID_];
   datGTRFrame1 = new GTRFrameData[maxTrkID_];
   datGTRFrame2 = new GTRFrameData[maxTrkID_];
   datGTRFrame3 = new GTRFrameData[maxTrkID_];
   datHBD = new HBDData[maxTrkID_];
   datLG  = new LGData[maxTrkID_];
   datLGVD  = new VDData[maxTrkID_];
   datVDCyl1  = new VDCylData[maxTrkID_];
   datVDCyl2  = new VDCylData[maxTrkID_];
   datVDCyl3  = new VDCylData[maxTrkID_];
   datVDCyl4  = new VDCylData[maxTrkID_];

   //dETar = new double[nTar_];
   
   if(ReadorWrite==0){
      ifs_= new ifstream(filename_,ios::in|ios::binary);
      if (!*ifs_){
         cout <<"********* Input File open fail! **********"<<endl;
         fexists_=false;
      }
      else{
         cout <<"Read from file "<<filename_<<endl;
         fexists_=true;
         ifs_->read((char*)&ndet_, sizeof(int));
      }
   }


}

E16ANA_G4Data::~E16ANA_G4Data(){
   delete ifs_;
}

bool E16ANA_G4Data::ReadAnEvent(){

  //   char proj[10];
  //   char tar[10];
  //   float win,bmax,bmin;

   //   char processname[10]; 
   //   char vtxvolname[20];  

   /*ifs.read((char*)&size, sizeof(int));
   ifs.read(proj, size);
   
   ifs.read((char*)&size, sizeof(int));
   ifs.read(tar, size);
   
   ifs.read((char*)&win, sizeof(datRUN->Win));
   ifs.read((char*)&bmin, sizeof(datRUN->bmin));
   ifs.read((char*)&bmax, sizeof(datRUN->bmax));
   
   ifs.read((char*)&size, sizeof(int));
   ifs.read(mf, size);
   ifs.read((char*)&size, sizeof(int));
   ifs.read(pl, size);
   cout <<proj<<" "<<tar <<" "<<win<<" "<<bmin<<" "<<bmax
      <<" "<<mf<<" "<<pl<<endl;*/

   int n=0;
   while(1){
      char str1[3];
      char str2[3];

      sprintf(str2,"---");
      if(n<maxTrkID_){
         ifs_->read((char*)&datDC[n].trkID,  sizeof(int));
         ifs_->read((char*)&datDC[n].partID,  sizeof(int));
         ifs_->read((char*)&datDC[n].GEN,  sizeof(int));
         ifs_->read((char*)&datDC[n].parentID,  sizeof(int));
         ifs_->read((char*)&datDC[n].procName,  sizeof(char[10]));
         ifs_->read((char*)&datDC[n].volName_vtx,  sizeof(char[20]));
         ifs_->read((char*)&datDC[n].posx_vtx,  sizeof(double));
         ifs_->read((char*)&datDC[n].posy_vtx,  sizeof(double));
         ifs_->read((char*)&datDC[n].posz_vtx,  sizeof(double));
      //cout <<"n="<<n<<" "<<datDC[n].trkID<<" "<<
      //datDC[n].partID<<" "<<
      //datDC[n].GEN<<" "<<
      //datDC[n].parentID<<" "<<
      //datDC[n].procName<<" "<<
      //datDC[n].volName_vtx<<" "<<
      //endl;
      if(strlen(datDC[n].volName_vtx)>30)cout <<datDC[n].volName_vtx<<endl;
      }
      else{
         char str_tmp[100];
         ifs_->read(str_tmp,  sizeof(int));
         ifs_->read(str_tmp,  sizeof(int));
         ifs_->read(str_tmp,  sizeof(int));
         ifs_->read(str_tmp,  sizeof(int));
         ifs_->read(str_tmp,  sizeof(char[10]));
         ifs_->read(str_tmp,  sizeof(char[20]));
      }
      ifs_->read((char*)&str1,  3);
      //cout <<str1[0]<<" "<<str1[1]<<" "<<str1[2]<<endl;
      //cout <<str1<<"/// "<<str2<<" "<<strcmp(str1,str2) <<endl;
      n++;
      if (str1[0]==str2[0] && str1[1]==str2[1] && str1[2]==str2[2]){
         //(strcmp(str1,str2)==1) not work
         //cout <<"break decaychain part"<<endl;
         break;
      }
      else if(n>10000){
         cout <<"Too many track!!!"<<endl;
         break;
      }
      else{
         ifs_->seekg(-(sizeof(char[3])),ios::cur);
      }
   }
   datEV->nDC=n;

   ifs_->read((char*)&datEV->evID_G4,  sizeof(int));
   ifs_->read((char*)&datEV->evID_JAM, sizeof(int));
   ifs_->read((char*)&datEV->mulID_JAM,sizeof(int));
   ifs_->read((char*)&datEV->mulNum_JAM, sizeof(int));
   ifs_->read((char*)&datEV->x,        sizeof(double));
   ifs_->read((char*)&datEV->y,        sizeof(double));
   ifs_->read((char*)&datEV->z,        sizeof(double));
   ifs_->read((char*)&datEV->px,       sizeof(double));
   ifs_->read((char*)&datEV->py,       sizeof(double));
   ifs_->read((char*)&datEV->pz,       sizeof(double));
   ifs_->read((char*)&datEV->PDG,      sizeof(int));
   ifs_->read((char*)&datEV->nTAR1hit, sizeof(int));
   ifs_->read((char*)&datEV->nTAR2hit, sizeof(int));
   ifs_->read((char*)&datEV->nTAR3hit, sizeof(int));
   ifs_->read((char*)&datEV->nGTR1hit, sizeof(int));
   ifs_->read((char*)&datEV->nGTR2hit, sizeof(int));
   ifs_->read((char*)&datEV->nGTR3hit, sizeof(int));
   ifs_->read((char*)&datEV->nGTRFrame1hit, sizeof(int));
   ifs_->read((char*)&datEV->nGTRFrame2hit, sizeof(int));
   ifs_->read((char*)&datEV->nGTRFrame3hit, sizeof(int));
   ifs_->read((char*)&datEV->nHBDhit, sizeof(int));
   ifs_->read((char*)&datEV->nLGVDhit, sizeof(int));
   ifs_->read((char*)&datEV->nVDCyl1hit, sizeof(int));
   ifs_->read((char*)&datEV->nVDCyl2hit, sizeof(int));
   ifs_->read((char*)&datEV->nVDCyl3hit, sizeof(int));
   ifs_->read((char*)&datEV->nVDCyl4hit, sizeof(int));


   /*cout <<
      datEV->evID_G4<<" "<<
      datEV->evID_JAM<<" "<<
      datEV->mulID_JAM<<" "<<
      datEV->mulNum_JAM<<" "<<
      datEV->x      <<" "<<
      datEV->y      <<" "<<
      datEV->z      <<" "<<
      datEV->px     <<" "<<
      datEV->py     <<" "<<
      datEV->pz     <<" "<<
      datEV->PDG    <<" "<<
      datEV->nTAR1hit<<" "<<
      datEV->nTAR2hit<<" "<<
      datEV->nTAR3hit<<" "<<
      datEV->nGTR1hit<<" "<<
      datEV->nGTR2hit<<" "<<
      datEV->nGTR3hit<<" "<<
      datEV->nGTRFrame1hit<<" "<<
      datEV->nGTRFrame2hit<<" "<<
      datEV->nGTRFrame3hit<<" "<<
      datEV->nHBDhit<<" "<<
      datEV->nLGVDhit<<" "<<
      datEV->nVDCyl1hit<<" "<<
      datEV->nVDCyl2hit<<" "<<
      datEV->nVDCyl3hit<<" "<<
      datEV->nVDCyl4hit<<" "<<
      endl;*/
   for (int i=0;i<min(datEV->nTAR1hit,maxTrkID_);i++){
      ifs_->read((char*)&datTAR1[i].trkID, sizeof(int));
      ifs_->read((char*)&datTAR1[i].x,  sizeof(double));
      ifs_->read((char*)&datTAR1[i].y,  sizeof(double));
      ifs_->read((char*)&datTAR1[i].z,  sizeof(double));
      ifs_->read((char*)&datTAR1[i].px, sizeof(double));
      ifs_->read((char*)&datTAR1[i].py, sizeof(double));
      ifs_->read((char*)&datTAR1[i].pz, sizeof(double));
      ifs_->read((char*)&datTAR1[i].PDG,sizeof(int));
      /*cout <<"    TAR1:"<<
        datTAR1[i].trkID<<" "<<
        datTAR1[i].x<<" "<<
        datTAR1[i].y<<" "<<
        datTAR1[i].z<<" "<<
        datTAR1[i].px<<" "<<
        datTAR1[i].py<<" "<<
        datTAR1[i].pz<<" "<<
        datTAR1[i].PDG<<" "<<
        endl;*/
   }
   
   for (int i=0;i<min(datEV->nTAR2hit,maxTrkID_);i++){
      ifs_->read((char*)&datTAR2[i].trkID, sizeof(int));
      ifs_->read((char*)&datTAR2[i].x,  sizeof(double));
      ifs_->read((char*)&datTAR2[i].y,  sizeof(double));
      ifs_->read((char*)&datTAR2[i].z,  sizeof(double));
      ifs_->read((char*)&datTAR2[i].px, sizeof(double));
      ifs_->read((char*)&datTAR2[i].py, sizeof(double));
      ifs_->read((char*)&datTAR2[i].pz, sizeof(double));
      ifs_->read((char*)&datTAR2[i].PDG,sizeof(int));
      /*cout <<"    TAR2:"<<i<<":"<<
        datTAR2[i].trkID<<" "<<
        datTAR2[i].x<<" "<<
        datTAR2[i].y<<" "<<
        datTAR2[i].z<<" "<<
        datTAR2[i].px<<" "<<
        datTAR2[i].py<<" "<<
        datTAR2[i].pz<<" "<<
        datTAR2[i].PDG<<" "<<
        endl;*/
   }
   for (int i=0;i<min(datEV->nTAR3hit,maxTrkID_);i++){
      ifs_->read((char*)&datTAR3[i].trkID, sizeof(int));
      ifs_->read((char*)&datTAR3[i].x,  sizeof(double));
      ifs_->read((char*)&datTAR3[i].y,  sizeof(double));
      ifs_->read((char*)&datTAR3[i].z,  sizeof(double));
      ifs_->read((char*)&datTAR3[i].px, sizeof(double));
      ifs_->read((char*)&datTAR3[i].py, sizeof(double));
      ifs_->read((char*)&datTAR3[i].pz, sizeof(double));
      ifs_->read((char*)&datTAR3[i].PDG,sizeof(int));
   }
   for (int i=0;i<min(datEV->nGTR1hit,maxTrkID_);i++){
      ifs_->read((char*)&datGTR1[i].trkID, sizeof(int));
      ifs_->read((char*)&datGTR1[i].GTRID, sizeof(int));
      ifs_->read((char*)&datGTR1[i].x,  sizeof(double));
      ifs_->read((char*)&datGTR1[i].y,  sizeof(double));
      ifs_->read((char*)&datGTR1[i].z,  sizeof(double));
      ifs_->read((char*)&datGTR1[i].xg, sizeof(double));
      ifs_->read((char*)&datGTR1[i].yg, sizeof(double));
      ifs_->read((char*)&datGTR1[i].zg, sizeof(double));
      ifs_->read((char*)&datGTR1[i].px, sizeof(double));
      ifs_->read((char*)&datGTR1[i].py, sizeof(double));
      ifs_->read((char*)&datGTR1[i].pz, sizeof(double));
      ifs_->read((char*)&datGTR1[i].PDG,sizeof(int));
      /*cout <<"    GTR1:"<<i<<":"<<
        datGTR1[i].trkID<<" "<<
        datGTR1[i].GTRID<<" "<<
        datGTR1[i].x<<" "<<
        datGTR1[i].y<<" "<<
        datGTR1[i].z<<" "<<
        datGTR1[i].xg<<" "<<
        datGTR1[i].yg<<" "<<
        datGTR1[i].zg<<" "<<
        datGTR1[i].px<<" "<<
        datGTR1[i].py<<" "<<
        datGTR1[i].pz<<" "<<
        datGTR1[i].PDG<<" "<<
        endl;*/
   }
   for (int i=0;i<min(datEV->nGTR2hit,maxTrkID_);i++){
      ifs_->read((char*)&datGTR2[i].trkID, sizeof(int));
      ifs_->read((char*)&datGTR2[i].GTRID, sizeof(int));
      ifs_->read((char*)&datGTR2[i].x,  sizeof(double));
      ifs_->read((char*)&datGTR2[i].y,  sizeof(double));
      ifs_->read((char*)&datGTR2[i].z,  sizeof(double));
      ifs_->read((char*)&datGTR2[i].xg, sizeof(double));
      ifs_->read((char*)&datGTR2[i].yg, sizeof(double));
      ifs_->read((char*)&datGTR2[i].zg, sizeof(double));
      ifs_->read((char*)&datGTR2[i].px, sizeof(double));
      ifs_->read((char*)&datGTR2[i].py, sizeof(double));
      ifs_->read((char*)&datGTR2[i].pz, sizeof(double));
      ifs_->read((char*)&datGTR2[i].PDG, sizeof(int));
      /*cout <<"    GTR2:"<<i<<":"<<
        datGTR2[i].trkID<<" "<<
        datGTR2[i].GTRID<<" "<<
        datGTR2[i].x<<" "<<
        datGTR2[i].y<<" "<<
        datGTR2[i].z<<" "<<
        datGTR2[i].xg<<" "<<
        datGTR2[i].yg<<" "<<
        datGTR2[i].zg<<" "<<
        datGTR2[i].px<<" "<<
        datGTR2[i].py<<" "<<
        datGTR2[i].pz<<" "<<
        datGTR2[i].PDG<<" "<<
        endl;*/
   }
   for (int i=0;i<min(datEV->nGTR3hit,maxTrkID_);i++){
      ifs_->read((char*)&datGTR3[i].trkID, sizeof(int));
      ifs_->read((char*)&datGTR3[i].GTRID, sizeof(int));
      ifs_->read((char*)&datGTR3[i].x, sizeof(double));
      ifs_->read((char*)&datGTR3[i].y, sizeof(double));
      ifs_->read((char*)&datGTR3[i].z, sizeof(double));
      ifs_->read((char*)&datGTR3[i].xg, sizeof(double));
      ifs_->read((char*)&datGTR3[i].yg, sizeof(double));
      ifs_->read((char*)&datGTR3[i].zg, sizeof(double));
      ifs_->read((char*)&datGTR3[i].px, sizeof(double));
      ifs_->read((char*)&datGTR3[i].py, sizeof(double));
      ifs_->read((char*)&datGTR3[i].pz, sizeof(double));
      ifs_->read((char*)&datGTR3[i].PDG, sizeof(int));
      /*cout <<"    GTR3:"<<i<<":"<<
        datGTR3[i].trkID<<" "<<
        datGTR3[i].GTRID<<" "<<
        datGTR3[i].x<<" "<<
        datGTR3[i].y<<" "<<
        datGTR3[i].z<<" "<<
        datGTR3[i].xg<<" "<<
        datGTR3[i].yg<<" "<<
        datGTR3[i].zg<<" "<<
        datGTR3[i].px<<" "<<
        datGTR3[i].py<<" "<<
        datGTR3[i].pz<<" "<<
        datGTR3[i].PDG<<" "<<
        endl;*/
   }

   for (int i=0;i<min(datEV->nHBDhit,maxTrkID_);i++){
      ifs_->read((char*)&datHBD[i].trkID, sizeof(int));
      ifs_->read((char*)&datHBD[i].DetID, sizeof(int));
      ifs_->read((char*)&datHBD[i].x, sizeof(double));
      ifs_->read((char*)&datHBD[i].y, sizeof(double));
      ifs_->read((char*)&datHBD[i].z, sizeof(double));
      ifs_->read((char*)&datHBD[i].xg, sizeof(double));
      ifs_->read((char*)&datHBD[i].yg, sizeof(double));
      ifs_->read((char*)&datHBD[i].zg, sizeof(double));
      ifs_->read((char*)&datHBD[i].px, sizeof(double));
      ifs_->read((char*)&datHBD[i].py, sizeof(double));
      ifs_->read((char*)&datHBD[i].pz, sizeof(double));
      ifs_->read((char*)&datHBD[i].PDG, sizeof(int));
      ifs_->read((char*)&datHBD[i].beta, sizeof(double));
      ifs_->read((char*)&datHBD[i].npe, sizeof(double));
      ifs_->read((char*)&datHBD[i].fl, sizeof(double));
      /*cout <<"    HBD:"<<
        datHBD[i].trkID<<" "<<
        datHBD[i].DetID<<" "<<
        datHBD[i].x<<" "<<
        datHBD[i].y<<" "<<
        datHBD[i].z<<" "<<
        datHBD[i].xg<<" "<<
        datHBD[i].yg<<" "<<
        datHBD[i].zg<<" "<<
        datHBD[i].px<<" "<<
        datHBD[i].py<<" "<<
        datHBD[i].pz<<" "<<
        datHBD[i].PDG<<" "<<
        datHBD[i].fl<<" "<<
        endl;*/
   }

   for (int i=0;i<min(datEV->nLGVDhit,maxTrkID_);i++){
      ifs_->read((char*)&datLGVD[i].trkID, sizeof(int));
      ifs_->read((char*)&datLGVD[i].DetID, sizeof(int));
      ifs_->read((char*)&datLGVD[i].x, sizeof(double));
      ifs_->read((char*)&datLGVD[i].y, sizeof(double));
      ifs_->read((char*)&datLGVD[i].z, sizeof(double));
      ifs_->read((char*)&datLGVD[i].xg, sizeof(double));
      ifs_->read((char*)&datLGVD[i].yg, sizeof(double));
      ifs_->read((char*)&datLGVD[i].zg, sizeof(double));
      ifs_->read((char*)&datLGVD[i].px, sizeof(double));
      ifs_->read((char*)&datLGVD[i].py, sizeof(double));
      ifs_->read((char*)&datLGVD[i].pz, sizeof(double));
      ifs_->read((char*)&datLGVD[i].PDG, sizeof(int));
      /*cout <<"    LGVD:"<<i<<": "<<
        datLGVD[i].trkID<<" "<<
        datLGVD[i].DetID<<" "<<
        datLGVD[i].x<<" "<<
        datLGVD[i].y<<" "<<
        datLGVD[i].z<<" "<<
        datLGVD[i].xg<<" "<<
        datLGVD[i].yg<<" "<<
        datLGVD[i].zg<<" "<<
        datLGVD[i].px<<" "<<
        datLGVD[i].py<<" "<<
        datLGVD[i].pz<<" "<<
        endl;*/
   }
   for (int i=0;i<min(datEV->nVDCyl1hit,maxTrkID_);i++){
      ifs_->read((char*)&datVDCyl1[i].trkID, sizeof(int));
      ifs_->read((char*)&datVDCyl1[i].xg, sizeof(double));
      ifs_->read((char*)&datVDCyl1[i].yg, sizeof(double));
      ifs_->read((char*)&datVDCyl1[i].zg, sizeof(double));
      ifs_->read((char*)&datVDCyl1[i].px, sizeof(double));
      ifs_->read((char*)&datVDCyl1[i].py, sizeof(double));
      ifs_->read((char*)&datVDCyl1[i].pz, sizeof(double));
      ifs_->read((char*)&datVDCyl1[i].PDG, sizeof(int));
      /*cout <<"    VDCyl1:"<<
        datVDCyl1[i].trkID<<" "<<
        datVDCyl1[i].DetID<<" "<<
        datVDCyl1[i].xg<<" "<<
        datVDCyl1[i].yg<<" "<<
        datVDCyl1[i].zg<<" "<<
        datVDCyl1[i].px<<" "<<
        datVDCyl1[i].py<<" "<<
        datVDCyl1[i].pz<<" "<<
        endl;*/
   }
   for (int i=0;i<min(datEV->nVDCyl2hit,maxTrkID_);i++){
      ifs_->read((char*)&datVDCyl2[i].trkID, sizeof(int));
      ifs_->read((char*)&datVDCyl2[i].xg, sizeof(double));
      ifs_->read((char*)&datVDCyl2[i].yg, sizeof(double));
      ifs_->read((char*)&datVDCyl2[i].zg, sizeof(double));
      ifs_->read((char*)&datVDCyl2[i].px, sizeof(double));
      ifs_->read((char*)&datVDCyl2[i].py, sizeof(double));
      ifs_->read((char*)&datVDCyl2[i].pz, sizeof(double));
      ifs_->read((char*)&datVDCyl2[i].PDG, sizeof(int));
      /*cout <<"    VDCyl2:"<<
        datVDCyl2[i].trkID<<" "<<
        datVDCyl2[i].xg<<" "<<
        datVDCyl2[i].yg<<" "<<
        datVDCyl2[i].zg<<" "<<
        datVDCyl2[i].px<<" "<<
        datVDCyl2[i].py<<" "<<
        datVDCyl2[i].pz<<" "<<
        endl;*/
   }
   for (int i=0;i<min(datEV->nVDCyl3hit,maxTrkID_);i++){
      ifs_->read((char*)&datVDCyl3[i].trkID, sizeof(int));
      ifs_->read((char*)&datVDCyl3[i].xg, sizeof(double));
      ifs_->read((char*)&datVDCyl3[i].yg, sizeof(double));
      ifs_->read((char*)&datVDCyl3[i].zg, sizeof(double));
      ifs_->read((char*)&datVDCyl3[i].px, sizeof(double));
      ifs_->read((char*)&datVDCyl3[i].py, sizeof(double));
      ifs_->read((char*)&datVDCyl3[i].pz, sizeof(double));
      ifs_->read((char*)&datVDCyl3[i].PDG, sizeof(int));
      /*cout <<"    VDCyl3:"<<
        datVDCyl3[i].trkID<<" "<<
        datVDCyl3[i].DetID<<" "<<
        datVDCyl3[i].xg<<" "<<
        datVDCyl3[i].yg<<" "<<
        datVDCyl3[i].zg<<" "<<
        datVDCyl3[i].px<<" "<<
        datVDCyl3[i].py<<" "<<
        datVDCyl3[i].pz<<" "<<
        endl;*/
   }
   for (int i=0;i<min(datEV->nVDCyl4hit,maxTrkID_);i++){
      ifs_->read((char*)&datVDCyl4[i].trkID, sizeof(int));
      ifs_->read((char*)&datVDCyl4[i].xg, sizeof(double));
      ifs_->read((char*)&datVDCyl4[i].yg, sizeof(double));
      ifs_->read((char*)&datVDCyl4[i].zg, sizeof(double));
      ifs_->read((char*)&datVDCyl4[i].px, sizeof(double));
      ifs_->read((char*)&datVDCyl4[i].py, sizeof(double));
      ifs_->read((char*)&datVDCyl4[i].pz, sizeof(double));
      ifs_->read((char*)&datVDCyl4[i].PDG, sizeof(int));
      /*cout <<"    VDCyl4:"<<
        datVDCyl4[i].trkID<<" "<<
        datVDCyl4[i].DetID<<" "<<
        datVDCyl4[i].xg<<" "<<
        datVDCyl4[i].yg<<" "<<
        datVDCyl4[i].zg<<" "<<
        datVDCyl4[i].px<<" "<<
        datVDCyl4[i].py<<" "<<
        datVDCyl4[i].pz<<" "<<
        endl;*/
   }
   for (int i=0;i<min(datEV->nGTRFrame1hit,maxTrkID_);i++){
      ifs_->read((char*)&datGTRFrame1[i].trkID, sizeof(int));
      ifs_->read((char*)&datGTRFrame1[i].GTRID, sizeof(int));
      ifs_->read((char*)&datGTRFrame1[i].xg, sizeof(double));
      ifs_->read((char*)&datGTRFrame1[i].yg, sizeof(double));
      ifs_->read((char*)&datGTRFrame1[i].zg, sizeof(double));
      ifs_->read((char*)&datGTRFrame1[i].PDG, sizeof(int));
      /*cout <<"    GTRFrame1:"<<
        datGTR3[i].trkID<<" "<<
        datGTR3[i].GTRID<<" "<<
        datGTR3[i].xg<<" "<<
        datGTR3[i].yg<<" "<<
        datGTR3[i].zg<<" "<<
        datGTR3[i].PDG<<" "<<
        endl;*/
   }
   for (int i=0;i<min(datEV->nGTRFrame2hit,maxTrkID_);i++){
      ifs_->read((char*)&datGTRFrame2[i].trkID, sizeof(int));
      ifs_->read((char*)&datGTRFrame2[i].GTRID, sizeof(int));
      ifs_->read((char*)&datGTRFrame2[i].xg, sizeof(double));
      ifs_->read((char*)&datGTRFrame2[i].yg, sizeof(double));
      ifs_->read((char*)&datGTRFrame2[i].zg, sizeof(double));
      ifs_->read((char*)&datGTRFrame2[i].PDG, sizeof(int));
      /*cout <<"    GTR3:"<<
        datGTR3[i].trkID<<" "<<
        datGTR3[i].GTRID<<" "<<
        datGTR3[i].xg<<" "<<
        datGTR3[i].yg<<" "<<
        datGTR3[i].zg<<" "<<
        datGTR3[i].PDG<<" "<<
        endl;*/
   }
   for (int i=0;i<min(datEV->nGTRFrame3hit,maxTrkID_);i++){
      ifs_->read((char*)&datGTRFrame3[i].trkID, sizeof(int));
      ifs_->read((char*)&datGTRFrame3[i].GTRID, sizeof(int));
      ifs_->read((char*)&datGTRFrame3[i].xg, sizeof(double));
      ifs_->read((char*)&datGTRFrame3[i].yg, sizeof(double));
      ifs_->read((char*)&datGTRFrame3[i].zg, sizeof(double));
      ifs_->read((char*)&datGTRFrame3[i].PDG, sizeof(int));
      /*cout <<"    GTR3:"<<
        datGTR3[i].trkID<<" "<<
        datGTR3[i].GTRID<<" "<<
        datGTR3[i].xg<<" "<<
        datGTR3[i].yg<<" "<<
        datGTR3[i].zg<<" "<<
        datGTR3[i].PDG<<" "<<
        endl;*/
   }

   return ifs_->eof();
}


void E16ANA_G4Data::MoveToEvent(int n){
   //ifs_->seekg(0,ios_base::beg);
}

void E16ANA_G4Data::WriteRUN(){
   ofstream ofs(filename_,ios::out|ios::binary|ios::trunc);
   if (!ofs){
      cout <<"*********Output File open fail! **********"<<endl;
   }
   else{
      cout <<"Write Run info to file "<<filename_<<endl;
   }

   int size;
   size=strlen(datRUN->projectile);
   ofs.write((char*)&size,sizeof(int));
   ofs.write(datRUN->projectile, size);

   size=strlen(datRUN->target);
   ofs.write((char*)&size,sizeof(int));
   ofs.write(datRUN->target, size);

   ofs.write((char*)&datRUN->Win, sizeof(datRUN->Win));
   ofs.write((char*)&datRUN->bmin, sizeof(datRUN->bmin));
   ofs.write((char*)&datRUN->bmax, sizeof(datRUN->bmax));

   size=strlen(datRUN->magfield);
   ofs.write((char*)&size,sizeof(int));
   ofs.write(datRUN->magfield, size);

   size=strlen(datRUN->physlist);
   ofs.write((char*)&size,sizeof(int));
   ofs.write(datRUN->physlist, size);

}

void E16ANA_G4Data::FillRUN(char* projectile,char* target,
      float Win, float bmin, float bmax,
      char* magfield,char* physlist)
{
   strcpy(datRUN->projectile,projectile);
   strcpy(datRUN->target,target);
   datRUN->Win=Win;
   datRUN->bmin=bmin;
   datRUN->bmax=bmax;
   strcpy(datRUN->magfield,magfield);
   strcpy(datRUN->physlist,physlist);
}

/*void E16ANA_G4Data::WriteEV(){
  ofstream ofs(filename_,ios::app|ios::binary);
  if (!ofs){
  cout <<"********* File open fail! **********"<<endl;
  }
  else{
  cout <<"Write Event info to file "<<filename_<<endl;
  }

  ofs.write((char*)&datEV->evJAM, sizeof(datEV->evJAM));
  ofs.write((char*)&datEV->multiplicity, sizeof(datEV->multiplicity));
  ofs.write((char*)&datEV->evG4, sizeof(datEV->evG4));
  ofs.write((char*)&datEV->x, sizeof(datEV->x));
  ofs.write((char*)&datEV->y, sizeof(datEV->y));
  ofs.write((char*)&datEV->z, sizeof(datEV->z));
  ofs.write((char*)&datEV->px, sizeof(datEV->px));
  ofs.write((char*)&datEV->py, sizeof(datEV->py));
  ofs.write((char*)&datEV->pz, sizeof(datEV->pz));
  ofs.write((char*)&datEV->PDG, sizeof(datEV->PDG));
  ofs.write((char*)&datEV->nGTRhit, sizeof(datEV->nGTRhit));
  ofs.write((char*)&datEV->nHBDhit, sizeof(datEV->nHBDhit));
  ofs.write((char*)&datEV->nLGhit, sizeof(datEV->nLGhit));

  }

  void E16ANA_G4Data::FillEV(
  int evJAM,int multiplicity,int evG4,
  float x, float y, float z,
  float px,float py,float pz,
  int PDG,int nGTRhit,int nHBDhit,int nLGhit
  )
  {
  datEV->evJAM=evJAM;
  datEV->multiplicity=multiplicity;
  datEV->evG4=evG4;
  datEV->x=x;
  datEV->y=y; 
  datEV->z=z;
  datEV->px=px;
  datEV->py=py;
  datEV->pz=pz;
  datEV->PDG=PDG;
  datEV->nGTRhit=nGTRhit;
  datEV->nHBDhit=nHBDhit;
  datEV->nLGhit=nLGhit;
  }*/
