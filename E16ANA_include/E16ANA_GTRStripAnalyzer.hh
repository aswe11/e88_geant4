//2016-11-22, uploaded by nakai
//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai
//2015-12-12, uploaded by nakai
//2015-11-14, uploaded by nakai
//2015-11-02, uploaded by yokkaich
//2015-11-02, uploaded by komatsu
//2015-10-05, uploaded by yokkaich
//2015-08-20, uploaded by yokkaich
//2015-07-15, uploaded by nakai
//2015-05-29, uploaded by yokkaich
//2015-04-08, uploaded by nakai
#ifndef E16ANA_GTRStripAnalyzer_hh
#define E16ANA_GTRStripAnalyzer_hh 1

#include <iostream>

#include "E16ANA_GTRAnalyzer.hh"
#include "E16ANA_GTRAnalyzer2.hh"
#include "E16ANA_WaveformFitter.hh"
#include "E16ANA_Waveform2dFitter.hh"

class E16ANA_GTRAnalyzedStripHit{
public:
   enum{
      n_sampling = 30,
      is_x  = 0,
      is_y  = 1,
      is_yb = 2,
      n_user_tag = 10,
   };

private:
   int gem_max_strip; // strip id
   double gem_max_value; // peak ADC value
   int gem_num_hit;
   double gem_cluster_charge; // sum of ADC values in a cluster
   double gem_center_of_gravity; // mm
   double gem_tdc_hit; // mm, it will be updated (TDC1 -> TDC2) when using CalcTdc2
   double gem_tdc1_hit; // mm, TDC method 1
   double gem_tdc2_hit; // mm, TDC method 2
   double gem_hit_pos; // mm
   double tan_incident_angle; // radian
   double timing; // ns
   double mean_z; // mm
   int  layerID; // 0-2
   int  moduleID; //0-32

   int id;//array intex in Analyzer
   int type;//x,y,yb=0,1,2

   int user_tag[n_user_tag];

   //double summed_waveform[n_sampling];
   std::vector<double> strip_charge;
   std::vector<double> strip_timing;
   std::vector<double> strip_pos;
   std::vector<double> strip_posz;

   void Copy(const E16ANA_GTRAnalyzedStripHit &obj){
      this->gem_max_strip          = obj.gem_max_strip         ; // strip id
      this->gem_max_value          = obj.gem_max_value         ; // peak ADC value
      this->gem_num_hit            = obj.gem_num_hit           ;
      this->gem_cluster_charge     = obj.gem_cluster_charge    ; // sum of ADC values in a cluster
      this->gem_center_of_gravity  = obj.gem_center_of_gravity ; // mm
      this->gem_tdc_hit            = obj.gem_tdc_hit           ; // mm, it will be updated (TDC1 -> TDC2) when using CalcTdc2
      this->gem_tdc1_hit           = obj.gem_tdc1_hit          ; // mm, TDC method 1
      this->gem_tdc2_hit           = obj.gem_tdc2_hit          ; // mm, TDC method 2
      this->gem_hit_pos            = obj.gem_hit_pos           ; // mm
      this->tan_incident_angle     = obj.tan_incident_angle    ; // radian
      this->timing                 = obj.timing                ; // ns
      this->mean_z                 = obj.mean_z                ; // mm
      this->layerID                = obj.layerID               ; // 0-2
      this->moduleID               = obj.moduleID              ; // 0-32
      this->id                     = obj.id                    ; // array intex in Analyzer
      this->type                   = obj.type                  ; // x,y,yb=0,1,2
      for(int i=0; i<n_user_tag; i++){
         this->user_tag[i] = obj.user_tag[i];
      }
      this->strip_charge = obj.strip_charge ;
      this->strip_timing = obj.strip_timing ;
      this->strip_pos    = obj.strip_pos    ;
      this->strip_posz   = obj.strip_posz   ;
   };

public:
   E16ANA_GTRAnalyzedStripHit(){SetInvalid();};
   ~E16ANA_GTRAnalyzedStripHit(){};
   E16ANA_GTRAnalyzedStripHit(const E16ANA_GTRAnalyzedStripHit &obj){Copy(obj);};
   E16ANA_GTRAnalyzedStripHit& operator=(const E16ANA_GTRAnalyzedStripHit &rhs){
      Copy(rhs);
      return (*this);
   };

