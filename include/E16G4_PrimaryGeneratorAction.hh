//2016-09-23, uploaded by yokkaich
//2016-08-12, uploaded by nakai
//2015-09-05, uploaded by nakai
//2015-03-27, uploaded by yokkaich
//2015-01-20, uploaded by yokkaich
//2015-01-05, uploaded by yokkaich
//2015-01-05, uploaded by yokkaich
//2014-08-27, uploaded by kawama
//2014-08-27, uploaded by kawama
//2014-04-30, uploaded by kawama
//2013-11-14, uploaded by kawama
//2013-05-13, modified by kawama
// ====================================================================
//    E16G4_PrimaryGeneratorAction.hh
//
// ====================================================================
#ifndef E16G4_PRIMARY_GENERATOR_ACTION_H
#define E16G4_PRIMARY_GENERATOR_ACTION_H
 
#include <globals.hh>
#include <G4ThreeVector.hh>
#include <G4VUserPrimaryGeneratorAction.hh>
#include <G4ParticleTable.hh>

#include "E16G4_PrimaryGeneratorMessenger.hh"

class G4ParticleGun;
class E16G4_Analysis;
class E16G4_PrimaryGeneratorMessenger;
class E16ANA_ParamManager;
class E16G4_SanfordWangGenerator;

// ====================================================================
//
// class definition
//
// ====================================================================

class E16G4_PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
private:
  // use G4 particle gun
  G4ParticleTable* particleTable;
  //      G4ParticleGun* particleGun;
  G4ParticleGun* eGun;
  G4ParticleGun* pGun;
  E16G4_Analysis* anaMan;
  FILE *dataFile;

  G4int nev,iev,imul, genID,evOffset;
  G4int randomMulti;// multiplicity for the random track case

  G4double xPoint, yPoint, zPoint;
  G4double sigmaX, sigmaY, sigmaZ;
  G4int rndmSeed;
  G4int nEv;
  G4String inputFileName;
      
  //Run header
  int nev_,nevtotal_;
  float bmax_,bmin_;
  G4String projectile_, target_, cwin_, frame_;

  //Event header in JAM
  int* iev_;
  int* nmul_;
  int* nv_;
  int* nbary_;
  int* nmeson_;
  int* mseed_;
  int* mevent_;
  float* xJAM_;
  float* yJAM_;
  double* impJAM_;
  //Event variables
  double** pxJAM_;
  double** pyJAM_;
  double** pzJAM_;
  //float** EkJAM_;
  double** mJAM_;
  int** pidJAM_;

  //sako 2019/10/5
  FILE *fpi_event;
  static const int NtrackMAX=2000;
  int nmul_ev;
  double impJAM_ev;
  //Event variables
  double pxJAM_ev[NtrackMAX];
  double pyJAM_ev[NtrackMAX];
  double pzJAM_ev[NtrackMAX];
  //float** EkJAM_;
  double mJAM_ev[NtrackMAX];
  int pidJAM_ev[NtrackMAX];

  E16ANA_ParamManager *paramMgr;

  void SetMom(G4ThreeVector gMom, G4ThreeVector gPos, 
    G4Event* anEvent, G4int evID);
  void SetMom(G4ThreeVector gMom1, G4ThreeVector gPos1, 
    G4ThreeVector gMom2, G4ThreeVector gPos2, 
    G4Event* anEvent, G4int evID);
  E16G4_PrimaryGeneratorMessenger* gunMessenger;   //messenger of this class

  E16G4_SanfordWangGenerator *sw_gen; // for wire target events

