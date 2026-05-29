//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai

#include "E16ANA_AlignmentV0.hh"

#include <TF1.h>
#include <TVector2.h>
#include <TH1F.h>
#include <TGraph.h>
#include <TLinearFitter.h>
#include <TMath.h>
#include <TRandom.h>

E16ANA_AlignTree::E16ANA_AlignTree() : 
   smear_x(0.01), smear_y(0.03)
{
   tree = new TTree("tree", "tree");
   tree->Branch("mxa2", &mxa2, "mxa2/D");
   tree->Branch("mxb2", &mxb2, "mxb2/D");
   tree->Branch("mxc2", &mxc2, "mxc2/D");
   tree->Branch("mya2", &mya2, "mya2/D");
   tree->Branch("myb2", &myb2, "myb2/D");
   tree->Branch("myc2", &myc2, "myc2/D");
}

E16ANA_AlignTree::~E16ANA_AlignTree(){
   delete tree;
}

bool E16ANA_AlignTree::GTRHitSelection(E16ANA_G4Detector<E16ANA_G4Hit> &gtr, int detector_id, E16ANA_G4Hit &hit_result){
   int noh = gtr.Noh();
   for(int i=0; i<noh; i++){
      E16ANA_G4Hit &hit = gtr.Hit(i);
      if(hit.PID() == 22){ continue; } // gamma cut
      if(hit.DetectorID() == detector_id){
         hit_result = hit;
         return true;
      }
   }
   return false;
}

void E16ANA_AlignTree::SetBranchAddress(
         double *xa, double *xb, double *xc,
         double *ya, double *yb, double *yc){
   tree->SetBranchAddress("mxa2", xa);
   tree->SetBranchAddress("mxb2", xb);
   tree->SetBranchAddress("mxc2", xc);
   tree->SetBranchAddress("mya2", ya);
   tree->SetBranchAddress("myb2", yb);
   tree->SetBranchAddress("myc2", yc);
}

void E16ANA_AlignTree::AppendFromG4Data(E16ANA_G4OutputData *g4data, int module_id){
   //double smear_x = 0.01; // cm
   //double smear_y = 0.03; // cm
   //double mxa2, mxb2, mxc2, mya2, myb2, myc2;
   //tree->Branch("mxa2", &mxa2, "mxa2/D");
   //tree->Branch("mxb2", &mxb2, "mxb2/D");
   //tree->Branch("mxc2", &mxc2, "mxc2/D");
   //tree->Branch("mya2", &mya2, "mya2/D");
   //tree->Branch("myb2", &myb2, "myb2/D");
   //tree->Branch("myc2", &myc2, "myc2/D");

   while(1){
      //int flag = g4data2.ReadAnEvent();
      double time[10] = {};
      int multi = 1;
      int flag = g4data->ReadOverlapEvent(multi, time, 0);
      if(flag != multi){
         //E16Message_FATAL("file end at event %d",n);
         break;
      }
      //if((n+1)%100 == 0) cout << "Event ID = " << n+1 << "/" << nEvent << endl;
      //EventAnalysis(g4data2.Event());
      //EventAnalysis2(&g4data->OverlapEvent(), module_id, tree);
      E16ANA_G4Event &g4event = g4data->OverlapEvent();
      int noh3 = g4event.GTR3().Noh();
      for(int i=0; i<noh3; i++){
         E16ANA_G4Hit &hit3 = g4event.GTR3().Hit(i);
         if(hit3.PID() == 22){ continue; } // gamma cut
         int gtr3id = hit3.DetectorID();
         if(gtr3id != module_id){ continue; } // module id cut
         E16ANA_G4Hit hit2;
         E16ANA_G4Hit hit1;
         if(    GTRHitSelection(g4event.GTR2(), gtr3id, hit2)
             && GTRHitSelection(g4event.GTR1(), gtr3id, hit1)){
            mxa2 = hit1.X()+gRandom->Gaus(0.0,smear_x);
            mxb2 = hit2.X()+gRandom->Gaus(0.0,smear_x);
            mxc2 = hit3.X()+gRandom->Gaus(0.0,smear_x);
            mya2 = hit1.Y()+gRandom->Gaus(0.0,smear_y);
            myb2 = hit2.Y()+gRandom->Gaus(0.0,smear_y);
            myc2 = hit3.Y()+gRandom->Gaus(0.0,smear_y);
            if(mxa2>-4.0 && mxa2<4.0){
            //if(mxc2>-4.5 && mxc2<4.5 && myc2>-4.5 && myc2<4.5){
            //if(mxc2>-1.5 && mxc2<1.5 && myc2>-1.5 && myc2<1.5){
            tree->Fill();
            }
         }
      }
      //int n_tracks = g4event.PrimaryTracks().Noh();
      //for(int i=0; i<n_tracks; i++){
      //   E16ANA_G4Track &track = g4event.PrimaryTracks(i);
      //   //out_file->FillVtx(track.TVertex());
      //   //out_file->FillVtxMom(track.PTV());
      //   track.TVertex().Print();
      //}
   }
}

