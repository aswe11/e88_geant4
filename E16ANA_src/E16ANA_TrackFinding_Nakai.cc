//2016-11-22, uploaded by nakai
#include "E16ANA_TrackFinding_Nakai.hh"

//#include "E16ANA_TrackFinding_Nakai_160805.cc"
//#include "E16ANA_TrackFinding_Nakai_160829.cc"
#include "E16ANA_TrackFinding_Nakai_161006.cc"

#include <iostream>
#include <cmath>

#include <TVector3.h>

std::vector<int> E16ANA_TrackFinding_Nakai::LinkedModuleIDs(int module){
   std::vector<int> ids;
   if(module/3 == 0){
      ids.push_back(module);
      ids.push_back(module+3);
   }else if(module/3 == 10){
      ids.push_back(module-3);
      ids.push_back(module);
   }else if(module == 13){
      ids.push_back(module-3);
      ids.push_back(module);
   }else if(module == 19){
      ids.push_back(module);
      ids.push_back(module+3);
   }else{
      ids.push_back(module-3);
      ids.push_back(module);
      ids.push_back(module+3);
   }
   return ids;
}

double E16ANA_SingleTrackCandidate_Nakai::SingleFit(E16ANA_MultiTrack *fitter, bool vertexfixflag, bool pyfixflag){
   fitter->Clear();
   this->SetSingleTrackHit(fitter);
   chisq = fitter->Fit(vertexfixflag, pyfixflag);
   mom_fit = fitter->GetFitMomentum(0);
   vtx_fit = fitter->GetFitVertex();
   UpdateAnaHits();
   UpdateFitHits(fitter);
   return chisq;
}

void E16ANA_SingleTrackCandidate_Nakai::UpdateFitHits(E16ANA_MultiTrack *fitter, int track_id){
   for(int i=0; i<n_layers; i++){
      hits_fit[i].clear();
      std::vector<TVector3> lpos;
      std::vector<TVector3> lmom;
      std::vector<int> module_id;
      fitter->GetFitLPos(track_id, i, module_id, lpos);
      fitter->GetFitLMom(track_id, i, module_id, lmom);
      for(int j=0; j<(int)module_id.size(); j++){
         hits_fit[i].push_back(E16ANA_G4Hit());
         E16ANA_G4Hit &hit = hits_fit[i].back();
         TVector3 gpos;
         TVector3 gmom;
         //std::cout << i << " " << module_id[j] << std::endl;
         if(i==0){
            gpos = geom->SSD(module_id[j])->GetGPos(lpos[j]);
            gmom = geom->SSD(module_id[j])->GetGMom(lmom[j]);
         }else if(i>=1 && i<=3){
            gpos = geom->GTR(module_id[j], i-1)->GetGPos(lpos[j]);
            gmom = geom->GTR(module_id[j], i-1)->GetGMom(lmom[j]);
         }
         int event_id = 0, temp_track_id = 0, pid = 0;
         hit.SetID(event_id, temp_track_id, module_id[j], pid);
         hit.SetXYZ(lpos[j]);
         hit.SetP(lmom[j]);
         hit.SetGXYZ(gpos);
         hit.SetGP(gmom);
      }
   }
}

void E16ANA_SingleTrackCandidate_Nakai::UpdateAnaHits(){
   for(int i=0; i<n_layers; i++){
      hits_ana[i].clear();
   }
   for(int l=0; l<n_layers; l++){
      for(int i=0; i<(int)hits[l].size(); i++){
         //int l = hits[i].layer_order;
         int m = hits[l][i].module_id;
         hits_ana[l].push_back(E16ANA_G4Hit());
         E16ANA_G4Hit &hit = hits_ana[l].back();
         TVector3 lpos = hits[l][i].lpos;
         //TVector3 lmom = hits[i].lmom;
         TVector3 gpos = hits[l][i].gpos;
         //TVector3 gmom = hits[i].gmom;
         if(hits[l][i].sigma.X() < E16ANA_MultiTrack::chisq_sigma_min){
            lpos.SetX(-10000.0);
         }
         if(hits[l][i].sigma.Y() < E16ANA_MultiTrack::chisq_sigma_min){
            lpos.SetY(-10000.0);
         }
         int event_id = 0, temp_track_id = 0, pid = 0;
         hit.SetID(event_id, temp_track_id, m, pid);
         hit.SetXYZ(lpos);
         //hit.SetP(lmom);
         hit.SetGXYZ(gpos);
         //hit.SetGP(gmom);
      }
   }
}

void E16ANA_SingleTrackCandidate_Nakai::SetSingleTrackHit(E16ANA_MultiTrack *single_track, int _track_id){
   //single_track->Clear();
   for(int i=0; i<n_layers; i++){
      set_flag[i] = 0;
   }
   int module_id = -1;
   TVector3 vtx_err(1.5, 1.7, 20e-3);
   //single_track->Initialize(vtx, vtx_err, &mom, &charge);
   single_track->SetInitialVertex(vtx, vtx_err);
   single_track->SetInitialMomentum(_track_id, mom);
   single_track->SetCharge(_track_id, charge);
   //std::cout << "SetSingleTrackHit : initialMomentum = "; mom.Print();
   //std::cout << "SetSingleTrackHit : initialVertex = "; vtx.Print();
   for(int l=0; l<n_layers; l++){
      for(int i=0; i<(int)hits[l].size(); i++){
         single_track->AddHit(
               //track_id, hits[i].layer_order,
               //0, hits[i].layer_order,
               _track_id, hits[l][i].layer_order,
               hits[l][i].geom,
               hits[l][i].lpos, hits[l][i].sigma
               );
         module_id = hits[l][i].module_id;
         set_flag[hits[l][i].layer_order] = 1;
      }
   }
   //std::cout << "SetSingleTrackHit : set_flag =";
   //for(int i=0; i<4; i++){
   //   std::cout << " " << set_flag[i];
   //}
   //std::cout << ", module_id = " << module_id << std::endl;
   std::vector<int> linked = E16ANA_TrackFinding_Nakai::LinkedModuleIDs(module_id);
   for(int i=1; i<4; i++){
      if(set_flag[i] == 0){
         for(int j=0; j<(int)linked.size(); j++){
            single_track->AddHit(
                  _track_id, i,
                  //0, i,
                  geom->GTR(linked[j], i-1),
                  TVector3(-10000.,-10000.,0.0), TVector3(0.,0.,0.)
                  );
         }
      }
   }
      //if(set_flag[4] == 0){
      //   single_track->AddHit(
      //         track_id, 4,
      //         geom->HBD(module_id),
      //         TVector3(-10000.,-10000.,0.0), TVector3(0.,0.,0.)
      //         );
      //}

}

TVector3 E16ANA_SingleTrackCandidate_Nakai::ProjectionHBD(E16ANA_MultiTrack *fitter, E16ANA_G4Hit &hbd_hit){
   fitter->Clear();
   fitter->AddHit(0, 0, geom->HBD(hbd_hit.ModuleID()),
         hbd_hit.XTV(), TVector3(0.0,0.0,0.0));
   fitter->RungeKuttaTracking(0, vtx_fit, mom_fit, charge);
   std::vector<int> module_id;
   std::vector<TVector3> lpos;
   fitter->GetFitLPos(0, 0, module_id, lpos);
   if(!lpos.empty()){
      return lpos[0];
   }
   return TVector3(-10000.0, -10000.0, 0.0);
}