   void CalcTdc2(double tan_theta);

   int ID(){return id;};
   void SetID(int i){id=i;};

   void SetMaxStrip(int _gem_max_strip){gem_max_strip = _gem_max_strip;};
   void SetMaxValue(double _gem_max_value){gem_max_value = _gem_max_value;};
   void SetNumHit(int _gem_num_hit){gem_num_hit = _gem_num_hit;};
   void SetClusterCharge(double _gem_cluster_charge){gem_cluster_charge = _gem_cluster_charge;};
   void SetCogHit(double _gem_center_of_gravity){gem_center_of_gravity = _gem_center_of_gravity;};
   void SetTdcHit(double _gem_tdc_hit){gem_tdc_hit = _gem_tdc_hit;};
   void SetTdc1Hit(double _gem_tdc_hit){gem_tdc1_hit = _gem_tdc_hit;};
   void SetTdc2Hit(double _gem_tdc_hit){gem_tdc2_hit = _gem_tdc_hit;};
   void SetTanTheta(double _tan_incident_angle){tan_incident_angle = _tan_incident_angle;};
   void SetInvalid(){
      gem_max_strip = kInvalidValue;
      gem_max_value = kInvalidValue;
      gem_num_hit = kInvalidValue;
      gem_cluster_charge = kInvalidValue;
      gem_center_of_gravity = kInvalidValue;
      gem_tdc_hit = kInvalidValue;
      gem_tdc1_hit = kInvalidValue;
      gem_tdc2_hit = kInvalidValue;
      gem_hit_pos = kInvalidValue;
      tan_incident_angle = kInvalidValue;
      //x_or_y = kInvalidValue;
      timing = kInvalidValue;
      mean_z = kInvalidValue;
      layerID = kInvalidValue;
      moduleID = kInvalidValue;
      id = kInvalidValue;
      type = kInvalidValue;

      strip_pos.clear();
      strip_posz.clear();
      strip_charge.clear();
      strip_timing.clear();
      for(int i=0; i<n_user_tag; i++){
         //user_tag[i] = kInvalidValue;
         user_tag[i] = 0;
      }
   };
   //void AddWaveForm(double *wf){
   //   for(int i=0; i<n_sampling; i++){
   //      summed_waveform[i] += wf[i];
   //   }
   //};
   //void ClearWaveForm(){
   //   for(int i=0; i<n_sampling; i++){
   //      summed_waveform[i] = 0.0;
   //   }
   //}
  void SetTiming(double t){timing=t;}
  void SetMeanZ(double z){mean_z = z;};
  void SetLayerAndModuleIDandType(int id1, int id2, int itype){
    layerID=id1; moduleID=id2; type=itype;}

   bool IsX() const {return type==is_x;}
   bool IsY() const {return type==is_y;}
   bool IsYb() const {return type==is_yb;}
   bool IsInvalid() const {return type==kInvalidValue;}
   int Type() const {return type;}
  
   void PushBackStrip(double pos, double posz, double charge, double t){
      //std::cout << pos << ", " << posz << ", " << charge << ", " << t << std::endl;
      strip_pos.push_back(pos);
      strip_posz.push_back(posz);
      strip_charge.push_back(charge);
      strip_timing.push_back(t);
   };

   int MaxStripId(){return gem_max_strip;};
   double MaxValue(){return gem_max_value;};
   int NumHit(){return gem_num_hit;};
   double ClusterCharge(){return gem_cluster_charge;};
   double CogHit(){return gem_center_of_gravity;};
   double TdcHit(){return gem_tdc_hit;};
   double Tdc1Hit(){return gem_tdc1_hit;};
   double Tdc2Hit(){return gem_tdc2_hit;};
   double TanTheta(){return tan_incident_angle;};
   double Timing(){return timing;}
   double MeanZ(){return mean_z;};
   int  ModuleID() const {return moduleID;}
   int  LayerID() const {return layerID;}
   double StripPos(int i){return strip_pos[i];};
   double StripPosZ(int i){return strip_posz[i];};
   double StripCharge(int i){return strip_charge[i];};
   double StripTiming(int i){return strip_timing[i];};
   bool IsInside(double z_min, double z_max){
      double min =  1000000.0;
      double max = -1000000.0;
      for(int i=0; i<NumHit(); i++){
         if(StripPosZ(i) < min){
            min = StripPosZ(i);
         }
         if(StripPosZ(i) > max){
            max = StripPosZ(i);
         }
      }
      return (min > z_min) && (max < z_max);
   };

