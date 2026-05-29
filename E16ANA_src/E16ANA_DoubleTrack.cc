//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai
//2015-12-31, uploaded by yokkaich
//2015-12-21, uploaded by yokkaich
//2015-11-02, uploaded by yokkaich
//2015-08-20, uploaded by yokkaich
//2015-08-10, uploaded by nakai
//2015-08-03, uploaded by yokkaich
//2015-07-15, uploaded by nakai
//2015-05-29, uploaded by yokkaich
//2015-03-27, uploaded by yokkaich
//2015-01-05, modified by yokkaich
//2015-01-05, modified by yokkaich
//2014-08-27, modified by kawama
//2014-05-07, modified by kawama
//2014-04-30, modified by kawama
//2014-04-24, modified by kawama
//2013-11-14, modified by kawama
//2013-05-13, modified by kawama
//
//E16ANA_DoubleTrack.cc 
//
#include <TMinuit.h>
#include <TVector3.h>
#include <G4ThreeVector.hh>
#include <Math/Minimizer.h>
#include <Math/Factory.h>

#include <Math/Plane3D.h>
#include <Math/Rotation3D.h>
#include <Math/RotationX.h>
#include <Math/RotationY.h>
#include <Math/RotationZ.h>
#include <TGeoBBox.h>
#include <math.h>

#include "E16ANA_Transform.hh"
#include "E16ANA_DoubleTrack.hh"

using namespace std;

TMinuit* dMinuit=new TMinuit(E16ANA_DoubleTrack::NParam() );

void trkfcnDouble(int &npar, double *gin, double &f, double* par, int iflag){
   double r0[3];
   double r1[3];
   double dr0[3];
   double dr1[3];
   double px0=par[0];
   double py0=par[1];
   double pz0=par[2];
   double px1=par[3];
   double py1=par[4];
   double pz1=par[5];
   r0[0]=par[6];
   r0[1]=par[7];
   r0[2]=par[8];
   r1[0]=par[6];
   r1[1]=par[7];
   r1[2]=par[8];
   double stp=1.e-1;
   double mom0=sqrt(px0*px0+py0*py0+pz0*pz0);
   double mom1=sqrt(px1*px1+py1*py1+pz1*pz1);
   dr0[0]=px0/mom0;
   dr0[1]=py0/mom0;
   dr0[2]=pz0/mom0;
   dr1[0]=px1/mom1;
   dr1[1]=py1/mom1;
   dr1[2]=pz1/mom1;
   //cout << "x0="<<r0[0]<<", y0="<<r0[1]<<", z0="<<r0[2]<<endl;
   //cout << "x1="<<r1[0]<<", y1="<<r1[1]<<", z1="<<r1[2]<<endl;
   //cout << "px0="<<px0<<", py0="<<py0<<", pz0="<<pz0<<endl;
   //cout << "px1="<<px1<<", py1="<<py1<<", pz1="<<pz1<<endl;
   E16ANA_DoubleTrack *trk=(E16ANA_DoubleTrack*)dMinuit->GetObjectFit();
   double xvtxerr=trk->GetXVTXErr();
   double yvtxerr=trk->GetYVTXErr();
   //double zvtxerr=trk->GetZVTXErr();
   TVector3 initVertex = trk->GetInitvtx();

   double dx = ( par[6]-initVertex.X() ) / xvtxerr;
   double dy = ( par[7]-initVertex.Y() ) / yvtxerr;
   //double dz = ( par[8]-initVertex.Z() ) / zvtxerr;
   double dxvtx2= dx*dx;
   double dyvtx2= dy*dy;
   //double dzvtx2= dz*dz;

   double dmin0=trk->StepRKN(0,stp,+mom0,r0,dr0);
   double dmin1=trk->StepRKN(1,stp,-mom1,r1,dr1);

   f=(dmin0+dmin1+dxvtx2+dyvtx2);//normarized, cm*cm
   
   //   cout << "dmin0="<<dmin0<<endl;
   //   cout << "dmin1="<<dmin1<<endl;
   //cout << "dvtx="<<dvtx<<endl;
   //cout << "vtx="<<par[6]<<", "<<par[7]<<", "<<par[8]<<endl;
   //cout << "dvtx="<<dxvtx2<<", "<<dyvtx2<<", "<<dzvtx2<<endl;
   //   cout << "f="<<f<<endl;
   //now neglect dof for z-coordinates of GTRa nd VTX
   //GTR(x,y)*6+VTX(x,y)=14-8-1=5 
   //cout << "Start Stepper"<<endl;
}

