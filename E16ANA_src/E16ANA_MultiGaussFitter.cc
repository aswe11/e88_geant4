//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai
//2015-11-14, uploaded by nakai

#include "E16ANA_MultiGaussFitter.hh"

#include <vector>
//#include <pair>
#include <algorithm>

#include <TMath.h>
#include <TF1.h>

int E16ANA_MultiGaussFitter::n_instances = 0;

E16ANA_MultiGaussFitter::E16ANA_MultiGaussFitter(int _n_bins, double _xmin, double _xmax) :
   n_bins(_n_bins), xmin(_xmin), xmax(_xmax), n_gauss(5), const_sigma(0.35*4),
   instance_id(n_instances)
{
   h_fit = new TH1F(Form("h_multi_gauss_fit_%d", instance_id), "Multi gauss fit",
         _n_bins, _xmin, _xmax);
   pars_mean.reserve(10);
   pars_peak.reserve(10);
   n_instances++;
}

E16ANA_MultiGaussFitter::~E16ANA_MultiGaussFitter(){
   delete h_fit;
}

double E16ANA_MultiGaussFitter::FitFunc(double *x, double *par){
   double xx = x[0];
   double result = 0.0;
   for(int i=0; i<n_gauss; i++){
      result += par[i*2]*TMath::Gaus(xx, par[i*2+1], const_sigma, false);
   }
   return result;
}

void E16ANA_MultiGaussFitter::AddPoint(double x, double y){
   h_fit->Fill(x, y);
}

void E16ANA_MultiGaussFitter::Clear(){
   h_fit->Reset();
}

void E16ANA_MultiGaussFitter::Fit(){
   n_gauss = ceil(((xmax-xmin)/const_sigma-4.0)/2.0+1.0);
   if(n_gauss <= 0) n_gauss = 1;

   TF1 *f_fit = new TF1(Form("f_multi_gauss_fit_%d", instance_id),
         this, &E16ANA_MultiGaussFitter::FitFunc,
         xmin, xmax, n_gauss*2,
         "E16ANA_MultiGaussFitter", "FitFunc");
   f_fit->SetNpx(n_bins*10);
   for(int i=0; i<n_gauss; i++){
      //f_fit->SetParameter(i*2+1, xmin+(xmax-xmin)/n_gauss);
      f_fit->SetParameter(i*2, 20.0);
      //f_fit->SetParameter(i*2+1, (xmin+xmax)/2.0);
      f_fit->SetParameter(i*2+1, (xmax-xmin)/(n_gauss+1.0)*(i+1)+xmin);
      f_fit->SetParLimits(i*2, 10.0, 1000.0);
      f_fit->SetParLimits(i*2+1, xmin, xmax);
   }
   //std::cout << "First fit : n_gauss = " << n_gauss << std::endl;
   h_fit->Fit(Form("f_multi_gauss_fit_%d", instance_id),"QN");
   std::vector<double> pars(n_gauss);
   for(int i=0; i<n_gauss; i++){
      //pars[i].first = f_fit->GetParameter(i*2); // height
      pars[i] = f_fit->GetParameter(i*2+1); // mean
   }
   delete f_fit;

   //std::sort(pars.begin(), pars.end());
   int *index = new int[pars.size()];
   TMath::Sort((int)pars.size(), &pars[0], index, false);
   double pre_mean = xmin-1000.0*const_sigma;
   int n_gauss_re = 0;
   for(int i=0; i<n_gauss; i++){
      int ii = index[i];
      double mean = pars[ii];
      if(fabs(mean-pre_mean) > const_sigma*2.0){
         n_gauss_re++;
         pre_mean = mean;
      }
   }

   n_gauss = n_gauss_re;
   f_fit = new TF1(Form("f_multi_gauss_fit_%d", instance_id),
         this, &E16ANA_MultiGaussFitter::FitFunc,
         xmin, xmax, n_gauss*2,
         "E16ANA_MultiGaussFitter", "FitFunc");
   f_fit->SetNpx(n_bins*10);
   for(int i=0; i<n_gauss; i++){
      //f_fit->SetParameter(i*2+1, xmin+(xmax-xmin)/n_gauss);
      f_fit->SetParameter(i*2, 20.0);
      //f_fit->SetParameter(i*2+1, (xmin+xmax)/2.0);
      f_fit->SetParameter(i*2+1, (xmax-xmin)/(n_gauss+1.0)*(i+1)+xmin);
      f_fit->SetParLimits(i*2, 10.0, 1000.0);
      f_fit->SetParLimits(i*2+1, xmin, xmax);
   }
   //std::cout << "Refit : n_gauss = " << n_gauss << std::endl;
   h_fit->Fit(Form("f_multi_gauss_fit_%d", instance_id), "QN");

   pars_peak.clear();
   pars_peak.resize(n_gauss);
   pars_mean.clear();
   pars_mean.resize(n_gauss);
   for(int i=0; i<n_gauss; i++){
      pars_peak[i] = f_fit->GetParameter(i*2); // height
      pars_mean[i] = f_fit->GetParameter(i*2+1); // mean
   }
   delete f_fit;
   delete [] index;
}

