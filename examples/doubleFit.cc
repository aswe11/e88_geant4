////doubleFit.cc 151122 by S. Yokkaichi
//    Last modified at <2015-12-31 16:30:46 >
// from
//
//waveformDoubleFit.cc 150505 by S. Yokkaichi


#include <ctime>
#include <cmath>

#include <iostream>
#include <strstream>
#include <vector>

#include <HepPDT/ParticleData.hh>

#include <TFile.h>
#include <TPostScript.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TText.h>
#include <TStyle.h>

#include <TH1.h>
#include <TH2.h>
#include <TTree.h>
#include <TMath.h>
#include <TVector3.h>
#include <TRandom3.h>

#include "E16ANA_Constant.hh"

#include "E16ANA_ParamManager.hh"
#include "E16ANA_MagneticFieldMap.hh"
#include "E16ANA_Transform.hh"
#include "E16ANA_Geometry.hh"

#include "E16ANA_G4OutputData.hh"
#include "E16ANA_WaveformOutputData.hh"

#include "E16ANA_GTRResponse.hh"

#include "E16ANA_GTRStripAnalyzer.hh"

#include "E16ANA_SingleTrack.hh"
#include "E16ANA_StepTrack.hh"
#include "E16ANA_DoubleTrack.hh"

using namespace std;


//#define USE_WAVEFILE
#define SINGLEOVERLAP

//----------------------------------------------------------
// global pointer

//E16ANA_Transform*        e16transform;
E16ANA_GeometryV1*         geom;
E16ANA_MagneticFieldMap* fmf;

class Histo;
Histo* histo;

strstream globalARGstr;

const double RESIDUAL_THR = 0.3;//300um

//double fitError[9];

//---------------------------------------------
class Histo{
  //  g4hit        : g, g4
  //  response hit : r, res, rc(cog) rt(tdc)
  //  angle : coorinate
private:

  TH1F* g4moduleid100; TH1F* g4moduleid200; TH1F* g4moduleid300;

  TH1F* g4local100x; TH1F* g4local200x; TH1F* g4local300x;
  TH1F* g4local100y; TH1F* g4local200y; TH1F* g4local300y;  TH1F*** g4local;
  TH1F* g4locals[7][33];

  TH2F* g4local1002x; TH2F* g4local2002x; TH2F* g4local3002x;
  TH2F* g4local1002y; TH2F* g4local2002y; TH2F* g4local3002y; TH2F*** g4local2;

  TH1F* g4angle100x; TH1F* g4angle200x; TH1F* g4angle300x;
  TH1F* g4angle100y; TH1F* g4angle200y; TH1F* g4angle300y; TH1F*** g4angle;
  TH1F* g4incident100x; TH1F* g4incident200x; TH1F* g4incident300x;
  TH1F* g4incident100y; TH1F* g4incident200y; TH1F* g4incident300y; TH1F*** g4incident;

  TH1F* g4time100; TH1F* g4time200; TH1F* g4time300; TH1F*** g4time;


  TH1F* localcog100x; TH1F* localcog200x;TH1F* localcog300x;
  TH1F* localcog100y; TH1F* localcog200y;TH1F* localcog300y;TH1F* localcog100yb
;
  TH1F*** localcog;
  TH1F* localcogs[7][33];

  TH1F* localtdc100x; TH1F* localtdc200x;TH1F* localtdc300x;
  TH1F* localtdc100y; TH1F* localtdc200y;TH1F* localtdc300y;TH1F* localtdc100yb;
  TH1F*** localtdc;
  TH1F* localtdcs[7][33];

  TH1F* localdiff100x; TH1F* localdiff200x; TH1F* localdiff300x;
  TH1F* localdiff100y; TH1F* localdiff200y; TH1F* localdiff300y;
  TH1F* localdiff100yb;
  TH1F*** localdiff;
  TH1F* localdiffs[7][33];

  TH1F* time100x; TH1F* time200x; TH1F* time300x;
  TH1F* time100y; TH1F* time200y; TH1F* time300y;  TH1F* time100yb;
  TH1F*** time;

  TH1F* ganglec100; TH1F* ganglec200; TH1F* ganglec300; TH1F*** gangle;
  TH2F* gxz100;TH2F* gxz200;TH2F* gxz300;        TH2F*** gxz;

  TH1F* gy100; TH1F* gy200; TH1F* gy300;  TH1F* gyb100; TH1F*** gy;
  TH2F* gyz100; TH2F* gyz200; TH2F* gyz300;  TH2F* gybz100; TH2F*** gyz;


  TH1F* gtan100; TH1F* gtan200; TH1F* gtan300;

  TH1F* g4diff100x; TH1F* g4diff200x;TH1F* g4diff300x;
  TH1F* g4diff100y; TH1F* g4diff200y;TH1F* g4diff300y;
  TH1F*** g4diff;
  TH1F* g4diffs[6][33];//g4-tdc2

  TH1F* g4diffc100x; TH1F* g4diffc200x;TH1F* g4diffc300x;
  TH1F* g4diffc100y; TH1F* g4diffc200y;TH1F* g4diffc300y;
  TH1F*** g4diffc;
  TH1F* g4diffcs[6][33];//g4-cog


  TH1F* g4phidiff100x; TH1F* g4phidiff200x;TH1F* g4phidiff300x;
  TH1F* g4phidiff100y; TH1F* g4phidiff200y;TH1F* g4phidiff300y;
  TH1F*** g4phidiff;
  TH1F* g4phidiffs[6][33];//g4-cog for phi

  TH1F* g4phidiffc100x; TH1F* g4phidiffc200x;TH1F* g4phidiffc300x;
  TH1F* g4phidiffc100y; TH1F* g4phidiffc200y;TH1F* g4phidiffc300y;
  TH1F*** g4phidiffc;
  TH1F* g4phidiffcs[6][33];//g4-cog for phi

  TH2F*** phiTimeXY;
  TH1F*** phiTimeXYdiff;
  TH2F* phiTime100XY; TH2F* phiTime200XY; TH2F* phiTime300XY; 
  TH1F* phiTime100XYdiff; TH1F* phiTime200XYdiff; TH1F* phiTime300XYdiff; 
  TH1F*** phiTime;
  TH1F* phiTime100x; TH1F* phiTime200x; TH1F* phiTime300x; 
  TH1F* phiTime100y; TH1F* phiTime200y; TH1F* phiTime300y; 


  TH1F* g4phi100; TH1F* g4phi200; TH1F* g4phi300;
  TH1F* g4phiinc100; TH1F* g4phiinc200; TH1F* g4phiinc300;
  TH1F* g4phi100t; TH1F* g4phi200t; TH1F* g4phi300t;
  TH1F* g4phi100t2; TH1F* g4phi200t2; TH1F* g4phi300t2;

  TH1F* chi2;  TH1F* mass1;TH1F* mass2; TH1F* bg1;
  TH1F* origvx; TH1F* origvy; TH1F* origvz; TH2F* origvxy;
  TH1F* fitvx; TH1F* fitvy; TH1F* fitvz; TH2F* fitvxy;
  TH1F* dvx; TH1F* dvy; TH1F* dvz; 
  TH1F* fitstat;
  TH1F* dm; TH1F* dp1; TH1F* dp2; TH1F* dpp1; TH1F* dpp2; 
  TH1F* origp1; TH1F* origp2;   TH1F* fitp1; TH1F* fitp2; 
  TH1F* mass1c;TH1F* mass2c; TH1F* dmc;


  TH1F* chi21;  TH1F* mass11;TH1F* mass21;TH1F* dm1;TH1F* dpp11; TH1F* dpp21; 
  TH1F* chi22;  TH1F* mass12;TH1F* mass22;TH1F* dm2;TH1F* dpp12; TH1F* dpp22; 
  TH1F* origp11; TH1F* origp21;
  TH1F* origp12; TH1F* origp22;


  TH1F* fitres1001x;TH1F* fitres2001x;TH1F* fitres3001x;
  TH1F* fitres1002x;TH1F* fitres2002x;TH1F* fitres3002x;
  TH1F* fitres1001y;TH1F* fitres2001y;TH1F* fitres3001y;
  TH1F* fitres1002y;TH1F* fitres2002y;TH1F* fitres3002y;

  TH1F* chi2s1;  TH1F* chi2s2;  TH1F* crossdistance;
  TH1F* crossSX; TH1F* crossSY; TH1F* crossSZ; TH2F* crossSXY; 
  TH1F* s1stat;  TH1F* s1mstat;   TH1F* s2stat;  TH1F* s2mstat;
  TH1F* dstat;  TH1F* dmstat;  TH1F* crossStat;

  TH2F* module1;

  TFile* file;
  string* psfilename;
  string* outputfilename;

  //  static const int NTIMEBIN  = 525;
  //  static const int OFFSETBIN = 175;
  static const double tanThr = 0.27;//tan15deg

public:

  void GTRLocal(int layer, E16ANA_GTRAnalyzedStripHit& hit){
        ((TH1F*) *localcog[layer])    ->Fill( hit.CogHit() );
        ((TH1F*) *localtdc[layer])    ->Fill( hit.TdcHit() );
        ((TH1F*) *localdiff[layer])   ->Fill( hit.CogHit()-hit.TdcHit() );
        ((TH1F*) *time[layer])        ->Fill(hit.Timing() );
	int moduleID = hit.ModuleID();
        ((TH1F*) localcogs[layer][moduleID])  ->Fill( hit.CogHit() );
	((TH1F*) localtdcs[layer][moduleID])  ->Fill( hit.TdcHit() );
        ((TH1F*) localdiffs[layer][moduleID])   ->Fill( hit.CogHit()-hit.TdcHit() );
  }

  void GTRGlobal(int layer, int moduleID, TVector3& gV){
    double anglex = RadToDeg * atan2( gV.X(), gV.Z() );
    double xz = sqrt( gV.X()*gV.X() + gV.Z()*gV.Z() );
    double angley = RadToDeg * atan2( gV.Y(), xz );

#if 1
    if( layer<3 ){
      ((TH1F*) *gangle[layer])->Fill( anglex );
      ((TH1F*) *gxz[layer])   ->Fill( gV.X(), gV.Z()  );
    }
    else{
      ((TH1F*) *gy[layer-3])  ->Fill( gV.Y() );
      ((TH1F*) *gyz[layer-3]) ->Fill( gV.Y(), gV.Z()  );
    }
#endif
  }


  void G4Local(int layer, int moduleID, E16ANA_G4Hit& hit){ 
    int layer2 = layer+3;// for y-histo

	double anglex = atan2(hit.GX(), hit.GZ()) * RadToDeg ;
	double xz = sqrt( hit.GX()*hit.GX() + hit.GZ()*hit.GZ() );
	double angley = atan2(hit.GY(), xz) * RadToDeg ;

        ((TH1F*) *g4local[layer])    ->Fill( hit.X()*10 );
        ((TH2F*) *g4local2[layer])   ->Fill( hit.X()*10, moduleID );
        ((TH1F*) *g4angle[layer])   ->Fill( anglex );
        ((TH1F*) *g4time[layer])   ->Fill( hit.Time());

        ((TH1F*) *g4local[layer2])    ->Fill( hit.Y()*10 );
        ((TH2F*) *g4local2[layer2])   ->Fill( hit.Y()*10, moduleID );
        ((TH1F*) *g4angle[layer2])   ->Fill( angley );

        ((TH1F*) g4locals[layer][moduleID])    ->Fill( hit.X()*10 );
        ((TH1F*) g4locals[layer2][moduleID])    ->Fill( hit.Y()*10 );



	TVector3 p = hit.PTV();

	double incidentAnglex = atan2(p.X(), p.Z()) * RadToDeg ;
	if( fabs(incidentAnglex ) >90){
	  //	  cerr<<"inc "<<incidentAnglex<<" "<<p.X()<<" "<<p.Z()<<endl;
	}
	double pxz = sqrt( p.X()*p.X() + p.Z()*p.Z() );
	double incidentAngley = atan2(p.Y(), pxz) * RadToDeg ;

        ((TH1F*) *g4incident[layer])   ->Fill( incidentAnglex );
        ((TH1F*) *g4incident[layer2])  ->Fill( incidentAngley );
  }

