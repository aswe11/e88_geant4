//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai
#ifndef E16ANA_AlignmentV0_hh
#define E16ANA_AlignmentV0_hh

#include "E16ANA_G4OutputData.hh"
#include "E16ANA_Geometry.hh"

#include <TVector2.h>
#include <TVector3.h>
#include <TTree.h>

class E16ANA_AlignTree {
public:
   E16ANA_AlignTree();
   ~E16ANA_AlignTree();
   bool GTRHitSelection(E16ANA_G4Detector<E16ANA_G4Hit> &gtr, int detector_id, E16ANA_G4Hit &hit_result);
   void SetBranchAddress(
         double *xa, double *xb, double *xc,
         double *ya, double *yb, double *yc);
   void AppendFromG4Data(E16ANA_G4OutputData *g4data, int module_id);
   int GetEntries(){return tree->GetEntries();};
   void GetEntry(int n){tree->GetEntry(n);};

private:
   TTree *tree;
   double mxa2;
   double mxb2;
   double mxc2;
   double mya2;
   double myb2;
   double myc2;
   double smear_x;
   double smear_y;

};

class E16ANA_AlignmentV0 {
   // Alignment version 0 (Morino method)
   // Align Layer-0,1,2 by using wire target-1 and target-2 data
public:
   E16ANA_AlignmentV0(E16ANA_GeometryV1 *_geom, int _module_id);
   ~E16ANA_AlignmentV0();

   void SetData1(E16ANA_AlignTree *tree, const TVector3 &target_pos){
      tree1 = tree;
      target_pos1 = target_pos;
   };
   void SetData2(E16ANA_AlignTree *tree, const TVector3 &target_pos){
      tree2 = tree;
      target_pos2 = target_pos;
   };
   void SetLocalYCut(double y0, double y1){ycut_min = y0; ycut_max = y1;};
   void SetPrintLevel(int level){print_level = level;};

   void AlignZX(double dtheta); // Alignment Z-X only
   void Align(); // Alignment Z-X-Theta

   double GetDeltaTheta(){return delta_theta;};
   void GetParametersL0(double &dx, double &dz){
      dx = delta_x[0];
      dz = delta_z[0];
   };
   void GetParametersL1(double &dx, double &dz){
      dx = delta_x[1];
      dz = delta_z[1];
   };
   void GetParametersL2(double &dx, double &dz){
      dx = delta_x[2];
      dz = delta_z[2];
   };
   void GetParameters(double &dx1, double &dx2, double &dx3,
         double &dz1, double &dz2, double &dz3, double &dtheta){
      dx1 = delta_x[0];
      dx2 = delta_x[1];
      dx3 = delta_x[2];
      dz1 = delta_z[0];
      dz2 = delta_z[1];
      dz3 = delta_z[2];
      dtheta = delta_theta;
   };

   TVector2 GetAlignedCenter(int layer_id){
      TVector2 aligned_dc(delta_x[layer_id], geom->GTRz[layer_id][module_id]+delta_z[layer_id]);
      aligned_dc = aligned_dc.Rotate(delta_theta);
      return aligned_dc;
   };

private:
   E16ANA_GeometryV1 *geom;
   int module_id;

   E16ANA_AlignTree *tree1;
   E16ANA_AlignTree *tree2;
   TVector3 target_pos1;
   TVector3 target_pos2;

   double ycut_min; // local_y[2] cut
   double ycut_max; // ycut_min < local_y[2] < ycut_max

   int print_level;

   // aligned parameters
   double delta_x[3];
   double delta_z[3];
   double delta_theta;

   void ZAlignment(double xvoff, double zvoff, E16ANA_AlignTree *tree,
         E16ANA_GeometryV1 *_geom, int _module_id, int l1, int l3,
         double dtheta, double &z1z3);

   void XAlignment(
         double xvoff, double zvoff, E16ANA_AlignTree *tree,
         E16ANA_GeometryV1 *_geom, int _module_id, int l1, int l3,
         double dtheta, double dz1, double dz3,
         double &mean, double &sigma);

   void ThetaAlignment(
         double xvoff, double zvoff, E16ANA_AlignTree *tree,
         E16ANA_GeometryV1 *_geom, int _module_id, int l1, int l3,
         double dtheta, double dx1, double dx3, double dz1, double dz3,
         double &mean);

   void ZXAlignment(
         double xvoff1, double zvoff1, double xvoff2, double zvoff2,
         E16ANA_AlignTree *tree1, E16ANA_AlignTree *tree2,
         E16ANA_GeometryV1 *_geom, int _module_id, int l1, int l3,
         double dtheta,
         double &aligned_dx1, double &aligned_dx3, double &aligned_dz1, double &aligned_dz3);

};

#endif // E16ANA_AlignmentV0_hh

