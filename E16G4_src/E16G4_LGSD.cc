//2016-08-12, uploaded by nakai
//2015-09-05, uploaded by nakai
//2015-03-01, uploaded by yokkaich
//2015-01-05, modified by yokkaich
//2015-01-05, modified by yokkaich
//2014-08-27, modified by kawama
//2014-08-27, modified by kawama
//2014-04-30, modified by kawama
//2013-11-14, modified by kawama
//2013-05-13, modified by kawama

//E16G4_LGSD.cc 150105 
//    Last modified at <2015-01-05 14:00:50 >



#include <G4VPhysicalVolume.hh>
#include <G4Step.hh>
#include <G4Track.hh>
#include <G4VTouchable.hh>
#include <G4TouchableHistory.hh>

#include <G4ParticleChange.hh>
#include <G4VProcess.hh>
#include <G4ProcessManager.hh>
#include <TVector3.h>


#include "E16ANA_Transform.hh"
#include "E16G4_BeamLineElement.hh"

#include "E16G4_LGHit.hh"
#include "E16G4_LGSD.hh"


//////////////////////////////////////////
   E16G4_LGSD::E16G4_LGSD(const G4String& name)
: G4VSensitiveDetector(name)
//////////////////////////////////////////
{
   for (int i=0;i<nID;i++){
      std::ostringstream id;
      id << i;
      G4String Name = G4String("LG")+id.str().c_str()+"-Collection";
      collectionName.insert(Name);
      elem_[i]=0;
   }
}

/////////////////////////
 E16G4_LGSD::~E16G4_LGSD(){
/////////////////////////
}

/////////////////////////////////////////////////////////
void E16G4_LGSD::SetElement(int id, E16G4_BeamLineElement *elem)
/////////////////////////////////////////////////////////
{
   elem_[id]=elem;
}

//////////////////////////////////////////////////
void E16G4_LGSD::Initialize(G4HCofThisEvent* HCTE){
//////////////////////////////////////////////////
   for (int i=0;i<nID;i++){
      hitsCollection[i]= 
         new LGHitsCollection(SensitiveDetectorName, 
               collectionName[i]);
      G4int hcid= GetCollectionID(i);
      HCTE-> AddHitsCollection(hcid, hitsCollection[i]);
   }
}

