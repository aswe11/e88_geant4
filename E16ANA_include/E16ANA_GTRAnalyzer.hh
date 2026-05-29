//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai
//2015-11-02, uploaded by yokkaich
//2015-11-02, uploaded by komatsu
//2015-08-20, uploaded by yokkaich
//2015-07-15, uploaded by nakai
//2015-05-29, uploaded by yokkaich
//2015-04-08, uploaded by nakai
//2015-03-27, uploaded by yokkaich
//2015-03-20, uploaded by nakai
#ifndef E16ANA_GTRAnalyzer_hh
#define E16ANA_GTRAnalyzer_hh 1

#include <vector>

#include "E16ANA_DeconvolutionManager.hh"
#include "E16ANA_GTROneCluster.hh"
#include "E16ANA_GTRSignal.hh"
//#include "E16ANA_GTRStripAnalyzer.hh"
//
class E16ANA_GTRStripAnalyzer;

// example
// E16ANA_GTRAnalyzer(288,  72,  360, 30); for GTR100
// E16ANA_GTRAnalyzer(576, 144,  720, 30); for GTR200
// E16ANA_GTRAnalyzer(864, 216, 1080, 30); for GTR300
//
// Arguments are num_of_xstrips, num_of_ystrips, num_of_fadcs, and num_of_samplings respectively.
// When using this for ordinary simulation, set num_of_fadcs equal to num_of_x + num_of_y.
// If not, set num_of_fadcs correct value (for example 384) and use SetPinAssign method.

class E16ANA_GTRAnalyzer{
private:
   int n_strip_x;
   int n_strip_y;
   int n_fadc_counts;
   int n_fadc_chs;

   double x_start;
   double y_start;

	//int gem_x[N_XSTRIP][STRIP_FADC_COUNT];
	//double gem_x[N_XSTRIP][STRIP_FADC_COUNT];
   double **gem_x;
	//int gem_y[N_YSTRIP][STRIP_FADC_COUNT];
	//double gem_y[N_YSTRIP][STRIP_FADC_COUNT];
   double **gem_y;
	//int strip_num[STRIP_FADC_CH];// Use to convert fadc_ch to strip_num (shared by X and Y)
   int *strip_num;// Use to convert fadc_ch to strip_num (shared by X and Y)
	//int y_strip_num[32];

  double strip_pitch_x;
  double strip_pitch_y;
  double gem_th_x;
  double gem_th_y;
  double drift_velocity;
  double drift_gap_center;
  double fadc_clock_period;
  double fadc_t0_correction;
  double tdc_lsb;
  double gem_t0_min;
  double gem_t0_max;
  double gem_tr;

  double *gemped_x;
  double *gemped_y;
  double *gemsigma_x;
  double *gemsigma_y;

  double **fadcped;
  double **fadcsigma;

  double *gem_peak_x;
  double *gem_peak_y;
  double *gem_peak_time_x;
  double *gem_peak_time_y;
  double *gem_t0_x;
  double *gem_t0_y;
  double threshold_fraction;

  float **gem_decon_x;
  float **gem_decon_y;
  E16ANA_DeconvolutionManager *decon_o;

  int gem_max_strip_x;
  int gem_max_strip_y;
  double gem_max_value_x;
  double gem_max_value_y;
  int gem_num_hit_x;
  int gem_num_hit_y;
  double gem_cluster_charge_x;
  double gem_cluster_charge_y;
  double gem_cluster_charge_tot;
  double gem_center_of_gravity_x;
  double gem_center_of_gravity_y;

  double track_projection_x;
  double track_projection_y;
  double track_analysis_window;

  int left_strip_x;
  int right_strip_x;
  int left_strip_y;
  int right_strip_y;

  double **gem_fit_peak_x;
  double **gem_fit_time0_x;
  double **gem_fit_trise_x;

  double tan_incident_angle;
  double gem_tdc_hit_x;
  double gem_tdc_hit_y;

  double gem_hit_x;
  double gem_hit_y;
  int analysis_method_x;
  int analysis_method_y;

  std::vector<E16ANA_GTRSignal> signals_array_x;
  std::vector<E16ANA_GTROneCluster*> clusters_array_x;

  int gem_num_signals_x;
  int gem_num_clusters_x;
  double gem_mc_hit_x;

  //int n_valid_counts;
  std::vector<int> fadc_valid_count;

  E16ANA_GTRStripAnalyzer *strip_ana_x;
  E16ANA_GTRStripAnalyzer *strip_ana_y;

  void ClearFadc();

  void CalcWaveParamsPeak();
  void CalcWaveParamsPeak(int ch, double t_cutoff);
  void CalcWaveParamsFit();
  void CalcCenterOfGravity();
  void CalcTdcHit(bool fixed_method);
  void CalcDeconvolution();

  void PutSignals();
  void CalcClustering(); // Charge clustering in a track for MultiCluster method. Don't use this.


public:
  E16ANA_GTRAnalyzer(int _n_strip_x, int _n_strip_y, int _n_fadc_chs, int _n_fadc_counts);
  virtual ~E16ANA_GTRAnalyzer();
  void SetParameters(std::string filename);
  void SetPinAssign(std::string filename);
  void SetPedestal(std::string filename);
  void SetFadcValidCount(std::string filename);
  void SetFadcPedestal(std::string filename);
  void SetFadc(double *_fadc);
  void SetTrackCutWindow(double _track_analysis_window){track_analysis_window= _track_analysis_window;};
  void Analyze();// old version
  void Analyze2();//use CalcTdc1
  void Analyze22X(int hitid);//use CalcTdc2 for a hit
  void Analyze22Y(int hitid);//use CalcTdc2 for a hit

