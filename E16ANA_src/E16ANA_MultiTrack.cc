//2016-11-22, uploaded by nakai
//2016-05-02, uploaded by nakai
//2016-04-01, uploaded by nakai
#include "E16ANA_MultiTrack.hh"

const double E16ANA_MultiTrack::chisq_sigma_min = 1.0e-4; // 0.1 um

E16ANA_MultiTrack::E16ANA_MultiTrack(E16ANA_MagneticFieldMap *_bfield_map, E16ANA_GeometryV2 *_goem, int _n_tracks) : 
   bfield_map(_bfield_map), geom(_goem), n_tracks(_n_tracks),
   rungekutta_step_size(1.0), // 1.0 mm
   print_level(-1),
   momentum_step_size(0.01),
   vertex_sigma(1.0, 1.0, 0.0)
{
   GTRlPos = new std::vector<LayerHits_t>[n_tracks];
   track_charge = new double[n_tracks];
   momentum_init = new TVector3[n_tracks];
   momentum_fit = new TVector3[n_tracks];
   track_steps = new std::vector<TVector3>[n_tracks];
   for(int i=0; i<n_tracks; i++){
      track_steps[i].reserve(2000);
   }
}

E16ANA_MultiTrack::~E16ANA_MultiTrack(){
   this->Clear();
   delete [] GTRlPos;
   delete [] track_charge;
   delete [] momentum_init;
   delete [] momentum_fit;
   delete [] track_steps;
}

void E16ANA_MultiTrack::Clear(){
   for(int i=0; i<n_tracks; i++){
      for(int j=0; j<(int)GTRlPos[i].size(); j++){
         GTRlPos[i][j].Delete();
      }
      GTRlPos[i].clear();
   }
}

void E16ANA_MultiTrack::SetCharge(int track_id, double charge){
   if(track_id >= n_tracks){
      std::cout << "E16ANA_MultiTrack::SetCharge() : Error."
                << " TrackID = " << track_id << std::endl;
      return;
   }
   track_charge[track_id] = charge;
}

void E16ANA_MultiTrack::SetInitialMomentum(int track_id, const TVector3 &mom){
   if(track_id >= n_tracks){
      std::cout << "E16ANA_MultiTrack::SetInitialMomentum() : Error."
                << " TrackID = " << track_id << std::endl;
      return;
   }
   momentum_init[track_id] = mom;
}

void E16ANA_MultiTrack::AddHit(int track_id, int layer_order,
      const E16ANA_DetectorGeometry *_geom,
      const TVector3 &lpos, const TVector3 &sigma){

   if(track_id >= n_tracks){
      std::cout << "E16ANA_MultiTrack::AddHit() : Error."
                << " TrackID = " << track_id
                << std::endl;
      return;
   }
   int array_index = -1;
   while(layer_order >= (int)GTRlPos[track_id].size()){
      array_index = (int)GTRlPos[track_id].size();
      GTRlPos[track_id].push_back(LayerHits_t(array_index, -1)); // Set Layer ID
      //GTRlPos[track_id].push_back(LayerHits_t(array_index, _geom->GetLayerID())); // Set Layer ID
   }
   std::vector<ModuleHits_t> &lhits = GTRlPos[track_id][layer_order].local_hits;
   array_index = -1;
   //std::cout << "array_index = " << array_index << std::endl;
   lhits.push_back(ModuleHits_t());
   lhits.back().first = _geom->GetModuleID(); // Set Module ID
   array_index = lhits.size()-1; // last
   //lhits[array_index].second.push_back(new Hit_t(lpos, sigma, _geom));
   lhits.back().second.push_back(new Hit_t(lpos, sigma, _geom));
}