E16ANA_DoubleTrack::E16ANA_DoubleTrack(  E16ANA_MagneticFieldMap* fmf,
				       const E16ANA_GeometryV1 *geom,
      double xvtxerr, double yvtxerr, double zvtxerr)
  :fmf_(fmf), geom_(geom),
   xvtxerr_(xvtxerr), yvtxerr_(yvtxerr), zvtxerr_(zvtxerr){

      ncallfmf_=0;
      crossflag[0]=crossflag[1]=90;
}

E16ANA_DoubleTrack::~E16ANA_DoubleTrack()
{
}

double E16ANA_DoubleTrack::Fit( bool vertexfixflag )
{
  //double ds=0.;
   double arglist[10],chi2=1000.;
   int ierflg=0;
   dMinuit->SetFCN(trkfcnDouble);
   arglist[0]=1;
   //dMinuit->mnexcm("SET ERR", arglist, 1, ierflg);
   dMinuit->SetPrintLevel(-1);
   arglist[0]=0;
   dMinuit->mnexcm("SET STRA 2", arglist, 0, ierflg);
   int ite=0;
   //while(chi2>1 && ite<50){
      while(ite<1){
      //cout << "Iteration"<<ite<<" Start"<<endl;
      double Step[nparam_double];
      double LLim[nparam_double];
      double ULim[nparam_double];
      if (ite==0){
         coeff[0]=p0_vtx.X();
         coeff[1]=p0_vtx.Y();
         coeff[2]=p0_vtx.Z();
         coeff[3]=p1_vtx.X();
         coeff[4]=p1_vtx.Y();
         coeff[5]=p1_vtx.Z();
         /*if(fabs(r_vtx.X())<.5)coeff[6]=r_vtx.X();
         else coeff[6]=0.001;
         if(fabs(r_vtx.Y())<.5)coeff[7]=r_vtx.Y();
         else coeff[7]=0.001;*/

         coeff[6]=r_vtx.X();
	 coeff[7]=r_vtx.Y();

         /*if(r_vtx.Z()>0.5){
            coeff[8]=1.;
            LLim[8]=0.5;
            ULim[8]=1.5;
         }*/
         if(r_vtx.Z()>1.0){
            coeff[8]=2.;
            LLim[8]=1.5;
            ULim[8]=2.5;
         }
         else if(r_vtx.Z()>-1.){
            coeff[8]=0.;
            LLim[8]=-0.5;
            ULim[8]=0.5;
         }
         else {
            coeff[8]=-2.;
            LLim[8]=-2.5;
            ULim[8]=-1.5;
         }
      }
      Step[0]=0.01;
      LLim[0]=-1.5;
      ULim[0]=1.5;

      Step[1]=0.01;
      LLim[1]=-1.5;
      ULim[1]=1.5;

      Step[2]=0.01;
      LLim[2]=-0.3;
      ULim[2]=6.0;

      Step[3]=0.01;
      LLim[3]=-1.5;
      ULim[3]=1.5;

      Step[4]=0.01;
      LLim[4]=-1.5;
      ULim[4]=1.5;

      Step[5]=0.01;
      LLim[5]=-0.3;
      ULim[5]=5.0;

      if( vertexfixflag == false ){
	Step[6]=0.01;//x
	Step[7]=0.01;//y
      }
      else{//fix
	Step[6]=0.0;//x
	Step[7]=0.0;//y
      }

      LLim[6]=-2.0;
      ULim[6]=2.0;
      LLim[7]=-2.0;
      ULim[7]=2.0;

      //Step[8]=0.1;
      Step[8]=0.0;//z

      for (int i=0;i<nparam_double;i++){
         char pName[10];
         //cout << "coeff"<<i<<" "<<coeff[i]<<endl;
         //cout << "Step"<<i<<" "<<Step[i]<<endl;
         sprintf(pName,"p%d",i);
         dMinuit->mnparm(i,pName,coeff[i],Step[i],LLim[i],ULim[i],ierflg);
      }
      arglist[0]=1e4;
      arglist[1]=1e-5;



      dMinuit->mnexcm("MIGRAD", arglist, 1, ierflg);
      //dMinuit->mnexcm("SIMPLEX", arglist, 2, ierflg);
      //if(ite==0)dMinuit->mnexcm("HESSE", arglist, 2, ierflg);
      //else dMinuit->mnexcm("MINIMUM", arglist, 2, ierflg);
      //dMinuit->mnexcm("MINIMUM", arglist, 2, ierflg);

      cout <<"Minuit Status:"<<dMinuit->GetStatus()<<" "<<ierflg<<endl;
      minimizeStatus = ierflg;

      //dMinuit->mnexcm("IMPROVE", arglist, 1, ierflg);

      double amin,edm,errdef,e;
      int nvpar,nparx,icstat;
      dMinuit->mnstat(amin,edm,errdef,nvpar,nparx,icstat);
      //cout <<"amin:"<< amin<<endl;
      //cout <<"icstat:"<< icstat<<endl;
      matrixStatus = icstat;

      dMinuit->mnprin(0,amin);
      if (amin>0) {chi2=amin;}
      for (int i=0;i<nparam_double;i++){
         dMinuit->GetParameter(i,coeff[i],e);
      }
      //if (amin<1)break;
      //cout << "ite="<<ite<<", chi2="<<chi2<<"mom="<<coeff[0]<<endl;
      //cout << "xmin="<<xMin_[0]<<", "<<xMin_[1]<<", "<<xMin_[2]<<endl;
      //cout << "ymin="<<yMin_[0]<<", "<<yMin_[1]<<", "<<yMin_[2]<<endl;
      //cout << "zmin="<<zMin_[0]<<", "<<zMin_[1]<<", "<<zMin_[2]<<endl;
      ite++;
      }
      //dMinuit->mncler();
      //arglist[0]=0;
      //dMinuit->mnexcm("RETURN", arglist, 0, ierflg);
      /*cout <<"final:"<<coeff[0]<<" "<<coeff[1]<<" "<<coeff[2]<<" "
         <<coeff[3]<<" "<<coeff[4]<<" "<<coeff[5]<<" "
         <<coeff[6]<<" "<<coeff[7]<<" "<<coeff[8]<<" "
         <<endl;*/
      p0_vtx=TVector3(coeff[0],coeff[1],coeff[2]);
      p1_vtx=TVector3(coeff[3],coeff[4],coeff[5]);
      r_vtx=TVector3(coeff[6],coeff[7],coeff[8]);
      //cout <<"number of fmf call="<<ncallfmf_<<endl;

      //      cerr<<"crossflagfin "<<crossflag[0]<<" "<<crossflag[1]<<endl;
      return (double)chi2;

}

