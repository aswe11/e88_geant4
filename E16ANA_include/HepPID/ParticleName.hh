#ifndef HepPID_ParticleName_hh
#define HepPID_ParticleName_hh

#include <string>
#include <TDatabasePDG.h>
#include <TParticlePDG.h>

namespace HepPID {
  inline std::string particleName(int pdgCode) {
    TDatabasePDG* pdgDB = TDatabasePDG::Instance();
    if (pdgDB) {
      TParticlePDG* particle = pdgDB->GetParticle(pdgCode);
      if (particle) {
        return particle->GetName();
      }
    }
    // Safe fallback if not found in ROOT database
    return "unknown(" + std::to_string(pdgCode) + ")";
  }
}

#endif // HepPID_ParticleName_hh
