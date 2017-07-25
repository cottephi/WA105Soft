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

#include <iostream>
#include <cmath>

#include <iomanip>
//#include <TMath.h>

#include "TrackBuilderSm.h"
#include "RecoUtils.h"

//
// ctor
//
TrackBuilderSm::TrackBuilderSm( std::vector<float> &params )
{
  fSmType   = (int)params[0];
  fNGapStep = (int)params[1];
  fMaxD     = params[2];
  fBass     = params[3];
  fSpan     = params[4];
  fSmooth   = (params[5]>=1);

  
  fGrIn     = new TGraph();
  fGrSmooth = new TGraphSmooth("trksmooth");
}


//
// dtor
//
TrackBuilderSm::~TrackBuilderSm()
{
  //fout->Close();
  delete fGrIn;
  delete fGrSmooth;

}


//
// function to get a new track
//
void TrackBuilderSm::MakeTrack( const HitCollection *trkhits, const HitCollection *allhits, 
				std::vector<LArReco::Track2d> &tracks )
{
  fTrkHitColl = trkhits;
  fAllHitColl = allhits;
  
  fHits.clear();
  fGrIn->Set(0);

  
  const std::deque<ChHitCollection*> chhitcoll = fTrkHitColl->GetHits();
  for(size_t i=0;i<chhitcoll.size();i++)
    {
      // hits for this channel
      const std::vector<const LArReco::QHit*> chhits = chhitcoll[i]->GetHits();
      
      //std::cout<<i<<" "<<chhits.size()<<std::endl;
      if(chhits.empty()) continue;

      ChHits hblk;
      hblk.flag   = 0;
      hblk.ch     = chhitcoll[i]->GetChId();
      hblk.x      = chhitcoll[i]->GetChPos();
      hblk.isave  = 0;
      hblk.mindis = 999;
      hblk.nhits  = (int)chhits.size();
      hblk.hits   = chhits;
      
      fHits.push_back( hblk );      
    }

  
  // tag as drays blocks with multiple hits 
  for(size_t i=0;i<fHits.size();i++)
    {
      if( fHits[i].nhits > 1 ) 
	{
	  fHits[i].flag = 1;

	  // also flag the previous point layer
	  if(fHits[i].flag == 0) 
	    fHits[i].flag = 1;
	  continue;
	}
    }
  
  // copy remaining points to graph 
  //iFirst = -1;
  //iLast  = 0;
  
  int largehitgroup = 0;
  
  // try to find a start a reasonable start
  iFirst = -1;
  for(size_t i=0;i<fHits.size();i++)
    {
      if(fHits[i].flag != 0 ) 
	{
	  // crude shower detection
	  if(fHits[i].nhits > 3) largehitgroup++;
	  if(largehitgroup > 20 ) 
	    {
	      iFirst = i;
	      break;	      
	    }
	  continue;
	}
      
      // pick up the first good layer and quit
      iFirst = i;
      break;
    }
  // why?
  if( iFirst == -1 ) iFirst = 0;

  // try to find a reasonable end
  iLast = -1;
  largehitgroup = 0;
  for(size_t i=fHits.size()-1;i>=0;i--)
    {
      if(fHits[i].flag != 0 ) 
	{
	  // crude shower detection
	  if(fHits[i].nhits > 3) largehitgroup++;
	  if(largehitgroup > 20 ) 
	    {
	      iLast = i;
	      break;	      
	    }
	  continue;
	}
      
      // pick up the first good layer and quit
      iLast = i;
      break;
    }
  if( iLast == -1 ) iLast = (int)fHits.size()-1;

  //std::cout<<"iFirst && iLast : "<<iFirst<<", "<<iLast<<std::endl;
  
  // in-between
  int counter = 0;
  for(int i=iFirst;i<=iLast;i++)
    {
      if(fHits[i].flag != 0) continue;
      fGrIn->SetPoint( fGrIn->GetN(), fHits[i].x, fHits[i].hits[0]->GetD() );
      counter++;
    }

  // if tagged more than 80% of all points
  // just give up and copy everything
  if( float(counter)/fHits.size() < 0.2 )
    {
      std::cerr<<"ERROR: TrackBuilderSm::MakeTrack failed. "
	       <<"Probably a perpendicular track.   "
	       <<"Number of single ch hits "<< counter
	       <<" out of "<<fHits.size()<<" hits "<<std::endl;
      
      //return all hits as part of the track
      iFirst = 0; 
      iLast  = (int)(fHits.size()) - 1;
      BuildTrack(tracks, -1); 
    }
  else
    {
      // try to smooth it
      RunSmoother();
  
      // build the track
      BuildTrack(tracks, 0);  
    }
}


