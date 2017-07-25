//////////////////////////////////////////////////////////////////////
// 
//  Join track fragments between different CRMs
//  The joining right now is done brute force, by looping over all CRMs
//
//  TODO: improve the efficiency for merging tracks by including 
//        some basic geometry considerations
//
//////////////////////////////////////////////////////////////////////


#ifndef _TRACKJOINCRM_H__
#define _TRACKJOINCRM_H__

#include <vector>
#include <cmath>

#include <TVector2.h>

#include "RecoConfig.h"
#include "Track2d.h"

class TrackJoinCRM
{
 public:
  TrackJoinCRM(RecoConfig &recopara);
  ~TrackJoinCRM();
  
  void Join( std::vector< std::vector<LArReco::Track2d> > &tracksegs,
	     std::vector< LArReco::Track2d > &tracks );
  
 private:
  int FindSegment( LArReco::Track2d &trk, 
		   std::vector<LArReco::Track2d> &trks_other, 
		   double sepdist, double tanstd );
  bool IsJoinable(LArReco::Track2d &trk1, LArReco::Track2d &trk2,
		  double sepdist, double tanstd);
  bool IntervalOverlap(double x0, double hlfw0, 
		       double x1, double hlfw1);
  
  double Distance( const TVector2 &pnt1, const TVector2 &pnt2 );

  bool   fNoJoin;
  double fNSgmTan;
  double fMinDist;
  //double fMaxDist;
  //double fStepDist;
};

#endif
