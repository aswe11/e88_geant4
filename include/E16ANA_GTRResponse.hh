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
#ifndef E16ANA_GTRResponse_hh
#define E16ANA_GTRResponse_hh 1

#include <vector>

#include "E16ANA_GTRStripResponse.hh"
#include "E16ANA_G4OutputData.hh"
#include "E16ANA_WaveformOutputData.hh"

class TVector3;
class TRotation;
class TH1D;

class E16ANA_ParamManager;

class E16ANA_WaveformTemplate;
//class E16ANA_GTRStripResponse;
//class E16ANA_G4Hit;

class E16ANA_GTRResponse{
   public:
      E16ANA_GTRResponse();
      virtual ~E16ANA_GTRResponse() = 0;
      void SetTracks( // old version
            const std::vector<TVector3> &local_pos,
            const std::vector<TVector3> &local_mom,
            const std::vector<double> &time);
      void SetTracks( // use this
            std::vector<E16ANA_G4Hit> &hits);

      E16ANA_GTRStripResponse* GetStripX(){return gtr_strips[0];};
      E16ANA_GTRStripResponse* GetStripY(){return gtr_strips[1];};

      // position smearing parameters for each clusters ('T' is transverse, 'L' is longitudinal)
      void SetClusterSmearT(double cluster_smear_t_){cluster_smear_t = cluster_smear_t_;};// mm
      void SetClusterSmearL(double cluster_smear_l_){cluster_smear_l = cluster_smear_l_;};// mm

      void SetGasGain(double gas_gain_){gas_gain = gas_gain_;};// conversion factor (h_cluster_charge -> ADC value)
      void PrintParameters();

   protected:
      static int n_instances;
      int instance_id;
      std::vector<TRotation*> rotations;
      std::vector<E16ANA_GTRStripResponse*> gtr_strips;// 0th component of this array is x_strip, the others are y_strips.
      std::vector<std::vector<TVector3> > cluster_pos; // Z-direction corresponds to the drift direction.
      std::vector<std::vector<double> > cluster_charge;
      std::vector<double> cluster_time;
      TH1D *h_charge; // charge distribution of real data
      double h_charge_fl; // flight length
      double h_charge_frac; // charge frac
      E16ANA_WaveformTemplate *wf_temp;
      double drift_gap;

      double n_collisions_mean; // poisson mean for 1 mm
      TH1D *h_cluster_charge; // charge distribution of "one" cluster simulated by using TrackHeed
      double gas_gain;
      double gas_gain_sigma; // sigma/gain

      double cluster_smear_t; // transverse (mm)
      double cluster_smear_l; // longitudinal (mm)

      void Initialize(const char *fname);
      void SetChargeHisto(const char *fname);
      void SetClusterChargeHisto(const char *fname);

};

//----------------------------------------------


class E16ANA_GTR100Response;
class E16ANA_GTR200Response;
class E16ANA_GTR300Response;


//-------------------------------------



class E16ANA_GTR100Response : public E16ANA_GTRResponse{
   public:
      E16ANA_GTR100Response(const char *fname);
      ~E16ANA_GTR100Response();
      E16ANA_GTRStripResponse* GetStripYa(){return gtr_strips[1];};
      E16ANA_GTRStripResponse* GetStripYb(){return gtr_strips[2];};

   private:
      enum{ n_strip_x = 288, n_strip_y = 72 };
      const static double strip_pitch_x;
      const static double strip_pitch_y;
};

class E16ANA_GTR200Response : public E16ANA_GTRResponse{
   public:
      E16ANA_GTR200Response(const char *fname);
      ~E16ANA_GTR200Response();
      //E16ANA_GTRStripResponse* GetStripYa(){return gtr_strips[1];};
      //E16ANA_GTRStripResponse* GetStripYb(){return gtr_strips[2];};

   private:
      enum{ n_strip_x = 576, n_strip_y = 144 };
      const static double strip_pitch_x;
      const static double strip_pitch_y;
};

class E16ANA_GTR300Response : public E16ANA_GTRResponse{
   public:
      E16ANA_GTR300Response(const char *fname);
      ~E16ANA_GTR300Response();
      //E16ANA_GTRStripResponse* GetStripYa(){return gtr_strips[1];};
      //E16ANA_GTRStripResponse* GetStripYb(){return gtr_strips[2];};

   private:
      enum{ n_strip_x = 864, n_strip_y = 216 };
      const static double strip_pitch_x;
      const static double strip_pitch_y;
};



class E16ANA_GTRResponseManager{

public:
  E16ANA_GTRResponseManager(E16ANA_ParamManager* iparam);
  E16ANA_GTR100Response*  Response100(int i) { return res100[i];}
  E16ANA_GTR200Response*  Response200(int i) { return res200[i];}
  E16ANA_GTR300Response*  Response300(int i) { return res300[i];}
  int WriteAnEvent(E16ANA_WaveformOutputData *file);

private:
  enum{Nmodule = 33};
  E16ANA_ParamManager* paramMgr;
  E16ANA_GTR100Response*  res100[ Nmodule ];
  E16ANA_GTR200Response*  res200[ Nmodule ];
  E16ANA_GTR300Response*  res300[ Nmodule ];

};



#endif // E16ANA_GTRResponse_hh
