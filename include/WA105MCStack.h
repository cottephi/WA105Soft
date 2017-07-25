// $Id: GlacierMCStack.h,v 1.1 2009/05/02 15:06:45 rubbiaa Exp $

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
// Class wa105MCStack
// -----------------
// Implementation of the TVirtualMCStack interface
//
// by Ivana Hrivnacova, 5.4.2002


#ifndef WA105_STACK_H
#define WA105_STACK_H

#include <TVirtualMCStack.h>
#include <stack>

#include "WA105Particle.h"

class WA105MCStack : public TVirtualMCStack
{
  public:
    WA105MCStack(Int_t size);
    WA105MCStack();
    virtual ~WA105MCStack();     

    // methods
    virtual void  PushTrack(Int_t toBeDone, Int_t parent, Int_t pdg,
  	              Double_t px, Double_t py, Double_t pz, Double_t e,
  		      Double_t vx, Double_t vy, Double_t vz, Double_t tof,
		      Double_t polx, Double_t poly, Double_t polz,
		      TMCProcess mech, Int_t& ntr, Double_t weight,
		      Int_t is) ;
    virtual TParticle* PopNextTrack(Int_t& track);
    virtual TParticle* PopPrimaryForTracking(Int_t i);    
    
    // set methods
    virtual void  SetCurrentTrack(Int_t track);                           

    // get methods
    virtual Int_t  GetNtrack() const;
    virtual Int_t  GetNprimary() const;
    virtual TParticle* GetCurrentTrack() const;    
    virtual Int_t  GetCurrentTrackNumber() const;
    virtual Int_t  GetCurrentParentTrackNumber() const;
    virtual Int_t  GetCurrentPrimaryTrackNumber() const;
    void Reset();
    Int_t                      fNPrimary;

  private:
    // methods
    WA105Particle* GetParticle(Int_t id) const;
  
    // data members
    std::stack<WA105Particle*>  fStack;    //!
    TObjArray*                 fParticles;
    Int_t                      fCurrentTrack;


    
    //ClassDef(WA105MCStack,1) // WA105MCStack
};

#endif //WA105_STACK_H   
   

