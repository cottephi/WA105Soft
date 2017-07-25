/////////////////////////////////////////////////////////////////////////////////////////
//
//
//  Base class for all monitoring tasks
//
//  Each derived method should provide specify
//  1) fName - unique name associated with the data quality files
//  2) fTKey - key in the tree (does not need to be unique) under which results appear
//  3) fFExt - extension of the data file
//  4) fSEnv - path if want to get files from env variable
//  
//  Basic function is to recover files from specified directory/fName*.fFExt
//
/////////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <unistd.h>

#include "FileFetcher.h"
#include "MonTask.h"

using namespace std;

//
// ctor
//
MonTask::MonTask()
{
  Init();
}

//
void MonTask::Init()
{
  //
  fName = "NONE";
  fTKey = "NONE"; 
  
  fSEnv = "";
  fFExt = ".root";

  fLastDir = "";

  fLastCnvs = NULL;

  // canvas default dimensions
  fCnvsWinWsz = 700;
  fCnvsWinHsz = 500;
  
  fConfDir   = false;
  fGotData   = false;

  fPollTimer = new TTimer();
  fPollTimer->Connect("Timeout()", "MonTask", this, "PollData()");


  SetPollTime( 30 /* seconds */ );
}
		 

//
// dtor
//
MonTask::~MonTask()
{
  delete fPollTimer;
}

//
// set directory to search for files
//
void MonTask::SetFileDirectory(std::string dir, std::string fext, std::string spattern)
{
  if( fConfDir ) 
    {
      cerr<<"Already configured "<<fName<<endl;
      return;      
    }

  // register file pattern
  FileFetcher::Instance().AddProduct( dir, fName, spattern, fext );
  fConfDir = true;

  fFExt = fext;
}

//
// if directory not set can search files based on settings of the environment
//
bool MonTask::FilePathFromEnv()
{
  const char *dir = getenv("THEANALYSISDIR");
  if(!dir) return false;
    
  // 
  if(fSEnv.empty()) return false;
  std::stringstream ss;
  ss<<dir<<"/"<<fSEnv;
  
  // register new directory
  if( fLastDir.compare( ss.str()) != 0 )
    {      
      // directory
      fLastDir = ss.str();
      FileFetcher::Instance().AddProduct( fLastDir, fName, "", fFExt );
      //FileFetcher::Instance().DumpConfig();
    }

  return true;
}


//
//
//
bool MonTask::NewCanvas(std::string &str, TCanvas *cnvs)
{
  std::string ctitle = cnvs->GetTitle();
  if(cnvs == fLastCnvs && (str.compare(ctitle) == 0)) return false;
  
  //fLastCnvsName = str;
  fLastCnvs = cnvs;
  fLastCnvs->SetTitle( str.c_str() );
  fLastCnvs->Clear();

  cnvs->SetWindowSize(fCnvsWinWsz, fCnvsWinHsz);

  return true;   // canvas changed !
}


//
//
//
ssize_t MonTask::GetFuncIdx( const char *s )
{
  ssize_t rval = -1;

  for(size_t i=0;i<fItemList.size();i++)
    {
      if( fItemList[i].compare( s ) == 0 )
	{
	  rval = i;
	  break;
	}
    } 
  
  return rval;
}


//
//
//
void MonTask::Draw( const char *mondata, TCanvas *cnvs )
{
  ssize_t idx = GetFuncIdx( mondata );
  if( idx < 0 )
    {
      cerr<<"Unrecognized plot data "<<mondata<<endl;
      return;
    }
  
  DoDraw( idx, cnvs );
  cnvs->Modified();
  cnvs->Update();
  if( fGotData ) fGotData = false;
}

//
//
//
void MonTask::SetPollTime(float secs)
{
  fPollTime = secs;
  fPollTimer->SetTime( fPollTime*1.0E+3 );
}

//
// poll for new data
//
void MonTask::PollData()
{
  if( fItemList.empty() ) return;

  //DoDummyData();
  //return;
  
  if( !fConfDir )
  {
    if( !FilePathFromEnv() )
      {
	cerr<<"No path to files has been set for "<<fName<<endl;
	return;
      }
  }


  std::vector<std::string> files;
  FileFetcher::Instance().GetFiles( fName, files );
  for(size_t i=0;i<files.size();i++)
    {
      DoFileRead( files[i] );
    }
}
