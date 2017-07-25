/////////////////////////////////////////////////////////////////////////////////
//
//  Hough transform accumulator to search for track segments
// 
//  Created: Fri Aug 28 16:03:34 CEST 2015  
//
//  Performs a transform of two dimensional coordinates x, y into HT space 
//  of alpha, rho to find linear segments and associated delta ray clusters
//
//  Transform:
//      rho = x cos(alpha) + y sin(alpha)  
// 
//  The slope of the line and intercept are given by 
//        -cos(alpha) / sin(alpha) (slope)
//         rho / sin(alpha)        (intercept)      
//  
//
/////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <cmath>

#include <TMath.h>

#include "HoughTransform.h"

using namespace std;



///////////////////////////////////////////////////////////////////////////////
//
// ctor
//
HoughTransform::HoughTransform(float xmax, float ymax, 
			       float xpitch, float ypitch, int sbinx, int sbiny)
{
  fXPitch = xpitch;
  fYPitch = ypitch;

  float dx = sbinx*fXPitch;
  float dy = sbiny*fYPitch;

  // rho axis range
  fRhoMin =  -sqrt( xmax*xmax + ymax*ymax);
  fRhoMax =  -fRhoMin; //sqrt( xmax*xmax + ymax*ymax);
  float rhobin = sqrt(dx*dx + dy*dy);
  // number of bins (could also set via SetRhoBinWidth() afterwards)
  fRhoNb = (int)((fRhoMax - fRhoMin) / rhobin);
  
  // alpha axis range
  fAlphaMin = -0.5*TMath::Pi();
  fAlphaMax =  0.5*TMath::Pi();
  // alpha bin size (could also set via SetAlphaBinWidth() afterwards)
  float alphabin = 0.1*TMath::Pi()/180; ////dy / fRhoMax;
  fAlphaNb  = (int)((fAlphaMax - fAlphaMin) / alphabin);
  
  cout<<"Pitch : "<<fXPitch<<", "<<fYPitch<<endl;
  cout<<"Bins  : "<<dx<<", "<<dy<<endl;
  cout<<"Rho : "<<fRhoMax<<", "<<rhobin<<", "<<fRhoNb<<endl;
  cout<<"Alpha : "<<alphabin<<", "<<fAlphaNb<<endl;
  
  fAlphaxis = NULL;
  fRhoaxis  = NULL;
  
  // build HT bins
  BuildAxes();
  
  //
  // parameters for track segment reconstruction
  // set via appropriate "set" methods
  //

  // flag to use hit positions (change with SetUseHitPos) in hit objects
  // otherwise HT will be calculated with scale factors provided
  fUseHitPos = false;

  // bins to sum near maxima
  fSumalpbin = 2;
  fSumrhobin = 1;

  //max ticks for merging into a single hit block
  fMultiHitBlock  = 1;  
  
  //max distance in ticks to seed a delta ray
  fDRayMaxTdcDist = 2; 

  // min number of hit channels in track
  fMinTrackSegHits = 20; 
  fHitOverlapRatio = 0.2; 
  
  // max number of missing ch to signify break in track
  fMaxChGap = 1;

  fTolNsgm  = 1.0;
}


///////////////////////////////////////////////////////////////////////////////
//
// dtor
//
HoughTransform::~HoughTransform()
{
  delete fAlphaxis;
  delete fRhoaxis;

  //while(!fHTbins.empty())
  //{
  //delete fHTbins.back();
  //fHTbins.pop_back();
  //}
  fHTbins.clear();
}


///////////////////////////////////////////////////////////////////////////////
//
// build our alpha - rho bins
//
void HoughTransform::BuildAxes()
{
  // bin total
  fTotBins = fAlphaNb * fRhoNb;

  if( fTotBins > MAXHTACCUBINS) // too many bins?
    {
      cout<<"WARNING: "<<__FILE__<<", "<<__LINE__<<" too many bins in HT"<<endl;
      cout<<"         reducing number of rho bins from "<<fRhoNb<<" to "
	  <<MAXHTACCUBINS / fAlphaNb <<endl;
      
      fRhoNb = MAXHTACCUBINS / fAlphaNb;
      
    }

  if(fAlphaxis) fAlphaxis->Delete();
  if(fRhoaxis) fRhoaxis->Delete();
  //
  fAlphaxis = new TAxis(fAlphaNb, fAlphaMin, fAlphaMax);
  fRhoaxis  = new TAxis(fRhoNb, fRhoMin, fRhoMax);
  
  size_t start = fHTbins.size();
  for(size_t i=start;i<(size_t)fTotBins;i++)
    {
      HT_Accum htbin;
      htbin.PointsUsed = 0;
      htbin.BinMask    = false;
      fHTbins.push_back(htbin);
      //fHTbins.push_back(new HTAccumBin());
    }
}

