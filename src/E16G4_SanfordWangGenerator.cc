//2016-08-12, uploaded by nakai
//2015-09-05, uploaded by nakai
#include "E16G4_SanfordWangGenerator.hh"

#include <cmath>

const double E16G4_SanfordWangGenerator::A_array[] = {
   1.092,
   0.821,
   0.05597,
   0.02210,
   0.001426
};

const double E16G4_SanfordWangGenerator::B_array[] = {
   0.6458,
   0.5271,
   0.6916,
   1.323,
   1.994
};

const double E16G4_SanfordWangGenerator::C_array[] = {
   4.046,
   3.956,
   3.744,
   9.671,
   9.320
};

const double E16G4_SanfordWangGenerator::D_array[] = {
   1.625,
   1.731,
   4.520,
   1.712,
   1.672
};

const double E16G4_SanfordWangGenerator::E_array[] = {
   1.656,
   1.617,
   4.190,
   1.643,
   1.480
};

const double E16G4_SanfordWangGenerator::F_array[] = {
   5.029,
   4.735,
   4.928,
   4.673,
   4.461
};

const double E16G4_SanfordWangGenerator::G_array[] = {
   0.1722,
   0.1984,
   0.1922,
   0.1686,
   0.2026
};

const double E16G4_SanfordWangGenerator::H_array[] = {
   82.65,
   88.75,
   50.28,
   77.27,
   78.00
};

const double E16G4_SanfordWangGenerator::theta_bin_width = M_PI/2.0/(double)(n_theta_bins-1.0);

E16G4_SanfordWangGenerator::E16G4_SanfordWangGenerator() :
   p_beam(30.0), p_max(30.0)
{
   for(int i=0; i<n_theta_bins; i++){
      //CreateRandGeneral(rand_gen[i], i);
      CreateRandGeneral(i);
   }
}

//E16G4_SanfordWangGenerator::E16G4_SanfordWangGenerator(double _p_beam, double _p_max) : 
//   p_beam(_p_beam), p_max(_p_max)
//{
//   for(int i=0; i<n_theta_bins; i++){
//      CreateRandGeneral(rand_gen[i], i);
//   }
//}

E16G4_SanfordWangGenerator::~E16G4_SanfordWangGenerator(){
   for(int i=0; i<n_theta_bins; i++){
      delete rand_gen[i];
   }
}

double E16G4_SanfordWangGenerator::ShootFixedTheta(double theta){
   int bin_theta = 0;
   //if(fabs(theta) > M_PI/2.0){
   //   bin_theta = n_theta_bins-1;
   //}else{
   //   bin_theta = floor(fabs(theta)/theta_bin_width+0.5);
   //}
   bin_theta = floor(fabs(theta)/theta_bin_width+0.5);
   if(bin_theta<0 || bin_theta>=n_theta_bins){
      bin_theta = n_theta_bins-1;
   }
   return rand_gen[bin_theta]->shoot()*p_max;
}

double E16G4_SanfordWangGenerator::CalcfTerm(int id, double p, double theta, double p_in){
   double A = A_array[id];
   double B = B_array[id];
   double C = C_array[id];
   double D = D_array[id];
   double E = E_array[id];
   //double F = F_array[id];
   //double G = G_array[id];
   //double H = H_array[id];
   return A*pow(p, B)*(1.0-p/p_in)*exp(-C*pow(p,D)/pow(p_in, E));
}

double E16G4_SanfordWangGenerator::CalcgTerm(int id, double p, double theta, double p_in){
   //double A = A_array[id];
   //double B = B_array[id];
   //double C = C_array[id];
   //double D = D_array[id];
   //double E = E_array[id];
   double F = F_array[id];
   double G = G_array[id];
   double H = H_array[id];
   return exp(-F*theta*(p-G*p_in*pow(cos(theta), H)));
}

double E16G4_SanfordWangGenerator::CalcSanfordWang(double p, double theta, double p_in){
   double result = 0.0;
   result += CalcfTerm(kPiplus , p, theta, p_in)*CalcgTerm(kPiplus , p, theta, p_in);
   result += CalcfTerm(kPiminus, p, theta, p_in)*CalcgTerm(kPiminus, p, theta, p_in);
   return result;
}

//void E16G4_SanfordWangGenerator::CreateRandGeneral(CLHEP::RandGeneral *rand, int theta_bin){
void E16G4_SanfordWangGenerator::CreateRandGeneral(int theta_bin){
   double prob_func[n_p_bins];
   double theta = theta_bin*theta_bin_width;
   double p_bin_width = p_max/(double)(n_p_bins-1.0);
   for(int i=0; i<n_p_bins; i++){
      double p = p_bin_width*i;
      prob_func[i] = CalcSanfordWang(p, theta, p_beam);
   }
   rand_gen[theta_bin] = new CLHEP::RandGeneral(prob_func, n_p_bins);
}

