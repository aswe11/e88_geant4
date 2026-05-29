//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai
//2015-12-12, uploaded by nakai
//2015-11-02, uploaded by yokkaich
//2015-11-02, uploaded by komatsu
//2015-08-20, uploaded by yokkaich
//2015-03-28, uploaded by yokkaich
//2015-03-27, uploaded by yokkaich
//2015-01-28, uploaded by komatsu
//2015-01-05, modified by yokkaich
//2015-01-05, modified by yokkaich
//2014-08-27, modified by kawama
//2014-05-07, modified by kawama
//2014-04-30, modified by kawama
//2014-04-24, modified by kawama
//2013-11-14, modified by kawama
//2013-05-13, modified by kawama
/*
//E16ANA_MagneticFieldMap.cc 150104 by S. Yokkaichi
//    Last modified at <>


  Field Map Manager for Tosca Fields

*/


#include <stdlib.h>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <CLHEP/Vector/ThreeVector.h>
#include <CLHEP/Vector/LorentzVector.h>
#include <TRandom3.h>
#include "E16ANA_MagneticFieldMap.hh"
#include "E16ANA_ErrorMessage.hh"

using namespace CLHEP;
using namespace std;

const double Deg2Rad = M_PI/180.;
const double Rad2Deg = 180./M_PI;

const int MaxChar = 1024;


void E16ANA_MagneticFieldMap3D::SetSmearParameters(){
   smear_bx = 0.00; // percent
   smear_by = 0.05; // percent
   smear_bz = 0.00; // percent

   shift_bx[0] =  0.00; // percent
   shift_bx[1] =  0.00; // percent
   shift_bx[2] =  0.00; // percent
   shift_bx[3] =  0.00; // percent
   shift_bx[4] =  0.00; // percent
   shift_bx[5] =  0.00; // percent

   shift_by[0] =  0.05; // percent
   shift_by[1] = -0.05; // percent
   shift_by[2] =  0.05; // percent
   shift_by[3] = -0.05; // percent
   shift_by[4] =  0.05; // percent
   shift_by[5] = -0.05; // percent

   shift_bz[0] =  0.00; // percent
   shift_bz[1] =  0.00; // percent
   shift_bz[2] =  0.00; // percent
   shift_bz[3] =  0.00; // percent
   shift_bz[4] =  0.00; // percent
   shift_bz[5] =  0.00; // percent
}

void E16ANA_MagneticFieldMap3D::SetSmearParameters(double smear){
   //smear_bx = 0.00; // percent
   smear_by = fabs(smear); // percent
   //smear_bz = 0.00; // percent

   //shift_bx[0] =  0.00; // percent
   //shift_bx[1] =  0.00; // percent
   //shift_bx[2] =  0.00; // percent
   //shift_bx[3] =  0.00; // percent
   //shift_bx[4] =  0.00; // percent
   //shift_bx[5] =  0.00; // percent

   shift_by[0] =  smear; // percent
   shift_by[1] = -smear; // percent
   shift_by[2] =  smear; // percent
   shift_by[3] = -smear; // percent
   shift_by[4] =  smear; // percent
   shift_by[5] = -smear; // percent

   //shift_bz[0] =  0.00; // percent
   //shift_bz[1] =  0.00; // percent
   //shift_bz[2] =  0.00; // percent
   //shift_bz[3] =  0.00; // percent
   //shift_bz[4] =  0.00; // percent
   //shift_bz[5] =  0.00; // percent

}

int E16ANA_MagneticFieldMap3D::RandomSmear(int seed){
   TRandom3 rand(seed);
   int ncall = 0;
   std::cout << "Smear = (" << smear_bx
      << ", " << smear_by
      << ", " << smear_bz
      << ")" << std::endl;
   for(int i=0; i<Nx; i++){
      for(int j=0; j<Ny; j++){
         for(int k=0; k<Nz; k++){
            //if(i==Nx/2 && j==Ny/2 && k==Nz/2){
            //std::cout << "before " << B[i][j][k].y << std::endl;
            //}
            B[i][j][k].x =B[i][j][k].x*rand.Gaus(1.0, smear_bx/100.0);
            B[i][j][k].y =B[i][j][k].y*rand.Gaus(1.0, smear_by/100.0);
            B[i][j][k].z =B[i][j][k].z*rand.Gaus(1.0, smear_bz/100.0);
            //if(i==Nx/2 && j==Ny/2 && k==Nz/2){
            //std::cout << "after " << B[i][j][k].y << std::endl;
            //}
            ncall++;
         }
      }
   }
   return ncall;
}

int E16ANA_MagneticFieldMap3D::AddShift(){
   int ncall = 0;
   for(int i=0; i<Nx; i++){
      for(int j=0; j<Ny; j++){
         for(int k=0; k<Nz; k++){
            int r = GetRegionId(i*dX+X0, j*dY+Y0, k*dZ+Z0);
            B[i][j][k].x *= (1.0+shift_bx[r]/100.0);
            B[i][j][k].y *= (1.0+shift_by[r]/100.0);
            B[i][j][k].z *= (1.0+shift_bz[r]/100.0);
            ncall++;
         }
      }
   }
   return ncall;
}

