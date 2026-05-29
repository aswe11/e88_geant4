//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai
//2015-12-25, uploaded by komatsu
//2015-11-02, uploaded by yokkaich
//2015-11-02, uploaded by komatsu
//2015-10-05, uploaded by yokkaich
//2015-08-10, uploaded by nakai
//2015-07-15, uploaded by nakai
//2015-04-08, uploaded by nakai
//2015-03-27, uploaded by yokkaich
//2015-03-01, uploaded by yokkaich
//2015-01-05, modified by yokkaich
//2015-01-05, modified by yokkaich
//2014-08-27, modified by kawama
//2014-05-07, modified by kawama
//2014-04-30, modified by kawama
//2014-04-24, modified by kawama
//2013-11-14, modified by kawama
//2013-05-13, modified by kawama

#include <TMinuit.h>
#include <TVector3.h>
#include <TRandom.h>
#include <G4ThreeVector.hh>
#include <Math/Minimizer.h>
#include <Math/Factory.h>
#include <Math/Functor.h>
#include <Math/Plane3D.h>
#include <Math/Rotation3D.h>
#include <Math/RotationX.h>
#include <Math/RotationY.h>
#include <Math/RotationZ.h>
#include <math.h>
using namespace std;

#include "E16ANA_Transform.hh"
#include "E16ANA_SingleTrack_Kawama.hh"
   
TMinuit *sMinuitK= new TMinuit(E16ANA_SingleTrack_Kawama::nparam_single);

void trkfcnSingleKawama(int &npar, double *gin, double &f, double* par, int iflag){
   double r0[3];
   double mom=par[0];
   double xp=par[1];
   double yp=par[2];
   double theta=par[3];
   double phi=par[4];
   r0[0]=r_ini2*cos(phi)*sin(theta);
   r0[1]=r_ini2*sin(phi);
   r0[2]=r_ini2*cos(phi)*cos(theta);
   double dr0[3];
   double stp=1e-1;
   //cout << "mom="<<mom<<",xp="<<xp<<",yp="<<yp<<endl;
   //cout << "theta="<<theta<<",phi="<<phi<<endl;
   if (fabs(theta)<M_PI/2.)dr0[2]=1/sqrt(xp*xp+yp*yp+1);
   else dr0[2]=-1/sqrt(xp*xp+yp*yp+1);
   dr0[0]=xp*dr0[2];
   dr0[1]=yp*dr0[2];
   //cout << "x="<<r0[0]<<", y="<<r0[1]<<", z="<<r0[2]<<endl;
   //cout << "dx="<<dr0[0]<<", dy="<<dr0[1]<<", dz="<<dr0[2]<<endl;
   E16ANA_SingleTrack_Kawama *trk=(E16ANA_SingleTrack_Kawama*)sMinuitK->GetObjectFit();
   //trk->Getxyz0(r0);
   double dmin=trk->StepRKN(stp,mom,r0,dr0);
   //cout << "dmin="<<dmin<<endl;
   f=dmin;
   //cout << "Start Stepper"<<endl;
}

E16ANA_SingleTrack_Kawama::E16ANA_SingleTrack_Kawama
(const E16ANA_GeometryV1 *geom, const E16ANA_G4Data* g4data)
   :geom_(geom),g4data_(g4data){
   ncallfmf_=0;
   ClearOffsetVal();
}

E16ANA_SingleTrack_Kawama::E16ANA_SingleTrack_Kawama
(const E16ANA_GeometryV1 *geom, const E16ANA_G4Data* g4data, int rndmSeed, 
 const char* offsetfilename)
:geom_(geom),g4data_(g4data),rndmSeed_(rndmSeed){
   sprintf(OffsetFileName,offsetfilename);
   ncallfmf_=0;
   rnd= new TRandom(rndmSeed_);
   //gRandom->SetSeed(rndmSeed_); 
   ClearOffsetVal();
}
E16ANA_SingleTrack_Kawama::E16ANA_SingleTrack_Kawama
(const E16ANA_GeometryV1 *geom)
  :geom_(geom){
}

E16ANA_SingleTrack_Kawama::~E16ANA_SingleTrack_Kawama(){
   //delete rnd;
}
void E16ANA_SingleTrack_Kawama::SetInitCond_Single_Kawama(TVector3 hitl1, TVector3 hitl2, TVector3 hitl3, TVector3 hitg1, TVector3 hitg2, TVector3 hitg3, TVector3 vtx_est, TVector3 mom_est, double *mom_est_abs, int *gtrid1, int *gtrid2, int *gtrid3){
  
  double GEM1xyz[3], GEM2xyz[3], GEM3xyz[3];
  GEM1xyz[0]=hitl1.X();
  GEM1xyz[1]=hitl1.Y();
  GEM1xyz[2]=hitl1.Z();
  GEM2xyz[0]=hitl2.X();
  GEM2xyz[1]=hitl2.Y();
  GEM2xyz[2]=hitl2.Z();
  GEM3xyz[0]=hitl3.X();
  GEM3xyz[1]=hitl3.Y();
  GEM3xyz[2]=hitl3.Z();

  SetlPos(GEM1xyz, GEM2xyz, GEM3xyz);
  
  GTRgPos[0][0]=hitg1.X();
  GTRgPos[0][1]=hitg1.Y();
  GTRgPos[0][2]=hitg1.Z();
  GTRgPos[1][0]=hitg2.X();
  GTRgPos[1][1]=hitg2.Y();
  GTRgPos[1][2]=hitg2.Z();
  GTRgPos[2][0]=hitg3.X();
  GTRgPos[2][1]=hitg3.Y();
  GTRgPos[2][2]=hitg3.Z();

  SetID(*gtrid1, *gtrid2, *gtrid3);
  
  mom0_= -1**mom_est_abs;
  xp0_=hitg1.X()/hitg1.Z();
  
  if (hitg1.Z()>0)theta0_=atan(hitg1.X()/hitg1.Z());
  else if(hitg1.X()>0)theta0_=M_PI-atan(-hitg1.X()/hitg1.Z());
  else theta0_=-M_PI-atan(-hitg1.X()/hitg1.Z());
  
  phi0_=atan(hitg1.Y()/sqrt(hitg1.X()*hitg1.X()+hitg1.Z()*hitg1.Z()));
  
  //Added by Komatsu 2015/10/30
  //if (mom_est.Z()>0)theta0_=atan(mom_est.X()/mom_est.Z());
  //else if(mom_est.X()>0)theta0_=M_PI-atan(-mom_est.X()/mom_est.Z());
  //else theta0_=-M_PI-atan(-mom_est.X()/mom_est.Z());
  //phi0_=atan(mom_est.Y()/sqrt(mom_est.X()*mom_est.X()+mom_est.Z()*mom_est.Z()));

  //yp0_=(y[2]-y[1])/(z[2]-z[1]);
  yp0_=hitg1.Y()/hitg1.Z();
  //yp0_=hitg1C.Y()/hitg1C.Z();
  
  cerr<<"mom0 "<<mom0_<<"\t"<<" xp0 "<<xp0_<<" yp0 "<<yp0_<<" theta0 "<<theta0_<<" phi0 "<<phi0_<<" Single Track L129 "<<endl;
  cerr<<"GTRid "<<GTRid[0]<<"\t"<<GTRid[1]<<"\t"<<GTRid[2]<<endl;
}
//added by Komatsu end---------------------------------------

