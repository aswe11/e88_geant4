//2016-11-22, uploaded by nakai
#ifndef E16ANA_TrackFinding_Nakai_hh
#define E16ANA_TrackFinding_Nakai_hh

#include "E16ANA_GeometryV2.hh"
#include "E16ANA_MultiTrack.hh"
#include "E16ANA_G4OutputData.hh"
#include "E16ANA_GTRAnalyzer2.hh"
#include "E16ANA_GTRStripAnalyzer.hh"
#include "E16ANA_SSDResponse.hh"
#include "E16ANA_HBDResponse.hh"

#include <algorithm>

//#include <TH1F.h>

class E16ANA_SingleTrackCandidate_Nakai {
public:
   E16ANA_SingleTrackCandidate_Nakai() :
      geom(NULL), track_id(0), charge(0.0),
      mom(0,0,0), vtx(0,0,0), mom_fit(0,0,0), vtx_fit(0,0,0), chisq(0.0)
   {
      for(int i=0; i<n_layers; i++){
         set_flag[i] = 0;
         g4_closest_flag[i] = 0;
         g4_near_flag[i] = 0;
      }
   };
   E16ANA_SingleTrackCandidate_Nakai(E16ANA_GeometryV2 *_geom, int _track_id, double _charge) : 
      mom(0,0,0), vtx(0,0,0), mom_fit(0,0,0), vtx_fit(0,0,0), chisq(0.0)
   {
      geom = _geom;
      track_id = _track_id;
      charge = _charge;
      for(int i=0; i<n_layers; i++){
         set_flag[i] = 0;
         g4_closest_flag[i] = 0;
         g4_near_flag[i] = 0;
      }
   };
   ~E16ANA_SingleTrackCandidate_Nakai(){};
   E16ANA_SingleTrackCandidate_Nakai(const E16ANA_SingleTrackCandidate_Nakai &obj){
      Copy(obj);
   };
   E16ANA_SingleTrackCandidate_Nakai& operator=(const E16ANA_SingleTrackCandidate_Nakai &rhs){
      Copy(rhs);
      return (*this);
   }

   static E16ANA_SingleTrackCandidate_Nakai MergeXandY(
         const E16ANA_SingleTrackCandidate_Nakai &x,
         const E16ANA_SingleTrackCandidate_Nakai &y
         ){
      E16ANA_SingleTrackCandidate_Nakai new_cand(x);

      for(int i=0; i<n_layers; i++){
         for(int j=0; j<(int)new_cand.hits[i].size(); j++){
            new_cand.hits[i][j].sigma.SetY(0.0);
         }
         for(int j=0; j<(int)y.hits[i].size(); j++){
            new_cand.hits[i].push_back(y.hits[i][j]);
         }
         new_cand.g4_closest_flag[i] = x.g4_closest_flag[i]+y.g4_closest_flag[i];
         new_cand.g4_near_flag[i] = x.g4_near_flag[i]+y.g4_near_flag[i];
      }
      return new_cand;
   }

   static bool CheckModuleID(
         const E16ANA_SingleTrackCandidate_Nakai &x,
         const E16ANA_SingleTrackCandidate_Nakai &y
         ){
      bool flag = true;
      for(int i=1; i<4; i++){
         //std::cout << "Layer = " << i << ", x = " << x.hits[i].size() << ", y = " << y.hits[i].size() << std::endl;
         flag = flag && IsSameModuleID(x.hits[i], y.hits[i]);
      }
      return flag;
   };

   void UpdateFitHits(E16ANA_MultiTrack *fitter, int track_id = 0);
   void UpdateAnaHits();
   bool IsG4Closest(int true_flag = 1){
      bool ret = true;
      for(int i=0; i<4; i++){
      //for(int i=1; i<4; i++){
         if(set_flag[i] != 0){
            ret = ret && ((g4_closest_flag[i] & true_flag) == true_flag);
         }
      }
      return ret;
   };
   bool IsG4Closest(int layer, int true_flag){
      if(set_flag[layer] == 0){
         return false;
      }
      return (g4_closest_flag[layer] & true_flag) == true_flag;
   };
   bool IsG4Near(int true_flag = 1){
      bool ret = true;
      for(int i=0; i<4; i++){
         if(set_flag[i] != 0){
            ret = ret && ((g4_near_flag[i] & true_flag) == true_flag);
         }
      }
      return ret;
   };
   bool IsG4Near(int layer, int true_flag){
      if(set_flag[layer] == 0){
         return false;
      }
      return (g4_near_flag[layer] & true_flag) == true_flag;
   };

