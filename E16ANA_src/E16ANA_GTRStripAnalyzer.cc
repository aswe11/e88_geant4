//2016-11-22, uploaded by nakai
//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai
//2015-12-21, uploaded by yokkaich
//2015-12-12, uploaded by nakai
//2015-11-14, uploaded by nakai
//2015-11-02, uploaded by yokkaich
//2015-11-02, uploaded by komatsu
//2015-10-05, uploaded by yokkaich
//2015-07-15, uploaded by nakai
//2015-05-29, uploaded by yokkaich
//2015-04-08, uploaded by nakai

#include <iostream>
#include <cmath>
#include <algorithm>

#include "E16ANA_GTRStripAnalyzer.hh"
#include "E16ANA_WaveformFitter.hh"
#include "E16ANA_Waveform2dFitter.hh"
#include "E16ANA_ErrorMessage.hh"


void E16ANA_GTRAnalyzedStripHit::CalcTdc2(double tan_theta){
   SetTanTheta(tan_theta);

   double temp_tdc_hit = 0.0;

   if((int)strip_pos.size()==0){
      return;
   }

   double A= 0.0;
   for(int i=0; i<(int)strip_pos.size(); i++){
      A += strip_pos[i]-tan_theta*strip_posz[i];
   }
   double tdc_x0= A/strip_pos.size();
   temp_tdc_hit = tdc_x0;

   SetTdcHit(temp_tdc_hit);
   SetTdc2Hit(temp_tdc_hit);
}

E16ANA_GTRStripAnalyzer::E16ANA_GTRStripAnalyzer(int _n_strips, int _n_sampling) : 
   n_strips(_n_strips), n_sampling(_n_sampling),
   strip_pitch(0.35), gem_threshold(4.0), drift_velocity(0.010), drift_gap_center(0.0), drift_gap_center_v3(0.0),
   fadc_clock_period(25.0), fadc_t0_correction(0.0), gem_tdc_min(-10000.0), gem_tdc_max(10000.0),
   gem_tr(75.0), threshold_fraction(0.5), inverted(1.0)
{
   fadc = new double*[n_strips];
   for(int i=0; i<n_strips; i++){
      fadc[i] = new double[n_sampling];
   }
   fadc_peak = new double[n_strips];
   fadc_peak_time = new double[n_strips];
   fadc_tdc = new double[n_strips];
   fadc_ped = new double[n_strips];
   gem_analyzed_hits.reserve(20);
   for(int i=0; i<n_strips; i++){
      fadc_ped[i] = 0.0;
   }
   //wf1d_fitter = new E16ANA_WaveformFitter("waveform_template.dat");
   //wf2d_fitter = new E16ANA_Waveform2dFitter("waveform_template.dat");
   wf1d_fitter = NULL;
   wf2d_fitter = NULL;
   Clear();
}

E16ANA_GTRStripAnalyzer::~E16ANA_GTRStripAnalyzer(){
   for(int i=0; i<n_strips; i++){
      delete [] fadc[i];
   }
   delete [] fadc;
   delete [] fadc_peak;
   delete [] fadc_peak_time;
   delete [] fadc_tdc;
   delete [] fadc_ped;
   if(wf1d_fitter){
      delete wf1d_fitter;
   }
   if(wf2d_fitter != NULL){
      delete wf2d_fitter;
   }
}

void E16ANA_GTRStripAnalyzer::SetWaveformTemplate(const std::string &file_name){
   wf1d_fitter = new E16ANA_WaveformFitter(file_name);
   wf2d_fitter = new E16ANA_Waveform2dFitter(file_name);
}

void E16ANA_GTRStripAnalyzer::Clear(){
   for(int i=0; i<n_strips; i++){
      for(int j=0; j<n_sampling; j++){
         fadc[i][j] = -100000.0;
      }
   }
   gem_analyzed_hits.clear();
   clustered_x.clear();
   clustered_z.clear();
   clustered_peak.clear();
}

void E16ANA_GTRStripAnalyzer::SetFadc(int strip_id, double *waveform){
   for(int j=0; j<n_sampling; j++){
      fadc[strip_id][j] = waveform[j];
   }
}

void E16ANA_GTRStripAnalyzer::Analyze(){

   //CalcWaveParamsPeak();
   int n_hits = HitClusteringV0();
   gem_analyzed_hits.resize(n_hits);
   //gem_analyzed_hits.resize(n_hits,E16ANA_GTRAnalyzedStripHit());
   for(int i=0; i<n_hits; i++){
      gem_analyzed_hits[i].SetInvalid();
      CalcCenterOfGravity(clustered_strip_id[i], gem_analyzed_hits[i]);
      CalcTdcHit1(clustered_strip_id[i], gem_analyzed_hits[i],  i );
      //CalcTdcHit2(clustered_strip_id[i], gem_analyzed_hits[i].TanTheta(), gem_analyzed_hits[i]);
   }
}
void E16ANA_GTRStripAnalyzer::Analyze2(){
   //CalcWaveParamsPeak();
   int n_hits = HitClusteringV0();
   gem_analyzed_hits.resize(n_hits);
   //gem_analyzed_hits.resize(n_hits,E16ANA_GTRAnalyzedStripHit());
   for(int i=0; i<n_hits; i++){
      CalcCenterOfGravity(clustered_strip_id[i], gem_analyzed_hits[i]);
      CalcTdcHit2(clustered_strip_id[i], gem_analyzed_hits[i].TanTheta(), gem_analyzed_hits[i]);
   }
}

