////////////////////////////////////////////////////////////////////////////
//
//   Charge truth information for a given channel
//   All the added charge should be in fC  
//
//   Created: Mon Oct 19 14:37:06 CEST 2015
//
//
////////////////////////////////////////////////////////////////////////////

#include <iomanip>
#include <vector>

#include "ChanTrueQ.h"

ClassImp(LArMCTruth::ChanTrueQ)

using namespace std;
using namespace LArMCTruth;

// ctor 1
ChanTrueQ::ChanTrueQ()
{
  fCh = 0;
  fStart = fEnd = 0;
  fQtot = 0;
}

// ctor 2
ChanTrueQ::ChanTrueQ(UInt_t ch)
{
  fCh = ch;
  fStart = fEnd = 0;
  fQtot = 0;
}
  
// dtor
ChanTrueQ::~ChanTrueQ()
{;}
      
  
// Print 
void ChanTrueQ::Print()
{
  cout<<endl<<"True charge in channel "<<fCh<<" is "<<fQtot<<" fC"<<endl;
  cout<<"Fragment starts at "<<fStart<<" and ends at "<<fEnd<<endl;
  for(size_t i=0;i<fData.size();i++)
    {
      cout<<setw(5)<<i<<setw(7)<<fStart+i<<setw(15)<<fData[i]<<endl;
    }
  cout<<"--"<<endl;
}

// set data vector
void ChanTrueQ::SetData(UInt_t tstart, std::vector<Float_t> &data)
{
  //any existing data will be overwritten
  fStart = tstart;
  fData  = data;
  fEnd   = fStart + fData.size()-1;    
  SumCharge();
}

// add data vector
void ChanTrueQ::AddData(UInt_t at, std::vector<Float_t> &data)
{
  if(fData.empty()) //same as SetData if vector is empty
    {
      SetData(at, data);      
    }
  else if(at > fStart)
    {
      for(size_t i=0;i<data.size();i++)
	{
	  AddDatum(at+i, data[i]);
	}
    }
  else
    {
      std::cerr<<"ERROR: cannot add a new data vector "
	       <<__FILE__<<" "<<__LINE__<<std::endl;
      std::cerr<<"       at the position "<<at<<" smaller than start "<<fStart<<endl;
    }
  
  return;
}
  
// add data point
void ChanTrueQ::AddDatum(UInt_t at, Float_t val)
{
  if(fData.empty())
    {
      fStart = at;
      fData.push_back(val);
      fEnd = fStart + fData.size() - 1;
      SumCharge();
    }
  else if(at >= fStart && at <= fEnd) //within existing bounds
    {
      fData[at - fStart] += val;
      
      // increment total charge
      fQtot += val;
    }
  else if(at > fEnd) // expand the bounds
    {
      size_t newsize = at - fStart + 1;
      fData.resize( newsize );
      fData[at - fStart] = val;
      fEnd = at;

      //increment total charge
      fQtot += val;
    }
  else
    {
      std::cerr<<"ERROR: data insertion at the fornt is not supported "
	       <<__FILE__<<" "<<__LINE__<<std::endl;
    }
  
  return;
}

// sum charge
void ChanTrueQ::SumCharge()
{
  fQtot = 0;
  for(size_t i=0;i<fData.size();i++)
    fQtot += fData[i];
}
  
  
