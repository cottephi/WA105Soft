#include <algorithm>
#include <iostream>
#include <iomanip>
#include <cmath>

#include <TMath.h>

#include "Track2d.h"
#include "RecoUtils.h"

using namespace std;

// truncated mean 
//   rmlow  - fraction of events to remove at the low part of the distribution
//   rmhigh - fraction of events to remove at the high part of the distribution 
//
double RecoUtils::TruncatedMean( const std::vector<double> &vec, double rmlow, double rmhigh )
{
  if(vec.empty()) return -999;
  
  std::vector<double> vtmp(vec);
  
  // sort it
  std::sort(vtmp.begin(), vtmp.end());
  
  size_t n = vtmp.size();
  size_t nlow, nhigh;

  nlow  = (size_t)(rmlow * n);
  nhigh = n - (size_t)(rmhigh * n);

  // some basic checks
  if(rmlow <= 0 || rmlow >= 1) nlow = 0;
  if(rmhigh <= 0 || rmhigh >= 1) nhigh = n;

  if( (rmlow + rmhigh) >= 0.99)
    {
      cerr<<"ERROR: RecoUtils::TruncatedMean() the fractions to remove are to high"<<endl;
      nlow  = 0;
      nhigh = n;
    }

  //cout<<"Subsample "<<nlow<<", "<<nhigh<<" out of "<<n<<endl;
  
  // should not happen though
  if( nlow >= n ) nlow = 0;
  if( nhigh >= n ) nhigh = n;
  

  double trmean = 0.0;

  for(size_t i=nlow;i<nhigh;i++)
    trmean += vtmp[i];
  
  trmean /= ((float)nhigh - (float)nlow);
  return trmean;
}

// fit line LSQ 
// returns value of correlation coefficient: the closer to 1 the better
// params[0] +/- params_err[0] - constant 
// params[1] +/- params_err[1] - tangent
// serr - estimator for variance
double RecoUtils::FitLineLSQ( double *xdata, double *ydata, int np, 
			      std::vector<double> &params, 
			      std::vector<double> &params_err, double &serr )
{
  params.clear();
  params_err.clear();
  
  
  if( np < 3 ) return -999;
  
  // various sums
  double sumx  = 0;
  double sumy  = 0;
  double sumxy = 0;
  double sumx2 = 0;
  double sumy2 = 0;
  
  for(int i=0;i<np;i++)
    {
      double x = xdata[i];
      double y = ydata[i];
      
      sumx  += x;
      sumy  += y;
      sumxy += x*y;
      sumx2 += x*x;
      sumy2 += y*y;
    }


  // intercept
  double a = (sumy*sumx2 - sumx*sumxy) / (np*sumx2 - sumx*sumx);
  // slope
  double b = (np*sumxy - sumx*sumy) / (np*sumx2 - sumx*sumx);
  
  // calculate covariance and error estimations
  // following formulae from Wolfram

  double ss_xx = sumx2 - sumx*sumx / np;
  double ss_yy = sumy2 - sumy*sumy / np;
  double ss_xy = sumxy - sumx*sumy / np;
  
  // compute correlation coefficient
  double rval = (ss_xy*ss_xy)/(ss_xx*ss_yy);

  // compute error estimator
  //double s = sqrt( (ss_yy - b*ss_xy) / (np - 2) );
  double s = (ss_yy - b*ss_xy) / (np - 2);
  if( s < 0 ) s = fabs(s);
  s = sqrt( s );
  
  // estimated error on fitted parameters
  double err_a = s * sqrt(1.0/np + (sumx*sumx/np/np)/ss_xx);
  double err_b = s / sqrt( ss_xx );
  
  params.resize( 2 );
  params_err.resize( 2 );
  
  params[0] = a;
  params[1] = b;
  params_err[0] = err_a;
  params_err[1] = err_b;
  
  serr = s;
  
  return rval;
}

// calculate tangent to line using defined number of points
double RecoUtils::LineTangent( const std::vector<TVector2> &vec, size_t nstart, size_t np,
			       double &tanth, double &tanth_err)
{
  tanth     = -999;
  tanth_err = -999;

  std::vector<double> xdata, ydata;
  std::vector<double> pars, pars_err;
  double serr;
  
  // cannot compute slope for short segments
  if(vec.size()<3) return -999;

  if( nstart >= vec.size()-3 ) nstart = 0;
  if( nstart + np >= vec.size() ) np = vec.size() - nstart;
  
  for(size_t i=nstart;i<nstart+np;i++)
    {
      xdata.push_back( vec[i].X() );
      ydata.push_back( vec[i].Y() );
    }

  double rval = RecoUtils::FitLineLSQ(&xdata[0], &ydata[0], xdata.size(), pars, pars_err, serr);
  tanth     = pars[1];
  tanth_err = pars_err[1];
  return rval;
}



