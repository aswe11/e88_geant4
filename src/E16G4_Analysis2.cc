//2016-08-12, uploaded by nakai
//2016-05-09, uploaded by yokkaich
//2016-05-05, uploaded by yokkaich
//2016-03-18, uploaded by nakai
//2015-08-30, uploaded by yokkaich
//2015-03-01, uploaded by yokkaich
//E16G4_Analysis2.cc 150222 by S. Yokkaichi
//    Last modified at <2016-05-06 10:46:24 >
//

//should be mearged to E16G4_Analysis.cc 

const G4double E16G4_Analysis::length_unit = mm;

////////////////////////////////////////////////////////////////
void E16G4_Analysis::PrimaryGenerationMulti(
      int n, int multi, 
      vector<G4int>& PID,
      vector<G4ThreeVector>& pos,
      vector<G4ThreeVector>& mom,
      const G4int eventID)
////////////////////////////////////////////////////////////////
{
   E16G4_PrimaryGeneratorAction *pPGA = E16G4_PrimaryGeneratorAction::GetpPGA();
   evID_JAM=pPGA->GetJAMEventNumber(n);

   mulNum_JAM= pPGA->GetJAMMultiplicity(n);

   evID_G4 = eventID;

   //   G4cerr<<multi<<" "<< pPGA->GetJAMMultiplicity(n)<<G4endl;

   //def   g4data->Event().SetHeader(eventID, pPGA->GetJAMEventNumber(n), multi);
   //sako 2019/11/18
   //   std::cout << "going to get impact " << std::endl;
   double impact = pPGA->GetJAMImpact(n);

   //   std::cout << "impact = " << impact << std::endl;

   g4data->Event().SetHeader(eventID, pPGA->GetJAMEventNumber(n), multi, impact);

   for(int i=0; i<(int)PID.size(); i++) E16MESSAGE("pdgid %i %i ",i,PID[i]);

   for(int i=0; i<multi; i++){
     g4data->Event().PrimaryTracks(i).SetData(evID_G4, 	    PID[i],
		   pos[i].x()/length_unit,  pos[i].y()/length_unit, pos[i].z()/length_unit,
		   mom[i].x()/GeV,  mom[i].y()/GeV, mom[i].z()/GeV,
		   mom[i].mag()/GeV  );
   }

   g4data->Event().PrimaryTracks().SetNoh(multi);

   //   std::cerr<<" track "<<mom[0].x()<<std::endl;

}

void E16G4_Analysis::G4OutputDataSSDFill(G4HCofThisEvent* HCE){

   int eventID = g4data->Event().G4EventID();
   //const int n_ssd = 33;
   int nSSDhit = 0; // # of hits in SSD layer

   //for(int k=0; k<n_ssd; k++){
   for(int k=0; k<nSSDmodule; k++){
      int moduleID=k; 
      int nTrkID = 1; // # of track ID in one chamber

      std::ostringstream id;
      id << k;
      G4String Name = "SSD"+id.str()+"-Collection";
      int colID = G4SDManager::GetSDMpointer()->GetCollectionID(Name);
      if(colID < 0){
         G4cout << "G4OutputDataSSDFill : Collection name = " << Name << ", Col id = " << colID << G4endl;
         continue;
      }
      //SSDHitsCollection* hc = (SSDHitsCollection*)(HCE->GetHC(colID));
      SSDHitsCollection* hc = dynamic_cast<SSDHitsCollection*>(HCE->GetHC(colID));

      if(hc){
         G4int nhits = hc->entries();
         G4ThreeVector lPos; G4ThreeVector lMom;
         G4ThreeVector gPos; G4ThreeVector gMom;
         G4ThreeVector Vertex;
         //if(nhits>0){
         //   G4cout << "E16G4_Analysis::SSD : nhits = " << nhits
         //      << ", ModuleID = " << moduleID
         //      << G4endl ;
         //}

         //if(nhits>0 && nhits<100){
         if(nhits>0){
            G4int ID = 0;
            G4int preID = 0;
            for(int i=0; i<nhits; i++){
               E16G4_SSDHit *ghit = (*hc)[i];
               G4String pName = ghit->GetPName();
               G4String procName = ghit->GetcrProcName();
               preID = ID;
               ID = ghit->GetTrackID();
               lPos = ghit->GetLPos();   lMom = ghit->GetLMom();
               gPos = ghit->GetGPos();   gMom = ghit->GetGMom();

               G4double edep = ghit->GetEdep();
               G4double beta = ghit->GetBeta();

               if(lMom.mag()/GeV > cutMomentum){ //momentum cut
                  if(ID!=preID && preID!=0){
                     nTrkID++;
                  }//if
                  int j = nSSDhit+nTrkID-1;

                  E16ANA_G4Hit& hit = g4data->Event().SSD().Hit(j);
                  hit.SetXYZ(lPos.x()/length_unit, lPos.y()/length_unit, lPos.z()/length_unit);
                  hit.SetGXYZ(gPos.x()/length_unit, gPos.y()/length_unit, gPos.z()/length_unit);
                  hit.SetP(lMom.x()/GeV, lMom.y()/GeV, lMom.z()/GeV);
                  hit.SetGP(gMom.x()/GeV, gMom.y()/GeV, gMom.z()/GeV);
                  hit.SetID(eventID, ghit->GetTrackID(), moduleID, ghit->GetPCode());
                  hit.SetEP(edep/keV, lMom.mag()/GeV, beta);
                  hit.SetT(ghit->GetTime()/ns);
                  //G4cout << "Filled SSD hits : TrackID = " << ghit->GetTrackID() << ", ModuleID = " << moduleID << G4endl;

		  //def                  if(nTrkID>100){
		  //sako 2019/10/7
                  if(nTrkID>maxhit){
		    //def                     G4cout << "Too many hit on SSD!!! Exit the loop." << G4endl;
		    G4cout << "Too many hit on SSD!!! Exit the loop. nTrkID = " << nTrkID << G4endl;
                     break;
                  }
               }// momentum cut
            }//nhits loop
            nSSDhit += nTrkID;
         }//if (nhits>0)
      }//if( hc )
   }//for loop (SSD)
   g4data->Event().SSD().SetNoh(nSSDhit);
}