void E16ANA_DoubleTrack::StepBackRKN(double dstp, int nstp, 
      double mom, double r0[], double dr0[])
{
  double rout[3],drout[3];
   for (int i=0;i<nstp;i++){
      rkn4(mom, r0, dr0, dstp, rout, drout);
      if (r0[2]<0)break;
      for (int j=0;j<3;j++){
         r0[j]=rout[j];
         dr0[j]=drout[j];
      }
      //cout << i<<" "<<r0[0]<<" "<<r0[1]<<" "<<r0[2]<<endl;
      //cout << i<<" "<<dr0[0]<<" "<<dr0[1]<<" "<<dr0[2]<<endl;
   }
}

double E16ANA_DoubleTrack::StepRKN(int flag, double dstp, 
      double mom, double r0[], double dr0[])
{
   double s=1.,Bdl=0;
   double rout[3],drout[3],B[3];
   //double dmin=1000.,dzmin=1000,d=0, dz,dz_pre;
   double xmin[6]={1000,1000,1000,1000,1000,1000};
   double ymin[6]={1000,1000,1000,1000,1000,1000};
   double zmin[6]={1000,1000,1000,1000,1000,1000};


   int nstp_min[6],iGEM,nGEM,istp=0;
   //   double xerr[3]={80e-4,80e-4,80e-4};         //80um
   //   double yerr[3]={500e-4,500e-4,500e-4};      //500um
   
   //   double zerr[3]={25e-3,25e-3,25e-3};        //250um -> should be 25um??
   //       Z is anyway not used now

   double dminsum=0.;
   if(flag==0){
      iGEM=0;nGEM=3;
   }
   else{
      iGEM=3;nGEM=6;
   }

   //double yscal[3],y[3],dydx[3],eps_rel=1e-2,hdid,hnext;

   TVector3 gPos_min[6];
   ROOT::Math::Plane3D GEMplane[6];
   for (int i=0;i<6;i++){GEMplane[i]=GetPlane(i);}
   //TGeoBBox GEMplane;
   //cout <<"mom="<<mom<<endl;
   //cout <<"flag="<<flag<<endl;
   //cout <<"GTRid="<<GTRid[iGEM]<<","<<GTRid[iGEM+1]
   // <<","<<GTRid[iGEM+2]<<endl;
   StepLength_=0.;

   while (iGEM<nGEM){
      //cout << "GTRlPos="<<GTRlPos[iGEM][0]<<" "<<GTRlPos[iGEM][1] 
      //<<" "<<GTRlPos[iGEM][2] <<endl;
      //cout<<"istp="<<istp<<endl;
      //for (int j=0;j<3;j++) yscal[i]=fabs(r0[i])+fabs(dr0[i]*dstp);
      //cout <<"r0="<<r0[0]<<" "<<r0[1]<<" "<<r0[2]<<endl;
      //cout <<"dr0="<<dr0[0]<<" "<<dr0[1]<<" "<<dr0[2]<<endl;
      //const double *org=GEMplane.GetOrigin();
      //cout << org[0]<<" "<<org[1]<<" "<<org[2]<<endl;
      rkn4(mom, r0, dr0, dstp, rout, drout);
      StepLength_+=dstp;
      fmf_->GetFieldValue(rout, B, 0);
   /*B[0]=0;
   B[1]=15000;
   B[2]=0;*/
      Bdl+=B[1]*dstp;
      //OneGoodStep(mom, r0, dr0,s,dstp, eps_rel,hdid, hnext );
      //double dir[3]={rout[0]-r0[0],rout[1]-r0[1],rout[2]-r0[2]};
      ROOT::Math::Plane3D::Point p0(r0[0],r0[1],r0[2]);
      ROOT::Math::Plane3D::Point p1(rout[0],rout[1],rout[2]);
      ROOT::Math::Plane3D::Point crossp;
      //cout <<"iGEM="<<iGEM<<",Is Crossed?:"<<GEMplane.CouldBeCrossed(r0,dir)<<endl;
      double dist0=GEMplane[iGEM].Distance(p0);
      double dist1=GEMplane[iGEM].Distance(p1);

      bool IsCrossed = ( dist1 > 0 ) ? true : false;

      for (int i=0;i<3;i++){
         r0[i]=rout[i];         dr0[i]=drout[i];
      }

      //cout << "step size="<<dstp<<endl;
      //cout <<"rout="<<r0[0]<<" "<<r0[1]<<" "<<r0[2]<<endl;
      //cout <<"B="<<B[0]<<" "<<B[1]<<" "<<B[2]<<endl;
      //double theta=27.315*(4-int(GTRid[iGEM]/3));
      //double theta=26.06*(4-int(GTRid[iGEM]/3));

      double theta=geom_->GTRtheta[GTRid[iGEM]];
      double phiR=0.;
      TVector3 gPos;//step-by-step
      TVector3 lPos;//step-by-step
      TVector3 lMom;//step-by-step

      /****Debug*/
        /*cout <<"iGEM,lPos.Z():"<<" "<<iGEM<<" "<<lPos.Z()<<endl;
        cout << "GTRgPos="<<GTRgPos[iGEM][0]<<" "<<GTRgPos[iGEM][1] 
        <<" "<<GTRgPos[iGEM][2] <<endl;
        cout << "GTRlPos="<<GTRlPos[iGEM][0]<<" "<<GTRlPos[iGEM][1] 
        <<" "<<GTRlPos[iGEM][2] <<endl;
        TVector3 gPostest(GTRgPos[iGEM][0],GTRgPos[iGEM][1],GTRgPos[iGEM][2]);
        TVector3 lPostest=Transform.GetLPos(gPostest,GTRid[iGEM],iGEM);
        cout << "lPostest="<<lPostest.X()<<" "<<lPostest.Y()
        <<" "<<lPostest.Z() <<endl;*/
       /***/
      if(IsCrossed){
         //E16ANA_Transform Transform;
         nstp_min[iGEM]=istp;
         crossp=p0+(p1-p0)*fabs(dist0)/(fabs(dist0)+fabs(dist1));
         GTRStepLength[iGEM]=StepLength_;
      //cout <<"iGEM="<<iGEM<<"Distance pre:"<<dist0<<" post:"<<dist1<<endl;
      //cout <<"p0="<<p0<<"p1="<<p1<<endl;
      //cout <<"corssp="<<crossp<<endl;

         gPos.SetXYZ(crossp.X(),crossp.Y(),crossp.Z());
         if(flag==0){
            lPos = E16ANA_Transform::GetLPos(gPos, geom_, iGEM, GTRid[iGEM]);
         }
         else{
            lPos = E16ANA_Transform::GetLPos(gPos, geom_, iGEM-3, GTRid[iGEM]);
	 }
         xmin[iGEM]=lPos.X()-GTRlPos[iGEM][0];
         ymin[iGEM]=lPos.Y()-GTRlPos[iGEM][1];
         zmin[iGEM]=lPos.Z()-GTRlPos[iGEM][2];

	 fitLPos[iGEM] = lPos;
	 fitGPos[iGEM] = gPos;

         TVector3 gMom(fabs(mom)*dr0[0],fabs(mom)*dr0[1],fabs(mom)*dr0[2]);
         lMom=E16ANA_Transform::GetLMom(gMom,theta,0.,0.,phiR);

	 fitLMom[iGEM] = lMom;
	 fitGMom[iGEM] = gMom;

	 //         cout <<"xmin,ymin,zmin:"<<xmin[iGEM]<<" "<<ymin[iGEM]<<" "<<zmin[iGEM]<<endl;
         gPos_min[iGEM]=gPos;
         iGEM+=1;
      }
      //dstp=3e-3;
      //}
      /*else if (dist>-0.2){
        dstp=fabs(dist)/2.;
        }*/
      else if (sqrt(r0[0]*r0[0]+r0[2]*r0[2])>70.||
            fabs(r0[1])>60. || istp>2000){

         iGEM=nGEM;
         dminsum=100000;
	 crossflag[flag] =1;
         break;
      }
      else {
	crossflag[flag] =2;
      }
      s += dstp;
      istp++;
   }//while

   if(flag == 0){Bdl0=Bdl;} else {Bdl1=Bdl;}

   ncallfmf_+=istp*1e-6;

   //cout << "iGEM=0:"<<gPos_min[0].X()<<" "<<gPos_min[0].Y()<<" "<<gPos_min[0].Z()<<endl;
   //cout << "iGEM=1:"<< gPos_min[1].X()<<" "<<gPos_min[1].Y()<<" "<<gPos_min[1].Z()<<endl;
   //cout << "iGEM=2:"<< gPos_min[2].X()<<" "<<gPos_min[2].Y()<<" "<<gPos_min[2].Z()<<endl;
 if( dminsum == 1000000) {return dminsum;}

 for (int j=0; j<3; j++){
   int j3 = (flag == 0) ? j : j+3 ;

   xmin[j3]=xmin[j3]/(xerr[j]);
   //   zmin[j3]=zmin[j3]/(zerr[j]);
   ymin[j3]=ymin[j3]/(yerr[j]);
   dminsum += xmin[j3]*xmin[j3] + ymin[j3]*ymin[j3];
   xMin_[j]=xmin[j3];
   yMin_[j]=ymin[j3];
   zMin_[j]=zmin[j3];

 }



#if 0 //yok dump
 if( dminsum > 1){
     if( dminsum > 10001){
       E16INFO("RKN large %8.1f",dminsum);
     }
     else{
	 E16INFO("RKN small %8.1f",dminsum);
     }
     int offset = flag*3;
     for(int i=offset; i< 3+offset ; i++){
       cerr<<"   fit/data["<<i<<"] "<<xmin[i]<<" "<<ymin[i]<<" : ";
	 cerr<< fitLPos[i].X() << " "<< fitLPos[i].Y() ;
       cerr<<" "<<fitLPos[i].Z()<<" / " ;
       cerr<<GTRlPos[i][0] <<" "<< GTRlPos[i][1] <<" ";
       cerr<< GTRlPos[i][2] <<endl;

       cerr<<" g fit/data["<<i<<"] "<<xmin[i]<<" "<<ymin[i]<<" : ";
	 cerr<< fitGPos[i].X() << " "<< fitGPos[i].Y() ;
       cerr<<" "<<fitGPos[i].Z()<<" / " ;
       cerr<<GTRgPos[i][0] <<" "<< GTRgPos[i][1] <<" ";
       cerr<< GTRgPos[i][2] <<endl;

     }
   }
#endif

   return dminsum;

}


