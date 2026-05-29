//2016-08-12, uploaded by nakai
//E16G4.cc
//    Last modified at <2016-05-05 20:14:22 >
//
//
// ====================================================================


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

#include <QGSP.hh>
#include <QGSP_BERT.hh>
#include <QGSP_BERT_HP.hh>
//#include <PhysicsListMessenger.hh>
//#include <PhysicsList.hh>


#ifdef G4VIS_USE
#include <G4VisExecutive.hh>
#endif




#include "E88G4_DetectorConstruction.hh"
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



const double WorldSizeX = 300.*cm;
//const double WorldSizeX = 1000.*cm;
const double WorldSizeY = 300.*cm;
const double WorldSizeZ = 300.*cm;
//const double WorldSizeZ = 2000.*cm;


E16ANA_G4OutputData* g4data;

// ====================================================================
//     main
// ====================================================================

int main(int argc, char** argv) 
{
   //gEnv->SetValue(ENVNAME,"$(ROOTSYS)/etc/gdb-backtrace.sh");
  //const char *paramFile = "/ccj/u/kawama/work/dev/database/E16data.cfg";

  const char *paramFile = "./E16CalibFiles-local.cfg";

  // param manager
  E16ANA_ParamManager *paramMgr = new E16ANA_ParamManager( paramFile );
  //Set the random seed
  //long seed=paramMgr->GetRandomSeed();
  //CLHEP::HepRandom::setTheSeed(seed);
  // run manager
  G4RunManager* runMgr= new G4RunManager;  
  
  // physics list
  /* 
  //G4String physName="QGSP_BIC_EMY";
   G4String physName="QGSP_BERT";
  //G4String physName="QGSP_BERT_HP";
  G4PhysListFactory factory;
  G4VModularPhysicsList* phys=0;
  //PhysicsListMessenger* mess=0;
  //G4VModularPhysicsList* phys=factory.GetReferencePhysList(physName);
  if(factory.IsReferencePhysList(physName)){
     phys=factory.GetReferencePhysList(physName);
   //mess = new PhysicsListMessenger();
  }*/
  
  
  //runMgr-> SetUserInitialization(phys);
  //runMgr-> SetUserInitialization(new QGSP_BERT);
  E88_PhysicsList *phys = new E88_PhysicsList();
  //phys ->AddPhysicsList("QGSP_BERT_EMV");
  //  phys ->AddPhysicsList("QGSP_BERT_HP");
  
  //if(!phys) {phys = new E88_PhysicsList();}
  
  phys->RegisterPhysics(new G4StepLimiterBuilder);
  runMgr-> SetUserInitialization(phys);


  
  g4data=new E16ANA_G4OutputData();
  int flag = g4data->OpenWriteFile("test.g4out");
  if( flag != g4data->OK ){     exit(1);   }


  //user defined manager
  E16G4_Analysis  *anaMgr    = new E16G4_Analysis();
  E16G4_PrimaryGeneratorAction *primGen =
    new E16G4_PrimaryGeneratorAction( anaMgr );
  E16G4_RunAction *runAction = new E16G4_RunAction( anaMgr, primGen );
  E16G4_EventAction *evAction = new E16G4_EventAction( anaMgr );
  E16G4_TrackingAction *trAction = new E16G4_TrackingAction( evAction, anaMgr);
  E16G4_SteppingAction *stepAction = new E16G4_SteppingAction( );
  
  E88G4_DetectorConstruction *detCon= new E88G4_DetectorConstruction();

  // set mandatory user initialization classes...
  // detector setup
  runMgr-> SetUserInitialization(detCon);
  // particles and physics processes
  // primary generator
  runMgr-> SetUserAction(primGen);

  runMgr-> SetUserAction( trAction);
  runMgr-> SetUserAction( runAction);
  runMgr-> SetUserAction( evAction);
  runMgr-> SetUserAction( stepAction);

#ifdef G4VIS_USE
   //initialize visualization package
  G4VisManager* visMan= new G4VisExecutive;
  visMan-> Initialize();
  G4cout << G4endl;
#endif

  // user session...
  //runMgr-> Initialize();
 
  if(argc==1) { // interactive session, if no arguments given
#ifdef G4UI_USE_TCSH
    // tcsh-like
    G4UItcsh* tcsh= new G4UItcsh("test(%s)[%/]:");
    G4UIterminal* session= new G4UIterminal(tcsh);
#else
    // csh-like
    G4UIterminal* session= new G4UIterminal();
    session-> SetPrompt("test(%s)[%/]:");
#endif
    session-> SessionStart();
    delete session;
     //G4UIExecutive* ui = new G4UIExecutive(argc, argv);
     //ui->SessionStart();
     //delete ui;
  }
  else if (argc==2){ // batch mode
    G4UImanager* UImanager= G4UImanager::GetUIpointer();
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager-> ApplyCommand(command+fileName);
    //runMgr-> Initialize();
    //UImanager-> ApplyCommand("/run/beamOn 10000");
    
    //runMgr-> Initialize();
    //UImanager-> ApplyCommand(command+fileName);
  }
  else{
   G4cout <<"./execute "<<G4endl;
   G4cout <<"OR "<<G4endl;
   G4cout <<"./execute [macro file]"<<G4endl;
   exit(-1);
  }

  // terminating...
#ifdef G4VIS_USE
  delete visMan;
#endif

  /*G4cout <<"Deleting EventAction ..."<<G4endl;
  delete evAction;
  G4cout <<"Deleting TrackingAction  ..."<<G4endl;
  delete trAction;
  G4cout <<"Deleting SteppingAction ..."<<G4endl;
  delete stepAction;
  G4cout <<"Deleting Prim. Gen. ..."<<G4endl;
  delete primGen;*/
  G4cout <<"Deleting Run Manager ..."<<G4endl;
  delete runMgr;
  G4cout <<"Deleting param Manager ..."<<G4endl;
  delete paramMgr; 
  G4cout <<"Deleting analysis Manager ..."<<G4endl;
  delete anaMgr;   


  g4data->CloseWriteFile();

  return 0;
}
