//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai
//2015-11-14, uploaded by nakai
//2015-07-15, uploaded by nakai
//2015-03-27, uploaded by yokkaich
//2015-03-20, uploaded by nakai
#include <iostream>
#include <fstream>

#include "E16ANA_WaveformTemplate.hh"

E16ANA_WaveformTemplate::E16ANA_WaveformTemplate(const char *fname){
   std::ifstream ifs(fname);
   if(ifs.fail()){
      return;
   }
   ifs >> n_points >> delta_t >> t_start;
   wf_temp.reserve(n_points);
   double buf;
   for(int i=0; i<n_points; i++){
      ifs >> buf;
      if(ifs.eof()) break;
      wf_temp.push_back(buf);
   }
   t_end = t_start+delta_t*(n_points-1);
}

E16ANA_WaveformTemplate::~E16ANA_WaveformTemplate(){
}

double E16ANA_WaveformTemplate::GetValue(double t){
   return LinearInterpolate(t);
}

double E16ANA_WaveformTemplate::LinearInterpolate(double t){
   //if(t<t_start || t>=t_start+delta_t*(n_points-1)){
   if(t<t_start || t>=t_end){
      return 0.0;
   }
   int left_n = (int)((t-t_start)/delta_t);
   if(left_n < 0) left_n = 0;
   if(left_n >= n_points-1) left_n = n_points-2;
   double left_t = t-left_n*delta_t-t_start;
   double right_t = (left_n+1)*delta_t+t_start-t;
   return (left_t*wf_temp[left_n+1]+right_t*wf_temp[left_n]) / delta_t;
}

