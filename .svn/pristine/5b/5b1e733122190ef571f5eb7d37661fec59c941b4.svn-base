//////////////////////////////////////////////////////////////////////
// 
//  Join track fragments between different CRMs
//  The joining right now is done brute force, by looping over all CRMs
//
//  TODO: improve the efficiency for merging tracks by including 
//        some basic geometry considerations
//
//////////////////////////////////////////////////////////////////////

#include <iostream>
#include <algorithm>

#include "TrackJoinCRM.h"

using namespace std;

//
//
//
TrackJoinCRM::TrackJoinCRM(RecoConfig &recopara)
{
  std::vector<float> params = recopara.GetValue("PARA_TRKJOIN");
  if( params.empty() ) fNoJoin = true;
  else
    {
      //
      fMinDist = params[0]; // min distance to look for merging trajectories in cm
      //fMaxDist = params[1]; // max distance to look for mergin trajectories in cm
      fNSgmTan = params[1]; // number if standard deviations on the track angle
      
      //if(fMinDist > fMaxDist) std::swap( fMinDist, fMaxDist );
      
      fNoJoin = false;
      if( fMinDist < 0 ) fNoJoin = true;
    }
  //
  //fStepDist = (fMaxDist - fMinDist)/3.0;
}


//
//
//
TrackJoinCRM::~TrackJoinCRM()
{;}


//
// Find if two intervals overlap. Interval in format: x +/- half width
//
bool TrackJoinCRM::IntervalOverlap(double x0, double hlfw0, double x1, double hlfw1)
{
  return ( fabs( x1 - x0 ) < (hlfw0 + hlfw1) );
}

//
// distance between points
//
double TrackJoinCRM::Distance(const TVector2 &pnt1, const TVector2 &pnt2)
{
  double d = (pnt1.X()-pnt2.X())*(pnt1.X()-pnt2.X()) + (pnt1.Y()-pnt2.Y())*(pnt1.Y()-pnt2.Y());
  return sqrt(d);
}


//
// join
//
void TrackJoinCRM::Join(  std::vector< std::vector<LArReco::Track2d> > &tracksegs,
			  std::vector< LArReco::Track2d > &tracks )
{
  
  if(fNoJoin || tracksegs.size() == 1)
    {
      // just merge all segments into one array
      for(size_t i=0;i<tracksegs.size();i++)
	{
	  tracks.insert( tracks.end(), tracksegs[i].begin(), tracksegs[i].end() );
	}
      return;
    }
  
  
  //cout<<"doing merging ..."<<endl;
  
  size_t ncrm = tracksegs.size();
  for(size_t i = 0;i<ncrm;++i)
    {
      // loop over tracks in this CRM
      for( size_t ii = 0;ii<tracksegs[i].size();++ii )
	{
	  // loop over other CRMs
	  if( tracksegs[i][ii].GetPid() != 11 )
	    {
	      size_t rep  = 0;
	      size_t skip = 0;
	      while( rep < (ncrm - i) )
		{
		  bool joined = false;
		  for(size_t j = i+1;j<ncrm;++j)
		    {
		      if(j == skip) continue;
		      if( FindSegment( tracksegs[i][ii], tracksegs[j], fMinDist, fNSgmTan ) > 0 )
			{
			  skip   = j;
			  joined = true;
			  break;
			}
		    }
		  if( !joined ) rep = ncrm; // break from while loop
		  else rep++;
		}
	    }
	  tracks.push_back( tracksegs[i][ii] );
	} // loop over track segments
    } // loop over CRMs
  
  return;
}


//
// assumes track points are ordered in time
//
bool TrackJoinCRM::IsJoinable( LArReco::Track2d &trk1, LArReco::Track2d &trk2, 
			       double sepdist, double tanstd )
{
  LArReco::Track2d aTrk1 = trk1;
  LArReco::Track2d aTrk2 = trk2;
  
  //
  if( aTrk1.GetEnd().Y() > aTrk2.GetEnd().Y() )
    {
      aTrk1 = trk2;
      aTrk2 = trk1;
    }
  
  // not overlapping?
  if( aTrk1.GetStart().Y() > aTrk2.GetStart().Y() ) 
    return false;
  
  // not overlapping?
  if( aTrk1.GetEnd().Y() > aTrk2.GetStart().Y() ) 
    return false;
  
  double dmin =  Distance( aTrk1.GetEnd(), aTrk2.GetStart() );
  if( dmin > sepdist ) return false;


  double tan1    = aTrk1.GetSlopeFinal();
  double tan1err = aTrk1.GetSlopeFinalErr();

  double tan2    = aTrk2.GetSlopeInit();
  double tan2err = aTrk2.GetSlopeInitErr();

  // have the same direction?
  if( !IntervalOverlap( tan1, tanstd*tan1err, tan2, tanstd*tan2err ) ) return false;

  //
  //
  aTrk1 += aTrk2;
  aTrk1.SetCRM(-1);
  
  trk1 = aTrk1;
  trk2 = aTrk2;

  return true;
}

//
//  assumes track points are ordered in time
//
int TrackJoinCRM::FindSegment( LArReco::Track2d &trk, 
			       std::vector<LArReco::Track2d> &trks_other, 
			       double sepdist, double tanstd )
{
  std::vector<LArReco::Track2d>::iterator it = trks_other.begin();
  
  int rval = 0;
  for(;it!=trks_other.end();)
    {
      if( IsJoinable( trk, *it, sepdist, tanstd ) )
	{
	  trks_other.erase( it );
	  rval = 1;
	  break;
	}
      it++;
    }

  return rval;
}
		 
