//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai
//2015-12-12, uploaded by nakai
//2015-11-02, uploaded by yokkaich
//2015-11-02, uploaded by komatsu
//2015-08-20, uploaded by yokkaich
//2015-08-14, uploaded by nakai
//2015-07-15, uploaded by nakai
//2015-05-29, uploaded by yokkaich
//2015-03-27, uploaded by yokkaich
//2015-03-20, uploaded by nakai

#include <iostream>
#include <fstream>
#include <cmath>
#include <map>
#include <string>
#include <sstream>
#include <iterator>

#include <TRandom.h>
#include <TH1D.h>
#include <TVector3.h>
#include <TRotation.h>
#include <TMath.h>

#include "E16ANA_ParamManager.hh"

#include "E16ANA_WaveformTemplate.hh"
#include "E16ANA_GTRResponse.hh"
#include "E16ANA_GTRStripResponse.hh"
#include "E16ANA_G4OutputData.hh"

int E16ANA_GTRResponse::n_instances = 0;

E16ANA_GTRResponse::E16ANA_GTRResponse() : 
   drift_gap(3.0), cluster_smear_t(0.0), cluster_smear_l(0.0), n_collisions_mean(3.0),
   gas_gain(8.0), gas_gain_sigma(0.1)
{
   //Initialize(fname);
  instance_id = n_instances;
   n_instances++;
}

E16ANA_GTRResponse::~E16ANA_GTRResponse(){
}

void E16ANA_GTRResponse::Initialize(const char *fname){
   std::cout << "E16ANA_GTRResponse::Initialize : filename = " << fname << std::endl;
   std::ifstream ifs(fname);

   if(ifs.fail()){
      std::cout << "\t ParamFile open fail" << std::endl;
      return;
   }

   std::map<std::string, std::string> parameter_list;
   while(ifs.good()){
      std::string buf;
      std::getline(ifs, buf);
      if(buf[0]=='#' || buf.empty()){
         continue;
      }
      std::istringstream is(buf);
      std::istream_iterator<std::string> issBegin(is);
      std::istream_iterator<std::string> issEnd;
      std::vector<std::string> param(issBegin, issEnd);
      if(param.empty() || param[0].empty()){
         continue;
      }
      //double parameter= atof(param[1].c_str());
      //parameter_list[param[0]]= parameter;
      if((int)param.size()>=2) parameter_list[param[0]] = param[1];
      //double pedestal= atof(param[1].c_str());
      //double sigma= atof(param[2].c_str());
   }
   ifs.close();

   drift_gap               = atof(parameter_list["DriftGap:"].c_str());
   gas_gain                = atof(parameter_list["GasGain:"].c_str());
   gas_gain_sigma          = atof(parameter_list["GasGainSigma:"].c_str());
   cluster_smear_t         = atof(parameter_list["ClusterSmearT:"].c_str());
   cluster_smear_l         = atof(parameter_list["ClusterSmearL:"].c_str());
   n_collisions_mean       = atof(parameter_list["NumCollisions:"].c_str());

   double drift_velocity   = atof(parameter_list["DriftVelocity:"].c_str());
   double diffusion_t      = atof(parameter_list["DiffusionT:"].c_str());
   double diffusion_l      = atof(parameter_list["DiffusionL:"].c_str());
   double preamp_noise     = atof(parameter_list["PreampNoise:"].c_str());
   double t_offset         = atof(parameter_list["TimeOffset:"].c_str());

   //wf_temp = new E16ANA_WaveformTemplate("/ccj/u/E16/database/response/gtr/waveform_template.dat");
   wf_temp = new E16ANA_WaveformTemplate(parameter_list["WaveformTemplateFile:"].c_str());
   for(int i=0; i<(int)gtr_strips.size(); i++){
      gtr_strips[i]->SetWaveformTemplate(wf_temp);
      gtr_strips[i]->SetDriftVelocity(drift_velocity);
      gtr_strips[i]->SetTransverseDiffusion(diffusion_t);
      gtr_strips[i]->SetLongitudinalDiffusion(diffusion_l);
      gtr_strips[i]->SetPreampNoise(preamp_noise);
      gtr_strips[i]->SetTimeOffset(t_offset);
   }
   //SetChargeHisto("gtr_charge_distribution.dat");
   //SetClusterChargeHisto("/ccj/u/E16/database/response/gtr/gtr_cluster_charge.dat");
   SetClusterChargeHisto(parameter_list["ClusterChargeFile:"].c_str());
}

