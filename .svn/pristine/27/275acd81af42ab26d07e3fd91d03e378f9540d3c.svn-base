//////////////////////////////////////////////////////////////////////////////////
//
//  Object to handle channel pedestals
//  It is a singleton object which loads a list of pedestal values
//  from $WA105_CALIBDATA/pedestals file the format should be:
//  seqch crm view view_ch ped pedrms
//
//////////////////////////////////////////////////////////////////////////////////
#ifndef __CALIDAQCHPEDESTAL_H__
#define __CALIDAQCHPEDESTAL_H__

#include <utility>
#include "CaliDaqData.h"

namespace calidaq
{
  class ChPedestal : public CaliDaqData
  {
  public:
        
    static ChPedestal& Instance()
    {
      static ChPedestal inst;
      return inst;
    }

    float Pedestal( int icrm, int iview, int ich, float &pedrms );    
    float Pedestal( int icrm, int iview, int ich );
    
    void Print();

  private:
    ChPedestal();
    ChPedestal(const ChPedestal&);
    ChPedestal& operator=(const ChPedestal&);
    
    ~ChPedestal();
    
    void LoadFile( std::string &fname );

    std::vector< std::pair< float, float > > m_peds; // mean, rms
  };  

}

#endif
