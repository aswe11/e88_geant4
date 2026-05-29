//2016-11-22, uploaded by nakai
//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai
//2015-11-14, uploaded by nakai
//2015-11-02, uploaded by yokkaich
//2015-11-02, uploaded by komatsu
//2015-07-15, uploaded by nakai
//2015-03-27, uploaded by yokkaich
//2015-03-20, uploaded by nakai
#include <cmath>
#include <iostream>

#include <TMath.h>
#include <TF1.h>
#include <TVector2.h>
#include <TVector3.h>
#include <TRandom3.h>

#include "E16ANA_GTRStripResponse.hh"
#include "E16ANA_WaveformTemplate.hh"

const double E16ANA_GTRStripResponse::clock_period = 25.0; // 40 MHz
const double E16ANA_GTRStripResponse::threshold = 1.0;
std::vector<std::vector<double> > E16ANA_GTRStripResponse::current_signal;
std::vector<double> E16ANA_GTRStripResponse::current_integral;

E16ANA_GTRStripResponse::E16ANA_GTRStripResponse(int n_strips_) : 
   n_strips(n_strips_),
   strip_pitch(0.35), drift_gap(3.0),
   drift_velocity(0.01), diffusion_t(0.5), diffusion_l(0.1), rise_time(20.0), preamp_noise(5.0), t_offset(25.0),
   boundary_y0(-300.0), boundary_y1(300.0)
{
   x_start = -(double)n_strips/2.0*strip_pitch+strip_pitch*0.5;
//#ifndef StripResponse_WorkSpace_Static
#if 0
   current_signal = new double*[n_strips];
   for(int i=0; i<n_strips; i++){
      current_signal[i] = new double[n_isignal];
   }
   current_integral = new double[n_strips];
#else
   current_signal.resize(n_strips, std::vector<double>(n_isignal, 0.0));
   current_integral.resize(n_strips, 0.0);
#endif
   //n_hit_strips_max = n_strips/3;
   //wf = new int*[n_hit_strips_max];
   //for(int i=0; i<n_hit_strips_max; i++){
      //wf[i] = new int[n_sampling];
   //}
   //wf_channel = new int[n_hit_strips_max];
   //n_hit_strips = n_hit_strips_max;
   n_hit_strips = 0;
   Clear();
}

void E16ANA_GTRStripResponse::SetStripPitch(double strip_pitch_){
   strip_pitch = strip_pitch_;
   x_start = -(double)n_strips/2.0*strip_pitch+strip_pitch*0.5;
}

void E16ANA_GTRStripResponse::SetWaveformTemplate(E16ANA_WaveformTemplate *wf_temp_){
   wf_temp = wf_temp_;
   current_start = 0.0-wf_temp->GetTimeEnd();
   current_end = clock_period*n_sampling-wf_temp->GetTimeStart();
   current_dt = (current_end-current_start)/(double)(n_isignal-1);
}

E16ANA_GTRStripResponse::E16ANA_GTRStripResponse(int n_strips_, E16ANA_WaveformTemplate *wf_temp_) : 
//E16ANA_GTRStripResponse::E16ANA_GTRStripResponse(int n_strips_, E16ANA_WaveformTemplate *wf_temp_, TRandom3 *rand_) : 
   //n_strips(n_strips_), wf_temp(wf_temp_), rand(rand_),
   n_strips(n_strips_), wf_temp(wf_temp_),
   strip_pitch(0.35), drift_gap(3.0),
   drift_velocity(0.01), diffusion_t(0.5), diffusion_l(0.2), rise_time(20.0), preamp_noise(5.0), t_offset(25.0),
   boundary_y0(-300.0), boundary_y1(300.0)
{
   x_start = -(double)n_strips/2.0*strip_pitch+strip_pitch*0.5;
//#ifndef StripResponse_WorkSpace_Static
#if 0
   current_signal = new double*[n_strips];
   for(int i=0; i<n_strips; i++){
      current_signal[i] = new double[n_isignal];
   }
   current_integral = new double[n_strips];
#else
   current_signal.resize(n_strips, std::vector<double>(n_isignal, 0.0));
   current_integral.resize(n_strips, 0.0);
#endif
   current_start = 0.0-wf_temp->GetTimeEnd();
   current_end = clock_period*n_sampling-wf_temp->GetTimeStart();
   current_dt = (current_end-current_start)/(double)(n_isignal-1);
   //n_hit_strips_max = n_strips/3;
   //wf = new int*[n_hit_strips_max];
   //for(int i=0; i<n_hit_strips_max; i++){
      //wf[i] = new int[n_sampling];
   //}
   //wf_channel = new int[n_hit_strips_max];
   //n_hit_strips = n_hit_strips_max;
   n_hit_strips = 0;
   Clear();
}