  void G4Diff(int layer, int moduleID, double v, double v2){ 
    ((TH1F*) *g4diff[layer])    ->Fill( v );
    ((TH1F*) g4diffs[layer][moduleID])    ->Fill( v );
    ((TH1F*) *g4diffc[layer])    ->Fill( v2 );
    ((TH1F*) g4diffcs[layer][moduleID])    ->Fill( v2 );
  }


  void G4phiDiff(int layer, int moduleID, double v, double v2){ 
    ((TH1F*) *g4phidiff[layer])    ->Fill( v );
    //    ((TH1F*) g4phidiffs[layer][moduleID])    ->Fill( v );
    ((TH1F*) *g4phidiffc[layer])    ->Fill( v2 );
    //    ((TH1F*) g4phidiffcs[layer][moduleID])    ->Fill( v2 );
  }


  void PhiTimeXY(int layer, int moduleID, double v, double v2){ 
    ((TH2F*) *phiTimeXY[layer])    ->Fill( v, v2 );
    ((TH1F*) *phiTime[layer])    ->Fill( v );
    ((TH1F*) *phiTime[layer+3])    ->Fill( v2 );
    ((TH1F*) *phiTimeXYdiff[layer])    ->Fill( v-v2 );

  }


  void L1001(double v1,double v2, double v3){ 
    //    g4diff100 ->Fill(v1); g4diff100t->Fill(v2);
    //    gtan100->Fill(atan(v3) * RadToDeg );
  }

  void G4phi100(double v1,double v2, double v3, double v4){ 
    g4phi100 ->Fill(v1); g4phi100t->Fill(v2);g4phi100t2->Fill(v3);
    g4phiinc100 ->Fill( atan(v4) * RadToDeg ); }

  bool DoubleFitResidual( E16ANA_DoubleTrack* dtrk) {

  TH1F* reshx[6]={ fitres1001x, fitres2001x, fitres3001x,
		 fitres1002x, fitres2002x, fitres3002x};
  TH1F* reshy[6]={ fitres1001y, fitres2001y, fitres3001y,
		 fitres1002y, fitres2002y, fitres3002y};
  bool flag1 = true; 

  for(int i=0;i<6;i++){
    TVector3 res = dtrk -> GetFitLPos(i) - dtrk -> GetLPos(i);
    reshx[i]->Fill( res.X()*10 );
    reshy[i]->Fill( res.Y()*10 );
    if( fabs( res.X()*10 )  > RESIDUAL_THR ){
      flag1 *= false;
    }
  }//i

  return flag1;

  }

  void DoubleFit( double chisq, int stat, 
    TVector3& origp1,TVector3& origp2, 
    TVector3& fitp1, TVector3& fitp2, 
    TVector3& origvtx, TVector3& fitvtx,
    double origmass, double fitmass, int stat1, int stat2,
    bool flag1, bool flag2){

    chi2->Fill(chisq);
    bg1->Fill( (origp1+origp2).Mag() / origmass );
    mass1 ->Fill(origmass * 1000);
    mass2 ->Fill(fitmass * 1000);
    dm    ->Fill( (fitmass-origmass)*1000 ) ;

    if( chisq < 3 ){
      mass1c ->Fill(origmass * 1000);
      mass2c ->Fill(fitmass * 1000);
      dmc    ->Fill( (fitmass-origmass)*1000 ) ;
    }
    dp1   ->Fill( fitp1.Mag() - origp1.Mag() );
    dp2   ->Fill( fitp2.Mag() - origp2.Mag() );
    dpp1   ->Fill( (fitp1.Mag() - origp1.Mag() )*100/origp1.Mag()  );
    dpp2   ->Fill( (fitp2.Mag() - origp2.Mag() )*100/origp2.Mag()  );
    fitvx ->Fill( fitvtx.x()*10) ;    fitvy ->Fill( fitvtx.y()*10) ;    
    fitvz ->Fill( fitvtx.z()*10) ;fitvxy ->Fill( fitvtx.x()*10,fitvtx.y()*10 );

    origvx ->Fill( origvtx.x()*10) ;    origvy ->Fill( origvtx.y()*10) ;    
    origvz ->Fill( origvtx.z()*10) ;origvxy ->Fill( origvtx.x()*10,origvtx.y()*10 );

    dvx->Fill( ( fitvtx.x() - origvtx.x() )*10  );
    dvy->Fill( ( fitvtx.y() - origvtx.y() )*10  );
    dvz->Fill( ( fitvtx.z() - origvtx.z() )*10  );

    fitstat->Fill(stat1);
    fitstat->Fill(stat2+10);

    this->origp1   ->Fill( origp1.Mag()   );
    this->origp2   ->Fill( origp2.Mag()   );
    this->fitp1   ->Fill( fitp1.Mag()   );
    this->fitp2   ->Fill( fitp2.Mag()   );

    if( flag1  ){
      chi21->Fill(chisq);
      mass11 ->Fill(origmass * 1000);
      mass21 ->Fill(fitmass * 1000);
      dm1    ->Fill( (fitmass-origmass)*1000 ) ;
      dpp11   ->Fill( (fitp1.Mag() - origp1.Mag() )*100/origp1.Mag()  );
      dpp21   ->Fill( (fitp2.Mag() - origp2.Mag() )*100/origp2.Mag()  );
      this->origp11   ->Fill( origp1.Mag()   );
      this->origp21   ->Fill( origp2.Mag()   );
    }
    if( flag2  ){
      chi22->Fill(chisq);
      dm2    ->Fill( (fitmass-origmass)*1000 ) ;
      mass12 ->Fill(origmass * 1000);
      mass22 ->Fill(fitmass * 1000);
      dpp12   ->Fill( (fitp1.Mag() - origp1.Mag() )*100/origp1.Mag()  );
      dpp22   ->Fill( (fitp2.Mag() - origp2.Mag() )*100/origp2.Mag()  );
      this->origp12   ->Fill( origp1.Mag()   );
      this->origp22   ->Fill( origp2.Mag()   );
    }


  }
  void G4ModuleHit( E16ANA_G4ModuleHitList& moduleHits){ 
    for (int k=0; k< 33 ;k++){   //module-by-module
      vector<E16ANA_G4Hit>& vec1 = moduleHits.GTRModule(0, k);
      vector<E16ANA_G4Hit>& vec2 = moduleHits.GTRModule(1, k);
      vector<E16ANA_G4Hit>& vec3 = moduleHits.GTRModule(2, k);

      module1 ->Fill((double)k, (double)0, (double)vec1.size() );
      module1 ->Fill((double)k, (double)1, (double)vec2.size() );
      module1 ->Fill((double)k, (double)2, (double)vec3.size() );

      g4moduleid100  ->Fill((double)k, (double)vec1.size() );
      g4moduleid200  ->Fill((double)k, (double)vec2.size() );
      g4moduleid300  ->Fill((double)k, (double)vec3.size() );


#if 1
      for(int i=0; i<(int)vec1.size();i++){
	E16ANA_G4Hit& hit = vec1[i];	
	G4Local(0, k, hit);
      }
      for(int i=0; i<(int)vec2.size();i++){
	E16ANA_G4Hit& hit = vec2[i];	
	G4Local(1, k, hit);
      }
      for(int i=0; i<(int)vec3.size();i++){
	E16ANA_G4Hit& hit = vec3[i];	
	G4Local(2, k, hit);
      }
#endif      
    }//k 

    
  }