E16ANA_AlignmentV0::E16ANA_AlignmentV0(E16ANA_GeometryV1 *_geom, int _module_id) :
   geom(_geom), module_id(_module_id) ,ycut_min(-15.0), ycut_max(15.0),
   print_level(1)
{
   delta_x[0] = 0.0;
   delta_x[1] = 0.0;
   delta_x[2] = 0.0;
   delta_z[0] = 0.0;
   delta_z[1] = 0.0;
   delta_z[2] = 0.0;
   delta_theta = 0.0;
}

E16ANA_AlignmentV0::~E16ANA_AlignmentV0(){
}

void E16ANA_AlignmentV0::AlignZX(double dtheta){
   double aligned_dx1;
   double aligned_dx2;
   double aligned_dx3;
   double aligned_dz1;
   double aligned_dz2;
   double aligned_dz3;

   double aligned_dx1_2;
   double aligned_dx2_2;
   double aligned_dx3_2;
   double aligned_dz1_2;
   double aligned_dz2_2;
   double aligned_dz3_2;

   double aligned_dtheta = dtheta;

   double xvoff1 = target_pos1.X();
   double zvoff1 = target_pos1.Z();
   double xvoff2 = target_pos2.X();
   double zvoff2 = target_pos2.Z();

   ZXAlignment(
         xvoff1, zvoff1, xvoff2, zvoff2, tree1, tree2,
         geom, module_id, 0, 2, aligned_dtheta,
         aligned_dx1, aligned_dx3, aligned_dz1, aligned_dz3);

   //TVector2 aligned_dc1(aligned_dx1, geom->GTRz[0][module_id]+aligned_dz1);
   //aligned_dc1 = aligned_dc1.Rotate(aligned_dtheta);
   //aligned_dc1.Print();
   //TVector2 aligned_dc3(aligned_dx3, geom->GTRz[2][module_id]+aligned_dz3);
   //aligned_dc3 = aligned_dc3.Rotate(aligned_dtheta);
   //aligned_dc3.Print();

   ZXAlignment(
         xvoff1, zvoff1, xvoff2, zvoff2, tree1, tree2,
         geom, module_id, 0, 1, aligned_dtheta,
         aligned_dx1_2, aligned_dx2, aligned_dz1_2, aligned_dz2);

   //TVector2 aligned_dc1_2(aligned_dx1_2, geom->GTRz[0][module_id]+aligned_dz1_2);
   //aligned_dc1_2 = aligned_dc1_2.Rotate(aligned_dtheta);
   //aligned_dc1_2.Print();
   //TVector2 aligned_dc2(aligned_dx2, geom->GTRz[1][module_id]+aligned_dz2);
   //aligned_dc2 = aligned_dc2.Rotate(aligned_dtheta);
   //aligned_dc2.Print();

   ZXAlignment(
         xvoff1, zvoff1, xvoff2, zvoff2, tree1, tree2,
         geom, module_id, 1, 2, aligned_dtheta,
         aligned_dx2_2, aligned_dx3_2, aligned_dz2_2, aligned_dz3_2);

   //TVector2 aligned_dc2_2(aligned_dx2_2, geom->GTRz[1][module_id]+aligned_dz2_2);
   //aligned_dc2_2 = aligned_dc2_2.Rotate(aligned_dtheta);
   //aligned_dc2_2.Print();
   //TVector2 aligned_dc3_2(aligned_dx3_2, geom->GTRz[2][module_id]+aligned_dz3_2);
   //aligned_dc3_2 = aligned_dc3_2.Rotate(aligned_dtheta);
   //aligned_dc3_2.Print();

   if(print_level >= 1){
      std::cout << "Corrected1 : "
         << aligned_dx1 << " " << aligned_dx2 << " " << aligned_dx3 << " "
         << aligned_dz1 << " " << aligned_dz2 << " " << aligned_dz3 << " "
         << aligned_dtheta << std::endl;

      std::cout << "Corrected2 : "
         << aligned_dx1_2 << " " << aligned_dx2_2 << " " << aligned_dx3_2 << " "
         << aligned_dz1_2 << " " << aligned_dz2_2 << " " << aligned_dz3_2 << " "
         << aligned_dtheta << std::endl;
   }

   delta_x[0] = aligned_dx1;
   delta_x[1] = aligned_dx2;
   delta_x[2] = aligned_dx3;
   delta_z[0] = aligned_dz1;
   delta_z[1] = aligned_dz2;
   delta_z[2] = aligned_dz3;
   delta_theta = aligned_dtheta;
}

