//2016-11-22, uploaded by nakai
//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai
//2015-12-21, uploaded by yokkaich
//2015-12-12, uploaded by nakai
//2015-12-03, uploaded by yokkaich
//2015-11-14, uploaded by nakai
//2015-11-02, uploaded by yokkaich
//2015-11-02, uploaded by komatsu
//2015-10-05, uploaded by yokkaich
//2015-08-20, uploaded by yokkaich
//2015-07-15, uploaded by nakai
//
//E16ANA_GTRAnalyzer2.cc
//    Last modified at <2015-10-05 15:26:06 >
//
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <iterator>
#include <vector>
#include <cstdlib>
#include <map>
#include <algorithm>

#include "E16ANA_ErrorMessage.hh"
#include "E16ANA_ParamManager.hh"
#include "E16ANA_Transform.hh"
#include "E16ANA_Geometry.hh"


#include "E16ANA_G4OutputData.hh"

#include "E16ANA_GTRAnalyzer2.hh"
#include "E16ANA_GTRStripAnalyzer.hh"
#include "E16ANA_GTRResponse.hh"
#include "E16ANA_WaveformOutputData.hh"

using namespace std;

E16ANA_GTRAnalyzer2::E16ANA_GTRAnalyzer2(int _n_strip_x, int _n_strip_y, int _n_fadc_chs, int _n_fadc_counts) : 
   n_strip_x(_n_strip_x), n_strip_y(_n_strip_y),
   n_fadc_chs(_n_fadc_chs), n_fadc_counts(_n_fadc_counts)
{
   strip_num = new int[n_fadc_chs];

	for(int i=0; i<n_fadc_chs; i++){
		strip_num[i]= i;
	}

	strip_pitch_x= 0.35;
	strip_pitch_y= 0.35;
	gem_th_x= 4.0;
	gem_th_y= 4.0;
	drift_velocity= 0.015;
	drift_gap_center= 0.0;
	drift_gap_center_v3= 0.0;
	fadc_clock_period= 10.0;
	fadc_t0_correction= 0.0;
	tdc_lsb= 0.035;
	gem_t0_min= -10000.0;
	gem_t0_max= 10000.0;
	gem_tr= 75.0;
	threshold_fraction= 0.5;
	fadc_valid_count.reserve(n_fadc_counts);
	for(int i=0; i<n_fadc_counts; i++){
		fadc_valid_count.push_back(i);
	}
   strip_ana_x = new E16ANA_GTRStripAnalyzer(n_strip_x, n_fadc_counts);
   strip_ana_x->SetInverse(false);
   strip_ana_y = new E16ANA_GTRStripAnalyzer(n_strip_y, n_fadc_counts);
   strip_ana_y->SetInverse(true);
   strip_list.push_back(strip_ana_x);
   strip_list.push_back(strip_ana_y);
}

E16ANA_GTRAnalyzer2::~E16ANA_GTRAnalyzer2(){
   delete [] strip_num;
   delete strip_ana_x;
   delete strip_ana_y;
}

void E16ANA_GTRAnalyzer2::Clear(){
   //strip_ana_x->Clear();
   //strip_ana_y->Clear();
   for(int i=0; i<(int)strip_list.size(); i++){
      strip_list[i]->Clear();
   }
}

void E16ANA_GTRAnalyzer2::SetParameters(std::string filename){
	//NConfReader &gConf= NConfReader::GetInstance();
	//std::string filename= gConf.GetFileName("GEMPARAM:");
	std::cout<< "[E16ANA_GTRAnalyzer2::SetParameters()]: filename = "<< filename<< std::endl;
	std::ifstream ifs(filename.c_str());

	if(ifs.fail()){
		std::cout<< "\t ParamFile open fail"<< std::endl;
		return;
	}

	std::map<std::string, double> parameter_list;

	while(ifs.good()){
		std::string buf;
		std::getline(ifs, buf);
		if(buf[0]=='#' || buf.empty()){
			continue;
		}
		std::istringstream is(buf);
		std::istream_iterator<std::string> issBegin(is);
		std::istream_iterator<std::string> issEnd;
		std::vector<std::string> param(issBegin, issEnd);
		if(param.empty() || param[0].empty()){
			continue;
		}
      if(param[0] == "WAVEFORM_TEMPLATE_FILE:"){
         wf_temp_file = param[1];
         continue;
      }
		double parameter= atof(param[1].c_str());
		parameter_list[param[0]]= parameter;
		//double pedestal= atof(param[1].c_str());
		//double sigma= atof(param[2].c_str());
	}
	ifs.close();
	strip_pitch_x= parameter_list["PITCH_X:"];
	strip_pitch_y= parameter_list["PITCH_Y:"];
	gem_th_x= parameter_list["TH_X:"];
	gem_th_y= parameter_list["TH_Y:"];
	gem_t0_min= parameter_list["TDC_MIN:"];
	gem_t0_max= parameter_list["TDC_MAX:"];
	drift_velocity= parameter_list["DRIFT_VELOCITY:"];
	drift_gap_center= parameter_list["DRIFT_CENTER:"]*drift_velocity;
	drift_gap_center_v3= parameter_list["DRIFT_CENTER_2:"]*drift_velocity;
	fadc_clock_period= parameter_list["FADC_CLOCK_PERIOD:"];
	tdc_lsb= parameter_list["TDC_LSB:"];

   x_start = -(double)n_strip_x/2.0*strip_pitch_x+strip_pitch_x*0.5;
   y_start = -(double)n_strip_y/2.0*strip_pitch_y+strip_pitch_y*0.5;

/*
	if(parameter_list["TDC_THRESHOLD_FRACTION"]>0.0){
		threshold_fraction= parameter_list["TDC_THRESHOLD_FRACTION:"];
	}
*/

   strip_ana_x->strip_pitch = strip_pitch_x;
   strip_ana_x->gem_threshold = gem_th_x;
   strip_ana_x->gem_tdc_min = gem_t0_min;
   strip_ana_x->gem_tdc_max = gem_t0_max;
   strip_ana_x->drift_velocity = drift_velocity;
   strip_ana_x->drift_gap_center = drift_gap_center;
   strip_ana_x->drift_gap_center_v3 = drift_gap_center_v3;
   strip_ana_x->fadc_clock_period = fadc_clock_period;
   strip_ana_x->position_start = x_start;
   strip_ana_x->gem_tr = gem_tr;
   strip_ana_x->threshold_fraction = threshold_fraction;
   strip_ana_x->SetWaveformTemplate(wf_temp_file);

   strip_ana_y->strip_pitch = strip_pitch_y;
   strip_ana_y->gem_threshold = gem_th_y;
   strip_ana_y->gem_tdc_min = gem_t0_min;
   strip_ana_y->gem_tdc_max = gem_t0_max;
   strip_ana_y->drift_velocity = drift_velocity;
   strip_ana_y->drift_gap_center = drift_gap_center;
   strip_ana_y->drift_gap_center_v3 = drift_gap_center_v3;
   strip_ana_y->fadc_clock_period = fadc_clock_period;
   strip_ana_y->position_start = y_start;
   strip_ana_y->gem_tr = gem_tr;
   strip_ana_y->threshold_fraction = threshold_fraction;
   strip_ana_y->SetWaveformTemplate(wf_temp_file);

}

