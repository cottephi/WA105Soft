// $Id: GlacierParticle.cc,v 1.1 2009/05/02 15:06:45 rubbiaa Exp $

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

//
// Geant4 novice ExampleN01 adapted to Virtual Monte Carlo 
//
// Class WA105Particle
// ------------------
// Extended TParticle with pointers to mother and daughters
// particles
//
// by Ivana Hrivnacova, 5.4.2002

#include "WA105Particle.h"

#include <TParticle.h>
#include <TObjArray.h>
#include <iostream>

//ClassImp(WA105Particle)

//_____________________________________________________________________________
WA105Particle::WA105Particle(Int_t id, TParticle* particle)
  : fID(id),
    fParticle(particle),
    fMother(0),
    fDaughters(0)    
{
//
}

//_____________________________________________________________________________
WA105Particle::WA105Particle(Int_t id, TParticle* particle, WA105Particle* mother)
  : fID(id),
    fParticle(particle),
    fMother(mother),
    fDaughters(0)    
{
//
}

//_____________________________________________________________________________
WA105Particle::WA105Particle()
  : fID(0),
    fParticle(0),
    fMother(0),
    fDaughters(0)    
{
//   
}

//_____________________________________________________________________________
WA105Particle::~WA105Particle() 
{
//
  delete fParticle;
  if (fDaughters) delete fDaughters;
}

// operators

// public methods

//_____________________________________________________________________________
void WA105Particle::AddDaughter(WA105Particle* particle)
{
// Add particles daughter
// ---

  if (!fDaughters) fDaughters = new TObjArray();
  
  fDaughters->Add(particle);
}  

//_____________________________________________________________________________
void WA105Particle::SetMother(WA105Particle* particle)
{
// Add particles daughter
// ---


  fMother = particle;
}  

//_____________________________________________________________________________
Int_t  WA105Particle:: GetID() const
{
// Returs particle ID.
// ---
 
  return fID;
}  


//_____________________________________________________________________________
TParticle*  WA105Particle::GetParticle() const
{
// Returns particle definition (TParticle).
// ---

   return fParticle;
}  

//_____________________________________________________________________________
WA105Particle* WA105Particle::GetMother() const
{
// Returns particle definition (TParticle).
// --
 
  return fMother;
}  

//_____________________________________________________________________________
Int_t WA105Particle::GetNofDaughters() const
{
// Returns number of daughters.
// ---
  
  if (!fDaughters) return 0;
  
  return fDaughters->GetEntriesFast();
}  

//_____________________________________________________________________________
WA105Particle* WA105Particle::GetDaughter(Int_t i) const
{
// Returns i-th daughter.
// ---

  // add test if i
  
 
  return (WA105Particle*) fDaughters->At(i);
}  