void E16ANA_GTRResponse::PrintParameters(){
   std::cout << "E16ANA_GTRResponse::PrintParameters : "
      << "drift_gap = " << drift_gap << " [mm]"
      << ", gas_gain = " << gas_gain
      << ", gas_gain_sigma = " << gas_gain_sigma
      << ", cluster_smear_t = " << cluster_smear_t << " [mm]"
      << ", cluster_smear_l = " << cluster_smear_l << " [mm]"
      << ", n_collisions_mean = " << n_collisions_mean << std::endl;
   for(int i=0; i<(int)gtr_strips.size(); i++){
      gtr_strips[i]->PrintParameters();
   }
}

void E16ANA_GTRResponse::SetChargeHisto(const char *fname){
   std::ifstream ifs(fname);
   int n_bins_charge;
   double xmin_charge;
   double xmax_charge;
   ifs >> n_bins_charge >> xmin_charge >> xmax_charge >> h_charge_fl >> h_charge_frac;
   h_charge = new TH1D("h_charge", "charge distribution",
         n_bins_charge, xmin_charge, xmax_charge);
   double buffer;
   for(int i=1; i<=n_bins_charge; i++){
      ifs >> buffer;
      h_charge->SetBinContent(i, buffer);
   }
   h_charge->Scale(1.0/h_charge->Integral());
}

void E16ANA_GTRResponse::SetClusterChargeHisto(const char *fname){
   std::ifstream ifs(fname);
   int n_bins;
   double xmin;
   double xmax;
   ifs >> n_bins >> xmin >> xmax;
   //   h_cluster_charge = new TH1D("h_cluster_charge", "charge distribution",
   //    n_bins, xmin, xmax);
   h_cluster_charge = new TH1D(Form("h_cluster_charge_%d",instance_id), 
     "charge distribution", n_bins, xmin, xmax);   

   //   std::cerr<<"h_cluster_charge init"<<h_cluster_charge<<std::endl;

   double buffer;
   for(int i=1; i<=n_bins; i++){
      ifs >> buffer;
      //std::cout << i << " " << buffer << std::endl;
      h_cluster_charge->SetBinContent(i, buffer);
   }
   //std::cout << h_cluster_charge->Integral() << std::endl;
   h_cluster_charge->Scale(1.0/h_cluster_charge->Integral());
   //std::cout << h_cluster_charge->Integral() << std::endl;
}