void E16ANA_AlignmentV0::Align(){
   double aligned_dx1;
   //double aligned_dx2;
   double aligned_dx3;
   double aligned_dz1;
   //double aligned_dz2;
   double aligned_dz3;

   //double aligned_dx1_2;
   //double aligned_dx2_2;
   //double aligned_dx3_2;
   //double aligned_dz1_2;
   //double aligned_dz2_2;
   //double aligned_dz3_2;

   double aligned_dtheta;

   double xvoff1 = target_pos1.X();
   double zvoff1 = target_pos1.Z();
   double xvoff2 = target_pos2.X();
   double zvoff2 = target_pos2.Z();

   double max_delta= 0.01; // radian
   int n_dtheta_bins= 20;
   double d_delta= max_delta/(double)n_dtheta_bins;
   double *shift= new double[n_dtheta_bins];
   double *dtheta= new double[n_dtheta_bins];

   for(int i=0; i<n_dtheta_bins; i++){
   //for(int i=0; i<0; i++){
      dtheta[i]= ((double)i-(double)n_dtheta_bins/2.0+0.5)*d_delta;
      if(print_level >= 1){
         std::cout << "theta loop = " << i << ", dtheta = " << dtheta[i] << std::endl;
      }

      double mod2 = geom->GTRtheta[module_id]*TMath::Pi()/180.0;
      //double toffx21 = -zvoff1*sin(mod2-dtheta[i]);
      //double toffz21 =  zvoff1*cos(mod2-dtheta[i]);
      //double toffx22 = -zvoff2*sin(mod2-dtheta[i]);
      //double toffz22 =  zvoff2*cos(mod2-dtheta[i]);
      TVector2 toff1(xvoff1, zvoff1);
      toff1 = toff1.Rotate(mod2-dtheta[i]);
      TVector2 toff2(xvoff2, zvoff2);
      toff2 = toff2.Rotate(mod2-dtheta[i]);
      double toffx21 = toff1.X();
      //double toffz21 = toff1.Y();
      double toffx22 = toff2.X();
      //double toffz22 = toff2.Y();

      double m1, m2;

      ZXAlignment(
            xvoff1, zvoff1, xvoff2, zvoff2, tree1, tree2,
            geom, module_id, 0, 2, dtheta[i],
            aligned_dx1, aligned_dx3, aligned_dz1, aligned_dz3);

      ThetaAlignment(xvoff1, zvoff1, tree1, geom, module_id, 0, 2,
            dtheta[i], aligned_dx1, aligned_dx3, aligned_dz1, aligned_dz3,
            m1);
      //std::cout<< "ThetaAlignment1 ends."<< std::endl;
      ThetaAlignment(xvoff2, zvoff2, tree2, geom, module_id, 0, 2,
            dtheta[i], aligned_dx1, aligned_dx3, aligned_dz1, aligned_dz3,
            m2);
      //std::cout<< "ThetaAlignment2 ends."<< std::endl;
      double Dp= toffx21-toffx22;
      shift[i]= m1-m2-Dp;
      if(print_level >= 1){
         std::cout << "ThetaAlignment ends. shift = " << shift[i] << std::endl << std::endl;
      }
   }

   TGraph *g_shift= new TGraph(n_dtheta_bins, dtheta, shift);
   double fit_min= -max_delta/2.0;
   double fit_max=  max_delta/2.0;
   TF1 *f_shift= new TF1("f_shift_align", "pol1", fit_min, fit_max);
   g_shift->Fit("f_shift_align", "QN", "", fit_min, fit_max);
   aligned_dtheta= -(f_shift->GetParameter(0))/(f_shift->GetParameter(1));
   //aligned_dtheta= miss7;
   //aligned_dtheta= 0.0;
   //std::cout<< "Aligned ratio (z1/z3) = "<< aligned_z1z3<< std::endl;
   delete g_shift;
   delete f_shift;
   delete [] shift;
   delete [] dtheta;

   AlignZX(aligned_dtheta);

}