void E16G4_Analysis::G4OutputDataRPCFill(G4HCofThisEvent* HCE){

   int eventID = g4data->Event().G4EventID();
   //const int n_rpc = 33;
   int nRPChit = 0; // # of hits in RPC layer

   //for(int k=0; k<n_rpc; k++){
   for(int k=0; k<nRPCmodule; k++){
      int moduleID=k; 
      int nTrkID = 1; // # of track ID in one chamber

      std::ostringstream id;
      id << k;
      G4String Name = "RPC"+id.str()+"-Collection";
      int colID = G4SDManager::GetSDMpointer()->GetCollectionID(Name);
      if(colID < 0){
         G4cout << "G4OutputDataRPCFill : Collection name = " << Name << ", Col id = " << colID << G4endl;
         continue;
      }
      //RPCHitsCollection* hc = (RPCHitsCollection*)(HCE->GetHC(colID));
      RPCHitsCollection* hc = dynamic_cast<RPCHitsCollection*>(HCE->GetHC(colID));

      if(hc){
         G4int nhits = hc->entries();
         G4ThreeVector lPos; G4ThreeVector lMom;
         G4ThreeVector gPos; G4ThreeVector gMom;
         G4ThreeVector Vertex;
         //if(nhits>0){
         //   G4cout << "E16G4_Analysis::RPC : nhits = " << nhits
         //      << ", ModuleID = " << moduleID
         //      << G4endl ;
         //}

         //if(nhits>0 && nhits<100){
         if(nhits>0){
            G4int ID = 0;
            G4int preID = 0;
            for(int i=0; i<nhits; i++){
               E88G4_RPCHit *ghit = (*hc)[i];
               G4String pName = ghit->GetPName();
               G4String procName = ghit->GetcrProcName();
               preID = ID;
               ID = ghit->GetTrackID();
               lPos = ghit->GetLPos();   lMom = ghit->GetLMom();
               gPos = ghit->GetGPos();   gMom = ghit->GetGMom();

               G4double edep = ghit->GetEdep();
               G4double beta = ghit->GetBeta();

               if(lMom.mag()/GeV > cutMomentum){ //momentum cut
                  if(ID!=preID && preID!=0){
                     nTrkID++;
                  }//if
                  int j = nRPChit+nTrkID-1;

                  E16ANA_G4Hit& hit = g4data->Event().RPC().Hit(j);
                  hit.SetXYZ(lPos.x()/length_unit, lPos.y()/length_unit, lPos.z()/length_unit);
                  hit.SetGXYZ(gPos.x()/length_unit, gPos.y()/length_unit, gPos.z()/length_unit);
                  hit.SetP(lMom.x()/GeV, lMom.y()/GeV, lMom.z()/GeV);
                  hit.SetGP(gMom.x()/GeV, gMom.y()/GeV, gMom.z()/GeV);
                  hit.SetID(eventID, ghit->GetTrackID(), moduleID, ghit->GetPCode());
                  hit.SetEP(edep/keV, lMom.mag()/GeV, beta);
                  hit.SetT(ghit->GetTime()/ns);
                  //G4cout << "Filled RPC hits : TrackID = " << ghit->GetTrackID() << ", ModuleID = " << moduleID << G4endl;

		  //def                  if(nTrkID>100){
		  //sako 2019/10/7
                  if(nTrkID>maxhit){
		    //def                     G4cout << "Too many hit on RPC!!! Exit the loop." << G4endl;
		    G4cout << "Too many hit on RPC!!! Exit the loop. nTrkID = " << nTrkID << G4endl;
                     break;
                  }
               }// momentum cut
            }//nhits loop
            nRPChit += nTrkID;
         }//if (nhits>0)
      }//if( hc )
   }//for loop (RPC)
   g4data->Event().RPC().SetNoh(nRPChit);
}

