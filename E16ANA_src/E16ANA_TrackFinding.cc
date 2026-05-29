//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai
//2015-12-25, uploaded by komatsu
//E16ANA_TrackFinding.cc
//by Komatsu
#include <TVector3.h>
#include "E16ANA_TrackFinding.hh"
#include "E16ANA_SingleTrack.hh"
void TransformCoordinate_2(TVector3 *v, double *x, double *z, TVector3 *vx_new, TVector3 *vy_new){
  //Added by Komatsu 2015/10/21.
  //Input: Vectors of target and 3 hit points.
  //Output: (X, Z) Coordinates of Vectors of targets & 3 hit points.
  //v[0](target) v[1] v[2] v[3], xaxis=v[2]-v[0]
  //Change coordinate to "v[1] is origin and, vx and vy are basis".

  TVector3 v_2[3];
  TVector3 v_dummy;
  TVector3 vx, vy;
  TVector3 v_vert;
  
  v_vert.SetXYZ(0,1.,0);
  v_dummy=v[2]-v[0];
  vx.SetXYZ(v_dummy.X(), 0, v_dummy.Z());
  vx=vx.Unit();
  vy=vx.Cross(v_vert);
  vy=vy.Unit();
  //vx & vy are new unit vectors.
  *vx_new=vx;
  *vy_new=vy;

  //cerr<<"vx "<<vx.X()<<" "<<vx.Z()<<" wfdf L1219"<<endl;
  //cerr<<"vy "<<vy.X()<<" "<<vy.Z()<<" wfdf L1219"<<endl;
  
  
  v_2[0]=v[0]-v[1];
  v_2[0].SetY(0);

  v_2[1]=v[2]-v[1];
  v_2[1].SetY(0);
 
  v_2[2]=v[3]-v[1];
  v_2[2].SetY(0);

  x[0]=vx.X()*v_2[0].X()+vx.Z()*v_2[0].Z();
  z[0]=vy.X()*v_2[0].X()+vy.Z()*v_2[0].Z();
  
  x[1]=0.;
  z[1]=0.;
 
  x[2]=vx.X()*v_2[1].X()+vx.Z()*v_2[1].Z();
  z[2]=vy.X()*v_2[1].X()+vy.Z()*v_2[1].Z();
 
  x[3]=vx.X()*v_2[2].X()+vx.Z()*v_2[2].Z();
  z[3]=vy.X()*v_2[2].X()+vy.Z()*v_2[2].Z();
 
  //for(int i=0;i<4;i++){
  //cerr<<i<<" "<<x[i]<<" "<<z[i]<<endl;
  //}

}