void E16ANA_AlignmentV0::ZAlignment(double xvoff, double zvoff, E16ANA_AlignTree *tree,
      E16ANA_GeometryV1 *_geom, int _module_id, int l1, int l3,
      double dtheta, double &z1z3){ // z1z3 = z_l1 / z_l3

   //double mxa2_,mxb2_,mxc2_,mya2_,myb2_,myc2_;
   double local_x[3];
   double local_y[3];
   //tree->SetBranchAddress("mxa2", &mxa2_);
   //tree->SetBranchAddress("mxb2", &mxb2_);
   //tree->SetBranchAddress("mxc2", &mxc2_);
   //tree->SetBranchAddress("mya2", &mya2_);
   //tree->SetBranchAddress("myb2", &myb2_);
   //tree->SetBranchAddress("myc2", &myc2_);
   //tree->SetBranchAddress(&mxa2_,&mxb2_,&mxc2_,&mya2_,&myb2_,&myc2_);
   tree->SetBranchAddress(&local_x[0],&local_x[1],&local_x[2],&local_y[0],&local_y[1],&local_y[2]);

   //double mod2  = imod2*25.275*3.14/180;
   double mod2 = _geom->GTRtheta[_module_id]*TMath::Pi()/180.0;
   //double toffx2 = -zvoff*sin(mod2-dtheta);
   //double toffz2 =  zvoff*cos(mod2-dtheta);
   TVector2 toff(xvoff, zvoff);
   toff = toff.Rotate(mod2-dtheta);
   double toffx2 = toff.X();
   double toffz2 = toff.Y();
   double design_z1 = _geom->GTRz[l1][_module_id];
   double design_z3 = _geom->GTRz[l3][_module_id];

   // Z alignment
   int n_entries= tree->GetEntries();

   //double max_delta= 0.5; // cm
   //int n_dz_bins= 10;
   double max_delta= 4.0; // cm
   int n_dz_bins= 80;
   double d_delta= max_delta/(double)n_dz_bins;
   double *slope= new double[n_dz_bins];
   double *ratio_z1z3= new double[n_dz_bins];

   //TH2F *h_align_z= new TH2F("h_align_z","",6,-4.5,4.5,500,-0.5,0.5);
   //TH2F *h_align_z= new TH2F("h_align_z","",15,-5.0,5.0,4000,-0.2,0.2);

   for(int i=0; i<n_dz_bins; i++){
      double delta_z3= ((double)i-(double)n_dz_bins/2.0+0.5)*d_delta;
      //h_align_z->Reset();
      TLinearFitter *lf_align_z = new TLinearFitter(1,"pol1");
      lf_align_z->StoreData(false);

      for(int n=0; n<n_entries; n++){
         tree->GetEntry(n);
         if(local_y[2] < ycut_min || local_y[2] > ycut_max) continue;
         // binned method
         //h_align_z->Fill(mxa2_, (mxc2_-toffx2)*(design_z1-toffz2)/(design_z3+delta_z3-toffz2)-mxa2_+toffx2);
         double fit_y = (local_x[l3]-toffx2)
            *(design_z1-toffz2)/(design_z3+delta_z3-toffz2)
            -local_x[l1]+toffx2;// to be updated (straight track + GTR plane -> cross point ?)
         if(local_x[l1]>-4.0 && local_x[l1]<4.0){
         if(fit_y>-0.2 && fit_y<0.2){
            lf_align_z->AddPoint(&local_x[l1], fit_y);
         }
         }
/*
         if(n%10==0){
            std::cout<< "n/n_entries = "<< n<< "/"<< n_entries
            << ", local x1 = "<< mxa2_
            << ", local x2 = "<< mxb2_
            << ", local x3 = "<< mxc2_
            << std::endl;
         }
*/
      }
#if 0 // binned method
      TProfile *h_align_z_pfx= h_align_z->ProfileX("h_align_z_pfx");
      TF1 *func= new TF1("func", "pol1", -5.0, 5.0);
      h_align_z_pfx->Fit("func", "QN", "", -5.0, 5.0);
      slope[i]= func->GetParameter(1);
      ratio_z1z3[i]= (design_z1-toffz2)/(design_z3+delta_z3-toffz2);
      //std::cout<<"z_loop = "<< i<< " : "<< ratio_z1z3[i]<< ", "<< slope[i]<< std::endl;
      delete func;
      delete h_align_z_pfx;
#else // un-binned method
      lf_align_z->Eval();
      slope[i] = lf_align_z->GetParameter(1);
      ratio_z1z3[i] = (design_z1-toffz2)/(design_z3+delta_z3-toffz2);
      delete lf_align_z;
#endif
   }

   TGraph *g_slope= new TGraph(n_dz_bins, ratio_z1z3, slope);
   double fit_min= (design_z1-toffz2)/(design_z3-toffz2+max_delta/2.0);
   double fit_max= (design_z1-toffz2)/(design_z3-toffz2-max_delta/2.0);
   TF1 *f_slope= new TF1("f_slope_align", "pol1", fit_min, fit_max);
   g_slope->Fit("f_slope_align", "QN", "", fit_min, fit_max);
   double aligned_z1z3= -(f_slope->GetParameter(0))/(f_slope->GetParameter(1));
   //std::cout<< "Aligned ratio (z1/z3) = "<< aligned_z1z3<< std::endl;
   delete f_slope;
   delete g_slope;
   delete [] slope;
   delete [] ratio_z1z3;
   //delete h_align_z;
   //delete tree;

   z1z3= aligned_z1z3;
}

