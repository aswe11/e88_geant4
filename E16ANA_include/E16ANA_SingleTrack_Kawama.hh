//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai
//2015-12-25, uploaded by komatsu
//2015-11-02, uploaded by yokkaich
//2015-11-02, uploaded by komatsu
//2015-04-08, uploaded by nakai
//2015-03-27, uploaded by yokkaich
//2015-01-05, uploaded by yokkaich
//2015-01-05, uploaded by yokkaich
//2014-08-27, uploaded by kawama
//2014-05-07, uploaded by kawama
//2014-04-30, uploaded by kawama
//2014-04-24, uploaded by kawama
//2013-11-14, uploaded by kawama
//2013-05-13, modified by kawama
//2013-05-13, modified by kawama

#ifndef E16ANA_SingleTrack_Kawama_h
#define E16ANA_SingleTrack_Kawama_h 1

#include <TVector3.h>
#include <TObject.h>
#include <TMinuit.h>
#include <TRandom.h>
#include <string> 
#include <math.h> 
#include <Math/Plane3D.h> 

#include "E16ANA_G4Data.hh"
#include "E16ANA_MagneticFieldMap.hh"
#include "E16ANA_Geometry.hh"
#include "E16ANA_Constant.hh"

const double r_ini2=15.;

class E16ANA_SingleTrack_Kawama : public TObject 
{
   private:
      E16ANA_MagneticFieldMap *fmf_;
      const E16ANA_GeometryV1 *geom_;
      const E16ANA_G4Data *g4data_;
      double GTRgPos[3][3];
      double GTRlPos[3][3];

  TVector3 fitLPos[3]; //layer positions of fit result
  TVector3 fitGPos[3]; //layer positions of fit result
  TVector3 fitLMom[3]; //momentum at each layer in fit result
  TVector3 fitGMom[3]; //momentum at each layer in fit result

      double mom0_,xp0_,yp0_,x0_,y0_,z0_,theta0_,phi0_;
      double r0_[3];
      TVector3 rvtx;
      TVector3 pvtx;
      TVector3 GTRlPos_trk[3];
      double dr0_[3];
      double fMinimumStep;
      double fMaximumStep;
      int GTRid[3];
      double ncallfmf_;
      double xMin_[3],yMin_[3],zMin_[3];
      double GTRoffsetx[3][33];
      double GTRoffsety[3][33];
      double GTRoffsetz[3][33];
      int alflagx,alflagy,alflagz;
      char OffsetFileName[100];
      int rndmSeed_;
      int MinuitStat;
      TRandom *rnd;

      void ClearOffsetVal(){
         for(int i=0; i<33; i++){
            GTRoffsetx[0][i] = 0.0;
            GTRoffsetx[1][i] = 0.0;
            GTRoffsetx[2][i] = 0.0;
            GTRoffsety[0][i] = 0.0;
            GTRoffsety[1][i] = 0.0;
            GTRoffsety[2][i] = 0.0;
            GTRoffsetz[0][i] = 0.0;
            GTRoffsetz[1][i] = 0.0;
            GTRoffsetz[2][i] = 0.0;
         }
      };

   public:

      E16ANA_SingleTrack_Kawama(const E16ANA_GeometryV1 *geom, 
            const E16ANA_G4Data* g4data);
      E16ANA_SingleTrack_Kawama(const E16ANA_GeometryV1 *geom, 
            const E16ANA_G4Data* g4data,
            int rndmSeed,
            const char* OffsetFileName);
       E16ANA_SingleTrack_Kawama(const E16ANA_GeometryV1 *geom);
       ~E16ANA_SingleTrack_Kawama();

      enum{nparam_single=5};
      double coeff[nparam_single];

      const int NParam(){return nparam_single;}
      double FitSingle();
      void Fit();
      void SmearData(int sxflag, int syflag);
      double StepRKN(double dstp,double mom, double r0[], double dr0[]);
      void StepBackRKN(double dstp, int nstp, 
            double mom, double r0[], double dr0[]);
      void EOM(double mom, double r[], double dr[], double d2r[]);
      //void rkn4(double mom, double y[3], double dydx[3], double x, double h, 
      // double yout[3], double dydxout[3]);
      void rkn4(double mom, double y[3], double dydx[3], double h, 
            double yout[3], double dydxout[3]);
      void OneGoodStep(double mom, double y[],        // InOut
            double dydx[],
            double& x,         // InOut
            double htry,
            double eps_rel_max,
            double& hdid,      // Out
            double& hnext );    // Out
      void rkn4Stepper( const double mom,
            const double yInput[],
            double dydx[],
            double hstep,
            double yOutput[],
            double yError[],
            double dydxOut[]  );

