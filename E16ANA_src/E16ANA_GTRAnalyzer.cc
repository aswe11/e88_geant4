//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai
//2015-11-02, uploaded by yokkaich
//2015-11-02, uploaded by komatsu
//2015-10-05, uploaded by yokkaich
//2015-07-15, uploaded by nakai
//2015-05-29, uploaded by yokkaich
//2015-04-08, uploaded by nakai
//2015-03-27, uploaded by yokkaich
//2015-03-20, uploaded by nakai
//
//E16ANA_GTRAnalyzer.cc 150906 
//    Last modified at <2015-09-10 21:36:02 >
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

#include "E16ANA_GTRAnalyzer.hh"
#include "E16ANA_GTRPedestalSet.hh"
#include "E16ANA_WaveformFitManager.hh"
#include "E16ANA_GTRStripAnalyzer.hh"


E16ANA_GTRAnalyzer::E16ANA_GTRAnalyzer(int _n_strip_x, int _n_strip_y, int _n_fadc_chs, int _n_fadc_counts) : 
   n_strip_x(_n_strip_x), n_strip_y(_n_strip_y),
   n_fadc_chs(_n_fadc_chs), n_fadc_counts(_n_fadc_counts)
{

   gem_x = new double*[n_strip_x];
   for(int i=0; i<n_strip_x; i++){
      gem_x[i] = new double[n_fadc_counts];
   }

   gem_y = new double*[n_strip_x];
   for(int i=0; i<n_strip_y; i++){
      gem_y[i] = new double[n_fadc_counts];
   }

   strip_num = new int[n_fadc_chs];
   gemped_x = new double[n_strip_x];
   gemped_y = new double[n_strip_y];
   gemsigma_x = new double[n_strip_x];
   gemsigma_y = new double[n_strip_y];

   fadcped = new double*[n_fadc_chs];
   fadcsigma = new double*[n_fadc_chs];
   for(int i=0; i<n_fadc_chs; i++){
      fadcped[i] = new double[n_fadc_counts];
      fadcsigma[i] = new double[n_fadc_counts];
   }

   gem_peak_x = new double[n_strip_x];
   gem_peak_y = new double[n_strip_y];
   gem_peak_time_x = new double[n_strip_x];
   gem_peak_time_y = new double[n_strip_y];
   gem_t0_x = new double[n_strip_x];
   gem_t0_y = new double[n_strip_y];

   gem_decon_x = new float*[n_strip_x];
   for(int i=0; i<n_strip_x; i++){
      gem_decon_x[i] = new float[n_fadc_counts];
   }

   gem_decon_y = new float*[n_strip_y];
   for(int i=0; i<n_strip_y; i++){
      gem_decon_y[i] = new float[n_fadc_counts];
   }

   gem_fit_peak_x = new double*[n_strip_x];
   gem_fit_time0_x = new double*[n_strip_x];
   gem_fit_trise_x = new double*[n_strip_x];
   for(int i=0; i<n_strip_x; i++){
      gem_fit_peak_x[i] = new double[2];
      gem_fit_time0_x[i] = new double[2];
      gem_fit_trise_x[i] = new double[2];
   }

	for(int i=0; i<n_strip_x; i++){
		for(int j=0; j<n_fadc_counts; j++){
			gem_x[i][j]= -1.0;
			gem_decon_x[i][j]= -255.0;
		}
	}
	for(int i=0; i<n_strip_y; i++){
		for(int j=0; j<n_fadc_counts; j++){
			gem_y[i][j]= -1;
			gem_decon_y[i][j]= -255.0;
		}
	}
	for(int i=0; i<n_fadc_chs; i++){
		strip_num[i]= i;
	}
	for(int i=0; i<n_strip_x; i++){
		gemped_x[i]= 0.0;
		gemsigma_x[i]= 1.0;
		gem_peak_x[i]= -255.0;
		gem_t0_x[i]= -300.0;
	}
	for(int i=0; i<n_strip_y; i++){
		gemped_y[i]= 0.0;
		gemsigma_y[i]= 1.0;
		gem_peak_y[i]= -255.0;
		gem_t0_y[i]= -300.0;
	}
	for(int i=0; i<n_fadc_chs; i++){
		for(int j=0; j<n_fadc_counts; j++){
			fadcped[i][j]= 0.0;
			fadcsigma[i][j]= 1.0;
		}
	}
	strip_pitch_x= 0.35;
	strip_pitch_y= 0.35;
	gem_th_x= 4.0;
	gem_th_y= 4.0;
	drift_velocity= 0.015;
	drift_gap_center= 0.0;
	fadc_clock_period= 10.0;
	fadc_t0_correction= 0.0;
	tdc_lsb= 0.035;
	gem_t0_min= -10000.0;
	gem_t0_max= 10000.0;
	gem_tr= 75.0;
	decon_o= new E16ANA_DeconvolutionManager(n_fadc_counts);
	fadc_valid_count.reserve(n_fadc_counts);
	for(int i=0; i<n_fadc_counts; i++){
		fadc_valid_count.push_back(i);
	}
	track_projection_x= 0.0;
	track_projection_y= 0.0;
	track_analysis_window= 1000.0;
	analysis_method_x= 0;
	analysis_method_y= 0;
	threshold_fraction= 0.5;
	left_strip_x= 0;
	right_strip_x= n_strip_x;
	left_strip_y= 0;
	right_strip_y= n_strip_y;

   strip_ana_x = new E16ANA_GTRStripAnalyzer(n_strip_x, n_fadc_counts);
   strip_ana_x->SetInverse(false);
   strip_ana_y = new E16ANA_GTRStripAnalyzer(n_strip_y, n_fadc_counts);
   strip_ana_y->SetInverse(true);

}

