//////////////////////////////////////////////////////////////////////
// 
// MTC - Multi Track Cluster tries to find several track segments
//       in  given cluster
//       Mainly geared towards cosmic ray tagging in 6x6x6
//
// Implementation details:
//     Search for linear track cluster segments using KF
//     Getting the good starting region is the key to not getting lost
//
// TODO: fix for tracks going along the time axes
//       possibly implement a filter of hit chi2 after smoother
//
//////////////////////////////////////////////////////////////////////

#ifndef __TRACKBUILDERMTC_H__
#define __TRACKBUILDERMTC_H__


#include <vector>

#include <TGraph.h>

#include "LILUtil.h"
#include "KFFitterView.h"
#include "Track2DTrajectory.h"

#include "QHit.h"
#include "Track2d.h"
#include "HitCollection.h"


//
//
//
class TrackBuilderMTC
{
 public:
  
  TrackBuilderMTC(std::vector<float> &params);
  ~TrackBuilderMTC();

  int MakeTrack(const HitCollection *trkhits, const HitCollection *allhits,
		std::vector<LArReco::Track2d> &tracks);
  
  
  // graph trajectories (for debugging purposes)
  TGraph MakeGraphData();
  TGraph MakeGraphTrj( int i );
  TGraph MakeGraphTrjHit( int i );

  void Print();

  void Dump(const char *fname);

  //
  //
  //
  static bool IsShorterTrj(const Track2DTrajectory &lhs, const Track2DTrajectory &rhs)
  {
    return ( lhs.Length() < rhs.Length() );
  }
  
  //
  //
  static bool IsLongerTrj(const Track2DTrajectory &lhs, const Track2DTrajectory &rhs)
  {
    return ( lhs.Length() > rhs.Length() );
  }

 private:
  
  // hit cluster to use for track building
  const HitCollection *fTrkHitColl; 
  
  // primary hit cluster to use for look-up of missing hits (just in case)
  const HitCollection *fAllHitColl; 
  
  //
  //
  std::vector<Track2DPoint> fHitPoints;
  std::vector<Track2DTrajectory> fTrjs;
  
  //
  std::vector<int> fMultiHitBlock;
  std::vector<int> fHitsInUse;
  
  // function to stich long trajectories
  int MergeTrjs(std::vector<Track2DTrajectory> &trjs, bool skipoverlapped);
  
  void AddToTracks(Track2DTrajectory &trj, std::vector<LArReco::Track2d> &tracks );
  bool TryJoin(Track2DTrajectory &trj1, Track2DTrajectory &trj2, bool skipoverlapped);
  void MergeTrjPoints(Track2DTrajectory &trj1, Track2DTrajectory &trj2, bool append);
  void FindMissingHits(Track2DTrajectory &trj);
  
  //
  bool IntervalOverlap(double x0, double w0, double x1, double w1);
  
  //
  void SetPointsFlag(const std::vector<int> &idx, int flag);

  //
  void Init();
  
  //
  int SeedTrj(int idx);
  
  //
  void ExpandTrj(Track2DTrajectory &trj);

  // Smooth trajectory and reject outliers
  double SmoothTrj(Track2DTrajectory &trj, double chi2outcut = 0.0);

  //
  void ClearTrj(Track2DTrajectory &trj);

  //
  int FindNextHit( Track2DPoint &lastpnt, 
		   Track2DPoint &nextpnt, int psteps );
  
  // get rough estimation for PBeta from MS
  double GetPBeta(Track2DTrajectory &trj);

  
  void SetStartCov( Track2DPoint &pnt, double tan, double pos,
		    double tanerr, double poserr);
  
  // channel difference
  double fPitch;
  double fDriftSgm;
  
  //
  double fDistThresh; // search range for hits in a given channel
  double fChi2PntCut; // cut on accepting a hit
  double fChi2DirCut; // cut on changing the direction
  double fChi2TrjCut; // remove trj with chi2 larger than this cut
  double fTrjMergeD1; // for overlapping trj
  double fTrjMergeD2; // for non-overlapping trj
  double fHitMergeD;  // distance to add missing hits
  
  double fNSgmTan;    // number of standard deviations for tangent

  // initial momentum for mips
  double fPBeta;
  // cut on electron momentum
  double fPBetaEle;

  //
  int fFirstCh;
  int fLastCh;

  double fMinX;
  double fMaxX;
  
  // gap counter
  int fGapCounter;
  int fResetGapCounter;
  
  LILUtil *fLIL;

  KFFitterView *fKF;
};


#endif