void E16ANA_GTRResponse::SetTracks(
      const std::vector<TVector3> &local_pos,
      const std::vector<TVector3> &local_mom,
      const std::vector<double> &time){

   for(int i=0; i<(int)cluster_pos.size(); i++){
      cluster_pos[i].clear();
   }
   for(int i=0; i<(int)cluster_charge.size(); i++){
      cluster_charge[i].clear();
   }
   cluster_time.clear();

   for(int i=0; i<(int)local_pos.size(); i++){
      TVector3 track_start = local_pos[i]-(drift_gap/2.0/local_mom[i].Z())*local_mom[i];
      // A z component of 1.0/v.Z()*v is always +1.0.
      TVector3 track_end = local_pos[i]+(drift_gap/2.0/local_mom[i].Z())*local_mom[i];
      double x0 = track_start.X();
      double y0 = track_start.Y();
      double z0 = track_start.Z();
      double x1 = track_end.X();
      double y1 = track_end.Y();
      double z1 = track_end.Z();
      double flight_length = 0.0;
      flight_length += (x1-x0)*(x1-x0);
      flight_length += (y1-y0)*(y1-y0);
      flight_length += (z1-z0)*(z1-z0);
      flight_length = sqrt(flight_length);
      double dz = 0.1;
      //double dz = 1.0;
      double cluster_x;
      double cluster_y;
      double cluster_z;

      // equally divide
      //cluster_z = gRandom->Uniform(z0, z0+dz);
      //cluster_x = x0+(x1-x0)/(z1-z0)*(cluster_z-z0);
      //cluster_y = y0+(y1-y0)/(z1-z0)*(cluster_z-z0);
      //int n_div = (int)((z1-cluster_z)/dz)+1; // equally divide

      int n_div = gRandom->Poisson(flight_length*n_collisions_mean); // poisson distribution
      if(n_div==0) continue;
      double charge = 4000.0;
      //if(h_charge != NULL){
      //   charge = h_charge->GetRandom();
      //   charge *= flight_length/h_charge_fl;
      //   charge /= h_charge_frac;
      //}
      //double a_cluster_charge = charge/((double)n_div);
      double a_cluster_charge = 0.0;
      double charge_frac_x = 0.4;
      double charge_frac_y = 1.0-charge_frac_x;
      double charge_factor = -1.0;
      //double z_resolution = 0.5;
      for(int j=0; j<n_div; j++){

         // poisson + flat distribution
         cluster_z = gRandom->Uniform(z0, z1);
         cluster_x = x0+(x1-x0)/(z1-z0)*(cluster_z-z0);
         cluster_y = y0+(y1-y0)/(z1-z0)*(cluster_z-z0);

         //std::cout << h_charge->Integral() << std::endl;
         a_cluster_charge = h_cluster_charge->GetRandom();
         //a_cluster_charge *= gas_gain;

         while(charge_factor <= 0.0){
            charge_factor = gRandom->Gaus(gas_gain, gas_gain*gas_gain_sigma);
         }
         a_cluster_charge *= charge_factor;

         cluster_pos[0].push_back(TVector3(
                  gRandom->Gaus(cluster_x, cluster_smear_t),
                  gRandom->Gaus(cluster_y, cluster_smear_t),
                  gRandom->Gaus(cluster_z, cluster_smear_l)));
         cluster_charge[0].push_back(a_cluster_charge*charge_frac_x);
         for(int k=1; k<(int)gtr_strips.size(); k++){
            cluster_charge[k].push_back(a_cluster_charge*charge_frac_y);
         }
         cluster_time.push_back(time[i]);

         // equally divide
         // cluster_z += dz;
         // cluster_x += dz*(x1-x0)/(z1-z0);
         // cluster_y += dz*(y1-y0)/(z1-z0);

      }
   }
   for(int i=1; i<(int)gtr_strips.size(); i++){
      int n_clusters = cluster_pos[0].size();
      cluster_pos[i].resize(n_clusters);
      for(int j=0; j<n_clusters; j++){
         cluster_pos[i][j] = (*(rotations[i]))*cluster_pos[0][j];
      }
      //std::cout << cluster_pos[i].size() << std::endl;
   }
   for(int i=0; i<(int)gtr_strips.size(); i++){
      for(int j=0; j<(int)cluster_charge[i].size(); j++){
         //std::cout << "q[" << i << "][" << j << "] = " << cluster_charge[i][j] << std::endl;
         //cluster_pos[i][j].Print();
         //std::cout << cluster_time[j] << std::endl;
      }
      gtr_strips[i]->SetClusters(cluster_pos[i], cluster_time, cluster_charge[i]);
   }
}

