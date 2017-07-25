//////////////////////////////////////////////////////////////////////////////////
//
//  Object to handle channel status mask 
//  It is a singleton object which loads a list of masked channel
//  from $WA105_CALIBDATA/chmasked file the format should be:
//  seqch crm view view_ch
//
//////////////////////////////////////////////////////////////////////////////////
#ifndef __CALIDAQCHMASK_H__
#define __CALIDAQCHMASK_H__

#include "CaliDaqData.h"

namespace calidaq
{
  class ChMask : public CaliDaqData
  {
  public:
        
    static ChMask& Instance()
    {
      static ChMask inst;
      return inst;
    }
    
    // true = ch is masked 
    bool Masked( int icrm, int iview, int ich );
    
    void Print();

  private:
    ChMask();
    ChMask(const ChMask&);
    ChMask& operator=(const ChMask&);
    
    ~ChMask();
    
    void LoadFile( std::string &fname );
  };  

}

#endif