void E16ANA_GTRStripAnalyzer::Analyze2(int id){
  //  E16MESSAGE("size %d %d", clustered_strip_id.size(), gem_analyzed_hits.size());
      CalcTdcHit2(clustered_strip_id[id], gem_analyzed_hits[id].TanTheta(), gem_analyzed_hits[id]);
  
}

void E16ANA_GTRStripAnalyzer::AnalyzeV1(){
   //CalcWaveParamsPeak();
   int n_hits = HitClusteringV1();
   gem_analyzed_hits.resize(n_hits);
   //gem_analyzed_hits.resize(n_hits,E16ANA_GTRAnalyzedStripHit());
   for(int i=0; i<n_hits; i++){
      gem_analyzed_hits[i].SetInvalid();
      CalcCenterOfGravity(clustered_strip_id[i], gem_analyzed_hits[i]);
      CalcTdcHit1(clustered_strip_id[i], gem_analyzed_hits[i],  i );
      //CalcTdcHit2(clustered_strip_id[i], gem_analyzed_hits[i].TanTheta(), gem_analyzed_hits[i]);
   }
}

int E16ANA_GTRStripAnalyzer::HitClusteringV0(){
   return HitClusteringV0(2, -10000.0);
}

int E16ANA_GTRStripAnalyzer::HitClusteringV0(const int min_gap, const double cluster_threshold, bool strip_share){
   CalcWaveParamsPeak();
   //const int min_gap = 2;
   int signal_gap = 100;

   for(int i=0; i<(int)clustered_strip_id.size(); i++){
      clustered_strip_id[i].clear();
   }
   clustered_strip_id.clear();

   //double pre_tdc = fadc_tdc[0];

   for(int i=0; i<n_strips; i++){
      if(fadc_peak[i] > gem_threshold){
         //double delta_tdc = fadc_tdc[i]-pre_tdc;
         //pre_tdc = fadc_tdc[i];
         //if(signal_gap>=min_gap || fabs(delta_tdc)>100.0){ // V1
         if(signal_gap >= min_gap){ // V0
            clustered_strip_id.push_back(std::vector<int>());
         }else if(fadc_peak[i] < cluster_threshold){
            if(strip_share){
               clustered_strip_id.back().push_back(i);
            }
            clustered_strip_id.push_back(std::vector<int>());
         }
         clustered_strip_id.back().push_back(i);
         signal_gap = 0;
      }else{
         signal_gap++;
      }
   }
   return clustered_strip_id.size();
}

int E16ANA_GTRStripAnalyzer::HitClusteringV1(const int min_gap, const double delta_tdc_threshold){
   CalcWaveParamsPeak();
   //const int min_gap = 2;
   int signal_gap = 100;
   //const double delta_tdc_threshold = 150.0;

   for(int i=0; i<(int)clustered_strip_id.size(); i++){
      clustered_strip_id[i].clear();
   }
   clustered_strip_id.clear();

   double pre_tdc = fadc_tdc[0];

   for(int i=0; i<n_strips; i++){
      if(fadc_peak[i]>gem_threshold){
         double delta_tdc = fadc_tdc[i]-pre_tdc;
         pre_tdc = fadc_tdc[i];
         if(signal_gap>=min_gap || fabs(delta_tdc)>delta_tdc_threshold){ // V1
         //if(signal_gap>=min_gap){ // V0
            clustered_strip_id.push_back(std::vector<int>());
         }
         clustered_strip_id.back().push_back(i);
         signal_gap = 0;
      }else{
         signal_gap++;
      }
   }
   return clustered_strip_id.size();
}

void E16ANA_GTRStripAnalyzer::CalcWaveParamsPeak(){
   for(int i=0; i<n_strips; i++){
      fadc_peak[i]= -255.0;
      fadc_peak_time[i]= -1000.0;
      fadc_tdc[i]= -1000.0;
   }

   for(int i=0; i<n_strips; i++){
   //for(int i=left_strip_x; i<right_strip_x; i++){
      if(fadc[i][0]<-255.0) continue;
      //this->CalcWaveParamsPeak(i, gem_tdc_max+gem_tr+50.0);
      this->CalcWaveParamsPeak(i, 100000.0);
      //if(fadc_tdc[i]>gem_tdc_max){
      //   this->CalcWaveParamsPeak(i, gem_tdc_max-gem_tr);
      //}
      if(fadc_tdc[i]>gem_tdc_max || fadc_tdc[i]<gem_tdc_min){
         fadc_peak[i]= -255.0;
         fadc_peak_time[i]= -1000.0;
         fadc_tdc[i]= -1000.0;
      }
   }
}

