//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai
//2015-11-02, uploaded by yokkaich
//2015-11-02, uploaded by komatsu
//2015-03-27, uploaded by yokkaich
//2015-03-20, uploaded by nakai
#ifndef E16ANA_DeconvlutionManager_hh
#define E16ANA_DeconvlutionManager_hh 1

#include <TSpectrum.h>

class E16ANA_DeconvolutionManager{
private:
   int n_fadc_counts;
	double *fadc;
	float *response;
	float **response_mat;
	//double response[STRIP_FADC_COUNT];
	float *result;
	//double result[STRIP_FADC_COUNT];
	double pedestal;
	TSpectrum *s_fadc;

	void SetResponse();
	void SetResponseMat();
	double CalcResponse(int count);
public:
	E16ANA_DeconvolutionManager(int n_fadc_counts_);
	virtual ~E16ANA_DeconvolutionManager();
	void DoDeconvolution1();
	void DoDeconvolution2();

	void SetFadc(double *afadc){memcpy(fadc, afadc, sizeof(fadc));};
	void SetPedestal(double apedestal){pedestal= apedestal;};

	void GetResult(float *aresult){memcpy(aresult, result, sizeof(result));};
	//void GetResult(float *aresult){memcpy(aresult, result, sizeof(float)*STRIP_FADC_COUNT);};
};

#endif // E16ANA_DeconvlutionManager_hh

