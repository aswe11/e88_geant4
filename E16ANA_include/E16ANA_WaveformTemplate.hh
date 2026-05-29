//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai
//2015-11-02, uploaded by yokkaich
//2015-11-02, uploaded by komatsu
//2015-07-15, uploaded by nakai
//2015-03-27, uploaded by yokkaich
//2015-03-20, uploaded by nakai
#ifndef E16ANA_WaveformTemplate_hh
#define E16ANA_WaveformTemplate_hh 1

#include <vector>

class E16ANA_WaveformTemplate{
   public:
      E16ANA_WaveformTemplate(const char *fname);
      ~E16ANA_WaveformTemplate();

      double GetValue(double t);
      double GetTimeStart(){return t_start;};
      //double GetTimeEnd(){return t_start+delta_t*(n_points-1);};
      double GetTimeEnd(){return t_end;};

   private:
      int n_points;
      double delta_t;
      double t_start;
      double t_end;
      std::vector<double> wf_temp;

      double LinearInterpolate(double t);

};

#endif //E16ANA_WaveformTemplate_hh