void E16ANA_GTRResponse::SetTracks(
      std::vector<E16ANA_G4Hit> &hits){

   for(int i=0; i<(int)cluster_pos.size(); i++){
      cluster_pos[i].clear();
   }
   for(int i=0; i<(int)cluster_charge.size(); i++){
      cluster_charge[i].clear();
   }
   cluster_time.clear();

   //   std::cout << "going to loop over hits" << std::endl;
   //std::cout << "hits.size() = "<< hits.size() << std::endl;
   for(int i=0; i<(int)hits.size(); i++){
      TVector3 track_start = hits[i].XTV()-(drift_gap/2.0/hits[i].PTV().Z())*hits[i].PTV();
      // A z component of 1.0/v.Z()*v is always +1.0.
      TVector3 track_end = hits[i].XTV()+(drift_gap/2.0/hits[i].PTV().Z())*hits[i].PTV();

      double x0 = track_start.X();
      double y0 = track_start.Y();
      double z0 = track_start.Z();
      double x1 = track_end.X();
      double y1 = track_end.Y();
      double z1 = track_end.Z();
      double flight_length = 0.0;
      flight_length += (x1-x0)*(x1-x0);
      flight_length += (y1-y0)*(y1-y0);
      flight_length += (z1-z0)*(z1-z0);
      flight_length = sqrt(flight_length);
      double dz = 0.1;
      //double dz = 1.0;
      double cluster_x;
      double cluster_y;
      double cluster_z;

      // equally divide
      //cluster_z = gRandom->Uniform(z0, z0+dz);
      //cluster_x = x0+(x1-x0)/(z1-z0)*(cluster_z-z0);
      //cluster_y = y0+(y1-y0)/(z1-z0)*(cluster_z-z0);
      //int n_div = (int)((z1-cluster_z)/dz)+1; // equally divide

      int n_div = gRandom->Poisson(flight_length*n_collisions_mean); // poisson distribution
      if(n_div==0) continue;
      double charge = 4000.0;
      //if(h_charge != NULL){
      //   charge = h_charge->GetRandom();
      //   charge *= flight_length/h_charge_fl;
      //   charge /= h_charge_frac;
      //}
      //double a_cluster_charge = charge/((double)n_div);
     double a_cluster_charge = 0.0;
      double charge_frac_x = 0.4;
      double charge_frac_y = 1.0-charge_frac_x;
      double charge_factor = -1.0;
      //double z_resolution = 0.5;
      for(int j=0; j<n_div; j++){

         // poisson + flat distribution
         cluster_z = gRandom->Uniform(z0, z1);
         cluster_x = x0+(x1-x0)/(z1-z0)*(cluster_z-z0);
         cluster_y = y0+(y1-y0)/(z1-z0)*(cluster_z-z0);

         //std::cout << h_charge->Integral() << std::endl;
         a_cluster_charge = h_cluster_charge->GetRandom();
         //a_cluster_charge *= gas_gain;

         while(charge_factor <= 0.0){
            charge_factor = gRandom->Gaus(gas_gain, gas_gain*gas_gain_sigma);
         }
         a_cluster_charge *= charge_factor;

         cluster_pos[0].push_back(TVector3(
                  gRandom->Gaus(cluster_x, cluster_smear_t),
                  gRandom->Gaus(cluster_y, cluster_smear_t),
                  //cluster_y,
                  gRandom->Gaus(cluster_z, cluster_smear_l)));
         cluster_charge[0].push_back(a_cluster_charge*charge_frac_x);
         for(int k=1; k<(int)gtr_strips.size(); k++){
            cluster_charge[k].push_back(a_cluster_charge*charge_frac_y);
         }
         cluster_time.push_back(hits[i].Time());

         // equally divide
         // cluster_z += dz;
         // cluster_x += dz*(x1-x0)/(z1-z0);
         // cluster_y += dz*(y1-y0)/(z1-z0);

      }
   }
   for(int i=1; i<(int)gtr_strips.size(); i++){
      int n_clusters = cluster_pos[0].size();
      cluster_pos[i].resize(n_clusters);
      for(int j=0; j<n_clusters; j++){
         cluster_pos[i][j] = (*(rotations[i]))*cluster_pos[0][j];
      }
      //std::cout << cluster_pos[i].size() << std::endl;
   }
   for(int i=0; i<(int)gtr_strips.size(); i++){
      for(int j=0; j<(int)cluster_charge[i].size(); j++){
         //std::cout << "q[" << i << "][" << j << "] = " << cluster_charge[i][j] << std::endl;
         //cluster_pos[i][j].Print();
         //std::cout << cluster_time[j] << std::endl;
      }
      gtr_strips[i]->SetClusters(cluster_pos[i], cluster_time, cluster_charge[i]);
   }
}

//-------------------------------
const double E16ANA_GTR100Response::strip_pitch_x = 0.35;
const double E16ANA_GTR100Response::strip_pitch_y = 1.40;