void TransformCoordinate_3(TVector3 *v, double *x, double *z, TVector3 *vx_new, TVector3 *vy_new, double *m, double *n, int *zero_flag){
  //Added by Komatsu 2015/11/15.
  //Input: Vectors of target and 3 hit points.
  //Output: (X, Z) Coordinates of Vectors of targets & 3 hit points.
  //v[0](target) v[1] v[2] v[3], xaxis=v[2]-v[0]
  //Change coordinate to "v[1] is origin and, vx and vy are basis".
  //target line z=0 -> y=mx+n;


  TVector3 v_2[3], v_o;
  TVector3 v_dummy;
  TVector3 vx, vy;
  TVector3 v_vert;
  
  double x_tar, y_tar;
  double x_org, y_org;


  v_vert.SetXYZ(0,1.,0);
  v_dummy=v[2]-v[0];
  vx.SetXYZ(v_dummy.X(), 0, v_dummy.Z());
  vx=vx.Unit();
  vy=vx.Cross(v_vert);
  vy=vy.Unit();
  //vx & vy are new unit vectors.
  *vx_new=vx;
  *vy_new=vy;

  //cerr<<"vx "<<vx.X()<<" "<<vx.Z()<<" wfdf L1219"<<endl;
  //cerr<<"vy "<<vy.X()<<" "<<vy.Z()<<" wfdf L1219"<<endl;
    
  v_2[0]=v[0]-v[1];
  v_2[0].SetY(0);

  v_2[1]=v[2]-v[1];
  v_2[1].SetY(0);
 
  v_2[2]=v[3]-v[1];
  v_2[2].SetY(0);

  v_o=-1.*v[1];
  v_o.SetY(0);

  x[0]=vx.X()*v_2[0].X()+vx.Z()*v_2[0].Z();
  z[0]=vy.X()*v_2[0].X()+vy.Z()*v_2[0].Z();
  
  x[1]=0.;
  z[1]=0.;
 
  x[2]=vx.X()*v_2[1].X()+vx.Z()*v_2[1].Z();
  z[2]=vy.X()*v_2[1].X()+vy.Z()*v_2[1].Z();
 
  x[3]=vx.X()*v_2[2].X()+vx.Z()*v_2[2].Z();
  z[3]=vy.X()*v_2[2].X()+vy.Z()*v_2[2].Z();
 
  x_tar=vx.X()*1.;//transfom line(taget) z=0. vector of (1, 0) is transformed.
  y_tar=vy.X()*1.;

  x_org=vx.X()*v_o.X()+vx.Z()*v_o.Z();
  y_org=vy.X()*v_o.X()+vy.Z()*v_o.Z();

  if(x_tar!=0){
    *m=y_tar/x_tar;
    *n=-(*m)*x_org+y_org;
  }else{
    *zero_flag=1;
  }

  //for(int i=0;i<4;i++){
  //cerr<<i<<" "<<x[i]<<" "<<z[i]<<endl;
  //}

}

void GetCurve(double *x, double *z, double *r){
  double A[3][3], B[3];
  double detA, Ai[3][3];
  double a, b, c;
  
  for(int i=0;i<3;i++){
    for(int j=0;j<3;j++){
      A[i][j]=0.;
      Ai[i][j]=0.;
    }
    B[i]=0.;
  }
  a=0.;
  b=0.;
  c=0.;

 //z=a*x*x+b*x+c
  
  for(int i=0;i<3;i++){
      A[0][0]+=x[i]*x[i]*x[i]*x[i];
      A[0][1]+=x[i]*x[i]*x[i];
      A[0][2]+=x[i]*x[i];
      B[0]+=z[i]*x[i]*x[i];
      A[1][2]+=x[i];
      B[1]+=z[i]*x[i];
      A[2][2]++;
      B[2]+=z[i];
  }
  A[1][0]=A[0][1];
  A[1][1]=A[0][2];
  A[2][0]=A[1][1];
  A[2][1]=A[1][2];
  
  detA=A[0][0]*A[1][1]*A[2][2]+
    A[1][0]*A[2][1]*A[0][2]+
    A[2][0]*A[0][1]*A[1][2]-
    A[0][0]*A[2][1]*A[1][2]-
    A[2][0]*A[1][1]*A[0][2]-
    A[1][0]*A[0][1]*A[2][2];
  
    Ai[0][0]=A[1][1]*A[2][2]-A[1][2]*A[2][1];
    Ai[0][1]=A[0][2]*A[2][1]-A[0][1]*A[2][2];
    Ai[0][2]=A[0][1]*A[1][2]-A[0][2]*A[1][1];
    Ai[1][0]=A[1][2]*A[2][0]-A[1][0]*A[2][2];
    Ai[1][1]=A[0][0]*A[2][2]-A[0][2]*A[2][0];
    Ai[1][2]=A[0][2]*A[1][0]-A[0][0]*A[1][2];
    Ai[2][0]=A[1][0]*A[2][1]-A[1][1]*A[2][0];
    Ai[2][1]=A[0][1]*A[2][0]-A[0][0]*A[2][1];
    Ai[2][2]=A[0][0]*A[1][1]-A[0][1]*A[1][0];
    
    if(detA>0 || detA<0){
      a=(Ai[0][0]*B[0]+Ai[0][1]*B[1]+Ai[0][2]*B[2])/detA;
      b=(Ai[1][0]*B[0]+Ai[1][1]*B[1]+Ai[1][2]*B[2])/detA;
      c=(Ai[2][0]*B[0]+Ai[2][1]*B[1]+Ai[2][2]*B[2])/detA;
    }else{
      a=0.;
      b=0.;
      c=0.;
    }
            
    //r is curvature at (x[0], z[0])<-target.
    //(x, z), (x+dx, z+dz), z=axx+bx+c,  l=sqrt(dx^2+dz^2)
    //tan(theta)=2adx
    //r theta=l
    //l~dx sqrt((2ax+b)(2ax+b)+1)
    //r~sqrt((2ax+b)(2ax+b)+1)/2/a
    (*r)=sqrt((2*a*x[0]+b)*(2*a*x[0]+b)+1)/2./a;
    //cerr<<"wfdf 1321 R "<<(*r)<<endl;
    
}