   void AddSSDHit(E16ANA_G4Hit &hit){
      int l = 0;
      hits[l].push_back(
         //Hit_t(l, hit.ModuleID(), geom->SSD(hit.ModuleID()), hit.XTV(), TVector3(0.040, 0.0, 0.0), NULL)
         Hit_t(l, hit.ModuleID(), geom->SSD(hit.ModuleID()), hit.XTV(), TVector3(0.040, 0.0, 0.0), E16ANA_GTRAnalyzedStripHit())
      );
      set_flag[l] = 1;
      if(hit.EventID() >= 0){
         g4_closest_flag[0] = 3;
         g4_near_flag[0] = 3;
      }
      //std::cout << "SSD : gPos = "; hit.GXTV().Print();
   };
   void AddGTRHit(int layer_id, E16ANA_GTRAnalyzedHit &hit){
      TVector3 lpos(hit.XstripHit()->CogHit(),0.0,0.0);
      hits[layer_id+1].push_back(
         //Hit_t(layer_id+1, hit.ModuleID(), geom->GTR(hit.ModuleID(), layer_id), lpos, TVector3(0.080, 0.500, 0.0), *(hit.XstripHit()))
         Hit_t(layer_id+1, hit.ModuleID(), geom->GTR(hit.ModuleID(), layer_id), lpos, TVector3(0.080, 0.000, 0.0), *(hit.XstripHit()))
      );
      set_flag[layer_id+1] = 1;
      if(hit.XstripHit()->UserTag(0) == 1){
         g4_closest_flag[layer_id+1]++;
      }
      if(hit.XstripHit()->UserTag(1) == 1){
         g4_near_flag[layer_id+1]++;
      }
      //TVector3 gpos = geom->GTR(hit.ModuleID(), layer_id)->GetGPos(lpos);
      //std::cout << "GTR : gPos = "; gpos.Print();
   };
   void AddGTRHit(E16ANA_GTRAnalyzedStripHit &hit){
      int layer_id = hit.LayerID();
      int module_id = hit.ModuleID();
      TVector3 lpos(hit.TdcHit(),0.0,0.0);
      hits[layer_id+1].push_back(
         //Hit_t(layer_id+1, module_id, geom->GTR(module_id, layer_id), lpos, TVector3(0.080, 0.500, 0.0), hit)
         Hit_t(layer_id+1, module_id, geom->GTR(module_id, layer_id), lpos, TVector3(0.080, 0.000, 0.0), hit)
      );
      set_flag[layer_id+1] = 1;
      if(hit.UserTag(0) == 1){
         g4_closest_flag[layer_id+1]++;
      }
      if(hit.UserTag(1) == 1){
         g4_near_flag[layer_id+1]++;
      }
      //TVector3 gpos = geom->GTR(module_id, layer_id)->GetGPos(lpos);
      //std::cout << "GTR : gPos = "; gpos.Print();
   };
  //sako
  void AddGTRHit(int layer, E16ANA_G4Hit &hit){
      int layer_id = layer;
      int module_id = hit.ModuleID();
      //      TVector3 lpos(hit.TdcHit(),0.0,0.0);
      TVector3 lpos = hit.XTV();
      hits[layer_id+1].push_back(
         //Hit_t(layer_id+1, module_id, geom->GTR(module_id, layer_id), lpos, TVector3(0.080, 0.500, 0.0), hit)
	 Hit_t(layer_id+1, module_id, geom->GTR(module_id, layer_id), lpos, TVector3(0.080, 0.000, 0.0), E16ANA_GTRAnalyzedStripHit())
      );
      set_flag[layer_id+1] = 1;
      /*
      if(hit.UserTag(0) == 1){
         g4_closest_flag[layer_id+1]++;
      }
      if(hit.UserTag(1) == 1){
         g4_near_flag[layer_id+1]++;
     }
      */

      //sako not sure if this is correct
      if(hit.EventID()>= 0){
	g4_closest_flag[layer_id+1] = 3;
      }
      if(hit.EventID()>=0){
	g4_near_flag[layer_id+1] = 3;
     }

      //TVector3 gpos = geom->GTR(module_id, layer_id)->GetGPos(lpos);
      //std::cout << "GTR : gPos = "; gpos.Print();
   };
   void AddGTRHitX(E16ANA_GTRAnalyzedStripHit &hit){
      int layer_id = hit.LayerID();
      int module_id = hit.ModuleID();
      TVector3 lpos(hit.Tdc2Hit(),0.0,0.0);
      hits[layer_id+1].push_back(
         Hit_t(layer_id+1, module_id, geom->GTR(module_id, layer_id), lpos, TVector3(0.080, 0.000, 0.0), hit)
      );
      set_flag[layer_id+1] = 1;
      if(hit.UserTag(0) == 1){
         g4_closest_flag[layer_id+1]++;
      }
      if(hit.UserTag(1) == 1){
         g4_near_flag[layer_id+1]++;
      }
      //TVector3 gpos = geom->GTR(module_id, layer_id)->GetGPos(lpos);
      //std::cout << "GTR : gPos = "; gpos.Print();
   };
   void AddGTRHitY(E16ANA_GTRAnalyzedStripHit &hit){
      int layer_id = hit.LayerID();
      int module_id = hit.ModuleID();
      TVector3 lpos(0.0,hit.CogHit(),0.0);
      hits[layer_id+1].push_back(
         Hit_t(layer_id+1, module_id, geom->GTR(module_id, layer_id), lpos, TVector3(0.000, 0.500, 0.0), hit)
      );
      set_flag[layer_id+1] = 1;
      if(hit.UserTag(0) == 1){
         g4_closest_flag[layer_id+1] += 2;
      }
      if(hit.UserTag(1) == 1){
         g4_near_flag[layer_id+1] += 2;
      }
      //TVector3 gpos = geom->GTR(module_id, layer_id)->GetGPos(lpos);
      //std::cout << "GTR : gPos = "; gpos.Print();
   };
   void AddGTRHitYTdc2(E16ANA_GTRAnalyzedStripHit &hit){
      int layer_id = hit.LayerID();
      int module_id = hit.ModuleID();
      TVector3 lpos(0.0,hit.Tdc2Hit(),0.0);
      hits[layer_id+1].push_back(
         Hit_t(layer_id+1, module_id, geom->GTR(module_id, layer_id), lpos, TVector3(0.000, 0.500, 0.0), hit)
      );
      set_flag[layer_id+1] = 1;
      if(hit.UserTag(0) == 1){
         g4_closest_flag[layer_id+1] += 2;
      }
      if(hit.UserTag(1) == 1){
         g4_near_flag[layer_id+1] += 2;
      }
      //TVector3 gpos = geom->GTR(module_id, layer_id)->GetGPos(lpos);
      //std::cout << "GTR : gPos = "; gpos.Print();
   };