E16ANA_GTRAnalyzer::~E16ANA_GTRAnalyzer(){

   for(int i=0; i<n_strip_x; i++){
      delete [] gem_x[i];
   }
   delete [] gem_x;
   for(int i=0; i<n_strip_y; i++){
      delete [] gem_y[i];
   }
   delete [] gem_y;
   delete [] strip_num;
   delete [] gemped_x;
   delete [] gemped_y;
   delete [] gemsigma_x;
   delete [] gemsigma_y;

   for(int i=0; i<n_fadc_chs; i++){
      delete [] fadcped[i];
      delete [] fadcsigma[i];
   }
   delete [] fadcped;
   delete [] fadcsigma;
   delete [] gem_peak_x;
   delete [] gem_peak_y;
   delete [] gem_peak_time_x;
   delete [] gem_peak_time_y;
   delete [] gem_t0_x;
   delete [] gem_t0_y;

   for(int i=0; i<n_strip_x; i++){
      delete [] gem_decon_x[i];
   }
   delete [] gem_decon_x;
   for(int i=0; i<n_strip_y; i++){
      delete [] gem_decon_y[i];
   }
   delete [] gem_decon_y;

   for(int i=0; i<n_strip_x; i++){
      delete [] gem_fit_peak_x[i];
      delete [] gem_fit_time0_x[i];
      delete [] gem_fit_trise_x[i];
   }
   delete [] gem_fit_peak_x;
   delete [] gem_fit_time0_x;
   delete [] gem_fit_trise_x;

	delete decon_o;
	for(int i=0; i<(int)clusters_array_x.size(); i++){
		delete clusters_array_x[i];
	}
   delete strip_ana_x;
   delete strip_ana_y;
}

void E16ANA_GTRAnalyzer::Clear(){
	for(int i=0; i<(int)clusters_array_x.size(); i++){
		delete clusters_array_x[i];
	}
	clusters_array_x.clear();
	signals_array_x.clear();
   this->ClearFadc();
}

void E16ANA_GTRAnalyzer::Clear2(){
   strip_ana_x->Clear();
   strip_ana_y->Clear();
}

void E16ANA_GTRAnalyzer::ClearFadc(){
	for(int i=0; i<n_strip_x; i++){
		for(int j=0; j<n_fadc_counts; j++){
			gem_x[i][j]= -1.0;
		}
	}
	for(int i=0; i<n_strip_y; i++){
		for(int j=0; j<n_fadc_counts; j++){
			gem_y[i][j]= -1.0;
		}
	}
}

void E16ANA_GTRAnalyzer::CalcWaveParamsPeak(){
	//int peak_count_x[n_strip_x];
	//int peak_count_y[n_strip_y];

	for(int i=0; i<n_strip_x; i++){
		gem_peak_x[i]= -255.0;
		gem_peak_time_x[i]= -1000.0;
		gem_t0_x[i]= -1000.0;
	}
	for(int i=0; i<n_strip_y; i++){
		gem_peak_y[i]= -255.0;
		gem_peak_time_y[i]= -1000.0;
		gem_t0_y[i]= -1000.0;
	}
/*
	int left_strip_x= (int)((track_projection_x-track_analysis_window)/strip_pitch_x);
	if(left_strip_x<0) left_strip_x= 0;
	int right_strip_x= (int)((track_projection_x+track_analysis_window)/strip_pitch_x);
	if(right_strip_x>n_strip_x) right_strip_x= n_strip_x;
*/
	//for(int i=0; i<n_strip_x; i++){
	for(int i=left_strip_x; i<right_strip_x; i++){
/*
		gem_peak_x[i]= -255.0;
		//gem_t0_x[i]= -1.0;
		peak_count_x[i]= -1;
		//for(int j=0; j<n_fadc_counts; j++){
		for(int l=0; l<fadc_valid_count.size(); l++){
			int j= fadc_valid_count[l];

			double tdc_j= j*fadc_clock_period+fadc_t0_correction;
			if(tdc_j>gem_t0_max) break;
			if(tdc_j<gem_t0_min) continue;

			if(gem_peak_x[i]<gem_x[i][j]-gemped_x[i]){
				gem_peak_x[i]= gem_x[i][j]-gemped_x[i];
				//gem_t0_x[i]= j*10.0;
				peak_count_x[i]= l;
			}
		}

		if(gem_peak_x[i]<gem_th_x){
			gem_peak_x[i]= -255.0;
			//gem_t0_x[i]= -1.0;
			peak_count_x[i]= -1;
		}
*/
		this->CalcWaveParamsPeak(i, gem_t0_max+gem_tr+50.0);
		if(gem_t0_x[i]>gem_t0_max){
			this->CalcWaveParamsPeak(i, gem_t0_max-gem_tr);
		}
	}
/*
	int left_strip_y= (int)((track_projection_y-track_analysis_window)/strip_pitch_y);
	if(left_strip_y<0) left_strip_y= 0;
	int right_strip_y= (int)((track_projection_y+track_analysis_window)/strip_pitch_y);
	if(right_strip_y>n_strip_y) right_strip_y= n_strip_y;
*/
	//for(int i=0; i<n_strip_y; i++){
	for(int i=left_strip_y; i<right_strip_y; i++){
/*
		gem_peak_y[i]= -255.0;
		//gem_t0_y[i]= -1.0;
		peak_count_y[i]= -1;
		//for(int j=0; j<n_fadc_counts; j++){
		for(int l=0; l<fadc_valid_count.size(); l++){
			int j= fadc_valid_count[l];
			double tdc_j= j*fadc_clock_period+fadc_t0_correction;
			if(tdc_j>gem_t0_max) break;
			if(tdc_j<gem_t0_min) continue;
			if(gem_peak_y[i]<gem_y[i][j]-gemped_y[i]){
				gem_peak_y[i]= gem_y[i][j]-gemped_y[i];
				//gem_t0_y[i]= j*10.0;
				peak_count_y[i]= l;
			}
		}
		if(gem_peak_y[i]<gem_th_y){
			gem_peak_y[i]= -255.0;
			//gem_t0_y[i]= -1.0;
			peak_count_y[i]= -1;
		}
*/
		this->CalcWaveParamsPeak(i+n_strip_x, gem_t0_max+gem_tr);
		if(gem_t0_y[i]>gem_t0_max){
			this->CalcWaveParamsPeak(i+n_strip_x, gem_t0_max-gem_tr);
		}
	}
/*
	//for(int i=0; i<n_strip_x; i++){
	for(int i=left_strip_x; i<right_strip_x; i++){
		if(peak_count_x[i]>-1){
			for(int l=peak_count_x[i]; l>-1; l--){
				int j= fadc_valid_count[l];
				//double tdc_j= j*fadc_clock_period+fadc_t0_correction;
				//if(tdc_j<gem_t0_min) break;
				if(gem_x[i][j]-gemped_x[i]<gem_peak_x[i]*threshold_fraction){
					gem_t0_x[i]= j*fadc_clock_period+fadc_t0_correction;
					gem_peak_time_x[i]= peak_count_x[i]*fadc_clock_period+fadc_t0_correction;

					if(gem_t0_x[i]>gem_t0_max){
						gem_t0_x[i]= -1000.0;
						gem_peak_x[i]= -255.0;
						gem_peak_time_x[i]= -1000.0;
					}

					break;
				}
			}
		}else{
			gem_t0_x[i]= -1000.0;
			gem_peak_time_x[i]= -1000.0;
		}
		//std::cout<< "gem_t0_x["<< i<< "] = "<< gem_t0_x[i]<< std::endl;
	}

	//for(int i=0; i<n_strip_y; i++){
	for(int i=left_strip_y; i<right_strip_y; i++){
		if(peak_count_y[i]>-1){
			for(int l=peak_count_y[i]; l>-1; l--){
				int j= fadc_valid_count[l];
				//double tdc_j= j*fadc_clock_period+fadc_t0_correction;
				//if(tdc_j<gem_t0_min) break;
				if(gem_y[i][j]-gemped_y[i]<gem_peak_y[i]*threshold_fraction){
					gem_t0_y[i]= j*fadc_clock_period+fadc_t0_correction;
					gem_peak_time_y[i]= peak_count_y[i]*fadc_clock_period+fadc_t0_correction;
					if(gem_t0_y[i]>gem_t0_max){
						gem_t0_y[i]= -1000.0;
						gem_peak_y[i]= -255.0;
						gem_peak_time_y[i]= -1000.0;
					}
					break;
				}
			}
		}else{
			gem_t0_y[i]= -1000.0;
			gem_peak_time_y[i]= -1000.0;
		}
		//std::cout<< "gem_t0_y["<< i<< "] = "<< gem_t0_y[i]<< std::endl;
	}
*/
}

