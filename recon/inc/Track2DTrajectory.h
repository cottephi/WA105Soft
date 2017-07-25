///////////////////////////////////////////////////////////////////////////
//
//
//
//  Utility to store track trajectories while building track object
//   
//
///////////////////////////////////////////////////////////////////////////

#ifndef __TRACK2DTRAJECTORY_H__
#define __TRACK2DTRAJECTORY_H__

#include <vector>
#include <deque>

#include <TMath.h>
#include <TVector2.h>
#include <TVector3.h>

#include "Track2DPoint.h"


//
// utility class to keep trajectory points
//
class Track2DTrajectory
{
 public:
  Track2DTrajectory();
  ~Track2DTrajectory();

  // comparison
  static bool ChCompPredicate(const Track2DPoint &lhs, const Track2DPoint &rhs)
  {
    return (lhs.GetCh() < rhs.GetCh());
  }

  void AddPoint( Track2DPoint& pnt );
  
  int GetFirstCh() const { return Points.front().GetCh(); }
  int GetLastCh() const {  return Points.back().GetCh(); }
  
  int GetColor() const { return fColor; }
  void SetColor(int val){ fColor = val; }

  int GetPid() const { return fPid; }
  void SetPid(int val){ fPid = val; }

  //
  double GetChi2() const { return fChi2; }
  void SetChi2( double val ) { fChi2 = val; }

  //
  void GetTrjPoints( std::vector< const LArReco::QHit* > &hits, std::vector<TVector2> &trkpath, 
		     std::vector<double> &dqdx);

  double Distance(Track2DPoint &pnt);

  // get coordinates of last points
  void GetCoordLast( size_t np, std::vector<double> &xcoord, 
		     std::vector<double> &ycoord );

  // get coordinates of the first points
  void GetCoordFirst(size_t np, std::vector<double> &xcoord, 
		     std::vector<double> &ycoord );
  
  // get total length
  double Length() const;
  
  void Print();

  //
  double GetPBeta() const { return fPBeta; }
  void SetPBeta( double val ){ fPBeta = val; }

  // track points
  std::deque<Track2DPoint> Points;  

  // delta rays ...
  std::vector<Track2DPoint> NearbyActivityPoints;
  
  void UpdateStats();
  
  // single vertical segment
  bool IsVertical() const { return GetFirstCh() == GetLastCh(); }

  void RemoveFirstPoint();
  void RemoveLastPoint();

 private:
  int fPid;
  double fPBeta;
  
  void AddPointToEnd( Track2DPoint& pnt );
  void AddPointToFront( Track2DPoint& pnt );
  
  double fChi2;  // chi2 / ndf

  //int fFirstCh;
  //int fLastCh;
  int fColor;
};

#endif
