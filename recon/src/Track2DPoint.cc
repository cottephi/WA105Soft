//////////////////////////////////////////////////////////////////////
//
//  Utility class to help build 2D tracks in views
//
//
//
//
//////////////////////////////////////////////////////////////////////

#include <iostream>
#include <iomanip>
#include <assert.h>

#include "Track2DPoint.h"

using namespace std;

//
// ctor
//
Track2DPoint::Track2DPoint( int uid, const LArReco::QHit* hit )
{
  
  fChHits.AddHit( hit );

  fQFrac = 1.0;

  fCh  = hit->GetCh();
  fX   = hit->GetX();
  fY   = hit->GetD();

  fChi2 = -999;
  fR    = -999;
  
  // a posteriori
  fS.resize(2, 0);
  fC.resize(3, 0);
  
  // a priori
  fSapri.resize(2, 0);
  fCapri.resize(3, 0);

  fUids.push_back( uid );
}

//
// dtor
//
Track2DPoint::~Track2DPoint()
{;}

//
//
//
void Track2DPoint::SetCovMatApri( const std::vector<double> &C )
{
  assert( C.size() >= fCapri.size() );
  fCapri[0] = C[0]; //Cmm
  fCapri[1] = C[1]; //Cbb
  fCapri[2] = C[2]; //Cmb 
}

//
//
//
void Track2DPoint::SetStateApri( const std::vector<double> &s )
{
  assert( s.size() >= fS.size() );
  fSapri[0] = s[0]; // m - slope
  fSapri[1] = s[1]; // b - intercept
}

//
//
//
void Track2DPoint::SetCovMat( const std::vector<double> &C )
{
  assert( C.size() >= fC.size() );
  fC[0] = C[0]; //Cmm
  fC[1] = C[1]; //Cbb
  fC[2] = C[2]; //Cmb 
}

//
//
//
void Track2DPoint::SetState( const std::vector<double> &s )
{
  assert( s.size() >= fS.size() );
  fS[0] = s[0]; // m - slope
  fS[1] = s[1]; // b - intercept
}

//
//
// calculate inverse of the 2x2 covariance matrix 
// which == 0 for a priori cov mat
// otherwise for a posteriori cov mat
//
int Track2DPoint::GetCovMatInv( std::vector<double> &Cinv, int which )
{
  if(Cinv.size()!=3) Cinv.resize(3);
  
  std::vector<double> C;
  
  if( which == 0 ) C = fCapri;
  else C = fC;

  

  double d = C[0]*C[1] - C[2]*C[2];

  if( d == 0) 
    {
      Cinv[0] = Cinv[1] = Cinv[2] = 0;
      return -1;
    }
  
  d = 1./d;

  Cinv[0] =  C[1] * d;
  Cinv[1] =  C[0] * d;
  Cinv[2] = -C[2] * d;
  return 0;
}

//
// Print
// 
void Track2DPoint::Print()
{
  cout<<setw(5)<<fCh
      <<setw(15)<<fX
      <<setw(15)<<fY
      <<setw(15)<<GetQ()
      <<setw(15)<<fS[1]
      <<setw(15)<<fS[0]
      <<setw(15)<<fC[0]
      <<setw(15)<<fC[1]
      <<setw(15)<<fC[2]
      <<setw(15)<<fR
      <<setw(15)<<fChi2
      <<endl;
}


//
//
//
Track2DPoint& Track2DPoint::operator+=(const Track2DPoint &pnt)
{
  if( pnt.GetCh() != fCh ) 
    {
      cerr<<"ERROR: critical error channel numbers should be equal"<<endl;
      return *this;
    }

  //
  fUids.insert( fUids.end(), pnt.fUids.begin(), pnt.fUids.end());
  AddHitVector( pnt.GetHits() );
  
  // calculate average value of Y
  fY = 0;
  const std::vector<const LArReco::QHit*> hits = GetHits();
  for(size_t i=0;i<hits.size();i++)
    {
      fY += hits[i]->GetD();
    }
  fY /= hits.size();

  return (*this);
}


//
//
//
double Track2DPoint::DistanceToClosestHit(Track2DPoint &pnt) const
{
  const std::vector<const LArReco::QHit*> hits = GetHits();
  if(hits.size() < 2) return Distance(pnt);
  
  double mindist = 1.0E+9;
  for(size_t i=0;i<hits.size();i++)
    {
      double x = hits[i]->GetX();
      double y = hits[i]->GetD();
      double d = ((x - pnt.GetX())*(x - pnt.GetX()) + 
		  (y - pnt.GetY())*(y - pnt.GetY()));
      if(mindist > d) mindist = d;
    }
  return sqrt(mindist);
}


//
// get y range
//
double Track2DPoint::GetYRange() const
{
  const std::vector<const LArReco::QHit*> hits = GetHits();
  if(hits.size() < 2) return 0;
  
  //
  return ( hits.back()->GetD() - hits[0]->GetD() );
}
