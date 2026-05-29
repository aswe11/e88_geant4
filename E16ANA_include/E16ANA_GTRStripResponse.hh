//2016-11-22, uploaded by nakai
//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai
//2015-12-12, uploaded by nakai
//2015-11-14, uploaded by nakai
//2015-11-02, uploaded by yokkaich
//2015-11-02, uploaded by komatsu
//2015-07-15, uploaded by nakai
//2015-03-27, uploaded by yokkaich
//2015-03-20, uploaded by nakai
#ifndef E16ANA_GTRStripResponse_hh
#define E16ANA_GTRStripResponse_hh 1

#define StripResponse_WorkSpace_Static

#include <vector>

//#include <TVector2.h>
class TVector2;
class TRandom3;

//#include "E16ANA_WaveformTemplate.hh"
class E16ANA_WaveformTemplate;

class E16ANA_GTRStripResponse
{
   public:
      E16ANA_GTRStripResponse(int n_strips_);
      E16ANA_GTRStripResponse(int n_strips_, E16ANA_WaveformTemplate *wf_temp_);
      //E16ANA_GTRStripResponse(int n_strips_, E16ANA_WaveformTemplate *wf_temp_, TRandom3 *rand_);
      ~E16ANA_GTRStripResponse();

      int GetNumStrips(){return n_strips;};
      double GetStripPitch(){return strip_pitch;};
      double GetDriftGap(){return drift_gap;};
      double GetDriftVelocity(){return drift_velocity;};
      double GetTransverseDiffusion(){return diffusion_t;};
      double GetRiseTime(){return rise_time;};
      double GetPreampNoise(){return preamp_noise;};

      //void SetStripPitch(double strip_pitch_){strip_pitch = strip_pitch_;};
      void SetStripPitch(double strip_pitch_);
      void SetDriftGap(double drift_gap_){drift_gap = drift_gap_;};
      void SetDriftVelocity(double drift_velocity_){drift_velocity = drift_velocity_;};
      void SetTransverseDiffusion(double diffusion_t_){diffusion_t = diffusion_t_;};
      void SetLongitudinalDiffusion(double diffusion_l_){diffusion_l = diffusion_l_;};
      void SetRiseTime(double rise_time_){rise_time = rise_time_;};
      void SetPreampNoise(double preamp_noise_){preamp_noise = preamp_noise_;};
      void SetTimeOffset(double t_offset_){t_offset = t_offset_;};
      void SetBoundaries(double y0, double y1){boundary_y0 = y0; boundary_y1 = y1;};
      void SetWaveformTemplate(E16ANA_WaveformTemplate *wf_temp_);

      void PrintParameters();

      int GetNumHitStrips(){return n_hit_strips;};

      void SetTracks(
            const std::vector<TVector2> &track_start,
            const std::vector<TVector2> &track_end,
            const std::vector<double> &t,
            const std::vector<double> &charge);
      void SetClusters(
            const std::vector<TVector3> &cluster_pos,
            const std::vector<double> &t,
            const std::vector<double> &charge);

      int GetChannel(int i){return wf_channel[i];};
      //int *GetWaveform(int i){return wf[i];};
      std::vector<int>& GetWaveform(int i){return wf[i];};
      //double GetCurrentSignal(int i, int j){return current_signal[i][j];};
      int GetNumSignal(){return n_isignal;};
      double GetOccupancy(){return (double)n_hit_strips/(double)n_strips;};

   private:
      int n_strips;
      double strip_pitch;
      double drift_gap;
      double drift_velocity;
      double diffusion_t;
      double diffusion_l;
      double rise_time;
      double preamp_noise;
      double t_offset;
      double x_start;

      double boundary_y0;
      double boundary_y1;

      //TRandom3 *rand;
      E16ANA_WaveformTemplate *wf_temp;

      const static double clock_period;
      const static double threshold;
      //enum{ n_sampling = 30,
      //      n_isignal = 1500};
      enum{ n_sampling = 21,
            //n_hit_strips_max = 100,
            n_isignal = 1500};

      //int waveform[n_hit_strips_max][n_sampling];
      //int **wf;
      std::vector<std::vector<int> > wf;
      //int wf_ptr;
      //int n_hit_strips_max;
      //int *wf_channel;
      std::vector<int> wf_channel;
      int n_hit_strips;

//#ifndef StripResponse_WorkSpace_Static
#if 0
      double **current_signal;
      double *current_integral;
#else
      static std::vector<std::vector<double> > current_signal;
      static std::vector<double> current_integral;
#endif

      double current_start;
      double current_end;
      double current_dt;

      void Clear();
      void CalcConvolution();
      void CalcConvolution(int ch);
      int GetSignalTimeBin(double t);
      double GetSignalTime(int bin);
      void AddCurrentSignals(const TVector2 &track_start, const TVector2 &track_end, double t, double charge);
      void AddStripCurrent(int ch, double t_start, double t_end, double charge);
      void AddStripCurrentGaus(int ch, double t_center, double charge);
      //void AddPreampNoise(int ch);
      double EquallyDivide(double x, double x_left, double x_right);
      void AddClusterCharge(double x, double z, double t, double charge);
      double GaussIntegralErf(double x1, double x2, double mean, double sigma); // Normalized (x1->-inf, x2->inf, returnValue->1.0)
      void ZeroSuppress(double threshold_sigma = 4.0);

};

#endif //E16ANA_GTRStripResponse_hh