      void SetMagneticFieldMap(E16ANA_MagneticFieldMap* fmf){fmf_=fmf;}
      void SetMom0(double x[], double y[], double z[]);
      void SetMom0(TVector3 vA, TVector3 vB, TVector3 vC);
      int GetMnStat(){return MinuitStat;}
      double GetMom0(){return mom0_;}
      double GetXp0(){return xp0_;}
      double GetYp0(){return yp0_;}
      double GetX0(){return x0_;}
      double GetY0(){return y0_;}
  
  TVector3 GetFitLPos(int i){ return fitLPos[i]; }
  TVector3 GetFitGPos(int i){ return fitGPos[i]; }
  TVector3 GetFitLMom(int i){ return fitLMom[i]; }
  TVector3 GetFitGMom(int i){ return fitGMom[i]; }

      TVector3 GetGTRgPos(int id){
         return TVector3(GTRgPos[id][0],GTRgPos[id][1],GTRgPos[id][2]);
      }
      TVector3 GetGTRlPos(int id){
         return TVector3(GTRlPos[id][0],GTRlPos[id][1],GTRlPos[id][2]);
      }
      ROOT::Math::Plane3D GetPlane(int iGEM);
      /*void SetgPos(TVector3 GEMxyz[] )
      {
         for (int i=0;i<3;i++){
            GTRgPos[i][0]=GEMxyz[i].X();
            GTRgPos[i][1]=GEMxyz[i].Y();
            GTRgPos[i][2]=GEMxyz[i].Z();
         }
      }
      void SetgPos(double GEM1xyz[], double GEM2xyz[], double GEM3xyz[])
      {
         for (int i=0;i<3;i++){
            GTRgPos[0][i]=GEM1xyz[i];
            GTRgPos[1][i]=GEM2xyz[i];
            GTRgPos[2][i]=GEM3xyz[i];
         }
      }*/
      void SetlPos(double GEM1xyz[], double GEM2xyz[], double GEM3xyz[])
      {
         for (int i=0;i<3;i++){
            GTRlPos[0][i]=GEM1xyz[i];
            GTRlPos[1][i]=GEM2xyz[i];
            GTRlPos[2][i]=GEM3xyz[i];
         }
      }
      void SetlPos_trk(double GEM1xyz[], double GEM2xyz[], double GEM3xyz[] ){
         GTRlPos_trk[0].SetXYZ(GEM1xyz[0],GEM1xyz[1],GEM1xyz[2]);
         GTRlPos_trk[1].SetXYZ(GEM2xyz[0],GEM2xyz[1],GEM2xyz[2]);
         GTRlPos_trk[2].SetXYZ(GEM3xyz[0],GEM3xyz[1],GEM3xyz[2]);
      }
      void SetID(int id0, int id1, int id2)
      {
         GTRid[0]=id0;GTRid[1]=id1;GTRid[2]=id2;
      }
      void Setxyz0(double x, double y, double z)
      {
         r0_[0]=x;r0_[1]=y;r0_[2]=z;
      }
      void Getid(double id[]){
         for(int i=0;i<3;i++){
            id[i]=GTRid[i];
         }
      }
      void Getxyz0(double r[]){
         for(int i=0;i<3;i++){
            r[i]=r0_[i];
         }
      }

  void SetInitCond_Single_Kawama(TVector3 hitl1, TVector3 hitl2, TVector3 hitl3, TVector3 hitg1, TVector3 hitg2, TVector3 hitg3, TVector3 vtx_est, TVector3 mom_est, double *mom_est_abs, int *gtrid1, int *gtrid2, int *gtrid3);

      TVector3 GetXYZvtx(){return rvtx;}
      TVector3 GetPvtx(){return pvtx;}
      TVector3 GetGTRlPos_trk(int id){return GTRlPos_trk[id];}

      void SetMinuit(TMinuit* min);

      void SetOffsetVal(){
         ifstream ifs(OffsetFileName);
         if(!ifs){
            std::cerr << "*** Offset file "<<OffsetFileName
               <<" was not found! Offset values are initialized to zeros. ***"<<std::endl;
            ClearOffsetVal();
         }
         else{
            std::cout << "Offset file="<<OffsetFileName<<std::endl;
            for(int i=0;i<33;i++) {
               ifs>>GTRoffsetx[0][i]>>GTRoffsety[0][i]>>GTRoffsetz[0][i];
               ifs>>GTRoffsetx[1][i]>>GTRoffsety[1][i]>>GTRoffsetz[1][i];
               ifs>>GTRoffsetx[2][i]>>GTRoffsety[2][i]>>GTRoffsetz[2][i];
            }
         }
      }
      void SetAlignOffsetFlag(int x, int y, int z){
         alflagx=x;
         alflagy=y;
         alflagz=z;
      }
};

//void trkfcn(int &npar, double *gin, double &f, double* par, int iflag);
//extern void trkfcnSingle(int &npar, double *gin, double &f, double* par, int iflag);
#endif
