//2016-08-12, uploaded by nakai
//2015-03-01, uploaded by yokkaich
//2015-01-05, uploaded by yokkaich
//2015-01-05, uploaded by yokkaich
//2014-08-27, uploaded by kawama
//2014-08-27, uploaded by kawama
//2014-04-30, uploaded by kawama
//2013-11-14, uploaded by kawama
//2013-05-13, modified by kawama

#ifndef E16G4_GTR_HIT_HH
#define E16G4_GTR_HIT_HH

#include <G4ThreeVector.hh>
#include <G4VHit.hh>
#include <G4THitsCollection.hh>
#include <G4Allocator.hh>

class G4ParticleDefinition;

class E16G4_GTRHit : public G4VHit{
   private:
      G4int ID_, segID_, trackID_, parentID_, PDGCode_;
      G4ThreeVector xyz_;
      G4ThreeVector gpos_,lpos_;
      G4ThreeVector gmom_,lmom_;
      G4double edep_,time_, beta_,length_;
      G4double charge_;
      const G4ParticleDefinition *part_;
      G4String name_,crProcName_;
      G4bool fSignal_;
   public:
      E16G4_GTRHit();
      E16G4_GTRHit(const G4ThreeVector& axyz, G4double t, G4double acharge);
      virtual ~E16G4_GTRHit();

      E16G4_GTRHit(const E16G4_GTRHit& right);
      const E16G4_GTRHit& operator=(const E16G4_GTRHit& right);

      void* operator new(size_t);
      void operator delete(void* aHit);

      const G4ThreeVector& GetPosition(){return xyz_;}

      G4double GetCharge(){return charge_;}
      void SetID( G4int id ) { ID_ = id; }
      void SetSegID( G4int id ) { segID_ = id; }
      void SetTrackID( G4int id ) { trackID_ = id; }
      void SetParentID( G4int id ) { parentID_ = id; }
      void SetGPos( const G4ThreeVector & pos ) { gpos_ = pos; }
      void SetGMom( const G4ThreeVector & mom ) { gmom_ = mom; }
      void SetLPos( const G4ThreeVector & pos ) { lpos_ = pos; }
      void SetLMom( const G4ThreeVector & mom ) { lmom_ = mom; }
      void SetEdep( G4double E ) { edep_ = E; }
      void AddEdep( G4double E ) { edep_ += E; }
      void SetTime( G4double T ) { time_ = T; }
      void SetBeta( G4double v ) { beta_ = v; }
      void SetTrueSignal() { fSignal_ = true; }
      void SetFalseSignal() { fSignal_ = false; }
      void SetParticle( G4ParticleDefinition *par ) { part_=par; }
      void SetLength( G4double len ) { length_=len; }
      void AddLength( G4double len ) { length_+=len; }
      //void SetNpe( G4double N ) { npe_ = N; }
      //void AddNpe( G4double N ) { npe_ += N; }
      void SetPName( const G4String &name ) { name_ = name; }
      void SetPCode( const G4int &PDGCode ) { PDGCode_ = PDGCode; }
      void SetcrProcName( G4String &name ) { crProcName_ = name; }

      G4int GetID( void ) const { return ID_; }
      G4int GetSegID( void ) const { return segID_; }
      G4int GetTrackID( void ) const { return trackID_; }
      G4int GetParentID( void ) const { return parentID_; }
      const G4ThreeVector & GetGPos() const { return gpos_; }
      const G4ThreeVector & GetGMom() const { return gmom_; }
      const G4ThreeVector & GetLPos() const { return lpos_; }
      const G4ThreeVector & GetLMom() const { return lmom_; }
      G4double GetEdep() const { return edep_; }
      G4double GetTime() const { return time_; }
      G4double GetBeta() const { return beta_; }
      G4bool IsTrueSignal() const { return fSignal_; }
      const G4ParticleDefinition *GetParticle() const { return part_; }
      G4double GetLength() const { return length_; }
      //G4double GetNpe() const { return npe_; }
      const G4String &GetPName() const { return name_;}
      const G4int &GetPCode() const { return PDGCode_;}
      const G4String &GetcrProcName() const { return crProcName_;}

      virtual void Draw();
      virtual void Print();
};

inline E16G4_GTRHit::E16G4_GTRHit(const E16G4_GTRHit& right)
   : G4VHit(){
      xyz_= right.xyz_;
      time_= right.time_;
      charge_= right.charge_;
   }

inline const E16G4_GTRHit& E16G4_GTRHit::operator=(const E16G4_GTRHit& right){
   xyz_= right.xyz_;
   time_= right.time_;
   charge_= right.charge_;
   return *this;
}

typedef G4THitsCollection<E16G4_GTRHit> GTRHitsCollection;

extern G4Allocator<E16G4_GTRHit> GTRHitAllocator;

inline void* E16G4_GTRHit::operator new(size_t){
   void* aHit= (void*)GTRHitAllocator.MallocSingle();
   return aHit;
}

inline void E16G4_GTRHit::operator delete(void* aHit){
   GTRHitAllocator.FreeSingle((E16G4_GTRHit*) aHit);
}

#endif//E16G4_GTR_HIT_HH


