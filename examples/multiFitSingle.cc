//2019-10/25, modified by sako
//2016-11-22, uploaded by nakai

#include <iostream>
#include <fstream>

#include <TFile.h>
#include <TTree.h>
#include <TGraph.h>
#include <TGraph2D.h>
#include <TVector3.h>
#include <TRandom3.h>
#include <TClonesArray.h>

#include "E16ANA_Constant.hh"
#include "E16ANA_ParamManager.hh"
#include "E16ANA_MagneticFieldMap.hh"
//#include "E16ANA_Geometry.hh"
#include "E16ANA_GeometryV2.hh"
#include "E16ANA_G4OutputData.hh"
#include "E16ANA_GTRResponse.hh"
#include "E16ANA_GTRAnalyzer2.hh"
#include "E16ANA_GTRStripAnalyzer.hh"
#include "E16ANA_DoubleTrack.hh"
#include "E16ANA_MultiTrack.hh"
#include "E16ANA_TrackFinding.hh"
#include "E16ANA_SSDResponse.hh"
#include "E16ANA_TrackFinding_Nakai.hh"

// ON or OFF
//#define SINGLE_OVERLAP

// Select only one
#define ANALYZE_V1//def
//#define ANALYZE_V2
//#define ANALYZE_V3
//#define ANALYZE_V4

// Select only one
//#define MULTI_FIT_G4
//#define MULTI_FIT_COG
#define MULTI_FIT_TDC
//#define MULTI_FIT_V1
//#define TRACK_FINDING

using namespace std;

class OutputFile {
public:
   OutputFile(char *fname_out) : n_max_hits(400) {
      file = TFile::Open(fname_out, "recreate");
      tree = new TTree("tree", "tree");
      tree->Branch("g4_event_id", &g4_event_id);
      tree->Branch("good_flag", &good_flag);
      tree->Branch("dfit_flag", &dfit_flag);
      tree->Branch("VTXpos", &VTXpos);
      tree->Branch("VTXpos_cp", &VTXpos_cp);
      tree->Branch("VTXpos_fitd", &VTXpos_fitd);
      tree->Branch("chisq_fitd", &chisq_fitd);
      tree->Branch("mass_org", &mass_org);
      tree->Branch("bg_org", &bg_org);
      tree->Branch("mass_fitd", &mass_fitd);
      tree->Branch("bg_fitd", &bg_fitd);
      tree->Branch("nGTRFrame1hit", &nGTRFrame1hit);
      tree->Branch("nGTRFrame2hit", &nGTRFrame2hit);
      tree->Branch("nGTRFrame3hit", &nGTRFrame3hit);
      for(int i=0; i<n_primaries; i++){
         tree->Branch(Form("chisq%d_fits",i), &chisq_fits[i]);
         tree->Branch(Form("VTXpos%d_fits",i), &VTXpos_fits[i]);
         tree->Branch(Form("VTXmom%d",i), &VTXmom[i]);
         tree->Branch(Form("VTXmom%d_quad",i), &VTXmom_quad[i]);
         tree->Branch(Form("VTXmom%d_fits",i), &VTXmom_fits[i]);
         tree->Branch(Form("VTXmom%d_fitd",i), &VTXmom_fitd[i]);
         tree->Branch(Form("GTR1id%d",i), &GTR1id[i]);
         tree->Branch(Form("GTR2id%d",i), &GTR2id[i]);
         tree->Branch(Form("GTR3id%d",i), &GTR3id[i]);
         tree->Branch(Form("SSDid%d",i), &SSDid[i]);
         tree->Branch(Form("GTR1gPos%d_g4",i), &GTR1gPos_g4[i]);
         tree->Branch(Form("GTR2gPos%d_g4",i), &GTR2gPos_g4[i]);
         tree->Branch(Form("GTR3gPos%d_g4",i), &GTR3gPos_g4[i]);
         tree->Branch(Form("GTR1lPos%d_g4",i), &GTR1lPos_g4[i]);
         tree->Branch(Form("GTR2lPos%d_g4",i), &GTR2lPos_g4[i]);
         tree->Branch(Form("GTR3lPos%d_g4",i), &GTR3lPos_g4[i]);
         tree->Branch(Form("GTR1gMom%d_g4",i), &GTR1gMom_g4[i]);
         tree->Branch(Form("GTR2gMom%d_g4",i), &GTR2gMom_g4[i]);
         tree->Branch(Form("GTR3gMom%d_g4",i), &GTR3gMom_g4[i]);
         tree->Branch(Form("GTR1lMom%d_g4",i), &GTR1lMom_g4[i]);
         tree->Branch(Form("GTR2lMom%d_g4",i), &GTR2lMom_g4[i]);
         tree->Branch(Form("GTR3lMom%d_g4",i), &GTR3lMom_g4[i]);
         tree->Branch(Form("GTR1gPos%d_ana",i), &GTR1gPos_ana[i]);
         tree->Branch(Form("GTR2gPos%d_ana",i), &GTR2gPos_ana[i]);
         tree->Branch(Form("GTR3gPos%d_ana",i), &GTR3gPos_ana[i]);
         tree->Branch(Form("GTR1lPos%d_ana",i), &GTR1lPos_ana[i]);
         tree->Branch(Form("GTR2lPos%d_ana",i), &GTR2lPos_ana[i]);
         tree->Branch(Form("GTR3lPos%d_ana",i), &GTR3lPos_ana[i]);
         tree->Branch(Form("GTR1gPos%d_fitd",i), &GTR1gPos_fitd[i]);
         tree->Branch(Form("GTR2gPos%d_fitd",i), &GTR2gPos_fitd[i]);
         tree->Branch(Form("GTR3gPos%d_fitd",i), &GTR3gPos_fitd[i]);
         tree->Branch(Form("GTR1lPos%d_fitd",i), &GTR1lPos_fitd[i]);
         tree->Branch(Form("GTR2lPos%d_fitd",i), &GTR2lPos_fitd[i]);
         tree->Branch(Form("GTR3lPos%d_fitd",i), &GTR3lPos_fitd[i]);
         tree->Branch(Form("GTR1lMom%d_fitd",i), &GTR1lMom_fitd[i]);
         tree->Branch(Form("GTR2lMom%d_fitd",i), &GTR2lMom_fitd[i]);
         tree->Branch(Form("GTR3lMom%d_fitd",i), &GTR3lMom_fitd[i]);
         tree->Branch(Form("SSDgPos%d_g4",i), &SSDgPos_g4[i]);
         tree->Branch(Form("SSDlPos%d_g4",i), &SSDlPos_g4[i]);
         tree->Branch(Form("SSDgMom%d_g4",i), &SSDgMom_g4[i]);
         tree->Branch(Form("SSDlMom%d_g4",i), &SSDlMom_g4[i]);
         new(&GTR1lPos_multi_ana[i]) TClonesArray("TVector3", n_max_hits);
         new(&GTR2lPos_multi_ana[i]) TClonesArray("TVector3", n_max_hits);
         new(&GTR3lPos_multi_ana[i]) TClonesArray("TVector3", n_max_hits);
         new(&GTR1lPos_multi_fitd[i]) TClonesArray("TVector3", n_max_hits);
         new(&GTR2lPos_multi_fitd[i]) TClonesArray("TVector3", n_max_hits);
         new(&GTR3lPos_multi_fitd[i]) TClonesArray("TVector3", n_max_hits);
         new(&SSDgPos_multi_ana[i]) TClonesArray("TVector3", n_max_hits);
         new(&SSDlPos_multi_ana[i]) TClonesArray("TVector3", n_max_hits);
         new(&SSDlPos_multi_fitd[i]) TClonesArray("TVector3", n_max_hits);
         new(&SSDlPos_multi_fits[i]) TClonesArray("TVector3", n_max_hits);
         new(&GTR1lPos_multi_fits[i]) TClonesArray("TVector3", n_max_hits);
         new(&GTR2lPos_multi_fits[i]) TClonesArray("TVector3", n_max_hits);
         new(&GTR3lPos_multi_fits[i]) TClonesArray("TVector3", n_max_hits);
         for(int k=0; k<3; k++){
            new(&GTR_cluster[k][i]) TClonesArray("TVector3", n_max_hits);
         }
         for(int j=0; j<n_max_hits; j++){
            new(GTR1lPos_multi_ana[i][j]) TVector3(error_value, error_value, error_value);
            new(GTR2lPos_multi_ana[i][j]) TVector3(error_value, error_value, error_value);
            new(GTR3lPos_multi_ana[i][j]) TVector3(error_value, error_value, error_value);
            new(GTR1lPos_multi_fitd[i][j]) TVector3(error_value, error_value, error_value);
            new(GTR2lPos_multi_fitd[i][j]) TVector3(error_value, error_value, error_value);
            new(GTR3lPos_multi_fitd[i][j]) TVector3(error_value, error_value, error_value);
            new(SSDgPos_multi_ana[i][j]) TVector3(error_value, error_value, error_value);
            new(SSDlPos_multi_ana[i][j]) TVector3(error_value, error_value, error_value);
            new(SSDlPos_multi_fitd[i][j]) TVector3(error_value, error_value, error_value);
            new(GTR1lPos_multi_fits[i][j]) TVector3(error_value, error_value, error_value);
            new(GTR2lPos_multi_fits[i][j]) TVector3(error_value, error_value, error_value);
            new(GTR3lPos_multi_fits[i][j]) TVector3(error_value, error_value, error_value);
            new(SSDlPos_multi_fits[i][j]) TVector3(error_value, error_value, error_value);
            for(int k=0; k<3; k++){
               new(GTR_cluster[k][i][j]) TVector3(error_value, error_value, error_value);
            }
         }
         tree->Branch(Form("nGTR1lPos%d_multi_ana",i), &nGTR1lPos_multi_ana[i]);
         tree->Branch(Form("nGTR2lPos%d_multi_ana",i), &nGTR2lPos_multi_ana[i]);
         tree->Branch(Form("nGTR3lPos%d_multi_ana",i), &nGTR3lPos_multi_ana[i]);
         tree->Branch(Form("nGTR1lPos%d_multi_fitd",i), &nGTR1lPos_multi_fitd[i]);
         tree->Branch(Form("nGTR2lPos%d_multi_fitd",i), &nGTR2lPos_multi_fitd[i]);
         tree->Branch(Form("nGTR3lPos%d_multi_fitd",i), &nGTR3lPos_multi_fitd[i]);
         tree->Branch(Form("GTR1lPos%d_multi_ana",i), &GTR1lPos_multi_ana[i]);
         tree->Branch(Form("GTR2lPos%d_multi_ana",i), &GTR2lPos_multi_ana[i]);
         tree->Branch(Form("GTR3lPos%d_multi_ana",i), &GTR3lPos_multi_ana[i]);
         tree->Branch(Form("GTR1lPos%d_multi_fitd",i), &GTR1lPos_multi_fitd[i]);
         tree->Branch(Form("GTR2lPos%d_multi_fitd",i), &GTR2lPos_multi_fitd[i]);
         tree->Branch(Form("GTR3lPos%d_multi_fitd",i), &GTR3lPos_multi_fitd[i]);
         tree->Branch(Form("nSSDgPos%d_multi_ana",i), &nSSDgPos_multi_ana[i]);
         tree->Branch(Form("nSSDlPos%d_multi_ana",i), &nSSDlPos_multi_ana[i]);
         tree->Branch(Form("nSSDlPos%d_multi_fitd",i), &nSSDlPos_multi_fitd[i]);
         tree->Branch(Form("SSDgPos%d_multi_ana",i), &SSDgPos_multi_ana[i]);
         tree->Branch(Form("SSDlPos%d_multi_ana",i), &SSDlPos_multi_ana[i]);
         tree->Branch(Form("SSDlPos%d_multi_fitd",i), &SSDlPos_multi_fitd[i]);
         tree->Branch(Form("nGTR1lPos%d_multi_fits",i), &nGTR1lPos_multi_fits[i]);
         tree->Branch(Form("nGTR2lPos%d_multi_fits",i), &nGTR2lPos_multi_fits[i]);
         tree->Branch(Form("nGTR3lPos%d_multi_fits",i), &nGTR3lPos_multi_fits[i]);
         tree->Branch(Form("nSSDlPos%d_multi_fits",i), &nSSDlPos_multi_fits[i]);
         tree->Branch(Form("GTR1lPos%d_multi_fits",i), &GTR1lPos_multi_fits[i]);
         tree->Branch(Form("GTR2lPos%d_multi_fits",i), &GTR2lPos_multi_fits[i]);
         tree->Branch(Form("GTR3lPos%d_multi_fits",i), &GTR3lPos_multi_fits[i]);
         tree->Branch(Form("SSDlPos%d_multi_fits",i), &SSDlPos_multi_fits[i]);
         for(int k=0; k<3; k++){
            tree->Branch(Form("GTR%d_cluster%d",k+1,i), &GTR_cluster[k][i]);
            tree->Branch(Form("GTR%d_cluster_id%d",k+1,i), &GTR_cluster_id[k][i]);
         }
         tree->Branch("cluster_width_x", cluster_width_x, Form("cluster_width_x[%d]/D",n_primaries*3));
         tree->Branch("cluster_width_y", cluster_width_y, Form("cluster_width_y[%d]/D",n_primaries*3));

         g_track_xz[i] = new TGraph();
         g_track_xy[i] = new TGraph();
         g_track_zy[i] = new TGraph();
         g_track_xyz[i] = new TGraph2D();
         g_strack_xz[i] = new TGraph();
      }
      for(int i=0; i<33; i++){
         for(int j=0; j<3; j++){
            tree->Branch(Form("gtr_strips_%02d%02d"         ,i,j), &gtr_strips         [i][j]);
            tree->Branch(Form("gtr_hit_strips_%02d%02d"     ,i,j), &gtr_hit_strips     [i][j]);
            tree->Branch(Form("gtr_strip_occupancy_%02d%02d",i,j), &gtr_strip_occupancy[i][j]);
         }
      }
   };
   void Clear(){
      module_org.clear();
      g4_event_id = error_value;
      good_flag = error_value;
      dfit_flag = error_value;
      Clear(VTXpos);
      Clear(VTXpos_cp);
      Clear(VTXpos_fitd);
      chisq_fitd = error_value;
      mass_org = error_value;
      bg_org = error_value;
      mass_fitd = error_value;
      bg_fitd = error_value;
      nGTRFrame1hit = error_value;
      nGTRFrame2hit = error_value;
      nGTRFrame3hit = error_value;
      for(int i=0; i<n_primaries; i++){
         chisq_fits[i] = error_value;
         Clear(VTXpos_fits[i]);
         GTR1id[i] = error_value;
         GTR2id[i] = error_value;
         GTR3id[i] = error_value;
         SSDid[i] = error_value;
         Clear(VTXmom[i]);
         Clear(VTXmom_quad[i]);
         Clear(VTXmom_fits[i]);
         Clear(VTXmom_fitd[i]);
         Clear(GTR1gPos_g4[i]);
         Clear(GTR2gPos_g4[i]);
         Clear(GTR3gPos_g4[i]);
         Clear(GTR1lPos_g4[i]);
         Clear(GTR2lPos_g4[i]);
         Clear(GTR3lPos_g4[i]);
         Clear(GTR1gMom_g4[i]);
         Clear(GTR2gMom_g4[i]);
         Clear(GTR3gMom_g4[i]);
         Clear(GTR1lMom_g4[i]);
         Clear(GTR2lMom_g4[i]);
         Clear(GTR3lMom_g4[i]);
         Clear(SSDgPos_g4[i]);
         Clear(SSDlPos_g4[i]);
         Clear(SSDgMom_g4[i]);
         Clear(SSDlMom_g4[i]);
         Clear(GTR1gPos_ana[i]);
         Clear(GTR2gPos_ana[i]);
         Clear(GTR3gPos_ana[i]);
         Clear(GTR1lPos_ana[i]);
         Clear(GTR2lPos_ana[i]);
         Clear(GTR3lPos_ana[i]);
         Clear(GTR1gPos_fitd[i]);
         Clear(GTR2gPos_fitd[i]);
         Clear(GTR3gPos_fitd[i]);
         Clear(GTR1lPos_fitd[i]);
         Clear(GTR2lPos_fitd[i]);
         Clear(GTR3lPos_fitd[i]);
         Clear(GTR1lMom_fitd[i]);
         Clear(GTR2lMom_fitd[i]);
         Clear(GTR3lMom_fitd[i]);
         for(int j=0; j<n_max_hits; j++){
            Clear((TVector3*)GTR1lPos_multi_ana[i][j]);
            Clear((TVector3*)GTR2lPos_multi_ana[i][j]);
            Clear((TVector3*)GTR3lPos_multi_ana[i][j]);
            Clear((TVector3*)GTR1lPos_multi_fitd[i][j]);
            Clear((TVector3*)GTR2lPos_multi_fitd[i][j]);
            Clear((TVector3*)GTR3lPos_multi_fitd[i][j]);
            Clear((TVector3*)GTR1lPos_multi_fits[i][j]);
            Clear((TVector3*)GTR2lPos_multi_fits[i][j]);
            Clear((TVector3*)GTR3lPos_multi_fits[i][j]);
            Clear((TVector3*)SSDgPos_multi_ana[i][j]);
            Clear((TVector3*)SSDlPos_multi_ana[i][j]);
            Clear((TVector3*)SSDlPos_multi_fitd[i][j]);
            Clear((TVector3*)SSDlPos_multi_fits[i][j]);
            Clear((TVector3*)GTR_cluster[0][i][j]);
            Clear((TVector3*)GTR_cluster[1][i][j]);
            Clear((TVector3*)GTR_cluster[2][i][j]);
         }
         GTR_cluster_id[0][i].clear();
         GTR_cluster_id[1][i].clear();
         GTR_cluster_id[2][i].clear();
         nGTR1lPos_multi_ana[i] = 0;
         nGTR2lPos_multi_ana[i] = 0;
         nGTR3lPos_multi_ana[i] = 0;
         nGTR1lPos_multi_fitd[i] = 0;
         nGTR2lPos_multi_fitd[i] = 0;
         nGTR3lPos_multi_fitd[i] = 0;

         nGTR1lPos_multi_fits[i] = 0;
         nGTR2lPos_multi_fits[i] = 0;
         nGTR3lPos_multi_fits[i] = 0;

         nSSDgPos_multi_ana[i] = 0;
         nSSDlPos_multi_ana[i] = 0;
         nSSDlPos_multi_fitd[i] = 0;
         nSSDlPos_multi_fits[i] = 0;
         for(int j=0; j<3; j++){
            cluster_width_x[i*3+j] = error_value;
            cluster_width_y[i*3+j] = error_value;
         }
      }
      for(int i=0; i<33; i++){
         for(int j=0; j<3; j++){
            gtr_strips         [i][j].clear();
            gtr_hit_strips     [i][j].clear();
            gtr_strip_occupancy[i][j].clear();
         }
      }
   };
   void TreeFill(){
      if(good_flag == 1) tree->Fill();
   };
   void SaveAndClose(){
      file->Write();
      for(int i=0; i<n_primaries; i++){
         g_track_xz[i]->Write();
         g_track_xy[i]->Write();
         g_track_zy[i]->Write();
         g_track_xyz[i]->Write(); // TGraph2D
         g_strack_xz[i]->Write();
      }
      file->Close();
   };