void E16ANA_GTRAnalyzer::CalcWaveParamsPeak(int ch, double t_cutoff){
   //std::cout << "E16ANA_GTRAnalyzer::CalcWaveParamsPeak(), Ch = " << ch <<std::endl;
	int peak_count;
	if(ch<n_strip_x){
		gem_peak_x[ch]= -255.0;
		gem_t0_x[ch]= -1000.0;
		peak_count= -1;
		//for(int j=0; j<n_fadc_counts; j++){
		for(int l=0; l<(int)fadc_valid_count.size(); l++){
			int j= fadc_valid_count[l];
			double tdc_j= j*fadc_clock_period+fadc_t0_correction;
			if(tdc_j>t_cutoff) break;
			//if(tdc_j<t_cutoff) continue;

			if(gem_peak_x[ch]<gem_x[ch][j]-gemped_x[ch]){
				gem_peak_x[ch]= gem_x[ch][j]-gemped_x[ch];
				//gem_t0_x[ch]= j*10.0;
				peak_count= l;
			}
		}	
		if(gem_peak_x[ch]<gem_th_x){
			gem_peak_x[ch]= -255.0;
			//gem_t0_x[ch]= -1.0;
			peak_count= -1;
		}else{
         //std::cout << "Peak detection, peak_value = " << gem_peak_x[ch] << std::endl;
      }

		if(peak_count>-1){
			for(int l=peak_count; l>-1; l--){
				int j= fadc_valid_count[l];
				//double tdc_j= j*fadc_clock_period+fadc_t0_correction;
				//if(tdc_j<gem_t0_min) break;
				if(gem_x[ch][j]-gemped_x[ch]<gem_peak_x[ch]*threshold_fraction){
					gem_t0_x[ch]= j*fadc_clock_period+fadc_t0_correction;
					gem_peak_time_x[ch]= peak_count*fadc_clock_period+fadc_t0_correction;
/*
					if(gem_t0_x[ch]>gem_t0_max){
						gem_t0_x[ch]= -1000.0;
						gem_peak_x[ch]= -255.0;
						gem_peak_time_x[ch]= -1000.0;
					}
*/
					break;
				}
			}
		}else{
			gem_t0_x[ch]= -1000.0;
			gem_peak_time_x[ch]= -1000.0;
		}
	}else if(ch<n_strip_x+n_strip_y){
		ch-= n_strip_x;
		gem_peak_y[ch]= -255.0;
		gem_t0_y[ch]= -1000.0;
		peak_count= -1;
		//for(int j=0; j<n_fadc_counts; j++){
		for(int l=0; l<(int)fadc_valid_count.size(); l++){
			int j= fadc_valid_count[l];	
			double tdc_j= j*fadc_clock_period+fadc_t0_correction;
			if(tdc_j>t_cutoff) break;
			//if(tdc_j<t_cutoff) continue;
	
			if(gem_peak_y[ch]<gem_y[ch][j]-gemped_y[ch]){
				gem_peak_y[ch]= gem_y[ch][j]-gemped_y[ch];
				//gem_t0_y[ch]= j*10.0;
				peak_count= l;
			}
		}	
		if(gem_peak_y[ch]<gem_th_y){
			gem_peak_y[ch]= -255.0;
			//gem_t0_y[ch]= -1.0;
			peak_count= -1;
		}

		if(peak_count>-1){
			for(int l=peak_count; l>-1; l--){
				int j= fadc_valid_count[l];
				//double tdc_j= j*fadc_clock_period+fadc_t0_correction;
				//if(tdc_j<gem_t0_min) break;
				if(gem_y[ch][j]-gemped_y[ch]<gem_peak_y[ch]*threshold_fraction){
					gem_t0_y[ch]= j*fadc_clock_period+fadc_t0_correction;
					gem_peak_time_y[ch]= peak_count*fadc_clock_period+fadc_t0_correction;
/*
					if(gem_t0_y[ch]>gem_t0_max){
						gem_t0_y[ch]= -1000.0;
						gem_peak_y[ch]= -255.0;
						gem_peak_time_y[ch]= -1000.0;
					}
*/
					break;
				}
			}
		}else{
			gem_t0_y[ch]= -1000.0;
			gem_peak_time_y[ch]= -1000.0;
		}
	}
}

