//////////////////////////////////////////////////////////////////////
//
//  Utility class to help build 2D tracks in views
//
// 
//
//
//////////////////////////////////////////////////////////////////////

#ifndef __TRACK2DPOINT_H__
#define __TRACK2DPOINT_H__

#include <vector>
#include <cmath>

#include "HitCollection.h"

class Track2DPoint
{
  
 public:
  Track2DPoint( int uid, const LArReco::QHit* hit );
  ~Track2DPoint();

  // for 2D need only 3 elements
  void SetCovMatApri( const std::vector<double> &C );
  
  // X[0] = tangent, X[1] = intercept
  void SetStateApri( const std::vector<double> &s );
  
  // for 2D need only 3 elements
  void SetCovMat( const std::vector<double> &C );
  
  // X[0] = tangent, X[1] = intercept
  void SetState( const std::vector<double> &s );

  void SetResidual(double v){ fR = v; }
  void SetChi2(double v){ fChi2 = v; }
  void SetQFrac(double val) { fQFrac = val; }  

  //
  void SetY(double v){ fY = v; }

  // distance between points
  double Distance(Track2DPoint &pnt) const
  {
    double d = sqrt((fX - pnt.GetX())*(fX - pnt.GetX()) + 
		    (fY - pnt.GetY())*(fY - pnt.GetY()));
    return d;    
  }
  
  // get range in Y
  double GetYRange() const;
  
  // distance to closest hit in the collection
  double DistanceToClosestHit(Track2DPoint &pnt) const;
  
  // hits associated with this point
  const std::vector<const LArReco::QHit*>& GetHits() const
  { return fChHits.GetHits(); }
  
  //
  ChHitCollection& GetHitCollection(){ return fChHits; }
  
  // a posteriori
  const std::vector<double>& GetState() const { return fS; }
  const std::vector<double>& GetCovMat() const { return fC; }

  // a priori
  const std::vector<double>& GetStateApri() const { return fSapri; }
  const std::vector<double>& GetCovMatApri() const { return fCapri; }

  // compute inverse of cov matrix
  int GetCovMatInv( std::vector<double> &Cinv, int which );
  
  // 
  int   GetCh() const { return fCh; }
  double GetX() const { return fX; }
  double GetY() const { return fY; }
  double GetQ() const { return fQFrac*fChHits.GetQtot(); }
  double GetQFrac() const { return fQFrac; }
  
  // return the charge from all the hits added to this point
  int GetFirstTdc() const { return fChHits.GetFirstTdc(); }
  int GetLastTdc() const { return fChHits.GetLastTdc(); }
  
  // a posterior state values
  double GetPredTan() const { return fS[0]; }
  double GetPredPos() const { return fS[1]; }
  
  // a posterior matrix elements
  double GetCmm() const { return fC[0]; }
  double GetCbb() const { return fC[1]; }
  double GetCmb() const { return fC[2]; }
  
  //
  double GetResidual() const { return fR; }
  double GetChi2() const { return fChi2; }
  
  // Print
  void Print();
  
  //
  const std::vector<int>& GetUids(){ return fUids; }
  
  // add point
  Track2DPoint& operator+=(const Track2DPoint &pnt);

 private:
  //
  bool AddHit(const LArReco::QHit* hit)
  {
    return fChHits.AddHit( hit );
  }
  void AddHitVector(const std::vector<const LArReco::QHit*> &hits)
  {
    fChHits.AddHitVector( hits );
  }
  
  // vector of unique ids from which this point has been built
  std::vector<int> fUids;

  // measurements
  ChHitCollection fChHits;  

  int fCh;
  double fX;
  double fY;

  // residual and chi2 for this point
  double fR;
  double fChi2;

  // fraction of charge 
  double fQFrac;


  // a posteriori 
  // state vector for y = m*x + b
  // fX[0] = y, fX[1] = b
  std::vector<double> fS; 

  // a posteriori
  // covariance for y = m*x + b
  // fC[0] = Cmm - error^2 in m 
  // fC[1] = Cbb - error^2 in b
  // fC[2] = Cmb - correlation term
  std::vector<double> fC;
  
  // a priori values for state and covariance
  // state vector and covariance
  std::vector<double> fSapri;
  std::vector<double> fCapri;
};

#endif
