//2016-08-12, uploaded by nakai
//2016-03-18, uploaded by nakai

#include <G4Step.hh>
#include <G4Track.hh>
#include <G4VProcess.hh>

#include "E88G4_RPCSD.hh"

E88G4_RPCSD::E88G4_RPCSD(const G4String &name, const E16ANA_GeometryV2 *geom) : 
   G4VSensitiveDetector(name), geom_(geom)
{
   for(int i=0; i<n_rpc; i++){
      std::ostringstream ss;
      ss << i;
      G4String Name = G4String("RPC")+ss.str()+G4String("-Collection");
      collectionName.insert(Name);
      elem_[i] = 0;
   }
}

E88G4_RPCSD::~E88G4_RPCSD(){
}

void E88G4_RPCSD::SetElement(int id, E16G4_BeamLineElement *elem){
   elem_[id] = elem;
}

void E88G4_RPCSD::Initialize(G4HCofThisEvent *HCTE){
   for(int i=0; i<n_rpc; i++){
      hitsCollection[i] = new RPCHitsCollection(SensitiveDetectorName, collectionName[i]);
      G4int hcid = GetCollectionID(i);
      HCTE->AddHitsCollection(hcid, hitsCollection[i]);
   }
}

G4bool E88G4_RPCSD::ProcessHits(G4Step *astep, G4TouchableHistory *ROhist){
   const G4StepPoint *pre_step_point = astep->GetPreStepPoint();
   const G4StepPoint *post_step_point = astep->GetPostStepPoint();
   const G4Track *atrack = astep->GetTrack();

   G4ThreeVector gpos_pre = pre_step_point->GetPosition();
   G4ThreeVector gpos_post = post_step_point->GetPosition();

   ROhist = (G4TouchableHistory*)pre_step_point->GetTouchable();
   G4VPhysicalVolume *physical_volume = ROhist->GetVolume();
   G4String name = physical_volume->GetName();

   G4int id = -1;
   std::stringstream(name.substr(3)) >> id;
   //G4cout << "ProcessHits : id = " << id << G4endl;

   if(id >= 0){
      G4ThreeVector lpos_cross;
      if(geom_->RPC(id)->IsCrossed(gpos_pre, gpos_post, lpos_cross)){
         //G4cout << "RPC::ProcessHits : VolumeName = " << name
         //   << ", ID = " << id
         //   << ", CrossPoint = " << lpos_cross/mm
         //   << G4endl;
         G4String process_name = "Primary";
         const G4VProcess *process = atrack->GetCreatorProcess();
         if(process != NULL){
            process_name = process->GetProcessName();
         }

         G4double charge = atrack->GetDefinition()->GetPDGCharge();
         G4double time = pre_step_point->GetGlobalTime();
         G4ParticleDefinition *particle = atrack->GetDynamicParticle()->GetDefinition();

         //G4ThreeVector gpos = gpos_pre;
         //G4ThreeVector lpos = geom_->RPC(id)->GetLPos(gpos);
         G4ThreeVector lpos = lpos_cross;
         G4ThreeVector gpos = geom_->RPC(id)->GetGPos(lpos);
         G4ThreeVector gmom = atrack->GetMomentum();
         G4ThreeVector lmom = geom_->RPC(id)->GetLMom(gmom);

         E88G4_RPCHit *hit = new E88G4_RPCHit(lpos, time, charge);
         hit->SetGPos(gpos);
         hit->SetLPos(lpos);
         hit->SetGMom(gmom);
         hit->SetLMom(lmom);
         hit->SetEdep(astep->GetTotalEnergyDeposit());
         hit->SetBeta(atrack->GetVelocity()/c_light);
         hit->SetLength(astep->GetStepLength());
         hit->SetPName(particle->GetParticleName());
         hit->SetPCode(particle->GetPDGEncoding());
         hit->SetcrProcName(process_name);
         hit->SetTrackID(atrack->GetTrackID());
         hit->SetParentID(atrack->GetParentID());

         hitsCollection[id]->insert(hit);
      }
   }
   return true;
}

void E88G4_RPCSD::EndOfEvent(G4HCofThisEvent *){
}

void E88G4_RPCSD::DrawAll(){
}

void E88G4_RPCSD::PrintAll(){
   for(int i=0; i<n_rpc; i++){
      hitsCollection[i]->PrintAllHits();
   }
}

