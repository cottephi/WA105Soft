/////////////////////////////////////////////////////////////////////////
//
//   Simple utility to fetch file names of the recently changed
// files in a specified direcrtory.
//   The basic idea is to run unix 'find' command on a specified 
// directory with a particular file name pattern. To pick up files  
// after a certain date a "timestamp" file is created and maintained 
// with the 'touch' command.
// E.g., 
//   find my_dir/ -newer lastread_pedestal -name pedestal_*.dat 
// The file lastread_pedestal is "touchted" after each successful find
//   touch lastread_pedestal -t yyyymmddhhmm.ss
//
// Date: Mon Feb  1 09:52:01 CET 2016 
// Created: vyacheslav.galymov@cern.ch
// Modified: 
//
/////////////////////////////////////////////////////////////////////////

// basic cpp
#include <algorithm>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <cstdlib>

// unix 
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "FileFetcher.h"


//
// ctor 1
//
FileFetcher::FileFetcher() 
{
  fPid      = getpid(); //process id

  fTempDir  = "";
  char const *tmpdir = getenv("TMPDIR");
  if( tmpdir )
    {
      fTempDir = tmpdir;
      if(*fTempDir.rbegin() != '/') fTempDir += "/";
    }
}

//
// dtor
//
FileFetcher::~FileFetcher()
{
  // clean up temporary files
  Cleanup();
}

//
// clean up all the timestamp temporary files
//
void FileFetcher::Cleanup()
{
  std::map<std::string, Product>::iterator it;
  for(it=fProdList.begin();it!=fProdList.end();++it)
    {
      RemoveTmpFile(it->second);
    }
}

//
//
//
void FileFetcher::RemoveTmpFile(Product &prd)
{
  std::string todelete = prd.AccessTimeFile;
  remove(todelete.c_str());
}

//
// add data product
//
void FileFetcher::AddProduct( std::string inputdir, std::string type, std::string fileext,
			      time_t latest, std::string subdir )
{
  std::string pattern = type+"*";
  AddProduct(inputdir, type, pattern, fileext, latest, subdir);
}

//
// add data product
//
void FileFetcher::AddProduct( std::string inputdir, std::string type, std::string spattern,
			      std::string fileext, time_t latest, std::string subdir )
{
  Product prd;
  prd.ProdType    = type; 
  if(!spattern.empty())  prd.FindPattern = spattern;
  else prd.FindPattern = prd.ProdType + "*";

  prd.FileExt  = fileext;
  if(*prd.FileExt.begin() != '.') prd.FileExt = "." + fileext;
  
  // configure in the input directory
  prd.InputDir = inputdir;
  if(*prd.InputDir.rbegin() != '/') prd.InputDir += "/";
  if(!subdir.empty()) prd.InputDir += subdir;
  if(*prd.InputDir.rbegin() != '/') prd.InputDir += "/";
  
  std::stringstream ss;
  if( fTempDir.empty() )
    ss<<prd.InputDir<<".lastaccess_"<<prd.ProdType<<"_pid"<<fPid;
  else
    ss<<fTempDir<<".lastaccess_"<<prd.ProdType<<"_pid"<<fPid;
  
  // our timestamp file
  prd.AccessTimeFile = ss.str();
  
  // partial touch command
  prd.TouchCmd = "touch "+prd.AccessTimeFile+" -t ";

  // form our find command
  ss.clear(); ss.str("");
  ss<<"find "<<prd.InputDir<<" -newer "<<prd.AccessTimeFile
    <<" -name "<<prd.FindPattern<<prd.FileExt
    <<" -printf '%T@\t%p\n' | cut -f 2-";
  prd.FindCmd = ss.str();
  
  // time stamp if == 0 --> now
  prd.lastaccess = latest;
  
  AddToProdList( prd );
  
  // create time stamp file
  //TouchTimestamp(prd);

  // add to the list
  //fProdList.push_back(prd);
}

