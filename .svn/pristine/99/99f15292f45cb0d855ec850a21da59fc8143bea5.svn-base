/////////////////////////////////////////////////////////////////////////////////
// 
//  Build smooth track representation from a reconstructed hit collection 
//  Should automatically tag and remove delta rays 
//
//  Uses ROOT TGraphSmooth to access smoothing algorithms 
//    - LOWESS (with fSmType = 0, also by default) 
//    - SMOOTH SUPER ( with fSmType = 1 )
//
// 
//  Created: Mon May  9 16:47:32 CEST 2016
//  Modified: 
//
//  TODO: fix for vertical tracks
//        electron shower detection at the end of the track
//
/////////////////////////////////////////////////////////////////////////////////

#ifndef __TRACKBUILDERSM_H__
#define __TRACKBUILDERSM_H__

#include <vector>

#include <TGraphSmooth.h> 

#include "QHit.h"
#include "Track2d.h"
#include "HitCollection.h"


class TrackBuilderSm
{
 public:
  TrackBuilderSm(std::vector<float> &params);
  ~TrackBuilderSm();
  
  void MakeTrack(const HitCollection *trkhits, const HitCollection *allhits,
		 std::vector<LArReco::Track2d> &tracks);
  
  //
  //void SetStep( float val ){ fStep = val; }
  
 private:

  void RunSmoother();
  void BuildTrack( std::vector<LArReco::Track2d> &tracks, int statusflag = 0 );
  void SmoothLowess();
  void SmoothSuper();

  double DistanceToLine(double x0, double y0, double a, double b);
  double GetTangent( double xval );

  int fSmType;     // type of smoothing algorithm
  int fNGapStep;   // number of steps before declaring the gap to be too large
  float fBass;     // controls smoothness
  float fSpan;     // smoother span
  float fMaxD;     // max distance from the curve to search for hits 
  bool fSmooth;

  // index
  int iFirst;
  int iLast;

  // hit cluster to use for track building
  const HitCollection *fTrkHitColl; 
  
  // primary hit cluster to use for look-up of missing hits (just in case)
  const HitCollection *fAllHitColl; 

  // hit block
  struct ChHits
  {
    int flag;
    int ch;
    int isave;
    int nhits;
    double mindis;
    double x;
    double y;
    std::vector<const LArReco::QHit*> hits;
  };

  // function to accept a given layer
  bool Accept(ChHits &ch);
  
  std::vector<ChHits> fHits;

  TGraphSmooth *fGrSmooth; 
  TGraph* fGrIn;  // input graph built from hits
  TGraph* fGrOut; // returned by TGraphSmooth

};
#endif
