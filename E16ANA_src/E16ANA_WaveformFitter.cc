//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai
//2015-11-14, uploaded by nakai

#include "E16ANA_WaveformFitter.hh"

#include <iostream>
#include <algorithm>

#include <TH1F.h>
#include <TF1.h>
//#include <Math/Factory.h>

int E16ANA_WaveformFitter::n_instances = 0;

E16ANA_WaveformFitter::E16ANA_WaveformFitter(const std::string &file_name) :
   n_wave(2), clock_period(25.0), noise_sigma(5.0),
   instance_id(E16ANA_WaveformFitter::n_instances)
{
   //pars_time.reserve(10);
   //pars_peak.reserve(10);
   fit_params.reserve(10);
   //wf_temp = new E16ANA_WaveformTemplate("waveform_template.dat");
   wf_temp = new E16ANA_WaveformTemplate(file_name.c_str());
   n_instances++;
}

E16ANA_WaveformFitter::~E16ANA_WaveformFitter(){
   delete wf_temp;
}

//double E16ANA_WaveformFitter::MinuitFunction(const std::vector<double> &pars){
double E16ANA_WaveformFitter::MinuitFunction(const double *pars){
   double chi2 = 0.0;
   double x[1];
   double *par_temp = new double[n_wave*2];
   for(int i=0; i<n_wave*2; i++){
      par_temp[i] = pars[i];
   }
   for(int i=0; i<(int)waveform.size(); i++){
      x[0] = i*clock_period;
      // Template fitting
      double temp = FitFuncTemplate(x, par_temp)-waveform[i];
      chi2 += (temp*temp)/(noise_sigma*noise_sigma);
   }
   delete [] par_temp;
   return chi2;
}

double E16ANA_WaveformFitter::FitFuncTemplate(double *x, double *par){
   double xx = x[0];
   double result = 0.0;
   for(int i=0; i<n_wave; i++){
      result += par[i*2]*(wf_temp->GetValue(xx-par[i*2+1]));
   }
   return result;
}

//void E16ANA_WaveformFitter::PrintFitCheck(){
void E16ANA_WaveformFitter::PrintFitCheck(TH1F *h, TGraph *g){
   double x[1];
   double *par_temp = new double[n_wave*2];
   for(int i=0; i<n_wave; i++){
      par_temp[i*2] = fit_params[i].peak;
      par_temp[i*2+1] = fit_params[i].time;
   }
   for(int i=0; i<(int)waveform.size(); i++){
      x[0] = i*clock_period;
      double wf_val = waveform[i];
      double ff_val = FitFuncTemplate(x, par_temp);
      //std::cout << i << "th sample : waveform = " << wf_val
      //   << ", Fitted func = " << ff_val << std::endl;
      h->Fill(i*clock_period, wf_val);
   }
   for(int i=0; i<(int)waveform.size()*10; i++){
      x[0] = (i-0.5)*clock_period/10.0;
      double ff_val = FitFuncTemplate(x, par_temp);
      g->SetPoint(i, x[0], ff_val);
   }
}

void E16ANA_WaveformFitter::SetWaveform(const double *_fadc, int size){
   waveform.clear();
   waveform.resize(size);
   for(int i=0; i<size; i++){
      waveform[i] = (double)_fadc[i];
   }
}

void E16ANA_WaveformFitter::SetWaveform(const std::vector<int> &_fadc){
//void E16ANA_WaveformFitter::SetWaveform(const std::vector<double> &_fadc){
   waveform.clear();
   waveform.resize(_fadc.size());
   for(int i=0; i<(int)_fadc.size(); i++){
      waveform[i] = (double)_fadc[i];
   }
}

//void E16ANA_WaveformFitter::Clear(){
//   h_fit->Reset();
//}

void E16ANA_WaveformFitter::Fit(){
   if((int)waveform.size()==0){
      std::cout << "E16ANA_WaveformFitter::Fit() : Data is empty." << std::endl;
      return;
   }
   //TemplateFit();
   //TMinuitMinimizer *minuit = new TMinuitMinimizer(ROOT::Minuit::kMigradImproved);
   TMinuitMinimizer *minuit = new TMinuitMinimizer(ROOT::Minuit::kSimplex);
   //TMinuitMinimizer *minuit = new TMinuitMinimizer(ROOT::Minuit::kMigrad, n_wave*2);
   //ROOT::Math::Minimizer *minuit = ROOT::Math::Factory::CreateMinimizer("Minuit","");
   E16ANA_WaveformFitterFCN fcn(this);
   //minuit->SetFunction(static_cast<ROOT::Math::IMultiGenFunction*>(&fcn));
   minuit->SetFunction(fcn);
   minuit->SetPrintLevel(-1);
   //minuit->SetPrintLevel(3);
   minuit->SetStrategy(2);
   //minuit->SetStrategy(0);
   minuit->SetMaxFunctionCalls(100000);
   minuit->SetTolerance(0.001);

   double init_peak = 100.0;
   double step_peak = 10.0;
   double lower_peak = 0.0;
   double upper_peak = 1000.0;
   double init_time = 0.0;
   double step_time = 25.0;
   double lower_time = (-0.5-5.0)*clock_period;
   double upper_time = (waveform.size()-0.5-2.0)*clock_period;

   for(int i=0; i<n_wave; i++){
      minuit->SetLimitedVariable(i*2  , Form("Peak_%d",i), init_peak, step_peak, lower_peak, upper_peak);
      init_time = (upper_time-lower_time)/((double)n_wave+1.0)*(i+1.0)+lower_time;
      minuit->SetLimitedVariable(i*2+1, Form("Time_%d",i), init_time, step_time, lower_time, upper_time);
      //minuit->SetVariable(i*2+1, Form("Time_%d",i), init_time, step_time);
   }
   bool iret = minuit->Minimize();
   //std::cout << "MinValue = " << minuit->MinValue() << std::endl;
   //if(!iret){
   //   std::cout << "E16ANA_WaveformFitter::Fit() : Minimize error." << std::endl;
   //   //return;
   //}
   //minuit->PrintResults();
   //pars_peak.clear();
   //pars_time.clear();
   fit_params.clear();
   for(int i=0; i<n_wave; i++){
      //pars_peak.push_back(minuit->X()[i*2]);
      //pars_time.push_back(minuit->X()[i*2+1]);
      fit_params.push_back(fit_param_t(minuit->X()[i*2], minuit->X()[i*2+1]));
   }
   std::sort(fit_params.begin(), fit_params.end());
   delete minuit;
}

ROOT::Math::IMultiGenFunction* E16ANA_WaveformFitterFCN::Clone() const {
   return new E16ANA_WaveformFitterFCN(this->fitter);
}