double E16ANA_SingleTrack_Kawama::FitSingle()
{
  //double ds=0.;
   double arglist[10],chi2=1000.;
   int ierflg=0;
   cerr<<"SingleTrackKawama L147 "<<endl;
   sMinuitK->SetFCN(trkfcnSingleKawama);
   arglist[0]=1;
   sMinuitK->mnexcm("SET ERR", arglist, 1, ierflg);
   sMinuitK->SetPrintLevel(-1);
   arglist[0]=0;
   cerr<<"SingleTrackKawama L153 "<<endl;
   sMinuitK->mnexcm("SET STRA 2", arglist, 0, ierflg);
   cerr<<"SingleTrackKawama L155 "<<endl;
   int ite=0;
   //while(chi2>1 && ite<10){
   while(ite<1){
      //cout << "Iteration"<<ite<<" Start"<<endl;
      if (ite==0){
         if(fabs(mom0_)>0.2 && fabs(mom0_)<5)coeff[0]=(double)mom0_;
         else coeff[0]=(double)mom0_/fabs(mom0_);
         coeff[1]=(double)xp0_;
         coeff[2]=(double)yp0_;
         coeff[3]=(double)theta0_;
         coeff[4]=(double)phi0_;
         //cout <<"Initial:"<<mom0_<<" "<<xp0_<<" "<<yp0_<<" "<<theta0_<<" "<<phi0_<<endl;
      }
      cerr<<"SingleTrackKawama L169 "<<endl;
      double Step[nparam_single];
      double LLim[nparam_single];
      double ULim[nparam_single];
      for (int i=0;i<nparam_single;i++){
         //Step[i]=fabs(coeff[i])/2.;
         //LLim[i]=coeff[i]-fabs(coeff[i])/2.;
         //ULim[i]=coeff[i]+fabs(coeff[i])/2.;
         Step[i]=fabs(coeff[i]);
         LLim[i]=coeff[i]-fabs(coeff[i]);
         ULim[i]=coeff[i]+fabs(coeff[i]);
      }
      cerr<<"SingleTrackKawama L181 "<<endl;
      //if (mom0_>0 && LLim[0]<0.25)LLim[0]=0.25;
      //else if (mom0_<0 && ULim[0]>-0.25)ULim[0]=-0.25;
      if (mom0_>0 && LLim[0]<0.1)LLim[0]=0.1;
      else if (mom0_<0 && ULim[0]>-0.1)ULim[0]=-0.1;
      //Step[0]=0.;
      //Step[1]=0.;
      //Step[2]=0.;
      //LLim[4]=-30;
      //ULim[4]=30;
      for (int i=0;i<nparam_single;i++){
         char pName[10];
         sprintf(pName,"p%d",i);
         //cout <<"i,coeff:"<<i<<" "<<coeff[i]<<endl;
         ierflg=0;
         sMinuitK->mnparm(i,pName,coeff[i],Step[i],LLim[i],ULim[i],ierflg);
      }
      cerr<<"SingleTrackKawama L198 "<<endl;
      arglist[0]=1e3;
      arglist[1]=1e-3;
      //sMinuitK->mnexcm("MIGRAD", arglist, 1, ierflg);
      //sMinuitK->mnexcm("SIMPLEX", arglist, 2, ierflg);
      sMinuitK->mnexcm("MINIMUM", arglist, 1, ierflg);
      //cout <<"Minuit Status:"<<sMinuitK->GetStatus()<<endl;
      MinuitStat=sMinuitK->GetStatus();
      cerr<<"SingleTrackKawama L203 "<<endl;
      //cout <<"ierflag:"<< ierflg<<endl;
      double amin,edm,errdef,e;
      int nvpar,nparx,icstat;
      sMinuitK->mnstat(amin,edm,errdef,nvpar,nparx,icstat);
      /*cout <<"amin:"<< amin<<endl;
        cout <<"icstat:"<< icstat<<endl;
        cout <<"edm:"<<edm<<endl;
        cout <<"errdef:"<<errdef<<endl;
        cout <<"nvpar:"<<nvpar<<endl;
        cout <<"nparx:"<<nparx<<endl;*/
      //sMinuitK->mnprin(0,amin);
      if (amin>0) chi2=amin;
      for (int i=0;i<nparam_single;i++){
         sMinuitK->GetParameter(i,coeff[i],e);
      }
      //if (amin<1)break;
      //cout << "ite="<<ite<<", chi2="<<chi2<<"mom="<<coeff[0]<<endl;
      //cout << "xmin="<<xMin_[0]<<", "<<xMin_[1]<<", "<<xMin_[2]<<endl;
      //cout << "ymin="<<yMin_[0]<<", "<<yMin_[1]<<", "<<yMin_[2]<<endl;
      //cout << "zmin="<<zMin_[0]<<", "<<zMin_[1]<<", "<<zMin_[2]<<endl;
      ite++;
   }
   sMinuitK->mncler();
   double theta=coeff[3];
   double phi=coeff[4];
   double r0[3]={r_ini2*cos(phi)*sin(theta),
      r_ini2*sin(phi),r_ini2*cos(phi)*cos(theta)};
   double dr[3];
   if (fabs(theta)<M_PI/2.)dr[2]=-1/sqrt(coeff[1]*coeff[1]+coeff[2]*coeff[2]+1);
   else dr[2]=1/sqrt(coeff[1]*coeff[1]+coeff[2]*coeff[2]+1);
   dr[0]=coeff[1]*dr[2];
   dr[1]=coeff[2]*dr[2];
   //cout <<"final:"<<coeff[0]<<" "<<coeff[1]<<" "<<coeff[2]<<" "<<coeff[3]<<" "<<coeff[4]<<endl;
   StepBackRKN(0.01,10000,-coeff[0],r0,dr);
   rvtx=TVector3(r0[0],r0[1],r0[2]);
   pvtx=TVector3(-fabs(coeff[0])*dr[0],-fabs(coeff[0])*dr[1],-fabs(coeff[0])*dr[2]);
   //cout <<"number of fmf call="<<ncallfmf_<<endl;
   return (double)chi2;
}

void E16ANA_SingleTrack_Kawama::StepBackRKN(double dstp, int nstp, 
      double mom, double r0[], double dr0[])
{
   double rout[3],drout[3],d=0.;
   for (int i=0;i<nstp;i++){
      rkn4(mom, r0, dr0, dstp, rout, drout);
      if (r0[2]<0 && i>1000){
         //cout << "i="<<i<<endl;
         break;
      }
      for (int j=0;j<3;j++){
         r0[j]=rout[j];
         dr0[j]=drout[j];
      }
      //cout << i<<" "<<r0[0]<<" "<<r0[1]<<" "<<r0[2]<<endl;
      //cout << i<<" "<<dr0[0]<<" "<<dr0[1]<<" "<<dr0[2]<<endl;
   }
}