bool E16ANA_SingleTrackCandidate_Nakai::HBDMatch(
      E16ANA_MultiTrack *fitter, E16ANA_G4Hit &hbd_hit, int xy, double match_distance){
   //std::cout << "E16ANA_SingleTrackCandidate_Nakai::HBDMatch : module_id = " << hbd_hit.ModuleID() << std::endl;
   //std::cout << "    HBD g4 hit = (" << hbd_hit.X() << ", " << hbd_hit.Y() << ")" << std::endl;
   //const double match_distance = 50.0;
   fitter->Clear();
   int use_x = xy & 0x1;
   int use_y = (xy & 0x2)>>1;
   fitter->AddHit(0, 0, geom->HBD(hbd_hit.ModuleID()),
         hbd_hit.XTV(), TVector3(1.0*use_x,1.0*use_y,0.0));
   double distance = fitter->RungeKuttaTracking(0, vtx_fit, mom_fit, charge);
   distance = sqrt(distance);
   std::vector<int> module_ids;
   std::vector<TVector3> lposs;
   fitter->GetFitLPos(0, 0, module_ids, lposs);
   double half_size = 400.0;
   bool hbd_hitted = false;
   if((int)module_ids.size() > 0){
      //std::cout << "    HBD cross point = (" << lposs[0].X() << ", " << lposs[0].Y() << ")"
      //   << ", distance = " << distance << std::endl;
      //hbd_hitted = (fabs(lposs[0].X()) < half_size) && (fabs(lposs[0].Y() < half_size));
      hbd_hitted = true;
   }
   return hbd_hitted && (distance < match_distance);
}

double E16ANA_SingleTrackCandidate_Nakai::LinearFitY(bool hbd_check, std::vector<E16ANA_G4Hit> &hbd_hits){
   std::vector<std::pair<TVector3, double> > gpos_ry; // gpos, sigma
   gpos_ry.push_back(std::pair<TVector3, double>(TVector3(0.0, 0.0, 0.0), 1.7));
   for(int l=0; l<n_layers; l++){
      for(int i=0; i<(int)hits[l].size(); i++){
         gpos_ry.push_back(std::pair<TVector3, double>(hits[l][i].gpos, hits[l][i].sigma.Y()));
         double r = hits[l][i].gpos.X()*hits[l][i].gpos.X();
         r += hits[l][i].gpos.Z()*hits[l][i].gpos.Z();
         gpos_ry.back().first.SetX(sqrt(r));
         gpos_ry.back().first.SetZ(0.0);
      }
   }
   double x = 0.0, y = 0.0, xx = 0.0, xy = 0.0, n = 0.0;
   for(int i=0; i<(int)gpos_ry.size(); i++){
      TVector3 &v = gpos_ry[i].first;
      double s = gpos_ry[i].second;
      x += v.X()/s/s;
      y += v.Y()/s/s;
      xx += v.X()*v.X()/s/s;
      xy += v.X()*v.Y()/s/s;
      n += 1.0/s/s;
   }
   double a = (n*xy-x*y)/(n*xx-x*x);
   double b = (xx*y-xy*x)/(n*xx-x*x);
   chisq = 0.0;
   for(int i=0; i<(int)gpos_ry.size(); i++){
      TVector3 &v = gpos_ry[i].first;
      double s = gpos_ry[i].second;
      double error = (v.Y()-a*v.X()-b)/s;
      chisq += error*error;
   }
   if(hbd_check){
      bool hbd_matched = false;
      double hbd_r = 1200.0;
      double extrapolate_gy = a*hbd_r + b;
      for(int i=0; i<(int)hbd_hits.size(); i++){
         TVector3 gpos = geom->HBD(hbd_hits[i].ModuleID())->GetGPos(hbd_hits[i].XTV());
         if(fabs(gpos.Y()-extrapolate_gy) < 40.0){
         //if(fabs(gpos.Y()-extrapolate_gy) < 20.0){
            hbd_matched = true;
         }
      }
      if(!hbd_matched){
         chisq += 100000.0;
      }
   }
   //std::cout << "LinearFitY : chisq = " << chisq << std::endl;
   return chisq;
}

TVector3 E16ANA_SingleTrackCandidate_Nakai::ConformalTransformation(const TVector3 &h){
   double u = h.X()/(h.X()*h.X()+h.Z()*h.Z());
   double v = h.Z()/(h.X()*h.X()+h.Z()*h.Z());
   return TVector3(u,0,v);
}

double E16ANA_SingleTrackCandidate_Nakai::CalcPhaseDiff(const TVector3 &v0, const TVector3 &v1){
   double p0 = atan2(v0.X(), v0.Z());
   double p1 = atan2(v1.X(), v1.Z());
   double diff = p0 - p1;
   if(diff > TMath::Pi()){
      diff = diff-2.0*TMath::Pi();
   }else if(diff < -TMath::Pi()){
      diff = diff+2.0*TMath::Pi();
   }
   return diff;
}

TVector3 E16ANA_SingleTrackCandidate_Nakai::CalcRoughMomentum(const TVector3 &gxz0, const TVector3 &gxz1){
   TVector3 rough_mom;
   TVector3 u0 = ConformalTransformation(gxz0);
   TVector3 u1 = ConformalTransformation(gxz1);
   double By = 1.0/1.5;
   double a = 1.0/(u0.X()*u1.Z()-u0.Z()*u1.X())*(u1.Z()-u0.Z());
   double b = 1.0/(u0.X()*u1.Z()-u0.Z()*u1.X())*(u0.X()-u1.X());
   double r = sqrt(a*a+b*b);
   double p = 0.3*(r/1000.0)*By;
   //double phase_diff = calc_phase_diff(TVector3(gxz1.X()-a,0.0,gxz1.Z()-b),TVector3(-a,0.0,-b));
   double phase_diff = CalcPhaseDiff(gxz1,gxz0);
   double theta = CalcPhaseDiff(TVector3(gxz1.X()-a,0.0,gxz1.Z()-b),TVector3(-a,0.0,-b));
   double flight_length = fabs(theta)*r;
   double py = p*gxz1.Y()/flight_length;
   if(phase_diff > 0.0){
      rough_mom.SetXYZ(-b,0.0,+a);
      rough_mom = rough_mom.Unit()*p;
   }else{
      rough_mom.SetXYZ(+b,0.0,-a);
      rough_mom = rough_mom.Unit()*p;
   }
   rough_mom.SetY(py);
   return rough_mom;
}

bool E16ANA_SingleTrackCandidate_Nakai::CalcRoughMomentum(){
   UpdateAnaHits();
   int l0 = -1, l1 = -1, i0 = -1, i1 = -1;
   for(int l=0; l<n_layers; l++){
      for(int i=0; i<(int)hits[l].size(); i++){
         Hit_t &h = hits[l][i];
         if(l0 < 0){
            l0 = h.layer_order;
            i0 = i;
         }else if(l0 != h.layer_order){
            l1 = h.layer_order;
            i1 = i;
            break;
         }
      }
   }
   if(l0 < 0 || l1 < 0 || l0 == l1){
      return false;
   }else if(l0 < l1){
      mom = CalcRoughMomentum(hits[l0][i0].gpos, hits[l1][i1].gpos);
   }else{
      mom = CalcRoughMomentum(hits[l1][i1].gpos, hits[l0][i0].gpos);
   }
   return true;
}

bool E16ANA_SingleTrackCandidate_Nakai::IsSameModuleID(int x, int y){ // x and y are coded module IDs. See Hit_t in header file.
   //std::cout << "E16ANA_SingleTrackCandidate_Nakai::IsSameModuleID : x = " << std::hex << x << ", y = " << std::hex << y << std::dec << std::endl;
   int xy = x ^ y;
   return ((xy & 0x03) != 0x03) && ((xy & 0xffffff00) == 0x00);
}

