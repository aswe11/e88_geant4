//2017-12-03, uploaded by yokkaich
//2016-05-12, uploaded by nakai
//2016-05-05, uploaded by yokkaich
//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai
//2016-01-04, uploaded by yokkaich
//2015-12-31, uploaded by yokkaich
//2015-12-21, uploaded by yokkaich
//2015-12-03, uploaded by yokkaich
//2015-11-02, uploaded by yokkaich
//2015-11-02, uploaded by komatsu
//2015-10-05, uploaded by yokkaich
//2015-08-20, uploaded by yokkaich
//2015-08-03, uploaded by yokkaich
//2015-05-29, uploaded by yokkaich
//2015-03-27, uploaded by yokkaich
//2015-03-01, uploaded by yokkaich
//E16ANA_G4OutputData.hh 150120 by S. Yokkaichi
//    Last modified at <2015-08-20 20:19:17 >

#ifndef E16ANA_G4OutputData_HH
#define E16ANA_G4OutputData_HH

#include <stdio.h>
#include <string.h>

#include <iostream>
#include <string>
#include <vector>
#include <bitset>

#include <CLHEP/Vector/ThreeVector.h>
#include <HepPID/ParticleName.hh>
#include <TVector3.h>

using namespace CLHEP;

#include "E16ANA_ErrorMessage.hh"

// read/write

class E16ANA_G4RunHeader{
public:
  int WriteData(FILE* fp){
    return fwrite(this, sizeof(E16ANA_G4RunHeader),1,fp);
  }
  int ReadData(FILE* fp){
    return fread(this, sizeof(E16ANA_G4RunHeader),1,fp);
  }
private:
  float jamEnergy;// jam projectile kinetic energy
  float bmin;//jam impact param lower limit
  float bmax;//jam impact param upper limit
  char projectile[32];
  char target[32];
  char magfield[32];
  char physlist[32];
};

class E16ANA_G4EventHeader{
public:
  E16ANA_G4EventHeader(){
    eventID_G4=-9;
    eventID_JAM=-9;
    multi=-9;
    appendCount=0;
    version = VERSION;
    type= TYPE;
    impact = -1;//sako 2019/11/18
  }
  static const short VERSION = 2;
  static const short TYPE = 1;     // for G4event

  int WriteData(FILE* fp){
    return fwrite(this, sizeof(E16ANA_G4EventHeader),1,fp);
  }
  int ReadData(FILE* fp){
    int n =  fread(this, sizeof(E16ANA_G4EventHeader),1,fp);
    E16MESSAGE("version : code is %d, file is %d", VERSION, version);
    if( version != VERSION || type != TYPE){
      E16FATAL("version/type unmatch: code is %d/%d, file is %d/%d", 
	VERSION, TYPE, version, type);
      Dump();
    }
    return n;
  }
  int EventID_G4(){return eventID_G4;}
  int EventID_JAM(){return eventID_JAM;}
  int InputMultiplicity(){return multi;}
  double Impact(){return impact;}//sako 2019/11/18
  int AppendCount(){return appendCount;}
  void AppendCountUp(){ appendCount++; }

  //def
  /*
  void Set(int id1, int id2, int m){
    eventID_G4 = id1;     eventID_JAM = id2; multi = m;
  };
  */
  //sako 2019/11/16
  void Set(int id1, int id2, int m, double imp){
    eventID_G4 = id1;     eventID_JAM = id2; multi = m; impact = imp;
  };

  void Dump(std::string tag=""){
    fprintf(stderr, 
	    "E16ANA_EventHeader: version=%3i : %s : evG4 evJAM M overlap = %5i %5i %5i %5i\n",
      version, tag.c_str(), eventID_G4, eventID_JAM, multi,appendCount);

    //  int eventID_G4;
    //  int eventID_JAM;
    //  short multi;// JAM track multiplicity
    //  short appendCount;//# of overlap(append) : 0 is new, 1 is single event
                    // 2 is double event,...
    // appendCount>0, eventID & multi  is for the first event.
  }

private:
  int eventID_G4;
  int eventID_JAM;
  short multi;// JAM track multiplicity
  short appendCount;//# of overlap(append) : 0 is new, 1 is single event
                    // 2 is double event,...
  short version;//data version
  short type;   //data type
  double impact;// JAM impact parameter sako, 2019/11/18
};