void   E16G4_Analysis::G4OutputDataGTRFill( G4HCofThisEvent* HCE){

  int eventID = g4data->Event().G4EventID();

   int nGTRhit[3]={0};
   for( int k=0 ; k < nGTRChamber; k++){
      int nTrkID[3]={1,1,1};
      int moduleID=k/3; 
      int layerID =k%3; 

      std::ostringstream id;
      id << k;
      G4String Name = G4String("GTR")+id.str().c_str()+"-Collection";
      int colID = G4SDManager::GetSDMpointer() -> GetCollectionID( Name );
      GTRHitsCollection* hc = (GTRHitsCollection*)( HCE-> GetHC( colID ) );

      if( hc ){
         G4int nhits = hc->entries();
         G4ThreeVector lPos;         G4ThreeVector lMom;
	 G4ThreeVector gPos;         G4ThreeVector gMom;
         G4ThreeVector Vertex;
	 //         G4double maxEdep=10000;
         //if (nhits>0){
         //   G4cout << "E16G4_Analysis::GTR : nhits = " << nhits
         //      << ", ModuleID = " << moduleID << ", LayerID = " << layerID
         //      << G4endl ;
         //}

         if (nhits>0 && nhits<100){
            G4int ID=0;
            G4int preID=0;
            for(G4int i=0; i< nhits; i++){
	      E16G4_GTRHit* ghit=(*hc)[i];
               G4String pName= ghit-> GetPName();
               G4String procName= ghit-> GetcrProcName();
               preID=ID;
               ID= ghit-> GetTrackID();
               lPos= ghit-> GetLPos();   lMom= ghit-> GetLMom();
               gPos= ghit-> GetGPos();   gMom= ghit-> GetGMom();

               G4double edep= ghit-> GetEdep()/keV;
               G4double beta= ghit-> GetBeta();

	       //      cerr<<" gtr edep/beta "<<edep<<" "<<beta<<endl;

               if( lMom.mag()/GeV > cutMomentum ){ //momentum cut
                  if (ID!=preID && preID!=0){
		    nTrkID[layerID] ++;
                  }//if
		  int j = nGTRhit[layerID] + nTrkID[layerID] -1;

		  E16ANA_G4Hit& hit = g4data->Event().GTRs(layerID).Hit(j);
		  hit.SetXYZ(  lPos.x()/length_unit, lPos.y()/length_unit, lPos.z()/length_unit  );
		  hit.SetGXYZ( gPos.x()/length_unit, gPos.y()/length_unit, gPos.z()/length_unit  );
		  hit.SetP(  lMom.x()/GeV, lMom.y()/GeV, lMom.z()/GeV);
		  hit.SetGP( gMom.x()/GeV, gMom.y()/GeV, gMom.z()/GeV);
		  hit.SetID( eventID, 
			     ghit-> GetTrackID(), moduleID, ghit-> GetPCode() );
		  hit.SetEP( edep, lMom.mag()/GeV, beta );
		  hit.SetT( ghit-> GetTime() );

		  //                  maxEdep=edep;
		  //def                  if(nTrkID[layerID]>100){
		  //sako 2019/10/7
                  if(nTrkID[layerID]>maxhit){
		    //def                     G4cout <<"Too many hit on GTR!!! Exit the loop."<<G4endl;
		    G4cout <<"Too many hit on GTR!!! Exit the loop. nTrkID["<< layerID <<"] = "<< nTrkID[layerID]<<G4endl;
                     break;
                  }

               }// momentum cut
            }//nhits loop

	    nGTRhit[layerID] += nTrkID[layerID]; 	    

         }//if (nhits>0)
      }//if( hc )
   }//for loop (GTR)

   g4data->Event().GTRs(0).SetNoh(nGTRhit[0]);	    
   g4data->Event().GTRs(1).SetNoh(nGTRhit[1]);	    
   g4data->Event().GTRs(2).SetNoh(nGTRhit[2]);	    

   if(nGTR1hit>0 && nGTR2hit>0 && nGTR3hit>0){      GTRTrig=true;   }
   else{      GTRTrig=false;   }

#if 0
   cerr<<"noh new gtr1 "<<g4data->Event().GTR1().Noh()<<endl;
   for(int ii = 0;ii< g4data->Event().GTR1().Noh()  ; ii++){
     E16ANA_G4Hit& h= g4data->Event().GTR1().Hit(ii);
     cerr<<"new gtr1 id/x/p "<<ii <<" "<<h.DetectorID()<<" "<<h.TrackID()<<" "<<h.X()<<" "<<h.PV().mag()<<endl;
   }
#endif

}