double E16ANA_SingleTrack_Kawama::StepRKN(double dstp, 
      double mom, double r0[], double dr0[])
{
   double s=1.;
   double rout[3],drout[3],d=0.,dz,dz_pre;
   double dmin=1000.,dzmin=1000;
   double xmin[3]={1000,1000,1000};
   double ymin[3]={1000,1000,1000};
   double zmin[3]={1000,1000,1000};
   double px[3];
   double py[3];
   double pz[3];
   //double GEMd[3]={20.,40.,60.};
   double lPos_trk[3][3];
   int nstp_min[3],iGEM=0,istp=0,nGEM;

   double yscal[3],y[3],dydx[3],eps_rel=1e-2,hdid,hnext;
   double dminsum=0.;
   const double hmin=1e-3;
   //cout <<"mom="<<mom<<endl;

   TVector3 gPos_min[3];
   ROOT::Math::Plane3D GEMplane[6];
   cerr<<"SingleTrackKawama L289 "<<endl;
   double gid[3];
   Getid(gid);
   cerr<<"L290 GTRid "<<gid[0]<<"\t"<<gid[1]<<"\t"<<gid[2]<<endl;
   //cerr<<"SingleTrackKawama L282 "<<GTRid[iGEM]<<endl;
   for (int i=0;i<3;i++)GEMplane[i]=GetPlane(i);

   while (iGEM<3){
      //for (int j=0;j<3;j++) yscal[i]=fabs(r0[i])+fabs(dr0[i]*dstp);
      //cout <<"r0="<<r0[0]<<" "<<r0[1]<<" "<<r0[2]<<endl;
      //cout <<"dr0="<<dr0[0]<<" "<<dr0[1]<<" "<<dr0[2]<<endl;
      rkn4(mom, r0, dr0, dstp, rout, drout);
      //OneGoodStep(mom, r0, dr0,s,dstp, eps_rel,hdid, hnext );
      ROOT::Math::Plane3D::Point p0(r0[0],r0[1],r0[2]);
      ROOT::Math::Plane3D::Point p1(rout[0],rout[1],rout[2]);
      ROOT::Math::Plane3D::Point crossp;
      double dist0=GEMplane[iGEM].Distance(p0);
      double dist1=GEMplane[iGEM].Distance(p1);
      bool IsCrossed;
      if(dist1>0) IsCrossed=true;
      else IsCrossed=false;


      for (int i=0;i<3;i++){
         r0[i]=rout[i];
         dr0[i]=drout[i];
      }
      //cout << "step size="<<dstp<<" "<<hnext<<endl;
      //cout <<"rout="<<r0[0]<<" "<<r0[1]<<" "<<r0[2]<<endl;
      //double theta=27.315*(4-int(GTRid[iGEM]/3));
      //double theta=26.06*(4-int(GTRid[iGEM]/3));
      double theta=geom_->GTRtheta[GTRid[iGEM]];
      //double phi=30*(GTRid[iGEM]%3-1);
      double phiR=0.;
      //if (GTRid[iGEM]%3==1) phiR=-45.;
      //else  phiR=0.;
      TVector3 gPos;
      TVector3 lPos;
      TVector3 lMom;
      //cout <<"iGEM,lPos.Z():"<<" "<<iGEM<<" "<<lPos.Z()<<endl;
      /****Debug
        cout << "GTRgPos="<<GTRgPos[iGEM][0]<<" "<<GTRgPos[iGEM][1] 
        <<" "<<GTRgPos[iGEM][2] <<endl;
        cout << "GTRlPos="<<GTRlPos[iGEM][0]<<" "<<GTRlPos[iGEM][1] 
        <<" "<<GTRlPos[iGEM][2] <<endl;
        TVector3 gPostest(GTRgPos[iGEM][0],GTRgPos[iGEM][1],GTRgPos[iGEM][2]);
        TVector3 lPostest=Transform.GetLPos(gPostest,GTRid[iGEM],iGEM);
        cout << "lPostest="<<lPostest.X()<<" "<<lPostest.Y()
        <<" "<<lPostest.Z() <<endl;
       ***/
      if (IsCrossed){
         //E16ANA_Transform Transform;
         nstp_min[iGEM]=istp;
         crossp=p0+(p1-p0)*fabs(dist0)/(fabs(dist0)+fabs(dist1));

         gPos.SetXYZ(crossp.X(),crossp.Y(),crossp.Z());

         //lPos=Transform.GetLPos(gPos,theta,
         //      geom_->GTRy[iGEM][GTRid[iGEM]],
         //      geom_->GTRz[iGEM][GTRid[iGEM]],
         //      0.,phiR);
         lPos = E16ANA_Transform::GetLPos(gPos,geom_,iGEM,GTRid[iGEM]);
	 
         xmin[iGEM]=lPos.X()-GTRlPos[iGEM][0];
         ymin[iGEM]=lPos.Y()-GTRlPos[iGEM][1];
         zmin[iGEM]=lPos.Z()-GTRlPos[iGEM][2];
         //lpos, gpos are fitted points.
	 fitLPos[iGEM] = lPos;
	 fitGPos[iGEM] = gPos;

	 TVector3 gMom(fabs(mom)*dr0[0],fabs(mom)*dr0[1],fabs(mom)*dr0[2]);
         lMom=E16ANA_Transform::GetLMom(gMom,theta,0.,0.,phiR);
	 fitLMom[iGEM] = lMom;
	 fitGMom[iGEM] = gMom;
	 px[iGEM]=lMom.X();
         py[iGEM]=lMom.Y();
         pz[iGEM]=lMom.Z();
         lPos_trk[iGEM][0]=lPos.X();
         lPos_trk[iGEM][1]=lPos.Y();
         lPos_trk[iGEM][2]=lPos.Z();
         //cout <<"xmin,ymin,zmin:"<<xmin[iGEM]<<" "<<ymin[iGEM]<<" "<<zmin[iGEM]<<endl;
         gPos_min[iGEM]=gPos;
         iGEM+=1;
      }
      else if (sqrt(r0[0]*r0[0]+r0[2]*r0[2])>70.||
            fabs(r0[1])>60. || istp>2000){
         //cout <<"Error!!!!!!"<<endl;
         //cout <<"r0[0]="<<r0[0]<<" r0[1]="<<r0[1]<<" r0[2]="<<r0[2]<<endl;
         /*cout << "iGEM=0:id="<<GTRid[iGEM]<<" "<<gPos_min[0].X()<<" "<<gPos_min[0].Y()<<" "<<gPos_min[0].Z()<<endl;
           cout << "GTRgPos="<<GTRgPos[0][0]<<" "<<GTRgPos[0][1] <<" "<<GTRgPos[0][2] <<endl;
           cout << "iGEM=1:id="<<GTRid[iGEM]<<" "<< gPos_min[1].X()<<" "<<gPos_min[1].Y()<<" "<<gPos_min[1].Z()<<endl;
           cout << "GTRgPos="<<GTRgPos[1][0]<<" "<<GTRgPos[1][1] <<" "<<GTRgPos[1][2] <<endl;
           cout << "iGEM=2:id="<<GTRid[iGEM]<<" "<< gPos_min[2].X()<<" "<<gPos_min[2].Y()<<" "<<gPos_min[2].Z()<<endl;
           cout << "GTRgPos="<<GTRgPos[2][0]<<" "<<GTRgPos[2][1] <<" "<<GTRgPos[2][2] <<endl;
           */     
         double dx=(r0[0]-GTRgPos[0][0])/1e-2;
         double dy=(r0[1]-GTRgPos[0][1])/1e-2;
         double dz=(r0[2]-GTRgPos[0][2])/1e-2;
         iGEM=nGEM;
         dminsum=10000;
         //return dx*dx+dy*dy+dz*dz;
         break; 
      }
      s+=dstp;
      istp++;
   }
   ncallfmf_+=istp*1e-6;
   /*cout << "iGEM=0:"<<gPos_min[0].X()<<" "<<gPos_min[0].Y()<<" "<<gPos_min[0].Z()<<endl;
     cout << "iGEM=1:"<< gPos_min[1].X()<<" "<<gPos_min[1].Y()<<" "<<gPos_min[1].Z()<<endl;
     cout << "iGEM=2:"<< gPos_min[2].X()<<" "<<gPos_min[2].Y()<<" "<<gPos_min[2].Z()<<endl;
     */
   double xerr[3]={0.8e-2,0.8e-2,0.8e-2};   
   double yerr[3]={5e-2,5e-2,5e-2};   
   double zerr[3]={5e-3,5e-3,5e-3};  
   //double zerr[3]={5e-4,5e-4,5e-4};   
   /*double xerr[3]={0.1e-2,0.1e-2,0.1e-2};   
     double yerr[3]={1e-2,1e-2,1e-2};   
     double zerr[3]={2e-4,2e-4,2e-4};*/   
   for (int j=0;j<3;j++){
      //cout << "iGEM="<<j<<":"<<xmin[j]<<" "<<ymin[j]<<" "<<zmin[j]<<endl;
      SetlPos_trk(lPos_trk[0],lPos_trk[1],lPos_trk[2]);
      xmin[j]=xmin[j]/(xerr[j]);
      ymin[j]=ymin[j]/(yerr[j]);
      zmin[j]=zmin[j]/(zerr[j]);
      //dminsum+=xmin[j]*xmin[j]+ymin[j]*ymin[j]+zmin[j]*zmin[j];
      dminsum+=xmin[j]*xmin[j]+ymin[j]*ymin[j];
      xMin_[j]=xmin[j];
      yMin_[j]=ymin[j];
      zMin_[j]=zmin[j];
   }
   //double dminsum=dmin[0]*dmin[0]+dmin[2]*dmin[2];
   //double dminsum=dmin[0]*dmin[0]+dmin[1]*dmin[1]+dmin[2]*dmin[2];
   //return (double)dminsum/(6-nparam_single);
   return (double)dminsum;
}