//
//
//
void TrackBuilderSm::RunSmoother()
{
  if( fSmType == 1 )
    fGrOut = fGrSmooth->SmoothSuper(fGrIn,"",fBass,fSpan);
  else
    fGrOut = fGrSmooth->SmoothLowess(fGrIn,"",fSpan);
  
  // calculate min distance for all the hit points
  for(size_t i=0;i<fHits.size();i++)
    {
      
      double xval = fHits[i].x;
      double yprd = fGrOut->Eval(xval);
      double tang = GetTangent( xval );

      // save the nominal value
      fHits[i].y  = yprd;

      // find and save the point with minimal distance
      for(size_t j=0;j<fHits[i].hits.size();j++)
	{
	  double yval = fHits[i].hits[j]->GetD();
	  double dist = DistanceToLine(0, yval, yprd, tang);
	  if( dist < fHits[i].mindis )
	    {
	      fHits[i].mindis = dist;
	      fHits[i].isave = (int)j;
	    }
	}
      
    }
  

  // loop over the central part
  int maxgap     = 0;
  for(int i=iFirst;i<iLast;i++)
    {  
      // always accepts the hits in the middle
      // assuming we did a good job with clustering
      Accept(fHits[i]);
      
      if( i > iFirst )
	{
	  int chgap = fHits[i].ch - fHits[i-1].ch;

	  if(chgap < 0)  // channels are in the increasing order (hopefully?)
	    {
	      std::cerr<<"ERROR: "<<__FILE__<<", "<<__LINE__
		       <<" The channel ordering appears to be invalid"<<std::endl;
	    }
	  
	  if( chgap > maxgap ) maxgap = chgap;
	}
    }
  
  //if( maxgap > 1 ) 
  //std::cout<<" Max channel gap of "<<maxgap<<" channels detected"<<std::endl;
      
  // track ends are tricky as there could be electron showers from stopping muons
  // a primitive way is to step back and forward until large 
  // number of missing hits are encountered
  int gapcounter;

  // step backward
  gapcounter = 0;
  for(int i=iFirst-1;i>=0;i--)
    {
      if( !Accept(fHits[i]) ) gapcounter++;
      else gapcounter = 0;

      if( gapcounter > fNGapStep ) // give up
	{
	  iFirst = i + gapcounter;
	  break; 
	}
    }

  // step forward
  gapcounter = 0;
  for(int i=iLast;i<(int)fHits.size();i++)
    {
      if( !Accept(fHits[i]) ) gapcounter++;
      else gapcounter = 0;

      if( gapcounter > fNGapStep ) // give up
	{
	  iLast = i - gapcounter; 
	  break;      
	}
    }
  

}


