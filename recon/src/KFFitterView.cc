/////////////////////////////////////////////////////////////////////////////////////////////
//
//  Fitter Kalman Filter style for 2D
//  Could be used to crawl over hits in 2D track search
//  Precompute the results of matrix manupulations in some cases to increase speed
//
//  Treatment of MS is very primitive as we do not have information on full 
//  3D path at this stage
//
/////////////////////////////////////////////////////////////////////////////////////////////

#include "KFFitterView.h"

using namespace std;

//
//
//
KFFitterView::KFFitterView()
{
  fSgm   = 1.0;
  fPBeta = 1.0;

  // residual matrix (just 1x1 for this problem)
  fRk  = 0;
  
  fSk.resize(2, 0);
  fCk.resize(3, 0);
  
  // covariance for the MS effects
  fQk.resize(3, 0);

  //
  fLocked = false;
}



//
// get chi2 from apriori
//
double KFFitterView::GetChi2( double z, double &r )
{
  // residual
  r = fabs(z - fSk[1]);
  double chi2 = r*r/fRk;
  if( chi2 != chi2 ) return 999;
  return chi2;
}


//
//
//
void KFFitterView::Clear()
{
  fLocked = false;
  for(size_t i=0;i<fSk.size();i++) fSk[i] = 0;
  for(size_t i=0;i<fCk.size();i++) fCk[i] = 0;
  fRk = 0;
}

//
// Rough estimation of the process noise due to multiple scattering
// Delta    : step in cm
// theta_MS : MS angle RMS
// theta    : incident angle
//
// Q11 = 1/cos(theta)^5 x theta_MS^2
// Q22 = 1/3 x 1/cos(theta)^5 x Delta^2 x theta_MS^2 
// Q12 = Q21 = 1/2 x 1/cos(theta)^5 x Delta x theta_MS^2
//
void KFFitterView::GetProcessNoise( double pbeta, double Delta, double tanTheta,
				    std::vector<double> &Q )
{
  if( Q.size() < 3 ) Q.resize(3);

  if( Delta == 0 || pbeta == 0 )
    {
      Q[0] = 0;
      Q[1] = 0;
      Q[2] = 0;
      return;
    }

  double w = fabs(Delta); // should be in cm
  double X0 = 14.0;       // radiation length in LAr in cm

  // 
  double Tht0Sq = ( 0.0136/pbeta ) * ( 0.0136/pbeta ) * w/X0;
  
  // effects of incidence angle
  double IncFac = pow( 1 + tanTheta * tanTheta, 2.5 );
  if(IncFac != IncFac) IncFac = 1.0; // problem ...
  
  double F = Tht0Sq * IncFac;
  
  Q[0] = F*1;
  Q[1] = F*0.333333*w*w;
  Q[2] = F*0.5*w;
}