/*bool E16ANA_SingleTrack_Kawama::AccurateAdvance(double ystart[],double dydxstart[],
  double     hstep,
  double     eps,
  double hinitial )
  {
// Runge-Kutta driver with adaptive stepsize control. Integrate starting
// values at y_current over hstep x2 with accuracy eps. 
// On output ystart is replaced by values at the end of the integration 
// interval. RightHandSide is the right-hand side of ODE system. 
// The source is similar to odeint routine from NRC p.721-722 .

int nstp, i, no_warnings=0;
double x, hnext, hdid, h;

//#ifdef G4DEBUG_FIELD
//  static G4int dbg=1;
//  static G4int nStpPr=50;   // For debug printing of long integrations
//  G4double ySubStepStart[G4FieldTrack::ncompSVEC];
//  G4FieldTrack  yFldTrkStart(y_current);
//#endif

double y[G4FieldTrack::ncompSVEC], dydx[G4FieldTrack::ncompSVEC];
//double ystart[G4FieldTrack::ncompSVEC], yEnd[G4FieldTrack::ncompSVEC]; 
double  x1, x2;
bool succeeded = true, lastStepSucceeded;

double startCurveLength=0.;

int  noFullIntegr=0, noSmallIntegr = 0 ;
static int  noGoodSteps =0 ;  // Bad = chord > curve-len 
const  int  nvar= fNoVars;

//G4FieldTrack yStartFT(y_current);

//  Ensure that hstep > 0
//
if( hstep <= 0.0 )
{ 
if(hstep==0.0)
{
std::ostringstream message;
message << "Proposed step is zero; hstep = " << hstep << " !";
G4Exception("G4MagInt_Driver::AccurateAdvance()", 
"GeomField1001", JustWarning, message);
return succeeded; 
}
else
{ 
std::ostringstream message;
message << "Invalid run condition." << G4endl
<< "Proposed step is negative; hstep = " << hstep << "." << G4endl
<< "Requested step cannot be negative! Aborting event.";
G4Exception("G4MagInt_Driver::AccurateAdvance()", 
"GeomField0003", EventMustBeAborted, message);
return false;
}
}

//y_current.DumpToArray( ystart );

//startCurveLength= y_current.GetCurveLength();
x1= startCurveLength; 
x2= x1 + hstep;

if ( (hinitial > 0.0) && (hinitial < hstep)
&& (hinitial > perMillion * hstep) )
{
h = hinitial;
}
else  //  Initial Step size "h" defaults to the full interval
{
h = hstep;
}

x = x1;

for (i=0;i<nvar;i++)  { y[i] = ystart[i]; }

bool lastStep= false;
nstp=1;

do
{
   //G4ThreeVector StartPos( y[0], y[1], y[2] );
   double StartPos[3]={ y[0], y[1], y[2] };

   //#ifdef G4DEBUG_FIELD
   //    G4double xSubStepStart= x; 
   //    for (i=0;i<nvar;i++)  { ySubStepStart[i] = y[i]; }
   //    yFldTrkStart.LoadFromArray(y, fNoIntegrationVariables);
   //    yFldTrkStart.SetCurveLength(x);
   //#endif

   // Old method - inline call to Equation of Motion
   //   pIntStepper->RightHandSide( y, dydx );
   // New method allows to cache field, or state (eg momentum magnitude)
   pIntStepper->ComputeRightHandSide( y, dydx );
   fNoTotalSteps++;

   // Perform the Integration
   //      
   if( h > fMinimumStep )
   { 
      OneGoodStep(y,dydx,x,h,eps,hdid,hnext) ;
      //--------------------------------------
      lastStepSucceeded= (hdid == h);   
      //#ifdef G4DEBUG_FIELD
      //      if (dbg>2) {
      //        PrintStatus( ySubStepStart, xSubStepStart, y, x, h,  nstp); // Only
      //      }
      //#endif
   }
   else
   {
      G4FieldTrack yFldTrk( G4ThreeVector(0,0,0), 
            G4ThreeVector(0,0,0), 0., 0., 0., 0. );
      G4double dchord_step, dyerr, dyerr_len;   // What to do with these ?
      yFldTrk.LoadFromArray(y, fNoIntegrationVariables); 
      yFldTrk.SetCurveLength( x );

      QuickAdvance( yFldTrk, dydx, h, dchord_step, dyerr_len ); 
      //-----------------------------------------------------

      yFldTrk.DumpToArray(y);    

      //#ifdef G4FLD_STATS
      //      fNoSmallSteps++; 
      //      if ( dyerr_len > fDyerr_max)  { fDyerr_max= dyerr_len; }
      //      fDyerrPos_smTot += dyerr_len;
      //      fSumH_sm += h;  // Length total for 'small' steps
      //      if (nstp<=1)  { fNoInitialSmallSteps++; }
      //#endif
      //#ifdef G4DEBUG_FIELD
      //      if (dbg>1)
      //      {
      //        if(fNoSmallSteps<2) { PrintStatus(ySubStepStart, x1, y, x, h, -nstp); }
      //        G4cout << "Another sub-min step, no " << fNoSmallSteps 
      //               << " of " << fNoTotalSteps << " this time " << nstp << G4endl; 
      //        PrintStatus( ySubStepStart, x1, y, x, h,  nstp);   // Only this
      //        G4cout << " dyerr= " << dyerr_len << " relative = " << dyerr_len / h 
      //               << " epsilon= " << eps << " hstep= " << hstep 
      //               << " h= " << h << " hmin= " << fMinimumStep << G4endl;
      //      }
      //#endif        
      if( h == 0.0 )
      { 
         G4Exception("G4MagInt_Driver::AccurateAdvance()",
               "GeomField0003", FatalException,
               "Integration Step became Zero!"); 
      }
      dyerr = dyerr_len / h;
      hdid= h;
      x += hdid;

      // Compute suggested new step
      hnext= ComputeNewStepSize( dyerr/eps, h);

      // .. hnext= ComputeNewStepSize_WithinLimits( dyerr/eps, h);
      lastStepSucceeded= (dyerr<= eps);
   }

   if (lastStepSucceeded)  { noFullIntegr++; }
   else                    { noSmallIntegr++; }

   //G4ThreeVector EndPos( y[0], y[1], y[2] );
   double EndPos[3]={ y[0], y[1], y[2] };

   //#ifdef  G4DEBUG_FIELD
   //    if( (dbg>0) && (dbg<=2) && (nstp>nStpPr))
   //    {
   //      if( nstp==nStpPr )  { G4cout << "***** Many steps ****" << G4endl; }
   //      G4cout << "MagIntDrv: " ; 
   //      G4cout << "hdid="  << std::setw(12) << hdid  << " "
   //             << "hnext=" << std::setw(12) << hnext << " " 
   //	     << "hstep=" << std::setw(12) << hstep << " (requested) " 
   //	     << G4endl;
   //      PrintStatus( ystart, x1, y, x, h, (nstp==nStpPr) ? -nstp: nstp); 
   //    }
   //#endif

   // Check the endpoint
   //G4double endPointDist= (EndPos-StartPos).mag(); 
   double endPointDist=Distance(EndPos, StartPos);
   if ( endPointDist >= hdid*(1.+perMillion) )
   {
      fNoBadSteps++;

      // Issue a warning only for gross differences -
      // we understand how small difference occur.
      if ( endPointDist >= hdid*(1.+perThousand) )
      { 
         //#ifdef G4DEBUG_FIELD
         //        if (dbg)
         //        {
         //          WarnEndPointTooFar ( endPointDist, hdid, eps, dbg ); 
         //          G4cerr << "  Total steps:  bad " << fNoBadSteps
         //                 << " good " << noGoodSteps << " current h= " << hdid
         //                 << G4endl;
         //          PrintStatus( ystart, x1, y, x, hstep, no_warnings?nstp:-nstp);  
         //        }
         //#endif
         no_warnings++;
      }
   }
   else
   {
      noGoodSteps ++;
   } 
   // #endif

   //  Avoid numerous small last steps
   if( (h < eps * hstep) || (h < fSmallestFraction * startCurveLength) )
   {
      // No more integration -- the next step will not happen
      lastStep = true;  
   }
   else
   {
      // Check the proposed next stepsize
      if(std::fabs(hnext) <= Hmin())
      {
         //#ifdef  G4DEBUG_FIELD
         //        // If simply a very small interval is being integrated, do not warn
         //        if( (x < x2 * (1-eps) ) &&        // The last step can be small: OK
         //            (std::fabs(hstep) > Hmin()) ) // and if we are asked, it's OK
         //        {
         //          if(dbg>0)
         //          {
         //            WarnSmallStepSize( hnext, hstep, h, x-x1, nstp );  
         //            PrintStatus( ystart, x1, y, x, hstep, no_warnings?nstp:-nstp);
         //          }
         //          no_warnings++;
         //        }
         //#endif
         // Make sure that the next step is at least Hmin.
         h = Hmin();
      }
      else
      {
         h = hnext;
      }

      //  Ensure that the next step does not overshoot
      if ( x+h > x2 )
      {                // When stepsize overshoots, decrease it!
         h = x2 - x ;   // Must cope with difficult rounding-error
      }                // issues if hstep << x2

      if ( h == 0.0 )
      {
         // Cannot progress - accept this as last step - by default
         lastStep = true;
         //#ifdef G4DEBUG_FIELD
         //        if (dbg>2)
         //        {
         //          int prec= G4cout.precision(12); 
         //          G4cout << "Warning: G4MagIntegratorDriver::AccurateAdvance"
         //                 << G4endl
         //                 << "  Integration step 'h' became "
         //                 << h << " due to roundoff. " << G4endl
         //		 << " Calculated as difference of x2= "<< x2 << " and x=" << x
         //                 << "  Forcing termination of advance." << G4endl;
         //          G4cout.precision(prec);
         //        }          
         //#endif
      }
   }
} while ( ((nstp++)<=fMaxNoSteps) && (x < x2) && (!lastStep) );
// Have we reached the end ?
// --> a better test might be x-x2 > an_epsilon

succeeded=  (x>=x2);  // If it was a "forced" last step

//for (i=0;i<nvar;i++)  { yEnd[i] = y[i]; }

// Put back the values.
//y_current.LoadFromArray( yEnd, fNoIntegrationVariables );
//y_current.SetCurveLength( x );

if(nstp > fMaxNoSteps)
{
   //no_warnings++;
   succeeded = false;
   //#ifdef G4DEBUG_FIELD
   //    if (dbg)
   //    {
   //      WarnTooManyStep( x1, x2, x );  //  Issue WARNING
   //      PrintStatus( yEnd, x1, y, x, hstep, -nstp);
   //    }
   //#endif
}

//#ifdef G4DEBUG_FIELD
//  if( dbg && no_warnings )
//  {
//    G4cerr << "G4MagIntegratorDriver exit status: no-steps " << nstp <<G4endl;
//    PrintStatus( yEnd, x1, y, x, hstep, nstp);
//  }
//#endif

return succeeded;
}  // end of AccurateAdvance ...........................
*/