void E16ANA_GTRStripAnalyzer::CalcWaveParamsPeak(int ch, double t_cutoff){
   int peak_count;
   if(ch<n_strips){
      fadc_peak[ch]= -255.0;
      fadc_tdc[ch]= -1000.0;
      peak_count= -1;
      for(int j=0; j<n_sampling; j++){
      //for(int l=0; l<(int)fadc_valid_count.size(); l++){
         //int j= fadc_valid_count[l];
         double tdc_j= j*fadc_clock_period+fadc_t0_correction;
         if(tdc_j>t_cutoff) break;
         //if(tdc_j<t_cutoff) continue;

         //if(fadc_peak[ch]<fadc[ch][j]-fadc_ped[ch]){
         if(fadc_peak[ch]<fadc[ch][j]){
            //fadc_peak[ch]= fadc[ch][j]-fadc_ped[ch];
            fadc_peak[ch]= fadc[ch][j];
            peak_count= j;
         }
      }
      if(fadc_peak[ch]<gem_threshold){
         fadc_peak[ch]= -255.0;
         peak_count= -1;
      }else{
         //std::cout << "Peak detection, peak_value = " << fadc_peak[ch] << ", peak_count = " << peak_count << std::endl;
      }

      if(peak_count>-1){
         for(int j=peak_count; j>-1; j--){
         //for(int l=peak_count; l>-1; l--){
            //int j= fadc_valid_count[l];
            //double tdc_j= j*fadc_clock_period+fadc_t0_correction;
            //if(tdc_j<gem_t0_min) break;
            //if(fadc[ch][j]-fadc_ped[ch]<fadc_peak[ch]*threshold_fraction){
            if(fadc[ch][j]<fadc_peak[ch]*threshold_fraction){
               //double tdc_j = j;
               double w_j0 =  fadc_peak[ch]*threshold_fraction-fadc[ch][j];
               double w_j1 = -fadc_peak[ch]*threshold_fraction+fadc[ch][j+1];
               double tdc_j = (j*w_j1+(j+1.0)*w_j0)/(w_j0+w_j1);
               //std::cout << "tdc_j = " << tdc_j << std::endl;
               fadc_tdc[ch]= tdc_j*fadc_clock_period+fadc_t0_correction;
               fadc_peak_time[ch]= peak_count*fadc_clock_period+fadc_t0_correction;
               break;
            }
         }
      }else{
         fadc_tdc[ch]= -1000.0;
         fadc_peak_time[ch]= -1000.0;
      }
   }
}


void E16ANA_GTRStripAnalyzer::CalcCenterOfGravity(const std::vector<int> &strip_ids, E16ANA_GTRAnalyzedStripHit &hit){
   int temp_max_strip = -1;
   double temp_max_value = -255.0;
   int temp_num_hit = 0;
   double temp_cc = 0.0;
   double temp_cog = 0.0;

   if((int)strip_ids.size()==0){
      hit.SetInvalid();
      return;
   }

   for(int i=0; i<(int)strip_ids.size(); i++){
      int id = strip_ids[i];
      if(fadc_peak[id]>temp_max_value){
         temp_max_strip = id;
         temp_max_value = fadc_peak[id];
      }
      temp_num_hit++;
      temp_cc += fadc_peak[id];
      //temp_cog += fadc_peak[id]*id;
      temp_cog += fadc_peak[id]*GetPosition(id);
      hit.PushBackStrip(GetPosition(id),fadc_tdc[id]*drift_velocity-drift_gap_center,fadc_peak[id],fadc_tdc[id]);
   }
   temp_cog /= temp_cc;
   //temp_cog *= strip_pitch;
   //temp_cog += position_start;
   //temp_cog *= inverted;

   hit.SetMaxStrip(temp_max_strip);
   hit.SetMaxValue(temp_max_value);
   hit.SetNumHit(temp_num_hit);
   hit.SetClusterCharge(temp_cc);
   hit.SetCogHit(temp_cog);
}

void E16ANA_GTRStripAnalyzer::SetArraysForTdcMethods(const std::vector<int> &strip_ids, std::vector<double> &x_array, std::vector<double> &time_array, std::vector<double> &peak_array){
  //   std::cerr<<std::endl<<"fadctime/peak ";
   for(int i=0; i<(int)strip_ids.size(); i++){
      //if(gem_peak_x[i]>gem_th_x && gem_t0_x[i]<800){
      //if(gem_peak_x[i]>gem_th_x && gem_t0_x[i]>0.0 && gem_t0_x[i]<gem_t0_max){
      int id = strip_ids[i];
      if(fadc_peak[id]>gem_threshold && fadc_tdc[id]>0.0){
         //x_array.push_back(((double)id*strip_pitch+position_start)*inverted);
         x_array.push_back(GetPosition(id));
         time_array.push_back(fadc_tdc[id]*drift_velocity);
         peak_array.push_back(fadc_peak[id]);
         //std::cerr<<id<<" "<<fadc_tdc[id]<<" "<<fadc_peak_time[id]<<" "<<drift_velocity<<" ";
      }
   }
}

void E16ANA_GTRStripAnalyzer::CalcTdcHit1(const std::vector<int> &strip_ids, E16ANA_GTRAnalyzedStripHit &hit, int hitID ){
   std::vector<double> x_array;
   std::vector<double> time_array;
   std::vector<double> peak_array;

   //int temp_max_strip = -1;
   //double temp_max_value = -255.0;
   int temp_num_hit = 0;
   double temp_cc = 0.0;
   double temp_tdc_hit = 0.0;
   double temp_tan_theta = 0.0;

   if((int)strip_ids.size()==0){
      hit.SetInvalid();
      return;
   }

   SetArraysForTdcMethods(strip_ids, x_array, time_array, peak_array);

   temp_num_hit = x_array.size();
   temp_cc = 0.0;

   double B= 0.0;
   double C= 0.0;
   double D= 0.0;
   double E= 0.0;


   for(int i=0; i<(int)x_array.size(); i++){
      B+= x_array[i]*x_array[i];
      C+= time_array[i];
      D+= x_array[i]*time_array[i];
      E+= x_array[i];
      temp_cc += peak_array[i];
      //      std::cerr<<time_array[i]<<" ";
      //hit.PushBackStrip(x_array[i], peak_array[i], time_array[i]);
   }

   double dtdx = (x_array.size()*D-C*E)/(x_array.size()*B-E*E);
   double t0 = (B*C-D*E)/(x_array.size()*B-E*E);
   temp_tan_theta= 1.0/dtdx;
   temp_tdc_hit= (drift_gap_center-t0)/dtdx;

   //hit.SetMaxStrip(temp_max_strip);
   //hit.SetMaxValue(temp_max_value);
   hit.SetNumHit(temp_num_hit);
   hit.SetClusterCharge(temp_cc);
   hit.SetTdcHit(temp_tdc_hit);
   hit.SetTdc1Hit(temp_tdc_hit);
   hit.SetTanTheta(temp_tan_theta);

   hit.SetID( hitID );

   double meantime = C/drift_velocity / x_array.size() ;//cluster mean
   hit.SetTiming( meantime );
   hit.SetMeanZ(meantime*drift_velocity-drift_gap_center);
   //   std::cerr<<"t0/mean "<<t0<<" "<<meantime<<std::endl;
   //   t0 is bad
}

