//////////////////////////////////////////////////////////////////////////
//
//  Abstract base class for hit finding algorithms
//  
//  Created: Fri Jul  3 14:34:53 CEST 2015
//
//////////////////////////////////////////////////////////////////////////

#ifndef __ALGOHITFINER_H__
#define __ALGOHITFINER_H__

#include <vector>

#include <TFile.h>
#include <TH1F.h>

// hit class container
#include "QHit.h"

class AlgoHitFinder
{
 public:
  AlgoHitFinder();
  virtual ~AlgoHitFinder(); 
  
  // to be derived 
  virtual void FindHits( float dt, UInt_t tstart, std::vector<float> &datafrag, 
			 float ped, float pedrms, bool subped = false ) = 0;

  // save any histograms
  virtual void SaveHists(const char *hbasename, TDirectory *dirout);

  size_t GetNhits(){ return Hits.size(); }
  std::string GetName(){ return AlgoName; }

  // get the hits vector
  const std::vector<LArReco::QHit*>& GetHits() const
    {
      return Hits;
    }

  void Print();
  void ClearHits();

 protected:
  void SubPedestal();
  std::string AlgoName;
  std::vector<LArReco::QHit*> Hits; // hits that have been found
  std::vector<float> Data;          // data for processing
  UInt_t tStart;                    // starting time bin of data fragment
  float Ped;                        // pedestal
  float PedRms;                     // pedestal RMS
  float dT;                         // time bin in us
};

#endif 
