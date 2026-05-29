//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai
//2015-11-02, uploaded by yokkaich
//2015-11-02, uploaded by komatsu
//2015-03-27, uploaded by yokkaich
//2015-03-20, uploaded by nakai
#ifndef E16ANA_WaveformFitManager_hh
#define E16ANA_WaveformFitManager_hh 1

#include <TH1F.h>
#include <TF1.h>

Double_t FitFunction1(double *x, double *par);

class E16ANA_WaveformFitManager{

private:
   int n_fadc_counts;
	double *fadc;
	double pedestal;
	double params[8];
	double chi2;
   double default_t0;
   double default_peak;
   double default_tau;

public:
	E16ANA_WaveformFitManager(int n_fadc_counts_);
	virtual ~E16ANA_WaveformFitManager();
	void DoFit();

	void SetFadc(double *afadc);
	void SetPedestal(double apedestal);

	void GetParams(double *par);
	double GetChi2();
};

#endif // E16ANA_WaveformFitManager_hh

