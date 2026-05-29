//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai
//2015-12-31, uploaded by yokkaich
//2015-12-21, uploaded by yokkaich
//2015-11-02, uploaded by yokkaich
//2015-11-02, uploaded by komatsu
//2015-08-03, uploaded by yokkaich
//2015-05-29, uploaded by yokkaich
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
#ifndef E16ANA_DoubleTrack_h

#define E16ANA_DoubleTrack_h 1

#include <TVector3.h>
#include <TObject.h>
#include <TMinuit.h>
#include <string> 
#include <math.h> 
#include <TGeoBBox.h>
#include <Math/Plane3D.h>

#include "E16ANA_MagneticFieldMap.hh"
#include "E16ANA_Geometry.hh"
#include "E16ANA_Constant.hh"


class E16ANA_DoubleTrack : public TObject {
private:
  E16ANA_MagneticFieldMap *fmf_;
  const E16ANA_GeometryV1 *geom_;

  double GTRgPos[6][3];    // global position of chamber hits
  double GTRlPos[6][3];    // local position of chamber hits 
  TVector3 fitLPos[6]; //position of fit result
  TVector3 fitGPos[6]; //position of fit result
  TVector3 fitLMom[6]; //momentum at each layer in fit result
  TVector3 fitGMom[6]; //momentum at each layer in fit result

  
  TVector3 initp0_vtx, initp1_vtx, initr_vtx;  //init
  TVector3 p0_vtx, p1_vtx, r_vtx;  //fit result
  double xerr[3], yerr[3]; //error of each layer[cm]

  double dr0_[3];
  double rR0_[3],rL0_[3];
  double fMinimumStep;
  double fMaximumStep;
  double StepLength_;
  double GTRStepLength[6];
  int GTRid[6];
  double ncallfmf_;
  double xMin_[3],yMin_[3],zMin_[3];

  double Bdl0,Bdl1;
  double xvtxerr_,yvtxerr_,zvtxerr_;  //for the weight of vertex data

  int minimizeStatus;
  // MIGRAD: 0:OK、 4:bad(not converged)
  int matrixStatus;   
  // MNSTAT
  //               matrix:  0= not calculated at all
  //*-*                    1= approximation only, not accurate
  //*-*                    2= full matrix, but forced positive-definite
  //*-*                    3= full accurate covariance matrix

  int crossflag[2];

  enum{nparam_double=9};
  double coeff[nparam_double];

  double StepRK(int flag, double dstp,double mom, double r0[], double dr0[]);
  void StepBackRKN(double dstp, int nstp, 
		   double mom, double r0[], double dr0[]);
  void EOM(double mom, double r[], double dr[], double d2r[]);//3 components
  void EOM(double mom, double r[], double dr[]);
  //6 components, x, y, z, px/|p|, py/|p|, pz/|p|
  void rkn4(double mom, double y[], double dydx[], double h, 
            double yout[], double dydxout[]);
  void rk4(double mom, double y[], double dydx[], double h, 
	   double yout[]);//Simple RK method,6 components
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


public:
  E16ANA_DoubleTrack(  E16ANA_MagneticFieldMap* fmf,
		     const E16ANA_GeometryV1* geom,
		     double xvtxerr, double yvtxerr, double zvtxerr);
  ~E16ANA_DoubleTrack();


  static const int NParam(){return nparam_double;}

  double StepRKN(int flag, double dstp,double mom, double r0[], double dr0[]);

  double Fit( bool vertexfixflag = false );
  // // if (flag ==1) vertex is fixed to init value.


  ROOT::Math::Plane3D GetPlane(int id);

  int MinimizeStatus(){return minimizeStatus;}
  int MatrixStatus(){return matrixStatus;}

  void Initialize(TVector3 mom0, TVector3 mom1, 
    TVector3 vtxpos, int* gtrIDs, double* errX, double* errY);

  
  void Initialize(TVector3 mom0, TVector3 mom1, TVector3 vtxpos,
    int id0, int id1, int id2, 
    int id3, int id4, int id5);//for backward compatibility



  void SetMom0(TVector3 vA, TVector3 vB, TVector3 vC);
  void SetLPos(TVector3 GTRxyz[] )  {
    for (int i=0;i<6;i++){
      GTRlPos[i][0]=GTRxyz[i].X();
      GTRlPos[i][1]=GTRxyz[i].Y();
      GTRlPos[i][2]=GTRxyz[i].Z();
    }
  }
  void SetGPos(TVector3 GTRxyz[] )  {
    for (int i=0;i<6;i++){
      GTRgPos[i][0]=GTRxyz[i].X();
      GTRgPos[i][1]=GTRxyz[i].Y();
      GTRgPos[i][2]=GTRxyz[i].Z();
    }
  }

  void SetLGPos(TVector3 GTRxyzL[], TVector3 GTRxyzG[]){
    SetLPos( GTRxyzL );
    SetGPos( GTRxyzG );
  }

  TVector3 GetLPos(int i){ return 
      TVector3( GTRlPos[i][0], GTRlPos[i][1], GTRlPos[i][2]); }

