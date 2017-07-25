#include "DataCRP.h"

ClassImp(LArRawData::DataCRP)

// ctor
LArRawData::DataCRP::DataCRP()
{
  NumCRM = 0;
  dCRM   = new TClonesArray("LArRawData::DataCRM");
}

// dtor
LArRawData::DataCRP::~DataCRP()
{
  Reset();
}

// Clear
void LArRawData::DataCRP::Clear(Option_t *)
{
  dCRM->Clear("C");
  NumCRM = 0;
}

// Reset
void LArRawData::DataCRP::Reset(Option_t *option)
{
  Clear(option);
  
  delete dCRM;
  dCRM = 0;
}

// AddCRM
LArRawData::DataCRM* LArRawData::DataCRP::AddCRM()
{
  TClonesArray &crms = *dCRM;
  LArRawData::DataCRM *crm = new(crms[NumCRM++]) LArRawData::DataCRM();
  return crm;
}

// AddCRM
LArRawData::DataCRM* LArRawData::DataCRP::AddCRM(Int_t crm_id)
{
  TClonesArray &crms = *dCRM;
  LArRawData::DataCRM *crm = new(crms[NumCRM++]) LArRawData::DataCRM(crm_id);
  return crm;
}

LArRawData::DataCRM* LArRawData::DataCRP::GetCRM(Int_t at)
{
  return (LArRawData::DataCRM*)dCRM->At(at);
}