  void Clear();// old version
  void Clear2();// use this

  void SetFadc(int ch, double *_fadc);// old version
  void SetFadc2(int ch, double *_fadc);// use this

  int GetGemNumStripsX(){return n_strip_x;};
  int GetGemNumStripsY(){return n_strip_y;};
  int GetGemNumFadcCounts(){return n_fadc_counts;};

  double *GetGemWaveformXPointer(){return &gem_x[0][0];};
  double *GetGemWaveformYPointer(){return &gem_y[0][0];};

  double *GetGemPeakXPointer(){return gem_peak_x;};
  double *GetGemPeakYPointer(){return gem_peak_y;};
  double *GetGemTdcXPointer(){return gem_t0_x;};
  double *GetGemTdcYPointer(){return gem_t0_y;};
  double *GetGemPeakTdcXPointer(){return gem_peak_time_x;};
  double *GetGemPeakTdcYPointer(){return gem_peak_time_y;};

  double *GetGemCenterOfGravityXPointer(){return &gem_center_of_gravity_x;};
  double *GetGemCenterOfGravityYPointer(){return &gem_center_of_gravity_y;};
  double *GetGemClusterChargeXPointer(){return &gem_cluster_charge_x;};
  double *GetGemClusterChargeYPointer(){return &gem_cluster_charge_y;};

  int *GetGemMaxStripXPointer(){return &gem_max_strip_x;};
  int *GetGemMaxStripYPointer(){return &gem_max_strip_y;};

  double *GetGemMaxValueXPointer(){return &gem_max_value_x;};
  double *GetGemMaxValueYPointer(){return &gem_max_value_y;};

  int *GetGemNumHitsXPointer(){return &gem_num_hit_x;};
  int *GetGemNumHitsYPointer(){return &gem_num_hit_y;};

  float *GetGemDeconvolutedWaveformXPointer(){return &gem_decon_x[0][0];};
  float *GetGemDeconvolutedWaveformYPointer(){return &gem_decon_y[0][0];};

  double *GetGemFitPeakXPointer(){return &gem_fit_peak_x[0][0];};
  double *GetGemFitTime0XPointer(){return &gem_fit_time0_x[0][0];};
  double *GetGemFitTriseXPointer(){return &gem_fit_trise_x[0][0];};

  void SetGemFitPeak(double *_gem_fit_peak_x){memcpy(gem_fit_peak_x, _gem_fit_peak_x, sizeof(gem_fit_peak_x));};
  void SetGemFitTime0(double *_gem_fit_time0_x){memcpy(gem_fit_time0_x, _gem_fit_time0_x, sizeof(gem_fit_time0_x));};
  void SetGemFitTrise(double *_gem_fit_trise_x){memcpy(gem_fit_trise_x, _gem_fit_trise_x, sizeof(gem_fit_trise_x));};

  //void SetTrackProjectionX(double _track_projection_x){track_projection_x= _track_projection_x;};
  //void SetTrackProjectionY(double _track_projection_y){track_projection_y= _track_projection_y;};

  void SetTrackProjectionX(double _track_projection_x);
  void SetTrackProjectionY(double _track_projection_y);

  //void SetFadcT0CorrectionParameter(double _fadc_t0){fadc_t0_correction= _fadc_t0;};
  void SetFadcT0CorrectionParameter(int _fadc_t0){fadc_t0_correction= _fadc_t0*tdc_lsb;};

  void SetTanIncidentAngle(double _tan_incident_angle){tan_incident_angle= _tan_incident_angle;};
  double *GetGemTanIncidentAnglePointer(){return &tan_incident_angle;};
  double *GetGemTdcHitXPointer(){return &gem_tdc_hit_x;};
  double *GetGemTdcHitYPointer(){return &gem_tdc_hit_y;};

  double GetGemHitPositionX(){return gem_hit_x;};
  double GetGemHitPositionY(){return gem_hit_y;};

  int *GetGemNumSignalsXPointer(){return &gem_num_signals_x;};
  int *GetGemNumClustersXPointer(){return &gem_num_clusters_x;};
  double *GetGemMcHitXPointer(){return &gem_mc_hit_x;};

  void SetDriftGapCenter(double _drift_gap_center){drift_gap_center = _drift_gap_center;};
  void SetDriftGapCenterTime(double _drift_gap_center_time){drift_gap_center = _drift_gap_center_time*drift_velocity;};
  void SetDriftVelocity(double _drift_velocity){
    drift_gap_center *= _drift_velocity/drift_velocity;
    drift_velocity = _drift_velocity;
  };
  void SetGogMethodModeX(){analysis_method_x = 0;};
  void SetGogMethodModeY(){analysis_method_y = 0;};
  void SetTimingMethod1ModeX(){analysis_method_x = 1;};
  void SetTimingMethod1ModeY(){analysis_method_y = 1;};
  void SetTimingMethod2ModeX(){analysis_method_x = 2;};
  void SetTimingMethod2ModeY(){analysis_method_y = 2;};

  E16ANA_GTRStripAnalyzer* GetStripX(){return strip_ana_x;};
  E16ANA_GTRStripAnalyzer* GetStripY(){return strip_ana_y;};


};

#endif // E16ANA_GTRAnalyzer_hh

