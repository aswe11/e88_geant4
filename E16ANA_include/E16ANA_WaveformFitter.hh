//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai
//2015-11-14, uploaded by nakai
#ifndef E16ANA_WaveformFitter_hh
#define E16ANA_WaveformFitter_hh

#include <vector>
#include <string>

#include <TMinuitMinimizer.h>
#include <TH1F.h>
#include <TGraph.h>

#include "E16ANA_WaveformTemplate.hh"

class E16ANA_WaveformFitter{
public:
   E16ANA_WaveformFitter(const std::string &file_name);
   ~E16ANA_WaveformFitter();
   //void SetWaveform(const std::vector<int> &_fadc);
   void SetWaveform(const double *_fadc, int size);
   void SetWaveform(const std::vector<int> &_fadc);
   void SetWaveform(const std::vector<double> &_fadc){waveform = _fadc;};
   //void Clear();
   void Fit();
   //double GetChi2();
   void SetNumWaveforms(int _n_wave){n_wave = _n_wave;};
   int GetNumWaveforms(){return n_wave;};
   //double GetWaveformTime(int i){return pars_time[i];};
   //double GetWaveformPeak(int i){return pars_peak[i];};
   double GetWaveformTime(int i){return fit_params[i].time;};
   double GetWaveformPeak(int i){return fit_params[i].peak;};
   //double MinuitFunction(const std::vector<double> &pars);
   double MinuitFunction(const double *pars);
   void PrintFitCheck(TH1F *h, TGraph *g);

private:
   struct fit_param_t {
      double peak;
      double time;
      fit_param_t(double _peak, double _time){
         peak = _peak;
         time = _time;
      };
      bool operator<(const fit_param_t &rhs) const {
         return time < rhs.time;
      };
   };

   std::vector<double> waveform;
   int n_wave;
   double clock_period;
   double noise_sigma;
   E16ANA_WaveformTemplate *wf_temp;

   //TH1F *h_fit;
   static int n_instances;
   int instance_id;
   //std::vector<double> pars_time;
   //std::vector<double> pars_peak;
   std::vector<fit_param_t> fit_params;

   double FitFuncTemplate(double *x, double *par);
   //void TemplateFit();

};

class E16ANA_WaveformFitterFCN : public ROOT::Math::IMultiGenFunction {
public:
   E16ANA_WaveformFitterFCN(E16ANA_WaveformFitter *_fitter){fitter = _fitter;};
   ~E16ANA_WaveformFitterFCN(){};
   ROOT::Math::IMultiGenFunction* Clone() const;
   double operator() (const double *x) const {
      return fitter->MinuitFunction(x);
   };
   unsigned int NDim() const {return fitter->GetNumWaveforms()*2;};

private:
   E16ANA_WaveformFitter *fitter;
   double DoEval(const double *x) const {return fitter->MinuitFunction(x);};

};

#endif // E16ANA_WaveformFitter_hh
