////////////////////////////////////////////////////////////////////////////
//
//   Truth information of the charge mapped to the anode
//  
//
////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <iomanip>

#include "AnodeTrueQ.h"

ClassImp(LArMCTruth::AnodeTrueQ)

using namespace std;
using namespace LArMCTruth;


//
AnodeTrueQ::AnodeTrueQ()
{
  fQView0 = fQView1 = 0;
}


//
AnodeTrueQ::~AnodeTrueQ()
{
  Clear();
}
 
 
//
void AnodeTrueQ::Clear()
{
  fQView0 = 0;
  fQView1 = 0;

  // clear the memory
  while(!fAnodeQView0.empty())
    {
      delete fAnodeQView0.back();
      fAnodeQView0.pop_back();
    }

  // clear the memory
  while(!fAnodeQView1.empty())
    {
      delete fAnodeQView1.back();
      fAnodeQView1.pop_back();
    }
}
    

// 
void AnodeTrueQ::AddChanData(ChanTrueQ &ChQ, int iView)
{
  //to save time not checking whether there is already channel data

  if(iView == 0) 
    {
      fQView0 += ChQ.GetQ();
      fAnodeQView0.push_back(new ChanTrueQ(ChQ));
    }
  else if(iView == 1)
    {
      fQView1 += ChQ.GetQ();
      fAnodeQView1.push_back(new ChanTrueQ(ChQ));
    }
}

//
bool AnodeTrueQ::IsValid() const
{
  bool rval = true;
  rval = rval && (!fAnodeQView0.empty() && !fAnodeQView1.empty());
  rval = rval && (fQView0 != 0 && fQView1 != 0);
  return rval;
}

//
void AnodeTrueQ::Print() const
{
  cout<<"Truth information for view 0 : "<<fQView0<<" fC"<<endl;
  for(size_t i=0;i<fAnodeQView0.size();i++)
    {
      fAnodeQView0[i]->Print();
    }


  cout<<"Truth information for view 1 : "<<fQView1<<" fC"<<endl;
  for(size_t i=0;i<fAnodeQView1.size();i++)
    {
      fAnodeQView1[i]->Print();
    }
  
}

// assignment operator
AnodeTrueQ& AnodeTrueQ::operator=(const AnodeTrueQ& rhs)
{
  if(this != &rhs)
    {
      Clear();

      fQView0 = rhs.fQView0;
      fQView1 = rhs.fQView1;
      
      fAnodeQView0 = rhs.fAnodeQView0;
      fAnodeQView1 = rhs.fAnodeQView1;
    }
  
  return *this;
}
