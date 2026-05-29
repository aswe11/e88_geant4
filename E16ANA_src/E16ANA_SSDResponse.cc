//2016-05-02, uploaded by nakai
#include "E16ANA_SSDResponse.hh"
#include "E16ANA_G4OutputData.hh"

#include <TRandom.h>

E16ANA_SSDResponse::E16ANA_SSDResponse() : 
   smear_x(0.030), smear_y(0.030), smear_t(3.6),
   time_window_min(-15.0), time_window_max(15.0)
{
   analyzed_hits.reserve(400);
}

E16ANA_SSDResponse::~E16ANA_SSDResponse(){
}

void E16ANA_SSDResponse::Clear(){
   analyzed_hits.clear();
}

void E16ANA_SSDResponse::SetTracks(std::vector<E16ANA_G4Hit> &hits){
   Clear();
   for(int i=0; i<(int)hits.size(); i++){
      analyzed_hits.push_back(hits[i]);
      E16ANA_G4Hit &hit = analyzed_hits.back();
      hit.SetXYZ(gRandom->Gaus(hit.X(), smear_x), E16ANA_G4Hit::ErrorValue, 0.0); // remove y information
      hit.SetT(gRandom->Gaus(hit.Time(), smear_t));
      if(hit.Time() < time_window_min || hit.Time() > time_window_max){ // outside of timing window
         analyzed_hits.pop_back();
      }
   }
}

