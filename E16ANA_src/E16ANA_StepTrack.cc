//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai
//2015-12-03, uploaded by yokkaich
//2015-11-02, uploaded by yokkaich
//2015-11-02, uploaded by komatsu
//2015-03-27, uploaded by yokkaich
//2015-03-01, uploaded by yokkaich
//2015-01-05, modified by yokkaich
//2015-01-05, modified by yokkaich
//2014-08-27, modified by kawama
//2014-05-07, modified by kawama
//2014-04-30, modified by kawama
//2014-04-24, modified by kawama
//2013-11-14, modified by kawama
////E16ANA_StepTrack.cc 130822 by S. Yokkaichi
//    Last modified at <2015-01-05 12:49:28 >
//from
//stepTrack.cc 980421 S.YOK.
//revised 981126 S.YOK.

#include <math.h>
#include <stdlib.h>

#include <iostream>

#include <TVector.h>
#include <TMinuit.h>

#include <CLHEP/Vector/ThreeVector.h>

#include "E16ANA_StepTrack.hh"
#include "E16ANA_MagneticFieldMap.hh"

using namespace std;
using namespace CLHEP;

inline double aint(double a){
  return a>0 ? floor(a) : ceil(a);// "marume" to zero
}

using namespace CLHEP;

//double E16ANA_StepTrack::currentMomentumInverse=0;


void E16ANA_StepTrack::RKN_1stepBockArray(Hep3Vector& iy, Hep3Vector& idy, 
					  double x, 
               double h, Hep3Vector& yResult,Hep3Vector& dyResult
	//	void (E16ANA_StepTrack::*f)(double, double*, double*, double*)
		//,             void (*f)(double, double*, double*, double*)
               ){
 //Bock et al, p276
  //to be implemented using array for speed.

  double y[3]={iy.x(),iy.y(),iy.z()};
  double dy[3]={idy.x(),idy.y(),idy.z()};
  double k=h*0.5;
  //  double kh4=k*h*0.05;
  double kh4=k*h*0.25;
  double h6=h/6.0;
  
  double k1[3],k2[3],k3[3],k4[3];
  double kdy[3]={k*dy[0], k*dy[1], k*dy[2] };

  EquationOfMotion( x, y, dy, k1);
  //  (*f)(   x,    y,     dy, k1);

  double y1[3]={  y[0]+kdy[0]+kh4*k1[0],
                  y[1]+kdy[1]+kh4*k1[1],
                  y[2]+kdy[2]+kh4*k1[2] };
  double dy1[3]={ dy[0]+k*k1[0],
                  dy[1]+k*k1[1],
                  dy[2]+k*k1[2] };

  EquationOfMotion( x+k, y1, dy1, k2);
  //  (*f)( x+k, y1, dy1 , k2);

  double y2[3]={  y[0]+kdy[0]+kh4*k2[0],
                  y[1]+kdy[1]+kh4*k2[1],
                  y[2]+kdy[2]+kh4*k2[2] };
  double dy2[3]={ dy[0]+k*k2[0],
                  dy[1]+k*k2[1],
                  dy[2]+k*k2[2] };
  EquationOfMotion( x+k, y2, dy2, k3);
  //  (*f)( x+k, y2, dy2,  k3);

  double y3[3]={  y[0]+h*(dy[0]+k*k3[0]),
                  y[1]+h*(dy[1]+k*k3[1]),
                  y[2]+h*(dy[2]+k*k3[2]) };
  double dy3[3]={ dy[0]+h*k3[0],
                  dy[1]+h*k3[1],
                  dy[2]+h*k3[2] };

  EquationOfMotion( x+k, y3, dy3, k4);
  //  (*f)( x+k, y3, dy3,  k4);

  yResult  =Hep3Vector( y[0] + h*( dy[0] + h6*(k1[0]+k2[0]+k3[0]) ),
                        y[1] + h*( dy[1] + h6*(k1[1]+k2[1]+k3[1]) ) ,
                        y[2] + h*( dy[2] + h6*(k1[2]+k2[2]+k3[2]) )
                        );
                                   
  dyResult =Hep3Vector( dy[0] + h6*(k1[0]+ 2*k2[0] + 2*k3[0] + k4[0]),
                        dy[1] + h6*(k1[1]+ 2*k2[1] + 2*k3[1] + k4[1]),
                        dy[2] + h6*(k1[2]+ 2*k2[2] + 2*k3[2] + k4[2])
                        );
  
}

