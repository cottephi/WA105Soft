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
#ifndef __KFFITTERVIEW_H__
#define __KFFITTERVIEW_H__

#include <iostream>
#include <vector>
#include <cmath>

#include <TMatrixT.h>

typedef TMatrixT<double> TMatrixD;
//
class KFFitterView
{
 public:
  KFFitterView();
  ~KFFitterView(){;}
  
  // set momentum x beta factor
  void SetProcessNoise( double v ){ fPBeta = v; }

  // set hit resolution 
  void SetResolution( double v ){ fSgm = v; }
  

  // predict
  template <class T> void MakePrediction(T &pnt, double Delta);
  template <class T> void UpdateEstimate(T &pnt);
  template <class T> void Smoother(T &pnt, T &ppnt);
  
  //
  double GetChi2( double z, double &r );

  double GetPredCmm() const { return fCk[0]; }
  double GetPredCbb() const { return fCk[1]; }
  double GetPredCmb() const { return fCk[2]; }
  
  double GetPredTan() const { return fSk[0]; }
  double GetPredPos() const { return fSk[1]; }

  double GetTht2() const { return fQk[0]; }
  double GetEps2() const { return fQk[1]; }
  double GetCorr() const { return fQk[2]; }
  
  //
  void GetPredState( std::vector<double> &s ) const { s = fSk; }
  
  // get covariance due to multiple scattering
  void GetProcessNoise( double pbeta, double Delta, double tanTheta, 
			std::vector<double> &Q );
  
  TMatrixD GetMatrix( const std::vector<double> &vec )
  {
    TMatrixD C(2,2);
    C(0,0) = vec[0];
    C(1,1) = vec[1];
    C(1,0) = C(0,1) = vec[2];
    return C;
  }
  
 private:
  void Clear();

  bool fLocked;
  
  std::vector<double> fSk;  // state
  std::vector<double> fCk;  // cov mat
  std::vector<double> fQk;  // process noise
  
  double fSgm;              // measurement precision
  double fPBeta;            // 

  double fRk;               // residual covariance 
};


//
//
//
template <class T> 
void KFFitterView::MakePrediction(T &pnt, double Delta)
{
  // previous state
  const std::vector<double> Spr = pnt.GetState();
  const std::vector<double> Cpr = pnt.GetCovMat();

  // apply transformation 
  // F = ( (1, 0), (Delta, 1) )
  
  fSk[0] = Spr[0];
  fSk[1] = Spr[1] + Delta*Spr[0];

  // compute predicted covariance for this plane
  fCk[0] = Cpr[0]; 
  fCk[1] = Cpr[0]*Delta*Delta + 2*Delta*Cpr[2] + Cpr[1];
  fCk[2] = Cpr[0]*Delta + Cpr[2];
  
  // introduce some noise in the slope due to MS
  GetProcessNoise( fPBeta, Delta, Spr[0], fQk );
  
  fCk[0] += fQk[0];
  fCk[1] += fQk[1];
  fCk[2] += fQk[2];

  // residual covariance
  // Rk = H Ck HT + Vk
  fRk = fCk[1] + fSgm*fSgm;

  fLocked = true;
}

//
//
//
template <class T>
void KFFitterView::UpdateEstimate( T &pnt )
{
  if( !fLocked ) 
    {
      std::cerr<<"ERROR: no valid prediction has been made"<<std::endl;
      return;
    }

  // save a priori state
  pnt.SetCovMatApri( fCk );
  pnt.SetStateApri( fSk );

  // now compute a poteriori estimate
  
  double r = pnt.GetY() - fSk[1];
  
  // Kalman gain 
  // G = Pk HT / Rk
  std::vector<double> G(2);
  G[0] = fCk[2] / fRk; // Cmb / Rk
  G[1] = fCk[1] / fRk; // Cbb / Rk

  // update state a posteriori
  std::vector<double> Sk(2);
  Sk[0] = fSk[0] + r * G[0];
  Sk[1] = fSk[1] + r * G[1];
  
  //
  std::vector<double> Ck(3);

  // standard cov matrix update
  // Ck = (I - KH ) Ck-1
  //Ck[0] = fCk[0] - (fCk[2]*fCk[2])/fRk;
  //Ck[1] = fCk[1] - (fCk[1]*fCk[1])/fRk;
  //Ck[2] = fCk[2] - (fCk[1]*fCk[2])/fRk;

  //
  // more stable cov matrix update: NIM A552 566-575 (2005)
  // Ck = (I - 2KH ) Ck-1 + K R K^T
  Ck[0] = fCk[0] - 2*(fCk[2]*fCk[2])/fRk + fRk*G[0]*G[0];
  Ck[1] = fCk[1] - 2*(fCk[1]*fCk[1])/fRk + fRk*G[1]*G[1];
  Ck[2] = fCk[2] - 2*(fCk[1]*fCk[2])/fRk + fRk*G[0]*G[1];
  
  pnt.SetCovMat( Ck );
  pnt.SetState( Sk );

  // calculate chi2 with respect to the smoothed trajectory
  double R = fSgm*fSgm - Ck[1];
  r = (pnt.GetY() - Sk[1]);
  double chi2 = r*r/R;
  pnt.SetChi2( chi2 );
  pnt.SetResidual( fabs(r) );

  Clear();
}


//
// smoother
//
template <class T> 
void KFFitterView::Smoother(T &pnt, T &ppnt)
{
  double Delta = ppnt.GetX() - pnt.GetX();
  if(Delta == 0) return;
  
  // calculate inverse
  std::vector<double> CkkpI(3);
  // get the inverse of a priori
  if( ppnt.GetCovMatInv( CkkpI, 0 ) < 0 )
    {
      return; 
    }
  
  // states
  const std::vector<double> S  = pnt.GetState();
  const std::vector<double> Sp = ppnt.GetStateApri();
  const std::vector<double> Ss = ppnt.GetState();
  
  TMatrixD Pk   = GetMatrix( pnt.GetCovMat() );
  TMatrixD Pkp  = GetMatrix( ppnt.GetCovMatApri() );
  TMatrixD PkpI = GetMatrix( CkkpI );
  TMatrixD Ps   = GetMatrix( ppnt.GetCovMat() );
  
  TMatrixD F(2,2);
  F(0,0) = 1.0;
  F(1,1) = 1.0;
  F(1,0) = Delta;
  F(0,1) = 0.0;
    
  // Pk F^T (Pk|k+1)^-1
  TMatrixD K(Pk, TMatrixD::kMultTranspose, F);
  K *= PkpI;
  
  //
  Ps -= Pkp;
  TMatrixD Pks = Pk + K*TMatrixD(Ps, TMatrixD::kMultTranspose, K);
  
  //
  std::vector<double> Xs(2);
  Xs[0] = S[0] + K(0,0)*(Ss[0] - Sp[0]) + K(0,1)*(Ss[1] - Sp[1]);
  Xs[1] = S[1] + K(1,0)*(Ss[0] - Sp[0]) + K(1,1)*(Ss[1] - Sp[1]);
 
  std::vector<double> Cs(3);
  Cs[0] = Pks(0,0);
  Cs[1] = Pks(1,1);
  Cs[2] = Pks(1,0); // should be symmetric!
  
  pnt.SetCovMat( Cs );
  pnt.SetState( Xs );
  
  // calculate chi2 with respect to the smoothed trajectory
  double R    = fSgm*fSgm - Cs[1];
  double r    = (pnt.GetY() - Xs[1]);
  double chi2 = r*r/R;
  pnt.SetChi2( chi2 );
  pnt.SetResidual( fabs(r) );
}

#endif