   void SetG4EventInfo(E16ANA_G4Event &g4event){
      g4_event_id = g4event.G4EventID();
      nGTRFrame1hit = g4event.GTRFrame1().Noh();
      nGTRFrame2hit = g4event.GTRFrame2().Noh();
      nGTRFrame3hit = g4event.GTRFrame3().Noh();
   };
   void SetPrimaryTracks(E16ANA_G4Track &t_pos, E16ANA_G4Track &t_ele){
      VTXpos = t_pos.TVertex(); // mm -> mm
      VTXmom[0] = t_pos.PTV();
      VTXmom[1] = t_ele.PTV();
   };
   void SetInvMassOrg(double mass, double bg){
      mass_org = mass;
      bg_org = bg;
   };
   void SetQuadraticFitData(const TVector3 &p0, const TVector3 &p1){
      VTXmom_quad[0] = p0;
      VTXmom_quad[1] = p1;
   };
   void SetSingleFitData(const TVector3 v[2], const TVector3 &vcp, const TVector3 p[2], const double chisq[2]){
      for(int i=0; i<n_primaries; i++){
         VTXpos_fits[i] = v[i];
         VTXmom_fits[i] = p[i];
         chisq_fits[i] = chisq[i];
      }
      VTXpos_cp = vcp;
   };
   void SetDoubleFitData(
         double chi2, double mass, double bg,
         const TVector3 &vtx, const TVector3 &p_pos, const TVector3 &p_ele
         ){
      chisq_fitd = chi2;
      mass_fitd = mass;
      bg_fitd = bg;
      VTXpos_fitd = vtx; // mm -> mm
      VTXmom_fitd[0] = p_pos;
      VTXmom_fitd[1] = p_ele;
   };
   void SetGTRPosG4(const TVector3 &gpos, const TVector3 &lpos, const TVector3 &gmom, const TVector3 &lmom, int module, int id){ // mm -> mm
      if(id%3 == 0){
         GTR1gPos_g4[id/3] = gpos;
         GTR1lPos_g4[id/3] = lpos;
         GTR1gMom_g4[id/3] = gmom;
         GTR1lMom_g4[id/3] = lmom;
         GTR1id[id/3] = module;
      }else if(id%3 == 1){
         GTR2gPos_g4[id/3] = gpos;
         GTR2lPos_g4[id/3] = lpos;
         GTR2gMom_g4[id/3] = gmom;
         GTR2lMom_g4[id/3] = lmom;
         GTR2id[id/3] = module;
      }else if(id%3 == 2){
         GTR3gPos_g4[id/3] = gpos;
         GTR3lPos_g4[id/3] = lpos;
         GTR3gMom_g4[id/3] = gmom;
         GTR3lMom_g4[id/3] = lmom;
         GTR3id[id/3] = module;
      }
   };
   void SetSSDPosG4(const TVector3 &gpos, const TVector3 &lpos, const TVector3 &gmom, const TVector3 &lmom, int module, int track_id){ // mm -> mm
      SSDgPos_g4[track_id] = gpos;
      SSDlPos_g4[track_id] = lpos;
      SSDgMom_g4[track_id] = gmom;
      SSDlMom_g4[track_id] = lmom;
      SSDid[track_id] = module;
   }
   void SetGTRPosAna(const TVector3 &gpos, const TVector3 &lpos, double width_x, double width_y, int id){ // mm -> mm
      if(id%3 == 0){
         GTR1gPos_ana[id/3] = gpos;
         GTR1lPos_ana[id/3] = lpos;
      }else if(id%3 == 1){
         GTR2gPos_ana[id/3] = gpos;
         GTR2lPos_ana[id/3] = lpos;
      }else if(id%3 == 2){
         GTR3gPos_ana[id/3] = gpos;
         GTR3lPos_ana[id/3] = lpos;
      }
      cluster_width_x[id] = width_x;
      cluster_width_y[id] = width_y;
   };
   void SetGTRPosDoubleFit(const TVector3 &gpos, const TVector3 &lpos, const TVector3 &lmom, int id){ // mm -> mm
      if(id%3 == 0){
         GTR1gPos_fitd[id/3] = gpos;
         GTR1lPos_fitd[id/3] = lpos;
         GTR1lMom_fitd[id/3] = lmom;
      }else if(id%3 == 1){
         GTR2gPos_fitd[id/3] = gpos;
         GTR2lPos_fitd[id/3] = lpos;
         GTR2lMom_fitd[id/3] = lmom;
      }else if(id%3 == 2){
         GTR3gPos_fitd[id/3] = gpos;
         GTR3lPos_fitd[id/3] = lpos;
         GTR3lMom_fitd[id/3] = lmom;
      }
   };
   void SetGTRPosMultiAna(const std::vector<TVector3> &lpos, int id){
      int n_filled_hits = min((int)lpos.size(), n_max_hits);
      if(id%3 == 0){
         nGTR1lPos_multi_ana[id/3] = lpos.size();
         for(int i=0; i<n_filled_hits; i++){
            SetToObject(&lpos[i], GTR1lPos_multi_ana[id/3][i]);
         }
      }else if(id%3 == 1){
         nGTR2lPos_multi_ana[id/3] = lpos.size();
         for(int i=0; i<n_filled_hits; i++){
            SetToObject(&lpos[i], GTR2lPos_multi_ana[id/3][i]);
         }
      }else if(id%3 == 2){
         nGTR3lPos_multi_ana[id/3] = lpos.size();
         for(int i=0; i<n_filled_hits; i++){
            SetToObject(&lpos[i], GTR3lPos_multi_ana[id/3][i]);
         }
      }
   };
   void SetGTRPosMultiSingleFit(const std::vector<TVector3> &lpos, int id){
      int n_filled_hits = min((int)lpos.size(), n_max_hits);
      if(id%3 == 0){
         nGTR1lPos_multi_fits[id/3] = lpos.size();
         for(int i=0; i<n_filled_hits; i++){
            SetToObject(&lpos[i], GTR1lPos_multi_fits[id/3][i]);
         }
      }else if(id%3 == 1){
         nGTR2lPos_multi_fits[id/3] = lpos.size();
         for(int i=0; i<n_filled_hits; i++){
            SetToObject(&lpos[i], GTR2lPos_multi_fits[id/3][i]);
         }
      }else if(id%3 == 2){
         nGTR3lPos_multi_fits[id/3] = lpos.size();
         for(int i=0; i<n_filled_hits; i++){
            SetToObject(&lpos[i], GTR3lPos_multi_fits[id/3][i]);
         }
      }
   };
   void SetGTRPosMultiDoubleFit(const std::vector<TVector3> &lpos, int id){
      int n_filled_hits = min((int)lpos.size(), n_max_hits);
      if(id%3 == 0){
         nGTR1lPos_multi_fitd[id/3] = lpos.size();
         for(int i=0; i<n_filled_hits; i++){
            SetToObject(&lpos[i], GTR1lPos_multi_fitd[id/3][i]);
         }
      }else if(id%3 == 1){
         nGTR2lPos_multi_fitd[id/3] = lpos.size();
         for(int i=0; i<n_filled_hits; i++){
            SetToObject(&lpos[i], GTR2lPos_multi_fitd[id/3][i]);
         }
      }else if(id%3 == 2){
         nGTR3lPos_multi_fitd[id/3] = lpos.size();
         for(int i=0; i<n_filled_hits; i++){
            SetToObject(&lpos[i], GTR3lPos_multi_fitd[id/3][i]);
         }
      }
   };
   void SetSSDPosMultiAna(const std::vector<TVector3> &lpos, int id){
      int n_filled_hits = min((int)lpos.size(), n_max_hits);
      nSSDlPos_multi_ana[id] = lpos.size();
      for(int i=0; i<n_filled_hits; i++){
         SetToObject(&lpos[i], SSDlPos_multi_ana[id][i]);
      }
   };
   void SetSSDPosBkgAna(const std::vector<TVector3> &lpos, int id){
      int n_filled_hits = min((int)lpos.size(), n_max_hits);
      nSSDgPos_multi_ana[id] = lpos.size();
      for(int i=0; i<n_filled_hits; i++){
         SetToObject(&lpos[i], SSDgPos_multi_ana[id][i]);
      }
   };
   void SetSSDPosMultiSingleFit(const std::vector<TVector3> &lpos, int id){
      int n_filled_hits = min((int)lpos.size(), n_max_hits);
      nSSDlPos_multi_fits[id] = lpos.size();
      for(int i=0; i<n_filled_hits; i++){
         SetToObject(&lpos[i], SSDlPos_multi_fits[id][i]);
      }
   };

