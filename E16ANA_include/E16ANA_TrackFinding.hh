//2016-05-02, uploaded by nakai
//2016-05-02, uploaded by yokkaich
//2016-04-01, uploaded by nakai
//2015-12-31, uploaded by yokkaich
//2015-12-25, uploaded by komatsu
//2015-12-25, written by Komatsu
//
//E16ANA_TrackFinding.hh 
//
#ifndef E16ANA_TrackFinding_h

#define E16ANA_TrackFinding_h 1
#include <TVector3.h>
#include <TObject.h>
#include <string> 
#include <math.h> 

#include "E16ANA_G4OutputData.hh"
#include "E16ANA_GTRAnalyzer2.hh"

using namespace std;
//---------------Define Class for Track Finding.----------------------------------
class XThreeHitCandidate;
class YThreeHitCandidate;
class E16ANA_G4Track;
class E16ANA_G4Hit;
class E16ANA_SingleTrack;

//extern "C"{
void SearchChi2MinSingleTrack( vector<E16ANA_SingleTrack*> ST_p, vector<E16ANA_SingleTrack*> ST_e, E16ANA_SingleTrack* *ST_p_chi2_min, E16ANA_SingleTrack* *ST_e_chi2_min, int *p_chi2min_flag, int *e_chi2min_flag);
//}
class G4ThreeHitTrack{
private:  
  E16ANA_G4Track* g4track;
  E16ANA_G4Hit* g4hits[3];
  XThreeHitCandidate *xcand_nearest;
  YThreeHitCandidate *ycand_nearest;
public:
  G4ThreeHitTrack(E16ANA_G4Track* track):g4track(track){
    xcand_nearest=NULL;

  };
  ~G4ThreeHitTrack(){}  

  void SetG4Hits(E16ANA_G4Hit* g4hit0, E16ANA_G4Hit* g4hit1, E16ANA_G4Hit* g4hit2){
    g4hits[0]=g4hit0;
    g4hits[1]=g4hit1;
    g4hits[2]=g4hit2;
  }
  void SetNearestX(XThreeHitCandidate *xcand){
    xcand_nearest=xcand;
  } 
  void SetNearestY(YThreeHitCandidate *ycand){
    ycand_nearest=ycand;
  } 

  XThreeHitCandidate* NearestCandX(){return xcand_nearest;}
  int PID(){return g4track->PID();}
  int ModuleID(int layer){return g4hits[layer]->ModuleID();}
  TVector3 HitLocal(int layer){return g4hits[layer]->XTV();}
  TVector3 HitGlobal(int layer){return g4hits[layer]->GXTV();}
  TVector3 PTV(){return g4track->PTV();}
  TVector3 TVertex(){return g4track->TVertex();}
  E16ANA_G4Hit* G4Hit(int layer){return g4hits[layer];}

  void Dump(){
    cerr<<"PID: "<<PID()<<endl;
    cerr<<"MOM: "<<g4track->PV()<<endl;

    cerr<<"Hit Global0: ";HitGlobal(0).Print();
    cerr<<"Hit Global1: ";HitGlobal(1).Print();
    cerr<<"Hit Global2: ";HitGlobal(2).Print();  

    cerr<<"Hit Local0: ";HitLocal(0).Print();
    cerr<<"Hit Local1: ";HitLocal(1).Print();
    cerr<<"Hit Local2: ";HitLocal(2).Print();  
  }
  
};

class XThreeHitCandidate{
private:  
  E16ANA_GTRAnalyzedHit* anahits[3];
  int isNearest;
  G4ThreeHitTrack* g3t_nearest;
  //New coordinate for quadratic approximation for track.
  //In the new coordinate,
  // (0',0') is hit in 100kaku. 
  // X axis is pararell to the line from (0, 0) to 200kaku hit.
  
  double pol2_par[3];//Parameters for pol2. y'=[0]*x'^2+[1]*x'+[2] in the new coordinate.
  double R;//Curvature.
  double mom0_xz;
  TVector3 mom0_xz_v;
  TVector3 org_new;//Origin in new coordinate system.
  TVector3 cross_target;//Cross point between x axis and quadratic curve in the original coordinate system.
  double m, n;//y'=mx'+n for the original x axis.

  void CalcPol2(double x[3], double z[3]);
  TVector3 CalcCrossTargetInNew();
  void EstInitVtxMom();
public:
  XThreeHitCandidate(E16ANA_GTRAnalyzedHit* anahit0, E16ANA_GTRAnalyzedHit* anahit1, E16ANA_GTRAnalyzedHit* anahit2){
    isNearest = 0; 
    g3t_nearest=NULL;
    for(int i=0;i<3;i++){
      pol2_par[i]=0.;
    }
    R=0.;  
    anahits[0]=anahit0;
    anahits[1]=anahit1;
    anahits[2]=anahit2;    
    EstInitVtxMom();
  }  
  void ChangeCoordinate(TVector3 *v, TVector3 *v2, TVector3 *x_axis2, TVector3 *z_axis2);
  