void E16G4_Analysis::G4OutputDataGTRFrameFill(G4HCofThisEvent* HCE){

   int eventID = g4data->Event().G4EventID();
   int nGTRFrameHits[3] = {0,0,0};

   std::map<int, E16ANA_G4Hit> hits[nGTRChamber];

   for(int k=0; k<nGTRChamber; k++){
      int moduleID=k/3; 
      int layerID=k%3; 
      int nTrkID = 1; // # of track ID in one chamber

      std::ostringstream id;
      id << k;
      int colID;
      G4String Name;
      Name = "GTRFrameDriftSpacer"+id.str()+"-Collection";
      colID = G4SDManager::GetSDMpointer()->GetCollectionID(Name);
      GTRFrameHitsCollection* hc_drift_spacer = (GTRFrameHitsCollection*)(HCE->GetHC(colID));
      Name = "GTRFrameGEM"+id.str()+"-Collection";
      colID = G4SDManager::GetSDMpointer()->GetCollectionID(Name);
      GTRFrameHitsCollection* hc_gem = (GTRFrameHitsCollection*)(HCE->GetHC(colID));
      Name = "GTRFrameReadOut"+id.str()+"-Collection";
      colID = G4SDManager::GetSDMpointer()->GetCollectionID(Name);
      GTRFrameHitsCollection* hc_readout = (GTRFrameHitsCollection*)(HCE->GetHC(colID));
      Name = "GTRFrameAlBase"+id.str()+"-Collection";
      colID = G4SDManager::GetSDMpointer()->GetCollectionID(Name);
      GTRFrameHitsCollection* hc_al_base = (GTRFrameHitsCollection*)(HCE->GetHC(colID));
      Name = "GTRFrameCFRP"+id.str()+"-Collection";
      colID = G4SDManager::GetSDMpointer()->GetCollectionID(Name);
      GTRFrameHitsCollection* hc_cfrp = (GTRFrameHitsCollection*)(HCE->GetHC(colID));

      G4OutputDataGTRFrameFillSub(hc_drift_spacer, moduleID, layerID, hits[k]);
      G4OutputDataGTRFrameFillSub(hc_gem         , moduleID, layerID, hits[k]);
      G4OutputDataGTRFrameFillSub(hc_readout     , moduleID, layerID, hits[k]);
      G4OutputDataGTRFrameFillSub(hc_al_base     , moduleID, layerID, hits[k]);
      G4OutputDataGTRFrameFillSub(hc_cfrp        , moduleID, layerID, hits[k]);
      //if((int)hits[k].size() > 0){
      //   G4cout << "E16G4_Analysis::GTRFrame : nhits = " << hits[k].size()
      //      << ", ModuleID = " << moduleID << ", LayerID = " << layerID
      //      << G4endl ;
      //}
      std::map<int, E16ANA_G4Hit>::iterator it;
      int i_trkid = 0;
      for(it = hits[k].begin(); it != hits[k].end(); it++){
         int j = i_trkid+nGTRFrameHits[layerID];
         g4data->Event().GTRFrames(layerID).Hit(j) = it->second;
         i_trkid++;
      }
      nGTRFrameHits[layerID] += i_trkid;
   }//for loop (GTRFrame)

   g4data->Event().GTRFrames(0).SetNoh(nGTRFrameHits[0]);
   g4data->Event().GTRFrames(1).SetNoh(nGTRFrameHits[1]);
   g4data->Event().GTRFrames(2).SetNoh(nGTRFrameHits[2]);
}

void E16G4_Analysis::G4OutputDataGTRFrameFillSub(GTRFrameHitsCollection *hc, int moduleID, int layerID, std::map<int, E16ANA_G4Hit> &hits){
   int eventID = g4data->Event().G4EventID();
   if(hc){
      G4int nhits = hc->entries();
      G4ThreeVector lPos; G4ThreeVector lMom;
      G4ThreeVector gPos; G4ThreeVector gMom;
      G4ThreeVector Vertex;
      //if(nhits>0){
      //   G4cout << "E16G4_Analysis::GTRFrame : nhits = " << nhits
      //      << ", ModuleID = " << moduleID << ", LayerID = " << layerID
      //      << G4endl ;
      //}

      //if(nhits>0 && nhits<100){
      if(nhits>0){
         G4int ID = 0;
         for(int i=0; i<nhits; i++){
            E16G4_GTRFrameHit *ghit = (*hc)[i];
            //G4String pName = ghit->GetPName();
            //G4String procName = ghit->GetcrProcName();
            ID = ghit->GetTrackID();
            lPos = ghit->GetLPos();   lMom = ghit->GetLMom();
            gPos = ghit->GetGPos();   gMom = ghit->GetGMom();

            G4double edep = ghit->GetEdep();
            //G4double beta = ghit->GetBeta();
            G4double beta = -100000.0;

            if(lMom.mag()/GeV > cutMomentum){ //momentum cut

               E16ANA_G4Hit& hit = hits[ID];
               hit.SetXYZ(lPos.x()/length_unit, lPos.y()/length_unit, lPos.z()/length_unit);
               hit.SetGXYZ(gPos.x()/length_unit, gPos.y()/length_unit, gPos.z()/length_unit);
               hit.SetP(lMom.x()/GeV, lMom.y()/GeV, lMom.z()/GeV);
               hit.SetGP(gMom.x()/GeV, gMom.y()/GeV, gMom.z()/GeV);
               hit.SetID(eventID, ghit->GetTrackID(), moduleID, ghit->GetPCode());
               hit.SetEP(edep/keV, lMom.mag()/GeV, beta);
               //hit.SetT(ghit->GetTime()/ns);

	       //def            if((int)hits.size()>100){
	       //sako 2019/10/7
               if((int)hits.size()>maxhit){
		 //def                  G4cout << "Too many hit on GTRFrame!!! Exit the loop." << G4endl;
		 G4cout << "Too many hit on GTRFrame!!! Exit the loop. hits.size() = " << hits.size() << G4endl;
                  break;
               }
            }// momentum cut
         }//nhits loop
      }//if (nhits>0)
   }//if( hc )
}

