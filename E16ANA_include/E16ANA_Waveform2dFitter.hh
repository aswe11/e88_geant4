//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai
//2015-11-14, uploaded by nakai
#ifndef E16ANA_Waveform2dFitter_hh
#define E16ANA_Waveform2dFitter_hh

#include <vector>
#include <map>
#include <string>

#include <TMinuitMinimizer.h>
#include <TH2F.h>
#include <TGraph.h>

#include "E16ANA_WaveformTemplate.hh"

class E16ANA_Waveform2dFitter{
public:
   E16ANA_Waveform2dFitter(const std::string &file_name);
   ~E16ANA_Waveform2dFitter();
   void AddWaveform(double pos, const double *_fadc, int size);
   void AddWaveform(double pos, const std::vector<int> &_fadc);
   void AddWaveform(double pos, const std::vector<double> &_fadc);
   void Clear();
   void Fit();
   //double GetChi2();
   void SetNumClusters(int _n_cluster){n_cluster = _n_cluster;};
   int GetNumClusters(){return n_cluster;};
   //double GetClusterPos(int i){return pars_pos[i];};
   //double GetClusterTime(int i){return pars_time[i];};
   //double GetClusterPeak(int i){return pars_peak[i];};
   double GetClusterPos(int i){return fit_params[i].pos;};
   double GetClusterTime(int i){return fit_params[i].time;};
   double GetClusterPeak(int i){return fit_params[i].peak;};
   //double MinuitFunction(const std::vector<double> &pars);
   double MinuitFunction(const double *pars);
   void PrintFitCheck(TH2F *h, TGraph *g);

private:
   struct waveform_t {
      double pos;
      std::vector<double> waveform;
      waveform_t(double p, const std::vector<double> w){
         pos = p;
         waveform = w;
      };
      bool operator<(const waveform_t &rhs) const {
         return pos < rhs.pos;
      };
   };

   struct pre_param_t {
      double peak;
      double pos;
      double time;
      pre_param_t(double _peak, double _pos, double _time){
         peak = _peak;
         pos  = _pos;
         time = _time;
      };
      bool operator<(const pre_param_t &rhs) const {
         return peak < rhs.peak;
      };
   };

   struct fit_param_t {
      double peak;
      double pos;
      double time;
      fit_param_t(double _peak, double _pos, double _time){
         peak = _peak;
         pos  = _pos;
         time = _time;
      };
      bool operator<(const fit_param_t &rhs) const {
         return pos < rhs.pos;
      };
   };

   //std::vector<std::pair<double, std::vector<double> > > waveforms;
   std::vector<waveform_t> waveforms;
   int n_cluster;
   double clock_period;
   double gauss_sigma;
   double noise_sigma;
   E16ANA_WaveformTemplate *wf_temp;

   //TH1F *h_fit;
   static int n_instances;
   int instance_id;

   double pos_min;
   double pos_max;

   //std::vector<double> pars_pos;
   //std::vector<double> pars_time;
   //std::vector<double> pars_peak;
   std::vector<fit_param_t> fit_params;

   std::vector<pre_param_t> pre_params;

   double FitFuncTemplate(double *x, double *par);
   //void TemplateFit();
   void PreConditioning();
   bool IsLocalPeak(int i, int j);
   double GetWaveformValue(int i, int j);

};

class E16ANA_Waveform2dFitterFCN : public ROOT::Math::IMultiGenFunction {
public:
   E16ANA_Waveform2dFitterFCN(E16ANA_Waveform2dFitter *_fitter){fitter = _fitter;};
   ~E16ANA_Waveform2dFitterFCN(){};
   ROOT::Math::IMultiGenFunction* Clone() const;
   double operator() (const double *x) const {
      return fitter->MinuitFunction(x);
   };
   unsigned int NDim() const {return fitter->GetNumClusters()*3;};

private:
   E16ANA_Waveform2dFitter *fitter;
   double DoEval(const double *x) const {return fitter->MinuitFunction(x);};

};

#endif // E16ANA_WaveformFitter_hh
