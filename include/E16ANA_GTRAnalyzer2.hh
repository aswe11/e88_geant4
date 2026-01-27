//2016-11-22, uploaded by nakai
//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai
//2015-12-12, uploaded by nakai
//2015-12-03, uploaded by yokkaich
//2015-11-14, uploaded by nakai
//2015-11-02, uploaded by yokkaich
//2015-11-02, uploaded by komatsu
//2015-10-05, uploaded by yokkaich
//2015-08-20, uploaded by yokkaich
//2015-07-15, uploaded by nakai
#ifndef E16ANA_GTRAnalyzer2_hh
#define E16ANA_GTRAnalyzer2_hh 1

#include <vector>

#include <TVector3.h>

class E16ANA_Geometry;
class E16ANA_GTRStripResponse;
class E16ANA_GTRStripAnalyzer;
class E16ANA_WaveformEvent;
class E16ANA_G4Hit;

// example
// E16ANA_GTR100Analyzer( 360, 30); for GTR100
// E16ANA_GTR200Analyzer( 720, 30); for GTR200
// E16ANA_GTR300Analyzer(1080, 30); for GTR300
//
// Arguments are num_of_fadcs, and num_of_samplings respectively.
// When using this for ordinary simulation, use default arguments.
// If not, set num_of_fadcs correct value (for example 384) and use SetPinAssign method.

class E16ANA_GTRAnalyzer2{
public:
   E16ANA_GTRAnalyzer2(int _n_strip_x, int _n_strip_y, int _n_fadc_chs, int _n_fadc_counts);
   virtual ~E16ANA_GTRAnalyzer2();
   virtual void SetParameters(std::string filename);
   virtual void SetPinAssign(std::string filename);
   void SetFadcValidCount(std::string filename);
   virtual void PrintParameters();

   void Analyze();

   void Analyze2X(int hitid);//use CalcTdc2 for a hit
   void Analyze2Y(int hitid, int strip_type = 1);//use CalcTdc2 for a hit
   //virtual void Analyze2Y(int hitid, int type)=0;

   void AnalyzeV0(){Analyze();};
   void AnalyzeV0X(int hitid){Analyze2X(hitid);};
   void AnalyzeV0Y(int hitid){Analyze2Y(hitid);};

   void AnalyzeV1();
   void AnalyzeV1X(int hitid){Analyze2X(hitid);};
   void AnalyzeV1Y(int hitid, int strip_type = 1){Analyze2Y(hitid, strip_type);};

   void AnalyzeV2();
   void AnalyzeV2X(int hitid);//use CalcTdc2 for a hit
   void AnalyzeV2Y(int hitid, int strip_type = 1);//use CalcTdc2 for a hit

   void AnalyzeV3();
   void AnalyzeV3(const TVector3 &track_lpos);// analyze only periphery of TV3
   void AnalyzeV3X(int hitid);//use CalcTdc2 for a hit
   void AnalyzeV3Y(int hitid, int strip_type = 1);//use CalcTdc2 for a hit

   void AnalyzeV4(const TVector3 &track_lpos, const TVector3 &track_lmom);

   void Clear();
   virtual void SetFadc(int ch, double *_fadc);// use this
   void SetFadcX(int strip_id, double *_fadc);// use this
   void SetFadcY(int strip_id, double *_fadc);// use this

   void SetFadcT0CorrectionParameter(int _fadc_t0){fadc_t0_correction= _fadc_t0*tdc_lsb;};

   E16ANA_GTRStripAnalyzer* GetStripX(){return strip_ana_x;};
   E16ANA_GTRStripAnalyzer* GetStripY(){return strip_ana_y;};
#if 0
  virtual E16ANA_GTRStripAnalyzer* GetStripYa()=0;
  virtual E16ANA_GTRStripAnalyzer* GetStripYb()=0;
  virtual void  SetStripDataYa()=0;
  virtual void  SetStripDataYb()=0;
#endif
  void SetStripDataX( E16ANA_GTRStripResponse* strip){
    SetStripData(strip, strip_ana_x);
  }

