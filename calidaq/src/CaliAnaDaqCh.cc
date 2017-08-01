//////////////////////////////////////////////////////////////////////////////
//
//  Base class for analysis of calibration data from dlardaq
//
//
//  Created: Fri Nov 18 16:15:55 CET 2016
//
//////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "CaliAnaDaqCh.h"

//
//
//
caliana::CaliAnaDaqCh::CaliAnaDaqCh(size_t ch_id)
{
  m_chid = ch_id;
  m_nres = 0;
}

//
//
//
caliana::CaliAnaDaqCh::~CaliAnaDaqCh()
{;}



//
//
//
void caliana::CaliAnaDaqCh::Reset()
{
  for(size_t i=0;i<m_nres;i++)
    m_evres[i].clear();
}


//
//
//
void caliana::CaliAnaDaqCh::GetResults( std::map< std::string, std::pair< double, double > > &res )
{
  res.clear();

  for(size_t i=0;i<m_nres;i++)
    {
      float mean, rms;
      if(i==1){FindMeanAndRms( m_evres[i], mean, rms, true );}
      else{FindMeanAndRms( m_evres[i], mean, rms, false );}
      res[ m_keys[i] ] = std::make_pair( mean, rms );
    }
}