void E16ANA_MultiTrack::AddHit(int track_id, int layer_order,
      int layer_id, int module_id,
      const TVector3 &lpos, const TVector3 &sigma){

   const int n_modules = 33;
   if(track_id >= n_tracks || module_id >= n_modules){
      std::cout << "E16ANA_MultiTrack::AddHit() : Error."
                << " TrackID = " << track_id
                << ", ModuleID = " << module_id << std::endl;
      return;
   }
   int array_index = -1;
   while(layer_order >= (int)GTRlPos[track_id].size()){
      array_index = (int)GTRlPos[track_id].size();
      GTRlPos[track_id].push_back(LayerHits_t(array_index, layer_id)); // Set Layer ID
   }
   std::vector<ModuleHits_t> &lhits = GTRlPos[track_id][layer_order].local_hits;
   array_index = -1;
   for(int i=0; i<(int)lhits.size(); i++){
      int mid_temp = lhits[i].first;
      if(module_id == mid_temp){
         array_index = i;
      }
   }
   //std::cout << "array_index = " << array_index << std::endl;
   if(array_index < 0){
      lhits.push_back(ModuleHits_t());
      lhits.back().first = module_id; // Set Module ID
      array_index = lhits.size()-1;
   }
   //lhits[array_index].second.push_back(new Hit_t(lpos, sigma, geom->GetPlane(layer_id, module_id, lpos.Z())));
   lhits[array_index].second.push_back(new Hit_t(lpos, sigma, geom->GTR(module_id, layer_id)));
}

/* Equation Of Motion */
void E16ANA_MultiTrack::EOM(double mom, double r[], double dr[], double d2r[]){
   int jj[3]={1,2,0};
   int kk[3]={2,0,1};
   double B[3];
   double point[3];
   for (int i=0;i<3;i++) point[i]=r[i]/10.0; // mm -> cm
   bfield_map->GetFieldValue(point, B, 0);
   for(int i=0; i<3; i++){
      //d2r[i]=0.299792458e-6/mom*(dr[jj[i]]*B[kk[i]]-dr[kk[i]]*B[jj[i]]); // d2r [1/cm]
      d2r[i]=0.299792458e-7/mom*(dr[jj[i]]*B[kk[i]]-dr[kk[i]]*B[jj[i]]); // d2r [1/mm]
      //d2r[i]=c*1e-2/(mom*1e9)*(dr[jj[i]]*B[kk[i]]*1e-4-dr[kk[i]]*B[jj[i]]*1e-4);
      //1/cm     cm->m     GeV->eV                  Gauss->Tesla
   }
}

/*A 4th order Runge-Kutta program.*/
void E16ANA_MultiTrack::rkn4(double mom, double y[], double dydx[], double h,
      double yout[], double dydxout[]){

   //double h2,h6,h28,dym[3],dyt[3],yt[3];
   double h2,h6,h28,dyt[3],yt[3];
   double k1[3],k2[3],k3[3],k4[3];

   //cout << y[0] <<" "<<y[1]<<" "<<y[2]<<endl;
   h2=h/2.;
   h6=h/6.;
   h28=h*h/8;
   EOM(mom,y,dydx,k1);
   for (int i=0;i<3;i++){
      yt[i]=y[i]+h2*dydx[i]+h28*k1[i];
      dyt[i]=dydx[i]+h2*k1[i];
   }
   EOM(mom,yt,dyt,k2);
   //for (int i=0;i<3;i++) dyt[i]=dydx[i]+h2*k2[i];
   for (int i=0;i<3;i++) {
      yt[i]=y[i]+h2*dydx[i]+h28*k1[i];
      dyt[i]=dydx[i]+h2*k2[i];
   }
   EOM(mom,yt,dyt,k3);
   for (int i=0;i<3;i++){
      yt[i]=y[i]+h*dydx[i]+4*h28*k3[i];
      dyt[i]=dydx[i]+h*k3[i];
   }
   EOM(mom,yt,dyt,k4);
   for (int i=0;i<3;i++){
      dydxout[i]=dydx[i]+h6*(k1[i]+2*(k2[i]+k3[i])+k4[i]);
      yout[i]=y[i]+h*dydx[i]+h*h6*(k1[i]+k2[i]+k3[i]);
   }
}

