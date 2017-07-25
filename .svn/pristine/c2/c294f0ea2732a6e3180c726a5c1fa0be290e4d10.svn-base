// $Id: GlacierMCApplication.h,v 1.1 2009/05/02 15:06:44 rubbiaa Exp $

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
// Class GlacierMCApplication
// ----------------------- 
// Implementation of the TVirtualMCApplication
//
// by Ivana Hrivnacova, 5.4.2002

#ifndef VMC_APPLICATION_H
#define VMC_APPLICATION_H

#include <iostream>
#include <fstream>

#include <TVirtualMCApplication.h>
#include <TVirtualMCStack.h>

#include "VMCPrimaryGenerator.h"

#include <TFile.h>
#include <TTree.h>

class RawDataVirtualMC;

class VMCApplication : public TVirtualMCApplication
{
 public:
  VMCApplication(const char *name, const char *title);
  VMCApplication();
  virtual ~VMCApplication();
  
  // static access method
  static VMCApplication* Instance(); 

  // methods
  void InitMC(const char* setup);
  void RunMC(Int_t nofEvents);
  void FinishRun();
 
  VMCPrimaryGenerator *GetPrimaryGenerator() { return fPrimaryGenerator; };
  void ConstructGeometry();
  void InitGeometry();
  void GeneratePrimaries();
  void BeginEvent();
  void BeginPrimary();
  void PreTrack();
  void Stepping();
  void PostTrack();
  void FinishPrimary();
  void FinishEvent();
  void Field(const Double_t* x, Double_t* b) const;
  
  void SetRawDataVirtualMC(RawDataVirtualMC *dat) {data=dat;}

  // used in debugging
  static int myDebugStepCounter;

 private:
  
  // data members
  RawDataVirtualMC *data;
  
  //
  TVirtualMCStack*  fStack;
  VMCPrimaryGenerator*  fPrimaryGenerator;
  Bool_t fOldGeometry;
};

// inline functions

inline VMCApplication* VMCApplication::Instance()
{ return (VMCApplication*)(TVirtualMCApplication::Instance()); }

#endif //GLACIER_MC_APPLICATION_H

