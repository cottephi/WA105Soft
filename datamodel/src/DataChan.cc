#include <iostream>

#include "DataChan.h"

ClassImp(LArRawData::DataChan)

// ctor for ROOT Streamer
LArRawData::DataChan::DataChan()
{
  ChanId     = -1;
  ChanStatus = LArRawData::CHANOFF;
  DataType   = LArRawData::DTID_FLOAT;
  
  DataFrags  = new TClonesArray("LArRawData::DataFragment<Float_t>");

  PedVal = 0;
  PedRMS = 0;
  NFrags = 0;
}

// ctor
LArRawData::DataChan::DataChan(Int_t chanid, LArRawData::Dtype dtype)
{
  ChanId     = chanid;
  ChanStatus = LArRawData::FULLDATA;
  DataType   = dtype;

  DataFrags  = NULL; //new TList(); 

  switch(DataType)
    {
    case LArRawData::DTID_USHORT:
      {
	DataFrags = new TClonesArray("LArRawData::DataFragment<UShort_t>");
	break;
      }
    case LArRawData::DTID_FLOAT:
      {
	DataFrags = new TClonesArray("LArRawData::DataFragment<Float_t>");
	break;
      }
    default:
      {
	std::cout<<"WARNING: Uknown data type using FLOAT"<<std::endl;
	DataType  = LArRawData::DTID_FLOAT;
	DataFrags = new TClonesArray("LArRawData::DataFragment<Float_t>");
	break;
      }
    }

  // pedestal and ped rms are not set by default
  PedVal = 0;
  PedRMS = 0;
  NFrags = 0;
}

// ctor
LArRawData::DataChan::DataChan(Int_t chanid, LArRawData::Dtype dtype,
			       LArRawData::Code scode)
{
  ChanId     = chanid;
  ChanStatus = scode;
  DataType   = dtype;

  DataFrags  = NULL; //new TList(); 

  switch(DataType)
    {
    case LArRawData::DTID_USHORT:
      {
	DataFrags = new TClonesArray("LArRawData::DataFragment<UShort_t>");
	break;
      }
    case LArRawData::DTID_FLOAT:
      {
	DataFrags = new TClonesArray("LArRawData::DataFragment<Float_t>");
	break;
      }
    default:
      {
	std::cout<<"WARNING: Uknown data type using FLOAT"<<std::endl;
	DataType  = LArRawData::DTID_FLOAT;
	DataFrags = new TClonesArray("LArRawData::DataFragment<Float_t>");
	break;
      }
    }

  // pedestal and ped rms are not set by defualt
  PedVal = 0;
  PedRMS = 0;
  NFrags = 0;
}


// dtor
LArRawData::DataChan::~DataChan()
{
  Reset();
}

// 
void LArRawData::DataChan::Clear(Option_t*)
{
  DataFrags->Clear("C");
  
  PedVal = 0;
  PedRMS = 0;
  NFrags = 0;
}

//
void LArRawData::DataChan::Reset(Option_t *option)
{
  Clear(option);
  
  delete DataFrags; 
  DataFrags = 0;
}


//
TObject* LArRawData::DataChan::AddDataFragment()
{
  if(ChanStatus == LArRawData::CHANOFF)
    {
      std::cerr<<"ERROR: Cannot add any data to a channel whose status is OFF"<<std::endl;
      return NULL;
    }
  if( (ChanStatus == LArRawData::FULLDATA || ChanStatus == LArRawData::PEDDATA) &&
      (NFrags == 1) )
    {
      std::cerr<<"ERROR: Need to change status code for this channel to add more than one data fragment"<<std::endl;
      return NULL;
    }
  
  TClonesArray &frags = *DataFrags;
  TObject *frag = NULL;
  
  switch(DataType)
    {
    case LArRawData::DTID_USHORT:
      {
	frag = new(frags[NFrags++]) DataFragment<UShort_t>();
	break;
      }
    case LArRawData::DTID_FLOAT:
      {
	frag = new(frags[NFrags++]) DataFragment<Float_t>();
	break;
      }
    }
  
  return frag;
} 


TObject* LArRawData::DataChan::GetDataFragment(Int_t at)
{
  return DataFrags->At(at);
}

// init fragments
void LArRawData::DataChan::InitFragments(UInt_t fstart, UInt_t fsize)
{
  
  TIter next(DataFrags);
  TObject *obj;
  while( (obj = next()) )
    {
      switch(DataType)
	{
	case LArRawData::DTID_USHORT:
	  {
	    LArRawData::DataFragment<UShort_t> *frag = (LArRawData::DataFragment<UShort_t>*)obj;
	    frag->InitFragment(fstart, fsize);
	    break;
	  }
	case LArRawData::DTID_FLOAT:
	  {
	    LArRawData::DataFragment<Float_t> *frag = (LArRawData::DataFragment<Float_t>*)obj;
	    frag->InitFragment(fstart, fsize);
	    break;
	  }
	}
    }
}	
  
