/////////////////////////////////////////////////////////////////////////////////
//
// Module to reconstruct 2D track segments mostly geared towards CR 
// track reconstruction in the dual-phase LAr TPC
// 
// Basic idea is to cluster hits using nearest neighbour algorithm 
// and then examine each cluster for presence of overlapping tracks
// In the simplest case if no overlapping tracks are detected
// the hits in the cluster split into main track hits and those beloning to 
// delta rays and then a 2D track segment is built
//
// For clusters with track overlaps track building is based on several methods
//
/////////////////////////////////////////////////////////////////////////////////


#ifndef __CRTRACKSEGRECON_H__
#define __CRTRACKSEGRECON_H__

#include <vector>

#include "RecoConfig.h"
#include "QHit.h"
#include "Track2d.h"
#include "HitCollection.h"

#include "NNCluster.h"
#include "TrackBuilderMTC.h"
#include "TrackBuilderSm.h"


class CRTrackSegRecon
{
 public:
  CRTrackSegRecon(RecoConfig &recopara);
  ~CRTrackSegRecon();
  
  int FindTracks(const HitCollection* hcoll, std::vector<LArReco::Track2d> &tracks);
		   
 private:
  
  // clustering algorithm
  NNCluster *fClusAlgo; 

  // multi track clustering
  TrackBuilderMTC *fTrkMTC;

  // track builder
  TrackBuilderSm *fTrkBuild;
};

#endif
