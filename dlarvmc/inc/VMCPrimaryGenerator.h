/**
 * \file VMCPrimaryGenerator.h
 * \author Devis Lussi
 * \date 28-Jul-2011
 */

/* fullreco -- Full reconstruction software
 * (C) 2000 - 2011,  The fullreco Team
 *
 * fullreco is mainly intended for academic purposes. Proper
 * credit must be given if you use fullreco or parts of it. Please
 * read the section 'Credit' in the README file.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef VMC_PRIMARYGENERATOR_H
#define VMC_PRIMARYGENERATOR_H

#include <iostream>

#include "TRandom3.h"
#include "RawDataGenie.h"
#include "WA105MCStack.h"
#include "WA105CRFlux.h"
#include "BeamHalo.h"

class VMCPrimaryGenerator
{
 public:
  VMCPrimaryGenerator(TVirtualMCStack *stack);
  virtual ~VMCPrimaryGenerator();

  // types
  enum Type {kSector, kGENIE, kGENIE_nd, kWA105};
  
  // methods
  
  void GeneratePrimaries();
   
  void SetType(Type type) { fType=type; };
  void SetNPrimaries(int number) { fNPrimaries=number; };
  void SetPdgCode(int code) { fPdgCode=code; };
  void SetP(double p) { fPMin=fPMax=p; };
  void SetPMin(double pmin) { fPMin=pmin; };
  void SetPMax(double pmax) { fPMax=pmax; };
  void SetPSigma(double psigma) { fPSigma=psigma; };
  void SetThetaMinMax(double min, double max) { fThetaMin=min;fThetaMax=max; };
  void SetVThetaMinMax(double min, double max) { fVThetaMin=min;fVThetaMax=max; };
  void SetRadiusMinMax(double min, double max) { fRadiusMin=min;fRadiusMax=max; };
  void SetPhiMinMax(double min, double max) { fPhiMin=min;fPhiMax=max; };
  void SetAngularDirection(double theta, double phi) { fThetaMin=fThetaMax=theta;fPhiMin=fPhiMax=phi; };
  void SetVxMinMax(double xmin,double xmax) { fVxMin=xmin;fVxMax=xmax; }
  void SetVyMinMax(double ymin,double ymax) { fVyMin=ymin;fVyMax=ymax; }
  void SetVzMinMax(double zmin,double zmax) { fVzMin=zmin;fVzMax=zmax; }
  
  void SetRawDataGeniePtr(RawDataGenie *ptr){ inputGeniePtr = ptr; }

  //--
  void SetWA105BeamOn(bool isTrue) { isWa105BeamOn = isTrue; }
  void SetWA105CRBackgroundOn(bool isTrue) { isWa105CRBkgOn = isTrue; }
  bool GetWA105BeamOn(){ return isWa105BeamOn; }
  bool GetWA105CRBackgroundOn(){ return isWa105CRBkgOn; }
  void ConfigWA105CRBackground( TString fname, float t0, float twin, float ratenorm ); 

  //--
  void SetWA105BHBackgroundOn(bool isTrue) { isWa105BHBkgOn = isTrue; }
  void ConfigWA105BHBackground(  float t0, float twin, float rate,
				 double x0, double thx0, double sgmx, 
				 double epsx, double alphx, 
				 double y0, double thy0, double sgmy, 
				 double epsy, double alphy );

  void SetVMCSeed(UInt_t seed) //{vmc_seed = seed;}
  { vmc_seed = seed; vmc_rndm->SetSeed(vmc_seed); }

  // beam parameters
  Int_t    pdgPri;
  Double_t vxPri;
  Double_t vyPri;
  Double_t vzPri;
  Double_t phiPri;
  Double_t thetaPri;
  Double_t pxPri;
  Double_t pyPri;
  Double_t pzPri;
  Double_t pPri;
  Double_t tof;
  
  Type fType;                        // generator type

 private:
  // methods
  void GenerateGENIEPrimary();
  void GenerateGENIE_ND_Primary();
  void GeneratePrimary_Sector();
  //void GeneratePrimary_Cone();
 
  //WA105 cosmics
  void GenerateWA105BkgPrimary(particle_fld &cryvec);
  void GenerateWA105EventPrimary();

  // data members
  TVirtualMCStack *fStack;

  // Genie input
  RawDataGenie *inputGeniePtr;
  
  int fNPrimaries;                   // number of primaries per event
  int fPdgCode;                      // pdg code, default: e-
  double fPMin,fPMax,fPSigma;        // momentum (GeV/c), default: 1GeV
  double fThetaMin,fThetaMax;        // ang. direction theta (ark)
  double fVThetaMin,fVThetaMax;      // vertex theta in cylindrical coordinates
  double fRadiusMin,fRadiusMax;      // vertex radius in cylindrical coordinates
  double fPhiMin,fPhiMax;            // ang. direction phi (ark)
  double fVxMin,fVxMax;              // x position (min, max) in cm
  double fVyMin,fVyMax;              // y position (min, max) in cm
  double fVzMin,fVzMax;              // z position (min, max) in cm
 
  TRandom3 *vmc_rndm ;  // random number generator
  UInt_t  vmc_seed;     // seed    
 

  // for WA105 simulation CR backgrounds
  WA105CRYFlux *wa105CRYflux;
  bool isWa105BeamOn;
  bool isWa105CRBkgOn;
  float fWA105CRT0;
  float fWA105CRTWin;

  // to generate muon beam halo
  bkgsrc::WA105HaloFlux *wa105BHflux;
  bool isWa105BHBkgOn;
  float fWA105BHT0;
  float fWA105BHTWin;
};

#endif