  Histo(char* outputfile){

    file = new TFile(outputfile, "recreate");
    outputfilename = new string(outputfile);
    psfilename = new string(*outputfilename);
    *(psfilename) +=".ps";

    //    cerr<<"next open psfile :"<<psfilename<<endl;
    
    g4moduleid100=new TH1F("g4moduleid100","100 g4moduleid", 35, -1.5, 33.5);
    g4moduleid200=new TH1F("g4moduleid200","200 g4moduleid", 35, -1.5, 33.5);
    g4moduleid300=new TH1F("g4moduleid300","300 g4moduleid", 35, -1.5, 33.5);


    char const* tag[7]={"100x", "200x", "300x", "100y", "200y", "300y", "100yb"};

    localcog=new TH1F**[7]{&localcog100x, &localcog200x, &localcog300x,
    		  &localcog100y, &localcog200y, &localcog300y, &localcog100yb};
    for(int i=0;i<7;i++){
      char name1[32]; sprintf(name1, "localcog%s",          tag[i]);
      char name2[32]; sprintf(name2, "%s response cog",tag[i]);
      *(localcog[i]) = new TH1F(name1, name2, 52,  -156, 156);
      //      cerr<<"lcog "<<localcog[i]<<" "<<localcog100x<<" "<<*localcog[i]<<" "<<*(localcog[i])<<endl;
      //        localcog100x->Fill( 100 );
	//	((TH1F*) *localcog[i]) ->Fill( 100 );

      for(int j=0;j<33;j++){
	sprintf(name1, "localcogs-%02d-%s", j, tag[i]);
	sprintf(name2, "%s-%02d localcogs", tag[i], j);
	localcogs[i][j] = new TH1F(name1, name2, 52,  -156, 156);
      }
    }

    localtdc=new TH1F**[7]{&localtdc100x, &localtdc200x, &localtdc300x,
	&localtdc100y, &localtdc200y, &localtdc300y, &localtdc100yb};
    for(int i=0;i<7;i++){
      char name1[32]; sprintf(name1, "localtdc%s",          tag[i]);
      char name2[32]; sprintf(name2, "%s response tdc",tag[i]);
      *(localtdc[i]) = new TH1F(name1, name2, 52,  -156, 156);

      for(int j=0;j<33;j++){
	sprintf(name1, "localtdcs-%02d-%s", j, tag[i]);
	sprintf(name2, "%s-%02d localtdcs", tag[i], j);
	localtdcs[i][j] = new TH1F(name1, name2, 52,  -156, 156);
      }
    }

    g4diff=new TH1F**[6]{&g4diff100x, &g4diff200x, &g4diff300x,
	&g4diff100y, &g4diff200y, &g4diff300y};
    for(int i=0;i<6;i++){
      char name1[32]; sprintf(name1, "g4diff%s",          tag[i]);
      char name2[32]; sprintf(name2, "%s g4-tdc2",tag[i]);
      *(g4diff[i]) = new TH1F(name1, name2, 50,  -1, 1);

      for(int j=0;j<33;j++){
	sprintf(name1, "g4diffs-%02d-%s", j, tag[i]);
	sprintf(name2, "%s-%02d g4-tdc2", tag[i], j);
	g4diffs[i][j] = new TH1F(name1, name2, 50,  -1, 1);
      }
    }

    g4diffc=new TH1F**[6]{&g4diffc100x, &g4diffc200x, &g4diffc300x,
	&g4diffc100y, &g4diffc200y, &g4diffc300y};
    for(int i=0;i<6;i++){
      char name1[32]; sprintf(name1, "g4diffc%s",          tag[i]);
      char name2[32]; sprintf(name2, "%s g4-cog",tag[i]);
      *(g4diffc[i]) = new TH1F(name1, name2, 50,  -1, 1);

      for(int j=0;j<33;j++){
	sprintf(name1, "g4diffcs-%02d-%s", j, tag[i]);
	sprintf(name2, "%s-%02d g4-cog", tag[i], j);
	g4diffcs[i][j] = new TH1F(name1, name2, 50,  -1, 1);
      }
    }

    g4phidiffc=new TH1F**[6]{&g4phidiffc100x, &g4phidiffc200x, &g4phidiffc300x,
	&g4phidiffc100y, &g4phidiffc200y, &g4phidiffc300y};
    for(int i=0;i<6;i++){
      char name1[32]; sprintf(name1, "g4phidiffc%s",          tag[i]);
      char name2[32]; sprintf(name2, "%s g4-cog for phi",tag[i]);
      *(g4phidiffc[i]) = new TH1F(name1, name2, 50,  -1, 1);

      for(int j=0;j<33;j++){
	sprintf(name1, "g4phidiffcs-%02d-%s", j, tag[i]);
	sprintf(name2, "%s-%02d g4-cog for phi", tag[i], j);
	//	g4phidiffcs[i][j] = new TH1F(name1, name2, 50,  -1, 1);
      }
    }

    g4phidiff=new TH1F**[6]{&g4phidiff100x, &g4phidiff200x, &g4phidiff300x,
	&g4phidiff100y, &g4phidiff200y, &g4phidiff300y};
    for(int i=0;i<6;i++){
      char name1[32]; sprintf(name1, "g4phidiff%s",          tag[i]);
      char name2[32]; sprintf(name2, "%s g4-tdc for phi",tag[i]);
      *(g4phidiff[i]) = new TH1F(name1, name2, 50,  -1, 1);

      for(int j=0;j<33;j++){
	sprintf(name1, "g4phidiffs-%02d-%s", j, tag[i]);
	sprintf(name2, "%s-%02d g4-tdc for phi", tag[i], j);
	//	g4phidiffs[i][j] = new TH1F(name1, name2, 50,  -1, 1);
      }
    }

    phiTime=new TH1F**[6]{&phiTime100x, &phiTime200x, &phiTime300x,
			    &phiTime100y, &phiTime200y, &phiTime300y};

    for(int i=0;i<6;i++){
      char name1[32]; sprintf(name1, "phitime%s",          tag[i]);
      char name2[32]; sprintf(name2, "%s response time for phi", tag[i]);
      *(phiTime[i]) = new TH1F(name1, name2,   50,  -200, 800);
    }

    phiTimeXY=new TH2F**[3]{&phiTime100XY, &phiTime200XY, &phiTime300XY};
   for(int i=0;i<3;i++){
      char name1[32]; sprintf(name1, "phiTimeXY%s",          tag[i]);
      char name2[32]; sprintf(name2, "%s nearest time xy for phi",tag[i]);
      *(phiTimeXY[i]) = new TH2F(name1, name2, 
	100,  -200, 800, 100,  -200, 800);
    }
    phiTimeXYdiff=new TH1F**[3]{&phiTime100XYdiff, 
				&phiTime200XYdiff, &phiTime300XYdiff};
   for(int i=0;i<3;i++){
      char name1[32]; sprintf(name1, "phiTimeXYdiff%s",          tag[i]);
      char name2[32]; sprintf(name2, "%s nearest time x-y for phi",tag[i]);
      *(phiTimeXYdiff[i]) = new TH1F(name1, name2, 50,  -200, 200);
    }

    localdiff=new TH1F**[7]{&localdiff100x, &localdiff200x, &localdiff300x,
  		  &localdiff100y, &localdiff200y, &localdiff300y, &localdiff100yb};
    for(int i=0;i<7;i++){
      char name1[32]; sprintf(name1, "localdiff%s",          tag[i]);
      char name2[32]; sprintf(name2, "%s response c-t",tag[i]);
      *(localdiff[i]) = new TH1F(name1, name2,  100,  -5, 5);

      for(int j=0;j<33;j++){
	sprintf(name1, "localdiffs-%02d-%s", j, tag[i]);
	sprintf(name2, "%s-%02d response c-t", tag[i], j);
	localdiffs[i][j] = new TH1F(name1, name2, 50,  -5, 5);
      }
    }

    time=new TH1F**[7]{&time100x, &time200x, &time300x,
  		  &time100y, &time200y, &time300y, &time100yb};
    for(int i=0;i<7;i++){
      char name1[32]; sprintf(name1, "time%s",          tag[i]);
      char name2[32]; sprintf(name2, "%s response time", tag[i]);
      *(time[i]) = new TH1F(name1, name2,   50,  -200, 800);
    }

    g4time=new TH1F**[3]{&g4time100, &g4time200, &g4time300};
    for(int i=0;i<3;i++){
      char name1[32]; sprintf(name1, "g4time%s",          tag[i]);
      char name2[32]; sprintf(name2, "%s g4 time", tag[i]);
      *(g4time[i]) = new TH1F(name1, name2,   50,  -200, 800);
    }

    g4local=new TH1F**[6]{&g4local100x, &g4local200x, &g4local300x,
			  &g4local100y, &g4local200y, &g4local300y};
    for(int i=0;i<6;i++){
      char name1[32]; sprintf(name1, "g4local%s",          tag[i]);
      char name2[32]; sprintf(name2, "%s g4 local",tag[i]);
      *(g4local[i]) = new TH1F(name1, name2, 52,  -156, 156);

      for(int j=0;j<33;j++){
	sprintf(name1, "g4locals-%02d-%s", j, tag[i]);
	sprintf(name2, "%s-%02d g4locals", tag[i], j);
	g4locals[i][j] = new TH1F(name1, name2, 52,  -156, 156);
      }
    }

    g4local2=new TH2F**[6]{&g4local1002x, &g4local2002x, &g4local3002x,
			  &g4local1002y, &g4local2002y, &g4local3002y};
    for(int i=0;i<6;i++){
      char name1[32]; sprintf(name1, "g4local2 %s",          tag[i]);
      char name2[32]; sprintf(name2, "%s g4 local2",tag[i]);
      *(g4local2[i]) = new TH2F(name1, name2, 52,  -156, 156, 33,-0.5,32.5);
    }

    g4angle=new TH1F**[6]{&g4angle100x, &g4angle200x, &g4angle300x,
			  &g4angle100y, &g4angle200y, &g4angle300y};
    for(int i=0;i<6;i++){
      char name1[32]; sprintf(name1, "g4angle coord %s",          tag[i]);
      char name2[32]; sprintf(name2, "%s g4 angle",tag[i]);
      *(g4angle[i]) = new TH1F(name1, name2, 60, -180, 180);
    }
    g4incident=new TH1F**[6]{&g4incident100x, &g4incident200x, &g4incident300x,
			  &g4incident100y, &g4incident200y, &g4incident300y};
    for(int i=0;i<6;i++){
      char name1[32]; sprintf(name1, "g4incident angle %s",       tag[i]);
      char name2[32]; sprintf(name2, "%s g4 incident",tag[i]);
      *(g4incident[i]) = new TH1F(name1, name2, 60, -180, 180);
    }


    ganglec100=new TH1F("ganglec100","100 angle coordinate", 60, -180, 180);
    ganglec200=new TH1F("ganglec200","200 angle coordinate", 60,  -180, 180);
    ganglec300=new TH1F("ganglec300","300 angle coordinate", 60,  -180, 180);
    gangle=new TH1F**[3]{&ganglec100, &ganglec200, &ganglec300};

    gxz100=new TH2F("gxz100","xz 100 ", 70,  -700, 700, 70,  -700, 700);
    gxz200=new TH2F("gxz200","xz 200 ", 70,  -700, 700, 70,  -700, 700);
    gxz300=new TH2F("gxz300","xz 300 ", 70,  -700, 700, 70,  -700, 700);
    gxz =new TH2F**[3]{&gxz100, &gxz200, &gxz300};

    gy100 =new TH1F("gy100","yz 100 ", 70,  -700, 700);
    gy200 =new TH1F("gy200","yz 200 ", 70,  -700, 700);
    gy300 =new TH1F("gy300","yz 300 ", 70,  -700, 700);
    gyb100=new TH1F("gyb100","yb 100 ", 70,  -700, 700);
    gy    =new TH1F**[4]{&gy100, &gy200, &gy300, &gyb100};


    gyz100=new TH2F("gyz100","yz 100 ", 70,  -700, 700, 70,  -700, 700);
    gyz200=new TH2F("gyz200","yz 200 ", 70,  -700, 700, 70,  -700, 700);
    gyz300=new TH2F("gyz300","yz 300 ", 70,  -700, 700, 70,  -700, 700);
    gybz100=new TH2F("gybz100","ybz 100 ", 70,  -700, 700, 70,  -700, 700);
    gyz =new TH2F**[4]{&gyz100, &gyz200, &gyz300, &gybz100};


    gtan100=new TH1F("gtan100","100 g4 incident", 60, -90, 90);
    gtan200=new TH1F("gtan200","200 g4 incident",  60, -90, 90);
    gtan300=new TH1F("gtan300","300 g4 incident",  60, -90, 90);

    g4phi100=new TH1F("g4phi100","100 g4-rc phi", 100,  -1, 1);
    g4phi200=new TH1F("g4phi200","200 g4-rc phi", 100,  -1, 1);
    g4phi300=new TH1F("g4phi300","300 g4-rc phi", 100,  -1, 1);
    g4phi100t=new TH1F("g4phi100t","100 g4-rt phi", 100,  -5, 5);
    g4phi200t=new TH1F("g4phi200t","200 g4-rt phi", 100,  -5, 5);
    g4phi300t=new TH1F("g4phi300t","300 g4-rt phi", 100,  -5, 5);
    g4phi100t2=new TH1F("g4phi100t2","100 g4-rt2 phi", 100,  -1, 1);
    g4phi200t2=new TH1F("g4phi200t2","200 g4-rt2 phi", 100,  -1, 1);
    g4phi300t2=new TH1F("g4phi300t2","300 g4-rt2 phi", 100,  -1, 1);
    g4phiinc100=new TH1F("g4phiinc100","100 phi incident", 60, -180, 180);
    g4phiinc200=new TH1F("g4phiinc200","200 phi incident", 60, -180, 180);
    g4phiinc300=new TH1F("g4phiinc300","300 phi incident", 60, -180, 180);


    module1  = new TH2F("module1","modulehit", 35, -1.5, 33.5, 3, -0.5, 2.5);

    chi21  = new TH1F("chi21","chi21 cut1", 20,  0, 20);
    dm1    = new TH1F("dm1","mass res. cut1", 50,  -50, 50);//MeV
    dpp11  = new TH1F("dpp11","dp1/p1 cut1 ", 100,  -10, 10);
    dpp21  = new TH1F("dpp21","dp2/p2  cut1", 100,  -10, 10);
    mass11= new TH1F("mass11","sim mass cut1", 50,  920, 1120);
    mass21= new TH1F("mass21","fit mass cut1", 50,  920, 1120);
    origp11   = new TH1F("origp11","origp1 cut1", 50,  0, 5);
    origp21   = new TH1F("origp21","origp2 cut1", 50,  0, 5);

    chi22  = new TH1F("chi22","chi2 cut2", 20,  0, 20);
    dm2    = new TH1F("dm1","mass res. cut2", 50,  -50, 50);//MeV
    dpp12  = new TH1F("dpp11","dp1/p1 cut2", 100,  -10, 10);
    dpp22  = new TH1F("dpp21","dp2/p2  cut2", 100,  -10, 10);
    mass12= new TH1F("mass11","sim mass cut2", 50,  920, 1120);
    mass22= new TH1F("mass21","fit mass cut2", 50,  920, 1120);
    origp12   = new TH1F("origp12","origp1 cut2", 50,  0, 5);
    origp22   = new TH1F("origp22","origp2 cut2", 50,  0, 5);

    chi2s1 = new TH1F("chi2s1","chi2s1", 20,  0, 20);
    chi2s2 = new TH1F("chi2s2","chi2s2", 20,  0, 20);
    s1stat = new TH1F("s1stat","s1 fitstat", 6,  -1.5, 4.5);
    s2stat = new TH1F("s2stat","s2 fitstat", 6,  -1.5, 4.5);
    dstat  = new TH1F("dstat","double fitstat", 6,  -1.5, 4.5);
    s1mstat = new TH1F("s1mstat","s1 matrixstat", 6,  -1.5, 4.5);
    s2mstat = new TH1F("s2mstat","s2 matrixstat", 6,  -1.5, 4.5);
    dmstat  = new TH1F("dmstat","double matrix", 6,  -1.5, 4.5);
    crossStat  = new TH1F("crossStat","cross status", 6,  -1.5, 4.5);

    crossSX =  new TH1F("crossSX","single cross x", 100,  -10, 10);//mm
    crossSXY=  new TH2F("crossSXY","single cross xy", 40,  -10, 10,
			40,-10,10);//mm
    crossSY =  new TH1F("crossSY","single cross y", 100,  -10, 10);//mm
    crossSZ =  new TH1F("crossSZ","single cross z", 100,  -50, 50);//mm
    crossdistance =  new TH1F("crossdistance","single cross d", 100, 0, 100);//mm

    chi2 = new TH1F("chi2","chi2", 20,  0, 20);
    dm   = new TH1F("dm","mass resolution", 50,  -50, 50);//MeV
    dmc  = new TH1F("dmc","mass resolution", 50,  -50, 50);//MeV
    dp1   = new TH1F("dp1","p1 resolution", 100,  -10, 10);
    dp2   = new TH1F("dp2","p2 resolution", 100,  -10, 10);
    dpp1  = new TH1F("dpp1","dp1/p1 ", 100,  -10, 10);
    dpp2  = new TH1F("dpp2","dp2/p2 ", 100,  -10, 10);
    fitp1   = new TH1F("fitp1","fitp1", 50,  0, 5);
    fitp2   = new TH1F("fitp2","fitp2", 50,  0, 5);
    origp1   = new TH1F("origp1","origp1", 50,  0, 5);
    origp2   = new TH1F("origp2","origp2", 50,  0, 5);
    mass1= new TH1F("mass1","sim mass", 50,  920, 1120);
    mass2= new TH1F("mass2","fit mass", 50,  920, 1120);
    mass1c= new TH1F("mass1c","sim mass",50,  920, 1120);
    mass2c= new TH1F("mass2c","fit mass",50,  920, 1120);
    fitvx =  new TH1F("fitvx","fit vx", 120,  -10, 10);//mm
    fitvy =  new TH1F("fitvy","fit vy", 120,  -10, 10);
    fitvz =  new TH1F("fitvz","fit vz", 100,  -1, 1);
    fitvxy = new TH2F("fitvxy","fit vz", 120,  -30, 30, 120, -30, 30);
    origvx =  new TH1F("origvx","orig vx", 100,  -10, 10);//mm
    origvy =  new TH1F("origvy","orig vy", 100,  -10, 10);
    origvz =  new TH1F("origvz","orig vz", 100,  -50, 50);
    origvxy = new TH2F("origvxy","orig vz", 120,  -30, 30, 120, -30, 30);
    dvx =  new TH1F("dvx","vx fit-orig", 100,  -4.0, 4.0);//mm
    dvy =  new TH1F("dvy","vy fit-orig", 100,  -4.0, 4.0);//mm
    dvz =  new TH1F("dvz","vz fit-orig", 100,  -0.5, 0.5);//mm
    fitstat =  new TH1F("fitstat","fit status", 20,  -0.5, 19.5);//mm
    bg1  =  new TH1F("bg1","beta gamma", 100,  -1, 4);


    fitres1001x = new TH1F("fitres1001x","fit residual 100x1", 100,  -1, 1);//mm
    fitres2001x = new TH1F("fitres2001x","fit residual 200x1", 100,  -1, 1);//mm
    fitres3001x = new TH1F("fitres3001x","fit residual 300x1", 100,  -1, 1);//mm
    fitres1002x = new TH1F("fitres1002x","fit residual 100x2", 100,  -1, 1);//mm
    fitres2002x = new TH1F("fitres2002x","fit residual 200x2", 100,  -1, 1);//mm
    fitres3002x = new TH1F("fitres3002x","fit residual 300x2", 100,  -1, 1);//mm
    fitres1001y = new TH1F("fitres1001y","fit residual 100y1", 100,  -1, 1);//mm
    fitres2001y = new TH1F("fitres2001y","fit residual 200y1", 100,  -1, 1);//mm
    fitres3001y = new TH1F("fitres3001y","fit residual 300y1", 100,  -1, 1);//mm
    fitres1002y = new TH1F("fitres1002y","fit residual 100y2", 100,  -1, 1);//mm
    fitres2002y = new TH1F("fitres2002y","fit residual 200y2", 100,  -1, 1);//mm
    fitres3002y = new TH1F("fitres3002y","fit residual 300y2", 100,  -1, 1);//mm

    //tree = new TTree("tree", "tree");
    //tree->Branch("waveform_ch", &waveform_ch, "waveform_ch/D");
    //tree->Branch("waveform", &waveform);

    for(int i=0; i<33; i++){
      //event2d[i] = new TH2F(Form("event2d_%d",i),"event display gtr100 waveform",288,-0.5,287.5,30,-0.5,29.5);
    }

  }

