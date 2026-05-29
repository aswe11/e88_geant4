//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai
//2015-12-03, uploaded by yokkaich
//2015-11-02, uploaded by yokkaich
//2015-11-02, uploaded by komatsu
//2015-03-27, uploaded by yokkaich
//2015-03-01, uploaded by yokkaich
//2015-01-05, uploaded by yokkaich
//2015-01-05, uploaded by yokkaich
//2014-08-27, uploaded by kawama
//2014-05-07, uploaded by kawama
//2014-04-30, uploaded by kawama
//2014-04-24, uploaded by kawama
//2013-11-14, uploaded by kawama
//stepTrack.hh 130822 by S. Yokkaichi
//    Last modified at <2015-01-04 18:45:29 >
//from
//stepTrack.hh 980421/981126 S.YOK.

#ifndef E16ANA_STEPTRACK_HH
#define E16ANA_STEPTRACK_HH

#include <TMinuit.h>

#include <CLHEP/Vector/ThreeVector.h>

using namespace CLHEP;



class E16ANA_MagneticFieldMap;

class E16ANA_StepTrack {
  //unit:
private:
  E16ANA_MagneticFieldMap* fmf_;

  Hep3Vector initPoint;
  Hep3Vector initMom;
  int charge;
  int arraySize;//maximum array size.
  int currentSize1;//current array max.(+)
  int currentSize2;//current array max.(-)

  static int fcnCalledCount;//not so good but ..
  double stepSize; //length 

  Hep3Vector* pointData1;//outbound trace
  Hep3Vector* pointData2;//inbound trace
  Hep3Vector* momData1;//outbound trace
  Hep3Vector* momData2;//inbound trace
  static const Hep3Vector errorVector;
  int errorFlag;

  static int XZdistanceFlag;

  int TraceInMagnet(E16ANA_MagneticFieldMap* ifmf, 
		    const double stepSize,
		    const int nStep, 
		    const Hep3Vector& initPoint, 
		    const Hep3Vector& initMom, 
                    const int charge, 
		    Hep3Vector* points,
		    Hep3Vector* momentums) ;

#if 0
  static double currentMomentumInverse;  //should be discuss to fact out...
  static void EquationOfMotion(double s, double* y, double* dy,double* f);
#else
  double currentMomentumInverse;  //should be discuss to fact out...
  void EquationOfMotion(double s, double* y, double* dy,double* f);
#endif
  void RKN_1stepBockArray(Hep3Vector& iy, Hep3Vector& idy, 
					  double x, double h, 
			  Hep3Vector& yResult,Hep3Vector& dyResult);


  Hep3Vector ArrayGet(int    step, Hep3Vector* array1,Hep3Vector* array2);
  //need step<arraySize. : here step is a paramater on the track
  //else return errorVector.
  Hep3Vector ArrayGet(double step, Hep3Vector* array1,Hep3Vector* array2);
  //for double 'step' : interpolate
  int MinuitDistance( double* step, double* lowLimit,double* upLimit,
			      double* distance,double* param,double* error) ;
  int MinuitDistanceXZ( double* step, double* lowLimit,double* upLimit,
			      double* distance,double* param,double* error) ;

  //void CalcDistance(const int *npar,double *grad, double *fval,
    static void CalcDistance(int & npar,double *grad, double & fval,
			     double *xval, int iflag);

public:
  enum {OK, ArraySizeOver};
  E16ANA_StepTrack(E16ANA_MagneticFieldMap* ifmf,
		   Hep3Vector ipoint, Hep3Vector imomentum,int icharge,
	    double istepSize,int iarraySize);
  //please check errorFlag.
  ~E16ANA_StepTrack() ;
    // step<arraySize.


  Hep3Vector InitMom(){return  initMom;}
  Hep3Vector InitPoint(){return  initPoint;}

  Hep3Vector Point(int    step){return  ArrayGet(step,pointData1,pointData2);  }
  Hep3Vector Point(double step) {return  ArrayGet(step,pointData1,pointData2);  }
  Hep3Vector Momentum(int    step) {  return  ArrayGet(step,momData1,momData2);  }
  Hep3Vector Momentum(double step) {  return  ArrayGet(step,momData1,momData2);  }

  Hep3Vector PointAtR(double r);

  int ErrorFlag() {return errorFlag;}
  int ArraySize() {return arraySize;}
  int CurrentSize1()  {return currentSize1;}
  int CurrentSize2()  {return currentSize2;}
  double StepSize() {return stepSize;}
  Hep3Vector ErrorVector(){return errorVector;}
  
  int Cross(E16ANA_StepTrack& st, double* distance,double* step1, double* step2);
  //return flag;: minuit minimize status. 3 is good.
  // cross is the mid-point of the most nearest point in each two track.


  int Cross(E16ANA_StepTrack& st, double* distance, Hep3Vector* point,
	       Hep3Vector* mom1, Hep3Vector* mom2) {
    double step1,step2;
    int flag=Cross(st,distance,&step1,&step2);
    //    std::cout << "Cross:flag="<<flag<<std::endl;
    *point =0.5*( Point(step1) + st.Point(step2) );
    *mom1  =    Momentum(step1);
    *mom2  = st.Momentum(step2);
    return flag;
  }

  int CrossZconstPlane(double iz, Hep3Vector& crossPoint,  Hep3Vector& crossMom);
  int CrossXconstPlane(double ix, Hep3Vector& crossPoint,  Hep3Vector& crossMom);

  int FcnCalledCount(){return fcnCalledCount;};

};


#endif// E16ANA_STEPTRACK_HH