void   E16G4_Analysis::G4OutputDataTargetFill( G4HCofThisEvent* HCE){
#if 1
  int eventID = g4data->Event().G4EventID();

   TargetHitsCollection *hcTar[nTarget];

   for (int k=0;k<nTarget;k++){ 
      std::ostringstream id;
      id << k;
      G4String Name = G4String("Target")+id.str().c_str()+"-Collection";
      int colID = G4SDManager::GetSDMpointer() -> GetCollectionID( Name );

      hcTar[k] = (TargetHitsCollection*)( HCE->GetHC(colID) );

      if( hcTar[k] ){
         G4int nhits = hcTar[k]->entries();
         //dETar[k]=0.;
         if (nhits>0 && nhits< g4data->Event().TARGET(k).TableSize){ 

	   G4ThreeVector gPos, lPos, gMom, lMom, Vertex;
	   //G4double maxEdep=10000;
	   G4int ID=0, preID=0;

	   for( G4int i=0; i<nhits; i++ ){//Loop of total number of hits
	     E16G4_TargetHit *aTargetHit = (*hcTar[k])[i];
	     //dETar += (aTargetHit->GetEdep())/(MeV);

	     preID=ID;
	     ID= aTargetHit-> GetTrackID();

	     //	     cerr<<"id "<<ID<<endl;

	     G4double edep= aTargetHit -> GetEdep()/keV;
	     gPos=  aTargetHit->GetGPos();
	     lPos=  aTargetHit->GetLPos();
	     lMom=  aTargetHit->GetLMom();
	     gMom=  aTargetHit->GetGMom();

	     E16ANA_G4Hit& hit = g4data->Event().TARGET(k).Hit(i);
	     hit.SetXYZ(  lPos.x()/length_unit, lPos.y()/length_unit, lPos.z()/length_unit  );
	     hit.SetGXYZ( gPos.x()/length_unit, gPos.y()/length_unit, gPos.z()/length_unit  );
	     hit.SetP(  lMom.x()/GeV, lMom.y()/GeV, lMom.z()/GeV);
	     hit.SetGP( gMom.x()/GeV, gMom.y()/GeV, gMom.z()/GeV);
	     hit.SetID( eventID,  ID, m,  aTargetHit-> GetPCode() );
	     hit.SetEP( edep, lMom.mag()/GeV, aTargetHit->GetBeta() );
	     hit.SetT( aTargetHit -> GetTime() );

               //maxEdep=edep;
	   }// nhits loop
	   g4data->Event().TARGET(k).SetNoh(nhits);

	 }//nhits check

      }//if hcTar

   }//k-loop

#endif//target

}
void   E16G4_Analysis::G4OutputDataLGFill( G4HCofThisEvent* HCE){
}

void E16G4_Analysis::G4OutputDataLGVDFill(G4HCofThisEvent* HCE){

   int eventID = g4data->Event().G4EventID();
   int nLGVDhit = 0; // # of hits in SSD layer

   //for(int k=0; k<n_ssd; k++){
   for(int k=0; k<nLGVD; k++){
      int moduleID=k; 
      int nTrkID = 1; // # of track ID in one chamber

      std::ostringstream id;
      id << k;
      G4String Name = "LGVD"+id.str()+"-Collection";
      int colID = G4SDManager::GetSDMpointer()->GetCollectionID(Name);
      if(colID < 0){
         G4cout << "G4OutputDataLGVDFill : Collection name = " << Name << ", Col id = " << colID << G4endl;
         continue;
      }
      VDHitsCollection* hc = dynamic_cast<VDHitsCollection*>(HCE->GetHC(colID));

      if(hc){
         G4int nhits = hc->entries();
         G4ThreeVector lPos; G4ThreeVector lMom;
         G4ThreeVector gPos; G4ThreeVector gMom;
         G4ThreeVector Vertex;
         //if(nhits>0){
         //   G4cout << "E16G4_Analysis::SSD : nhits = " << nhits
         //      << ", ModuleID = " << moduleID
         //      << G4endl ;
         //}

         //if(nhits>0 && nhits<100){
         if(nhits>0){
            G4int ID = 0;
            G4int preID = 0;
            for(int i=0; i<nhits; i++){
               E16G4_VDHit *ghit = (*hc)[i];
               //G4String pName = ghit->GetPName();
               //G4String procName = ghit->GetcrProcName();
               preID = ID;
               ID = ghit->GetTrackID();
               lPos = ghit->GetLPos();   lMom = ghit->GetLMom();
               gPos = ghit->GetGPos();   gMom = ghit->GetGMom();

               G4double edep = ghit->GetEdep();
               //G4double beta = ghit->GetBeta();
               G4double beta = -100000.0;

               if(lMom.mag()/GeV > cutMomentum){ //momentum cut
                  if(ID!=preID && preID!=0){
                     nTrkID++;
                  }//if
                  int j = nLGVDhit+nTrkID-1;

                  E16ANA_G4Hit& hit = g4data->Event().LGVD().Hit(j);
                  hit.SetXYZ(lPos.x()/length_unit, lPos.y()/length_unit, lPos.z()/length_unit);
                  hit.SetGXYZ(gPos.x()/length_unit, gPos.y()/length_unit, gPos.z()/length_unit);
                  hit.SetP(lMom.x()/GeV, lMom.y()/GeV, lMom.z()/GeV);
                  hit.SetGP(gMom.x()/GeV, gMom.y()/GeV, gMom.z()/GeV);
                  hit.SetID(eventID, ghit->GetTrackID(), moduleID, ghit->GetPCode());
                  hit.SetEP(edep/keV, lMom.mag()/GeV, beta);
                  hit.SetT(ghit->GetTime()/ns);
                  //G4cout << "Filled LGVD hits : TrackID = " << ghit->GetTrackID() << ", ModuleID = " << moduleID << G4endl;

		  //def                  if(nTrkID>100){
		  //sako 2019/10/7
                  if(nTrkID>maxhit){
		    //def                     G4cout << "Too many hit on LGVD!!! Exit the loop." << G4endl;
		    G4cout << "Too many hit on LGVD!!! Exit the loop. nTrkID = " << nTrkID << G4endl;
                     break;
                  }
               }// momentum cut
            }//nhits loop
            nLGVDhit += nTrkID;
         }//if (nhits>0)
      }//if( hc )
   }//for loop (LGVD)
   g4data->Event().LGVD().SetNoh(nLGVDhit);
}