public:
  E16G4_PrimaryGeneratorAction(E16G4_Analysis* anaManager=0);
  ~E16G4_PrimaryGeneratorAction();
  void SetPrimaryConditions();
  bool JAMDataRead_Phi(G4String inputFilename);
  //  bool JAMDataRead_Phi_2(G4String inputFilename);
  bool JAMDataRead_Pi0(G4String inputFilename);
  bool JAMDataRead_Pi0_2(G4String inputFilename);
  //sako 2019/10/5
  bool JAMDataOpen_Pi0_2_Event(G4String inputFilename);
  bool JAMDataRead_Pi0_2_Event(G4String inputFilename);
  bool JAMDataRead(G4String inputFilename);
  void JAMDataWrite(char* inputFIlename, char* outputFilename);

      
  virtual void GeneratePrimaries(G4Event* anEvent);
  void GeneratePie(G4Event* anEvent);
  //      void GenerateFromFile(G4Event* anEvent);
  void GeneratePrimaryBeam(G4Event* anEvent);
  void GenerateJAMEvent(G4Event* anEvent);
  void GenerateMultiTrackJAMEvent(G4Event* anEvent, bool wire = false);
  //sako 2019/10/5
  void GenerateMultiTrackJAMEvent_Event(G4Event* anEvent, bool wire = false);
  void GenerateWireTargetEvent(G4Event* anEvent, int module_id);
  void GenerateMultiTrackRandom(G4Event* anEvent, bool flag45=false);
  void GenerateMultiTrackRandom45(G4Event* anEvent){
    GenerateMultiTrackRandom(anEvent, true);
  }



  void SetRndmSeed(G4int val) { rndmSeed = val;}
  void SetGenID(G4int val) { genID = val;}
  void SetRandomMulti(G4int val) { randomMulti = val;}
  void SetSeedFile(G4String val) { inputFileName = val;}
  void SetEvOffset(G4int val) { evOffset = val;}
  void SetXPoint(G4double val) { xPoint = val;}
  void SetYPoint(G4double val) { yPoint = val;}
  void SetZPoint(G4double val) { zPoint = val;}
  void SetSigmaX(G4double val) { sigmaX = val;}
  void SetSigmaY(G4double val) { sigmaY = val;}
  void SetSigmaZ(G4double val) { sigmaZ = val;}
  G4int GetGenID() { return genID;}
  void SetProjectile( G4String &str ) { projectile_ = str; }
  void SetTarget( G4String &str ) { target_ = str; }
  void SetCwin( G4String &str ) { cwin_ = str; }
  void SetFrame( G4String &str ) { frame_ = str; }
  const G4String &GetProjectile() const { return projectile_;}
  const G4String &GetTarget() const { return target_;}
  const G4String &GetCwin() const { return cwin_;}
  const G4String &GetFrame() const { return frame_;}
  const int GetJAMEventNumber(int n){
    if(genID>=100 && genID<=132){//not JAM
      return n;
    }
    else if (genID ==7 || genID ==8){//not JAM
      return n; 
      //sako 2019/10/5
    } else if (genID == 1014) {
      return n;
    }
    return iev_[n];
  }
  const int GetJAMMultiplicity(int n){
    if(genID>=100 && genID<=132){//not JAM
      return 1;
    }
    else if (genID ==7 || genID ==8){//not JAM
      return 1; 
    } else if (genID ==1014){//sako 2019/10/5
      return nmul_ev; 
    }
    return nmul_[n];
  }
  //sako 2019/11/18
  const double GetJAMImpact(int n){
    if(genID>=100 && genID<=132){//not JAM
      return -1;
    }
    else if (genID ==7 || genID ==8){//not JAM
      return -1; 
    } else if (genID ==1014){//sako 2019/10/5
      return impJAM_ev; 
    } else if (genID ==14){//sako 2021/6/19
      return impJAM_[n];
    }
    //def sako 2020/8/31    return impJAM_[n];
    //new sako 2020/9/1
    return -1;
  }


private:
  static E16G4_PrimaryGeneratorAction *thisPointer;

  void ParticleTableTest();//by Yokkaichi, for geantino

public:
  static E16G4_PrimaryGeneratorAction *GetpPGA(){ return thisPointer; }

};

#endif//E16G4_PRIMARY_GENERATOR_ACTION_H