class E16ANA_G4BitFlag{
private:
  std::bitset<32> bitflag;//for detector hit
  //org
  //  enum {SSD=1, GTR1, GTR2, GTR3, HBD, LG, HBDfront, HBDback};
  //sako 2019/9/16
  enum {SSD=1, GTR1, GTR2, GTR3, HBD, LG, HBDfront, HBDback, RPC};
public:
  void Clear()   { bitflag.reset();}

  void SetSSD()  { bitflag.set(SSD,1);}
  void SetGTR1()  { bitflag.set(GTR1,1);}
  void SetGTR2()  { bitflag.set(GTR2,1);}
  void SetGTR3()  { bitflag.set(GTR3,1);}
  void SetHBD()  { bitflag.set(HBD,1);}
  void SetLG()  { bitflag.set(LG,1);}
  void SetHBDfront()  { bitflag.set(HBDfront,1);}
  void SetHBDback()  { bitflag.set(HBDback,1);}
  //sako
  void SetRPC()  { bitflag.set(RPC,1);}

  bool SSDFire()  { return bitflag.test(SSD);}
  bool GTR1Fire()  { return bitflag.test(GTR1);}
  bool GTR2Fire()  { return bitflag.test(GTR2);}
  bool GTR3Fire()  { return bitflag.test(GTR3);}
  bool HBDFire()  { return bitflag.test(HBD);}
  bool LGFire()  { return bitflag.test(LG);}
  bool HBDfrontFire()  { return bitflag.test(HBDfront);}
  bool HBDbackFire()  { return bitflag.test(HBDback);}
  //sako
  bool RPCFire()  { return bitflag.test(RPC);}
};

class E16ANA_G4Trajectory{
private:
  short eventID;
  short trackID;          //track id in the event 
  short pID;         //PDG ID : pion, electron ...
  short parentID;    //parent Track id
  double x,y,z;      //primary position
  double px,py,pz;   // at init point
  double p;          // abs. value at init point, 
  double t;

  char processName[32];
  char vertexVolumeName[32];  
  E16ANA_G4BitFlag bitflag;
  int noh;//number of hit in the trajectory
  //  list of hit point?

public:
  E16ANA_G4Trajectory(){
    Clear();
  }
  
  enum{ErrorValue = -9999};

  E16ANA_G4BitFlag* HitFlag(){return &bitflag;}

  void Clear(){
    eventID = ErrorValue;
    trackID = ErrorValue; pID = ErrorValue; parentID = ErrorValue;
    x = ErrorValue;    y = ErrorValue;    z = ErrorValue;
    px= ErrorValue;    py= ErrorValue;    pz= ErrorValue;
    t = ErrorValue;
    bitflag.Clear();    noh = 0;
    //    std::cerr<<"sizeof bitset/int "<<sizeof(bitflag)<<" "<<sizeof(hitflag)<<std::endl;
    //size of bitset<32> = 8[byte]
  }

  void SetData(int eventid, int trackid, int pid, int parentid,
    double ix, double iy, double iz, 
    double ipx, double ipy, double ipz, double ip, 
    double it){
    eventID = eventid; trackID = trackid;  pID = pid; parentID=parentid,
    x=ix; y=iy; z=iz; px=ipx; py=ipy; pz=ipz; 
    p=ip;  t=it;
  }

  void SetNames(const char* procName, const char* vertexVolName){
    sprintf(processName, "%s", procName);
    sprintf(vertexVolumeName, "%s", vertexVolName);
  }
  void SetNames(std::string procName, std::string vertexVolName){
    sprintf(processName, "%s", procName.c_str() );
    sprintf(vertexVolumeName, "%s", vertexVolName.c_str() );
  }

  void SetT(double it){t=it;}

  int WriteData(FILE* fp){
    return fwrite(this, sizeof(E16ANA_G4Trajectory),1,fp);
  }
  int ReadData(FILE* fp){
    return fread(this, sizeof(E16ANA_G4Trajectory),1,fp);
  }
  int TrackID(){return trackID;}
  int EventID(){return eventID;}
  void EventIDputOffset(int offset){eventID += offset;}
  int PID(){return pID;}
  int ParentID(){return parentID;}
  double Time(){return t;}
  Hep3Vector PV() {return Hep3Vector(px,py,pz);}
  TVector3 PTV() {return TVector3(px,py,pz);}
  Hep3Vector Vertex() {return Hep3Vector(x,y,z);}
  TVector3 TVertex() {return TVector3(x,y,z);}
  char* Process(){return processName;}
  char* VertexVolume(){return vertexVolumeName;}