void E16ANA_GTRStripAnalyzer::CalcTdcHit2(const std::vector<int> &strip_ids, double tan_theta, E16ANA_GTRAnalyzedStripHit &hit){
   std::vector<double> x_array;
   std::vector<double> time_array;
   std::vector<double> peak_array;

   //int temp_max_strip = -1;
   //double temp_max_value = -255.0;
   int temp_num_hit = 0;
   double temp_cc = 0.0;
   double temp_tdc_hit = 0.0;

   if((int)strip_ids.size()==0){
      hit.SetInvalid();
      return;
   }

   SetArraysForTdcMethods(strip_ids, x_array, time_array, peak_array);

   temp_num_hit = x_array.size();
   temp_cc = 0.0;

   double A= 0.0;
   for(int i=0; i<(int)x_array.size(); i++){
      A += x_array[i]-tan_theta*time_array[i];
      temp_cc += peak_array[i];
   }
   double tdc_x0= A/x_array.size();
   temp_tdc_hit = tan_theta*drift_gap_center+tdc_x0;

   //hit.SetMaxStrip(temp_max_strip);
   //hit.SetMaxValue(temp_max_value);
   hit.SetNumHit(temp_num_hit);
   hit.SetClusterCharge(temp_cc);
   hit.SetTdcHit(temp_tdc_hit);
   hit.SetTdc2Hit(temp_tdc_hit);
   hit.SetTanTheta(tan_theta);

   //   hit.SetTiming( 0 );
   //std::cerr<<"t02 "<<std::endl;
}

/*----------------- Analyze version 2 -----------------*/
void E16ANA_GTRStripAnalyzer::AnalyzeV2(){
   if(!wf1d_fitter){
      std::cout << "E16ANA_GTRStripAnalyzer::AnalyzeV2() : Error. E16ANA_WaveformFitter is not initialized." << std::endl;
      return;
   }
   int n_hits = HitClusteringV2(2,50.0);
   //std::cout << "E16ANA_GTRStripAnalyzer::AnalyzeV2 : multiplicity = " << n_hits << std::endl;
   gem_analyzed_hits.resize(n_hits);
   for(int i=0; i<n_hits; i++){
      //std::cout << "cid = " << i << ", n_signal = " << clustered_x[i].size() << std::endl;
      CalcTdcHit3(false, clustered_x[i], clustered_z[i], clustered_peak[i], gem_analyzed_hits[i], i);
   }
}

void E16ANA_GTRStripAnalyzer::CalcWaveParamsFit(std::vector<fit_params_t> &fit_pars_array){
   fit_pars_array.clear();
   for(int i=0; i<n_strips; i++){
   //for(int i=left_strip_x; i<right_strip_x; i++){
      if(fadc[i][0]<-255.0) continue;
      fit_pars_array.push_back(fit_params_t(i,5));
      this->CalcWaveParamsFit(i, fit_pars_array.back());
   }
}

void E16ANA_GTRStripAnalyzer::CalcWaveParamsFit(int ch, fit_params_t &fit_pars){
   wf1d_fitter->SetWaveform(fadc[ch], n_sampling);
   wf1d_fitter->SetNumWaveforms(3);
   wf1d_fitter->Fit();
   const double merge_time = 30.0;
   double time = wf1d_fitter->GetWaveformTime(0);
   double peak = wf1d_fitter->GetWaveformPeak(0);
   double pre_time = time;
   for(int i=1; i<wf1d_fitter->GetNumWaveforms(); i++){
      double temp_time = wf1d_fitter->GetWaveformTime(i);
      double temp_peak = wf1d_fitter->GetWaveformPeak(i);
      if(temp_time > (pre_time+merge_time)){
         time /= peak;
         if(peak>25.0 && time>0.0 && time<750.0){
            //std::cout << "StripID = " << ch << ", Time = " << time << ", Peak = " << peak << std::endl;
            fit_pars.times.push_back(time);
            fit_pars.peaks.push_back(peak);
         }
         time = 0.0;
         peak = 0.0;
         pre_time = temp_time;
      }
      peak += temp_peak;
      time += temp_peak*temp_time; // cog merge
   }
   if(peak>25.0 && time>0.0 && time<750.0){
      time /= peak;
      //std::cout << "StripID = " << ch << ", Time = " << time << ", Peak = " << peak << std::endl;
      fit_pars.times.push_back(time+fadc_t0_correction);
      fit_pars.peaks.push_back(peak);
   }
}

