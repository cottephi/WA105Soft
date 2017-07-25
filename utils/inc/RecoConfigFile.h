/////////////////////////////////////////////////////////////////////////////////////////
//
//  Read reco configuration file
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////
#ifndef __RECOCONFIGFILE_H__
#define __RECOCONFIGFILE_H__

#include <vector>
#include <string>
#include <sstream>

#include "RecoConfig.h"

//
//
//
class RecoConfigFile
{
 public:
  RecoConfigFile();
  ~RecoConfigFile();
  
  void Read(std::string, std::vector<RecoConfig> &recmods);
  
  bool EventsSelected(Long64_t &start, Long64_t &nevents);

 private:
  bool ReadLine(std::istringstream &line, std::string &keyword);
  
  std::vector<RecoConfig> fRecMods;

  int fRecoAlgCtr;

  Long64_t fStartEv, fTotalEv;

  std::string fReco;
  std::string fRecoAlg;

  std::string fKeyEvents;
  std::string fKeyRecoBegin;
  std::string fKeyRecoEnd;
  std::string fKeyAlgoBegin;
  std::string fKeyAlgoEnd;
};

#endif