double E16ANA_MultiTrack::RungeKuttaTracking(int track_id, double dstp, double mom, double r0[], double dr0[]){
   track_steps[track_id].clear();
   double r1[3];
   double dr1[3];
   double chisq = 0.0;
   int layer_id, module_id;
   std::vector<LayerHits_t> &hits_in_track = GTRlPos[track_id];
   for(int layer_order=0; layer_order<(int)hits_in_track.size(); layer_order++){ // layer_order
      LayerHits_t hits_in_next_layer = hits_in_track[layer_order]; // copy
      layer_id = hits_in_next_layer.layer_id;
      //std::cout << "vector length = " << hits_in_next_layer.local_hits.size() << std::endl;
      //for(int i=0; i<hits_in_next_layer.NumHitModules(); i++){
      //   std::cout << "    module id = " << hits_in_next_layer.ModuleId(i) << std::endl;
      //}
      //std::cout << "Layer order = " << layer_order << std::endl;
      while(!hits_in_next_layer.Empty()){
         int module_index = RungeKuttaTrackingToNextHit(track_id, hits_in_next_layer, dstp, mom, r0, dr0, r1, dr1);
         if(module_index < 0){ // If RungeKuttaTrackingToNextHit is exited with error.
            chisq = 1.0e+10;
            //momentum_fit[track_id].SetXYZ(-10000.0,-10000.0,-10000.0);
            break;
         }
         module_id = hits_in_next_layer.ModuleId(module_index);
         //std::cout << "Module ID = " << module_id << std::endl;
         std::vector<Hit_t*> &hits_in_chamber = hits_in_next_layer.HitsInModule(module_index);
         std::vector<Hit_t*>::iterator it = hits_in_chamber.begin();
         //std::cout << "Num hits in one chamber = " << hits_in_chamber.size() << std::endl;
         while(it != hits_in_chamber.end()){
            TVector3 lpos_cp;
            //if(IsCrossed((*it)->plane, r0, r1, cp)){
            if((*it)->IsCrossed(r0, r1, lpos_cp)){
               //TVector3 lpos_cp = geom->GetLPos(cp, layer_id, module_id);
               TVector3 gmom_cp(fabs(mom)*dr0[0], fabs(mom)*dr0[1], fabs(mom)*dr0[2]);
               // Linear interpolate is needed ?
               //TVector3 lmom_cp = E16ANA_Transform::GetLMom(gmom_cp,geom->GTRtheta[module_id],0.,0.,0.);
               TVector3 lmom_cp = (*it)->geometry->GetLMom(gmom_cp);
               chisq += (*it)->Chisquare(lpos_cp, lmom_cp);
               //if((*it)->sigma.X() > chisq_sigma_min){
                  //std::cout << "    G4 Hit  : "; (*it)->lpos.Print();
                  //std::cout << "    Fit Hit : "; lpos_cp.Print();
                  //std::cout << "    Chisquare = " << chisq << std::endl;
               if(print_level >= 2){
                  std::cout << "    Ana : (" << (*it)->lpos.X() << ", " << (*it)->lpos.Y() << ", " << (*it)->lpos.Z() << ")"
                     << ", Fit : (" << lpos_cp.X() << ", " << lpos_cp.Y() << ", " << lpos_cp.Z() << ")" << std::endl;
               }
               it = hits_in_chamber.erase(it);
            }else{
               it++;
            }
         }
         if(hits_in_chamber.empty()){
            hits_in_next_layer.Erase(module_index);
         }
      }
   }
   return chisq;
}

int E16ANA_MultiTrack::RungeKuttaTrackingToNextHit(int track_id, LayerHits_t &next_layer_hits, double dstp, double mom,
      double r0[], double dr0[],
      double r1[], double dr1[]){

   int module_index = -1;
   int n_steps = 0;
   TVector3 cross_point;
   while(1){
      rkn4(mom, r0, dr0, dstp, r1, dr1);
      //std::cout << "r1 = (" << r1[0] << ", " << r1[1] << ", " << r1[2] << ")" << std::endl;
      bool is_crossed = false;
      for(int i=0; i<next_layer_hits.NumHitModules(); i++){
         //if(IsCrossed(next_layer_hits.FirstPlaneInModule(i), r0, r1, cross_point)){
         if(next_layer_hits.FirstHitInModule(i)->IsCrossed(r0, r1, cross_point)){
            is_crossed = true;
            module_index = i;
         }
      }
      if(is_crossed){
         // success
         break;
      }else if(!IsTrackingRegion(r1)){
         if(print_level >= 1){
            std::cout << "E16ANA_MultiTrack::RungeKuttaTrackingToNextHit : Out of tracking region." << std::endl;
         }
         break;
      }else if(n_steps > 10000){
         if(print_level >= 1){
            std::cout << "E16ANA_MultiTrack::RungeKuttaTrackingToNextHit : Excess max number of steps." << std::endl;
         }
         break;
      }
      track_steps[track_id].push_back(TVector3(r0[0],r0[1],r0[2]));
      for(int i=0; i<3; i++){ // update
         r0[i] = r1[i];
         dr0[i] = dr1[i];
      }
      n_steps++;
   }
   return module_index;
}

