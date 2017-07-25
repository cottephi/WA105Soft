//////////////////////////////////////////////////////////////////////////////
// 
//  Calibration information for each channel base class
//
//  Convert the 3-fold key (CRM, view, chan) complexity into a single index.
//  This is done by mapping bits into 32bit integer
//  Current mapping:
//      - 10 bits LSB are ch number, so up-to 1023
//      - 11-12 bits are view, so up-to 4 (more than enough)
//      - 13-20 bits are CRM number, so up-to 255 (also more than enough)
//
//////////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <algorithm>

#include "CaliDaqData.h"
#include "LogMsg.h"

using namespace std;

//
// ctor
//
calidaq::CaliDaqData::CaliDaqData()
{
  // env variable to find to look for calibration files
  m_path = "";
  char *path = getenv("THEDATAFILES");
  if( path )
    {
      m_path = path;
      if(*m_path.rbegin() != '/') m_path += "/";
    }
  
  // define masks
  _crmMask = 0xFF;  // 8 bits for CRP module
  _vwMask  = 0x3;   // 2 bits for view
  _chMask  = 0x3FF; // 10 bits for ch number
}


//
// dtor
//
calidaq::CaliDaqData::~CaliDaqData()
{;}


//
// TO DO check the arguments for consistency
//
uint32_t calidaq::CaliDaqData::mapIt(uint16_t icrm, uint16_t iview, uint16_t ich)
{
  
  // make our key
  uint32_t rval = ((icrm & _crmMask) << 12) + ((iview & _vwMask) << 10) + (ich & _chMask);
  return rval;
}


//
// apply inverse transformation
//
void calidaq::CaliDaqData::invmapIt(uint32_t idx, uint16_t &icrm, uint16_t &iview, uint16_t &ich)
{
  icrm  = (idx >> 12 )  & _crmMask;
  iview = (idx >> 10 ) & _vwMask;
  ich   = idx & _chMask;
}

//
// find value in the mapped idx
//
ssize_t calidaq::CaliDaqData::findIt(uint32_t idx)
{
  if( m_chid.empty() ) return -1;
  
  //std::vector<uint32_t>::iterator it;
  _it = lower_bound( m_chid.begin(), m_chid.end(), idx );
  bool found = ( _it != m_chid.end() && *_it == idx );
  if(!found) return -1;

  //
  return std::distance( m_chid.begin(), _it );
}

//
// 
//
ssize_t calidaq::CaliDaqData::addIndx(uint32_t idx)
{
  if( !m_chid.empty() ) 
    {
      if( idx <= m_chid.back() )
	{
	  // check if we already have it
	  ssize_t rval = findIt( idx );
	  
	  // cannot re-add it
	  if( rval >= 0)
	    {
	      dlardaq::msg_err<<"Cannot re-insert the same index"<<std::endl;
	      return -2;
	    }
	  
	  // otherwise return the index where we inserted it
	  rval = std::distance( m_chid.begin(), _it );
	  m_chid.insert( _it, idx );
	  return rval;
	}
    }
  
  // otherwise just push back
  m_chid.push_back( idx );  
  return -1;
}