//
//  basic function to match reonstructed tracks between views
//
size_t RecoUtils::ViewTrackMatch( const TObjArray *trks0, const TObjArray *trks1,
				  std::vector< std::pair<int, int> > &mpairs, 
				  double maxdist, double maxdelq )
{
  mpairs.clear();

  // track parallel to view (handle elsewhere) ...
  if(trks0->GetEntries() < 1 || trks1->GetEntries() < 1) return 0;
  
  LArReco::Track2d* trk0 = NULL;
  LArReco::Track2d* trk1 = NULL;

  std::vector<int> trkused;
  trkused.resize( trks1->GetEntries(), 0 );
  float mindist;
  int   idx;

  for(int i=0;i<trks0->GetEntries();i++)
    {
      trk0 = (LArReco::Track2d*)trks0->At(i);

      // skip electrons (delta rays)
      if(trk0->GetPid() == 11) continue;
      
      double di_0 = trk0->GetStart().Y();
      double df_0 = trk0->GetEnd().Y();
      
      // get total charge for this track
      double q0 = trk0->GetQ(); 
      
      //
      bool idirstate = (di_0 > df_0);
      
      mindist = 1.0E+9;
      idx     = -1;
      for(int j=0;j<trks1->GetEntries();j++)
	{
	  trk1 = (LArReco::Track2d*)trks1->At(j);

	  // skip electrons (delta rays)
	  if(trk1->GetPid() == 11) continue;
	  
	  //
	  double di_1 = trk1->GetStart().Y();
	  double df_1 = trk1->GetEnd().Y();
	  
	  double q1 = trk1->GetQ();
	  double aQ = fabs(q0 - q1)/(q0 + q1);
	  
	  bool jdirstate = (di_1 > df_1);
	  if( jdirstate != idirstate )
	    {
	      std::swap( di_1, df_1 );
	    }
	  
	  double d1 = fabs(di_0 - di_1);
	  double d2 = fabs(df_0 - df_1);
	  if( d1<= maxdist && d2<= maxdist && aQ < maxdelq )
	    {
	      double d = d1 + d2; 
	      if( d < mindist ) 
		{
		  mindist = d;
		  idx = j;
		}
	    }
	}
      if( idx >=0 )
	{
	  if(trkused[idx] == 0)
	    {
	      mpairs.push_back( make_pair( i, idx ) );
	      trkused[idx] = 1;
	      trk1 = (LArReco::Track2d*)trks1->At(idx);
	    }
	  //else cout<<" already used "<<mindist<<endl;
	}
      //else cout<<" no match "<<endl;
    }
  
  return mpairs.size();

  // //
  // for(int i=0;i<trks0->GetEntries();i++)
  //   {
  //     trk0 = (LArReco::Track2d*)trks0->At(i);
  //     double di_0 = trk0->GetStart().Y();
  //     double df_0 = trk0->GetEnd().Y();

  //     bool idirstate = (di_0 > df_0);
  //     for(int j=0;j<trks1->GetEntries();j++)
  // 	{
  // 	  trk1 = (LArReco::Track2d*)trks1->At(j);
  // 	  double di_1 = trk1->GetStart().Y();
  // 	  double df_1 = trk1->GetEnd().Y();

  // 	  bool jdirstate = (di_1 > df_1);
  // 	  if( jdirstate != idirstate )
  // 	    std::swap( di_1, df_1 );
	  
  // 	  if( fabs(di_0 - di_1) <= maxdist && fabs(df_0 - df_1) <= maxdist )
  // 	    {
  // 	      mpairs.push_back( make_pair( i, j ) );
  // 	      break;
  // 	    }
	  
  // 	}
  //   }
  
  // return mpairs.size();
}

//
// inputs: slopes measured in each view
//
void RecoUtils::FindThetaAndPhi( const double slp_view0, const double slp_view1, 
				 double &theta, double &phi )
{
  // !! NOTE we are assuming cathode is at positive drift distance !!
  // !!      and anode is at 0                                     !!
  double sx = -1*slp_view0;
  double sy = -1*slp_view1;

  phi = TMath::ATan(sx/sy);
  
  double cphi = cos(phi);
  double sphi = sin(phi);
  
  theta = (cphi + sphi)/((sx+sy)*cphi*sphi);
  theta = TMath::ATan( theta );  
  if(theta < 0) theta = 0.5*TMath::Pi() - theta;
}