size_t HoughTransform::GetBinValue(HT_Accum &htbin)
{
  if(htbin.Points.empty()) return 0;
  if(htbin.BinMask) return 0;
  if(htbin.PointsUsed > htbin.Points.size()) return 0;
  
  return (size_t)(htbin.Points.size() - htbin.PointsUsed);
}


//
// set width for alpha bins in radians
//
void HoughTransform::SetAlphaBinWidth(float dalpha_rad)
{
  fAlphaNb = (int)((fAlphaMax - fAlphaMin) / dalpha_rad);
  BuildAxes();
}

//
// set width for rho bins in mm
//
void HoughTransform::SetRhoBinWidth(float drho_mm)
{
  fRhoNb = (int)((fRhoMax - fRhoMin) / drho_mm);
  BuildAxes();
}


//
// build track segment clusters
//
void HoughTransform::BuildClusters(const HitCollection* hcoll, std::vector<HitCollection*> &hitclus)
{
  int nhitstot  = hcoll->GetNhits();
  if ( nhitstot >= (int)(pow(2, sizeof(htindex)*8))) 
    {
      cerr<<"ERROR: "<<__FILE__<<", "<<__LINE__<<" number of hits in the cluster is too large "<<endl;
      cerr<<"        If this is not a bug, need to increase the number of bytes for htindex type "<<endl;
      return;
    }
  
  //
  fDataVec.clear();

  HT_HitPoint aPoint;
  ofstream out("test.dat");
  out<<"Cluster"<<endl;
  const std::deque<ChHitCollection*> hits = hcoll->GetHits();
  for(size_t i=0;i<hits.size();i++)
    {
      const std::vector<const LArReco::QHit*> chhits = hits[i]->GetHits();
      for(size_t j=0;j<chhits.size();j++)
	{
	  const LArReco::QHit* hit = chhits[j];
	
	  aPoint.ch        = hit->GetCh();
	  aPoint.chhitidx  = j;
	  aPoint.starttdc  = hit->GetTbStart();
	  aPoint.endtdc    = hit->GetTbEnd();
	  aPoint.itracks   = 0;
	  
	  out<<setw(5)<<aPoint.ch<<setw(10)<<aPoint.starttdc<<setw(10)<<aPoint.endtdc<<endl;
	  
	  if(fUseHitPos)
	    {
	      aPoint.x = hit->GetX();
	      aPoint.y = hit->GetD();
	    }
	  else
	    {
	      aPoint.x = aPoint.ch * fXPitch;
	      aPoint.y = aPoint.starttdc * fYPitch;
	    }
	  
	  fDataVec.push_back(aPoint);
	}
    }


  // now build HT
  Transform();

  // now search for line elements
  int nclushits = 0;

  // loop and get all maxima in HT
  while( nclushits < 3*nhitstot ) //inflate to account for double-counted hits
    {
      float alpha, rho;
      std::deque<htindex> linepts;
      int rval = GetGlobalMax(alpha, rho, linepts);

      if(!rval)
	break;
      
      //
      float a, b, res;
      int nhits = GetLinePoints( linepts, a, b, res );
      
      // subtract these from HT
      SubDataPts( linepts );
      
      // short segment
      if(nhits<fMinTrackSegHits) 
	break;
      
      //cout<<" found "<<nhits<<" in a track"<<endl;

      nclushits += nhits;
      
      out<<"Track "<<setw(10)<<a<<setw(10)<<b<<setw(10)<<res<<endl;
      
      // build track cluster
      hitclus.push_back(new HitCollection());
      HitCollection *aClus = hitclus.back();
      for(size_t p=0;p<linepts.size();p++)
	{
	  aPoint = fDataVec[linepts[p]];
	  out<<setw(5)<<aPoint.ch<<setw(10)<<aPoint.starttdc<<setw(10)<<aPoint.endtdc<<endl;
	  const std::vector<const LArReco::QHit*> chhits = hcoll->GetChHits(fDataVec[linepts[p]].ch)->GetHits();
	  aClus->AddHit( chhits[fDataVec[linepts[p]].chhitidx] );
	}
      
      ///// TESTING
      if( hitclus.size() == 6) break;
    }
  out.close();
  
  cout<<"Clustered "<<nclushits<<" out of "<<hcoll->GetNhits()<<" in "<<hitclus.size()<<" tracks"<<endl;
}

