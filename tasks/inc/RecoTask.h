//////////////////////////////////////////////////////////////////////////////////
//
//  
//   Base class for event reconstruction tasks
//  
//  
//   Created: Tue Apr 26 17:58:56 CEST 2016
//
//
//////////////////////////////////////////////////////////////////////////////////

#ifndef __RECOTASK_H__
#define __RECOTASK_H__

#include <string>

#include "RecoConfig.h"
#include "EventDataManager.h"

class RecoTask
{
 public:
  RecoTask();
  virtual ~RecoTask();
  
  //void StartTimer();
  //void StopTimer();

  virtual void Configure(RunConfig& runpara, RecoConfig &recopara) = 0;
  // override 
  virtual void ProcessEvent(Event &event) = 0;

  // possible handle for load calibrations
  virtual void LoadCalibrations(std::string &path){;}
  
  RecoConfig& GetRecoConfig(){ return fRecConf; }
  
  std::vector<std::string>& GetDataProducts(){ return fDataProds; }
  
  
 protected:
  size_t fExecTime;
  std::vector<std::string> fDataProds;
  RecoConfig fRecConf;
  
};

#endif
