///////////////////////////////////////////////////////////////////////////
//
//
//
//  Utility to store track tracjectories while build track objects
//
//
///////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <cmath>

#include "Track2DTrajectory.h"


using namespace std;

//
// ctor
//
Track2DTrajectory::Track2DTrajectory() : fPBeta(1.0), fColor(1)
{;}


//
// dtor
//
Track2DTrajectory::~Track2DTrajectory()
{;}


//
// get coords of the last np points
//
void Track2DTrajectory::GetCoordLast( size_t np, std::vector<double> &xcoord, 
				    std::vector<double> &ycoord )
{
  xcoord.clear(); 
  ycoord.clear();
  if(Points.empty()) return;
  
  if(np > Points.size()) np = Points.size();
  
  std::deque<Track2DPoint>::iterator it;
  for(it = (Points.end() - np);it!=Points.end();it++)
    {
      xcoord.push_back( (*it).GetX() );
      ycoord.push_back( (*it).GetY() );
    }
}

//
//  get coords of the first np points
//
void Track2DTrajectory::GetCoordFirst(size_t np, std::vector<double> &xcoord, 
				    std::vector<double> &ycoord)
{
  xcoord.clear(); 
  ycoord.clear();
  if(Points.empty()) return;
  
  if(np > Points.size()) np = Points.size();
  
  std::deque<Track2DPoint>::iterator it;
  for(it = Points.begin();it!=(Points.begin()+np);it++)
    {
      xcoord.push_back( (*it).GetX() );
      ycoord.push_back( (*it).GetY() );
    }
}


//
// trajectory length
//
double Track2DTrajectory::Length() const
{
  if( Points.size() < 1 ) return 0;
  if( IsVertical() ) return Points[0].GetYRange();
  
  double x0 = Points.front().GetX();
  double y0 = Points.front().GetY();

  double x1 = Points.back().GetX();
  double y1 = Points.back().GetY();
 
  return TMath::Sqrt( (x1-x0)*(x1-x0) + (y1-y0)*(y1-y0) );
}


//
//
//
void Track2DTrajectory::AddPoint( Track2DPoint &pnt )
{
  if(Points.empty() ) 
    {
      Points.push_back( pnt );
      
      fChi2    = 999;

      return;
    }
  
  int fFirstCh = GetFirstCh();
  int fLastCh  = GetLastCh();
      
  //
  int ch = pnt.GetCh();
  if( ch > fLastCh )
    {
      AddPointToEnd( pnt );
      return;
    }
  else if(ch == fLastCh)
    {
      Points.back() += pnt;
      return;
    }
  else if( ch < fFirstCh )
    {
      AddPointToFront( pnt );
      return;
    }
  else if( ch == fFirstCh )
    {
      Points.front() += pnt;
      return;
    }
  
  //
  std::deque<Track2DPoint>::iterator it;
  it = std::lower_bound( Points.begin(), Points.end(), pnt, 
			 Track2DTrajectory::ChCompPredicate );
  if( (*it).GetCh() > pnt.GetCh() )
    Points.insert( it, pnt );
  else     
    {
      //cout<<(*it).GetCh()<<" "<< pnt.GetCh() <<" "<<fFirstCh<<" "<<fLastCh<<endl;
      *it += pnt;
    }
}

//
//
//
void Track2DTrajectory::AddPointToEnd( Track2DPoint &pnt )
{
  //fLastCh = pnt.GetCh();
  Points.push_back( pnt );
}

//
//
//
void Track2DTrajectory::AddPointToFront( Track2DPoint &pnt )
{
  //fFirstCh = pnt.GetCh();  
  Points.push_front( pnt );
}

//
//
//
void Track2DTrajectory::RemoveFirstPoint()
{
  Points.pop_front();
  //fFirstCh = Points.front().GetCh();
}

//
//
//
void Track2DTrajectory::RemoveLastPoint()
{
  Points.pop_back();
  //fLastCh = Points.back().GetCh();
}


//
//
//
void Track2DTrajectory::Print()
{
  
  cout<<"Trajectory length  "<<Length()<<endl;
  cout<<"Trajectory points  "<<Points.size()<<endl;

  for(size_t i=0;i<Points.size();i++)
    {
      Points[i].Print();
    }
}


//
// distance from a  given point to trajectory
//
double Track2DTrajectory::Distance(Track2DPoint &pnt)
{
  //
  int ch = pnt.GetCh();
  
  if( ch <= GetFirstCh() )
    return Points.front().Distance( pnt );
  else if( ch >= GetLastCh() ) 
    return Points.back().Distance( pnt );
  
  if( IsVertical() ) 
    return Points[0].DistanceToClosestHit( pnt );

  // do binary search for the closest point
  size_t lo = 0;
  size_t hi = Points.size()-1;
  size_t idx;
  while( lo <= hi )
    {
      idx = (lo + hi)/2;
      int pnt_ch = Points[idx].GetCh();
      if( pnt_ch == ch ) 
	break;
      else if( pnt_ch > ch )
	{
	  hi = idx - 1;
	  if( Points[hi].GetCh() < ch ) 
	    {
	      idx = hi;
	      break;
	    }
	}
      else if( pnt_ch < ch )
	{
	  lo = idx + 1;
	  if( Points[lo].GetCh() > ch ) 
	    {
	      //idx = idx;
	      break;
	    }

	}
    }
  
  // line parameters in the vicinity
  double mp = Points[idx].GetPredTan();
  double yp = Points[idx].GetY() + mp*(pnt.GetX() - Points[idx].GetX());
  
  // distance to the line 
  double rval = TMath::Abs(pnt.GetY() - yp)/TMath::Sqrt(1+mp*mp);

  return rval;
}

//
// a function to construct a path representation of the track and get all 
// the hits associated with it
//
void Track2DTrajectory::GetTrjPoints( std::vector< const LArReco::QHit* > &trkhits,
				      std::vector<TVector2> &trkpath, 
				      std::vector<double> &dqdx)
  
{
  trkhits.clear();
  trkpath.clear();
  dqdx.clear();
  
  for( size_t i=0;i<Points.size();i++)
    {
      double x = Points[i].GetX();
      double y = Points[i].GetPredPos();
      double q = Points[i].GetQ();
      const std::vector<const LArReco::QHit*> hits = Points[i].GetHits();
      trkhits.insert( trkhits.begin(), hits.begin(), hits.end() );
      dqdx.push_back( q );
      trkpath.push_back( TVector2( x, y ) );
    }
}


//
//
//
void Track2DTrajectory::UpdateStats()
{
  // calculate chi2 for the track
  fChi2 = 0;
  
  for(size_t i=0;i<Points.size();i++)
    {
      fChi2 += Points[i].GetChi2();
    }
  
  fChi2 /= Points.size();
}
