/***********
 *
 *
 *
 *
 *
 *
 *
 *
***********/



#ifndef GLARPROC_H
#define GLARPROC_H

//
#include <iostream>
#include <stdio.h>
#include <vector>

//
#include <TROOT.h>
#include <TString.h>
#include <TMath.h>
#include <TH1F.h>

//
#include "HEPSystemOfUnits.h"

//#include "NoElProcConfiguration.h"
#include "GLArProcDB.h"


using std::cout;
using std::endl;










//------------------------------------------------
//
//
//
//
//
//
//
//------------------------------------------------




class GLArProc {

public:

  GLArProc();
  GLArProc(const GLArProc&);
  virtual ~GLArProc();


  void Init() const ;
  void Clear() const;
  void ClearSettings() const;
  void ClearYields() const;
  void ClearParticle() const;
  void ClearResults() const;

  enum Phase   { kLiquid, kGas};
  enum Element { kArgon, kXenon};
  enum Type    { kLAr, kGAr, kLXe, kGXe, nTypes};


  int Compute() const;

  //  bool SetParticle(TString particle, int ZQ, float A=0) const;
  bool SetParticle(int pdg) const;
  bool SetEnergy(float deposited, float step, float kinetic) const;

  bool SetElectricField(float EField) const;

  bool IsApplicable() const { return fIsApplicable; }

  int BinomFluct(int N0, double prob) const;

  void SetHighEnergyLowerLimit(float /*MeV*/) const;

  bool SetElement(Element,Phase) const;
  bool SetPhase(Phase) const;

  void SetDensity(float) const;

  void UseLArRecombData(bool) const;

  void Print() const;


  float GetOpPhMeanEne() const {return fOPMeanEne;}
  float GetOpPhEneWidth() const {return fOPEneWidth;}

  float GetMeanNumOfEle() const { return fMeanNumOfEle;}
  int   GetNumOfEle() const { return fNumOfEle;}
  float GetMeanNumOfPhot() const { return fMeanNumOfPhot;}
  int   GetNumOfPhot() const { return fNumOfPhot;}
  int   GetNumOfExcitons() const {return fNumOfExcit;}

  float GetSingToTripRatioExcit() const {return fSingletToTripletX;}
  float GetSingToTripRatioRecomb() const {return fSingletToTripletR;}

  float GetTauSinglet() const { return fTauSinglet;}
  float GetTauTriplet() const { return fTauTriplet;}
  float GetTauRecomb()  const { return fTauRecomb;}
  float GetTauRise()    const { return fTauRise;}

  float GetRecombProb() const {return fRecombProb;}
  float GetDokeBirksA() const {return fDokeBirks_A;}
  float GetDokeBirksB() const {return fDokeBirks_B;}
  float GetDokeBirksC() const {return fDokeBirks_C;}

  float SampleScintTime(float tau_rise, float tau_decay) const;

  float Scint_bi_exp(float time, float tau_rise, float tau_decay) const;
  float Scint_single_exp(float time, float tau) const;

   void GenerateScintTime(float &time, bool& isSinglet, bool &isExcit);


  float GetLET() const {return fLET;}






  private:


  int ComputeArgon() const;

  private:

  mutable TString fParticleName;

  mutable bool fIsApplicable;
  mutable bool fIsElectron;
  mutable bool fIsMuon;
  mutable bool fIsNeutron;
  mutable bool fIsAlpha;
  mutable bool fIsNucleus;

  mutable bool fIsHighEnergy;
  mutable bool fIsNuclearRecoil;
  mutable bool fIsExcitedNucleaus;
  mutable bool fIsShortTrack;

  mutable bool fModelDokeBirks;
  mutable bool fModelThomasImel;
  mutable bool fModelLindhard;

  mutable bool fUseLArRecombData;

  mutable float fRecombProb;
  mutable float fExcitationRatio;
  mutable float fYieldFactor;

  mutable float fDokeBirks_A;
  mutable float fDokeBirks_B;
  mutable float fDokeBirks_C;
  mutable float fThomasImel;

  mutable Phase fPhase;
  mutable Element fElement;

  mutable int fMaterialZ;
  mutable float fMaterialA;
  mutable float fRefDensity;
  mutable float fDensity;

  mutable int   fPartZ;
  mutable float fPartA;

  mutable float fKin;
  mutable float fdE;
  mutable float fdx;
  mutable float fLET;
  mutable float fElectronLET;

  mutable float fElectricField;
  mutable bool  fIsZeroField;

  mutable float fMeanNumOfQuanta;
  mutable float fMeanNumOfEle;
  mutable float fMeanNumOfPhot;
  mutable float fMeanNumOfExcit;
  mutable float fMeanNumOfIon;

  mutable int fNumOfQuanta;
  mutable int fNumOfEle;
  mutable int fNumOfPhot;
  mutable int fNumOfExcit;
  mutable int fNumOfIon;

  mutable float fSingletToTripletX;
  mutable float fSingletToTripletR;

  mutable float fTauSinglet;
  mutable float fTauTriplet;
  mutable float fTauRecomb;
  mutable float fTauRise;

  mutable float fOPMeanEne;
  mutable float fOPEneWidth;


private:
  mutable float fHighEnergyLowerLimit;


};


//
// -------------------------------------
//




#endif
