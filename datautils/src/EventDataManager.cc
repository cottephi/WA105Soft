//////////////////////////////////////////////////////////////////////////////////
//
//  This is a very basic manager of the event data
// 
//  Event is the container structure to exchange event information is Event
//  The EventDataManager manipulates branches in the event tree
//  Reception of the data over network is enabled with 
//  by compiling with WITHLIBEVENT2 CCFLAG
//
//
//////////////////////////////////////////////////////////////////////////////////

//#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <algorithm>

#include "EventDataManager.h"


//
// Event
//
Event::Event()
{
  fRunHead  = NULL;
  fBeamHead = NULL; 
  fGeoConf  = NULL;
  fEvtHead  = NULL;
  
  fQData   = NULL;
  fLData   = NULL;
  fVxlInfo = NULL;

  fCut = false;
}


//
// dtor
//
Event::~Event()
{;}


//
// get data product by branch name
//
const TObjArray* Event::Get(std::string &brname) const
{
  std::map<std::string, TObjArray*>::iterator it;
  it = (*fRecoData).find(brname);
  if(it == (*fRecoData).end())
    return NULL;
  
  return it->second;
}


//
//
// ctor
//
EventDataManager::EventDataManager()
{
  // no active files
  fInFileStat  = kNOFILE;
  fOutFileStat = kNOFILE;

  // number of events and current event
  fNEvents  = 0;
  fCurEvent = -1;
  
  fEventToWrite = false;
  fStartEvLoop  = false;

  fRunHead  = NULL;
  fBeamHead = NULL;
  fGeoConf  = NULL;
  fEvtHead  = NULL;
  fQData    = NULL;
  fLData    = NULL;
  fVxlInfo  = NULL;
  
  // mapping raw data into our CRP strutures
  fRDFormator = NULL;

  // set correct frame dimensions later
  fInDecode = new dlardaq::EventDecoder(1, 1);
  
  // event object
  fEvent = new Event();

  // tree branches
  fRnTrName  = "RunHeader";
  fEvTrName  = "EventTree";
}

//
// dtor
//
EventDataManager::~EventDataManager()
{
  // see one needs to close 
  Close();

  if(fRunHead)  delete fRunHead;
  if(fBeamHead) delete fBeamHead;
  if(fGeoConf)  delete fGeoConf;
  if(fEvtHead)  delete fEvtHead;
  if(fQData)    delete fQData;
  if(fLData)    delete fLData;
  if(fVxlInfo)  delete fVxlInfo;
  
  if(fRDFormator) delete fRDFormator;
  delete fInDecode;

  delete fEvent;
}


//
// open file for reading
//
void EventDataManager::Open(std::string finput)
{
  // if have active files close them
  if( fInFileStat | fOutFileStat ) Close();
  
  // no output file
  fOut = NULL;
  
  // check if we have a correct extension
  std::string fext = ".root";
  bool rootinput;
  if( fext.size() > finput.size() ) 
    rootinput = false;  
  else
    rootinput = std::equal( fext.rbegin(), fext.rend(), finput.rbegin() );
  
  // open file for reading
  if( rootinput )
    OpenRootInput( finput );
  else
    OpenBinaryInput( finput );
}

//
//
//
void EventDataManager::Open( unsigned short port, const char *hostip )
{
#ifdef WITHLIBEVENT2
  if( fInFileStat | fOutFileStat ) Close();
  
  // net input
  fInFileStat  = kNETFILE;

  // connect client
  fNetClient.Connect( port, hostip );

  if(!fRunHead) fRunHead = new RunHeader();
  fEvent->fRunHead = fRunHead;
  if(!fGeoConf) fGeoConf = new GeomConfig();
  fEvent->fGeoConf = fGeoConf;
  if(!fEvtHead) fEvtHead = new EventHeader();
  if(!fQData) fQData = new LArRawData::DataCRP(); 

  if(!fRDFormator) delete fRDFormator;
  fRDFormator = new RawDataFormatUtil( fRunHead, fGeoConf,
				       fEvtHead, fQData );
  
  // total number of events for the moment
  fNEvents   = 0;
  fCurEvent = -1;

  // set detector properties
  dlardaq::runheader_t rh;
  rh.run_num = 999; //dummy run number for now to fill later
  fRDFormator->FillRun(rh);
  
  // set correct event dimensions for decoding
  fInDecode->SetNCh( fRDFormator->GetNCh() );
  fInDecode->SetNSample( fRDFormator->GetNSample() );
  fNetClient.SetDecoder( fInDecode );
#else
  std::cerr<<"The network client is not supported. Recompile with WITHLIBEVENT2 flag"<<std::endl;
#endif
}

