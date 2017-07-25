/////////////////////////////////////////////////////////////////////////////////
//
//  Nearest neighbour clustering useful for finding track segments
//     Need to specify:
//         1. min size of the cluster to return after clustering (minclus)
//
//  Created: Tue Apr 12 11:12:08 CEST 2016
//  Modified: 
//
/////////////////////////////////////////////////////////////////////////////////

#ifndef __NNCLUS_CLUSTER_H__
#define __NNCLUS_CLUSTER_H__

#include <vector>

#include "HitCollection.h"
#include "QHit.h"

class NNCluster
{
  
 public:
  NNCluster(int minclus);
  ~NNCluster();
  
  // build clusters
  void BuildClusters(const HitCollection* hcoll, std::vector<HitCollection*> &hitclus);
  
  void SetEpsx(float val){ fEpsx = val; }
  void SetEpsy(float val){ fEpsy = val; }
  void SetMinPts(int val){ fMinPts = val; }
  
  
 private:
  void ExpandCluster(std::vector<int> &hitList, 
		     std::vector<int> &hitCluster);
  void RegionQuery(int idx, std::vector<int> &hitList);
  
  //
  std::vector< const LArReco::QHit* > fData;    // hit data
  std::vector<int> isVisited; 


  // define search region
  float fEpsx;    // search region in X
  float fEpsy;    // search region in Y
  int   fMinPts;  // min number of points in search region <-- seeds cluster
  int   fMinClus; // min number of points in cluster
};

#endif