   void AddHBDHit(E16ANA_G4Hit &hit){
      int l = 4;
      hits[l].push_back(
         Hit_t(l, hit.ModuleID(), geom->HBD(hit.ModuleID()), hit.XTV(), TVector3(10.0, 10.0, 0.0), E16ANA_GTRAnalyzedStripHit())
      );
      set_flag[l] = 1;
      //std::cout << "HBD : gPos = "; hit.GXTV().Print();
   };
   double GetCharge() const {return charge;};
   int GetTrackID() const {return track_id;};
   TVector3 Momentum() const {return mom;};
   TVector3 Vertex() const {return vtx;};
   TVector3 GetFitMomentum() const {return mom_fit;};
   TVector3 GetFitVertex() const {return vtx_fit;};
   void UpdateInitialConditions(){
      mom = mom_fit;
      vtx = vtx_fit;
   };
  //sako
  void SetTrackID(int _track_id ) {track_id = _track_id;};
   void SetMomentum(const TVector3 &_mom){mom = _mom;};
   void SetVertex(const TVector3 &_vtx){vtx = _vtx;};
   void SetSingleTrackHit(E16ANA_MultiTrack *single_track, int _track_id = 0);
   double GetChisq() const {return chisq;};
   struct LessChisq {
      bool operator()(const E16ANA_SingleTrackCandidate_Nakai &lhs, const E16ANA_SingleTrackCandidate_Nakai &rhs){
         return lhs.GetChisq() < rhs.GetChisq();
      }
   };
   double SingleFit(E16ANA_MultiTrack *fitter, bool vertexfixflag = false, bool pyfixflag = false);
   double LinearFitY(bool hbd_check, std::vector<E16ANA_G4Hit> &hbd_hits);
   double LinearFitY(){
      std::vector<E16ANA_G4Hit> temp;
      return LinearFitY(false, temp);
   };
   int GetSetFlag(int i){return set_flag[i];};
   //int GetModuleID(int i){return hits[i].module_id;};
   std::vector<E16ANA_G4Hit>& GetAnaHits(int layer){return hits_ana[layer];};
   std::vector<E16ANA_G4Hit>& GetFitHits(int layer){return hits_fit[layer];};
   void GetModuleIDs(int m[]){
      for(int l=0; l<n_layers; l++){
         if(hits[l].empty()){
            m[l] = -1;
         }else{
            m[l] = hits[l][0].module_id;
         }
      }
   };

