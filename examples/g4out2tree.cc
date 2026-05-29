//2016-11-22, uploaded by nakai



#include <G4RunManager.hh>
#include <G4UIterminal.hh>
//#include <G4UIExecutive.hh>
#include <G4PhysListFactory.hh>
#include <G4StepLimiterBuilder.hh>
#include <G4DecayTable.hh>
#include <G4PhaseSpaceDecayChannel.hh>
#ifdef G4UI_USE_TCSH
#include <G4UItcsh.hh>
#endif
#include "G4ParticleTable.hh"//sako
#include <QGSP.hh>
#include <QGSP_BERT.hh>
#include <QGSP_BERT_HP.hh>
//#include <PhysicsListMessenger.hh>
//#include <PhysicsList.hh>


#ifdef G4VIS_USE
#include <G4VisExecutive.hh>
#endif




#include "E16G4_DetectorConstruction.hh"
#include "E16G4_PrimaryGeneratorAction.hh"
#include "E16G4_TrackingAction.hh"
#include "E16G4_RunAction.hh"
#include "E16G4_EventAction.hh"
#include "E16G4_SteppingAction.hh"
#include "E16G4_Analysis.hh"
#include "E16G4_GlobalVar.hh"
#include "E88_PhysicsList.hh"
#include "E16ANA_ParamManager.hh"

#include "E16ANA_G4OutputData.hh"

#include "E16ANA_GeometryV2.hh"

#include <TFile.h>
#include <TTree.h>

/*
//#include <globals.hh>

#include <G4RunManager.hh>
#include <G4UIterminal.hh>
//#include <G4UIExecutive.hh>
#include <G4PhysListFactory.hh>
#include <G4StepLimiterBuilder.hh>
#include <G4DecayTable.hh>
#include <G4PhaseSpaceDecayChannel.hh>
#ifdef G4UI_USE_TCSH
#include <G4UItcsh.hh>
#endif
#include "G4ParticleTable.hh"//sako
#include <QGSP.hh>
#include <QGSP_BERT.hh>
#include <QGSP_BERT_HP.hh>
//#include <PhysicsListMessenger.hh>
//#include <PhysicsList.hh>


#ifdef G4VIS_USE
#include <G4VisExecutive.hh>
#endif


//#include <G4ParticleTable.hh>
//#include <G4ParticleDefinition.hh>
#include <G4SystemOfUnits.hh>
//#include <G4ExceptionSeverity.hh>

#include <G4UIcommand.hh>
#include <G4UIcmdWithABool.hh>
//#include <cstdlib>
//#include <iostream>
#include <vector>

#include <TFile.h>
#include <TTree.h>
#include <TVector3.h>

//#include "G4ParticleTable.hh"//sako

#include "E16G4_DetectorConstruction.hh"
#include "E16G4_PrimaryGeneratorAction.hh"
#include "E16G4_TrackingAction.hh"
#include "E16G4_RunAction.hh"
#include "E16G4_EventAction.hh"
#include "E16G4_SteppingAction.hh"
#include "E16G4_Analysis.hh"
#include "E16G4_GlobalVar.hh"
#include "E88_PhysicsList.hh"
#include "E16ANA_ParamManager.hh"

#include "E16ANA_G4OutputData.hh"

#include "E16ANA_GeometryV2.hh"

//#include "E16G4_DetectorConstruction.hh"
*/