   void SetSSDPosMultiDoubleFit(const std::vector<TVector3> &lpos, int id){
      int n_filled_hits = min((int)lpos.size(), n_max_hits);
      nSSDlPos_multi_fitd[id] = lpos.size();
      for(int i=0; i<n_filled_hits; i++){
         SetToObject(&lpos[i], SSDlPos_multi_fitd[id][i]);
      }
   };
   void SetToObject(const TVector3 *v, TObject *obj){
      TVector3 *temp_ptr = (TVector3*)obj;
      //temp_ptr->SetXYZ(v->X()*10.0, v->Y()*10.0, v->Z()*10.0); // cm -> mm
      temp_ptr->SetXYZ(v->X(), v->Y(), v->Z()); // mm -> mm
   };
   void SetGood(int a){good_flag = a;};
   void SetDfitFlag(int a){dfit_flag = a;};

   void SetGTRCluster(E16ANA_GTRAnalyzedHit* hit, int id){
      int track = id/3;
      int layer = id%3;
      int cluster = (GTR_cluster_id[layer][track].empty()) ? 0 : GTR_cluster_id[layer][track].back()+1;
      E16ANA_GTRAnalyzedStripHit *strip = hit->XstripHit();
      for(int i=0; i<strip->NumHit(); i++){
         int j = GTR_cluster_id[layer][track].size();
         GTR_cluster_id[layer][track].push_back(cluster);
         TVector3 lpos(strip->StripPos(i), -10000.0, strip->StripPosZ(i));
         SetToObject(&lpos, GTR_cluster[layer][track][j]);
      }
   };

   void SetSingleTrackSteps(int track_id, const std::vector<TVector3> &track_steps){
      g_strack_xz[track_id]->Set(0);
      for(int i=0; i<(int)track_steps.size(); i++){
         g_strack_xz[track_id]->SetPoint(i,track_steps[i].X(),track_steps[i].Z());
      }
   };
   void SetTrackSteps(int track_id, const std::vector<TVector3> &track_steps){
      g_track_xz[track_id]->Set(0);
      g_track_xy[track_id]->Set(0);
      g_track_zy[track_id]->Set(0);
      g_track_xyz[track_id]->Set(0);
      for(int i=0; i<(int)track_steps.size(); i++){
         g_track_xz[track_id]->SetPoint(i,track_steps[i].X(),track_steps[i].Z());
         g_track_xy[track_id]->SetPoint(i,track_steps[i].X(),track_steps[i].Y());
         g_track_zy[track_id]->SetPoint(i,track_steps[i].Z(),track_steps[i].Y());
         g_track_xyz[track_id]->SetPoint(i,track_steps[i].X(),track_steps[i].Y(),track_steps[i].Z());
      }
   };
   void SetGTRStripOccupancy(int i, int j, E16ANA_GTRStripResponse *s){
      gtr_strips         [i][j].push_back(s->GetNumStrips());
      gtr_hit_strips     [i][j].push_back(s->GetNumHitStrips());
      gtr_strip_occupancy[i][j].push_back(s->GetOccupancy());
   };

private:
   TFile *file;
   TTree *tree;

   enum {n_primaries = 2, error_value = -10000};

   TGraph *g_track_xz[n_primaries];
   TGraph *g_track_xy[n_primaries];
   TGraph *g_track_zy[n_primaries];
   TGraph2D *g_track_xyz[n_primaries];
   TGraph *g_strack_xz[n_primaries];

   int g4_event_id;
   int good_flag;
   int dfit_flag;
   TVector3 VTXpos;
   TVector3 VTXpos_cp;
   TVector3 VTXpos_fits[n_primaries];
   TVector3 VTXpos_fitd;
   double chisq_fits[n_primaries];
   double chisq_fitd;
   double mass_org;
   double bg_org;
   double mass_fitd;
   double bg_fitd;
   int nGTRFrame1hit;
   int nGTRFrame2hit;
   int nGTRFrame3hit;
   int GTR1id[n_primaries];
   int GTR2id[n_primaries];
   int GTR3id[n_primaries];
   int SSDid[n_primaries];
   TVector3 VTXmom[n_primaries];
   TVector3 VTXmom_quad[n_primaries];
   TVector3 VTXmom_fits[n_primaries];
   TVector3 VTXmom_fitd[n_primaries];
   TVector3 GTR1gPos_g4[n_primaries];
   TVector3 GTR2gPos_g4[n_primaries];
   TVector3 GTR3gPos_g4[n_primaries];
   TVector3 GTR1lPos_g4[n_primaries];
   TVector3 GTR2lPos_g4[n_primaries];
   TVector3 GTR3lPos_g4[n_primaries];
   TVector3 GTR1gMom_g4[n_primaries];
   TVector3 GTR2gMom_g4[n_primaries];
   TVector3 GTR3gMom_g4[n_primaries];
   TVector3 GTR1lMom_g4[n_primaries];
   TVector3 GTR2lMom_g4[n_primaries];
   TVector3 GTR3lMom_g4[n_primaries];
   TVector3 SSDgPos_g4[n_primaries];
   TVector3 SSDlPos_g4[n_primaries];
   TVector3 SSDgMom_g4[n_primaries];
   TVector3 SSDlMom_g4[n_primaries];
   TVector3 GTR1gPos_ana[n_primaries];
   TVector3 GTR2gPos_ana[n_primaries];
   TVector3 GTR3gPos_ana[n_primaries];
   TVector3 GTR1lPos_ana[n_primaries];
   TVector3 GTR2lPos_ana[n_primaries];
   TVector3 GTR3lPos_ana[n_primaries];
   TVector3 GTR1gPos_fitd[n_primaries];
   TVector3 GTR2gPos_fitd[n_primaries];
   TVector3 GTR3gPos_fitd[n_primaries];
   TVector3 GTR1lPos_fitd[n_primaries];
   TVector3 GTR2lPos_fitd[n_primaries];
   TVector3 GTR3lPos_fitd[n_primaries];
   TVector3 GTR1lMom_fitd[n_primaries];
   TVector3 GTR2lMom_fitd[n_primaries];
   TVector3 GTR3lMom_fitd[n_primaries];
   double cluster_width_x[n_primaries*3];
   double cluster_width_y[n_primaries*3];

   const int n_max_hits;
   int nGTR1lPos_multi_ana[n_primaries];
   int nGTR2lPos_multi_ana[n_primaries];
   int nGTR3lPos_multi_ana[n_primaries];
   int nGTR1lPos_multi_fitd[n_primaries];
   int nGTR2lPos_multi_fitd[n_primaries];
   int nGTR3lPos_multi_fitd[n_primaries];

   int nGTR1lPos_multi_fits[n_primaries];
   int nGTR2lPos_multi_fits[n_primaries];
   int nGTR3lPos_multi_fits[n_primaries];

   TClonesArray GTR1lPos_multi_ana[n_primaries];
   TClonesArray GTR2lPos_multi_ana[n_primaries];
   TClonesArray GTR3lPos_multi_ana[n_primaries];
   TClonesArray GTR1lPos_multi_fitd[n_primaries];
   TClonesArray GTR2lPos_multi_fitd[n_primaries];
   TClonesArray GTR3lPos_multi_fitd[n_primaries];
   TClonesArray GTR1lPos_multi_fits[n_primaries];
   TClonesArray GTR2lPos_multi_fits[n_primaries];
   TClonesArray GTR3lPos_multi_fits[n_primaries];

   TClonesArray GTR_cluster[3][n_primaries];
   std::vector<int> GTR_cluster_id[3][n_primaries];

   int nSSDgPos_multi_ana[n_primaries];
   int nSSDlPos_multi_ana[n_primaries];
   int nSSDlPos_multi_fitd[n_primaries];
   int nSSDlPos_multi_fits[n_primaries];
   TClonesArray SSDgPos_multi_ana[n_primaries];
   TClonesArray SSDlPos_multi_ana[n_primaries];
   TClonesArray SSDlPos_multi_fitd[n_primaries];
   TClonesArray SSDlPos_multi_fits[n_primaries];

   std::vector<int> module_org;

   std::vector<int> gtr_strips[33][3];
   std::vector<int> gtr_hit_strips[33][3];
   std::vector<double> gtr_strip_occupancy[33][3];

   void Clear(TVector3 &v){
      v.SetXYZ(error_value, error_value, error_value);
   }
   void Clear(TVector3 *v){
      v->SetXYZ(error_value, error_value, error_value);
   }

};

class E16ANA_TrackPairCandidateV05 : public E16ANA_TrackPairCandidateV0 {
private:
   TVector3 gtr_lmom[6];
   TVector3 gtr_gmom[6];
public:
   E16ANA_TrackPairCandidateV05(TVector3 ip1, TVector3 ip2, TVector3 iv) :
      E16ANA_TrackPairCandidateV0(ip1,ip2,iv)
   {};
   void SetHit(int i, E16ANA_G4Hit &hit){
      E16ANA_TrackPairCandidateV0::SetHit(i, hit);
      gtr_lmom[i] = hit.PTV();
      gtr_gmom[i] = hit.GPTV();
   }
   TVector3* Ps(){return gtr_lmom;};
   TVector3* GPs(){return gtr_gmom;};
   void SetP1(const TVector3 &p){P1() = p;};
   void SetP2(const TVector3 &p){P2() = p;};
};