void GetCurve_2(double *x, double *z, double *r, double *p){
  double A[3][3], B[3];
  double detA, Ai[3][3];
  double a, b, c;
  //cerr<<"GetCurve"<<endl;
  for(int i=0;i<3;i++){
    //cerr<<"i "<<i<<"\t"<<x[i]<<"\t"<<z[i]<<endl;
    for(int j=0;j<3;j++){
      A[i][j]=0.;
      Ai[i][j]=0.;
    }
    B[i]=0.;
  }
  a=0.;
  b=0.;
  c=0.;

 //z=a*x*x+b*x+c
  
  for(int i=0;i<3;i++){
      A[0][0]+=x[i]*x[i]*x[i]*x[i];
      A[0][1]+=x[i]*x[i]*x[i];
      A[0][2]+=x[i]*x[i];
      B[0]+=z[i]*x[i]*x[i];
      A[1][2]+=x[i];
      B[1]+=z[i]*x[i];
      A[2][2]++;
      B[2]+=z[i];
  }
  A[1][0]=A[0][1];
  A[1][1]=A[0][2];
  A[2][0]=A[1][1];
  A[2][1]=A[1][2];
  
  detA=A[0][0]*A[1][1]*A[2][2]+
    A[1][0]*A[2][1]*A[0][2]+
    A[2][0]*A[0][1]*A[1][2]-
    A[0][0]*A[2][1]*A[1][2]-
    A[2][0]*A[1][1]*A[0][2]-
    A[1][0]*A[0][1]*A[2][2];
  
    Ai[0][0]=A[1][1]*A[2][2]-A[1][2]*A[2][1];
    Ai[0][1]=A[0][2]*A[2][1]-A[0][1]*A[2][2];
    Ai[0][2]=A[0][1]*A[1][2]-A[0][2]*A[1][1];
    Ai[1][0]=A[1][2]*A[2][0]-A[1][0]*A[2][2];
    Ai[1][1]=A[0][0]*A[2][2]-A[0][2]*A[2][0];
    Ai[1][2]=A[0][2]*A[1][0]-A[0][0]*A[1][2];
    Ai[2][0]=A[1][0]*A[2][1]-A[1][1]*A[2][0];
    Ai[2][1]=A[0][1]*A[2][0]-A[0][0]*A[2][1];
    Ai[2][2]=A[0][0]*A[1][1]-A[0][1]*A[1][0];
    
    if(detA>0 || detA<0){
      a=(Ai[0][0]*B[0]+Ai[0][1]*B[1]+Ai[0][2]*B[2])/detA;
      b=(Ai[1][0]*B[0]+Ai[1][1]*B[1]+Ai[1][2]*B[2])/detA;
      c=(Ai[2][0]*B[0]+Ai[2][1]*B[1]+Ai[2][2]*B[2])/detA;
    }else{
      a=0.;
      b=0.;
      c=0.;
    }
            
    //r is curvature at (x[0], z[0])<-target.
    //(x, z), (x+dx, z+dz), z=axx+bx+c,  l=sqrt(dx^2+dz^2)
    //tan(theta)=2adx
    //r theta=l
    //l~dx sqrt((2ax+b)(2ax+b)+1)
    //r~sqrt((2ax+b)(2ax+b)+1)/2/a
    p[0]=a;
    p[1]=b;
    p[2]=c;
    (*r)=sqrt((2*a*x[0]+b)*(2*a*x[0]+b)+1)/2./a;
   
    cerr<<"Pol2 Par "<<p[0]<<"\t"<<p[1]<<"\t"<<p[2]<<endl;
    cerr<<"Est Vtx R "<<(*r)<<endl;
    
}