int main(int argc, char **argv){

  //  G4ParticleTable *particleTable= G4ParticleTable::GetParticleTable();

   if(argc != 3){
      std::cout << "Usage : " << argv[0] << " input output" << std::endl;
      return 1;
   }

   //sako new
  G4RunManager* runMgr= new G4RunManager;  
  E88_PhysicsList *phys = new E88_PhysicsList();
  phys->RegisterPhysics(new G4StepLimiterBuilder);
  runMgr-> SetUserInitialization(phys);

  //  E16G4_Analysis  *anaMgr    = new E16G4_Analysis();
  //  E16G4_PrimaryGeneratorAction *primGen =
    //    new E16G4_PrimaryGeneratorAction( anaMgr );
  //  E16G4_RunAction *runAction = new E16G4_RunAction( anaMgr, primGen );
  //  E16G4_EventAction *evAction = new E16G4_EventAction( anaMgr );
  //  E16G4_TrackingAction *trAction = new E16G4_TrackingAction( evAction, anaMgr);
  //  E16G4_SteppingAction *stepAction = new E16G4_SteppingAction( );
  
  //  E16G4_DetectorConstruction *detCon= new E16G4_DetectorConstruction();

  // set mandatory user initialization classes...
  // detector setup
  //  runMgr-> SetUserInitialization(detCon);
  // particles and physics processes
  // primary generator
  //  runMgr-> SetUserAction(primGen);

  //  runMgr-> SetUserAction( trAction);
  //  runMgr-> SetUserAction( runAction);
  //  runMgr-> SetUserAction( evAction);
  //  runMgr-> SetUserAction( stepAction);


   char* E16calibfilename = "./E16CalibFiles-local.cfg";
   E16ANA_ParamManager *paramMgr= new E16ANA_ParamManager( E16calibfilename );
   char* fname_geom = paramMgr->GetGeomFileName();
   std::cout << "  Geometry data file: " << fname_geom << std::endl;
   E16ANA_GeometryV2 *geom = new E16ANA_GeometryV2(fname_geom);


   char *g4filename = argv[1];
   char *outfilename = argv[2];

   E16ANA_G4OutputData g4data;
   //   #ifdef TEST_G4OUT2TREE
   //   std::cout << "opening " << g4filename << std::endl;

   int flag = g4data.OpenReadFile(g4filename);//core dump
   if( flag != g4data.OK ){     exit(1);   }


   TFile *out_file = TFile::Open(outfilename, "recreate");
   TTree *tree = new TTree("tree", "tree");

   int g4_event_id;
   //sako 2019/11/18
   int jam_event_id;
   int jam_mult;
   double jam_impact;

   std::vector<int> ssd_mid;
   std::vector<int> ssd_tid;
   std::vector<int> ssd_pid;
   std::vector<double> ssd_lx;
   std::vector<double> ssd_ly;
   std::vector<double> ssd_lz;
   std::vector<double> ssd_gx;
   std::vector<double> ssd_gy;
   std::vector<double> ssd_gz;
   std::vector<double> ssd_gt;//sako
   std::vector<double> ssd_gx_r;
   std::vector<double> ssd_gy_r;
   std::vector<double> ssd_gz_r;
   std::vector<double> ssd_px;
   std::vector<double> ssd_py;
   std::vector<double> ssd_pz;
   std::vector<double> ssd_beta;//sako
   std::vector<double> ssd_mass;//sako

   std::vector<int> gtr1_mid;
   std::vector<int> gtr1_tid;
   std::vector<int> gtr1_pid;
   std::vector<double> gtr1_lx;
   std::vector<double> gtr1_ly;
   std::vector<double> gtr1_lz;
   std::vector<double> gtr1_gx;
   std::vector<double> gtr1_gy;
   std::vector<double> gtr1_gz;
   std::vector<double> gtr1_gt;//sako
   std::vector<double> gtr1_gx_r;
   std::vector<double> gtr1_gy_r;
   std::vector<double> gtr1_gz_r;
   std::vector<double> gtr1_px;//sako
   std::vector<double> gtr1_py;//sako
   std::vector<double> gtr1_pz;//sako
   std::vector<double> gtr1_beta;//sako
   std::vector<double> gtr1_mass;//sako

   std::vector<int> gtr2_mid;
   std::vector<int> gtr2_tid;
   std::vector<int> gtr2_pid;
   std::vector<double> gtr2_lx;
   std::vector<double> gtr2_ly;
   std::vector<double> gtr2_lz;
   std::vector<double> gtr2_gx;
   std::vector<double> gtr2_gy;
   std::vector<double> gtr2_gz;
   std::vector<double> gtr2_gt;//sako
   std::vector<double> gtr2_gx_r;
   std::vector<double> gtr2_gy_r;
   std::vector<double> gtr2_gz_r;
   std::vector<double> gtr2_px;//sako
   std::vector<double> gtr2_py;//sako
   std::vector<double> gtr2_pz;//sako
   std::vector<double> gtr2_beta;//sako
   std::vector<double> gtr2_mass;//sako

   std::vector<int> gtr3_mid;
   std::vector<int> gtr3_tid;
   std::vector<int> gtr3_pid;
   std::vector<double> gtr3_lx;
   std::vector<double> gtr3_ly;
   std::vector<double> gtr3_lz;
   std::vector<double> gtr3_gx;
   std::vector<double> gtr3_gy;
   std::vector<double> gtr3_gz;
   std::vector<double> gtr3_gt;//sako
   std::vector<double> gtr3_gx_r;
   std::vector<double> gtr3_gy_r;
   std::vector<double> gtr3_gz_r;
   std::vector<double> gtr3_px;//sako
   std::vector<double> gtr3_py;//sako
   std::vector<double> gtr3_pz;//sako
   std::vector<double> gtr3_beta;//sako
   std::vector<double> gtr3_mass;//sako

   std::vector<int> hbd_mid;
   std::vector<int> hbd_tid;
   std::vector<int> hbd_pid;
   std::vector<double> hbd_lx;
   std::vector<double> hbd_ly;
   std::vector<double> hbd_lz;
   std::vector<double> hbd_gx;
   std::vector<double> hbd_gy;
   std::vector<double> hbd_gz;
   std::vector<double> hbd_gt;
   std::vector<double> hbd_gx_r;
   std::vector<double> hbd_gy_r;
   std::vector<double> hbd_gz_r;
   std::vector<double> hbd_px;
   std::vector<double> hbd_py;
   std::vector<double> hbd_pz;

   //sako 2019/9/24
   std::vector<int> rpc_mid;
   std::vector<int> rpc_tid;
   std::vector<int> rpc_pid;
   std::vector<double> rpc_lx;
   std::vector<double> rpc_ly;
   std::vector<double> rpc_lz;
   std::vector<double> rpc_gx;
   std::vector<double> rpc_gy;
   std::vector<double> rpc_gz;
   std::vector<double> rpc_gt;//sako
   //   std::vector<double> rpc_gx_r;
   //   std::vector<double> rpc_gy_r;
   //   std::vector<double> rpc_gz_r;
   std::vector<double> rpc_px;//sako
   std::vector<double> rpc_py;//sako
   std::vector<double> rpc_pz;//sako
   std::vector<double> rpc_beta;
   std::vector<double> rpc_mass;

   std::vector<int> lgvd_mid;
   std::vector<int> lgvd_tid;
   std::vector<int> lgvd_pid;
   std::vector<double> lgvd_lx;
   std::vector<double> lgvd_ly;
   std::vector<double> lgvd_lz;
   std::vector<double> lgvd_gx;
   std::vector<double> lgvd_gy;
   std::vector<double> lgvd_gz;
   std::vector<double> lgvd_gx_r;
   std::vector<double> lgvd_gy_r;
   std::vector<double> lgvd_gz_r;

   std::vector<int> track_id;
   //sako 2020/2/17
   std::vector<int> track_mid;//mother id
   std::vector<int> particle_id;
   std::vector<double> track_vx;
   std::vector<double> track_vy;
   std::vector<double> track_vz;
   std::vector<double> track_px;
   std::vector<double> track_py;
   std::vector<double> track_pz;
   std::vector<double> track_mass;

   ///   std::cout << "declared vectors" << std::endl;
   tree->Branch("g4_event_id", &g4_event_id);
   //sako 2019/11/18
   tree->Branch("jam_event_id", &jam_event_id);
   tree->Branch("jam_mult", &jam_mult);
   tree->Branch("jam_impact", &jam_impact);

   tree->Branch("ssd_mid", &ssd_mid);
   tree->Branch("ssd_tid", &ssd_tid);
   tree->Branch("ssd_pid", &ssd_pid);
   tree->Branch("ssd_lx", &ssd_lx);
   tree->Branch("ssd_ly", &ssd_ly);
   tree->Branch("ssd_lz", &ssd_lz);
   tree->Branch("ssd_gx", &ssd_gx);
   tree->Branch("ssd_gy", &ssd_gy);
   tree->Branch("ssd_gz", &ssd_gz);
   tree->Branch("ssd_gt", &ssd_gt);//sako
   tree->Branch("ssd_gx_r", &ssd_gx_r);
   tree->Branch("ssd_gy_r", &ssd_gy_r);
   tree->Branch("ssd_gz_r", &ssd_gz_r);
   tree->Branch("ssd_px", &ssd_px);
   tree->Branch("ssd_py", &ssd_py);
   tree->Branch("ssd_pz", &ssd_pz);
   tree->Branch("ssd_beta", &ssd_beta);
   tree->Branch("ssd_mass", &ssd_mass);

   tree->Branch("gtr1_mid", &gtr1_mid);
   tree->Branch("gtr1_tid", &gtr1_tid);
   tree->Branch("gtr1_pid", &gtr1_pid);
   tree->Branch("gtr1_lx", &gtr1_lx);
   tree->Branch("gtr1_ly", &gtr1_ly);
   tree->Branch("gtr1_lz", &gtr1_lz);
   tree->Branch("gtr1_gx", &gtr1_gx);
   tree->Branch("gtr1_gy", &gtr1_gy);
   tree->Branch("gtr1_gz", &gtr1_gz);
   tree->Branch("gtr1_gt", &gtr1_gt);//sako
   tree->Branch("gtr1_gx_r", &gtr1_gx_r);
   tree->Branch("gtr1_gy_r", &gtr1_gy_r);
   tree->Branch("gtr1_gz_r", &gtr1_gz_r);
   tree->Branch("gtr1_px", &gtr1_px);//sako
   tree->Branch("gtr1_py", &gtr1_py);//sako
   tree->Branch("gtr1_pz", &gtr1_pz);//sako
   tree->Branch("gtr1_beta", &gtr1_beta);//sako
   tree->Branch("gtr1_mass", &gtr1_mass);//sako

   tree->Branch("gtr2_mid", &gtr2_mid);
   tree->Branch("gtr2_tid", &gtr2_tid);
   tree->Branch("gtr2_pid", &gtr2_pid);
   tree->Branch("gtr2_lx", &gtr2_lx);
   tree->Branch("gtr2_ly", &gtr2_ly);
   tree->Branch("gtr2_lz", &gtr2_lz);
   tree->Branch("gtr2_gx", &gtr2_gx);
   tree->Branch("gtr2_gy", &gtr2_gy);
   tree->Branch("gtr2_gz", &gtr2_gz);
   tree->Branch("gtr2_gt", &gtr2_gt);//sako
   tree->Branch("gtr2_gx_r", &gtr2_gx_r);
   tree->Branch("gtr2_gy_r", &gtr2_gy_r);
   tree->Branch("gtr2_gz_r", &gtr2_gz_r);
   tree->Branch("gtr2_px", &gtr2_px);//sako
   tree->Branch("gtr2_py", &gtr2_py);//sako
   tree->Branch("gtr2_pz", &gtr2_pz);//sako
   tree->Branch("gtr2_beta", &gtr2_beta);//sako
   tree->Branch("gtr2_mass", &gtr2_mass);//sako

   tree->Branch("gtr3_mid", &gtr3_mid);
   tree->Branch("gtr3_tid", &gtr3_tid);
   tree->Branch("gtr3_pid", &gtr3_pid);
   tree->Branch("gtr3_lx", &gtr3_lx);
   tree->Branch("gtr3_ly", &gtr3_ly);
   tree->Branch("gtr3_lz", &gtr3_lz);
   tree->Branch("gtr3_gx", &gtr3_gx);
   tree->Branch("gtr3_gy", &gtr3_gy);
   tree->Branch("gtr3_gz", &gtr3_gz);
   tree->Branch("gtr3_gt", &gtr3_gt);//sako
   tree->Branch("gtr3_gx_r", &gtr3_gx_r);
   tree->Branch("gtr3_gy_r", &gtr3_gy_r);
   tree->Branch("gtr3_gz_r", &gtr3_gz_r);
   tree->Branch("gtr3_px", &gtr3_px);//sako
   tree->Branch("gtr3_py", &gtr3_py);//sako
   tree->Branch("gtr3_pz", &gtr3_pz);//sako
   tree->Branch("gtr3_beta", &gtr3_beta);//sako
   tree->Branch("gtr3_mass", &gtr3_mass);//sako

   tree->Branch("hbd_mid", &hbd_mid);
   tree->Branch("hbd_tid", &hbd_tid);
   tree->Branch("hbd_pid", &hbd_pid);
   tree->Branch("hbd_lx", &hbd_lx);
   tree->Branch("hbd_ly", &hbd_ly);
   tree->Branch("hbd_lz", &hbd_lz);
   tree->Branch("hbd_gx", &hbd_gx);
   tree->Branch("hbd_gy", &hbd_gy);
   tree->Branch("hbd_gz", &hbd_gz);
   tree->Branch("hbd_gt", &hbd_gt);//sako
   tree->Branch("hbd_gx_r", &hbd_gx_r);
   tree->Branch("hbd_gy_r", &hbd_gy_r);
   tree->Branch("hbd_gz_r", &hbd_gz_r);
   tree->Branch("hbd_px", &hbd_px);
   tree->Branch("hbd_py", &hbd_py);
   tree->Branch("hbd_pz", &hbd_pz);

   //sako
   tree->Branch("rpc_mid", &rpc_mid);
   tree->Branch("rpc_tid", &rpc_tid);
   tree->Branch("rpc_pid", &rpc_pid);
   tree->Branch("rpc_lx", &rpc_lx);
   tree->Branch("rpc_ly", &rpc_ly);
   tree->Branch("rpc_lz", &rpc_lz);
   tree->Branch("rpc_gx", &rpc_gx);
   tree->Branch("rpc_gy", &rpc_gy);
   tree->Branch("rpc_gz", &rpc_gz);
   tree->Branch("rpc_gt", &rpc_gt);//sako
   //   tree->Branch("rpc_gx_r", &rpc_gx_r);
   //   tree->Branch("rpc_gy_r", &rpc_gy_r);
   //   tree->Branch("rpc_gz_r", &rpc_gz_r);
   tree->Branch("rpc_px", &rpc_px);
   tree->Branch("rpc_py", &rpc_py);
   tree->Branch("rpc_pz", &rpc_pz);
   tree->Branch("rpc_beta", &rpc_beta);
   tree->Branch("rpc_mass", &rpc_mass);

   tree->Branch("lgvd_mid", &lgvd_mid);
   tree->Branch("lgvd_tid", &lgvd_tid);
   tree->Branch("lgvd_pid", &lgvd_pid);
   tree->Branch("lgvd_lx", &lgvd_lx);
   tree->Branch("lgvd_ly", &lgvd_ly);
   tree->Branch("lgvd_lz", &lgvd_lz);
   tree->Branch("lgvd_gx", &lgvd_gx);
   tree->Branch("lgvd_gy", &lgvd_gy);
   tree->Branch("lgvd_gz", &lgvd_gz);
   tree->Branch("lgvd_gx_r", &lgvd_gx_r);
   tree->Branch("lgvd_gy_r", &lgvd_gy_r);
   tree->Branch("lgvd_gz_r", &lgvd_gz_r);

   tree->Branch("track_id", &track_id);
   tree->Branch("track_mid", &track_mid);
   tree->Branch("particle_id", &particle_id);
   tree->Branch("track_vx", &track_vx);
   tree->Branch("track_vy", &track_vy);
   tree->Branch("track_vz", &track_vz);
   tree->Branch("track_px", &track_px);
   tree->Branch("track_py", &track_py);
   tree->Branch("track_pz", &track_pz);
   tree->Branch("track_mass", &track_mass);

   //   std::cout << "set tree->Branch" << std::endl;

   int n = 0;

   while(1){
     //     std::cout << "** Start Event " << n << std::endl;
      ssd_mid.clear();
      ssd_tid.clear();
      ssd_pid.clear();
      ssd_lx.clear();
      ssd_ly.clear();
      ssd_lz.clear();
      ssd_gx.clear();
      ssd_gy.clear();
      ssd_gz.clear();
      ssd_gt.clear();//sako
      ssd_gx_r.clear();
      ssd_gy_r.clear();
      ssd_gz_r.clear();
      ssd_px.clear();
      ssd_py.clear();
      ssd_pz.clear();
      ssd_beta.clear();//sako
      ssd_mass.clear();//sako

      gtr1_mid.clear();
      gtr1_tid.clear();
      gtr1_pid.clear();
      gtr1_lx.clear();
      gtr1_ly.clear();
      gtr1_lz.clear();
      gtr1_gx.clear();
      gtr1_gy.clear();
      gtr1_gz.clear();
      gtr1_gt.clear();//sako
      gtr1_gx_r.clear();
      gtr1_gy_r.clear();
      gtr1_gz_r.clear();
      gtr1_px.clear();//sako
      gtr1_py.clear();//sako
      gtr1_pz.clear();//sako
      gtr1_beta.clear();//sako
      gtr1_mass.clear();//sako

      gtr2_mid.clear();
      gtr2_tid.clear();
      gtr2_pid.clear();
      gtr2_lx.clear();
      gtr2_ly.clear();
      gtr2_lz.clear();
      gtr2_gx.clear();
      gtr2_gy.clear();
      gtr2_gz.clear();
      gtr2_gt.clear();//sako
      gtr2_gx_r.clear();
      gtr2_gy_r.clear();
      gtr2_gz_r.clear();
      gtr2_px.clear();//sako
      gtr2_py.clear();//sako
      gtr2_pz.clear();//sako
      gtr2_beta.clear();//sako
      gtr2_mass.clear();//sako

      gtr3_mid.clear();
      gtr3_tid.clear();
      gtr3_pid.clear();
      gtr3_lx.clear();
      gtr3_ly.clear();
      gtr3_lz.clear();
      gtr3_gx.clear();
      gtr3_gy.clear();
      gtr3_gz.clear();
      gtr3_gt.clear();//sako
      gtr3_gx_r.clear();
      gtr3_gy_r.clear();
      gtr3_gz_r.clear();
      gtr3_px.clear();//sako
      gtr3_py.clear();//sako
      gtr3_pz.clear();//sako
      gtr3_beta.clear();//sako
      gtr3_mass.clear();//sako

      hbd_mid.clear();
      hbd_tid.clear();
      hbd_pid.clear();
      hbd_lx.clear();
      hbd_ly.clear();
      hbd_lz.clear();
      hbd_gx.clear();
      hbd_gy.clear();
      hbd_gz.clear();
      hbd_gt.clear();//sako
      hbd_gx_r.clear();
      hbd_gy_r.clear();
      hbd_gz_r.clear();
      hbd_px.clear();
      hbd_py.clear();
      hbd_pz.clear();

      rpc_mid.clear();
      rpc_tid.clear();
      rpc_pid.clear();
      rpc_lx.clear();
      rpc_ly.clear();
      rpc_lz.clear();
      rpc_gx.clear();
      rpc_gy.clear();
      rpc_gz.clear();
      rpc_gt.clear();//sako
      //      rpc_gx_r.clear();
      //      rpc_gy_r.clear();
      //      rpc_gz_r.clear();
      rpc_px.clear();
      rpc_py.clear();
      rpc_pz.clear();
      rpc_beta.clear();
      rpc_mass.clear();

      lgvd_mid.clear();
      lgvd_tid.clear();
      lgvd_pid.clear();
      lgvd_lx.clear();
      lgvd_ly.clear();
      lgvd_lz.clear();
      lgvd_gx.clear();
      lgvd_gy.clear();
      lgvd_gz.clear();
      lgvd_gx_r.clear();
      lgvd_gy_r.clear();
      lgvd_gz_r.clear();

      track_id.clear();
      track_mid.clear();
      particle_id.clear();
      track_vx.clear();
      track_vy.clear();
      track_vz.clear();
      track_px.clear();
      track_py.clear();
      track_pz.clear();
      track_mass.clear();

      //      std::cout << "going to Read an event, n = " << n << std::endl;
      n++;
      
      int read_event = g4data.ReadAnEvent();
      if(read_event != 1){
         std::cout << "End of file" << std::endl;
         break;
      }
      //      std::cout<<"end read on event " << std::endl;
      E16ANA_G4Event &g4event = g4data.Event();
      G4ParticleTable *particleTable= G4ParticleTable::GetParticleTable();
      g4_event_id = g4event.G4EventID();
      //sako 2019/11/18
      jam_event_id = g4event.JAMEventID();
      jam_mult = g4event.InputMultiplicity();
      jam_impact = g4event.JAMImpact();
      //      std::cout << "g4_event_id = " << g4_event_id << std::endl;

      //sako 2021/5/14
      int noht0 = g4event.TARGET(0).Noh();
      int noht1 = g4event.TARGET(1).Noh();
      int noht2 = g4event.TARGET(2).Noh();
      int noht3 = g4event.TARGET(3).Noh();
      int noht4 = g4event.TARGET(4).Noh();

      //      std::cout << "target noh = " << noht0 << " " << noht1 << " " << noht2 << " " << noht3 << " " << noht4 << std::endl;
      /*
      for(int i=0; i<noht0; i++){
	E16ANA_G4Hit &hit = g4event.TARGET(0).Hit(i);
	std::cout << "target0 = " << hit.X() <<" " << hit.Y() << " " << hit.Z() << std::endl;
      }
      for(int i=0; i<noht2; i++){
	E16ANA_G4Hit &hit = g4event.TARGET(2).Hit(i);
	std::cout << "target2 = " << hit.X() <<" " << hit.Y() << " " << hit.Z() << std::endl;
	}*/

      int noh = g4event.SSD().Noh();
      //      std::cout << "ssd_noh = " << noh << std::endl;
      for(int i=0; i<noh; i++){
         E16ANA_G4Hit &hit = g4event.SSD().Hit(i);
         ssd_mid.push_back(hit.DetectorID());
         ssd_tid.push_back(hit.TrackID());
	 //	 std::cout << "ssd_tid = " << hit.TrackID() << std::endl;
         ssd_pid.push_back(hit.PID());
         ssd_lx.push_back(hit.X());
         ssd_ly.push_back(hit.Y());
         ssd_lz.push_back(hit.Z());
         ssd_gx.push_back(hit.GX());
         ssd_gy.push_back(hit.GY());
         ssd_gz.push_back(hit.GZ());
         ssd_gt.push_back(hit.Time()/ns);//sako
	 //	 std::cout << "hit.Time()/ns = " << hit.Time()/ns << std::endl;//sako
         TVector3 ssd_re = geom->SSD(hit.DetectorID())->GetGPos(hit.XTV());
         ssd_gx_r.push_back(ssd_re.X());
         ssd_gy_r.push_back(ssd_re.Y());
         ssd_gz_r.push_back(ssd_re.Z());
         ssd_px.push_back(hit.GPTV().X());
         ssd_py.push_back(hit.GPTV().Y());
         ssd_pz.push_back(hit.GPTV().Z());
         ssd_beta.push_back(hit.Beta());//sako

	 G4ParticleDefinition *particle = particleTable->FindParticle(hit.PID());
	 if (particle != 0) {
	   ssd_mass.push_back(particle->GetPDGMass()/GeV);
	   //	   G4cout << "*** ["<<i << "] ssd: pid, particle = " << hit.PID() << " " << particle << G4endl;
	 } else {
	   G4cerr << "particle (ssd) == 0, pid = " << hit.PID() << G4endl;
	   ssd_mass.push_back(-1);
	 }
      }
      int noh1 = g4event.GTR1().Noh();
      //      std::cout << "gtr1_noh = " << noh1 << std::endl;
      for(int i=0; i<noh1; i++){
         E16ANA_G4Hit &hit = g4event.GTR1().Hit(i);
         gtr1_mid.push_back(hit.DetectorID());
         gtr1_tid.push_back(hit.TrackID());
         gtr1_pid.push_back(hit.PID());
         gtr1_lx.push_back(hit.X());
         gtr1_ly.push_back(hit.Y());
         gtr1_lz.push_back(hit.Z());
         gtr1_gx.push_back(hit.GX());
         gtr1_gy.push_back(hit.GY());
         gtr1_gz.push_back(hit.GZ());
         gtr1_gt.push_back(hit.Time()/ns);//sako
         TVector3 gtr_re = geom->GTR(hit.DetectorID(),0)->GetGPos(hit.XTV());
         gtr1_gx_r.push_back(gtr_re.X());
         gtr1_gy_r.push_back(gtr_re.Y());
         gtr1_gz_r.push_back(gtr_re.Z());
         gtr1_px.push_back(hit.GPTV().X());//sako
         gtr1_py.push_back(hit.GPTV().Y());//sako
         gtr1_pz.push_back(hit.GPTV().Z());//sako
         gtr1_beta.push_back(hit.Beta());//sako
	 G4ParticleDefinition *particle = particleTable->FindParticle(hit.PID());
	 if (particle!=0) {
	   gtr1_mass.push_back(particle->GetPDGMass()/GeV);
	 } else {
	   G4cerr << "particle (gtr1) == 0, pid = " << hit.PID() << G4endl;
	   gtr1_mass.push_back(-1);
	 }

      }
      int noh2 = g4event.GTR2().Noh();
      //      std::cout << "gtr2_noh = " << noh2 << std::endl;
      for(int i=0; i<noh2; i++){
         E16ANA_G4Hit &hit = g4event.GTR2().Hit(i);
         gtr2_mid.push_back(hit.DetectorID());
         gtr2_tid.push_back(hit.TrackID());
         gtr2_pid.push_back(hit.PID());
         gtr2_lx.push_back(hit.X());
         gtr2_ly.push_back(hit.Y());
         gtr2_lz.push_back(hit.Z());
         gtr2_gx.push_back(hit.GX());
         gtr2_gy.push_back(hit.GY());
         gtr2_gz.push_back(hit.GZ());
         gtr2_gt.push_back(hit.Time()/ns);//sako
         TVector3 gtr_re = geom->GTR(hit.DetectorID(),1)->GetGPos(hit.XTV());
         gtr2_gx_r.push_back(gtr_re.X());
         gtr2_gy_r.push_back(gtr_re.Y());
         gtr2_gz_r.push_back(gtr_re.Z());
         gtr2_px.push_back(hit.GPTV().X());//sako
         gtr2_py.push_back(hit.GPTV().Y());//sako
         gtr2_pz.push_back(hit.GPTV().Z());//sako
         gtr2_beta.push_back(hit.Beta());//sako
	 G4ParticleDefinition *particle = particleTable->FindParticle(hit.PID());
	 if (particle !=0) {
	   gtr2_mass.push_back(particle->GetPDGMass()/GeV);
	 } else {
	   G4cerr << "particle (gtr2) == 0, pid = " << hit.PID() << G4endl;
	   gtr2_mass.push_back(-1);
	 }


      }
      int noh3 = g4event.GTR3().Noh();
      //      std::cout << "gtr3_noh = " << noh3 << std::endl;
      for(int i=0; i<noh3; i++){
         E16ANA_G4Hit &hit = g4event.GTR3().Hit(i);
         gtr3_mid.push_back(hit.DetectorID());
         gtr3_tid.push_back(hit.TrackID());
	 //	 std::cout << "gtr3_tid = " << hit.TrackID() << std::endl;
         gtr3_pid.push_back(hit.PID());
         gtr3_lx.push_back(hit.X());
         gtr3_ly.push_back(hit.Y());
         gtr3_lz.push_back(hit.Z());
         gtr3_gx.push_back(hit.GX());
         gtr3_gy.push_back(hit.GY());
         gtr3_gz.push_back(hit.GZ());
         gtr3_gt.push_back(hit.Time()/ns);//sako
         TVector3 gtr_re = geom->GTR(hit.DetectorID(),2)->GetGPos(hit.XTV());
         gtr3_gx_r.push_back(gtr_re.X());
         gtr3_gy_r.push_back(gtr_re.Y());
         gtr3_gz_r.push_back(gtr_re.Z());
         gtr3_px.push_back(hit.GPTV().X());//sako
         gtr3_py.push_back(hit.GPTV().Y());//sako
         gtr3_pz.push_back(hit.GPTV().Z());//sako
         gtr3_beta.push_back(hit.Beta());//sako
	 G4ParticleDefinition *particle = particleTable->FindParticle(hit.PID());

	 if (particle != 0) {
	   gtr3_mass.push_back(particle->GetPDGMass()/GeV);
	 } else {
	   G4cerr << "particle (gtr3) == 0, pid = " << hit.PID() << G4endl;
	   gtr3_mass.push_back(-1);
	 }

      }
      int noh4 = g4event.HBD().Noh();
      //      std::cout << "hbd_noh = " << noh4 << std::endl;
      for(int i=0; i<noh4; i++){
         E16ANA_G4Hit &hit = g4event.HBD().Hit(i);
         //if(hit.TrackID() == 1){
         hbd_mid.push_back(hit.DetectorID());
         hbd_tid.push_back(hit.TrackID());
         hbd_pid.push_back(hit.PID());
         hbd_lx.push_back(hit.X());
         hbd_ly.push_back(hit.Y());
         hbd_lz.push_back(hit.Z());
         hbd_gx.push_back(hit.GX());
         hbd_gy.push_back(hit.GY());
         hbd_gz.push_back(hit.GZ());
         hbd_gt.push_back(hit.Time()/ns);//sako
         TVector3 hbd_re = geom->HBD(hit.DetectorID())->GetGPos(hit.XTV());
         hbd_gx_r.push_back(hbd_re.X());
         hbd_gy_r.push_back(hbd_re.Y());
         hbd_gz_r.push_back(hbd_re.Z());
         hbd_px.push_back(hit.GPTV().X());//sako
         hbd_py.push_back(hit.GPTV().Y());//sako
         hbd_pz.push_back(hit.GPTV().Z());//sako

         //}
      }
      int noh5 = g4event.LGVD().Noh();
      //      std::cout << "lgvd_noh = " << noh5 << std::endl;
      for(int i=0; i<noh5; i++){
         E16ANA_G4Hit &hit = g4event.LGVD().Hit(i);
         //if(hit.TrackID() == 1){
         lgvd_mid.push_back(hit.DetectorID());
         lgvd_tid.push_back(hit.TrackID());
         lgvd_pid.push_back(hit.PID());
         lgvd_lx.push_back(hit.X());
         lgvd_ly.push_back(hit.Y());
         lgvd_lz.push_back(hit.Z());
         lgvd_gx.push_back(hit.GX());
         lgvd_gy.push_back(hit.GY());
         lgvd_gz.push_back(hit.GZ());
         TVector3 lgvd_re = geom->LGVD(hit.DetectorID())->GetGPos(hit.XTV());
         lgvd_gx_r.push_back(lgvd_re.X());
         lgvd_gy_r.push_back(lgvd_re.Y());
         lgvd_gz_r.push_back(lgvd_re.Z());
         //}
      }
      int noh6 = g4event.RPC().Noh();
      //      std::cout << "rpc_noh = " << noh6 << std::endl;
      for(int i=0; i<noh6; i++){
         E16ANA_G4Hit &hit = g4event.RPC().Hit(i);
         //if(hit.TrackID() == 1){
         rpc_mid.push_back(hit.DetectorID());
         rpc_tid.push_back(hit.TrackID());
         rpc_pid.push_back(hit.PID());
         rpc_lx.push_back(hit.X());
         rpc_ly.push_back(hit.Y());
         rpc_lz.push_back(hit.Z());
         rpc_gx.push_back(hit.GX());
         rpc_gy.push_back(hit.GY());
         rpc_gz.push_back(hit.GZ());
         rpc_gt.push_back(hit.Time()/ns);//sako
	 //         TVector3 rpc_re = geom->RPC(hit.DetectorID())->GetGPos(hit.XTV());
	 //         rpc_gx_r.push_back(rpc_re.X());
	 //         rpc_gy_r.push_back(rpc_re.Y());
	 //         rpc_gz_r.push_back(rpc_re.Z());
         rpc_px.push_back(hit.GPTV().X());//sako
         rpc_py.push_back(hit.GPTV().Y());//sako
         rpc_pz.push_back(hit.GPTV().Z());//sako
         rpc_beta.push_back(hit.Beta());//sako
	 G4ParticleDefinition *particle = particleTable->FindParticle(hit.PID());

	 if (particle != 0) {
	   rpc_mass.push_back(particle->GetPDGMass()/GeV);
	 } else {
	   G4cerr << "particle (rpc) == 0, pid = " << hit.PID() << G4endl;
	   rpc_mass.push_back(-1);
	 }
         //}
      }

      //std::cout << "OK" << std::endl;
      //def      int noht = g4event.PrimaryTracks().Noh();
      int noht = g4event.AllTrajectories().size();
      //      std::cout << "track_noh = " << noht << std::endl;
      for(int i=0; i<noht; i++){
	//def         E16ANA_G4Track &track = g4event.PrimaryTracks(i);
	//         E16ANA_G4Trajectory &track = g4event.AllTrajectories(i);
	E16ANA_G4Trajectory &track = *g4event.AllTrajectory(i);
         track_id.push_back(track.TrackID());
	 //	 std::cout << "trackid = " << track.TrackID() << std::endl;
         track_mid.push_back(track.ParentID());
         particle_id.push_back(track.PID());
         track_vx.push_back(track.TVertex().X());
         track_vy.push_back(track.TVertex().Y());
         track_vz.push_back(track.TVertex().Z());
         track_px.push_back(track.PTV().X());
         track_py.push_back(track.PTV().Y());
         track_pz.push_back(track.PTV().Z());

	 G4ParticleDefinition *particle = particleTable->FindParticle(track.PID());
	 if (particle !=0) {
	   track_mass.push_back(particle->GetPDGMass()/GeV);
	 } else {
	   G4cerr << "particle (track) == 0, pid = " << track.PID() << G4endl;
	   track_mass.push_back(-1);
	 }
	 //	 std::cout << "PID, mass = " << track.PID() << " " << particle->GetPDGMass()/GeV << std::endl;
	 
	 //	 std::cout << "velocity = " << track.GetVelocity() << std::endl;
	 //	 std::cout << "E = " << track.PTV().E() << std::endl;
	 //	 std::cout << "velocity = " << track.beta() << std::endl;
      }
      //std::cout << "OK" << std::endl;
      //      std::cout << "going to tree->Fill()" << std::endl;
      tree->Fill();
      //      std::cout << "end tree->Fill()" << std::endl;
      //std::cout << "OK" << std::endl;
   }
   out_file->Write();
   out_file->Close();

   //  #endif
   return 0;
}