void E16ANA_GTRAnalyzer2::PrintParameters(){
   std::cout << "E16ANA_GTRAnalyzer2::PrintParameters : "
      << "strip_pitch_x = " << strip_pitch_x << " [mm]"
      << ", strip_pitch_y = " << strip_pitch_y << " [mm]"
      << ", gem_th_x = " << gem_th_x
      << ", gem_th_y = " << gem_th_y
      << ", gem_t0_min = " << gem_t0_min << " [ns]"
      << ", gem_t0_max = " << gem_t0_max << " [ns]"
      << ", drift_velocity = " << drift_velocity << " [mm/ns]"
      << ", drift_gap_center = " << drift_gap_center << " [mm]"
      << ", drift_gap_center_v3 = " << drift_gap_center_v3 << " [mm]"
      << ", fadc_clock_period = " << fadc_clock_period << " [ns]"
      << std::endl;
}

void E16ANA_GTRAnalyzer2::SetPinAssign(std::string filename){
	//NConfReader &gConf= NConfReader::GetInstance();
	//std::string filename= gConf.GetFileName("FADCGEO:");
	std::cout<< "[E16ANA_GTRAnalyzer2::SetPinAssign()]: filename = "<< filename<< std::endl;
	std::ifstream ifs(filename.c_str());

	if(ifs.fail()){
		std::cout<< "\t PinAssignFile open fail"<< std::endl;
		return;
	}

	for(int ch=0; ch<n_fadc_chs; ch++){
		strip_num[ch]= -1;
	}

	while(ifs.good()){
		std::string buf;
		std::getline(ifs, buf);
		if(buf[0]=='#' || buf.empty()){
			continue;
		}
		std::istringstream is(buf);
		std::istream_iterator<std::string> issBegin(is);
		std::istream_iterator<std::string> issEnd;
		std::vector<std::string> param(issBegin, issEnd);
		if(param.empty() || param[0].empty()){
			continue;
		}
		int ch= atoi(param[0].c_str());
		if(ch>=n_fadc_chs){
			std::cout << "\t Channel exceeds the maximum of FADC channel ID." << std::endl;
			continue;
		}
		//double pedestal= atof(param[1].c_str());
		//double sigma= atof(param[2].c_str());
		if(param[1]=="x"){
			strip_num[ch]= atoi(param[2].c_str());
		}else if(param[1]=="y"){
			strip_num[ch]= atoi(param[2].c_str())+n_strip_x;
		}else{
			strip_num[ch]= -1;
		}
	}
/*
	for(int ch=0; ch<n_fadc_chs; ch++){
		std::cout<< "strip_num["<< ch<< "] = "<< strip_num[ch]<< std::endl;
	}
*/
	ifs.close();
}

void E16ANA_GTRAnalyzer2::SetFadcValidCount(std::string filename){
	//NConfReader &gConf= NConfReader::GetInstance();
	//std::string filename= gConf.GetFileName("FADCVALID:");
	//std::cout<< "[E16ANA_GTRAnalyzer2::SetFadcValidCount()] "<< filename<< std::endl;
	std::ifstream ifs(filename.c_str());

	if(ifs.fail()){
		std::cout<< "File open fail"<< std::endl;
		return;
	}

	fadc_valid_count.clear();
	fadc_valid_count.reserve(n_fadc_counts);
	while(ifs.good()){
		std::string buf;
		std::getline(ifs, buf);
		if(buf[0]=='#' || buf.empty()){
			continue;
		}
		std::istringstream is(buf);
		std::istream_iterator<std::string> issBegin(is);
		std::istream_iterator<std::string> issEnd;
		std::vector<std::string> param(issBegin, issEnd);
		if(param.empty() || param[0].empty()){
			continue;
		}
		for(int i=0; i<(int)param.size(); i++){
			fadc_valid_count.push_back(atoi(param[i].c_str()));
		}
	}
/*
	for(int ch=0; ch<n_fadc_chs; ch++){
		std::cout<< "strip_num["<< ch<< "] = "<< strip_num[ch]<< std::endl;
	}
*/
	ifs.close();
}

