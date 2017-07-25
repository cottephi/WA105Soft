#ifndef __RECOCHHITS_H__
#define __RECOCHHITS_H__

#include <vector>
#include <string>

#include "ElecSim.h"
#include "AlgoMultiHit.h"
#include "AlgoTDeconHit.h"
#include "RawChDataProcess.h"

// class to hold info on hit reco
class HitReconConfig
{
 public:
  ~HitReconConfig();
  
  static HitReconConfig* Instance();
  static void DestroyInstance();
  
  // delete the memory for this object yourself
  AlgoHitFinder* CreateHitAlgo();

  void SetElecModule(std::string name);
  
  // parameters for ROI search
  float ROI_adcTh;
  float ROI_nSigTh;
  float ROI_nSigLow1;
  float ROI_nSigLow2;
  UInt_t ROI_padLeft;
  UInt_t ROI_padRight;

  // general parameters
  std::string hitAlgo;
  BasicDigitizer *elecMod;
  float preampCalib;
  float adcCalib;
  float dT;         
  std::vector<float> eResp;
  std::vector<float> eRespADC;

  //paramerers for multi hit
  float MultiHit_relTh1;
  float MultiHit_relTh2;
  float MultiHit_absTh1;
  float MultiHit_absTh2;
  float MultiHit_aboveT; 
  UInt_t MultiHit_padLeft;
  UInt_t MultiHit_padRight;

  //parameter for tdecon
  float TDecon_hitTh;      //fC

 private:
  HitReconConfig();

   std::string ElecModName;
   std::string MultiHitName; 
   std::string TDeconName;
  
  static HitReconConfig* pInstance;
};


#endif
