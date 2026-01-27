//2016-08-12, uploaded by nakai
//2016-05-05, uploaded by yokkaich
//2016-03-18, uploaded by nakai
//2015-03-01, uploaded by yokkaich
//2015-01-20, uploaded by yokkaich
//2015-01-05, uploaded by yokkaich
//2015-01-05, uploaded by yokkaich
//2014-08-27, uploaded by kawama
//2014-08-27, uploaded by kawama
//2014-04-30, uploaded by kawama
//2013-11-14, uploaded by kawama
//2013-05-13, modified by kawama
/*
   E16G4_Analysis.hh

   D.Kawama
   */

#ifndef E16G4_Analysis_h
#define E16G4_Analysis_h 1


#include <vector>

#include <globals.hh>
#include <G4ThreeVector.hh>
#include <Randomize.hh>


#ifdef USE_CSTREAM
#include <cstdio>
#else
#include <fstream>
#endif
#include <iostream>

#include "E16G4_AnalysisMessenger.hh"
#include "E16G4_GTRFrameHit.hh"

//class TTree;

class G4Run;
class G4Event;
class G4Step;
class G4HCofThisEvent;

class E16G4_AnalysisMessenger;
class E16ANA_ParamManager;
class E16ANA_G4Hit;

class E16G4_Analysis 
{
public:
  E16G4_Analysis();
  virtual ~E16G4_Analysis();

  void BeginOfRun( const G4Run *aRun );
  void EndOfRun( const G4Run *aRun );
  void BeginOfEvent( const G4Event *anEvent );
  void EndOfEvent( const G4Event *anEvent );
  void PrimaryGeneration( int n, int m, const G4int PID[], 
    const G4ThreeVector pos[],const G4ThreeVector mom[], 
    const G4int ID);
  void PrimaryGeneration( int n, const G4int PID[], 
    const G4ThreeVector pos[],const G4ThreeVector mom[], 
    const G4int ID);
  void PrimaryGenerationMulti( int n, int m, 
    std::vector<G4int>& PID,
    std::vector<G4ThreeVector>& pos,
    std::vector<G4ThreeVector>& mom,
    const G4int ID);
  void SaveFile( void ) const;
  void Terminate( void ) const;
  void InitializeEvent( void );
  void SetActive( void ) { fActive_=true; }
  void SetInActive( void ) { fActive_=false; }

private:
  E16ANA_ParamManager *paramMgr;
  //TTree *tree;
  G4String filename_,title;
  G4int trigNum;
  G4bool fTriggered;
  G4bool fActive_;
  G4bool fOutput;
  G4int evID_G4, genID, evID_JAM, mulID_JAM, mulNum_JAM;
  G4int nGen;
  G4int nTAR1hit;
  G4int nTAR2hit;
  G4int nTAR3hit;
  G4int nGTR1hit;
  G4int nGTR2hit;
  G4int nGTR3hit;
  G4int nGTRFrame1hit;
  G4int nGTRFrame2hit;
  G4int nGTRFrame3hit;
  G4int nHBDhit;
  G4int nLGhit;
  G4int nLGVDhit;
  G4int nVDCylhit[4];
  G4bool GTRTrig;
  /*static const G4int nGTR;
    static const G4int nGTRGEM;
    //Target quantities
    static const G4int nTar;
    //Tracker GEM quantities
    static const G4int nTrGEM;
    //HBD quantities
    static const G4int nHBD;
    //LG quantities
    static const G4int nLG;
    //LGVD quantities
    static const G4int nLGVD;*/
  //VD quantities
  enum{nVD=8};
  E16G4_AnalysisMessenger* anaMessenger;   //messenger of this class

  const static G4double length_unit;

  //TTree *treeVD[nVD];
  enum {maxTrkID=10000};

  void NormalAnalysis( const G4Event *anEvent );
  void defineHistograms( void );

  void G4OutputDataTargetFill( G4HCofThisEvent* HCE);