class E16ANA_TrackPairCandidateV1 {
public:
   E16ANA_TrackPairCandidateV1(){};
   ~E16ANA_TrackPairCandidateV1(){};
   void PushBackHit(int _track_id, int _layer_id, int _module_id, const E16ANA_DetectorGeometry *_geom, const TVector3 &_lpos, const TVector3 &_sigma){
      track_id.push_back(_track_id);
      layer_id.push_back(_layer_id);
      module_id.push_back(_module_id);
      geoms.push_back(_geom);
      lpos.push_back(_lpos);
      sigma.push_back(_sigma);
   };
   void SetMultiTrackHit(E16ANA_MultiTrack *multi){
      for(int i=0; i<(int)track_id.size(); i++){
         multi->AddHit(
               track_id[i], layer_id[i],
               geoms[i],
               lpos[i], sigma[i]
               );
      }
   };
   void Clear(){
      track_id.clear();
      layer_id.clear();
      module_id.clear();
      geoms.clear();
      lpos.clear();
      sigma.clear();
   };
   void SetP1(const TVector3 &p1){initp1 = p1;};
   void SetP2(const TVector3 &p2){initp2 = p2;};
   void SetVertex(const TVector3 &vtx){initvtx = vtx;};
   TVector3& P1(){return initp1;};
   TVector3& P2(){return initp2;};
   TVector3& Vertex(){return initvtx;};

private:
   std::vector<int> track_id;
   std::vector<int> layer_id;
   std::vector<int> module_id;
   std::vector<const E16ANA_DetectorGeometry*> geoms;
   std::vector<TVector3> lpos;
   std::vector<TVector3> sigma;
   TVector3 initp1;
   TVector3 initp2;
   TVector3 initvtx;
};

class E16ANA_GTRResponseAnalysis {
public:
   E16ANA_GTRResponseAnalysis(
         OutputFile *_file,
         E16ANA_GTRResponseManager *_gtr_res,
         E16ANA_GTRAnalyzerManager *_gtr_ana,
         E16ANA_MagneticFieldMap *_bfiled_map,
         E16ANA_GeometryV2 *_geom
         );
   ~E16ANA_GTRResponseAnalysis();
   bool EventAnalysis(E16ANA_G4Event &_g4event);
  bool SingleEventAnalysis(E16ANA_G4Event &_g4event);//sako

private:
   enum{n_ssd_modules = 33};
   OutputFile *file;
   E16ANA_GTRResponseManager *gtr_res;
   E16ANA_GTRAnalyzerManager *gtr_ana;
   E16ANA_SSDResponse *ssd_res[n_ssd_modules];
   E16ANA_HBDResponseManager *hbd_res;
   E16ANA_MagneticFieldMap *bfield_map;
   E16ANA_GeometryV2 *geom;
   E16ANA_G4Event *g4event;

   void G4EventAnalysis();
   void SingleFit(E16ANA_TrackPairCandidateV05 &pair);
   void SingleFit(E16ANA_SingleTrackCandidate_Nakai &single, int track_id = -1);
   void MultiFitV1(E16ANA_TrackPairCandidateV05 &pair){
      E16ANA_TrackPairCandidateV1 pair_v1;
      pair_v1.SetP1(pair.P1());
      pair_v1.SetP2(pair.P2());
      pair_v1.SetVertex(pair.Vertex()); // mm -> mm
      for(int i=0; i<6; i++){
         int layer = i%3;
         int module = pair.ModuleIDs()[i];
         //pair_v1.PushBackHit(i/3, layer+1, module, geom->GTR(module,layer), TVector3(-10000.0,-10000.0,-2.0), TVector3(0.0,0.0,0.0)); // dummy hit
         pair_v1.PushBackHit(i/3, layer+1, module, geom->GTR(module,layer), pair.Xs()[i], TVector3(0.080,0.500,0.0));
         //pair_v1.PushBackHit(i/3, layer+1, module, geom->GTR(module,layer), TVector3(-10000.0,-10000.0,+2.0), TVector3(0.0,0.0,0.0)); // dummy hit
      }
      AddSSDHit(pair_v1);
      MultiFitV1(pair_v1);
   };
   void MultiFitV1(E16ANA_TrackPairCandidateV1 &pair);
   bool PairSelectionG4(E16ANA_TrackPairCandidateV05 &pair);
   bool PairSelectionResponse(
         E16ANA_TrackPairCandidateV05 &pair_g4,
         E16ANA_TrackPairCandidateV05 &pair,
         bool cog_method = true);
   bool PairSelectionCog(
         E16ANA_TrackPairCandidateV05 &pair_g4,
         E16ANA_TrackPairCandidateV05 &pair
         ){
      return PairSelectionResponse(pair_g4, pair, true);
   };
   bool PairSelectionTdc(
         E16ANA_TrackPairCandidateV05 &pair_g4,
         E16ANA_TrackPairCandidateV05 &pair
         ){
      return PairSelectionResponse(pair_g4, pair, false);
   };
   bool PairSelectionV1(E16ANA_TrackPairCandidateV05 &pair_g4, E16ANA_TrackPairCandidateV1 &pair);
   bool MakePairV1(E16ANA_TrackPairCandidateV05 &pair_g4, E16ANA_TrackPairCandidateV1 &pair);
   void ReAnalysis(E16ANA_G4Hit &hit_org, int layer);
   void KawamaSmear(E16ANA_TrackPairCandidateV05 &pair);
   int AddSSDHit(E16ANA_TrackPairCandidateV1 &pair);

   double InvMass(double m1, double m2, const TVector3 &p1, const TVector3 &p2);
   double BetaGamma(double m, const TVector3 &p1, const TVector3 &p2);

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

   void GetSingleTrackCandidates(std::vector<E16ANA_SingleTrackCandidate_Nakai> &single_track_candidates);
  void GetG4SingleTrackCandidates(std::vector<E16ANA_SingleTrackCandidate_Nakai> &_candidates); //sako

};

E16ANA_GTRResponseAnalysis::E16ANA_GTRResponseAnalysis(
         OutputFile *_file,
         E16ANA_GTRResponseManager *_gtr_res,
         E16ANA_GTRAnalyzerManager *_gtr_ana,
         E16ANA_MagneticFieldMap *_bfield_map,
         E16ANA_GeometryV2 *_geom
      ){
   file = _file;
   gtr_res = _gtr_res;
   gtr_ana = _gtr_ana;
   bfield_map = _bfield_map;
   geom = _geom;
   for(int i=0; i<n_ssd_modules; i++){
      ssd_res[i] = new E16ANA_SSDResponse();
   }
   hbd_res = new E16ANA_HBDResponseManager();
}

E16ANA_GTRResponseAnalysis::~E16ANA_GTRResponseAnalysis(){
   for(int i=0; i<n_ssd_modules; i++){
      delete ssd_res[i];
   }
}

bool E16ANA_GTRResponseAnalysis::EventAnalysis(E16ANA_G4Event &_g4event){
   bool double_fitted = false;
   int good_flag = 0;
   g4event = &_g4event;
   file->SetG4EventInfo(*g4event);

   int nprimary = g4event->PrimaryTracks().Noh();
   std::vector<int> phiee;
   for (int i=0; i< nprimary ; i++){
      if(g4event->PrimaryTracks(i).EventID() > 10000){
         phiee.push_back(i);
      }
   }

   E16ANA_G4Track *t1 = NULL;
   E16ANA_G4Track *t2 = NULL;
   E16ANA_TrackPairCandidateV05 *pair_g4 = NULL;
   bool only2Track = true;

   if(phiee.size() == 2){
      t1 = &g4event->PrimaryTracks(phiee[0]);
      t2 = &g4event->PrimaryTracks(phiee[1]);
   }else if(nprimary == 2){                           //only phi in 1st file
      t1 = &g4event->PrimaryTracks(0);
      t2 = &g4event->PrimaryTracks(1);
   }
   else{
      E16WARNING("not only2Track : no t1/t2");
      only2Track = false;
   }

   if(only2Track){
      double diffz = t1->Vertex().z()-t2->Vertex().z();
      if(fabs(diffz)>1.0){
         E16INFO("eepair vertex cand diff size2 %5.3f",diffz);
      }
      if(t1->PID() == -11){ // e+
         pair_g4 = new E16ANA_TrackPairCandidateV05(t1->PTV(),t2->PTV(),t1->TVertex());
         file->SetPrimaryTracks(*t1, *t2);
      }else if( t1->PID() == + 11){ // e-
         pair_g4 = new E16ANA_TrackPairCandidateV05(t2->PTV(),t1->PTV(),t2->TVertex());
         file->SetPrimaryTracks(*t2, *t1);
      }else{
         E16WARNING("not e+/e- %d %d", t1->PID(), t2->PID());
      }

      if(pair_g4 != NULL){
         std::cout << "Pair selection G4 start." << std::endl;
         if(PairSelectionG4(*pair_g4)){
            std::cout << "Pair selection G4 is done succesfully." << std::endl;
            const double emass=0.000511;
            double invmassorg = InvMass(emass, emass, pair_g4->P1(), pair_g4->P2());
            double bg_org = BetaGamma(invmassorg, pair_g4->P1(), pair_g4->P2());
            file->SetInvMassOrg(invmassorg, bg_org);

            G4EventAnalysis(); // Response calculation and 1st-Analysis
            std::cout << "Response and 1st-Analysis end." << std::endl;
            for(int k=0; k<6; k++){
               ReAnalysis(*(pair_g4->G4Hits(k)), k%3);
            }
            std::cout << "Re-Analysis end." << std::endl;
            good_flag = 1;
            E16ANA_TrackPairCandidateV05 pair_cog = *pair_g4;//initialize by copy
            E16ANA_TrackPairCandidateV05 pair_tdc = *pair_g4;//initialize by copy
            //bool selection_flag = PairSelectionCog(*pair_g4, pair_cog) && PairSelectionTdc(*pair_g4, pair_tdc);
            bool selection_flag = false;
#ifdef MULTI_FIT_G4
            selection_flag = PairSelectionTdc(*pair_g4, pair_tdc); // for filling analyzed hits
            selection_flag = true;
            if(selection_flag){
               SingleFit(*pair_g4); // Quadratic fit and Single fit for initial values of Double fit
               std::cout << "Double fitting start." << std::endl;
               KawamaSmear(*pair_g4);
               MultiFitV1(*pair_g4);
               double_fitted = true;
            }
#elif defined(MULTI_FIT_COG)
            selection_flag = PairSelectionCog(*pair_g4, pair_cog); // G4 closest selection by COG method
            if(selection_flag){
               SingleFit(pair_cog);
               std::cout << "Double fitting start." << std::endl;
               MultiFitV1(pair_cog);
               double_fitted = true;
            }
#elif defined(MULTI_FIT_TDC)
            selection_flag = PairSelectionTdc(*pair_g4, pair_tdc); // G4 closest selection by TDC method
            if(selection_flag){
               SingleFit(pair_tdc);
               std::cout << "Double fitting start." << std::endl;
               MultiFitV1(pair_tdc);
               double_fitted = true;
            }
#elif defined(MULTI_FIT_V1)
            E16ANA_TrackPairCandidateV1 pair_v1;
            selection_flag = PairSelectionTdc(*pair_g4, pair_tdc); // G4 closest selection by TDC method
            selection_flag = PairSelectionV1(*pair_g4, pair_v1); // All clusters between +- 2.0 mm from G4
            //selection_flag = MakePairV1(*pair_g4, pair_v1);
            if(selection_flag){
               SingleFit(pair_tdc);
               std::cout << "Double fitting start." << std::endl;
               MultiFitV1(pair_v1);
               double_fitted = true;
            }
#elif defined(TRACK_FINDING)
            selection_flag = PairSelectionTdc(*pair_g4, pair_tdc); // G4 closest selection by TDC method
            std::vector<E16ANA_SingleTrackCandidate_Nakai> single_track_candidates;
            GetSingleTrackCandidates(single_track_candidates);
            if(selection_flag){
               SingleFit(pair_tdc);
               std::cout << "Double fitting start." << std::endl;
               MultiFitV1(pair_tdc);
               double_fitted = true;
            }
#endif
         }
      }
      delete pair_g4;
   }
   file->SetGood(good_flag);
   return double_fitted;
}

//sako
bool E16ANA_GTRResponseAnalysis::SingleEventAnalysis(E16ANA_G4Event &_g4event){
   bool double_fitted = false;
   int good_flag = 0;
   g4event = &_g4event;
   file->SetG4EventInfo(*g4event);

   
   G4EventAnalysis(); // Response calculation and 1st-Analysis
   std::cout << "Response and 1st-Analysis end." << std::endl;

   int nprimary = g4event->PrimaryTracks().Noh();
   cout << "nprimary = " << nprimary << endl;
   //   std::vector<int> phiee;
   for (int i=0; i< nprimary ; i++){//i : track id
     
     E16ANA_G4Track *t = &g4event->PrimaryTracks(i);
     //print true momentum
     cout << "true p["<<i<<"] = " << t->PTV().x() <<" " << t->PTV().y() << " " << t->PTV().z() << endl;
   }

   std::vector<E16ANA_SingleTrackCandidate_Nakai> single_track_candidates;
   //   GetSingleTrackCandidates(single_track_candidates);
   GetG4SingleTrackCandidates(single_track_candidates);
   cout << "single_track_candidate.size() = " << single_track_candidates.size() << endl;
     for (unsigned int ic=0;ic<single_track_candidates.size();ic++) {
       E16ANA_SingleTrackCandidate_Nakai &single = single_track_candidates[ic];
       TVector3 mom = single.Momentum();
       TVector3 vtx = single.Vertex();
       TVector3 fmom = single.GetFitMomentum();
       TVector3 fvtx = single.GetFitVertex();
       double chi2 = single.GetChisq();
       int itrack = single.GetTrackID();
       cout << "it = " << itrack << endl;
       cout << "mom[" << ic << "] = " << mom.x() << " " << mom.y() << " " << mom.z() << endl;
       cout << "fmom["<< ic << "] = " << fmom.x() << " " << fmom.y() << " " << fmom.z() << endl;
       cout << "chi2["<< ic << "] = " << chi2 << endl;
     }
   
   
   file->SetGood(good_flag);
   //   return double_fitted;
}