void E16ANA_AlignmentV0::XAlignment(double xvoff, double zvoff, E16ANA_AlignTree *tree,
      E16ANA_GeometryV1 *_geom, int _module_id, int l1, int l3,
      double dtheta, double dz1, double dz3,
      double &mean, double &sigma){

   //double mxa2_,mxb2_,mxc2_,mya2_,myb2_,myc2_;
   double local_x[3];
   double local_y[3];
   //tree->SetBranchAddress("mxa2", &mxa2_);
   //tree->SetBranchAddress("mxb2", &mxb2_);
   //tree->SetBranchAddress("mxc2", &mxc2_);
   //tree->SetBranchAddress("mya2", &mya2_);
   //tree->SetBranchAddress("myb2", &myb2_);
   //tree->SetBranchAddress("myc2", &myc2_);
   //tree->SetBranchAddress(&mxa2_,&mxb2_,&mxc2_,&mya2_,&myb2_,&myc2_);
   tree->SetBranchAddress(&local_x[0],&local_x[1],&local_x[2],&local_y[0],&local_y[1],&local_y[2]);

   TH1F *h_align_x= new TH1F("h_align_x", "", 500, -0.5, 0.5);

   //double mod2  = imod2*25.275*3.14/180;
   double mod2 = _geom->GTRtheta[_module_id]*TMath::Pi()/180.0;
   //double toffx2 = -zvoff*sin(mod2-dtheta);
   //double toffz2 =  zvoff*cos(mod2-dtheta);
   TVector2 toff(xvoff, zvoff);
   toff = toff.Rotate(mod2-dtheta);
   double toffx2 = toff.X();
   double toffz2 = toff.Y();

   double design_z1 = _geom->GTRz[l1][_module_id];
   double design_z3 = _geom->GTRz[l3][_module_id];

   int n_entries= tree->GetEntries();
   for(int n=0; n<n_entries; n++){
      tree->GetEntry(n);
      if(local_y[2] < ycut_min || local_y[2] > ycut_max) continue;
      h_align_x->Fill((local_x[l3]-toffx2)*(design_z1+dz1-toffz2)/(design_z3+dz3-toffz2)-local_x[l1]+toffx2); // to be updated
   }

   TF1 *func= new TF1("func_align", "gaus", -0.5, 0.5);
   h_align_x->Fit("func_align", "QN", "", -0.5, 0.5);
   mean= func->GetParameter(1);
   sigma= func->GetParameter(2);
   //mean= h_align_x->GetMean();
   delete h_align_x;
   delete func;
}