int   G4OutputDataHBDcutFlagSet(int nhits, HBDHitsCollection* hcHBD, 
			      	 vector<bool>& shouldFill ){
	 G4int ID=-1;   G4int preID= -1;
	 G4ThreeVector lMom;
	 int nModuleHit = 0;

	 for(G4int i=0; i<nhits; i++){//check & fill flag set
	   shouldFill[i]=true;
	   E16G4_HBDHit* ghit=(*hcHBD)[i];
	   lMom= ghit -> GetLMom();              
	   if( lMom.mag()/GeV < HBDcutMomentum ){ //momentum cut
	     shouldFill[i]=false;
	     continue;
	   }
	   else{
	     nModuleHit ++;
	     preID=ID;
	     ID= ghit-> GetTrackID();
	     //	     if (ID != preID && preID != 0){    
	     if (ID == preID ){    
	       nModuleHit --;
	     }//same track ID as previous
	   }//else
	 }

	 return nModuleHit;
  
}

void   E16G4_Analysis::G4OutputDataHBDFill( G4HCofThisEvent* HCE){
  int eventID = g4data->Event().G4EventID();

   int nHBDhit=0;
   //   int colIdHBD[nHBDmodule];
   //HBDHitsCollection* hcHBD[nHBDmodule];

   for( int k=0 ; k<nHBDmodule; k++){
     G4int nModuleHit=0;
      std::ostringstream id;
      id << k;
      G4String Name = G4String("HBD")+id.str().c_str()+"-Collection";
      int colID = G4SDManager::GetSDMpointer() -> GetCollectionID( Name );

      HBDHitsCollection* hcHBD= (HBDHitsCollection*)( HCE-> GetHC( colID ) );
      if( hcHBD ){
         G4int nhits = hcHBD->entries();
         G4ThreeVector lPos;         G4ThreeVector lMom;
	 G4ThreeVector gPos;         G4ThreeVector gMom;
         G4ThreeVector Vertex;
	 //         G4double npe=0; G4double stepl=0;
	 G4int ID=0;   G4int preID=0;
	 
	 vector<bool> shouldFill(nhits);
	 nModuleHit = G4OutputDataHBDcutFlagSet(nhits, hcHBD,  shouldFill);

	 int tempsize = nHBDhit + nModuleHit;
	 if ( tempsize > g4data->Event().HBD().TableSize ){
	   E16WARNING("HBD tablesize over at module %02i: %5i + %5i", 
			      k, nHBDhit, nModuleHit);
	   g4data->Event().HBD().SetNoh(nHBDhit);	    
	   return;
	 }

	 int j = nHBDhit; //offset
	 for(G4int i=0; i<nhits; i++){//fill
	      E16G4_HBDHit* ghit=(*hcHBD)[i];
	      lPos= ghit -> GetLPos();lMom= ghit -> GetLMom();              
	      gPos= ghit -> GetGPos();gMom= ghit -> GetGMom();
	      G4double edep=  ghit -> GetEdep()/keV;
	       //               npe= ghit-> GetNpe();
	       //               stepl= ghit-> GetLength();

	      if( shouldFill[i] ){
		preID=ID;
		ID= ghit-> GetTrackID();
		if (ID != preID && preID != 0){    
		  j++;
		}//new track ID...fill position update
		
		E16ANA_G4Hit& hit = g4data->Event().HBD().Hit(j);
		hit.SetXYZ(  lPos.x()/length_unit, lPos.y()/length_unit, lPos.z()/length_unit  );
		hit.SetGXYZ( gPos.x()/length_unit, gPos.y()/length_unit, gPos.z()/length_unit  );
		hit.SetP(  lMom.x()/GeV, lMom.y()/GeV, lMom.z()/GeV);
		hit.SetGP( gMom.x()/GeV, gMom.y()/GeV, gMom.z()/GeV);
		hit.SetID( eventID, ghit-> GetTrackID(), k, ghit-> GetPCode() );
		hit.SetEP( edep, lMom.mag()/GeV, ghit -> GetBeta() );
		hit.SetT( ghit-> GetTime() );
		//The last hit of the track survives

	      }//flag
            }//for i fill

            nHBDhit += nModuleHit ;

      }//if hcHBD
   }// for k<nHBD

   g4data->Event().HBD().SetNoh(nHBDhit);	    
#if 0
   cerr<<"noh hbd "<<g4data->Event().HBD().Noh()<<endl;
   for(int ii = 0;ii< nHBDhit; ii++){
     E16ANA_G4Hit& h= g4data->Event().HBD().Hit(ii);
     cerr<<"hbd id/x/p "<<ii <<" "<<h.DetectorID()<<" "<<h.TrackID()<<" "<<h.X()<<" "<<h.PV().mag()<<endl;
  }
#endif


}