bool E16ANA_SingleTrackCandidate_Nakai::IsSameModuleID(const std::vector<Hit_t> &x, const std::vector<Hit_t> &y){
   //std::cout << "E16ANA_SingleTrackCandidate_Nakai::IsSameModuleID : " << x.size() << ", " << y.size() << std::endl;
   bool flag = !x.empty();
   for(int i=0; i<(int)x.size(); i++){
      bool ftemp = false;
      for(int j=0; j<(int)y.size(); j++){
         ftemp = ftemp || IsSameModuleID(x[i].GetCodedModuleID(), y[j].GetCodedModuleID());
      }
      flag = flag && ftemp;
   }
   for(int j=0; j<(int)y.size(); j++){
      bool ftemp = false;
      for(int i=0; i<(int)x.size(); i++){
         ftemp = ftemp || IsSameModuleID(x[i].GetCodedModuleID(), y[j].GetCodedModuleID());
      }
      flag = flag && ftemp;
   }
   return flag;
}

/*
bool E16ANA_EventSelection_Nakai::SelectionBySSDHBD(E16ANA_SSDResponse *ssd_res[], std::vector<E16ANA_SingleTrackCandidate_Nakai> &candidates){
   int ssd_module[27][3] = {
      // HBD = 0, 1, 2
      { 0, 3, 6},
      { 1, 4, 7},
      { 2, 5, 8},
      // HBD = 3, 4, 5
      { 0, 3, 6},
      { 1, 4, 7},
      { 2, 5, 8},
      // HBD = 6, 7, 8
      { 3, 6, 9},
      { 4, 7,10},
      { 5, 8,11},
      // HBD = 9, 10, 11
      { 9,12,15},
      {10,13,16},
      {11,14,17},
      // HBD = 12, 13, 14
      {12,15,18},
      {13,16,19},
      {14,17,20},
      // HBD = 15, 16, 17
      {15,18,21},
      {16,19,22},
      {17,20,23},
      // HBD = 18, 19, 20
      {21,24,27},
      {22,25,28},
      {23,26,29},
      // HBD = 21, 22, 23
      {24,27,30},
      {25,28,31},
      {26,29,32},
      // HBD = 24, 25, 26
      {24,27,30},
      {25,28,31},
      {26,29,32}
   };
   return true;
}
*/

E16ANA_DoubleTrackCandidate_Nakai::E16ANA_DoubleTrackCandidate_Nakai(
      const E16ANA_SingleTrackCandidate_Nakai &_single_plus,
      const E16ANA_SingleTrackCandidate_Nakai &_single_minus) : 
   single_plus(_single_plus), single_minus(_single_minus), chisq(0.0)
{
   //single_plus.SetMomentum(_single_plus.GetFitMomentum());
   //single_minus.SetMomentum(_single_minus.GetFitMomentum());
   //TVector3 vtx_init =
   //   (_single_plus.GetFitVertex()+_single_minus.GetFitVertex())
   //   *0.5;
   single_plus.SetMomentum(_single_plus.Momentum());
   single_minus.SetMomentum(_single_minus.Momentum());
   TVector3 vtx_init =
      (_single_plus.Vertex()+_single_minus.Vertex())
      *0.5;

   single_plus.SetVertex(vtx_init);
   single_minus.SetVertex(vtx_init);
}

E16ANA_DoubleTrackCandidate_Nakai::~E16ANA_DoubleTrackCandidate_Nakai(){
}

double E16ANA_DoubleTrackCandidate_Nakai::DoubleFit(E16ANA_MultiTrack *fitter){
   fitter->Clear();
   single_plus .SetSingleTrackHit(fitter, 0);
   single_minus.SetSingleTrackHit(fitter, 1);
   chisq = fitter->Fit(false);
   mom_fit_plus  = fitter->GetFitMomentum(0);
   mom_fit_minus = fitter->GetFitMomentum(1);
   vtx_fit       = fitter->GetFitVertex();
   single_plus .UpdateAnaHits();
   single_minus.UpdateAnaHits();
   single_plus .UpdateFitHits(fitter, 0);
   single_minus.UpdateFitHits(fitter, 1);
   return chisq;
}

void E16ANA_DoubleTrackCandidate_Nakai::Print(){
   std::cout << "E16ANA_DoubleTrackCandidate_Nakai : " << std::endl;
   single_plus.Print();
   single_minus.Print();
   std::cout << "    Double fit momentum (plus) = (" << mom_fit_plus.X() << ", " << mom_fit_plus.Y() << ", " << mom_fit_plus.Z() << ")" << std::endl;
   std::cout << "    Double fit momentum (minus) = (" << mom_fit_minus.X() << ", " << mom_fit_minus.Y() << ", " << mom_fit_minus.Z() << ")" << std::endl;
   std::cout << "    Double fit vertex = (" << vtx_fit.X() << ", " << vtx_fit.Y() << ", " << vtx_fit.Z() << ")" << std::endl;
   std::cout << "    Double fit chisq = " << chisq << std::endl;
}

E16ANA_TrackFinding_Nakai::E16ANA_TrackFinding_Nakai(
         E16ANA_G4Event *_g4event,
         E16ANA_SSDResponse *_ssd_res[],
         E16ANA_GTRAnalyzerManager *_gtr_ana,
         E16ANA_HBDResponseManager *_hbd_res,
         E16ANA_MagneticFieldMap *_bfield,
         E16ANA_GeometryV2 *_geom
      ) : 
   g4event(_g4event), gtr_ana(_gtr_ana), hbd_res(_hbd_res), bfield_map(_bfield), geom(_geom)
{
   for(int i=0; i<n_ssd_modules; i++){
      ssd_res[i] = _ssd_res[i];
      ssd_g4hit[i] = E16ANA_G4Hit();
   }
   for(int i=0; i<n_gtr_modules; i++){
      for(int j=0; j<3; j++){
         gtr_g4hit[i][j] = E16ANA_G4Hit();
      }
   }
}

E16ANA_TrackFinding_Nakai::~E16ANA_TrackFinding_Nakai(){
}

void E16ANA_TrackFinding_Nakai::SingleFit(E16ANA_SingleTrackCandidate_Nakai &single, int track_id){
   E16ANA_MultiTrack *single_track = new E16ANA_MultiTrack(bfield_map,geom,1);
   //single_track->SetPrintLevel(2);
   single.SetSingleTrackHit(single_track);
   //single_track->PrintHits();
   //double chisq = single_track->Fit(false);
   single_track->Fit(false);
   //double chisq = single_track->RungeKuttaTracking(0, single.Vertex(), single.Momentum(), single.GetCharge());
   TVector3 mom = single_track->GetFitMomentum(0);
   //std::cout << "3 points fit chisq = " << chisq << std::endl;
   //std::cout << "Fit momentum = (" << mom.X() << ", " << mom.Y() << ", " << mom.Z() << ")" << std::endl;
   //std::cout << "3-PointsFit momentum : "; mom.Print();
   if(track_id == 0 || track_id == 1){
   for(int i=0; i<3; i++){
      int layer_id = i;
      int layer_order = layer_id+1;
      std::vector<TVector3> lpos;
      std::vector<TVector3> lmom;
      std::vector<int> module_id;
      //multi_track->GetFitLPos(track_id, layer_order, module_id, lpos);
      single_track->GetFitLPos(0, layer_order, module_id, lpos);
      //for(int j=0; j<(int)lpos.size(); j++){
      //   std::cout << "lpos_fit " << j << " : "; lpos[j].Print();
      //}
      //multi_track->GetLPos(track_id, layer_id, module_id, lpos);
      //for(int j=0; j<(int)lpos.size(); j++){
      //   std::cout << "lpos_mes " << j << " : "; lpos[j].Print();
      //}
      TVector3 gpos = geom->GetGPos(lpos[0], layer_id, module_id[0]);
      //file->SetGTRPosMultiSingleFit(lpos, i+track_id*3);
   }
   {
      std::vector<TVector3> lpos;
      std::vector<TVector3> lmom;
      std::vector<int> module_id;
      single_track->GetFitLPos(0, 0, module_id, lpos);
      single_track->GetFitLMom(0, 0, module_id, lmom);
      //file->SetSSDPosMultiSingleFit(lpos, track_id);
   }
   }

   delete single_track;
}