  void SaveAndClose(){
    file->Write();
    file->Close();
    E16MESSAGE("histo end");
  }
  void Plot1(TPad* c1, TPad* c0){
    c1->Clear();
    c1->Divide(4,4);

    for( int i=0;i<7;i++){
      int  i1 = (i<3)?  i+1 : i+2;// i=3, 4th histo is 100y, cd->(5)
      int  i2 = i1 + 8;  
      c1->cd(i1) ;((TH1F*) *localcog[i])->Draw();
      c1->cd(i2);((TH1F*) *localtdc[i])->Draw();
    }//for
    c0->Draw();    c0->Update();
  }
  void Plot2(TPad* c1, TPad* c0){
    c1->Clear();
    c1->Divide(4,4);

    for( int i=0;i<7;i++){
      int  i1 = (i<3)?  i+1 : i+2;// i=3, 4th histo is 100y, cd->(5)
      int  i2 = i1 + 8;
      c1->cd(i1) ;((TH1F*) *localdiff[i])->Draw();
      c1->cd(i2);((TH1F*) *time[i])->Draw();
    }//for
    c0->Draw();    c0->Update();
  }

  void Plot3(TPad* c1, TPad* c0){
    c1->Clear();
    c1->Divide(3,4);

    for( int i=0;i<6;i++){
      int  i1 = i+1;
      int  i2 = i+1+6;
      c1->cd(i1) ;((TH1F*) *g4local[i])->Draw();
      c1->cd(i2);((TH1F*) *g4local2[i])->Draw("box");
    }//for
    c0->Draw();    c0->Update();
  }

  void PlotAngle(TPad* c1, TPad* c0){
    c1->Clear();
    c1->Divide(4,4);

    int i1s[6]={1,2,3,5,6,7};
    for( int i=0;i<6;i++){ 
      int  i1 = i1s[i];
      int  i2 = i1+8;
      c1->cd(i1) ;((TH1F*) *g4angle[i])->Draw();
      c1->cd(i2) ;((TH1F*) *g4incident[i])->Draw();

      //      if( i<3 ){
      //	c1->cd(i2);((TH1F*) *g4time[i])    ->Draw();
      //      }
    }//for
    c1->cd(4);   g4moduleid100->Draw();
    c1->cd(8);   g4moduleid200->Draw();
    c1->cd(12);  g4moduleid300->Draw();
    c1->cd(16);  module1->Draw("box");

    c0->Draw();    c0->Update();
  }
  void PlotG4Diff(TPad* c1, TPad* c0){
        c1->Clear();
	c1->Divide(3,4);

    for( int i=0;i<6;i++){
      int  i1 = i+1;
      int  i2 = i+1+6;
      c1->cd(i1) ;((TH1F*) *g4diff[i])->Draw();
      c1->cd(i2) ;((TH1F*) *g4diffc[i])->Draw();
    }//for
    c0->Draw();    c0->Update();
  }
  void PlotG4phiDiff(TPad* c1, TPad* c0){
    c1->Clear();
    c1->Divide(3,4);

    for( int i=0;i<6;i++){
      int  i1 = i+1;
      int  i2 = i+1+6;
      c1->cd(i1) ;((TH1F*) *g4phidiff[i])->Draw();
      c1->cd(i2) ;((TH1F*) *g4phidiffc[i])->Draw();
    }//for
    c0->Draw();    c0->Update();
  }
  void PlotPhiTime(TPad* c1, TPad* c0){
    c1->Clear();

    c1->Divide(3,4);

    c1->cd(7);phiTime100XY->Draw();
    c1->cd(8);phiTime200XY->Draw();
    c1->cd(9);phiTime300XY->Draw();
    c1->cd(10);phiTime100XYdiff->Draw();
    c1->cd(11);phiTime200XYdiff->Draw();
    c1->cd(12);phiTime300XYdiff->Draw();

    for( int i=0;i<6;i++){
      int  i1 = i+1;
      c1->cd(i1) ;((TH1F*) *phiTime[i])->Draw();
    }//for
    c0->Draw();    c0->Update();
  }
  void PlotFit2(TPad* c1, TPad* c0){
    c1->Clear();

    c1->Divide(4,4);

    c1->cd(1);chi21->Draw();
    c1->cd(2);mass11->Draw();
    c1->cd(3);mass21->Draw();
    c1->cd(4);dm1->Draw();
    dm1->Fit("gaus","","",-15.0, 15.0);
    c1->cd(5);dpp11->Draw();
    dpp11->Fit("gaus","","",-4.0, 4.0);
    c1->cd(6);dpp21->Draw();
    dpp21->Fit("gaus","","",-4.0, 4.0);
    c1->cd(7);origp11->Draw();
    c1->cd(8);origp21->Draw();

    c1->cd(9);chi22->Draw();
    c1->cd(10);mass12->Draw();
    c1->cd(11);mass22->Draw();
    c1->cd(12);dm2->Draw();
    dm2->Fit("gaus","","",-15.0, 15.0);
    c1->cd(13);dpp12->Draw();
    dpp12->Fit("gaus","","",-4.0, 4.0);
    c1->cd(14);dpp22->Draw();
    dpp22->Fit("gaus","","",-4.0, 4.0);
    c1->cd(15);origp12->Draw();
    c1->cd(16);origp22->Draw();

    c0->Draw();    c0->Update();
  }

  void Plot6(TPad* c1,TPad* c0, TPostScript* psfile,TH1F* ptr[6][33]){
    c1->Clear();

    int place66[36]={13,7,1, 14,8,2, 15,9,3, 16,10,4, 17,11,5, 18,12,6,
	     31,25,19, 32,26,20, 33,27,21, 34,28,22, 35,29,23, 36,30,24};

    for( int i=0;i<6;i++){
      psfile->NewPage();
      c1->Clear();
      c1->Divide(6,6);

      for( int j=0;j<33;j++){
	//		cerr<<"plot5 "<<i<<" "<<j<<endl;
	c1->cd(place66[j]) ;  ptr[i][j]->Draw();
      }//for j
      
      c0->Draw();
      c0->Update();
    }//for i

  }