  void Dump(std::string tag=""){
    fprintf(stderr, 
	    "E16ANA_G4Trajectory: %s : %5i %6.1f %6.1f %6.1f / %6.2f %6.2f %6.2f:%6.2f/ %6.1f %6i %6i %s %s %s\n",
	    tag.c_str(), eventID, x, y, z, px, py, pz, p, t, trackID, pID,
      HepPID::particleName(pID).c_str(), processName, vertexVolumeName );
  }

};



class E16ANA_G4Track{
private:
  short eventID;
  short trackID;          //track id in the event 
  short pID;         //PDG ID : pion, electron ...
  short parentID;    //parent Track id
  double x,y,z;      //primary position
  double px,py,pz;   // abs. value at init point, 
  double p;
  double t;
  //  list of hit point?
public:
  E16ANA_G4Track(){
    Clear();
  }
  enum{ErrorValue = -9999};
  void Clear(){
    eventID = ErrorValue;
    trackID = ErrorValue; pID = ErrorValue; parentID = ErrorValue;
    x = ErrorValue;    y = ErrorValue;    z = ErrorValue;
    px= ErrorValue;    py= ErrorValue;    pz= ErrorValue;
    t = ErrorValue;
  }
  void SetData(int eventid, int ipid, double ix, double iy, double iz, 
	       double ipx, double ipy, double ipz, double ip,  
	       double it = ErrorValue){
    eventID = eventid; pID=ipid;
    x=ix; y=iy; z=iz; px=ipx; py=ipy; pz=ipz; 
    p=ip;  t=it;
  }
  void SetT(double it){t=it;}
  int WriteData(FILE* fp){
    return fwrite(this, sizeof(E16ANA_G4Track),1,fp);
  }
  int ReadData(FILE* fp){
    return fread(this, sizeof(E16ANA_G4Track),1,fp);
  }
  int TrackID(){return trackID;}
  int EventID(){return eventID;}
  void EventIDputOffset(int offset){eventID += offset;}
  int PID(){return pID;}
  int ParentID(){return parentID;}
  double Time(){return t;}
  Hep3Vector PV() {return Hep3Vector(px,py,pz);}
  TVector3 PTV() {return TVector3(px,py,pz);}
  Hep3Vector Vertex() {return Hep3Vector(x,y,z);}
  TVector3 TVertex() {return TVector3(x,y,z);}
  void Dump(std::string tag=""){
    fprintf(stderr, 
	    "E16ANA_G4Track: %s : %5i %6.1f %6.1f %6.1f / %6.2f %6.2f %6.2f:%6.2f/ %6.1f %6i %6i %s\n",
	    tag.c_str(), eventID, x, y, z, px, py, pz, p, t, trackID, pID,
	    HepPID::particleName(pID).c_str() );
  }


};


class E16ANA_G4Hit{
private:
  short eventID;
  short trackID;
  short detectorID;   //module ID
  short pID;           //particle ID in PDG code
  float dE;          // energy deposit
  float flightLength;//
  float beta;
  double p;          //momentum value
  double x,y,z;      //local cooridnate in the detector
  double gx,gy,gz;   //global cooridnate
  double px,py,pz;   //momentum at hit point
  double gpx,gpy,gpz;   //momentum at hit point:global
  double t;          //timing

public:
  E16ANA_G4Hit(){
    Clear();
  }
  enum{ErrorValue = -9999};

  void SetID  (int id1, int id2, int id3, int id4){ 
    eventID = id1; trackID = id2; detectorID = id3; pID = id4;};
  void SetEP  (double ide, double ip, double ibeta){
    dE=ide; p=ip; beta=ibeta; };
  void SetXYZ (double ix, double iy, double iz){    x=ix; y=iy; z=iz; }
  void SetXYZ(TVector3& v){    x=v.X();y=v.Y();z=v.Z(); }
  void SetGXYZ(double ix, double iy, double iz){    gx=ix; gy=iy; gz=iz;}
  void SetGXYZ(TVector3& gv){    gx=gv.X();gy=gv.Y();gz=gv.Z(); }
  void SetP   (double ix, double iy, double iz){    px=ix; py=iy; pz=iz; }
  void SetP   (TVector3& v){    px=v.X(); py=v.Y(); pz=v.Z(); }

  void SetGP  (double ix, double iy, double iz){   gpx=ix;gpy=iy;gpz=iz; }
  void SetGP  (TVector3& gv){    gpx=gv.X(); gpy=gv.Y(); gpz=gv.Z(); }

  void SetT(double it){ t=it;}

