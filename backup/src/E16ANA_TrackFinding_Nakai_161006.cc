//2016-11-22, uploaded by nakai

void E16ANA_TrackFinding_Nakai::TrackFinding(){
   SetG4HitSSD();
   for(int module=0; module<n_gtr_modules; module++){
      SetG4Hit(module);
   }
   //for(int i=0; i<n_ssd_modules; i++){
   //   SetG4ClosestTagSSD(ssd_res[i]->AnalyzedHits(), 0.1);
   //}
   int noh = g4event->GTR3().Noh();
   int gtr3_multiplicity[n_gtr_modules] = {};
   int gtr3_multiplicity_e[n_gtr_modules] = {};
   int gtr3_multiplicity_p[n_gtr_modules] = {};

   int gtr3_minid_e = -1;
   int gtr3_minid_p = -1;
   double gtr3_minlx_e = 10000.0;
   double gtr3_minlx_p = 10000.0;

   for(int i=0; i<noh; i++){
      E16ANA_G4Hit &h = g4event->GTR3().Hit(i);
      if(h.ModuleID() == 16) continue;
      if(h.PID() == 11 || h.PID() == -11){
         gtr3_multiplicity[h.ModuleID()]++;
      }
      if(h.PID() == 11){
         if(fabs(h.X()) < gtr3_minlx_e){
            gtr3_minlx_e = fabs(h.X());
            gtr3_minid_e = h.ModuleID();
         }
         //gtr3_multiplicity_e[h.ModuleID()]++;
      }
      if(h.PID() == -11){
         if(fabs(h.X()) < gtr3_minlx_p){
            gtr3_minlx_p = fabs(h.X());
            gtr3_minid_p = h.ModuleID();
         }
         //gtr3_multiplicity_p[h.ModuleID()]++;
      }
   }
   if(gtr3_minid_e >= 0){
      gtr3_multiplicity_e[gtr3_minid_e] = 1;
   }
   if(gtr3_minid_p >= 0){
      gtr3_multiplicity_p[gtr3_minid_p] = 1;
   }

   std::vector<std::vector<E16ANA_SingleTrackCandidate_Nakai> > cand_plus, cand_minus;
   std::vector<std::vector<E16ANA_SingleTrackCandidate_Nakai> > cand_y;

   //const double cluster_width_cut = 5.5*0.35; // default
   const double cluster_width_cut = 6.5*0.35; // ?
   //const double cluster_width_cut = 1000.0;

   for(int module=0; module<n_gtr_modules; module++){
      if(gtr3_multiplicity_p[module] > 0){ // only G4 Answer module
         //SetG4Hit(module);
         SetG4ClosestTag(gtr_ana->AnalyzedHitXChamber(0,module),0,0,0.1); // COG
         SetG4ClosestTag(gtr_ana->AnalyzedHitXChamber(1,module),0,0,0.1); // COG
         SetG4ClosestTag(gtr_ana->AnalyzedHitXChamber(2,module),0,0,0.1); // COG
         cand_plus.push_back(std::vector<E16ANA_SingleTrackCandidate_Nakai>());
         cand_minus.push_back(std::vector<E16ANA_SingleTrackCandidate_Nakai>());
         std::vector<int> ssd_ids = LinkedModuleIDs(module);
         for(int i=0; i<(int)ssd_ids.size(); i++){
            // SSD x GTR2
            GetSingleTrackCandidatesNonPolluted(ssd_ids[i], module, 1, cand_plus.back(), cand_minus.back(), cluster_width_cut);
            // SSD x GTR3
            GetSingleTrackCandidatesNonPolluted(ssd_ids[i], module, 2, cand_plus.back(), cand_minus.back(), cluster_width_cut);
         }
         // GTR1 x GTR2 x GTR3 (Y-strip)
         //cand_y.push_back(std::vector<E16ANA_SingleTrackCandidate_Nakai>());
         //GetSingleTrackCandidatesY(module, cand_y.back());
         //GetSingleTrackCandidatesV4Y(module, cand_y.back());
         // minus erase
         cand_minus.back().clear();
      }
   }
   for(int module=0; module<n_gtr_modules; module++){
      if(gtr3_multiplicity_e[module] > 0){ // only G4 Answer module
         //SetG4Hit(module);
         SetG4ClosestTag(gtr_ana->AnalyzedHitXChamber(0,module),0,0,0.1); // COG
         SetG4ClosestTag(gtr_ana->AnalyzedHitXChamber(1,module),0,0,0.1); // COG
         SetG4ClosestTag(gtr_ana->AnalyzedHitXChamber(2,module),0,0,0.1); // COG
         cand_plus.push_back(std::vector<E16ANA_SingleTrackCandidate_Nakai>());
         cand_minus.push_back(std::vector<E16ANA_SingleTrackCandidate_Nakai>());
         std::vector<int> ssd_ids = LinkedModuleIDs(module);
         for(int i=0; i<(int)ssd_ids.size(); i++){
            // SSD x GTR2
            GetSingleTrackCandidatesNonPolluted(ssd_ids[i], module, 1, cand_plus.back(), cand_minus.back(), cluster_width_cut);
            // SSD x GTR3
            GetSingleTrackCandidatesNonPolluted(ssd_ids[i], module, 2, cand_plus.back(), cand_minus.back(), cluster_width_cut);
         }
         // GTR1 x GTR2 x GTR3 (Y-strip)
         //cand_y.push_back(std::vector<E16ANA_SingleTrackCandidate_Nakai>());
         //GetSingleTrackCandidatesY(module, cand_y.back());
         //GetSingleTrackCandidatesV4Y(module, cand_y.back());
         // plus erase
         cand_plus.back().clear();
      }
   }

   std::cout << "Before HBD cut" << std::endl;
   std::cout << "SSD x GTR2/3 (plus)  : "; PrintNumberOfCandidates(cand_plus );
   std::cout << "SSD x GTR2/3 (minus) : "; PrintNumberOfCandidates(cand_minus);
   std::cout << std::endl;

   CheckHBDMatch(cand_plus,  1, 70.0);
   CheckHBDMatch(cand_minus, 1, 70.0);

   std::cout << "After HBD cut" << std::endl;
   std::cout << "SSD x GTR2/3 (plus)  : "; PrintNumberOfCandidates(cand_plus );
   std::cout << "SSD x GTR2/3 (minus) : "; PrintNumberOfCandidates(cand_minus);
   std::cout << std::endl;

   //std::cout << "Number of single track candidates (SSD x GTR2/3) = " << cand_minus.size() << std::endl;
   //std::vector<E16ANA_SingleTrackCandidate_Nakai> cand2_plus, cand2_minus;
   std::cout << "GetSingleTrackCandidatesNonPolluted and GetSingleTrackCandidatesY done." << std::endl << std::endl;
#if 0
   for(int i=0; i<(int)cand_plus.size(); i++){
      std::cout << "Number of single track candidates (plus, SSD x GTR2/3) = " << cand_plus[i].size() << std::endl;
      for(int j=0; j<(int)cand_plus[i].size(); j++){
         cand_plus[i][j].Print();
      }
   }
   for(int i=0; i<(int)cand_minus.size(); i++){
      std::cout << "Number of single track candidates (minus, SSD x GTR2/3) = " << cand_minus[i].size() << std::endl;
      for(int j=0; j<(int)cand_minus[i].size(); j++){
         cand_minus[i][j].Print();
      }
   }
   for(int i=0; i<(int)cand_y.size(); i++){
      std::cout << "Number of single track candidates (y, GTR1 x 2 x 3) = " << cand_y[i].size() << std::endl;
      for(int j=0; j<(int)cand_y[i].size(); j++){
         cand_y[i][j].Print();
      }
   }
#endif
   CheckG4ClosestLayer2or3(cand_plus , ranking_xselection_plus , hbd_proj_xselection_plus , 1);
   CheckG4ClosestLayer2or3(cand_minus, ranking_xselection_minus, hbd_proj_xselection_minus, 1);
   for(int i=0; i<(int)ranking_xselection_plus.size(); i++){
      std::cout << ranking_xselection_plus[i] << "-th candidate (plus) is G4 closest" << std::endl;
      std::cout << ranking_xselection_minus[i] << "-th candidate (minus) is G4 closest" << std::endl;
   }


   std::vector<std::vector<E16ANA_SingleTrackCandidate_Nakai> > cand2_plus, cand2_minus;
   cand2_plus.resize(cand_plus.size());
   cand2_minus.resize(cand_minus.size());
   SelectOtherLayerHits(cand_plus, cand2_plus);
   SelectOtherLayerHits(cand_minus, cand2_minus);
   //SelectOtherLayerHits(cand_plus, cand2_plus, false);
   //SelectOtherLayerHits(cand_minus, cand2_minus, false);
   SingleFitAllCandidates(cand2_plus , false, true);
   SingleFitAllCandidates(cand2_minus, false, true);
   //ChisqCut(cand2_plus , 1000000.0);
   //ChisqCut(cand2_minus, 1000000.0);

   std::cout << "Before HBD cut" << std::endl;
   std::cout << "SSD x GTR1 x GTR2 x GTR3 (plus)  : "; PrintNumberOfCandidates(cand2_plus );
   std::cout << "SSD x GTR1 x GTR2 x GTR3 (minus) : "; PrintNumberOfCandidates(cand2_minus);
   std::cout << std::endl;

   CheckHBDMatch(cand2_plus,  1, 40.0);
   CheckHBDMatch(cand2_minus, 1, 40.0);

   std::cout << "After HBD cut" << std::endl;
   std::cout << "SSD x GTR1 x GTR2 x GTR3 (plus)  : "; PrintNumberOfCandidates(cand2_plus );
   std::cout << "SSD x GTR1 x GTR2 x GTR3 (minus) : "; PrintNumberOfCandidates(cand2_minus);
   std::cout << std::endl;

   //std::cout << "Number of single track candidates (SSD x GTR1 x GTR2 x GTR3) = " << cand2_plus.size() << std::endl;
   //std::cout << "Number of single track candidates (SSD x GTR1 x GTR2 x GTR3) = " << cand2_minus.size() << std::endl;

   for(int m=0; m<(int)cand2_plus.size(); m++){
      int loops;
      loops = std::min(10, (int)cand2_plus[m].size());
      for(int i=0; i<loops; i++){
         std::cout << i << "-th Chisq candidate (plus) : ";
         cand2_plus[m][i].Print();
      }
      loops = std::min(10, (int)cand2_minus[m].size());
      for(int i=0; i<loops; i++){
         std::cout << i << "-th Chisq candidate (minus) : ";
         cand2_minus[m][i].Print();
      }
   }
   CheckG4Closest(cand2_plus , ranking_xyselection_plus , hbd_proj_xyselection_plus , 1);
   CheckG4Closest(cand2_minus, ranking_xyselection_minus, hbd_proj_xyselection_minus, 1);
   for(int i=0; i<(int)ranking_xyselection_plus.size(); i++){
      std::cout << ranking_xyselection_plus[i] << "-th candidate (plus) is G4 closest" << std::endl;
      std::cout << ranking_xyselection_minus[i] << "-th candidate (minus) is G4 closest" << std::endl;
   }
   std::cout << "SelectOtherLayerHits done." << std::endl << std::endl;
#if 0
   exist_best = false;
   if((int)ranking_xyselection_plus.size() >= 2 && ranking_xyselection_minus.size() >= 2){
      int m0 = -100;
      int m1 = -100;
      m0 = ranking_xyselection_plus [0];
      m1 = ranking_xyselection_minus[1];
      if(  m0 >= 0 && m0 < (int)cand2_plus [0].size()
        && m1 >= 0 && m1 < (int)cand2_minus[1].size()){
         exist_best = true;
         best_candidate = E16ANA_DoubleTrackCandidate_Nakai(
                              cand2_plus[0][m0], cand2_minus[1][m1]);
      }
   }

   return;
#endif
   std::vector<std::vector<E16ANA_SingleTrackCandidate_Nakai> > cand_xy_plus;
   std::vector<std::vector<E16ANA_SingleTrackCandidate_Nakai> > cand_xy_minus;
   cand_xy_plus.resize(cand2_plus.size());
   cand_xy_minus.resize(cand2_minus.size());

   // Y part
   cand_y.resize(cand2_plus.size());
   for(int i=0; i<(int)cand2_plus.size(); i++){
      if(!cand2_plus[i].empty()){
         int m[4];
         //cand2_plus[i][ranking_xyselection_plus[i]].GetModuleIDs(m);
         cand2_plus[i][0].GetModuleIDs(m);
         //GetSingleTrackCandidatesY(m[1], m[2], m[3], cand_y[i]);
         GetSingleTrackCandidatesV4Y(m[1], m[2], m[3], cand_y[i]);
      }
      if(!cand2_minus[i].empty()){
         int m[4];
         //cand2_minus[i][ranking_xyselection_minus[i]].GetModuleIDs(m);
         cand2_minus[i][0].GetModuleIDs(m);
         //GetSingleTrackCandidatesY(m[1], m[2], m[3], cand_y[i]);
         GetSingleTrackCandidatesV4Y(m[1], m[2], m[3], cand_y[i]);
      }
   }

   for(int i=0; i<(int)cand_y.size(); i++){
      ChisqSort(cand_y[i]);
   }
   CheckG4Closest(cand_y, ranking_yselection, hbd_proj_yselection, 2);
   for(int i=0; i<(int)ranking_yselection.size(); i++){
      std::cout << ranking_yselection[i] << "-th candidate (Y) is G4 closest" << std::endl;
   }
   std::cout << std::endl;

   for(int i=0; i<(int)cand_y.size(); i++){
      //int loopy = std::min(20, (int)cand_y[i].size());
      int loopy = (int)cand_y[i].size();
      //for(int j=0; j<(int)cand_y[i].size(); j++){
      for(int j=0; j<loopy; j++){
         int loops;
         loops = std::min(20, (int)cand2_plus[i].size());
         for(int k=0; k<loops; k++){
            E16ANA_SingleTrackCandidate_Nakai &x = cand2_plus[i][k];
            E16ANA_SingleTrackCandidate_Nakai &y = cand_y[i][j];
            if(E16ANA_SingleTrackCandidate_Nakai::CheckModuleID(x, y)){
               cand_xy_plus[i].push_back(
                     E16ANA_SingleTrackCandidate_Nakai::MergeXandY(x, y));
            }
         }
         loops = std::min(20, (int)cand2_minus[i].size());
         for(int k=0; k<loops; k++){
            E16ANA_SingleTrackCandidate_Nakai &x = cand2_minus[i][k];
            E16ANA_SingleTrackCandidate_Nakai &y = cand_y[i][j];
            if(E16ANA_SingleTrackCandidate_Nakai::CheckModuleID(x, y)){
               cand_xy_minus[i].push_back(
                     E16ANA_SingleTrackCandidate_Nakai::MergeXandY(x, y));
            }
         }
      }
   }
   SingleFitAllCandidates(cand_xy_plus);
   SingleFitAllCandidates(cand_xy_minus);
   //ReAnalyzeAndReFit(cand_xy_plus);
   //ReAnalyzeAndReFit(cand_xy_minus);
   //ChisqCut(cand_xy_plus, 10.0);
   //ChisqCut(cand_xy_minus, 10.0);
   CheckHBDMatch(cand_xy_plus,  3, 40.0);
   CheckHBDMatch(cand_xy_minus, 3, 40.0);

   for(int m=0; m<(int)cand_xy_plus.size(); m++){
      int loops;
      loops = std::min(10, (int)cand_xy_plus[m].size());
      for(int i=0; i<loops; i++){
         std::cout << i << "-th Chisq candidate (plus) : ";
         cand_xy_plus[m][i].Print();
      }
      loops = std::min(10, (int)cand_xy_minus[m].size());
      for(int i=0; i<loops; i++){
         std::cout << i << "-th Chisq candidate (minus) : ";
         cand_xy_minus[m][i].Print();
      }
   }
   CheckG4Closest(cand_xy_plus , ranking_xyselection_hbd_plus , hbd_proj_xyselection_hbd_plus , 3);
   CheckG4Closest(cand_xy_minus, ranking_xyselection_hbd_minus, hbd_proj_xyselection_hbd_minus, 3);
   for(int i=0; i<(int)ranking_xyselection_hbd_plus.size(); i++){
      std::cout << ranking_xyselection_hbd_plus[i] << "-th candidate (plus) is G4 closest" << std::endl;
      std::cout << ranking_xyselection_hbd_minus[i] << "-th candidate (minus) is G4 closest" << std::endl;
   }
   std::cout << "XY merge done." << std::endl << std::endl;
#if 0
   exist_best = false;
   if((int)ranking_xyselection_hbd_plus.size() >= 2 && ranking_xyselection_hbd_minus.size() >= 2){
      int m0 = -100;
      int m1 = -100;
      m0 = ranking_xyselection_hbd_plus [0];
      m1 = ranking_xyselection_hbd_minus[1];
      if(  m0 >= 0 && m0 < (int)cand_xy_plus [0].size()
        && m1 >= 0 && m1 < (int)cand_xy_minus[1].size()){
         exist_best = true;
         best_candidate = E16ANA_DoubleTrackCandidate_Nakai(
                              cand_xy_plus[0][m0], cand_xy_minus[1][m1]);
      }
   }

   return;
#endif
   //CheckHBDMatch(cand_xy_plus);
   //CheckHBDMatch(cand_xy_minus);
   //for(int m=0; m<(int)cand_xy_plus.size(); m++){
   //   int loops;
   //   loops = std::min(10, (int)cand_xy_plus[m].size());
   //   for(int i=0; i<loops; i++){
   //      std::cout << i << "-th Chisq candidate (plus) : ";
   //      cand_xy_plus[m][i].Print();
   //   }
   //   loops = std::min(10, (int)cand_xy_minus[m].size());
   //   for(int i=0; i<loops; i++){
   //      std::cout << i << "-th Chisq candidate (minus) : ";
   //      cand_xy_minus[m][i].Print();
   //   }
   //}
   //CheckG4Closest(cand_xy_plus , ranking_xyselection_hbd_plus , hbd_proj_xyselection_hbd_plus , 3);
   //CheckG4Closest(cand_xy_minus, ranking_xyselection_hbd_minus, hbd_proj_xyselection_hbd_minus, 3);
   //for(int i=0; i<(int)ranking_xyselection_hbd_plus.size(); i++){
   //   std::cout << ranking_xyselection_hbd_plus[i] << "-th candidate (plus) is G4 closest" << std::endl;
   //   std::cout << ranking_xyselection_hbd_minus[i] << "-th candidate (minus) is G4 closest" << std::endl;
   //}
   //std::cout << "HBD matching check done." << std::endl << std::endl;

   std::vector<E16ANA_DoubleTrackCandidate_Nakai> double_cand;
   //TH1F *h_vdiff_x = new TH1F("h_vdiff_x", "Vertex difference (All);X [mm]", 200, -1.0, 1.0);
   //TH1F *h_vdiff_y = new TH1F("h_vdiff_y", "Vertex difference (All);Y [mm]", 200, -1.0, 1.0);
   //TH1F *h_vdiff_g4_x = new TH1F("h_vdiff_g4_x", "Vertex difference (G4 closest);X [mm]", 200, -1.0, 1.0);
   //TH1F *h_vdiff_g4_y = new TH1F("h_vdiff_g4_y", "Vertex difference (G4 closest);Y [mm]", 200, -1.0, 1.0);


   int n_double = 0;
   for(int m0=0; m0<(int)cand_xy_plus.size(); m0++){
      for(int m1=0; m1<(int)cand_xy_minus.size(); m1++){
         if(m0 == m1){continue;}
         //int loop0 = std::min(10, (int)cand_xy_plus[m0].size());
         //int loop1 = std::min(10, (int)cand_xy_minus[m1].size());
         int loop0 = (int)cand_xy_plus[m0].size();
         int loop1 = (int)cand_xy_minus[m1].size();
         for(int i=0; i<loop0; i++){
            for(int j=0; j<loop1; j++){
               TVector3 vtx_diff = cand_xy_plus [m0][i].GetFitVertex()
                                 - cand_xy_minus[m1][j].GetFitVertex();
               if(fabs(vtx_diff.X()) < 1.0 && fabs(vtx_diff.Y()) < 2.0){
                  double_cand.push_back(
                        E16ANA_DoubleTrackCandidate_Nakai(
                           cand_xy_plus[m0][i], cand_xy_minus[m1][j]));
                  n_double++;
                  if(n_double >= 100) goto DOUBLE_FIT;
               }
            }
         }
      }
   }
   DOUBLE_FIT:
   std::cout << "# of Double track candidates = " << double_cand.size() << std::endl;
   DoubleFitAllCandidates(double_cand);
   CheckG4ClosestDouble(double_cand, ranking_double, hbd_proj_double, 3);
   int loops = std::min(3, (int)double_cand.size());
   for(int i=0; i<loops; i++){
      std::cout << i << "-th Chisq candidate (double) : ";
      double_cand[i].Print();
   }
   std::cout << std::endl;
   exist_best = false;
   if((int)double_cand.size() > 0){
      for(int i=0; i<(int)double_cand.size(); i++){
         E16ANA_SingleTrackCandidate_Nakai &single0 = double_cand[i].GetSingleTrackCandidate(0);
         E16ANA_SingleTrackCandidate_Nakai &single1 = double_cand[i].GetSingleTrackCandidate(1);
         if(IsHBDMatched(single0, 3, 40.0) && IsHBDMatched(single1, 3, 40.0)){
            exist_best = true;
            best_candidate = double_cand[i];
            std::cout << "HBD matched candidate is exist. Chi2_Rank = " << i << std::endl;
            break;
         }
      }
   }

   return;

   //if(ranking_xyselection_plus[0] >= 0 && ranking_xyselection_minus[1] >= 0){
   //E16ANA_DoubleTrackCandidate_Nakai double_g4 =
   //   E16ANA_DoubleTrackCandidate_Nakai(
   //      cand_xy_plus[0][ranking_xyselection_plus[0]],
   //      cand_xy_minus[1][ranking_xyselection_minus[1]]);

   //E16ANA_MultiTrack *fitter = new E16ANA_MultiTrack(bfield_map,geom,2);
   //double_g4.DoubleFit(fitter);
   //delete fitter;
   //std::cout << "G4 closest candidate (double) : ";
   //double_g4.Print();
   //exist_best = true;
   //best_candidate = double_g4;
   //}
}