//
// Reset
//
void HoughTransform::Reset()
{
  for(size_t i=0;i<fHTbins.size();i++)
    {
      //fHTbins[i]->Clear();
      fHTbins[i].Points.clear();
      fHTbins[i].PointsUsed = 0;
      fHTbins[i].BinMask = 0;;
    }

  fHTbinmap.clear();
}


//
// Calculated HT for the Hits
//
void HoughTransform::Transform()
{
  //
  Reset();
  fHTbinmap.resize( fDataVec.size() );

  for(size_t i=0;i<fDataVec.size();i++) //loop over all the points
    {
      for(int abin=1;abin<=fAlphaNb;abin++) // loop over all alpha
	{
	  // compute transform of a given point
	  float alpha = fAlphaxis->GetBinCenter(abin);
	  float rho   = fDataVec[i].x * cos(alpha) + fDataVec[i].y * sin(alpha);
	  
	  if(rho < fRhoMin || rho > fRhoMax) 
	    {
	      cout<<"WARNING: "<<__FILE__<<", "<<__LINE__<<" rho value "<<rho
		  <<" is out of bounds "<<endl;
	      cout<<"        This should not happen. So could be a bug"<<endl;
	      cout<<"        "<<alpha<<", "<< fDataVec[i].x<<", "<<fDataVec[i].y
		  <<"        "<<fDataVec[i].ch<<", "<<fDataVec[i].starttdc<<endl;
	      continue;
	    }
	  
	  // rho bin
	  int rbin = fRhoaxis->FindBin(rho);
	  
	  // 1D bin number (subtract 1 since array goes from 0)
	  int nbin = (rbin-1) *  fAlphaNb + (abin-1);
  
	  if(nbin < 0) // why should this be?
	    {
	      cerr<<"ERROR: "<<__FILE__<<", "<<__LINE__
		  <<" bin index should not be negative"<<endl;
	      continue;
	    }
	  
	  // append to the bin
	  //fHTbins[nbin]->Add((htindex)i);
	  fHTbins[nbin].Points.push_back((htindex)i);
	  fHTbinmap[i].push_back(nbin);
	}
    }
}



//
// Get global maximum
//
int HoughTransform::GetGlobalMax( float &alpha, float &rho, std::deque<htindex> &linepts )
{
  alpha = 0;
  rho   = 0;
  linepts.clear();
  
  // find max bin
  int maxbin, maxval;
  maxbin = GetMaxBin(maxval);
  
  if(maxbin < 0)
    return 0;
    
  if(maxval < fMinTrackSegHits)
    return 0;
  
  
  // add nearby points 
  AddBins( maxbin, fSumalpbin, fSumrhobin,  alpha, rho, 
	   (int)0.5*maxval, linepts, true );

  return maxval;
}


//
// Get HT bin with max value
//
int HoughTransform::GetMaxBin(int &maxval)
{
  size_t maxcount = 0;
  size_t maxbin   = 0;

  for(size_t i=0;i<fHTbins.size();i++)
    {
      //size_t bval = fHTbins[i]->GetBinValue();
      size_t bval = GetBinValue(fHTbins[i]);
      if(bval == 0) continue;

      if(maxcount < bval)
	{
	  maxcount = bval;
	  maxbin   = i;
	}

    }
  
  maxval = (int)maxcount;
  //cout<<"maximum "<<maxcount<<" "<<maxbin<<endl;
  if(maxcount == 0) // failed to find maximum
    return -1;
  
  return (int)maxbin;
}

