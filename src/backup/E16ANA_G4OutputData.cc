//2017-12-03, uploaded by yokkaich
//2016-05-05, uploaded by yokkaich
//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai
//2016-01-04, uploaded by yokkaich
//2015-12-31, uploaded by yokkaich
//2015-12-03, uploaded by yokkaich
//2015-11-02, uploaded by yokkaich
//2015-11-02, uploaded by komatsu
//2015-10-05, uploaded by yokkaich
//2015-08-20, uploaded by yokkaich
//2015-05-29, uploaded by yokkaich
//2015-03-27, uploaded by yokkaich
//2015-03-01, uploaded by yokkaich
//E16ANA_G4OutputData.cc 150120 by S. Yokkaichi
//    Last modified at <2015-08-20 20:23:16 >
//

#include "E16ANA_Constant.hh"

#include "E16ANA_G4OutputData.hh"
#include "E16ANA_Transform.hh"
#include "E16ANA_Geometry.hh"

#include <TMath.h>


using namespace std;

//------------------------------------------------------
int E16ANA_G4Event::WriteData(FILE* fpWrite){
    int flag=eventHeader.WriteData(fpWrite);
    primaryTracks.WriteData(fpWrite);
    primaryTrajectories.WriteData(fpWrite);
    secondaryTrajectories.WriteData(fpWrite);

              ssd.WriteData(fpWrite);
             gtr1.WriteData(fpWrite);
             gtr2.WriteData(fpWrite);
             gtr3.WriteData(fpWrite);
	     hbd.WriteData(fpWrite);
	     //sako
              rpc.WriteData(fpWrite);
               lg.WriteData(fpWrite);
             lgvd.WriteData(fpWrite);
        gtrframe1.WriteData(fpWrite);
        gtrframe2.WriteData(fpWrite);
        gtrframe3.WriteData(fpWrite);
              vd1.WriteData(fpWrite);
    for(int i=0;i<4;i++){ vdcyls[i] ->WriteData(fpWrite);};
    for(int i=0;i<5;i++){ targets[i]->WriteData(fpWrite);};
    return flag;
}

int E16ANA_G4Event::ReadData(FILE* fpRead){

    int flag=eventHeader.ReadData(fpRead);
    //cerr<<"E16ANA_G4OutputData L38 "<<flag<<endl;
    if (flag != 1 ) {
      E16INFO("eof");
      return 0;
    }
    primaryTracks.ReadData(fpRead);
    primaryTrajectories.ReadData(fpRead);
    secondaryTrajectories.ReadData(fpRead);

    allTrajectories.clear();
    for(int i=0; i< primaryTrajectories.Noh(); i++){
      allTrajectories.push_back( & primaryTrajectories.Hit(i) );
    }
    for(int i=0; i< secondaryTrajectories.Noh(); i++){
      allTrajectories.push_back( & secondaryTrajectories.Hit(i) );
    }
    //    E16INFO("tra size read %d %d %d",primaryTrajectories.Noh(),
    //      secondaryTrajectories.Noh(), allTrajectories.size() );


      ssd.ReadData(fpRead);
      gtr1.ReadData(fpRead);
      gtr2.ReadData(fpRead);
      gtr3.ReadData(fpRead);
      hbd.ReadData(fpRead);
      //sako
      rpc.ReadData(fpRead);
      lg.ReadData(fpRead);
      lgvd.ReadData(fpRead);
      gtrframe1.ReadData(fpRead);
      gtrframe2.ReadData(fpRead);
      gtrframe3.ReadData(fpRead);
      vd1.ReadData(fpRead);
      for(int i=0;i<4;i++){ vdcyls[i] ->ReadData(fpRead);};
      for(int i=0;i<5;i++){ targets[i]->ReadData(fpRead);};

    return flag;
}

//------------------------------------------------------
E16ANA_G4OutputData::E16ANA_G4OutputData(){
    fpRead=NULL;
    fpWrite=NULL;
    overlapEvent = new E16ANA_G4Event();
}
  
E16ANA_G4OutputData::~E16ANA_G4OutputData(){
  if( fpRead != NULL){    fclose(fpRead);     }
  if( fpWrite != NULL){      fclose(fpWrite); }
}
  