void E16ANA_TrackFinding_Nakai::GetSingleTrackCandidates(std::vector<E16ANA_SingleTrackCandidate_Nakai> &_candidates){
   //int gtr_module_id[n_ssd_modules] = {12,13,14,15,16,17,18,19,20};
   int n_candidates = 0;
   for(int i=0; i<n_ssd_modules; i++){
      std::vector<E16ANA_G4Hit> &ssd_hits = ssd_res[i]->AnalyzedHits();
      std::cout << "SSD : ModuleID = " << i << ", Multiplicity (time window) = " << ssd_res[i]->AnalyzedHits().size() << std::endl;
      int gtr_module_id = i;
      //std::vector<E16ANA_GTRAnalyzedHit> &gtr_hits = gtr_ana->AnalyzedHitXChamber(2, gtr_module_id[i]);
      std::vector<E16ANA_GTRAnalyzedHit> &gtr_hits = gtr_ana->AnalyzedHitXChamber(2, gtr_module_id);
      n_candidates += ssd_hits.size()*gtr_hits.size();
      for(int j=0; j<(int)ssd_hits.size(); j++){
         E16ANA_G4Hit &ssd = ssd_hits[j];
         double theta_ssd = atan2(ssd.GX(), ssd.GZ());
         //TVector3 rough_mom = ssd.GXTV().Unit();
         TVector3 rough_mom = ssd.GXTV();
         rough_mom.SetY(geom->SSD(ssd.ModuleID())->GetDetectorCenter().Y());
         rough_mom = rough_mom.Unit();
         for(int k=0; k<(int)gtr_hits.size(); k++){
            E16ANA_GTRAnalyzedHit &gtr = gtr_hits[k];
            //double hit_width = CalcClusterWidth(gtr.XstripHit());
            //if(hit_width > 5.5*0.35) continue;
            double theta_gtr = atan2(gtr.Position().X(), gtr.Position().Z());
            double charge = -1.0;
            if(theta_ssd < theta_gtr) charge = 1.0;
            E16ANA_SingleTrackCandidate_Nakai single(geom, 0, charge);
            single.SetMomentum(rough_mom);
            single.SetVertex(TVector3(0,0,0)); // center of magnet
            //single.SetVertex(g4event->PrimaryTracks(0).TVertex()); // true vertex
            single.AddSSDHit(ssd);
            single.AddGTRHit(2, gtr);
            //single.AddGTRHit(0, gtr);
            //single.AddGTRHit(1, gtr);
            _candidates.push_back(single);
            SingleFit(single);
         }
      }
   }
   //std::cout << "Number of single track candidates (SSD x GTR3) = " << n_candidates << std::endl;
   std::cout << "Number of single track candidates (SSD x GTR3) = " << _candidates.size() << std::endl;
}

void E16ANA_TrackFinding_Nakai::GetSingleTrackCandidatesNonPolluted(
      int m_ssd,
      int m_gtr, int gtr_layer,
      std::vector<E16ANA_SingleTrackCandidate_Nakai> &cand_plus,
      std::vector<E16ANA_SingleTrackCandidate_Nakai> &cand_minus,
      double hit_width_cut){

   const double chi2_threshold = 100.0;
   int n_candidates_p = 0;
   int n_candidates_m = 0;
   std::vector<E16ANA_G4Hit> &ssd_hits = ssd_res[m_ssd]->AnalyzedHits();
   std::vector<E16ANA_GTRAnalyzedHit> &gtr_hits = gtr_ana->AnalyzedHitXChamber(gtr_layer, m_gtr);
   E16ANA_MultiTrack *fitter = new E16ANA_MultiTrack(bfield_map,geom,1);
   fitter->SetMomentumStepSize(0.1);
   for(int j=0; j<(int)ssd_hits.size(); j++){
      E16ANA_G4Hit &ssd = ssd_hits[j];
      double theta_ssd = atan2(ssd.GX(), ssd.GZ());
      //TVector3 rough_mom = ssd.GXTV().Unit();
      //TVector3 rough_mom = ssd.GXTV();
      //rough_mom.SetY(geom->SSD(ssd.ModuleID())->GetDetectorCenter().Y());
      //rough_mom = rough_mom.Unit();
      for(int k=0; k<(int)gtr_hits.size(); k++){
         E16ANA_GTRAnalyzedHit &gtr = gtr_hits[k];
         double hit_width = CalcClusterWidth(gtr.XstripHit());
         if(hit_width > hit_width_cut) continue;
         double theta_gtr = atan2(gtr.Position().X(), gtr.Position().Z());
         double charge = -1.0;
         if(theta_ssd < theta_gtr) charge = 1.0;
         E16ANA_SingleTrackCandidate_Nakai single(geom, 0, charge);
         //single.SetMomentum(rough_mom);
         single.SetVertex(TVector3(0,0,0)); // center of magnet
         single.AddSSDHit(ssd);
         single.AddGTRHit(gtr_layer, gtr);
         if(!single.CalcRoughMomentum()){
            continue;
         }
         //}else if(single.Momentum().Mag() < 0.3 || single.Momentum().Mag() > 3.0){
         //   continue;
         //}
         //if(single.Momentum().Mag() < 0.0001){
         //   std::cout << "WARNING : Rough memntum estimation" << std::endl;
         //   single.Print();
         //}
         double chi2 = single.SingleFit(fitter, true, true);
         //double chi2 = 0.0;
         single.UpdateInitialConditions();
         //if(chi2 > 1.0){
         //   chi2 = single.SingleFit(fitter, true, true);
         //   single.UpdateInitialConditions();
         //}
         single.ReAnalyzeTdc2(gtr_ana, gtr_layer+1);
         chi2 = single.SingleFit(fitter, true, true);
         single.UpdateInitialConditions();
         //TVector3 mom_fit = single.GetFitMomentum();
         //if(chi2 < 1.0 && mom_fit.Mag() > 0.3 && mom_fit.Mag() < 3.0){
         if(chi2 < chi2_threshold){
            if(charge > 0.0){
               cand_plus.push_back(single);
               n_candidates_p++;
            }else{
               cand_minus.push_back(single);
               n_candidates_m++;
            }
         }
      }
   }
   std::cout << "E16ANA_TrackFinding_Nakai::GetSingleTrackCandidatesNonPolluted()"
             << " : ssd_module = " << m_ssd
             << ", gtr_module = " << m_gtr
             << ", gtr_layer = " << gtr_layer
             << ", # of ssd hits = " << ssd_hits.size()
             << ", # of gtr hits = " << gtr_hits.size()
             << ", # of plus charge candidates = " << n_candidates_p
             << ", # of minus charge candidates = " << n_candidates_m << std::endl;
   delete fitter;
}

void E16ANA_TrackFinding_Nakai::SingleFitAllCandidates(
      std::vector<std::vector<E16ANA_SingleTrackCandidate_Nakai> > &cand,
      bool vertexfixflag, bool pyfixflag){
   E16ANA_MultiTrack *fitter = new E16ANA_MultiTrack(bfield_map,geom,1);
   for(int i=0; i<(int)cand.size(); i++){
      for(int j=0; j<(int)cand[i].size(); j++){
         //fitter->Clear();
         cand[i][j].SingleFit(fitter, vertexfixflag, pyfixflag);
         cand[i][j].UpdateInitialConditions();
      }
      ChisqSort(cand[i]);
   }
   delete fitter;
}

void E16ANA_TrackFinding_Nakai::ChisqCut(
      std::vector<std::vector<E16ANA_SingleTrackCandidate_Nakai> > &cand,
      double threshold
      ){
   for(int i=0; i<(int)cand.size(); i++){
      std::vector<E16ANA_SingleTrackCandidate_Nakai>::iterator it = cand[i].begin();
      while(it != cand[i].end()){
         if(it->GetChisq() > threshold){
            it = cand[i].erase(it);
         }else{
            it++;
         }
      }
   }
}