//
// make a sum with nearby bins of a given HT phase-space neighbourhood
//
void HoughTransform::AddBins( int binn, int halfnx, int halfny, float alpha, float rho, 
			      int mincut, std::deque<htindex> &datapts, bool binmask )
{
  datapts.clear();

  // add all the data points in the given bin
  
  int abin, rbin;
  GetAlphaRhoBins(binn, abin, rbin);
  
  int binlowx  = abin - halfnx;
  if(binlowx < 0) binlowx = 0;  
  int binhighx = abin + halfnx;
  if(binhighx >= fAlphaNb) binhighx = fAlphaNb-1;
  
  int binlowy = rbin - halfny;
  if(binlowy < 0) binlowy = 0;
  int binhighy = rbin + halfny;
  if(binhighy >= fRhoNb) binhighy = fRhoNb-1;
  
  for(int i=binlowy;i<=binhighy;i++)
    {
      for(int j=binlowx;j<=binhighx;j++)
	{
	  int bin1d = j + i*fAlphaNb;

	  size_t bval = GetBinValue(fHTbins[bin1d]);
	  if(bval < (size_t)mincut) continue;
	  
	  fHTbins[bin1d].BinMask = binmask;
	  std::vector<htindex> pnts = fHTbins[bin1d].Points;
	  for(size_t k=0;k<pnts.size();k++)
	    AddHTIndexPoint(datapts, pnts[k]);
	}
    }
}


//
// subtract contribution from other HT bins
//
void HoughTransform::SubDataPts(std::deque<htindex> &datapts)
{
  for(size_t i=0;i<datapts.size();i++)
    {
      int idx = datapts[i];
      
      for(size_t j=0;j<fHTbinmap[idx].size();j++)
	{
	  int bin = fHTbinmap[idx][j];
	  //fHTbins[bin]->UpUsedCount();
	  fHTbins[bin].PointsUsed++;;
	}

    }
}


//
// get actual values of alpha and rho
//
void HoughTransform::GetAlphaRho(int bina, int binrho, float &alpha, float &rho)
{  
  alpha = fAlphaxis->GetBinCenter(bina);
  rho   = fRhoaxis->GetBinCenter(binrho);
 
  return;
}

//
// go from 1D bin to 2D bin numbers
//
void HoughTransform::GetAlphaRhoBins(int binn, int &bina, int &binrho)
{
  bina   = binn % fAlphaNb;
  binrho = (binn - bina) / fAlphaNb;
      
  return;
}


