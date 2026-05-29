//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai
//2015-11-14, uploaded by nakai
#ifndef E16ANA_MultiGaussFitter_hh
#define E16ANA_MultiGaussFitter_hh

#include <TH1F.h>

class E16ANA_MultiGaussFitter{
public:
   E16ANA_MultiGaussFitter(int _n_bins, double _xmin, double _xmax);
   ~E16ANA_MultiGaussFitter();
   void AddPoint(double x, double y);
   void Clear();
   void Fit();
   //double GetChi2();
   //void SetNumGaussian(int _n_gauss){n_gauss = _n_gauss;};
   void SetConstSigma(double _const_sigma){const_sigma = _const_sigma;};
   //void GetParameters(double *pars);
   int GetNumGaussian(){return n_gauss;};
   double GetGaussianMean(int i){return pars_mean[i];};
   double GetGaussianPeak(int i){return pars_peak[i];};

private:
   int n_bins;
   double xmin;
   double xmax;
   TH1F *h_fit;
   int n_gauss;
   double const_sigma;
   static int n_instances;
   int instance_id;
   std::vector<double> pars_mean;
   std::vector<double> pars_peak;

   double FitFunc(double *x, double *par);

};

#endif // E16ANA_MultiGaussFitter_hh
