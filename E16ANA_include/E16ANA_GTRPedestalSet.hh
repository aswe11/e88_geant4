//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai
//2015-11-02, uploaded by yokkaich
//2015-11-02, uploaded by komatsu
//2015-03-27, uploaded by yokkaich
//2015-03-20, uploaded by nakai
#ifndef E16ANA_GTRPedestalSet_hh
#define E16ANA_GTRPedestalSet_hh 1

#include "E16ANA_GTRPedestal.hh"

#include <vector>
#include <string>

class E16ANA_GTRPedestalSet{
private:
	std::vector<E16ANA_GTRPedestal*> ped_array;
public:
	E16ANA_GTRPedestalSet(){};
	virtual ~E16ANA_GTRPedestalSet(){Clear();};
	virtual void Clear();
	void Initialize(std::string filename);
	void PutPed(E16ANA_GTRPedestal *pedo){ped_array.push_back(pedo);};
	int GetNumPed(){return ped_array.size();};
	E16ANA_GTRPedestal* GetPedestalObject(int nn){return ped_array[nn];};
};

#endif // E16ANA_GTRPedestalSet_hh