E16ANA_GTR100Response::E16ANA_GTR100Response(const char *fname){
   gtr_strips.resize(3);
   rotations.resize(3);
   cluster_pos.resize(3);
   cluster_charge.resize(3);
   for(int i=0; i<(int)cluster_pos.size(); i++){
      cluster_pos[i].reserve(30*10);
   }
   for(int i=0; i<(int)cluster_charge.size(); i++){
      cluster_charge[i].reserve(30*10);
   }
   cluster_time.reserve(30*10);

   gtr_strips[0] = new E16ANA_GTRStripResponse(n_strip_x); // X strip
   gtr_strips[0]->SetStripPitch(strip_pitch_x);
   gtr_strips[0]->SetBoundaries(-50.0, 50.0);
   //gtr_strips[0]->SetTimeOffset(200.0);
   rotations[0] = new TRotation();

   gtr_strips[1] = new E16ANA_GTRStripResponse(n_strip_y); // Y strip A
   gtr_strips[1]->SetStripPitch(strip_pitch_y);
   gtr_strips[1]->SetBoundaries(0, 50.0);
   //gtr_strips[1]->SetTimeOffset(200.0);
   rotations[1] = new TRotation();
   rotations[1]->RotateZ(TMath::Pi()/2.0);

   gtr_strips[2] = new E16ANA_GTRStripResponse(n_strip_y); // Y strip B
   gtr_strips[2]->SetStripPitch(strip_pitch_y);
   gtr_strips[2]->SetBoundaries(0, 50.0);
   //gtr_strips[2]->SetTimeOffset(200.0);
   rotations[2] = new TRotation();
   rotations[2]->RotateZ(-TMath::Pi()/2.0);

   Initialize(fname);
}

E16ANA_GTR100Response::~E16ANA_GTR100Response(){
   delete gtr_strips[0];
   delete gtr_strips[1];
   delete gtr_strips[2];
   delete rotations[0];
   delete rotations[1];
   delete rotations[2];
   if(wf_temp != NULL) delete wf_temp;
}


//-------------------------------------------------------

const double E16ANA_GTR200Response::strip_pitch_x = 0.35;
const double E16ANA_GTR200Response::strip_pitch_y = 1.40;

E16ANA_GTR200Response::E16ANA_GTR200Response(const char *fname){
   gtr_strips.resize(2);
   rotations.resize(2);
   cluster_pos.resize(2);
   cluster_charge.resize(2);

   for(int i=0; i<(int)cluster_pos.size(); i++){
      cluster_pos[i].reserve(30*10);
   }
   for(int i=0; i<(int)cluster_charge.size(); i++){
      cluster_charge[i].reserve(30*10);
   }
   cluster_time.reserve(30*10);

   gtr_strips[0] = new E16ANA_GTRStripResponse(n_strip_x); // X strip
   gtr_strips[0]->SetStripPitch(strip_pitch_x);
   gtr_strips[0]->SetBoundaries(-100.0, 100.0);
   gtr_strips[0]->SetTimeOffset(200.0);
   rotations[0] = new TRotation();

   gtr_strips[1] = new E16ANA_GTRStripResponse(n_strip_y); // Y strip A
   gtr_strips[1]->SetStripPitch(strip_pitch_y);
   gtr_strips[1]->SetBoundaries(-100.0, 100.0);
   gtr_strips[1]->SetTimeOffset(200.0);
   rotations[1] = new TRotation();
   rotations[1]->RotateZ(TMath::Pi()/2.0);
#if 0
   gtr_strips[2] = new E16ANA_GTRStripResponse(n_strip_y); // Y strip B
   gtr_strips[2]->SetStripPitch(strip_pitch_y);
   gtr_strips[2]->SetBoundaries(0.0, 100.0);
   gtr_strips[2]->SetTimeOffset(200.0);
   rotations[2] = new TRotation();
   rotations[2]->RotateZ(TMath::Pi()/2.0);
#endif
   Initialize(fname);
}

E16ANA_GTR200Response::~E16ANA_GTR200Response(){
   delete gtr_strips[0];
   delete gtr_strips[1];
   //   delete gtr_strips[2];
   delete rotations[0];
   delete rotations[1];
   //   delete rotations[2];
   if(wf_temp != NULL) delete wf_temp;
}

//-------------------------------

const double E16ANA_GTR300Response::strip_pitch_x = 0.35;
const double E16ANA_GTR300Response::strip_pitch_y = 1.40;

