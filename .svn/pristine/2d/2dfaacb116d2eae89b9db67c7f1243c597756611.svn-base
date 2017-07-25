#include <iostream>
#include <algorithm>
#include <assert.h>

#include "HitCollection.h"

//
// ctor
//
ChHitCollection::ChHitCollection()
{
  Reset();
}

//
// dtor
//
ChHitCollection::~ChHitCollection()
{;}

//
// Reset
//
void ChHitCollection::Reset()
{
  fCrmId  = -1;
  fView   = -1;
  fChId   = -1;
  fQtot   = 0.0;
  fQtotMC = 0.0;
  
  fHits.clear(); //don't own hits so no memory delete
  fHitsMap.clear();
}

//
// add hit to the collection
//
bool ChHitCollection::AddHit(const LArReco::QHit *hit)
{

  // no hits have been added
  if(fChId == -1)
    {
      //std::cout<<hit<<std::endl;
      fCrmId = hit->GetCRM();
      fView  = hit->GetView();
      fChId  = hit->GetCh();
      if( !fHits.empty() )
	{
	  std::cout<<__FILE__<<", "<<__LINE__<<": looks like a potential bug"<<std::endl;
	  fHits.clear();
	}
      fQtot   += hit->GetQ();
      fQtotMC += hit->GetQTotMC();
      fHits.push_back(hit);
      fHitsMap[hit->GetTbStart()] = hit;
      return true;
    }
  
  if(!ChMatch(hit)) return false;

  // already have some hits  
  fQtot += hit->GetQ();
  fQtotMC += hit->GetQTotMC();

  // always add hits according to later time
  if(fHits.back()->GetTbStart() < hit->GetTbStart())
    {
      fHits.push_back( hit );
      fHitsMap[hit->GetTbStart()] = hit;
    }
  else
    {
      
      // insert into specific location (slow so try to avoid if possible)
      fHits.insert(std::lower_bound(fHits.begin(), fHits.end(), hit, 
				    ChHitCollection::ChCompPredicate), hit);
    }
  
  fHitsMap[hit->GetTbStart()] = hit;
  return true;
}

//
// add hit collection vector
//
void ChHitCollection::AddHitVector(const std::vector<const LArReco::QHit*> &hits)
{
  for(size_t i=0;i<hits.size();i++)
    {
      AddHit(hits[i]);
    }
}

//
// Get Ch and time tick for hits
//
void ChHitCollection::GetChTbin(std::vector< std::pair<int, int> >&coord, bool mknew)
{
  if(mknew) coord.clear();

  for(std::vector<const LArReco::QHit*>::iterator it=fHits.begin();
      it!=fHits.end();it++)
    {
      coord.push_back( std::make_pair(fChId, (*it)->GetTbStart()) );
    }

}

//
// Get time and position for hits
//
void ChHitCollection::GetPosTime(std::vector< std::pair<double, double> > &coord, bool mknew)
{
  if(mknew) coord.clear();

  for(std::vector<const LArReco::QHit*>::iterator it=fHits.begin();
      it!=fHits.end();it++)
    {
      coord.push_back( std::make_pair( (*it)->GetX(), (*it)->GetT() ) );
    }
}

//
// Get hit for a given time bin (use GetClosestHit if time bin is not known)
//
const LArReco::QHit* ChHitCollection::GetHit(int tbin)
{
  fHitsMapIt = fHitsMap.find(tbin);
  if(fHitsMapIt == fHitsMap.end())
    return NULL;
  
  return fHitsMapIt->second;  
}

