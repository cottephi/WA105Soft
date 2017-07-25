#include "DataCRM.h"

#include <iostream>

ClassImp(LArRawData::DataCRM)

// ctor
LArRawData::DataCRM::DataCRM()
{
  CRMId = -1;
  Init();
}

// ctor
LArRawData::DataCRM::DataCRM(Int_t id)
{
  CRMId = id;
  Init();
}

void LArRawData::DataCRM::Init()
{
  NChView0 = 0;
  ChView0  = new TClonesArray("LArRawData::DataChan");

  NChView1 = 0;
  ChView1  = new TClonesArray("LArRawData::DataChan");
}

// dtor
LArRawData::DataCRM::~DataCRM()
{
  Reset();
}

// Clear
void LArRawData::DataCRM::Clear(Option_t*)
{
  ChView0->Clear("C");
  ChView1->Clear("C");
  
  NChView0 = 0;
  NChView1 = 0;
}

// Reset
void LArRawData::DataCRM::Reset(Option_t *option)
{
  Clear(option);
  
  delete ChView0; 
  ChView0 = 0;

  delete ChView1; 
  ChView1 = 0;
}


// AddDataChannel
LArRawData::DataChan* LArRawData::DataCRM::AddDataChannel( Int_t iview )
  
{
  LArRawData::DataChan *chan = NULL;
  if(iview == 0)
    {
      TClonesArray &chlist = *ChView0;
      chan = new(chlist[NChView0++]) LArRawData::DataChan();
    }
  else if(iview == 1)
    {
      TClonesArray &chlist = *ChView1;
      chan = new(chlist[NChView1++]) LArRawData::DataChan();
    }
  else
    {
      std::cerr<<"ERROR: Uknown view "<<iview<<" "<<__FILE__<<__LINE__<<std::endl;
    }
   
   return chan;
}

// AddDataChannel
LArRawData::DataChan* LArRawData::DataCRM::AddDataChannel( Int_t iview, Int_t chanid,
							   LArRawData::Dtype dtype )
							   
{
  LArRawData::DataChan *chan = NULL;
  if(iview == 0)
    {
      TClonesArray &chlist = *ChView0;
      chan = new(chlist[NChView0++]) LArRawData::DataChan(chanid, dtype);
    }
  else if(iview == 1)
    {
      TClonesArray &chlist = *ChView1;
      chan = new(chlist[NChView1++]) LArRawData::DataChan(chanid, dtype);
    }
  else
    {
      std::cerr<<"ERROR: Uknown view "<<iview<<" "<<__FILE__<<__LINE__<<std::endl;
    }

  return chan;
}


// AddDataChannel
LArRawData::DataChan* 
LArRawData::DataCRM::AddDataChannel(Int_t iview, Int_t chanid, 
				    LArRawData::Dtype dtype,
				    LArRawData::Code scode)
{
  LArRawData::DataChan *chan = NULL;
  if(iview == 0)
    {
      TClonesArray &chlist = *ChView0;
      chan = new(chlist[NChView0++]) LArRawData::DataChan(chanid, dtype, scode);
    }
  else if(iview == 1)
    {
      TClonesArray &chlist = *ChView1;
      chan = new(chlist[NChView1++]) LArRawData::DataChan(chanid, dtype, scode);
    }
  else
    {
      std::cerr<<"ERROR: Uknown view "<<iview<<" "<<__FILE__<<__LINE__<<std::endl;
    }

  return chan;
}

// get chan
LArRawData::DataChan* LArRawData::DataCRM::GetChan(Int_t iview, Int_t at)
{
  TClonesArray *lst = NULL;
  if(iview == 0)
    lst = ChView0;
  else if(iview == 1)
    lst = ChView1;
  else
    {
      std::cerr<<"ERROR: Uknown view "<<iview<<" "<<__FILE__<<__LINE__<<std::endl;
      return NULL;
    }
  
  return (LArRawData::DataChan*)lst->At(at);
}
