////////////////////////////////////////////////////////////////////////////////
//
//   Class to hold raw channel data
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __DATACHAN_H__
#define __DATACHAN_H__

#include <TROOT.h>
#include <TObject.h>
#include <TList.h>
#include <TClonesArray.h>

#include "DataFragment.h"

//#define MAXDFRAG 100

namespace LArRawData
{
  enum Code
  { 
    CHANOFF,   //no data
    FULLDATA,  //full data
    PARTDATA,  //partial data
    ZSDATA,    //zero-suppressed data
    PEDDATA    //pdestal only
  };  

  enum Dtype
  { 
    DTID_USHORT = 1,
    //DTID_UINT = 2,
    DTID_FLOAT = 3 
  };

  class DataChan : public TObject
  {
    
  public:
    DataChan();
    DataChan(Int_t chanid, Dtype dtype);
    DataChan(Int_t chanid, Dtype dtype, Code scode);
    ~DataChan();
    
    TObject* AddDataFragment();          // necessary to downcast !
    TObject* GetDataFragment(Int_t at);  // necessary to downcast !
    TClonesArray *GetDataFragments() const { return DataFrags; }
    
    // initialize all fragments in the list
    void InitFragments(UInt_t fstart, UInt_t fsize);
    
    UInt_t  GetNFrags(){ return NFrags; }
    Int_t   GetChanId(){ return ChanId; }
    Float_t GetPed(){ return PedVal; }
    Float_t GetPedRMS(){ return PedRMS; }
    Code    GetChanStatus(){ return ChanStatus; }
    Dtype   GetDataType(){ return DataType; }
    
    void SetChanId(Int_t val){ ChanId = val; }
    void SetPed(Float_t val){ PedVal = val; }
    void SetPedRMS(Float_t val){ PedRMS = val; }
    void SetChanStatus(Code val){ ChanStatus = val; }

    //
    void Clear(Option_t *option = "");
    void Reset(Option_t *option = "");

  private:

    Int_t         ChanId;        // channel id
    Float_t       PedVal;        // pedestal value
    Float_t       PedRMS;        // pedestal rms
    UInt_t        NFrags;        // number of fragments
    Code          ChanStatus;    // channel status
    Dtype         DataType;      // data ttpe
    TClonesArray *DataFrags;     // data fragments
 
    
  public:
    ClassDef(DataChan,1);
  };
}

#endif