/*bool E16ANA_DoubleTrack::AccurateAdvance(double ystart[],float dydxstart[],
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
         //          G4cout << "  Total steps:  bad " << fNoBadSteps
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
//    G4cout << "G4MagIntegratorDriver exit status: no-steps " << nstp <<G4endl;
//    PrintStatus( yEnd, x1, y, x, hstep, nstp);
//  }
//#endif

return succeeded;
}  // end of AccurateAdvance ...........................
*/

/*A 4th order Runge-Kutta program.*/
void E16ANA_DoubleTrack::rk4(double mom, 
      double y[], double dydx[], double h, 
      double yout[])
{
   double h2,h6,h28,dym[6],dyt[6],yt[6];

   //cout << y[0] <<" "<<y[1]<<" "<<y[2]<<endl;
   h2=h/2.;
   h6=h/6.;
   h28=h*h/8;
   for (int i=0;i<6;i++){
      yt[i]=y[i]+h2*dydx[i];
   }
   EOM(mom,yt,dyt);
   for (int i=0;i<6;i++) {
      yt[i]=y[i]+h2*dyt[i];
   }
   EOM(mom,yt,dym);
   for (int i=0;i<6;i++){
      yt[i]=y[i]+h*dym[i];
      dym[i]+=dyt[i];
   }
   EOM(mom,yt,dyt);
   for (int i=0;i<6;i++){
      yout[i]=y[i]+h6*(dydx[i]+dyt[i]+2*dym[i]);
   }
}