void E16ANA_GTRResponseAnalysis::G4EventAnalysis(){
   E16ANA_G4ModuleHitList g4_module_hits(*g4event);
   std::vector<E16ANA_G4Hit> hits_org[3];
   for(int layer=0; layer<3; layer++){
      for(int i=0; i<33; i++){
         std::vector<E16ANA_G4Hit> &gtr_hits = g4_module_hits.GTRModule(layer,i);
	 cout << "layer, i, GrEventAnalysis.gtr_hits.size() = " << layer << " " << i << " " << gtr_hits.size() << endl;
         for(int j=0; j<(int)gtr_hits.size(); j++){
            //file->SetOrgHit(gtr_hits[j]);
            //std::cout << i << " " << gtr_hits[j].DetectorID() << std::endl;
            hits_org[layer].push_back(gtr_hits[j]);
         }
      }
   }

#ifdef SINGLE_OVERLAP
   E16ANA_EventOverlap::SetGeometry(geom);
   E16ANA_EventOverlap::SetSingleRateDefault();
   g4_module_hits.OverlapGTRSingleHits();
   g4_module_hits.OverlapSSDSingleHits();
#endif

   for(int i=0; i<33; i++){
      for(int j=0; j<3; j++){
         std::vector<E16ANA_G4Hit> &hits = g4_module_hits.GTRModule(j,i);
         //for(int j=0; j<(int)gtr_hits.size(); j++){
         //   //file->SetSingleHit(gtr_hits[j]);
         //   std::cout << i << " " << gtr_hits[j].DetectorID() << std::endl;
         //}
         std::cout << "GTR : ModuleID = " << i
                   << ", LayerID = " << j
                   << ", Multiplicity = " << hits.size() << std::endl;
	 for (int ih=0;ih<(int)hits.size();ih++) {
	   E16ANA_G4Hit &hit = hits[ih];
	   //	   if (hit.PTV()) {
	   //	     cerr << "hits["<<ih<<"].PTV = 0 " << endl;
	   //	   } else {
	     cout << "hits["<<ih<<"].PTV = " << hit.PTV().x() << " " << hit.PTV().y() << " " << hit.PTV().z() << endl;
	     //	   }
	 }
      }
   }
   std::vector<TVector3> gpos;
   for(int i=0; i<n_ssd_modules; i++){
      std::vector<E16ANA_G4Hit> &hits = g4_module_hits.SSDModule(i);
      //for(int j=0; j<(int)gtr_hits.size(); j++){
      //   //file->SetSingleHit(gtr_hits[j]);
      //   std::cout << i << " " << gtr_hits[j].DetectorID() << std::endl;
      //}
      std::cout << "SSD : ModuleID = " << i << ", Multiplicity = " << hits.size() << std::endl;
      ssd_res[i]->SetTracks(hits);
      std::cout << "SSD : ModuleID = " << i << ", Multiplicity (time window) = " << ssd_res[i]->AnalyzedHits().size() << std::endl;
      for(int j=0; j<(int)ssd_res[i]->AnalyzedHits().size(); j++){
         //TVector3 v = geom->SSD(i+12)->GetGPos((ssd_res[i]->AnalyzedHits())[j].XTV());
         TVector3 v = geom->SSD(i)->GetGPos((ssd_res[i]->AnalyzedHits())[j].XTV());
         gpos.push_back(v);
      }
   }
   file->SetSSDPosBkgAna(gpos,0);

#ifdef ANALYZE_V1
   gtr_ana->G4AnalyzeV1(g4_module_hits, gtr_res, geom);
#elif defined(ANALYZE_V2)
   gtr_ana->G4AnalyzeV2(g4_module_hits, gtr_res, geom);
#elif defined(ANALYZE_V3)
   //gtr_ana->G4AnalyzeV3(g4_module_hits, gtr_res, geom);
   gtr_ana->G4AnalyzeV3(g4_module_hits, gtr_res, geom, hits_org[0], hits_org[1], hits_org[2]);
#elif defined(ANALYZE_V4)
   gtr_ana->G4AnalyzeV4(g4_module_hits, gtr_res, geom, hits_org[0], hits_org[1], hits_org[2]);
#endif
   for(int module=0; module<33; module++){
      for(int layer=0; layer<3; layer++){
         std::vector<E16ANA_GTRAnalyzedHit> &hits_x = gtr_ana->AnalyzedHitXChamber(layer, module);
         std::vector<E16ANA_GTRAnalyzedHit> &hits_y = gtr_ana->AnalyzedHitYChamber(layer, module);
         std::cout << "GTR (analyzed) : ModuleID = " << module
                   << ", LayerID = " << layer
                   << ", Multiplicity = " << hits_x.size() << ", " << hits_y.size()
                   << std::endl;
      }
   }
   for(int module=0; module<33; module++){
      E16ANA_GTR100Response *res1 = gtr_res->Response100(module);
      file->SetGTRStripOccupancy(module, 0, res1->GetStripX ());
      file->SetGTRStripOccupancy(module, 0, res1->GetStripYa());
      file->SetGTRStripOccupancy(module, 0, res1->GetStripYb());

      E16ANA_GTR200Response *res2 = gtr_res->Response200(module);
      file->SetGTRStripOccupancy(module, 1, res2->GetStripX ());
      file->SetGTRStripOccupancy(module, 1, res2->GetStripY ());

      E16ANA_GTR300Response *res3 = gtr_res->Response300(module);
      file->SetGTRStripOccupancy(module, 2, res3->GetStripX ());
      file->SetGTRStripOccupancy(module, 2, res3->GetStripY ());
   }
}

//sako g4 true tracks fit
void E16ANA_GTRResponseAnalysis::GetG4SingleTrackCandidates(std::vector<E16ANA_SingleTrackCandidate_Nakai> &_candidates){
   //int gtr_module_id[n_ssd_modules] = {12,13,14,15,16,17,18,19,20};
   int n_candidates = 0;

   E16ANA_G4ModuleHitList g4_module_hits(*g4event);

   //loop over track
   int nprimary = g4event->PrimaryTracks().Noh();
   for (int it=0; it< nprimary ; it++){
     E16ANA_G4Track *t = &g4event->PrimaryTracks(it);
     TVector3 p_true = t->PTV();
     TVector3 vtx_true = t->TVertex();
     int pid_true = t->PID();
     //get gtr hit

     E16ANA_G4Hit *gtr[3]={0};

     for(int layer=0; layer<3; layer++){
       for(int i=0; i<33; i++){
         std::vector<E16ANA_G4Hit> &gtr_hits = g4_module_hits.GTRModule(layer,i);
	 //	 cout << "layer, i, GrEventAnalysis.gtr_hits.size() = " << layer << " " << i << " " << gtr_hits.size() << endl;
         for(int j=0; j<(int)gtr_hits.size(); j++){
	   E16ANA_G4Hit &gtr_hit = gtr_hits[j];
	   if (gtr_hit.TrackID() == it) {
	     //found the GTRhit for track i!
	     if (gtr[layer]==0) {
	       gtr[layer] = &gtr_hit;
	       break;
	     }
	   }
         }
       }
     }
   
     //find ssd hit

     E16ANA_G4Hit *ssd=0;     
     for(int i=0; i<n_ssd_modules; i++){
       std::vector<E16ANA_G4Hit> &hits = g4_module_hits.SSDModule(i);
           for(int j=0; j<(int)hits.size(); j++){
	   E16ANA_G4Hit &ssd_hit = hits[j];
	   if (ssd_hit.TrackID() == it) {
	     //found the SSDhit for track i!
	     if (ssd==0) {
	       ssd = &ssd_hit;
	       break;
	     }
	   }
         }
     }

   
     if (ssd&&gtr[0]&&gtr[1]&&gtr[2]) {
     //     if (ssd&&gtr[2]) {
       double theta_ssd = atan2(ssd->GX(), ssd->GZ());
       //TVector3 rough_mom = ssd.GXTV().Unit();
       TVector3 rough_mom = ssd->GXTV();
       rough_mom.SetY(geom->SSD(ssd->ModuleID())->GetDetectorCenter().Y());
       rough_mom = rough_mom.Unit();

       double theta_gtr3 = atan2(gtr[2]->X(), gtr[2]->Z());
       double charge = -1.0;
       if(theta_ssd < theta_gtr3) charge = 1.0;

       E16ANA_SingleTrackCandidate_Nakai single(geom, 0, charge);
       single.SetMomentum(rough_mom);
       single.SetTrackID(it);
       single.SetVertex(TVector3(0,0,0)); // center of magnet
       //single.SetVertex(g4event->PrimaryTracks(0).TVertex()); // true vertex
       single.AddSSDHit(*ssd);
              single.AddGTRHit(0, *gtr[0]);
              single.AddGTRHit(1, *gtr[1]);
       single.AddGTRHit(2, *gtr[2]);
       _candidates.push_back(single);
       //       SingleFit(single);
   

   //track fit
   //test sako
     
   E16ANA_MultiTrack *single_track = new E16ANA_MultiTrack(bfield_map,geom,1);
   bool vertexfixflag=true, pyfixflag=false;
   double chisq =  single.SingleFit(single_track,vertexfixflag,pyfixflag);
   cout << "chisq = " << chisq << endl;
   cout << "true.p = " <<  p_true.X() << " " << p_true.Y() << " " << p_true.Z() << endl;
   cout << "single.Momentum() = " << single.Momentum().X() << " " << single.Momentum().Y() << " " <<single.Momentum().Z() << endl;
   cout << "single.GetFitMomentum() = " << single.GetFitMomentum().X() << " " << single.GetFitMomentum().Y() << " " <<single.GetFitMomentum().Z() << endl;
   cout << "single.GetTrackID() = " <<single.GetTrackID() << endl;

   /*
   //single_track->SetPrintLevel(2);
   single.SetSingleTrackHit(single_track);
   single_track->PrintHits();
   //double chisq = single_track->Fit(false);
   single_track->Fit(false);
   //   double chisq = single_track->RungeKuttaTracking(0, single.Vertex(), single.Momentum(), single.GetCharge());
   TVector3 mom = single_track->GetFitMomentum(0);
   //std::cout << "3 points fit chisq = " << chisq << std::endl;
   //std::cout << "Fit momentum = (" << mom.X() << ", " << mom.Y() << ", " << mom.Z() << ")" << std::endl;
   //std::cout << "3-PointsFit momentum : "; mom.Print();

   //   if(track_id == 0 || track_id == 1){
   for(int i=0; i<3; i++){
      int layer_id = i;
      int layer_order = layer_id+1;
      std::vector<TVector3> lpos;
      std::vector<TVector3> lmom;
      std::vector<int> module_id;
      //multi_track->GetFitLPos(track_id, layer_order, module_id, lpos);
      single_track->GetFitLPos(0, layer_order, module_id, lpos);
      for(int j=0; j<(int)lpos.size(); j++){
	std::cout << "lpos_fit " << j << " : "; lpos[j].Print();
      }
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
*/
     n_candidates++;
     delete single_track;

     }
   }


/*
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
*/
}


