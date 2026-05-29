//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai
//2015-12-31, uploaded by yokkaich
//2015-12-25, uploaded by komatsu
//2015-11-02, uploaded by yokkaich
//2015-11-02, uploaded by komatsu
//2015-11-02, modified by komatsu
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
#include "E16ANA_SingleTrack.hh"
   
TMinuit *sMinuit= new TMinuit(E16ANA_SingleTrack::nparam_single);

void trkfcnSingle(int &npar, double *gin, double &f, double* par, int iflag){
  //Developed by Komatsu 2015/10/30
  //dr (direction of momentum) is calculated from TVector3 'mom_est'

  double r0[3];
  double mom=par[0];
  double vtxx=par[1];
  double vtxy=par[2];
  double theta=par[3];
  double phi=par[4];

  //vertex: assumed origin
   r0[0]=vtxx;
   r0[1]=vtxy;
   r0[2]=0.;

   double dr0[3];
   double stp=1e-1;
   //cout<< "SingleTrack L48 "<<endl;
   //cout << "mom="<<mom<<",xp="<<xp<<",yp="<<yp<<endl;
   //cout << "theta="<<theta<<",phi="<<phi<<endl;
   //if (fabs(theta)<M_PI/2.)dr0[2]=1/sqrt(xp*xp+yp*yp+1);
   //else dr0[2]=-1/sqrt(xp*xp+yp*yp+1);
   
   dr0[2]=cos(phi)*cos(theta);
   dr0[0]=cos(phi)*sin(theta);
   dr0[1]=sin(phi);

   //cout << "x="<<r0[0]<<", y="<<r0[1]<<", z="<<r0[2]<<endl;
   //cout << "dx="<<dr0[0]<<", dy="<<dr0[1]<<", dz="<<dr0[2]<<endl;
   E16ANA_SingleTrack *trk=(E16ANA_SingleTrack*)sMinuit->GetObjectFit();
   //trk->Getxyz0(r0);
   double dmin=trk->StepRKN(stp,mom,r0,dr0);
   //cout << "dmin="<<dmin<<endl;
   f=dmin;
   //cout << "Start Stepper"<<endl;
}


//Added by Komatsu 2015/10/29------------
//Set initial conditions for single track fitting in waveformDoubleFit.cc
E16ANA_SingleTrack::E16ANA_SingleTrack
(const E16ANA_GeometryV1 *geom)
  :geom_(geom){

  crossflag = 0;
  charge=0;
  chi2=1000000;
}

E16ANA_SingleTrack::~E16ANA_SingleTrack(){
   //delete rnd;
}

