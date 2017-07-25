//////////////////////////////////////////////////////////////////////////////////
//
//  Object to handle channel status mask 
//  It is a singleton object which loads a list of masked channel
//  from $WA105_CALIBDATA/chmasked file the format should be:
//  seqch crm view view_ch
//
//////////////////////////////////////////////////////////////////////////////////
#include <fstream>
#include <sstream>
#include <iomanip>

#include "ChMask.h"
#include "LogMsg.h"

using namespace std;

calidaq::ChMask::ChMask()
{
  //
  std::string path = GetPath();
  if( !path.empty() )
    {
      string fname = path + calidaq::chmaskfile + "/" + calidaq::chmaskfile;
      LoadFile ( fname );
    }

  // debug
  //Print();
}


//
//
//
calidaq::ChMask::~ChMask()
{;}



//
// load file with masked channels
// 
void calidaq::ChMask::LoadFile(std::string &fname)
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
      ss>>sqch>>icrm>>iview>>ich;
      
      uint32_t idx = this->mapIt( icrm, iview, ich );
      ssize_t rval = this->addIndx( idx );
      if( rval < -1 )
	{
	  dlardaq::msg_warn<<"Could not add index for "
			   <<icrm<<", "<<iview<<", "<<ich
			   <<" -> "<<idx<<endl;
	}
    }
}



//
//
//
bool calidaq::ChMask::Masked( int icrm, int iview, int ich )
{
  uint32_t idx = this->mapIt( icrm, iview, ich );
  ssize_t pos  = this->findIt( idx );
  
  bool masked = true;

  // if not found, the channel is not masked
  if( pos < 0 ) masked = !masked;
  
  return masked;
}


//
//
//
void calidaq::ChMask::Print()
{
  cout<<"Loaded channel mask"<<endl;
  cout<<"-->"<<endl;
  for(size_t i=0;i<m_chid.size();i++)
    {
      uint32_t idx = m_chid[i];
      uint16_t icrm, iview, ich;
      this->invmapIt( idx, icrm, iview, ich );
      cout<<setw(10)<<idx
	  <<setw(4)<<icrm
	  <<setw(2)<<iview
	  <<setw(5)<<ich<<endl;
    }
  cout<<"<--"<<endl;
}
