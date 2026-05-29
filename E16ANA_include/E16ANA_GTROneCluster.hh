//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai
//2015-11-02, uploaded by yokkaich
//2015-11-02, uploaded by komatsu
//2015-03-27, uploaded by yokkaich
//2015-03-20, uploaded by nakai
#ifndef E16ANA_GTROneCluster_hh
#define E16ANA_GTROneCluster_hh 1

#include <vector>

class E16ANA_GTROneCluster{
private:
	std::vector<double> charge;
	std::vector<double> position;
	std::vector<double> timing;

public:
	E16ANA_GTROneCluster();
	virtual ~E16ANA_GTROneCluster();
	void Put(double acharge, double aposition, double atiming);

	int GetNumSignals(){return charge.size();};
	double GetClusterCharge();
	double GetClusterPosition();
	double GetClusterTiming();

};

#endif // E16ANA_GTROneCluster_hh

