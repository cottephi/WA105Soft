/////////////////////////////////////////////////////////////////////////////////
//
//  2D track in a given view
//
//
//
/////////////////////////////////////////////////////////////////////////////////

#ifndef __TRACK2D_H__
#define __TRACK2D_H__

#include <TVector2.h>
#include <vector>

#include "Cluster2d.h"

namespace LArReco
{
  class Track2d : public Cluster2d
  {
  public:
    Track2d();
    ~Track2d();
    
    Track2d(std::vector<const LArReco::QHit*> &hits);
    Track2d( std::vector<TVector2> &xz, std::vector<double> dqdx, 
	     std::vector<const LArReco::QHit*> &hits );
    
    virtual Track2d* clone() const { return new Track2d(*this); }
    
    void SetPid(Int_t val){ fPid = val; }
    void SetMomentum(double val){ fMom = val; }
    void SetCRM(Int_t val){ fCrm = val; }
    
    //
    void SetSlopeInit(Double_t val, Double_t valerr)
    { fTanThInit = val; fTanThInitErr = valerr;}
    void SetSlopeFinal(Double_t val, Double_t valerr)
    { fTanThFinal = val; fTanThFinalErr  = valerr; }
    
    //
    void SetSlopeInit( Double_t val ) { fTanThInit = val; }
    void SetSlopeFinal( Double_t val ) { fTanThFinal = val; }

    //
    void SetChi2(Double_t val) { fChi2 = val; }
    
    //
    void SetStartErrors( Double_t errPos, Double_t errTan, Double_t rho )
    { fDPosInitErr = errPos; fTanThInitErr = errTan; fCorrCoeffInit = rho; }
    
    void SetEndErrors( Double_t errPos, Double_t errTan, Double_t rho )
    { fDPosFinalErr = errPos; fTanThFinalErr = errTan; fCorrCoeffFinal = rho; }
    
    
    //void SetSlopeGlobal(double val, double valerr);
    
    void AddDeltaRays(std::vector<const LArReco::QHit*> &drays);
    
    Int_t GetCRM() const { return fCrm; }
    Int_t GetPid() const { return fPid; }
    Int_t GetNpts() const { return (Int_t)fQdX.size(); }
    Double_t GetMomentum() const { return fMom; }
    Double_t GetChi2() const { return fChi2; }
	
    Double_t GetSlopeInit() const { return fTanThInit; }
    Double_t GetSlopeInitErr() const { return fTanThInitErr; }
    Double_t GetSlopeFinal() const { return fTanThFinal; }
    Double_t GetSlopeFinalErr() const { return fTanThFinalErr; }

    Double_t GetDistInitErr() const { return fDPosInitErr; }
    Double_t GetDistFinalErr() const { return fDPosFinalErr; }
    
    Double_t GetCorrCoeffInit() const { return fCorrCoeffInit; }
    Double_t GetCorrCoeffFinal() const { return fCorrCoeffFinal; }
    
    Double_t GetLength() const { return fLength; }

    const std::vector<TVector2>& GetPath() const { return fPath; }
    const std::vector<Double_t>& GetDQ() const { return fQdX; }

    const std::vector<LArReco::RecoObjLink>& GetHits() const { return GetDownNodes(); }
    const std::vector<LArReco::RecoObjLink>& GetDRays() const { return fDeltaRays; }

    // append to this track
    Track2d& operator+=(const Track2d& rhs);

    void Print() const;

  private:
    void SetDefaults();
    
    void Init( std::vector<const LArReco::QHit*> &hits ); //, bool buildpath );
    void MakePath( std::vector<const LArReco::QHit*> &hits );
    
    Int_t    fCrm;
    Int_t    fPid;
    Double_t fMom; 
    Double_t fChi2;
    Double_t fLength;

    // initial direction
    Double_t fTanThInit; 
    Double_t fTanThInitErr; 

    // final direction
    Double_t fTanThFinal;
    Double_t fTanThFinalErr;

    // error on the initial drift distance
    Double_t fDPosInitErr; 
    // error on the final drift distance
    Double_t fDPosFinalErr; 
    
    // correlation coefficient
    Double_t fCorrCoeffInit;
    Double_t fCorrCoeffFinal;


    std::vector<TVector2> fPath; // pos and distance in cm
    std::vector<Double_t> fQdX;

    // delta ray hits
    std::vector<LArReco::RecoObjLink> fDeltaRays;

  public:
    ClassDef(Track2d, 4);
  };
}

#endif
