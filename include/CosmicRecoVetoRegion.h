//////////////////////////////////////////////////////////////////////////////////
//
// Class to define veto region for cosmic ray track reconstruction to avoid
// E.g., to prevent reconstructing tracks related to the beam shower events 
// Implemented as a singleton object 
// 
//////////////////////////////////////////////////////////////////////////////////

#ifndef __COSMICRECOVETOREGION_H__
#define __COSMICRECOVETOREGION_H__

#include <vector>

namespace RecoUtils
{
  
  class CosmicRecoVetoRegion
  {
    
  public:
    
    static CosmicRecoVetoRegion& Instance()
    {
      static CosmicRecoVetoRegion inst;
      return inst;
    }
    
    void Set( std::vector<float> &params );
    bool isInside( double x, double y, int iview );
    
  private:
    CosmicRecoVetoRegion();
    CosmicRecoVetoRegion( const CosmicRecoVetoRegion& );
    CosmicRecoVetoRegion& operator=( const CosmicRecoVetoRegion& );
  
    ~CosmicRecoVetoRegion();

    bool InsideCylinder( double x, double y, double vx, double vy );

    bool _isSet;
    size_t _npars;

    //
    double fVx, fVy, fVz;
    double fAlpha;
    double fR, fL;

    // cos(alpha) and sin(alpha)
    double fC, fS;
  };
};

#endif
