//2016-11-22, uploaded by nakai
//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai
//2015-11-14, uploaded by nakai

#include "E16ANA_Waveform2dFitter.hh"

#include <iostream>
#include <algorithm>

#include <TH1F.h>
#include <TF1.h>
#include <TMath.h>
//#include <Math/Factory.h>

int E16ANA_Waveform2dFitter::n_instances = 0;

E16ANA_Waveform2dFitter::E16ANA_Waveform2dFitter(const std::string &file_name) :
   n_cluster(2), clock_period(25.0), gauss_sigma(0.3), noise_sigma(5.0),
   instance_id(E16ANA_Waveform2dFitter::n_instances)
{
   //pars_pos.reserve(10);
   //pars_time.reserve(10);
   //pars_peak.reserve(10);
   fit_params.reserve(10);
   //wf_temp = new E16ANA_WaveformTemplate("waveform_template.dat");
   wf_temp = new E16ANA_WaveformTemplate(file_name.c_str());
   n_instances++;
   //waveforms.resize(20,std::pair<double, std::vector<double> >(0.0, std::vector<double>(30)));
   waveforms.resize(20, waveform_t(0.0, std::vector<double>(30)));
   Clear();
}

E16ANA_Waveform2dFitter::~E16ANA_Waveform2dFitter(){
   delete wf_temp;
}

void E16ANA_Waveform2dFitter::Clear(){
   //pars_pos.clear();
   //pars_time.clear();
   //pars_peak.clear();
   fit_params.clear();
   waveforms.clear();
   pos_min =  10000.0;
   pos_max = -10000.0;
}

void E16ANA_Waveform2dFitter::AddWaveform(double pos, const double *_fadc, int size){
   waveforms.push_back(waveform_t(pos, std::vector<double>(size)));
   for(int i=0; i<size; i++){
      (waveforms.back().waveform)[i] = (double)_fadc[i];
   }
   if(pos_min > pos) pos_min = pos;
   if(pos_max < pos) pos_max = pos;
}

void E16ANA_Waveform2dFitter::AddWaveform(double pos, const std::vector<int> &_fadc){
   //waveforms.push_back(std::pair<double, std::vector<double> >(pos, std::vector<double>(_fadc.size())));
   waveforms.push_back(waveform_t(pos, std::vector<double>(_fadc.size())));
   for(int i=0; i<(int)_fadc.size(); i++){
      (waveforms.back().waveform)[i] = (double)_fadc[i];
   }
   if(pos_min > pos) pos_min = pos;
   if(pos_max < pos) pos_max = pos;
}

void E16ANA_Waveform2dFitter::AddWaveform(double pos, const std::vector<double> &_fadc){
   waveforms.push_back(waveform_t(pos, _fadc));
   if(pos_min > pos) pos_min = pos;
   if(pos_max < pos) pos_max = pos;
}

//double E16ANA_Waveform2dFitter::MinuitFunction(const std::vector<double> &pars){
double E16ANA_Waveform2dFitter::MinuitFunction(const double *pars){
   double chi2 = 0.0;
   double x[2];
   double *par_temp = new double[n_cluster*3];
   for(int i=0; i<n_cluster*3; i++){
      par_temp[i] = pars[i];
   }
   for(int i=0; i<(int)waveforms.size(); i++){
      //x[0] = waveforms[i].first; // xx
      x[0] = waveforms[i].pos; // xx
      //std::vector<double> &waveform = waveforms[i].second;
      std::vector<double> &waveform = waveforms[i].waveform;
      for(int j=0; j<(int)waveform.size(); j++){
         x[1] = j*clock_period; // yy
         // Template fitting
         double temp = FitFuncTemplate(x, par_temp)-waveform[j];
         chi2 += (temp*temp)/(noise_sigma*noise_sigma);
      }
   }
   delete [] par_temp;
   return chi2;
}

double E16ANA_Waveform2dFitter::FitFuncTemplate(double *x, double *par){
   double xx = x[0];
   double yy = x[1];
   double result = 0.0;
   for(int i=0; i<n_cluster; i++){
      result += par[i*3]*TMath::Gaus(xx, par[i*3+1], gauss_sigma, false)
         *(wf_temp->GetValue(yy-par[i*3+2]));
   }
   return result;
}