void E16ANA_TrackFinding_Nakai::DoubleFitAllCandidates(
      std::vector<E16ANA_DoubleTrackCandidate_Nakai> &cand){
   E16ANA_MultiTrack *fitter = new E16ANA_MultiTrack(bfield_map,geom,2);
   for(int i=0; i<(int)cand.size(); i++){
      cand[i].DoubleFit(fitter);
   }
   ChisqSort(cand);
   delete fitter;
}

void E16ANA_TrackFinding_Nakai::SelectOtherLayerHits(
      E16ANA_SingleTrackCandidate_Nakai &input,
      std::vector<E16ANA_SingleTrackCandidate_Nakai> &_candidates,
      bool exclude_rough_fit_layer){

   input.Print();
   double lx_fit[3] = {-20000.0, -20000.0, -20000.0};
   double lx_g4[3] = {-20000.0, -20000.0, -20000.0};
   double ly_g4[3] = {-20000.0, -20000.0, -20000.0};
   //int n_candidates = 0;
   //double near_cut = 3.0; // +- near_cut [mm]
   //std::vector<E16ANA_GTRAnalyzedHit> hit_candidate[3];
   int mm = -1;
   std::vector<E16ANA_GTRAnalyzedStripHit> hit_candidate[3];
   for(int layer=0; layer<3; layer++){
      if((input.GetSetFlag(layer+1) == 1) && exclude_rough_fit_layer){continue;} // rough fit layer
      std::vector<E16ANA_G4Hit> &hits_fit = input.GetFitHits(layer+1);
      if((int)hits_fit.size() == 0){continue;} // if error
      std::vector<E16ANA_GTRAnalyzedStripHit> gtr_hits;
      for(int i=0; i<(int)hits_fit.size(); i++){
         int module = hits_fit[i].ModuleID();
         mm = module;
         TVector3 lpos = hits_fit[i].XTV();
         lx_fit[layer] = lpos.X();
         //std::vector<E16ANA_GTRAnalyzedHit> &gtr_hits = gtr_ana->AnalyzedHitXChamber(layer, module);
         std::vector<E16ANA_GTRAnalyzedStripHit> ghits = gtr_ana->ReAnalyzeV4X(layer, hits_fit[i]);
         gtr_hits.insert(gtr_hits.end(), ghits.begin(), ghits.end());
      }
      for(int i=0; i<(int)gtr_hits.size(); i++){
         //double lx_ana = gtr_hits[i].Tdc2Hit();
         //double lx_fit = lpos.X();
         //if(fabs(lx_ana-lx_fit) < near_cut){
            hit_candidate[layer].push_back(gtr_hits[i]);
         //}
      }
      int true_flag = 1;
      //if(input.IsG4Closest(2, true_flag) || input.IsG4Closest(3, true_flag)){
      if(input.IsG4Closest(true_flag)){
      //if(fabs(hits_fit[0].X()-gtr_g4hit[module][layer].X()) < 5.0){
         SetG4ClosestTag(hit_candidate[layer],2,0.1); // TDC2
      }
   }
/*
   for(int layer=0; layer<3; layer++){
      for(int i=0; i<(int)hit_candidate[layer].size(); i++){
         bool is_inside = hit_candidate[layer][i].IsInside(-2.0, 2.0);
         bool is_multi_cluster = (hit_candidate[layer][i].NumHit() >= 3);
         if(is_inside && is_multi_cluster){ // very good hit
            E16ANA_GTRAnalyzedStripHit hit = hit_candidate[layer][i];
            hit_candidate[layer].clear();
            hit_candidate[layer].push_back(hit);
            break;
         }
      }
   }
*/
   int loop1 = ((input.GetSetFlag(1) == 1) && exclude_rough_fit_layer) ? 1 : hit_candidate[0].size();
   int loop2 = ((input.GetSetFlag(2) == 1) && exclude_rough_fit_layer) ? 1 : hit_candidate[1].size();
   int loop3 = ((input.GetSetFlag(3) == 1) && exclude_rough_fit_layer) ? 1 : hit_candidate[2].size();
   //int loop1 = hit_candidate[0].size();
   //int loop2 = hit_candidate[1].size();
   //int loop3 = hit_candidate[2].size();

   {
      int layer = 0;
      int noh = g4event->GTR1().Noh();
      for(int i=0; i<noh; i++){
         E16ANA_G4Hit &h = g4event->GTR1().Hit(i);
         if(h.ModuleID() == 16) continue;
         if((h.PID() == 11 || h.PID() == -11) && h.ModuleID() == mm){
            lx_g4[layer] = h.X();
            ly_g4[layer] = h.Y();
         }
      }
   }
   {
      int layer = 1;
      int noh = g4event->GTR2().Noh();
      for(int i=0; i<noh; i++){
         E16ANA_G4Hit &h = g4event->GTR2().Hit(i);
         if(h.ModuleID() == 16) continue;
         if((h.PID() == 11 || h.PID() == -11) && h.ModuleID() == mm){
            lx_g4[layer] = h.X();
            ly_g4[layer] = h.Y();
         }
      }
   }
   {
      int layer = 2;
      int noh = g4event->GTR3().Noh();
      for(int i=0; i<noh; i++){
         E16ANA_G4Hit &h = g4event->GTR3().Hit(i);
         if(h.ModuleID() == 16) continue;
         if((h.PID() == 11 || h.PID() == -11) && h.ModuleID() == mm){
            lx_g4[layer] = h.X();
            ly_g4[layer] = h.Y();
         }
      }
   }

   std::cout << "ModuleID = " << mm
      << " : Multiplicity (GTR1, GTR2, GTR3) = (" << loop1
      << ", " << loop2
      << ", " << loop3 << ")"
      << std::endl
      << "    GTR1 : lx_fit = " << lx_fit[0] << ", lx_g4 = " << lx_g4[0] << ", ly_g4 = " << ly_g4[0] << std::endl
      << "    GTR2 : lx_fit = " << lx_fit[1] << ", lx_g4 = " << lx_g4[1] << ", ly_g4 = " << ly_g4[1] << std::endl
      << "    GTR3 : lx_fit = " << lx_fit[2] << ", lx_g4 = " << lx_g4[2] << ", ly_g4 = " << ly_g4[2] << std::endl;
   int ii[3];
   for(ii[0]=0; ii[0]<loop1; ii[0]++){
      for(ii[1]=0; ii[1]<loop2; ii[1]++){
         for(ii[2]=0; ii[2]<loop3; ii[2]++){
            E16ANA_SingleTrackCandidate_Nakai new_single(input);
            //new_single.Clear();
            bool standalone_cut = false;
            for(int layer=0; layer<3; layer++){
               if((input.GetSetFlag(layer+1) == 0) || !exclude_rough_fit_layer){
                  E16ANA_GTRAnalyzedStripHit &hit = hit_candidate[layer][ii[layer]];
                  new_single.AddGTRHitX(hit);
                  //if(hit.NumHit() == 1){standalone_cut = true;}
                  //if((layer == 0 && hit.NumHit() == 1) || !hit.IsInside(-2.0, 2.0)){standalone_cut = true;}
                  //if(!hit.IsInside(-2.0, 2.0)){standalone_cut = true;}
               }
            }
            if(!standalone_cut){_candidates.push_back(new_single);}
            //n_candidates++;
         }
      }
   }
}