void E16G4_Analysis::G4OutputDataHBDFill_Nakai(G4HCofThisEvent* HCE){
   int eventID = g4data->Event().G4EventID();
   int nHBDhit = 0;

   for(int k=0; k<nHBDmodule; k++){
      G4int nModuleHit=0;
      std::ostringstream id;
      id << k;
      G4String Name = G4String("HBD")+id.str().c_str()+"-Collection";
      int colID = G4SDManager::GetSDMpointer()->GetCollectionID(Name);
      HBDHitsCollection* hcHBD = (HBDHitsCollection*)(HCE->GetHC(colID));

      if(hcHBD == NULL){
         continue;
      }
      G4int nhits = hcHBD->entries();
      G4ThreeVector lPos;
      G4ThreeVector lMom;
      G4ThreeVector gPos;
      G4ThreeVector gMom;
      G4ThreeVector Vertex;
      G4int ID = 0;
      G4int preID = 0;

      int j = nHBDhit;
      double min_lz =  10000.0;
      double max_lz = -10000.0;
      int min_i = -1;
      int max_i = -1;
      E16ANA_G4Hit min_hit;
      E16ANA_G4Hit max_hit;
      for(G4int i=0; i<nhits; i++){
         E16G4_HBDHit* ghit=(*hcHBD)[i];
         lPos = ghit->GetLPos();
         lMom = ghit->GetLMom();
         gPos = ghit->GetGPos();
         gMom = ghit->GetGMom();
         G4double edep = ghit -> GetEdep()/keV;
         if( lMom.mag()/GeV < HBDcutMomentum ){ //momentum cut
            continue;
         }

         preID = ID;
         ID = ghit->GetTrackID();
         if(ID != preID && preID != 0){
            if(min_i != max_i && min_i > -1 && max_i > -1){
               if(j+2 > g4data->Event().HBD().TableSize){
                  E16WARNING("HBD tablesize over at module %02i: %5i", k, j+2);
                  g4data->Event().HBD().SetNoh(j);
                  return;
               }
               g4data->Event().HBD().Hit(j  ) = min_hit;
               g4data->Event().HBD().Hit(j+1) = max_hit;
               j += 2;
            }else if(min_i > -1){
               if(j+1 > g4data->Event().HBD().TableSize){
                  E16WARNING("HBD tablesize over at module %02i: %5i", k, j+1);
                  g4data->Event().HBD().SetNoh(j);
                  return;
               }
               g4data->Event().HBD().Hit(j  ) = min_hit;
               j++;
            }
            min_hit.Clear();
            max_hit.Clear();
            min_i = -1;
            max_i = -1;
            min_lz =  10000.0;
            max_lz = -10000.0;
         }
         if(lPos.z() < min_lz){
            min_i = i;
            min_lz = lPos.z();
            min_hit.SetXYZ (lPos.x()/length_unit, lPos.y()/length_unit, lPos.z()/length_unit);
            min_hit.SetGXYZ(gPos.x()/length_unit, gPos.y()/length_unit, gPos.z()/length_unit);
            min_hit.SetP (lMom.x()/GeV, lMom.y()/GeV, lMom.z()/GeV);
            min_hit.SetGP(gMom.x()/GeV, gMom.y()/GeV, gMom.z()/GeV);
            min_hit.SetID(eventID, ghit-> GetTrackID(), k, ghit->GetPCode());
            min_hit.SetEP(edep, lMom.mag()/GeV, ghit -> GetBeta());
            min_hit.SetT(ghit-> GetTime());
         }
         if(lPos.z() > max_lz){
            max_i = i;
            max_lz = lPos.z();
            max_hit.SetXYZ (lPos.x()/length_unit, lPos.y()/length_unit, lPos.z()/length_unit);
            max_hit.SetGXYZ(gPos.x()/length_unit, gPos.y()/length_unit, gPos.z()/length_unit);
            max_hit.SetP (lMom.x()/GeV, lMom.y()/GeV, lMom.z()/GeV);
            max_hit.SetGP(gMom.x()/GeV, gMom.y()/GeV, gMom.z()/GeV);
            max_hit.SetID(eventID, ghit-> GetTrackID(), k, ghit->GetPCode());
            max_hit.SetEP(edep, lMom.mag()/GeV, ghit -> GetBeta());
            max_hit.SetT(ghit-> GetTime());
         }
      }//for i fill
      if(min_i != max_i && min_i > -1 && max_i > -1){
         if(j+2 > g4data->Event().HBD().TableSize){
            E16WARNING("HBD tablesize over at module %02i: %5i", k, j+2);
            g4data->Event().HBD().SetNoh(j);
            return;
         }
         g4data->Event().HBD().Hit(j  ) = min_hit;
         g4data->Event().HBD().Hit(j+1) = max_hit;
         j += 2;
      }else if(min_i > -1){
         if(j+1 > g4data->Event().HBD().TableSize){
            E16WARNING("HBD tablesize over at module %02i: %5i", k, j+1);
            g4data->Event().HBD().SetNoh(j);
            return;
         }
         g4data->Event().HBD().Hit(j  ) = min_hit;
         j++;
      }
      nHBDhit = j;
   }// for k<nHBD
   g4data->Event().HBD().SetNoh(nHBDhit);
}