//
// Get closest hit for a given  clock tick
// the difference in time ticks is returned by reference 
//
const LArReco::QHit* ChHitCollection::GetClosestHit(double ctick, double &dtick)
{
  if(fHits.empty()) return NULL;

  // check the first hit
  if( ctick <= (double)fHits[0]->GetTbEnd() )
    {
      dtick = ctick - 0.5*(fHits[0]->GetTbStart() + fHits[0]->GetTbEnd());
      return fHits[0];
    }
  
  if( ctick >= (double)fHits.back()->GetTbStart())
    {
      dtick = ctick - 0.5*(fHits.back()->GetTbStart() + fHits.back()->GetTbEnd());
      return fHits.back();
    }
  
  // make search for other cases
  int imin, imax;
  imin = 0;
  imax = fHits.size()-1;

  // binary search
  while( imin != imax )
    {
      int imid = (int)floor( 0.5*(imin + imax) );
      
      if((double)fHits[imid]->GetTbStart() <= ctick)
	imin = imid+1;
      else 
	imax = imid;
    }
  
  // the search returns next greatest element so decrement
  int idx = imax - 1;

  // find the closest hit to the start edge
  double tmp1 = fabs( (double)fHits[imax-1]->GetTbStart() - ctick );
  double tmp2 = fabs( (double)fHits[imax]->GetTbStart() - ctick );

  if(tmp2 < tmp1 ) idx += 1;
  
  dtick = ctick - 0.5*(fHits[idx]->GetTbStart() + fHits[idx]->GetTbEnd());
  return fHits[idx];  
}

//
// check if the hit is in the collection
//
bool ChHitCollection::IsInCollection(const LArReco::QHit* hit)
{
  if(!ChMatch(hit)) return false;

  for(size_t i=0;i<fHits.size();i++)
    {
      // this hit is in the collection already?
      if(hit == fHits[i]) //check memory addresses
	return true;
    } 

  return false;
}

//
//
//
void ChHitCollection::Print()
{
  std::cout<<std::endl<<"Hits in CRM "<<fCrmId<<" View "<<fView<<" Ch "<<fChId<<" tot "
	   <<GetNhits()<<std::endl;
  for(size_t i=0;i<fHits.size();i++)
    fHits[i]->Print();
}


//
//
//
void ChHitCollection::GetTMaxWindow(double &tlow, double &thigh, bool tus)
{
  tlow = thigh = -1;
  if(fHits.empty()) return;
  
  if(tus) //return hit time window in us
    {
      tlow = fHits[0]->GetT();
      thigh = fHits.back()->GetT() + fHits.back()->GetT()*((float)(fHits.back()->GetTbEnd())/(float)(fHits.back()->GetTbStart()));
    }
  else // return hit time window in ticks
    {
      tlow = (double)fHits[0]->GetTbStart();
      thigh = (double)fHits.back()->GetTbEnd();
    }
}

//
//
//
void ChHitCollection::GetTWindow(double &tlow, double &thigh, bool tus)
{
  tlow = thigh = -1;
  if(fHits.empty()) return;
  
  if(tus) //return hit time window in us
    {
      tlow = fHits[0]->GetT();
      thigh = fHits.back()->GetT();
    }
  else // return hit time window in ticks
    {
      tlow = (double)fHits[0]->GetTbStart();
      thigh = (double)fHits.back()->GetTbStart();
    }
}

//
// find max distance in tdc counts between hits on this channel
//
int ChHitCollection::GetMaxTdcDiff()
{
  if(fHits.size() == 1) return 0;
  
  // find max time distance between hits on this channel
  int tdcmax = 0;
  for(size_t i=1;i<fHits.size();i++)
    {
      int dist = fHits[i]->GetTbStart() - fHits[i-1]->GetTbEnd();
      if(dist > tdcmax) tdcmax = dist;
    }

  return tdcmax;
}


/////////////////////////////////////////////////////////
//
// Hit collection for a continuous block of channels
// Note: could have several view / CRMs in the same 
// collection, provided that ch number is unique
// The list is maintained in the increased ch order
//
HitCollection::HitCollection()
{
  fNch     = 0;
  fChStart = -1;
  fChEnd   = -1;
  fNhits   = 0;
}

//
// dtor
//
HitCollection::~HitCollection()
{
  Reset();
}


