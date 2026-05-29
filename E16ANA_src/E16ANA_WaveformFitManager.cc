//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai
//2015-11-02, uploaded by yokkaich
//2015-11-02, uploaded by komatsu
//2015-03-27, uploaded by yokkaich
//2015-03-20, uploaded by nakai

#include <cmath>

#include "E16ANA_WaveformFitManager.hh"

E16ANA_WaveformFitManager::E16ANA_WaveformFitManager(int n_fadc_counts_) : 
   n_fadc_counts(n_fadc_counts_),
   default_t0(50.0), default_peak(50.0), default_tau(50.0)
{
   fadc = new double[n_fadc_counts];
	for(int i=0; i<n_fadc_counts; i++){
		fadc[i]= -1.0;
	}
	params[0]= default_t0;
	params[1]= default_peak;
	params[2]= default_tau;
	params[3]= 0.0;
	params[4]= 0.0;
	pedestal= 0.0;
	chi2= 0.0;
}

E16ANA_WaveformFitManager::~E16ANA_WaveformFitManager(){
   delete [] fadc;
}

Double_t FitFunction1(double *x, double *par){
	double xx= x[0];
	double base_line= par[0];
	double tau= par[1];
	double t0= par[2];
	double peak= par[3];
	double tr= par[4];
	double result= base_line;
	if(xx<t0){
		return result;
	}else if(xx<t0+tr){
		result+= peak/tr*exp(-(xx-t0)/tau+1)*(tau*(exp((xx-t0)/tau)-1)-(xx-t0));
	}else{
		result+= peak/tr*exp(-(xx-t0)/tau+1)*((xx-t0+tau)*(exp(tr/tau)-1)-tr*exp(tr/tau));
	}
	return result;
}

Double_t FitFunction2(double *x, double *par){
	double xx= x[0];
	double base_line= par[0];
	double tau= par[1];
	double t0[2];
	double peak[2];
	double tr[2];

	for(int i=0; i<2; i++){
		t0[i]= par[2+i*3];
		peak[i]= par[3+i*3];
		tr[i]= par[4+i*3];
	}
	//tr[0]= par[4];
	//tr[1]= par[4];

	double result= base_line;
	for(int i=0; i<2; i++){
		if(xx<t0[i]){
		}else if(xx<t0[i]+tr[i]){
			result+= peak[i]/tr[i]*exp(-(xx-t0[i])/tau+1)*(tau*(exp((xx-t0[i])/tau)-1)-(xx-t0[i]));
		}else{
			result+= peak[i]/tr[i]*exp(-(xx-t0[i])/tau+1)*((xx-t0[i]+tau)*(exp(tr[i]/tau)-1)-tr[i]*exp(tr[i]/tau));
		}
	}
	return result;
}

void E16ANA_WaveformFitManager::SetFadc(double *afadc){
	memcpy(fadc, afadc, sizeof(fadc));
}

void E16ANA_WaveformFitManager::SetPedestal(double apedestal){
	pedestal= apedestal;
}

void E16ANA_WaveformFitManager::DoFit(){
	TH1F *h_wave= new TH1F("h_wave", "waveform", n_fadc_counts, -5.0, n_fadc_counts*10.0-5.0);
	for(int i=0; i<n_fadc_counts; i++){
		h_wave->Fill(i*10.0, fadc[i]);
	}
	double peak_bin_value;
	int peak_bin;
	peak_bin_value= h_wave->GetMaximum()-pedestal;
	peak_bin= (h_wave->GetMaximumBin()-1)*10;
	if(peak_bin_value>4.0){

/*
		TF1 *func= new TF1("func", FitFunction1, 0.0, n_fadc_counts*10.0, 5);
		func->SetParameters(peak_bin-default_tau, peak_bin_value, default_tau, 10.0, pedestal);
		func->SetParLimits(0, peak_bin-default_tau-100.0, peak_bin-default_tau+100.0);
		func->SetParLimits(1, 3.0, peak_bin_value+10.0);
		//func->SetParLimits(2, default_tau-10.0, default_tau+10.0);
		func->FixParameter(2, default_tau);
		func->SetParLimits(3, 0.0, 500.0);
		//func->FixParameter(3, 0.0);
		func->FixParameter(4, pedestal);
*/

		TF1 *func= new TF1("func", FitFunction2, 0.0, n_fadc_counts*10.0, 8);
		func->SetParameters(pedestal, default_tau, 
							peak_bin-default_tau*2.0, peak_bin_value, 150.0, 
							peak_bin-default_tau, peak_bin_value, 150.0);
		func->FixParameter(0, pedestal);
		//func->FixParameter(1, default_tau);
		func->SetParLimits(1, default_tau-20.0, default_tau+20.0);
		func->SetParLimits(2, 0.0, peak_bin-default_tau);
		//func->SetParLimits(2, 0.0, peak_bin-default_tau);
		func->SetParLimits(3, 3.0, peak_bin_value+10.0);
		func->SetParLimits(4, 0.0, 300.0);
		func->SetParLimits(5, peak_bin-default_tau-200.0, peak_bin-default_tau+300.0);
		func->SetParLimits(6, 3.0, peak_bin_value+10.0);
		func->SetParLimits(7, 0.0, 300.0);

		//h_wave->Fit("func", "");
		h_wave->Fit("func", "QN");

		func->GetParameters(params);
		chi2= func->GetChisquare();
		delete func;
		double t0_small, tr_small, t0_large;
		if(params[2]<params[5]){
			t0_small= params[2];
			tr_small= params[4];
			t0_large= params[5];
		}else{
			t0_small= params[5];
			tr_small= params[7];
			t0_large= params[2];
		}
		if(t0_large<t0_small+tr_small){
			TF1 *func= new TF1("func", FitFunction1, 0.0, n_fadc_counts*10.0, 5);
			func->SetParameters(pedestal, default_tau, peak_bin-default_tau, peak_bin_value, 150.0);
			func->FixParameter(0, pedestal);
			func->SetParLimits(1, default_tau-10.0, default_tau+10.0);
			//func->FixParameter(1, default_tau);
			func->SetParLimits(2, peak_bin-default_tau-200.0, peak_bin-default_tau+200.0);
			func->SetParLimits(3, 3.0, peak_bin_value+10.0);
			func->SetParLimits(4, 0.0, 300.0);

			h_wave->Fit("func", "QN");
			func->GetParameters(params);
			chi2= func->GetChisquare();
			delete func;
			for(int i=5; i<8; i++){
				params[i]= -1.0;
			}
		}
	}else{
		for(int i=0; i<8; i++){
			params[i]= -1.0;
		}
	}
	delete h_wave;
}

void E16ANA_WaveformFitManager::GetParams(double *par){
	memcpy(par, params, sizeof(params));
}

double E16ANA_WaveformFitManager::GetChi2(){
	return chi2;
}

