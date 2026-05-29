//2016-11-22, uploaded by nakai
//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai
#ifndef E16ANA_MultiTrack_h
#define E16ANA_MultiTrack_h

#include <iostream>
#include <vector>
#include <algorithm>

#include <TVector3.h>
#include <Math/Plane3D.h>
#include <TMinuitMinimizer.h>

#include "E16ANA_MagneticFieldMap.hh"
#include "E16ANA_Geometry.hh"
#include "E16ANA_GeometryV2.hh"

class E16ANA_MultiTrack {
private:
   /* typedef and declaration of struct */
   //typedef ROOT::Math::Plane3D Plane_t;
   struct Hit_t;
   struct LessModuleHit;
   typedef std::pair<int, std::vector<Hit_t*> > ModuleHits_t;
   struct LayerHits_t;
   /*
    *
    * Hit_t is one hit.
    *                ---------------------------
    * Hit_t        = | lpos | sigma | lpos_fit |
    *                ---------------------------
    *
    * ModuleHits_t is hits in one chamber, one layer, one track.
    *                ---------------------------
    * ModuleHits_t = | module_id | Hit_t*      |
    *                ---------------------------
    *                            | Hit_t*      |
    *                            ---------------
    *                            | Hit_t*      |
    *                            ---------------
    *
    * LayerHits_t is hits in one layer, one track.
    *                -----------------------------------------
    * LayerHits_t  = | layer_order | layer_id | ModuleHits_t |
    *                -----------------------------------------
    *                                         | ModuleHits_t |
    *                                         ----------------
    *                                         | ModuleHits_t |
    *                                         ----------------
    *
    * one track consists of multi LayerHits_t.
    *               ---------------
    * one-track   = | LayerHits_t |  layer_order = 0
    *               ---------------
    *               | LayerHits_t |  layer_order = 1
    *               ---------------
    *               | LayerHits_t |  layer_order = 2
    *               ---------------
    *
    */

public:
   //E16ANA_MultiTrack(E16ANA_MagneticFieldMap *_bfield_map, E16ANA_Geometry *_geom, int _n_tracks = 2);
   E16ANA_MultiTrack(E16ANA_MagneticFieldMap *_bfield_map, E16ANA_GeometryV2 *_geom, int _n_tracks = 2);
   ~E16ANA_MultiTrack();

   /* Set initial values of fitting */
   // initial vertex, vertex error (chi2 sigma), momenta, signs of tracks
   void Initialize(const TVector3 &vtx, const TVector3 &vtx_sigma, const TVector3 mom[], double charge[]){
      Clear();
      SetInitialVertex(vtx, vtx_sigma);
      SetInitialMomenta(mom);
      SetCharges(charge);
   };
   void SetInitialVertex(const TVector3 &vtx, const TVector3 &vtx_sigma){
      vertex_init = vtx;
      vertex_sigma = vtx_sigma;
   };
   void SetInitialMomentum(int track_id, const TVector3 &mom);
   void SetInitialMomenta(const TVector3 mom[]){
      for(int i=0; i<n_tracks; i++){
         SetInitialMomentum(i, mom[i]);
      }
   };
   void SetCharge(int track_id, double charge);
   void SetCharges(double charge[]){
      for(int i=0; i<n_tracks; i++){
         SetCharge(i, charge[i]);
      }
   };

   /* Add measured points (hits) */
   //    track_id    : ID number of tracks in this class
   //    layer_order : the order of crossing detection in tracking
   //    layer_id    : GTR layer id (100 mm -> 0, 200 mm -> 1, 300 mm -> 2) used for global -> local transformation
   //    module_id   : GTR module id (detector id) used for g-l transformation
   //    lpos        : measured local hit position
   //    sigma       : chi2 sigma (error)
   void AddHit(int track_id, int layer_order, int layer_id, int module_id, const TVector3 &lpos, const TVector3 &sigma); // for GTR
   void AddHit(int track_id, int layer_order, const E16ANA_DetectorGeometry *_geom, const TVector3 &lpos, const TVector3 &sigma); // for additional tracker
   // Clear hits
   void Clear();