int E16ANA_G4OutputData::CloseReadFile(){
  fclose(fpRead); fpRead=NULL;
  return 0;
}
int E16ANA_G4OutputData::CloseWriteFile(){
  fclose(fpWrite); fpWrite=NULL;
  return 0;
}

int E16ANA_G4OutputData::OpenReadFile(const char* filename){
  if( fpRead != NULL ){
    cerr<<__FILE__<<" "<<__func__<<"(): file is already opend for reading: "<< filename <<endl;
    return OpenERROR;
  }

  fpRead = fopen(filename, "r");

  if ( fpRead==NULL ){
    cerr<<__FILE__<<" "<<__func__<<"(): file open error for reading: "<< filename <<endl;
    return OpenERROR;
  }
  return OK;
}

int E16ANA_G4OutputData::OpenWriteFile(const char* filename){
  if( fpWrite != NULL ){
    cerr<<__FILE__<<" "<<__func__<<"():  file is already opend for writing: "<< filename <<endl;
    return OpenERROR;
  }

  fpWrite = fopen(filename, "w");

  if ( fpWrite==NULL ){
    cerr<<__FILE__<<" "<<__func__<<"():  file open error for writing: "<< filename <<endl;
    return OpenERROR;
  }
  return OK;
}

int E16ANA_G4Event::EventAppend(double timeoffset,
				E16ANA_G4Event& ev2, int eventIDoffset  ){
  if( eventHeader.AppendCount()== 0){//first event of append
    //def    eventHeader.Set(ev2.G4EventID(), ev2.JAMEventID(), ev2.InputMultiplicity());
    //sako 2019/11/18
    eventHeader.Set(ev2.G4EventID(), ev2.JAMEventID(), ev2.InputMultiplicity(), ev2.JAMImpact());
    eventHeader.AppendCountUp();
  }
  else{
    eventHeader.AppendCountUp();
  }
  
  primaryTracks.HitsAppend(  timeoffset, ev2.PrimaryTracks(), 
			    eventIDoffset );

  primaryTrajectories.HitsAppend(  timeoffset, ev2.PrimaryTrajectories(),
    eventIDoffset);
  secondaryTrajectories.HitsAppend(  timeoffset, ev2.SecondaryTrajectories(),
    eventIDoffset);

  allTrajectories.clear();
  for(int i=0; i< primaryTrajectories.Noh(); i++){
    allTrajectories.push_back( & primaryTrajectories.Hit(i) );
  }
  for(int i=0; i< secondaryTrajectories.Noh(); i++){
    allTrajectories.push_back( & secondaryTrajectories.Hit(i) );
  }

  for(int i=0;i<5;i++){
    targets[i]->HitsAppend(  timeoffset, ev2.TARGET(i), eventIDoffset );
  }
  for(int i=0;i<3;i++){
    gtrs[i]->HitsAppend(  timeoffset, ev2.GTRs(i), eventIDoffset );
    gtrframes[i]->HitsAppend(  timeoffset, ev2.GTRFrames(i), eventIDoffset ); 
  }
  for(int i=0;i<4;i++){
    vdcyls[i]->HitsAppend(  timeoffset, ev2.VDCylinders(i), eventIDoffset );
    }
  ssd.  HitsAppend(  timeoffset, ev2.SSD() , eventIDoffset );
  hbd.  HitsAppend(  timeoffset, ev2.HBD() , eventIDoffset );
  //sako
  rpc.  HitsAppend(  timeoffset, ev2.RPC() , eventIDoffset );
  lg.   HitsAppend(  timeoffset, ev2.LG()  , eventIDoffset );
  lgvd. HitsAppend(  timeoffset, ev2.LGVD(), eventIDoffset );
  vd1.  HitsAppend(  timeoffset, ev2.VD1() , eventIDoffset );

  return 0;
}

int E16ANA_G4OutputData::ReadOverlapEvent(int multi, double* timeInterval, 
int jamInteractionFlag){
  delete overlapEvent;
  overlapEvent = new E16ANA_G4Event();//in order to clear the previous.

  int nEvent=0;
  for (int i=0 ; i<multi;i++){
    int flag = ReadAnEvent();
    if( flag != 1 ) {
      E16FATAL("file end at event %d",i); 
      break;
    }

    //    cerr<<"jam multi "<<event.InputMultiplicity()<<endl;
    if( jamInteractionFlag == 1 && event.InputMultiplicity() < 2 ){
      continue; // not interaction// definition m>1 should be discussed.
    }

    overlapEvent->EventAppend(timeInterval[i], event);

    nEvent++;
  }

  return nEvent;
  
}
//--------------------------------------
void E16ANA_G4ModuleHitList::HitPrint(){
  cerr<<"G4ModuleHitList::HitPrint"<<endl;
  for(int layerID=0; layerID<3; layerID++){
    for( int k=0; k< NmoduleGTR; k++){
      cerr<<GTRModule(layerID, k).size()<<" ";
    }
    cerr<<endl;
  }//for i

}

