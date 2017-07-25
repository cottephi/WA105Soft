//////////////////////////////////////////////////////////////////////////////
// 
//  Base class for analysis of calibration data from dlardaq
//
//
//  Created: Fri Nov 18 16:15:55 CET 2016
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __CALIANADAQCH_H__
#define __CALIANADAQCH_H__

#include <sstream>
#include <utility>
#include <string>
#include <map>

#include <TCanvas.h>
#include <TDirectory.h>

#include "dlardaq.h"

namespace caliana
{
  //
  //
  //
  class CaliAnaDaqCh
  {
  public:
    CaliAnaDaqCh(size_t ch_id);
    ~CaliAnaDaqCh();

    // get channel id
    size_t GetCh() const { return m_chid; }
    
    //
    //virtual void SetParameters( std::vector<float> &pars ) = 0;
    void ProcessEvent( size_t evid, dlardaq::adc16_t* adc, size_t nsz)
    {
      m_evid = evid;
      Fill( adc, nsz );
    }

    // show data for debug
    virtual void Show( TCanvas *cnvs, TDirectory *dirout, std::string opt ) = 0;
    
    // get results of the calibrations
    virtual void GetResults( std::map< std::string, std::pair< double, double > > &res );
    
    // execute a reset 
    virtual void Reset();
        

  protected:
    // fill with data
    virtual void Fill(dlardaq::adc16_t* adc, size_t nsz) = 0; 

    // possible results
    enum DataList { PED = 0, PEDRMS = 1,
		    AMPL = 2, PSUM = 3, TMAX = 4 };
    
    // number of results
    size_t m_nres;
    
    // ch id
    size_t m_chid;
    
    //
    size_t m_evid;

    // event data for this channel
    // represent data internally as floats
    std::vector<float> m_data;

    //
    std::vector<std::string> m_keys;           // different keys
    std::vector< std::vector<float> > m_evres; // event results
    
    
    //
    template< class T >
      void FindMeanAndRms( std::vector<T> &data, float &mean, float &rms, float chi2cut = 0 );
    
    // remove outliers
    template< class T >
      size_t RemoveOutliers( std::vector<T> &data, 
			     float mean, float var, float chi2cut );
  };

  // calcluate mean and RMS from a vector of data samples
  // using Welford's method to ensure numerical stability
  // if chi2cut > 2.71 try to remove outliers
  template< class T >
    inline void CaliAnaDaqCh::FindMeanAndRms( std::vector<T> &data, 
					      float &mean, float &rms, float chi2cut )
    {
        mean = 0;
	rms  = 0;
	
	if(data.size()<=1) return;
	
	double A = 0;
	double Q = 0;
	for(size_t i=0;i<data.size();i++)
	  {
	    //cout<<data[i]<<endl;
	    double d  = (double)data[i];
	    double Ak = A + (d - A)/(i+1);
	    double Qk = Q + (d - Ak)*(d-A);
	    A = Ak;
	    Q = Qk;
	  }
	mean = A;
	rms  = sqrt( Q/(data.size()-1) );
	
	// try to remove outliers
	// chi2cut cut should be reasonably high though
	if( chi2cut > 2.71 ) 
	  {
	    int cloop = 0;
	    while( cloop < 3 ) 
	      {
		size_t rval = RemoveOutliers( data, mean, rms, chi2cut );
		FindMeanAndRms( data, mean, rms, 0);
		if( rval == 0 ) break;
		cloop++;
	      }
	  }
    }

  // remove outliers
  template< class T >
    inline size_t CaliAnaDaqCh::RemoveOutliers( std::vector<T> &data, 
						float mean, float var, float chi2cut )
    {
      if(chi2cut <= 0) 
	return 0;
      
      float C = 1./(2*var*var);
      
      typename std::vector<T>::iterator it = data.begin();
      size_t rval = 0;  
      while(it!=data.end())
	{
	  float d2 = C * ((*it - mean)*(*it - mean));
	  if( d2 > chi2cut )
	    {
	      it = data.erase( it );
	      rval++;
	    }
	  else it++;
	}
      
      return rval;
    }
};

#endif