  void Plot7(TPad* c1,TPad* c0, TPostScript* psfile, TH1F* ptr[7][33]){
    c1->Clear();

    int place66[36]={13,7,1, 14,8,2, 15,9,3, 16,10,4, 17,11,5, 18,12,6,
	     31,25,19, 32,26,20, 33,27,21, 34,28,22, 35,29,23, 36,30,24};

    for( int i=0;i<7;i++){
      psfile->NewPage();
      c1->Clear();
      c1->Divide(6,6);

      for( int j=0;j<33;j++){
	c1->cd(place66[j]) ;  ptr[i][j]->Draw();
      }//for j
      
      c0->Draw();      c0->Update();
    }//for i
    cerr<<"plot7 end"<<endl;
  }
  void PlotFitResidual(TPad* c1, TPad* c0){
    c1->Clear();

    c1->Divide(4,4);
    c1->cd(1); fitres1001x->Draw();
    c1->cd(2); fitres2001x->Draw();
    c1->cd(3); fitres3001x->Draw();
    c1->cd(4); dvx->Draw();
    dvx->Fit("gaus","","",-3, 3);

    c1->cd(5); fitres1002x->Draw();
    fitres1002x->Fit("gaus","","",-0.2, 0.2);
    c1->cd(6); fitres2002x->Draw();
    fitres2002x->Fit("gaus","","",-0.2, 0.2);
    c1->cd(7); fitres3002x->Draw();
    fitres3002x->Fit("gaus","","",-0.2, 0.2);

    c1->cd(9); fitres1001y->Draw();
    c1->cd(10); fitres2001y->Draw();
    c1->cd(11); fitres3001y->Draw();
    c1->cd(12); dvy->Draw();
    dvy->Fit("gaus","","",-3, 3);

    c1->cd(13); fitres1002y->Draw();
    fitres1002y->Fit("gaus","","",-0.8, 0.8);
    c1->cd(14); fitres2002y->Draw();
    fitres2002y->Fit("gaus","","",-0.8, 0.8);
    c1->cd(15); fitres3002y->Draw();
    fitres3002y->Fit("gaus","","",-0.8, 0.8);
    c1->cd(16); dvz->Draw();
    dvz->Fit("gaus","","",-0.2, 0.2);

    c0->Draw();      c0->Update();
  }
  void PlotFit(TPad* c1, TPad* c0){
    c1->Clear();

    c1->Divide(4,5);
    c1->cd(1); chi2->Draw();
    c1->cd(2); mass1->Draw();
    c1->cd(3); mass2->Draw();
    c1->cd(4); dm->Draw();
    dm->Fit("gaus","","",-15.0, 15.0);

    c1->cd(5); gPad->SetLogy();dp1->Draw();
    c1->cd(6); gPad->SetLogy();dp2->Draw();
    c1->cd(7); gPad->SetLogy(false);dpp1->Draw();
    dpp1->Fit("gaus","","",-4.0, 4.0);
    c1->cd(8); dpp2->Draw();
    dpp2->Fit("gaus","","",-4.0, 4.0);


    c1->cd(9); fitvx->Draw();
    fitvx->Fit("gaus","","",-4.0, 4.0);
    c1->cd(10); fitvy->Draw();
    fitvy->Fit("gaus","","",-4.0, 4.0);
    c1->cd(11); fitvz->Draw();
    c1->cd(12); fitstat->Draw();
    c1->cd(13); origvx->Draw();
    origvx->Fit("gaus","","",-4.0, 4.0);
    c1->cd(14); origvy->Draw();
    origvy->Fit("gaus","","",-4.0, 4.0);
    c1->cd(15); origvz->Draw();
    c1->cd(16);  bg1->Draw();

    // c1->cd(10); fitvxy->Draw();
    // c1->cd(11); bg1->Draw();
    c1->cd(17); origp1->Draw();
    c1->cd(18); origp2->Draw();
    c1->cd(19); fitp1->Draw();
    c1->cd(20); fitp2->Draw();

    c0->Draw();      c0->Update();
  }

 void Date(char* buf){
    time_t     now;
    struct tm  *ts;
    //    char       buf[80];
 
    // Get the current time
    now = std::time(NULL);
 
    // Format and print the time, "ddd yyyy-mm-dd hh:mm:ss zzz"
    ts = localtime(&now);
    //    strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", ts);
    strftime(buf, 40, "%Y-%m-%d %H:%M", ts);
    //    printf("%s\n", buf);
    //    return buf;
}


  void PrintPS(){
    //for the time being, works only in 'dynamic link'

    E16MESSAGE("histo print start");

    //    gROOT->SetStyle("Plain");//cannot compile 
    //    gStyle->SetStyle("Plain"); //cannot compile also
    gStyle->SetOptStat(1111111);
    gStyle->SetOptFit(111);
    
    gStyle->SetStatW(0.25);
    gStyle->SetStatH(0.25);
    gStyle->SetStatFontSize(0.06);
    gStyle->SetStatFormat("6.2f");
    gStyle->SetFitFormat("5.3f");
    //    gStyle->SetStatStyle(0)

    TCanvas* c0=new TCanvas("c0","c0",10,10,2000,1500);

    //x1,y1,x2,y1
    TPad* c1=new TPad("c1","c1", 0.0, 0.0, 1.0, 0.97);
    TPad* header=new TPad("header","header", 0.0, 0.97, 1.0, 1.0);
    //  c1->SetFillColor(2);
    //  c2->SetFillColor(3);
    c1->Draw();
    header->Draw();

    header->cd();
    TText* label = new TText();
    char labeltext[512];char datetext[40];
    Date(datetext);
    //    sprintf(labeltext,"%s  %s", datetext, outputfilename->c_str() );
    sprintf(labeltext,"%s  %s %s", datetext, outputfilename->c_str(), 
      globalARGstr.str() );
    label->SetTextSize(0.5);
    label->DrawText(0.1, 0.1, labeltext);

    //------------------

    TPostScript* psfile = new TPostScript(psfilename->c_str(), 112);

    psfile->NewPage();
    PlotFit(c1,c0);

    psfile->NewPage();
    PlotFitResidual(c1,c0);

    psfile->NewPage();
    Plot1(c1,c0);

    psfile->NewPage();
    Plot2(c1,c0);

    psfile->NewPage();
    Plot3(c1,c0);

    psfile->NewPage();
    PlotAngle(c1,c0);

    psfile->NewPage();
    PlotG4Diff(c1,c0);

    psfile->NewPage();
    PlotG4phiDiff(c1,c0);

    psfile->NewPage();
    PlotPhiTime(c1,c0);

    psfile->NewPage();
    PlotFit2(c1,c0);


#if 0
    Plot6(c1,c0, psfile, g4locals);
    Plot6(c1,c0, psfile, g4diffs);

    Plot7(c1,c0, psfile, localcogs);
    Plot7(c1,c0, psfile, localtdcs);
    Plot7(c1,c0, psfile, localdiffs);
#endif

    psfile->Close();

    E16MESSAGE("histo print end");
  }


  ~Histo(){
    delete outputfilename;
    delete psfilename;
  }

};

//const int Histo::NTIMEBIN;
//const int Histo::OFFSETBIN;


//----------------------------------------------------------

class E16ANA_GTRStripG4ResponsePair{
private:
  E16ANA_G4Hit* g4hit;
  E16ANA_GTRAnalyzedStripHit* strip;
public:
  E16ANA_GTRStripG4ResponsePair(  
    E16ANA_G4Hit* ig4, E16ANA_GTRAnalyzedStripHit* istrip):
    g4hit(ig4), strip(istrip){
  }
  E16ANA_G4Hit* G4Hit(){return g4hit;}
  E16ANA_GTRAnalyzedStripHit* StripHit(){return strip;}

  E16ANA_GTRAnalyzedStripHit* Match(E16ANA_G4Hit* ig4){
    double x1 = ig4->XTV().X();
    double x2 = g4hit->XTV().X();
    //    cerr<<"match "<<ig4<<" "<<g4hit<<" "<<x1<<" "<<x2<<endl;

    E16ANA_GTRAnalyzedStripHit* ptr = ( fabs(x1-x2)< std::numeric_limits<double>::epsilon() ) ? (strip) : NULL;

    return ptr;
  }

};

class E16ANA_GTRResponseAnalysis{
private:

  E16ANA_GTRResponseManager* gtrRes;
  E16ANA_GTRAnalyzerManager* gtrAna;

  vector<E16ANA_GTRStripG4ResponsePair> nearestXStripList;
  vector<E16ANA_GTRStripG4ResponsePair> nearestYStripList;

  void EventAnalysis(E16ANA_G4Event& g4event, 
    E16ANA_G4ModuleHitList g4moduleHits);

  
public:

  E16ANA_GTRResponseAnalysis(  E16ANA_GTRResponseManager* igtrRes, 
    E16ANA_GTRAnalyzerManager* igtrAna):
    gtrRes( igtrRes ),  gtrAna( igtrAna ){

  }

  ~E16ANA_GTRResponseAnalysis(){}


  void G4EventAnalysis(E16ANA_G4Event& g4event);
  void WaveformEventAnalysis(E16ANA_G4Event& g4event, E16ANA_WaveformEvent *wfevent);

  void Fitting( E16ANA_TrackPairCandidateV0& pair, bool flag1);

  bool PairSelectionG4(E16ANA_G4Event& g4event,
    E16ANA_TrackPairCandidateV0& pair);



  bool PairSelectionResponse(
    E16ANA_TrackPairCandidateV0& pairG4, 
    E16ANA_TrackPairCandidateV0& pairCOG, 
    E16ANA_TrackPairCandidateV0& pairTOF,
    bool& flag_cogNarrow);


  double InvMass(double m1, double m2, TVector3 p1, TVector3 p2);

private:
  void AnalyzedHitSimpleHisto();
  void ReAnalysisUsingNearestG4Hit(E16ANA_G4ModuleHitList& moduleHits);
  void NearestXYReanalysis(  E16ANA_G4Hit& ghit,
    vector<E16ANA_GTRAnalyzedHit>& vecAhitX,
    vector<E16ANA_GTRAnalyzedHit>& vecAhitY);
  E16ANA_GTRAnalyzedStripHit*  NearestYstripSearch(E16ANA_G4Hit* g4hit);

};


//--------------------------------------------------------

double E16ANA_GTRResponseAnalysis::InvMass(double m1, double m2,
  TVector3 p1, TVector3 p2){

    double E1 = sqrt ( m1*m1 + p1 * p1 );
    double E2 = sqrt ( m2*m2 + p2 * p2 );
    double mm = (E1+E2)*(E1+E2) - (p1+p2)*(p1+p2);
    double m = sqrt(mm);
    return m;
}

E16ANA_GTRAnalyzedStripHit* E16ANA_GTRResponseAnalysis::NearestYstripSearch(
  E16ANA_G4Hit* g4hit){

  for( int j=0; j< (int)nearestYStripList.size() ;j++){
    E16ANA_GTRAnalyzedStripHit* hit = nearestYStripList[j].Match(g4hit);
    if( hit == NULL ){
      //	E16WARNING("no G4hit-response pair %d",i);
      continue;
    }
    else{//match
      return hit;
    }
  }//j

  return NULL;
  
}

