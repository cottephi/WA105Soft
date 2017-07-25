//////////////////////////////////////////////////////////////////////////////////
//
// Class to define veto region for cosmic ray track reconstruction to avoid
// E.g., to prevent reconstructing tracks related to the beam shower events 
// Implemented as a singleton object 
//
//////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <cmath>

#include "CosmicRecoVetoRegion.h"


using namespace std;

//
RecoUtils::CosmicRecoVetoRegion::CosmicRecoVetoRegion()
{
  fVx = fVy = fVz = 0;
  fAlpha = 0;
  fR = fL = 0;
  
  _npars = 6;
  _isSet = false;
}

//
RecoUtils::CosmicRecoVetoRegion::~CosmicRecoVetoRegion()
{;}

//
// define veto region
//
void RecoUtils::CosmicRecoVetoRegion::Set( std::vector<float> &params )
{
  if(params.size() < _npars) 
    {
      _isSet = false;
      return;
    }
  
  // cylinderical region
  fVx    = params[0];  // cm
  fVy    = params[1];  // cm
  fVz    = params[2];  // cm
  fR     = params[3];  // cm
  fL     = params[4];  // cm
  fAlpha = params[5];  // rad

  fC = cos(fAlpha);
  fS = sin(fAlpha);
  
  _isSet = true;
}


//
//
//
bool RecoUtils::CosmicRecoVetoRegion::isInside( double x, double y, int iview )
{
  // if no region is defined
  if( !_isSet ) return false;
  
  bool ret = false;
  
  if( iview == 0 )
    ret = InsideCylinder( x, y, fVx, fVz );
  else if( iview == 1 )
    ret = InsideCylinder( x, y, fVy, fVz );
  
  return ret;
}


//
//
//
bool RecoUtils::CosmicRecoVetoRegion::InsideCylinder( double x, double y, double vx, double vy )
{
  // translate
  double xx = (x - vx);
  double yy = (y - vy);
  
  // rotate
  double xp =  xx*fC + yy*fS;
  double yp = -xx*fS + yy*fC;

  bool ret = (xp > 0 && xp <= fL) && ( fabs(yp) <= fR );

  return ret;
}
