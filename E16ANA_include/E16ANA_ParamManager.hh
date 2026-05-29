//2017-12-03, uploaded by yokkaich
//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai
//2015-11-02, uploaded by yokkaich
//2015-11-02, uploaded by komatsu
//2015-07-15, uploaded by nakai
//2015-03-27, uploaded by yokkaich
//2015-01-05, uploaded by yokkaich
//2015-01-05, uploaded by yokkaich
//2014-08-27, uploaded by kawama
//2014-05-07, uploaded by kawama
//2014-04-30, uploaded by kawama
//2014-04-24, uploaded by kawama
//2013-11-14, uploaded by kawama
//2013-05-13, modified by kawama
//2013-05-13, modified by kawama
/*
   E16ANA_ParamMan.hh

   D.Kawama
   */

#ifndef E16ANA_ParamManager_h
#define E16ANA_ParamManager_h 1

#define MAXCHAR 144


class E16ANA_ParamManager
{
   public:
      E16ANA_ParamManager( const char *fileName );

   private:
      static E16ANA_ParamManager *thisPointer;

      bool ReadParam( void );

   private:
      enum {
         maxGTRId = 33
      };
      // Input file name
      const char *fileName;

      // ROOT file name
      char ROOTFileName[MAXCHAR];
      char LogFileName[MAXCHAR];
      char MapFileName[MAXCHAR];
      char GeomFileName[MAXCHAR];
      char GTR100AnalysisParamFileName[maxGTRId][MAXCHAR];
      char GTR200AnalysisParamFileName[maxGTRId][MAXCHAR];
      char GTR300AnalysisParamFileName[maxGTRId][MAXCHAR];
      char GTR100ResponseParamFileName[maxGTRId][MAXCHAR];
      char GTR200ResponseParamFileName[maxGTRId][MAXCHAR];
      char GTR300ResponseParamFileName[maxGTRId][MAXCHAR];

      // Generation Type
      int GenerationID;
      // Particle Type
      int ParticleID;
      // input for dummy data
      char InputDataName[MAXCHAR];
      // Interpolation Type
      int ipFlag;
      int evStart;
      long rseed;


   public:
      static E16ANA_ParamManager *GetParamManager(){ return thisPointer; }

      char *GetROOTFileName( void ){ return ROOTFileName; }
      char *GetMapFileName( void ){ return MapFileName; }
      char *GetGeomFileName( void ){ return GeomFileName; }
      char *GetInputDataName( void ){ return InputDataName; }
      char *GetGTR100AnalysisParamFileName(int id){return GTR100AnalysisParamFileName[id];};
      char *GetGTR200AnalysisParamFileName(int id){return GTR200AnalysisParamFileName[id];};
      char *GetGTR300AnalysisParamFileName(int id){return GTR300AnalysisParamFileName[id];};
      char *GetGTR100ResponseParamFileName(int id){return GTR100ResponseParamFileName[id];};
      char *GetGTR200ResponseParamFileName(int id){return GTR200ResponseParamFileName[id];};
      char *GetGTR300ResponseParamFileName(int id){return GTR300ResponseParamFileName[id];};

      int GetGenerationID( void ){ return GenerationID; }
      int GetipFlag( void ){ return ipFlag; }
      int GetEvOffset( void ){ return evStart; }
      long GetRandomSeed( void ){ return rseed; }

      void SetGeomFileName( char* name ){ 
	sprintf(GeomFileName, "%s", name);
      }
};

#endif// E16ANA_ParamManager_h