//void E16ANA_Waveform2dFitter::PrintFitCheck(){
void E16ANA_Waveform2dFitter::PrintFitCheck(TH2F *h, TGraph *g){
   //double x[2];
   //double *par_temp = new double[n_cluster*3];
   //for(int i=0; i<n_wave; i++){
   //   par_temp[i*2] = pars_peak[i];
   //   par_temp[i*2+1] = pars_time[i];
   //}
   for(int i=0; i<(int)waveforms.size(); i++){
      //double xx = waveforms[i].first; // xx
      double xx = waveforms[i].pos; // xx
      //std::vector<double> &waveform = waveforms[i].second;
      std::vector<double> &waveform = waveforms[i].waveform;
      for(int j=0; j<(int)waveform.size(); j++){
         double yy = j*clock_period;
         double wf_val = waveform[j];
         //double ff_val = FitFuncTemplate(x, par_temp);
         //std::cout << i << "th sample : waveform = " << wf_val
         //   << ", Fitted func = " << ff_val << std::endl;
         h->Fill(xx, yy, wf_val);
      }
   }
   for(int i=0; i<n_cluster; i++){
      if(fit_params[i].peak > 5.0){
         //g->SetPoint(g->GetN(), pars_pos[i], pars_time[i]+50.0);
         g->SetPoint(g->GetN(), fit_params[i].pos, fit_params[i].time+50.0);
      }
   }
   //delete [] par_temp;
}

void E16ANA_Waveform2dFitter::Fit(){
   if((int)waveforms.size() == 0){
      std::cout << "E16ANA_Waveform2dFitter::Fit() : Data is empty." << std::endl;
      return;
   }

   PreConditioning();

   //TemplateFit();
   //TMinuitMinimizer *minuit = new TMinuitMinimizer(ROOT::Minuit::kMigrad);
   //TMinuitMinimizer *minuit = new TMinuitMinimizer(ROOT::Minuit::kMigradImproved);
   //TMinuitMinimizer *minuit = new TMinuitMinimizer(ROOT::Minuit::kCombined);
   TMinuitMinimizer *minuit = new TMinuitMinimizer(ROOT::Minuit::kSimplex);
   //ROOT::Math::Minimizer *minuit = ROOT::Math::Factory::CreateMinimizer("Minuit","Migrad");

   E16ANA_Waveform2dFitterFCN fcn(this);
   //minuit->SetFunction(static_cast<ROOT::Math::IMultiGenFunction*>(&fcn));
   minuit->SetFunction(fcn);
   minuit->SetPrintLevel(-1);
   //minuit->SetPrintLevel(3);
   minuit->SetStrategy(2);
   //minuit->SetStrategy(0);
   minuit->SetMaxFunctionCalls(100000);
   minuit->SetTolerance(0.001);

   double init_peak = 50.0;
   //double step_peak = 10.0;
   double lower_peak = 0.0;
   double upper_peak = 1000.0;
   double init_pos = (pos_max+pos_min)/2.0;
   //double step_pos = 1.0;
   double lower_pos = pos_min;
   double upper_pos = pos_max;
   double init_time = 0.0;
   //double step_time = 25.0;
   double lower_time = (-0.5-10.0)*clock_period;

   double step_peak = 500.0;
   //double step_pos = 5.6;
   double step_pos = (pos_max-pos_min)/2.0;
   double step_time = 250.0;

   //double upper_time = (waveforms[0].second.size()-0.5-2.0)*clock_period;
   double upper_time = (waveforms[0].waveform.size()-0.5)*clock_period;

   for(int i=0; i<n_cluster; i++){
      if(i<(int)pre_params.size()){
         minuit->SetLimitedVariable(i*3  , Form("Peak_%d",i), pre_params[i].peak, step_peak, lower_peak, upper_peak);
         minuit->SetLimitedVariable(i*3+1, Form("Pos_%d",i), pre_params[i].pos, step_pos, lower_pos, upper_pos);
         //minuit->SetVariable(i*3+2, Form("Time_%d",i), pre_params[i].time-50.0, step_time);
         minuit->SetLimitedVariable(i*3+2, Form("Time_%d",i), pre_params[i].time-50.0, step_time, lower_time, upper_time);
      }else{
         minuit->SetLimitedVariable(i*3  , Form("Peak_%d",i), init_peak, step_peak, lower_peak, upper_peak);
         init_pos = (pos_max-pos_min)/((double)n_cluster+1.0)*(i+1.0)+pos_min;
         if((int)waveforms.size() > 1){
            minuit->SetLimitedVariable(i*3+1, Form("Pos_%d",i), init_pos, step_pos, lower_pos, upper_pos);
         }else{
            minuit->SetFixedVariable(i*3+1, Form("Pos_%d",i), init_pos);
         }
         //init_time = (upper_time-lower_time)/((double)n_wave+1.0)*(i+1.0)+lower_time;
         init_time = (upper_time+lower_time)/2.0;
         minuit->SetLimitedVariable(i*3+2, Form("Time_%d",i), init_time, step_time, lower_time, upper_time);
         //minuit->SetVariable(i*3+2, Form("Time_%d",i), init_time, step_time);
      }
   }
   bool iret = minuit->Minimize();
   //std::cout << "MinValue = " << minuit->MinValue() << std::endl;
   //if(!iret){
   //   std::cout << "E16ANA_Waveform2dFitter::Fit() : Minimize error." << std::endl;
   //   //return;
   //}
   //minuit->PrintResults();
   //pars_peak.clear();
   //pars_pos.clear();
   //pars_time.clear();
   fit_params.clear();
   for(int i=0; i<n_cluster; i++){
      //pars_peak.push_back(minuit->X()[i*3]);
      //pars_pos.push_back( minuit->X()[i*3+1]);
      //pars_time.push_back(minuit->X()[i*3+2]);
      fit_params.push_back(
            fit_param_t(
               minuit->X()[i*3],
               minuit->X()[i*3+1],
               minuit->X()[i*3+2]));
   }
   std::sort(fit_params.begin(), fit_params.end());
   delete minuit;
}