void SearchChi2MinSingleTrack( vector<E16ANA_SingleTrack*> ST_p, vector<E16ANA_SingleTrack*> ST_e, E16ANA_SingleTrack* *ST_p_chi2_min, E16ANA_SingleTrack* *ST_e_chi2_min, int *p_chi2min_flag, int *e_chi2min_flag){

  double chi2_min_p=1.0e10;
  double chi2_min_e=1.0e10;    
  int chi2minid_p[2], chi2minid_e[2];
  for(int i=0;i<2;i++){
    chi2minid_p[i]=-1;
    chi2minid_e[i]=-1;    
  }
   
  for(int i=0;i<(int)ST_p.size();i++){
    if(ST_p[i]->Chi2()<chi2_min_p){
      ST_p_chi2_min[0]=ST_p[i];      
      chi2_min_p=ST_p[i]->Chi2();
      chi2minid_p[0]=i;
      p_chi2min_flag[0]=1;
    }
  }
  for(int i=0;i<(int)ST_e.size();i++){
    if(ST_e[i]->Chi2()<chi2_min_e){
      ST_e_chi2_min[0]=ST_e[i];      
      chi2_min_e=ST_e[i]->Chi2();
      chi2minid_e[0]=i;
      e_chi2min_flag[0]=1;
    }
  }
  
  chi2_min_p=1.0e10;
  chi2_min_e=1.0e10;
  
  for(int i=0;i<(int)ST_p.size();i++){
    if(ST_p[i]->Chi2()<chi2_min_p && i!=chi2minid_p[0]){
      ST_p_chi2_min[1]=ST_p[i];      
      chi2_min_p=ST_p[i]->Chi2();
      chi2minid_p[1]=i;
      p_chi2min_flag[1]=1;
    }
  }
  for(int i=0;i<(int)ST_e.size();i++){
    if(ST_e[i]->Chi2()<chi2_min_e && i!=chi2minid_e[0]){
      ST_e_chi2_min[1]=ST_e[i];      
      chi2_min_e=ST_e[i]->Chi2();
      chi2minid_e[1]=i;
      e_chi2min_flag[1]=1;
    }
  }
  
  cerr<<"In SearchChi2!"<<endl;
  if((int)ST_p.size()>1){
    cerr<<"STp_chi2"<<endl;
    for(int i=0;i<2;i++){
      cerr<<"STp "<<i<<"\t"<<"chi2: "<<ST_p[chi2minid_p[i]]->Chi2()<<endl;
      cerr<<"Fit Mom at vtx: "<<ST_p[chi2minid_p[i]]->GetPvtx().X()<<"\t"<<ST_p[chi2minid_p[i]]->GetPvtx().Y()<<"\t"<<ST_p[chi2minid_p[i]]->GetPvtx().Z()<<endl;
    }
  }
  if((int)ST_e.size()>1){
    cerr<<"STe_chi2"<<endl;
    for(int i=0;i<2;i++){
      cerr<<"STe "<<i<<"\t"<<"chi2: "<<ST_e[chi2minid_e[i]]->Chi2()<<endl;
      cerr<<"Fit Mom at vtx: "<<ST_e[chi2minid_e[i]]->GetPvtx().X()<<"\t"<<ST_e[chi2minid_e[i]]->GetPvtx().Y()<<"\t"<<ST_e[chi2minid_e[i]]->GetPvtx().Z()<<endl;
    }
  }

}//SearchChi2MinSingleTrack

