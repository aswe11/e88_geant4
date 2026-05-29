//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai
//2015-11-02, uploaded by yokkaich
//2015-11-02, uploaded by komatsu
//2015-04-08, uploaded by nakai
//2015-03-27, uploaded by yokkaich
//2015-03-20, uploaded by nakai

#include "E16ANA_GTROneCluster.hh"

E16ANA_GTROneCluster::E16ANA_GTROneCluster(){
}

E16ANA_GTROneCluster::~E16ANA_GTROneCluster(){
}

void E16ANA_GTROneCluster::Put(double acharge, double aposition, double atiming){
	charge.push_back(acharge);
	position.push_back(aposition);
	timing.push_back(atiming);
}

double E16ANA_GTROneCluster::GetClusterCharge(){
	double cluster_charge= 0.0;
	for(int i=0; i<(int)charge.size(); i++){
		cluster_charge+= charge[i];
	}
	return cluster_charge;
}

double E16ANA_GTROneCluster::GetClusterPosition(){
	double cluster_charge= 0.0;
	double cluster_position= 0.0;
	for(int i=0; i<(int)charge.size(); i++){
		cluster_charge+= charge[i];
		cluster_position+= charge[i]*position[i];
	}
	if((int)charge.size() != 0){
		cluster_position/= cluster_charge;
	}else{
		cluster_position= -255.0;
	}
	return cluster_position;
}

double E16ANA_GTROneCluster::GetClusterTiming(){
	double cluster_charge= 0.0;
	double cluster_timing= 0.0;
	for(int i=0; i<(int)charge.size(); i++){
		cluster_charge+= charge[i];
		cluster_timing+= charge[i]*timing[i];
	}
	if((int)charge.size() != 0){
		cluster_timing/= cluster_charge;
	}else{
		cluster_timing= -255.0;
	}
	return cluster_timing;
}