bool E16ANA_GTRResponseAnalysis::PairSelectionG4(//sako comment: set hits in pair assuming only e+ and e- in g4event
   E16ANA_TrackPairCandidateV05 &pair){

   int noh1 = g4event->GTR1().Noh();
   int noh2 = g4event->GTR2().Noh();
   int noh3 = g4event->GTR3().Noh();

   // int noh = noh1 + noh2 + noh3;

   int copied[6] = {0};

   // first track   : positive
   E16ANA_G4Hit h11;

   for(int i=0; i<noh1; i++){
      E16ANA_G4Hit& h = g4event->GTR1().Hit(i);
      //if(h.PID() !=  -11){continue;} // -11 is e+
      if(h.PID() != -11 || h.ModuleID() == 16){continue;} // -11 is e+
      h11 = h;
      pair.SetHit(0,h);
      copied[0] = 1;
      break;
   }

   if(copied[0] == 0){
      for(int i=0; i< noh1 ;i++){
         cerr<<i<<":";g4event->GTR1().Hit(i).Dump("GTR-1-no-e+");
      }
      return false;
   } // no 1st track candidate

   double p1 = h11.PV().mag();

   for(int i=0; i<noh2; i++){
      E16ANA_G4Hit& h = g4event->GTR2().Hit(i);
      if(h.PID() != h11.PID() || h.ModuleID() == 16){continue;}
      if(fabs(h.PV().mag()-p1) > 0.01){continue;}
      pair.SetHit(1,h);
      copied[1] = 1;
      break;
   }
   for(int i=0; i<noh3; i++){
      E16ANA_G4Hit& h = g4event->GTR3().Hit(i);
      //if(h.PID() != h11.PID()){continue;}
      if(h.PID() != h11.PID() || h.ModuleID() == 16){continue;}
      if(fabs(h.PV().mag()-p1) > 0.01){continue;}
      pair.SetHit(2,h); 
      copied[2] = 1;
      break;
   }
   // second track
   E16ANA_G4Hit h21;

   for(int i=0; i< noh1;i++){
      E16ANA_G4Hit& h = g4event->GTR1().Hit(i);
      if(h.PID() != 11 || h.ModuleID() == 16){continue;} // 11 is e-
      h21 = h;
      pair.SetHit(3,h); 
      copied[3] = 1;
      break;
   }

   if(copied[3] == 0){
      for(int i=0; i< noh1 ;i++){
         cerr<<i<<":";g4event->GTR1().Hit(i).Dump("GTR-1-fail");
      }
      return false;
   } //no 2nd track candidate

   double p2 = h21.PV().mag();

   for(int i=0; i<noh2; i++){
      E16ANA_G4Hit& h = g4event->GTR2().Hit(i);
      //if(h.PID() != h21.PID()){continue;}
      if(h.PID() != h21.PID() || h.ModuleID() == 16){continue;}
      if(fabs(h.PV().mag()-p2) > 0.01){continue;}
      pair.SetHit(4,h); 
      copied[4] = 1;
      break;
   }
   for(int i=0; i<noh3; i++){
      E16ANA_G4Hit& h = g4event->GTR3().Hit(i);
      //if(h.PID() != h21.PID()){continue;}
      if(h.PID() != h21.PID() || h.ModuleID() == 16){continue;}
      if(fabs(h.PV().mag()-p2) > 0.01){continue;}
      pair.SetHit(5,h); 
      copied[5] = 1;
      break;
   }

   int noh_ssd = g4event->SSD().Noh();
   for(int i=0; i<noh_ssd; i++){
      E16ANA_G4Hit& h = g4event->SSD().Hit(i);
      if(h.PID() == -11){
         file->SetSSDPosG4(h.GXTV(),h.XTV(),h.GPTV(),h.PTV(),h.ModuleID(),0);
      }else if(h.PID() == 11){
         file->SetSSDPosG4(h.GXTV(),h.XTV(),h.GPTV(),h.PTV(),h.ModuleID(),1);
      }
   }

   //check
   int nullCount = 0;
   //cerr << "pair ids ";
   for(int i=0; i<6; i++){
      //cerr << (pair.ModuleIDs())[i] << " ";
      if(copied[i] == 0){
         E16WARNING("null %d %d", i, nullCount);
         nullCount++;
      }
   }
   //cerr << endl;
   if(nullCount > 0){return false;}
   return true;
}

bool E16ANA_GTRResponseAnalysis::PairSelectionResponse(
      E16ANA_TrackPairCandidateV05 &pair_g4,
      E16ANA_TrackPairCandidateV05 &pair,
      bool cog_method){
   pair.SetVertex(pair_g4.Vertex()); // mm -> mm
   const double residual_threshold = 2.0; // mm
   int n_valid_hits = 0;
   for(int i=0; i<6; i++){
      int layer = i%3;
      E16ANA_G4Hit *g4hit = pair_g4.G4Hits(i);
      file->SetGTRPosG4(pair_g4.GXs()[i], pair_g4.Xs()[i], pair_g4.GPs()[i], pair_g4.Ps()[i], pair_g4.ModuleIDs()[i], i);
      int module = g4hit->DetectorID();
      std::vector<E16ANA_GTRAnalyzedHit> &hits_x = gtr_ana->AnalyzedHitXChamber(layer, module);
      std::vector<E16ANA_GTRAnalyzedHit> &hits_y = gtr_ana->AnalyzedHitYChamber(layer, module);
      double min_res_x = 100000.0;
      double min_res_y = 100000.0;
      int min_jx = -1;
      int min_jy = -1;
      for(int j=0; j<(int)hits_x.size(); j++){
         double res = 1000.0;
         if(cog_method){
            res = g4hit->X()-hits_x[j].XstripHit()->CogHit();
         }else{
            res = g4hit->X()-hits_x[j].XstripHit()->TdcHit();
         }
         if(fabs(min_res_x) > fabs(res)){
            min_res_x = res;
            min_jx = j;
         }
         file->SetGTRCluster(&hits_x[j], i);
      }
      for(int j=0; j<(int)hits_y.size(); j++){
         double res = 1000.0;
         if(cog_method){
            res = g4hit->Y()-hits_y[j].YstripHit()->CogHit();
         }else{
            res = g4hit->Y()-hits_y[j].YstripHit()->TdcHit();
         }
         if(fabs(min_res_y) > fabs(res)){
            min_res_y = res;
            min_jy = j;
         }
      }
      if((min_jx >= 0) && (min_jy >= 0)
         && (fabs(min_res_x)<residual_threshold)
         && (fabs(min_res_y)<residual_threshold)){
         TVector3 lpos;
         if(cog_method){
            lpos.SetXYZ(
                  //hits_x[min_jx].XstripHit()->CogHit()*0.1, // mm -> cm
                  //hits_y[min_jy].YstripHit()->CogHit()*0.1, // mm -> cm
                  hits_x[min_jx].XstripHit()->CogHit(), // mm -> mm
                  hits_y[min_jy].YstripHit()->CogHit(), // mm -> mm
                  0.0);
         }else{
            lpos.SetXYZ(
                  //Hits_x[min_jx].XstripHit()->TdcHit()*0.1, // mm -> cm
                  //Hits_y[min_jy].YstripHit()->TdcHit()*0.1, // mm -> cm
                  hits_x[min_jx].XstripHit()->TdcHit(), // mm -> mm
                  hits_y[min_jy].YstripHit()->TdcHit(), // mm -> mm
                  0.0);
         }
         double cluster_width_x = CalcClusterWidth(hits_x[min_jx].XstripHit());
         double cluster_width_y = CalcClusterWidth(hits_y[min_jy].YstripHit());
         pair.Xs()[i] = lpos;
         TVector3 gpos = geom->GTR(module, layer)->GetGPos(lpos);
         pair.GXs()[i] = gpos;
         n_valid_hits++;
         file->SetGTRPosAna(gpos, lpos, cluster_width_x, cluster_width_y, i);
      }
   }
   if(n_valid_hits < 6){
      std::cout << "PairSelectionResponse failed : # of valid hits = " << n_valid_hits << std::endl;
      return false;
   }
   return true;
}

bool E16ANA_GTRResponseAnalysis::PairSelectionV1(
      E16ANA_TrackPairCandidateV05 &pair_g4,
      E16ANA_TrackPairCandidateV1 &pair){
   const double residual_threshold = 2.0; // mm
   const double z_threshold = 2.0; // mm
   int n_valid_hits = 0;
   pair.SetP1(pair_g4.P1());
   pair.SetP2(pair_g4.P2());
   pair.SetVertex(pair_g4.Vertex()); // mm -> mm
   for(int i=0; i<6; i++){
      int track = i/3;
      int layer = i%3;
      E16ANA_G4Hit *g4hit = pair_g4.G4Hits(i);
      file->SetGTRPosG4(pair_g4.GXs()[i], pair_g4.Xs()[i], pair_g4.GPs()[i], pair_g4.Ps()[i], pair_g4.ModuleIDs()[i], i);
      int module = g4hit->DetectorID();
      std::vector<E16ANA_GTRAnalyzedHit> &hits_x = gtr_ana->AnalyzedHitXChamber(layer, module);
      std::vector<E16ANA_GTRAnalyzedHit> &hits_y = gtr_ana->AnalyzedHitYChamber(layer, module);
      pair.PushBackHit(track, layer+1, module, geom->GTR(module,layer), TVector3(-10000.0,-10000.0,-2.0), TVector3(0.0,0.0,0.0));
      pair.PushBackHit(track, layer+1, module, geom->GTR(module,layer), TVector3(-10000.0,-10000.0,+2.0), TVector3(0.0,0.0,0.0));

      int min_jx = 0;
      int min_jy = 0;
      for(int j=0; j<(int)hits_x.size(); j++){
         double res = 1000.0;
         res = g4hit->X()-hits_x[j].XstripHit()->CogHit(); // mm -> mm
         if(fabs(res) < residual_threshold){
            double x = hits_x[j].XstripHit()->CogHit(); // mm -> mm
            double z = hits_x[j].XstripHit()->MeanZ(); // mm -> mm
            if(fabs(z)<z_threshold){
               pair.PushBackHit(track, layer+1, module, geom->GTR(module,layer), TVector3(x,-10000.0,z), TVector3(160e-3,0.0,0.0));
               min_jx++;
               //break; // 1-hit only
            }
         }
      }
      for(int j=0; j<(int)hits_y.size(); j++){
         double res = 1000.0;
         res = g4hit->Y()-hits_y[j].YstripHit()->CogHit(); // mm -> mm
         if(fabs(res) < residual_threshold){
            double y = hits_y[j].YstripHit()->CogHit(); // mm -> mm
            double z = hits_y[j].YstripHit()->MeanZ(); // mm -> mm
            if(fabs(z)<z_threshold){
               pair.PushBackHit(track, layer+1, module, geom->GTR(module,layer), TVector3(-10000.0,y,z), TVector3(0.0,400e-3,0.0));
               min_jy++;
               //break; // 1-hit only
            }
         }
      }
      if((min_jx > 0) && (min_jy > 0)){
         TVector3 lpos;
            lpos.SetXYZ(
                  hits_x[0].XstripHit()->CogHit(), // mm -> mm
                  hits_y[0].YstripHit()->CogHit(), // mm -> mm
                  0.0);
         TVector3 gpos = geom->GTR(module, layer)->GetGPos(lpos);
         n_valid_hits++;
         //file->SetGTRPosAna(gpos, lpos, i);
         double cluster_width_x = CalcClusterWidth(hits_x[0].XstripHit());
         double cluster_width_y = CalcClusterWidth(hits_y[0].YstripHit());
         file->SetGTRPosAna(gpos, lpos, cluster_width_x, cluster_width_y, i);
      }
   }
   AddSSDHit(pair);
   if(n_valid_hits < 6){
      std::cout << "Response selection fail. n_valid_hits = " << n_valid_hits << std::endl;
      return false;
   }
   return true;
}

int E16ANA_GTRResponseAnalysis::AddSSDHit(E16ANA_TrackPairCandidateV1 &pair){
   int noh_ssd = g4event->SSD().Noh();
   int m_pos = 0;
   int m_ele = 0;
   const double ssd_smear_x = 0.030;
   const double ssd_smear_y = 0.200;
   std::cout << "noh_ssd = " << noh_ssd << std::endl;
   E16ANA_G4Hit h0;
   E16ANA_G4Hit h1;
   for(int i=0; i<noh_ssd; i++){
      E16ANA_G4Hit& h = g4event->SSD().Hit(i);
      int module = h.ModuleID();
      std::cout << "PID = " << h.PID() << std::endl;
      int track;
      if(h.PID() == -11 && h.TrackID() == 2){
         track = 0;
         m_pos++;
         h0 = h;
      //}else if(h.PID() == 11){
      }else if(h.PID() == 11 && h.TrackID() == 1){
         track = 1;
         m_ele++;
         h1 = h;
      }else{
         continue;
      }
      TVector3 lpos = h.XTV();
      lpos.SetX(gRandom->Gaus(lpos.X(), ssd_smear_x));
      //lpos.SetY(gRandom->Gaus(lpos.Y(), ssd_smear_y));
      //pair.PushBackHit(track, 0, module, geom->SSD(module), lpos, TVector3(40e-3,240e-3,0.0));
      lpos.SetY(-10000.0);
      //pair.PushBackHit(track, 0, module, geom->SSD(module), TVector3(-10000.0,-10000.0,-2.0), TVector3(0.0,0.0,0.0)); // dummy hit
      pair.PushBackHit(track, 0, module, geom->SSD(module), lpos, TVector3(40e-3,0.0,0.0));
      //pair.PushBackHit(track, 0, module, geom->SSD(module), lpos, TVector3(0.0,0.0,0.0));
      //pair.PushBackHit(track, 0, module, geom->SSD(module), TVector3(-10000.0,-10000.0,+2.0), TVector3(0.0,0.0,0.0)); // dummy hit
   }
   //pair.PushBackHit(0, 0, h0.ModuleID(), geom->SSD(h0.ModuleID()), h0.XTV(), TVector3(0.0,0.0,0.0));
   //pair.PushBackHit(1, 0, h1.ModuleID(), geom->SSD(h1.ModuleID()), h1.XTV(), TVector3(0.0,0.0,0.0));
   std::cout << "# of SSD hits (e^+, e^-) = " << m_pos << ", " << m_ele << std::endl;
   return m_pos+m_ele;
}