   void Print(){
      std::cout << "E16ANA_SingleTrackCandidate_Nakai : set_flag =";
      for(int i=0; i<n_layers; i++){
         std::cout << " " << set_flag[i];
      }
      std::cout << ", hits_fit.size =";
      for(int i=0; i<n_layers; i++){
         std::cout << " " << hits_fit[i].size();
      }
      std::cout << ", g4_closest_flag =";
      for(int i=0; i<n_layers; i++){
         std::cout << " " << g4_closest_flag[i];
      }
      std::cout << ", g4_near_flag =";
      for(int i=0; i<n_layers; i++){
         std::cout << " " << g4_near_flag[i];
      }
      std::cout << std::endl;
      for(int i=0; i<n_layers; i++){
         if(hits_fit[i].size() > 0){
            std::cout << "    layer_order = " << i
               << " : module_id = " << hits_fit[i][0].ModuleID()
               << ", lpos_fit[0] = (" << hits_fit[i][0].X()
               << ", " << hits_fit[i][0].Y()
               << ")" << std::endl;
         }
      }
      std::cout << "    Initial momentum = (" << mom.X() << ", " << mom.Y() << ", " << mom.Z() << ")" << std::endl;
      std::cout << "    Fit momentum = (" << mom_fit.X() << ", " << mom_fit.Y() << ", " << mom_fit.Z() << ")" << std::endl;
      std::cout << "    Initial vertex = (" << vtx.X() << ", " << vtx.Y() << ", " << vtx.Z() << ")" << std::endl;
      std::cout << "    Fit vertex = (" << vtx_fit.X() << ", " << vtx_fit.Y() << ", " << vtx_fit.Z() << ")" << std::endl;
      std::cout << "    chisq = " << chisq << std::endl;
   };