//
// open root file input
//
void EventDataManager::OpenRootInput( std::string finput )
{
  
  fInFileStat = kROOFILE;
  fIn  = TFile::Open( finput.c_str() );
  if(!fIn->IsOpen())
    {
      std::cerr<<"ERROR: could not open input file "<<finput<<std::endl;
      fInFileStat = kNOFILE;
      abort();
    }
  
  fCurEvent = 0;
  fStartEvLoop  = false;

  // init trees
  InitInRnTree();
  InitInEvTree();  
}

//
// open raw data file input
//
void EventDataManager::OpenBinaryInput( std::string finput )
{
  fInFileStat = kBINFILE;
  
  if( fInDecode->Open( finput ) < 0)
    {
      std::cerr<<"ERROR: could not open input file "<<finput<<std::endl;
      fInFileStat = kNOFILE;
      abort();
    }

  if(!fRunHead) fRunHead = new RunHeader();
  fEvent->fRunHead = fRunHead;
  if(!fGeoConf) fGeoConf = new GeomConfig();
  fEvent->fGeoConf = fGeoConf;
  if(!fEvtHead) fEvtHead = new EventHeader();
  if(!fQData) fQData = new LArRawData::DataCRP(); 

  if(!fRDFormator) delete fRDFormator;
  fRDFormator = new RawDataFormatUtil( fRunHead, fGeoConf,
				       fEvtHead, fQData );
  
  // get total number of events
  fNEvents = fInDecode->GetTotEvents();

  // set detector properties
  dlardaq::runheader_t rh = fInDecode->GetRunHeader();
  fRDFormator->FillRun(rh);
  
  // set correct event dimensions for decoding
  fInDecode->SetNCh( fRDFormator->GetNCh() );
  fInDecode->SetNSample( fRDFormator->GetNSample() );
}


//
//
//
void EventDataManager::Open(std::string finput, std::string foutput)
{
  // if have active files close them
  if( fInFileStat | fOutFileStat ) Close();

  // open input for reading
  Open( finput );

  fOutFileStat = kROOFILE;
  
  // open file for writing
  fOut = TFile::Open( foutput.c_str(), "RECREATE" );
  if(!fOut->IsOpen())
    {
      std::cerr<<"ERROR: could not open output file "<<foutput<<std::endl;
      fOutFileStat = kNOFILE;
      abort();
    }

  // init trees
  InitOutRnTree();
  InitOutEvTree();  

  fEventToWrite = false;
}

//
// Close
//
void EventDataManager::Close()
{
  //if(fFileStatus == -1) return;

  if(fInFileStat == kNOFILE && 
     fOutFileStat == kNOFILE) return;

  // 
  if( fInFileStat == kBINFILE ) // close binary
    {
      fInDecode->Close();
    }
  else if( fInFileStat == kROOFILE ) // close ROOT file
    {
      fIn->Close();
      fIn = NULL;
    }
  else if( fInFileStat == kNETFILE ) // close network connection
    {
      #ifdef WITHLIBEVENT2
      fNetClient.Stop();
      #endif
    }
  

  // close output stream
  if(fOutFileStat != kNOFILE)
    {
      EndEvent();
      fOut->cd();
      fRunTreeOut->Write("",TObject::kOverwrite);
      fEvTreeOut->Write("",TObject::kOverwrite);
      fOut->Close();
      fOut = NULL;
    }

  // remove from memory
  while(!fRecoConfig.empty())
    {
      delete fRecoConfig.back();
      fRecoConfig.pop_back();
    }

  // clean up the map
  std::map<std::string, TObjArray*>::iterator it;
  for ( it = fRecoData.begin(); it != fRecoData.end(); it++) 
    delete it->second;

  //
  fRecoData.clear();
  
  //
  fNewDataBr.clear();
  fAddedBranches.clear();

  // set file status flag
  fInFileStat  = kNOFILE;
  fOutFileStat = kNOFILE;
}


//
//  evstart = -1, nev = -1 will defaults to process all events
// 
void EventDataManager::ProcessSubSample(Long64_t start, Long64_t nevents)
{
  if(fStartEvLoop) 
    {
      std::cerr<<"ERROR: can only call this function befor starting event loop"<<std::endl;
      return;
    }

  // default values are 
  //   fCurEvent = 0
  //   fNEvents  = Number of entries in the tree
  
  if(  start >= 0 && start < fNEvents ) 
    fCurEvent = start;
  
  if( nevents > 0 && fCurEvent + nevents < fNEvents ) 
    fNEvents = fCurEvent + nevents;
}



