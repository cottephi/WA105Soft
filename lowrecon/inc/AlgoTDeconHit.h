//////////////////////////////////////////////////////////////////////////
//
//  Hit finder based on deconvolution in time using NNLS fitter
//  
//  Created: Fri Jul  3 17:12:54 CEST 2015
//
//////////////////////////////////////////////////////////////////////////

#ifndef __ALGOTDECONHIT_H__
#define __ALGOTDECONHIT_H__

#include <algorithm>
#include <utility>

#include <TH1F.h>

#include "NNLS.h"
#include "AlgoHitFinder.h"

#define QZERO 0.01 
//#define MAXTDECONSAMPLES 100

class AlgoTDeconHit : public AlgoHitFinder
{
 public:
  AlgoTDeconHit(std::vector<float> &eresp, float hitth = 1.0);
  ~AlgoTDeconHit();

  // find hits ( time bin(us), start time, data, ped, pedrms, sub pedestal, append to list )
  void FindHits( float dt, UInt_t tstart, std::vector<float> &datafrag, 
		 float ped, float pedrms, bool subped = false );
  
  // save histograms to file
  void SaveHists(const char *hbasename, TDirectory *dirout);

 private:
  void Algorithm();
  void FinalizeHits();
  void SubDivide(std::vector< std::pair<size_t, size_t> > &subsamples);
  void MakeHisto( TH1F* &h, UInt_t tstart, const char *hname, std::vector<float> &data );
  
  NNLS  *nnlsSolver;        // NNLS solver
  float  hitTh;             // hit thershold in fC 
  std::vector<float> Qvals; // deconvolution results
};

#endif
