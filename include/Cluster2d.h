/////////////////////////////////////////////////////////////////////////////////
//
//  2D cluster of hits
//   Normally a base class for 2D track and 2D shower
//   Could also be used for delta rays     
//
//  
/////////////////////////////////////////////////////////////////////////////////


#ifndef __CLUSTER2D_H__
#define __CLUSTER2D_H__

#include <vector>

//#include <TRefArray.h>
#include <TVector2.h>
#include "RecoObject.h"
#include "QHit.h"

namespace LArReco
{
  class Cluster2d : public RecoObject 
  {    
  public:
    Cluster2d();
    virtual ~Cluster2d();
    
    Cluster2d(std::vector<const LArReco::QHit*> &hits);
    Cluster2d(TVector2 &start, TVector2 &end, 		
	      std::vector<const LArReco::QHit*> &hits);

    virtual Cluster2d* clone() const { return new Cluster2d(*this); }

    Double_t GetQ() const { return fTotQ; } //fC
    Double_t GetSlope() const { return fSlope; }
    Double_t GetT0() const { return fT0; }
    
    //TRefArray* GetHits() const { return fHits; }
    
    virtual const TVector2& GetStart() const { return fStart; }
    virtual const TVector2& GetEnd() const { return fEnd; }
    
    
    void SetT0(Double_t val) { fT0 = val; }
    
  protected:
    //
    virtual void AddHit(const LArReco::QHit* hit);
    
    TVector2 fStart;  // pos cm, drift distance cm
    TVector2 fEnd;    // pos cm, drift distance cm
    Double_t fTotQ;   // total Q in fC
    Double_t fSlope;  // us / cm
    Double_t fT0;     // time of arrival with respect to readout start in us

  private:
    void Init(std::vector<const LArReco::QHit*> &hits);

  public:
    
    ClassDef(Cluster2d, 1);
  };
}

#endif