  int EventID(){return eventID;}
  void EventIDputOffset(int offset){eventID += offset;}
  int TrackID(){return trackID;}
  int DetectorID(){return detectorID;}
  int ModuleID(){return detectorID;}//same as DetectorID();
  int PID(){return pID;}

  double X()  {return x;}
  double Y()  {return y;}
  double Z()   {return z;}
  double GX()  {return gx;}
  double GY()   {return gy;}
  double GZ()   {return gz;}
  Hep3Vector XV()  {return Hep3Vector(x,y,z);}
  Hep3Vector GXV()  {return Hep3Vector(gx,gy,gz);}
  Hep3Vector PV()  {return Hep3Vector(px,py,pz);}
  Hep3Vector GPV() {return Hep3Vector(gpx,gpy,gpz);}

  TVector3 XTV() {return TVector3(x,y,z);}
  TVector3 GXTV(){return TVector3(gx,gy,gz);}
  TVector3 PTV() {return TVector3(px,py,pz);}
  //  TVector3 PTV() {std::cout << "PTV ("<< px << " " << py << " " << pz << std::endl;return TVector3(px,py,pz);}
  TVector3 GPTV() {return TVector3(gpx,gpy,gpz);}

  double Time(){return t;}

  double Beta(){return beta;}

  void Clear(){
    trackID = detectorID = pID = ErrorValue; 
    dE = flightLength = beta = ErrorValue;
    p=ErrorValue;
    x  = y = z = ErrorValue;
    gx = gy =gz = ErrorValue;
    px = py =pz = ErrorValue;
    t=ErrorValue;
  }

  void Dump(std::string tag=""){
    fprintf(stderr, 
	    "E16ANA_G4Hit: %s : %6.1f %6.1f %6.1f /%6.3f %6.3f %6.3f\n",
	    tag.c_str(), x, y, z, gx, gy, gz);
    fprintf(stderr, 
	    "             (evid,t,track,det,pid): %5i %6.3f %5i %2i %4i %s\n",
	    eventID, t, trackID, detectorID, pID,
	    HepPID::particleName(pID).c_str() );
    fprintf(stderr, 
	    "              (p,dE,beta,flen   : %6.1f %6.1f %6.1f :%6.1f /%6.3f %5.3f %6.1f\n",
	    px, py, pz, p, dE, beta,flightLength );
  }

};

class E16ANA_G4HitSimple{
private:
  double x,y,z;    //local cooridnate in the detector
  double gx,gy,gz; //global cooridnate
  int t; //timing
public:
  E16ANA_G4HitSimple(){
    Clear();
  }
  enum{ErrorValue = -9999};
  void SetT(double it){ t=it;}
  void SetXYZ(double ix, double iy, double iz){
    x=ix; y=iy; z=iz;;
  }
  void SetGXYZ(double ix, double iy, double iz){
    gx=ix; gy=iy; gz=iz;
  }

  double X(){return x;}
  double Y(){return y;}
  double Z(){return z;}
  double Time(){return t;}
  double GX(){return gx;}
  double GY(){return gy;}
  double GZ(){return gz;}

  void Clear(){
    x = ErrorValue;    y = ErrorValue;    z = ErrorValue;
    gx= ErrorValue;    gy= ErrorValue;    gz= ErrorValue;
    t = ErrorValue;
  }
#if 0
  int WriteData(FILE* fp){
    return fwrite(this, sizeof(E16ANA_G4Hit),1,fp);
  }
  int ReadData(FILE* fp){
    return fread(this, sizeof(E16ANA_G4Hit),1,fp);
  }
#endif
  void Dump(std::string tag=""){
    fprintf(stderr, 
	    "E16ANA_G4Hit: %s : %6.1f %6.1f %6.1f /%6.1f %6.1f %6.1f %6i\n",
	    tag.c_str(), x, y, z, gx, gy, gz,t);
  }

};


