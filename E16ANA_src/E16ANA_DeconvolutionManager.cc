//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai
//2015-11-02, uploaded by yokkaich
//2015-11-02, uploaded by komatsu
//2015-03-27, uploaded by yokkaich
//2015-03-20, uploaded by nakai

#include <cmath>
#include <iostream>

#include "E16ANA_DeconvolutionManager.hh"

E16ANA_DeconvolutionManager::E16ANA_DeconvolutionManager(int n_fadc_counts_) : 
   n_fadc_counts(n_fadc_counts_)
{
   fadc = new double[n_fadc_counts];
   response = new float[n_fadc_counts];
   response_mat = new float*[n_fadc_counts];
   for(int i=0; i<n_fadc_counts; i++){
      response_mat[i] = new float[n_fadc_counts];
   }
   result = new float[n_fadc_counts];

	for(int i=0; i<n_fadc_counts; i++){
		fadc[i]= -1.0;
	}
	pedestal= 0.0;
	SetResponse();
	SetResponseMat();
	s_fadc= new TSpectrum();
}

E16ANA_DeconvolutionManager::~E16ANA_DeconvolutionManager(){
   delete [] fadc;
   delete [] response;

   for(int i=0; i<n_fadc_counts; i++){
      delete [] response_mat[i];
   }
   delete [] response_mat;
   delete [] result;
	delete s_fadc;
}

double E16ANA_DeconvolutionManager::CalcResponse(int count){
	double tau= 80.0;
	double time= count*10.0;
	return time/tau*exp(-time/tau);
}

void E16ANA_DeconvolutionManager::SetResponse(){
	for(int i=0; i<n_fadc_counts; i++){
		response[i]= CalcResponse(i);
		//response[i]= CalcResponse(i);
	}
}

void E16ANA_DeconvolutionManager::SetResponseMat(){
	for(int i=0; i<n_fadc_counts; i++){
		for(int j=0; j<i+1; j++){
			response_mat[i][j]= CalcResponse(i-j+8);
		}
	}
}

void E16ANA_DeconvolutionManager::DoDeconvolution1(){
	double pedestal2= 0.0;
	for(int i=0; i<10; i++){
		pedestal2+= (double)fadc[i]/10.0;
	}
	for(int i=0; i<n_fadc_counts; i++){
		//result[i]= fadc[i]-pedestal;
		result[i]= fadc[i]-pedestal2;
	}
	//s_fadc->DeconvolutionRL(result, response, n_fadc_counts, 1000, 1, 1.2);
	s_fadc->Deconvolution(result, response, n_fadc_counts, 1000, 1, 1.2);
/*
	for(int i=0; i<n_fadc_counts; i++){
		std::cout<< result[i]<< std::endl;
	}
*/
}

void E16ANA_DeconvolutionManager::DoDeconvolution2(){
	for(int i=0; i<n_fadc_counts; i++){
		result[i]= fadc[i]-pedestal;
		for(int j=0; j<i; j++){
			result[i]-= response_mat[i][j]*result[j];
		}
		result[i]/= response_mat[i][i];
	}
}