  void SetStripDataY( E16ANA_GTRStripResponse* strip){
    SetStripData(strip, strip_ana_y);
  }


protected:
   int n_strip_x;
   int n_strip_y;
   int n_fadc_counts;
   int n_fadc_chs;
   int *strip_num;// Use to convert fadc_ch to strip_num (shared by X and Y)
   //int y_strip_num[32];

   double x_start;
   double y_start;
   double strip_pitch_x;
   double strip_pitch_y;
   double gem_th_x;
   double gem_th_y;
   double drift_velocity;
   double drift_gap_center;
   double drift_gap_center_v3;
   double fadc_clock_period;
   double fadc_t0_correction;
   double tdc_lsb;
   double gem_t0_min;
   double gem_t0_max;
   double gem_tr;
   double threshold_fraction;
   std::string wf_temp_file;

   //int n_valid_counts;
   std::vector<int> fadc_valid_count;

   E16ANA_GTRStripAnalyzer *strip_ana_x;
   E16ANA_GTRStripAnalyzer *strip_ana_y;
   std::vector<E16ANA_GTRStripAnalyzer*> strip_list;

  void SetStripData( E16ANA_GTRStripResponse* strip,
    E16ANA_GTRStripAnalyzer* strip_ana);

};

class E16ANA_GTR100Analyzer;

class E16ANA_GTR200Analyzer;
class E16ANA_GTR300Analyzer;

class E16ANA_GTR100Analyzer : public E16ANA_GTRAnalyzer2 {
private:
   E16ANA_GTRStripAnalyzer *strip_ana_yb;
public:
  E16ANA_GTR100Analyzer(int _n_fadc_chs = 432, int _n_fadc_counts = 21);
  ~E16ANA_GTR100Analyzer();

  //100 chamber has separated Y strip
  //Ya:   0 <(local) x< 50 [mm]
  //Yb: -50 <(local) x< 0  [mm]

  E16ANA_GTRStripAnalyzer* GetStripYa(){return strip_ana_y;};
  E16ANA_GTRStripAnalyzer* GetStripYb(){return strip_ana_yb;};

  void AnalyzeYb();
  void Analyze2Ya(int hitid);//using CalcTdc2 for a hit
  void Analyze2Yb(int hitid);//using CalcTdc2 for a hit
  void Analyze2Y(int hitid, int type);// a(is_y) or b(is_yb): 


  void SetFadc(int ch, double *_fadc);// use this
  void SetFadcYa(int strip_id, double *_fadc);
  void SetFadcYb(int strip_id, double *_fadc);

  void SetParameters(std::string filename);
  void SetPinAssign(std::string filename);

  void SetStripDataYa( E16ANA_GTRStripResponse* strip){
    SetStripData(strip, strip_ana_y);
  }
  void SetStripDataYb( E16ANA_GTRStripResponse* strip){
    SetStripData(strip, strip_ana_yb);
  }


};

class E16ANA_GTR200Analyzer : public E16ANA_GTRAnalyzer2 {
public:
   E16ANA_GTR200Analyzer(int _n_fadc_chs = 720, int _n_fadc_counts = 21) :
      E16ANA_GTRAnalyzer2(576, 144, _n_fadc_chs, _n_fadc_counts)
   {
   };

  void Analyze2Y(int hitid, int type){
    //    std::cerr<<"200 2Y type "<<type<<std::endl;
    E16ANA_GTRAnalyzer2::Analyze2Y(hitid);
  }

};

class E16ANA_GTR300Analyzer : public E16ANA_GTRAnalyzer2 {
public:
   E16ANA_GTR300Analyzer(int _n_fadc_chs = 1080, int _n_fadc_counts = 21) :
      E16ANA_GTRAnalyzer2(864, 216, _n_fadc_chs, _n_fadc_counts)
   {
   };
  void Analyze2Y(int hitid, int type){
    //    std::cerr<<"300 2Y type "<<type<<std::endl;
    E16ANA_GTRAnalyzer2::Analyze2Y(hitid);
  }
};

//-----------------------------------------------------

class E16ANA_ParamManager;
class E16ANA_G4ModuleHitList;
class E16ANA_GTRResponseManager;

class E16ANA_GTRAnalyzedStripHit;

