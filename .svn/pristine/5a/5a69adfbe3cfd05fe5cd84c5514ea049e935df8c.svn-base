////////////////////////////////////////////////////////////////////////////////////////////
//
//  CR track reconstruction task
//  
//  
//
//
////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CRTRACKRECOTASK_H__
#define __CRTRACKRECOTASK_H__


#include <vector>

#include "RecoTask.h"

#include "CRTrackSegRecon.h"
#include "TrackJoinCRM.h"

class CRTrackRecoTask : public RecoTask
{
 public:
  CRTrackRecoTask();
  ~CRTrackRecoTask();
  
  // basic configurations
  void Configure(RunConfig &runpara, RecoConfig &recopara);
  
  // load calibration data if any
  void LoadCalibrations(std::string &path){;}

  // process event
  void ProcessEvent(Event &event);
  
 private:
  void FindTracksView(const TObjArray *hits, std::vector<LArReco::Track2d> &tracks );
  
  // merge track segments in each module into global segments
  void MergeSegments( std::vector< std::vector<LArReco::Track2d> > &tracksegs, 
		      std::vector< LArReco::Track2d > &tracks );
  
  // track segment reco algo
  CRTrackSegRecon *fTrackRecAlgo;
  TrackJoinCRM    *fTrackJoiner;
  //int fNcrm;
};

#endif