void E16ANA_StepTrack::EquationOfMotion(
                        double s, double* y, double* dy,double* f){
  //dy is tangent vector, not momentum
  //  double constant=0.299792458; //[(GeV/c)/T/m]
  double constant=0.299792458*1e-6; //[?]  kawama FMField

  constant *= currentMomentumInverse;
  double B[3]={-99999,-99999,-99999};
  double point[3]={y[0],y[1],y[2]};

  fmf_->GetFieldValue(point, B, 0);
   //cout <<"dr="<< dr[0]<<" "<<dr[1]<<" "<<dr[2]<<endl;
   //cout <<"r="<< r[0]<<" "<<r[1]<<" "<<r[2]<<endl;

  //  cerr <<"B="<< B[0]<<" "<<B[1]<<" "<<B[2]<<" at ("<<y[0]<<" "<<y[1]<<" "<<y[2]<<")"<<endl;

  double Bz = B[2], Bx=B[0], By=B[1];//[gauss]by kawama ?

  f[0]=constant*(dy[1]*Bz - dy[2]*By);//d2x/ds2
  f[1]=constant*(dy[2]*Bx - dy[0]*Bz);//d2y/ds2
  f[2]=constant*(dy[0]*By - dy[1]*Bx);//d2z/ds2   [Bock et al,p270]

  //  cerr<<"B="<<Bz<<" "<<Bx<<" "<<By<<endl;
  return;
}

int E16ANA_StepTrack::TraceInMagnet(E16ANA_MagneticFieldMap* ifmf, 
				    const double stepSize,
				    const int nStep,
				    const Hep3Vector& initPoint, 
				    const Hep3Vector& initMom, 
				    const int charge, 
           		    Hep3Vector* points,
                      Hep3Vector* momenta) {
  //trace the particle with Runge-Kutta method

  double momInverse= 1/initMom.mag();
  currentMomentumInverse= charge * momInverse;

  Hep3Vector y  = initPoint;
  Hep3Vector dy = initMom * momInverse;//tangent vector
  Hep3Vector yResult(-1,-1,-1),dyResult(-1,-1,-1);

  double s=0;
  int i;
  for( i=0;  i< nStep ;i++){
    //    cerr<<"i "<<i<<endl;
    points[i]=y;
    momenta[i]  =dy * initMom.mag();

    RKN_1stepBockArray( y,dy, s, stepSize, yResult, dyResult);
    //for next step
    y=yResult;    dy=dyResult;

    //    cerr<<stepSize<<" x "<<nStep<<" "<<initPoint<<" "<<initMom<<" "<<yResult<<" "<<dyResult<<endl;

#if 0
     if( condition->ToBeFinish(y,dy) ){       break;     }
#endif

  }//for i
    //cerr<<stepSize<<" x "<<nStep<<" "<<initPoint<<" "<<initMom<<" "<<yResult<<" "<<dyResult<<endl;


    return i;// max cycle of the trace. 

}


const Hep3Vector E16ANA_StepTrack::errorVector = Hep3Vector(-99999,-99999,-99999);

E16ANA_StepTrack::E16ANA_StepTrack(E16ANA_MagneticFieldMap* ifmf, 
	Hep3Vector ipoint, Hep3Vector imomentum,int icharge,
	  double  istepSize,int iarraySize):
  fmf_(ifmf),
  initPoint(ipoint),initMom(imomentum),charge(icharge),
  stepSize(istepSize),arraySize(iarraySize) {



    pointData1=new Hep3Vector[arraySize];
    pointData2=new Hep3Vector[arraySize];
    momData1=new Hep3Vector[arraySize];
    momData2=new Hep3Vector[arraySize];
    Hep3Vector initMom2 = -initMom;

    const int nStep=arraySize;

#if 1
    int flag1=TraceInMagnet( fmf_, stepSize, nStep, initPoint, initMom,
			     charge,  pointData1, momData1);
    int flag2=TraceInMagnet( fmf_, stepSize, nStep, initPoint, initMom2,
			     -charge, pointData2, momData2);
#endif
    

    for(int i=0;i<arraySize;i++) {
      momData2[i] *= -1;// because initMom2 is inverted.
    }
    if(flag1 == arraySize || flag2 == arraySize ) {
      errorFlag=ArraySizeOver;    }
    else                            {      errorFlag=OK;    }
    currentSize1=flag1;
    currentSize2=flag2;
}

