// $Id: VMCApplication.cc,v 1.1 2009/05/02 15:06:44 rubbiaa Exp $

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

//
// Geant4 Glacier adapted to Virtual Monte Carlo 
//
// Class VMCApplication
// ----------------------- 
// Implementation of the TVirtualMCApplication
//
// by Ivana Hrivnacova, 5.4.2002

#include "VMCApplication.h"
#include "WA105MCStack.h"

#include <TROOT.h>
#include <TFile.h>
#include <Riostream.h>
#include <TInterpreter.h>
#include <TVirtualMC.h>
#include <TLorentzVector.h>
#include <TVector3.h>
#include <TArrayD.h>
#include <TGeoManager.h>
#include <TGeoMatrix.h>
#include <TSystem.h>
#include "Preferences.h"
#include "RawDataVirtualMC.h"

int VMCApplication::myDebugStepCounter = 0;

//_____________________________________________________________________________
VMCApplication::VMCApplication(const char *name, const char *title) 
: TVirtualMCApplication(name,title),
  fStack(0),
  fOldGeometry(kFALSE)
{
  // create a user stack
  const int kMaxTrack = 100;
  fStack = new WA105MCStack(kMaxTrack); 
  
  // create primary generator
  fPrimaryGenerator = new VMCPrimaryGenerator(fStack);
}

//_____________________________________________________________________________
VMCApplication::VMCApplication()
  : TVirtualMCApplication(),
    fStack(0),
    fOldGeometry(kFALSE)
{    
  // create a user stack
  const int kMaxTrack = 100;
  fStack = new WA105MCStack(kMaxTrack);

  // create primary generator
  fPrimaryGenerator = new VMCPrimaryGenerator(fStack);
}

//_____________________________________________________________________________
VMCApplication::~VMCApplication() 
{
  //
  // Destructor  
  //
  
  delete fStack;
  delete gMC;
}

//_____________________________________________________________________________
void VMCApplication::InitMC(const char* setup)
{    
  //
  // Initialize MC.
  //
 
  
  // load vmc
  gROOT->LoadMacro(setup);
  gInterpreter->ProcessLine("Config()");
 
 
  // set global cuts (before Init())
  const double MeV=1e-3;
  gMC->SetCut("CUTGAM", 10.0  * MeV);
  gMC->SetCut("CUTELE", 10.0  * MeV);
  gMC->SetCut("DCUTE",  10.0 * MeV);
  gMC->SetCut("DCUTM",  10.0 * MeV);
  gMC->SetCut("BCUTE",  10.0 * MeV);
  gMC->SetCut("BCUTM",  10.0 * MeV);
  //gMC->SetCut("PPCUTM", 10.0 * MeV);
  gMC->SetCut("TOFMAX", 1.0e10);
  
  gMC->SetStack(fStack); 
  gMC->Init();
  
  gROOT->LoadMacro("SpecialCuts.C");
  gInterpreter->ProcessLine("SpecialCuts()");
  
  gMC->BuildPhysics(); 
}

//_____________________________________________________________________________
void VMCApplication::RunMC(Int_t nofEvents)
{    
  //
  // MC run.
  //
  ((WA105MCStack *)fStack)->Reset();
  gMC->ProcessRun(nofEvents);
 
  FinishRun();
}

//_____________________________________________________________________________
void VMCApplication::FinishRun()
{    
  //
  
  //
}

//_____________________________________________________________________________
void VMCApplication::ConstructGeometry()
{    
  //
  // Construct geometry using TVirtualMC functions.
  //
  
  // Cannot use Root geometry if not supported with 
  // selected MC
  if ( !fOldGeometry && ! gMC->IsRootGeometrySupported() ) {
    cerr << "Selected MC does not support TGeo geometry"<< endl;
    cerr << "Exiting program"<< endl;
    exit(1);
  } 

  // notify VMC about Root geometry
  gMC->SetRootGeometry();
}

//_____________________________________________________________________________
void VMCApplication::InitGeometry()
{    
  // nothing to be done (use root geometry)
}


//_____________________________________________________________________________
void VMCApplication::GeneratePrimaries()
{    
  fPrimaryGenerator->GeneratePrimaries();
  
  // save vertex and direction to event header
  data->SetTruePhi(fPrimaryGenerator->phiPri);
  data->SetTrueTht(fPrimaryGenerator->thetaPri);
  data->SetTrueVx(fPrimaryGenerator->vxPri);
  data->SetTrueVy(fPrimaryGenerator->vyPri);
  data->SetTrueVz(fPrimaryGenerator->vzPri);

  // save momentum information of the primary particle (beam)
  data->SetTrueP(fPrimaryGenerator->pPri);
  data->SetTruePx(fPrimaryGenerator->pxPri);
  data->SetTruePy(fPrimaryGenerator->pyPri);
  data->SetTruePz(fPrimaryGenerator->pzPri);
}