   void SetUserTag(int i, int tag){
      if(i >= n_user_tag) return;
      user_tag[i] = tag;
   };
   int UserTag(int i){
      if(i >= n_user_tag) return kInvalidValue;
      return user_tag[i];
   };

   enum{
      kInvalidValue = -1000000,
   };

   void Print(){
      std::cout << "E16ANA_GTRStripAnalyzer : "
         << "Num hit strips = " << gem_num_hit
         << ", Cluster charge = " << gem_cluster_charge
         << ", Cog hit pos = " << gem_center_of_gravity
         << " [mm], Tdc hit pos = " << gem_tdc_hit << " [mm]" << std::endl;
   };

};

class E16ANA_GTRStripAnalyzer{
   friend void E16ANA_GTRAnalyzer2::SetParameters(std::string filename);
   friend void E16ANA_GTRAnalyzer::SetParameters(std::string filename);
   friend void E16ANA_GTR100Analyzer::SetParameters(std::string filename);

public:
   E16ANA_GTRStripAnalyzer(int _n_strips, int _n_sampling);
   virtual ~E16ANA_GTRStripAnalyzer();
   virtual void Clear();
   virtual void SetFadc(int strip_id, double *waveform);
   virtual void Analyze();

   virtual void Analyze2();
   virtual void Analyze2(int hitid);//only one hit

   virtual void AnalyzeV1();
   virtual void AnalyzeV1(int hitid){Analyze2(hitid);};

   virtual void AnalyzeV2();
   virtual void AnalyzeV2(int hitid);//only one hit

   virtual void AnalyzeV3();
   virtual void AnalyzeV3(double local_x); // analyzed only periphery of local_x
   virtual void AnalyzeV3(int hitid);//only one hit

   virtual void AnalyzeV4(double local_x, double tan_theta);
   virtual std::vector<E16ANA_GTRAnalyzedStripHit> ReAnalyzeV4(double local_x, double tan_theta, double near_cut = 5.0);

   void SetInverse(bool flag){
      if(flag){
         inverted = -1.0;
      }else{
         inverted = +1.0;
      }
   };

   void SetWaveformTemplate(const std::string &file_name);

   std::vector<E16ANA_GTRAnalyzedStripHit>& GetAnalyzedHits(){return gem_analyzed_hits;};

protected:
   // Analysis parameters
   double drift_velocity;
   double drift_gap_center;
   double drift_gap_center_v3;
   double strip_pitch;
   double fadc_clock_period;
   double fadc_t0_correction;
   double gem_tdc_min;
   double gem_tdc_max;
   double gem_tr;
   double gem_threshold;
   double threshold_fraction;
   double position_start;

   double inverted; // If strip order is the opposite direction to the local coordinate, ...

   double **fadc;
   double *fadc_ped;
   double *fadc_peak;
   double *fadc_peak_time;
   double *fadc_tdc;

   int n_strips;
   int n_sampling;

   std::vector<int> fadc_valid_count;
   std::vector<std::vector<int> > clustered_strip_id;
   std::vector<E16ANA_GTRAnalyzedStripHit> gem_analyzed_hits;

   E16ANA_WaveformFitter *wf1d_fitter;
   E16ANA_Waveform2dFitter *wf2d_fitter;

   struct fit_params_t {
      int strip_id;
      std::vector<double> times;
      std::vector<double> peaks;
      fit_params_t(int _id, int size){
         strip_id = _id;
         times.reserve(size);
         peaks.reserve(size);
      };
   };