bool E16ANA_GTRResponseAnalysis::MakePairV1(
      E16ANA_TrackPairCandidateV05 &pair_g4,
      E16ANA_TrackPairCandidateV1 &pair){
   double drift_gap = 3.0; // mm
   pair.SetP1(pair_g4.P1());
   pair.SetP2(pair_g4.P2());
   pair.SetVertex(pair_g4.Vertex()); // mm -> mm
   for(int i=0; i<6; i++){
      int track = i/3;
      int layer = i%3;
      E16ANA_G4Hit *g4hit = pair_g4.G4Hits(i);
      file->SetGTRPosG4(pair_g4.GXs()[i], pair_g4.Xs()[i], pair_g4.GPs()[i], pair_g4.Ps()[i], pair_g4.ModuleIDs()[i], i);
      int module = g4hit->DetectorID();
      TVector3 hit_direction = g4hit->PTV();
      hit_direction = hit_direction*(1.0/hit_direction.Z());
      TVector3 hit_position = g4hit->XTV(); // mm -> mm
      int n_clusters = gRandom->Poisson(3.0)+1;
      for(int n=0; n<n_clusters; n++){
         TVector3 lpos = hit_position+gRandom->Uniform(-drift_gap/2.0, drift_gap/2.0)*hit_direction;
         lpos += TVector3(gRandom->Gaus(0,100e-3),gRandom->Gaus(0,300e-3),0);
         pair.PushBackHit(track, layer, module, geom->GTR(module,layer), lpos, TVector3(160e-3,400e-3,0.0));
      }
   }
   return true;
}

void E16ANA_GTRResponseAnalysis::SingleFit(E16ANA_TrackPairCandidateV05 &pair){
   const int n_primaries = 2;
   E16ANA_MultiTrack *single_track = new E16ANA_MultiTrack(bfield_map,geom,1);
   double charge[2] = {1.0, -1.0};
   TVector3 vtx_err(1.5, 1.7, 20e-3);
   TVector3 mom_quad[n_primaries];
   TVector3 mom_fits[n_primaries];
   TVector3 vtx_fits[n_primaries];
   double chisq_fits[2];

   E16ANA_GTRAnalyzedHit analyzed_hit[6];
   for(int i=0; i<6; i++){
      //analyzed_hit[i].Position() = pair.GXs()[i]*0.1; // mm -> cm
      analyzed_hit[i].Position() = pair.GXs()[i]; // mm -> mm
   }

   for(int i=0; i<n_primaries; i++){
      XThreeHitCandidate xcand(
            &analyzed_hit[0+i*3],
            &analyzed_hit[1+i*3],
            &analyzed_hit[2+i*3]);
      mom_quad[i] = xcand.Mom0XZv();
      E16ANA_GTRAnalyzedHit &hit = analyzed_hit[0+i*3];
      double mom_y = mom_quad[i].Mag()
         *hit.Position().Y()
         /sqrt(hit.Position().X()*hit.Position().X()+hit.Position().Z()*hit.Position().Z());
      mom_quad[i].SetY(mom_y);
      TVector3 vtx(0.,0.,0.);
      single_track->Initialize(vtx, vtx_err, &mom_quad[i], &charge[i]);
      //TVector3 sigma(80.0e-3, 500.0e-3, 0.0);
      TVector3 sigma(800.0e-3, 5000.0e-3, 0.0);
      for(int j=0; j<3; j++){
         single_track->AddHit(
               0, j,
               geom->GTR(pair.ModuleIDs()[i*3+j],j),
               pair.Xs()[i*3+j], sigma);
      }
      //single_track->SetPrintLevel(2);
      double chisq = single_track->Fit(false);
      std::cout << "SingleFit chisq = " << chisq << std::endl;
      mom_fits[i] = single_track->GetFitMomentum(0);
      vtx_fits[i] = single_track->GetFitVertex();
      file->SetSingleTrackSteps(i, single_track->GetTrackSteps(0));
      //TVector3 mom;
      if(i==0){
         std::cout << "Original momentum : "; pair.P1().Print();
         //mom = pair.P1();
      }else{
         std::cout << "Original momentum : "; pair.P2().Print();
         //mom = pair.P2();
      }
      std::cout << "QuadraticFit momentum : "; mom_quad[i].Print();
      std::cout << "SingleFit momentum : "; mom_fits[i].Print();
      //double chisq = single_track->RungeKuttaTracking(0, vtx, mom_quad[i], charge[i]);
      //double chisq = single_track->RungeKuttaTracking(0, vtx, mom, charge[i]);
      chisq_fits[i] = chisq;
   }
   TVector3 vtx_cp = (vtx_fits[0]+vtx_fits[1])*0.5;
   file->SetQuadraticFitData(mom_quad[0], mom_quad[1]);
   file->SetSingleFitData(vtx_fits, vtx_cp, mom_fits, chisq_fits);
   // set initial vertex, momenta for common vertex fitting
   pair.SetP1(mom_fits[0]);
   pair.SetP2(mom_fits[1]);
   pair.SetVertex(vtx_cp);
   delete single_track;
}

void E16ANA_GTRResponseAnalysis::SingleFit(E16ANA_SingleTrackCandidate_Nakai &single, int track_id){
   E16ANA_TrackFinding_Nakai finder(g4event, ssd_res, gtr_ana, hbd_res, bfield_map, geom);
   finder.SingleFit(single, track_id);
}

void E16ANA_GTRResponseAnalysis::MultiFitV1(E16ANA_TrackPairCandidateV1 &pair){
   const double emass=0.000511;
   E16ANA_MultiTrack* multi_track = new E16ANA_MultiTrack(bfield_map,geom,2);

   //TVector3 vtx_err(250e-4, 250e-4, 40e-4);
   TVector3 vtx_err(1.5, 1.7, 20e-3);

   //multi_track->SetCharge(0, 1.0);
   //multi_track->SetCharge(1,-1.0);
   //multi_track->SetInitialVertex(crossPoint*1.01, vtx_err);
   //multi_track->SetInitialVertex(TVector3(0,0,0), vtx_err);
   //multi_track->SetInitialMomentum(0, pair.P1()*1.01);
   //multi_track->SetInitialMomentum(1, pair.P2()*1.01);
   double charge[2] = {1.0, -1.0};
   //TVector3 vtx(0,0,0);
   TVector3 vtx = pair.Vertex();
   TVector3 mom[2];
   //mom[0] = pair.P1()*1.01;
   //mom[1] = pair.P2()*1.01;
   mom[0] = pair.P1();
   mom[1] = pair.P2();
   multi_track->Initialize(vtx, vtx_err, mom, charge);
   std::cout << "MultiFitV1 : Initial conditions : CommonVertex = (" << vtx.X() << ", " << vtx.Y() << ", " << vtx.Z() << ")"
      << ", Mom0 = (" << mom[0].X() << ", " << mom[0].Y() << ", " << mom[0].Z() << ")"
      << ", Mom1 = (" << mom[1].X() << ", " << mom[1].Y() << ", " << mom[1].Z() << ")"
      << std::endl;

   pair.SetMultiTrackHit(multi_track);

   //double chi2 = 0.0;
   double chi2 = multi_track->Fit(false);
   //double chi2 = 0.0;
   //multi_track->PreConditioning();
   //chi2 += multi_track->RungeKuttaTracking(0, vtx, pair.P1(), 1.0);
   //chi2 += multi_track->RungeKuttaTracking(1, vtx, pair.P2(),-1.0);

   TVector3 p0 = multi_track->GetFitMomentum(0);
   TVector3 p1 = multi_track->GetFitMomentum(1);
   TVector3 fitvtx = multi_track->GetFitVertex();
   double invmassfit = InvMass(emass, emass, p0, p1);
   double bg_fit = BetaGamma(invmassfit, p0, p1);
   file->SetDoubleFitData(chi2, invmassfit, bg_fit, fitvtx, p0, p1);
   std::cout << "CommonVertexFit momentum0 : "; p0.Print();
   std::cout << "CommonVertexFit momentum1 : "; p1.Print();
   std::cout << "chi2 = " << chi2 << ", mass = " << invmassfit << std::endl;
   for(int i=0; i<6; i++){
      int track_id = i/3;
      int layer_id = i%3;
      int layer_order = layer_id+1;
      std::vector<TVector3> lpos;
      std::vector<TVector3> lmom;
      std::vector<int> module_id;
      multi_track->GetFitLPos(track_id, layer_order, module_id, lpos);
      //for(int j=0; j<(int)lpos.size(); j++){
      //   std::cout << "lpos_fit " << j << " : "; lpos[j].Print();
      //}
      //multi_track->GetLPos(track_id, layer_id, module_id, lpos);
      //for(int j=0; j<(int)lpos.size(); j++){
      //   std::cout << "lpos_mes " << j << " : "; lpos[j].Print();
      //}
      multi_track->GetFitLMom(track_id, layer_order, module_id, lmom);
      TVector3 gpos = geom->GetGPos(lpos[0], layer_id, module_id[0]);
      std::cout << "layer = " << layer_id << ", module = " << module_id[0] << std::endl;
      file->SetGTRPosDoubleFit(gpos, lpos[0], lmom[0], i);
      file->SetGTRPosMultiDoubleFit(lpos, i);
      lpos.clear();
      module_id.clear();
      multi_track->GetLPos(track_id, layer_order, module_id, lpos);
      file->SetGTRPosMultiAna(lpos, i);
   }
   for(int i=0; i<2; i++){ // SSD
      int track_id = i;
      std::vector<TVector3> lpos;
      std::vector<TVector3> lmom;
      std::vector<int> module_id;
      multi_track->GetFitLPos(track_id, 0, module_id, lpos);
      multi_track->GetFitLMom(track_id, 0, module_id, lmom);
      file->SetSSDPosMultiDoubleFit(lpos, i);
      lpos.clear();
      module_id.clear();
      multi_track->GetLPos(track_id, 0, module_id, lpos);
      file->SetSSDPosMultiAna(lpos, i);
   }
   file->SetTrackSteps(0, multi_track->GetTrackSteps(0));
   file->SetTrackSteps(1, multi_track->GetTrackSteps(1));
   delete multi_track;
}