void E16ANA_DoubleTrack::EOM(double mom, double r[],double dr[]) 
{
   int jj[3]={1,2,0};
   int kk[3]={2,0,1};
   double B[3];
   double point[3];
   for (int i=0;i<3;i++) point[i]=r[i];
   fmf_->GetFieldValue(point, B, 0);
   /*B[0]=0;
   B[1]=15000;
   B[2]=0;*/
   //cout <<"dr="<< dr[0]<<" "<<dr[1]<<" "<<dr[2]<<endl;
   //cout <<"r="<< r[0]<<" "<<r[1]<<" "<<r[2]<<endl;
   //cout <<"B="<< B[0]<<" "<<B[1]<<" "<<B[2]<<endl;

   for (int i=0;i<3;i++){
      dr[i]=r[i+3];
   }
   for (int i=3;i<6;i++){
      dr[i]=0.299792458e-6/mom*
         (r[jj[i-3]+3]*B[kk[i-3]]-r[kk[i-3]+3]*B[jj[i-3]]);
   }
}

/*A 4th order Runge-Kutta program.*/
void E16ANA_DoubleTrack::rkn4(double mom, 
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
   //for (int i=0;i<3;i++) dyt[i]=dydx[i]+h2*k2[i];
   for (int i=0;i<3;i++) {
      yt[i]=y[i]+h2*dydx[i]+h28*k1[i];
      dyt[i]=dydx[i]+h2*k2[i];
   }
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

void E16ANA_DoubleTrack::EOM(double mom, double r[], 
      double dr[], double d2r[]) 
{
   int jj[3]={1,2,0};
   int kk[3]={2,0,1};
   double B[3];
   double point[3];
   for (int i=0;i<3;i++) point[i]=r[i];
   fmf_->GetFieldValue(point, B, 0);
   /*B[0]=0;
   B[1]=15000;
   B[2]=0;*/
   //cout <<"dr="<< dr[0]<<" "<<dr[1]<<" "<<dr[2]<<endl;
   //cout <<"r="<< r[0]<<" "<<r[1]<<" "<<r[2]<<endl;
   //cout <<"B="<< B[0]<<" "<<B[1]<<" "<<B[2]<<endl;

   for (int i=0;i<3;i++){
      d2r[i]=0.299792458e-6/mom*(dr[jj[i]]*B[kk[i]]-dr[kk[i]]*B[jj[i]]);
      //d2r[i]=c*1e-2/(mom*1e9)*(dr[jj[i]]*B[kk[i]]*1e-4-dr[kk[i]]*B[jj[i]]*1e-4);
   }
}

ROOT::Math::Plane3D E16ANA_DoubleTrack::GetPlane(int iGEM){
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
   if(iGEM<3){
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
   }
   else{
      //origin.SetXYZ(
      //      geom_->GTRdx[iGEM-3][GTRid[iGEM]],
      //      geom_->GTRy[iGEM-3][GTRid[iGEM]]+geom_->GTRdy[iGEM-3][GTRid[iGEM]],
      //      geom_->GTRz[iGEM-3][GTRid[iGEM]]+geom_->GTRdz[iGEM-3][GTRid[iGEM]]
      //      );
      //origin = ROOT::Math::RotationY(theta/180.0*M_PI)*origin;
      origin = E16ANA_Transform::GetGPos(TVector3(0.0,0.0,0.0),geom_,iGEM-3,GTRid[iGEM]);
      normal.SetXYZ(0.0,0.0,1.0);
      normal = ROOT::Math::RotationX(geom_->GTRrotx[iGEM-3][GTRid[iGEM]]/180.0*M_PI)*normal;
      normal = ROOT::Math::RotationY(geom_->GTRroty[iGEM-3][GTRid[iGEM]]/180.0*M_PI)*normal;
      normal = ROOT::Math::RotationZ(geom_->GTRrotz[iGEM-3][GTRid[iGEM]]/180.0*M_PI)*normal;
      normal = ROOT::Math::RotationX(geom_->GTRrotx_frame[GTRid[iGEM]]/180.0*M_PI)*normal;
      normal = ROOT::Math::RotationY(geom_->GTRroty_frame[GTRid[iGEM]]/180.0*M_PI)*normal;
      normal = ROOT::Math::RotationZ(geom_->GTRrotz_frame[GTRid[iGEM]]/180.0*M_PI)*normal;
      normal = ROOT::Math::RotationY(geom_->GTRtheta[GTRid[iGEM]]/180.0*M_PI)*normal;
      plane=ROOT::Math::Plane3D::Plane3D(normal,origin);
   }
   return plane;
}

void E16ANA_DoubleTrack::SetMinuit(TMinuit* min)
{
   dMinuit=min;
} 

void E16ANA_DoubleTrack::Initialize(TVector3 mom0, TVector3 mom1, 
      TVector3 vtxpos,
      int id0, int id1, int id2, int id3, int id4, int id5){


  int gtrIDs[6]={ id0, id1, id2, id3, id4, id5};

   double errx[3]={80e-4,80e-4,80e-4};         //80um
   double erry[3]={500e-4,500e-4,500e-4};      //500um


   Initialize( mom0, mom1, vtxpos, gtrIDs, errx, erry);

}

void E16ANA_DoubleTrack::Initialize(TVector3 mom0, TVector3 mom1, 
  TVector3 vtxpos,	    int* gtrIDs, double* errX, double* errY){

  initp0_vtx=mom0;
  initp1_vtx=mom1;
  initr_vtx=vtxpos;

  p0_vtx=mom0;
  p1_vtx=mom1;
  r_vtx=vtxpos;
  for( int i=0; i<3 ; i++ ){  xerr[i]=errX[i]; yerr[i]=errY[i];}
  for( int i=0; i<6 ; i++ ){  GTRid[i]  =  gtrIDs[i]; }

}


