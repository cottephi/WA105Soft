//////////////////////////////////////////////////////////////////////////////
//
//  utility to handle a spare matrix as list of lists
// 
//
//
//////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>


#include "LILUtil.h"

using namespace std;

//
//
//
LILUtil::LILUtil()
{
  fNbins = 0;
  fXBinFirst = 0;
  fXBinLast  = 0;
  fXFirst = 0;
  fXLast  = 0;
  fXPitch = 0;

  fLocked = false;
}


//
//
//
LILUtil::~LILUtil()
{;}


//
// new configuration
//
void LILUtil::Init( double x0, double xstep, 
		    int xidx_start, int xidx_end )
{
  fLocked = false;

  
  fXBinFirst = xidx_start;
  fXBinLast  = xidx_end;

  // assumes we start from bin count from 0
  fNbins     = fXBinLast - fXBinFirst + 1;
  
  if( fNbins < 0 )
    {
      std::swap( fXBinFirst, fXBinLast );
      fNbins = -fNbins;      
    }
  
  
  fLIL.clear();
  fLIL.resize( fNbins );
  
  fXPitch = xstep;
  fXFirst = x0;
  fXLast  = fXFirst + fNbins * fXPitch;
}


//
// we assume the data is sorted in increasing x 
// and for each x in increasing y
int LILUtil::Build( std::vector<double> xdata, std::vector<double> ydata )
{
  if( fLocked ) 
    {
      cout<<"ERROR: configuration is locked"<<endl;
      return -1;
    }

  //
  for(size_t i=0;i<xdata.size();i++)
    {
      int bin = FindBinNum( xdata[i] );

      if( bin < 0 ) 
	{
	  //cout<<xdata[i]<<" "<<fXFirst<<" "<<fXLast<<" "<<fNbins<<endl;
	  cout<<"WARNING: LILUtil cannot is not capable of storing all data with current configurations"<<endl;
	}
      if( fLIL[bin].empty() ) fLIL[bin].push_back( make_pair((int)i, ydata[i]) );
      else
	{
	  double last = fLIL[bin].back().second;
	  if( last > ydata[i] ) 
	    {
	      cerr<<"ERROR: the data does not appear to be sorted correctly"<<endl;
	      cerr<<"     "<<bin<<" "<<xdata[i]<<" "<<last<<" "<<ydata[i]<<endl;
	      continue;
	    }
	  
	  fLIL[bin].push_back( make_pair((int)i, ydata[i]) );
	}
    }
  
  fLocked = true;
  
  return 0;
}


//
//
//
int LILUtil::FindBinNum( double x )
{
  if( x < fXFirst || x >= fXLast ) return -1;
  
  // add a small offset to avoid rounding off errors
  return (int)( (x - fXFirst)/(fXPitch) + 0.05);
}


//
//
//
int LILUtil::GetN( double x )
{
  int bin = FindBinNum( x );
  if ( bin < 0 ) return 0;
  
  return (int)fLIL[bin].size();
}

//
//
//
int LILUtil::GetN( int xbin )
{
  if( xbin < fXBinFirst || xbin > fXBinLast ) return 0;
  
  return (int)fLIL[xbin - fXBinFirst].size();
}


//
//
//
int LILUtil::Find( double x, double y, double ytol, std::vector<int> &res)
{
  res.clear();
  int bin = FindBinNum( x );
  if( bin < 0 ) return -1;
  return FindFromBin( bin, y, ytol, res );
}


//
//
//
int LILUtil::Find( int xbin, double y, double ytol,
		   std::vector<int> &res )
{
  res.clear();
  int bin = (xbin - fXBinFirst);
  if( bin < 0 || bin >= fNbins ) return -1;
  
  return FindFromBin( bin, y, ytol, res );
}

//
//
//
int LILUtil::FindClosest( int xbin, double y, double ytol )
{
  vector<int> res;
  int bin = (xbin - fXBinFirst);
  if( bin < 0 || bin >= fNbins ) return -1;

  vector< pair<int, double> > yd = fLIL[ bin ];
  
  if( yd.empty() ) return -1;

  double ylow = y - ytol;
  double yhi  = y + ytol;
  
  vector< pair<int, double> >::iterator it;

  if( ylow < yd[0].second ) 
    {
      ylow = yd[0].second;
      it = yd.begin();    
    }
  else
    {
      it = std::upper_bound( yd.begin(), yd.end(), std::make_pair(-1, ylow), CompFunc);
      if( it == yd.end() ) 
	{
	  return -1;
	}
    }
  
  double mind = 1.0E+6;
  int idx = -1;
  for(;it!=yd.end();it++)
    {
      if( it->second > yhi ) break;
      double d = fabs(it->second - y);
      if( d < mind )
	{
	  mind = d;
	  idx  = it->first;
	}
    }

  
  return idx;
}


//
//
//
int LILUtil::FindFromBin( int bin, double y, double ytol,
			  std::vector<int> &res )
{
  vector< pair<int, double> > yd = fLIL[ bin ];

  if( yd.empty() ) return 0;

  double ylow = y - ytol;
  double yhi  = y + ytol;
  
  vector< pair<int, double> >::iterator it;

  if( ylow < yd[0].second ) 
    {
      ylow = yd[0].second;
      it = yd.begin();    
    }
  else
    {
      it = std::upper_bound( yd.begin(), yd.end(), std::make_pair(-1, ylow), CompFunc);
      if( it == yd.end() ) return 0;
    }
  
  for(;it!=yd.end();it++)
    {
      if( it->second < yhi ) res.push_back( it->first );
      else break;
    }


  return (int)res.size();
}


//
//
//
void LILUtil::Print()
{
  for(size_t i=0;i<fLIL.size();i++)
    {
      cout<<setw(5)<<i+fXBinFirst
	  <<setw(10)<<fXFirst+i*fXPitch;
      for( size_t j=0;j<fLIL[i].size();j++ )
	cout<<setw(5)<<j<<setw(5)<<fLIL[i][j].first<<setw(10)<<fLIL[i][j].second;
      cout<<endl;
    }
}