void E16ANA_G4ModuleHitList::OverlapGTRSingleHits(){

  for (int k=0; k< NmoduleGTR ;k++){   //module-by-module
    vector<E16ANA_G4Hit>& vec1 = GTRModule(0, k);
    vector<E16ANA_G4Hit>& vec2 = GTRModule(1, k);
    vector<E16ANA_G4Hit>& vec3 = GTRModule(2, k);

    E16ANA_EventOverlap::InsertSingleRateBKG100(k, vec1);
    E16ANA_EventOverlap::InsertSingleRateBKG200(k, vec2);
    E16ANA_EventOverlap::InsertSingleRateBKG300(k, vec3);

  }//for k
}

void E16ANA_G4ModuleHitList::OverlapSSDSingleHits(){
  // SSD Module ID          0   1   2   3   4   5   6   7   8
  int is_forward[9]    = {  1,  1,  1,  1,  1,  1,  1,  1,  1};
  double halo_angle[9] = {-30,-30,-30,  0,  0,  0, 30, 30, 30};
  for(int k=0; k<NmoduleSSD; k++){
     vector<E16ANA_G4Hit> &vec = SSDModule(k);
     E16ANA_EventOverlap::InsertSingleRateBKG_SSD(k, vec, is_forward[k], halo_angle[k]);
  }
}

void E16ANA_G4ModuleHitList::OverlapGTRSingleHits151201(){
  //old single rate-time interval
  E16ANA_EventOverlap::SetSingleRate151201();

  for (int k=0; k< NmoduleGTR ;k++){   //module-by-module
    vector<E16ANA_G4Hit>& vec1 = GTRModule(0, k);
    vector<E16ANA_G4Hit>& vec2 = GTRModule(1, k);
    vector<E16ANA_G4Hit>& vec3 = GTRModule(2, k);

    E16ANA_EventOverlap::InsertSingleRateBKG100_151201(k, vec1);
    E16ANA_EventOverlap::InsertSingleRateBKG200_151201(k, vec2);
    E16ANA_EventOverlap::InsertSingleRateBKG300_151201(k, vec3);

  }//for k

}

#if 0
void E16ANA_G4ModuleHitList::OverlapGTRSingleHits(){

  for (int k=0; k< NmoduleGTR ;k++){   //module-by-module
    vector<E16ANA_G4Hit>& vec1 = GTRModule(0, k);
    vector<E16ANA_G4Hit>& vec2 = GTRModule(1, k);
    vector<E16ANA_G4Hit>& vec3 = GTRModule(2, k);

    vector<E16ANA_G4Hit> vec1s;  E16ANA_EventOverlap::SingleRate100(k, vec1s);
    vector<E16ANA_G4Hit> vec2s;  E16ANA_EventOverlap::SingleRate200(k, vec2s);
    vector<E16ANA_G4Hit> vec3s;  E16ANA_EventOverlap::SingleRate300(k, vec3s);

    vec1.insert(vec1.end(), vec1s.begin(), vec1s.end() );
    vec2.insert(vec2.end(), vec2s.begin(), vec2s.end() );
    vec3.insert(vec3.end(), vec3s.begin(), vec3s.end() );
  }//for k

}
#endif

