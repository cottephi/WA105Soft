////////////////////////////////////////////////////////////////////////////////////
//
//  Class for hit reconstuction with support for POSIX threads
//
//  Created: Tue Jul  7 11:56:23 CEST 2015
//
////////////////////////////////////////////////////////////////////////////////////

#ifndef __HITRECON_H__
#define __HITRECON_H__

#include <iostream>
#include <algorithm>
#include <vector>
#include <unistd.h>
#include <pthread.h>

#include "QHit.h"
#include "DataChan.h"
#include "HitReconConfig.h"

// hit recon for channel data
class HitRecon
{
 public:
  HitRecon();
  ~HitRecon();
  
  void SetOutputFile(TFile *fout){ m_fout = fout; }

  void FindHits( LArRawData::DataChan *chdata );
  std::vector<LArReco::QHit>& GetHits(){ return Hits; }
  
  
 private:
  TFile *m_fout;
  AlgoHitFinder *hitFinder;
  std::vector<LArReco::QHit> Hits;
};

#endif