#if 0
bool E16ANA_MultiTrack::IsCrossed(Plane_t &plane, double r0[], double r1[], TVector3 &cross_point){
   ROOT::Math::Plane3D::Point p0(r0[0], r0[1], r0[2]);
   ROOT::Math::Plane3D::Point p1(r1[0], r1[1], r1[2]);
   double dist0 = plane.Distance(p0);
   double dist1 = plane.Distance(p1);
   if((dist0*dist1) > 0.0){
      return false;
   }
   ROOT::Math::Plane3D::Point cp = p0+(p1-p0)*fabs(dist0)/(fabs(dist0)+fabs(dist1));
   cross_point.SetXYZ(cp.X(), cp.Y(), cp.Z());
   return true;
}
#endif

void E16ANA_MultiTrack::PreConditioning(){
   // Pre-conditioning (local_z sort)
   for(int i=0; i<n_tracks; i++){
      std::vector<LayerHits_t> &hits_in_track = GTRlPos[i];
      for(int j=0; j<(int)hits_in_track.size(); j++){
         hits_in_track[j].Sort();
      }
   }
}

double E16ANA_MultiTrack::Fit(bool vertexfixflag, bool pyfixflag){
   PreConditioning();

   //TMinuitMinimizer *minuit = new TMinuitMinimizer(ROOT::Minuit::kMigrad);
   TMinuitMinimizer *minuit = new TMinuitMinimizer(ROOT::Minuit::kMigradImproved);
   E16ANA_MultiTrackFunction func(this);
   minuit->SetFunction(func);
   minuit->SetPrintLevel(-1);
   minuit->SetStrategy(2);
   minuit->SetMaxFunctionCalls(1.0e+4);
   minuit->SetTolerance(1.0e-5);

   double vtxz = vertex_init.Z();
   double inital_value;
   //double step;
   double upper_limit;
   double lower_limit;

   if(vertexfixflag){
      minuit->SetFixedVariable(0, "Vertex_X", vertex_init.X());
      minuit->SetFixedVariable(1, "Vertex_Y", vertex_init.Y());
   }else{
      minuit->SetLimitedVariable(0, "Vertex_X", vertex_init.X(), 0.1, -20.0, 20.0); // mm
      if(pyfixflag){
         minuit->SetFixedVariable(1, "Vertex_Y", vertex_init.Y());
      }else{
         minuit->SetLimitedVariable(1, "Vertex_Y", vertex_init.Y(), 0.1, -20.0, 20.0); // mm
      }
   }
   if(vtxz > 10.0){ // mm
      inital_value = 20.0;
      upper_limit = 25.0;
      lower_limit = 15.0;
   }else if(vtxz > -10.0){
      inital_value = 0.0;
      upper_limit = 5.0;
      lower_limit = -5.0;
   }else{
      inital_value = -20.0;
      upper_limit = -15.0;
      lower_limit = -25.0;
   }
   //minuit->SetLimitedVariable(2, "Vertex_Z", inital_value, 0.00, lower_limit, upper_limit);
   minuit->SetFixedVariable(2, "Vertex_Z", inital_value);
   for(int i=0; i<n_tracks; i++){
      minuit->SetLimitedVariable(i*3+3, Form("Momentum%02d_X", i),
            momentum_init[i].X(), momentum_step_size, -1.5, 1.5); // GeV
            //-track_charge[i]*momentum_init[i].X(), momentum_step_size, -1.5, 1.5); // 160928
      if(pyfixflag){
         minuit->SetFixedVariable(i*3+4, Form("Momentum%02d_Y", i), momentum_init[i].Y());
      }else{
         minuit->SetLimitedVariable(i*3+4, Form("Momentum%02d_Y", i),
               momentum_init[i].Y(), momentum_step_size, -1.5, 1.5); // GeV
      }
      minuit->SetLimitedVariable(i*3+5, Form("Momentum%02d_Z", i),
            momentum_init[i].Z(), momentum_step_size, -0.3, 6.0); // GeV
   }
   minimize_status = minuit->Minimize();
   matrix_status = minuit->CovMatrixStatus();
   double chisq = minuit->MinValue();
   vertex_fit.SetXYZ(
         minuit->X()[0],
         minuit->X()[1],
         minuit->X()[2]
         );
   for(int i=0; i<n_tracks; i++){
      momentum_fit[i].SetXYZ(
            minuit->X()[i*3+3],
            //-track_charge[i]*minuit->X()[i*3+3], // 160928
            minuit->X()[i*3+4],
            minuit->X()[i*3+5]
            );
   }
   if(chisq > 0.9e+10){
      for(int i=0; i<n_tracks; i++){
         //momentum_fit[i].SetXYZ(-10000.0, -10000.0, -10000.0);
         momentum_fit[i].SetXYZ(kInvalidValue, kInvalidValue, kInvalidValue);
      }
   }

   delete minuit;
   return chisq;
}