   /* Fit routine */
   // if flag==true, vertex is fixed to initial value
   double Fit(bool vertexfixflag = false, bool pyfixflag = false);

   void PreConditioning(); // local-Z sort
   double RungeKuttaTracking(int track_id, const TVector3 &vertex, const TVector3 &momentum, double charge);
   /* for IMultiGenFunction */
   double MinuitFunction(const double *pars);
   unsigned int GetNumParameters(){return n_tracks*3+3;};

   // default value is 0.1 cm (1.0 mm)
   void SetRungeKuttaStepSize(double step){
      rungekutta_step_size = step;
   };

   /* Accessors */
   TVector3 GetFitVertex(){return vertex_fit;};
   TVector3 GetFitMomentum(int i){return momentum_fit[i];};
   void GetFitMomenta(TVector3 mom[]){
      for(int i=0; i<n_tracks; i++){
         mom[i] = momentum_fit[i];
      }
   };
   int GetMinimizeStatus(){return minimize_status;};
   int GetMatrixStatus(){return matrix_status;};
   /* Get TVector3s from Hit_t */
   // Module wide (module-by-module)
   //    track_id     : input, ID number of tracks in this class
   //    layer_order  : input, same as AddHit
   //    module_index : input, order number of registered module in AddHit (not module id)
   //    module_id    : output, GTR module id (detector id), it will be -1 if inputs are invalid
   //    lposs        : output, input local_pos or fit result (array of TVector3)
   //    return value : size of lposs
   int GetFitLPos( // module wide
         int track_id, int layer_order, int module_index, // inputs
         int &module_id, std::vector<TVector3> &lposs // outputs
         ){
      return GetTVector3(track_id, layer_order, module_index,
            module_id, lposs,
            &Hit_t::GetFitLPos);
   };
   int GetLPos( // module wide
         int track_id, int layer_order, int module_index, // inputs
         int &module_id, std::vector<TVector3> &lposs // outputs
         ){
      return GetTVector3(track_id, layer_order, module_index,
            module_id, lposs,
            &Hit_t::GetLPos);
   };
   int GetFitLMom( // module wide
         int track_id, int layer_order, int module_index, // inputs
         int &module_id, std::vector<TVector3> &lposs // outputs
         ){
      return GetTVector3(track_id, layer_order, module_index,
            module_id, lposs,
            &Hit_t::GetFitLMom);
   };
   int GetFitResidual( // module wide
         int track_id, int layer_order, int module_index, // inputs
         int &module_id, std::vector<TVector3> &lposs // outputs
         ){
      return GetTVector3(track_id, layer_order, module_index,
            module_id, lposs,
            &Hit_t::GetFitResidual);
   };
   // Layer wide (layer-by-layer)
   //    track_id     : input, ID number of tracks in this class
   //    layer_order  : input, same as AddHit
   //    module_ids   : output, array of GTR module id (detector id)
   //    lposs        : output, input local_pos or fit result (array of TVector3)
   //    return value : size of module_ids and lposs
   int GetFitLPos( // layer wide
         int track_id, int layer_order, // inputs
         std::vector<int> &module_ids, std::vector<TVector3> &lposs // outputs
         ){
      return GetTVector3(track_id, layer_order,
            module_ids, lposs,
            &Hit_t::GetFitLPos);
   };
   int GetLPos( // layer wide
         int track_id, int layer_order, // inputs
         std::vector<int> &module_ids, std::vector<TVector3> &lposs // outputs
         ){
      return GetTVector3(track_id, layer_order,
            module_ids, lposs,
            &Hit_t::GetLPos);
   };
   int GetFitLMom( // layer wide
         int track_id, int layer_order, // inputs
         std::vector<int> &module_ids, std::vector<TVector3> &lposs // outputs
         ){
      return GetTVector3(track_id, layer_order,
            module_ids, lposs,
            &Hit_t::GetFitLMom);
   };
   int GetFitResidual( // layer wide
         int track_id, int layer_order, // inputs
         std::vector<int> &module_ids, std::vector<TVector3> &lposs // outputs
         ){
      return GetTVector3(track_id, layer_order,
            module_ids, lposs,
            &Hit_t::GetFitResidual);
   };