void E16ANA_AlignmentV0::ThetaAlignment(double xvoff, double zvoff, E16ANA_AlignTree *tree,
      E16ANA_GeometryV1 *_geom, int _module_id, int l1, int l3,
      double dtheta, double dx1, double dx3, double dz1, double dz3,
      double &mean){

   //double mxa2_,mxb2_,mxc2_,mya2_,myb2_,myc2_;
   double local_x[3];
   double local_y[3];
   //tree->SetBranchAddress("mxa2", &mxa2_);
   //tree->SetBranchAddress("mxb2", &mxb2_);
   //tree->SetBranchAddress("mxc2", &mxc2_);
   //tree->SetBranchAddress("mya2", &mya2_);
   //tree->SetBranchAddress("myb2", &myb2_);
   //tree->SetBranchAddress("myc2", &myc2_);
   //tree->SetBranchAddress(&mxa2_,&mxb2_,&mxc2_,&mya2_,&myb2_,&myc2_);
   tree->SetBranchAddress(&local_x[0],&local_x[1],&local_x[2],&local_y[0],&local_y[1],&local_y[2]);

   //TH1F *h_align_theta= new TH1F("h_align_theta", "", 500, -0.5, 0.5);

   //double mod2  = imod2*25.275*3.14/180;
   double mod2 = _geom->GTRtheta[_module_id]*TMath::Pi()/180.0;
   //double toffx2 = -zvoff*sin(mod2-dtheta);
   //double toffz2 =  zvoff*cos(mod2-dtheta);
   TVector2 toff(xvoff, zvoff);
   toff = toff.Rotate(mod2-dtheta);
   //double toffx2 = toff.X();
   double toffz2 = toff.Y();

   double design_z1 = _geom->GTRz[l1][_module_id];
   double design_z3 = _geom->GTRz[l3][_module_id];

   mean= 0.0;
   int n_entries= tree->GetEntries();
   for(int n=0; n<n_entries; n++){
      tree->GetEntry(n);
      if(local_y[2] < ycut_min || local_y[2] > ycut_max) continue;
      double a= design_z1+dz1-toffz2;
      double b= design_z3+dz3-toffz2;
      double x= local_x[l1]-dx1;
      double z= local_x[l3]-dx3;
      //h_align_theta->Fill((x*b-z*a)/(b-a));
      mean+= (x*b-z*a)/(b-a)/(double)n_entries;
   }
/*
   TF1 *func= new TF1("func", "gaus", -0.5, 0.5);
   h_align_theta->Fit("func", "QN", "", -0.5, 0.5);
   mean= func->GetParameter(1);
   delete func;
   delete h_align_theta;
*/
}