void E16ANA_GTRAnalyzer2::SetFadc(int ch, double *_fadc){
	if(strip_num[ch]<n_strip_x && strip_num[ch]>-1){
      strip_ana_x->SetFadc(strip_num[ch], _fadc);
	}else if(strip_num[ch]>=n_strip_x && strip_num[ch]<n_strip_x+n_strip_y){
      strip_ana_y->SetFadc(strip_num[ch]-n_strip_x, _fadc);
	}
}



void E16ANA_GTRAnalyzer2::SetFadcX(int strip_id, double *_fadc){
   strip_ana_x->SetFadc(strip_id, _fadc);
}

void E16ANA_GTRAnalyzer2::SetFadcY(int strip_id, double *_fadc){
   strip_ana_y->SetFadc(strip_id, _fadc);
}

void E16ANA_GTRAnalyzer2::SetStripData( E16ANA_GTRStripResponse* strip,
  E16ANA_GTRStripAnalyzer* strip_ana){

  std::vector< std::vector<double> > waveforms;

  int size = strip->GetNumHitStrips();
  waveforms.resize( size );
      
  for(int i=0; i< size; i++){
    std::vector<int> &ww = strip -> GetWaveform(i);
    waveforms[i].resize((int)ww.size());
    int ch = strip -> GetChannel(i);
    for(int j=0; j<(int)ww.size(); j++){
      waveforms[i][j] = ww[j];
    }//for j
    strip_ana -> SetFadc(ch, &waveforms[i][0]);
  }//for i

}



void E16ANA_GTRAnalyzer2::Analyze(){
   //strip_ana_x->Analyze();
   //strip_ana_y->Analyze();
   for(int i=0; i<(int)strip_list.size(); i++){
      strip_list[i]->Analyze();
   }
}
void E16ANA_GTRAnalyzer2::Analyze2X(int hitid){
   strip_ana_x->Analyze2(hitid);
}
void E16ANA_GTRAnalyzer2::Analyze2Y(int hitid, int strip_type){
   //strip_ana_y->Analyze2(hitid);
   strip_list[strip_type]->Analyze2(hitid);
}

void E16ANA_GTRAnalyzer2::AnalyzeV1(){
   for(int i=0; i<(int)strip_list.size(); i++){
      strip_list[i]->AnalyzeV1();
   }
}

void E16ANA_GTRAnalyzer2::AnalyzeV2(){
   for(int i=0; i<(int)strip_list.size(); i++){
      strip_list[i]->AnalyzeV2();
   }
}
void E16ANA_GTRAnalyzer2::AnalyzeV2X(int hitid){
   strip_ana_x->AnalyzeV2(hitid);
}
void E16ANA_GTRAnalyzer2::AnalyzeV2Y(int hitid, int strip_type){
   //strip_ana_y->AnalyzeV2(hitid);
   strip_list[strip_type]->AnalyzeV3(hitid);
}

void E16ANA_GTRAnalyzer2::AnalyzeV3(){
   for(int i=0; i<(int)strip_list.size(); i++){
      strip_list[i]->AnalyzeV3();
   }
}

void E16ANA_GTRAnalyzer2::AnalyzeV3(const TVector3 &track_lpos){
   strip_list[0]->AnalyzeV3(track_lpos.X()); // X-strip
   for(int i=1; i<(int)strip_list.size(); i++){
      strip_list[i]->AnalyzeV3(track_lpos.Y()); // Y-strip or (Ya and Yb-strip)
   }
}

void E16ANA_GTRAnalyzer2::AnalyzeV3X(int hitid){
   strip_ana_x->AnalyzeV3(hitid);
}
void E16ANA_GTRAnalyzer2::AnalyzeV3Y(int hitid, int strip_type){
   //strip_ana_y->AnalyzeV3(hitid);
   strip_list[strip_type]->AnalyzeV3(hitid);
}

void E16ANA_GTRAnalyzer2::AnalyzeV4(const TVector3 &track_lpos, const TVector3 &track_lmom){
   strip_list[0]->AnalyzeV4(track_lpos.X(), track_lmom.X()/track_lmom.Z()); // X-strip
   for(int i=1; i<(int)strip_list.size(); i++){
      strip_list[i]->AnalyzeV4(track_lpos.Y(), track_lmom.Y()/track_lmom.Z()); // Y-strip or (Ya and Yb-strip)
   }
}
/* ---- E16ANA_GTR100Analyzer ---- */

//E16ANA_GTR100Analyzer::E16ANA_GTR100Analyzer(int _n_fadc_chs = 432, int _n_fadc_counts = 30) : 
E16ANA_GTR100Analyzer::E16ANA_GTR100Analyzer(int _n_fadc_chs, int _n_fadc_counts) : 
   E16ANA_GTRAnalyzer2(288, 72, _n_fadc_chs, _n_fadc_counts)
{
   strip_ana_yb = new E16ANA_GTRStripAnalyzer(n_strip_y, n_fadc_counts);
   strip_ana_yb->SetInverse(false);
   strip_list.push_back(strip_ana_yb);
}

E16ANA_GTR100Analyzer::~E16ANA_GTR100Analyzer(){
   delete strip_ana_yb;
}

void E16ANA_GTR100Analyzer::SetParameters(std::string filename){
   E16ANA_GTRAnalyzer2::SetParameters(filename);
   strip_ana_yb->strip_pitch = strip_pitch_y;
   strip_ana_yb->gem_threshold = gem_th_y;
   strip_ana_yb->gem_tdc_min = gem_t0_min;
   strip_ana_yb->gem_tdc_max = gem_t0_max;
   strip_ana_yb->drift_velocity = drift_velocity;
   strip_ana_yb->drift_gap_center = drift_gap_center;
   strip_ana_yb->drift_gap_center_v3 = drift_gap_center_v3;
   strip_ana_yb->fadc_clock_period = fadc_clock_period;
   strip_ana_yb->position_start = y_start;
   strip_ana_yb->gem_tr = gem_tr;
   strip_ana_yb->threshold_fraction = threshold_fraction;
   strip_ana_yb->SetWaveformTemplate(wf_temp_file);
}