//_____________________________________________________________________________
void VMCApplication::BeginEvent()
{    

  myDebugStepCounter = 0;
  //cout<<" ----------------------> Random number from gMC "<< gMC->GetRandom() << endl;
  //cout<<"                                                "<< gMC->GetRandom()->ClassName()<<endl;
  //cout<<"                                                "<< gMC->GetRandom()->GetSeed()<<endl;
  //cout<<" gRandom -->                   "<<gRandom<<" "<<gRandom->GetSeed()<<endl; 

  // initalize to 0 at the beginning of event
  data->SetEdep( 0 );
  data->SetEdepQ( 0 );
  data->SetEdepE( 0 );
  data->SetEdepEQ( 0 );
  data->SetEdepH( 0 );
  data->SetEdepHQ( 0 );
  data->SetEdepBeam( 0 );
  data->SetEdepCR( 0 );
  data->SetElost( 0 );
  data->SetEdepCRT1( 0 );
  data->SetEdepCRT2( 0 );
  // clear array of primaries
  //data->GetPrimaries()->Clear();
  data->ClearPrimaries();
  

  // primary beam parameters
  fPrimaryGenerator->pdgPri = 0;
  fPrimaryGenerator->vxPri = 0;
  fPrimaryGenerator->vyPri = 0;
  fPrimaryGenerator->vzPri = 0;
  fPrimaryGenerator->phiPri   = 0;
  fPrimaryGenerator->thetaPri = 0;
  fPrimaryGenerator->pPri  = 0;
  fPrimaryGenerator->pxPri = 0;
  fPrimaryGenerator->pyPri = 0;
  fPrimaryGenerator->pzPri = 0;
}

//_____________________________________________________________________________
void VMCApplication::BeginPrimary()
{   
  //
  // User actions at beginning of a primary track
  //
  /*
  TLorentzVector momentum;
  TParticle *particle = fStack->GetCurrentTrack();
  particle->Momentum(momentum);
  cout<< " begin primary "<< fStack->GetCurrentTrack()->GetPdgCode() << endl;
  */

  // add the primary to the obj array for storage
  TParticle *particle = fStack->GetCurrentTrack();
  data->AddPrimary( particle );
}

//_____________________________________________________________________________
void VMCApplication::PreTrack()
{;}

//_____________________________________________________________________________
void VMCApplication::Stepping()
{    
  //
  // User actions at each step
  //
   
  //step size and max. number of steps
  int maxNStep = 10000000;
  
  gMC->SetMaxNStep(maxNStep);
  
  if (strcmp("ACTIVE VOLUME",gMC->CurrentVolName())==0)
    gMC->SetMaxStep(0.05);  //set step size to 0.5 mm in ACTIVE VOLUME
  else 
    gMC->SetMaxStep(1.0);   //otherwise set it to 1.0 cm
  
  //Set step size to 1 km for geometry tests only 
  //gMC->SetMaxStep(10000.0);                     

  double edep = gMC->Edep()*1.0E+3; //edep in MeV
  double currStep = gMC->TrackStep();

  //int trackID = gMC->GetStack()->GetCurrentTrackNumber();
  int pdg = gMC->TrackPid();
      
  TLorentzVector position;
  gMC->TrackPosition(position);
  double pos[3] = {position.X(), position.Y(), position.Z()};
  
  
  //if(myDebugStepCounter < 15 )
  //{
  //cout<<endl<< "==================================================> stepping information "<<myCounter<< endl;
  //position.Print();
  //  cout<<"         Volume = "<<gMC->CurrentVolName()<<endl;
  //cout<<"         Energy = "<<edep<<endl;
  //cout<<"         PDG    = "<<pdg<<endl;
  //cout<<"         currStep  = "<<currStep<<endl;
  //cout<<gMC->IsTrackExiting()<<" "<<gMC->IsTrackEntering()<<" "<<gMC->IsTrackInside()<<endl;
  //myDebugStepCounter++;
  //}
  
  // store step 
  data->StoreTrackStep(pdg, pos, position.T(), edep, currStep); 
  
  //if(myDebugStepCounter < 15 )
  //cout<< "==================================================> "<<endl<<endl;
}

//_____________________________________________________________________________
void VMCApplication::PostTrack()
{;}

//_____________________________________________________________________________
void VMCApplication::FinishPrimary()
{;}    


//_____________________________________________________________________________
void VMCApplication::FinishEvent()
{;} 

//_____________________________________________________________________________
void VMCApplication::Field(const Double_t* /*x*/, Double_t* b) const
{
  // 
  // No magnetic field.
  //
  
  b[0] = 0.;
  b[1] = 0.;
  b[2] = 0.;
}