void E16ANA_TrackFinding_Nakai::GetSingleTrackCandidatesY(int m1, int m2, int m3, std::vector<E16ANA_SingleTrackCandidate_Nakai> &cand){
   std::vector<E16ANA_GTRAnalyzedHit> &gtr1_hits = gtr_ana->AnalyzedHitYChamber(0, m1);
   std::vector<E16ANA_GTRAnalyzedHit> &gtr2_hits = gtr_ana->AnalyzedHitYChamber(1, m2);
   std::vector<E16ANA_GTRAnalyzedHit> &gtr3_hits = gtr_ana->AnalyzedHitYChamber(2, m3);
   SetG4ClosestTag(gtr1_hits,1,0,0.5); // COG
   SetG4ClosestTag(gtr2_hits,1,0,0.5); // COG
   SetG4ClosestTag(gtr3_hits,1,0,0.5); // COG
   const int n_div = 10;
   double size1 = 100.0;
   double size2 = 200.0;
   double size3 = 300.0;
   double margin = 0.5;
   int n_candidates = 0;
   std::vector<E16ANA_GTRAnalyzedStripHit> gtr2_hits_div[n_div];
   std::vector<E16ANA_GTRAnalyzedStripHit> gtr3_hits_div[n_div];
   for(int i=0; i<n_div; i++){
      gtr2_hits_div[i].reserve(100);
      gtr3_hits_div[i].reserve(100);
   }
   for(int i=0; i<(int)gtr2_hits.size(); i++){
      E16ANA_GTRAnalyzedStripHit &hit = *(gtr2_hits[i].YstripHit());
      int div = floor((hit.CogHit()+size2*0.5)/(size2/(double)n_div));
      if(div < 0 || div >= n_div) continue;
      gtr2_hits_div[div].push_back(hit);
      double dy = hit.CogHit()+size2*0.5-div*(size2/(double)n_div);
      if(dy < size2/(double)n_div*margin){
         if(div-1 >= 0){
            gtr2_hits_div[div-1].push_back(hit);
         }
      }
      if(dy > size2/(double)n_div*(1.0-margin)){
         if(div+1 < n_div){
            gtr2_hits_div[div+1].push_back(hit);
         }
      }
   }
   for(int i=0; i<(int)gtr3_hits.size(); i++){
      E16ANA_GTRAnalyzedStripHit &hit = *(gtr3_hits[i].YstripHit());
      int div = floor((hit.CogHit()+size3*0.5)/(size3/(double)n_div));
      if(div < 0 || div >= n_div) continue;

      // g4 cut (Layer-3)
      double res = hit.CogHit() - gtr_g4hit[hit.ModuleID()][2].Y();
      if(fabs(res) > 10.0) continue;

      gtr3_hits_div[div].push_back(hit);
      double dy = hit.CogHit()+size3*0.5-div*(size3/(double)n_div);
      if(dy < size3/(double)n_div*margin){
         if(div-1 >= 0){
            gtr3_hits_div[div-1].push_back(hit);
         }
      }
      if(dy > size3/(double)n_div*(1.0-margin)){
         if(div+1 < n_div){
            gtr3_hits_div[div+1].push_back(hit);
         }
      }
   }

   for(int i=0; i<(int)gtr1_hits.size(); i++){
      E16ANA_GTRAnalyzedStripHit &hit1 = *(gtr1_hits[i].YstripHit());
      int div = floor((hit1.CogHit()+size1*0.5)/(size1/(double)n_div));
      for(int j=0; j<(int)gtr2_hits_div[div].size(); j++){
         E16ANA_GTRAnalyzedStripHit &hit2 = gtr2_hits_div[div][j];
         for(int k=0; k<(int)gtr3_hits_div[div].size(); k++){
            E16ANA_GTRAnalyzedStripHit &hit3 = gtr3_hits_div[div][k];
            E16ANA_SingleTrackCandidate_Nakai single(geom, 0, 0.0);
            single.AddGTRHitY(hit1);
            single.AddGTRHitY(hit2);
            single.AddGTRHitY(hit3);
            double chisq = single.LinearFitY();
            //double chisq = single.LinearFitY(true, hbd_res->AllAnalyzedHits());
            if(chisq < 200.0){
               cand.push_back(single);
               n_candidates++;
            }
         }
      }
   }
   std::cout << "E16ANA_TrackFinding_Nakai::GetSingleTrackCandidatesY : # of candidates = " << n_candidates << std::endl;
}

void E16ANA_TrackFinding_Nakai::GetSingleTrackCandidatesV4Y(int m1, int m2, int m3, std::vector<E16ANA_SingleTrackCandidate_Nakai> &cand){
   const int n_div = 10;
   const int n_div1 = 5;
   double size1 = 100.0;
   double size2 = 200.0;
   double size3 = 300.0;
   double margin = 0.5; // ratio
   double margin1 = 3.0; // mm
   int n_candidates = 0;
   std::vector<E16ANA_GTRAnalyzedStripHit> gtr1_hits;
   std::vector<E16ANA_GTRAnalyzedStripHit> gtr2_hits_div[n_div];
   std::vector<E16ANA_GTRAnalyzedStripHit> gtr3_hits_div[n_div];
   for(int i=0; i<n_div1; i++){
      double bin_width = size1/(double)n_div1;
      double bin_center = (i+0.5)*bin_width-size1*0.5;
      TVector3 lpos(0.0, bin_center, 0.0);
      TVector3 gmom = geom->GTR(m1, 0)->GetGPos(lpos);
      TVector3 lmom = geom->GTR(m1, 0)->GetLMom(gmom);
      E16ANA_G4Hit hit;
      hit.SetXYZ(lpos);
      hit.SetP(lmom);
      hit.SetID(0,0,m1,0);
      std::vector<E16ANA_GTRAnalyzedStripHit> v2 = gtr_ana->ReAnalyzeV4Y(0, hit, bin_width/2.0+margin1);
      gtr1_hits.insert(gtr1_hits.end(), v2.begin(), v2.end());
   }
   SetG4ClosestTag(gtr1_hits, 2, 1.0);
   for(int i=0; i<n_div; i++){
      double bin_width = size2/(double)n_div;
      double bin_center = (i+0.5)*bin_width-size2*0.5;
      TVector3 lpos(0.0, bin_center, 0.0);
      TVector3 gmom = geom->GTR(m2, 1)->GetGPos(lpos);
      TVector3 lmom = geom->GTR(m2, 1)->GetLMom(gmom);
      E16ANA_G4Hit hit;
      hit.SetXYZ(lpos);
      hit.SetP(lmom);
      hit.SetID(0,0,m2,0);
      gtr2_hits_div[i] = gtr_ana->ReAnalyzeV4Y(1, hit, bin_width*(margin+0.5));
      SetG4ClosestTag(gtr2_hits_div[i], 2, 0.5);
   }
   for(int i=0; i<n_div; i++){
      double bin_width = size3/(double)n_div;
      double bin_center = (i+0.5)*bin_width-size3*0.5;
      // g4 cut (Layer-3)
      double res = bin_center - gtr_g4hit[m3][2].Y();
      //if(fabs(res) > bin_width*(0.5)) continue;
      if(fabs(res) > bin_width*(margin+0.5)) continue;

      TVector3 lpos(0.0, bin_center, 0.0);
      TVector3 gmom = geom->GTR(m3, 2)->GetGPos(lpos);
      TVector3 lmom = geom->GTR(m3, 2)->GetLMom(gmom);
      E16ANA_G4Hit hit;
      hit.SetXYZ(lpos);
      hit.SetP(lmom);
      hit.SetID(0,0,m3,0);
      gtr3_hits_div[i] = gtr_ana->ReAnalyzeV4Y(2, hit, bin_width*(margin+0.5));

      // g4 cut (Layer-3)
      //std::vector<E16ANA_GTRAnalyzedStripHit>::iterator it = gtr3_hits_div[i].begin();
      //while(it != gtr3_hits_div[i].end()){
      //   double res = it->Tdc2Hit() - gtr_g4hit[it->ModuleID()][2].Y();
      //   if(fabs(res) > 10.0){
      //      it = gtr3_hits_div[i].erase(it);
      //   }else{
      //      it++;
      //   }
      //}

      SetG4ClosestTag(gtr3_hits_div[i], 2, 0.5);
   }

   for(int i=0; i<(int)gtr1_hits.size(); i++){
      E16ANA_GTRAnalyzedStripHit &hit1 = gtr1_hits[i];
      int div = floor((hit1.CogHit()+size1*0.5)/(size1/(double)n_div));
      for(int j=0; j<(int)gtr2_hits_div[div].size(); j++){
         E16ANA_GTRAnalyzedStripHit &hit2 = gtr2_hits_div[div][j];
         for(int k=0; k<(int)gtr3_hits_div[div].size(); k++){
            E16ANA_GTRAnalyzedStripHit &hit3 = gtr3_hits_div[div][k];
            E16ANA_SingleTrackCandidate_Nakai single(geom, 0, 0.0);
            single.AddGTRHitYTdc2(hit1);
            single.AddGTRHitYTdc2(hit2);
            single.AddGTRHitYTdc2(hit3);
            double chisq = single.LinearFitY();
            //double chisq = single.LinearFitY(true, hbd_res->AllAnalyzedHits());
            //if(chisq < 20.0){
            if(chisq < 200.0){
               cand.push_back(single);
               n_candidates++;
            }
         }
      }
   }
   std::cout << "E16ANA_TrackFinding_Nakai::GetSingleTrackCandidatesV4Y : # of candidates = " << n_candidates << std::endl;
}


