//////////////////////////////////////////////////////////////////////////////
// 
//   Classes to do some basic processing on the raw data for each channel
//   (e.g., pedestal finder)
//   
//   Created: Mon May 11 11:03:06 CEST 2015
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __PROCESSRAWCHDATA_H__
#define __PROCESSRAWCHDATA_H__

#include <algorithm>
#include <iostream>
#include <vector>
#include <utility>
#include <cmath>
#include <string>

#include <TClonesArray.h>

#include "DataChan.h"

// 
// class to find pedestal and subdivide channel data into sections with peaks
// zero suppress anything below a certain threshold 
//

class RawChDataROI 
{
 public:
  RawChDataROI();
  ~RawChDataROI();

  size_t FindROI( LArRawData::DataChan *chdata, 
		  std::vector< LArRawData::DataFragment<Float_t> > &aFrags, 
		  float thresh_guess, bool padroi );

  size_t FindROI( LArRawData::DataChan *chdata, 
		  std::vector< LArRawData::DataFragment<Float_t> > &aFrags, 
		  float ped, float pedrms, bool padroi );
  
  // find regions of interest in the data
  template<typename T>
    size_t FindROI( std::vector<T> &data, float thresh, float lowbound, 
		    std::vector< std::pair<size_t, size_t> > &subsamples,
		    std::vector< size_t > &pedsamples, float &minval, bool padroi );

  template<typename T>
    size_t FindROI( std::vector<T> &data, float thresh_guess,
		    std::vector< std::pair<size_t, size_t> > &subsamples, bool padroi );

  //
  float  GetPed(){ return m_ped; }
  float  GetPedRMS(){ return m_pedrms; }
  float  GetNSigTh(){ return m_nsigth; }
  float  GetNSigLow1(){ return m_nsiglow1; }
  float  GetNSigLow2(){ return m_nsiglow2; }
  size_t GetNPadRight(){ return m_padright;}
  size_t GetNPadLeft(){ return m_padleft; }
  
  //
  void SetNSigTh(float val){ m_nsigth = val; }
  void SetNSigLow1(float val){ m_nsiglow1 = val; }
  void SetNSigLow2(float val){ m_nsiglow2 = val; }
  void SetNPadRight(size_t val) { m_padright = val; }
  void SetNPadLeft(size_t val){ m_padleft = val; }  
  
 protected:
  // find pedestal with out-ROI samples
  template<typename T>
    void FindPed( std::vector<T> &data, std::vector< size_t > &pedsamples,
		  float &ped, float &pedrms, float baseline = 0 );
  
  //
  void PadROI( std::vector< std::pair<size_t, size_t> > &rois, 
	       size_t padleft, size_t padright, size_t maxsize );
  
  float m_ped;       // pedestal
  float m_pedrms;    // pedstal rms
  float m_nsigth;    // threshold : m_nsigth x ped_rms + ped
  float m_nsiglow1;  // lowbound  : ped - m_nsiglow1 x ped_rms
  float m_nsiglow2;  // lowbound  : ped - m_nsiglow2 x ped_rms
  size_t m_padright; // pad ROI on the right
  size_t m_padleft;  // pad ROI on the left
  size_t m_nminped;  // min number of pedestal samples
};


//
// find regions of interest with a guess of threshold and lowbound on waveform
//
template<typename T> 
inline size_t RawChDataROI::FindROI( std::vector<T> &data, float thresh, float lowbound,
				     std::vector< std::pair<size_t, size_t> > &subsamples,
				     std::vector< size_t > &pedsamples, float &minval, bool padroi )
{
  subsamples.clear();
  pedsamples.clear();
  
  minval = 9999;

  for(size_t i=0;i<data.size();i++)
    {
      pedsamples.push_back(i);
      float dflt = (float)data[i];
      if( dflt < minval)
	minval = dflt;

      if(dflt > thresh)
	{
	  size_t strt_idx = i;
	  
	  // step forward
	  size_t end_idx = strt_idx;
	  for(;end_idx<data.size();end_idx++)
	    {
	      float dval = (float)data[end_idx];
	      if(dval < minval)
		minval = dval;
	      
	      if(dval < lowbound) break;
	    }
	  
	  // update loop counter
	  i = end_idx;

	  //now step back
	  for(;strt_idx!=0;strt_idx--)
	    {
	      float dval = (float)data[strt_idx];
	      if( dval < minval)
		minval = dval;

	      if(dval < lowbound) break;
	      pedsamples.pop_back(); //remove from ped sample
	    }
	  
	  if(strt_idx < end_idx)
	    subsamples.push_back(std::make_pair(strt_idx, end_idx));
	  
	}
    } 

  if(padroi)
    {
      // pad the data
      PadROI(subsamples, m_padleft, m_padright, data.size());
    }
    
  return subsamples.size();
}