void XThreeHitCandidate::ChangeCoordinate(TVector3 *v, TVector3 *v2, TVector3 *x_axis2, TVector3 *z_axis2){
   
  TVector3 v_vert;
  TVector3 v_dummy;

  TVector3 v_2[3], v_o;
  TVector3 vx, vy;

  double x[4], z[4];
  
  double x_tar, y_tar;
  double x_org, y_org;
  
  v_vert.SetXYZ(0,1.,0);
  v_dummy=v[2]-v[0];
  vx.SetXYZ(v_dummy.X(), 0, v_dummy.Z());
  vx=vx.Unit();
  vy=vx.Cross(v_vert);
  vy=vy.Unit();

  *x_axis2=vx;
  *z_axis2=vy;

  v_2[0]=v[0]-v[1];
  v_2[0].SetY(0);

  v_2[1]=v[2]-v[1];
  v_2[1].SetY(0);
 
  v_2[2]=v[3]-v[1];
  v_2[2].SetY(0);

  v_o=-1.*v[1];
  v_o.SetY(0);

  //-------------------------------------//
  x[0]=vx.X()*v_2[0].X()+vx.Z()*v_2[0].Z();
  z[0]=vy.X()*v_2[0].X()+vy.Z()*v_2[0].Z();
  
  x[1]=0.;
  z[1]=0.;
 
  x[2]=vx.X()*v_2[1].X()+vx.Z()*v_2[1].Z();
  z[2]=vy.X()*v_2[1].X()+vy.Z()*v_2[1].Z();
 
  x[3]=vx.X()*v_2[2].X()+vx.Z()*v_2[2].Z();
  z[3]=vy.X()*v_2[2].X()+vy.Z()*v_2[2].Z();
  //-------------------------------------// 
  for(int i=0;i<4;i++){
    v2[i].SetXYZ(x[i], 0., z[i]);
  }
    
  x_tar=vx.X();//transfom line(taget) z=0. vector of (1, 0) is transformed.
  y_tar=vy.X();

  x_org=vx.X()*v_o.X()+vx.Z()*v_o.Z();
  y_org=vy.X()*v_o.X()+vy.Z()*v_o.Z();

  org_new.SetXYZ(x_org, y_org, 0.);


  if(x_tar!=0){
    m=y_tar/x_tar;
    n=-1.*m*x_org+y_org;
  }else{
    m=0.;
    n=0.;
  }

}