//
// Reset
//
void HitCollection::Reset()
{
  //for(int i=0;i<fHits.size();i++)
  //fHits[i].Reset();
  //fHits.clear();

  // clean up
  while(!fHits.empty())
    {
      fHits.back()->Reset();
      delete fHits.back();
      fHits.pop_back();
    }
  
  fHitChCoord.clear();

  fNch     = 0;
  fChStart = -1;
  fChEnd   = -1;
  fNhits   = 0;
}


//
// Add a hit
//
void HitCollection::AddHit(const LArReco::QHit* hit)
{
  int ch = hit->GetCh();
  
  ChHitCollection* myChHits = GetChHitCollection(ch);
  if(myChHits)
    { 
      if(myChHits->AddHit(hit)) // this will check whether we have the right CRM, view, ch etc.
	{
	  fNhits++;
	  fHitChCoord.push_back( std::make_pair(ch, hit->GetTbStart()) );
	}
    }
}


//
// we always want to have a continuos block of 
// channels even if there are no hits (should be rare)
//
ChHitCollection* HitCollection::GetChHitCollection(int ch)
{
  // ch number should not be negative
  assert(ch>=0);

  if(fHits.empty())
    {
      //fCrmId = hit->GetCRM();
      //fView  = hit->GetView();

      fHits.push_back(new ChHitCollection());
      fChStart = fChEnd = ch;
      fNch = 1;
    }

  //if(!ChMatch(hit)) return;
  

  // add to front 
  if(ch < fChStart)
    {
      int N2add = fChStart - ch;
      int Ntot  = fNch + N2add;
      fChStart  = ch;
      while(fNch < Ntot)
	{
	  fHits.push_front(new ChHitCollection());
	  fNch++;
	}
    }
  // push back
  else if(ch > fChEnd) 
    {
      int N2add = ch - fChEnd;
      int Ntot  = fNch + N2add;
      fChEnd = fChEnd + N2add;
      while( fNch <  Ntot)
	{
	  fHits.push_back(new ChHitCollection());
	  fNch++;
	}
    }

  return fHits[ch-fChStart];  
}


//
//
//
void HitCollection::SetHitVector(std::vector<const LArReco::QHit*> &hits)
{
  Reset();
  
  // add hits to the collection
  for(size_t i=0;i<hits.size();i++)
    {
      AddHit(hits[i]);
    }
}


//
//
//
void HitCollection::Print()
{
  for(size_t i=0;i<fHits.size();i++) 
    fHits[i]->Print();
}

//
//
//
void HitCollection::GetTMaxWindow(double &tlow, double &thigh, bool tus)
{
  double tmax = -1000;
  double tmin =  1000;
  for(size_t i=0;i<fHits.size();i++)
    {
      fHits[i]->GetTMaxWindow(tlow, thigh, tus);
      if(tlow != -1)
	if(tmin > tlow) tmin = tlow;
      if(thigh != -1)
	if(tmax < thigh) tmax = thigh;
    }

  tlow  = tmin;
  thigh = tmax;
  if(tmin > tmax)
    {
      tlow  = tmax;
      thigh = tmin;
    }
}

//
//
//
void HitCollection::GetHitList(std::vector<const LArReco::QHit*>& hitlist, int fromch) const
{
  if(fromch == -1) fromch = fChStart;    // get crm & view from first channel
  else if(fromch == -2) fromch = fChEnd; // get crm & view from last channel
  else if(fromch<fChStart || fromch>fChEnd) fromch = fChStart;
  
  int crm  = fHits[fromch - fChStart]->GetCRM();
  int view = fHits[fromch - fChStart]->GetView();

  //hitlist.clear();
  
  for(size_t i=0;i<fHits.size();i++)
    {
      if(fHits[i]->GetCRM() != crm) continue;
      if(fHits[i]->GetView() != view) continue;
      
      const std::vector<const LArReco::QHit*> chhits = fHits[i]->GetHits();
      for(size_t j=0;j<chhits.size();j++)
	{
	  const LArReco::QHit* hit = chhits[j];
	  hitlist.push_back( hit );
	}
    }
}


