//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai
//2015-11-02, uploaded by yokkaich
//2015-11-02, uploaded by komatsu
//2015-03-27, uploaded by yokkaich
//2015-03-20, uploaded by nakai
#ifndef E16ANA_GTRPedestal_hh
#define E16ANA_GTRPedestal_hh 1

class E16ANA_GTRPedestal{
private:
	int ch;
	double pedestal;
	double sigma;

public:
	E16ANA_GTRPedestal(){};
	E16ANA_GTRPedestal(int _ch, double _pedestal, double _sigma);
	virtual ~E16ANA_GTRPedestal(){};
	int GetCh(){return ch;};
	double GetPedestal(){return pedestal;};
	double GetSigma(){return sigma;};
};

#endif // E16ANA_GTRPedestal_hh

