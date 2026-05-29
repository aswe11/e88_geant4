//2016-08-12, uploaded by nakai
//2016-03-18, uploaded by nakai
//2015-09-05, uploaded by nakai
#ifndef E16G4_SANFORD_WANG_GENERATOR_H
#define E16G4_SANFORD_WANG_GENERATOR_H

#include <Randomize.hh>

// pseudo Sanford-Wang generator (input theta -> corresponding momentum distribution)

class E16G4_SanfordWangGenerator {
public:
   E16G4_SanfordWangGenerator();
   //E16G4_SanfordWangGenerator(double _p_beam, double _p_max);
   ~E16G4_SanfordWangGenerator();
   double ShootFixedTheta(double theta);

private:
   enum {
      kPiplus   = 0,
      kPiminus  = 1,
      kKplus    = 2,
      kKminus   = 3,
      kPbar     = 4,
      n_params  = 5
   };

   // Sanford-Wang parameters
   static const double A_array[];
   static const double B_array[];
   static const double C_array[];
   static const double D_array[];
   static const double E_array[];
   static const double F_array[];
   static const double G_array[];
   static const double H_array[];

   enum {
      n_theta_bins = 91,
      n_p_bins     = 1001
   };
   const static double theta_bin_width;
   CLHEP::RandGeneral *rand_gen[n_theta_bins];

   // Incident beam momentum [GeV]
   double p_beam;
   // max momentum of secondary particle [GeV]
   double p_max;

   double CalcfTerm(int id, double p, double theta, double p_in);
   double CalcgTerm(int id, double p, double theta, double p_in);
   double CalcSanfordWang(double p, double theta, double p_in);
   void CreateRandGeneral(int theta_bin);
   //void CreateRandGeneral(CLHEP::RandGeneral *rand, int theta_bin);

};

#endif // E16G4_SANFORD_WANG_GENERATOR_H