int E16ANA_GTRStripAnalyzer::HitClusteringV2(const int min_gap, const double delta_tdc_threshold){
   std::vector<fit_params_t> fit_pars;
   CalcWaveParamsFit(fit_pars);
   clustered_x.clear();
   clustered_z.clear();
   clustered_peak.clear();

   std::vector<fit_params_t>::iterator it = fit_pars.begin();

   double pre_tdc = -10000.0;
   int pre_strip_id = -100;
   while(!fit_pars.empty()){
      if(it == fit_pars.end()){
         pre_tdc = -10000.0;
         pre_strip_id = -100;
         it = fit_pars.begin();
      }
      int signal_gap = (*it).strip_id-pre_strip_id;
      int min_j = -1;
      double min_delta_tdc = 1000000.0;
      for(int j=0; j<(int)(*it).peaks.size(); j++){
         if(fabs((*it).times[j]-pre_tdc)<min_delta_tdc){
            min_j = j;
            min_delta_tdc = fabs((*it).times[j]-pre_tdc);
         }
      }
      //std::cout << "min_j = " << min_j << ", delta_tdc = " << min_delta_tdc << std::endl;
      if(min_j < 0){
         it = fit_pars.erase(it);
         continue;
      }
      double delta_tdc = min_delta_tdc;
      if(signal_gap>min_gap){
         clustered_x.push_back(std::vector<double>());
         clustered_z.push_back(std::vector<double>());
         clustered_peak.push_back(std::vector<double>());
         //pre_tdc = (*it).times[min_j];
         //pre_strip_id = (*it).strip_id;
         //clustered_x.back().push_back(GetPosition(pre_strip_id));
         //clustered_z.back().push_back(pre_tdc*drift_velocity-drift_gap_center);
         //clustered_peak.back().push_back((*it).peaks[min_j]);
         //(*it).times.erase((*it).times.begin()+min_j);
         //(*it).peaks.erase((*it).peaks.begin()+min_j);
      }
      //if(signal_gap<=min_gap && fabs(delta_tdc)<delta_tdc_threshold){ // V1
      if(signal_gap>min_gap || fabs(delta_tdc)<delta_tdc_threshold){
         pre_tdc = (*it).times[min_j];
         pre_strip_id = (*it).strip_id;
         //std::cout << "cid = " << clustered_x.size()-1 << ", t = " << pre_tdc
         //   << ", sid = " << pre_strip_id
         //   << ", peak = " << (*it).peaks[min_j] << std::endl;
         clustered_x.back().push_back(GetPosition(pre_strip_id));
         clustered_z.back().push_back(pre_tdc*drift_velocity-drift_gap_center_v3);
         clustered_peak.back().push_back((*it).peaks[min_j]);
         (*it).times.erase((*it).times.begin()+min_j);
         (*it).peaks.erase((*it).peaks.begin()+min_j);
      }
      it++;
   }
   return clustered_x.size();
}

void E16ANA_GTRStripAnalyzer::AnalyzeV2(int id){
   CalcTdcHit3(true, clustered_x[id], clustered_z[id], clustered_peak[id], gem_analyzed_hits[id], id);
}

/*----------------- Analyze version 2 -----------------*/


/*----------------- Analyze version 3 -----------------*/

void E16ANA_GTRStripAnalyzer::AnalyzeV3(){
   if(wf2d_fitter == NULL){
      std::cout << "E16ANA_GTRStripAnalyzer::AnalyzeV3() : Error. E16ANA_Waveform2dFitter is not initialized." << std::endl;
      return;
   }
   clustered_x.clear();
   clustered_z.clear();
   clustered_peak.clear();
   std::vector<cluster_param_t> v1;
   CalcClusterParams(v1, GetPosition(-1), GetPosition(n_strips));
   //int n_hits = HitClusteringV3(clustered_x, clustered_z, clustered_peak, GetPosition(-1), GetPosition(n_strips));
   int n_hits = HitClusteringV3(v1, clustered_x, clustered_z, clustered_peak);
   //std::cout << "E16ANA_GTRStripAnalyzer::AnalyzeV3 : multiplicity = " << n_hits << std::endl;
   gem_analyzed_hits.resize(n_hits);
   for(int i=0; i<n_hits; i++){
      gem_analyzed_hits[i].SetInvalid();
      CalcTdcHit3(false, clustered_x[i], clustered_z[i], clustered_peak[i], gem_analyzed_hits[i], i);
   }
}

void E16ANA_GTRStripAnalyzer::AnalyzeV3(double local_x){
   if(wf2d_fitter == NULL){
      std::cout << "E16ANA_GTRStripAnalyzer::AnalyzeV3() : Error. E16ANA_Waveform2dFitter is not initialized." << std::endl;
      return;
   }
   //double near_cut = 2.0; // mm
   double near_cut = 3.0; // mm
   int i_offset = clustered_x.size();
   std::vector<cluster_param_t> v1;
   CalcClusterParams(v1, local_x-near_cut, local_x+near_cut);
   //int n_hits = HitClusteringV3(clustered_x, clustered_z, clustered_peak,
   //      local_x-near_cut, local_x+near_cut);
   int n_hits = HitClusteringV3(v1, clustered_x, clustered_z, clustered_peak);
   //std::cout << "E16ANA_GTRStripAnalyzer::AnalyzeV3 "
   //   << ": multiplicity = " << n_hits
   //   << ", i_offset = " << i_offset
   //   << ", vector_size = " << clustered_x.size()
   //   << std::endl;
   //gem_analyzed_hits.resize(n_hits);
   for(int i=0; i<n_hits; i++){
      gem_analyzed_hits.push_back(E16ANA_GTRAnalyzedStripHit());
      CalcTdcHit3(false, clustered_x[i+i_offset], clustered_z[i+i_offset], clustered_peak[i+i_offset], gem_analyzed_hits.back(), i+i_offset);
      //std::cout << "    hit_id = " << gem_analyzed_hits.back().ID() << std::endl;
   }
}

