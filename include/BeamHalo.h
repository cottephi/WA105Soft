#ifndef __BEAMHALO_H__
#define __BEAMHALO_H__

#include <vector>

#include <TMath.h>
#include <TMatrixT.h>
#include <TMatrixTSym.h>
#include <TVectorT.h>
#include <TDecompChol.h>

// for particle_fld structure
#include "WA105CRFlux.h"

namespace bkgsrc
{
  //
  /* typedef struct{ */
  /*   float x, y, z, dirx, diry, dirz, eneKin, time; */
  /*   int pdg; */
  /* } particle_fld; */
  
  //
  // Sample parameters randomly from multivariate normal distribution
  // We use Cholesky decomposition of the covariance matrix
  //
  class MultivarNormal
  {
  private:
    typedef TVectorT<double> TVectorD;
    typedef TMatrixT<double> TMatrixD;
    typedef TMatrixTSym<double> TMatrixDSym;
    
    //
    TVectorD    *pvals;
    TMatrixD    *chold;
    TMatrixDSym *covar; 
    int          npars;
    int          half_pars;
    
  public:
    MultivarNormal(TVectorD &parms, TMatrixDSym &covm);
    
    ~MultivarNormal();
    
    int GetNpars() const {return npars;};
    
    // draw random set
    template< typename T> 
      void DrawRandom(std::vector<T> &parms)
      {
	//if(!parms.empty()) parms.clear();
	parms.resize(npars);
	TVectorD std_rand(npars);
	for(int j=0; j<half_pars; j++)
	  {
	    double z[2];
	    StdNormRand(z);
	    std_rand(j) = z[0];
	    if(npars%2==0 || j!=half_pars-1)
	      std_rand(j+half_pars) = z[1];
	  }
	TVectorD prod = (*chold)*std_rand;
	for(int i=0;i<npars;i++)
	  parms[i] = prod(i) + (*pvals)(i);
      };
    
  private:
    void StdNormRand(double *z);
  };


  //
  // Beam profile in the plane
  //
  class BeamProfile
  {
  public:
    BeamProfile( double x0, double thx0, double sgmx, double epsx, double alphx,
		 double y0, double thy0, double sgmy, double epsy, double alphy );
    ~BeamProfile();
    
  
    double GetProbX( double x, double thx );
    double GetProbY( double y, double thy );
    
    void GetRandom( std::vector<double> &vec );
  
  private:
    double Bivariate(double pos, double tht, double *parMean,  double *parErr);
  
    double fEps[2];   // emittance
    double fAlpha[2]; // Twiss alpha

    double fMeanx[2]; // x0 (cm), xp0 (mrad)
    double fMeany[2]; // y0 (cm), yp0 (mrad)
    double fParsx[3]; // sx (cm), sxp (mrad), rho
    double fParsy[3]; // sy (cm), sxp (mrad), rho

    
    MultivarNormal *fMvNorm;
  };

  
  class WA105HaloFlux
  {
  public:
    WA105HaloFlux();
    ~WA105HaloFlux();
    
    
    size_t GetHalo(double t0, double twin, std::vector<particle_fld> &flux);
    
    void SetRate( double val ) { fRate = val; } //muons / s
    void SetProfile( double x0, double thx0, double sgmx, double epsx, double alphx, 
		     double y0, double thy0, double sgmy, double epsy, double alphy );
    
  private:
    BeamProfile *fProfile;
    
    double fRate;
    double fDetSzX;
    double fDetSzY;
    double fDetSzZ;
    double fDiaSz;
    double fPlaneDist;
    double fRotAngle;
    
    size_t GetTiming(double t0, double twin, std::vector<float> &evtimes);
    int    GetPid();
    double GetEkin();
    
    // check intercept with the detector
    bool   DetectorIntercept( double nx, double ny, double nz,
			      double vx, double vy, double vz,
			      double &x, double &y, double &z);
    
    
  };

}// namespace BeamHalo

#endif