void E16ANA_G4ModuleHitList::SetG4Event(E16ANA_G4Event& event){

  for(int layerID=0; layerID<3; layerID++){
    int jmax = event.GTRs( layerID ).Noh();
    //    cerr<<"jmax "<<jmax<<endl;
    for (int j=0; j< jmax ;j++){
      E16ANA_G4Hit & hit = event.GTRs( layerID ).Hit(j);//reference
      int detID = hit.DetectorID();
      int pid = hit.PID();
      //      cerr<<"detID "<<detID<<endl;
      if( pid != 22 ){ // i.e. not gamma
	GTRModule(layerID, detID).push_back(hit);//copy
      }
    }//for j
    //    cerr<<layerID<<"sizes  ";
  }//for i

  int jmax = event.HBD().Noh();
  //    cerr<<"jmax "<<jmax<<endl;
  for (int j=0; j< jmax ;j++){
    E16ANA_G4Hit & hit = event.HBD().Hit(j);//reference
    int detID = hit.DetectorID();
    int pid = hit.PID();
    //      cerr<<"detID "<<detID<<endl;
    if( pid != 22 ){ // i.e. not gamma
      HBDModule( detID ).push_back(hit);//copy
    }
  }//for j

  //sako
  jmax = event.RPC().Noh();
  //    cerr<<"jmax "<<jmax<<endl;
  for (int j=0; j< jmax ;j++){
    E16ANA_G4Hit & hit = event.RPC().Hit(j);//reference
    int detID = hit.DetectorID();
    int pid = hit.PID();
    //      cerr<<"detID "<<detID<<endl;
    if( pid != 22 ){ // i.e. not gamma
      RPCModule( detID ).push_back(hit);//copy
    }
  }//for j

  jmax = event.LGVD().Noh();
  //    cerr<<"jmax "<<jmax<<endl;
  for (int j=0; j< jmax ;j++){
    E16ANA_G4Hit & hit = event.LGVD().Hit(j);//reference
    int detID = hit.DetectorID();
    int pid = hit.PID();
    //      cerr<<"detID "<<detID<<endl;
    if( pid != 22 ){ // i.e. not gamma
      LGVDModule( detID ).push_back(hit);//copy
    }
  }//for j

  int noh_ssd = event.SSD().Noh();
  for(int i=0; i<noh_ssd; i++){ // SSD
     E16ANA_G4Hit &hit = event.SSD().Hit(i);
     int detID = hit.DetectorID();
     int pid = hit.PID();
     if(pid != 22){
        SSDModule(detID).push_back(hit);
     }
  } // for i
}

//------------------------------------
#include <TRandom3.h>

//#include "E16ANA_ErrorMessage.hh"
//#include "E16ANA_EventOverlap.hh"
//#include "E16ANA_G4OutputData.hh"

//static member

E16ANA_Geometry* E16ANA_EventOverlap::geom=NULL;
double E16ANA_EventOverlap::mean100;
double E16ANA_EventOverlap::halo100;
double E16ANA_EventOverlap::mean200;
double E16ANA_EventOverlap::mean300;
double E16ANA_EventOverlap::backwardRatio100;
double E16ANA_EventOverlap::halo_rate_ssd;
double E16ANA_EventOverlap::random_rate_ssd;

E16ANA_EventOverlap::E16ANA_EventOverlap(){
    SetSingleRateDefault();
    E16INFO("initialized.");
}

//static member function

int E16ANA_EventOverlap::RandomForwardAngleSphere(double* x,
  double* y, double* z, double polarAngleThr){
  //  double x=y=z=0;
  double r=1;
  int i=0;
  double thr = DegToRad * polarAngleThr;

  for(i=0;i<10000;i++){
    gRandom->Sphere( (Double_t&)*x, (Double_t&)*y, (Double_t&)*z, r);
    //    double r2 = *x**x + *y**y + *z**z ;
    //    cerr<<"in RH "<<i<<" "<<*x<<" "<<*y<<" "<<*z<<" "<<r2<<endl;
    double polar = Hep3Vector(*x, *y, *z).theta();
    if( polar < thr ){
      break;
    }
  }
  return i;
}

int E16ANA_EventOverlap::RandomForwardHemisphere(double* x,
  double* y, double* z){
  //  double x=y=z=0;
  double r=1;
  int i=0;

  for(i=0;i<10000;i++){
    gRandom->Sphere( (Double_t&)*x, (Double_t&)*y, (Double_t&)*z, r);
    //    double r2 = *x**x + *y**y + *z**z ;
    //    cerr<<"in RH "<<i<<" "<<*x<<" "<<*y<<" "<<*z<<" "<<r2<<endl;
    if( *z > 0 ){      break;    }
  }
  return i;
}