E16ANA_GTRStripResponse::~E16ANA_GTRStripResponse(){
//#ifndef StripResponse_WorkSpace_Static
#if 0
   for(int i=0; i<n_strips; i++){
      delete [] current_signal[i];
   }
   delete [] current_signal;
   delete [] current_integral;
#endif
   //for(int i=0; i<n_hit_strips_max; i++){
      //delete [] wf[i];
   //}
   //delete [] wf;
   //delete [] wf_channel;
}

void E16ANA_GTRStripResponse::PrintParameters(){
   std::cout << "E16ANA_GTRStripResponse::PrintParameters : "
      << "drift_velocity = " << drift_velocity << " [mm/ns]"
      << ", strip_pitch = " << strip_pitch << " [mm]"
      << ", diffusion_t = " << diffusion_t << " [mm]"
      << ", diffusion_l = " << diffusion_l << " [mm]"
      << ", preamp_noise = " << preamp_noise
      << ", t_offset = " << t_offset << " [ns]" << std::endl;
}



void E16ANA_GTRStripResponse::SetTracks(
            const std::vector<TVector2> &track_start,
            const std::vector<TVector2> &track_end,
            const std::vector<double> &t,
            const std::vector<double> &charge){
   Clear();
   for(int i=0; i<(int)t.size(); i++){
      AddCurrentSignals(track_start[i], track_end[i], t[i], charge[i]);
   }

   CalcConvolution();
   ZeroSuppress();
   return;
}

void E16ANA_GTRStripResponse::SetClusters(
            const std::vector<TVector3> &cluster_pos,
            const std::vector<double> &t,
            const std::vector<double> &charge){
   Clear();
   double cut_nsigma = 8.0;
   for(int i=0; i<(int)t.size(); i++){
      double y = cluster_pos[i].Y();
      if(y>(boundary_y0+diffusion_t*cut_nsigma) && y<(boundary_y1-diffusion_t*cut_nsigma)){
         //std::cout << "E16ANA_GTRStripResponse::AddClusterCharge(), charge = " << charge[i] << std::endl;
         AddClusterCharge(cluster_pos[i].X(), cluster_pos[i].Z(), t[i], charge[i]);
      }else if(y<(boundary_y0-diffusion_t*cut_nsigma) || y>(boundary_y1+diffusion_t*cut_nsigma)){
         continue;
      }else if(y>=(boundary_y0-diffusion_t*cut_nsigma) && y<=(boundary_y0+diffusion_t*cut_nsigma)){

         //TF1 f_gaus("f_gaus", "gaus", y-diffusion_t*cut_nsigma, y+diffusion_t*cut_nsigma);
         //f_gaus.SetParameter(0, 1.0/sqrt(2.0*TMath::Pi())/diffusion_t);// normalize
         //f_gaus.SetParameter(1, y);// mean
         //f_gaus.SetParameter(2, diffusion_t);// sigma
         //double charge_frac = f_gaus.Integral(boundary_y0, y+diffusion_t*cut_nsigma);

         double charge_frac = GaussIntegralErf(boundary_y0, y+diffusion_t*cut_nsigma, y, diffusion_t);
         //std::cout << "charge_frac = " << charge_frac << std::endl;
         AddClusterCharge(cluster_pos[i].X(), cluster_pos[i].Z(), t[i], charge[i]*charge_frac);
      }else if(y>=(boundary_y1-diffusion_t*cut_nsigma) && y<=(boundary_y1+diffusion_t*cut_nsigma)){

         //TF1 f_gaus("f_gaus", "gaus", y-diffusion_t*cut_nsigma, y+diffusion_t*cut_nsigma);
         //f_gaus.SetParameter(0, 1.0/sqrt(2.0*TMath::Pi())/diffusion_t);// normalize
         //f_gaus.SetParameter(1, y);// mean
         //f_gaus.SetParameter(2, diffusion_t);// sigma
         //double charge_frac = f_gaus.Integral(y-diffusion_t*cut_nsigma, boundary_y1);

         double charge_frac = GaussIntegralErf(y-diffusion_t*cut_nsigma, boundary_y1, y, diffusion_t);
         //std::cout << "charge_frac = " << charge_frac << std::endl;
         AddClusterCharge(cluster_pos[i].X(), cluster_pos[i].Z(), t[i], charge[i]*charge_frac);
      }
   }

   CalcConvolution();
   ZeroSuppress();
   return;
}