void E16ANA_GTR100Analyzer::SetFadcYa(int strip_id, double *_fadc){
   strip_ana_y->SetFadc(strip_id, _fadc);
}

void E16ANA_GTR100Analyzer::SetFadcYb(int strip_id, double *_fadc){
   strip_ana_yb->SetFadc(strip_id, _fadc);
}
void E16ANA_GTR100Analyzer::AnalyzeYb(){
    strip_ana_yb->Analyze();
}

void E16ANA_GTR100Analyzer::Analyze2Ya(int hitid){
  strip_ana_y->Analyze2(hitid);
}
void E16ANA_GTR100Analyzer::Analyze2Yb(int hitid){
  strip_ana_yb->Analyze2(hitid);
}
void E16ANA_GTR100Analyzer::Analyze2Y(int hitid,int type){
  //  cerr<<"100 2Y type "<<type<<endl;
  if( type == E16ANA_GTRAnalyzedStripHit::is_y ){
    strip_ana_y->Analyze2(hitid);
  }
  else if ( type == E16ANA_GTRAnalyzedStripHit::is_yb ){
    strip_ana_yb->Analyze2(hitid);
  }
  else{
    E16WARNING("type error: ",type);
  }
}

void E16ANA_GTR100Analyzer::SetFadc(int ch, double *_fadc){
	if(strip_num[ch]<n_strip_x && strip_num[ch]>-1){
      strip_ana_x->SetFadc(strip_num[ch], _fadc);
	}else if(strip_num[ch]>=n_strip_x && strip_num[ch]<n_strip_x+n_strip_y){
      strip_ana_y->SetFadc(strip_num[ch]-n_strip_x, _fadc);
	}else if(strip_num[ch]>=n_strip_x+n_strip_y && strip_num[ch]<n_strip_x+n_strip_y+n_strip_y){
      strip_ana_yb->SetFadc(strip_num[ch]-n_strip_x-n_strip_y, _fadc);
	}
}

void E16ANA_GTR100Analyzer::SetPinAssign(std::string filename){
	//NConfReader &gConf= NConfReader::GetInstance();
	//std::string filename= gConf.GetFileName("FADCGEO:");
	std::cout<< "[E16ANA_GTRAnalyzer2::SetPinAssign()]: filename = "<< filename<< std::endl;
	std::ifstream ifs(filename.c_str());

	if(ifs.fail()){
		std::cout<< "\t PinAssignFile open fail"<< std::endl;
		return;
	}

	for(int ch=0; ch<n_fadc_chs; ch++){
		strip_num[ch]= -1;
	}

	while(ifs.good()){
		std::string buf;
		std::getline(ifs, buf);
		if(buf[0]=='#' || buf.empty()){
			continue;
		}
		std::istringstream is(buf);
		std::istream_iterator<std::string> issBegin(is);
		std::istream_iterator<std::string> issEnd;
		std::vector<std::string> param(issBegin, issEnd);
		if(param.empty() || param[0].empty()){
			continue;
		}
		int ch= atoi(param[0].c_str());
		if(ch>=n_fadc_chs){
			std::cout << "\t Channel exceeds the maximum of FADC channel ID." << std::endl;
			continue;
		}
		//double pedestal= atof(param[1].c_str());
		//double sigma= atof(param[2].c_str());
		if(param[1]=="x"){
			strip_num[ch]= atoi(param[2].c_str());
		}else if(param[1]=="y" || param[1]=="ya"){
			strip_num[ch]= atoi(param[2].c_str())+n_strip_x;
		}else if(param[1]=="yb"){
			strip_num[ch]= atoi(param[2].c_str())+n_strip_x+n_strip_y;
		}else{
			strip_num[ch]= -1;
		}
	}
	ifs.close();
}





//------------------------------------------

void E16ANA_GTRAnalyzedHit::SetXstripHit(
  E16ANA_GTRAnalyzedStripHit* xhit, TVector3& gX){
    xstriphit = xhit;
    gPosition =  gX;
    layerID   = xhit->LayerID();
    moduleID  = xhit->ModuleID();
}


void E16ANA_GTRAnalyzedHit::SetYstripHit(
  E16ANA_GTRAnalyzedStripHit* yhit, TVector3& gY){
    ystriphit = yhit;
    gPosition =  gY;
    layerID   = yhit->LayerID();
    moduleID  = yhit->ModuleID();
}

//-----------------------------------------------------
E16ANA_GTRAnalyzerManager::E16ANA_GTRAnalyzerManager(
  E16ANA_ParamManager* iparam): paramMgr(iparam) {

  anaS[0] = ana100;
  anaS[1] = ana200;
  anaS[2] = ana300;

  gtrAHitsXChamber[0] =  gtrAHits1XChamber; //array of vector
  gtrAHitsXChamber[1] =  gtrAHits2XChamber; 
  gtrAHitsXChamber[2] =  gtrAHits3XChamber; 
  gtrAHitsYChamber[0] =  gtrAHits1YChamber; 
  gtrAHitsYChamber[1] =  gtrAHits2YChamber; 
  gtrAHitsYChamber[2] =  gtrAHits3YChamber;
  gtrAHitsYChamber[3] =  gtrAHits1YbChamber;

  gtrSHitsX[0]= &gtrSHits1X;
  gtrSHitsX[1]= &gtrSHits2X;
  gtrSHitsX[2]= &gtrSHits3X;
  gtrSHitsY[0]= &gtrSHits1Y;
  gtrSHitsY[1]= &gtrSHits2Y;
  gtrSHitsY[2]= &gtrSHits3Y;
  gtrSHitsY[3]= &gtrSHits1Yb;

  gtrAHitsX[0]= &gtrAHits1X;
  gtrAHitsX[1]= &gtrAHits2X;
  gtrAHitsX[2]= &gtrAHits3X;
  gtrAHitsY[0]= &gtrAHits1Y;
  gtrAHitsY[1]= &gtrAHits2Y;
  gtrAHitsY[2]= &gtrAHits3Y;
  gtrAHitsY[3]= &gtrAHits1Yb;

  for(int i = 0; i< Nmodule ; i++ ){

    ana100[i] = new E16ANA_GTR100Analyzer();
    ana100[i]->SetParameters( paramMgr->GetGTR100AnalysisParamFileName(i)  );
    ana200[i] = new E16ANA_GTR200Analyzer();
    ana200[i]->SetParameters(  paramMgr->GetGTR200AnalysisParamFileName(i)  );
    ana300[i] = new E16ANA_GTR300Analyzer();
    ana300[i]->SetParameters(  paramMgr->GetGTR300AnalysisParamFileName(i)  );

  }//for i

  //  E16MESSAGE("end");
}

