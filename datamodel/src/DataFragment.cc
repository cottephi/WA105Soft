#include <iostream>

#include "DataFragment.h"

templateClassImp(LArRawData::DataFragment)

template <typename T>
LArRawData::DataFragment<T>::DataFragment()
{
  fStart = 0;
  fEnd   = 0;
}

template <typename T>
LArRawData::DataFragment<T>::~DataFragment()
{
  //fData.clear();
  Clear();
}
  
template <typename T>
UInt_t LArRawData::DataFragment<T>::GetData(std::vector<T> &data)
{
  data = fData;
  return fStart;
}

template <typename T>
UInt_t LArRawData::DataFragment<T>::GetDataFloat(std::vector<float> &data) 
{
  data.clear();
  data.assign( fData.begin(), fData.end() );
  return fStart;
}

template <typename T>
void LArRawData::DataFragment<T>::InitFragment(UInt_t fstart, UInt_t fsize)
{
  fStart = fstart;
  fEnd   = fstart + fsize;
  fData.clear();
  fData.resize(fsize);
}

template <typename T>
void LArRawData::DataFragment<T>::AddDatum(UInt_t at, T val, Bool_t overwrite)
{
  
  if(at < fStart || at >= fEnd )
    {
      std::cerr<<"ERROR: array size is exceeded "<<__FILE__<<__LINE__<<std::endl;
      return;
    }

  if(overwrite)
    fData[at-fStart]  = val;
  else
    fData[at-fStart] += val;
}

// add an array at a given position (useful for convolution)
template <typename T>
void LArRawData::DataFragment<T>::AddData(UInt_t at, std::vector<T> data, 
				       Bool_t overwrite)
{
    if(at < fStart || at >= fEnd )
    {
      std::cerr<<"ERROR: array size is exceeded "<<__FILE__<<__LINE__<<std::endl;
      return;
    }

    for(size_t i=0;i<data.size();i++)
      {
	size_t idx = i + fStart;
	if(idx >= fEnd)
	  break;
	
	if(overwrite)
	  fData[at - fStart + i]  = data[i];
	else
	  fData[at - fStart + i] += data[i];
      }
}

template <typename T>
T LArRawData::DataFragment<T>::GetDatum(UInt_t at)
{
  
  if(at < fStart || at >= fEnd )
    {
      std::cerr<<"ERROR: array size is exceeded "<<__FILE__<<__LINE__<<std::endl;
      return NULL;
    }

  return fData[at-fStart];
}


template <typename T>
void LArRawData::DataFragment<T>::SetData( UInt_t fstart, 
					   std::vector<T> &data )
{
  fStart = fstart;
  fData  = data;
  fEnd   = fStart + fData.size();
}


template class LArRawData::DataFragment<Float_t>;
//template class LArRawData::DataFragment<UInt_t>;
template class LArRawData::DataFragment<UShort_t>;