int E16ANA_EventOverlap::PoissonIntervalArray(
    double mean, int range, int start, double* array){
    int nbin = range;
    double probability  = mean / nbin; 
    int nhit = 0;

    for(int i=0; i<nbin;i++){
      double random = gRandom->Uniform(1.0);
      if( random < probability ) {// hit!
	array[nhit] = i +  start;
	nhit ++;
      }
    }
    return nhit;
  }

void E16ANA_EventOverlap::SetSingleRate( double imean100, 
  double imean200, double imean300, double ibackwardRatio){
  mean100 = imean100;
  mean200 = imean200;
  mean300 = imean300;
  backwardRatio100 = ibackwardRatio;
}

void E16ANA_EventOverlap::SetSingleRateDefault(){//default rate
  mean100 = 75*0.5;         // 75= [1475nsec for 50MHz (= 5kHz/mm2*100*100)]
  halo100 = 75*0.5;         // and its half is halo
  mean200 = mean100;
  mean300 = mean100;
  backwardRatio100 = 0.5;
  halo_rate_ssd = 20.0*0.5; // 5.0 [kHz/mm^2] (GTR100) * 4 (half R) / 2 (halo and random rate are same)
  random_rate_ssd = 20.0*0.5; // 5.0 [kHz/mm^2] (GTR100) * 4 (half R) / 2
}


void E16ANA_EventOverlap::SetSingleRateDefaultAndReScale(double scale){
  SetSingleRateDefault();
  mean100 *= scale;
  halo100 *= scale;
  mean200 *= scale;
  mean300 *= scale;
}

void E16ANA_EventOverlap::SetSingleRate151201(){//old default rate
  mean100 = 25;         //25 = [525nsec for 50MHz (= 5kHz/mm2*100*100) ]
  mean200 = mean100 * 0.5;
  mean300 = mean100 * 0.5;
  backwardRatio100 = 0.25;
}

void E16ANA_EventOverlap::InsertSingleRateBKG(int flag, int layerID, 
  int moduleID, double mean, double size, std::vector<E16ANA_G4Hit>& hits){

  double half = 0.5*size;
  double timeInterval[1024]={0};
#if 0
  //-175 to 350 : range=525
  int nhit = E16ANA_EventOverlap::PoissonIntervalArray(
    mean, 525, -175, timeInterval);
#endif
  //-950 to 475: range 1425
  int nhit = E16ANA_EventOverlap::PoissonIntervalArray(
    mean, 1425, -950, timeInterval);

  if(nhit > 1024 ){nhit=1024;}

  for(int i=0; i<nhit; i++){

    double gtr_hit_x = gRandom->Uniform(-half, half);
    double gtr_hit_y = gRandom->Uniform(-half, half);
    E16ANA_G4Hit hit;
    hit.SetXYZ(gtr_hit_x, gtr_hit_y, 0.0);
    TVector3 localPos = hit.XTV();
    //TVector3 gPos = E16ANA_Transform::GetGPos(localPos, geom,  layerID, moduleID);
    TVector3 gPos = geom->GetGPos(localPos, layerID, moduleID);
    hit.SetGXYZ(gPos);

    int event = -1;
    int track = -1;
    int pID   = 211;//pi+      //should be proton?
    hit.SetID(event, track, moduleID, pID);
    double time = timeInterval[i];
    hit.SetT( time );

    double x=1, y=1, z=1;
    double polarAngleThr=85;//degree    : tan(85deg)*3mm =34.2mm
    if( flag ==  0 ){    //random direction for local forward hemisphare
      int n=RandomForwardAngleSphere(&x, &y, &z, polarAngleThr );
      //      int n=RandomForwardHemisphere(&x, &y, &z);
      //    cerr<<"random hemisphere "<<n<<" "<<x<<" "<<y<<" "<<z<<endl;
    }
    else if( flag ==  1 ){  //halo
      if( moduleID == 15 || moduleID == 16 || moduleID == 17){//up/down
	x=0; y=0; z=1.0;//0deg
      }
      else if (moduleID == 13){//left
      	x= -1.0; y=0; z=1.732;//-30deg
      }
      else if (moduleID == 19){//right
	x= +1.0; y=0; z=1.732;//+30deg
      }
    }

    hit.SetP(x,y,z);

    hits.push_back( hit ); // copy
  }


}

// -------------------------------------------------
// -------------------- SSD ------------------------
// -------------------------------------------------