   struct cluster_param_t {
      double pos;
      double time;
      double peak;
      cluster_param_t(double _pos, double _time, double _peak){
         pos = _pos;
         time = _time;
         peak = _peak;
      };
   };

   struct cluster_v4_t {
      double x; // local perpendicular coordinate of cluster position (x-z)
      double z;
      double peak;
      double u; // oblique coordinate of cluster position (u-z)
      cluster_v4_t(double _x, double _z, double _peak, double tan_theta){
         x = _x;
         z = _z;
         peak = _peak;
         u = x - z*tan_theta;
      };
      void Print(){
         std::cout << "(x, z, u) = " << x << ", " << z << ", " << u << ")" << std::endl;
      };
   };
   struct LessClusterV4U {
      bool operator()(const cluster_v4_t &lhs, const cluster_v4_t &rhs){
         return lhs.u < rhs.u;
      };
   };
   struct LessClusterV4Z {
      bool operator()(const cluster_v4_t &lhs, const cluster_v4_t &rhs){
         return lhs.z < rhs.z;
      };
   };
   struct LessClusterV4X {
      bool operator()(const cluster_v4_t &lhs, const cluster_v4_t &rhs){
         return lhs.x < rhs.x;
      };
   };

   double GetPosition(int strip_id){return (strip_id*strip_pitch+position_start)*inverted;};

   void CalcWaveParamsPeak();
   void CalcWaveParamsPeak(int ch, double t_cutoff);
   void CalcWaveParamsFit(std::vector<fit_params_t> &fit_pars_array);
   void CalcWaveParamsFit(int ch, fit_params_t &fit_pars);
   //void CalcClusterParams(
   //      std::vector<double> &v1_pos, // 1D-array
   //      std::vector<double> &v1_time,
   //      std::vector<double> &v1_peak
   //      );
   void CalcClusterParams(std::vector<cluster_param_t> &v1, double analysis_cut_x0, double analysis_cut_x1);
   virtual int HitClusteringV0();
   //virtual int HitClusteringV0(const int min_gap, const double cluster_threshold);
   virtual int HitClusteringV0(const int min_gap, const double cluster_threshold, bool strip_share = false);
   virtual int HitClusteringV1(const int min_gap = 2, const double delta_tdc_threshold = 150.0);
   virtual int HitClusteringV2(const int min_gap = 1, const double delta_tdc_threshold = 30.0);
   virtual int HitClusteringV3(
         std::vector<cluster_param_t> &v1,
         std::vector<std::vector<double> > &v2_x, // 2D-array
         std::vector<std::vector<double> > &v2_z,
         std::vector<std::vector<double> > &v2_peak,
         double cluster_min_distance = 2.0 // mm
         //double analysis_cut_x0, double analysis_cut_x1 // strips between x0 and x1 will be analyzed
         );
   virtual int HitClusteringV4(
         std::vector<cluster_v4_t> &v1,
         std::vector<std::vector<double> > &v2_x, // 2D-array
         std::vector<std::vector<double> > &v2_z,
         std::vector<std::vector<double> > &v2_peak,
         double cluster_cut_v4 = 0.3
         );
   std::vector<std::vector<double> > clustered_x;
   std::vector<std::vector<double> > clustered_z;
   std::vector<std::vector<double> > clustered_peak;

   void CalcCenterOfGravity(const std::vector<int> &strip_ids, E16ANA_GTRAnalyzedStripHit &hit);
   void CalcTdcHit1(const std::vector<int> &strip_ids, E16ANA_GTRAnalyzedStripHit &hit, int hitID );
   void CalcTdcHit2(const std::vector<int> &strip_ids, double tan_theta, E16ANA_GTRAnalyzedStripHit &hit); // fixed method
   void SetArraysForTdcMethods(const std::vector<int> &strip_ids, std::vector<double> &x_array, std::vector<double> &time_array, std::vector<double> &peak_array);
   void CalcTdcHit3(bool fix_flag, const std::vector<double> &x, const std::vector<double> &z, const std::vector<double> &peak, E16ANA_GTRAnalyzedStripHit &hit, int hitID);

};

#endif // E16ANA_GTRStripAnalyzer_hh