  ~XThreeHitCandidate(){}  
  void SetNearestG4Track(G4ThreeHitTrack* g3t){
    g3t_nearest=g3t;
  }
  G4ThreeHitTrack* NearestG4Track(){
    return g3t_nearest;
  }
  E16ANA_GTRAnalyzedHit* AnaHit(int i){
    return anahits[i];
  }
  double Pol2Par(int i){
    return pol2_par[i];
  }
  double Radius(){
    return R;
  }
  double Mom0XZ(){
    return mom0_xz;
  }
  TVector3 Mom0XZv(){
    return mom0_xz_v;
  }
  TVector3 CrossTarget(){
    return cross_target;
  }
  int CheckG4Nearest(E16ANA_GTRAnalyzedHit* hit0, E16ANA_GTRAnalyzedHit* hit1, E16ANA_GTRAnalyzedHit* hit2){
    
    if(anahits[0]->ModuleID()!=hit0->ModuleID()){return 0;}
    if(anahits[1]->ModuleID()!=hit1->ModuleID()){return 0;}  
    if(anahits[2]->ModuleID()!=hit2->ModuleID()){return 0;}
    
    if( &anahits[0] == &hit0 ){
      return 0;
    }
    if( &anahits[1] == &hit1 ){
      return 0;
    }
    if( &anahits[2] == &hit2 ){
      return 0;
    }

    /*if(fabs(anahits[0]->Position().X()-hit0->Position().X())
       >std::numeric_limits<double>::epsilon()){
      return 0;
    }
    if(fabs(anahits[1]->Position().X()-hit1->Position().X())
       >std::numeric_limits<double>::epsilon()){
      return 0;
    }
    if(fabs(anahits[2]->Position().X()-hit2->Position().X())
       >std::numeric_limits<double>::epsilon()){
      return 0;
      }*/
   
    //Three hits are matched.
    isNearest = 1;
    return isNearest;
    
  }
  
  int AskXNearest(){
    return isNearest;
  }
  
};//Close class definition

class YThreeHitCandidate{
private:  
  E16ANA_GTRAnalyzedHit* anahits[3];
  int isNearest;
  G4ThreeHitTrack* g3t_nearest;
public:
  YThreeHitCandidate(){isNearest = 0; g3t_nearest=NULL;}    
  void SetHits(E16ANA_GTRAnalyzedHit* anahit0, E16ANA_GTRAnalyzedHit* anahit1, E16ANA_GTRAnalyzedHit* anahit2){
    anahits[0]=anahit0;
    anahits[1]=anahit1;
    anahits[2]=anahit2;    
  };
  ~YThreeHitCandidate(){}  
  
  void SetNearestG4Track(G4ThreeHitTrack* g3t){
    g3t_nearest=g3t;
  }
  G4ThreeHitTrack* NearestG4Track(){
    return g3t_nearest;
  }
  E16ANA_GTRAnalyzedHit* AnaHit(int i){
    return anahits[i];
  }   

  int CheckG4Nearest(E16ANA_GTRAnalyzedHit* hit0, E16ANA_GTRAnalyzedHit* hit1, E16ANA_GTRAnalyzedHit* hit2){
    
    if(anahits[0]->ModuleID()!=hit0->ModuleID()){return 0;}
    if(anahits[1]->ModuleID()!=hit1->ModuleID()){return 0;}  
    if(anahits[2]->ModuleID()!=hit2->ModuleID()){return 0;}
    
    if( &anahits[0] == &hit0 ){
      return 0;
    }
    if( &anahits[1] == &hit1 ){
      return 0;
    }
    if( &anahits[2] == &hit2 ){
      return 0;
    }
       
    /* if(fabs(anahits[0]->Position().X()-hit0->Position().X())
       >std::numeric_limits<double>::epsilon()){
      return 0;
    }
    if(fabs(anahits[1]->Position().X()-hit1->Position().X())
       >std::numeric_limits<double>::epsilon()){
      return 0;
    }
    if(fabs(anahits[2]->Position().X()-hit2->Position().X())
       >std::numeric_limits<double>::epsilon()){
      return 0;
      }*/
   
    //Three hits are matched.
    isNearest = 1;
    return isNearest;
    
   }

  int AskYNearest(){
    return isNearest;
  }

};
#endif
//---------------End Definition of Class for Track Finding.----------------------------------