  TVector3 GetFitLPos(int i){ return fitLPos[i]; }
  TVector3 GetFitGPos(int i){ return fitGPos[i]; }
  TVector3 GetFitLMom(int i){ return fitLMom[i]; }
  TVector3 GetFitGMom(int i){ return fitGMom[i]; }

#if 0
  void SetLPos_trk0(double GEM1xyz[], double GEM2xyz[], double GEM3xyz[] ){
    GTRlPos_trk[0].SetXYZ(GEM1xyz[0],GEM1xyz[1],GEM1xyz[2]);
    GTRlPos_trk[1].SetXYZ(GEM2xyz[0],GEM2xyz[1],GEM2xyz[2]);
    GTRlPos_trk[2].SetXYZ(GEM3xyz[0],GEM3xyz[1],GEM3xyz[2]);
  }
  void SetLPos_trk1(double GEM1xyz[], double GEM2xyz[], double GEM3xyz[] ){
    GTRlPos_trk[3].SetXYZ(GEM1xyz[0],GEM1xyz[1],GEM1xyz[2]);
    GTRlPos_trk[4].SetXYZ(GEM2xyz[0],GEM2xyz[1],GEM2xyz[2]);
    GTRlPos_trk[5].SetXYZ(GEM3xyz[0],GEM3xyz[1],GEM3xyz[2]);
  }


  void Setxyz0(double x, double y, double z)
  {
    r0_[0]=x;r0_[1]=y;r0_[2]=z;
  }
  void Setxyztarget(double x, double y, double z)
  {
    rtar_[0]=x;rtar_[1]=y;rtar_[2]=z;
  }
  void Setmomtarget(double x, double y, double z)
  {
    ptar_[0]=x;ptar_[1]=y;ptar_[2]=z;
  }
#endif
  void Getid(double id[]){
    for(int i=0;i<3;i++){
      id[i]=GTRid[i];
    }
  }
  TVector3 GetInitvtx(){return initr_vtx;}
  TVector3 GetXYZvtx(){return r_vtx;}
  TVector3 GetP0vtx(){return p0_vtx;}
  TVector3 GetP1vtx(){return p1_vtx;}

  double InvMass(double m0, double m1){
    double E0 = sqrt ( m0*m0 + p0_vtx * p0_vtx );
    double E1 = sqrt ( m1*m1 + p1_vtx * p1_vtx );

    double mm = (E0+E1)*(E0+E1) - ( p1_vtx + p0_vtx )*( p1_vtx + p0_vtx );
    double m = sqrt(mm);
    return m;
  }
    
         
  double GetBdl0(){return Bdl0;}
  double GetBdl1(){return Bdl1;}
  double GetXVTXErr(){return xvtxerr_;}
  double GetYVTXErr(){return yvtxerr_;}
  double GetZVTXErr(){return zvtxerr_;}
  double GetGTRStepLength(int id){return GTRStepLength[id];}
  void SetMinuit(TMinuit* min);

  void Dump(){
    std::cerr<<"DoubleTrack dump : vtx "<<std::endl;
    r_vtx.Print();std::cerr<<std::endl;
    //    std::cerr<<"   p0 ";p0_vtx.Print(); std::cerr<<std::endl;
    //    std::cerr<<"   p1 ";p1_vtx.Print(); std::cerr<<std::endl;
    std::cerr<<"   id0 "<< GTRid[0]<<" "<< GTRid[1]<<" "<< GTRid[2]<<std::endl;
    std::cerr<<"   id1 "<< GTRid[3]<<" "<< GTRid[4]<<" "<< GTRid[5]<<std::endl;
    for( int i = 0; i<6 ; i++ ){
      std::cerr<<"   Lpos "<<i<<" "<<GTRlPos[i][0]<<" "<<GTRlPos[i][1]<<" "<<GTRlPos[i][2]<<std::endl;
    }//i
    for( int i = 0; i<6 ; i++ ){
      std::cerr<<"   Gpos "<<i<<" "<<GTRgPos[i][0]<<" "<<GTRgPos[i][1]<<" "<<GTRgPos[i][2]<<std::endl;
    }//i


  }


};

//extern void trkfcnDouble(int &npar, double *gin, double &f, double* par, int iflag);




//------------------------------------------------------


#include "E16ANA_G4OutputData.hh"

class E16ANA_TrackPairCandidateV0{
  // taking care only 6 hits : 
  //   inner(100)/middle(200)/outer(300) layer of GTRs x 2 tracks
  //
private:
  TVector3 initp1;
  TVector3 initp2;
  TVector3 initVertex;

  int moduleIDs[6];
  int pIDs[6];
  TVector3 gtrLocalPosition[6];
  TVector3 gtrGlobalPosition[6];
  E16ANA_G4Hit* g4hits[6];

public:
  E16ANA_TrackPairCandidateV0(TVector3 ip1, TVector3 ip2, TVector3 iv)
    :initp1(ip1), initp2(ip2), initVertex(iv){
  }
  TVector3& P1(){return initp1;}
  TVector3& P2(){return initp2;}
  TVector3& Vertex(){return initVertex;}

  int* ModuleIDs(){return moduleIDs;}// 0-32
  int* PIDs(){return pIDs;}
  TVector3* Xs(){return gtrLocalPosition;}
  TVector3* GXs(){return gtrGlobalPosition;}
  E16ANA_G4Hit* G4Hits(int i){return g4hits[i];}

  void SetHit(int i, E16ANA_G4Hit& hit){
    g4hits[i] = & hit;//save pointer
    moduleIDs[i] = hit.DetectorID();
    pIDs[i]   = hit.PID();
    gtrLocalPosition[i]  = hit.XTV();
    gtrGlobalPosition[i] = hit.GXTV();
  }

  void SetVertex(TVector3 iv){ initVertex = iv;}

};

#endif