int E16ANA_MagneticFieldMap3D::WriteBinaryFile(const std::string &file_name){
#ifdef  USE_ZLIB
  gzFile fp2;
  if( (fp2=gzopen( file_name.c_str(),"wb1"))==0 ){
    E16FATAL("writing open fail  file  %s ", file_name.c_str() );
    exit(-1);
  }
  
#else
  FILE *fp2;
  if( (fp2=fopen( file_name.c_str(),"wb"))==0 ){
    E16FATAL("writing open fail  file  %s ", file_name.c_str() );
    exit(-1);
  }
#endif

  int buf1[3];
  double buf2[6];
  buf1[0]=Nx;  buf1[1]=Ny;  buf1[2]=Nz;
  buf2[0]=X0;  buf2[1]=Y0;  buf2[2]=Z0;
  buf2[3]=dX;  buf2[4]=dY;  buf2[5]=dZ;

#ifdef USE_ZLIB
  int n1=gzwrite(fp2, buf1, 3*sizeof(int) );
  int n2=gzwrite(fp2, buf2, 6*sizeof(double) );
  if( n1 != 12 || n2 != 48 ) {
    E16FATAL("write error  %d %d ", n1, n2);
    //    cerr << "[" << funcname << "]: write error"<< n1 <<" "<<n2<<endl;
    exit(-1);
  }
#else
  int n1=fwrite(buf1, sizeof(int)    ,3, fp2 );
  int n2=fwrite(buf2, sizeof(double) ,6, fp2 );
  if( n1 != 3 || n2 != 6 ) {
    E16FATAL("write error  %d %d ", n1, n2);
    exit(-1);
  }
#endif

   int ncall = 0;
   double buf3[24];
   for(int i=0; i<Nx; i++){
      for(int j=0; j<Ny; j++){
         for(int k=0; k<Nz; k++){
            double x = i*dX+X0;
            double y = j*dY+Y0;
            double z = k*dZ+Z0;
            buf3[0]=x;                  buf3[1]=y;                  buf3[2]=z;
            buf3[3]=B[i][j][k].x;       buf3[4]=B[i][j][k].y;       buf3[5]=B[i][j][k].z;
            buf3[6]=B_x[i][j][k].x;     buf3[7]=B_x[i][j][k].y;     buf3[8]=B_x[i][j][k].z;
            buf3[9]=B_y[i][j][k].x;     buf3[10]=B_y[i][j][k].y;    buf3[11]=B_y[i][j][k].z;
            buf3[12]=B_z[i][j][k].x;    buf3[13]=B_z[i][j][k].y;    buf3[14]=B_z[i][j][k].z;
            buf3[15]=B_xy[i][j][k].x;   buf3[16]=B_xy[i][j][k].y;   buf3[17]=B_xy[i][j][k].z;
            buf3[18]=B_yz[i][j][k].x;   buf3[19]=B_yz[i][j][k].y;   buf3[20]=B_yz[i][j][k].z;
            buf3[21]=B_zx[i][j][k].x;   buf3[22]=B_zx[i][j][k].y;   buf3[23]=B_zx[i][j][k].z;
#ifdef USE_ZLIB
            gzwrite(fp2, buf3, sizeof(double)*24);
#else
            fwrite(buf3, sizeof(double), 24, fp2);
#endif
            ncall++;
         }
      }
   }

#ifdef USE_ZLIB
   gzclose(fp2);
#else
   fclose(fp2);
#endif
   return ncall;
}