E16ANA_StepTrack::~E16ANA_StepTrack() {

  delete[] pointData1;  delete[] pointData2;
  delete[] momData1;  delete[] momData2;

}
Hep3Vector E16ANA_StepTrack::PointAtR(double r){
//assuming 'step' : if momentum is infinite, track is linear.
  if(r<0){return errorVector;}

  double rDiff=r - initPoint.perp();
  int stepCand = (int) (rDiff/( stepSize* initMom.cosTheta() ) );
  int i;
  Hep3Vector inside,outside,pointCand;
  //  stepCand=-2;//temp
  if(rDiff>0){
    //    for(i=stepCand;i++;i<arraySize){
    for(i=stepCand;i<arraySize;i++){
      Hep3Vector& candPoint=pointData1[i];
      cerr<<" cand:r "<<candPoint.perp()<<endl;
      if( r<candPoint.perp() ){ break; }
    }
    if(i==arraySize){return errorVector;}
    inside=pointData1[i-1];    outside=pointData1[i];
    cerr<<" in/out:r "<<inside.perp()<<" "<<outside.perp()<<endl;
  }
  else{
    //    for(i= -stepCand;i++;i<arraySize){
    for(i= -stepCand;i<arraySize;i++){
      Hep3Vector& candPoint=pointData2[i];
      if( r>candPoint.perp() ){ break;      }
    }
    if(i==arraySize){return errorVector;}
    inside=pointData2[i];	outside=pointData2[i-1];
    cerr<<" in/out:r "<<inside.perp()<<" "<<outside.perp()<<endl;
  }
  double coef=(r-inside.perp())/(outside.perp()-inside.perp());
  pointCand=coef*(outside-inside) + inside;
  cerr<<" AtR "<<i<<" "<<coef<<" "<<inside<<" "<<outside<<" "<<pointCand<<endl;
  return pointCand;
}
Hep3Vector E16ANA_StepTrack::ArrayGet(int    step,
			     Hep3Vector* array1,Hep3Vector* array2) {
  if( step>arraySize ) { return errorVector;}
  if( step >= 0 ) {    return array1[step];  }
  else  {return array2[-step];  }
}

Hep3Vector E16ANA_StepTrack::ArrayGet(double step,
			     Hep3Vector* array1,Hep3Vector* array2) {
  if( step>arraySize ) { return errorVector;}

  Hep3Vector* array;  double step2;// usually >0
  if( step >= 0 ) {    step2=  step ; array=array1;  }
  else            {    step2= -step ; array=array2;  }


  int i    = (int)aint( step2 );// 'marume' to zero.
  double coef=step2-i;

  //  cout<<"aint i/step2/coef "<<i<<" "<<step2<<" "<<coef<<endl;

  Hep3Vector diffV= array[i+1]-array[i] ;
  Hep3Vector point= array[i]+ coef*diffV;
 
  return point;
}
//--------------------------------------------------------------
static E16ANA_StepTrack* crossSt1;
static E16ANA_StepTrack* crossSt2;//for cross calculation.

int E16ANA_StepTrack::Cross(E16ANA_StepTrack& st, double* distance,
		     double* step1, double* step2) {

  crossSt1 = this;
  crossSt2 = & st;

  double param[2],error[2];//parameter answer and error.

#if 1
  double upLimit[2] = { (double)crossSt1->CurrentSize1(), 
			(double)crossSt2->CurrentSize1() };
  double lowLimit[2]= { (double)-(crossSt1->CurrentSize2()),
			(double)-(crossSt2->CurrentSize2()) };
  //parameter lower/upper limit.

  double minuitStepSize[2]    = { 0.1, 0.1};
    //parameter search step used by minuit
    //if track step is 1cm, 0.1 means 1mm.

#else
  double upLimit[2] = { 10,10 };

  double lowLimit[2]= { (double)-(crossSt1->CurrentSize2()),
			(double)-(crossSt2->CurrentSize2()) };

  double minuitStepSize[2]    = { 0.1, 0.1};
#endif


  int flag=MinuitDistance(minuitStepSize, lowLimit, upLimit, 
			  distance, param, error);
  //cerr<<" in cross: distance  "<<flag<<" "<<*distance<<" "<<param[0]<<" "<<param[1]<<endl;

#if 0
  int flag2=MinuitDistanceXZ(minuitStepSize, lowLimit, upLimit, 
			     distance, param, error);

  cerr<<" in cross: distanceXZ  "<<flag2<<" "<<*distance<<" "<<param[0]<<" "<<param[1]<<endl;
#endif

  *step1=param[0];
  *step2=param[1];
  
  return flag;
}