E16ANA_GTR300Response::E16ANA_GTR300Response(const char *fname){
   gtr_strips.resize(2);
   rotations.resize(2);
   cluster_pos.resize(2);
   cluster_charge.resize(2);
   for(int i=0; i<(int)cluster_pos.size(); i++){
      cluster_pos[i].reserve(30*10);
   }
   for(int i=0; i<(int)cluster_charge.size(); i++){
      cluster_charge[i].reserve(30*10);
   }
   cluster_time.reserve(30*10);

   gtr_strips[0] = new E16ANA_GTRStripResponse(n_strip_x); // X strip
   gtr_strips[0]->SetStripPitch(strip_pitch_x);
   gtr_strips[0]->SetBoundaries(-150.0, 150.0);
   gtr_strips[0]->SetTimeOffset(200.0);
   rotations[0] = new TRotation();

   gtr_strips[1] = new E16ANA_GTRStripResponse(n_strip_y); // Y strip A
   gtr_strips[1]->SetStripPitch(strip_pitch_y);
   gtr_strips[1]->SetBoundaries(-150.0, 150.0);
   gtr_strips[1]->SetTimeOffset(200.0);
   rotations[1] = new TRotation();
   rotations[1]->RotateZ(TMath::Pi()/2.0);
#if 0
   gtr_strips[2] = new E16ANA_GTRStripResponse(n_strip_y); // Y strip B
   gtr_strips[2]->SetStripPitch(strip_pitch_y);
   gtr_strips[2]->SetBoundaries(0.0, 100.0);
   gtr_strips[2]->SetTimeOffset(200.0);
   rotations[2] = new TRotation();
   rotations[2]->RotateZ(TMath::Pi()/2.0);
#endif
   Initialize(fname);
}

E16ANA_GTR300Response::~E16ANA_GTR300Response(){
   delete gtr_strips[0];
   delete gtr_strips[1];
   //   delete gtr_strips[2];
   delete rotations[0];
   delete rotations[1];
   //   delete rotations[2];
   if(wf_temp != NULL) delete wf_temp;
}

//---------------------
E16ANA_GTRResponseManager::E16ANA_GTRResponseManager(
  E16ANA_ParamManager* iparam): paramMgr(iparam) {

  for(int i = 0; i< Nmodule ; i++ ){

    res100[i] = new E16ANA_GTR100Response(
      paramMgr->GetGTR100ResponseParamFileName(i)  );
    res200[i] = new E16ANA_GTR200Response(
      paramMgr->GetGTR200ResponseParamFileName(i)  );
    res300[i] = new E16ANA_GTR300Response(
      paramMgr->GetGTR300ResponseParamFileName(i)  );

  }//for i

}

int E16ANA_GTRResponseManager::WriteAnEvent(E16ANA_WaveformOutputData *file){
   E16ANA_WaveformEvent *event = file->Event();
   int nx100 = res100[0]->GetStripX()->GetNumStrips();
   int ny100 = res100[0]->GetStripY()->GetNumStrips();
   int nx200 = res200[0]->GetStripX()->GetNumStrips();
   int nx300 = res300[0]->GetStripX()->GetNumStrips();
   E16ANA_GTRStripResponse *str_res;
   for(int i=0; i<33; i++){
      // GTR100
      str_res = res100[i]->GetStripX();
      for(int j=0; j<str_res->GetNumHitStrips(); j++){
         event->SetWaveform(0, i, str_res->GetChannel(j), str_res->GetWaveform(j));
      }
      str_res = res100[i]->GetStripY();
      for(int j=0; j<str_res->GetNumHitStrips(); j++){
         event->SetWaveform(0, i, str_res->GetChannel(j)+nx100, str_res->GetWaveform(j));
      }
      str_res = res100[i]->GetStripYb();
      for(int j=0; j<str_res->GetNumHitStrips(); j++){
         event->SetWaveform(0, i, str_res->GetChannel(j)+nx100+ny100, str_res->GetWaveform(j));
      }
      // GTR200
      str_res = res200[i]->GetStripX();
      for(int j=0; j<str_res->GetNumHitStrips(); j++){
         event->SetWaveform(1, i, str_res->GetChannel(j), str_res->GetWaveform(j));
      }
      str_res = res200[i]->GetStripY();
      for(int j=0; j<str_res->GetNumHitStrips(); j++){
         event->SetWaveform(1, i, str_res->GetChannel(j)+nx200, str_res->GetWaveform(j));
      }
      // GTR300
      str_res = res300[i]->GetStripX();
      for(int j=0; j<str_res->GetNumHitStrips(); j++){
         event->SetWaveform(2, i, str_res->GetChannel(j), str_res->GetWaveform(j));
      }
      str_res = res300[i]->GetStripY();
      for(int j=0; j<str_res->GetNumHitStrips(); j++){
         event->SetWaveform(2, i, str_res->GetChannel(j)+nx300, str_res->GetWaveform(j));
      }
   }
   return file->WriteAnEvent();
}