class E16ANA_G4Hit;



void E16ANA_GTRAnalyzerManager::G4Analyze( 
  int version,
  E16ANA_G4ModuleHitList& g4moduleHits,
  E16ANA_GTRResponseManager* gtrRes,
  E16ANA_Geometry* geom
){

   //   std::vector<int> waveforms_ch;

  //   std::cerr<<" ---------------------"<<std::endl;
  //  g4moduleHits.HitPrint();

  E16INFO("clustering version V%d",version);

  this->Clear();

  for (int k=0; k< Nmodule ;k++){   //module-by-module

    E16ANA_GTR100Response* gtr1 = gtrRes->Response100(k);
    E16ANA_GTR200Response* gtr2 = gtrRes->Response200(k);
    E16ANA_GTR300Response* gtr3 = gtrRes->Response300(k);
    E16ANA_GTRResponse* gtrResS[3]={gtr1, gtr2, gtr3};

    for( int layer = 0; layer < 3 ;  layer ++){
      E16ANA_GTRResponse*  res = gtrResS[ layer ];
      //      cout << "layer, k = " << layer << " " << k << endl;
      res ->  SetTracks( g4moduleHits.GTRModule(layer, k) );

      E16ANA_GTRAnalyzer2* ana = anaS[ layer ][k];
      ana->SetStripDataX( res->GetStripX() );
      ana->SetStripDataY( res->GetStripY() );
      if(layer==0){
         E16ANA_GTR100Analyzer* ana1 = (E16ANA_GTR100Analyzer*) anaS[ 0 ][k];
         ana1->SetStripDataYb( gtr1->GetStripYb() );
      }

      if( version == 0 ){
         ana->AnalyzeV0();  //for X, Y, Yb, ClusteringV0 and TDC method 1
      }
      else if ( version == 1 ){
         ana->AnalyzeV1();// for X, Y, Yb, ClusteringV1 and TDC method 1
      }
      else if ( version == 2 ){
         ana->AnalyzeV2();// Waveform 1D fit and HitClusteringV2
      }
      else if ( version == 3 ){
         ana->AnalyzeV3();//  Waveform 2D fit and HitClusteringV3
      }
      else{
         E16FATAL("version unmatch : %d",version);
         exit(1);
      }

      vector<E16ANA_GTRAnalyzedStripHit> & sx=
	ana ->GetStripX()->GetAnalyzedHits();
      vector<E16ANA_GTRAnalyzedStripHit> & sy=
	ana ->GetStripY()->GetAnalyzedHits();

      //      E16INFO("size x,y = %d %d : %d , %d", layer, k, sx.size(), sy.size());

      for(int i=0;i<(int)sx.size();i++){
	E16ANA_GTRAnalyzedStripHit& shit = sx[i];
	shit.SetLayerAndModuleIDandType(layer, k, shit.is_x);

	E16ANA_GTRAnalyzedHit ahit;
	TVector3 local( shit.CogHit()*0.1  ,0,0);//mm->cm in 
	//TVector3 gX = 	E16ANA_Transform::GetGPos(local, geom, 
	//  layer, shit.ModuleID() ) * 10;//cm->mm
   TVector3 gX = geom->GetGPos(local, layer, shit.ModuleID())*10.0;
	ahit.SetXstripHit(&shit, gX);

	gtrAHitsXChamber[layer][k].push_back(ahit);//copy
	gtrAHitsX[layer]->push_back( & (gtrAHitsXChamber[layer][k].back() ) );
	gtrSHitsX[layer]->push_back(&shit);
      }

      for(int i=0;i<(int)sy.size();i++){
	E16ANA_GTRAnalyzedStripHit& shit = sy[i];
	shit.SetLayerAndModuleIDandType(layer, k, shit.is_y);
	E16ANA_GTRAnalyzedHit ahit;

	TVector3 local( 0, shit.CogHit()*0.1  ,0);//mm->cm in 
	//TVector3 gY = 	E16ANA_Transform::GetGPos(local, geom, 
	//  layer, shit.ModuleID() ) * 10;//cm->mm
   TVector3 gY = geom->GetGPos(local, layer, shit.ModuleID())*10.0;
	ahit.SetYstripHit(&shit, gY);

	gtrAHitsYChamber[layer][k].push_back(ahit);//copy
	gtrAHitsY[layer]->push_back(& (gtrAHitsYChamber[layer][k].back() ) );
	gtrSHitsY[layer]->push_back(&shit);
      }

    }//for layer

    //Yb in only 100-----------------------------
    E16ANA_GTR100Analyzer* ana1 = (E16ANA_GTR100Analyzer*) anaS[ 0 ][k];
    //ana1->SetStripDataYb( gtr1->GetStripYb() );
    //ana1->AnalyzeYb();
    vector<E16ANA_GTRAnalyzedStripHit> & syb=	  
    ana1 ->GetStripYb()->GetAnalyzedHits();

    for(int i=0;i<(int)syb.size();i++){
	E16ANA_GTRAnalyzedStripHit& shit =  syb[i];
	shit.SetLayerAndModuleIDandType(0, k, shit.is_yb); //100 means layer=0

	E16ANA_GTRAnalyzedHit ahit;
	//	TVector3 local( 0, shit.TdcHit()*0.1  ,0);//mm->cm in 
	TVector3 local( 0, shit.CogHit()*0.1  ,0);//mm->cm in 
	//TVector3 gY = 	E16ANA_Transform::GetGPos(local, geom, 
	//  0, shit.ModuleID() ) * 10;//cm->mm
   TVector3 gY = geom->GetGPos(local, 0, shit.ModuleID())*10.0;
	ahit.SetYstripHit(&shit, gY);

	gtrAHitsYChamber[0][k].push_back(ahit);//copy: 
	       //analyzedHitChamber has only 3-layer: Yb appended to Y[0]
	gtrAHits1Yb.push_back(& (gtrAHitsYChamber[0][k].back() ) );
	gtrSHits1Yb.push_back(&shit);//only pointer
    }

  }//k:module


}

