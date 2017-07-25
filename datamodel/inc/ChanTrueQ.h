////////////////////////////////////////////////////////////////////////////
//
//   Charge truth information for a given channel
//   All the added charge should be in fC
//
//   Created: Mon Oct 19 14:37:06 CEST 2015
//
//
////////////////////////////////////////////////////////////////////////////

#ifndef __CHANQTRUTH_H__
#define __CHANQTRUTH_H__

#include <iostream>

#include <TObject.h>

namespace LArMCTruth
{
  class ChanTrueQ : public TObject
  {
  public:
    ChanTrueQ();
    ChanTrueQ(UInt_t ch);
    ~ChanTrueQ();

    void Print();

    //
    void SetChan(UInt_t val){ fCh = val; }
    void SetData(UInt_t tstart, std::vector<Float_t> &data);
    void AddData(UInt_t at, std::vector<Float_t> &data);
    void AddDatum(UInt_t at, Float_t val);


    // gets
    Float_t GetQ() const { return fQtot; }
    UInt_t GetChan() const { return fCh; }
    UInt_t GetTstart() const { return fStart; }
    UInt_t GetTend() const { return fEnd; }
    UInt_t GetSize() const { return fData.size(); }

    const std::vector<Float_t>& GetData() { return fData; }

    void Reset(){ fStart = fEnd = 0; fQtot = 0; fData.clear();}

  private:

    void SumCharge();

    Float_t fQtot;               //total Q
    UInt_t fCh;                  //channel data
    UInt_t fStart;               //start of time sequence
    UInt_t fEnd;                 //end of the time sequence
    std::vector<Float_t> fData;  //contnuous sequence of time data

    ClassDef(ChanTrueQ,1);
  };

}

#endif