void E16ANA_SingleTrack::SetInitCond_Single(TVector3 hitl1, TVector3 hitl2, TVector3 hitl3, TVector3 hitg1, TVector3 hitg2, TVector3 hitg3, TVector3 vtx_est, TVector3 mom_est, double *mom_est_abs, int *gtrid1, int *gtrid2, int *gtrid3){
  
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
  charge = (mom0_>0)? +1 :-1;
  cerr<<"charge in singletrack: "<<charge<<endl;
  xp0_=hitg1.X()/hitg1.Z();

  //if (hitg1.Z()>0)theta0_=atan(hitg1.X()/hitg1.Z());
  //else if(hitg1.X()>0)theta0_=M_PI-atan(-hitg1.X()/hitg1.Z());
  //else theta0_=-M_PI-atan(-hitg1.X()/hitg1.Z());

  //phi0_=atan(hitg1.Y()/sqrt(hitg1.X()*hitg1.X()+hitg1.Z()*hitg1.Z()));

  //Added by Komatsu 2015/10/30
  if (mom_est.Z()>0)theta0_=atan(mom_est.X()/mom_est.Z());
  else if(mom_est.X()>0)theta0_=M_PI-atan(-mom_est.X()/mom_est.Z());
  else theta0_=-M_PI-atan(-mom_est.X()/mom_est.Z());
  phi0_=atan(mom_est.Y()/sqrt(mom_est.X()*mom_est.X()+mom_est.Z()*mom_est.Z()));

  //yp0_=(y[2]-y[1])/(z[2]-z[1]);
  yp0_=hitg1.Y()/hitg1.Z();
  //yp0_=hitg1C.Y()/hitg1C.Z();

  
  vtxx_=0.;
  vtxy_=0.;
  
  cerr<<"mom0 "<<mom0_<<"\t"<<" xp0 "<<xp0_<<" yp0 "<<yp0_<<" theta0 "<<theta0_<<" phi0 "<<phi0_<<" Single Track L129 "<<endl;

}
//added by Komatsu end---------------------------------------
double E16ANA_SingleTrack::FitSingle()
{
  //double ds=0.;
  double arglist[10];
  chi2=1000.;
   int ierflg=0;
   sMinuit->SetFCN(trkfcnSingle);
   arglist[0]=1;
   sMinuit->mnexcm("SET ERR", arglist, 1, ierflg);
   sMinuit->SetPrintLevel(-1);
   arglist[0]=0;
   sMinuit->mnexcm("SET STRA 2", arglist, 0, ierflg);
   int ite=0;
   //while(chi2>1 && ite<10){
   while(ite<1){
      //cout << "Iteration"<<ite<<" Start"<<endl;
      if (ite==0){
         if(fabs(mom0_)>0.2 && fabs(mom0_)<5)coeff[0]=(double)mom0_;
         else coeff[0]=(double)mom0_/fabs(mom0_);
         //coeff[1]=(double)xp0_;
         //coeff[2]=(double)yp0_;
         coeff[1]=(double)vtxx_;
         coeff[2]=(double)vtxy_;
         coeff[3]=(double)theta0_;
         coeff[4]=(double)phi0_;
         //cout <<"Initial:"<<mom0_<<" "<<xp0_<<" "<<yp0_<<" "<<theta0_<<" "<<phi0_<<endl;
      }
      double Step[nparam_single];
      double LLim[nparam_single];
      double ULim[nparam_single];

      //Kawama par limints.
      //for (int i=0;i<nparam_single;i++){
         //Step[i]=fabs(coeff[i])/2.;
         //LLim[i]=coeff[i]-fabs(coeff[i])/2.;
         //ULim[i]=coeff[i]+fabs(coeff[i])/2.;
         //Step[i]=fabs(coeff[i]);
         //LLim[i]=coeff[i]-fabs(coeff[i]);
         //ULim[i]=coeff[i]+fabs(coeff[i]);
      //}
      
      //Written by Komatsu.
      Step[0]=0.1;//mom0
      LLim[0]=-5.0;
      ULim[0]=5.0;
      
      Step[1]=0.1;//vtxx      
      LLim[1]=-0.75;//0.15cmx5
      ULim[1]=0.75;
      //LLim[1]=-10.0;
      //ULim[1]=10.0;      

      Step[2]=0.1;//vtxy
      LLim[2]=-0.85;//0.17cmx5
      ULim[2]=0.85;
      //LLim[2]=-10.0;
      //ULim[2]=10.0;      

      Step[3]=0.1;//theta0
      LLim[3]=-1.*M_PI;//
      ULim[3]=M_PI;
      Step[4]=0.1;//phi0
      LLim[4]=-1.*M_PI;//
      ULim[4]=M_PI;

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
         sMinuit->mnparm(i,pName,coeff[i],Step[i],LLim[i],ULim[i],ierflg);
      }
      arglist[0]=1e3;
      arglist[1]=1e-3;
      sMinuit->mnexcm("MIGRAD", arglist, 1, ierflg);
      //sMinuit->mnexcm("SIMPLEX", arglist, 2, ierflg);
      //      sMinuit->mnexcm("MINIMUM", arglist, 1, ierflg);
      minimizeStatus = ierflg;
      //cout <<"ierflag:"<< ierflg<<endl;
      double amin,edm,errdef,e;
      int nvpar,nparx,icstat;
      sMinuit->mnstat(amin,edm,errdef,nvpar,nparx,icstat);
      matrixStatus = icstat;

      /*cout <<"amin:"<< amin<<endl;
        cout <<"icstat:"<< icstat<<endl;
        cout <<"edm:"<<edm<<endl;
        cout <<"errdef:"<<errdef<<endl;
        cout <<"nvpar:"<<nvpar<<endl;
        cout <<"nparx:"<<nparx<<endl;*/
      //sMinuit->mnprin(0,amin);
      if (amin>0) chi2=amin;
      for (int i=0;i<nparam_single;i++){
         sMinuit->GetParameter(i,coeff[i],e);
      }
      //if (amin<1)break;
      //cout << "ite="<<ite<<", chi2="<<chi2<<"mom="<<coeff[0]<<endl;
      //cout << "xmin="<<xMin_[0]<<", "<<xMin_[1]<<", "<<xMin_[2]<<endl;
      //cout << "ymin="<<yMin_[0]<<", "<<yMin_[1]<<", "<<yMin_[2]<<endl;
      //cout << "zmin="<<zMin_[0]<<", "<<zMin_[1]<<", "<<zMin_[2]<<endl;
      ite++;
   }
   sMinuit->mncler();
   //added------------
   double vtxx=coeff[1];
   double vtxy=coeff[2];
   double r0[3]={vtxx, vtxy, 0.};
   //-----------------
   double theta=coeff[3];
   double phi=coeff[4];
   //Coordinates of momentum direction are...--------//   
   //x: cos(phi)*sin(theta);
   //y: sin(phi);
   //z: cos(phi)*cos(theta); 
   //--------------------------//


   //double r0[3]={r_ini*cos(phi)*sin(theta),r_ini*sin(phi),r_ini*cos(phi)*cos(theta)};
   //double r0[3]={vtxx, vtxy, 0.};
   double dr[3];
   //if (fabs(theta)<M_PI/2.)dr[2]=-1/sqrt(coeff[1]*coeff[1]+coeff[2]*coeff[2]+1);
   //else dr[2]=1/sqrt(coeff[1]*coeff[1]+coeff[2]*coeff[2]+1);
   //dr[0]=coeff[1]*dr[2];
   //dr[1]=coeff[2]*dr[2];
   //cout <<"final:"<<coeff[0]<<" "<<coeff[1]<<" "<<coeff[2]<<" "<<coeff[3]<<" "<<coeff[4]<<endl;
   
   dr[0]=cos(phi)*sin(theta);
   dr[1]=sin(phi);
   dr[2]=cos(phi)*cos(theta); 

   //StepBackRKN(0.01,10000,-coeff[0],r0,dr);//StepBackRKN
   
   rvtx=TVector3(r0[0],r0[1],r0[2]);//StepBackRKN
   //pvtx=TVector3(-fabs(coeff[0])*dr[0],-fabs(coeff[0])*dr[1],-fabs(coeff[0])*dr[2]);//StepBackRKN
   pvtx=TVector3(fabs(coeff[0])*dr[0],fabs(coeff[0])*dr[1],abs(coeff[0])*dr[2]);
   //cout <<"number of fmf call="<<ncallfmf_<<endl;
   return (double)chi2;
}

