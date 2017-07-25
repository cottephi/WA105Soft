///////////////////////////////////////////////////////////////////////////////////////
//
// File with results of online analysis 
// Assumed format
//  DDMMYY_HHMMSS
//  etau_vw0 etau_vw0_err etau_vw1 etau_vw1_err (in us)
//  lem_id dqds_vw0 dqds_vw0_err dqds_vw1 dqds_vw1_err
//  ...
//
// CRPAnaSummaryFile::Open() will open a file for RW if it exists
//                           otherwise the file will be opened for W
// Write to different field can be done with an appropriate Write function
// Note that it is not possible to write if a particular datum has already been read
//
///////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <sstream>

#include "CRPAnaSummaryFile.h"

using namespace std;

//
CRPAnaSummaryFile::CRPAnaSummaryFile()
{
  // timestamp format string
  fTFormat = "%d%m%y_%H%M%S";
}

//
CRPAnaSummaryFile::~CRPAnaSummaryFile()
{
  Close();
}

//
time_t CRPAnaSummaryFile::GetTime() const
{
  return fTime;
}


//
int CRPAnaSummaryFile::GetPurity(double &p0, double &pe0, 
				 double &p1, double &pe1) const
{
  p0 = pe0 = p1 = pe1 = 0;
  
  if(fData.size() == 0) return -1;
  
  p0  = fData[0].data[0];
  pe0 = fData[0].data[1];

  p1  = fData[0].data[2];
  pe1 = fData[0].data[3];
  
  return 0;
}


// lem id should start from 1
int CRPAnaSummaryFile::GetCharge(int lemid, double &dq0, double &dq0e, 
				 double &dq1, double &dq1e) const
{
  dq0 = dq0e = dq1 = dq1e = 0;

  if(lemid < 1 || lemid > (int)fData.size()) return -1;
  if(fData[lemid].id < 1) return -1;
  
  dq0  = fData[lemid].data[0];
  dq0e = fData[lemid].data[1];
  dq1  = fData[lemid].data[2];
  dq1e = fData[lemid].data[3];

  return fData[lemid].id;
}


//
void CRPAnaSummaryFile::WriteTime(time_t val)
{
  if(!fFile.is_open())
    {
      cerr<<"No valid file is open"<<endl;
      return;
    }
  if(fTime) 
    {
      cerr<<"Cannot overwrite the existing timestamp values"<<endl;
      return;
    }

  if( fFile.tellg() !=0 || fLines != 0) 
    {
      cerr<<"Can only write timestamp at the beginning"<<endl;
      return;
    }

  fTime = val;

  char mbstr[100];
  strftime(mbstr, sizeof(mbstr), fTFormat.c_str(), localtime(&fTime));
  fFile<<mbstr<<endl;

  fLines++;
}

//
void CRPAnaSummaryFile::WritePurity(double p0, double pe0, double p1, double pe1)
{
  if(!fFile.is_open())
    {
      cerr<<"No valid file is open"<<endl;
      return;
    }

  if(!AddToData(-1, p0, pe0, p1, pe1)) return;
    
  if( fFile.tellg() == 0 || fLines != 1) 
    {
      cerr<<"Can only write purity results after timestamp"<<endl;
      return;
    }
  
  fFile<<fData[0].data[0]<<" "
       <<fData[0].data[1]<<" "
       <<fData[0].data[2]<<" "
       <<fData[0].data[3]<<endl;

  fLines++;
}

//
void CRPAnaSummaryFile::WriteCharge(int lemid, double dq0, double dq0e, double dq1, double dq1e)
{
  if(!fFile.is_open())
    {
      cerr<<"No valid file is open"<<endl;
      return;
    }

  if( fFile.tellg() == 0 || fLines <= 1) 
    {
      cerr<<"Can only write charge results after purity results"<<endl;
      return;
    }

  if(!AddToData(lemid, dq0, dq0e, dq1, dq1e)) return;
  fFile<<fData[lemid].id<<" "
       <<fData[lemid].data[0]<<" "
       <<fData[lemid].data[1]<<" "
       <<fData[lemid].data[2]<<" "
       <<fData[lemid].data[3]<<endl;

  fLines++;  
}


//
bool CRPAnaSummaryFile::AddToData(int id, double d1, double d2, double d3, double d4)
{
  if( id == 0 ) 
    {
      cerr<<"The LEM id should start from 1"<<endl;
      return false;
    }
  int pos = -1;
  
  if( id < 0 ) // purity field
    {
      if(fData.empty())
	{
	  datafield_t dat;
	  dat.id = -1;
	  fData.push_back( dat );
	}
      pos = 0;
    }
  else
    {
      while((int)fData.size()<=id)
	{
	  datafield_t dat;
	  dat.id = -1;
	  fData.push_back( dat );
	}
      pos = id;
    }
  
  if( fData[pos].id != -1 )
    {
      cerr<<"Cannot overwrite the existing values for field "<<pos<<endl;
      return false;
    }
  
  fData[pos].id = pos;
  fData[pos].data[0] = d1;
  fData[pos].data[1] = d2;
  fData[pos].data[2] = d3;
  fData[pos].data[3] = d4;

  return true;
}


//
//
void CRPAnaSummaryFile::Close()
{
  if(fFile.is_open())
    {
      fFile.close();
    }

  fLines    = 0;
  fTime     = 0;
  fData.clear();
}


//
//
void CRPAnaSummaryFile::Open(std::string fname)
{
  Close();
  
  if( fstreamExists( fname.c_str() ) )
    {
      fFile.open( fname.c_str(), std::ios::in | std::ios::out | std::ios::app );
      ReadLines();
    }
  else
    {
      fLines = 0;
      fFile.open( fname.c_str(), std::ios::out );
    }
}


//
//
//
void CRPAnaSummaryFile::ReadLines()
{
  fLines = 0;
  //stringstream ss;
  string line;
  double d1, d2, d3, d4;
  int id;

  while( getline(fFile, line ) )
    {
      if(line.empty()) continue;
      
      stringstream ss(line);
      if( fLines == 0 ) // timestamp line
	{
	  struct tm tm_local;
	  strptime(line.c_str(), fTFormat.c_str(), &tm_local);
	  
	  // get UTC timestamp
	  fTime = mktime(&tm_local);
	}
      else if( fLines == 1 ) // purity field
	{
	  ss>>d1>>d2>>d3>>d4;
	  AddToData(-1, d1, d2, d3, d4);
	}
      else // lem stuff
	{
	  ss>>id>>d1>>d2>>d3>>d4;
	  AddToData(id, d1, d2, d3, d4);
	}
      fLines++;
      
      
      // break when we get to EOF
      if( fFile.peek() == EOF )
	{
	  fFile.clear(); // clear the error code
	  break;
	}
    }
  
  
  //cout<<"Lines read "<<fLines<<" "<<fFile.tellg()<<endl;
}