//
// init run tree
//
void EventDataManager::InitInRnTree()
{
  fRunTreeIn = NULL;
  fRunTreeIn = (TTree*)fIn->Get(fRnTrName.c_str());
  if(!fRunTreeIn)
    {
      std::cerr<<"ERROR: run tree "<<fRnTrName<<" could not be found"<<std::endl;
      return;
    }
  
  // get run config branches
  TObjArray *myBranches = fRunTreeIn->GetListOfBranches();
      
  TIter next(myBranches);
  TObject *obj;
  while( (obj = next()) )
    {
      std::string objname = obj->GetName(); 
      if(objname.compare("theRunHeader") == 0)
	{
	  std::cout<<" -> has run header "<<std::endl;
	  if(!fRunHead) fRunHead = new RunHeader();
	  fRunTreeIn->SetBranchAddress(objname.c_str(), &fRunHead);

	  //
	  fEvent->fRunHead = fRunHead;
	}
      else if (objname.compare("theBeamInfo") == 0)
	{
	  std::cout<<" -> has beam info "<<std::endl;
	  if(!fBeamHead) fBeamHead = new BeamInfo();
	  fRunTreeIn->SetBranchAddress(objname.c_str(), &fBeamHead);

	  //
	  fEvent->fBeamHead = fBeamHead; 
	}
      else if (objname.compare("theGeomConfig") == 0)
	{
	  std::cout<<" -> has geom config "<<std::endl;
	  if(!fGeoConf) fGeoConf = new GeomConfig();
	  fRunTreeIn->SetBranchAddress(objname.c_str(), &fGeoConf);
	  
	  //
	  fEvent->fGeoConf = fGeoConf;
	}
      else if (objname.find("theRecoConfig") == 0 ) //e.g., theRecoConfigHit, theRecoConfigTrack2d, ...
	{
	  std::cout<<" -> has "<<objname<<std::endl;
	  fRecoConfig.push_back(new RecoConfig());
	  //RecoConfig *rconf = fRecoConfig.back();
	  //fRunTreeIn->SetBranchAddress(objname.c_str(), &rconf);
	}
      else
	{
	  std::cout<<"WARNING: unused branch in run header "<<objname<<std::endl;
	}
      
    }
 
  // get content of the the run header tree
  fRunTreeIn->GetEvent(0); 
}


//
// init output run tree
//
void EventDataManager::InitOutRnTree()
{
  // just clone the tree
  if( fInFileStat == kROOFILE )
    {
      fOut->cd();
      fRunTreeOut = fRunTreeIn->CloneTree();
    }
  else // raw data binary
    {
      fOut->cd();
      fRunTreeOut = new TTree(fRnTrName.c_str(), fRnTrName.c_str());
      fRunTreeOut->Branch("theRunHeader","RunHeader", &fRunHead);  
      fRunTreeOut->Branch("theGeomConfig","GeomConfig", &fGeoConf);
      // we should have the valid configurations by now
      fRunTreeOut->Fill(); 
    }
}



//
// init event tree
//
void EventDataManager::InitInEvTree()
{
  
  fEvTreeIn = NULL;
  fEvTreeIn = (TTree*)fIn->Get(fEvTrName.c_str());
  if(!fEvTreeIn)
    {
      std::cerr<<"ERROR: run tree "<<fEvTrName<<" could not be found"<<std::endl;
      return;
    }

  fNEvents  = fEvTreeIn->GetEntries();

  // get event branches
  TObjArray *myBranches = fEvTreeIn->GetListOfBranches();

  TIter next(myBranches);
  TObject *obj;
  while( (obj = next()) )
    {
      std::string objname = obj->GetName(); 
      if(objname.compare("theEventHeader") == 0)
	{
	  std::cout<<" -> has event header "<<std::endl;

	  fEvTreeIn->SetBranchStatus(objname.c_str(), 1);
	  if(!fEvtHead) fEvtHead = new EventHeader();
	  
	  fEvTreeIn->SetBranchAddress(objname.c_str(), &fEvtHead);
	}
      else if(objname.compare("CRPRawData") == 0)
	{
	  std::cout<<" -> has crp data "<<std::endl;
	  if(!fQData) fQData = new LArRawData::DataCRP(); 
	  fEvTreeIn->SetBranchAddress(objname.c_str(), &fQData);
	}
      else if(objname.compare("LRORawData") == 0)
	{
	  std::cout<<" -> has lro data "<<std::endl;
	  if(!fLData) fLData = new LArRawData::DataLRO();
	  fEvTreeIn->SetBranchAddress(objname.c_str(), &fLData);
	}
      else if(objname.compare("CRPVoxelData") == 0)
	{
	  std::cout<<" -> has voxel data "<<std::endl;
	  if(!fVxlInfo) fVxlInfo = new LArVoxel::VoxelInfoCRP();
	  fEvTreeIn->SetBranchAddress(objname.c_str(), &fVxlInfo);
	}
      else
	{
	  TBranchElement *be = dynamic_cast<TBranchElement*>(obj);
	  std::string objclass = be->GetClassName();
	  if(objclass.compare("TObjArray") == 0)
	    {
	      fEvTreeIn->SetBranchStatus(objname.c_str(), 1);
	      fRecoData[objname] = new TObjArray();
	      fEvTreeIn->SetBranchAddress(objname.c_str(), &fRecoData[objname]);
	    }
	  else
	    std::cerr<<"ERROR: cannot handle "<<objclass<<" in "
		     <<objname<<" branch "<<std::endl;
	}
    }
}