void E16ANA_GTRAnalyzerManager::G4Analyze( 
  int version,
  E16ANA_G4ModuleHitList& g4moduleHits,
  E16ANA_GTRResponseManager* gtrRes,
  E16ANA_Geometry* geom,
  std::vector<E16ANA_G4Hit> &gtr1_hits,
  std::vector<E16ANA_G4Hit> &gtr2_hits,
  std::vector<E16ANA_G4Hit> &gtr3_hits
){

   //   std::vector<int> waveforms_ch;

  //   std::cerr<<" ---------------------"<<std::endl;
  //  g4moduleHits.HitPrint();

  E16INFO("clustering version V%d",version);

  this->Clear();

  // SetStripData
  for (int k=0; k< Nmodule ;k++){   //module-by-module
    E16ANA_GTR100Response* gtr1 = gtrRes->Response100(k);
    E16ANA_GTR200Response* gtr2 = gtrRes->Response200(k);
    E16ANA_GTR300Response* gtr3 = gtrRes->Response300(k);
    E16ANA_GTRResponse* gtrResS[3]={gtr1, gtr2, gtr3};
    for( int layer = 0; layer < 3 ;  layer ++){
      E16ANA_GTRResponse*  res = gtrResS[ layer ];
      res ->  SetTracks( g4moduleHits.GTRModule(layer, k) );
      E16ANA_GTRAnalyzer2* ana = anaS[ layer ][k];
      ana->SetStripDataX( res->GetStripX() );
      ana->SetStripDataY( res->GetStripY() );
      if(layer==0){
         E16ANA_GTR100Analyzer* ana1 = (E16ANA_GTR100Analyzer*) anaS[ 0 ][k];
         ana1->SetStripDataYb( gtr1->GetStripYb() );
      }
    }
  }
  // SetStripData ends.

  std::vector<E16ANA_G4Hit> *gtr_hits[3] = {&gtr1_hits, &gtr2_hits, &gtr3_hits}; 
  for(int layer=0; layer<3; layer++){
    std::vector<E16ANA_G4Hit> &hits = *(gtr_hits[layer]);
    for(int k=0; k<hits.size(); k++){
      E16ANA_GTRAnalyzer2* ana = anaS[layer][hits[k].DetectorID()];
      if( version == 0 ){
         ana->AnalyzeV0();  //for X, Y, Yb, ClusteringV0 and TDC method 1
      }
      else if ( version == 1 ){
         ana->AnalyzeV1();// for X, Y, Yb, ClusteringV1 and TDC method 1
      }
      else if ( version == 2 ){
         ana->AnalyzeV2();// Waveform 1D fit and HitClusteringV2
      }
      else if ( version == 3 ){
         //ana->AnalyzeV3(hits[k].XTV()*10.0);//  Waveform 2D fit and HitClusteringV3, cm -> mm
         ana->AnalyzeV3(hits[k].XTV());//  Waveform 2D fit and HitClusteringV3, cm -> mm
      }
      else if ( version == 4 ){
         ana->AnalyzeV4(hits[k].XTV(), hits[k].PTV());//  Waveform 2D fit and HitClusteringV3, cm -> mm
      }
      else{
         E16FATAL("version unmatch : %d",version);
         exit(1);
      }

      vector<E16ANA_GTRAnalyzedStripHit> & sx=
      ana ->GetStripX()->GetAnalyzedHits();
      vector<E16ANA_GTRAnalyzedStripHit> & sy=
      ana ->GetStripY()->GetAnalyzedHits();

      //      E16INFO("size x,y = %d %d : %d , %d", layer, k, sx.size(), sy.size());

      for(int i=0;i<(int)sx.size();i++){
         E16ANA_GTRAnalyzedStripHit& shit = sx[i];
         shit.SetLayerAndModuleIDandType(layer, hits[k].DetectorID(), shit.is_x);
         E16ANA_GTRAnalyzedHit ahit;
         TVector3 local( shit.CogHit()*0.1  ,0,0);//mm->cm in 
         //TVector3 gX = E16ANA_Transform::GetGPos(local, geom, layer, shit.ModuleID() ) * 10;//cm->mm
         TVector3 gX = geom->GetGPos(local, layer, shit.ModuleID() ) * 10;//cm->mm
         ahit.SetXstripHit(&shit, gX);

         gtrAHitsXChamber[layer][hits[k].DetectorID()].push_back(ahit);//copy
         gtrAHitsX[layer]->push_back( & (gtrAHitsXChamber[layer][hits[k].DetectorID()].back() ) );
         gtrSHitsX[layer]->push_back(&shit);
      }

      for(int i=0;i<(int)sy.size();i++){
         E16ANA_GTRAnalyzedStripHit& shit = sy[i];
         shit.SetLayerAndModuleIDandType(layer, hits[k].DetectorID(), shit.is_y);
         E16ANA_GTRAnalyzedHit ahit;
         TVector3 local( 0, shit.CogHit()*0.1  ,0);//mm->cm in 
         //TVector3 gY = E16ANA_Transform::GetGPos(local, geom, layer, shit.ModuleID() ) * 10;//cm->mm
         TVector3 gY = geom->GetGPos(local, layer, shit.ModuleID() ) * 10;//cm->mm
         ahit.SetYstripHit(&shit, gY);
         gtrAHitsYChamber[layer][hits[k].DetectorID()].push_back(ahit);//copy
         gtrAHitsY[layer]->push_back(& (gtrAHitsYChamber[layer][hits[k].DetectorID()].back() ) );
         gtrSHitsY[layer]->push_back(&shit);
      }

      if(layer == 0){
        //Yb in only 100-----------------------------
        E16ANA_GTR100Analyzer* ana1 = (E16ANA_GTR100Analyzer*) anaS[ 0 ][hits[k].DetectorID()];
        //ana1->SetStripDataYb( gtr1->GetStripYb() );
        //ana1->AnalyzeYb();
        vector<E16ANA_GTRAnalyzedStripHit> & syb = ana1->GetStripYb()->GetAnalyzedHits();
        for(int i=0;i<(int)syb.size();i++){
          E16ANA_GTRAnalyzedStripHit& shit =  syb[i];
          shit.SetLayerAndModuleIDandType(0, hits[k].DetectorID(), shit.is_yb); //100 means layer=0
          E16ANA_GTRAnalyzedHit ahit;
          //	TVector3 local( 0, shit.TdcHit()*0.1  ,0);//mm->cm in 
          TVector3 local( 0, shit.CogHit()*0.1  ,0);//mm->cm in 
          //TVector3 gY = E16ANA_Transform::GetGPos(local, geom, 0, shit.ModuleID() ) * 10;//cm->mm
          TVector3 gY = geom->GetGPos(local, 0, shit.ModuleID() ) * 10;//cm->mm
          ahit.SetYstripHit(&shit, gY);
          gtrAHitsYChamber[0][hits[k].DetectorID()].push_back(ahit);//copy:
          //analyzedHitChamber has only 3-layer: Yb appended to Y[0]
          gtrAHits1Yb.push_back(& (gtrAHitsYChamber[0][hits[k].DetectorID()].back() ) );
          gtrSHits1Yb.push_back(&shit);//only pointer
        }
      }
    }//for hits
  }//layer
}