//
//
//
const ChHitCollection* HitCollection::GetChHits(int ch) const
{
  if(ch>=fChStart && ch<=fChEnd)
    if(fHits[ch - fChStart]->GetNhits() > 0)
      return fHits[ch - fChStart];

  return NULL;
}

//
// 
//
const ChHitCollection* HitCollection::GetChHitsFromChCoord(int idx) const
{
  if(idx >=0 && idx<(int)fHitChCoord.size())
    if(fHits[ fHitChCoord[idx].first - fChStart ]->GetNhits()>0)
      return fHits[ fHitChCoord[idx].first - fChStart ];

  return NULL;
}


//////////////////////////////////////////////////////////////////
//
// Graph hit collection
//
//
GraphHits::GraphHits()
{
  fHistAxis = NULL;
}


// dtor
GraphHits::~GraphHits()
{
  Clear();
  if(fHistAxis) fHistAxis->Delete();
}
  

//
//
//
void GraphHits::Clear()
{
  while(!fGraphs.empty())
    {
      fGraphs.back()->Delete();
      fGraphs.pop_back();
    }
}


//
//
//
void GraphHits::SetRange(float xmin, float xmax, float ymin, float ymax)
{
  std::string hname = "dummy_axishisto"; 
  if(fHistAxis) 
    {
      hname = fHistAxis->GetName();
      fHistAxis->Delete();
    }
  
  fHistAxis = new TH2F(hname.c_str(), hname.c_str(), 500, xmin, xmax,
		       1000, ymin, ymax);
  fHistAxis->SetDirectory(0);
  //fHistAxis->SetStats(0);
  //fHistAxis->SetTitle(0);
  fHistAxis->GetXaxis()->SetTitle("ch");
  fHistAxis->GetYaxis()->SetTitle("tick");
  //fHistAxis->SetMinimum(ymin);
  //fHistAxis->SetMaximum(ymax);
  
}


//
//
//
void GraphHits::BuildBoxPrimitive(const LArReco::QHit *ahit, TGraph* &hitgraph)
{
  hitgraph = new TGraph();
  hitgraph->SetPoint(0, ahit->GetCh(), ahit->GetTbStart());
  hitgraph->SetPoint(1, ahit->GetCh()+1, ahit->GetTbStart());
  hitgraph->SetPoint(2, ahit->GetCh()+1, ahit->GetTbEnd());
  hitgraph->SetPoint(3, ahit->GetCh(), ahit->GetTbEnd());
  hitgraph->SetPoint(4, ahit->GetCh(), ahit->GetTbStart());
}

//
// Draw hit collection
//
void GraphHits::Draw(HitCollection &hitcoll, TPad *pad, int colour, std::string opt)
{
  //Clear();
  size_t spos = fGraphs.size();
  TGraph *gr;
  const std::deque<ChHitCollection*> hits = hitcoll.GetHits();
  for(size_t i=0;i<hits.size();i++)
    {
      const std::vector<const LArReco::QHit*> chhits = hits[i]->GetHits();
      for(size_t j=0;j<chhits.size();j++)
	{
	  const LArReco::QHit* ahit = chhits[j];
	  BuildBoxPrimitive( ahit, gr );
	  fGraphs.push_back( gr );
	  fGraphs.back()->SetLineColor(colour);
	}
    }
  
  if( opt.compare("same") == 0 || 
      opt.compare("SAME") == 0 )
    {
      pad->cd();
    }
  else
    {
      if(!fHistAxis)
	{
	  double tst, tend;
	  hitcoll.GetTMaxWindow(tst, tend);
	  SetRange( hitcoll.GetChStart(), hitcoll.GetChEnd()+1, tst, tend);
	}
      pad->cd();
      fHistAxis->Draw("AXIS");
    }

  for(size_t i=spos;i<fGraphs.size();i++)
    {
      fGraphs[i]->Draw("lsame");
    }  
}



