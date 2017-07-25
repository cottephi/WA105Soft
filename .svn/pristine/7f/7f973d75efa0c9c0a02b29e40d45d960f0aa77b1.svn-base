/////////////////////////////////////////////////////////////////////////////////
//
//  Basic 2D hit
//
//  Modified: Mon Mar 14 15:17:20 CET 2016
//            VG: removed second spatial coordinates
//                added const for function returns
//
/////////////////////////////////////////////////////////////////////////////////

#ifndef __BASICHIT_H__
#define __BASICHIT_H__

//#include <TObject.h>
#include "RecoObject.h"

#define MAXQHIT 10000 // max q per hit
#define MINQHIT 0.01  // min q per hit
#define MCHITINFO 6   // for MC association

namespace LArReco
{
  class QHit : public RecoObject //public TObject
  {
  public:
    QHit();
    //QHit( const QHit &hit );
    QHit( Int_t crmid, Int_t iview, Int_t chnum );
    QHit( Int_t crmid, Int_t iview, Int_t chnum, 
	  UInt_t tstart, UInt_t tend, UInt_t tmax,
	  Double_t qmax, Double_t qtot );
  
    ~QHit();
    
    // clone function
    virtual QHit* clone() const { return new QHit(*this); }
    
    //shift hit time by a given t0
    void ShiftHitTb(UInt_t t0); 
    
    // 
    void SetCRM(Int_t val){ crmId = val; }
    void SetView(Int_t val){ iView = val; }
    void SetCh(Int_t val){ chId = val; }
    
    void SetTbStart(UInt_t val){ tbStart = val; }
    void SetTbEnd(UInt_t val){ tbEnd = val; }
    void SetTbMax(UInt_t val){ tbMax = val; }
    
    void SetQ(Double_t val){ Qtot = val; }
    void SetAmplitude(Double_t val){ Ampl = val; }
    
    void SetHitIndex(Int_t val) { SetObjIndex(val); }
    
    void SetHitMCCharge(int i, Float_t MCQ) {HitMCQ[i]=MCQ;}
    
    void SetT(Float_t val){ Ht = val; }
    void SetX(Float_t val){ Hx = val; }
    void SetD(Float_t val){ Hd = val; }

    //
    Int_t GetCRM() const { return crmId; }
    Int_t GetView() const { return iView; }
    Int_t GetCh() const { return chId; }

    UInt_t GetTbStart() const { return tbStart; }
    UInt_t GetTbEnd() const { return tbEnd; }
    UInt_t GetTbMax() const { return tbMax; }

    Double_t GetAmplitude()  const { return Ampl; }
    Double_t GetQ() const { return Qtot; }
    Double_t GetQTotMC() const;
  
    Int_t GetHitIndex() const {return GetObjIndex(); }
    Float_t GetHitMCCharge(int i) const {return  HitMCQ[i];}
    
    //
    Float_t GetT() const { return Ht; }
    Float_t GetX() const { return Hx; }
    Float_t GetD() const { return Hd; }
  
    Bool_t IsHitValid(){ return isValid; }

    Bool_t ValidateHitParam();

    void Print() const;
    void Reset();
 
    // hit location
    Int_t  crmId;    //CRM id
    Int_t  iView;    //view id
    Int_t  chId;     //channel id
    
    //all hit parameters are valid
    Bool_t isValid;  

    // basic hit parameters
    UInt_t tbStart;  //time bin start of the hit
    UInt_t tbEnd;    //time bin end of the hit
    UInt_t tbMax;    //time bin of the peak
  
    Double_t Ampl;  // amplitude 
    Double_t Qtot;  // total charge (fC)
    
    // from MC
    Float_t HitMCQ[MCHITINFO];
    /*0= charge contribution from muons
      1= charge contribution from (e- and e+)
      2= charge contribution from gammas
      3= charge contribution from pi0
      4= charge contribution from pi+ pi-
      5= charge contribution from all other particles */
    
    //Int_t HitIndex; //hit index in view

    // hit coordinates in global system
    Float_t Ht;     //hit time in us
    Float_t Hx;     //hit position in cm
    Float_t Hd;     //hit distance from CRP in cm

   
  private:
    //
    bool CheckUplinkPolicy(RecoObjLink &ref);
    bool CheckDnlinkPolicy(RecoObjLink &ref);


  public:
    ClassDef(QHit, 3);
  };
}


#endif