///////////////////////////////////
int E16ANA_MagneticFieldMap3D::Initialize_binary( const char* filename)
///////////////////////////////////
{

  char str[MaxChar];
  double x, y, z, bx, by, bz;
  double bxx, bxy, bxz;
  double byx, byy, byz;
  double bzx, bzy, bzz;
  double bxyx, bxyy, bxyz;
  double byzx, byzy, byzz;
  double bzxx, bzxy, bzxz;
  int ncall=0;

#undef USE_ZLIB
#ifdef  USE_ZLIB
  gzFile fp;
  if( (fp=gzopen( filename,"rb"))==0 ){
    E16FATAL("open fail gz map file: %s",filename);
    exit(-1);
  }
#else
  FILE *fp;
  if( (fp=fopen( filename,"rb"))==0 ){
    E16FATAL("open fail text map file: %s",filename);
    exit(-1);
  }
#endif

  cleanupMap();


  int buf1[3];
  double buf2[6];

#ifdef  USE_ZLIB
  int n1=gzread(fp, buf1, sizeof(int)*3 );
  int n2=gzread(fp, buf2, sizeof(double)*6 );
  if( n1 != 12 || n2 != 48 ) {
    E16FATAL("Invalid format %d %d",n1, n2);
    //    cerr << "[" << funcname << "]: Invalid format"<< n1 <<" "<<n2<<endl;
    exit(-1);
  }
#else
  int n1=fread(buf1, sizeof(int)    ,3,fp );
  int n2=fread(buf2, sizeof(double) ,6,fp );
  if( n1 != 3 || n2 != 6 ) {
    E16FATAL("Invalid format %d %d",n1, n2);
    exit(-1);
  }
#endif

  Nx = buf1[0];  Ny = buf1[1];  Nz = buf1[2];
  X0 = buf2[0];  Y0 = buf2[1];  Z0 = buf2[2];
  dX = buf2[3];  dY = buf2[4];  dZ = buf2[5];
  
#if 0
  cerr<<"magnet bin init "<<Nx<<" "<<Ny <<" "<<Nz<<endl;
  cerr<<"                "<<X0<<" "<<Y0 <<" "<<Z0<<endl;
  cerr<<"                "<<dX<<" "<<dY <<" "<<dZ<<endl;
#else
  E16INFO("3D magnet map using binary file");
#endif

  B.resize(Nx);
  B_x.resize(Nx);
  B_y.resize(Nx);
  B_z.resize(Nx);
  B_xy.resize(Nx);
  B_yz.resize(Nx);
  B_zx.resize(Nx);
  for( int ix=0; ix<Nx; ++ix ){
    B[ix].resize(Ny);
    B_x[ix].resize(Ny);
    B_y[ix].resize(Ny);
    B_z[ix].resize(Ny);
    B_xy[ix].resize(Ny);
    B_yz[ix].resize(Ny);
    B_zx[ix].resize(Ny);
    for( int iy=0; iy<Ny; ++iy ){
      B[ix][iy].resize(Nz);
      B_x[ix][iy].resize(Nz);
      B_y[ix][iy].resize(Nz);
      B_z[ix][iy].resize(Nz);
      B_xy[ix][iy].resize(Nz);
      B_yz[ix][iy].resize(Nz);
      B_zx[ix][iy].resize(Nz);
    }
  }

  double buf3[24];
#ifdef USE_ZLIB
  while( gzread(fp, buf3, sizeof(double)*24 ) == 24*8 ){
#else
  while( fread(buf3, sizeof(double), 24, fp) == 24){
#endif
         x=buf3[ 0];    y=buf3[ 1];    z=buf3[ 2]; 
        bx=buf3[ 3];   by=buf3[ 4];   bz=buf3[ 5];
       bxx=buf3[ 6];  bxy=buf3[ 7];  bxz=buf3[ 8];
       byx=buf3[ 9];  byy=buf3[10];  byz=buf3[11];
       bzx=buf3[12];  bzy=buf3[13];  bzz=buf3[14];
      bxyx=buf3[15]; bxyy=buf3[16]; bxyz=buf3[17];
      byzx=buf3[18]; byzy=buf3[19]; byzz=buf3[20];
      bzxx=buf3[21]; bzxy=buf3[22]; bzxz=buf3[23];


	int ix = int((x-X0+0.5*dX)/dX);
	int iy = int((y-Y0+0.5*dY)/dY);
	int iz = int((z-Z0+0.5*dZ)/dZ);
	if( ix<0 || ix>=Nx || iy<0 || iy>=Ny || iz<0 || iz>=Nz ){
	  E16WARNING("Invalid Range at ncall %d %s",ncall, str);
	  //	  string mes=str;
	  //	  cerr << "[" << funcname << "]: Invalid Range at ncall "<<ncall<<endl;
	  //	  cerr << mes << endl;
	  cerr << x  << "," << y << "," << z  << endl;
	  cerr << ix  << "," << iy << "," << iz  << endl;
  	  cerr << Ny  << "," << dY << ","  << Y0  << endl;
	}
	else{
	  B[ix][iy][iz].x = bx;
	  B[ix][iy][iz].y = by;
	  B[ix][iy][iz].z = bz;
	  B_x[ix][iy][iz].x = bxx;
	  B_x[ix][iy][iz].y = bxy;
	  B_x[ix][iy][iz].z = bxz;
	  B_y[ix][iy][iz].x = byx;
	  B_y[ix][iy][iz].y = byy;
	  B_y[ix][iy][iz].z = byz;
	  B_z[ix][iy][iz].x = bzx;
	  B_z[ix][iy][iz].y = bzy;
	  B_z[ix][iy][iz].z = bzz;
	  B_xy[ix][iy][iz].x = bxyx;
	  B_xy[ix][iy][iz].y = bxyy;
	  B_xy[ix][iy][iz].z = bxyz;
	  B_yz[ix][iy][iz].x = byzx;
	  B_yz[ix][iy][iz].y = byzy;
	  B_yz[ix][iy][iz].z = byzz;
	  B_zx[ix][iy][iz].x = bzxx;
	  B_zx[ix][iy][iz].y = bzxy;
	  B_zx[ix][iy][iz].z = bzxz;
	  /*double dB=0.;
         B[ix][iy][iz].x = G4RandGauss::shoot(bx, dB);
	  B[ix][iy][iz].y = G4RandGauss::shoot(by, dB);
	  B[ix][iy][iz].z = G4RandGauss::shoot(bz, dB);*/

	}//invalid range

	ncall++; 
  }     /* while( fread ) */

#ifdef USE_ZLIB
  gzclose(fp);
#else
  fclose(fp);
#endif

  return ncall;
  }
////////////////////////////////
  int E16ANA_MagneticFieldMap3D::Initialize_shift(Hep3Vector shiftV){
    int n = Initialize_binary();
    X0= X0+shiftV.x();
    Y0= Y0+shiftV.y();
    Z0= Z0+shiftV.z();
    return n;
  }
///////////////////////////////////
int E16ANA_MagneticFieldMap3D::Convert( char* filename)
///////////////////////////////////
{

  FILE *fp;
  char str[MaxChar];
  double x, y, z, bx, by, bz;
  double bxx, bxy, bxz;
  double byx, byy, byz;
  double bzx, bzy, bzz;
  double bxyx, bxyy, bxyz;
  double byzx, byzy, byzz;
  double bzxx, bzxy, bzxz;
  int ncall=0;

  if( (fp=fopen(FieldMapFileName.c_str(),"r"))==0 ){
    E16FATAL("open fail  file  %s ", FieldMapFileName.c_str() );
    exit(-1);
  }
  cleanupMap();
  
  if( fgets(str,MaxChar,fp)==0 ){
    E16FATAL("Error in reading file  %s ", FieldMapFileName.c_str() );
    fclose(fp);
    exit(-1);
  }    
  else if( sscanf(str,"%d %d %d %lf %lf %lf %lf %lf %lf",&Nx,&Ny,&Nz,
	     &X0,&Y0,&Z0,&dX,&dY,&dZ)!=9 ){

    E16FATAL("Invalid format [0] ==> %s ", str );
    fclose(fp);
    exit(-1);
  }

  //yok-------------------
#undef USE_ZLIB

#ifdef  USE_ZLIB
  gzFile fp2;
  if( (fp2=gzopen( filename,"wb1"))==0 ){
    E16FATAL("writing open fail  file  %s ", FieldMapFileName.c_str() );
    exit(-1);
  }
  
#else
  FILE *fp2;
  if( (fp2=fopen( filename,"wb"))==0 ){
    E16FATAL("writing open fail  file  %s ", FieldMapFileName.c_str() );
    exit(-1);
  }
#endif

  int buf1[3];
  double buf2[6];
  buf1[0]=Nx;  buf1[1]=Ny;  buf1[2]=Nz;
  buf2[0]=X0;  buf2[1]=Y0;  buf2[2]=Z0;
  buf2[3]=dX;  buf2[4]=dY;  buf2[5]=dZ;

#ifdef USE_ZLIB
  int n1=gzwrite(fp2, buf1, 3*sizeof(int) );
  int n2=gzwrite(fp2, buf2, 6*sizeof(double) );
  if( n1 != 12 || n2 != 48 ) {
    E16FATAL("write error  %d %d ", n1, n2);
    //    cerr << "[" << funcname << "]: write error"<< n1 <<" "<<n2<<endl;
    exit(-1);
  }
#else
  int n1=fwrite(buf1, sizeof(int)    ,3, fp2 );
  int n2=fwrite(buf2, sizeof(double) ,6, fp2 );
  if( n1 != 3 || n2 != 6 ) {
    E16FATAL("write error  %d %d ", n1, n2);
    exit(-1);
  }
#endif

  //yok-------------------

  B.resize(Nx);
  B_x.resize(Nx);
  B_y.resize(Nx);
  B_z.resize(Nx);
  B_xy.resize(Nx);
  B_yz.resize(Nx);
  B_zx.resize(Nx);
  for( int ix=0; ix<Nx; ++ix ){
    B[ix].resize(Ny);
    B_x[ix].resize(Ny);
    B_y[ix].resize(Ny);
    B_z[ix].resize(Ny);
    B_xy[ix].resize(Ny);
    B_yz[ix].resize(Ny);
    B_zx[ix].resize(Ny);
    for( int iy=0; iy<Ny; ++iy ){
      B[ix][iy].resize(Nz);
      B_x[ix][iy].resize(Nz);
      B_y[ix][iy].resize(Nz);
      B_z[ix][iy].resize(Nz);
      B_xy[ix][iy].resize(Nz);
      B_yz[ix][iy].resize(Nz);
      B_zx[ix][iy].resize(Nz);
    }
  }


  double buf3[24]={0};
  while( fgets(str,MaxChar,fp)!=0){
    if(str[0]!='#'){
      if( sscanf(str,"%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
		 &x,&y,&z,&bx,&by,&bz
                 ,&bxx,&bxy,&bxz
                 ,&byx,&byy,&byz
                 ,&bzx,&bzy,&bzz
                 ,&bxyx,&bxyy,&bxyz
                 ,&byzx,&byzy,&byzz
                 ,&bzxx,&bzxy,&bzxz
                 ) ==24 ){

	buf3[0]=x;	buf3[1]=y;   buf3[2]=z;
	buf3[3]=bx;	buf3[4]=by;   buf3[5]=bz;
	buf3[6]=bxx;	buf3[7]=bxy;   buf3[8]=bxz;
	buf3[9]=byx;	buf3[10]=byy;   buf3[11]=byz;
	buf3[12]=bzx;	buf3[13]=bzy;   buf3[14]=bzz;
	buf3[15]=bxyx;	buf3[16]=bxyy;   buf3[17]=bxyz;
	buf3[18]=byzx;	buf3[19]=byzy;   buf3[20]=byzz;
	buf3[21]=bzxx;	buf3[22]=bzxy;   buf3[23]=bzxz;

#ifdef USE_ZLIB
	gzwrite(fp2, buf3, sizeof(double)*24);
#else
	fwrite(buf3, sizeof(double), 24, fp2);
#endif
	int ix = int((x-X0+0.5*dX)/dX);
	int iy = int((y-Y0+0.5*dY)/dY);
	int iz = int((z-Z0+0.5*dZ)/dZ);
	if( ix<0 || ix>=Nx || iy<0 || iy>=Ny || iz<0 || iz>=Nz ){
	  E16WARNING("Invalid Range at ncall %d %s",ncall,str);
	  //	  string mes=str;
	  //	  cerr << "[" << funcname << "]: Invalid Range at ncall "<<ncall<< endl;
	  //	  cerr << mes << endl;

	  cout << ix  << "," << iy << ","
	       << iz  << endl;
//  	  cout << Ny  << "," << dY << ","
// 		    << Y0  << std::endl;

	}
	else{
	  B[ix][iy][iz].x = bx;
	  B[ix][iy][iz].y = by;
	  B[ix][iy][iz].z = bz;
	  B_x[ix][iy][iz].x = bxx;
	  B_x[ix][iy][iz].y = bxy;
	  B_x[ix][iy][iz].z = bxz;
	  B_y[ix][iy][iz].x = byx;
	  B_y[ix][iy][iz].y = byy;
	  B_y[ix][iy][iz].z = byz;
	  B_z[ix][iy][iz].x = bzx;
	  B_z[ix][iy][iz].y = bzy;
	  B_z[ix][iy][iz].z = bzz;
	  B_xy[ix][iy][iz].x = bxyx;
	  B_xy[ix][iy][iz].y = bxyy;
	  B_xy[ix][iy][iz].z = bxyz;
	  B_yz[ix][iy][iz].x = byzx;
	  B_yz[ix][iy][iz].y = byzy;
	  B_yz[ix][iy][iz].z = byzz;
	  B_zx[ix][iy][iz].x = bzxx;
	  B_zx[ix][iy][iz].y = bzxy;
	  B_zx[ix][iy][iz].z = bzxz;
	  /*double dB=0.;
         B[ix][iy][iz].x = G4RandGauss::shoot(bx, dB);
	  B[ix][iy][iz].y = G4RandGauss::shoot(by, dB);
	  B[ix][iy][iz].z = G4RandGauss::shoot(bz, dB);*/
	}
      }
      else {
	E16WARNING("Invalid format [1] ==> %s ", str );
      }	/* if( sscanf... ) */

    }   /* if(str[0]...) */
    ncall++; 
  }     /* while( fgets... ) */

  fclose(fp);

#ifdef USE_ZLIB
  gzclose(fp2);
#else
  fclose(fp2);
#endif

  return ncall;
}

 
///////////////////////////////////
int E16ANA_MagneticFieldMap3D::Initialize( void )
///////////////////////////////////
{

  FILE *fp;
  char str[MaxChar];
  double x, y, z, bx, by, bz;
  double bxx, bxy, bxz;
  double byx, byy, byz;
  double bzx, bzy, bzz;
  double bxyx, bxyy, bxyz;
  double byzx, byzy, byzz;
  double bzxx, bzxy, bzxz;
  int ncall=0;

  if( (fp=fopen(FieldMapFileName.c_str(),"r"))==0 ){
    E16FATAL("open fail  file  %s ", FieldMapFileName.c_str()  );
    //    cerr << "[" << funcname << "]: file open fail" << endl;
    exit(-1);
  }
  cleanupMap();
  
  if( fgets(str,MaxChar,fp)==0 ){
    E16FATAL("Error in reading file  %s ", FieldMapFileName.c_str() );
    fclose(fp);
    exit(-1);
  }    
  else if( sscanf(str,"%d %d %d %lf %lf %lf %lf %lf %lf",&Nx,&Ny,&Nz,
	     &X0,&Y0,&Z0,&dX,&dY,&dZ)!=9 ){
    E16FATAL("Invalid format [0] ==> %s ", str );
    //    string mes=str;
    //    cerr << "[" << funcname << "]: Invalid format [0] ==> "<< mes << endl;

    fclose(fp);
    exit(-1);
  }

  B.resize(Nx);
  B_x.resize(Nx);
  B_y.resize(Nx);
  B_z.resize(Nx);
  B_xy.resize(Nx);
  B_yz.resize(Nx);
  B_zx.resize(Nx);
  for( int ix=0; ix<Nx; ++ix ){
    B[ix].resize(Ny);
    B_x[ix].resize(Ny);
    B_y[ix].resize(Ny);
    B_z[ix].resize(Ny);
    B_xy[ix].resize(Ny);
    B_yz[ix].resize(Ny);
    B_zx[ix].resize(Ny);
    for( int iy=0; iy<Ny; ++iy ){
      B[ix][iy].resize(Nz);
      B_x[ix][iy].resize(Nz);
      B_y[ix][iy].resize(Nz);
      B_z[ix][iy].resize(Nz);
      B_xy[ix][iy].resize(Nz);
      B_yz[ix][iy].resize(Nz);
      B_zx[ix][iy].resize(Nz);
    }
  }

  while( fgets(str,MaxChar,fp)!=0){
    if(str[0]!='#'){
      if( sscanf(str,"%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
		 &x,&y,&z,&bx,&by,&bz
                 ,&bxx,&bxy,&bxz
                 ,&byx,&byy,&byz
                 ,&bzx,&bzy,&bzz
                 ,&bxyx,&bxyy,&bxyz
                 ,&byzx,&byzy,&byzz
                 ,&bzxx,&bzxy,&bzxz
                 ) ==24 ){
	int ix = int((x-X0+0.5*dX)/dX);
	int iy = int((y-Y0+0.5*dY)/dY);
	int iz = int((z-Z0+0.5*dZ)/dZ);
	if( ix<0 || ix>=Nx || iy<0 || iy>=Ny || iz<0 || iz>=Nz ){
	  E16WARNING("Invalid Range ==> %s ", str );
	  //	  string mes=str;
	  //	  cerr << "[" << funcname << "]: Invalid Range ==> " << endl;
	  //	  cerr << mes << endl;
	  cout << ix  << "," << iy << ","
	       << iz  << endl;
//  	  cout << Ny  << "," << dY << ","
// 		    << Y0  << std::endl;

	}
	else{
	  B[ix][iy][iz].x = bx;
	  B[ix][iy][iz].y = by;
	  B[ix][iy][iz].z = bz;
	  B_x[ix][iy][iz].x = bxx;
	  B_x[ix][iy][iz].y = bxy;
	  B_x[ix][iy][iz].z = bxz;
	  B_y[ix][iy][iz].x = byx;
	  B_y[ix][iy][iz].y = byy;
	  B_y[ix][iy][iz].z = byz;
	  B_z[ix][iy][iz].x = bzx;
	  B_z[ix][iy][iz].y = bzy;
	  B_z[ix][iy][iz].z = bzz;
	  B_xy[ix][iy][iz].x = bxyx;
	  B_xy[ix][iy][iz].y = bxyy;
	  B_xy[ix][iy][iz].z = bxyz;
	  B_yz[ix][iy][iz].x = byzx;
	  B_yz[ix][iy][iz].y = byzy;
	  B_yz[ix][iy][iz].z = byzz;
	  B_zx[ix][iy][iz].x = bzxx;
	  B_zx[ix][iy][iz].y = bzxy;
	  B_zx[ix][iy][iz].z = bzxz;
	  /*double dB=0.;
         B[ix][iy][iz].x = G4RandGauss::shoot(bx, dB);
	  B[ix][iy][iz].y = G4RandGauss::shoot(by, dB);
	  B[ix][iy][iz].z = G4RandGauss::shoot(bz, dB);*/
	}
      }
      else {
	E16WARNING("Invalid format [1] ==> %s ", str );
	//	string mes=str;
	//	cerr << "[" << funcname << "]: Invalid format [1] ==> "<< mes << endl;
      }	/* if( sscanf... ) */

    }   /* if(str[0]...) */
    ncall++; 
  }     /* while( fgets... ) */

  fclose(fp);

  return ncall;
}

////////////////////////////////////////////////////////////////////////////
bool E16ANA_MagneticFieldMap3D::GetFieldValue( const double point[3], double *Bfield, int bcflag ) const
////////////////////////////////////////////////////////////////////////////
{

   double xt = point[0];
   double yt = point[1];
   double zt = point[2];
   double Bx=0,By=0,Bz=0;
   Bfield[0]=0; Bfield[1]=0; Bfield[2]=0; 

   int ix1, iy1, iz1, ix2, iy2, iz2;
   ix1=int( (xt-X0)/dX );
   iy1=int( (yt-Y0)/dY );
   iz1=int( (zt-Z0)/dZ );

   double wx1, wx2, wy1, wy2, wz1, wz2;
   if( ix1<0 ) {
      ix1=ix2=0; wx1=1.; wx2=0.;
   }
   else if( ix1>=Nx-1 ) {
      ix1=ix2=Nx-1; wx1=1.; wx2=0.;
   }
   else {
      ix2=ix1+1; wx1=(X0+dX*ix2-xt)/dX; wx2=1.-wx1;
   }
   if( iy1<0 ) {
      iy1=iy2=0; wy1=1.; wy2=0.;
   }
   else if( iy1>=Ny-1 ) {
      iy1=iy2=Ny-1; wy1=1.; wy2=0.;
   }
   else {
      iy2=iy1+1; wy1=(Y0+dY*iy2-yt)/dY; wy2=1.-wy1;
   }
   if( iz1<0 ) {
      iz1=iz2=0; wz1=1.; wz2=0.;
   }
   else if( iz1>=Nz-1 ) {
      iz1=iz2=Nz-1; wz1=1.; wz2=0.;
   }
   else {
      iz2=iz1+1; wz1=(Z0+dZ*iz2-zt)/dZ; wz2=1.-wz1;
   }

   //float *b, *b1, *b2, *b12;
   float B0x[4], B1x[4], B2x[4], B12x[4],dB0x[4];
   float ansBx, ansB1x, ansB2x;
   float B0y[4], B1y[4], B2y[4], B12y[4],dB0y[4];
   float ansBy, ansB1y, ansB2y;
   float B0z[4], B1z[4], B2z[4], B12z[4],dB0z[4];
   float ansBz, ansB1z, ansB2z;
   float buf0[4][6];
   float buf1[4][6];
   float buf2[4][6];
   float yy[6]={float( Y0+dY*(iy1-2) ),
		float( Y0+dY*(iy1-1) ),
		float( Y0+dY*(iy1) ),
                float( Y0+dY*(iy2) ),
		float( Y0+dY*(iy2+1) ),
		float( Y0+dY*(iy2+2) )     };

   int ixx[4]={ix1,ix2,ix2,ix1};
   int izz[4]={iz1,iz1,iz2,iz2};
   float xl=X0+dX*(ix1);
   float xu=X0+dX*(ix2);
   float zl=Z0+dZ*(iz1);
   float zu=Z0+dZ*(iz2);
   if (bcflag==1 && ix1>0 && iy1>2 && iz1>0 && ix2<Nx-1 && iy2<Ny-3 && iz2<Ny-1){
      for (int j=0;j<6;j++){
         for (int i=0;i<4;i++){
            buf0[i][j]=B[ixx[i]][iy1-2+j][izz[i]].x;
            buf1[i][j]=B[ixx[i]][iy1-2+j][izz[i]].y;
            buf2[i][j]=B[ixx[i]][iy1-2+j][izz[i]].z;
         }
      }
      
      for (int i=0;i<4;i++){
         polint(yy,buf0[i],6,yt,&B0x[i],&dB0x[i]); //need more than 6 points
         B1x[i]=wy1*B_x[ixx[i]][iy1][izz[i]].x+wy2*B_x[ixx[i]][iy2][izz[i]    ].x;
         B2x[i]=wy1*B_z[ixx[i]][iy1][izz[i]].x+wy2*B_z[ixx[i]][iy2][izz[i]    ].x;
         B12x[i]=wy1*B_zx[ixx[i]][iy1][izz[i]].x+wy2*B_zx[ixx[i]][iy2][izz    [i]].x;
         
         polint(yy,buf1[i],6,yt,&B0y[i],&dB0y[i]);
         B1y[i]=wy1*B_x[ixx[i]][iy1][izz[i]].y+wy2*B_x[ixx[i]][iy2][izz[i]    ].y;
         B2y[i]=wy1*B_z[ixx[i]][iy1][izz[i]].y+wy2*B_z[ixx[i]][iy2][izz[i]    ].y;
         B12y[i]=wy1*B_zx[ixx[i]][iy1][izz[i]].y+wy2*B_zx[ixx[i]][iy2][izz    [i]].y;
         
         polint(yy,buf2[i],6,yt,&B0z[i],&dB0z[i]);
         B1z[i]=wy1*B_x[ixx[i]][iy1][izz[i]].z+wy2*B_x[ixx[i]][iy2][izz[i]    ].z;
         B2z[i]=wy1*B_z[ixx[i]][iy1][izz[i]].z+wy2*B_z[ixx[i]][iy2][izz[i]    ].z;
         B12z[i]=wy1*B_zx[ixx[i]][iy1][izz[i]].z+wy2*B_zx[ixx[i]][iy2][izz    [i]].z;
      }
      bcuint(B0x, B1x, B2x, B12x, xl, xu, zl, zu, xt, zt,
            &ansBx, &ansB1x, &ansB2x);
      Bx=ansBx;
      bcuint(B0y, B1y, B2y, B12y, xl, xu, zl, zu, xt, zt,
            &ansBy, &ansB1y, &ansB2y);
      By=ansBy;
      bcuint(B0z, B1z, B2z, B12z, xl, xu, zl, zu, xt, zt,
            &ansBz, &ansB1z, &ansB2z);
      Bz=ansBz;
   }
   else{
      double by1=wx1*wy1*B[ix1][iy1][iz1].y+wx1*wy2*B[ix1][iy2][iz1].y
         +wx2*wy1*B[ix2][iy1][iz1].y+wx2*wy2*B[ix2][iy2][iz1].y;
      double by2=wx1*wy1*B[ix1][iy1][iz2].y+wx1*wy2*B[ix1][iy2][iz2].y
         +wx2*wy1*B[ix2][iy1][iz2].y+wx2*wy2*B[ix2][iy2][iz2].y;
      By=wz1*by1+wz2*by2;
      double bx1=wx1*wy1*B[ix1][iy1][iz1].x+wx1*wy2*B[ix1][iy2][iz1].x
         +wx2*wy1*B[ix2][iy1][iz1].x+wx2*wy2*B[ix2][iy2][iz1].x;
      double bx2=wx1*wy1*B[ix1][iy1][iz2].x+wx1*wy2*B[ix1][iy2][iz2].x
         +wx2*wy1*B[ix2][iy1][iz2].x+wx2*wy2*B[ix2][iy2][iz2].x;
      Bx=wz1*bx1+wz2*bx2;
      double bz1=wx1*wy1*B[ix1][iy1][iz1].z+wx1*wy2*B[ix1][iy2][iz1].z
         +wx2*wy1*B[ix2][iy1][iz1].z+wx2*wy2*B[ix2][iy2][iz1].z;
      double bz2=wx1*wy1*B[ix1][iy1][iz2].z+wx1*wy2*B[ix1][iy2][iz2].z
         +wx2*wy1*B[ix2][iy1][iz2].z+wx2*wy2*B[ix2][iy2][iz2].z;
      Bz=wz1*bz1+wz2*bz2;
   }

   //cout <<iy1<<iy2 <<endl;
   //cout << B0[0] <<", "<< B0[1] <<", "<< B0[2] <<", "<< B0[3] << endl;
   //cout <<"Bicubic:"<< ansB <<", Linear:"<< by <<endl;

   Bfield[0]= Bx;
   Bfield[1]= By;
   Bfield[2]= Bz;

   return true;

}




//////////////////////////////////
void E16ANA_MagneticFieldMap3D::cleanupMap( void )
   //////////////////////////////////
{
   for( int ix=0; ix<Nx; ++ix ){
      for( int iy=0; iy<Ny; ++iy ){
         B[ix][iy].clear();
      }
      B[ix].clear();
   }
   B.clear();
}

//////////////////////////////////////////////////////////////////////
void E16ANA_MagneticFieldMap3D::bcuint(float y[], float y1[], float y2[], float y12[],
      float x1l, float x1u, float x2l, float x2u, 
      const float x1, const float x2,
      float *ansy, float *ansy1, float *ansy2) const
/* Bicubic interpolation calculation function from Numerical Recipies,
 * Used wth bcucof.
 * y, y1, y2 and y12 have same meaning as that of bcucof.
 * x1l and x1u are the coordinates of the grids (lower and upper),
 * same for x2l and x2u.
 * x1 and x2 is the coordinates to calculate the inperpolated value.
 * Returns ansy (the interpolated value) and ansy1,2 (partial differentials)
 */
//////////////////////////////////////////////////////////////////////
{
   //void bucof(float y[], float y1[], float y2[], float y12[],
   // float d1, float d2, float **c);
   float t,u,d1,d2,c[4][4];

   d1=x1u-x1l;
   d2=x2u-x2l;
   bcucof(y,y1,y2,y12,d1,d2,c);
   if (x1u==x1l || x2u==x2l) cerr <<"Bad input in routine bcuint"<<endl;
   t=(x1-x1l)/d1;
   u=(x2-x2l)/d2;
   *ansy=(*ansy2)=(*ansy1)=0.;
   for (int i=3;i>=0;i--){
      *ansy=t*(*ansy)+((c[i][3]*u+c[i][2])*u+c[i][1])*u+c[i][0];
      /**ansy2=t*(*ansy2)+(3.*c[i][3]*u+2.*c[i][2])*u+c[i][1];
       *ansy1=u*(*ansy1)+(3.*c[3][i]*t+2.*c[2][i])*t+c[1][i];
       if (i>1){
       *ansy12=t*(*ansy12)+(i-1)*(3.*c[3][i]*u+2.*c[2][i])*u+c[1][i];
       }*/
   }
   /**ansy1/=d1;
    *ansy2/=d2;
    *ansy12/=(d1*d2);*/
}

//////////////////////////////////////////////////////////////////////
void E16ANA_MagneticFieldMap3D::bcuint2(float y[], float y1[], float y2[], float y12[],
      float x1l, float x1u, float x2l, float x2u, float x, 
      float *ansy1, float *ansy2) const
/* Bicubic interpolation calculation function from Numerical Recipies,
 * Used wth bcucof.
 * y, y1, y2 and y12 have same meaning as that of bcucof.
 * x1l and x1u are the coordinates of the grids (lower and upper),
 * same for x2l and x2u.
 * x1 and x2 is the coordinates to calculate the inperpolated value.
 * Returns ansy (the interpolated value) and ansy1,2 (partial differentials)
 */
//////////////////////////////////////////////////////////////////////
{
   //void bucof(float y[], float y1[], float y2[], float y12[],
   // float d1, float d2, float **c);
   float u,d1,d2,c[4][4];

   d1=x1u-x1l;
   d2=x2u-x2l;
   bcucof(y,y1,y2,y12,d1,d2,c);
   if (x1u==x1l || x2u==x2l) cerr <<"Bad input in routine bcuint"<<endl;
   u=(x-x2l)/d2;
   (*ansy2)=(*ansy1)=0.;
   for (int i=3;i>=0;i--){
      *ansy1=((c[i][3]*u+c[i][2])*u+c[i][1])*u+c[i][0];
      *ansy2=(*ansy2)+((c[i][3]*u+c[i][2])*u+c[i][1])*u+c[i][0];
   }
}

//////////////////////////////////////////////////////////////////////
void E16ANA_MagneticFieldMap3D::bcucof(float y[], float y1[], float y2[], float y12[],
      float d1, float d2, float c[4][4]) const
/* Bicubic coefficiencies calculation function from Numerical Recipies,
 * Used in the buint function 
 * y ... func. value
 * y1 ... partial differential by x1
 * y2 ... partial differential by x2
 * y12 ... partial differential by x12
 * index 1-4 ... grid number
 * d1, d2 ... distance between the grids for x1 and x2 direction
 * Return c as the coefficiencies
 */
//////////////////////////////////////////////////////////////////////
{
   static int wt[16][16]=
   {{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
      {-3,0,0,3,0,0,0,0,-2,0,0,-1,0,0,0,0},
      {2,0,0,-2,0,0,0,0,1,0,0,1,0,0,0,0},
      {0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0},
      {0,0,0,0,-3,0,0,3,0,0,0,0,-2,0,0,-1},
      {0,0,0,0,2,0,0,-2,0,0,0,0,1,0,0,1},
      {-3,3,0,0,-2,-1,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,-3,3,0,0,-2,-1,0,0},
      {9,-9,9,-9,6,3,-3,-6,6,-6,-3,3,4,2,1,2},
      {-6,6,-6,6,-4,-2,2,4,-3,3,3,-3,-2,-1,-1,-2},
      {2,-2,0,0,1,1,0,0,0,0,0,0,0,0,0,0},
      {0,0,0,0,0,0,0,0,2,-2,0,0,1,1,0,0},
      {-6,6,-6,6,-3,-3,3,3,-4,4,2,-2,-2,-2,-1,-1},
      {4,-4,4,-4,2,2,-2,-2,2,-2,-2,2,1,1,1,1}};

   float xx,d1d2,cl[16],x[16];

   d1d2=d1*d2;

   for(int i=1;i<=4;i++){
      x[i-1]=y[i-1];
      x[i+3]=y1[i-1]*d1;
      x[i+7]=y2[i-1]*d2;
      x[i+11]=y12[i-1]*d1d2;
   }
   for (int i=0;i<16;i++){
      xx=0.0;
      for (int k=0;k<16;k++) xx+=wt[i][k]*x[k];
      cl[i]=xx;
   }
   int l=0;
   for (int i=0;i<4;i++){
      for (int j=0;j<4;j++) c[i][j]=cl[l++];
   }
}

void E16ANA_MagneticFieldMap3D::polint(float xa[], float ya[], int n, float x, 
      float *y, float *dy) const{
/*Polynomial interpolation for function ya[i]=f(xa[i]).
 * Return value is y=f(x)
 */
   int i,m,ns=0;
   float den,dif,dift,ho,hp,w;
   float c[6], d[6];

   dif=fabs(x-xa[0]);
   for (i=0;i<n;i++){
      if((dift=fabs(x-xa[i]))<dif){
         ns=i;
         dif=dift;
      }
      c[i]=ya[i];
      d[i]=ya[i];
   }
   *y=ya[ns--];
   //if (ns>0) *y=ya[ns--];
   //else cout <<"Error about ns in routine polint"<<endl;
   for (m=1;m<n;m++){
      for (i=0;i<=n-m-1;i++){
         ho=xa[i]-x;
         hp=xa[i+m]-x;
         w=c[i+1]-d[i];
         if((den=ho-hp)==0.0) cout <<"Error in routine polint"<<endl;
         den=w/den;
         d[i]=hp*den;
         c[i]=ho*den;
      }
      *y+=(*dy=(2*ns<(n-m)?c[ns+1]:d[ns--]));
   }
}

//---------------------------------
int E16ANA_MagneticFieldMap::Initialize( void ){
  E16WARNING("this call may mean a bug in FieldMap pointer initialize");
    return ERROR;}

int E16ANA_MagneticFieldMap::Initialize_binary( const char* bindatafilename){
  E16WARNING("this call may mean a bug in FieldMap pointer initialize");
    return ERROR;}

int E16ANA_MagneticFieldMap::Initialize_binary(){
  E16WARNING("this call may mean a bug in FieldMap pointer initialize");
    return ERROR;}

int E16ANA_MagneticFieldMap::Initialize_shift(CLHEP::Hep3Vector shiftV){
  E16WARNING("this call may mean a bug in FieldMap pointer initialize");
    return ERROR;}

int E16ANA_MagneticFieldMap::Initialize_Bz( double field){
  E16WARNING("this call may mean a bug in FieldMap pointer initialize");
    return ERROR;}
