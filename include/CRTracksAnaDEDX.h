////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//  Analysis module to look at cosmic ray charge loss for calibration of CRP gain
//  The calculation is performed in each view separetely to allow to study charge 
//  sharing uniformity between views
//
//
////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CRTRACKSANADEDX_H__
#define __CRTRACKSANADEDX_H__


#include "RunHeader.h"
#include "GeomConfig.h"

#include "Track2d.h"

#include "RecoUtils.h"

//
//
//
class CRTracksAnaDEDX
{
 public:
  CRTracksAnaDEDX(const RunHeader *rhead, const GeomConfig *ghead);
  ~CRTracksAnaDEDX();

  int Process( const LArReco::Track2d* trk0, 
	       const LArReco::Track2d* trk1 );
  
  std::vector<TVector3> &Get3dPath0(){ return fPath3dTrk0; }
  std::vector<double> &GetQloss0(){ return fDqTrk0; }
  
  std::vector<TVector3> &Get3dPath1(){ return fPath3dTrk1; }
  std::vector<double> &GetQloss1(){ return fDqTrk1; }

  double GetMeanQloss0() const { return fMean_dQdx0; }
  double GetMeanQloss1() const { return fMean_dQdx1; }
  double GetTotalQloss0() const { return fTotal_Q0; }
  double GetTotalQloss1() const { return fTotal_Q1; }
  double GetTotalPath0() const { return fPathLen0; }
  double GetTotalPath1() const { return fPathLen1; }
  
  
  // should be in micro seconds !!!
  void SetLifetime( double val )
  { 
    fLifetime = val; 
    fEleAtten = fVdrift * fLifetime;
  }

 private:
  double LifetimeCorrection(double zdrift);
  //void QuenchingCorrection();
  void Init( const LArReco::Track2d *atrk, int iview, std::vector<TVector3> &path,
	     std::vector<double> &charge, RecoUtils::LineSegment &lseg );
  
  double Build( double pitch, int iview, double &qtot,
		std::vector<TVector3> &path, 
		std::vector<double> &charge, 
		std::vector<double> &dstep, 
		RecoUtils::LineSegment &lseg, 
		RecoUtils::LineSegment &lseg_other );
	     


  double fLifetime;   // electron lifetime in us
  double fEleAtten;   // electron attenuation per cm
  double fVdrift;     // drift velocity in cm / us
  double fPitchT;     // time pitch in us
  double fPitch0;     // readout pitch in mm
  double fPitch1;     // readout pitch in mm
  double fEfield;     //

  std::vector<TVector3> fPath3dTrk0;
  std::vector<double> fDqTrk0;
  std::vector<double> fStep0;

  std::vector<TVector3> fPath3dTrk1;
  std::vector<double> fDqTrk1;
  std::vector<double> fStep1;

  double fMean_dQdx0;
  double fMean_dQdx1;
  double fTotal_Q0;  // purity corrected
  double fTotal_Q1;  // purity corrected 
  double fPathLen0;
  double fPathLen1;
  // for line segment interpolation
  RecoUtils::LineSegment fLn0;
  RecoUtils::LineSegment fLn1;
  
};


#endif 