bool E16ANA_GTRResponseAnalysis::PairSelectionResponse(
		       E16ANA_TrackPairCandidateV0& pairG4, 
		       E16ANA_TrackPairCandidateV0& pairCOG, 
		       E16ANA_TrackPairCandidateV0& pairTOF,
		       bool& flag_cogNarrow){

  // use G4 pair and select nearest response
  cerr<<"pairselection Response "<<endl;

  // module-hit-loop is not required....

  int match[6]={0};
  double ysigma = 0.03;//cm : 300um

  flag_cogNarrow = true;

  for(int i=0; i<6; i++){
    E16ANA_G4Hit* g4hit=pairG4.G4Hits(i);
    double x = pairG4.Xs()[i].X();//hit local x
    double y = pairG4.Xs()[i].Y();//hit local y
    double ygauss = gRandom->Gaus( y, ysigma);
    double z = pairG4.Xs()[i].Z();//hit local y
    int k = pairG4.ModuleIDs()[i];
    int layer = i%3;//chamber type: 100/200/300=0/1/2

    //    cerr<<" response1 "<<i<<" "<<x<<" "<<nearestXStripList.size()<<" "<<g4hit<<endl;

    for( int j=0; j< (int)nearestXStripList.size() ;j++){
      E16ANA_GTRAnalyzedStripHit* hit = nearestXStripList[j].Match(g4hit);
      if( hit == NULL ){
	//	E16WARNING("no G4hit-response pair %d",i);
	continue;
      }
      else{//match
	match[i]=1;

	double hx1=hit->CogHit();//mm
	double hx2=hit->TdcHit();//mm

	histo->G4phiDiff( hit->LayerID(), hit->ModuleID(), 
	  g4hit->X()*10-hx2, g4hit->X()*10 - hx1);

	if( fabs(g4hit->X()*10 - hx1) > RESIDUAL_THR){
	  flag_cogNarrow *= false;
	}

	E16ANA_GTRAnalyzedStripHit* hity = NearestYstripSearch(g4hit);
	double hy=-9999;
	if ( hity != NULL ){
	  hy = hity->CogHit() *0.1;
	  histo->G4phiDiff( hit->LayerID() +3 , hit->ModuleID(), 
	    g4hit->Y()*10 - hity->TdcHit(), g4hit->Y()*10 - hy*10 );

	  histo->PhiTimeXY( hit->LayerID()  , hit->ModuleID(), 
	    hit->Timing(), hity->Timing() );

	  double diff = hit->Timing() - hity->Timing() ;
	//	cerr<<"hit->Timing "<<diff<<" " <<hit->Timing() <<" "<<hity->Timing()<<endl; //zero

	}
	else{
	  E16WARNING("no G4hit-response y %d",i);
	  hy = ygauss;//cm :  effectively there is no case
	}

	double hz=z;
	//	cerr<<" response2 "<<i<<" "<<x*10<<" "<<hx1<<" "<<hx2<<endl;

	TVector3 local1( hx1*0.1, hy, z);//cm
	TVector3 local2( hx2*0.1, hy, z);//cm
	//	TVector3 gX1 =GTRLocalToGlobal( k, type, local1);
	TVector3 gX1 =E16ANA_Transform::GetGPos(local1, geom, layer,k );
	TVector3 gX2 =E16ANA_Transform::GetGPos(local2, geom, layer,k );

	pairCOG.Xs()[i]  =local1;
	pairCOG.GXs()[i] =gX1;

#if 0
	pairTOF.Xs()[i]  =local2;
	pairTOF.GXs()[i] =gX2;
#else
	double tanThetaX = fabs( g4hit->PTV().X()/g4hit->PTV().Z() );
	
	if( tanThetaX * RadToDeg > 10 ){ //use TOF for large angle
	  pairTOF.Xs()[i]  =local2;
	  pairTOF.GXs()[i] =gX2;
	  E16INFO("phi-tdc");
	}
	else{ //use COG for small angle
	  pairTOF.Xs()[i]  =local1;
	  pairTOF.GXs()[i] =gX1;
	  E16INFO("phi-cog");
	}
#endif

	break;//j-loop
      }//not null

    }//for j-x


  }//i : pair-layer loop 0-5

  int flag =0;
  for (int i=0; i<6; i++){    flag += match[i] ;}
  cerr<<"response match "<<flag<<endl;;
  // 
  //  cerr<<endl;
  if( flag == 6 ){  return true; }
  else{
    E16WARNING("no G4hit-response pair %d",flag);
    return false;
  }
}

bool E16ANA_GTRResponseAnalysis::PairSelectionG4(
  E16ANA_G4Event& g4event,
  E16ANA_TrackPairCandidateV0& pair ){

  int noh1 = g4event.GTR1().Noh();
  int noh2 = g4event.GTR2().Noh();
  int noh3 = g4event.GTR3().Noh();

  //  int noh = noh1 + noh2 + noh3;

  int  copied[6]={0};

  // first track  : positive
  E16ANA_G4Hit h11;

  for (int i=0;i< noh1;i++){
    E16ANA_G4Hit& h = g4event.GTR1().Hit(i);
    if ( h.PID() !=  -11 ) {continue;}  // -11 is e+
    h11 = h;
    pair.SetHit( 0,  h );
    copied[0] =1;
    break;
  }

  if ( copied[0]  == 0 ) {
    for(int i=0; i< noh1 ;i++){
	cerr<<i<<":";g4event.GTR1().Hit(i).Dump("GTR-1-no-e+");
    }  
    return false;
  }  //no 1st track candidate

  double p1 = h11.PV().mag();

  for (int i=0;i< noh2;i++){
    E16ANA_G4Hit& h = g4event.GTR2().Hit(i);
    if ( h.PID() !=  h11.PID() ) {continue;}
    if ( h.PV().mag() - p1  > 0.01 ){continue;}
    pair.SetHit( 1,  h );
    copied[1] =1;
    break;
  }
  for (int i=0;i< noh3;i++){
    E16ANA_G4Hit& h = g4event.GTR3().Hit(i);
    if ( h.PID() !=  h11.PID() ) {continue;}
    if ( h.PV().mag() - p1  > 0.01 ){continue;}
    pair.SetHit( 2,  h ); 
    copied[2] =1;
    break;
  }
  // second track
  E16ANA_G4Hit h21;

  for (int i=0; i< noh1;i++){
    E16ANA_G4Hit& h = g4event.GTR1().Hit(i);
    if ( h.PID() !=  11  ) {continue;} //11 is e-
    h21 = h;
    pair.SetHit( 3,  h  ); 
    copied[3] =1;
    break;
  }

  if ( copied[3]  == 0 ) {
    for(int i=0; i< noh1 ;i++){
	cerr<<i<<":";g4event.GTR1().Hit(i).Dump("GTR-1-fail");
    }  
    return false;
  }  //no 2nd track candidate

  double p2 = h21.PV().mag();

  for (int i=0;i< noh2;i++){
    E16ANA_G4Hit& h = g4event.GTR2().Hit(i);
    if ( h.PID() !=  h21.PID() ) {continue;}
    if ( h.PV().mag() - p2  > 0.01 ){continue;}
    pair.SetHit( 4,  h ); 
    copied[4] =1;
    break;
  }
  for (int i=0;i< noh3;i++){
    E16ANA_G4Hit& h = g4event.GTR3().Hit(i);
    if ( h.PID() !=  h21.PID() ) {continue;}
    if ( h.PV().mag() - p2  > 0.01 ){continue;}
    pair.SetHit( 5,  h ); 
    copied[5] =1;
    break;
  }

  //  cerr<<"mom "<<p1<<" "<<p2<<endl;

  //check
  int nullCount =0;
  cerr<<"pair ids ";
  for (int i=0; i<6; i++){
    cerr<< ( pair.ModuleIDs())[i] <<" ";
    if( copied[i] == 0 ){
      E16WARNING("null %d %d",i, nullCount);
      nullCount ++;
    }
  }
  cerr<<endl;

  if (nullCount >0){return false;}
  
  
  return true;

}

void E16ANA_GTRResponseAnalysis::Fitting(E16ANA_TrackPairCandidateV0& pair,
  bool flag1){

   const double emass=0.000511;

   //   double xvtxerr=0.15, yvtxerr=0.17, zvtxerr=0.004;//[cm]
      double xvtxerr=250e-4, yvtxerr=250e-4, zvtxerr=40e-4;//[cm]
   //   double xvtxerr=fitError[0]*1e-4;
   //   double yvtxerr=fitError[1]*1e-4;

   E16ANA_DoubleTrack* dtrk= 
     new E16ANA_DoubleTrack(fmf, geom,
			    xvtxerr,yvtxerr,zvtxerr);

   TMinuit* dmin=new TMinuit(dtrk->NParam());
   dmin->SetObjectFit(dtrk);
   dtrk->SetMinuit(dmin);

#if 1

   //    TVector3 crossPoint(0,0,0);
      TVector3 crossPoint= pair.Vertex()*1.01;
   //   TVector3 crossPoint= SingleTrackCross( pair );

      double errX[3]={80e-4, 80e-4, 80e-4};
      double errY[3]={270e-4, 270e-4, 270e-4 };

   dtrk->Initialize(pair.P1()*1.01, pair.P2()*1.01, 
     crossPoint, pair.ModuleIDs(), errX, errY );

   dtrk->SetLGPos( pair.Xs(), pair.GXs()  );

   //   dtrk->Dump();

     bool vertexFix  = true;
     bool vertexNotFix = false;
   double chi2 = dtrk->Fit( vertexNotFix );

   int stat = dtrk->MinimizeStatus();
   TVector3 p0  = dtrk->GetP0vtx();
   TVector3 p1  = dtrk->GetP1vtx();
   TVector3 fitvtx = dtrk->GetXYZvtx();


   double invmassorg = InvMass(emass, emass, pair.P1(), pair.P2() );
   double invmassfit = dtrk->InvMass(emass, emass);

   E16MESSAGE("chi2/stat/m/mfit=%7.4f %2d %7.4f %7.4f\n", chi2, 
	   stat, invmassorg, invmassfit);

#endif

   bool flag2 = histo->DoubleFitResidual( dtrk );

   histo->DoubleFit(chi2, stat, pair.P1(), pair.P2(), p0, p1, 
     pair.Vertex(), fitvtx, 
     invmassorg, invmassfit, dtrk->MinimizeStatus(), dtrk->MatrixStatus(),
     flag1, flag2 );

   delete dtrk;
   delete dmin;

}