double E16ANA_MultiTrack::CalcVertexChisquare(){
   //double dx = (vertex_fit.X()-vertex_init.X())/vertex_sigma.X();
   //double dy = (vertex_fit.Y()-vertex_init.Y())/vertex_sigma.Y();
   //double dz = (vertex_fit.Z()-vertex_init.Z())/vertex_sigma.Z();
   double dx = (vertex_fit.X())/vertex_sigma.X();
   double dy = (vertex_fit.Y())/vertex_sigma.Y();
   double dz = (vertex_fit.Z())/vertex_sigma.Z();
   if(vertex_sigma.X() < chisq_sigma_min) dx = 0.0;
   if(vertex_sigma.Y() < chisq_sigma_min) dy = 0.0;
   if(vertex_sigma.Z() < chisq_sigma_min) dz = 0.0;
   return dx*dx+dy*dy+dz*dz;
}

double E16ANA_MultiTrack::RungeKuttaTracking(int track_id, const TVector3 &vertex, const TVector3 &momentum, double charge){
   double r0[3] = {vertex.X(), vertex.Y(), vertex.Z()};
   double dstp = rungekutta_step_size;
   double mom = momentum.Mag();
   double dr0[3] = {momentum.X()/mom, momentum.Y()/mom, momentum.Z()/mom};
   return RungeKuttaTracking(track_id, dstp, mom*charge, r0, dr0);
}

double E16ANA_MultiTrack::MinuitFunction(const double *pars){
   double chisq = 0.0;
   vertex_fit.SetXYZ(pars[0], pars[1], pars[2]);
   for(int i=0; i<n_tracks; i++){
      momentum_fit[i].SetXYZ(pars[i*3+3], pars[i*3+4], pars[i*3+5]);
      //momentum_fit[i].SetXYZ(-track_charge[i]*pars[i*3+3], pars[i*3+4], pars[i*3+5]); // 160928
      chisq += RungeKuttaTracking(i, vertex_fit, momentum_fit[i], track_charge[i]);
   }
   chisq += CalcVertexChisquare();
   return chisq;
}

void E16ANA_MultiTrack::PrintHits(LayerHits_t &layer){
   std::cout << "    layer_order = " << layer.layer_order << ", # of modules = " << layer.NumHitModules() << std::endl;
   for(int i=0; i<(int)layer.NumHitModules(); i++){
      std::vector<Hit_t*> &hits = layer.HitsInModule(i);
      std::cout << "      module_id = " << layer.ModuleId(i) << ", # of hits = " << hits.size() << std::endl;
      for(int j=0; j<(int)hits.size(); j++){
         Hit_t *hit = hits[j];
         std::cout << "        lpos = (" << hit->lpos.X() << ", " << hit->lpos.Y() << ", " << hit->lpos.Z() << ")" << std::endl;
      }
   }
}

void E16ANA_MultiTrack::PrintHits(){
   std::cout << "E16ANA_MultiTrack::PrintHits() : # of tracks = " << n_tracks << std::endl;
   for(int i=0; i<n_tracks; i++){
      std::cout << "  track_id = " << i << ", # of layers = " << GTRlPos[i].size() << std::endl;
      for(int j=0; j<(int)GTRlPos[i].size(); j++){
         PrintHits(GTRlPos[i][j]);
      }
   }
}

ROOT::Math::IMultiGenFunction* E16ANA_MultiTrackFunction::Clone() const {
   return new E16ANA_MultiTrackFunction(this->multi_track);
}