void E16ANA_EventOverlap::InsertSingleRateBKG_SSD(int moduleID, std::vector<E16ANA_G4Hit> &hits, int is_forward, double halo_angle){
   double size = 50.0; // [mm]
   InsertSingleRateBKGRandom_SSD(moduleID, random_rate_ssd, size, hits);
   if(is_forward == 1){
      TVector3 halo_direction(tan(halo_angle/180.0*TMath::Pi()), 0, 1.0);
      InsertSingleRateBKGHalo_SSD(moduleID, halo_rate_ssd, size, hits, halo_direction);
   }
}

void E16ANA_EventOverlap::InsertSingleRateBKG_SSD(int flag,
  int moduleID, double rate, double size, std::vector<E16ANA_G4Hit>& hits, const TVector3 &halo_direction){

  // mean = rate [kHz/mm^2] * size^2 [mm^2] * time_window [ns] * 1.0e-6
  double half = 0.5*size;
  double timeInterval[1024]={0};
  // -600.0 ns < t < 200.0 ns
  double t_start = -600.0;
  double t_range =  800.0;
  // -15.0 ns < t < 15.0 ns
  //double t_start = -15.0;
  //double t_range =  30.0;
  double mean = rate*size*size*t_range*1.0e-6;
  int nhit = E16ANA_EventOverlap::PoissonIntervalArray(
        mean, t_range, t_start, timeInterval);

  if(nhit > 1024 ){nhit=1024;}

  for(int i=0; i<nhit; i++){

    double gtr_hit_x = gRandom->Uniform(-half, half);
    double gtr_hit_y = gRandom->Uniform(-half, half);
    E16ANA_G4Hit hit;
    hit.SetXYZ(gtr_hit_x, gtr_hit_y, 0.0);
    TVector3 localPos = hit.XTV();
    //TVector3 gPos = geom->SSD(moduleID)->GetGPos(localPos); // V2
    TVector3 gPos(0,0,0);
    hit.SetGXYZ(gPos);

    int event = -1;
    int track = -1;
    int pID   = 211;//pi+      //should be proton?
    hit.SetID(event, track, moduleID, pID);
    double time = timeInterval[i];
    hit.SetT( time );

    double x=1, y=1, z=1;
    double polarAngleThr=85;//degree    : tan(85deg)*3mm =34.2mm
    if( flag ==  0 ){    //random direction for local forward hemisphare
      int n=RandomForwardAngleSphere(&x, &y, &z, polarAngleThr );
      //      int n=RandomForwardHemisphere(&x, &y, &z);
      //    cerr<<"random hemisphere "<<n<<" "<<x<<" "<<y<<" "<<z<<endl;
    }
    else if( flag ==  1 ){  //halo
      x = halo_direction.X();
      y = halo_direction.Y();
      z = halo_direction.Z();
    }

    hit.SetP(x,y,z);

    hits.push_back( hit ); // copy
  }
}

#if 0
void SingleRate100(int detID, vector<E16ANA_G4Hit>& hits){
  double mean =  25; // /525nsec for 50MHz = 5kHz/mm2*100*100
  double size =10;//cm : current G4

  //  if( detID == 13 || detID == 15 || detID == 16 || detID == 17 || detID==19 ){
  if( detID == 15 || detID == 16 || detID == 17){//up/down
    SingleRate(detID, mean, size, hits, 5);
  }
  else if (detID == 13){//left
    SingleRate(detID, mean, size, hits, -30);
  }
  else if (detID == 19){//right
    SingleRate(detID, mean, size, hits, +30);
  }
  else{
    mean *= 0.25;//temporary check
    SingleRate(detID, mean, size, hits);
  }

}

    else if (angleflag == 5){//halo  up/down
      if( d<2.0){
	hit.SetP( 0.1, 0.0, 1.0);  //6deg
      }
      else             { hit.SetP(-0.1, -0.0, 1.0);}
    }
    else if (angleflag == 30){//halo   :right
	hit.SetP( 1.0, 0.0, 2.0);  //30deg
    }
    else if (angleflag == -30){//halo  : left
	hit.SetP( -1.0, 0.0, 2.0);  //30deg
    }
    

#endif