void E16ANA_GTRAnalyzerManager::WaveformAnalyze( 
  int version,
  E16ANA_WaveformEvent *event,
  E16ANA_Geometry* geom
){

   //   std::vector<int> waveforms_ch;

  //   std::cerr<<" ---------------------"<<std::endl;
  //  g4moduleHits.HitPrint();

  E16INFO("clustering version V%d",version);

  this->Clear();

  for (int k=0; k< Nmodule ;k++){   //module-by-module
    for( int layer = 0; layer < 3 ;  layer ++){
      E16ANA_GTRAnalyzer2* ana = anaS[ layer ][k];
      int noh = event->GetNumHits(layer, k);
      for(int i=0; i<noh; i++){
         int ch = event->GetChannel(layer, k, i);
         std::vector<int> &wave = event->GetWaveform(layer, k, i);
         std::vector<double> waved((int)wave.size());
         for(int s=0; s<(int)wave.size(); s++){
            waved[s] = (double)wave[s];
         }
         ana->SetFadc(ch, &waved[0]);
      }

      if( version == 0 ){
	ana->AnalyzeV0();  //for X, Y, Yb, ClusteringV0 and TDC method 1
      }
      else if ( version == 1 ){
	ana->AnalyzeV1();// for X, Y, Yb, ClusteringV1 and TDC method 1
      }
      else if ( version == 2 ){
	ana->AnalyzeV2();// Waveform 1D fit and HitClusteringV2
      }
      else if ( version == 3 ){
	ana->AnalyzeV3();//  Waveform 2D fit and HitClusteringV3
      }
      else{
	E16FATAL("version unmatch : %d",version);
	exit(1);
      }

      vector<E16ANA_GTRAnalyzedStripHit> & sx=
	ana ->GetStripX()->GetAnalyzedHits();
      vector<E16ANA_GTRAnalyzedStripHit> & sy=
	ana ->GetStripY()->GetAnalyzedHits();

      //      E16INFO("size x,y = %d %d : %d , %d", layer, k, sx.size(), sy.size());

      for(int i=0;i<(int)sx.size();i++){
	E16ANA_GTRAnalyzedStripHit& shit = sx[i];
	shit.SetLayerAndModuleIDandType(layer, k, shit.is_x);

	E16ANA_GTRAnalyzedHit ahit;
	TVector3 local( shit.CogHit()*0.1  ,0,0);//mm->cm in 
	//TVector3 gX = 	E16ANA_Transform::GetGPos(local, geom, 
	//  layer, shit.ModuleID() ) * 10;//cm->mm
   TVector3 gX = geom->GetGPos(local, layer, shit.ModuleID())*10.0;
	ahit.SetXstripHit(&shit, gX);

	gtrAHitsXChamber[layer][k].push_back(ahit);//copy
	gtrAHitsX[layer]->push_back( & (gtrAHitsXChamber[layer][k].back() ) );
	gtrSHitsX[layer]->push_back(&shit);
      }

      for(int i=0;i<(int)sy.size();i++){
	E16ANA_GTRAnalyzedStripHit& shit = sy[i];
	shit.SetLayerAndModuleIDandType(layer, k, shit.is_y);
	E16ANA_GTRAnalyzedHit ahit;

	TVector3 local( 0, shit.CogHit()*0.1  ,0);//mm->cm in 
	//TVector3 gY = 	E16ANA_Transform::GetGPos(local, geom, 
	//  layer, shit.ModuleID() ) * 10;//cm->mm
   TVector3 gY = geom->GetGPos(local, layer, shit.ModuleID())*10.0;
	ahit.SetYstripHit(&shit, gY);

	gtrAHitsYChamber[layer][k].push_back(ahit);//copy
	gtrAHitsY[layer]->push_back(& (gtrAHitsYChamber[layer][k].back() ) );
	gtrSHitsY[layer]->push_back(&shit);
      }

    }//for layer

    //Yb in only 100-----------------------------
    E16ANA_GTR100Analyzer* ana1 = (E16ANA_GTR100Analyzer*) anaS[ 0 ][k];
    //ana1->SetStripDataYb( gtr1->GetStripYb() );
    //ana1->AnalyzeYb();
    vector<E16ANA_GTRAnalyzedStripHit> & syb=	  
    ana1 ->GetStripYb()->GetAnalyzedHits();

    for(int i=0;i<(int)syb.size();i++){
	E16ANA_GTRAnalyzedStripHit& shit =  syb[i];
	shit.SetLayerAndModuleIDandType(0, k, shit.is_yb); //100 means layer=0

	E16ANA_GTRAnalyzedHit ahit;
	TVector3 local( 0, shit.TdcHit()*0.1  ,0);//mm->cm in 
	//TVector3 gY = 	E16ANA_Transform::GetGPos(local, geom, 
	//  0, shit.ModuleID() ) * 10;//cm->mm
   TVector3 gY = geom->GetGPos(local, 0, shit.ModuleID())*10.0;
	ahit.SetYstripHit(&shit, gY);

	gtrAHitsYChamber[0][k].push_back(ahit);//copy: 
	       //analyzedHitChamber has only 3-layer: Yb appended to Y[0]
	gtrAHits1Yb.push_back(& (gtrAHitsYChamber[0][k].back() ) );
	gtrSHits1Yb.push_back(&shit);//only pointer
    }

  }//k:module


}

