//2016-08-12, uploaded by nakai
//2016-03-18, uploaded by nakai
//2015-09-05, uploaded by nakai
//2015-03-01, uploaded by yokkaich
//2015-01-05, modified by yokkaich
//2015-01-05, modified by yokkaich
//2014-08-27, modified by kawama
//2014-08-27, modified by kawama
//2014-04-30, modified by kawama
//2013-11-14, modified by kawama
//2013-05-13, modified by kawama

#include <G4VPhysicalVolume.hh>
#include <G4Step.hh>
#include <G4Track.hh>
#include <G4VTouchable.hh>
#include <G4TouchableHistory.hh>

#include <G4ParticleChange.hh>
#include <G4VProcess.hh>
#include <G4ProcessManager.hh>
#include <TVector3.h>

#include "E16G4_HBDSD.hh"
#include "E16G4_BeamLineElement.hh"
#include "E16ANA_Transform.hh"
#include "E16ANA_GeometryV2.hh"

const G4double CF4ind = 1.000620;
const G4double ThresholdBeta = 1./CF4ind;
//const G4double lmin = 100.*1e-9; // wave length at minimum
//const G4double lmax = 200.*1e-9; // wave length at maximum
const G4double CE = 0.5; // light collection efficiency of a box and GEM;
const G4int ndata = 17;
//const G4int ndata = 16;
const G4double dWL = 5.e-9;
const G4double WL[ndata] = {
   120e-9,125.e-9,130.e-9,135.e-9,140.e-9,145.e-9,150.e-9,155.e-9,160.e-9,
   165.e-9,170.e-9,175.e-9,180.e-9,185.e-9,190.e-9,195.e-9,200.e-9
};// in nm
const G4double QE[ndata] = {
   0.37,0.39,0.37,0.37,0.35,0.32,0.29,0.26,0.23,
   0.20,0.17,0.15,0.13,0.10,0.07,0.03,0.01
}; // quantum efficiencies from meeting 120418 (Aoki-san's report)
/*const G4double QE[ndata] = {
   0.20,0.23,0.25,0.25,0.24,0.21,0.20,0.17,
   0.15,0.12,0.11,0.10,0.08,0.06,0.02,0.01
};*/ // quantum efficiencies from NIM A 628 (2011) 300-303 (Aoki-san's paper)

//////////////////////////////////////////
   E16G4_HBDSD::E16G4_HBDSD(const G4String& name, const E16ANA_GeometryV2 *geom)
: G4VSensitiveDetector(name), geom_(geom)
//////////////////////////////////////////
{
   for (int i=0;i<nHBD;i++){
      std::ostringstream id;
      id << i;
      G4String Name = G4String("HBD")+id.str().c_str()+"-Collection";
      collectionName.insert(Name);
      elem_[i]=0;
   }
}

/////////////////////////
 E16G4_HBDSD::~E16G4_HBDSD(){
/////////////////////////
}

/////////////////////////////////////////////////////////
void E16G4_HBDSD::SetElement(int id, E16G4_BeamLineElement *elem)
/////////////////////////////////////////////////////////
{
   elem_[id]=elem;
}

//////////////////////////////////////////////////
void E16G4_HBDSD::Initialize(G4HCofThisEvent* HCTE){
//////////////////////////////////////////////////
   for (int i=0;i<nHBD;i++){
      hitsCollection[i]= 
         new HBDHitsCollection(SensitiveDetectorName, 
               collectionName[i]);
      G4int hcid= GetCollectionID(i);
      HCTE-> AddHitsCollection(hcid, hitsCollection[i]);
   }
}

