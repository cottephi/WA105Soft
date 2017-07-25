////////////////////////////////////////////////////////////////////////////////
//
//   Class to hold raw data from each CRM (charge readout module)
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __DATACRM_H__
#define __DATACRM_H__

#include <TROOT.h>
#include <TObject.h>
//#include <TList.h>
#include <TClonesArray.h>

#include "DataChan.h"

namespace LArRawData
{
  class DataCRM : public TObject
  {
    
  public:
    DataCRM();
    DataCRM(Int_t id);
    ~DataCRM();
    
    DataChan* AddDataChannel(Int_t iview);
    DataChan* AddDataChannel(Int_t iview, Int_t chanid, Dtype dtype);
    DataChan* AddDataChannel(Int_t iview, Int_t chanid, Dtype dtype, Code scode);
    
    // channel data
    TClonesArray *GetChanView0() const { return ChView0; }
    TClonesArray *GetChanView1() const { return ChView1; }
    DataChan* GetChan(Int_t iview, Int_t at);

    UInt_t GetNChanView0(){ return NChView0; }
    UInt_t GetNChanView1(){ return NChView1; }
    Int_t  GetCRMId(){ return CRMId; }
    
    void SetCRMId(Int_t val){ CRMId = val; }
    
    //
    void Clear(Option_t *option = "");
    void Reset(Option_t *option = "");

  private:
    void   Init();

    TClonesArray *ChView0;
    TClonesArray *ChView1;
    UInt_t        NChView0;
    UInt_t        NChView1;
    Int_t         CRMId;

  public:
    ClassDef(DataCRM, 1);
  };
}

#endif