//void E16ANA_GTRStripAnalyzer::CalcClusterParams(
//      std::vector<double> &v1_pos,
//      std::vector<double> &v1_time,
//      std::vector<double> &v1_peak
//      ){
void E16ANA_GTRStripAnalyzer::CalcClusterParams(std::vector<cluster_param_t> &v1,
      double analysis_cut_x0, double analysis_cut_x1){
   HitClusteringV0(1, 30.0, true); // Pre clustering
   //E16ANA_Waveform2dFitter *wf_fitter = new E16ANA_Waveform2dFitter();
   double x_min, x_max;
   //if(analysis_cut_x0*inverted < analysis_cut_x1*inverted){
   if(analysis_cut_x0 < analysis_cut_x1){
      x_min = analysis_cut_x0;
      x_max = analysis_cut_x1;
   }else{
      x_min = analysis_cut_x1;
      x_max = analysis_cut_x0;
   }
   for(int i=0; i<(int)clustered_strip_id.size(); i++){
      std::vector<int> &strips = clustered_strip_id[i];
      //if((GetPosition(strips[0])*inverted > x_max*inverted)
      //      || (GetPosition(strips.back())*inverted < x_min*inverted)){
      //   continue;
      //}
      wf2d_fitter->Clear();
      //wf2d_fitter->SetNumClusters(ceil((double)strips.size()/2.5));
      int noh = 0;
      for(int j=0; j<(int)strips.size(); j++){
         double pos = GetPosition(strips[j]);
         if(pos > x_min && pos < x_max){
            wf2d_fitter->AddWaveform(pos, fadc[strips[j]], n_sampling);
            noh++;
         }
      }
      if(noh < 1) continue;
      wf2d_fitter->SetNumClusters(ceil((double)noh/1.5));
      //wf2d_fitter->SetNumClusters(ceil((double)noh));
      wf2d_fitter->Fit();
      for(int n=0; n<wf2d_fitter->GetNumClusters(); n++){
         //v1_pos.push_back(wf2d_fitter->GetClusterPos(n));
         //v1_time.push_back(wf2d_fitter->GetClusterTime(n)+fadc_t0_correction);
         //v1_peak.push_back(wf2d_fitter->GetClusterPeak(n));
         v1.push_back(cluster_param_t(
                  wf2d_fitter->GetClusterPos(n),
                  wf2d_fitter->GetClusterTime(n)+fadc_t0_correction,
                  wf2d_fitter->GetClusterPeak(n)
                  ));
      }
   }
   //delete wf_fitter;
}

int E16ANA_GTRStripAnalyzer::HitClusteringV3(
      std::vector<cluster_param_t> &v1, // sorted by pos
      std::vector<std::vector<double> > &v2_x,
      std::vector<std::vector<double> > &v2_z,
      std::vector<std::vector<double> > &v2_peak,
      double cluster_min_distance
      //double analysis_cut_x0, double analysis_cut_x1
      ){
   int ret = 0;
   const double cluster_peak_threshold = 10.0;
   //const double cluster_min_distance = 2.0; // mm
   //const double cluster_min_distance = 1.0; // mm
   //const double cluster_min_distance = 0.5; // mm

   //std::vector<double> v1_pos; // sorted by pos
   //std::vector<double> v1_time; // sorted by pos
   //std::vector<double> v1_peak; // sorted by pos
   //CalcClusterParams(v1_pos, v1_time, v1_peak);
   //std::vector<cluster_param_t> v1; // sorted by pos
   //CalcClusterParams(v1, analysis_cut_x0, analysis_cut_x1);

   if((int)v1.size() == 0){
      return 0;
   }

   double pre_x = -10000.0;
   double pre_z = -10000.0;
   double dist = 10000.0;
   double min_distance = 1000.0;
   std::vector<cluster_param_t>::iterator it = v1.begin();
   while(it != v1.end()){
      //std::cout << "v1.size() = " << v1.size() << std::endl;
      if((*it).peak < cluster_peak_threshold){
         it = v1.erase(it);
         continue;
      }
      if(min_distance > cluster_min_distance){
         it = v1.begin();
         v2_x.push_back(std::vector<double>());
         v2_z.push_back(std::vector<double>());
         v2_peak.push_back(std::vector<double>());
         ret++;
      }
      pre_x = (*it).pos;
      pre_z = ((*it).time)*drift_velocity-drift_gap_center_v3;
      v2_x.back().push_back(pre_x);
      v2_z.back().push_back(pre_z);
      v2_peak.back().push_back((*it).peak);
      it = v1.erase(it);

      min_distance = 1000.0;
      //std::vector<cluster_param_t>::iterator min_it;
      std::vector<cluster_param_t>::iterator it_temp = v1.begin();
      while(it_temp != v1.end()){
         double cur_x = (*it_temp).pos;
         double cur_z = ((*it_temp).time)*drift_velocity-drift_gap_center_v3;
         dist = sqrt((pre_x-cur_x)*(pre_x-cur_x)+(pre_z-cur_z)*(pre_z-cur_z));
         if((cur_x-pre_x) > cluster_min_distance) break;
         if(dist < min_distance){
            //min_it = it_temp;
            it = it_temp;
            min_distance = dist;
         }
         it_temp++;
      }
      //std::cout << "distance = " << min_distance << " [mm]" << std::endl;
   }
   // fiducial cut
   //double fiducial_min = -2.0;
   //double fiducial_max =  2.0;
   double fiducial_min = -20.0;
   double fiducial_max =  20.0;
   std::vector<std::vector<double> >::iterator it_z = v2_z.begin();
   std::vector<std::vector<double> >::iterator it_x = v2_x.begin();
   std::vector<std::vector<double> >::iterator it_peak = v2_peak.begin();
   while(it_z != v2_z.end()){
      double min_z =  1000.0;
      double max_z = -1000.0;
      for(int i=0; i<(int)it_z->size(); i++){
         if(min_z > it_z->at(i)) min_z = it_z->at(i);
         if(max_z < it_z->at(i)) max_z = it_z->at(i);
      }
      if(min_z < fiducial_min || max_z > fiducial_max){
         it_z = v2_z.erase(it_z);
         it_x = v2_x.erase(it_x);
         it_peak = v2_peak.erase(it_peak);
         ret--;
      }else{
         it_z++;
         it_x++;
         it_peak++;
      }
   }
   //return v2_x.size();
   return ret;
}