//
// output branches
//
void EventDataManager::InitOutEvTree()
{
  fOut->cd();
  fEvTreeOut = new TTree(fEvTrName.c_str(), fEvTrName.c_str());
  
  // add event header branch
  fEvTreeOut->Branch("theEventHeader", "EventHeader", &fEvtHead);

  // also copy existing reconstruction objects
  if( fInFileStat == kROOFILE )
    {
      std::map<std::string, TObjArray*>::iterator it;
      for(it = fRecoData.begin();it!=fRecoData.end();it++)
	{
	  std::string objname = it->first; 
	  fEvTreeOut->Branch(objname.c_str(), &fRecoData[objname]);
	}
    }
}


//
//
//
void EventDataManager::RegisterDataProduct(std::vector< std::string > &brnames)
{
  if(fStartEvLoop)
    std::cerr<<"ERROR: cannot add more data branches at this point"<<std::endl;

  if(fOutFileStat == kNOFILE)
    {
      //PrintError( fFileStatus );
      //return;
      cout<<"WARNING: EventDataManager::RegisterDataProduct now output file has been set"<<endl;
    }

  // check if we already have this branch
  int count = 0;
  std::map<std::string, TObjArray*>::iterator it;
  for(size_t i=0;i<brnames.size();i++)
    {
      it = fRecoData.find( brnames[i] );
      if( it != fRecoData.end() )
	{
	  std::cerr<<"ERROR: Branch with name '"<<brnames[i]<<"' already exists"<<std::endl;
	  std::cerr<<"       Cannot add multiple branches with the same name"<<std::endl;
	  continue;
	}
      
      fAddedBranches.push_back( brnames[i] );
      fRecoData[ brnames[i] ] = new TObjArray();
      fRecoData[ brnames[i] ]->SetOwner(true);

      if( fOutFileStat == kROOFILE )
	{
	  TBranch* br = fEvTreeOut->Branch(brnames[i].c_str(), "TObjArray", &fRecoData[ brnames[i] ]);
	  fNewDataBr.push_back(br);
	}
   
      count++;
    }

  if(!count)
    std::cerr<<"ERROR: failed to add new branches"<<std::endl;
  
  // add new reco config branch to list
}


//
//
//
void EventDataManager::AddNewConfigBranch(RecoConfig &rconf)
{
  //if(fFileStatus < 1)
  //{
  //PrintError( fFileStatus );
  //return;
  //}

  if(fOutFileStat == kNOFILE)
    cout<<"WARNING: EventDataManager::AddNewConfigBranch now output file has been set"<<endl;
      

  std::string str1 = rconf.GetTaskName();

  // see we already have one 
  for(size_t i=0;i<fRecoConfig.size();i++)
    {
      std::string str2 = fRecoConfig[i]->GetTaskName();
      if(str1 == str2)
	{
	  std::cout<<"Configuration for "<<str1
		   <<" already exists. Cannot add it to run header"<<std::endl;
	  return;
	}
    }
  
  RecoConfig *rconfptr = new RecoConfig(rconf);
  fRecoConfig.push_back( rconfptr );
  
  // add it to tree (TODO: investigate tree friends for future)
  const char *brname = Form("theRecoConfig_%s",str1.c_str());

  if( fOutFileStat == kROOFILE )
    {
      TBranch *br = fRunTreeOut->Branch(brname, &rconfptr);
      br->Fill();      
    }
}


//
// use this function to retreive events
//
Event* EventDataManager::GetEvent(Long64_t ev)
{
  if( fInFileStat == kNOFILE || fInFileStat == kNETFILE ) return NULL;
  if( ev >= fNEvents ) return NULL;

  fCurEvent = ev;  
  LoadNewEvent();

  return fEvent;
}