//
// Calculate line parameters using LSQ. These are returned by reference
//   b - slope in tdc/ch num
//   a - intercept with x axis (in ch num units)
//       the return value is an average residual
// If several points with same ch number are found the one with smallest  
// residual is taken
// Attempt to expand the line by looking into nearby points
//
int HoughTransform::GetLinePoints(std::deque<htindex> &datapts, float &a, float &b, float &resval)
{
  //
  std::deque<htindex> fdatapts;     // filtered data pts
  std::deque<float> residuals;      // residuals 

  
  // get line parametesr from our first guess
  // use only the points without any previous track association -> itracks == 0 
  int maxseg;
  resval = GetLinePara( datapts, a, b, maxseg, 0 ); 
  if(resval < 0) return 0;

  cout<<"Starting MSE "<<resval<<" "<<a<<" "<<b<<endl;

  // check if we have just random hits
  //if(maxseg < fMinTrackSegHits && 
  //(fDataVec[datapts.back()].ch - fDataVec[datapts.front()].ch) > fMinTrackSegHits)
  //{
  //return 0;      
  //}
  

  /*
  // loop over data pts if multi pts on a given ch 
  for(size_t i=0;i<datapts.size();i++)
    {
      
      int ch = fDataVec[datapts[i]].ch;
      // ch number
      float x = (float)(ch);
      // tdc
      float y = (float)(fDataVec[datapts[i]].starttdc);
      //
      float r = (y - (a + b*x))*(y - (a + b*x));
      
      // first point
      if(fdatapts.empty())
	{
	  fdatapts.push_back(datapts[i]);
	  residuals.push_back(r);
	  continue;
	}
	 
      int lastch = fDataVec[fdatapts.back()].ch;
      // require only one group of connected ch hits
      if(lastch == ch)
	{
	  // remove if new residuals are smaller
	  if( r < residuals.back() )
	    {
	      //cout<<"-> "<<ch<<" "<<lastch<<" "<<r<<" "<<residuals.back()<<endl;
	      residuals.pop_back();
	      fdatapts.pop_back();

	      if(fdatapts.empty())
		{
		  fdatapts.push_back(datapts[i]);
		  residuals.push_back(r);
		  continue;
		}
	    }
	  else continue;
	}
      
      // require continuous channel blocks for track segments 
      if(( ch - lastch ) > 1 )
	{
	  // try to fill this gap

	  int chgap  = 0;
	  
	  for(int j = lastch+1;j<ch;j++)
	    {
	      float tol = floor(resval+0.5); 
	      
	      // y = slope ( x - x0 ) + y0
	      float tval = b * (j - lastch) + fDataVec[fdatapts.back()].starttdc;
	      int idx;
	      float rval = FindChHit(fdatapts.back()+1, datapts[i]-1, j, tval, idx);
	      
	      if(rval <= fTolNsgm*tol)
		{
		  residuals.push_back(rval);
		  fdatapts.push_back(idx);
		}
	      else
		chgap++;
	      
	      // break if gap is too large
	      if(chgap >= fMaxChGap) break;
	    }
	      
	  if(chgap >= fMaxChGap ) 
	    {
	      if((int)fdatapts.size() < (int)(fMinTrackSegHits))
		{
		  fdatapts.clear();
		  residuals.clear();
		}
	      // finish with this track segment
	      else if ((int)(datapts.size()-i) < (int)(fMinTrackSegHits))
		break;
	    }
	}	  
      
      // otherwise buisness as usual
      fdatapts.push_back(datapts[i]);
      residuals.push_back(r);
    }
  */
  fdatapts = datapts;
  // refit and attempt to expand to lower channels
  resval = GetLinePara( fdatapts, a, b, maxseg, -1);
  /*
  // attempt to expand hits to the lower channels
  int lastch   = fDataVec[fdatapts.front()].ch;
  int chgap    = 0;
  
  for(int j = lastch-1;j>=fDataVec[0].ch;j--)
    {
      // our residuals so far
      float tol  = resval;
      float tval = b * (j - fDataVec[fdatapts.front()].ch) + fDataVec[fdatapts.front()].starttdc;
      int idx;
      float rval = FindChHit(fdatapts.front()-1, 0, j, tval, idx);
      if(rval <= tol) fdatapts.push_front(idx);
      else chgap++;
      
      // break if gap is too large
      if(chgap >= fMaxChGap) break;
    }

  
  
  // attempt to expand hits to higher channels
  lastch   = fDataVec[fdatapts.back()].ch;
  chgap    = 0;
  
  for(int j = lastch+1;j<=fDataVec.back().ch;j++)
    {
      // our residuals so far
      float tol = resval;
      float tval = b * (j - fDataVec[fdatapts.back()].ch) + fDataVec[fdatapts.back()].starttdc;
      int idx;
      float rval = FindChHit(fdatapts.back()+1, fDataVec.size()-1, j, tval, idx);
      if(rval <= tol) fdatapts.push_back(idx);
      else chgap++;
      
      // break if gap is too large
      if(chgap >= fMaxChGap) break;
    }
  
  */  
  int tmp1 = 0;
  int tmp2 = 0;
  for(size_t i=0;i<fdatapts.size();i++)
    {
      int val = fDataVec[fdatapts[i]].itracks;
      if(val == 0) tmp1++;
      tmp2++;
      fDataVec[fdatapts[i]].itracks++;
    }
  
  float fracoverlap = (float)(tmp1)/tmp2;
  cout<<fracoverlap<<endl;
  //if(fracoverlap < (1-fHitOverlapRatio)) return 0;
    
  // calculate final linear parameters and residuals
  if( fdatapts.size()>0)
    {
      resval = GetLinePara( fdatapts, a, b, maxseg, -1);
      cout<<"Final MSE: "<<resval<<" "<<a<<" "<<b<<endl;
    }
  // assign our new points
  datapts = fdatapts;

  return (int)fdatapts.size();
}