//
// track builder
//
void TrackBuilderSm::BuildTrack( std::vector<LArReco::Track2d> &tracks, 
				int statusflag )
{
  // track path
  std::vector<TVector2> trkpath; 
  std::vector<double> dqdx;  
  
  // 
  std::vector<const LArReco::QHit*> trkhits;
  std::vector<const LArReco::QHit*> trkdrhits;

  //for(size_t i=0;i<fHits.size();i++)
  for(int i=iFirst;i<=iLast;i++)
    {
      if(statusflag == -1) //just add everything
	{
	  double xval = fHits[i].x;
	  double yval = fHits[i].hits[0]->GetD();
	  double qtot = 0;
	  for(size_t j=0;j<fHits[i].hits.size();j++)
	    {
	      trkhits.push_back( fHits[i].hits[j] );
	      qtot += fHits[i].hits[j]->GetQ();
	    }
	  trkpath.push_back( TVector2(xval, yval) );
	  dqdx.push_back( qtot );
	}
      else // be more selective
	{
	  //if(fHits[i].flag == 0) 
	  //{

	  // add up all the charge for now
	  int ihit = fHits[i].isave;
	  double xval = fHits[i].x;
	  double yval = fHits[i].hits[ihit]->GetD();
	  //use interpolated if asked and positive
	  if( fSmooth && fHits[i].y >= 0 ) 
	    yval = fHits[i].y; 
	      
	  // add only charge associated with track
	  //double Q    = fHits[i].hits[ihit]->GetQ();

	  // add all the charge on this hit ch as part of track
	  double Q    = 0;
	  for(size_t j=0;j<fHits[i].hits.size();j++)
	    {
	      Q += fHits[i].hits[j]->GetQ();
	    }
	      
	  trkpath.push_back( TVector2(xval, yval ) );
	  dqdx.push_back( Q );
	  trkhits.push_back( fHits[i].hits[ihit] );

	  // move the rest as hits tagged due to delta ray
	  for(size_t j=0;j<fHits[i].hits.size();j++)
	    {
	      if( (int)j == fHits[i].isave) continue;

	      trkdrhits.push_back( fHits[i].hits[j] );
	    }
	  //}
	  
	  //else //just tag everything as delta rays
	    //{
	  // copy hits
	  //for(size_t j=0;j<fHits[i].hits.size();j++)
	  //{
	  //trkdrhits.push_back( fHits[i].hits[j] );
	  //}
	  
	}
    }

  // could not do it, give up
  if(trkhits.empty()) return;

  
  // calculate initial and final slopes
  size_t minpts = 20;
  double tan0, tan0_err;
  double tan1, tan1_err;
  RecoUtils::LineTangent( trkpath, 0, minpts, tan0, tan0_err );
  
  int nstart = trkpath.size() - minpts;
  if(nstart > 0)
    {
      RecoUtils::LineTangent( trkpath, (size_t)nstart, minpts, tan1, tan1_err );
    }
  else
    {
      tan1     = tan0;
      tan1_err = tan0_err;
    }
  
  // make a new track
  tracks.push_back( LArReco::Track2d( trkpath, dqdx, trkhits ) );
  tracks.back().SetSlopeInit( tan0, tan0_err );
  tracks.back().SetSlopeFinal( tan1, tan1_err );

  // add delta ray hits if any
  if(!trkdrhits.empty())
    tracks.back().AddDeltaRays( trkdrhits );
}


//
// distance of point xp, yp to line y = a + b*x
//
double TrackBuilderSm::DistanceToLine(double x0, double y0, double a, double b)
{

  // point where the normal and original lines intercept
  float x1 = (x0 + b*y0 - a*b)/(b*b + 1);
  float y1 = b*x1 + a;
  
  float d = sqrt((x1 - x0)*(x1 - x0) + (y1 - y0)*(y1 - y0));
  
  if(d != d) 
    {
      std::cerr<<"ERROR: TrackBuilderSm::DistanceToLine() "<<std::endl;
      std::cerr<<"      "<<x0<<", "<<y0<<", "<<a<<", "<<b<<std::endl;
      return 0;
    }

  return d;
}


//
// calculate slope near the point
//
double TrackBuilderSm::GetTangent(double xval)
{
  //
  double dstp = 1.0; //cm 
  
  double y0 = fGrOut->Eval(xval - 0.5*dstp);
  double y1 = fGrOut->Eval(xval + 0.5*dstp);
  
  return (y1 - y0)/dstp;
}


//
//
//
bool TrackBuilderSm::Accept(ChHits &ch)
{
  
  if( ch.mindis < fMaxD ) // accept the point
    ch.flag = 0;
  else
    {
      // single hits on ch will be accepted even if greater threshold distance
      if( ch.flag != 0)
	{
	  // try to raise the treshold
	  if( ch.mindis < 1.5 * fMaxD )
	    {
	      ch.flag = 0; //accept
	    }
	  else //skip this channel altogether
	    {
	      ch.flag = 2; // make it 2 in case need more cut levels
	    }
	}
    } // else

  return (ch.flag == 0);  
}