void E16ANA_SingleTrack_Kawama::OneGoodStep(double mom, 
      double   y[],        // InOut
      double dydx[],
      double& x,         // InOut
      double htry,
      double eps_rel_max,
      double& hdid,      // Out
      double& hnext )    // Out

// Driver for one Runge-Kutta Step with monitoring of local truncation error
// to ensure accuracy and adjust stepsize. Input are dependent variable
// array y[0,...,5] and its derivative dydx[0,...,5] at the
// starting value of the independent variable x . Also input are stepsize
// to be attempted htry, and the required accuracy eps. On output y and x
// are replaced by their new values, hdid is the stepsize that was actually
// accomplished, and hnext is the estimated next stepsize. 
// This is similar to the function rkqs from the book:
// Numerical Recipes in C: The Art of Scientific Computing (NRC), Second
// Edition, by William H. Press, Saul A. Teukolsky, William T.
// Vetterling, and Brian P. Flannery (Cambridge University Press 1992),
// 16.2 Adaptive StepSize Control for Runge-Kutta, p. 719

{
   fMaximumStep=2e-2;
   fMinimumStep=1e-3;
   const double  max_stepping_increase=1.5; 
   const double  Safety=0.9;
   const double  Pgrow=-0.2;
   const double  Pshrnk=-0.25;
   const double  errcon=6.e-4;
   double errmax_sq;
   double h, htemp, xnew ;

   //double yerr[G4FieldTrack::ncompSVEC], ytemp[G4FieldTrack::ncompSVEC];
   double yerr[3], ytemp[3], dydxtemp[3];
   double dyout[3];

   h = htry ; // Set stepsize to the initial trial value

   //double inv_eps_vel_sq = 1.0 / (eps_rel_max*eps_rel_max);

   double errpos_sq=0.0;    // square of displacement error
   //double errvel_sq=0.0;    // square of momentum vector difference
   double errspin_sq=0.0;   // square of spin vector difference

   int iter;

   static int tot_no_trials=0; 
   const int max_trials=100; 

   //G4ThreeVector Spin(y[9],y[10],y[11]);
   //bool     hasSpin= (Spin.mag2() > 0.0); 

   for (iter=0; iter<max_trials ;iter++)
   {
      tot_no_trials++;
      //pIntStepper-> Stepper(y,dydx,h,ytemp,yerr); 
      rkn4Stepper(mom, y, dydx, h, ytemp, yerr, dydxtemp);
      //            *******
      //cout <<"yerr:"<< yerr[0]<<" "<< yerr[1]<<" "<< yerr[2]<<endl;
      double eps_pos = eps_rel_max * std::max(h, fMinimumStep); 
      double inv_eps_pos_sq = 1.0 / (eps_pos*eps_pos); 

      // Evaluate accuracy
      //
      errpos_sq =  sqr(yerr[0]) + sqr(yerr[1]) + sqr(yerr[2]) ;
      errpos_sq *= inv_eps_pos_sq; // Scale relative to required tolerance
      //cout <<"eps_pos:"<< eps_pos<<endl;
      //cout << "inv_eps_pos_sq:" << inv_eps_pos_sq<<endl;; 

      // Accuracy for momentum
      /*errvel_sq =  (sqr(yerr[3]) + sqr(yerr[4]) + sqr(yerr[5]) )
        / (sqr(y[3]) + sqr(y[4]) + sqr(y[5]) );
        errvel_sq *= inv_eps_vel_sq;*/
      //errmax_sq = std::max( errpos_sq, errvel_sq ); // Square of maximum error
      errmax_sq =  errpos_sq; // Square of maximum error
      //cout << "errmax_sq:" << errmax_sq<<endl;; 

      /*if( hasSpin )
        { 
      // Accuracy for spin
      errspin_sq =  ( sqr(yerr[9]) + sqr(yerr[10]) + sqr(yerr[11]) )
      /  ( sqr(y[9]) + sqr(y[10]) + sqr(y[11]) );
      errspin_sq *= inv_eps_vel_sq;
      errmax_sq = std::max( errmax_sq, errspin_sq ); 
      }*/

      if ( errmax_sq <= 1.0 )  { break; } // Step succeeded. 

      // Step failed; compute the size of retrial Step.
      //htemp = GetSafety()*h* std::pow( errmax_sq, 0.5*GetPshrnk() );
      htemp = Safety*h* std::pow( errmax_sq, 0.5*Pshrnk );

      if (htemp >= 0.1*h)  { h = htemp; }  // Truncation error too large,
      else  { h = 0.1*h; }                 // reduce stepsize, but no more
      // than a factor of 10
      xnew = x + h;
      if(xnew == x)
      {
         /*G4cerr << "G4MagIntegratorDriver::OneGoodStep:" << G4endl
           << "  Stepsize underflow in Stepper " << G4endl ;
           G4cerr << "  Step's start x=" << x << " and end x= " << xnew 
           << " are equal !! " << G4endl
           <<"  Due to step-size= " << h 
           << " . Note that input step was " << htry << G4endl;*/
         break;
      }
   }

   /*#ifdef G4FLD_STATS
   // Sum of squares of position error // and momentum dir (underestimated)
   fSumH_lg += h; 
   fDyerrPos_lgTot += errpos_sq;
   fDyerrVel_lgTot += errvel_sq * h * h; 
#endif*/

   // Compute size of next Step
   //cout <<"errmax_sq:"<<errmax_sq<<endl;
   if (errmax_sq > errcon*errcon)
   { 
      //hnext = GetSafety()*h*std::pow(errmax_sq, 0.5*GetPgrow());
      hnext = Safety*h*std::pow(errmax_sq, 0.5*Pgrow);
   }
   else
   {
      hnext = max_stepping_increase*h ; // No more than a factor of 5 increase
   }
   if (hnext>fMaximumStep) hnext=fMaximumStep;
   x += (hdid = h);

   //for(int k=0;k<fNoIntegrationVariables;k++) { y[k] = ytemp[k]; }
   for(int k=0;k<3;k++) { 
      y[k] = ytemp[k]; 
      dydx[k] = dydxtemp[k]; 
   }

   return;
}   // end of  OneGoodStep .............................

