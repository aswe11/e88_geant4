//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai
//2015-12-12, uploaded by nakai
#include "E16ANA_WaveformOutputData.hh"
#include <iostream>

E16ANA_WaveformEvent::E16ANA_WaveformEvent(){
   Clear();
}

E16ANA_WaveformEvent::~E16ANA_WaveformEvent(){
}

void E16ANA_WaveformEvent::Clear(){
   for(int i=0; i<3; i++){
      for(int j=0; j<33; j++){
         module_waveforms[i][j].clear();
      }
   }
}

void E16ANA_WaveformEvent::SetWaveform(int layer, int module, int ch, const std::vector<int> &wf){
   module_waveforms[layer][module].push_back(waveform_t(ch,wf));
}

int E16ANA_WaveformEvent::Read(FILE *fp){
   int ncall = 0;
   std::vector<int> wf_temp;
   for(int i=0; i<3; i++){
      for(int j=0; j<33; j++){
         int n_hits = 0, n_sampling, module_id, layer_id;
         ncall += fread(&layer_id, sizeof(int), 1, fp);
         ncall += fread(&module_id, sizeof(int), 1, fp);
         ncall += fread(&n_hits, sizeof(int), 1, fp);
         ncall += fread(&n_sampling, sizeof(int), 1, fp);
         for(int k=0; k<n_hits; k++){
            int ch;
            ncall += fread(&ch, sizeof(int), 1, fp);
            wf_temp.resize(n_sampling);
            ncall += fread(&wf_temp[0], sizeof(int), n_sampling, fp);
            this->SetWaveform(layer_id, module_id, ch, wf_temp);
         }
      }
   }
   return ncall;
}

int E16ANA_WaveformEvent::Write(FILE *fp){
   int ncall = 0;
   for(int i=0; i<3; i++){
      for(int j=0; j<33; j++){
         int n_hits = module_waveforms[i][j].size();
         int n_sampling = 21;
         if(n_hits > 0){
            n_sampling = module_waveforms[i][j][0].waveform.size();
         }
         ncall += fwrite(&i, sizeof(int), 1, fp);
         ncall += fwrite(&j, sizeof(int), 1, fp);
         ncall += fwrite(&n_hits, sizeof(int), 1, fp);
         ncall += fwrite(&n_sampling, sizeof(int), 1, fp);
         for(int k=0; k<n_hits; k++){
            int ch = module_waveforms[i][j][k].ch;
            std::vector<int> &wf_temp = module_waveforms[i][j][k].waveform;
            ncall += fwrite(&ch, sizeof(int), 1, fp);
            ncall += fwrite(&wf_temp[0], sizeof(int), n_sampling, fp);
         }
      }
   }
   return ncall;
}

E16ANA_WaveformOutputData::E16ANA_WaveformOutputData(const std::string &_file_name) : 
   file_mode(-1), fp(NULL), file_name(_file_name)
{
   event = new E16ANA_WaveformEvent();
}

E16ANA_WaveformOutputData::~E16ANA_WaveformOutputData(){
   if(fp != NULL){
      fclose(fp);
   }
   delete event;
}

int E16ANA_WaveformOutputData::OpenReadFile(){
   if(fp != NULL){
      std::cout << "E16ANA_WaveformOutputData::OpenReadFile() : file is already opened." << std::endl; 
      return 0;
   }
   fp = fopen(file_name.c_str(), "r");
   if(fp == NULL){
      std::cout << "E16ANA_WaveformOutputData::OpenReadFile() : file open fail. " << file_name << std::endl; 
      return 0;
   }
   file_mode = 0;
   return 1;
}

int E16ANA_WaveformOutputData::OpenWriteFile(){
   if(fp != NULL){
      std::cout << "E16ANA_WaveformOutputData::OpenWriteFile() : file is already opened." << std::endl; 
      return 0;
   }
   fp = fopen(file_name.c_str(), "w");
   if(fp == NULL){
      std::cout << "E16ANA_WaveformOutputData::OpenWriteFile() : file open fail. " << file_name << std::endl; 
      return 0;
   }
   file_mode = 1;
   return 1;
}

