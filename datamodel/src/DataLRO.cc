#include "DataLRO.h"

ClassImp(LArRawData::DataLRO)

// 
LArRawData::DataLRO::DataLRO()
{
  NumPMTs = 0;
  dLRO   = new TClonesArray("LArRawData::DataLRO");
}

// 
LArRawData::DataLRO::~DataLRO()
{
  Reset();
}

// 
void LArRawData::DataLRO::Clear(Option_t *)
{
  dLRO->Clear("C");
  NumPMTs = 0;
}

// Reset
void LArRawData::DataLRO::Reset(Option_t *option)
{
  Clear(option);
  
  delete dLRO;
  dLRO = 0;
}