void E16ANA_SingleTrack_Kawama::rkn4Stepper( const double mom, 
      const double yInput[],
      double dydx[],
      double hstep,
      double yOutput[],
      double yError[],
      double dydxOut[]  )
{
   const int nvar = 3;  // number of variables integrated
   //const int maxvar= GetNumberOfStateVariables();
   const int maxvar= 3;
   double yInitial[3],yMiddle[3],yOneStep[3], dydxOne[3],dydxMid[3];

   // Correction for Richardson extrapolation
   //double  correction = 1. / ( (1 << IntegratorOrder()) -1 );
   double  correction = 1. ;

   int i;

   // Saving yInput because yInput and yOutput can be aliases for same array
   for (i=0;    i<maxvar; i++) { yInitial[i]= yInput[i]; }

   // Must copy the part of the state *not* integrated to the output
   //for (i=nvar; i<maxvar; i++) { yOutput[i]=  yInput[i]; }

   // yInitial[7]= yInput[7];  //  The time is typically needed
   //yMiddle[7]  = yInput[7];   // Copy the time from initial value 
   //yOneStep[7] = yInput[7];   // As it contributes to final value of yOutput ?
   // yOutput[7] = yInput[7];  // -> dumb stepper does it too for RK4
   //yError[7] = 0.0;         

   double halfStep = hstep * 0.5; 
   // Do two half steps
   //
   //GetConstField(yInitial,Field);
   //DumbStepper  (yInitial,  dydx,   halfStep, yMiddle);
   rkn4  (mom, yInitial,  dydx,   halfStep, yMiddle, dydxMid);
   //RightHandSideConst(yMiddle, dydxMid);    
   //DumbStepper  (yMiddle, dydxMid, halfStep, yOutput); 
   rkn4  (mom, yMiddle,  dydxMid,   halfStep, yOutput, dydxOut);

   // Store midpoint, chord calculation
   //
   //fMidPoint = G4ThreeVector( yMiddle[0],  yMiddle[1],  yMiddle[2]); 

   // Do a full Step
   //
   //DumbStepper(yInitial, dydx, hstep, yOneStep);
   rkn4  (mom, yInitial,  dydx,   hstep, yOneStep, dydxOne);
   for(i=0;i<nvar;i++)
   {
      //cout <<"twostep, onestep:"<<i<<" "<< yOutput[i] <<" "<< yOneStep[i]<<endl ;
      yError [i] = yOutput[i] - yOneStep[i] ;
      yOutput[i] += yError[i]*correction ;
      // Provides accuracy increased by 1 order via the 
      // Richardson extrapolation  
   }

   //fInitialPoint = G4ThreeVector( yInitial[0], yInitial[1], yInitial[2]); 
   //fFinalPoint   = G4ThreeVector( yOutput[0],  yOutput[1],  yOutput[2]); 

   return;
}

/*A 4th order Runge-Kutta program.*/
void E16ANA_SingleTrack_Kawama::rkn4(double mom, 
      double y[], double dydx[], double h, 
      double yout[], double dydxout[])
{
   double h2,h6,h28,dym[3],dyt[3],yt[3];
   double k1[3],k2[3],k3[3],k4[3];

   //cout << y[0] <<" "<<y[1]<<" "<<y[2]<<endl;
   h2=h/2.;
   h6=h/6.;
   h28=h*h/8;
   EOM(mom,y,dydx,k1);
   for (int i=0;i<3;i++){
      yt[i]=y[i]+h2*dydx[i]+h28*k1[i];
      dyt[i]=dydx[i]+h2*k1[i];
   }
   EOM(mom,yt,dyt,k2);
   for (int i=0;i<3;i++) dyt[i]=dydx[i]+h2*k2[i];
   EOM(mom,yt,dyt,k3);
   for (int i=0;i<3;i++){
      yt[i]=y[i]+h*dydx[i]+4*h28*k3[i];
      dyt[i]=dydx[i]+h*k3[i];
   }
   EOM(mom,yt,dyt,k4);
   for (int i=0;i<3;i++){
      dydxout[i]=dydx[i]+h6*(k1[i]+2*(k2[i]+k3[i])+k4[i]);
      yout[i]=y[i]+h*dydx[i]+h*h6*(k1[i]+k2[i]+k3[i]);
   }
}

void E16ANA_SingleTrack_Kawama::EOM(double mom, double r[], double dr[], double d2r[]) 
{
   int jj[3]={1,2,0};
   int kk[3]={2,0,1};
   double B[3];
   double point[3];
   for (int i=0;i<3;i++) point[i]=r[i];
   fmf_->GetFieldValue(point, B, 0);
   /*B[0]=0.;
     B[1]=15000.;
     B[2]=0.;*/
   //cout <<"dr="<< dr[0]<<" "<<dr[1]<<" "<<dr[2]<<endl;
   //cout <<"r="<< r[0]<<" "<<r[1]<<" "<<r[2]<<endl;
   //cout <<"B="<< B[0]<<" "<<B[1]<<" "<<B[2]<<endl;

   for (int i=0;i<3;i++){
      d2r[i]=0.299792e-6/mom*(dr[jj[i]]*B[kk[i]]-dr[kk[i]]*B[jj[i]]);
      //d2r[i]=c*1e-2/(mom*1e9)*(dr[jj[i]]*B[kk[i]]*1e-4-dr[kk[i]]*B[jj[i]]*1e-4);
   }
}