   TVector3 ProjectionHBD(E16ANA_MultiTrack *fitter, E16ANA_G4Hit &hbd_hit);
   bool HBDMatch(E16ANA_MultiTrack *fitter, E16ANA_G4Hit &hbd_hit, int xy = 3, double match_distance = 50.0);
   int GetG4ClosestFlag(int i){return g4_closest_flag[i];};
   int GetG4NearFlag(int i){return g4_near_flag[i];};
   void Clear(){
      for(int i=0; i<n_layers; i++){
         hits[i].clear();
         hits_ana[i].clear();
         hits_fit[i].clear();
         set_flag[i] = 0;
         g4_closest_flag[i] = 0;
         g4_near_flag[i] = 0;
      }
   };
   void ReAnalyzeTdc2(E16ANA_GTRAnalyzerManager *ana, int l){
      //for(int l=0; l<n_layers; l++){
         for(int i=0; i<(int)hits[l].size(); i++){
            //if(hits[l][i].strip_hit == NULL) continue;
            if(hits[l][i].strip_hit.IsInvalid()) return;
            //int layer = hits[l][i].layer_order;
            //if(layer != _layer) continue;
            if((int)hits_fit[l].size() == 0) return;
            E16ANA_GTRAnalyzedStripHit *shit = &(hits[l][i].strip_hit);
            double prex = hits[l][i].lpos.X();
            double tan_theta = -10000.0;
            if(shit->IsX()){
               tan_theta = hits_fit[l][0].PTV().X()/hits_fit[l][0].PTV().Z();
               if(fabs(tan_theta) > tan(10.0/180.0*TMath::Pi())){
                  //shit->SetTanTheta(tan_theta);
                  //ana->ChamberAnalyzer(shit->LayerID(), shit->ModuleID())->AnalyzeV1X(shit->ID());
                  shit->CalcTdc2(tan_theta);
                  hits[l][i].lpos.SetX(shit->Tdc2Hit());
               }
            }else{
               tan_theta = hits_fit[l][0].PTV().Y()/hits_fit[l][0].PTV().Z();
               if(fabs(tan_theta) > tan(10.0/180.0*TMath::Pi())){
                  //shit->SetTanTheta(tan_theta);
                  //ana->ChamberAnalyzer(shit->LayerID(), shit->ModuleID())->AnalyzeV1Y(shit->ID(), shit->Type());
                  shit->CalcTdc2(tan_theta);
                  hits[l][i].lpos.SetY(shit->Tdc2Hit());
               }

            }
            //double postx = hits[l][i].lpos.X();
            //std::cout << "E16ANA_SingleTrackCandidate_Nakai::ReAnalyzeTdc2 : x_or_y = " << shit->IsX()
            //   << ", tan_theta = " << tan_theta
            //   << ", X = " << prex << " -> " << postx
            //   << std::endl;
         }
      //}
   };
   bool CalcRoughMomentum();

private:
   struct Hit_t {
      int layer_order;
      int module_id;
      const E16ANA_DetectorGeometry *geom;
      TVector3 lpos;
      //TVector3 lmom;
      TVector3 sigma;
      TVector3 gpos;
      E16ANA_GTRAnalyzedStripHit strip_hit;
      //TVector3 gmom;
      //Hit_t(int _layer_order, int _module_id, const E16ANA_DetectorGeometry *_geom, const TVector3 &_lpos, const TVector3 &_lmom, const TVector3 &_sigma) : 
      Hit_t(int _layer_order, int _module_id, const E16ANA_DetectorGeometry *_geom, const TVector3 &_lpos, const TVector3 &_sigma, E16ANA_GTRAnalyzedStripHit _shit) : 
         layer_order(_layer_order),
         module_id(_module_id),
         //geom(_geom), lpos(_lpos), lmom(_lmom), sigma(_sigma)
         geom(_geom), lpos(_lpos), sigma(_sigma),
         strip_hit(_shit)
      {
         gpos = geom->GetGPos(lpos);
         //gmom = geom->GetGMom(lmom);
      };
      int GetCodedModuleID() const {
         int ya_or_yb = 0;
         const double mergin = 1.0; // mm
         if(!strip_hit.IsInvalid()){
            if(strip_hit.LayerID() != 0){
               ya_or_yb = 0;
            }else if(strip_hit.IsX()){ // x-strip
               ya_or_yb = 0;
               if(lpos.X() < mergin){ // yb
                  ya_or_yb |= 0x02;
               }
               if(lpos.X() > -mergin){ // ya
                  ya_or_yb |= 0x01;
               }
            }else{ // ya-strip or yb-strip
               ya_or_yb = (strip_hit.IsYb()) ? 0x02 : 0x01;
            }
         }
         return (layer_order<<16) + (module_id<<8) + ya_or_yb;
      };
   };
   //enum { n_layers = 5 };
   enum { n_layers = 4 };
   E16ANA_GeometryV2 *geom;
   int track_id;
   double charge;
   TVector3 mom;
   TVector3 vtx;
   TVector3 mom_fit;
   TVector3 vtx_fit;
   std::vector<Hit_t> hits[n_layers];
   std::vector<E16ANA_G4Hit> hits_ana[n_layers];
   std::vector<E16ANA_G4Hit> hits_fit[n_layers];
   int set_flag[n_layers];
   double chisq;
   int g4_closest_flag[n_layers];
   int g4_near_flag[n_layers];

   void Copy(const E16ANA_SingleTrackCandidate_Nakai &rhs){
      this->geom = rhs.geom;
      this->track_id = rhs.track_id;
      this->charge = rhs.charge;
      this->mom = rhs.mom;
      this->vtx = rhs.vtx;
      this->mom_fit = rhs.mom_fit;
      this->vtx_fit = rhs.vtx_fit;
      for(int i=0; i<n_layers; i++){
         this->hits[i] = rhs.hits[i];
         this->hits_ana[i] = rhs.hits_ana[i];
         this->hits_fit[i] = rhs.hits_fit[i];
         this->set_flag[i] = rhs.set_flag[i];
         this->g4_closest_flag[i] = rhs.g4_closest_flag[i];
         this->g4_near_flag[i] = rhs.g4_near_flag[i];
      }
      this->chisq = rhs.chisq;
   }
   static TVector3 ConformalTransformation(const TVector3 &h);
   static double CalcPhaseDiff(const TVector3 &v0, const TVector3 &v1);
   static TVector3 CalcRoughMomentum(const TVector3 &gxz0, const TVector3 &gxz1);
   static bool IsSameModuleID(int x, int y);
   static bool IsSameModuleID(const std::vector<Hit_t> &x, const std::vector<Hit_t> &y);

};