std::vector<E16ANA_GTRAnalyzedStripHit> E16ANA_GTRAnalyzerManager::ReAnalyzeV4X(int layer, E16ANA_G4Hit &hit){
   int module = hit.ModuleID();
   E16ANA_GTRStripAnalyzer* ana = anaS[layer][module]->GetStripX();
   TVector3 track_lpos = hit.XTV();
   TVector3 track_lmom = hit.PTV();
   std::vector<E16ANA_GTRAnalyzedStripHit> ret_array = ana->ReAnalyzeV4(track_lpos.X(), track_lmom.X()/track_lmom.Z()); // X-strip
   for(int i=0; i<(int)ret_array.size(); i++){
      ret_array[i].SetLayerAndModuleIDandType(layer, module, ret_array[i].is_x);
   }
   return ret_array;
}

std::vector<E16ANA_GTRAnalyzedStripHit> E16ANA_GTRAnalyzerManager::ReAnalyzeV4Y(int layer, E16ANA_G4Hit &hit, double near_cut){
   int module = hit.ModuleID();
   E16ANA_GTRStripAnalyzer* ana = anaS[layer][module]->GetStripY();
   TVector3 track_lpos = hit.XTV();
   TVector3 track_lmom = hit.PTV();
   std::vector<E16ANA_GTRAnalyzedStripHit> ret_array = ana->ReAnalyzeV4(track_lpos.Y(), track_lmom.Y()/track_lmom.Z(), near_cut); // Y-strip
   for(int i=0; i<(int)ret_array.size(); i++){
      ret_array[i].SetLayerAndModuleIDandType(layer, module, ret_array[i].is_y);
   }
   if(layer == 0){
      E16ANA_GTR100Analyzer* ana100 = (E16ANA_GTR100Analyzer*)anaS[layer][module];
      ana = ana100->GetStripYb();
      std::vector<E16ANA_GTRAnalyzedStripHit> ret2 = ana->ReAnalyzeV4(track_lpos.Y(), track_lmom.Y()/track_lmom.Z(), near_cut); // Y-strip
      for(int i=0; i<(int)ret2.size(); i++){
         ret2[i].SetLayerAndModuleIDandType(layer, module, ret2[i].is_yb);
      }
      ret_array.insert(ret_array.end(), ret2.begin(), ret2.end());
   }
   return ret_array;
}

void E16ANA_GTRAnalyzerManager::Clear(){
  for(int i = 0; i< Nmodule ; i++ ){   
    ana100[i]->Clear();
    ana200[i]->Clear();
    ana300[i]->Clear();
  }
  gtrSHits1X.clear();
  gtrSHits2X.clear();
  gtrSHits3X.clear();
  gtrSHits1Y.clear();
  gtrSHits2Y.clear();
  gtrSHits3Y.clear();
  gtrSHits1Yb.clear();

  gtrAHits1X.clear();
  gtrAHits2X.clear();
  gtrAHits3X.clear();
  gtrAHits1Y.clear();
  gtrAHits2Y.clear();
  gtrAHits3Y.clear();
  gtrAHits1Yb.clear();
  
  for( int k=0; k < Nmodule ; k++){
    gtrAHits1XChamber[k].clear();
    gtrAHits2XChamber[k].clear();
    gtrAHits3XChamber[k].clear();
    gtrAHits1YChamber[k].clear();
    gtrAHits2YChamber[k].clear();
    gtrAHits3YChamber[k].clear();
    gtrAHits1YbChamber[k].clear();
  }

}