void E16ANA_SingleTrack::StepBackRKN(double dstp, int nstp, 
      double mom, double r0[], double dr0[])
{
  //   double rout[3],drout[3],d=0.;
   double rout[3],drout[3];
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

double E16ANA_SingleTrack::StepRKN(double dstp, 
      double mom, double r0[], double dr0[])
{
   double s=1.;
   //double rout[3],drout[3],d=0.,dz,dz_pre;
   double rout[3],drout[3];
   //double dmin=1000.,dzmin=1000;
   double xmin[3]={1000,1000,1000};
   double ymin[3]={1000,1000,1000};
   double zmin[3]={1000,1000,1000};

   double xerr[3]={0.8e-2,0.8e-2,0.8e-2};   
   double yerr[3]={5e-2,5e-2,5e-2};   
   double zerr[3]={5e-3,5e-3,5e-3};  

   //   double lPos_trk[3][3];
   int nstp_min[3],iGEM=0,istp=0,nGEM;

   //double yscal[3],y[3],dydx[3],eps_rel=1e-2,hdid,hnext;
   double dminsum=0.;
   //const double hmin=1e-3;
   //cout <<"mom="<<mom<<endl;

   //double gid[3];
   //Getid(gid);
   //cerr<<"L290 GTRid "<<gid[0]<<"\t"<<gid[1]<<"\t"<<gid[2]<<endl;

   TVector3 gPos_min[3];
   ROOT::Math::Plane3D GEMplane[6];
   for (int i=0;i<3;i++)GEMplane[i]=GetPlane(i);

   //cout<<"in StepRKN r0 "<<r0[0]<<"\t"<<r0[1]<<"\t"<<r0[2]<<endl;

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

      bool IsCrossed = ( dist1 > 0 ) ? true : false;

      for (int i=0;i<3;i++){
         r0[i]=rout[i];         dr0[i]=drout[i];
      }

      //cout << "step size="<<dstp<<" "<<hnext<<endl;
      //cout <<"rout="<<r0[0]<<" "<<r0[1]<<" "<<r0[2]<<"\t"<<"IsCrossed "<<IsCrossed<<"\t"<<"answer "<<GTRgPos[iGEM][0]<<"\t"<<GTRgPos[iGEM][1]<<"\t"<<GTRgPos[iGEM][2]<<endl;
      //double theta=27.315*(4-int(GTRid[iGEM]/3));
      //double theta=26.06*(4-int(GTRid[iGEM]/3));

      double theta=geom_->GTRtheta[GTRid[iGEM]];
      double phiR=0.;
      TVector3 gPos;//step-by-step
      TVector3 lPos;//step-by-step
      TVector3 lMom;//step-by-step

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
	lPos = E16ANA_Transform::GetLPos(gPos,geom_,iGEM,GTRid[iGEM]);

         xmin[iGEM]=lPos.X()-GTRlPos[iGEM][0];
         ymin[iGEM]=lPos.Y()-GTRlPos[iGEM][1];
         zmin[iGEM]=lPos.Z()-GTRlPos[iGEM][2];
     
	 fitLPos[iGEM] = lPos;
	 fitGPos[iGEM] = gPos;

	 TVector3 gMom(fabs(mom)*dr0[0],fabs(mom)*dr0[1],fabs(mom)*dr0[2]);
         lMom=E16ANA_Transform::GetLMom(gMom,theta,0.,0.,phiR);

	 fitLMom[iGEM] = lMom;
	 fitGMom[iGEM] = gMom;


         gPos_min[iGEM]=gPos;
         iGEM+=1;
      }
      else if (sqrt(r0[0]*r0[0]+r0[2]*r0[2])>70.||
            fabs(r0[1])>60. || istp>2000){

         iGEM=nGEM;
         dminsum=100000;
	 crossflag =1;
         break; 
      }
      else{
	crossflag =2;
      }
      s+=dstp;
      istp++;
   }//while

   ncallfmf_+=istp*1e-6;

   if( dminsum == 1000000) {return dminsum;}

   for (int j=0;j<3;j++){
     //cout << "dminsum calc iGEM="<<j<<":"<<xmin[j]<<" "<<ymin[j]<<" "<<zmin[j]<<endl;
     //      SetlPos_trk(lPos_trk[0],lPos_trk[1],lPos_trk[2]);
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
   return dminsum;
}