void E16ANA_Waveform2dFitter::PreConditioning(){
   pre_params.clear();
   std::sort(waveforms.begin(), waveforms.end());
   for(int i=0; i<(int)waveforms.size(); i++){
      std::vector<double> &wf = waveforms[i].waveform;
      for(int j=0; j<(int)wf.size(); j++){
         double val = wf[j];
         bool is_peak = false;
         if(j==0 && val>wf[j+1]){
            is_peak = IsLocalPeak(i, j);
         }else if(j==(int)(wf.size()-1) && val>wf[j-1]){
            is_peak = IsLocalPeak(i, j);
         }else if(val>wf[j-1] && val>wf[j+1]){
            is_peak = IsLocalPeak(i, j);
         }
         if(is_peak){
            //std::cout << "LocalPeak : pos = " << waveforms[i].pos
            //   << ", time = " << j*clock_period
            //   << ", peak = " << GetWaveformValue(i,j)
            //   << std::endl;
            pre_params.push_back(pre_param_t(GetWaveformValue(i,j), waveforms[i].pos, j*clock_period));
         }
      }
   }
   std::sort(pre_params.begin(), pre_params.end());
   //std::sort(pre_params.begin(), pre_params.end(), pre_params[0].compare_peak);
}

bool E16ANA_Waveform2dFitter::IsLocalPeak(int i, int j){
   if(i<0 || i>=(int)waveforms.size()){
      return false;
   }
   if(j<0 || j>=(int)waveforms[i].waveform.size()){
      return false;
   }
   double threshold = 20.0;
   bool ret = true;
   double val = GetWaveformValue(i, j);
   //ret = ret && (val > GetWaveformValue(i-1, j-1));
   ret = ret && (val > GetWaveformValue(i-1, j  ));
   //ret = ret && (val > GetWaveformValue(i-1, j+1));
   ret = ret && (val > GetWaveformValue(i  , j-1));
   ret = ret && (val > GetWaveformValue(i  , j+1));
   //ret = ret && (val > GetWaveformValue(i+1, j-1));
   ret = ret && (val > GetWaveformValue(i+1, j  ));
   //ret = ret && (val > GetWaveformValue(i+1, j+1));

   //ret = ret && (GetWaveformValue(i-1, j  ) > GetWaveformValue(i-2, j  ));
   //ret = ret && (GetWaveformValue(i+1, j  ) < GetWaveformValue(i+2, j  ));
   ret = ret && (GetWaveformValue(i  , j-1) > GetWaveformValue(i  , j-2));
   //ret = ret && (GetWaveformValue(i  , j+1) < GetWaveformValue(i  , j+2));

   ret = ret && (val > threshold);

   return ret;
}

double E16ANA_Waveform2dFitter::GetWaveformValue(int i, int j){
   if(i<0 || i>=(int)waveforms.size()){
      return 10000;
   }
   if(j<0 || j>=(int)waveforms[i].waveform.size()){
      return 10000;
   }
   return waveforms[i].waveform[j];
}

ROOT::Math::IMultiGenFunction* E16ANA_Waveform2dFitterFCN::Clone() const {
   return new E16ANA_Waveform2dFitterFCN(this->fitter);
}