void E16ANA_GTRStripAnalyzer::CalcTdcHit3(
      bool fix_flag,
      const std::vector<double> &x,
      const std::vector<double> &z,
      const std::vector<double> &peak,
      E16ANA_GTRAnalyzedStripHit &hit, int hitID){

   //int temp_max_strip = -1;
   //double temp_max_value = -255.0;
   int temp_num_hit = 0;
   double temp_cc = 0.0;
   double temp_tdc_hit = 0.0;
   double temp_tan_theta = 0.0;

   if((int)x.size()==0){
      hit.SetInvalid();
      return;
   }

   temp_num_hit = x.size();
   temp_cc = 0.0;

   //if(fabs(hit.TanTheta()) > 2.0){
   if(!fix_flag){
      double B= 0.0;
      double C= 0.0;
      double D= 0.0;
      double E= 0.0;
      double mean_z = 0.0;

      for(int i=0; i<(int)x.size(); i++){
         B+= x[i]*x[i];
         C+= z[i];
         D+= x[i]*z[i];
         E+= x[i];
         temp_cc += peak[i];
         mean_z += z[i];
         //std::cout << "(x, z, p) = (" << x[i] << ", " << z[i] << ", " << peak[i] << ")" << std::endl;
         hit.PushBackStrip(x[i], z[i], peak[i], (z[i]+drift_gap_center_v3)/drift_velocity);
      }
      //std::cout << std::endl;

      if((int)x.size() > 1){
         double dzdx = (x.size()*D-C*E)/(x.size()*B-E*E);
         double z0 = (B*C-D*E)/(x.size()*B-E*E);
         temp_tan_theta= 1.0/dzdx;
         //temp_tdc_hit= (drift_gap_center-z0)/dzdx;
         temp_tdc_hit= -z0/dzdx;
      }else if((int)x.size() == 1){
         temp_tan_theta = 0.0;
         temp_tdc_hit = E;
      }
      mean_z /= (double)x.size();
      hit.SetMeanZ(mean_z);
      hit.SetTiming((mean_z+drift_gap_center_v3)/drift_velocity);
   }else{
      double A= 0.0;
      for(int i=0; i<(int)x.size(); i++){
         A += x[i]-hit.TanTheta()*z[i];
         temp_cc += peak[i];
      }
      double tdc_x0 = A/x.size();
      //temp_tdc_hit = hit.TanTheta()*drift_gap_center+tdc_x0;
      temp_tdc_hit = tdc_x0;
   }

   double temp_cog = 0.0;
   for(int i=0; i<(int)x.size(); i++){
      //temp_cog += x[i]/(double)x.size();
      temp_cog += x[i]*peak[i]/temp_cc;
   }

   hit.SetCogHit(temp_cog);
   hit.SetNumHit(temp_num_hit);
   hit.SetClusterCharge(temp_cc);
   hit.SetTdcHit(temp_tdc_hit);
   if(!fix_flag){
      hit.SetTdc1Hit(temp_tdc_hit);
   }else{
      hit.SetTdc2Hit(temp_tdc_hit);
   }
   hit.SetTanTheta(temp_tan_theta);
   hit.SetID(hitID);

}

void E16ANA_GTRStripAnalyzer::AnalyzeV3(int id){
   //std::cout << "E16ANA_GTRStripAnalyzer::AnalyzeV3 "
   //   << ": id = " << id
   //   << ", vector_size = " << clustered_x.size()
   //   << std::endl;
   CalcTdcHit3(true, clustered_x[id], clustered_z[id], clustered_peak[id], gem_analyzed_hits[id], id);
}

/*----------------- Analyze version 3 -----------------*/

/*----------------- Analyze version 4 -----------------*/