template <class T> class E16ANA_G4Detector{ 
 // list of hit for each detector and item as target,frame, etc.
  // also list of track, trajectory, etc.

public:
  //def
  //  enum{TableSize=2048}; 
  //  enum{TableSize=3072}; //works
  enum{TableSize=3072, TableSizeTraj=100000}; //works
  //sako 2019/10/5 crashes
  //  enum{TableSize=204800}; 
  //  enum{TableSize=20480}; 
private:
  int noh;//number of hit
  T hits[TableSize];
  //  T* hits;

public:

  E16ANA_G4Detector(){
    //  hits = new T[TableSize];

    noh=0;
    for(int i=0;i<TableSize;i++){
      hits[i].Clear();
    }
  }
  ~E16ANA_G4Detector(){};

  T& Hit(int i){return hits[i];}
  void SetNoh(int i){noh=i;}
  int Noh(){return noh;}

  int WriteData(FILE* fp){
    int length= (noh<TableSize)? noh : TableSize;
    if( noh > TableSize ){      
      E16WARNING("TableSizeOver: noh=%d, shrinked to %d", noh,TableSize); 
    }
    int flag = fwrite(&length, sizeof(int), 1, fp);
    flag = fwrite(hits, sizeof(T), length, fp);
    return flag;
  }

  int ReadData(FILE* fp){
    //printf("E16ANA_G4OutputData.hh ReadData noh L292 before fread %d\n",noh);
    int flag = fread(&noh, sizeof(int), 1, fp);

    if( (uint)noh > TableSize ){
      E16WARNING("noh TableSizeOver: noh=%d", noh); 
      noh=TableSize;
    }

    flag = fread(hits, sizeof(T), noh, fp);
    for(int i=noh;i<TableSize;i++){
      hits[i].Clear();
    }
    return flag;
  }

  int HitsAppend(double timeoffset, E16ANA_G4Detector& det2, 
		 int eventIDoffset = 0){
    int noh3 = noh + det2.Noh();
    if (noh3 > TableSize){
      E16WARNING("TableSizeOver %5i %5i",noh, noh3);
      return 0;
    }
    for(int i=0; i< det2.Noh() ; i++ ){
      hits[noh+i]    = det2.Hit(i);
      double t = hits[noh+i].Time();
      hits[noh+i].SetT( t + timeoffset );
      hits[noh+i].EventIDputOffset( eventIDoffset );
    }
    noh = noh3;
    return noh3;
  }
  
};

#if 0
// to be removed soon  by S.Yokkaichi

class E16ANA_G4DecayChain{
public:
  enum{nameSize=32, ErrorValue = -99999};

private:
  int trackID;
  int pID;
  int parentID;//parent what?  track?pid?
  int GEN; //?
  double x_vtx;
  double y_vtx;
  double z_vtx;
  char    procName[nameSize];
  char     volName[nameSize];        // at where?
  char volName_vtx[nameSize];
public:
  E16ANA_G4DecayChain(){
    Clear();
  }
  void SetT(double it){ }//dummy
  double Time(){ return 0;}//dummy

  //  int EventID(){return eventID;}
  int EventID(){return 0;}
  void EventIDputOffset(int offset){;}
       //do nothing, for compatibility

  void Clear(){
    trackID = pID = parentID = GEN = ErrorValue; 
    x_vtx  = y_vtx = z_vtx = ErrorValue;
    memset(procName,    '0', nameSize );
    memset(volName,     '0', nameSize );
    memset(volName_vtx, '0', nameSize );
  }

};
#endif





class E16ANA_G4Event{
  //  List of track & hit
private:
  E16ANA_G4EventHeader eventHeader;
  E16ANA_G4Detector<E16ANA_G4Track> primaryTracks;

  E16ANA_G4Detector<E16ANA_G4Trajectory> primaryTrajectories;
  E16ANA_G4Detector<E16ANA_G4Trajectory> secondaryTrajectories;
  std::vector<E16ANA_G4Trajectory*> allTrajectories;

  E16ANA_G4Detector<E16ANA_G4Hit> target1;
  E16ANA_G4Detector<E16ANA_G4Hit> target2;
  E16ANA_G4Detector<E16ANA_G4Hit> target3;
  E16ANA_G4Detector<E16ANA_G4Hit> target4;
  E16ANA_G4Detector<E16ANA_G4Hit> target5;
  E16ANA_G4Detector<E16ANA_G4Hit> ssd;
  E16ANA_G4Detector<E16ANA_G4Hit> gtr1;
  E16ANA_G4Detector<E16ANA_G4Hit> gtr2;
  E16ANA_G4Detector<E16ANA_G4Hit> gtr3;
  E16ANA_G4Detector<E16ANA_G4Hit> hbd;
  //sako
  E16ANA_G4Detector<E16ANA_G4Hit> rpc;
  E16ANA_G4Detector<E16ANA_G4Hit> lg;
  E16ANA_G4Detector<E16ANA_G4Hit> lgvd;
  E16ANA_G4Detector<E16ANA_G4Hit> gtrframe1;
  E16ANA_G4Detector<E16ANA_G4Hit> gtrframe2;
  E16ANA_G4Detector<E16ANA_G4Hit> gtrframe3;
  E16ANA_G4Detector<E16ANA_G4Hit> vd1;          //virtual detector
  E16ANA_G4Detector<E16ANA_G4Hit> vdcyl1;       //virtual detector/cylindrical
  E16ANA_G4Detector<E16ANA_G4Hit> vdcyl2;
  E16ANA_G4Detector<E16ANA_G4Hit> vdcyl3;
  E16ANA_G4Detector<E16ANA_G4Hit> vdcyl4;

