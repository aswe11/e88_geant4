//2019-10-25 sako
//2015-03-27, uploaded by yokkaich
//2015-01-05, modified by yokkaich
//2015-01-05, modified by yokkaich
//2014-08-27, modified by kawama
//2014-05-07, modified by kawama
//2014-04-30, modified by kawama
//2014-04-24, modified by kawama
//2013-11-14, modified by kawama
//2013-05-13, modified by kawama

//E16ANA_SingleFitDevelopment.cc 150104 by S. Yokkaichi
//    Last modified at <2015-01-04 21:22:45 >
// 2019-10-25
// modified to use E16ANA_SingleTrack

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TROOT.h>
#include <TSystem.h>
#include <TMinuit.h>
#include <TRandom.h>
#include <TVector3.h>
#include <Math/Minimizer.h>
#include <Math/Factory.h>
#include <Math/Functor.h>
using namespace std;

//#include "E16ANA_G4Data.hh"//sako
#include "E16ANA_G4OutputData.hh"//sako
//#include "E16ANA_Geometry.hh"//sako
#include "E16ANA_GeometryV2.hh"//sako
#include "E16ANA_Transform.hh"
//#include "E16ANA_SingleTrack_Kawama.hh"//sako
#include "E16ANA_SingleTrack.hh"//sako
#include "E16ANA_MagneticFieldMap.hh"
#include "E16ANA_ParamManager.hh"
#include "E16ANA_Constant.hh"