void E16ANA_GTRStripAnalyzer::AnalyzeV4(double local_x, double tan_theta){
   if(wf2d_fitter == NULL){
      std::cout << "E16ANA_GTRStripAnalyzer::AnalyzeV4() : Error. E16ANA_Waveform2dFitter is not initialized." << std::endl;
      return;
   }
   double near_cut = 3.0; // mm
   std::vector<cluster_param_t> v1;
   CalcClusterParams(v1, local_x-near_cut, local_x+near_cut);
   // pre clustering
   std::vector<std::vector<double> > v2_temp_x, v2_temp_z, v2_temp_peak;
   int n_hits = HitClusteringV3(v1, v2_temp_x, v2_temp_z, v2_temp_peak, 0.3);
   std::vector<cluster_v4_t> v4_array;
   // 2D -> 1D array
   for(int i=0; i<n_hits; i++){
      int n_clusters = v2_temp_x[i].size();
      for(int j=0; j<n_clusters; j++){
         v4_array.push_back(cluster_v4_t(v2_temp_x[i][j], v2_temp_z[i][j], v2_temp_peak[i][j], tan_theta));
         //v4_array.push_back(cluster_v4_t(v2_temp_x[i][j]-local_x, v2_temp_z[i][j], v2_temp_peak[i][j], tan_theta));
      }
   }
   // main clustering
   int i_offset = clustered_x.size();
   n_hits = HitClusteringV4(v4_array, clustered_x, clustered_z, clustered_peak, 0.3);
   std::cout << "E16ANA_GTRStripAnalyzer::AnalyzeV4 : n_hits = " << n_hits << std::endl;
   for(int i=0; i<n_hits; i++){
      std::cout << "    i = " << i << std::endl;
      for(int j=0; j<(int)clustered_x[i].size(); j++){
         std::cout << "        j = " << j << " : (x, z) = (" << clustered_x[i][j] << ", " << clustered_z[i][j] << ")" << std::endl;
      }
      gem_analyzed_hits.push_back(E16ANA_GTRAnalyzedStripHit());
      CalcTdcHit3(false, clustered_x[i+i_offset], clustered_z[i+i_offset], clustered_peak[i+i_offset], gem_analyzed_hits.back(), i+i_offset);
      gem_analyzed_hits.back().SetTanTheta(tan_theta);
      CalcTdcHit3(true, clustered_x[i+i_offset], clustered_z[i+i_offset], clustered_peak[i+i_offset], gem_analyzed_hits.back(), i+i_offset);
      //std::cout << "    hit_id = " << gem_analyzed_hits.back().ID() << std::endl;
   }
}

int E16ANA_GTRStripAnalyzer::HitClusteringV4(std::vector<cluster_v4_t> &v1,
      std::vector<std::vector<double> > &v2_x,
      std::vector<std::vector<double> > &v2_z,
      std::vector<std::vector<double> > &v2_peak,
      double cluster_cut_v4 // mm
      ){
   //std::cout << "Before sorting" << std::endl;
   //for(int i=0; i<(int)v1.size(); i++){
   //   std::cout << "    i = " << i << " : "; v1[i].Print();
   //}
   std::sort(v1.begin(), v1.end(), LessClusterV4U()); // u-sort
   //std::cout << "After sorting" << std::endl;
   //for(int i=0; i<(int)v1.size(); i++){
   //   std::cout << "    i = " << i << " : "; v1[i].Print();
   //}
   double pre_u = -10000.0;
   int n_hits = 0;
   for(int i=0; i<(int)v1.size(); i++){
      if((v1[i].u - pre_u) > cluster_cut_v4){
         v2_x.push_back(std::vector<double>());
         v2_z.push_back(std::vector<double>());
         v2_peak.push_back(std::vector<double>());
         n_hits++;
      }
      v2_x.back().push_back(v1[i].x);
      v2_z.back().push_back(v1[i].z);
      v2_peak.back().push_back(v1[i].peak);
      pre_u = v1[i].u;
   }
   return n_hits;
}

std::vector<E16ANA_GTRAnalyzedStripHit> E16ANA_GTRStripAnalyzer::ReAnalyzeV4(double local_x, double tan_theta, double near_cut){
   std::vector<E16ANA_GTRAnalyzedStripHit> ret_array;
   if(wf2d_fitter == NULL){
      std::cout << "E16ANA_GTRStripAnalyzer::AnalyzeV4() : Error. E16ANA_Waveform2dFitter is not initialized." << std::endl;
      return ret_array;
   }
   //double near_cut = 5.0; // mm
   std::vector<cluster_param_t> v1;
   CalcClusterParams(v1, local_x-near_cut, local_x+near_cut);
   // pre clustering
   std::vector<std::vector<double> > v2_temp_x, v2_temp_z, v2_temp_peak;
   int n_hits = HitClusteringV3(v1, v2_temp_x, v2_temp_z, v2_temp_peak, 0.3);
   std::vector<cluster_v4_t> v4_array;
   // 2D -> 1D array
   for(int i=0; i<n_hits; i++){
      int n_clusters = v2_temp_x[i].size();
      for(int j=0; j<n_clusters; j++){
         v4_array.push_back(cluster_v4_t(v2_temp_x[i][j], v2_temp_z[i][j], v2_temp_peak[i][j], tan_theta));
         //v4_array.push_back(cluster_v4_t(v2_temp_x[i][j]-local_x, v2_temp_z[i][j], v2_temp_peak[i][j], tan_theta));
      }
   }
   // main clustering
   int i_offset = clustered_x.size();
   n_hits = HitClusteringV4(v4_array, clustered_x, clustered_z, clustered_peak, 0.3);
   //std::cout << "E16ANA_GTRStripAnalyzer::AnalyzeV4 : n_hits = " << n_hits << std::endl;
   for(int i=0; i<n_hits; i++){
      //std::cout << "    i = " << i << std::endl;
      //for(int j=0; j<(int)clustered_x[i].size(); j++){
      //   std::cout << "        j = " << j << " : (x, z) = (" << clustered_x[i][j] << ", " << clustered_z[i][j] << ")" << std::endl;
      //}
      ret_array.push_back(E16ANA_GTRAnalyzedStripHit());
      CalcTdcHit3(false, clustered_x[i+i_offset], clustered_z[i+i_offset], clustered_peak[i+i_offset], ret_array.back(), i+i_offset);
      ret_array.back().SetTanTheta(tan_theta);
      CalcTdcHit3(true, clustered_x[i+i_offset], clustered_z[i+i_offset], clustered_peak[i+i_offset], ret_array.back(), i+i_offset);
      //std::cout << "    hit_id = " << ret_array.back().ID() << std::endl;
   }
   return ret_array;
}

/*----------------- Analyze version 4 -----------------*/