   std::vector<TVector3>& GetTrackSteps(int track_id){
      return track_steps[track_id];
   };

   void SetMomentumStepSize(double step){momentum_step_size = step;};

   void PrintHits();
   void SetPrintLevel(int i){print_level = i;};
   const static double chisq_sigma_min;

private:
   enum {kInvalidValue = -100000};
   E16ANA_MagneticFieldMap *bfield_map;
   E16ANA_GeometryV2 *geom;
   int n_tracks;
   double *track_charge;
   double rungekutta_step_size;
   int print_level;
   double momentum_step_size;

   std::vector<LayerHits_t> *GTRlPos; // n_tracks * n_layers * n_modules * n_hits_per_chamber : 4D-Array

   TVector3 *momentum_init; // initial value
   TVector3 vertex_init; // initial value
   TVector3 *momentum_fit; // fit result
   TVector3 vertex_fit; // fit result
   TVector3 vertex_sigma; // chisq sigma
   std::vector<TVector3> *track_steps; // tracking result (for event display)

   //double dr0_[3];
   //double rR0_[3],rL0_[3];
   //double fMinimumStep;
   //double fMaximumStep;
   //double StepLength_;
   //double GTRStepLength[6];
   //double ncallfmf_;

   int minimize_status;
   // 0 = Error, 1 = Success ???
   int matrix_status;
   // MNSTAT
   //               matrix: 0 = not calculated at all
   //*-*                    1 = approximation only, not accurate
   //*-*                    2 = full matrix, but forced positive-definite
   //*-*                    3 = full accurate covariance matrix

   struct Hit_t {
      TVector3 lpos;
      TVector3 sigma;
      //Plane_t plane;
      TVector3 lpos_fit;
      TVector3 lmom_fit;
      TVector3 residual_fit;
      const E16ANA_DetectorGeometry *geometry;

      //Hit_t(const TVector3 &_lpos, const TVector3 &_sigma, const Plane_t &_plane){
      Hit_t(const TVector3 &_lpos, const TVector3 &_sigma, const E16ANA_DetectorGeometry *_geometry){
         lpos = _lpos;
         sigma = _sigma;
         //plane = _plane;
         geometry = _geometry;
         lpos_fit = TVector3(kInvalidValue,kInvalidValue,kInvalidValue);
         lmom_fit = TVector3(kInvalidValue,kInvalidValue,kInvalidValue);
         residual_fit = TVector3(kInvalidValue,kInvalidValue,kInvalidValue);
      };
      TVector3& GetLPos(){return lpos;};
      TVector3& GetFitLPos(){return lpos_fit;};
      TVector3& GetFitLMom(){return lmom_fit;};
      TVector3& GetFitResidual(){return residual_fit;};
      double Chisquare(const TVector3 &flpos, const TVector3 &flmom){ // chi2 calculation and update of lpos_fit
         double res_x = (lpos.X()-flpos.X())/sigma.X();
         double res_y = (lpos.Y()-flpos.Y())/sigma.Y();
         if(sigma.X() < chisq_sigma_min) res_x = 0.0;
         if(sigma.Y() < chisq_sigma_min) res_y = 0.0;
         lpos_fit = flpos;
         lmom_fit = flmom;
         residual_fit = lpos-lpos_fit;
         return res_x*res_x+res_y*res_y;
      };
      bool IsCrossed(double r0[3], double r1[3], TVector3 &cp_lpos){
         return geometry->IsCrossed(r0, r1, cp_lpos, lpos.Z());
      };
   };
   struct LessModuleHit { // predicator for sort of TVector3
      bool operator()(const Hit_t *lhs, const Hit_t *rhs){
         return (lhs->lpos).Z() < (rhs->lpos).Z();
      };
   };
   struct LayerHits_t {
      int layer_order;
      int layer_id;
      std::vector<ModuleHits_t> local_hits; // vector of pair of module_id and local_pos
      LayerHits_t(int _order, int _id){
         layer_order = _order;
         layer_id = _id;
      };
      int NumHitModules(){
         return local_hits.size();
      };
      int ModuleId(int i){
         return local_hits[i].first;
      };
      //ROOT::Math::Plane3D& FirstPlaneInModule(int i){
      //   return (local_hits[i].second.front())->plane;
      //};
      Hit_t* FirstHitInModule(int i){
         return local_hits[i].second.front();
      }
      std::vector<Hit_t*>& HitsInModule(int i){
         return local_hits[i].second;
      }
      bool Empty(){
         return local_hits.empty();
      };
      void Erase(int i){
         local_hits.erase(local_hits.begin()+i);
      };
      void Sort(){
         for(int i=0; i<(int)local_hits.size(); i++){
            std::sort(local_hits[i].second.begin(), local_hits[i].second.end(), LessModuleHit());
         }
      };
      void Delete(){
         for(int i=0; i<(int)local_hits.size(); i++){
            std::vector<Hit_t*> &vecs = local_hits[i].second;
            for(int j=0; j<(int)vecs.size(); j++){
               delete vecs[j];
            }
            vecs.clear();
         }
         local_hits.clear();
      }
   };