//
// find regions of interest with some treshold guess
//
template<typename T> 
inline size_t RawChDataROI::FindROI( std::vector<T> &data, float thresh_guess, 
				     std::vector< std::pair<size_t, size_t> > &subsamples, bool padroi )
{
  m_nminped = data.size()/10;
  
  float thresh = thresh_guess;
  int  np      = 0;
  float Mean   = 0;
  float Sum    = 0;
  float SumSq  = 0;
  float K      = 0; 
  bool  Kinit  = false;
  
  size_t npresample = 100;
  if(npresample > data.size()) 
    npresample = data.size(); 

  float minval =  99999;
  float maxval = -99999;
  float avgval =  0.0;
  
  for(size_t i=0;i<npresample;i++)
    {
      float dval = (float)data[i];
      if(dval > maxval) maxval = dval;
      if(dval < minval) minval = dval;
      avgval += dval;
    }

  avgval /= npresample;
  if(thresh < minval) // try to improve on the first guess
    {
      float bestguess = minval;      
      if(maxval / minval > 5)
	bestguess +=  0.3 * (maxval - minval);
      else if ( maxval / minval < 1)
	bestguess +=  2*maxval;
      else
	bestguess +=  0.5 * (maxval - minval);
      
      std::cout<<"WARNING: Threshold guess appears to be smaller than the min value "<<minval
	       <<" in the first "<<npresample<<" samples "<<std::endl;
      std::cout<<"         Will update the threshold guess from "<<thresh<<" to "<<bestguess<<std::endl;
      thresh = bestguess;
    }

  // first guess for pedestal
  for(size_t i=0;i<data.size();i++)
    {
      float dval = (float)data[i];
      if(dval >= thresh) continue;
      
      if( !Kinit ) 
	{
	  K     = dval;
	  Kinit = true;
	}
      
      Mean += dval;
      // use method of assumed mean to get variance
      float tmp = dval - K;
      Sum   += tmp;
      SumSq += tmp * tmp;
      np++;
    }
  
  // our first guess for the pedestal
  m_ped    = Mean / np;
  m_pedrms = sqrt( (SumSq - (Sum * Sum) / np)/(np - 1) );

  // update the guess for the threshold
  thresh = m_ped + m_nsigth * m_pedrms;
  if(thresh > thresh_guess) thresh = thresh_guess;

  // get estimate for lower bound
  float lowbound = m_ped - m_nsiglow1 * m_pedrms;
  std::vector< size_t > pedsamples;
  size_t rval = FindROI(data, thresh, lowbound, subsamples, pedsamples, minval, true);

  if( pedsamples.size() < m_nminped && minval > lowbound) // lowbound is too low
    {
      lowbound = minval + 1;
      rval = FindROI(data, thresh, lowbound, subsamples, pedsamples, minval, true);
    }
  // update pedestal calcuation
  if(pedsamples.size() < m_nminped)
    {
      std::cout<<"WARNING: Number of pedestal samples appears to be low. Please check ..."<<std::endl;
      std::cout<<"         the number of pedestal samples is "<<pedsamples.size()<<std::endl;
    }
  
  // refine pedestal and rms calculation excluding found ROI
  FindPed(data, pedsamples, m_ped, m_pedrms);

  // Update our calculation of ROI
  thresh    = m_ped + m_nsigth * m_pedrms;
  lowbound  = m_ped - m_nsiglow2 * m_pedrms;

  if(m_pedrms < 0.1)
    {
      thresh   = m_ped + 2;
      lowbound = m_ped + 0.5;
    }

  
  return FindROI(data, thresh, lowbound, subsamples, pedsamples, minval, padroi);
}



//
// Compute pedestal and ped rms in a single-pass using Welford's algorithm
//
template<typename T>
inline void RawChDataROI::FindPed( std::vector<T> &data, 
				   std::vector< size_t > &pedsamples,
				   float &ped, float &pedrms, float baseline )
{
  /* int   np    = 0; */
  /* float Mean  = 0; */
  /* float Sum   = 0; */
  /* float SumSq = 0; */
  /* float K     = data[pedsamples[0]];  */
  /* for(size_t i=0;i<pedsamples.size();i++) */
  /*   { */
  /*     size_t idx = pedsamples[i]; */
  /*     float datum = data[idx] - baseline; */
  /*     Mean  += datum; */
  /*     // use method of assumed mean to get variance */
  /*     float tmp = datum - K; */
  /*     Sum   += tmp; */
  /*     SumSq += tmp * tmp; */
  /*     np++; */
  /*   } */
  
  /* ped    = Mean / np; // ok, np should be = pedsamples.size() */
  /* pedrms = sqrt( (SumSq - (Sum * Sum) / np)/(np - 1) ); */

  ped    = 0;
  pedrms = 0;
  if( pedsamples.size() < 1 )
    return;
  else if(pedsamples.size() < 2)
    {
      ped = data[pedsamples[0]];
      return;
    }

  double A = 0;
  double Q = 0;
  for(size_t i=0;i<pedsamples.size();i++)
    {
      double d  = ((double)data[ pedsamples[i] ] - baseline);
      double Ak = A + (d - A)/(i+1);
      double Qk = Q + (d - Ak)*(d-A);
      A = Ak;
      Q = Qk;
    }
  ped     = A;
  pedrms  = sqrt( Q/(pedsamples.size()-1) );
}


#endif