void E16ANA_EventOverlap::InsertSingleRateBKG100(
  int moduleID, std::vector<E16ANA_G4Hit>& hits){

  int layerID = 0;
  double mean =  mean100; 
  //double size =10;//cm : current G4
  double size =100.0;//mm : current G4

  if( moduleID == 13 || moduleID == 15 || moduleID == 17 || moduleID==19 ){
    InsertSingleRateBKGHalo(layerID, moduleID, halo100, size, hits);
    mean *= 1.0;
  }
  else{
    mean *= backwardRatio100;
  }
  InsertSingleRateBKGRandom(layerID, moduleID, mean, size, hits);
}

void E16ANA_EventOverlap::InsertSingleRateBKG200(int moduleID, std::vector<E16ANA_G4Hit>& hits){
  //    double mean =  12.5; // /525nsec for 25MHz = 1.25kHz/mm2*200*200/2
  double mean =  mean200;
  //double size =20;//cm
  double size =200.0;//mm
  InsertSingleRateBKGRandom(1, moduleID, mean, size, hits);
}

void E16ANA_EventOverlap::InsertSingleRateBKG300(int moduleID, std::vector<E16ANA_G4Hit>& hits){
  //    double mean =  12.5; // /525nsec for 25MHz = 5kHz/mm2*300*300
  double mean =  mean300;
  //double size =30;//cm 
  double size =300.0;//mm 
  InsertSingleRateBKGRandom(2, moduleID, mean, size, hits);
}


// in order to use the old (151201) set------------------------
//  to compare the old and new results

void E16ANA_EventOverlap::InsertSingleRateBKG151201(double moduleID, double mean, double size, std::vector<E16ANA_G4Hit>& hits){
  double half = 0.5*size;
  double timeInterval[1024]={0};

  int nhit = E16ANA_EventOverlap::PoissonIntervalArray(
    mean, 525, -175, timeInterval);

  if(nhit > 1024 ){nhit=1024;}

  //std::cerr<<"PoissonSingle: "<<mean<<" "<<nhit<<" ";
  for(int i=0; i<nhit; i++){

    double gtr_hit_x_org = gRandom->Uniform(-half, half);
    double gtr_hit_y_org = gRandom->Uniform(-half, half);
    double d = gRandom->Uniform(0,4);
    E16ANA_G4Hit hit;
    hit.SetXYZ(gtr_hit_x_org, gtr_hit_y_org, 0.0);

    if( d<1.0){
      hit.SetP( 1.0, 1.0, 1.0);  //45deg
    }
    else if ( d<2.0) { hit.SetP( 1.0, -1.0, 1.0);}
    else if ( d<3.0) { hit.SetP(-1.0,  1.0, 1.0);}
    else             { hit.SetP(-1.0, -1.0, 1.0);}

    double time = timeInterval[i];
    hit.SetT( time );
    //    cerr<<time<<" ";
    int event = -1;
    int track = -1;
    int pID   = 211;//pi+      //should be proton?
    hit.SetID(event, track, moduleID, pID);

    hits.push_back( hit ); // copy
  }


}
void E16ANA_EventOverlap::InsertSingleRateBKG100_151201(
  int moduleID, std::vector<E16ANA_G4Hit>& hits){
  //double mean =  25; // /525nsec for 50MHz = 5kHz/mm2*100*100

  double mean =  mean100; // /525nsec for 50MHz = 5kHz/mm2*100*100
  if( moduleID == 13 || moduleID == 15 || moduleID == 16 || moduleID == 17 || moduleID==19 ){
    mean *= 1.0;
  }
  else{
    mean *= backwardRatio100;
  }
  double size =10;//cm : current G4
  InsertSingleRateBKG151201(moduleID, mean, size, hits);
}
void E16ANA_EventOverlap::InsertSingleRateBKG200_151201(int moduleID, std::vector<E16ANA_G4Hit>& hits){
  //    double mean =  12.5; // /525nsec for 25MHz = 1.25kHz/mm2*200*200/2
  double mean =  mean200;
  double size =20;//cm
  InsertSingleRateBKG151201(moduleID, mean, size, hits);
}

void E16ANA_EventOverlap::InsertSingleRateBKG300_151201(int moduleID, std::vector<E16ANA_G4Hit>& hits){
  //    double mean =  12.5; // /525nsec for 25MHz = 5kHz/mm2*300*300
  double mean =  mean300;
  double size =30;//cm 
  InsertSingleRateBKG151201(moduleID, mean, size, hits);
}
// in order to use the old set------------------------

