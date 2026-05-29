//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai
//2015-12-12, uploaded by nakai
#ifndef E16ANA_WaveformOutputData_hh
#define E16ANA_WaveformOutputData_hh

#include <stdio.h>
#include <vector>
#include <string>

class E16ANA_WaveformEvent {
public:
   //E16ANA_WaveformEvent(int _n_sampling = 21);
   E16ANA_WaveformEvent();
   ~E16ANA_WaveformEvent();
   void Clear();

   void SetWaveform(int layer, int module, int ch, const std::vector<int> &wf);
   int Read(FILE *fp);
   int Write(FILE *fp);
   int GetNumHits(int layer, int module){return (int)module_waveforms[layer][module].size();};
   int GetChannel(int layer, int module, int i){return module_waveforms[layer][module][i].ch;};
   std::vector<int>& GetWaveform(int layer, int module, int i){
      return module_waveforms[layer][module][i].waveform;
   };

private:
   int n_sampling;
   struct waveform_t{
      int ch;
      std::vector<int> waveform;
      waveform_t(int _ch, const std::vector<int> &wf){
         ch = _ch;
         waveform = wf;
      };
   };
   std::vector<waveform_t> module_waveforms[3][33];
};

class E16ANA_WaveformOutputData {
public:
   E16ANA_WaveformOutputData(const std::string &file_name);
   ~E16ANA_WaveformOutputData();
   int OpenReadFile();
   int OpenWriteFile();
   int ReadAnEvent(){
      event->Clear();
      if(file_mode != 0){
         return 0;
      }
      return event->Read(fp);
   };
   int WriteAnEvent(){
      if(file_mode != 1){
         return 0;
      }
      int ret = event->Write(fp);
      event->Clear();
      return ret;
   };
   E16ANA_WaveformEvent* Event(){return event;};

private:
   int file_mode;
   FILE *fp;
   std::string file_name;
   E16ANA_WaveformEvent *event;
};

#endif // E16ANA_WaveformOutputData_hh