void E16ANA_GTRResponseAnalysis::NearestXYReanalysis(
  E16ANA_G4Hit& ghit,
  vector<E16ANA_GTRAnalyzedHit>& vecAhitX,
  vector<E16ANA_GTRAnalyzedHit>& vecAhitY
){
  int size2 = vecAhitX.size();
  int size3 = vecAhitY.size();
  //  E16INFO("size x,y = %d , %d", size2, size3);

  double xdiffmin  = 1000;//mm
  double ydiffmin  = 1000;//mm

  E16ANA_GTRAnalyzedHit* nearestX=NULL;
  E16ANA_GTRAnalyzedHit* nearestY=NULL;
  
  bool flag2 = true;

  for( int j=0 ; j < size2; j++){
    E16ANA_GTRAnalyzedHit& ahitX = vecAhitX[j];
    //    double xdiff = ahitX.Position().X() - ghit.GX()*10;
    if ( ghit.ModuleID() != ahitX.XstripHit()->ModuleID() ) {continue;}

    double xdiff = fabs( ghit.X()*10 - ahitX.XstripHit()->CogHit() );
    if( xdiff < xdiffmin ){ 
      xdiffmin = xdiff;
      nearestX = & ahitX;
    }
  }//ahitX
	
  if (nearestX != NULL){
    E16ANA_GTRAnalyzedStripHit* hit= nearestX -> XstripHit();
    //    cerr<<"in nearest 1 "<<hit->Timing()<<endl;

    double tx1 = hit->TdcHit() ;
    double tanThetaX = ghit.PTV().X()/ghit.PTV().Z();
    //re-analysis
    hit->SetTanTheta( tanThetaX );
    gtrAna -> ChamberAnalyzer( hit->LayerID(), hit->ModuleID() ) 
      -> Analyze2X( hit->ID() );
    double tx2 = hit->TdcHit() ;//mm    
    //    cerr<<"reanalysisX "<<ghit.X()*10<<" "<<tx1<<" "<<tx2<<" "<<tanThetaX<<endl;

    histo->G4Diff( hit->LayerID(), hit->ModuleID(), 
      ghit.X()*10-tx2, ghit.X()*10 - hit->CogHit());

    E16ANA_GTRStripG4ResponsePair pairX(&ghit, hit );
    nearestXStripList.push_back(pairX);//copy
    //    cerr<<"in nearest 2 "<<hit->Timing()<<" "<< pairX.StripHit()->Timing()<<endl;
    //zero,zero
  }

  for( int jj=0 ;jj < size3; jj++){
    E16ANA_GTRAnalyzedHit& ahitY = vecAhitY[jj];
    if ( ghit.ModuleID() != ahitY.YstripHit()->ModuleID() ) {continue;}

    if ( nearestX == NULL) {continue;} 
    double xytdiff=fabs( nearestX->XstripHit()->Timing() - ahitY.YstripHit()->Timing() );
    if( xytdiff > 100 ){
      continue;
    }
    //    cerr<<"xytdiff = "<<xytdiff<<" "<<nearestX->XstripHit()->Timing()
    //	<<" "<<ahitY.YstripHit()->Timing() << endl;

    double ydiff = fabs( ghit.Y()*10 - ahitY.YstripHit()->CogHit() );
    if( ydiff < ydiffmin ){ 
      ydiffmin = ydiff;
      nearestY = & ahitY;
    }
  }//ahitY
  if (nearestY != NULL){
    E16ANA_GTRAnalyzedStripHit* hit= nearestY -> YstripHit();
    double ty1 = hit->TdcHit() ;
    TVector3 p = ghit.PTV();
    double pxz = sqrt( p.X()*p.X() + p.Z()*p.Z() );
    double tanThetaY =  p.Y()/pxz;//local tan
    //re-analysis
    hit->SetTanTheta( tanThetaY );
    gtrAna -> ChamberAnalyzer( hit->LayerID(), hit->ModuleID() ) 
      -> Analyze2Y( hit->ID(), hit->Type() );
    double ty2 = hit->TdcHit() ;//mm    
    //    cerr<<"reanalysisY "<<ghit.Y()*10<<" "<<ty1<<" "<<ty2<<" "<<tanThetaY<<endl;
    histo->G4Diff( hit->LayerID()+3, hit->ModuleID(), 
      ghit.Y()*10-ty2, ghit.Y()*10 - hit->CogHit() );

    E16ANA_GTRStripG4ResponsePair pairY(&ghit, hit );
    nearestYStripList.push_back(pairY);//copy
  }

}



void E16ANA_GTRResponseAnalysis::ReAnalysisUsingNearestG4Hit(
  E16ANA_G4ModuleHitList& g4moduleHits){
  //module-by-module access

#if 1
  for (int k=0; k< 33 ;k++){   //module-by-module
    for( int layer = 0; layer < 3 ;  layer ++){
      vector<E16ANA_G4Hit>& vecG4 = g4moduleHits.GTRModule(layer, k) ;
      vector<E16ANA_GTRAnalyzedHit>& vecAhitX = gtrAna->AnalyzedHitXChamber(layer, k);
      vector<E16ANA_GTRAnalyzedHit>& vecAhitY = gtrAna->AnalyzedHitYChamber(layer, k);

      int size  = vecG4.size();
      //      E16INFO("layer, k, size = %d %d: %d %d %d", layer, k, size,
      //	vecAhitX.size(),vecAhitY.size() );

      for( int i=0; i< size ; i++){// loop
	E16ANA_G4Hit& ghit = vecG4[i];

	NearestXYReanalysis(ghit, vecAhitX, vecAhitY);

      }//g4hit

    }//layer
  }//module
#endif

  

#if 0
     if( hit1.EventID()< 10000 ){continue;}//not overlapped phi->ee
     //if( hit1.EventID()< 0 ){continue;}//not overlapped phi->ee
#endif

}
#if 0
// see histo->G4ModuleHit()
void E16ANA_GTRResponseAnalysis::G4HitSimpleHisto(E16ANA_G4Event& g4event){


  for(int layerID=0; layerID<3; layerID++){
    //    int jmax = g4event.GTRs( layerID ).Noh();
    int jmax = g4event.GTRs( layerID ).Noh();
    //    cerr<<"jmax "<<jmax<<endl;
    for (int j=0; j< jmax ;j++){
      E16ANA_G4Hit & hit = g4event.GTRs( layerID ).Hit(j);//reference
      int moduleID = hit.DetectorID();
      //      int pid = hit.PID();
      //      if( pid == 11 || pid == -11){
      	histo->G4Local(layerID, moduleID, hit);
    }//for j
  }//for layer

}
#endif

void E16ANA_GTRResponseAnalysis::AnalyzedHitSimpleHisto(){
  //layer-wide access
  //count
#if 1
   cerr<<"AHSH  ";
  cerr<<(int)gtrAna->AnalyzedGTRX(0).size()<<" ";
  cerr<<(int)gtrAna->AnalyzedGTRX(1).size()<<" ";
  cerr<<(int)gtrAna->AnalyzedGTRX(2).size()<<" ";
  cerr<<(int)gtrAna->AnalyzedStripGTR1X().size()<<" ";
  cerr<<(int)gtrAna->AnalyzedStripGTR2X().size()<<" ";
  cerr<<(int)gtrAna->AnalyzedStripGTR3X().size()<<" "<<endl;
#endif

  int counter =0;
  for (int i = 0; i< (int)gtrAna->AnalyzedStripGTR1X().size();i++){
    E16ANA_GTRAnalyzedStripHit* shit =  gtrAna->AnalyzedStripGTR1X()[i];
    int id1 = shit->ModuleID() ;
    for (int i2 = 0; i2< (int)gtrAna->AnalyzedStripGTR2X().size();i2++){
      E16ANA_GTRAnalyzedStripHit* shit2 =  gtrAna->AnalyzedStripGTR2X()[i2];
      int id2 = shit2->ModuleID() ;
      int diff2 = id1 - id2;
            if( fabs(diff2) > 1 ) {continue;}

      for (int i3 = 0; i3< (int)gtrAna->AnalyzedStripGTR3X().size();i3++){
	E16ANA_GTRAnalyzedStripHit* shit3 =  gtrAna->AnalyzedStripGTR3X()[i3];
	int id3 = shit3->ModuleID() ;
	int diff3 = id2 - id3;
		if( fabs(diff3) > 1 ) {continue;}
	counter ++;
	//	fprintf(stderr, "%02d %02d %02d,%8d\r", id1, id2, id3, counter);


#if 0
	for (int i4 = 0; i4< (int)gtrAna->AnalyzedGTRX1().size();i4++){
	  for (int i5 = 0; i5< (int)gtrAna->AnalyzedGTRX2().size();i5++){
	    for (int i6 = 0; i6< (int)gtrAna->AnalyzedGTRX2().size();i6++){
	      counter ++;
	    }//i6
	  }//i5
	}//i4
#endif
      }//i3
    }//i2
  }//i

  fprintf(stderr, "\n");

#if 1
  //response analysis---------------------
  for( int layer=0; layer<3 ; layer++){
    for (int i = 0; i< (int)gtrAna->AnalyzedGTRX(layer).size();i++){
      //   cerr<<"Nhit "<< gtrAna->AnalyzedGTRX(layer).size()<<endl;
      E16ANA_GTRAnalyzedStripHit* shit =  gtrAna->AnalyzedStripGTRX(layer)[i];
      E16ANA_GTRAnalyzedHit* ahit =  gtrAna->AnalyzedGTRX(layer)[i];
      histo->GTRLocal( layer, *shit );
      histo->GTRGlobal( layer, shit->ModuleID(), ahit->Position() );
    }//i
    for (int i = 0; i< (int)gtrAna->AnalyzedGTRY(layer).size();i++){
      E16ANA_GTRAnalyzedStripHit* shit =  gtrAna->AnalyzedStripGTRY(layer)[i];
      E16ANA_GTRAnalyzedHit* ahit =  gtrAna->AnalyzedGTRY(layer)[i];
      histo->GTRLocal( layer+3, *shit );
      histo->GTRGlobal( layer+3, shit->ModuleID(), ahit->Position() );
    }//i
  }//layer

  for (int i = 0; i< (int)gtrAna->AnalyzedGTR1Yb().size();i++){
    E16ANA_GTRAnalyzedStripHit* shit =  gtrAna->AnalyzedStripGTR1Yb()[i];
      E16ANA_GTRAnalyzedHit* ahit =  gtrAna->AnalyzedGTR1Yb()[i];
    histo->GTRLocal( 6, *shit );
    histo->GTRGlobal( 6, shit->ModuleID(), ahit->Position() );
    
  }//i

#endif


}
void E16ANA_GTRResponseAnalysis::G4EventAnalysis(E16ANA_G4Event& g4event){
 E16ANA_G4ModuleHitList g4moduleHits( g4event );

  cerr<<" ---------------------"<<endl;
  //  g4moduleHits.HitPrint();
  //  histo->G4ModuleHit( g4moduleHits );

  nearestXStripList.clear();
  nearestYStripList.clear();

#ifdef SINGLEOVERLAP
  E16ANA_EventOverlap::SetGeometry( geom );
  //E16ANA_EventOverlap::SetSingleRate(mean100, mean200, mean300, backRatio100);
  E16ANA_EventOverlap::SetSingleRateDefault();
    g4moduleHits.OverlapGTRSingleHits();
    //      g4moduleHits.OverlapGTRSingleHits151201();
  int tm=clock();
  cerr<<"overlap end "<<tm<<" "<<tm/CLOCKS_PER_SEC<<endl;
#endif

  histo->G4ModuleHit( g4moduleHits );

  // gtrAna->G4AnalyzeV0( g4moduleHits, gtrRes, geom);
  //gtrAna->G4AnalyzeV1( g4moduleHits, gtrRes, geom);

  //global coordinate are saved here for the global tracking

  int tm2=clock();
  cerr<<"G4Analyze end "<<tm2<<" "<<tm2/CLOCKS_PER_SEC<<endl;

  EventAnalysis(g4event, g4moduleHits);


}

