//////////////////////////////////////////////////////////////////////
//
// Hit finder based on MultiHit algorithm from Qscan
// see D. Lussi Diss.-No. ETH 21314    
// 
//////////////////////////////////////////////////////////////////////

#ifndef __ALGOMULTIHIT_H__
#define __ALGOMULTIHIT_H__

#include "AlgoHitFinder.h"

class AlgoMultiHit : public AlgoHitFinder
{
 public:
  AlgoMultiHit( float calibc, float relth1, float relth2, 
		float absth1, float absth2, float abovetbin,
		UInt_t padleft = 10, UInt_t padright = 30);
  ~AlgoMultiHit();
  
  // find hits ( time bin(us), start time, data, ped, pedrms, sub pedestal)
  void FindHits( float dt, UInt_t tstart, std::vector<float> &datafrag, 
		 float ped, float pedrms, bool subped = false );
  
 private:
  void Algorithm();
  void FinalizeHits(); // finalize hits
  void ComputeSignalIntegral(LArReco::QHit *hit, UInt_t tbins, UInt_t tbine);
  // to be implemented to get more accurate charge from overalapping hits
  void FitHits(){;}    
  
  float calibC;    // calibration constant for ADC x us -> fC
  float relTh1;    // relative threshold 1
  float relTh2;    // relative threshold 2
  float absTh1;    // absolute threshold 1
  float absTh2;    // absolute threshold 2
  float aboveTb;   // number of bins above threshold

  // add bins to integral to avoid bias due to threshold
  UInt_t padLeft;  // bins to pad on the left
  UInt_t padRight; // bins to pad on the right 
};

#endif
