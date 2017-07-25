//////////////////////////////////////////////////////////////////////////////////
//
//  This is a very basic manager of the event data
// 
//  Event is the container structure to exchange event information is Event
//  The EventDataManager manipulates branches in the event tree
//  Reception of the data over network is enabled with 
//  by compiling with WITHLIBEVENT2 CCFLAG
//
//////////////////////////////////////////////////////////////////////////////////

#ifndef __RECODATAMANAGER_H__
#define __RECODATAMANAGER_H__

#include <iostream>
#include <vector>
#include <string>
#include <map>

#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TBranchElement.h>
#include <TObjArray.h>
#include <TFolder.h>

#include "RunHeader.h"
#include "BeamInfo.h"
#include "GeomConfig.h"
#include "EventHeader.h"
#include "RecoConfig.h"
#include "RecoObject.h"
#include "DLArCRP.h"
#include "DataLRO.h"

#include "EventDecoder.h"
#include "EventClient.h"
#include "RawDataFormatUtil.h"

//
//
//
class Event
{
 public: 
  Event();
  ~Event();
  
  // possibility to make a cut
  bool PassedCut(){ return !fCut; }
  void SetCut(bool val){ fCut = val; }
  
  //
  const RunHeader*   GetRunHeader() const { return fRunHead; }
  const BeamInfo*    GetBeamInfo() const { return fBeamHead; }
  const GeomConfig*  GetGeoConfig() const { return fGeoConf; }
  
  // event header couldl be modified
  EventHeader* GetEventHeader() { return fEvtHead; }
  
  // CPR data
  const LArRawData::DataCRP* GetCRPData() const { return fQData; }
  const LArVoxel::VoxelInfoCRP* GetVoxelInfo() const { return fVxlInfo; }
  
  // LRO data
  const  LArRawData::DataLRO* GetLROData() const { return fLData; }

  // add data to the branch arrays 
  template<typename T>
  void Add(std::string &brname, std::vector<T> &recoprod)
    {
      std::vector<std::string>::iterator it = find(fAddedBranches.begin(), 
						   fAddedBranches.end(), brname);
      if(it == fAddedBranches.end())
	{
	  std::cerr<<"ERROR: could not find branch "<<brname<<std::endl;
	  return;
	}
      
      TObjArray* arr = fRecoData->operator[](brname);
      int nev = arr->GetEntries();
      for(size_t i=0;i<recoprod.size();i++)
	{
	  recoprod[i].SetObjIndex(nev + (int)i);
	  arr->Add(recoprod[i].clone());
	}
    }
  
  
  // various reco data
  const TObjArray* Get(std::string &brname) const;
  
  friend class EventDataManager;
  
 protected:
  
  //TFolder *fEveRoot;
  
  // run head
  RunHeader *fRunHead;
  
  // beam info
  BeamInfo *fBeamHead; 

  // geo config
  GeomConfig *fGeoConf; 

  // event header
  EventHeader *fEvtHead;
  

  // raw CRP data
  LArRawData::DataCRP *fQData;

  // voxel information
  LArVoxel::VoxelInfoCRP *fVxlInfo;
  
  // light raw data ...
  LArRawData::DataLRO* fLData;

  //
  //std::vector<RecoConfig*> *fRecoConfig;

  // list of added branches to be used with Add
  std::vector<std::string> fAddedBranches;
  
  // reco branches
  std::map<std::string, TObjArray*> *fRecoData;

  bool fCut;
};


//
//  
//
class RunConfig
{
 public:
  
  RunConfig()
    {
      fRunPtr = NULL;
      fGeoPtr = NULL;
    }
  ~RunConfig(){;}
  
  void SetRunHeader( RunHeader *ptr ) { fRunPtr = ptr; };
  void SetCRPConfig( GeomConfig *ptr ) { fGeoPtr = ptr; };
  
  const RunHeader* GetRunHeader() const { return fRunPtr; }
  const GeomConfig* GetCRPConfig() const { return fGeoPtr; }

 private:

  RunHeader *fRunPtr; 
  GeomConfig *fGeoPtr;
};


//
// event manager
//
class EventDataManager 
{
 public:
  EventDataManager();
  ~EventDataManager();
  
  void Open(std::string finput);
  void Open(std::string finput, std::string foutput);
  void Close();

  void ProcessSubSample(Long64_t start, Long64_t nevents);

  // has reco branch of a given name
  bool HasRecoBranch(std::string name);

  // use this function to move through events with RW mode
  Event* NextEvent();
  void EndEvent();

  // use this function to move through events in Read-only mode
  Event* GetEvent(Long64_t ev);

  //// functions to recieve event data from event builder
  //// use these functions only if WITHLIBEVENT2 has been enabled
  void Open( unsigned short port, const char *hostip );
  
  // get event online from event builder
  Event* GetOnlineEvent();
  
  // get status of online connection
  bool ConnectionStatus(); 
  ////////////////////////////////////////////////////////////////////

  Long64_t GetEntries(){ return fNEvents; }
  Long64_t GetCurrentIndex(){ return fCurEvent; }
  
  // add new branches to the tree (the file has to be opened in RW mode)
  void RegisterDataProduct(std::vector< std::string > &brnames);
  
  // add new config branch to run header
  void AddNewConfigBranch(RecoConfig &rconf);
  
  // get subset of run header
  void GetRunConfig(RunConfig &rconf)
  {
    rconf.SetRunHeader( fRunHead );
    rconf.SetCRPConfig( fGeoConf );
  }

  // print information recovered from run config
  void PrintRunConfig();
  

 private:
  void LoadNewEvent();

  enum FileStatus
  { 
    kNOFILE  = 0,
    kBINFILE = 1,
    kROOFILE = 2,
    kNETFILE = 3
  };
  
  FileStatus fInFileStat;
  FileStatus fOutFileStat;

  // root input file
  void OpenRootInput(std::string finput);

  // binary input file with raw data
  void OpenBinaryInput(std::string finput);
    
  void PrintError(int code);

  void InitInRnTree();
  void InitInEvTree();

  void InitOutRnTree();
  void InitOutEvTree();

  void Clear();

  // run header
  RunHeader *fRunHead;

  // beam info
  BeamInfo *fBeamHead; 

  // geo config
  GeomConfig *fGeoConf; 

  // event header
  EventHeader *fEvtHead;

  // raw CRP data
  LArRawData::DataCRP *fQData;

  // raw LRO data
  LArRawData::DataLRO* fLData;
  
  // voxel information
  LArVoxel::VoxelInfoCRP *fVxlInfo;

  // event holder
  Event* fEvent;
  
  // new event to write
  bool fEventToWrite;
  bool fStartEvLoop;
  
  // ROOT I/O files
  TFile *fIn;
  TFile *fOut;

  // for handling raw data files
  RawDataFormatUtil* fRDFormator;
  dlardaq::EventDecoder* fInDecode; 
  
  
  std::string fRnTrName;
  std::string fEvTrName;

  // run tree from input
  TTree *fRunTreeIn;
  // and its output version
  TTree *fRunTreeOut;
  
  // event tree from input
  TTree *fEvTreeIn;
  // and its output version
  TTree *fEvTreeOut;

  Long64_t fNEvents;
  Long64_t fCurEvent;

  // list of newly added branches
  std::vector<TBranch*> fNewDataBr;
  
  //
  std::map<std::string, TObjArray*> fRecoData;
  
  std::vector< std::string > fAddedBranches;

  //
  std::vector<RecoConfig*> fRecoConfig;

  // network event client
#ifdef WITHLIBEVENT2
  dlardaq::EventClient fNetClient;
#endif
};

#endif