int E16ANA_StepTrack::fcnCalledCount=0;
int E16ANA_StepTrack::XZdistanceFlag=0;

int E16ANA_StepTrack::MinuitDistanceXZ( double* stepSize, double* lowLimit,double* upLimit,   double* distance,double* param,double* error) {

  XZdistanceFlag = 1;
  int flag = MinuitDistance( stepSize, lowLimit, upLimit,  
			     distance, param, error ) ;

  XZdistanceFlag = 0;
  return flag;

}


int E16ANA_StepTrack::MinuitDistance( double* stepSize, double* lowLimit,double* upLimit,   double* distance,double* param,double* error) {

  //itteration with MINUIT  to minimize the distance 

#if 0
  //not static:compile ok but bad work(iflag=0)
  void(*fcn) (const int *npar, double *grad, double *fval, const double *xval,
	      const int *iflag, double (*futil)())  =
(  void(*) (const int *npar, double *grad, double *fval, const double *xval,
	      const int *iflag, double (*futil)())  )
    E16ANA_StepTrack::CalcDistance;

#endif

  //  MNINIT(5,6,7);//fortran I/O
  int flag,istat;
  
   double initValue[2]= { 0.0, 0.0};
  //  double initValue[2]= { 20, -20};//test 
  char* paramName[2] = {"stepParam1","stepParam2"};

  int errorFlag;double argument[10];int nArg;
  //  cerr<<"next minuit start"<<endl;

  TMinuit* minuit = new TMinuit(2);//only 2 param.

#if 1
  //  minuit->SetFCN( fcn );// compile error 
  minuit->SetFCN(      & E16ANA_StepTrack::CalcDistance );

#ifdef FORTRAN_MINUIT
  enum {NOPRINT=-1, PRINT_MIN, PRINT_DEFAULT,PRINT_SECOND,PRINT_MAX};
  //exec command:set printout level.-1:no,0:min.,1:default,3:max.
  //  argument[0]=PRINT_DEFAULT;nArg=1;
  argument[0]=NOPRINT; nArg=1;
  MNEXCM(fcn, "set print", argument, nArg, errorFlag, 0);
  //exec command:set printout level.-1:no,0:min.,1:default,3:max.
  //  cerr<<"next parameter set(MNPARM)"<<endl;
#else
   minuit->SetPrintLevel(-1);
   //minuit->SetPrintLevel(0);
#endif  

for(int i=0;i<2;i++) {//parameter defining, and init value setting
#ifdef FORTRAN_MINUIT
    MNPARM(i+1, paramName[i],initValue[i],stepSize[i],
	   lowLimit[i],upLimit[i],flag);
#else
    minuit->mnparm(i,paramName[i], initValue[i], stepSize[i],
		   lowLimit[i], upLimit[i], flag);
#endif

    if(flag>0) {//error
      cerr<<"Minimize: set parameter error:"<<i<<" exit."<<endl;exit(1);
    }
  }

  //  cerr<<"next  MNSETI"<<endl;

#ifdef FORTRAN_MINUIT
  MNSETI("distance");//set title of the job.
#endif  

  fcnCalledCount=0;

#ifdef FORTRAN_MINUIT
  argument[0]=1;nArg=1;
  //  cerr<<"next MNEXCM(call fcn)"<<endl;
  MNEXCM(fcn,"call fcn",argument,nArg,errorFlag,0);//exec command:init. of fcn
  //  argument[0]=3;nArg=1;
  //  MNEXCM(fcn,"fix",   argument,nArg,errorFlag,0);
#else


#endif  

  argument[0]=1000;nArg=1;

#ifdef FORTRAN_MINUIT
  //  cerr<<"next  MIGRAD"<<endl;
  MNEXCM(fcn,"migrad",   argument,nArg,errorFlag,0);
                       //exec command:simple minimization.
  //  MNEXCM(fcn,"minos",    argument,nArg,errorFlag,0);//exec command:
  //    argument[0]=3;nArg=1;
  //    MNEXCM(fcn,"call fcn", argument,nArg,errorFlag,0);
  //exec command:fin. of fcn
#else
  minuit->mnexcm("MIGRAD", argument, nArg, errorFlag);
#endif  

  cerr<<"fcnCalledCount ="<<fcnCalledCount<<endl;

  nArg=0;
  //  char name[256];
  TString name;
  for(int i=0;i<2;i++) {
    double low,up;int ivarbl;
#ifdef FORTRAN_MINUIT
    MNPOUT(i+1,name,param[i],error[i],low,up,ivarbl);
#else
    minuit->mnpout(i,name,param[i],error[i],low,up,ivarbl);
#endif  
        //cout<<" param:"<<param[i]<<" "<<error[i]<<endl;
  }
  double fedm,errdef;int npari,nparx;
  

#ifdef FORTRAN_MINUIT
  MNSTAT(*distance, fedm,errdef,npari,nparx,istat);
#else
  minuit->mnstat(*distance, fedm,errdef,npari,nparx,istat);
#endif  

  //  fprintf(stderr,"MNSTAT:fmin=%lf fedm=%lf errdef=%lf npari=%d nparx=%d istat=%d\n",*distance,fedm,errdef,npari,nparx,istat);

  //   *distance *= 0.001;//990701

  //  minuit->mnprin(3,*distance);


#ifdef FORTRAN_MINUIT
  MNEXCM(fcn,"stop",     argument,nArg,errorFlag,0);//exec command:init. of fcn
#else
  minuit->mncler();
#endif  

#else
  *distance=9999;
  istat=9999;
#endif

  cerr<<" in MinuitDistance: distance  "<<*distance<<endl;

  delete minuit;

  return istat;
}