/// line segment

//
//
//
RecoUtils::LineSegment::LineSegment(){;}


//
// 
//
RecoUtils::LineSegment::~LineSegment(){;}

//
// set points
//
void RecoUtils::LineSegment::SetPoints(std::vector<TVector2> &vec)
{
  fAbcissa.clear();
  fOrdinata.clear();
  fTangent.clear();
  
  fNp = vec.size();

  if(fNp < 3 ) 
    {
      std::cerr<<"ERROR: LineSegment::SetPoints() too few points in the array"<<std::endl;
      return;
    }

  fMinX =  1.0E+6;
  fMaxX = -1.0E+6;

  fMinY =  1.0E+6;
  fMaxY = -1.0E+6;
  
  fAbcissa.resize( fNp );
  fOrdinata.resize( fNp );
  fTangent.resize( fNp );
  
  int itneg = 0;
  int itpos = 0;

  for(size_t i=0;i<fNp;i++)
    {
      double xval = vec[i].X();
      double yval = vec[i].Y();

      if( xval > fMaxX ) fMaxX = xval;
      else if(xval < fMinX ) fMinX = xval;
      
      if( yval > fMaxY ) fMaxY = yval;
      else if(yval < fMinY ) fMinY = yval;
    
      fAbcissa[i]  = xval;
      fOrdinata[i] = yval;
      
      if(i > 0 && i<fNp-1)
	{
	  double tang = (fOrdinata[i+1] - fOrdinata[i-1])/(fAbcissa[i+1] - fAbcissa[i-1]);
	  if(tang>=0) itpos++;
	  else itneg++;
	  fTangent[i] = tang;
	}
    }

  if(itpos > itneg) fSlope = 1;
  else fSlope = -1;

  // set last points
  fTangent[0] = (fOrdinata[1] - fOrdinata[0])/(fAbcissa[1] - fAbcissa[0]);
  fTangent[fNp - 1] = (fOrdinata[fNp-1] - fOrdinata[fNp-2])/(fAbcissa[fNp-1] - fAbcissa[fNp-2]);
}

//
// set points
//
void RecoUtils::LineSegment::SetPoints(std::vector<double> &x, std::vector<double> &y)
{
  fAbcissa.clear();
  fOrdinata.clear();
  fTangent.clear();
  
  fNp = x.size();

  if(fNp < 3 ) 
    {
      std::cerr<<"ERROR: LineSegment::SetPoints() too few points in the array"<<std::endl;
      return;
    }

  fMinX =  1.0E+6;
  fMaxX = -1.0E+6;

  fMinY =  1.0E+6;
  fMaxY = -1.0E+6;
  
  fAbcissa.resize( fNp );
  fOrdinata.resize( fNp );
  fTangent.resize( fNp );
  
  int itneg = 0;
  int itpos = 0;
  
  for(size_t i=0;i<fNp;i++)
    {
      double xval = x[i];
      double yval = y[i];

      if( xval > fMaxX ) fMaxX = xval;
      if( xval < fMinX ) fMinX = xval;
      
      if( yval > fMaxY ) fMaxY = yval;
      if( yval < fMinY ) fMinY = yval;
    
      fAbcissa[i]  = xval;
      fOrdinata[i] = yval;
      
      if(i > 0)
	{
	  double tang = (y[i] - y[i-1])/(x[i] - x[i-1]);
	  fTangent[i-1] = tang;
	  if(tang>=0) itpos++;
	  else itneg++;
	}
    }
  
  if(itpos > itneg) fSlope = 1;
  else fSlope = -1;
  
  // set last point
  fTangent[fNp - 1] = (fOrdinata[fNp-1] - fOrdinata[fNp-2])/(fAbcissa[fNp-1] - fAbcissa[fNp-2]);
  
  /*
  for(size_t i=0;i<fAbcissa.size();i++)
    {
      cout<<setw(5)<<i
	  <<setw(15)<<fAbcissa[i]
	  <<setw(15)<<fOrdinata[i]
	  <<setw(15)<<fTangent[i]<<endl;
    }
  */
}


//
// Line least squares fit y = a + bx
//
double RecoUtils::LineSegment::LinearFit(size_t ifirst, size_t ilast, 
					 double &a, double &b, double &a_err, double &b_err)
{
  if( ifirst > fNp ) ifirst = 0;
  if( ilast  > fNp || ilast == 0 ) ilast = fNp-1;
  
  std::vector<double> params, params_err;
  double serr;
  //for(size_t i=ifirst;i<=ilast;i++)
  //cout<<fAbcissa[i]<<" "<<fOrdinata[i]<<endl;

  double r2 = FitLineLSQ( &fAbcissa[ifirst], &fOrdinata[ifirst], 
			  (int)(ilast - ifirst), params, params_err, serr );
  
  a = params[0];
  b = params[1];
  
  a_err = params_err[0];
  b_err = params_err[1];

  return r2;
}