void E16ANA_GTRStripResponse::Clear(){
//#ifndef StripResponse_WorkSpace_Static
#if 0
   for(int i=0; i<n_strips; i++){
      for(int j=0; j<n_isignal; j++){
         current_signal[i][j] = 0.0;
      }
   }
   for(int i=0; i<n_strips; i++){
      current_integral[i] = 0.0;
   }
#else
   current_signal.clear();
   current_integral.clear();
   current_signal.resize(n_strips, std::vector<double>(n_isignal, 0.0));
   current_integral.resize(n_strips, 0.0);
#endif
   //for(int i=0; i<n_hit_strips; i++){
   //   for(int j=0; j<n_sampling; j++){
   //      wf[i][j] = 0.0;
   //   }
   //}
   for(int i=0; i<(int)wf.size(); i++){
      wf[i].clear();
   }
   wf.clear();
   //for(int i=0; i<n_hit_strips; i++){
   //   wf_channel[i] = -1;
   //}
   wf_channel.clear();
   n_hit_strips = 0;
   //wf_ptr = 0;
}

void E16ANA_GTRStripResponse::CalcConvolution(){
   for(int i=0; i<n_strips; i++){
      if(current_integral[i]>threshold*preamp_noise){
         CalcConvolution(i);
      }
   }
}

void E16ANA_GTRStripResponse::CalcConvolution(int ch){
   //if(n_hit_strips>=n_hit_strips_max) return;
   double wf_start = wf_temp->GetTimeStart();
   double wf_end = wf_temp->GetTimeEnd();
   wf.push_back(std::vector<int>(n_sampling,0));
   wf_channel.push_back(ch);
   for(int j=0; j<n_sampling; j++){
      double t = j*clock_period;
      double k_start = GetSignalTimeBin(t-wf_end);
      double k_end = GetSignalTimeBin(t-wf_start);
      double wf_buf = 0.0;
      for(int k=k_start; k<=k_end; k++){
         double tp = GetSignalTime(k);
         wf_buf += wf_temp->GetValue(t-tp)*current_signal[ch][k]*current_dt;
         //wf[n_hit_strips][j] += wf_temp->GetValue(t-tp)*current_signal[ch][k]*current_dt;
      }
      //wf_buf += rand->Gaus(0.0, preamp_noise);// Add preamp noise
      wf_buf += gRandom->Gaus(0.0, preamp_noise);// Add preamp noise
      (wf.back())[j] = (int)wf_buf;
      //wf_channel[n_hit_strips] = ch;
   }
   n_hit_strips++;
   //std::cout << "E16ANA_GTRStripResponse::CalcConvolution(), ch = " << ch << ", charge = " << current_integral[ch] << std::endl;
}

int E16ANA_GTRStripResponse::GetSignalTimeBin(double t){
   if(t<(current_start-current_dt*0.5)){
      return -1;
   }else if(t>=(current_end+current_dt*0.5)){
      return n_isignal;
   }else{
      return (int)((t-current_start+current_dt*0.5)/current_dt);
   }
}

