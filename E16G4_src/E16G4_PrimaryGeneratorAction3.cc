//2016-08-12, uploaded by nakai
//2015-09-05, uploaded by nakai

#include <TVector3.h>

void E16G4_PrimaryGeneratorAction::GenerateWireTargetEvent(G4Event* anEvent, int module_id){
   static int event_id = 0;
   G4ParticleTable *particle_table = G4ParticleTable::GetParticleTable();
   int n_part = 1;
   //G4int *Pdg = new G4int[n_part];
   //G4ThreeVector *gPos = new G4ThreeVector[n_part];
   //G4ThreeVector *gMom = new G4ThreeVector[n_part];
   vector<int> Pdg(n_part);
   vector<G4ThreeVector> gPos(n_part);
   vector<G4ThreeVector> gMom(n_part);

   G4double detector_phi = (5-module_id/3)*25.0*degree;
   G4double phi_width = 15.0*degree;
   double detector_theta = (4-module_id%3)*30.0*degree/rad;
   double theta_width = 15.0*degree/rad;

   // particle vertex
   double vertex_theta = RandFlat::shoot(0.,2.0*PI);
   double u = RandFlat::shoot(0.,(sigmaZ/cm)*(sigmaZ/cm));
   G4double vertex_r = sqrt(u)*cm;
   G4double vertex_y = RandGauss::shoot(0.,sigmaY);

   for(int i=0; i<n_part; i++){
      // particle definition
      G4ParticleDefinition *particle_definition;
      if(RandFlat::shoot(0.,1.)<0.5){
         particle_definition = particle_table->FindParticle("pi+");
      }else{
         particle_definition = particle_table->FindParticle("pi-");
      }
      Pdg[i] = particle_definition->GetPDGEncoding();

      // particle vertex
      gPos[i] = G4ThreeVector(
            vertex_r*sin(vertex_theta)+xPoint,
            //vertex_r*sin(vertex_theta),
            vertex_y+yPoint,
            //vertex_y,
            vertex_r*cos(vertex_theta)+zPoint
            );

      // particle momentum
      // ToDo : Generate from Sanford-Wang or JAM
      double particle_phi = RandFlat::shoot(detector_phi-phi_width, detector_phi+phi_width)/rad;
      G4double particle_cosphi = cos(particle_phi);
      G4double particle_sinphi = sin(particle_phi);
      G4double particle_costheta = RandFlat::shoot(
            cos(detector_theta+theta_width),
            cos(detector_theta-theta_width));
      G4double particle_sintheta = sqrt(1.0-particle_costheta*particle_costheta);
      //G4double particle_momentum = RandFlat::shoot(0.1,1.5)*GeV;
      TVector3 beam_direction(
            particle_sintheta*particle_sinphi,
            particle_costheta,
            particle_sintheta*particle_cosphi
            );
      double particle_theta = beam_direction.Theta();
      G4double particle_momentum = sw_gen->ShootFixedTheta(particle_theta)*GeV;
      gMom[i] = G4ThreeVector(
            particle_momentum*particle_sintheta*particle_sinphi,
            particle_momentum*particle_costheta,
            particle_momentum*particle_sintheta*particle_cosphi
            );

      G4ParticleGun gun(1);
      gun.SetParticleEnergy(0.0*GeV); // reset
      gun.SetParticleDefinition(particle_definition);
      gun.SetParticlePosition(gPos[i]);
      gun.SetParticleMomentum(gMom[i]);
      gun.GeneratePrimaryVertex(anEvent);
   }
   G4cout << "EventID = " << event_id << ", multiplicity = " << gPos.size() << G4endl;

   if(anaMan){
      int n = event_id;
      anaMan->PrimaryGenerationMulti(n, n_part, Pdg, gPos, gMom, event_id);
   }
   event_id++;
   //delete [] gPos;
   //delete [] gMom;
}