  E16ANA_G4Detector<E16ANA_G4Hit>* targets[5];
  E16ANA_G4Detector<E16ANA_G4Hit>* gtrs[3];
  E16ANA_G4Detector<E16ANA_G4Hit>* gtrframes[3];
  E16ANA_G4Detector<E16ANA_G4Hit>* vdcyls[4];


public:
  E16ANA_G4Event(){
    targets[0]=&target1;    targets[1]=&target2;
    targets[2]=&target3;    targets[3]=&target4;    targets[4]=&target5;

    gtrs[0]     =&gtr1;      gtrs[1]     =&gtr2;      gtrs[2]     =&gtr3;
    gtrframes[0]=&gtrframe1; gtrframes[1]=&gtrframe2; gtrframes[2]=&gtrframe3;
    vdcyls[0]= &vdcyl1;     vdcyls[1]= &vdcyl2;
    vdcyls[2]= &vdcyl3;     vdcyls[3]= &vdcyl4;

  }

  //setter & related ------------------------------
  int WriteData(FILE* fpWrite);
  int ReadData(FILE* fpRead);

  //old
  /*
  void SetHeader(int id1, int id2, int m){
    eventHeader.Set(id1, id2, m);
  }
  */

  void SetHeader(int id1, int id2, int m, double imp){
    eventHeader.Set(id1, id2, m, imp);
  }

  int EventAppend(double timeoffset,  E16ANA_G4Event& ev2, 
		  int eventIDoffset = 0);

  //getter & related ------------------------------
  void HeaderDump(){ eventHeader.Dump();}
  int G4EventID(){return eventHeader.EventID_G4();};
  int JAMEventID(){return eventHeader.EventID_JAM();};
  int InputMultiplicity(){return eventHeader.InputMultiplicity();};
  //sako 2019/11/18
  double JAMImpact(){return eventHeader.Impact();};


  //accesser
  E16ANA_G4Track&  PrimaryTracks(int i){
    return primaryTracks.Hit(i);};
  E16ANA_G4Detector<E16ANA_G4Track>&  PrimaryTracks(){
    return primaryTracks;
  };

  E16ANA_G4Trajectory&  PrimaryTrajectory(int i){
    return primaryTrajectories.Hit(i);};
  E16ANA_G4Detector<E16ANA_G4Trajectory>&  PrimaryTrajectories(){
    return primaryTrajectories;
  };
  E16ANA_G4Trajectory&  SecondaryTrajectory(int i){
    return secondaryTrajectories.Hit(i);};
  E16ANA_G4Detector<E16ANA_G4Trajectory>&  SecondaryTrajectories(){
    return secondaryTrajectories;
  };
  E16ANA_G4Trajectory*  AllTrajectory(int i){
    return allTrajectories[i];};
  std::vector<E16ANA_G4Trajectory*>&  AllTrajectories(){
    return allTrajectories;
  };

