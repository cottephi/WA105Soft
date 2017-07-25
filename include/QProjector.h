/////////////////////////////////////////////////////////////////////////////////////
//
//  Primary function is to assign the channel / time bin number
//  Also model longitudinal and transverse diffusion with projection onto 2D plane
//
//
//  Created: Fri Oct 16 15:09:34 CEST 2015
//
/////////////////////////////////////////////////////////////////////////////////////

#ifndef __QPROJECTION_h__
#define __QPROJECTION_h__

#include <cmath>

#include <map>
#include <iostream>
#include <vector>
#include <string>
#include <utility>

#include <TH2F.h>
#include <TMath.h>
#include <TSpline.h>

#include "AnodeTrueQ.h"

// class to handle gaussian integrals for diffusion
class GIntegral
{
 public:
  GIntegral(double nsgm = 7, double step = 0.05);
  ~GIntegral();

  void SetRange( int nsgm );
  double GetIntegral(double x1, double x2);
  TSpline3* GetSpline() const { return fSpline; }

  void BinIntegral(double mu, double sigma,
		   double x1, double x2, double step,
		   double norm, double epszero,
		   std::vector< std::pair<double, double> > &res);

 private:
  double Integral(double x1, double x2);
  bool CheckRange(double &val);
  void ComputeSpline();

  TSpline3 *fSpline;

  double fMaxX;
  double fStep;
};

/////////////////////////////////////////////////////
// projection of charge on 2D anode
class QProjectorCRM
{
 public:

  QProjectorCRM( double posX0, double posY0, 
		 double pitchX, double pitchY, double pitchT,
		 unsigned int nchx, unsigned int nchy, unsigned int nsamples );
  ~QProjectorCRM();

  const LArMCTruth::AnodeTrueQ* Projection( double posx, double posy,
					    double tarr, double Q,
					    double sigmaL = 0, double sigmaT = 0 );
  
  void SetQZero(double val){ fQzero = val; }

  // time bin & channel numbers
  int GetTimeBin(double tval) const;
  int GetChNum(double pos, int iview) const;

  // gaussian integrator
  const GIntegral* GetIntegrator() const { return fGausIntegrator; }

  // fill histogram
  void FillHisto(TH2F *histo, int iview);

  // get CRM parameters
  double GetPitchX() const { return fPitchX; }
  double GetPitchY() const { return fPitchY; }
  double GetPitchT() const { return fPitchT; }
  double GetPosX0() const { return fPosX0; }
  double GetPosY0() const { return fPosY0; }
  double GetWidthX() const { return fWidthX; }
  double GetWidthY() const { return fWidthY; }
  double GetMaxT() const { return fMaxT; }
  double GetEff_2D(int channelx, int channely);

  unsigned int GetNChX() const { return fNChX; }
  unsigned int GetNChY() const { return fNChY; }
  unsigned int GetNSamT() const { return fNSamT; }

  void LoadEffFile( std::string fname );
  
 protected:
  
  bool GetLimits( double pos, double sigma, int &binn,
		  double &x1, double &x2, int iType );

  void BuildGausIntegrator(int nsgm);

  GIntegral *fGausIntegrator;

  double    fPitchX; // pitch in view 0 (cm)
  double    fPitchY; // pitch in view 1 (cm)
  double    fPitchT; // pitch in time (us)
  double    fPosX0;  // pos of the centre (cm)
  double    fPosY0;  // pos of the centre (cm)
  UInt_t    fNChX;   // number of channels in view 0
  UInt_t    fNChY;   // number of channels in view 1
  UInt_t    fNSamT;  // total number of time samples

  double    fWidthX; // width of active view 0
  double    fWidthY; // width of active view 1
  double    fMaxT;   // width in time (drift view)

  //collection anode
  LArMCTruth::AnodeTrueQ* fCollAnode;

  // these numerical cuts in diffusion calculation
  int       fNSgm;   //max range in nsigma
  double    fQzero;  //cut deposition below this value (in fC)


  // charge collection efficiency
  TH2D*     fAnodeEffMap;
};

#endif