/*void E16ANA_SingleTrack_Kawama::SetMom0(double x[], double y[], double z[])
  {
  int jj[3]={1,2,0};
  int kk[3]={2,0,1};
  double d[3],A[3];

  double vecAB[2]={x[1]-x[0],z[1]-z[0]};
  double vecAC[2]={x[2]-x[0],z[2]-z[0]};
  double ABxAC=vecAB[0]*vecAC[1]-vecAB[1]*vecAC[0];
  double r0=sqrt(x[0]*x[0]+y[0]*y[0]+z[0]*z[0]);
  for (int i=0;i<3;i++){
  d[i]=sqrt((x[jj[i]]-x[kk[i]])*(x[jj[i]]-x[kk[i]])+
  (z[jj[i]]-z[kk[i]])*(z[jj[i]]-z[kk[i]]));
  }
  for (int i=0;i<3;i++){
  A[i]=acos((d[jj[i]]*d[jj[i]]+d[kk[i]]*d[kk[i]]-d[i]*d[i])
  /2/d[jj[i]]/d[kk[i]]);
  }

  double B[3],point[3];
  double B_ave=0;
//for (int i=0;i<3;i++){
// point[0]=(double)x[i];
//point[1]=(double)y[i];
//point[2]=(double)z[i];
//fmf_->GetFieldValue(point, B, 0);
//B_ave+=fabs(B[1]*1e-4)/3.;
//}
point[0]=(double)x[0];
point[1]=(double)y[0];
point[2]=(double)z[0];
fmf_->GetFieldValue(point, B, 0);
B_ave=fabs(B[1]*1e-4);

//mom0_=ABxAC/fabs(ABxAC)*d[0]/sin(A[0])/2.*1e-2*1.7/3.34;
mom0_=-ABxAC/fabs(ABxAC)*d[0]/sin(A[0])/2.*1e-2*B_ave/3.34;
//xp0_=(x[2]-x[1])/(z[2]-z[1])+A[0]*ABxAC/fabs(ABxAC);
//xp0_=(x[2]-x[1])/(z[2]-z[1]);
xp0_=x[0]/z[0];
theta0_=atan(xp0_);
//x0_=x[0]-(r0-15.)*sin(atan(xp0_));
for (int i=0;i<3;i++){
d[i]=sqrt((y[jj[i]]-y[kk[i]])*(y[jj[i]]-y[kk[i]])+
(z[jj[i]]-z[kk[i]])*(z[jj[i]]-z[kk[i]]));
}
for (int i=0;i<3;i++){
A[i]=acos((d[jj[i]]*d[jj[i]]+d[kk[i]]*d[kk[i]]-d[i]*d[i])
/2/d[jj[i]]/d[kk[i]]);
}
//yp0_=(y[2]-y[1])/(z[2]-z[1]);
yp0_=y[0]/z[0];
phi0_=atan(y[0]/sqrt(x[0]*x[0]+z[0]*z[0]));
//y0_=15./cos(atan(yp0_));
//y0_=y[0]-(r0-15.)*sin(atan(yp0_));
}*/

void E16ANA_SingleTrack_Kawama::SetMom0(TVector3 vA, TVector3 vB, TVector3 vC)
{
   int jj[3]={1,2,0};
   int kk[3]={2,0,1};
   double d[3],A[3];

   GTRgPos[0][0]=vA.X();
   GTRgPos[0][1]=vA.Y();
   GTRgPos[0][2]=vA.Z();
   GTRgPos[1][0]=vB.X();
   GTRgPos[1][1]=vB.Y();
   GTRgPos[1][2]=vB.Z();
   GTRgPos[2][0]=vC.X();
   GTRgPos[2][1]=vC.Y();
   GTRgPos[2][2]=vC.Z();

   TVector3 vAB=vB-vA;
   TVector3 vAC=vC-vA;
   TVector3 vBC=vC-vB;
   TVector3 vABxAC=vAB.Cross(vAC);
   //Lebgth of three sides 
   d[0]=sqrt(vBC.x()*vBC.x()+vBC.z()*vBC.z());   
   d[1]=sqrt(vAC.x()*vAC.x()+vAC.z()*vAC.z());   
   d[2]=sqrt(vAB.x()*vAB.x()+vAB.z()*vAB.z());   
   //Angles
   for (int i=0;i<3;i++){
      A[i]=acos((d[jj[i]]*d[jj[i]]+d[kk[i]]*d[kk[i]]-d[i]*d[i])
            /2/d[jj[i]]/d[kk[i]]);
   }

   double B[3],point[3];
   double B_ave=0;
   point[0]=(double)vA.x();
   point[1]=(double)vA.y();
   point[2]=(double)vA.z();
   fmf_->GetFieldValue(point, B, 0);
   /*B[0]=0;
     B[1]=15000;
     B[2]=0;*/
   B_ave=fabs(B[1]*1e-4);

   if (A[0]!=0){
     mom0_=vABxAC.y()/fabs(vABxAC.y())*d[0]/sin(A[0])/2.*1e-2*B_ave/3.34;
   }
   else{
     mom0_=5.0;
   }
   xp0_=vA.x()/vA.z();
   if (vA.z()>0)theta0_=atan(vA.x()/vA.z());
   else if(vA.x()>0)theta0_=M_PI-atan(-vA.x()/vA.z());
   else theta0_=-M_PI-atan(-vA.x()/vA.z());
   //yp0_=(y[2]-y[1])/(z[2]-z[1]);
   yp0_=vA.y()/vA.z();
   //yp0_=vAC.y()/vAC.z();
   phi0_=atan(vA.y()/sqrt(vA.x()*vA.x()+vA.z()*vA.z()));
   //y0_=15./cos(atan(yp0_));
   //y0_=y[0]-(r0-15.)*sin(atan(yp0_));
   //cout <<"mom0="<<mom0_<<" theta0="<<theta0_<<" phi0="<<phi0_<<endl;
   /*cout <<"x0="<<r_ini2*cos(phi0_)*sin(theta0_)
     <<", y0="<<r_ini2*sin(phi0_)
     <<", z0="<<r_ini2*cos(phi0_)*cos(theta0_)<<endl;*/
}

