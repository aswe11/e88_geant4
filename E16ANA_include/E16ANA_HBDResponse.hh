//2016-11-22, uploaded by nakai
#ifndef E16ANA_HBDResponse_hh
#define E16ANA_HBDResponse_hh

#include "E16ANA_G4OutputData.hh"

class E16ANA_HBDResponse {
public:
   E16ANA_HBDResponse(int _module_id);
   ~E16ANA_HBDResponse();
   void Clear();
   void SetTracks(std::vector<E16ANA_G4Hit> &hits);
   std::vector<E16ANA_G4Hit>& AnalyzedHits(){
      return analyzed_hits;
   };

private:
   int module_id;
   double smear_x;
   double smear_y;
   std::vector<E16ANA_G4Hit> analyzed_hits;
};

class E16ANA_HBDResponseManager {
public:
   E16ANA_HBDResponseManager();
   ~E16ANA_HBDResponseManager();
   void SetG4Event(E16ANA_G4Event *g4event);
   void Clear(){
      for(int i=0; i<n_modules; i++){
         hbd_res[i]->Clear();
      }
      all_hits.clear();
   };
   E16ANA_HBDResponse* Response(int module_id){return hbd_res[module_id];};
   std::vector<E16ANA_G4Hit>& AllAnalyzedHits(){return all_hits;};

private:
   enum{
      n_modules = 27
   };
   //std::vector<E16ANA_HBDResponse*> hbd_res;
   E16ANA_HBDResponse *hbd_res[n_modules];
   std::vector<E16ANA_G4Hit> all_hits;

};

#endif // E16ANA_HBDResponse_hh