/////////////////////////////////////////////
G4bool E16G4_LGSD::ProcessHits(G4Step* aStep, 
      G4TouchableHistory* ROhist){
/////////////////////////////////////////////
   const G4StepPoint* aStepPoint= aStep-> GetPreStepPoint();
   const G4Track* aTrack= aStep-> GetTrack();

   G4double charge= aTrack-> GetDefinition()-> GetPDGCharge();
   //Track Momentum
   G4ThreeVector mom= aTrack-> GetMomentum();
   //if(charge == 0. || mom.mag()==0){
   if(mom.mag()==0){
      return false;
   }
   /*if(aStepPoint-> GetStepStatus() != fGeomBoundary){
      G4cout <<"ProcessHits:GeoBound "<<G4endl;
      return false;
   }*/

   //Hit Position
   G4ThreeVector pos= aStepPoint-> GetPosition();
   //G4cout<< pos*(1./cm)<< G4endl;
   //Hit Time
   G4double time= aStepPoint-> GetGlobalTime();
   //   G4cerr <<"lg "<< time <<G4endl;

   //Particle information
   G4ParticleDefinition *particle= 
      aTrack-> GetDynamicParticle()->GetDefinition();
   G4String pName=particle->GetParticleName();
   G4int PDGCode=particle->GetPDGEncoding();
   G4double steplength=aStep->GetStepLength();
   G4double edep=aStep->GetTotalEnergyDeposit();

   //const G4VProcess *pr = aStepPoint->GetProcessDefinedStep();
   //G4ProcessManager *prman = pr->GetProcessManager();
   //G4cout << prman->GetParticleType()<<G4endl;
   //G4cout <<aTrack-> GetDynamicParticle()->GetPrimaryParticle()->GetParticleDefinition()->GetParticleName()<<G4endl;
   const G4VProcess *proc=aTrack-> GetCreatorProcess();
   G4String procName;
   if(proc!=0){
      procName=proc->GetProcessName();
   }
   else{
      procName="Primary";
   }
   //G4cout <<"E16G4_LGSD:"<<pName<<" "<<procName<<G4endl;

   /*G4ParticleChange *aParticleChange= new G4ParticleChange();
   aParticleChange->Initialize(*aTrack);
   const G4ThreeVector *pos_test=aParticleChange->GetPosition();

   G4cout <<"pos="<<pos<<G4endl;
   G4cout <<"Parent pos="<<pos_test<<G4endl;*/

   /*G4Track *tr_test = new G4Track();
     tr_test->SetTrackID(aTrack->GetParentID());
     G4cout <<tr_test-> GetDynamicParticle()->GetDefinition()->GetParticleName()<<G4endl;*/

   ROhist=(G4TouchableHistory*)aStepPoint->GetTouchable();
   G4VPhysicalVolume *PV = ROhist->GetVolume();
   G4String Name = PV->GetName();
   std::stringstream sid(Name.substr(2));
   int id=-1;sid>>id;
   //G4cout << "LG::ProcessHits:Name="<<Name<<", id="<<id<<G4endl;
   //E16ANA_Transform *ptr = new E16ANA_Transform();
   //E16G4_LGHit* aHit;
   G4double dLG=(140.+17.);
   //G4cout << "LG::ProcessHits:ID="<<id<<G4endl;
   if (id>=0){
      //int id = id/3;
      int idTr = id/3;
      int idGEM =id%3;
      double phi=30*(idTr%3-1);
      double theta=30*(4-int(idTr/3));
      G4ThreeVector cent=elem_[id]->GetCent();
      //G4ThreeVector orig=elem_[id]->GetLocalPosition(gPos);
      pos+=cent;
      //TVector3 cent_t3=trLG->GetLPos(TVector3(cent.x()/cm,cent.y()/cm,cent.z()/cm),id);
      //TVector3 lPos_t3=ptr->GetLPos(TVector3(pos.x()/cm,pos.y()/cm,pos.z()/cm),
      TVector3 lPos_t3=E16ANA_Transform::GetLPos(TVector3(pos.x()/cm,pos.y()/cm,pos.z()/cm),
            theta,phi,dLG,0,0);
      //G4cout << "LG::ProcessHits:cent="<<G4ThreeVector(cent_t3.X(),cent_t3.Y(),cent_t3.Z())<<G4endl;
      G4ThreeVector lPos(lPos_t3.X()*cm,lPos_t3.Y()*cm,lPos_t3.Z()*cm);
      /*G4cout << "LG::ProcessHits:lPos="<<lPos/cm<<G4endl;
      G4cout << "LG::ProcessHits:edep="<<edep/keV<<G4endl;
      G4cout << "LG::ProcessHits:sl="<<steplength/cm<<G4endl;*/
      //G4ThreeVector lPos=elem_[id]->GetLocalPosition(pos);
      //G4ThreeVector lMom=elem_[id]->GetLocalDirection(mom);
      //TVector3 lMom_t3=ptr->GetLMom(TVector3(mom.x()/GeV,mom.y()/GeV,mom.z()/GeV),
      TVector3 lMom_t3=E16ANA_Transform::GetLMom(TVector3(mom.x()/GeV,mom.y()/GeV,mom.z()/GeV),
            theta,phi,0,0);
      G4ThreeVector lMom(lMom_t3.X()*GeV,lMom_t3.Y()*GeV,lMom_t3.Z()*GeV);
      /*if (id==36){
        G4cout << "ProcessHits:pos="<<pos<<G4endl;
        G4cout << "ProcessHits:lPos="<<lPos<<G4endl;
        }*/

      G4double beta = aTrack-> GetVelocity()/c_light;
      /*G4double npe = 0.;
      G4double npeconst = 0.;
      if( beta >= ThresholdBeta && charge!=0 ) {
         npeconst = (steplength/m)*2.*pi/137.*
            (1.-1./(CF4ind*CF4ind*beta*beta))*CE;
         for (int i=0;i<ndata;i++){
            npe += npeconst*(1./WL[i]/WL[i])*dWL*QE[i];
         }// integrate with the wavelength
      }*/
      //G4cout <<"LGSD:"<<npe<<" "<<beta<<" "<<mom.mag()/MeV<<G4endl;

      /*const G4int nHits=hitsCollection[id]->entries();
      G4cout <<"LGSD:"<<nHits<<" "<<PDGCode<<" "<<mom.mag()<<G4endl;
      for (int i=0;i<nHits;i++){
         aHit =(*hitsCollection[id])[i];
         if(id==aHit->GetID()){
            aHit->AddEdep(edep);
            aHit->AddLength(steplength);
            return true;
         }
      }*/
      aHit = new E16G4_LGHit(pos, time, charge);
      aHit->SetGPos(pos);
      aHit->SetLPos(lPos);
      aHit->SetLMom(lMom);
      aHit->SetEdep(edep);
      //aHit->SetNpe(npe);
      aHit->SetBeta(beta);
      aHit->SetLength(steplength);
      aHit->SetPName(pName);
      aHit->SetPCode(PDGCode);
      aHit->SetProcName(procName);
      hitsCollection[id]-> insert(aHit);

      //return true;
   }
   //delete ptr;
   return true;
}

///////////////////////////////////////////////////
void E16G4_LGSD::EndOfEvent(G4HCofThisEvent* ){
   ///////////////////////////////////////////////////
}

//////////////////////////
void E16G4_LGSD::DrawAll(){
   //////////////////////////
}

///////////////////////////
void E16G4_LGSD::PrintAll(){
   ///////////////////////////
   for (int i=0;i<nID;i++){
      hitsCollection[i]-> PrintAllHits();
   }
}

