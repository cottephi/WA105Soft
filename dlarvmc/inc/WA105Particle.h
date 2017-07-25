// $Id: GlacierParticle.h,v 1.1 2009/05/02 15:06:45 rubbiaa Exp $

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


#ifndef WA105_PARTICLE_H
#define WA105_PARTICLE_H

#include <TObject.h>
#include <TParticle.h>

class TParticle;

class WA105Particle : public TObject
{
  public:
    WA105Particle(Int_t id, TParticle* particle);
    WA105Particle(Int_t id, TParticle* particle, WA105Particle* mother);
    WA105Particle();
    virtual ~WA105Particle();     

    // methods
    void SetMother(WA105Particle* particle);
    void AddDaughter(WA105Particle* particle);

    // get methods  
    Int_t         GetID() const;
    TParticle*    GetParticle() const;
    WA105Particle* GetMother() const;
    Int_t         GetNofDaughters() const;
    WA105Particle* GetDaughter(Int_t i) const;
    
  private:
    // data members
    Int_t         fID;
    TParticle*    fParticle;
    WA105Particle* fMother;
    TObjArray*    fDaughters;
    
    //ClassDef(WA105Particle,1) // Extended TParticle
};

#endif //WA105_PARTICLE_H   
   

