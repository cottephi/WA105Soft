/////////////////////////////////////////////////////////////////////////////////
//
//  Nearest neighbour clustering useful for finding track segments
//     Need to specify:
//         1. min size of the cluster to return after clustering (minclus)
//
//  Basically DBSCAN but with very tight constraints
//  Modified:    
//
/////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <algorithm>

#include "NNCluster.h"

using namespace std;

NNCluster::NNCluster(int minclus)
{
  fEpsx = 2;          // distance to next hit in ch counts 
  fEpsy = 2;          // distance to next hit in tdc
  fMinPts  = 2;       // min number of points to seed cluster
  fMinClus = minclus; // min number of points after clustering
}


NNCluster::~NNCluster()
{;}


void NNCluster::BuildClusters(const HitCollection* hcoll, std::vector<HitCollection*> &hitclus)
{
  //fClusters.clear();
  fData.clear();
  isVisited.clear();

  hcoll->GetHitList(fData);
  
  //cout<<"Number of hits to cluster "<<fData.size()<<" "<<fMinClus<<endl;
  
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
void NNCluster::RegionQuery(int idx, std::vector<int> &hitList)
{
  hitList.clear();

  const LArReco::QHit* hitA = fData[idx];
  const LArReco::QHit* hitB = NULL;

  Float_t x0   = hitA->GetCh();
  Float_t y0lo = hitA->GetTbStart();
  Float_t y0hi = hitA->GetTbEnd();


  for(size_t i=0;i<fData.size();i++)
    {
      if((int)i == idx)
	{
	  hitList.push_back(i);
	  continue;
	}

      hitB = fData[i];

      Float_t x    = hitB->GetCh(); 
      Float_t y1lo = hitB->GetTbStart();
      Float_t y1hi = hitB->GetTbEnd();

      if( TMath::Abs(x - x0) > fEpsx) continue;

      Float_t d1  = y0hi - y0lo;
      Float_t d2  = y1hi - y1lo;
      
      if( std::max( y0hi, y1hi ) - std::min( y0lo, y1lo ) > d1 + d2 + fEpsy ) continue;

      hitList.push_back(i);
    }
}

//
//
//
void NNCluster::ExpandCluster(std::vector<int> &hitList, std::vector<int> &hitCluster)
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