void   E16G4_Analysis::G4OutputDataFill( const G4Event* anEvent ){
   G4HCofThisEvent* HCE = anEvent->GetHCofThisEvent();
   if (!HCE){      return;   }

   G4OutputDataTargetFill( HCE );
   G4OutputDataSSDFill( HCE );
   G4OutputDataRPCFill( HCE );
   //G4OutputDataHBDFill( HCE );
   G4OutputDataHBDFill_Nakai( HCE );
   G4OutputDataLGFill( HCE );
   G4OutputDataLGVDFill( HCE );
   G4OutputDataGTRFill( HCE );
   G4OutputDataGTRFrameFill( HCE );
   G4OutputDataTrajectoryFill(anEvent);
   
}

void E16G4_Analysis::G4OutputDataTrajectoryFill(const G4Event* anEvent){

  G4TrajectoryContainer* trContainer = anEvent->GetTrajectoryContainer();
  if(!trContainer){
    G4cout << "trContainer is NULL." << G4endl;
    return;
  }

  int n_traj = trContainer->size();
  //def
  /*
  const int tableSize = g4data->Event().PrimaryTrajectories().TableSize ;
  if( n_traj >   tableSize ){
    E16WARNING("TableSize over : n_traj = %d shurinked to %d", n_traj, tableSize);
    n_traj = tableSize ;// save only within the 'TableSize'
  }
  */

  //sako 2019/10/7
  const int tableSize = g4data->Event().PrimaryTrajectories().TableSizeTraj ;
  if( n_traj >   tableSize ){
    E16WARNING("TableSize over : n_traj = %d shurinked to %d", n_traj, tableSize);
    n_traj = tableSize ;// save only within the 'TableSize'
  }

  int id1 = 0;
  int id2 = 0;

  for(int i=0;i<n_traj;i++){

    E16G4_Trajectory* traj = (E16G4_Trajectory*) (*trContainer)[i];
    E16ANA_G4Trajectory* anaTraj;
    
    if(traj->GetParentID() == 0){//primary tracks
      anaTraj = &(g4data->Event().PrimaryTrajectories().Hit(id1));
      id1++;
    }
    else{//secondary
      double mom=traj->GetInitialMomentum().mag()/GeV;
      if( mom < 0.05 ){ continue;}//no save
      //if( mom < 100 ){ continue;}//no save
      //      E16MESSAGE("secondary save: mom= %lf", mom);    
      anaTraj= &(g4data->Event().SecondaryTrajectories().Hit(id2));
      id2++;
    }

    anaTraj->SetData(anEvent->GetEventID(), traj->GetTrackID(), 
      traj->GetPID(), traj->GetParentID(),
		     traj->GetInitialVertex().x()/mm,
		     traj->GetInitialVertex().y()/mm,
		     traj->GetInitialVertex().z()/mm,
		     traj->GetInitialMomentum().x()/GeV,
		     traj->GetInitialMomentum().y()/GeV,
		     traj->GetInitialMomentum().z()/GeV,
		     traj->GetInitialMomentum().mag()/GeV,
		     traj->GetGenerateTime()/ns
		     );

    anaTraj->SetNames((char*) traj->GetProcessName().c_str(), (char*) traj->GetGenerateVolume().c_str());
    
  }//i

  g4data->Event().PrimaryTrajectories().SetNoh(id1);
  g4data->Event().SecondaryTrajectories().SetNoh(id2);
  
}