double E16ANA_GTRStripResponse::GetSignalTime(int bin){
   return bin*current_dt+current_start;
}

void E16ANA_GTRStripResponse::AddCurrentSignals(const TVector2 &track_start, const TVector2 &track_end, double t, double charge){
   //std::cout << "E16ANA_GTRStripResponse::AddCurrentSignals(), charge = " << charge << std::endl;
   double x0, z0, x1, z1;
   //if(track_start.X()<track_end.X()){
   if(track_start.Y()<track_end.Y()){
      x0 = track_start.X();
      z0 = track_start.Y();
      x1 = track_end.X();
      z1 = track_end.Y();
   }else{
      x1 = track_start.X();
      z1 = track_start.Y();
      x0 = track_end.X();
      z0 = track_end.Y();
   }

   //double x_center = 0.0;
   //double z_center = 0.0;
/*
   int i_start = (x0-x_start+0.5*strip_pitch-diffusion_t*4.0)/strip_pitch;
   int i_end = (x1-x_start+0.5*strip_pitch+diffusion_t*4.0)/strip_pitch;
   double charge_per_strip[n_strips];
   double total_charge = 0.0;
   //AddStripCurrent(0, t+(z0-z_center)*drift_velocity+t_offset, charge);

   for(int i=i_start; i<=i_end; i++){
      double x = i*strip_pitch+x_start;// coordinate of a center of GTR strip
      charge_per_strip[i] = EquallyDivide(x, x0, x1);
      total_charge += charge_per_strip[i];
   }
   for(int i=i_start; i<=i_end; i++){
      double t_start = (z0-z_center)*drift_velocity+t+t_offset;
      double t_end = (z0-z_center)*drift_velocity+t+t_offset;
      AddStripCurrent(i, t_start, t_end, charge_per_strip[i]*charge/total_charge);
   }
*/
/*
   int i_strip = 0;

   int k_start = GetSignalTimeBin(t+(z0-z_center)*drift_velocity+t_offset);
   int k_end = GetSignalTimeBin(t+(z0-z_center)*drift_velocity+t_offset+rise_time);
   for(int k=k_start; k<=k_end; k++){
      if(k>=n_isignal) break;
      current_signal[i_strip][k] += charge/(double)(k_end-k_start+1)/current_dt;
      current_integral[i_strip] += charge/(double)(k_end-k_start+1)/current_dt;
   }
   //std::cout << "x0 = " << x0
   //   << ", z0 = " << z0
   //   << ", x1 = " << x1
   //   << ", z1 = " << z1
   //   << ", t = " << t
   //   << ", charge = " << charge << std::endl;
*/
   double dz = 0.1;
   //double dz = 1.0;
   double cluster_x = x0;
   double cluster_z = z0;
   int n_div = (int)((z1-z0)/dz);
   //std::cout << "E16ANA_GTRStripResponse::AddCurrentSignals(), n_div = " << n_div << std::endl;
   double cluster_charge = charge/((double)n_div);
   for(int i=0; i<n_div; i++){
      AddClusterCharge(cluster_x, cluster_z, t, cluster_charge);
      cluster_z += dz;
      cluster_x += dz*(x1-x0)/(z1-z0);
   }

}

void E16ANA_GTRStripResponse::AddStripCurrent(int ch, double t_start, double t_end, double charge){
   int k_start = GetSignalTimeBin(t_start);
   int k_end = GetSignalTimeBin(t_end);
   for(int k=k_start; k<=k_end; k++){
      if(k<0) continue;
      if(k>=n_isignal) break;
      current_signal[ch][k] += charge/(double)(k_end-k_start+1)/current_dt;
      current_integral[ch] += charge/(double)(k_end-k_start+1)/current_dt;
   }
}