void E16ANA_AlignmentV0::ZXAlignment(double xvoff1, double zvoff1, double xvoff2, double zvoff2,
      E16ANA_AlignTree *_tree1, E16ANA_AlignTree *_tree2,
      E16ANA_GeometryV1 *_geom, int _module_id, int l1, int l3,
      double dtheta,
      double &aligned_dx1, double &aligned_dx3, double &aligned_dz1, double &aligned_dz3){

   double mod2 = _geom->GTRtheta[_module_id]*TMath::Pi()/180.0;
   //double toffx21 = -zvoff1*sin(mod2-dtheta);
   //double toffz21 =  zvoff1*cos(mod2-dtheta);
   //double toffx22 = -zvoff2*sin(mod2-dtheta);
   //double toffz22 =  zvoff2*cos(mod2-dtheta);
   TVector2 toff1(xvoff1, zvoff1);
   toff1 = toff1.Rotate(mod2-dtheta);
   TVector2 toff2(xvoff2, zvoff2);
   toff2 = toff2.Rotate(mod2-dtheta);
   //double toffx21 = toff1.X();
   double toffz21 = toff1.Y();
   //double toffx22 = toff2.X();
   double toffz22 = toff2.Y();

   double r1, r2;
   ZAlignment(xvoff1, zvoff1, _tree1, _geom, _module_id, l1, l3, dtheta, r1);
   if(print_level >= 2){
      //std::cout << "theta_loop = " << i << ", ZAlignment1 ends. r1 = " << r1 << std::endl;
      std::cout << "ZAlignment1 ends. r1 = " << r1 << std::endl;
   }
   ZAlignment(xvoff2, zvoff2, _tree2, _geom, _module_id, l1, l3, dtheta, r2);
   if(print_level >= 2){
      //std::cout << "theta_loop = " << i << ", ZAlignment2 ends. r2 = " << r2 << std::endl;
      std::cout << "ZAlignment2 ends. r2 = " << r2 << std::endl;
   }
   //double D= zvoff1-zvoff2;
   double D= toffz21-toffz22;
   double design_z1 = _geom->GTRz[l1][_module_id];
   double design_z3 = _geom->GTRz[l3][_module_id];
   aligned_dz1= D*(r2-1.0)/(1.0-r2/r1)-design_z1+toffz21;
   aligned_dz3= D*(r2-1.0)/(r1-r2)-design_z3+toffz21;
   if(print_level >= 2){
      //std::cout << "theta_loop = " << i << ", delta_z1 = " << aligned_dz1*10.0 << " [mm], delta_z3 = " << aligned_dz3*10.0 << " [mm]" << std::endl;
      std::cout << "delta_z1 = " << aligned_dz1*10.0 << " [mm], delta_z3 = " << aligned_dz3*10.0 << " [mm]" << std::endl;
   }

   double m1, m2, s1, s2;

   XAlignment(xvoff1, zvoff1, _tree1, _geom, _module_id, l1, l3, dtheta, aligned_dz1, aligned_dz3, m1, s1);
   if(print_level >= 2){
      std::cout << "XAlignment1 ends. m1 = " << m1 << std::endl;
   }

   XAlignment(xvoff2, zvoff2, _tree2, _geom, _module_id, l1, l3, dtheta, aligned_dz1, aligned_dz3, m2, s2);
   if(print_level >= 2){
      std::cout << "XAlignment2 ends. m2 = " << m2 << std::endl;
   }

   double ratio1= (design_z1+aligned_dz1-toffz21)/(design_z3+aligned_dz3-toffz21);// a/b
   double ratio2= (design_z1+aligned_dz1-toffz22)/(design_z3+aligned_dz3-toffz22);// a'/b'

   aligned_dx1= (m1/ratio1-m2/ratio2)/(1.0/ratio1-1.0/ratio2);
   aligned_dx3= -(m1-m2)/(ratio1-ratio2);

   if(print_level >= 2){
      //std::cout << "theta_loop = " << i << ", delta_x1 = " << aligned_dx1*10.0 << " [mm], delta_x3 = " << aligned_dx3*10.0 << " [mm]" << std::endl;
      std::cout << "delta_x1 = " << aligned_dx1*10.0 << " [mm], delta_x3 = " << aligned_dx3*10.0 << " [mm]" << std::endl;
   }
}