void E16ANA_GTRAnalyzer::CalcWaveParamsFit(){

	double params[8];
	double chi2;
	for(int ch=0; ch<n_fadc_chs; ch++){
		E16ANA_WaveformFitManager *fito= new E16ANA_WaveformFitManager(n_fadc_counts);
		// See "E16ANA_GTRAnalyzer::SetPinAssign()" to know the definition of strip_num
		if(strip_num[ch]<n_strip_x && strip_num[ch]>-1){
			int x_ch= strip_num[ch];
			fito->SetFadc(gem_x[strip_num[ch]]);
			fito->SetPedestal(gemped_x[strip_num[ch]]);
			fito->DoFit();
			fito->GetParams(params);
			chi2= fito->GetChi2();
			//gem_t0_x[strip_num[ch]]= params[0];
			//gem_peak_x[strip_num[ch]]= params[1];
/*
			double peak0[2];
			double time0[2];
			double trise[2];
			peak0[0]= params[3];
			peak0[1]= params[6];
			time0[0]= params[2];
			time0[1]= params[5];
			trise[0]= params[4];
			trise[1]= params[7];
*/
			gem_fit_peak_x[x_ch][0]= params[3];
			gem_fit_peak_x[x_ch][1]= params[6];
			gem_fit_time0_x[x_ch][0]= params[2];
			gem_fit_time0_x[x_ch][1]= params[5];
			gem_fit_trise_x[x_ch][0]= params[4];
			gem_fit_trise_x[x_ch][1]= params[7];

/*
			for(int count=0; count<n_fadc_counts; count++){
				gem_decon_x[x_ch][count]= 0.0;
			}
			for(int i=0; i<2; i++){
				if(peak0[i]>0){
					for(int count=(int)(time0[i]/10.0); count<(int)((time0[i]+trise[i])/10.0); count++){
						gem_decon_x[x_ch][count]+= peak0[i];
					}
				}
			}
*/
/*
			for(int i=0; i<2; i++){
				if(peak0[i]>0 && time0[i]<800){
					E16ANA_GTRSignal asignal;
					asignal.height= peak0[i];
					asignal.time0= time0[i];
					asignal.trise= trise[i];
					asignal.strip= x_ch;
					signals_array_x.push_back(asignal);
				}
			}
*/
		}else if(strip_num[ch]>=n_strip_x && strip_num[ch]<n_strip_x+n_strip_y){
			int y_ch= strip_num[ch]-n_strip_x;
			fito->SetFadc(gem_y[strip_num[ch]-n_strip_x]);
			fito->SetPedestal(gemped_y[strip_num[ch]-n_strip_x]);
			fito->DoFit();
			fito->GetParams(params);
			chi2= fito->GetChi2();
			//gem_t0_y[strip_num[ch]-32]= params[0];
			//gem_peak_y[strip_num[ch]-32]= params[1];

			double peak0[2];
			double time0[2];
			double trise[2];
			peak0[0]= params[3];
			peak0[1]= params[6];
			time0[0]= params[2];
			time0[1]= params[5];
			trise[0]= params[4];
			trise[1]= params[7];

/*
			for(int count=0; count<n_fadc_counts; count++){
				gem_decon_y[y_ch][count]= 0.0;
			}
			for(int i=0; i<2; i++){
				if(peak0[i]>0){
					for(int count=(int)(time0[i]/10.0); count<(time0[i]+trise[i]); count++){
						gem_decon_y[y_ch][count]+= peak0[i];
					}
				}
			}
*/

		}
		delete fito;
	}
}

void E16ANA_GTRAnalyzer::CalcDeconvolution(){
	for(int ch=0; ch<n_fadc_chs; ch++){
		if(strip_num[ch]<n_strip_x && strip_num[ch]>-1){// x_strip
			int x_ch= strip_num[ch];
			if(gem_peak_x[x_ch]>gem_th_x){
				decon_o->SetFadc(gem_x[x_ch]);
				decon_o->SetPedestal(gemped_x[x_ch]);
				decon_o->DoDeconvolution1();
				decon_o->GetResult(gem_decon_x[x_ch]);
			}else{
				for(int i=0; i<n_fadc_counts; i++){
					gem_decon_x[x_ch][i]= -255.0;
				}
			}
		}else if(strip_num[ch]>=n_strip_x && strip_num[ch]<n_strip_x+n_strip_y){// y_strip
			int y_ch= strip_num[ch]-n_strip_x;
			if(gem_peak_y[y_ch]>gem_th_y){
				decon_o->SetFadc(gem_y[y_ch]);
				decon_o->SetPedestal(gemped_y[y_ch]);
				decon_o->DoDeconvolution1();
				decon_o->GetResult(gem_decon_y[y_ch]);
			}else{
				for(int i=0; i<n_fadc_counts; i++){
					gem_decon_y[y_ch][i]= -255.0;
				}
			}
		}
	}
}