   void EOM(double mom, double r[], double dr[], double d2r[]);//3 components
   void rkn4(double mom, double y[], double dydx[], double h, double yout[], double dydxout[]);
   //double RungeKuttaTracking(int track_id, double dstp, double mom, double r0[], double dr0[]); // RK and chisq calculation (return value) for 1 track
   int RungeKuttaTrackingToNextHit(int track_id, LayerHits_t &next_layer_hits, double dstp, double mom,
         double r0[], double dr0[], // pre point and direction
         double r1[], double dr1[]); // next point and direction
   //bool IsCrossed(Plane_t &plane, double r0[], double r1[], TVector3 &cross_point);
   bool IsTrackingRegion(double r[]){
      return    (sqrt(r[0]*r[0]+r[2]*r[2]) < 2000.0) // mm
             && (fabs(r[1]) < 2000.0); // mm
   };
   double CalcVertexChisquare();
   double RungeKuttaTracking(int track_id, double dstp, double mom, double r0[], double dr0[]); // RK and chisq calculation (return value) for 1 track
   int GetTVector3( // module wide
         int track_id, int layer_order, int module_index, // inputs
         int &module_id, std::vector<TVector3> &lposs, // outputs
         TVector3& (Hit_t::*pfunc)()
         ){
      int ret = 0;
      lposs.clear();
      LayerHits_t &layer = GTRlPos[track_id][layer_order];
      if(layer.NumHitModules() <= module_index){
         module_id = -1;
         return ret;
      }
      std::vector<Hit_t*> &module = layer.HitsInModule(module_index);
      module_id = layer.ModuleId(module_index);
      for(int i=0; i<(int)module.size(); i++){
         lposs.push_back((module[i]->*pfunc)());
         ret++;
      }
      return ret;
   };
   int GetTVector3( // layer wide
         int track_id, int layer_order, // inputs
         std::vector<int> &module_ids, std::vector<TVector3> &lposs, // outputs
         TVector3& (Hit_t::*pfunc)()
         ){
      int ret = 0;
      lposs.clear();
      module_ids.clear();
      LayerHits_t &layer = GTRlPos[track_id][layer_order];
      for(int i=0; i<layer.NumHitModules(); i++){
         std::vector<Hit_t*> &module = layer.HitsInModule(i);
         int module_id = layer.ModuleId(i);
         for(int j=0; j<(int)module.size(); j++){
            module_ids.push_back(module_id);
            lposs.push_back((module[j]->*pfunc)());
            ret++;
         }
      }
      return ret;
   };
   void PrintHits(LayerHits_t &layer);
};

class E16ANA_MultiTrackFunction : public ROOT::Math::IMultiGenFunction {
public:
   E16ANA_MultiTrackFunction(E16ANA_MultiTrack *_multi_track){multi_track = _multi_track;};
   ~E16ANA_MultiTrackFunction(){};
   ROOT::Math::IMultiGenFunction* Clone() const;
   double operator()(const double *x) const {
      return multi_track->MinuitFunction(x);
   };
   unsigned int NDim() const {return multi_track->GetNumParameters();};

private:
   E16ANA_MultiTrack *multi_track;
   double DoEval(const double *x) const {return multi_track->MinuitFunction(x);};
};

#endif // E16ANA_MultiTrack_h