void E16ANA_GTRStripResponse::AddStripCurrentGaus(int ch, double t_center, double charge){
   double cut_nsigma = 4.0;
   int k_start = GetSignalTimeBin(t_center-diffusion_l/drift_velocity*cut_nsigma);
   int k_end = GetSignalTimeBin(t_center+diffusion_l/drift_velocity*cut_nsigma);
   for(int k=k_start; k<=k_end; k++){
      if(k<0) continue;
      if(k>=n_isignal) break;
      double buf = charge*TMath::Gaus(GetSignalTime(k), t_center, diffusion_l/drift_velocity, true);
      current_signal[ch][k] += buf;
      current_integral[ch] += buf;
   }
   //std::cout << "E16ANA_GTRStripResponse::AddStripCurrentGaus(), ch = " << ch << ", charge = " << charge << std::endl;
}


/*
void AddPreampNoise(int i){
   for(int j=0; j<n_sampling; j++){
      wf[ch][i] += rand->Gaus(0.0, preamp_noise);
   }
}
*/

double E16ANA_GTRStripResponse::EquallyDivide(double x, double x_left, double x_right){
   if(x>x_left && x<x_right){
      return 1.0;
   }else if(x<=x_left){
      return 1.0*exp((x-x_left)*(x-x_left)/2.0/diffusion_t/diffusion_t);
   }else if(x>=x_right){
      return 1.0*exp((x-x_right)*(x-x_right)/2.0/diffusion_t/diffusion_t);
   }
   return 0.0;
}

void E16ANA_GTRStripResponse::AddClusterCharge(double x, double z, double t, double charge){

   //std::cout << "E16ANA_GTRStripResponse::AddClusterCharge(), charge = " << charge << std::endl;
   double cut_nsigma = 8.0;
   int i_start = (x-x_start+0.5*strip_pitch-diffusion_t*cut_nsigma)/strip_pitch;
   int i_end = (x-x_start+0.5*strip_pitch+diffusion_t*cut_nsigma)/strip_pitch;
   if(i_start<0) i_start = 0;
   if(i_end>=n_strips) i_end = n_strips-1;

   for(int i=i_start; i<=i_end; i++){
      //double q = charge*TMath::Gaus(i*strip_pitch+x_start, x, diffusion_t, true)*strip_pitch;
      double x0 = (i-0.5)*strip_pitch+x_start;
      double x1 = (i+0.5)*strip_pitch+x_start;

      //TF1 f_gaus("f_gaus", "gaus", x0, x1);
      //f_gaus.SetParameter(0, 1.0/sqrt(2.0*TMath::Pi())/diffusion_t);// normalize
      //f_gaus.SetParameter(1, x);// mean
      //f_gaus.SetParameter(2, diffusion_t);// sigma
      //double q = charge*f_gaus.Integral(x0, x1);

      double q = charge*GaussIntegralErf(x0, x1, x, diffusion_t);
      //std::cout << "q = " << q << std::endl;

      // t smear ?
      //AddStripCurrentGaus(i, z/drift_velocity+t+t_offset, q);
      AddStripCurrent(i, // strip_id
            z/drift_velocity+t+t_offset-diffusion_l/2.0/drift_velocity, // t_start
            z/drift_velocity+t+t_offset+diffusion_l/2.0/drift_velocity, // t_end
            q); // charge
   }
}

double E16ANA_GTRStripResponse::GaussIntegralErf(double x1, double x2, double mean, double sigma){
   double acc1 = 0.5*(1.0+erf((x1-mean)/sqrt(2.0*sigma*sigma)));
   double acc2 = 0.5*(1.0+erf((x2-mean)/sqrt(2.0*sigma*sigma)));
   return acc2-acc1;
}

void E16ANA_GTRStripResponse::ZeroSuppress(double threshold_sigma){
   double wf_threshold = preamp_noise*threshold_sigma;
   std::vector<std::vector<int> >::iterator it = wf.begin();
   std::vector<int>::iterator it_ch = wf_channel.begin();
   while(it != wf.end()){
      int max = -10000;
      for(int j=0; j<(int)it->size(); j++){
         if(max < it->at(j)){
            max = it->at(j);
         }
      }
      if((double)max < wf_threshold){
         it = wf.erase(it);
         it_ch = wf_channel.erase(it_ch);
         n_hit_strips--;
      }else{
         it++;
         it_ch++;
      }
   }
}