void E16ANA_GTRResponseAnalysis::EventAnalysis(E16ANA_G4Event& g4event, 
 E16ANA_G4ModuleHitList g4moduleHits){

  AnalyzedHitSimpleHisto();//sample of layer-wide hit list

  int tm3=clock();
  cerr<<"AHSH end "<<tm3<<" "<<tm3/CLOCKS_PER_SEC<<endl;


  ReAnalysisUsingNearestG4Hit( g4moduleHits );
  //sample of chamber-by-chamber hit list
  ///  nearestX/YStripList is set in this method

  //ToDo;  
  // track find using g4hit
  // fit




  //Fitting-----------------------------------------------

#define FIT
#ifdef FIT
  cerr<<" fit - - - - - - - - - - "<<endl;
  g4event.HeaderDump();
  int nprimary = g4event.PrimaryTracks().Noh();

  vector<int> phiee;

  cerr<<" track evid ";
  for (int i=0; i< nprimary ; i++){
    cerr<< g4event.PrimaryTracks(i).EventID() <<" ";
    if( g4event.PrimaryTracks(i).EventID() > 10000 ){
      phiee.push_back( i );
    }
  }

  E16ANA_G4Track* t1=NULL;
  E16ANA_G4Track* t2=NULL;
  E16ANA_TrackPairCandidateV0*  pairG4=NULL;
  E16ANA_TrackPairCandidateV0*  pairCOG=NULL;
  E16ANA_TrackPairCandidateV0*  pairTOF=NULL;
  bool only2Track = true;

  if ( phiee.size() == 2){
    cerr<<"only 2track > 10000 "<<endl;  //overlay by 2nd file
    t1 = & g4event.PrimaryTracks( phiee[0] );
    t2 = & g4event.PrimaryTracks( phiee[1] );
  }
  else if ( nprimary == 2){                  //only phi in 1st file
    cerr<<"only 2track <10000 "<<endl;
    t1 = & g4event.PrimaryTracks(0);
    t2 = & g4event.PrimaryTracks(1);
  }
  else{
    E16WARNING("not only2Track : no t1/t2");
    only2Track = false;
  }



 if( only2Track ){
    cerr<<"paircand vertex "<< t1->Vertex()<<" "<<t2->Vertex()<<endl;
    double diffz = t1->Vertex().z() - t2->Vertex().z();
    if( diffz > 1.0 ) {
      E16INFO("eepair vertex cand diff size2 %5.3f",diffz);
    }

    if ( t1->PID() == - 11){  //e+
      pairG4= new E16ANA_TrackPairCandidateV0(t1->PTV(),t2->PTV(),t1->TVertex());
    }
    else if ( t1->PID() == + 11){ //e-
      pairG4= new E16ANA_TrackPairCandidateV0(t2->PTV(),t1->PTV(),t2->TVertex());
    }
    else{
      E16WARNING("not e+/e- %d %d", t1->PID(), t2->PID() );
    }

    t1->Dump();
    t2->Dump();

    if ( pairG4 != NULL ){
      if ( PairSelectionG4( g4event, *pairG4) ) {
	E16ANA_TrackPairCandidateV0 pairCOG = *pairG4;//initialize by copy
	E16ANA_TrackPairCandidateV0 pairTOF = *pairG4;//initialize by copy

	bool flag_cogNarrow=true;
	//Fitting( *pairG4 );
	if ( PairSelectionResponse(*pairG4, pairCOG, 
	                           pairTOF, flag_cogNarrow ) ){
	  	//	Fitting( pairCOG );
	  Fitting( pairTOF, flag_cogNarrow );
	}

      }//pairselected
    }//pairG4

    delete pairG4;

 }//if only2track

#endif//FIT

  //Fitting end-----------------------------------------------

    cerr<<" G4eventANA end - - - - - - - - - - "<<endl;



}

void E16ANA_GTRResponseAnalysis::WaveformEventAnalysis(E16ANA_G4Event& g4event, E16ANA_WaveformEvent *wfevent){


  E16ANA_G4ModuleHitList g4moduleHits( g4event );

  cerr<<" ---------------------"<<endl;
  //  g4moduleHits.HitPrint();
  histo->G4ModuleHit( g4moduleHits );

  nearestXStripList.clear();
  nearestYStripList.clear();

  //  gtrAna->WaveformAnalyzeV0( wfevent, geom);
   gtrAna->WaveformAnalyzeV1( wfevent, geom);
   //read waveform from file,  and clustering them.

  //global coordinate are saved here for the global tracking

  int tm2=clock();
  cerr<<"WFAnalyze end "<<tm2<<" "<<tm2/CLOCKS_PER_SEC<<endl;

  EventAnalysis(g4event, g4moduleHits);


}




void FixOverlapAndAnalysis(E16ANA_G4OutputData& g4data,
  E16ANA_GTRResponseAnalysis& gtrra,  int nEvent , int fixFlag,
  E16ANA_G4OutputData* g4data2,
  E16ANA_WaveformOutputData &wfdata
  ){

  //multiplicity fix
  double TimeInterval[]={-100,0,100,200,300}; //for 5 events
  int multi =5;
  if( fixFlag == 1  ){    TimeInterval[0]=0;    multi = 1;  }
  else if( fixFlag == 2  ){    TimeInterval[0]=2000;    multi = 1;  }

  double timeoffset  = 0;//for g4data2
  double evidoffset = 10000;//for g4data2

  for( int i = 0; i< nEvent ; i++){
    int jamInteraction = 0;
    //    int jamInteraction = 1;//select only interaction event
    int flag = g4data.ReadOverlapEvent(multi, TimeInterval, jamInteraction);

    if (flag < multi) {
      // not remained #multi events in file
      E16WARNING("only last %d events are in the file",flag);
      break;	// G4file-reading loop
    }//if

    if( g4data2 != NULL ){
      int flag2=g4data2->ReadAnEvent();
      if( flag2 != 1 ) {
	E16FATAL("file end at event %d",i); 
	break;
      }
      g4data.OverlapEvent().EventAppend( timeoffset, g4data2->Event(), evidoffset );
    }

    int wf_read_size = wfdata.ReadAnEvent();
    if(wf_read_size <= 0){
      E16FATAL("file end at event %d",i); 
      break;
    }

#ifndef USE_WAVEFILE
    gtrra.G4EventAnalysis( g4data.OverlapEvent() );
#else
    gtrra.WaveformEventAnalysis( g4data.OverlapEvent(), wfdata.Event());
#endif

  }//for nEvent


}

const double POISSONFACTOR = 2.0;


void PoissonOverlapAndAnalysis(E16ANA_G4OutputData& g4data,
  E16ANA_GTRResponseAnalysis& gtrra,
  int nEvent,  E16ANA_G4OutputData* g4data2,
  E16ANA_WaveformOutputData &wfdata
  ){

  const int NTIMEBIN  = 525;//ns
  const int OFFSETBIN = 175;
  double TimeInterval[128]={0};
  double poissonmean = NTIMEBIN/100.0;   // 10MHz = 1/100ns

  poissonmean *= POISSONFACTOR;

  //for g4data2
  double toffset  = 0;//for g4data2
  double evoffset = 10000;//for g4data2

  for( int i = 0; i< nEvent ; i++){

    int multi = E16ANA_EventOverlap::PoissonIntervalArray(poissonmean, 
				     NTIMEBIN, -OFFSETBIN, TimeInterval);

    cerr<<"Poisson: m="<<poissonmean<<" multi="<<multi<<" :";

    for(int j=0;j<multi;j++){
      cerr<<TimeInterval[j]<<" ";
    }//j
    cerr<<endl;
    int jamInteraction = 1;//select only interaction event
    int flag = g4data.ReadOverlapEvent(multi, TimeInterval, jamInteraction);

    if (flag < multi) {
      // not remained #multi events in file
      E16WARNING("only last %d events are in the file",flag);
      break;	 												// G4file-reading loop
    }//if

    if( g4data2 != NULL ){
      int flag2=g4data2->ReadAnEvent();
      if( flag2 != 1 ) {
	E16FATAL("file end at event %d",i); 
	break;	// G4file-reading loop
      }
      g4data.OverlapEvent().EventAppend( toffset, g4data2->Event(), evoffset );
    }

    int wf_read_size = wfdata.ReadAnEvent();
    if(wf_read_size <= 0){
      E16FATAL("file end at event %d",i); 
      break;
    }
#ifndef USE_WAVEFILE
    gtrra.G4EventAnalysis( g4data.OverlapEvent() );
#else
    gtrra.WaveformEventAnalysis( g4data.OverlapEvent(), wfdata.Event());
#endif

  }//for nEvent

    
}




int main(int argc, char **argv){

   E16ANA_ParamManager *paramMgr;
   char* inputfile;   
   char* inputfile2=NULL;
   char* outputfile;
   char* wavefile;
   int nEvent = 0;
   int randomSeed=0;
   int fixFlag = 0;  //->0:poisson, 1or5 number of overlap

   if (argc!=7 && argc!=8 ){
      cerr << "Usage: ./wavefromDoubleFit inputfile outputfile nevent randomseed fixflag wavefile (inputfile2) "<<endl;
      cerr<<"    fixflag : 0 : multiplicity poisson"<<endl;
      cerr<<"    fixflag : 1 : multiplicity fix    "<<endl;
      cerr<<"    fixflag : 2 :  overlap time +1000 i.e. no overlap effectively"<<endl;
      return 1;
   }
   else if(argc==5){
     inputfile=argv[1];
     outputfile=argv[2];
     nEvent = atoi(argv[3]);
     randomSeed = atoi(argv[4]);
     fixFlag = atoi(argv[5]);
     wavefile = argv[6];
   }
   else{
     inputfile=argv[1];
     outputfile=argv[2];
     nEvent = atoi(argv[3]);
     randomSeed = atoi(argv[4]);
     fixFlag = atoi(argv[5]);
     wavefile = argv[6];
     inputfile2=argv[7];
   };

   for(int i=0;i<argc;i++){
     globalARGstr <<argv[i]<<" ";
   }
   
      //-------------------------------------------
   E16MESSAGE("Initializing...");

   //   char* E16calibfilename = (char*)"./E16CalibFiles-local.cfg";
   char const* E16calibfilename = "./E16CalibFiles-local.cfg";

   paramMgr= new E16ANA_ParamManager( E16calibfilename );
   char* fname_field = paramMgr->GetMapFileName();
   char* fname_geom = paramMgr->GetGeomFileName();
   cerr <<"  Magnetic field map: "<<fname_field<<endl;
   cerr <<"  Geometry data file: "<<fname_geom<<endl;

   histo = new Histo(outputfile);

   geom  = new E16ANA_GeometryV1(fname_geom);
   fmf= new E16ANA_MagneticFieldMap3D(fname_field);
   //   Hep3Vector shiftV(-1.0,0,0);
   //   int ncall =fmf->Initialize_shift(shiftV);
   int ncall = fmf->Initialize_binary();

   E16ANA_G4OutputData g4data;
   int flag2 = g4data.OpenReadFile( inputfile );
   if( flag2 != g4data.OK ){     
     E16FATAL("exit: file open error:%s\n", inputfile);
     exit(1);   
   }
   E16ANA_G4OutputData g4data2;
   if (inputfile2 !=NULL){
     cerr<<"input2 "<<inputfile2<<endl;
     int flag3 = g4data2.OpenReadFile( inputfile2 );
     if( flag3 != g4data2.OK ){     
       E16FATAL("exit: file open error:%s\n", inputfile2);
       exit(1);   
     }
   }
   E16ANA_WaveformOutputData wfdata(wavefile);
   int flagwf = wfdata.OpenReadFile();
   if(flagwf == 0){
       E16FATAL("exit: file open error:%s\n", wavefile);
       exit(1);
   }

   delete gRandom;
   gRandom = new TRandom3(randomSeed);


   E16ANA_GTRResponseManager gtrRes(paramMgr);
   E16ANA_GTRAnalyzerManager gtrAna(paramMgr);
   E16ANA_GTRResponseAnalysis gtrs( &gtrRes, &gtrAna);

   cerr <<"Initializing ...end"<<endl;
   //----------------------------------------------

   E16ANA_G4OutputData* g4data2ptr = (inputfile2 == NULL )? NULL : &g4data2;
   if (fixFlag == 0 ){
     PoissonOverlapAndAnalysis(g4data, gtrs, nEvent, g4data2ptr, wfdata);   
   }
   else{
     FixOverlapAndAnalysis(g4data, gtrs, nEvent, fixFlag, g4data2ptr, wfdata);   
   }
#ifdef __E16_DYNAMICLINK__
   histo->PrintPS();//for the time being, works only dynamic link 
#endif
   histo->SaveAndClose();

    g4data.CloseReadFile();
    if(inputfile2 != NULL){
      g4data2.CloseReadFile();
    }

   E16MESSAGE("end");
   return 0;
}



