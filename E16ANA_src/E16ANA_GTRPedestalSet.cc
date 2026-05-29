//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai
//2015-11-02, uploaded by yokkaich
//2015-11-02, uploaded by komatsu
//2015-03-27, uploaded by yokkaich
//2015-03-20, uploaded by nakai

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include <cstdlib>

#include "E16ANA_GTRPedestalSet.hh"

void E16ANA_GTRPedestalSet::Clear(){
	for(int i=0; i<GetNumPed(); i++){
		delete ped_array[i];
	}
	ped_array.clear();
	return;
}

void E16ANA_GTRPedestalSet::Initialize(std::string filename){
	//NConfReader &gConf= NConfReader::GetInstance();
	//std::string filename= gConf.GetFileName(file_type);
	std::cout<< "[E16ANA_GTRPedestalSet::Initialize()] "<< filename<< std::endl;
	std::ifstream ifs(filename.c_str());

	if(ifs.fail()){
		std::cout<< "PedFile open fail"<< std::endl;
		return;
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
		double pedestal= atof(param[1].c_str());
		double sigma= atof(param[2].c_str());
		E16ANA_GTRPedestal *pedo= new E16ANA_GTRPedestal(ch, pedestal, sigma);
		this->PutPed(pedo);
	}
	ifs.close();
}

