#include <iostream>

#include "HitRecon.h"

// singleton instance
HitReconConfig* HitReconConfig::pInstance = NULL;

// get static instance
HitReconConfig* HitReconConfig::Instance()
{
  if(! pInstance ) pInstance = new HitReconConfig();
  return pInstance;
}

// destroy static instance
void HitReconConfig::DestroyInstance()
{
  if(pInstance)
    {
      delete pInstance;
      pInstance = NULL;
    }
}
 
// set default values
HitReconConfig::HitReconConfig()
{
 

 MultiHitName  = "AlgoMultiHit";
 TDeconName   = "AlgoTDeconHit";

}

HitReconConfig::~HitReconConfig()
{
  if(elecMod) delete elecMod;
  elecMod = NULL;
}

// define electronics module
void HitReconConfig::SetElecModule(std::string name)
{
  if(elecMod) delete elecMod;
  elecMod = NULL;
  
  preampCalib = 0;
  adcCalib    = 0;
  dT          = 0;
  eResp.clear();
  eRespADC.clear();

  if(name.compare("ETHZ") == 0)
    {
      elecMod = new DigitizerETHZ(); 
    }
  else
    {
      std::cerr<<"ERROR: Uknown elecsim module "<<name<<std::endl;
      return;
    }
  
  preampCalib = elecMod->GetCalibConst();
  adcCalib    = elecMod->GetmVtoADC();
  dT          = 1.0/elecMod->GetSampleFreq(); //in us
  
  elecMod->ComputeRespVector();
  elecMod->GetRespVector(eResp);

  for(size_t i=0;i<eResp.size();i++)
    eRespADC.push_back(eResp[i]/adcCalib);

}

// create hit algorithm
AlgoHitFinder* HitReconConfig::CreateHitAlgo()
{
  

  if( hitAlgo.compare(MultiHitName) == 0 )
    {
      return (new AlgoMultiHit(preampCalib/adcCalib, MultiHit_relTh1, MultiHit_relTh2, 
			       MultiHit_absTh1, MultiHit_absTh2, MultiHit_aboveT/dT,
			       MultiHit_padLeft, MultiHit_padRight));
    }
  else if ( hitAlgo.compare(TDeconName) == 0 )
    {
      return (new AlgoTDeconHit( eRespADC, TDecon_hitTh ));
    }

  
  std::cerr<<"ERROR: Uknown hit algorithm "<<hitAlgo<<std::endl;
  return NULL;  
}