/////////////////////////////////////////////
G4bool E16G4_HBDSD::ProcessHits(G4Step* aStep, 
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
   //   G4cerr <<"hbd "<< time <<G4endl;

   //Particle information
   G4ParticleDefinition *particle= 
      aTrack-> GetDynamicParticle()->GetDefinition();
   G4String pName=particle->GetParticleName();
   G4int PDGCode=particle->GetPDGEncoding();
   G4double steplength=aStep->GetStepLength();
   G4double edep=aStep->GetTotalEnergyDeposit();
   G4int trkid = aTrack->GetTrackID(); 

   //const G4VProcess *pr = aStepPoint->GetProcessDefinedStep();
   //G4ProcessManager *prman = pr->GetProcessManager();
   //G4cout << prman->GetParticleType()<<G4endl;
   const G4VProcess *proc=aTrack-> GetCreatorProcess();
   G4String procName;
   if(proc!=0){
      procName=proc->GetProcessName();
   }
   else{
      procName="Primary";
   }
   //G4cout <<"E16G4_HBDSD:"<<pName<<" "<<procName<<G4endl;

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
   //G4cout << "HBD::ProcessHits:VolName="<<Name<<G4endl;
   std::stringstream sid(Name.substr(3));
   int id=-1;sid>>id;
   //E16ANA_Transform *ptr = new E16ANA_Transform();
   if (id>=0){
      //int id = id/3;
      //G4cout << "HBD::ProcessHits:ID="<<id<<G4endl;
      //double theta=30.76*(4-int(id/3));
      //G4ThreeVector cent=elem_[id]->GetCent();
      //G4ThreeVector orig=elem_[id]->GetLocalPosition(gPos);
      //G4cout << "HBD::ProcessHits:cent="<<cent<<G4endl;
      //pos+=cent;
      //G4cout << "HBD::ProcessHits:pos="<<pos/cm<<G4endl;
      //G4cout << "HBD::ProcessHits:geom="<<geom_->HBDy[id]<<" "<<geom_->HBDz[id]<<G4endl;
      //G4cout << "HBD::ProcessHits:x,y,z="<<x<<" "<<y<<" "<<z<<G4endl;
      //TVector3 lPos_t3=ptr->GetLPos(
      //TVector3 lPos_t3=E16ANA_Transform::GetLPos(
      //      TVector3(pos.x()/cm,pos.y()/cm,pos.z()/cm),
      //      theta,geom_->HBDy[id],geom_->HBDz[id],0,0);
      //G4ThreeVector lPos(lPos_t3.X()*cm,lPos_t3.Y()*cm,lPos_t3.Z()*cm);
      G4ThreeVector lPos = geom_->HBD(id)->GetLPos(pos);
      //G4cout << "ProcessHits:lPos="<<lPos/cm<<G4endl;
      //G4ThreeVector lPos=elem_[id]->GetLocalPosition(pos);
      //G4ThreeVector lMom=elem_[id]->GetLocalDirection(mom);
      //TVector3 lMom_t3=ptr->GetLMom(TVector3(mom.x()/GeV,mom.y()/GeV,mom.z()/GeV),
      //TVector3 lMom_t3=E16ANA_Transform::GetLMom(TVector3(mom.x()/GeV,mom.y()/GeV,mom.z()/GeV),
      //      theta,0,0,0);
      //G4ThreeVector lMom(lMom_t3.X()*GeV,lMom_t3.Y()*GeV,lMom_t3.Z()*GeV);
      G4ThreeVector lMom = geom_->HBD(id)->GetLMom(mom);
      /*if (id==36){
        G4cout << "ProcessHits:pos="<<pos<<G4endl;
        G4cout << "ProcessHits:lPos="<<lPos<<G4endl;
        }*/

      G4double beta = aTrack-> GetVelocity()/c_light;
      G4double npe = 0.;
      G4double npeconst = 0.;
      /*if( beta >= ThresholdBeta && charge!=0 ) {
         npeconst = (steplength/m)*2.*pi/137.*
            (1.-1./(CF4ind*CF4ind*beta*beta))*CE;
         for (int i=0;i<ndata;i++){
            npe += npeconst*(1./WL[i]/WL[i])*dWL*QE[i];
         }// integrate with the wavelength
      }*/
      //G4cout <<"HBDSD:"<<npe<<" "<<beta<<" "<<mom.mag()/MeV<<G4endl;

      E16G4_HBDHit* aHit;
      /*const G4int nHits=hitsCollection[id]->entries();
      G4cout <<"HBDSD:"<<nHits<<" "<<PDGCode<<" "<<mom.mag()<<G4endl;
      for (int i=0;i<nHits;i++){
         aHit =(*hitsCollection[id])[i];
         if(id==aHit->GetID()){
            aHit->AddEdep(edep);
            aHit->AddLength(steplength);
            return true;
         }
      }*/
      aHit = new E16G4_HBDHit(pos, time, charge);
      aHit->SetGPos(pos);
      aHit->SetGMom(mom);
      aHit->SetLPos(lPos);
      aHit->SetLMom(lMom);
      aHit->SetEdep(edep);
      aHit->SetNpe(npe);
      aHit->SetBeta(beta);
      aHit->SetLength(steplength);
      aHit->SetPName(pName);
      aHit->SetPCode(PDGCode);
      aHit->SetProcName(procName);
      aHit->SetTrackID(trkid);
      hitsCollection[id]-> insert(aHit);

      //return true;
   }
   //delete ptr;
   return true;
}

///////////////////////////////////////////////////
void E16G4_HBDSD::EndOfEvent(G4HCofThisEvent* ){
   ///////////////////////////////////////////////////
}

//////////////////////////
void E16G4_HBDSD::DrawAll(){
   //////////////////////////
}

///////////////////////////
void E16G4_HBDSD::PrintAll(){
   ///////////////////////////
   for (int i=0;i<nHBD;i++){
      hitsCollection[i]-> PrintAllHits();
   }
}