void E16ANA_TrackFinding_Nakai::CheckHBDMatch(std::vector<std::vector<E16ANA_SingleTrackCandidate_Nakai> > &cand, int xy, double match_distance){
   E16ANA_MultiTrack *fitter = new E16ANA_MultiTrack(bfield_map,geom,1);
   std::vector<E16ANA_G4Hit> &hbd_hits = hbd_res->AllAnalyzedHits();

   //std::cout << "Size of hbd_hits = " << hbd_hits.size() << std::endl;
   //for(int i=0; i<(int)hbd_hits.size(); i++){
   //   std::cout << "    Module ID = " << hbd_hits[i].ModuleID()
   //      << ", PID = " << hbd_hits[i].PID()
   //      << ", TrackID = " << hbd_hits[i].TrackID()
   //      << std::endl;
   //}

   for(int i=0; i<(int)cand.size(); i++){ // module loop
      std::vector<E16ANA_SingleTrackCandidate_Nakai>::iterator it = cand[i].begin();
      while(it != cand[i].end()){ // candidate loop
         bool hbd_matched = false;
         for(int k=0; k<(int)hbd_hits.size(); k++){ // check all HBD hits
            if(it->HBDMatch(fitter, hbd_hits[k], xy, match_distance)){
               hbd_matched = true;
            }
         }
         if(!hbd_matched){
            it = cand[i].erase(it);
         }else{
            it++;
         }
      } // candidate loop end
   } // module loop end
   delete fitter;
}

bool E16ANA_TrackFinding_Nakai::IsHBDMatched(E16ANA_SingleTrackCandidate_Nakai &cand, int xy, double match_distance){
   E16ANA_MultiTrack *fitter = new E16ANA_MultiTrack(bfield_map,geom,1);
   std::vector<E16ANA_G4Hit> &hbd_hits = hbd_res->AllAnalyzedHits();
   bool flag = false;
   for(int k=0; k<(int)hbd_hits.size(); k++){ // check all HBD hits
      flag = flag || cand.HBDMatch(fitter, hbd_hits[k], xy, match_distance);
   }
   delete fitter;
   return flag;
}

TVector3 E16ANA_TrackFinding_Nakai::GetMostPlausibleProjectionHBD(E16ANA_SingleTrackCandidate_Nakai &cand){
   E16ANA_MultiTrack *fitter = new E16ANA_MultiTrack(bfield_map,geom,1);
   std::vector<E16ANA_G4Hit> &hbd_hits = hbd_res->AllAnalyzedHits();
   double distance_min = 1000.0;
   TVector3 plausible_proj(-10000.0, -10000.0, 0.0);
   for(int k=0; k<(int)hbd_hits.size(); k++){ // check all HBD hits
      TVector3 proj = cand.ProjectionHBD(fitter, hbd_hits[k]);
      double distance = sqrt(proj.X()*proj.X()+proj.Y()*proj.Y()); // distance from detector center
      if(distance < distance_min){
         distance_min = distance;
         plausible_proj = proj;
      }
   }
   delete fitter;
   return plausible_proj;
}

void E16ANA_TrackFinding_Nakai::SetG4HitSSD(){
   int noh = g4event->SSD().Noh();
   for(int i=0; i<noh; i++){
      E16ANA_G4Hit &h = g4event->SSD().Hit(i);
      //if(h.ModuleID() == 16) continue;
      if(h.PID() == 11 || h.PID() == -11){
         int mm = h.ModuleID();
         ssd_g4hit[mm] = h;
      }
   }
}

void E16ANA_TrackFinding_Nakai::SetG4Hit(int mm){
   {
      int layer = 0;
      int noh = g4event->GTR1().Noh();
      for(int i=0; i<noh; i++){
         E16ANA_G4Hit &h = g4event->GTR1().Hit(i);
         if(h.ModuleID() == 16) continue;
         if((h.PID() == 11 || h.PID() == -11) && h.ModuleID() == mm){
            gtr_g4hit[mm][layer] = h;
         }
      }
   }
   {
      int layer = 1;
      int noh = g4event->GTR2().Noh();
      for(int i=0; i<noh; i++){
         E16ANA_G4Hit &h = g4event->GTR2().Hit(i);
         if(h.ModuleID() == 16) continue;
         if((h.PID() == 11 || h.PID() == -11) && h.ModuleID() == mm){
            gtr_g4hit[mm][layer] = h;
         }
      }
   }
   {
      int layer = 2;
      int noh = g4event->GTR3().Noh();
      for(int i=0; i<noh; i++){
         E16ANA_G4Hit &h = g4event->GTR3().Hit(i);
         if(h.ModuleID() == 16) continue;
         if((h.PID() == 11 || h.PID() == -11) && h.ModuleID() == mm){
            gtr_g4hit[mm][layer] = h;
         }
      }
   }
}

E16ANA_GTRAnalyzedHit* E16ANA_TrackFinding_Nakai::SetG4ClosestTag(std::vector<E16ANA_GTRAnalyzedHit> &hits, int x_or_y, int cog_or_tdc, double g4_near_threshold){
   double (E16ANA_GTRAnalyzedStripHit::*p_hit)();
   if(cog_or_tdc == 0){p_hit = &E16ANA_GTRAnalyzedStripHit::CogHit;}
   else if(cog_or_tdc == 1){p_hit = &E16ANA_GTRAnalyzedStripHit::Tdc1Hit;}
   else if(cog_or_tdc == 2){p_hit = &E16ANA_GTRAnalyzedStripHit::Tdc2Hit;}
   else {return NULL;}
   double res_min = 1000000.0;
   int i_min = -1;
   //const double g4_residual_cut = 1.0;
   for(int i=0; i<(int)hits.size(); i++){
      E16ANA_G4Hit &g4hit = gtr_g4hit[hits[i].ModuleID()][hits[i].LayerID()];
      double residual = 100000.0;
      E16ANA_GTRAnalyzedStripHit *shit;
      if(x_or_y == 0){
         shit = hits[i].XstripHit();
      }else{
         shit = hits[i].YstripHit();
      }
      if(shit->IsX()){
         residual = (shit->*p_hit)() - g4hit.X();
      }else{
         residual = (shit->*p_hit)() - g4hit.Y();
      }
      if(fabs(residual) < res_min){
         res_min = fabs(residual);
         i_min = i;
      }
      if(fabs(residual) < g4_near_threshold){
         shit->SetUserTag(1,1);
      }
   }
   //if(i_min >= 0 && res_min < g4_residual_cut){
   if(i_min >= 0){
      if(x_or_y == 0){
         hits[i_min].XstripHit()->SetUserTag(0,1);
      }else{
         hits[i_min].YstripHit()->SetUserTag(0,1);
      }
   }
   if(i_min >= 0){
      return &hits[i_min];
   }
   return NULL;
}