//
// get tangent at a given point if np > 0 average nearby points
//
double RecoUtils::LineSegment::GetTangent( size_t idx, int np )
{
  size_t iPnt = idx;
  
  if( idx >= fNp ) iPnt = fNp - 1;
  
  if( np == 0 ) return fTangent[iPnt];
  
  //calculate average
  int istart = (int)(iPnt) - np;
  int iend   = (int)(iPnt) + np;

  if(istart < 0) 
    {
      istart = 0;
      if( iend + np < (int)fNp ) iend += np;
    }
  if(iend >= (int)fNp)
    {
      iend = (fNp-1);
      if( istart - np >= 0 ) istart -= np;
    }
  
  int ntot = iend - istart + 1;
  double avg = 0;
  for(int i=istart;i<=iend;i++)
    {
      avg += fTangent[i];
    }
  avg /= ntot;
  
  return avg;
}

//
// 0 - extrapolated, 1 - normal use
//
int RecoUtils::LineSegment::GetXandTan(double y, int np, int &startpnt, double &x, double &slope)
{
  
  if( y < fMinY || y > fMaxY ) //need to extrapolate
    {
      //use first 10 pnts by default or np if > 10
      size_t n = 10; 
      if( np > (int)n ) n = (size_t)np;
      if( n >= fNp ) n = fNp - 1;
      
      
      size_t istart, iend;
      if((y < fMinY && fSlope > 0) || (y > fMaxY && fSlope < 1))
	{
	  istart = 0;
	  iend   = n;
	}
      else
	{
	  if( ((int)fNp - (int)n - 1) < 0 ) istart = 0;
	  else istart = fNp - n - 1; 
	  
	  iend = fNp-1;
	}
      
      double a, a_err, b, b_err;
      LinearFit(istart, iend, a, b, a_err, b_err);
      
      if( fabs( b ) < 1.0E-3 )
	x = fAbcissa[0];
      else x = (y - a)/b;
      
      slope = b;

      return 0;
    }

  // this should not happen though
  if(startpnt >= (int) fNp ) startpnt = (int)fNp - 1;

  //
  //cout<<startpnt<<" -> "<<endl;
  x = Interpolate( y, startpnt );
  //cout<<startpnt<<" <- "<<endl;
  slope = GetTangent( (size_t)(startpnt), np );
		      
  return 1;
}


//
// assume that the our data was sorted for sure x, 
// but this may not necessarily be true for all y 
// ( although they should be in increasing order to large degree )
//
double RecoUtils::LineSegment::Interpolate( double y, int &startpnt )
{
  if(startpnt < 0)
    {
      // find the first value which above the reference
      for(size_t i=0;i<fOrdinata.size();i++)
	{
	  if( fSlope > 0 )
	    {
	      if( fOrdinata[i] >= y ) 
		{
		  startpnt = (int)i - 1;
		  if(startpnt < 0 ) 
		    startpnt = 0;
		  break;
		}
	    }
	  else
	    {
	      if( fOrdinata[i] <= y ) 
		{
		  startpnt = (int)i - 1;
		  if(startpnt < 0 ) 
		    startpnt = 0;
		  break;
		}
	    }
	}
      if( startpnt < 0 ) 
	{
  	  std::cerr<<"ERROR: RecoUtils::LineSegment::Interpolate() this should not happen "<<std::endl;
  	  std::cerr<<"       Counld not find "<<y<<" in range "<<fMinY<<", "<<fMaxY<<std::endl;
  	  return -999;
  	}
    }
  else
    {
      double ylow = fOrdinata[ startpnt ];
      
      int istate = ( ylow < y ) ? 1:0;
      // always move index forward
      for(int i=startpnt+1;i<(int)fNp-1;i++)
	{
	  int cstate = ( fOrdinata[i] < y ) ? 1:0;
	  if(cstate != istate )
	    break;	      
	  
	  startpnt = i;
	}
    }
  
  // just do a linear interpolation
  int ilow = startpnt;
  int ihi  = startpnt + 1;
 
  double x0 = fAbcissa[ilow];
  double x1 = fAbcissa[ihi];
  double y0 = fOrdinata[ilow];
  double y1 = fOrdinata[ihi];
  
  double x = x0 + (x1 - x0)*(y - y0)/(y1 - y0);
  
  return x;
}
