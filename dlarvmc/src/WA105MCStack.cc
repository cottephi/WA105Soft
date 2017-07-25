// $Id: GlacierMCStack.cc,v 1.1 2009/05/02 15:06:44 rubbiaa Exp $

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

//
// Geant4 ExampleN01 adapted to Virtual Monte Carlo 
//
// Class WA105MCStack
// -----------------
// Implementation of the TVirtualMCStack interface
//
// by Ivana Hrivnacova, 5.4.2002

#include "WA105MCStack.h"

#include <TParticle.h>
#include <TObjArray.h>
#include <TError.h>
#include <iostream>

//ClassImp(WA105MCStack)

//_____________________________________________________________________________
WA105MCStack::WA105MCStack(Int_t size)
: fNPrimary(0),
  fParticles(0),
  fCurrentTrack(-1)
{
//
   fParticles = new TObjArray(size);
}

//_____________________________________________________________________________
WA105MCStack::WA105MCStack()
  : fNPrimary(0),
    fParticles(0),
    fCurrentTrack(-1)
{
 
//_____________________________________________________________________________
}
WA105MCStack::~WA105MCStack() 
{
//
   if (fParticles) fParticles->Delete();
  delete fParticles;
}

//_____________________________________________________________________________
void WA105MCStack::Reset()
{
 
  if (fParticles) fParticles->Delete();
  delete fParticles;
  fCurrentTrack=-1;
  fNPrimary=0;
  fParticles = new TObjArray(100);
}
// private methods

//_____________________________________________________________________________
WA105Particle*  WA105MCStack::GetParticle(Int_t id) const
{
// Returns id-th particle in fParticles.
// ---
  
  if (id < 0 || id >= fParticles->GetEntriesFast())
    Fatal("GetParticle", "Index out of range"); 
   
  return (WA105Particle*)fParticles->At(id);
}

// public methods

//_____________________________________________________________________________
void  WA105MCStack::PushTrack(Int_t toBeDone, Int_t parent, Int_t pdg,
  	                 Double_t px, Double_t py, Double_t pz, Double_t e,
  		         Double_t vx, Double_t vy, Double_t vz, Double_t tof,
		         Double_t polx, Double_t poly, Double_t polz,
		         TMCProcess mech, Int_t& ntr, Double_t weight,
		         Int_t is) 
{
// Creates a new particle with specified properties,
// adds it to the particles array (fParticles) and if not done to the 
// stack (fStack).
// ---
 
  const Int_t kFirstDaughter=-1;
  const Int_t kLastDaughter=-1;
   
  TParticle* particleDef
    = new TParticle(pdg, is, parent, -1, kFirstDaughter, kLastDaughter,
		     px, py, pz, e, vx, vy, vz, tof);
   
  particleDef->SetPolarisation(polx, poly, polz);
  particleDef->SetWeight(weight);
  particleDef->SetUniqueID(mech);

  WA105Particle* mother = 0;
  if (parent>=0) 
    mother = GetParticle(parent);
  else
    fNPrimary++;  

  WA105Particle* particle = new WA105Particle(GetNtrack(), particleDef, mother);

  fParticles->Add(particle);
    
  if (toBeDone) fStack.push(particle);    
  
  ntr = GetNtrack() - 1;   
}			 

//_____________________________________________________________________________
TParticle* WA105MCStack::PopNextTrack(Int_t& itrack)
{
// Gets next particle for tracking from the stack.
// ---
 
  itrack = -1;
  if  (fStack.empty()) return 0;
		      
  WA105Particle* particle = fStack.top();
  fStack.pop();

  if (!particle) return 0;  
  
  itrack = particle->GetID();
  fCurrentTrack = itrack;

  return particle->GetParticle();
}    

//_____________________________________________________________________________
TParticle* WA105MCStack::PopPrimaryForTracking(Int_t i)
{
// Returns i-th particle in fParticles.
// ---
   
  if (i < 0 || i >= fNPrimary)
    Fatal("GetPrimaryForTracking", "Index out of range"); 
  
  return ((WA105Particle*)fParticles->At(i))->GetParticle();
}     

//_____________________________________________________________________________
void  WA105MCStack::SetCurrentTrack(Int_t track) 
{
// Sets the current track to a given value.
// ---
 
  fCurrentTrack = track;
}     

//_____________________________________________________________________________
Int_t  WA105MCStack::GetNtrack() const 
{
// Returns the number of all tracks.
// ---

    return fParticles->GetEntriesFast();
}  

//_____________________________________________________________________________
Int_t  WA105MCStack::GetNprimary() const 
{
// Returns the number of primary tracks.
// ---

   return fNPrimary;
}  

//_____________________________________________________________________________
TParticle* WA105MCStack::GetCurrentTrack() const
{
// Gets the current track particle.
// ---

 
  WA105Particle* current = GetParticle(fCurrentTrack);
  
  if (current) 
    return  current->GetParticle();
  else 
    return 0;
}

//_____________________________________________________________________________
Int_t  WA105MCStack::GetCurrentTrackNumber() const 
{
// Returns the current track ID.
// ---
 
  return fCurrentTrack;
}  
//_____________________________________________________________________________
Int_t  WA105MCStack::GetCurrentParentTrackNumber() const 
{
// Returns the current track parent ID.
// ---

  
  WA105Particle* current = GetParticle(fCurrentTrack);
  
  if (!current) return -1; 
  
  WA105Particle* mother = current->GetMother();
  
  if (!mother) return -1;
    
  return  mother->GetID();
}  
//_____________________________________________________________________________
Int_t  WA105MCStack::GetCurrentPrimaryTrackNumber() const
{
// Returns the current primary track ID.
// ---

 
  WA105Particle* current = GetParticle(fCurrentTrack);
  
  if (!current) return -1; 

  WA105Particle* mother = current->GetMother();

  WA105Particle* parent = current;

  while(mother) {
    parent = mother;
    mother = mother->GetMother();
  }
 
  return parent->GetID();
}