  E16ANA_G4Detector<E16ANA_G4Hit>&  TARGET(int i){return *(targets[i]);};
  E16ANA_G4Detector<E16ANA_G4Hit>&  SSD(){return ssd;};
  //sako
  E16ANA_G4Detector<E16ANA_G4Hit>&  RPC(){return rpc;};
  E16ANA_G4Detector<E16ANA_G4Hit>&  GTR1(){return gtr1;};
  E16ANA_G4Detector<E16ANA_G4Hit>&  GTR2(){return gtr2;};
  E16ANA_G4Detector<E16ANA_G4Hit>&  GTR3(){return gtr3;};
  E16ANA_G4Detector<E16ANA_G4Hit>&  GTRs(int i){return *(gtrs[i]);};
  E16ANA_G4Detector<E16ANA_G4Hit>&  HBD(){return hbd;};
  E16ANA_G4Detector<E16ANA_G4Hit>&  LG(){return lg;};
  E16ANA_G4Detector<E16ANA_G4Hit>&  LGVD(){return lgvd;};
  E16ANA_G4Detector<E16ANA_G4Hit>&  GTRFrame1(){return gtrframe1;};
  E16ANA_G4Detector<E16ANA_G4Hit>&  GTRFrame2(){return gtrframe2;};
  E16ANA_G4Detector<E16ANA_G4Hit>&  GTRFrame3(){return gtrframe3;};
  E16ANA_G4Detector<E16ANA_G4Hit>&  GTRFrames(int i){return *(gtrframes[i]);};
  E16ANA_G4Detector<E16ANA_G4Hit>&  VD1(){return vd1;};
  E16ANA_G4Detector<E16ANA_G4Hit>&  VDCyl1(){return vdcyl1;};
  E16ANA_G4Detector<E16ANA_G4Hit>&  VDCyl2(){return vdcyl2;};
  E16ANA_G4Detector<E16ANA_G4Hit>&  VDCyl3(){return vdcyl3;};
  E16ANA_G4Detector<E16ANA_G4Hit>&  VDCyl4(){return vdcyl4;};
  E16ANA_G4Detector<E16ANA_G4Hit>&  VDCylinders(int i){return *(vdcyls[i]);};
  
};

//----------------------------------------------
class E16ANA_G4OutputData{
public:
  E16ANA_G4OutputData();
  ~E16ANA_G4OutputData();

private:
  FILE* fpRead;
  FILE* fpWrite;
  E16ANA_G4Event  event;            //handle for current event in the data file
  E16ANA_G4Event* overlapEvent;     //handle for the overlapped event
  E16ANA_G4RunHeader run;

public:
  enum{OK=0, OpenERROR};
  int OpenReadFile(const char* filename);
  int OpenWriteFile(const char* filename);
  int CloseReadFile();
  int CloseWriteFile();

  E16ANA_G4RunHeader& RunData(){ return run;}

  E16ANA_G4Event& Event(){ return event;}
  E16ANA_G4Event& OverlapEvent(){ return *overlapEvent;}
  
  int WriteRunHeader(){
    return run.WriteData(fpWrite);
  }
  int ReadRunHeader(){
    return run.ReadData(fpRead);
  }
  int WriteAnEvent(){
    return event.WriteData(fpWrite);
  }
  int ReadAnEvent(){
    return event.ReadData(fpRead);
  }

  int ReadOverlapEvent(int multi, double* timeInterval, int jamInteractionFlag);


};


//----------------------------------------------

class E16ANA_G4ModuleHitList{ 
  // E16ANA_G4Event -> vector, by-module, by-chamber
  //independent on E16ANA_G4Event,  in order to avoid 
  //changing the file-IO structure

  //lpos, lmom, t
  //  vector<E16ANA_G4Hit>  gtr1List;
private:

  //org 
  //  enum{NmoduleGTR = 33, NmoduleSSD = 33};
  //sako
  enum{NmoduleGTR = 33, NmoduleSSD = 33, NmoduleRPC = 81};
  std::vector<E16ANA_G4Hit>  ssdList[NmoduleSSD];
  //sako
  std::vector<E16ANA_G4Hit>  rpcList[NmoduleRPC];
  std::vector<E16ANA_G4Hit>  gtr1List[33];
  std::vector<E16ANA_G4Hit>  gtr2List[33];
  std::vector<E16ANA_G4Hit>  gtr3List[33];

  std::vector<E16ANA_G4Hit>* gtr[3];
  void SetG4Event(E16ANA_G4Event& g4event);

  std::vector<E16ANA_G4Hit>  hbdList[33];
  std::vector<E16ANA_G4Hit>  lgvdList[33];

public:

  E16ANA_G4ModuleHitList(E16ANA_G4Event& g4event){
    gtr[0]=gtr1List;    gtr[1]=gtr2List;    gtr[2]=gtr3List;
    SetG4Event(g4event);
  }
  void OverlapGTRSingleHits();
  void OverlapGTRSingleHits151201();
  void OverlapSSDSingleHits();
  //sako
  //  void OverlapRPCSingleHits();

  std::vector<E16ANA_G4Hit>& SSDModule(int moduleID){
    return ssdList[moduleID];
  }
  //sako
  std::vector<E16ANA_G4Hit>& RPCModule(int moduleID){
    return rpcList[moduleID];
  }

