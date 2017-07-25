/////////////////////////////////////////////////////////////////////////////////
//
//  Clustering using DBSCAN algorithm
//     Need to specify:
//         1. a channel range to look for nearby hits (epsx)
//         2. tick range to look for nearby hits (epsy)
//         3. min number of hits to seed cluster (minpts)
//         4. min size of the cluster to return after clustering (minclus)
//
//  Created: Fri Aug 28 16:03:34 CEST 2015  
//  Modified: 
//
/////////////////////////////////////////////////////////////////////////////////

#ifndef __DBSCAN_CLUSTER_H__
#define __DBSCAN_CLUSTER_H__

#include <vector>

#include "HitCollection.h"
#include "QHit.h"

class DBScanCluster
{
  
 public:
  DBScanCluster(float epsx, float epsy, int minpts, int minclus);
  ~DBScanCluster();
  
  // build clusters
  void BuildClusters(const HitCollection* hcoll, std::vector<HitCollection*> &hitclus);
    
  
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
