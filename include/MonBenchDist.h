/////////////////////////////////////////////////////////////////////////////////////////
//
//   Monitoring task for the reconstructed benchmark distributions
//  
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////

#ifndef __MONBENCHDIST_H__
#define __MONBENCHDIST_H__

#include <TH1F.h>

#include "MonTask.h"

class MonBenchDist : public MonTask
{
 public:
  MonBenchDist();
  ~MonBenchDist();
  
 private:
  void DoFileRead(std::string &fname);
  void DoDraw(size_t idx, TCanvas *cnvs);

  // for debug
  void DoDummyData(){ return; }
    
  TH1F* fHitsChView0;
  TH1F* fHitsChView1;
  TH1F* fHitsChQView0;
  TH1F* fHitsChQView1;
  
  TH1F* fTrkStartView0;
  TH1F* fTrkStartView1;
  TH1F* fTrkEndView0;
  TH1F* fTrkEndView1;
  
  void Clear();

  bool Copy( TH1F* from, TH1F* &to, 
	     const char *name, const char *xtitle );
  
  ClassDef(MonBenchDist,0);
};



#endif
