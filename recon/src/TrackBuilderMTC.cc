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

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>

#include <TMath.h>
#include "RecoUtils.h"

#include "CosmicRecoVetoRegion.h"

//
#include "TrackBuilderMTC.h"

using namespace std;


//
//
//
TrackBuilderMTC::TrackBuilderMTC(std::vector<float> &params)
{
  // ch pitch
  fPitch = 0.3125;      // cm

  // resolution in drift direction for hits
  fDriftSgm = params[0]; //0.06;     // cm

  // distance to search for hits
  fDistThresh = params[1];

  // chi2 cut to accept points
  fChi2PntCut = params[2];
  fChi2DirCut = fChi2PntCut; //5.0;    // only use points with chi2 below this to update direction
  fChi2TrjCut = 10;

  // Kalman Filter parameters
  fPBeta      = params[3]; // Initial MS noise factor guess

  // set some value for pbeta of the delta rays
  fPBetaEle   = params[4]; //0.05;

  // how many channels are one is allowed to skip
  fGapCounter = (int)params[5];

  //
  fResetGapCounter = 2; //fGapCounter;

  // distance at which one can try to merge trajectories
  fTrjMergeD1 = params[6]; // overlapping 0.5;
  fTrjMergeD2 = params[7]; // non overlapping

  // number of sigmas to accept merging track direction
  fNSgmTan    = params[8]; //3.0;

  //
  fHitMergeD  = params[9]; //3.0;

  //
  fKF = new KFFitterView();
  fKF->SetResolution( fDriftSgm );
  fKF->SetProcessNoise( fPBeta );

  // sparse matrix utility
  fLIL = new LILUtil();
}


//
//
//
TrackBuilderMTC::~TrackBuilderMTC()
{
  delete fLIL;
  delete fKF;
}

//
// make tracks
//
int TrackBuilderMTC::MakeTrack(const HitCollection *trkhits, const HitCollection *allhits,
  std::vector<LArReco::Track2d> &tracks)
  {
    fTrkHitColl = trkhits;
    fAllHitColl = allhits;

    Init();

    for( size_t i=0;i<fHitPoints.size();i++)
    {
      if( fHitsInUse[i] != 0 ) continue;


      // try to seed a trajectory at this point
      if( SeedTrj((int)i) < 0 ) continue;

      // set initial conditions
      fKF->SetProcessNoise( fPBeta );
      fTrjs.back().SetPBeta( fPBeta );

      // try to expand
      ExpandTrj( fTrjs.back() );

      // only seed size
      if( fTrjs.back().Points.size() < 3 )
      {
        ClearTrj( fTrjs.back() );
        fTrjs.pop_back();
        continue;
      }

      // smooth trajectory
      double pbeta = GetPBeta( fTrjs.back() );
      if( pbeta > 0 )
      fTrjs.back().SetPBeta( pbeta );

      // short delta ray
      if( fTrjs.back().GetPBeta() == fPBeta && fTrjs.back().Length() < 5.0 )
      fTrjs.back().SetPBeta(0.99 * fPBetaEle);

      //
      SmoothTrj( fTrjs.back() );
    }

    // sort trajectories according to their lengths
    std::sort( fTrjs.begin(), fTrjs.end(), IsLongerTrj );

    //
    //
    //
    std::vector<Track2DTrajectory> trjsLong;
    std::vector<Track2DTrajectory> trjsShort;

    // remove short delta ray like trajectories
    for(size_t i=0;i<fTrjs.size();++i)
    {
      fTrjs[i].SetColor(i+2);
      if( fTrjs[i].GetPBeta() < fPBetaEle )
      {
        trjsShort.push_back( fTrjs[i] );
        trjsShort.back().SetPid( 11 );
      }
      else
      {
        trjsLong.push_back( fTrjs[i] );
        trjsLong.back().SetPid( 13 );
      }
    }

    fTrjs.clear();


    // try to merge trajectories
    MergeTrjs( trjsLong, false );
    MergeTrjs( trjsShort, true );

    size_t ntrkstart = tracks.size();

    // remove short trajectories which were not merged or have bad chi2
    std::vector<Track2DTrajectory>::iterator it = trjsLong.begin();
    for(;it!=trjsLong.end();)
    {
      SmoothTrj( *it );
      if(it->Points.size() <= 3 || it->Length() < 0.5*fTrjMergeD2)
      {
        ClearTrj( *it );
        trjsLong.erase( it );
        continue;
      }

      FindMissingHits( *it );
      AddToTracks( *it, tracks );

      ++it;
    }

    it = trjsShort.begin();
    for(;it!=trjsShort.end();)
    {
      SmoothTrj( *it );
      double trjchi2 = it->GetChi2();
      if( trjchi2 == 0 || trjchi2 > fChi2TrjCut )
      {
        ClearTrj( *it );
        trjsShort.erase( it );
        continue;
      }

      AddToTracks( *it, tracks );
      ++it;
    }



    // DEBUGING
    /*
    fTrjs.insert( fTrjs.end(), trjsLong.begin(), trjsLong.end() );
    size_t ntrk = fTrjs.size();
    fTrjs.insert( fTrjs.end(), trjsShort.begin(), trjsShort.end() );

    for(size_t i=0;i<fTrjs.size();++i)
    {
    if( i>=ntrk )
    fTrjs[i].SetColor(46);
  }

  cout<<"Found "<<fTrjs.size()<<" trajectories "<<endl;

  // add missing hits for muon tracks and create track objects
  for( size_t i=0;i<fTrjs.size();++i )
  {
  // smooth trajectory before adding missing channels
  double chi2fit = fTrjs[i].GetChi2(); //SmoothTrj( fTrjs[i] );
  //if(fTrjs[i].GetColor() == 2) fTrjs[i].Print();
  cout<<" chi2 fit for this trajectory "<<setw(15)<<chi2fit
  <<setw(15)<<fTrjs[i].Length()<<setw(15)<<fTrjs[i].Points.front().GetX()
  <<setw(15)<<fTrjs[i].GetPBeta()<<setw(5)<<fTrjs[i].GetColor()<<endl;
}
*/
return (int)(tracks.size() - ntrkstart);
}