void XThreeHitCandidate::CalcPol2(double x[3], double z[3]){
  double A[3][3], B[3];
  double detA, Ai[3][3];
  double a, b, c;
  //cerr<<"GetCurve"<<endl;
  for(int i=0;i<3;i++){
    //cerr<<"i "<<i<<"\t"<<x[i]<<"\t"<<z[i]<<endl;
    for(int j=0;j<3;j++){
      A[i][j]=0.;
      Ai[i][j]=0.;
    }
    B[i]=0.;
  }
  a=0.;
  b=0.;
  c=0.;

 //z=a*x*x+b*x+c
  
  for(int i=0;i<3;i++){
      A[0][0]+=x[i]*x[i]*x[i]*x[i];
      A[0][1]+=x[i]*x[i]*x[i];
      A[0][2]+=x[i]*x[i];
      B[0]+=z[i]*x[i]*x[i];
      A[1][2]+=x[i];
      B[1]+=z[i]*x[i];
      A[2][2]++;
      B[2]+=z[i];
  }
  A[1][0]=A[0][1];
  A[1][1]=A[0][2];
  A[2][0]=A[1][1];
  A[2][1]=A[1][2];
  
  detA=A[0][0]*A[1][1]*A[2][2]+
    A[1][0]*A[2][1]*A[0][2]+
    A[2][0]*A[0][1]*A[1][2]-
    A[0][0]*A[2][1]*A[1][2]-
    A[2][0]*A[1][1]*A[0][2]-
    A[1][0]*A[0][1]*A[2][2];
  
    Ai[0][0]=A[1][1]*A[2][2]-A[1][2]*A[2][1];
    Ai[0][1]=A[0][2]*A[2][1]-A[0][1]*A[2][2];
    Ai[0][2]=A[0][1]*A[1][2]-A[0][2]*A[1][1];
    Ai[1][0]=A[1][2]*A[2][0]-A[1][0]*A[2][2];
    Ai[1][1]=A[0][0]*A[2][2]-A[0][2]*A[2][0];
    Ai[1][2]=A[0][2]*A[1][0]-A[0][0]*A[1][2];
    Ai[2][0]=A[1][0]*A[2][1]-A[1][1]*A[2][0];
    Ai[2][1]=A[0][1]*A[2][0]-A[0][0]*A[2][1];
    Ai[2][2]=A[0][0]*A[1][1]-A[0][1]*A[1][0];
    
    if(detA>0 || detA<0){
      a=(Ai[0][0]*B[0]+Ai[0][1]*B[1]+Ai[0][2]*B[2])/detA;
      b=(Ai[1][0]*B[0]+Ai[1][1]*B[1]+Ai[1][2]*B[2])/detA;
      c=(Ai[2][0]*B[0]+Ai[2][1]*B[1]+Ai[2][2]*B[2])/detA;
    }else{
      a=0.;
      b=0.;
      c=0.;
    }
    cerr<<"pol2 par: "<<a<<"\t"<<b<<"\t"<<c<<endl;       
    //r is curvature at (x[0], z[0])<-target.
    //(x, z), (x+dx, z+dz), z=axx+bx+c,  l=sqrt(dx^2+dz^2)
    //tan(theta)=2adx
    //r theta=l
    //l~dx sqrt((2ax+b)(2ax+b)+1)
    //r~sqrt((2ax+b)(2ax+b)+1)/2/a
    pol2_par[0]=a;
    pol2_par[1]=b;
    pol2_par[2]=c;
    
    R=sqrt((2*a*x[0]+b)*(2*a*x[0]+b)+1)/2./a;

    cerr<<"R "<<R<<endl;
    
    
}

TVector3 XThreeHitCandidate::CalcCrossTargetInNew(){
  //Cross point between quadratic curve and x-axis in the new coordinate.

  //p: y=p[0]xx+p[1]x+p[2]

  double a=pol2_par[0];
  double b=pol2_par[1];
  double c=pol2_par[2];
  double q;
  
  double alpha, beta, gamma;
  double ans[2];

  double s= m;
  double t= n;
 
  double cross_x, cross_y;
  
  alpha=a;
  beta=b-s;
  gamma=c-t;
    
  //cerr<<"Cross point alph beta gamma "<<alpha<<"\t"<<beta<<"\t"<<gamma<<endl;
  
  
  if(alpha!=0 && beta*beta-4.*alpha*gamma>0){
    if(beta>0){
      q=-0.5*(beta+sqrt(beta*beta-4.*alpha*gamma));
      ans[0]=q/alpha;
      ans[1]=gamma/q;
    }else{
      q=-0.5*(beta-sqrt(beta*beta-4.*alpha*gamma));
      ans[0]=q/alpha;
      ans[1]=gamma/q;		
    }
     
    //cerr<<"Crosspoint "<<ans[0]<<"\t"<<ans[1]<<"\t"<<histo->x_org_trans<<endl;

    if(fabs(ans[0]-org_new.X())<fabs(ans[1]-org_new.X())){
      cross_x=ans[0];
    }else if(fabs(ans[1]-org_new.X())<fabs(ans[0]-org_new.X())){
      cross_x=ans[1];
    }else{
      cross_x=-10000.;
    }
    
    cross_y=s*(cross_x)+t;
    
  }else if(alpha==0 && beta!=0){
    cross_x=-1.*gamma/beta;
    cross_y=s*(cross_x)+t;
  }else{
    cross_x=-10000.;
    cross_y=-10000.;
  }

  return TVector3(cross_x, cross_y, 0.);

}

