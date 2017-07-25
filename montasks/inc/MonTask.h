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
#ifndef __MONTASK_H__
#define __MONTASK_H__

#include <TTimer.h>
#include <TCanvas.h>

//#include <TNamed.h>
#include <vector>
#include <string>

class MonTask 
{
 public:
  MonTask();
  virtual ~MonTask();

  const char* GetName() const { return fName.c_str(); }
  const char* GetTaskKey() const { return fTKey.c_str(); }
  
  // names of monitoring tasks
  const std::vector<std::string>& GetItems() const { return fItemList; }
  float GetPollTime() const { return fPollTime; } // seconds

  void SetFileDirectory(std::string dir, std::string fext, std::string spattern = "");
  
  // polling time for new data in seconds
  void SetPollTime( float secs );
  void StartPolling(){ fPollTimer->Start(-1, false); }
  void StopPolling(){ fPollTimer->Stop(); }

  // draw monitoring data in a given canvas
  void Draw( const char *mondata, TCanvas *cnvs );

  // poll data function
  void PollData();
  

 protected:
  //
  std::string fName; // name of the monitoring task (used to find dq files)
  std::string fTKey; // one word hint to its function (used for display in the tree)
  std::string fSEnv; // environment to search 
  std::string fFExt; // file extension to use

  // init
  virtual void Init();

  // the derived classes should overwrite these methods
  virtual void DoFileRead(std::string &fname) = 0;
  virtual void DoDraw( size_t idx, TCanvas *cnvs ) = 0;
  virtual void DoDummyData() = 0;
  
  // could overwrite in the derived class
  virtual bool FilePathFromEnv();

  //
  ssize_t GetFuncIdx( const char *s );

  // polling timer
  TTimer* fPollTimer;
  float   fPollTime;
  
  //
  bool NewCanvas(std::string &str, TCanvas *cnvs);
  
  bool fGotData;
  bool fConfDir;

  std::string fLastDir; //
  
  size_t fCnvsWinWsz;
  size_t fCnvsWinHsz;
  TCanvas* fLastCnvs;

  std::vector<std::string> fItemList;
  
 public:
  ClassDef(MonTask,0);
};


#endif
