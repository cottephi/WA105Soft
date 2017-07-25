////////////////////////////////////////////////////////////////////////////////
//
//   Class to hold raw data from all CRMs 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __DATACRP_H__
#define __DATACRP_H__

#include <TROOT.h>
#include <TObject.h>
#include <TClonesArray.h>

#include "DataCRM.h"

namespace LArRawData
{
  class DataCRP : public TObject
  {
  public:
    DataCRP();
    ~DataCRP();
    
    DataCRM* AddCRM();
    DataCRM* AddCRM(Int_t crm_id);
    DataCRM* GetCRM(Int_t at);
    TClonesArray *GetCRMs() const { return dCRM; }
    
    UInt_t GetNumCRM(){ return NumCRM; } 
    
    //
    void Clear(Option_t *option = "");
    void Reset(Option_t *option = "");

  private:
    UInt_t NumCRM;
    TClonesArray *dCRM;

  public:
    ClassDef(DataCRP, 1);
  };
}

#endif

