//2016-05-02, uploaded by nakai
#ifndef E16ANA_SSDResponse_hh
#define E16ANA_SSDResponse_hh

#include <vector>

class E16ANA_G4Hit;

class E16ANA_SSDResponse {
public:
   E16ANA_SSDResponse();
   ~E16ANA_SSDResponse();
   void Clear();
   void SetTracks(std::vector<E16ANA_G4Hit> &hits);
   std::vector<E16ANA_G4Hit>& AnalyzedHits(){
      return analyzed_hits;
   };

private:
   double smear_x; // mm
   double smear_y; // mm
   double smear_t; // ns
   double time_window_min; // ns (trigger timing is 0.0 ns)
   double time_window_max; // ns
   //double cluster_size_x;
   std::vector<E16ANA_G4Hit> analyzed_hits;

};

#endif // E16ANA_SSDResponse_hh

