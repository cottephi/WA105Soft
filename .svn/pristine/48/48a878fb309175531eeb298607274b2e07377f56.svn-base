/////////////////////////////////////////////////////////////////////////////////
//
//  2D cluster of hits
//   Normally a base class for 2D track and 2D shower
//   Could also be used for delta rays     
//
/////////////////////////////////////////////////////////////////////////////////

#include <TProcessID.h>

#include "Cluster2d.h"

ClassImp(LArReco::Cluster2d)

//
//
//
LArReco::Cluster2d::Cluster2d()
{
  fThisObj = LArReco::RecoObjLink(LArReco::kCluster2d, -1);  
  //fHits = NULL;
  fT0   = 0;
  fTotQ = 0;
  fStart.Set(0.0, 0.0);
  fEnd.Set(0.0, 0.0);
  fViewType = kBadView;
}


//
//
//
LArReco::Cluster2d::~Cluster2d()
{
  //if(fHits) fHits->Delete();
}


//
//
//
LArReco::Cluster2d::Cluster2d(std::vector<const LArReco::QHit*> &hits)
{
  fThisObj = LArReco::RecoObjLink(LArReco::kCluster2d, -1);  

  fStart.Set(hits[0]->GetX(), hits[0]->GetD());
  fEnd.Set(hits.back()->GetX(), hits.back()->GetD());
  
  Init(hits);
}

//
//
//
LArReco::Cluster2d::Cluster2d(TVector2 &start, TVector2 &end,
			      std::vector<const LArReco::QHit*> &hits)
{
  fThisObj = LArReco::RecoObjLink(LArReco::kCluster2d, -1);  
  fStart = start;
  fEnd   = end;

  Init(hits);
}


//
//
//
void LArReco::Cluster2d::AddHit(const LArReco::QHit* hit)
{
  
  // link to this hit (not needed since hit ptrs are stored)
  LinkToLowerNode(hit->GetRecoObjLink());
}




//
//
//
void LArReco::Cluster2d::Init(std::vector<const LArReco::QHit*> &hits)
{
  fViewType = GetRecoObjView( hits[0]->GetView() );
  
  fSlope = (fEnd.Y() - fStart.Y())/(fEnd.X() - fStart.Y());
  
  //fHits = new TRefArray( TProcessID::GetProcessWithUID( hits[0] ) );
  fTotQ = 0;
  for(size_t i=0;i<hits.size();i++)
    {
      AddHit( hits[i] );
      
      //fHits->Add( const_cast<LArReco::QHit*>(hits[i]) );
      
      // add charge
      Double_t qval = hits[i]->GetQ();
      fTotQ += qval;
    }
}