class E16ANA_GTRAnalyzedHit{

private:
  int layerID;
  int moduleID;
  TVector3 gPosition;
  E16ANA_GTRAnalyzedStripHit* xstriphit;
  E16ANA_GTRAnalyzedStripHit* ystriphit;


public:
  E16ANA_GTRAnalyzedHit(){
    layerID = moduleID = -999;
    xstriphit=NULL;
    ystriphit=NULL;
    gPosition = TVector3(0,0,0);
  }

  void SetXstripHit(E16ANA_GTRAnalyzedStripHit* xhit, TVector3& gX);

  void SetYstripHit(E16ANA_GTRAnalyzedStripHit* yhit, TVector3& gY);

  void AppendYstripHit(E16ANA_GTRAnalyzedStripHit* yhit, double y){
    ystriphit = yhit;
    gPosition.SetY(y);
  }

  TVector3& Position(){return gPosition;}
  int LayerID(){return layerID;}
  int ModuleID(){return moduleID;}

  E16ANA_GTRAnalyzedStripHit* XstripHit(){return xstriphit;}
  E16ANA_GTRAnalyzedStripHit* YstripHit(){return ystriphit;}

};



class E16ANA_GTRAnalyzerManager{

public:
  enum{Nmodule = 33};
  E16ANA_GTRAnalyzerManager(E16ANA_ParamManager* iparam);
  E16ANA_GTRAnalyzer2*  ChamberAnalyzer(int layer, int moduleID){
    return anaS[layer][moduleID];
  }

  void G4Analyze(
    int version, 
    E16ANA_G4ModuleHitList& g4moduleHits,
    E16ANA_GTRResponseManager* gtrRes, 
    E16ANA_Geometry* geom
  );

  void G4Analyze(
    int version, 
    E16ANA_G4ModuleHitList& g4moduleHits,
    E16ANA_GTRResponseManager* gtrRes, 
    E16ANA_Geometry* geom,
    std::vector<E16ANA_G4Hit> &gtr1_hits,
    std::vector<E16ANA_G4Hit> &gtr2_hits,
    std::vector<E16ANA_G4Hit> &gtr3_hits
  );

  void G4Analyze(
    E16ANA_G4ModuleHitList& g4moduleHits,
    E16ANA_GTRResponseManager* gtrRes, 
    E16ANA_Geometry* geom
  ){
    G4AnalyzeV0( g4moduleHits, gtrRes, geom);
  }

  void G4AnalyzeV0(    E16ANA_G4ModuleHitList& g4moduleHits,
    E16ANA_GTRResponseManager* gtrRes, 
    E16ANA_Geometry* geom
  ){
  int version = 0;
  G4Analyze( version, g4moduleHits, gtrRes, geom);
  }

  void G4AnalyzeV1(    E16ANA_G4ModuleHitList& g4moduleHits,
    E16ANA_GTRResponseManager* gtrRes, 
    E16ANA_Geometry* geom
  ){
  int version = 1;
  G4Analyze( version, g4moduleHits, gtrRes, geom);
  }

  void G4AnalyzeV2(    E16ANA_G4ModuleHitList& g4moduleHits,
    E16ANA_GTRResponseManager* gtrRes, 
    E16ANA_Geometry* geom
  ){
  int version = 2;
  G4Analyze( version, g4moduleHits, gtrRes, geom);
  }

  void G4AnalyzeV3(    E16ANA_G4ModuleHitList& g4moduleHits,
    E16ANA_GTRResponseManager* gtrRes, 
    E16ANA_Geometry* geom
  ){
    int version = 3;
    G4Analyze( version, g4moduleHits, gtrRes, geom);
  }

  void G4AnalyzeV3(    E16ANA_G4ModuleHitList& g4moduleHits,
    E16ANA_GTRResponseManager* gtrRes, 
    E16ANA_Geometry* geom,
    std::vector<E16ANA_G4Hit> &gtr1_hits,
    std::vector<E16ANA_G4Hit> &gtr2_hits,
    std::vector<E16ANA_G4Hit> &gtr3_hits
  ){
    int version = 3;
    G4Analyze( version, g4moduleHits, gtrRes, geom, gtr1_hits, gtr2_hits, gtr3_hits);
  }

