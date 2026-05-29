//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai
//2015-11-02, uploaded by yokkaich
//2015-11-02, uploaded by komatsu
//2015-03-27, uploaded by yokkaich
//2015-03-20, uploaded by nakai
#ifndef E16ANA_GTRSignal_hh
#define E16ANA_GTRSignal_hh 1

struct E16ANA_GTRSignal{
	double height;
	double time0;
	double trise;
	int strip;
	friend bool operator<(E16ANA_GTRSignal s0, E16ANA_GTRSignal s1){
		if(s0.strip!=s1.strip){
			return s0.strip<s1.strip;
		}else{
			return s0.time0<s1.time0;
		}
	};
};

#endif // E16ANA_GTRSignal_hh

