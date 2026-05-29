//2016-11-22, uploaded by nakai
#include "E16ANA_HBDResponse.hh"

#include <TRandom.h>

E16ANA_HBDResponse::E16ANA_HBDResponse(int _module_id) :
   module_id(_module_id), smear_x(10.0), smear_y(10.0)
   //module_id(_module_id), smear_x(0.0), smear_y(0.0)
{
   analyzed_hits.reserve(10);
}

E16ANA_HBDResponse::~E16ANA_HBDResponse(){
}

void E16ANA_HBDResponse::Clear(){
   analyzed_hits.clear();
}

void E16ANA_HBDResponse::SetTracks(std::vector<E16ANA_G4Hit> &hits){
   for(int i=0; i<(int)hits.size(); i++){
      //if(hits[i].ModuleID() != module_id){continue;}
      if(hits[i].PID() != 11 && hits[i].PID() != -11){continue;}
      if(hits[i].Z() < -25.0){continue;}
      analyzed_hits.push_back(hits[i]);
      E16ANA_G4Hit &hit = analyzed_hits.back();
      hit.SetXYZ(
            gRandom->Gaus(hit.X(), smear_x),
            gRandom->Gaus(hit.Y(), smear_y),
            hit.Z()
            );
   }
}

E16ANA_HBDResponseManager::E16ANA_HBDResponseManager(){
   for(int i=0; i<n_modules; i++){
      hbd_res[i] = new E16ANA_HBDResponse(i);
   }
   all_hits.reserve(300);
}

E16ANA_HBDResponseManager::~E16ANA_HBDResponseManager(){
   for(int i=0; i<n_modules; i++){
      delete hbd_res[i];
   }
}

void E16ANA_HBDResponseManager::SetG4Event(E16ANA_G4Event *g4event){
   Clear();
   std::vector<E16ANA_G4Hit> hbd_hits[n_modules];
   for(int i=0; i<g4event->HBD().Noh(); i++){
      E16ANA_G4Hit &hit = g4event->HBD().Hit(i);
      hbd_hits[hit.ModuleID()].push_back(hit);
   }
   for(int i=0; i<n_modules; i++){
      hbd_res[i]->SetTracks(hbd_hits[i]);
      std::vector<E16ANA_G4Hit> &module_hits = hbd_res[i]->AnalyzedHits();
      all_hits.insert(all_hits.end(), module_hits.begin(), module_hits.end());
   }
}