  void G4AnalyzeV4(    E16ANA_G4ModuleHitList& g4moduleHits,
    E16ANA_GTRResponseManager* gtrRes, 
    E16ANA_Geometry* geom,
    std::vector<E16ANA_G4Hit> &gtr1_hits,
    std::vector<E16ANA_G4Hit> &gtr2_hits,
    std::vector<E16ANA_G4Hit> &gtr3_hits
  ){
    int version = 4;
    G4Analyze( version, g4moduleHits, gtrRes, geom, gtr1_hits, gtr2_hits, gtr3_hits);
  }

  std::vector<E16ANA_GTRAnalyzedStripHit> ReAnalyzeV4X(int layer, E16ANA_G4Hit &hit);
  std::vector<E16ANA_GTRAnalyzedStripHit> ReAnalyzeV4Y(int layer, E16ANA_G4Hit &hit, double near_cut);

  // WaveformAnalyze
  void WaveformAnalyze(
    int version, 
    E16ANA_WaveformEvent *event,
    E16ANA_Geometry* geom
  );
  void WaveformAnalyzeV0(E16ANA_WaveformEvent *event, E16ANA_Geometry* geom){
     WaveformAnalyze(0,event,geom);
  };
  void WaveformAnalyzeV1(E16ANA_WaveformEvent *event, E16ANA_Geometry* geom){
     WaveformAnalyze(1,event,geom);
  };
  void WaveformAnalyzeV2(E16ANA_WaveformEvent *event, E16ANA_Geometry* geom){
     WaveformAnalyze(2,event,geom);
  };
  void WaveformAnalyzeV3(E16ANA_WaveformEvent *event, E16ANA_Geometry* geom){
     WaveformAnalyze(3,event,geom);
  };
    //  should be called event-by-event
    //make lists of AnalyzedStripHit(SH) &  AnalyzedHit(AH) from the G4 data
    // Here, x-y correalation is not solved in the AH.
    //  -> only transform to global coordinate from the SH.

  //hit list handler
  std::vector<E16ANA_GTRAnalyzedStripHit*>&  AnalyzedStripGTRX(int i){
    return *(gtrSHitsX[i]); }
  std::vector<E16ANA_GTRAnalyzedStripHit*>&  AnalyzedStripGTRY(int i){
    return *(gtrSHitsY[i]); }// 1Y,2Y,3Y,1Yb
  
  std::vector<E16ANA_GTRAnalyzedStripHit*>& AnalyzedStripGTR1X(){return gtrSHits1X;}
  std::vector<E16ANA_GTRAnalyzedStripHit*>& AnalyzedStripGTR2X(){return gtrSHits2X;}
  std::vector<E16ANA_GTRAnalyzedStripHit*>& AnalyzedStripGTR3X(){return gtrSHits3X;}
  std::vector<E16ANA_GTRAnalyzedStripHit*>& AnalyzedStripGTR1Y(){return gtrSHits1Y;}
  std::vector<E16ANA_GTRAnalyzedStripHit*>& AnalyzedStripGTR1Yb(){return gtrSHits1Yb;}
  std::vector<E16ANA_GTRAnalyzedStripHit*>& AnalyzedStripGTR2Y(){return gtrSHits2Y;}
  std::vector<E16ANA_GTRAnalyzedStripHit*>& AnalyzedStripGTR3Y(){return gtrSHits3Y;}


  //hit list handler
  std::vector<E16ANA_GTRAnalyzedHit*>&  AnalyzedGTRX(int i){
    return *(gtrAHitsX[i]); }
  std::vector<E16ANA_GTRAnalyzedHit*>&  AnalyzedGTRY(int i){
    return *(gtrAHitsY[i]); }// 1Y,2Y,3Y,1Yb
  
