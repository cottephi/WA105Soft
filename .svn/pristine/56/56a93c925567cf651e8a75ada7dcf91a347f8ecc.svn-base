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

#include <iostream>

#include "DBScanCluster.h"

using namespace std;

DBScanCluster::DBScanCluster(float epsx, float epsy, int minpts, int minclus)
{
  fEpsx = epsx;       // range in x
  fEpsy = epsy;       // range in y
  fMinPts  = minpts;  // min number of points to seed cluster
  fMinClus = minclus; // min number of points after clustering
}


DBScanCluster::~DBScanCluster()
{;}


void DBScanCluster::BuildClusters(const HitCollection* hcoll, std::vector<HitCollection*> &hitclus)
{
  //fClusters.clear();
  fData.clear();
  isVisited.clear();

  hcoll->GetHitList(fData);
  
  //cout<<"Number of hits to cluster "<<fData.size()<<endl;
  
  //
  isVisited.resize( fData.size() );
  
  std::vector<int> hitList;
  for(int i=0;i<(int)fData.size();i++)
    {
      if(isVisited[i] != 0) 
	continue;
      
      isVisited[i] = 1;
      
      RegionQuery( i, hitList );
      if( (int)hitList.size() < fMinPts )
	{
	  isVisited[i] = -1;
	}
      else
	{
	  std::vector<int> hitCluster;
	  ExpandCluster(hitList, hitCluster);
	  
	  // is our cluster large enough?
	  if(hitCluster.size() <= (size_t)fMinClus ) continue;
	  
	  //new hit collection with clustered hits
	  hitclus.push_back(new HitCollection());
	  HitCollection *aClus = hitclus.back();
	  for(size_t p=0;p<hitCluster.size();p++)
	    {
	      aClus->AddHit( fData[hitCluster[p]] );
	    }
	  
	}
    }

  //cout<<"Found "<<hitclus.size()<<" clusters"<<endl;
}

//
//
//
void DBScanCluster::RegionQuery(int idx, std::vector<int> &hitList)
{
  hitList.clear();

  const LArReco::QHit* hitA = fData[idx];
  const LArReco::QHit* hitB = NULL;

  Float_t x0 = 1.5*hitA->GetCh();
  Float_t y0 = 0.5*( hitA->GetTbStart() + hitA->GetTbEnd());

  Float_t epsx = fEpsx;
  Float_t epsy = fEpsy + 0.5*( hitA->GetTbEnd() - hitA->GetTbStart()); 
  
  // square it
  //espx *= espx;
  //espy *= epsy;

  for(size_t i=0;i<fData.size();i++)
    {
      if((int)i == idx)
	{
	  hitList.push_back(i);
	  continue;
	}

      hitB = fData[i];

      Float_t x = 1.5*hitB->GetCh(); 
      Float_t y = 0.5*( hitB->GetTbStart() + hitB->GetTbEnd());
      
      //if( (x - x0)*(x - x0)/espx + (y - y0) * (y - y0)/epsy <= 1) //elliptical border
      
      if( TMath::Abs(x-x0)<=epsx && TMath::Abs(y-y0)<=epsy ) //rectangular border
	{
	  hitList.push_back(i);
	}
	  
    }
  //cout << " Found "<<hitList.size()<<" hits in the vicinity of "<<idx<<endl;
}

//
//
//
void DBScanCluster::ExpandCluster(std::vector<int> &hitList, std::vector<int> &hitCluster)
{
  hitCluster.clear(); //new cluster!

  std::vector<int>::iterator it;

  //cout <<" size of hit list "<<hitList.size()<<endl;
  //for(size_t i=0;i<hitList.size();i++)
  //cout <<" orignal list "<<i<<" "<<hitList[i]<<endl;

  for(size_t i=0;i<hitList.size();i++)
    {
      int idx = hitList[i];
      if(isVisited[idx] == 0) 
	{
	  isVisited[idx] = 1;
	  std::vector<int> hitList2;
	  RegionQuery( idx, hitList2 );
	  
	  if((int)hitList2.size() >= fMinPts )
	    {
	      hitList.insert(hitList.end(), hitList2.begin(), hitList2.end());
	    }
	}
      else if(isVisited[idx] == 1 || isVisited[idx] == -1) //if not a member of cluster yet
	{
	  isVisited[idx] = 2; // make a cluster member
	  hitCluster.push_back(idx);
	}

    }
  
}