int E16ANA_StepTrack::CrossZconstPlane(double iz, Hep3Vector& crossPoint,
Hep3Vector& crossMom){

  int i=0,j=0;
  double prevZdiff = 0;
  Hep3Vector pointA, pointB;

  double zdiff = initPoint.z() - iz;
  //  if( fabs(zdiff) < DBL_EPSILON ){
  if( fabs(zdiff) < std::numeric_limits<double>::epsilon() ){
    //    cerr<<"crossz 0"<<endl;
    crossPoint = initPoint;
    crossMom = initMom;
    return 0;
  }
  for(i=0; i<arraySize; i++){ //follow the first array from the initPoint
    double zdiff = pointData1[i].z()- iz;
    if( zdiff * prevZdiff < 0 ) {//cross
      pointA=pointData1[i-1];      pointB=pointData1[i];
      double ratio = fabs( zdiff / (pointB.z()-pointA.z()) );
      double step = i - ratio;
      crossPoint = ArrayGet(step, pointData1, pointData2);
      crossMom   = ArrayGet(step, momData1, momData2);
      //  cerr<<"crossz 1"<<" "<<i<<" "<<step<<" "<<pointA<<" "<<pointB<<endl;
      break;
    }
    else{
      prevZdiff = zdiff;
    }
  }//for i
  //  cerr<<"crossz 11"<<" "<<i<<endl;
  if(i < arraySize ){    return 1;  }

  prevZdiff = 0;
  for(j=0; j<arraySize; j++){//follow the second array from the initPoint
    double zdiff = pointData2[j].z()- iz;
    if( zdiff * prevZdiff < 0 ) {//cross
      pointA=pointData2[j-1];      pointB=pointData2[j];
      double ratio = fabs( zdiff / (pointB.z()-pointA.z()) ) ;
      double step = (j-ratio)* -1;
      crossPoint = ArrayGet(step, pointData1, pointData2);
      crossMom   = ArrayGet(step, momData1, momData2);
      //    cerr<<"crossz 2"<<" "<<j<<" "<<step<<" "<<pointA<<" "<<pointB<<endl;
      break;
    }
    else{
      prevZdiff = zdiff;
    }
  }//for j

  //  cerr<<"crossz 21"<<" "<<j<<endl;
  if(j < arraySize ){    return 2;  }
  else{
    //    cerr<<"crossz 3"<<endl;
    crossPoint=errorVector;
    crossMom=errorVector;
    return -1;
  }


}
int E16ANA_StepTrack::CrossXconstPlane(double ix, Hep3Vector& crossPoint,
Hep3Vector& crossMom){

  int i=0,j=0;
  double prevdiff = 0;
  Hep3Vector pointA, pointB;

  double diff = initPoint.x() - ix;
  if( fabs(diff) < std::numeric_limits<double>::epsilon() ){
    crossPoint = initPoint;
    crossMom = initMom;
    return 0;
  }
  for(i=0;i<arraySize;i++){ //follow the first array from the initPoint
    double diff = pointData1[i].x()- ix;
    if( diff * prevdiff < 0 ) {//cross
      pointA=pointData1[i-1];      pointB=pointData1[i];
      double ratio = fabs (diff / (pointB.x()-pointA.x()) );
      double step = i- ratio;
      crossPoint = ArrayGet(step, pointData1, pointData2);
      crossMom   = ArrayGet(step, momData1, momData2);
      break;
    }
    else{
      prevdiff = diff;
    }
  }//for i

  if(i<arraySize){
    return 1;//OK
  }

  prevdiff = 0;
  for(j=0;i<arraySize;j++){//follow the second array from the initPoint
    double diff = pointData2[j].x()- ix;
    if( diff * prevdiff < 0 ) {//cross
      pointA=pointData2[j-1];      pointB=pointData2[j];
      double ratio = fabs( diff / (pointB.x()-pointA.x()) );
      double step = (j-ratio)* -1;
      crossPoint = ArrayGet(step, pointData1, pointData2);
      crossMom   = ArrayGet(step, momData1, momData2);
      break;
    }
    else{
      prevdiff = diff;
    }
  }//for j

  if(j<arraySize){ return 2; }
  else{
    crossPoint=errorVector;
    crossMom=errorVector;
    return -1;
  }


}