/*
class E16ANA_EventSelection_Nakai {
public:
   //static bool SelectionByGTR123(E16ANA_G4Event *g4event);
   static bool SelectionBySSDHBD(E16ANA_SSDResponse *ssd_res[], std::vector<E16ANA_SingleTrackCandidate_Nakai> &candidates);
};
*/

class E16ANA_DoubleTrackCandidate_Nakai {
public:
   E16ANA_DoubleTrackCandidate_Nakai(
         const E16ANA_SingleTrackCandidate_Nakai &_single_plus,
         const E16ANA_SingleTrackCandidate_Nakai &_single_minus);
   ~E16ANA_DoubleTrackCandidate_Nakai();
   E16ANA_DoubleTrackCandidate_Nakai(){};
   double DoubleFit(E16ANA_MultiTrack *fitter);
   void Print();
   double GetChisq() const {return chisq;};
   struct LessChisq {
      bool operator()(const E16ANA_DoubleTrackCandidate_Nakai &lhs, const E16ANA_DoubleTrackCandidate_Nakai &rhs){
         return lhs.GetChisq() < rhs.GetChisq();
      }
   };
   TVector3 GetFitP0(){return mom_fit_plus;};
   TVector3 GetFitP1(){return mom_fit_minus;};
   TVector3 GetFitVertex(){return vtx_fit;};
   int GetG4ClosestFlag0(int i){return single_plus.GetG4ClosestFlag(i);};
   int GetG4ClosestFlag1(int i){return single_minus.GetG4ClosestFlag(i);};
   int GetG4NearFlag0(int i){return single_plus.GetG4NearFlag(i);};
   int GetG4NearFlag1(int i){return single_minus.GetG4NearFlag(i);};
   E16ANA_SingleTrackCandidate_Nakai& GetSingleTrackCandidate(int i){
      if(i == 0){
         return single_plus;
      }else{
         return single_minus;
      }
   };

private:
   E16ANA_SingleTrackCandidate_Nakai single_plus;
   E16ANA_SingleTrackCandidate_Nakai single_minus;
   double chisq;
   TVector3 mom_fit_plus;
   TVector3 mom_fit_minus;
   TVector3 vtx_fit;

};

class E16ANA_TrackFinding_Nakai {
public:
   E16ANA_TrackFinding_Nakai(
         E16ANA_G4Event *_g4event,
         E16ANA_SSDResponse *_ssd_res[],
         E16ANA_GTRAnalyzerManager *_gtr_ana,
         E16ANA_HBDResponseManager *_hbd_res,
         E16ANA_MagneticFieldMap *_bfield,
         E16ANA_GeometryV2 *_geom
         );
   ~E16ANA_TrackFinding_Nakai();
   void SingleFit(E16ANA_SingleTrackCandidate_Nakai &single, int track_id = -1);
   void GetSingleTrackCandidates(std::vector<E16ANA_SingleTrackCandidate_Nakai> &_candidates);

   void TrackFinding();
   //void TrackFinding2();
   //void GetSingleTrackCandidatesG4(std::vector<E16ANA_SingleTrackCandidate_Nakai> &_candidates);
   std::vector<int>& GetRankingXselectionPlus(){return ranking_xselection_plus;};
   std::vector<int>& GetRankingXselectionMinus(){return ranking_xselection_minus;};
   std::vector<int>& GetRankingYselection(){return ranking_yselection;};
   std::vector<int>& GetRankingXYselectionPlus(){return ranking_xyselection_plus;};
   std::vector<int>& GetRankingXYselectionMinus(){return ranking_xyselection_minus;};
   std::vector<int>& GetRankingXYselectionHBDPlus(){return ranking_xyselection_hbd_plus;};
   std::vector<int>& GetRankingXYselectionHBDMinus(){return ranking_xyselection_hbd_minus;};
   std::vector<int>& GetRankingDoubleSelection(){return ranking_double;};
   std::vector<TVector3>& GetHBDProjectionXselectionPlus(){return hbd_proj_xselection_plus;};
   std::vector<TVector3>& GetHBDProjectionXselectionMinus(){return hbd_proj_xselection_minus;};
   std::vector<TVector3>& GetHBDProjectionYselection(){return hbd_proj_yselection;};
   std::vector<TVector3>& GetHBDProjectionXYselectionPlus(){return hbd_proj_xyselection_plus;};
   std::vector<TVector3>& GetHBDProjectionXYselectionMinus(){return hbd_proj_xyselection_minus;};
   std::vector<TVector3>& GetHBDProjectionXYselectionHBDPlus(){return hbd_proj_xyselection_hbd_plus;};
   std::vector<TVector3>& GetHBDProjectionXYselectionHBDMinus(){return hbd_proj_xyselection_hbd_minus;};
   std::vector<TVector3>& GetHBDProjectionDoubleSelection(){return hbd_proj_double;};