/*bool E16ANA_SingleTrack::AccurateAdvance(double ystart[],double dydxstart[],
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

void E16ANA_SingleTrack::OneGoodStep(double mom, 
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
   //double dyout[3];

   h = htry ; // Set stepsize to the initial trial value

   //double inv_eps_vel_sq = 1.0 / (eps_rel_max*eps_rel_max);

   double errpos_sq=0.0;    // square of displacement error
   //double errvel_sq=0.0;    // square of momentum vector difference
   //double errspin_sq=0.0;   // square of spin vector difference

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

void E16ANA_SingleTrack::rkn4Stepper( const double mom, 
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
void E16ANA_SingleTrack::rkn4(double mom, 
      double y[], double dydx[], double h, 
      double yout[], double dydxout[])
{
  //double h2,h6,h28,dym[3],dyt[3],yt[3];
double h2,h6,h28,dyt[3],yt[3];
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

void E16ANA_SingleTrack::EOM(double mom, double r[], double dr[], double d2r[]) 
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


void E16ANA_SingleTrack::SetMom0(TVector3 vA, TVector3 vB, TVector3 vC)
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
   /*cout <<"x0="<<r_ini*cos(phi0_)*sin(theta0_)
     <<", y0="<<r_ini*sin(phi0_)
     <<", z0="<<r_ini*cos(phi0_)*cos(theta0_)<<endl;*/
}



ROOT::Math::Plane3D E16ANA_SingleTrack::GetPlane(int iGEM){
#if 0
   double dx[3]={10.,20.,30.};
   double dy[3]={10.,20.,30.};
   double dz[3]={10e-4,10e-4,10e-4};
#endif
   //double theta=27.315*(4-int(GTRid[iGEM]/3));
   //double theta=26.06*(4-int(GTRid[iGEM]/3));
   //double theta=geom_->GTRtheta[GTRid[iGEM]];
   //   double phiR=0.;
   //   double x,y,z,r;
   ROOT::Math::Plane3D plane;
   ROOT::Math::Plane3D::Vector normal;
   ROOT::Math::Plane3D::Point origin;
   //origin.SetXYZ(
   //      geom_->GTRdx[iGEM][GTRid[iGEM]],
   //      geom_->GTRy[iGEM][GTRid[iGEM]]+geom_->GTRdy[iGEM][GTRid[iGEM]],
   //      geom_->GTRz[iGEM][GTRid[iGEM]]+geom_->GTRdz[iGEM][GTRid[iGEM]]
   //      );
   //origin = ROOT::Math::RotationY(theta/180.0*M_PI)*origin;
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

void E16ANA_SingleTrack::SetMinuit(TMinuit* min)
{
   sMinuit=min;
} 