  std::vector<E16ANA_GTRAnalyzedHit*>& AnalyzedGTR1X(){return gtrAHits1X;}
  std::vector<E16ANA_GTRAnalyzedHit*>& AnalyzedGTR2X(){return gtrAHits2X;}
  std::vector<E16ANA_GTRAnalyzedHit*>& AnalyzedGTR3X(){return gtrAHits3X;}
  std::vector<E16ANA_GTRAnalyzedHit*>& AnalyzedGTR1Y(){return gtrAHits1Y;}
  std::vector<E16ANA_GTRAnalyzedHit*>& AnalyzedGTR1Yb(){return gtrAHits1Yb;}
  std::vector<E16ANA_GTRAnalyzedHit*>& AnalyzedGTR2Y(){return gtrAHits2Y;}
  std::vector<E16ANA_GTRAnalyzedHit*>& AnalyzedGTR3Y(){return gtrAHits3Y;}

  std::vector<E16ANA_GTRAnalyzedHit>&  AnalyzedHitXChamber(int layer, int k){
    return gtrAHitsXChamber[layer][k];
  }
  std::vector<E16ANA_GTRAnalyzedHit>&  AnalyzedHitYChamber(int layer, int k){
    return gtrAHitsYChamber[layer][k];
  }


private:

  E16ANA_ParamManager* paramMgr;
  E16ANA_GTRAnalyzer2*  ana100[ Nmodule ];
  E16ANA_GTRAnalyzer2*  ana200[ Nmodule ];
  E16ANA_GTRAnalyzer2*  ana300[ Nmodule ];
  E16ANA_GTRAnalyzer2** anaS[3];

  //module-by-module; substance (not pointer ) of AnalyzedHit
  std::vector<E16ANA_GTRAnalyzedHit> gtrAHits1XChamber[Nmodule];
  std::vector<E16ANA_GTRAnalyzedHit> gtrAHits2XChamber[Nmodule];
  std::vector<E16ANA_GTRAnalyzedHit> gtrAHits3XChamber[Nmodule];
  std::vector<E16ANA_GTRAnalyzedHit>* gtrAHitsXChamber[3];

  std::vector<E16ANA_GTRAnalyzedHit> gtrAHits1YChamber[Nmodule];
  std::vector<E16ANA_GTRAnalyzedHit> gtrAHits1YbChamber[Nmodule];
  std::vector<E16ANA_GTRAnalyzedHit> gtrAHits2YChamber[Nmodule];
  std::vector<E16ANA_GTRAnalyzedHit> gtrAHits3YChamber[Nmodule];
  std::vector<E16ANA_GTRAnalyzedHit>* gtrAHitsYChamber[4];


  //layer-wide: only pointer to the Analyzed(Strip)Hit
  std::vector<E16ANA_GTRAnalyzedStripHit*> gtrSHits1X;
  std::vector<E16ANA_GTRAnalyzedStripHit*> gtrSHits2X;
  std::vector<E16ANA_GTRAnalyzedStripHit*> gtrSHits3X;
  std::vector<E16ANA_GTRAnalyzedStripHit*>* gtrSHitsX[3];

  std::vector<E16ANA_GTRAnalyzedStripHit*> gtrSHits1Y;
  std::vector<E16ANA_GTRAnalyzedStripHit*> gtrSHits1Yb;
  std::vector<E16ANA_GTRAnalyzedStripHit*> gtrSHits2Y;
  std::vector<E16ANA_GTRAnalyzedStripHit*> gtrSHits3Y;
  std::vector<E16ANA_GTRAnalyzedStripHit*>* gtrSHitsY[4];

  std::vector<E16ANA_GTRAnalyzedHit*> gtrAHits1X;
  std::vector<E16ANA_GTRAnalyzedHit*> gtrAHits2X;
  std::vector<E16ANA_GTRAnalyzedHit*> gtrAHits3X;
  std::vector<E16ANA_GTRAnalyzedHit*>* gtrAHitsX[3];

  std::vector<E16ANA_GTRAnalyzedHit*> gtrAHits1Y;
  std::vector<E16ANA_GTRAnalyzedHit*> gtrAHits1Yb;
  std::vector<E16ANA_GTRAnalyzedHit*> gtrAHits2Y;
  std::vector<E16ANA_GTRAnalyzedHit*> gtrAHits3Y;
  std::vector<E16ANA_GTRAnalyzedHit*>* gtrAHitsY[4];

  void Clear();//event-by-event



};


#endif // E16ANA_GTRAnalyzer2_hh