   bool ExistBestCandidate(){return exist_best;};
   E16ANA_DoubleTrackCandidate_Nakai& GetBestCandidate(){return best_candidate;};
   //std::vector<TH1F*>& GetHistos(){return hist_list;};
   static std::vector<int> LinkedModuleIDs(int module);


private:
   enum{
      n_ssd_modules = 33,
      n_gtr_modules = 33
   };
   E16ANA_G4Event *g4event;
   E16ANA_SSDResponse *ssd_res[n_ssd_modules];
   E16ANA_GTRAnalyzerManager *gtr_ana;
   E16ANA_HBDResponseManager *hbd_res;
   E16ANA_MagneticFieldMap *bfield_map;
   E16ANA_GeometryV2 *geom;

   std::vector<E16ANA_SingleTrackCandidate_Nakai> single_track_candidates;

   E16ANA_G4Hit ssd_g4hit[n_ssd_modules];
   E16ANA_G4Hit gtr_g4hit[n_gtr_modules][3];

   E16ANA_DoubleTrackCandidate_Nakai best_candidate;
   bool exist_best;
   std::vector<int> ranking_xselection_plus;
   std::vector<int> ranking_xselection_minus;
   std::vector<int> ranking_yselection;
   std::vector<int> ranking_xyselection_plus;
   std::vector<int> ranking_xyselection_minus;
   std::vector<int> ranking_xyselection_hbd_plus;
   std::vector<int> ranking_xyselection_hbd_minus;
   std::vector<int> ranking_double;

   std::vector<TVector3> hbd_proj_xselection_plus;
   std::vector<TVector3> hbd_proj_xselection_minus;
   std::vector<TVector3> hbd_proj_yselection;
   std::vector<TVector3> hbd_proj_xyselection_plus;
   std::vector<TVector3> hbd_proj_xyselection_minus;
   std::vector<TVector3> hbd_proj_xyselection_hbd_plus;
   std::vector<TVector3> hbd_proj_xyselection_hbd_minus;
   std::vector<TVector3> hbd_proj_double;

   //std::vector<TH1F*> hist_list;

   double CalcClusterWidth(E16ANA_GTRAnalyzedStripHit *hit){
      double min =  100000.0;
      double max = -100000.0;
      for(int i=0; i<hit->NumHit(); i++){
         double pos = hit->StripPos(i);
         if(pos<min){
            min = pos;
         }
         if(pos>max){
            max = pos;
         }
      }
      return max-min;
   };

   // X-candidates
   void GetSingleTrackCandidatesNonPolluted(
         int m_ssd, int m_gtr, int gtr_layer,
         std::vector<E16ANA_SingleTrackCandidate_Nakai> &cand_plus,
         std::vector<E16ANA_SingleTrackCandidate_Nakai> &cand_minus,
         double hit_width_cut = 5.5*0.35
         );
   void GetSingleTrackCandidatesNonPolluted(
         int module, int gtr_layer,
         std::vector<E16ANA_SingleTrackCandidate_Nakai> &cand_plus,
         std::vector<E16ANA_SingleTrackCandidate_Nakai> &cand_minus,
         double hit_width_cut = 5.5*0.35
         ){
      GetSingleTrackCandidatesNonPolluted(module, module, gtr_layer, cand_plus, cand_minus, hit_width_cut);
   };

   void SelectOtherLayerHits(
         E16ANA_SingleTrackCandidate_Nakai &input,
         std::vector<E16ANA_SingleTrackCandidate_Nakai> &_candidates,
         bool exclude_rough_fit_layer);
   void SelectOtherLayerHits(
         std::vector<std::vector<E16ANA_SingleTrackCandidate_Nakai> > &input,
         std::vector<std::vector<E16ANA_SingleTrackCandidate_Nakai> > &output,
         bool exclude_rough_fit_layer = true){
      for(int i=0; i<(int)input.size(); i++){
         for(int j=0; j<(int)input[i].size(); j++){
            SelectOtherLayerHits(input[i][j], output[i], exclude_rough_fit_layer);
         }
      }
   };

