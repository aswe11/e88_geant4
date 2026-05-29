//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai
//2015-11-02, uploaded by yokkaich
//2015-11-02, uploaded by komatsu
//2015-03-27, uploaded by yokkaich
//2015-03-20, uploaded by nakai

#include "E16ANA_GTRPedestal.hh"

E16ANA_GTRPedestal::E16ANA_GTRPedestal(int _ch, double _pedestal, double _sigma){
	ch= _ch;
	pedestal= _pedestal;
	sigma= _sigma;
}

