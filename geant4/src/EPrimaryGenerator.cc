#include "EPrimaryGenerator.hh"

EPrimaryGenerator::EPrimaryGenerator()
{
    fMessenger = new G4GenericMessenger(this, "/E_source/", "Settings for the source");
    fMessenger->DeclareProperty("sourcePosition", sourcePositionR, "Position of point source");

    // fMessenger->DeclareProperty("sampleDiameter", sampleDiameterR, "Sample diameter (mm)");
    // fMessenger->DeclareProperty("sampleHeight", sampleThicknessR, "Sample thickness (mm)");
    
    sourcePositionR = 1.25;

    G4int n_particle = 1;
    fParticleGun  = new G4ParticleGun(n_particle);

    fParticleGun->SetParticleEnergy(0*eV);
    fParticleGun->SetParticlePosition(G4ThreeVector(0.,0.,0.));
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(1.,0.,0.));
}

EPrimaryGenerator::~EPrimaryGenerator()
{
    delete fParticleGun;
}

void EPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    // If the particle has not bee assigned yet, assign it as an ion
    if (fParticleGun->GetParticleDefinition() == G4Geantino::Geantino())
    {
        G4double sourcePosition = sourcePositionR * mm;
        G4int Z = 55, A = 137;
        G4double excitEnergy = 0. * keV;
        G4IonTable *ionTable = G4IonTable::GetIonTable();
        G4ParticleDefinition *ion = ionTable->GetIon(Z, A, excitEnergy);

        fParticleGun->SetParticleDefinition(ion);
        fParticleGun->SetParticleCharge(0. * eplus);
        fParticleGun->SetParticleEnergy(0 * eV);
        fParticleGun->SetParticlePosition(G4ThreeVector(0.,0.,sourcePosition));
    }

    // New random position inside filter source
    G4double filterDiameter = 27.4 * mm;
    G4double filterHeight = 5.71 * mm;
    // Randomized position (corrected for uniform sampling in cylinder)
    G4double rho = G4UniformRand() * (filterDiameter/2) * (filterDiameter/2);
    G4double phi = G4UniformRand() * 2 * pi;

    G4double x = std::cos(phi) * std::sqrt(rho);
    G4double y = std::sin(phi) * std::sqrt(rho);
    G4double z = filterHeight * (G4UniformRand() - 0.5);

    fParticleGun->SetParticlePosition(G4ThreeVector(x,y,z));

    // Create vertex
    fParticleGun->GeneratePrimaryVertex(anEvent);
}