void E16ANA_SingleTrack_Kawama::SmearData(int sxflag, int syflag){
   double GTRanglex[3];
   double GTRangley[3];
   double GTRxl[3];
   double GTRyl[3];
   double GTRzl[3];
   double GTRlxyz_s[3][3];//[GEM1,2,3][x,y,z]
   TVector3 gPos[3];//[GEM1,2,3][x,y,z]
   double pxGTR1=g4data_->datGTR1[0].px;
   double pyGTR1=g4data_->datGTR1[0].py;
   double pzGTR1=g4data_->datGTR1[0].pz;
   double pxGTR2=g4data_->datGTR2[0].px;
   double pyGTR2=g4data_->datGTR2[0].py;
   double pzGTR2=g4data_->datGTR2[0].pz;
   double pxGTR3=g4data_->datGTR3[0].px;
   double pyGTR3=g4data_->datGTR3[0].py;
   double pzGTR3=g4data_->datGTR3[0].pz;
   GTRanglex[0]=atan(pxGTR1/pzGTR1);
   GTRangley[0]=atan(pyGTR1/pzGTR1);
   GTRanglex[1]=atan(pxGTR2/pzGTR2);
   GTRangley[1]=atan(pyGTR2/pzGTR2);
   GTRanglex[2]=atan(pxGTR3/pzGTR3);
   GTRangley[2]=atan(pyGTR3/pzGTR3);
   GTRxl[0]=g4data_->datGTR1[0].x;
   GTRyl[0]=g4data_->datGTR1[0].y;
   GTRzl[0]=g4data_->datGTR1[0].z;
   GTRxl[1]=g4data_->datGTR2[0].x;
   GTRyl[1]=g4data_->datGTR2[0].y;
   GTRzl[1]=g4data_->datGTR2[0].z;
   GTRxl[2]=g4data_->datGTR3[0].x;
   GTRyl[2]=g4data_->datGTR3[0].y;
   GTRzl[2]=g4data_->datGTR3[0].z;
   //E16ANA_Transform Transform;
   SetID(g4data_->datGTR1[0].GTRID,
         g4data_->datGTR2[0].GTRID,g4data_->datGTR3[0].GTRID);


   //gRandom->SetSeed(rndmSeed_); 
   for (int i=0;i<3;i++){
      //GTRid...0-26
      //double theta=27.315*(4-int(GTRid[i]/3));
      //double theta=26.06*(4-int(GTRid[i]/3));
      double theta=geom_->GTRtheta[GTRid[i]];
      //double phi=30*(GTRid[i]%3-1);
      //cout <<"i="<<i<<",Before smear"<< GTRxl[i]
      //<<" "<< GTRyl[i]<<" "<< GTRzl[i]<<endl;
      double smear_x;
      double smear_y;
      if(sxflag==1){
         //smear_x=60e-4+20e-4/15.*(fabs(GTRanglex[i])*180/3.14);//cm
         if(fabs(GTRanglex[i])<15){
            smear_x=60e-4+40e-4/15.*(fabs(GTRanglex[i])*180/M_PI);//cm
         }
         else if(fabs(GTRanglex[i])<30){
            smear_x=100e-4;//cm
         }
         else {
            smear_x=100e-4+50e-4/15.*((fabs(GTRanglex[i])-30)*180/M_PI);//cm
         }
      }
      else{
         smear_x=0.;
      }
      if(syflag==1){
         smear_y=3.0e-2;//cm
      }
      else{
         smear_y=0.;//cm
      }
      //double smear_y=0.;//cm
      //if (i==1) smear_x=sqrt(smear_x*smear_x+220e-6*220e-6);
      //if (i==2) smear_x=sqrt(smear_x*smear_x+320e-6*320e-6);
      //cout << "angle="<<GTRanglex[i]<<",smear="<<smear_x*1e4<<endl;
      //cout <<GTRoffsetx[i][GTRid[i]]<<" "<<GTRoffsety[i][GTRid[i]]<<" "<<GTRoffsetz[i][GTRid[i]]<<endl;

      if (alflagx==0){   
         //GTRlxyz_s[i][0]=gRandom->Gaus(GTRxl[i],smear_x);
         GTRlxyz_s[i][0]=rnd->Gaus(GTRxl[i],smear_x);
      }else{
         //GTRlxyz_s[i][0]=gRandom->Gaus(GTRxl[i],smear_x)
            GTRlxyz_s[i][0]=rnd->Gaus(GTRxl[i],smear_x)
            +GTRoffsetx[i][GTRid[i]];
      }
      if (alflagy==0){   
         //GTRlxyz_s[i][1]=gRandom->Gaus(GTRyl[i],smear_y);
         GTRlxyz_s[i][1]=rnd->Gaus(GTRyl[i],smear_y);
      }else{
         //GTRlxyz_s[i][1]=gRandom->Gaus(GTRyl[i],smear_y)
            GTRlxyz_s[i][1]=rnd->Gaus(GTRyl[i],smear_y)
            +GTRoffsety[i][GTRid[i]];
      }
      if (alflagz==0){   
         GTRlxyz_s[i][2]=GTRzl[i];
      }else{
         GTRlxyz_s[i][2]=GTRzl[i]+GTRoffsetz[i][GTRid[i]];
      }
      /*cout <<"i="<<i<<",After smear"<< GTRlxyz_s[i][0]
        <<" "<< GTRlxyz_s[i][1]<<" "<< GTRlxyz_s[i][2]<<endl;*/
      TVector3 lPos(GTRlxyz_s[i][0],GTRlxyz_s[i][1],
            GTRlxyz_s[i][2]);
      double phiR=0.;
      //gPos[i]=Transform.GetGPos(lPos,theta,
      //gPos[i]=E16ANA_Transform::GetGPos(lPos,theta,
      //      geom_->GTRy[i][GTRid[i]],
      //      geom_->GTRz[i][GTRid[i]],
      //      0.,phiR);
      gPos[i] = E16ANA_Transform::GetGPos(lPos, geom_, i, GTRid[i]);
      //cout<<"gPos"<<i<<":"<<gPos[i].X()<<" "
      //<<gPos[i].Y()<<" "<<gPos[i].Z()<<endl;
   }
   SetlPos(GTRlxyz_s[0],GTRlxyz_s[1],GTRlxyz_s[2]);
   SetMom0(gPos[0],gPos[1],gPos[2]);
}

ROOT::Math::Plane3D E16ANA_SingleTrack_Kawama::GetPlane(int iGEM){
#if 0
   double dx[3]={10.,20.,30.};
   double dy[3]={10.,20.,30.};
   double dz[3]={10e-4,10e-4,10e-4};
#endif
   //double theta=27.315*(4-int(GTRid[iGEM]/3));
   //double theta=26.06*(4-int(GTRid[iGEM]/3));
   double theta=geom_->GTRtheta[GTRid[iGEM]];
   double phiR=0.;
   double x,y,z,r;
   ROOT::Math::Plane3D plane;
   ROOT::Math::Plane3D::Vector normal;
   ROOT::Math::Plane3D::Point origin;
   //origin.SetXYZ(
   //      geom_->GTRdx[iGEM][GTRid[iGEM]],
   //      geom_->GTRy[iGEM][GTRid[iGEM]]+geom_->GTRdy[iGEM][GTRid[iGEM]],
   //      geom_->GTRz[iGEM][GTRid[iGEM]]+geom_->GTRdz[iGEM][GTRid[iGEM]]
   //      );
   //origin = ROOT::Math::RotationY(theta/180.0*M_PI)*origin;
   cerr<<"SingleTrackKawama L1226 "<<iGEM<<endl;
   origin = E16ANA_Transform::GetGPos(TVector3(0.0,0.0,0.0),geom_,iGEM,GTRid[iGEM]);
   normal.SetXYZ(0.0,0.0,1.0);
   normal = ROOT::Math::RotationX(geom_->GTRrotx[iGEM][GTRid[iGEM]]/180.0*M_PI)*normal;
   normal = ROOT::Math::RotationY(geom_->GTRroty[iGEM][GTRid[iGEM]]/180.0*M_PI)*normal;
   normal = ROOT::Math::RotationZ(geom_->GTRrotz[iGEM][GTRid[iGEM]]/180.0*M_PI)*normal;
   normal = ROOT::Math::RotationX(geom_->GTRrotx_frame[GTRid[iGEM]]/180.0*M_PI)*normal;
   normal = ROOT::Math::RotationY(geom_->GTRroty_frame[GTRid[iGEM]]/180.0*M_PI)*normal;
   normal = ROOT::Math::RotationZ(geom_->GTRrotz_frame[GTRid[iGEM]]/180.0*M_PI)*normal;
   normal = ROOT::Math::RotationY(geom_->GTRtheta[GTRid[iGEM]]/180.0*M_PI)*normal;
   plane=ROOT::Math::Plane3D::Plane3D(normal,origin);
   return plane;
}

void E16ANA_SingleTrack_Kawama::SetMinuit(TMinuit* min)
{
   sMinuitK=min;
} 