//
// Line least squares fit y = a + bx
//
float HoughTransform::GetLinePara(std::deque<htindex> &datapts, float &a, float &b, 
				  int &maxseg, int itrack_cut)
{
  
  std::deque<htindex> fdatapts;
  
  // various sums
  double sumx  = 0;
  double sumy  = 0;
  double sumxy = 0;
  double sumx2 = 0;
  double sumy2 = 0;
  
  //loop over data pts
  int nseg = 0;
  maxseg = 0;
  int lastch = -1;
  for(size_t i=0;i<datapts.size();i++)
    {
      if(itrack_cut >= 0 && fDataVec[datapts[i]].itracks > itrack_cut)
	continue;
      
      // ch number
      int ch   = fDataVec[datapts[i]].ch;
      double x = (double)(ch);
      // tdc
      double y = (double)(fDataVec[datapts[i]].starttdc);
      
      sumx  += x;
      sumy  += y;
      sumxy += x*y;
      sumx2 += x*x;
      sumy2 += y*y;
      
      if(lastch == -1)
	{
	  lastch = ch;
	  nseg++;
	}
      else if( (ch-lastch) == 1 )
	{
	  nseg++;
	  if(nseg>maxseg)
	    maxseg = nseg;
	}
      else if( (ch-lastch)>1 )
	{
	  if(nseg > maxseg)
	    maxseg = nseg;
	  nseg = 0;
	}

      //cout<<lastch<<" "<<fDataVec[datapts.back()].ch<<" "<<nseg<<" "<<maxseg<<endl;

      fdatapts.push_back(datapts[i]);
      lastch = fDataVec[fdatapts.back()].ch;      

    }
  
  //datapts  = fdatapts;
  size_t n = fdatapts.size();
  float rval = -999;
  a = -999;
  b = -999;
  
  if(fdatapts.size() <= 2) 
    return rval; //cannot calculate the fit
    
  // intercept
  a = (sumy*sumx2 - sumx*sumxy) / (n*sumx2 - sumx*sumx);
  // slope
  b = (n*sumxy - sumx*sumy) / (n*sumx2 - sumx*sumx);

  rval = GetLinFitMSE(a, b, fdatapts);
  
  return rval;
}


//
// FindChHitBlock 
//
float HoughTransform::FindChHit(htindex startidx, htindex endidx, int ch, float tval, int &idx)
{
  int inc = (endidx > startidx)?1:-1;


  int  didx = -1;
  bool foundch = false;
  float minres = 1.0E+6;
  if(startidx == endidx )
    {
      if( fDataVec[startidx].ch == ch )
	{
	  foundch = true;
	  float r = fabs(tval - fDataVec[startidx].starttdc);
	  minres = r;
	  didx   = startidx;
	}
    }
  else
    {
      while(startidx != endidx)
	{
	  if( fDataVec[startidx].ch == ch )
	    {
	      foundch = true;
	      float r = fabs(tval - fDataVec[startidx].starttdc);
	      if(r < minres)
		{
		  minres = r;
		  didx   = startidx;
		}
	    }
	  else
	    {
	      if(foundch) break; //we went past it
	    }
      
	  startidx += inc;
	}
    }
  
  // this should happen
  if(startidx == endidx && !foundch)
    {
      cout<<"WARNING: "<<__FILE__<<", "<<__LINE__<<" :  "
	  <<"could not find ch hit "<<ch<<". Must be a bug"<<endl;
      idx = -1;
      return 1.0E+6;
    }
  
  //
  idx = didx;
  
  return minres;
}


//
//
//
float HoughTransform::GetLinFitMSE(float p0, float p1, std::deque<htindex> &dpnts)
{
  if(dpnts.size() <= 2) return -999.0;
  
  float rval = 0.0;
  
  for(size_t i=0;i<dpnts.size();i++)
    {
      // ch number
      float x = (float)(fDataVec[dpnts[i]].ch);
      // tdc
      float y = (float)(fDataVec[dpnts[i]].starttdc);
      // this residual
      float r = y - (p0 + p1*x);
      
      rval += r*r;
      
      //cout<<x<<" "<<y<<" "<<sqrt(r*r)<<endl;
    }

  // estimate the uncertainty for the data points
  rval = sqrt(rval / (dpnts.size() - 2));
  return rval;
}


//
//
//
void HoughTransform::AddHTIndexPoint(std::deque<htindex>& datapts, htindex val)
{
 if(datapts.empty())
    {
      datapts.push_back(val);
      return;
    }
  
  if(val > datapts.back()) 
    {
      datapts.push_back(val);
      return;
    }
  
  if(val < datapts.front())
    {
      datapts.push_front(val);
      return;
    }
  
  // otherwise need to search for this 
  std::deque<htindex>::iterator found = std::find(datapts.begin(), datapts.end(), val);
  
  if( found == datapts.end())
    {
      // this is rather expensive, since all 
      // entries will have to be reshuffled 
      datapts.insert(std::lower_bound(datapts.begin(), datapts.end(), val), val);
    }

}