void E16ANA_GTRResponseAnalysis::ReAnalysis(E16ANA_G4Hit &hit_org, int layer){
   int module = hit_org.DetectorID();
   std::vector<E16ANA_GTRAnalyzedHit> &hits_x = gtr_ana->AnalyzedHitXChamber(layer, module);
   for(int j=0; j<(int)hits_x.size(); j++){
      E16ANA_GTRAnalyzedStripHit &hit_ana = *(hits_x[j].XstripHit());
      double res = 10000.0;
      if(hit_org.DetectorID() == hit_ana.ModuleID()){
         res = hit_org.X()-hit_ana.CogHit(); // mm -> mm
      }
      //std::cout << "ReAnalysis : ModuleID = " << hit_ana.ModuleID()
      //   << ", LayerID = " << hit_ana.LayerID() << ", HitID = " << hit_ana.ID() << std::endl;
      if(fabs(res) < 2.0){
         hit_ana.SetTanTheta(hit_org.PTV().X()/hit_org.PTV().Z());
#ifdef ANALYZE_V1
         gtr_ana->ChamberAnalyzer(hit_ana.LayerID(), hit_ana.ModuleID())->AnalyzeV1X(hit_ana.ID());
#elif defined(ANALYZE_V2)
         gtr_ana->ChamberAnalyzer(hit_ana.LayerID(), hit_ana.ModuleID())->AnalyzeV2X(hit_ana.ID());
#elif defined(ANALYZE_V3)
         gtr_ana->ChamberAnalyzer(hit_ana.LayerID(), hit_ana.ModuleID())->AnalyzeV3X(hit_ana.ID());
#endif
      }
   }
   std::vector<E16ANA_GTRAnalyzedHit> &hits_y = gtr_ana->AnalyzedHitYChamber(layer, module);
   for(int j=0; j<(int)hits_y.size(); j++){
      E16ANA_GTRAnalyzedStripHit &hit_ana = *(hits_y[j].YstripHit());
      double res = 10000.0;
      if(hit_org.DetectorID() == hit_ana.ModuleID()){
         res = hit_org.Y()-hit_ana.CogHit(); // mm -> mm
      }
      //std::cout << "ReAnalysis : ModuleID = " << hit_ana.ModuleID()
      //   << ", LayerID = " << hit_ana.LayerID()
      //   << ", HitID = " << hit_ana.ID()
      //   << ", Type = " << hit_ana.Type()
      //   << std::endl;
      if(fabs(res) < 2.0){
         hit_ana.SetTanTheta(hit_org.PTV().Y()/hit_org.PTV().Z());
#ifdef ANALYZE_V1
         gtr_ana->ChamberAnalyzer(hit_ana.LayerID(), hit_ana.ModuleID())->AnalyzeV1Y(hit_ana.ID(), hit_ana.Type());
#elif defined(ANALYZE_V2)
         gtr_ana->ChamberAnalyzer(hit_ana.LayerID(), hit_ana.ModuleID())->AnalyzeV2Y(hit_ana.ID(), hit_ana.Type());
#elif defined(ANALYZE_V3)
         gtr_ana->ChamberAnalyzer(hit_ana.LayerID(), hit_ana.ModuleID())->AnalyzeV3Y(hit_ana.ID(), hit_ana.Type());
#endif
      }
   }
   //double min_res_x = 1000000.0;
   //int min_j = -1;
   //for(int j=0; j<(int)strip_hits.size(); j++){
   //   E16ANA_GTRAnalyzedStripHit &hit_ana = *(strip_hits[j]);
   //   double res = 10000.0;
   //   if(hit_org.DetectorID() == hit_ana.ModuleID()){
   //      res = hit_org.X()*10.0-hit_ana.TdcHit();
   //   }
   //   if(fabs(min_res_x) > fabs(res)){
   //      min_res_x = res;
   //      min_j = j;
   //   }
   //}
   //if(min_j>=0){
   //   file->SetClosestAnaHit(*(strip_hits[min_j]));
   //}else{
   //   E16ANA_GTRAnalyzedStripHit hit;
   //   file->SetClosestAnaHit(hit);
   //}
}

void E16ANA_GTRResponseAnalysis::KawamaSmear(E16ANA_TrackPairCandidateV05 &pair){
   const double smear_x00 = 0.060;
   const double smear_x15 = 0.100;
   const double smear_y = 0.300;
   for(int i=0; i<6; i++){
      E16ANA_G4Hit *hit = pair.G4Hits(i);
      double angle = atan2(hit->PTV().X(), hit->PTV().Z());
      angle = fabs(angle)/TMath::Pi()*180.0;
      double smear_x = smear_x00+(smear_x15-smear_x00)*angle/15.0;
      //double smear_x = smear_x00;
      TVector3 l = pair.Xs()[i];
      pair.Xs()[i].SetXYZ(gRandom->Gaus(l.X(), smear_x),
                          gRandom->Gaus(l.Y(), smear_y),
                          l.Z());
   }
}

double E16ANA_GTRResponseAnalysis::InvMass(double m1, double m2, const TVector3 &p1, const TVector3 &p2){
    double E1 = sqrt ( m1*m1 + p1 * p1 );
    double E2 = sqrt ( m2*m2 + p2 * p2 );
    double mm = (E1+E2)*(E1+E2) - (p1+p2)*(p1+p2);
    double m = sqrt(mm);
    return m;
}

double E16ANA_GTRResponseAnalysis::BetaGamma(double m, const TVector3 &p1, const TVector3 &p2){
   TVector3 p = p1+p2;
   return p.Mag()/m;
}

void E16ANA_GTRResponseAnalysis::GetSingleTrackCandidates(std::vector<E16ANA_SingleTrackCandidate_Nakai> &single_track_candidates){
   E16ANA_TrackFinding_Nakai finder(g4event, ssd_res, gtr_ana, hbd_res, bfield_map, geom);
   finder.GetSingleTrackCandidates(single_track_candidates);
}

int main_double(int argc, char **argv){
   char str[100], fname_cfg[100], fname_in[100], fname_out[100];
   char* fname_field;
   char* fname_geom;
   //int ipFlag=0, nEv=0;
   int n_start = 0, n_end = 0;
   int rndmSeed=0;
   //clock_t start, end;
   E16ANA_ParamManager *paramMgr;
   ifstream ifs;
   if (argc==2){
      ifs.open(argv[1]);
      //paramMgr= new E16ANA_ParamManager("./E16CalibFiles-local.cfg");
      while (ifs.getline(str,sizeof(str))){
         if(sscanf(str,"E16 Config file : %s", fname_cfg)==1){
            cout <<"E16 Config file: "<<fname_cfg<<endl;
         }
         else if(sscanf(str,"Input Data: %s", fname_in)==1){
            cout <<"Input data file: "<<fname_in<<endl;
         }
         else if(sscanf(str,"Output Data: %s", fname_out)==1){
            cout <<"Output data file: "<<fname_out<<endl;
         }
         else if(sscanf(str,"Start event id: %d", &n_start)==1){
            cout <<"To be analyzed from: "<<n_start<<endl;
         }
         else if(sscanf(str,"End event id: %d", &n_end)==1){
            cout <<"To be analyzed until: "<<n_end<<endl;
         }
         else if(sscanf(str,"Set Random Seed : %d", 
                  &rndmSeed)==1){
         }
      }
   }
   else{
      cout << "Usage: ./E16ANA filename"<<endl;
      return 0;
   }

   //TApplication theApp("app", &argc, argv);
   //gROOT->ProcessLine(".x ~/.rootlogon.C");

   paramMgr= new E16ANA_ParamManager(fname_cfg);
   fname_field = paramMgr->GetMapFileName();
   fname_geom = paramMgr->GetGeomFileName();
   cout <<"Magnetic field map: "<<fname_field<<endl;
   cout <<"Geometry data file: "<<fname_geom<<endl;

   cout <<"Initializing ..."<<endl;

   E16ANA_MagneticFieldMap *fmf = new E16ANA_MagneticFieldMap3D(fname_field);
   fmf->Initialize_binary();

   E16ANA_GeometryV2* geom = new E16ANA_GeometryV2(fname_geom);

   //TRandom3 *rand = new TRandom3(rndmSeed);
   delete gRandom;
   gRandom = new TRandom3(rndmSeed);

   E16ANA_G4OutputData g4data;
   int flag_g4_open = g4data.OpenReadFile(fname_in);
   if(flag_g4_open != g4data.OK){
      cout <<"NO input FILE!!!"<<endl;
      return 1;
   }

   E16ANA_GTRResponseManager gtr_res(paramMgr);
   E16ANA_GTRAnalyzerManager gtr_ana(paramMgr);

   //E16ANA_EventOverlap::SetSingleRate(5,0,0,0);

   OutputFile *file = new OutputFile(fname_out);
   E16ANA_GTRResponseAnalysis *res_ana = new E16ANA_GTRResponseAnalysis(file, &gtr_res, &gtr_ana, fmf, geom);

   for(int n=0; n<=n_end; n++){
      //if(n%1==0)cout<<"-------- Event number "<<n<<"/"<<n_end<<" ----------"<<endl; 
      cout<<"-------- Event number "<<n<<"/"<<n_end<<" ----------"<<endl; 
      file->Clear();
      int multi = 1;
      double time_interval[] = {0.0};
      int jam_interaction = 0;
      int flag_g4_read = g4data.ReadOverlapEvent(multi, time_interval, jam_interaction);
      if(flag_g4_read != multi){
         E16FATAL("file end at event %d",n);
         break;
      }
      if(n<n_start){
         continue;
      }
      int dfit_flag = 0;
      if(res_ana->EventAnalysis(g4data.OverlapEvent())){
         dfit_flag = 1;
      }
      file->SetDfitFlag(dfit_flag);
      file->TreeFill();
      cout<<"-------- Event number "<<n<<"/"<<n_end<<" ----------"<<endl<<endl;
   }
   file->SaveAndClose();
   return 0;
}


//new sako for single track analysis
int main(int argc, char **argv){
   char str[100], fname_cfg[100], fname_in[100], fname_out[100];
   char* fname_field;
   char* fname_geom;
   int n_start = 0, n_end = 0;
   int rndmSeed=0;
   E16ANA_ParamManager *paramMgr;
   ifstream ifs;
   if (argc==2){
      ifs.open(argv[1]);
      //paramMgr= new E16ANA_ParamManager("./E16CalibFiles-local.cfg");
      while (ifs.getline(str,sizeof(str))){
         if(sscanf(str,"E16 Config file : %s", fname_cfg)==1){
            cout <<"E16 Config file: "<<fname_cfg<<endl;
         }
         else if(sscanf(str,"Input Data: %s", fname_in)==1){
            cout <<"Input data file: "<<fname_in<<endl;
         }
         else if(sscanf(str,"Output Data: %s", fname_out)==1){
            cout <<"Output data file: "<<fname_out<<endl;
         }
         else if(sscanf(str,"Start event id: %d", &n_start)==1){
            cout <<"To be analyzed from: "<<n_start<<endl;
         }
         else if(sscanf(str,"End event id: %d", &n_end)==1){
            cout <<"To be analyzed until: "<<n_end<<endl;
         }
         else if(sscanf(str,"Set Random Seed : %d", 
                  &rndmSeed)==1){
         }
      }
   }
   else{
      cout << "Usage: ./E16ANA filename"<<endl;
      return 0;
   }

   //TApplication theApp("app", &argc, argv);
   //gROOT->ProcessLine(".x ~/.rootlogon.C");

   paramMgr= new E16ANA_ParamManager(fname_cfg);
   fname_field = paramMgr->GetMapFileName();
   fname_geom = paramMgr->GetGeomFileName();
   cout <<"Magnetic field map: "<<fname_field<<endl;
   cout <<"Geometry data file: "<<fname_geom<<endl;

   cout <<"Initializing ..."<<endl;

   E16ANA_MagneticFieldMap *fmf = new E16ANA_MagneticFieldMap3D(fname_field);
   fmf->Initialize_binary();

   E16ANA_GeometryV2* geom = new E16ANA_GeometryV2(fname_geom);

   //TRandom3 *rand = new TRandom3(rndmSeed);
   delete gRandom;
   gRandom = new TRandom3(rndmSeed);

   E16ANA_G4OutputData g4data;
   int flag_g4_open = g4data.OpenReadFile(fname_in);
   if(flag_g4_open != g4data.OK){
      cout <<"NO input FILE!!!"<<endl;
      return 1;
   }

   E16ANA_GTRResponseManager gtr_res(paramMgr);
   E16ANA_GTRAnalyzerManager gtr_ana(paramMgr);

   //E16ANA_EventOverlap::SetSingleRate(5,0,0,0);

   OutputFile *file = new OutputFile(fname_out);
   E16ANA_GTRResponseAnalysis *res_ana = new E16ANA_GTRResponseAnalysis(file, &gtr_res, &gtr_ana, fmf, geom);

   for(int n=0; n<=n_end; n++){
      //if(n%1==0)cout<<"-------- Event number "<<n<<"/"<<n_end<<" ----------"<<endl; 
      cout<<"-------- Event number "<<n<<"/"<<n_end<<" ----------"<<endl; 
      file->Clear();
      int multi = 1;
      double time_interval[] = {0.0};
      int jam_interaction = 0;
      //int flag_g4_read = g4data.ReadOverlapEvent(multi, time_interval, jam_interaction);
      int flag_g4_read = g4data.ReadAnEvent();// does not work all hit = 0
      if(flag_g4_read != multi){
         E16FATAL("file end at event %d",n);
         break;
      }
      if(n<n_start){
         continue;
      }
      int dfit_flag = 0;
      // if(res_ana->EventAnalysis(g4data.OverlapEvent())){
      if(res_ana->SingleEventAnalysis(g4data.Event())){
         dfit_flag = 1;
      }
      file->SetDfitFlag(dfit_flag);
      file->TreeFill();
      cout<<"-------- Event number "<<n<<"/"<<n_end<<" ----------"<<endl<<endl;
   }
   file->SaveAndClose();
   return 0;
}