void XThreeHitCandidate::EstInitVtxMom(){
  
  E16ANA_GTRAnalyzedHit* ahit1=anahits[0];
  E16ANA_GTRAnalyzedHit* ahit2=anahits[1];
  E16ANA_GTRAnalyzedHit* ahit3=anahits[2];

  TVector3 v_xz[4], v_xz2[4], x_axis2, z_axis2;
  v_xz[0].SetXYZ(0., 0., 0.);
  v_xz[1].SetXYZ(ahit1->Position().X(), 0., ahit1->Position().Z());
  v_xz[2].SetXYZ(ahit2->Position().X(), 0., ahit2->Position().Z());
  v_xz[3].SetXYZ(ahit3->Position().X(), 0., ahit3->Position().Z());
  
  for(int i=0;i<4;i++){
    v_xz[i]*=0.1;
  }
  
  cerr<<"v_xz1: "<<v_xz[1].X()<<"\t"<<v_xz[1].Y()<<"\t"<<v_xz[1].Z()<<endl;
 
  cerr<<"Change Coord"<<endl;
  ChangeCoordinate(v_xz, v_xz2, &x_axis2, &z_axis2);
  double x[3], z[3];
  for(int i=0;i<3;i++){
    x[i]=v_xz2[i+1].X();
    z[i]=v_xz2[i+1].Z();
    cerr<<"x y z: "<<i<<"\t"<<x[i]<<"\t"<<z[i]<<endl;
  }
  CalcPol2(x, z);
  TVector3 cross_new = CalcCrossTargetInNew();
  double cross_x, cross_z; 
  cross_x=cross_new.X()*x_axis2.X()+cross_new.Y()*z_axis2.X()+v_xz[1].X();
  cross_z=cross_new.X()*x_axis2.Z()+cross_new.Y()*z_axis2.Z()+v_xz[1].Z();
  cross_target.SetXYZ(cross_x, 0., cross_z);

  cerr<<"Cross at target: "<<cross_target.X()<<"\t"<<cross_target.Z()<<endl;
  
  //Get initial momentum. pol2 is re-determined using (0,0,0), hits in gtr1 and gtr2.
  double x_hit[4], z_hit[4];
  TVector3 vx_new, vy_new;
  double par[3];
  double R_target;
  TransformCoordinate_2(v_xz, x_hit, z_hit, &vx_new, &vy_new);//Use pol2
  GetCurve_2(x_hit, z_hit, &R_target, par);//Use pol2
  

  //  par[0]=xcand->Pol2Par(0);
  //par[1]=xcand->Pol2Par(1);
  //par[2]=xcand->Pol2Par(2);
 

  //mom0_xz_n=0.3*1.7*xcand->Radius()*0.01;//initial mom in XZ plane. 
  mom0_xz=0.3*1.7*R_target*0.01;//initial mom in XZ plane. 
  //Direction of momentum in XZ plane.
  double mom_dir_x=vx_new.X()+(2.*par[0]+par[1])*(vy_new.X());//diffrential of a[0]xx+[1]x+a[2].
  double mom_dir_z=vx_new.Z()+(2.*par[0]+par[1])*(vy_new.Z());
  mom0_xz_v.SetXYZ(fabs(mom0_xz)*mom_dir_x, 0, fabs(mom0_xz)*mom_dir_z);

  cerr<<"Radius: "<<R<<endl;
  cerr<<"mom_dir x z:(using class) "<<mom_dir_x<<"\t"<<mom_dir_z<<endl;
  cerr<<"mom0xz_n:"<<mom0_xz<<endl;
  cerr<<"InitMom XZ: "<<endl;
  mom0_xz_v.Print();
  
}

