////////////////////////////////////////////////////////////////////////////////////////////
//
//  Hit reconstructio task
//  Add here you algorithms (see InitHitFinder function)
//  
//  Created: Fri Apr 29 14:30:22 CEST 2016
//
//
////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __HITRECOTASK_H__
#define __HITRECOTASK_H__

#include <vector>
#include <string>

#include "RecoTask.h"

#include "DLArCRP.h"
#include "QHit.h"
#include "ElecSim.h"
#include "DataChan.h"

// hit reconstruction algorithms
#include "AlgoMultiHit.h"
#include "AlgoTDeconHit.h"
#include "RawChDataProcess.h"

// channel pedestal
#include "ChPedestal.h"

class HitRecoTask : public RecoTask
{
 public:
  HitRecoTask();
  ~HitRecoTask();
  
  // basic configurations
  void Configure(RunConfig &runpara, RecoConfig &recopara);
  
  // load calibration data if any
  void LoadCalibrations(std::string &path){;}

  // process event
  void ProcessEvent(Event &event);
  
  
 private:
  void InitElecModule(std::string& name);
  void InitHitFinder(std::string& name);
  
  void FindHitsView(LArRawData::DataCRM* crm, int iview,
		    std::vector<LArReco::QHit> &hits,
		    const GeomConfig *geo_config);
  void FindHitsCh( LArRawData::DataChan *chdata, 
		   std::vector<LArReco::QHit> &hits, 
		   float ped = 0, float pedrms = 0 );

  void ReadVoxelInfo(Event &event);
  void AssignHitTruth(std::vector<LArReco::QHit> &hits0,
		      std::vector<LArReco::QHit> &hits1);
  
  // configuration for this reco task
  //RecoConfig fRecConf;

  // process raw data to find ROIs
  RawChDataROI *fRoiFinder;

  // hit finder
  AlgoHitFinder *fHitFinder;

  // general parameters
  BasicDigitizer *fElecMod;
  float fPreampCalib;  // nominal calibration for pre-amp
  float fAdcCalib;     // nominal calibration for adc
  float fDt;         
  std::vector<float> fResp;
  std::vector<float> fRespADC;

  // parameters for ROI search
  float fROI_adcTh;
  float fROI_nSigTh;
  float fROI_nSigLow1;
  float fROI_nSigLow2;
  UInt_t fROI_padLeft;
  UInt_t fROI_padRight;
  
  // hit algo name
  std::string fHitAlgoName;
  
  //paramerers for multi hit
  float fMultiHit_relTh1;
  float fMultiHit_relTh2;
  float fMultiHit_absTh1;
  float fMultiHit_absTh2;
  float fMultiHit_aboveT; 
  UInt_t fMultiHit_padLeft;
  UInt_t fMultiHit_padRight;

  //parameter for tdecon
  float fTDecon_hitTh;      //fC
  
  // MC truth for the hit
  bool fHasTruth;

  // array for truth accociation
  float MC_HIT0 [4][960] [20] [7];  
  float MC_HIT1 [4][960] [20] [7]; 

  //
  float fVdrift;
  
  // CRP configuration
  //const GeomConfig *fGeoConfig;

  // calibration data
  calidaq::ChPedestal *fChPed;
  
};

#endif
