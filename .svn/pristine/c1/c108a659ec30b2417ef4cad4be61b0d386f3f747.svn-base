//////////////////////////////////////////////////////////////////////////////////
//
//  Object to handle channel pedestals
//  It is a singleton object which loads a list of pedestal values
//  from $WA105_CALIBDATA/pedestals file the format should be:
//  seqch crm view view_ch ped pedrms
//
//////////////////////////////////////////////////////////////////////////////////
#include <fstream>
#include <sstream>
#include <iomanip>

#include "ChPedestal.h"
#include "LogMsg.h"

using namespace std;

calidaq::ChPedestal::ChPedestal()
{
  //
  std::string path = GetPath();
  if( !path.empty() )
    {
      string fname = path + calidaq::pedfile + "/" + calidaq::pedfile;
      LoadFile ( fname );
    }
  
  // debug
  //Print();
}


//
//
//
calidaq::ChPedestal::~ChPedestal()
{;}



//
// load file with pedestals
// 
void calidaq::ChPedestal::LoadFile(std::string &fname)
{
  ifstream in(fname.c_str());
  if( !in.is_open() ) return;
  std::string line;
  
  while( getline(in, line ) )
    {
      if(line.empty()) continue;
      
      // strip leading 0 if any
      size_t pos = line.find_first_not_of(' ');
      if( pos == std::string::npos ) 
	continue;
      
      std::stringstream ss(line);

      uint32_t sqch;
      uint16_t icrm, iview, ich;
      float ped, pedrms;
      ss>>sqch>>icrm>>iview>>ich>>ped>>pedrms;
      uint32_t idx = this->mapIt( icrm, iview, ich );
      ssize_t rval = this->addIndx( idx );
      if( rval < -1 )
	{
	  dlardaq::msg_warn<<"Could not add index for "
			   <<icrm<<", "<<iview<<", "<<ich
			   <<" -> "<<idx<<endl;
	}
      else if(rval == -1)
	{
	  m_peds.push_back( std::make_pair( ped, pedrms ) );
	}
      else
	{
	  m_peds.insert( m_peds.begin() + rval, std::make_pair( ped, pedrms ) );
	}
    }
}



//
// get pedestal value
//
float calidaq::ChPedestal::Pedestal( int icrm, int iview, int ich, float &pedrms )
{
  float ped = 0;
  pedrms = 0;

  uint32_t idx = this->mapIt( icrm, iview, ich );
  ssize_t pos  = this->findIt( idx);
  
  if( pos >= 0 )  // found it
    {
      ped    = m_peds[ pos ].first;
      pedrms = m_peds[ pos ].second;
    }
  
  return ped;
}

//
// get pedestal value
//
float calidaq::ChPedestal::Pedestal( int icrm, int iview, int ich )
{
  float ped, pedrms;
  ped = Pedestal( icrm, iview, ich, pedrms );
  return ped;
}

//
//
//
void calidaq::ChPedestal::Print()
{
  cout<<"Loaded pedestals"<<endl;
  cout<<"-->"<<endl;
  for(size_t i=0;i<m_chid.size();i++)
    {
      uint32_t idx = m_chid[i];
      uint16_t icrm, iview, ich;
      this->invmapIt( idx, icrm, iview, ich );
      cout<<setw(10)<<idx
	  <<setw(4)<<icrm
	  <<setw(2)<<iview
	  <<setw(5)<<ich
	  <<setw(10)<<m_peds[i].first
	  <<setw(10)<<m_peds[i].second<<endl;
      
    }
  cout<<"<--"<<endl;
}