  std::vector<E16ANA_G4Hit>& GTR1Module(int moduleID){
    return gtr1List[moduleID];}
  std::vector<E16ANA_G4Hit>& GTR2Module(int moduleID){
    return gtr2List[moduleID];}
  std::vector<E16ANA_G4Hit>& GTR3Module(int moduleID){
    return gtr3List[moduleID];}
  std::vector<E16ANA_G4Hit>& HBDModule(int moduleID){
    return hbdList[moduleID];}
  std::vector<E16ANA_G4Hit>& LGVDModule(int moduleID){
    return lgvdList[moduleID];}

  std::vector<E16ANA_G4Hit>& GTRModule(int layerID, int moduleID){
    return (gtr[layerID])[moduleID] ;}
    
  void HitPrint();
};

//----------------------------------------------

class E16ANA_Geometry;

class E16ANA_EventOverlap{
public:
  E16ANA_EventOverlap();
  ~E16ANA_EventOverlap(){}

  static int PoissonIntervalArray(
    double mean, int range, int start, double* array);
  static int RandomForwardHemisphere(double* x,
    double* y, double* z);
  static int RandomForwardAngleSphere(double* x,
    double* y, double* z, double polarAngleThr); //forward than the thr-angle

  static void SetGeometry(E16ANA_Geometry* igeom){
    geom = igeom;
  }

  static  void InsertSingleRateBKG(int flag, int layerID, int  moduleID, 
  double mean, double size, std::vector<E16ANA_G4Hit>& hits);

  static void InsertSingleRateBKGRandom(int layerID, int moduleID, 
    double meanbkg, double size, std::vector<E16ANA_G4Hit>& hits){
    int flag =0;
    InsertSingleRateBKG(flag, layerID, moduleID, meanbkg, size, hits);
  }

  static void InsertSingleRateBKGHalo(int layerID, int moduleID, 
    double meanhalo, double size, std::vector<E16ANA_G4Hit>& hits){
    int flag =1;
    InsertSingleRateBKG(flag, layerID, moduleID, meanhalo, size, hits);
  }
  

  static  void InsertSingleRateBKG100(int moduleID, 
    std::vector<E16ANA_G4Hit>& hits);
  static  void InsertSingleRateBKG200(int moduleID, 
    std::vector<E16ANA_G4Hit>& hits);
  static  void InsertSingleRateBKG300(int moduleID, 
    std::vector<E16ANA_G4Hit>& hits);

  static void SetSingleRate( double imean100, 
    double imean200, double imean300, double ibackwardRatio);
  static void SetSingleRateDefault();

  static void SetSingleRateDefaultAndReScale(double scale);
  //set default rate, and multiply common scale factor


  // SSD
  static void InsertSingleRateBKG_SSD(int moduleID, std::vector<E16ANA_G4Hit>& hits, int is_forward, double halo_angle);
  static void InsertSingleRateBKG_SSD(int flag, int moduleID, double rate, double size, std::vector<E16ANA_G4Hit>& hits, const TVector3 &halo_direction = TVector3(0,0,0));
  static void InsertSingleRateBKGHalo_SSD(int moduleID, double rate_halo, double size, std::vector<E16ANA_G4Hit>& hits, const TVector3 &halo_direction){
     int flag = 1;
     InsertSingleRateBKG_SSD(flag, moduleID, rate_halo, size, hits, halo_direction);
  };
  static void InsertSingleRateBKGRandom_SSD(int moduleID, double rate_bkg, double size, std::vector<E16ANA_G4Hit>& hits){
     int flag = 0;
     InsertSingleRateBKG_SSD(flag, moduleID, rate_bkg, size, hits);
  };

  //in order to use the old set
  //, in E16ANA_G4ModuleHitList::OverlapGTRSingleHits151201();
  static void SetSingleRate151201();
  static  void InsertSingleRateBKG151201(double moduleID, 
    double mean, double size, std::vector<E16ANA_G4Hit>& hits);
  static  void InsertSingleRateBKG100_151201(int moduleID, 
    std::vector<E16ANA_G4Hit>& hits);
  static  void InsertSingleRateBKG200_151201(int moduleID, 
    std::vector<E16ANA_G4Hit>& hits);
  static  void InsertSingleRateBKG300_151201(int moduleID, 
    std::vector<E16ANA_G4Hit>& hits);

private:
  static E16ANA_Geometry* geom;

  static double mean100;
  static double mean200;
  static double mean300;
  static double halo100;
  static double backwardRatio100;
  static double backwardRatio200;
  static double backwardRatio300;

  static double halo_rate_ssd; // [kHz/mm^2]
  static double random_rate_ssd; // [kHz/mm^2]

};


#endif //E16ANA_G4OutputData_HH