//------------------------------------------------------------
#if 1
void E16ANA_StepTrack::CalcDistance(int & npar,double *grad, double & fval,
				    double *xval, int iflag){
#else
void E16ANA_StepTrack::CalcDistance(const int *npar, double *grad,
				    double *fval, const double *xval,
			        const int *iflag, double (*futil)()){
#endif
  //to be minimized :calculate distance.b

  //  static int called=0;  called++;
  //  cout<<"fcn called:"<<called<<" iflag =" <<*iflag<<endl;
  fcnCalledCount++;

  Hep3Vector diffV(999,999,999);
  double distance = -999;
  if(iflag==4) {//set *fval
    double step1=xval[0];    double step2=xval[1];
    
    if(XZdistanceFlag == 0){  //distance in 3D
      diffV=crossSt1->Point(step1) - crossSt2->Point(step2);
      distance = diffV.mag();
    }
    else{// distance in 2D
      Hep3Vector p1( crossSt1->Point(step1).x(),0, crossSt1->Point(step1).z());
      Hep3Vector p2( crossSt2->Point(step2).x(),0, crossSt2->Point(step2).z());
      diffV=p1-p2;
      distance = diffV.mag();
    }

#if 1
    fval=distance;
#else
    *fval=distance;//normarize with the number of hit
#endif
	    //	    *fval=distance*1000;//990701
	    //	    cerr<<"cdist "<<distance*1000<<"[mm] "<<step1<<" "<<step2;
	    //	    cerr<<" "<<crossSt1->Point(step1)<<" "<<crossSt2->Point(step2)<<endl;
    return;
  }
  else if(iflag==1) {//for initialize
  }
  else if(iflag==3) {//for finish.(after fit is finished)
  }
  else if(iflag==2) {//calculate grad(optional)
  }
  else {
    cerr<<"Distance(minuit-fcn):invalid flag value:"<<iflag<<":exit"<<endl;
    exit(1);
  }
  
}