//
// the order is that the start of the track is earlier in time
// this works well for cosmics
//
void TrackBuilderMTC::AddToTracks( Track2DTrajectory &trj,
  std::vector<LArReco::Track2d> &tracks )
  {
    // track hits and path
    std::vector<const LArReco::QHit*> trkhits;
    std::vector<TVector2> trkpath;
    std::vector<double> dqdx;

    double chi2 = trj.GetChi2();
    double mom  = trj.GetPBeta();
    int    pid  = trj.GetPid();
    std::vector<double> errStart(3);
    std::vector<double> errEnd(3);
    double tanStart;
    double tanEnd;
    int lastch;

    //
    deque<Track2DPoint>::iterator          it = trj.Points.begin();
    deque<Track2DPoint>::reverse_iterator rit = trj.Points.rbegin();


    if( it->GetPredPos() < rit->GetPredPos() )
    {
      tanStart    = it->GetPredTan();
      errStart[0] = sqrt( it->GetCmm() );
      errStart[1] = sqrt( it->GetCbb() );
      errStart[2] = it->GetCmb() / (errStart[0] * errStart[1]);

      tanEnd    = rit->GetPredTan();
      errEnd[0] = sqrt( rit->GetCmm() );
      errEnd[1] = sqrt( rit->GetCbb() );
      errEnd[2] = rit->GetCmb() / (errEnd[0] * errEnd[1]);

      lastch = -1;
      for(;it!=trj.Points.end();++it)
      {
        double x   = it->GetX();
        double y   = it->GetPredPos();
        double q   = it->GetQ();
        int ch     = it->GetCh();
        int nch    = fabs( ch - lastch );
        if( lastch < 0 ) nch = 1;
        if( nch > 1 ) // set q = 0 if missing hits in a given channel
        {
          float sgn = (ch - lastch ) / nch;
          double x0 = trkpath.back().X();
          double y0 = trkpath.back().Y();
          double s0 = (*(it-1)).GetPredTan();
          for( int i=1;i<nch;i++)
          {
            double d = sgn*i*fPitch;
            dqdx.push_back( 0 );
            trkpath.push_back( TVector2( x0+d, y0+s0*d ) );
          }
        }

        const std::vector<const LArReco::QHit*> hits = it->GetHits();
        trkhits.insert( trkhits.end(), hits.begin(), hits.end() );
        dqdx.push_back( q );
        trkpath.push_back( TVector2( x, y ) );
        lastch = ch;
      }
    }
    else
    {
      tanStart    = rit->GetPredTan();
      errStart[0] = sqrt( rit->GetCmm() );
      errStart[1] = sqrt( rit->GetCbb() );
      errStart[2] = rit->GetCmb() / (errStart[0] * errStart[1]);

      tanEnd    = it->GetPredTan();
      errEnd[0] = sqrt( it->GetCmm() );
      errEnd[1] = sqrt( it->GetCbb() );
      errEnd[2] = it->GetCmb() / (errEnd[0] * errEnd[1]);

      lastch = -1;
      for(;rit!=trj.Points.rend();++rit)
      {
        double x   = rit->GetX();
        double y   = rit->GetPredPos();
        double q   = rit->GetQ();
        int ch     = rit->GetCh();
        int nch    = fabs( ch - lastch );
        if( lastch < 0 ) nch = 1;
        if( nch > 1 ) // set q = 0 if missing hits in a given channel
        {
          float sgn = (ch - lastch ) / nch;
          double x0 = trkpath.back().X();
          double y0 = trkpath.back().Y();
          double s0 = (*(rit-1)).GetPredTan();
          for( int i=1;i<nch;i++)
          {
            double d = sgn*i*fPitch;
            dqdx.push_back( 0 );
            trkpath.push_back( TVector2( x0+d, y0+s0*d ) );
          }
        }

        const std::vector<const LArReco::QHit*> hits = rit->GetHits();
        trkhits.insert( trkhits.end(), hits.begin(), hits.end() );
        dqdx.push_back( q );
        trkpath.push_back( TVector2( x, y ) );
        lastch = ch;
      }

    }
    LArReco::Track2d atrk( trkpath, dqdx, trkhits );
    atrk.SetChi2( chi2 );
    atrk.SetMomentum( mom );
    atrk.SetPid( pid );
    atrk.SetSlopeInit( tanStart );
    atrk.SetStartErrors( errStart[1], errStart[0], errStart[2] );
    atrk.SetSlopeFinal( tanEnd );
    atrk.SetEndErrors( errEnd[1], errEnd[0], errEnd[2] );

    // add to list
    tracks.push_back( atrk );
  }


  //
  //
  //
  void TrackBuilderMTC::Init()
  {
    std::vector<double> xdata, ydata;

    fHitPoints.clear();
    fHitsInUse.clear();
    fHitsInUse.resize( fTrkHitColl->GetNhits(), 0 );

    fTrjs.clear();

    fMultiHitBlock.clear();
    fMultiHitBlock.resize( fTrkHitColl->GetNhits(), -1 );


    const std::deque<ChHitCollection*> chhitcoll = fTrkHitColl->GetHits();
    for(size_t i=0;i<chhitcoll.size();i++)
    {
      // hits for this channel
      const std::vector<const LArReco::QHit*> chhits = chhitcoll[i]->GetHits();

      for( size_t j=0;j<chhits.size();j++)
      {
        const LArReco::QHit* ahit = chhits[j];

        // add the hit to our hit vector
        int uid = (int)fHitPoints.size();

        Track2DPoint pnt = Track2DPoint( uid, ahit );

        if( j > 0 )
        {
          // multihit block
          if( (ahit->GetD() - fHitPoints.back().GetY()) <  5.0 )
          {
            int idx = (int)fHitPoints.size();
            if(fMultiHitBlock[idx-1] < 0)
            fMultiHitBlock[idx]   = idx-1; // this hit
            else
            fMultiHitBlock[idx] = fMultiHitBlock[idx-1];
          }
        }

        bool hveto = RecoUtils::CosmicRecoVetoRegion::Instance().isInside( ahit->GetX(),
        ahit->GetD(),
        ahit->GetView() );
        // if hit is in the veto region mask it
        // NOTE we will use fHitPoints.size() as index
        //      so important to do this before changing the size
        if( hveto ) fHitsInUse[ fHitPoints.size() ] = -1;

        // add hit to the vector
        fHitPoints.push_back( pnt );
        xdata.push_back( pnt.GetX() );
        ydata.push_back( pnt.GetY() );
      }
    }

    fFirstCh = fHitPoints[0].GetCh();
    fLastCh  = fHitPoints.back().GetCh();

    fMinX = fHitPoints[0].GetX();
    fMaxX = fHitPoints.back().GetX();

    fLIL->Init( fHitPoints[0].GetX(), fPitch, fFirstCh, fLastCh );
    fLIL->Build( xdata, ydata );
  }

  //
  //
  // try to seed from 3 nearby points
  //
  int TrackBuilderMTC::SeedTrj( int idx )
  {
    if( fHitsInUse[idx] != 0 ) return -1;

    vector<int> pidx(3);
    vector<double> xsub(3), ysub(3);
    int ch    = fHitPoints[idx].GetCh();
    double xp = fHitPoints[idx].GetX();
    double yp = fHitPoints[idx].GetY();

    fHitsInUse[idx] = -1;
    pidx[0] = idx;
    xsub[0] = xp;
    ysub[0] = yp;

    int nextch = ch+1;
    int ridx  = fLIL->FindClosest( nextch, yp, fDistThresh );
    if( ridx < 0 ) return -1;
    if( fHitsInUse[ridx] > 0 ) return -2;
    fHitsInUse[ridx] = -1;
    pidx[1] = ridx;
    xsub[1] = fHitPoints[ridx].GetX();
    ysub[1] = fHitPoints[ridx].GetY();

    nextch = nextch + 1;
    xp = xsub[1] + fPitch;
    yp = ysub[1] + ( xp - xsub[1] )*( ysub[1] - ysub[0] )/( xsub[1] - xsub[0] );
    ridx  = fLIL->FindClosest( nextch, yp, fDistThresh );
    if( ridx < 0 ) return -1;
    if( fHitsInUse[ridx] > 0 ) return -2;
    fHitsInUse[ridx] = -1;
    pidx[2] = ridx;
    xsub[2] = fHitPoints[ridx].GetX();
    ysub[2] = fHitPoints[ridx].GetY();


    std::vector<double> params(2), params_err(2);
    double serr;
    double rval = RecoUtils::FitLineLSQ( &xsub[0], &ysub[0], xsub.size(),
    params, params_err, serr);
    if( rval < 0 ) return -1;

    // init new trajectory
    fHitsInUse[ idx ] = 1;
    fTrjs.push_back( Track2DTrajectory() );

    Track2DPoint pnt = fHitPoints[ idx ];

    double tan = params[1];
    double pos = params[0] + params[1]*pnt.GetX();
    double tanerr = 0.5*params[1];
    double poserr = 1.0;

    SetStartCov( pnt, tan, pos, tanerr, poserr );
    // std::vector<double> s(2), C(3);
    // s[0] = params[1];
    // s[1] = params[0] + params[1]*pnt.GetX();
    // // set some large errors to start
    // C[0] = (0.5*params[1])*(0.5*params[1]);  //params_err[1] * params_err[1];
    // C[1] = 0.16;                             //params_err[0] * params_err[0];
    // C[2] = 0.0;
    // pnt.SetState( s );
    // pnt.SetCovMat( C );

    // fKF->MakePrediction( pnt, 0 );


    fTrjs.back().AddPoint( pnt );
    return 1;
  }

  //
  //
  //
  void TrackBuilderMTC::SetStartCov( Track2DPoint &pnt, double tan, double pos,
    double tanerr, double poserr)
    {
      std::vector<double> s(2), C(3);
      s[0] = tan;
      s[1] = pos;
      // set some large errors to start
      C[0] = tanerr*tanerr;
      C[1] = poserr*poserr;
      C[2] = 0.0;
      pnt.SetState( s );
      pnt.SetCovMat( C );

      fKF->MakePrediction( pnt, 0 );
      fKF->UpdateEstimate( pnt );
    }



    //
    //
    //
    int TrackBuilderMTC::FindNextHit( Track2DPoint &lastpnt,
      Track2DPoint &nextpnt, int psteps )
      {
        int c_ch = lastpnt.GetCh();
        int n_ch = c_ch + psteps;

        double delta = psteps * fPitch;

        //
        fKF->MakePrediction( lastpnt, delta );

        double eps = fDistThresh;
        int idx = fLIL->FindClosest( n_ch, fKF->GetPredPos(), eps);

        if( idx < 0 ) return -1;
        double chi2, res;
        chi2 = fKF->GetChi2( fHitPoints[ idx ].GetY(), res );

        nextpnt = fHitPoints[ idx ];
        nextpnt.SetChi2( chi2 );
        nextpnt.SetResidual( res );

        // save cov and state vector
        fKF->UpdateEstimate( nextpnt );

        if( chi2 > fChi2PntCut )
        {
          return -3;
        }


        return idx;
      }


      //
      //
      //
      void TrackBuilderMTC::ExpandTrj(Track2DTrajectory &trj)
      {
        // we always go in the positive sense +ve x
        int gapcounter   = 0; // couter for ch gaps
        int resetcounter = 0; // avoid reseting gapcounter too often
        int nextch       = trj.Points.back().GetCh();

        std::vector<Track2DPoint> tmpPoints;

        bool pbetaMS = false;

        // dummy loop, but should not run longer than the number of points!
        for(size_t i=0;i<fHitPoints.size();i++)
        {

          Track2DPoint endpnt = trj.Points.back();
          Track2DPoint nextpnt = endpnt;

          if( !pbetaMS ) // calculate an estimate for pbeta
          {
            double pbeta = GetPBeta( fTrjs.back() );
            if( pbeta > 0 )
            {
              trj.SetPBeta( pbeta );
              pbetaMS = true;
            }
          }

          // check ch status .... (TO DO)
          nextch++;
          if(nextch > fLastCh) break;

          int steps = nextch - endpnt.GetCh();

          int hit_idx = FindNextHit( endpnt, nextpnt, steps );

          if( hit_idx < 0 )  gapcounter++;
          else
          {
            // this hit is not used
            if( fHitsInUse[hit_idx] <= 0 )
            {
              if(resetcounter>=fResetGapCounter)
              {
                gapcounter = 0;
                resetcounter = 0;
              }
              if( gapcounter > 0 ) resetcounter++;
              fHitsInUse[hit_idx] = 1;

              // update the direction only if we are below chi2 on direction
              if( nextpnt.GetChi2() < fChi2DirCut )
              {
                trj.AddPoint( nextpnt );
                if( !tmpPoints.empty() )
                {
                  for(size_t ii=0;ii<tmpPoints.size();++ii)
                  trj.AddPoint( tmpPoints[ii] );
                  tmpPoints.clear();
                }
              }
              else
              {
                tmpPoints.push_back( nextpnt );
              }

              //
              nextch = nextpnt.GetCh();
            }
            else
            {
              gapcounter++;
            }
          }
          if( gapcounter >= fGapCounter )
          {
            if( !tmpPoints.empty() )
            {
              for(size_t ii=0;ii<tmpPoints.size();++ii)
              trj.AddPoint( tmpPoints[ii] );
              tmpPoints.clear();
            }

            break;
          }
        }

        // go in -ve x
        gapcounter = 0;
        resetcounter = 0;
        nextch =  trj.Points.front().GetCh();
        tmpPoints.clear();
        // dummy loop, but should not run longer than the number of points!
        for(size_t i=0;i<fHitPoints.size();i++)
        {

          Track2DPoint endpnt  = trj.Points.front();
          Track2DPoint nextpnt = endpnt;

          if( !pbetaMS ) // calculate an estimate for pbeta
          {
            double pbeta = GetPBeta( fTrjs.back() );
            if( pbeta > 0 )
            {
              trj.SetPBeta( pbeta );
              pbetaMS = true;
            }
          }


          // check ch status .... (TO DO)
          nextch--;
          if(nextch < fFirstCh) break;

          int steps = nextch - endpnt.GetCh();

          int hit_idx = FindNextHit( endpnt, nextpnt, steps );

          if( hit_idx < 0 ) gapcounter++;
          else
          {
            // this hit is not used
            if( fHitsInUse[hit_idx] <= 0 )
            {
              if(resetcounter>=fResetGapCounter)
              {
                gapcounter = 0;
                resetcounter = 0;
              }
              if( gapcounter > 0 ) resetcounter++;
              fHitsInUse[hit_idx] = 1;

              if( nextpnt.GetChi2() < fChi2DirCut )
              {
                trj.AddPoint( nextpnt );
                if(!tmpPoints.empty())
                {
                  for(size_t ii=0;ii<tmpPoints.size();++ii)
                  trj.AddPoint( tmpPoints[ii] );
                  tmpPoints.clear();
                }
              }
              else
              {
                tmpPoints.push_back(nextpnt);
              }

              //
              nextch = nextpnt.GetCh();
            }
            else
            {
              gapcounter++;
            }
          }

          if( gapcounter >= fGapCounter )
          {
            for(size_t ii=0;ii<tmpPoints.size();++ii)
            trj.AddPoint( tmpPoints[ii] );
            tmpPoints.clear();
            break;
          }
        }


        // remove isolated end hits at endpoints
        // deque<Track2DPoint>::reverse_iterator rit = trj.Points.rbegin();

        // if( fabs((*rit).GetCh() - (*(rit+1)).GetCh()) > 2 )
        //   {
        //     SetPointsFlag( (*rit).GetUids(), -1 );
        //     trj.RemoveFirstPoint();
        //   }

        // deque<Track2DPoint>::iterator it = trj.Points.begin();
        // if( fabs((*it).GetCh() - (*(it+1)).GetCh()) > 2 )
        //   {
        //     SetPointsFlag( (*it).GetUids(), -1 );
        //     trj.RemoveLastPoint();
        //   }
      }

      //
      //
      //
      void TrackBuilderMTC::FindMissingHits( Track2DTrajectory &trj )
      {
        deque<Track2DPoint>::iterator it = trj.Points.begin();

        fKF->SetProcessNoise( trj.GetPBeta() );

        // use some offset
        for(size_t i=0;i<trj.Points.size()-1;++i)
        {
          Track2DPoint cpnt = trj.Points[i];
          Track2DPoint npnt = trj.Points[i+1];

          int nsteps = npnt.GetCh() - cpnt.GetCh();
          if( nsteps > 1)
          {
            for(int j=1;j<nsteps;j++)
            {

              double delta = j * fPitch;

              //
              fKF->MakePrediction( cpnt, delta );

              int n_ch = cpnt.GetCh() + j;
              int idx = fLIL->FindClosest( n_ch, fKF->GetPredPos(), fHitMergeD);
              if( idx < 0 ) continue;
              if( fHitsInUse[idx] > 0 ) continue;

              Track2DPoint nextpnt = fHitPoints[ idx ];
              //if( fHitsInUse[idx] > 0 )
              //nextpnt.SetQFrac( 0.5*nextpnt.GetQFrac() );
              //nextpnt.SetChi2( chi2 );
              //nextpnt.SetResidual( res );
              //nextpnt.SetCovMat( fKF->GetPredCov() );

              // but do not use this point in the fit
              std::vector<double> s;
              fKF->GetPredState(s);
              nextpnt.SetState(s);

              trj.AddPoint( nextpnt );
              fHitsInUse[idx] = 1;
            }
          }
        }
      }

      //
      // TODO: add outlier rejection
      //
      double TrackBuilderMTC::SmoothTrj(Track2DTrajectory &trj, double chi2outcut)
      {
        fKF->SetProcessNoise( trj.GetPBeta() );

        double chi2fit = 0.0;

        // run forward filter
        deque<Track2DPoint>::iterator it = trj.Points.begin();

        // set some initial conditions
        double tan = ( (*(it+1)).GetY() - it->GetY() )/( (*(it+1)).GetX() - it->GetX() );
        double pos = it->GetY();
        SetStartCov( *it, tan, pos, 0.5*tan, 0.5 );
        ++it;
        for(;it!=trj.Points.end();it++)
        {
          double step = (*it).GetX() - (*(it-1)).GetX();
          fKF->MakePrediction( *(it-1), step );
          fKF->UpdateEstimate( *(it) );
        }


        // run KF smoother
        deque<Track2DPoint>::reverse_iterator rit = trj.Points.rbegin();
        chi2fit = rit->GetChi2();
        rit++;
        //if( trj.GetColor() == 2 ) trj.Points.rbegin()->Print();
        for(;rit!=trj.Points.rend();rit++)
        {
          fKF->Smoother(*(rit), *(rit-1));
          chi2fit += rit->GetChi2();
        }

        // chi2 for each point should be independent
        // for 2D chi2 is that of 1ndf
        // so global chi2 should be ideally ~1
        chi2fit = chi2fit/trj.Points.size();

        trj.SetChi2( chi2fit );
        return chi2fit;
      }

      //
      // clear used points flags
      //
      void TrackBuilderMTC::ClearTrj(Track2DTrajectory &trj)
      {
        for(size_t i=0;i<trj.Points.size();i++)
        {
          SetPointsFlag( trj.Points[i].GetUids(), -1 );
        }

        for(size_t i=0;i<trj.NearbyActivityPoints.size();i++)
        {
          SetPointsFlag( trj.NearbyActivityPoints[i].GetUids(), -1 );
        }
      }

      //
      //
      //
      void TrackBuilderMTC::SetPointsFlag( const std::vector<int> &idx, int flag )
      {
        for(size_t i=0;i<idx.size();i++)
        {
          int p = idx[i];
          if( p < 0 || p>= (int)fHitsInUse.size() )
          {
            cerr<<"ERROR: "<<__FILE__<<": "<<__LINE__
            <<" hit index is not correct "<<p<<endl;
            continue;
          }
          fHitsInUse[p] = flag;
        }
      }


      //
      // Find if two intervals overlap. Interval in format: x +/- half width
      //
      bool TrackBuilderMTC::IntervalOverlap(double x0, double hlfw0, double x1, double hlfw1)
      {
        return ( TMath::Abs( x1 - x0 ) < (hlfw0 + hlfw1) );
      }


      //
      // recursive function to merge trajectories
      //
      int TrackBuilderMTC::MergeTrjs(std::vector<Track2DTrajectory> &trjs, bool skipoverlapped)
      {
        if(trjs.size() <= 1) return 0;

        // we make a double loop and then remove empties
        size_t nsz = trjs.size();
        size_t njn = 0;
        for(size_t i=0;i<nsz;i++)
        {
          if( trjs[i].Points.empty() ) continue;

          // try to avoid merging with delta rays
          for( size_t j=0;j<nsz;j++)
          {
            // skip the current
            if( j == i ) continue;

            // skip the empty
            if( trjs[j].Points.empty() ) continue;

            if( TryJoin( trjs[i], trjs[j], skipoverlapped ) )
            {
              //SmoothTrj( trjs[i] );
              njn++;
            }
          }
        }

        if(!njn) return 0;

        std::vector<Track2DTrajectory>::iterator it = trjs.begin();
        for(;it!=trjs.end();)
        {
          if((*it).Points.empty())
          {
            trjs.erase( it );
          }
          else
          {
            ++it;
          }
        }

        return MergeTrjs( trjs, skipoverlapped );
      }


      //
      // try to merge trj2 into trj1
      //
      bool TrackBuilderMTC::TryJoin(Track2DTrajectory &trj1, Track2DTrajectory &trj2,
        bool skipoverlapped)
        {
          //return false;

          Track2DTrajectory aTrj1 = trj1;
          Track2DTrajectory aTrj2 = trj2;

          //
          if( aTrj2.Points.front().GetX() < aTrj1.Points.front().GetX() )
          {
            aTrj1 = trj2;
            aTrj2 = trj1;
          }

          Track2DPoint Pnt1Str = aTrj1.Points.front();
          Track2DPoint Pnt1End = aTrj1.Points.back();

          Track2DPoint Pnt2Str = aTrj2.Points.front();
          Track2DPoint Pnt2End = aTrj2.Points.back();

          bool tomerge = false;

          // overlaping tracks in x
          if( Pnt1End.GetX() >= Pnt2Str.GetX() )
          {
            if( skipoverlapped ) return false;
            double dmin1 = aTrj1.Distance( Pnt2Str );
            double dmin2 = aTrj2.Distance( Pnt1End );

            if( dmin1 > fTrjMergeD1 && dmin2 > fTrjMergeD1 ) return false;

            if( Pnt1End.GetX() >= Pnt2End.GetX() )
            {
              if( dmin1 < fTrjMergeD1 && aTrj1.Distance( Pnt2End ) < fTrjMergeD1 )
              tomerge = true;
              else return false;
            }
            else
            {
              if( dmin1 <= dmin2 )
              {
                double tan2    = Pnt2Str.GetPredTan();
                double tan2err = sqrt(Pnt2Str.GetCmm());

                //
                std::deque<Track2DPoint>::iterator it;
                it = std::lower_bound( aTrj1.Points.begin(), aTrj1.Points.end(), Pnt2Str,
                Track2DTrajectory::ChCompPredicate );

                if( it == aTrj1.Points.end() ) return false;

                double tan1    = it->GetPredTan();
                double tan1err = sqrt(it->GetCmm());

                if( !IntervalOverlap( tan1, fNSgmTan*tan1err, tan2, fNSgmTan*tan2err ) ) return false;

                tomerge = true;
              }
              else
              {
                double tan1    = Pnt1End.GetPredTan();
                double tan1err = sqrt(Pnt1End.GetCmm());

                //
                std::deque<Track2DPoint>::iterator it;
                it = std::lower_bound( aTrj2.Points.begin(), aTrj2.Points.end(), Pnt1End,
                Track2DTrajectory::ChCompPredicate );

                if( it == aTrj2.Points.end() ) return false;

                double tan2    = it->GetPredTan();
                double tan2err = sqrt(it->GetCmm());

                if( !IntervalOverlap( tan1, fNSgmTan*tan1err, tan2, fNSgmTan*tan2err ) ) return false;

                tomerge = true;
              }
            }
          }
          else // non overlapping
          {
            double dmin = Pnt1End.Distance( Pnt2Str );

            if( dmin > fTrjMergeD2 ) return false;

            // if( aTrj1.GetColor() == 3 && aTrj2.GetColor() == 7 )
            // 	{
            // 	  aTrj1.Print();
            // 	  aTrj2.Print();
            // 	  cout<<" dmin "<<endl;
            // 	}

            // test whether directions are colinear
            double tan1    = Pnt1End.GetPredTan();
            double tan1err = sqrt(Pnt1End.GetCmm());
            double tan2    = Pnt2Str.GetPredTan();
            double tan2err = sqrt(Pnt2Str.GetCmm());

            if( !IntervalOverlap( tan1, fNSgmTan*tan1err, tan2, fNSgmTan*tan2err ) ) return false;
            tomerge = true;
          }

          if( tomerge )
          {
            MergeTrjPoints( aTrj1, aTrj2, true );

            // save and exit
            trj1 = aTrj1;
            trj2 = aTrj2; //empty ...

            trj1.SetPBeta( 0.5*(trj1.GetPBeta() + trj2.GetPBeta()) );
          }

          return tomerge;
        }

        //
        //
        //
        void TrackBuilderMTC::MergeTrjPoints(Track2DTrajectory &trj1,
          Track2DTrajectory &trj2, bool append)
          {
            // add points to the first trajectory
            while( !trj2.Points.empty() )
            {
              if(append)
              {
                trj1.AddPoint( trj2.Points.front() );
                trj2.Points.pop_front();
              }
              else
              {
                trj1.AddPoint( trj2.Points.back() );
                trj2.Points.pop_back();
              }
            }
          }


          //
          // dump trajectory parameters into file (for debugging)
          //
          void TrackBuilderMTC::Dump(const char *fname)
          {
            ofstream fout(fname);

            for(size_t i=0;i<fTrjs.size();i++)
            {
              fout<<"========================= Start "<<i+1<<"/"<<fTrjs.size()<<endl;
              for(size_t j=0;j<fTrjs[i].Points.size();j++)
              {
                fout<<setw(7)<<fTrjs[i].Points[j].GetCh()
                <<setw(15)<<fTrjs[i].Points[j].GetX()
                <<setw(15)<<fTrjs[i].Points[j].GetY()
                <<setw(15)<<fTrjs[i].Points[j].GetPredTan()
                <<setw(15)<<sqrt(fTrjs[i].Points[j].GetCmm())
                <<setw(15)<<fTrjs[i].Points[j].GetPredPos()
                <<setw(15)<<sqrt(fTrjs[i].Points[j].GetCbb())
                <<setw(15)<<fTrjs[i].Points[j].GetChi2()<<endl;
              }
              fout<<"========================= End   "<<i+1<<"/"<<fTrjs.size()<<endl;
            }


            fout.close();

          }

          //
          // pbeta from MS (not an accuruate estimation since we are in 2D)
          //
          double TrackBuilderMTC::GetPBeta(Track2DTrajectory &trj)
          {
            if(trj.Points.size() <= 3 ) return 0;

            size_t npt = 20;
            if( trj.Points.size() < npt ) npt = trj.Points.size();

            double pbeta = 0;
            size_t ncnt  = 0;
            for(size_t i=0;i<trj.Points.size()-npt+1;i+=(npt-1))
            {
              double x1 = trj.Points[i].GetX();
              double y1 = trj.Points[i].GetY();
              double x2 = trj.Points[i+npt-1].GetX();
              double y2 = trj.Points[i+npt-1].GetY();
              double dx =  x2 - x1;
              if(dx < 0) continue;

              double dy  =  y2 - y1;
              double dd  = dx*dx + dy*dy;
              double len = sqrt( dd );

              double s2  = 0;
              size_t nsz = 0;
              for(size_t j=1;j<npt-1;j++)
              {
                double x0 = trj.Points[i+j].GetX();
                double y0 = trj.Points[i+j].GetY();

                double tmp = ( dy * x0 - dx * y0 + x2*y1 - y2*x1 );
                s2 += tmp*tmp/dd;
                nsz++;
              }
              s2 = sqrt(s2/nsz);
              double tht = (6.93 * s2)/ len;
              pbeta += 13.6E-3/tht * sqrt(len / 14.0);
              ncnt++;
            }
            if(ncnt==0) return 0;

            // average from many points
            pbeta /= ncnt;

            return pbeta; //GeV
          }


          //
          //
          //
          void TrackBuilderMTC::Print()
          {
            //
            cout<<" ========= Total number of trajectories "<<fTrjs.size()<<endl;
            for(size_t i=0;i<fTrjs.size();i++)
            {
              cout<<" ---------------- > "<<i+1<<"/"<<fTrjs.size()<<endl;
              fTrjs[i].Print();
              cout<<endl;
            }
          }


          //// Graphing functions for debugging purposes

          //
          // Make graph from all the points
          //
          TGraph TrackBuilderMTC::MakeGraphData()
          {
            TGraph gr;
            for(size_t i=0;i<fHitPoints.size();i++)
            gr.SetPoint(i, fHitPoints[i].GetX(), fHitPoints[i].GetY() );

            return gr;
          }

          //
          // Make graph from all the points in a given trajectory
          //
          TGraph TrackBuilderMTC::MakeGraphTrj( int i )
          {
            Track2DTrajectory trj = fTrjs[i];


            TGraph gr;
            for(size_t i=0;i<trj.Points.size();i++)
            gr.SetPoint(i, trj.Points[i].GetX(), trj.Points[i].GetPredPos());

            gr.SetLineColor( trj.GetColor() );
            return gr;
          }

          //
          // Make graph from all the hits in a given trajectory
          //
          TGraph TrackBuilderMTC::MakeGraphTrjHit( int i )
          {
            Track2DTrajectory trj = fTrjs[i];

            TGraph gr;
            for(size_t i=0;i<trj.Points.size();i++)
            {
              const std::vector<const LArReco::QHit*> hits = trj.Points[i].GetHits();
              for(size_t j=0;j<hits.size();j++)
              {
                double x = hits[j]->GetX();
                double y = hits[j]->GetD();

                gr.SetPoint(gr.GetN(), x, y);
              }
            }

            gr.SetMarkerColor( trj.GetColor() );
            return gr;
          }
