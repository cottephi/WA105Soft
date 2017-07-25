/////////////////////////////////////////////////////////////////////////
//
// Simple utility to fetch file names of the recently changed
// files in a specified direcrtory
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

#ifndef __FILEFETCHER_H__
#define __FILEFETCHER_H__

#include <vector>
#include <map>
#include <string>
#include <ctime>



class FileFetcher
{
 private:
  // ctor
  FileFetcher();
  FileFetcher(const FileFetcher&);
  FileFetcher& operator=(const FileFetcher&);
  // Prevent unwanted destruction
  ~FileFetcher();

  
 public:
  static FileFetcher& Instance()
  {
    static FileFetcher inst;
    return inst;
  }
  
  
  // dqtype should be a unique strubg such 
  // one can do wild search: e.g., pedestal_*.dat
  void AddProduct( std::string inputdir, 
		   std::string type, 
		   std::string fileext = ".dat", 
		   time_t latest = 0, 
		   std::string subdir = "" );

  void AddProduct( std::string inputdir, 
		   std::string type, 
		   std::string spattern, 
		   std::string fileext = ".dat", 
		   time_t latest = 0, 
		   std::string subdir = "" );
  

  // get files of a given type
  void GetFiles(std::string type, std::vector<std::string> &files);

  // get files for all registered produces
  void GetFiles(std::map< std::string, std::vector<std::string> > &files);

  
  // print useful info for debugging 
  void DumpConfig();
  
 private:
  
  //
  struct Product
  {
    std::string ProdType;       //product type keyword: e.g., pedestal
    std::string FileExt;        //product file extension: e.g., .dat
    std::string FindPattern;    //input pattern search: e.g., pedestal_*
    std::string InputDir;       //input directory to search
    std::string AccessTimeFile; //file to keep track of last access time
    std::string TouchCmd;       //partial touch command to use for this product
    std::string FindCmd;        //find command to search for the files for this product
    time_t lastaccess;          //unix timestamp 
  };
  
  // methods
  void TouchTimestamp(Product &prd);
  void GetFileList(Product &prd, std::vector<std::string> &files);
  void Cleanup();
  void RemoveTmpFile(Product &prd);
  void AddToProdList( Product &prd );

  // 
  int fPid;               //system process id
  std::string fTempDir;   //directory to store temporary files
  
  // list of products to monitor
  std::map<std::string, Product> fProdList;
};


#endif
