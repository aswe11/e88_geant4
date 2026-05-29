//2016-08-12, uploaded by nakai
//2015-01-05, uploaded by yokkaich
//2015-01-05, uploaded by yokkaich
//2014-08-27, uploaded by kawama
//2014-08-27, uploaded by kawama
//2014-04-30, uploaded by kawama
//2013-11-14, uploaded by kawama
//2013-05-13, modified by kawama
/*
  E16G4_VDCylinderHit.hh

  D.Kawama
*/

#ifndef E16G4_VDCylinderHit_h
#define E16G4_VDCylinderHit_h 1

#include <G4VHit.hh>
#include <G4THitsCollection.hh>
#include <G4ThreeVector.hh>
#include <G4Allocator.hh>

class G4ParticleDefinition;

class E16G4_VDCylinderHit : public G4VHit
{
public:
  E16G4_VDCylinderHit( G4int id, const G4ThreeVector &gPos, const G4ThreeVector &gMom,
	    G4double edep, 
		 G4ThreeVector Vertex_,
		 G4String &name);
  ~E16G4_VDCylinderHit();

private:
  E16G4_VDCylinderHit( const E16G4_VDCylinderHit &  );
  E16G4_VDCylinderHit & operator = ( const E16G4_VDCylinderHit & );

public:
  int operator == ( const E16G4_VDCylinderHit & ) const
  { return 0; }

  inline void* operator new( size_t size );
  inline void operator delete( void* aHit );

  void Draw() ;
  void Print();

private:
  G4int VDId_, tID_;
  G4int PDG_;
  G4ThreeVector gPos_ , gMom_;
  G4ThreeVector lPos_, lMom_;
  G4double Edep_;
  G4double tLength_;
  G4ThreeVector Vertex_;
  G4String name_;
  G4String procname_;
  const G4ParticleDefinition *part_;

public:
  void SetGPos( const G4ThreeVector & pos ) { gPos_ = pos; }
  void SetGMom( const G4ThreeVector & mom ) { gMom_ = mom; }
  void SetLPos( const G4ThreeVector & pos ) { lPos_ = pos; }
  void SetLMom( const G4ThreeVector & mom ) { lMom_ = mom; }
  void SetEdep( const G4double & Sedep ) { Edep_ = Sedep; }
  void AddEdep( const G4double & Aedep ) { Edep_ = Aedep; }
  void SetVertex( const G4ThreeVector & ver ) { Vertex_ = ver; }
  void SettLength( const G4double & tlen ) { tLength_ = tlen; }
  void SetParticle( G4String &name ) { name_ = name; }
  void SetParticle( G4ParticleDefinition *part ) { part_ = part; }
  void SetPCode( G4int pdg ) { PDG_ = pdg; }
  void SetTrackID( G4int tid ) { tID_ = tid; }
  void SetProcName( G4String &procname ) { procname_ = procname; }

  const G4ThreeVector & GetLPos() const { return lPos_; }
  const G4ThreeVector & GetGPos() const { return gPos_; }
  const G4ThreeVector & GetLMom() const { return lMom_; }
  const G4ThreeVector & GetGMom() const { return gMom_; } 
  const G4double & GetEdep() const { return Edep_; } 
  G4int GetVDid( void ) const { return VDId_; }
  const G4ThreeVector & GetVertex() const { return Vertex_; } 
  const G4ParticleDefinition *GetParticle() const { return part_; }
  const G4String & GetParticleName() const { return name_;}
  const G4double & GettLength() const { return tLength_; }
  G4int GetPCode() const { return PDG_;}
  G4int GetTrackID() const { return tID_;}
  const G4String & GetProcName() const { return procname_;}

};

typedef G4THitsCollection<E16G4_VDCylinderHit> VDCylinderHitsCollection;

extern G4Allocator<E16G4_VDCylinderHit> VDCylinderHitAllocator;

inline void* E16G4_VDCylinderHit::operator new( size_t )
{
   return static_cast<void*>( VDCylinderHitAllocator.MallocSingle() );
}

inline void E16G4_VDCylinderHit::operator delete( void* aHit )
{
   VDCylinderHitAllocator.FreeSingle
      ( static_cast<E16G4_VDCylinderHit *>( aHit ) );
}

#endif//E16G4_VDCylinderHit_h