//
//
//
Event* EventDataManager::GetOnlineEvent()
{
#ifdef WITHLIBEVENT2
  if(fInFileStat != kNETFILE)
    {
      std::cerr<<"No valid connection parameters has been given"<<std::endl;
      return NULL;
    }
  
  // get event data if any
  dlardaq::evheader_t eh;
  std::vector<dlardaq::adc16_t> DataAdc;
  ssize_t rval = fInDecode->GetEvent( eh, DataAdc );
  if(rval < 0) return NULL; //no new event recieved

  // new event, so get run header info
  dlardaq::runheader_t rh = fInDecode->GetRunHeader();
  fRunHead->SetRunNumber( (int)(rh.run_num) );

  // fill event data
  fRDFormator->FillEvent( eh, DataAdc );
  fCurEvent = eh.ev_num;
  
  // copy pointers
  LoadNewEvent();

  return fEvent;
#else
  std::cerr<<"DO NOT use this function"<<std::endl;
  return NULL;
#endif
}

//
// go to next event, use it to move between events 
// in sequential way
//
Event* EventDataManager::NextEvent()
{
  if( fInFileStat == kNOFILE || fInFileStat == kNETFILE ) return NULL;
  
  // lock branch additions at this point
  if(!fStartEvLoop)
    {
      fStartEvLoop = true;
      if(fCurEvent<0) fCurEvent = 0;
      fEvent->fAddedBranches = fAddedBranches;
    }

  // call end event for previous event if this was not done
  if(fEventToWrite)
    EndEvent();
  
  if( fCurEvent >= fNEvents ) 
    return NULL;
  
  // write any output
  fEventToWrite = true;
  
  LoadNewEvent();
  fCurEvent++;
  
  return fEvent;
}

//
//
//
void EventDataManager::LoadNewEvent()
{
  // get new entry
  if( fInFileStat == kROOFILE )
    {
      fEvTreeIn->GetEntry( fCurEvent );
    }
  else if(fInFileStat == kBINFILE)
    {
      dlardaq::evheader_t eh;
      std::vector<dlardaq::adc16_t> DataAdc;
      fInDecode->GetEvent( fCurEvent, eh, DataAdc );
      fRDFormator->FillEvent( eh, DataAdc );
    }
    
  if(fEvtHead) fEvent->fEvtHead = fEvtHead;  
  if(fQData) fEvent->fQData = fQData;
  if(fLData) fEvent->fLData = fLData;
  if(fVxlInfo) fEvent->fVxlInfo = fVxlInfo;
  fEvent->fRecoData = &fRecoData;

}

//
// end event to peform fill operations
//
void EventDataManager::EndEvent()
{
  if(!fEventToWrite) return;

  if( fOutFileStat != kROOFILE )
    {
      std::cerr<<"ERROR: EventDataManager::EndEvent() cannot be called in read-only mode"<<std::endl;
      return;
    }
  
  fEventToWrite = false;
  
  if(fEvent->PassedCut()) 
    fEvTreeOut->Fill();

  // prepare for next cycle
  Clear();
}


//
//
//
void EventDataManager::Clear()
{
  for(size_t i=0;i<fAddedBranches.size();i++)
    fRecoData[ fAddedBranches[i] ]->Clear();
}


//
// print various errors
//
void EventDataManager::PrintError(int ecode)
{
  
  if(ecode == -1)
    std::cerr<<"ERROR: no open file is found"<<std::endl;
  else if(ecode == 0)
    std::cerr<<"ERROR: file is open in READ-ONLY mode"<<std::endl;
  else if(ecode >= 1)
    std::cerr<<"ERROR: GetEvent() function could only be used in read only mode"<<std::endl;
  
  return;
}


//
// print information in the fRnTrName
//
void EventDataManager::PrintRunConfig()
{
  if( fInFileStat != kROOFILE ) return;

  fRunHead->DumpRunHeader();
  fBeamHead->DumpBeamInfo();
  fGeoConf->DumpGeomConfig();

  for(size_t i=0;i<fRecoConfig.size();i++)
    fRecoConfig[i]->Print();
    
}

//
//
//
bool EventDataManager::HasRecoBranch(std::string name)
{

  std::map<std::string, TObjArray*>::iterator it;
  it = fRecoData.find(name);
  if(it == fRecoData.end())
    return false;
  
  return true;
}


//
//
//
bool EventDataManager::ConnectionStatus()
{
#ifdef WITHLIBEVENT2
  return fNetClient.Connected();
#endif 
  return false;  
}