   void SingleFitAllCandidates(std::vector<std::vector<E16ANA_SingleTrackCandidate_Nakai> > &cand, bool vertexfixflag = false, bool pyfixflag = false);
   void DoubleFitAllCandidates(std::vector<E16ANA_DoubleTrackCandidate_Nakai> &cand);
   void ChisqCut(std::vector<std::vector<E16ANA_SingleTrackCandidate_Nakai> > &cand, double threshold);

   // Y-candidates
   void GetSingleTrackCandidatesY(int m1, int m2, int m3, std::vector<E16ANA_SingleTrackCandidate_Nakai> &cand);
   void GetSingleTrackCandidatesY(int module, std::vector<E16ANA_SingleTrackCandidate_Nakai> &cand){
      GetSingleTrackCandidatesY(module, module, module, cand);
   };
   void GetSingleTrackCandidatesV4Y(int m1, int m2, int m3, std::vector<E16ANA_SingleTrackCandidate_Nakai> &cand);
   void GetSingleTrackCandidatesV4Y(int module, std::vector<E16ANA_SingleTrackCandidate_Nakai> &cand){
      GetSingleTrackCandidatesV4Y(module, module, module, cand);
   };
   void CheckHBDMatch(std::vector<std::vector<E16ANA_SingleTrackCandidate_Nakai> > &cand, int xy = 3, double match_distance = 50.0);
   bool IsHBDMatched(E16ANA_SingleTrackCandidate_Nakai &cand, int xy = 3, double match_distance = 50.0);
   TVector3 GetMostPlausibleProjectionHBD(E16ANA_SingleTrackCandidate_Nakai &cand);
   void ChisqSort(std::vector<E16ANA_SingleTrackCandidate_Nakai> &_candidates){
      std::sort(_candidates.begin(), _candidates.end(), E16ANA_SingleTrackCandidate_Nakai::LessChisq());
   };
   void ChisqSort(std::vector<E16ANA_DoubleTrackCandidate_Nakai> &_candidates){
      std::sort(_candidates.begin(), _candidates.end(), E16ANA_DoubleTrackCandidate_Nakai::LessChisq());
   };

   void SetG4Hit(int mm);
   void SetG4HitSSD();
   E16ANA_GTRAnalyzedHit* SetG4ClosestTag(std::vector<E16ANA_GTRAnalyzedHit> &hits, int x_or_y, int cog_or_tdc, double g4_near_threshold = 1.0);
   void SetG4ClosestTag(std::vector<E16ANA_GTRAnalyzedStripHit> &hits, int cog_or_tdc, double g4_near_threshold = 1.0);
   //void SetG4ClosestTagSSD(std::vector<E16ANA_G4Hit> &hits, double g4_near_threshold = 1.0);
   //void CheckG4Closest(std::vector<std::vector<E16ANA_SingleTrackCandidate_Nakai> > &cand, std::vector<int> &rank, int true_flag = 1);
   void CheckG4ClosestLayer2or3(std::vector<std::vector<E16ANA_SingleTrackCandidate_Nakai> > &cand,
         std::vector<int> &rank, std::vector<TVector3> &hbd_proj, int true_flag = 1);
   void CheckG4Closest(std::vector<std::vector<E16ANA_SingleTrackCandidate_Nakai> > &cand,
         std::vector<int> &rank, std::vector<TVector3> &hbd_proj, int true_flag = 1);
   void CheckG4ClosestDouble(std::vector<E16ANA_DoubleTrackCandidate_Nakai> &cand,
         std::vector<int> &rank, std::vector<TVector3> &hbd_proj, int true_flag = 1);
   void PrintNumberOfCandidates(std::vector<std::vector<E16ANA_SingleTrackCandidate_Nakai> > &cand);

   void ReAnalyzeAndReFit(std::vector<E16ANA_SingleTrackCandidate_Nakai> &cand);
   void ReAnalyzeAndReFit(std::vector<std::vector<E16ANA_SingleTrackCandidate_Nakai> > &cand){
      for(int i=0; i<(int)cand.size(); i++){
         ReAnalyzeAndReFit(cand[i]);
      }
   };

};

#endif // E16ANA_TrackFinding_Nakai_hh