void E16ANA_TrackFinding_Nakai::SetG4ClosestTag(std::vector<E16ANA_GTRAnalyzedStripHit> &hits, int cog_or_tdc, double g4_near_threshold){
   double (E16ANA_GTRAnalyzedStripHit::*p_hit)();
   if(cog_or_tdc == 0){p_hit = &E16ANA_GTRAnalyzedStripHit::CogHit;}
   else if(cog_or_tdc == 1){p_hit = &E16ANA_GTRAnalyzedStripHit::Tdc1Hit;}
   else if(cog_or_tdc == 2){p_hit = &E16ANA_GTRAnalyzedStripHit::Tdc2Hit;}
   else {return;}
   double res_min = 1000000.0;
   int i_min = -1;
   //const double g4_residual_cut = 1.0;
   for(int i=0; i<(int)hits.size(); i++){
      E16ANA_G4Hit &g4hit = gtr_g4hit[hits[i].ModuleID()][hits[i].LayerID()];
      double residual = 100000.0;
      if(hits[i].IsX()){
         residual = (hits[i].*p_hit)() - g4hit.X();
      }else{
         const double mergin = 1.0; // mm (ya-yb mergin)
         if(g4hit.X() > -mergin && hits[i].IsY()){ // ya
            residual = (hits[i].*p_hit)() - g4hit.Y();
         }
         if(g4hit.X() < mergin && hits[i].IsYb()){ // yb
            residual = (hits[i].*p_hit)() - g4hit.Y();
         }
         if(hits[i].LayerID() >= 1){ // GTR2 or 3
            residual = (hits[i].*p_hit)() - g4hit.Y();
         }
      }
      if(fabs(residual) < res_min){
         res_min = fabs(residual);
         i_min = i;
      }
      if(fabs(residual) < g4_near_threshold){
         hits[i].SetUserTag(1,1);
      }
   }
   //if(i_min >= 0 && res_min < g4_residual_cut){
   if(i_min >= 0){
      hits[i_min].SetUserTag(0,1);
   }
   return;
}

//void E16ANA_TrackFinding_Nakai::SetG4ClosestTagSSD(std::vector<E16ANA_G4Hit> &hits, double g4_near_threshold){
//   double res_min = 1000000.0;
//   int i_min = -1;
//   //const double g4_residual_cut = 1.0;
//   for(int i=0; i<(int)hits.size(); i++){
//      E16ANA_G4Hit &g4hit = ssd_g4hit[hits[i].ModuleID()];
//      double residual = 10000.0;
//      if(g4hit.EventID != E16ANA_G4Hit.ErrorValue){
//         residual = hits[i].X() - g4hit.X();
//      }
//      if(fabs(residual) < res_min){
//         res_min = fabs(residual);
//         i_min = i;
//      }
//      if(fabs(residual) < g4_near_threshold){
//         hits[i].SetUserTag(1,1);
//      }
//   }
//   //if(i_min >= 0 && res_min < g4_residual_cut){
//   if(i_min >= 0){
//      hits[i_min].SetUserTag(0,1);
//   }
//   return;
//}

void E16ANA_TrackFinding_Nakai::CheckG4ClosestLayer2or3(
      std::vector<std::vector<E16ANA_SingleTrackCandidate_Nakai> > &cand,
      std::vector<int> &rank, std::vector<TVector3> &hbd_proj, int true_flag){
   rank.resize(cand.size());
   hbd_proj.resize(cand.size());
   for(int i=0; i<(int)cand.size(); i++){
      rank[i] = -1;
      hbd_proj[i].SetXYZ(-10000.0, -10000.0, 0.0);
      if((int)cand[i].size() == 0) rank[i] = -2;
      for(int j=0; j<(int)cand[i].size(); j++){
         //if(cand[i][j].IsG4Closest(2, true_flag) || cand[i][j].IsG4Closest(3, true_flag)){
         if(cand[i][j].IsG4Closest(true_flag)){
            rank[i] = j;
            hbd_proj[i] = GetMostPlausibleProjectionHBD(cand[i][j]);
            break;
         }
      }
   }
}

//void E16ANA_TrackFinding_Nakai::CheckG4Closest(std::vector<std::vector<E16ANA_SingleTrackCandidate_Nakai> > &cand, std::vector<int> &rank, int true_flag){
void E16ANA_TrackFinding_Nakai::CheckG4Closest(
      std::vector<std::vector<E16ANA_SingleTrackCandidate_Nakai> > &cand,
      std::vector<int> &rank, std::vector<TVector3> &hbd_proj, int true_flag){
   rank.resize(cand.size());
   hbd_proj.resize(cand.size());
   for(int i=0; i<(int)cand.size(); i++){
      rank[i] = -1;
      hbd_proj[i].SetXYZ(-10000.0, -10000.0, 0.0);
      if((int)cand[i].size() == 0) rank[i] = -2;
      for(int j=0; j<(int)cand[i].size(); j++){
         if(cand[i][j].IsG4Closest(true_flag)){
         //if(cand[i][j].IsG4Near(true_flag)){
            rank[i] = j;
            hbd_proj[i] = GetMostPlausibleProjectionHBD(cand[i][j]);
            break;
         }
      }
   }
}

void E16ANA_TrackFinding_Nakai::CheckG4ClosestDouble(
      std::vector<E16ANA_DoubleTrackCandidate_Nakai> &cand,
      std::vector<int> &rank, std::vector<TVector3> &hbd_proj, int true_flag){
   rank.resize(1);
   hbd_proj.resize(2);
   rank[0] = -1;
   hbd_proj[0] = TVector3(-10000.0, -10000.0, 0.0);
   hbd_proj[1] = TVector3(-10000.0, -10000.0, 0.0);
   if(cand.empty()) rank[0] = -2;
   for(int i=0; i<(int)cand.size(); i++){
      if(true
      && cand[i].GetSingleTrackCandidate(0).IsG4Closest(true_flag)
      && cand[i].GetSingleTrackCandidate(1).IsG4Closest(true_flag)
            ){
         rank[0] = i;
         hbd_proj[0] = GetMostPlausibleProjectionHBD(cand[i].GetSingleTrackCandidate(0));
         hbd_proj[1] = GetMostPlausibleProjectionHBD(cand[i].GetSingleTrackCandidate(1));
         break;
      }
   }
}

void E16ANA_TrackFinding_Nakai::PrintNumberOfCandidates(std::vector<std::vector<E16ANA_SingleTrackCandidate_Nakai> > &cand){
   int n = 0;
   for(int i=0; i<(int)cand.size(); i++){
      n += cand[i].size();
   }
   std::cout << "Number of single track candidates = " << n << std::endl;
}

void E16ANA_TrackFinding_Nakai::ReAnalyzeAndReFit(std::vector<E16ANA_SingleTrackCandidate_Nakai> &cand){
   E16ANA_MultiTrack *fitter = new E16ANA_MultiTrack(bfield_map,geom,1);
   for(int i=0; i<(int)cand.size(); i++){
      E16ANA_SingleTrackCandidate_Nakai &single = cand[i];
      for(int l=1; l<4; l++){
         single.ReAnalyzeTdc2(gtr_ana, l);
      }
      single.SingleFit(fitter);
   }
   ChisqSort(cand);
}