  // These methods should be unified by using template or super class of E16G4_***Hit. :)
  void G4OutputDataSSDFill(  G4HCofThisEvent* HCE);
  void G4OutputDataRPCFill(  G4HCofThisEvent* HCE);
  void G4OutputDataGTRFill(  G4HCofThisEvent* HCE);
  void G4OutputDataGTRFrameFill(  G4HCofThisEvent* HCE);
  void G4OutputDataGTRFrameFillSub(GTRFrameHitsCollection *hc, int moduleID, int layerID, std::map<int, E16ANA_G4Hit> &hits);
  void G4OutputDataHBDFill(  G4HCofThisEvent* HCE);
  void G4OutputDataHBDFill_Nakai(  G4HCofThisEvent* HCE);
  void G4OutputDataLGFill(  G4HCofThisEvent* HCE);
  void G4OutputDataLGVDFill(  G4HCofThisEvent* HCE);

  void G4OutputDataFill( const G4Event *anEvent );
  void G4OutputDataTrajectoryFill(const G4Event* anEvent);


public:
  static const double cutMomentum;
  G4bool GetTriggerStatus( void ) const { return fTriggered; }
  //void SetEventID( G4int id ) { evID_ = id; }
  G4double *dETar;
  typedef struct {
    G4double x, y, z;
    G4double p;
    G4double px,py,pz;
    G4int PID;
  } GENData;
  GENData *datGEN;

  typedef struct {
    G4int trkid;
    G4double x, y, z;
    G4double p;
    G4double px,py,pz;
    G4int PID;
  } TARData;
  TARData *datTAR1;
  TARData *datTAR2;
  TARData *datTAR3;
      
  typedef struct {
    G4int trkid;
    G4int id;
    G4double x, y, z;
    G4double xg, yg, zg;
    G4double p;
    G4double px,py,pz;
    G4int PID;
    G4int acc;
  } GTRData;
  GTRData *datGTR1;
  GTRData *datGTR2;
  GTRData *datGTR3;
      
  typedef struct {
    G4int trkid;
    G4int id;
    G4double xg, yg, zg;
    G4int PID;
  } GTRFrameData;
  GTRFrameData *datGTRFrame1;
  GTRFrameData *datGTRFrame2;
  GTRFrameData *datGTRFrame3;

  typedef struct {
    G4int trkid;
    G4int id;
    G4double x, y, z;
    G4double xg, yg, zg;
    G4double p;
    G4double px,py,pz;
    G4double npe,fl,beta;
    G4int PID;
  } HBDData;
  HBDData *datHBD;

  typedef struct {
    /*G4double x, y, z;
      G4double xg, yg, zg;
      G4double p;
      G4double px,py,pz;*/
    G4double fl,beta,edep;
    G4int PID;
  } LGData;
  LGData *datLG;

  typedef struct {
    G4int trkid;
    G4int id;
    G4double x, y, z;
    G4double xg, yg, zg;
    G4double p;
    G4double px,py,pz;
    G4int PID, proc;
  } VDData;
  VDData *datVD;
  VDData *datLGVD;
      
  typedef struct {
    G4int trkid;
    G4int id;
    G4double xg, yg, zg;
    G4double p;
    G4double px,py,pz;
    G4int PID, proc;
  } VDCylData;
  VDCylData *datVDCyl1;
  VDCylData *datVDCyl2;
  VDCylData *datVDCyl3;
  VDCylData *datVDCyl4;

  typedef struct {
    G4int GTR[27];
  } trigData;
  trigData datTrig;


  typedef struct {
    G4int partID;
    G4int parentID;
    G4int GEN;
    double posx_vtx;
    double posy_vtx;
    double posz_vtx;
    /*G4String procName;
      G4String volName;
      G4String volName_vtx;*/
    //char procName[10];
    char procName[20];
    char volName[30];
    char volName_vtx[30];
  } decayChain;
  decayChain *datDC;
  void outputDC(int trkID, int flag);

  void SetOutFile(G4String val) { filename_ = val;}

private:
#if 0
  G4double RandGauss( G4double center, G4double dev );
#endif
  void InitializeGTRData(GTRData data);

  //sako 2019/10/7 limit for n hits
  static const int maxhit = 10000;//def=100
};

#endif//E16G4_Analysis_h