int main(int argc, char* argv[])
{
   char str[100], fname_in[100], fname_out[100];
   char* fname_field;
   char* fname_geom;
   int nEv=0;
   int sxflag=0, syflag=0;
   E16ANA_ParamManager *paramMgr;
   ifstream ifs;
   if (argc==2){
      ifs.open(argv[1]);
      paramMgr= new E16ANA_ParamManager("./E16CalibFiles-local.cfg");
      fname_field = paramMgr->GetMapFileName();
      fname_geom = paramMgr->GetGeomFileName();
      cout <<"Magnetic field map: "<<fname_field<<endl;
      cout <<"Geometry data file: "<<fname_geom<<endl;
      while (ifs.getline(str,sizeof(str))){
         if(sscanf(str,"Input Data: %s", fname_in)==1){
            cout <<"Input data file: "<<fname_in<<endl;
         }
         else if(sscanf(str,"Output Data: %s", fname_out)==1){
            cout <<"Output data file: "<<fname_out<<endl;
         }
         else if(sscanf(str,"Number of event: %d", &nEv)==1){
            cout <<"Number of events to be analyzed: "<<nEv<<endl;
         }
         else if(sscanf(str,"GTR Smear Flag (x,y) : %d %d", 
                  &sxflag, &syflag)==2){
	   cout <<"GTR Smear Flag (x,y) : %d, %d"<<sxflag << " " << syflag<<endl;
         }
      }
   }
   else{
      cerr << "Usage: ./E16ANA filename"<<endl;
      return 0;
   }



   E16ANA_MagneticFieldMap *fmf= new E16ANA_MagneticFieldMap3D(fname_field);
   cerr <<"field map Initializing ...";
   int ncall=fmf->Initialize_binary();
   cerr <<"..end"<<endl;

   //sako   E16ANA_GeometryV1* geom = new E16ANA_GeometryV1(fname_geom);
   E16ANA_GeometryV2* geom = new E16ANA_GeometryV2(fname_geom);

   float mom0=0.,mom1=0;
   float px0=0.,px1=0;
   float py0=0.,py1=0;
   float pz0=0.,pz1=0;
   //   float mom_fit0=0.,mom_fit1=0;
   float chi2_fit0=0.,chi2_fit1=0;
   float TARxyz0[3];
   float TARxyz1[3];
   TVector3 *TARmom_fit0=new TVector3();
   TVector3 *TARmom_fit1=new TVector3();
   TVector3 *TARxyz_fit0=new TVector3();
   TVector3 *TARxyz_fit1=new TVector3();
   float GTRp0[3];
   float GTRp1[3];
   //int GTRhit[27][3], GTRPDG[3]; 
   int GTRid0[3];
   int GTRid1[3];
   int PDG0,PDG1;
   //TVector3 gPos[3];//[GEM1,2,3][x,y,z]
   float GTRxl0[3];//[GEM1,2,3]
   float GTRyl0[3];//[GEM1,2,3]
   float GTRzl0[3];//[GEM1,2,3]
   float GTRxl1[3];//[GEM1,2,3]
   float GTRyl1[3];//[GEM1,2,3]
   float GTRzl1[3];//[GEM1,2,3]
   //float GTRlxyz_s0[3][3];//[GEM1,2,3][x,y,z]
   //   double GTRlxyz_s1[3][3];//[GEM1,2,3][x,y,z]
   //   float GTRlxyz_t[3][3];//[GEM1,2,3][x,y,z]
   float GTRxg0[3];//[GEM1,2,3]
   float GTRyg0[3];//[GEM1,2,3]
   float GTRzg0[3];//[GEM1,2,3]
   float GTRxg1[3];//[GEM1,2,3]
   float GTRyg1[3];//[GEM1,2,3]
   float GTRzg1[3];//[GEM1,2,3]
   float GTRanglex0[3];
   float GTRangley0[3];
   float GTRanglex1[3];
   float GTRangley1[3];
   //float d[3];
   bool fitflag0=false;
   bool fitflag1=false;
   
   TFile *fout = new TFile(fname_out,"recreate");
   TTree *tout= new TTree("tree","tree of E16 with tracking results");
   //Obtained momentum, chi2
   tout->Branch("VTXmomfit_0","TVector3",&TARmom_fit0);
   tout->Branch("VTXmomfit_1","TVector3",&TARmom_fit1);
   tout->Branch("chi2fit_0",&chi2_fit0,"chi2fit_0/F");
   tout->Branch("chi2fit_1",&chi2_fit1,"chi2fit_1/F");
   //Obtained target position
   tout->Branch("VTXposfit_0","TVector3",&TARxyz_fit0);
   tout->Branch("VTXposfit_1","TVector3",&TARxyz_fit1);
   //Actual momentum
   tout->Branch("px_0",&px0,"px_0/F");
   tout->Branch("py_0",&py0,"py_0/F");
   tout->Branch("pz_0",&pz0,"pz_0/F");
   tout->Branch("px_1",&px1,"px_1/F");
   tout->Branch("py_1",&py1,"py_1/F");
   tout->Branch("pz_1",&pz1,"pz_1/F");
   //Actual target position
   tout->Branch("TARx_0",&TARxyz0[0],"TARx_0/F");
   tout->Branch("TARy_0",&TARxyz0[1],"TARy_0/F");
   tout->Branch("TARz_0",&TARxyz0[2],"TARz_0/F");
   tout->Branch("TARx_1",&TARxyz1[0],"TARx_1/F");
   tout->Branch("TARy_1",&TARxyz1[1],"TARy_1/F");
   tout->Branch("TARz_1",&TARxyz1[2],"TARz_1/F");
   tout->Branch("PDG_0",&PDG0,"PDG_0/I");
   tout->Branch("PDG_1",&PDG1,"PDG_1/I");
   //GTR
   tout->Branch("GTR0id_0",&GTRid0[0],"GTR0id_0/I");
   tout->Branch("GTR1id_0",&GTRid0[1],"GTR1id_0/I");
   tout->Branch("GTR2id_0",&GTRid0[2],"GTR2id_0/I");
   tout->Branch("GTR0pl_0",&GTRp0[0], "GTR0pl_0/F");
   tout->Branch("GTR1pl_0",&GTRp0[1], "GTR1pl_0/F");
   tout->Branch("GTR2pl_0",&GTRp0[2], "GTR2pl_0/F");
   tout->Branch("GTR0xl_0",&GTRxl0[0],"GTR0xl_0/F");
   tout->Branch("GTR0yl_0",&GTRyl0[0],"GTR0yl_0/F");
   tout->Branch("GTR0zl_0",&GTRzl0[0],"GTR0zl_0/F");
   tout->Branch("GTR1xl_0",&GTRxl0[1],"GTR1xl_0/F");
   tout->Branch("GTR1yl_0",&GTRyl0[1],"GTR1yl_0/F");
   tout->Branch("GTR1zl_0",&GTRzl0[1],"GTR1zl_0/F");
   tout->Branch("GTR2xl_0",&GTRxl0[2],"GTR2xl_0/F");
   tout->Branch("GTR2yl_0",&GTRyl0[2],"GTR2yl_0/F");
   tout->Branch("GTR2zl_0",&GTRzl0[2],"GTR2zl_0/F");
   tout->Branch("GTR0anglex_0",&GTRanglex0[0],"GTR0anglex_0/F");
   tout->Branch("GTR1anglex_0",&GTRanglex0[1],"GTR1anglex_0/F");
   tout->Branch("GTR2anglex_0",&GTRanglex0[2],"GTR2anglex_0/F");
   tout->Branch("GTR0angley_0",&GTRangley0[0],"GTR0angley_0/F");
   tout->Branch("GTR1angley_0",&GTRangley0[1],"GTR1angley_0/F");
   tout->Branch("GTR2angley_0",&GTRangley0[2],"GTR2angley_0/F");
   tout->Branch("GTR0xg_0",&GTRxg0[0],"GEM0xg_0/F");
   tout->Branch("GTR0yg_0",&GTRyg0[0],"GEM0yg_0/F");
   tout->Branch("GTR0zg_0",&GTRzg0[0],"GEM0zg_0/F");
   tout->Branch("GTR1xg_0",&GTRxg0[1],"GEM1xg_0/F");
   tout->Branch("GTR1yg_0",&GTRyg0[1],"GEM1yg_0/F");
   tout->Branch("GTR1zg_0",&GTRzg0[1],"GEM1zg_0/F");
   tout->Branch("GTR2xg_0",&GTRxg0[2],"GEM2xg_0/F");
   tout->Branch("GTR2yg_0",&GTRyg0[2],"GEM2yg_0/F");
   tout->Branch("GTR2zg_0",&GTRzg0[2],"GEM2zg_0/F");
   
   tout->Branch("GTR0id_1",&GTRid1[0],"GTR0id_1/I");
   tout->Branch("GTR1id_1",&GTRid1[1],"GTR1id_1/I");
   tout->Branch("GTR2id_1",&GTRid1[2],"GTR2id_1/I");
   tout->Branch("GTR0pl_1",&GTRp1[0], "GTR0pl_1/F");
   tout->Branch("GTR1pl_1",&GTRp1[1], "GTR1pl_1/F");
   tout->Branch("GTR2pl_1",&GTRp1[2], "GTR2pl_1/F");
   tout->Branch("GTR0xl_1",&GTRxl1[0],"GTR0xl_1/F");
   tout->Branch("GTR0yl_1",&GTRyl1[0],"GTR0yl_1/F");
   tout->Branch("GTR0zl_1",&GTRzl1[0],"GTR0zl_1/F");
   tout->Branch("GTR1xl_1",&GTRxl1[1],"GTR1xl_1/F");
   tout->Branch("GTR1yl_1",&GTRyl1[1],"GTR1yl_1/F");
   tout->Branch("GTR1zl_1",&GTRzl1[1],"GTR1zl_1/F");
   tout->Branch("GTR2xl_1",&GTRxl1[2],"GTR2xl_1/F");
   tout->Branch("GTR2yl_1",&GTRyl1[2],"GTR2yl_1/F");
   tout->Branch("GTR2zl_1",&GTRzl1[2],"GTR2zl_1/F");
   tout->Branch("GTR0anglex_1",&GTRanglex1[0],"GTR0anglex_1/F");
   tout->Branch("GTR1anglex_1",&GTRanglex1[1],"GTR1anglex_1/F");
   tout->Branch("GTR2anglex_1",&GTRanglex1[2],"GTR2anglex_1/F");
   tout->Branch("GTR0angley_1",&GTRangley1[0],"GTR0angley_1/F");
   tout->Branch("GTR1angley_1",&GTRangley1[1],"GTR1angley_1/F");
   tout->Branch("GTR2angley_1",&GTRangley1[2],"GTR2angley_1/F");
   tout->Branch("GTR0xg_1",&GTRxg1[0],"GEM0xg_1/F");
   tout->Branch("GTR0yg_1",&GTRyg1[0],"GEM0yg_1/F");
   tout->Branch("GTR0zg_1",&GTRzg1[0],"GEM0zg_1/F");
   tout->Branch("GTR1xg_1",&GTRxg1[1],"GEM1xg_1/F");
   tout->Branch("GTR1yg_1",&GTRyg1[1],"GEM1yg_1/F");
   tout->Branch("GTR1zg_1",&GTRzg1[1],"GEM1zg_1/F");
   tout->Branch("GTR2xg_1",&GTRxg1[2],"GEM2xg_1/F");
   tout->Branch("GTR2yg_1",&GTRyg1[2],"GEM2yg_1/F");
   tout->Branch("GTR2zg_1",&GTRzg1[2],"GEM2zg_1/F");
   //   E16ANA_G4Data *g4data=new E16ANA_G4Data(fname_in,0,100);
   E16ANA_G4OutputData *g4data=new E16ANA_G4OutputData();
   int flag = g4data->OpenReadFile(g4filename);//core dump
   if( flag != g4data->OK ){     exit(1);   }

   E16ANA_SingleTrack_Kawama* strk= new E16ANA_SingleTrack_Kawama(geom,g4data);
   gMinuit->SetObjectFit(strk);
   strk->SetMagneticFieldMap(fmf);
   for (int iev=0;iev<nEv;iev++){
      cout << "--------Event number in JAM"<<iev+1<<"/"<<nEv<<"----------"<<endl; 
      fitflag0=false;
      fitflag1=false;

      //      g4data->ReadAnEvent();
      int read_event = g4data->ReadAnEvent();
      if(read_event != 1){
         std::cout << "End of file" << std::endl;
         break;
      }

      //sako      if(g4data->datEV->evID_JAM<iev)break;
      if(g4data->Event().JAMEventID()<iev)break;
      //I want to loop over track (sako) it was 1 track before
      int noht = g4event.PrimaryTracks().Noh();//sako
      std::cout << "track_noh = " << noht << std::endl;

      PDG0=g4data->datEV->PDG;
      px0=g4data->datEV->px;
      py0=g4data->datEV->py;
      pz0=g4data->datEV->pz;
      mom0=sqrt(px0*px0+py0*py0+pz0*pz0);
      TARxyz0[0]=g4data->datEV->x;
      TARxyz0[1]=g4data->datEV->y;
      TARxyz0[2]=g4data->datEV->z;
      GTRxl0[0]=g4data->datGTR1[0].x;
      GTRyl0[0]=g4data->datGTR1[0].y;
      GTRzl0[0]=g4data->datGTR1[0].z;
      GTRxl0[1]=g4data->datGTR2[0].x;
      GTRyl0[1]=g4data->datGTR2[0].y;
      GTRzl0[1]=g4data->datGTR2[0].z;
      GTRxl0[2]=g4data->datGTR3[0].x;
      GTRyl0[2]=g4data->datGTR3[0].y;
      GTRzl0[2]=g4data->datGTR3[0].z;
      GTRxg0[0]=g4data->datGTR1[0].xg;
      GTRyg0[0]=g4data->datGTR1[0].yg;
      GTRzg0[0]=g4data->datGTR1[0].zg;
      GTRxg0[1]=g4data->datGTR2[0].xg;
      GTRyg0[1]=g4data->datGTR2[0].yg;
      GTRzg0[1]=g4data->datGTR2[0].zg;
      GTRxg0[2]=g4data->datGTR3[0].xg;
      GTRyg0[2]=g4data->datGTR3[0].yg;
      GTRzg0[2]=g4data->datGTR3[0].zg;
      GTRid0[0]=g4data->datGTR1[0].GTRID;
      GTRid0[1]=g4data->datGTR2[0].GTRID;
      GTRid0[2]=g4data->datGTR3[0].GTRID;
      double pxGTR1=g4data->datGTR1[0].px;
      double pyGTR1=g4data->datGTR1[0].py;
      double pzGTR1=g4data->datGTR1[0].pz;
      double pxGTR2=g4data->datGTR2[0].px;
      double pyGTR2=g4data->datGTR2[0].py;
      double pzGTR2=g4data->datGTR2[0].pz;
      double pxGTR3=g4data->datGTR3[0].px;
      double pyGTR3=g4data->datGTR3[0].py;
      double pzGTR3=g4data->datGTR3[0].pz;
      GTRp0[0]=sqrt(pxGTR1*pxGTR1+pyGTR1*pyGTR1+pzGTR1*pzGTR1);
      GTRp0[1]=sqrt(pxGTR2*pxGTR2+pyGTR2*pyGTR2+pzGTR2*pzGTR2);
      GTRp0[2]=sqrt(pxGTR3*pxGTR3+pyGTR3*pyGTR3+pzGTR3*pzGTR3);
      GTRanglex0[0]=atan(pxGTR1/pzGTR1);
      GTRangley0[0]=atan(pyGTR1/pzGTR1);
      GTRanglex0[1]=atan(pxGTR2/pzGTR2);
      GTRangley0[1]=atan(pyGTR2/pzGTR2);
      GTRanglex0[2]=atan(pxGTR3/pzGTR3);
      GTRangley0[2]=atan(pyGTR3/pzGTR3);

      //cout<<"GTRID"<<":"<<GTRid0[0]<<" "
       //<<GTRid0[1]<<" "
       //<<GTRid0[2]<<endl;
      if(fabs(mom0-GTRp0[0])<1e-3 &&
            fabs(GTRp0[0]-GTRp0[1])<1e-3 &&
            fabs(GTRp0[1]-GTRp0[2])<1e-3 &&
            fabs(GTRid0[0]-GTRid0[1])<7 && 
            fabs(GTRid0[1]-GTRid0[2])<7){
            strk->SmearData(sxflag,syflag);
         float mom_ini=strk->GetMom0();
         if(fabs(mom_ini)>0 && fabs(mom_ini)<10){
            //cout << "Do Fit "<<endl; 
            chi2_fit0=strk->FitSingle();
            TARxyz_fit0=&strk->GetXYZvtx();
            TARmom_fit0=&strk->GetPvtx();
            //strk->GetGTRlPos_trk(GTRlxyz_t[0],GTRlxyz_t[1],GTRlxyz_t[2]);
            cout <<"Target pos: x="<< TARxyz0[0]
               <<" y="<<TARxyz0[1]<<endl;
            cout <<"Obtained pos: x="<< TARxyz_fit0->X()
               <<" y="<< TARxyz_fit0->Y()<<endl;
            //cout<<" particle GEN:"<<PDG0<< "GEM1:"<<GTRPDG[0]
            //<< "GEM2:"<<GTRPDG[1]<< "GEM3:"<<GTRPDG[2] <<endl;
            cout<<" p="<<mom_ini<<" x'="<<strk->GetXp0()<<" y'="<<strk->GetYp0() <<endl;
            cout <<" mom="<< mom0<<" "<<GTRp0[0]<<" "<<GTRp0[1]
               <<" "<<GTRp0[2]<<endl;
            cout <<" mom_fit0="<<TARmom_fit0->Mag()<<endl;
            cout <<" chi2_fit0="<<chi2_fit0<<endl;
            fitflag0=true;
         }
         else{
            cout << "Momentum initialization error"<<endl;
         }

      }
      //}
      for (int i=1;i<g4data->datEV->mulNum_JAM;i++){
         g4data->ReadAnEvent();
         /*cerr<<"GTR hit"<<g4data->datEV->nGTR1hit<<" "<<
           g4data->datEV->nGTR2hit<<" "<<
           g4data->datEV->nGTR3hit<<endl;*/
         //if (g4data->datEV->nGTR1hit==1 && 
         //    g4data->datEV->nGTR2hit==1 && 
         //  g4data->datEV->nGTR3hit==1) {//only single hit events
         PDG1=g4data->datEV->PDG;
         //cout << "PDG1"<<PDG1<<endl;
         //cout << "JAM event ID"<<g4data->datEV->evID_JAM<<endl;
         px1=g4data->datEV->px;
         py1=g4data->datEV->py;
         pz1=g4data->datEV->pz;
         mom1=sqrt(px1*px1+py1*py1+pz1*pz1);
         GTRxl1[0]=g4data->datGTR1[0].x;
         GTRyl1[0]=g4data->datGTR1[0].y;
         GTRzl1[0]=g4data->datGTR1[0].z;
         GTRxl1[1]=g4data->datGTR2[0].x;
         GTRyl1[1]=g4data->datGTR2[0].y;
         GTRzl1[1]=g4data->datGTR2[0].z;
         GTRxl1[2]=g4data->datGTR3[0].x;
         GTRyl1[2]=g4data->datGTR3[0].y;
         GTRzl1[2]=g4data->datGTR3[0].z;
         GTRxg1[0]=g4data->datGTR1[0].xg;
         GTRyg1[0]=g4data->datGTR1[0].yg;
         GTRzg1[0]=g4data->datGTR1[0].zg;
         GTRxg1[1]=g4data->datGTR2[0].xg;
         GTRyg1[1]=g4data->datGTR2[0].yg;
         GTRzg1[1]=g4data->datGTR2[0].zg;
         GTRxg1[2]=g4data->datGTR3[0].xg;
         GTRyg1[2]=g4data->datGTR3[0].yg;
         GTRzg1[2]=g4data->datGTR3[0].zg;
         GTRid1[0]=g4data->datGTR1[0].GTRID;
         GTRid1[1]=g4data->datGTR2[0].GTRID;
         GTRid1[2]=g4data->datGTR3[0].GTRID;
         double pxGTR1=g4data->datGTR1[0].px;
         double pyGTR1=g4data->datGTR1[0].py;
         double pzGTR1=g4data->datGTR1[0].pz;
         double pxGTR2=g4data->datGTR2[0].px;
         double pyGTR2=g4data->datGTR2[0].py;
         double pzGTR2=g4data->datGTR2[0].pz;
         double pxGTR3=g4data->datGTR3[0].px;
         double pyGTR3=g4data->datGTR3[0].py;
         double pzGTR3=g4data->datGTR3[0].pz;
         GTRp1[0]=sqrt(pxGTR1*pxGTR1+pyGTR1*pyGTR1+pzGTR1*pzGTR1);
         GTRp1[1]=sqrt(pxGTR2*pxGTR2+pyGTR2*pyGTR2+pzGTR2*pzGTR2);
         GTRp1[2]=sqrt(pxGTR3*pxGTR3+pyGTR3*pyGTR3+pzGTR3*pzGTR3);
         GTRanglex1[0]=atan(pxGTR1/pzGTR1);
         GTRangley1[0]=atan(pyGTR1/pzGTR1);
         GTRanglex1[1]=atan(pxGTR2/pzGTR2);
         GTRangley1[1]=atan(pyGTR2/pzGTR2);
         GTRanglex1[2]=atan(pxGTR3/pzGTR3);
         GTRangley1[2]=atan(pyGTR3/pzGTR3);

         //cerr<<"GTRID"<<":"<<GTRid1[0]<<" "
         // <<GTRid1[1]<<" "
         // <<GTRid1[2]<<endl;
         if(fabs(mom1-GTRp1[0])<1e-3 &&
               fabs(GTRp1[0]-GTRp1[1])<1e-3 &&
               fabs(GTRp1[1]-GTRp1[2])<1e-3 &&
               fabs(GTRid1[0]-GTRid1[1])<7 && 
               fabs(GTRid1[1]-GTRid1[2])<7){
            strk->SmearData(sxflag,syflag);
            float mom_ini=strk->GetMom0();
            if(fabs(mom_ini)>0 && fabs(mom_ini)<10){
               //cout << "Do Fit "<<endl; 
               chi2_fit1=strk->FitSingle();
               TARxyz_fit1=&strk->GetXYZvtx();
               TARmom_fit1=&-strk->GetPvtx();
               //strk->GetGTRlPos_trk(GTRlxyz_t[0],GTRlxyz_t[1],GTRlxyz_t[2]);
               cout <<"Target pos: x="<< TARxyz1[0]
                  <<" y="<<TARxyz1[1]<<endl;
               cout <<"Obtained pos: x="<< TARxyz_fit1->X()
                  <<" y="<< TARxyz_fit1->Y()<<endl;
               //cout<<" particle GEN:"<<PDG0<< "GEM1:"<<GTRPDG[0]
               //<< "GEM2:"<<GTRPDG[1]<< "GEM3:"<<GTRPDG[2] <<endl;
               cout<<" p="<<mom_ini<<" x'="<<strk->GetXp0()<<" y'="<<strk->GetYp0() <<endl;
               cout <<" mom="<< mom1<<" "<<GTRp1[0]<<" "<<GTRp1[1]
                  <<" "<<GTRp1[2]<<endl;
               cout <<" mom_fit1="<<TARmom_fit1->Mag()<<endl;
               cout <<" chi2_fit1="<<chi2_fit1<<endl;
               fitflag1=true;
            }
            else{
               cout << "Momentum initialization error"<<endl;
            }
         }
      }
      //}
      if(fitflag0 && fitflag1)tout->Fill();
   }
   delete strk;
   tout->Write();
   fout->Close();
   //end=clock(); 
   //time_t time_ite=(end-start)/1e6;
   //cout << "Initialize Time[sec]:"<<time_ini<<", number of call="<<ncall<<endl;
   //cout << "Iteration Time[sec]:"<<time_ite<<endl;


   return 0;
}