//
// add to product list
//
void FileFetcher::AddToProdList( Product &prd )
{
  std::map<std::string, Product>::iterator it = fProdList.find(prd.ProdType);
  if(it != fProdList.end() ) // already added ... will update
    {
      // remove temporary file if not in the same place
      if( prd.AccessTimeFile.compare( it->second.AccessTimeFile ) != 0 )
	RemoveTmpFile( it->second );
      it->second = prd;
    }
  else
    {
      fProdList[ prd.ProdType ] = prd;
    }

  // create timestamp file
  TouchTimestamp(prd);
}


//
// update / create time stamp file
//
void FileFetcher::TouchTimestamp(Product &prd)
{
  char buf[20];
  if(prd.lastaccess == 0) //use current time
    {
      prd.lastaccess = time(NULL);
      strftime(buf,sizeof(buf), "%Y%m%d%H%M.%S", localtime( &prd.lastaccess ));
    }
  else
    strftime(buf,sizeof(buf), "%Y%m%d%H%M.%S", localtime( &prd.lastaccess ));

  std::string cmd = prd.TouchCmd + buf;
  
  std::system(cmd.c_str());

  return;
}

//
// get latest files for a given group
//
void FileFetcher::GetFiles(std::string type, std::vector<std::string> &files)
{
  files.clear();
  
  std::map<std::string, Product>::iterator it = fProdList.find(type);
  if(it != fProdList.end() )
    GetFileList( it->second, files );
  else
    std::cerr<<"ERROR: the group type "<<type<<" had not been registered"<<std::endl;
  
  return;
}

//
// get latest files for all registered group types
//
void FileFetcher::GetFiles(std::map< std::string, std::vector<std::string> > &files)
{
  files.clear();
  
  std::map<std::string, Product>::iterator it;
  for(it=fProdList.begin();it!=fProdList.end();++it)
    {
      std::vector<std::string> flist;
      GetFileList(it->second, flist);
      
      if(!flist.empty())
	{
	  files.insert(std::make_pair(it->second.ProdType, flist));
	}
    }
  
  return;
}



//
// get file list for a given om product
//
void FileFetcher::GetFileList(Product &prd, std::vector<std::string> &files)
{
  std::string cmd = prd.FindCmd;
  files.clear();  

  // pipe our command into the file with popen
  FILE *sptr; 
  
  if(!(sptr = popen(cmd.c_str(), "r")))
    {
      return;
    }
  
  time_t timenow = time(NULL);

  // get files if any
  char buf[200];
  while(fgets(buf, sizeof(buf), sptr))
    {
      std::string fname = buf;
      
      // strip any new lines
      fname.erase(std::remove(fname.begin(), fname.end(), '\n'), fname.end());

      // !!! require some extension !!!
      size_t found = fname.find_last_of(prd.FileExt); 
      if(found != std::string::npos)
	files.push_back( fname );
    }
      
  pclose(sptr);

  // update time and timestamp file if found something
  if(!files.empty())
    {
      prd.lastaccess = timenow;
      TouchTimestamp(prd);
    }

  return;
}


//
// dump configurations
//
void FileFetcher::DumpConfig()
{
  std::cout<<"Data Fetcher PID     : "<<fPid<<std::endl;

  std::map<std::string, Product>::iterator it;
  for(it=fProdList.begin();it!=fProdList.end();++it)
    {
      Product prd = it->second;
      std::cout<<std::endl;
      std::cout<<" Product type      : "<<prd.ProdType<<std::endl;
      std::cout<<" Reading directory : "<<prd.InputDir<<std::endl;
      std::cout<<" File extension    : "<<prd.FileExt<<std::endl;
      std::cout<<" Last access file  : "<<prd.AccessTimeFile<<std::endl;
      std::cout<<" Last access time  : "<<prd.lastaccess<<std::endl;
      std::cout<<" Touch command     : "<<prd.TouchCmd<<std::endl;
      std::cout<<" Find command      : "<<prd.FindCmd<<std::endl;
    }
}