void E16ANA_GTRAnalyzer::SetParameters(std::string filename){
	//NConfReader &gConf= NConfReader::GetInstance();
	//std::string filename= gConf.GetFileName("GEMPARAM:");
	//std::cout<< "[E16ANA_GTRAnalyzer::SetParameters()] "<< filename<< std::endl;
	std::ifstream ifs(filename.c_str());

	if(ifs.fail()){
		std::cout<< "ParamFile open fail"<< std::endl;
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
	fadc_clock_period= parameter_list["FADC_CLOCK_PERIOD:"];
	tdc_lsb= parameter_list["TDC_LSB:"];
	track_analysis_window= parameter_list["TRACK_CUT_WINDOW:"];
	analysis_method_x= parameter_list["ANALYSIS_METHOD_X:"];// 0: COG, 1: Timing1, 2: Timing2
	analysis_method_y= parameter_list["ANALYSIS_METHOD_Y:"];// 0: COG, 1: Timing1, 2: Timing2

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
   strip_ana_x->fadc_clock_period = fadc_clock_period;
   strip_ana_x->position_start = x_start;
   strip_ana_x->gem_tr = gem_tr;
   strip_ana_x->threshold_fraction = threshold_fraction;

   strip_ana_y->strip_pitch = strip_pitch_y;
   strip_ana_y->gem_threshold = gem_th_y;
   strip_ana_y->gem_tdc_min = gem_t0_min;
   strip_ana_y->gem_tdc_max = gem_t0_max;
   strip_ana_y->drift_velocity = drift_velocity;
   strip_ana_y->drift_gap_center = drift_gap_center;
   strip_ana_y->fadc_clock_period = fadc_clock_period;
   strip_ana_y->position_start = y_start;
   strip_ana_y->gem_tr = gem_tr;
   strip_ana_y->threshold_fraction = threshold_fraction;

}

void E16ANA_GTRAnalyzer::SetPinAssign(std::string filename){
	//NConfReader &gConf= NConfReader::GetInstance();
	//std::string filename= gConf.GetFileName("FADCGEO:");
	//std::cout<< "[E16ANA_GTRAnalyzer::SetPinAssign()] "<< filename<< std::endl;
	std::ifstream ifs(filename.c_str());

	if(ifs.fail()){
		std::cout<< "GeoFile open fail"<< std::endl;
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

void E16ANA_GTRAnalyzer::SetFadcValidCount(std::string filename){
	//NConfReader &gConf= NConfReader::GetInstance();
	//std::string filename= gConf.GetFileName("FADCVALID:");
	//std::cout<< "[E16ANA_GTRAnalyzer::SetFadcValidCount()] "<< filename<< std::endl;
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


void E16ANA_GTRAnalyzer::SetPedestal(std::string filename){
	E16ANA_GTRPedestalSet *setped= new E16ANA_GTRPedestalSet();

	//NConfReader &gConf= NConfReader::GetInstance();
	//std::string filename= gConf.GetFileName("FADCPED:");
	setped->Initialize(filename);

	for(int i=0; i<setped->GetNumPed(); i++){
		E16ANA_GTRPedestal *pedo= setped->GetPedestalObject(i);
		int ch= pedo->GetCh();
		double pedestal= pedo->GetPedestal();
		double sigma= pedo->GetSigma();
		if(ch>=n_fadc_chs) continue;
		if(strip_num[ch]>-1){
			if(strip_num[ch]<n_strip_x){
				gemped_x[strip_num[ch]]= pedestal;
				gemsigma_x[strip_num[ch]]= sigma;
			}else{
				gemped_y[strip_num[ch]-n_strip_x]= pedestal;
				gemsigma_y[strip_num[ch]-n_strip_x]= sigma;
			}
		}
	}
	delete setped;
}

void E16ANA_GTRAnalyzer::SetFadcPedestal(std::string filename){
	E16ANA_GTRPedestalSet *setped= new E16ANA_GTRPedestalSet();

	//NConfReader &gConf= NConfReader::GetInstance();
	//std::string filename= gConf.GetFileName("FADCPED:");
	setped->Initialize(filename);

	for(int i=0; i<setped->GetNumPed(); i++){
		E16ANA_GTRPedestal *pedo= setped->GetPedestalObject(i);
		int ch= pedo->GetCh();
		double pedestal= pedo->GetPedestal();
		double sigma= pedo->GetSigma();
		if(ch>-1 && ch<n_fadc_chs*n_fadc_counts){
			fadcped[ch/n_fadc_counts][ch%n_fadc_counts]= pedestal;
			fadcsigma[ch/n_fadc_counts][ch%n_fadc_counts]= sigma;
		}
	}
	delete setped;
}

void E16ANA_GTRAnalyzer::SetFadc(double *_fadc){
	//double fadc[n_fadc_chs][n_fadc_counts];
   double **fadc;
   fadc = new double*[n_fadc_chs];
	for(int i=0; i<n_fadc_chs; i++){
      fadc[i] = new double[n_fadc_counts];
   }
	memcpy(&fadc[0][0], _fadc, sizeof(fadc));
	for(int i=0; i<n_fadc_chs; i++){
		for(int j=0; j<n_fadc_counts; j++){
			fadc[i][j]-= fadcped[i][j];
         //std::cout << "fadc[" << i << "][" << j << "] = " << fadc[i][j] << std::endl;
		}
	}
	for(int i=0; i<n_fadc_chs; i++){
		if(strip_num[i]<n_strip_x && strip_num[i]>-1){
			memcpy(gem_x[strip_num[i]], fadc[i], sizeof(gem_x[0]));
		}else if(strip_num[i]>=n_strip_x && strip_num[i]<n_strip_x+n_strip_y){
			memcpy(gem_y[strip_num[i]-n_strip_x], fadc[i], sizeof(gem_y[0]));
		}
		//std::cout<< i<< " "<< strip_num[i]<< std::endl;
	}

	for(int i=0; i<n_fadc_chs; i++){
      delete [] fadc[i];
   }
   delete [] fadc;
/*
	int ch= 1;
	for(int j=0; j<n_fadc_counts; j++){
		std::cout<< "gem_x["<< ch<< "]["<< j<< "] = "<< gem_x[ch][j]<< std::endl;
	}
*/
}

void E16ANA_GTRAnalyzer::SetFadc(int ch, double *_fadc){
	if(strip_num[ch]<n_strip_x && strip_num[ch]>-1){
		//memcpy(gem_x[strip_num[ch]], _fadc, sizeof(gem_x[0]));
      for(int j=0; j<n_fadc_counts; j++){
         gem_x[strip_num[ch]][j] = _fadc[j];
      }
	}else if(strip_num[ch]>=n_strip_x && strip_num[ch]<n_strip_x+n_strip_y){
		//memcpy(gem_y[strip_num[ch]-n_strip_x], _fadc, sizeof(gem_y[0]));
      for(int j=0; j<n_fadc_counts; j++){
         gem_y[strip_num[ch]-n_strip_x][j] = _fadc[j];
      }
	}
}

void E16ANA_GTRAnalyzer::SetFadc2(int ch, double *_fadc){
	if(strip_num[ch]<n_strip_x && strip_num[ch]>-1){
      strip_ana_x->SetFadc(strip_num[ch], _fadc);
	}else if(strip_num[ch]>=n_strip_x && strip_num[ch]<n_strip_x+n_strip_y){
      strip_ana_y->SetFadc(strip_num[ch]-n_strip_x, _fadc);
	}
}

void E16ANA_GTRAnalyzer::Analyze(){
	//this->Clear();
	//this->PutSignals();
	//this->CalcClustering();
	//std::cout<<"ok"<<std::endl;

   //for(int i=0; i<n_strip_x; i++){
      //for(int j=0; j<n_fadc_counts; j++){
         //std::cout << "gem_x[" << i << "][" << j << "] = " << gem_x[i][j] << std::endl;
      //}
   //}

	this->CalcWaveParamsPeak();
	//std::cout<<"peak ok"<<std::endl;
	gem_max_strip_x= -1;
	gem_max_strip_y= -1;
	gem_max_value_x= -255.0;
	gem_max_value_y= -255.0;
/*
	int left_strip_x= (int)((track_projection_x-track_analysis_window)/strip_pitch_x);
	if(left_strip_x<0) left_strip_x= 0;
	int right_strip_x= (int)((track_projection_x+track_analysis_window)/strip_pitch_x);
	if(right_strip_x>n_strip_x) right_strip_x= n_strip_x;
*/
	// COG method
	//for(int i=0; i<n_strip_x; i++){
	for(int i=left_strip_x; i<right_strip_x; i++){
		if(gem_max_value_x<gem_peak_x[i]){
			gem_max_value_x= gem_peak_x[i];
			gem_max_strip_x= i;
		}
	}
	if(gem_max_value_x<gem_th_x){
		gem_max_value_x= -255.0;
		gem_max_strip_x= -1;
	}
/*
	int left_strip_y= (int)((track_projection_y-track_analysis_window)/strip_pitch_y);
	if(left_strip_y<0) left_strip_y= 0;
	int right_strip_y= (int)((track_projection_y+track_analysis_window)/strip_pitch_y);
	if(right_strip_y>n_strip_y) right_strip_y= n_strip_y;
*/
	//for(int i=0; i<n_strip_y; i++){
	for(int i=left_strip_y; i<right_strip_y; i++){
		if(gem_max_value_y<gem_peak_y[i]){
			gem_max_value_y= gem_peak_y[i];
			gem_max_strip_y= i;
		}
	}
	if(gem_max_value_y<gem_th_y){
		gem_max_value_y= -255.0;
		gem_max_strip_y= -1;
	}
	this->CalcCenterOfGravity();

	switch(analysis_method_x){
		case 0:
			gem_hit_x= gem_center_of_gravity_x;
			break;
		case 1:
			this->CalcTdcHit(false);
			gem_hit_x= gem_tdc_hit_x;
			break;
		case 2:
			this->CalcTdcHit(true);
			gem_hit_x= gem_tdc_hit_x;
			break;
		default:
			gem_hit_x= gem_center_of_gravity_x;
			break;
	}

	switch(analysis_method_y){
		case 0:
			gem_hit_y= gem_center_of_gravity_y;
			break;
		case 1:
			this->CalcTdcHit(false);
			gem_hit_y= gem_tdc_hit_y;
			break;
		case 2:
			this->CalcTdcHit(true);
			gem_hit_y= gem_tdc_hit_y;
			break;
		default:
			gem_hit_y= gem_center_of_gravity_y;
			break;
	}


	//this->CalcTdcHit(false);
	//this->CalcTdcHit(true);

	//this->CalcDeconvolution();
	//this->CalcWaveParamsFit();
	//this->CalcClustering();


}

void E16ANA_GTRAnalyzer::Analyze2(){
   strip_ana_x->Analyze();
   strip_ana_y->Analyze();
}
void E16ANA_GTRAnalyzer::Analyze22X(int hitid){
   strip_ana_x->Analyze2(hitid);
}
void E16ANA_GTRAnalyzer::Analyze22Y(int hitid){
   strip_ana_y->Analyze2(hitid);
}


void E16ANA_GTRAnalyzer::CalcCenterOfGravity(){
	gem_num_hit_x= 0;
	gem_cluster_charge_x= 0.0;
	gem_center_of_gravity_x= 0.0;
	// x left
	if(gem_max_strip_x!=-1){
	for(int distance=0; distance<n_strip_x; distance++){
		int soi= gem_max_strip_x-distance;
		if(soi>-1 && soi<n_strip_x && gem_peak_x[soi]>gem_th_x){
			gem_num_hit_x++;
			gem_cluster_charge_x+= gem_peak_x[soi];
			gem_center_of_gravity_x+= gem_peak_x[soi]*soi;
		}else{
			break;
		}
	}
	// x right
	for(int distance=1; distance<n_strip_x; distance++){
		int soi= gem_max_strip_x+distance;
		if(soi>-1 && soi<n_strip_x && gem_peak_x[soi]>gem_th_x){
			gem_num_hit_x++;
			gem_cluster_charge_x+= gem_peak_x[soi];
			gem_center_of_gravity_x+= gem_peak_x[soi]*soi;
		}else{
			break;
		}
	}
	if(gem_num_hit_x!=0){
		gem_center_of_gravity_x/= gem_cluster_charge_x;
		gem_center_of_gravity_x*= strip_pitch_x;
		gem_center_of_gravity_x+= x_start;
	}else{
		gem_center_of_gravity_x= -1000.0;
	}
	}

	gem_num_hit_y= 0;
	gem_cluster_charge_y= 0.0;
	gem_center_of_gravity_y= 0.0;
	// y left
	if(gem_max_strip_y!=-1){
	for(int distance=0; distance<n_strip_y; distance++){
		int soi= gem_max_strip_y-distance;
		if(soi>-1 && soi<n_strip_y && gem_peak_y[soi]>gem_th_y){
			gem_num_hit_y++;
			gem_cluster_charge_y+= gem_peak_y[soi];
			gem_center_of_gravity_y+= gem_peak_y[soi]*soi;
		}else{
			break;
		}
	}
	// y right
	for(int distance=1; distance<n_strip_y; distance++){
		int soi= gem_max_strip_y+distance;
		if(soi>-1 && soi<n_strip_y && gem_peak_y[soi]>gem_th_y){
			gem_num_hit_y++;
			gem_cluster_charge_y+= gem_peak_y[soi];
			gem_center_of_gravity_y+= gem_peak_y[soi]*soi;
		}else{
			break;
		}
	}
	if(gem_num_hit_y!=0){
		gem_center_of_gravity_y/= gem_cluster_charge_y;
		gem_center_of_gravity_y*= strip_pitch_y;
		gem_center_of_gravity_y+= y_start;
	}else{
		gem_center_of_gravity_y= -1000.0;
	}
	}
	gem_cluster_charge_tot= gem_cluster_charge_x+gem_cluster_charge_y;
}

void E16ANA_GTRAnalyzer::CalcTdcHit(bool fixed_method){
	//gem_tdc_hit_y= -1.0;

	std::vector<double> x_array;
	std::vector<double> time_array_x;
	std::vector<double> peak_array_x;
/*
	int left_strip_x= (int)((track_projection_x-track_analysis_window)/strip_pitch_x);
	if(left_strip_x<0) left_strip_x= 0;
	int right_strip_x= (int)((track_projection_x+track_analysis_window)/strip_pitch_x);
	if(right_strip_x>n_strip_x) right_strip_x= n_strip_x;
*/
	if(gem_max_strip_x!=-1){
		//for(int i=0; i<n_strip_x; i++){
		for(int i=left_strip_x; i<right_strip_x; i++){
			//if(gem_peak_x[i]>gem_th_x && gem_t0_x[i]<800){
			//if(gem_peak_x[i]>gem_th_x && gem_t0_x[i]>0.0 && gem_t0_x[i]<gem_t0_max){
			if(gem_peak_x[i]>gem_th_x && gem_t0_x[i]>0.0){
				x_array.push_back((double)i*strip_pitch_x+x_start);
				time_array_x.push_back(gem_t0_x[i]*drift_velocity);
				peak_array_x.push_back(gem_peak_x[i]);
			}
		}

/*
	// x left
	for(int distance=0; distance<32; distance++){
		int soi= gem_max_strip_x-distance;
		if(soi>-1 && soi<32 && gem_peak_x[soi]>gem_th_x){
			x_array.push_back((double)soi*strip_pitch_x);
			time_array.push_back(gem_t0_x[soi]*drift_velocity);
		}else{
			break;
		}
	}
	// x right
	for(int distance=1; distance<32; distance++){
		int soi= gem_max_strip_x+distance;
		if(soi>-1 && soi<32 && gem_peak_x[soi]>gem_th_x){
			x_array.push_back((double)soi*strip_pitch_x);
			time_array.push_back(gem_t0_x[soi]*drift_velocity);
		}else{
			break;
		}
	}
*/
	}

	gem_num_hit_x= x_array.size();
	gem_cluster_charge_x= 0.0;

	std::vector<double> y_array;
	std::vector<double> time_array_y;
	std::vector<double> peak_array_y;

	if(gem_max_strip_y!=-1){
		//for(int i=0; i<n_strip_x; i++){
		for(int i=left_strip_y; i<right_strip_y; i++){
			//if(gem_peak_x[i]>gem_th_x && gem_t0_x[i]<800){
			//if(gem_peak_y[i]>gem_th_y && gem_t0_y[i]>0.0 && gem_t0_y[i]<gem_t0_max){
			if(gem_peak_y[i]>gem_th_y && gem_t0_y[i]>0.0){
				y_array.push_back((double)i*strip_pitch_y+y_start);
				time_array_y.push_back(gem_t0_y[i]*drift_velocity);
				peak_array_y.push_back(gem_peak_y[i]);
			}
		}
	}
	gem_num_hit_y= y_array.size();
	gem_cluster_charge_y= 0.0;

	if(!fixed_method){
		double B= 0.0;
		double C= 0.0;
		double D= 0.0;
		double E= 0.0;
		for(int i=0; i<(int)x_array.size(); i++){
			//std::cout<< "x="<< x_array[i]<< " t="<< time_array_x[i]<< std::endl;
			//B+= time_array_x[i]*time_array_x[i];
			B+= x_array[i]*x_array[i];
			//C+= x_array[i];
			C+= time_array_x[i];
			//D+= x_array[i]*time_array_x[i];
			D+= x_array[i]*time_array_x[i];
			//E+= time_array_x[i];
			E+= x_array[i];
			gem_cluster_charge_x+= peak_array_x[i];
		}
		//tan_incident_angle= (x_array.size()*D-C*E)/(x_array.size()*B-E*E);
		//double tdc_x0= (B*C-D*E)/(x_array.size()*B-E*E);
		//gem_tdc_hit_x= tan_incident_angle*drift_gap_center+tdc_x0;
		double dtdx= (x_array.size()*D-C*E)/(x_array.size()*B-E*E);
		double t0= (B*C-D*E)/(x_array.size()*B-E*E);
		tan_incident_angle= 1.0/dtdx;
		gem_tdc_hit_x= (drift_gap_center-t0)/dtdx;
		//std::cout<< "tanTheta="<< tan_incident_angle<< " gem_x="<< gem_tdc_hit_x<< std::endl;

		B= 0.0;
		C= 0.0;
		D= 0.0;
		E= 0.0;
		for(int i=0; i<(int)y_array.size(); i++){
			//std::cout<< "x="<< y_array[i]<< " t="<< time_array[i]<< std::endl;
			//B+= time_array[i]*time_array[i];
			B+= y_array[i]*y_array[i];
			//C+= y_array[i];
			C+= time_array_y[i];
			//D+= y_array[i]*time_array[i];
			D+= y_array[i]*time_array_y[i];
			//E+= time_array[i];
			E+= y_array[i];
			gem_cluster_charge_y+= peak_array_y[i];
		}
		//tan_incident_angle= (y_array.size()*D-C*E)/(y_array.size()*B-E*E);
		//double tdc_y0= (B*C-D*E)/(y_array.size()*B-E*E);
		//gem_tdc_hit_y= tan_incident_angle*drift_gap_center+tdc_y0;
		dtdx= (y_array.size()*D-C*E)/(y_array.size()*B-E*E);
		t0= (B*C-D*E)/(y_array.size()*B-E*E);
		tan_incident_angle= 1.0/dtdx;
		gem_tdc_hit_y= (drift_gap_center-t0)/dtdx;
	}else{
		double A= 0.0;
		for(int i=0; i<(int)x_array.size(); i++){
			A+= x_array[i]-tan_incident_angle*time_array_x[i];
			gem_cluster_charge_x+= peak_array_x[i];
		}
		double tdc_x0= A/x_array.size();
		gem_tdc_hit_x= tan_incident_angle*drift_gap_center+tdc_x0;

		A= 0.0;
		for(int i=0; i<(int)y_array.size(); i++){
			A+= y_array[i]-tan_incident_angle*time_array_y[i];
			gem_cluster_charge_y+= peak_array_y[i];
		}
		double tdc_y0= A/y_array.size();
		gem_tdc_hit_y= tan_incident_angle*drift_gap_center+tdc_y0;

	}
}

void E16ANA_GTRAnalyzer::PutSignals(){
	for(int ch=0; ch<n_strip_x; ch++){
		for(int i=0; i<2; i++){
			if(gem_fit_peak_x[ch][i]>0 && gem_fit_time0_x[ch][i]<800){
				E16ANA_GTRSignal asignal;
				asignal.height= gem_fit_peak_x[ch][i];
				asignal.time0= gem_fit_time0_x[ch][i];
				asignal.trise= gem_fit_trise_x[ch][i];
				asignal.strip= ch;
				signals_array_x.push_back(asignal);
			}
		}
	}
}

void E16ANA_GTRAnalyzer::CalcClustering(){
	std::vector<double> charge_clustering;
	std::vector<double> time0_clustering;
	std::vector<double> trise_clustering;
	std::vector<int> strip_clustering;
	std::vector<int> rise_flag_clustering;
	std::sort(signals_array_x.begin(), signals_array_x.end());
	for(int i=0; i<(int)signals_array_x.size(); i++){
		bool it_is_new= true;
		double charge_tmp= signals_array_x[i].height;
		double time0_tmp= signals_array_x[i].time0;
		double trise_tmp= signals_array_x[i].trise;
		int strip_tmp= signals_array_x[i].strip;
		//std::cout<< "strip="<< strip_tmp<< ", time0="<< time0_tmp<< ", charge="<< charge_tmp;
		for(int j=0; j<(int)clusters_array_x.size(); j++){
			bool it_belongs_to_this_cluster= true;

			// clustering condition
			if(strip_tmp!=strip_clustering[j]+1){
				it_belongs_to_this_cluster= false;
			}

			if(time0_tmp<time0_clustering[j]-100.0 || time0_tmp>time0_clustering[j]+100.0){
				it_belongs_to_this_cluster= false;
			}

			if(!rise_flag_clustering[j] && charge_tmp>charge_clustering[j]){
				it_belongs_to_this_cluster= false;
			}

			// clustering condition end

			if(it_belongs_to_this_cluster){
				clusters_array_x[j]->Put(charge_tmp, strip_tmp*strip_pitch_x, time0_tmp*drift_velocity);
				if(rise_flag_clustering[j] && charge_tmp<charge_clustering[j]){
					rise_flag_clustering[j]= 0;
				}
				charge_clustering[j]= charge_tmp;
				time0_clustering[j]= time0_tmp;
				trise_clustering[j]= trise_tmp;
				strip_clustering[j]= strip_tmp;
				it_is_new= false;
				//std::cout<< ", cluster # "<< j<< std::endl;
				break;
			}
		}
		if(it_is_new){
			E16ANA_GTROneCluster *acluster= new E16ANA_GTROneCluster();
			acluster->Put(charge_tmp, strip_tmp*strip_pitch_x, time0_tmp*drift_velocity);
			clusters_array_x.push_back(acluster);
			charge_clustering.push_back(charge_tmp);
			time0_clustering.push_back(time0_tmp);
			trise_clustering.push_back(trise_tmp);
			strip_clustering.push_back(strip_tmp);
			rise_flag_clustering.push_back(1);
			//std::cout<< ", cluster # "<< clusters_array_x.size()-1<< std::endl;
		}
	}
	gem_num_signals_x= signals_array_x.size();
	gem_num_clusters_x= clusters_array_x.size();
	double A= 0.0;
	int n=0;
	for(int i=0; i<(int)clusters_array_x.size(); i++){
		//std::cout<< "cluster # "<< i<< ", num_signal="<< clusters_array_x[i]->GetNumSignals()<< std::endl;
		if(clusters_array_x[i]->GetNumSignals()>1){
			//std::cout<< "cluster_pos="<< clusters_array_x[i]->GetClusterPosition()
			//		<< ", cluster_timing="<< clusters_array_x[i]->GetClusterTiming()<< std::endl;
			A+= (clusters_array_x[i]->GetClusterPosition())
				-tan_incident_angle*(clusters_array_x[i]->GetClusterTiming());
			n++;
		}
	}
	if(n==0){
		gem_mc_hit_x= -1.0;
	}else{
		gem_mc_hit_x= tan_incident_angle*drift_gap_center+A/(double)n;
	}
	//std::cout<< "gem_mc_hit_x="<< gem_mc_hit_x<< std::endl;
}

void E16ANA_GTRAnalyzer::SetTrackProjectionX(double _track_projection_x){
	track_projection_x= _track_projection_x-x_start;
	left_strip_x= (int)((track_projection_x-track_analysis_window)/strip_pitch_x);
	if(left_strip_x<0 || left_strip_x>n_strip_x) left_strip_x= 0;
	right_strip_x= (int)((track_projection_x+track_analysis_window)/strip_pitch_x)+1;
	if(right_strip_x<0 || right_strip_x>n_strip_x) right_strip_x= n_strip_x;
   //std::cout << "E16ANA_GTRAnalyzer::SetTrackProjectionX(), left = " << left_strip_x << ", right = " << right_strip_x << std::endl;
}

void E16ANA_GTRAnalyzer::SetTrackProjectionY(double _track_projection_y){
	track_projection_y= _track_projection_y-y_start;
	left_strip_y= (int)((track_projection_y-track_analysis_window)/strip_pitch_y);
	if(left_strip_y<0 || left_strip_y>n_strip_y) left_strip_y= 0;
	right_strip_y= (int)((track_projection_y+track_analysis_window)/strip_pitch_y)+1;
	if(right_strip_y<0 || right_strip_y>n_strip_y) right_strip_y= n_strip_y;
	//std::cout<< left_strip_y<< " "<< right_strip_y<< std::endl;
}


