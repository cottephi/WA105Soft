#ifndef _EVENTLOOPER_H_
#define _EVENTLOOPER_H_

#include <TFile.h>
#include <TTree.h>
#include <TF1.h>
#include <TCanvas.h>

#include "Preferences.h"

#include "GeomConfig.h"
#include "Geometry.h"

//
#include "RawDataVirtualMC.h"

//
#include "ChargeDrift.h"
#include "FieldMap.h"


#include "RunHeader.h"
#include "BeamInfo.h"
#include "EventHeader.h"


#include "DLArCRP.h"
#include "DLArLRO.h"


class EventLooper
{

 public:
  EventLooper();
  virtual ~EventLooper();


  // main methods for i/o and looping
  int  FileOpen(const char *fname, const char *evtype = "notused");
  void FileClose(void);
  int  NextEvent(void);


  // ROOT I/O
  void SaveFullRoot(const char* filename="save.fullroot");
  int LoadTree(const char* filename="tree.root");
  void SaveEvent();
  void CloseTree();

   // methods for accessing event counter
  int GetEventIndexNumber() { return iEvent; };
  void SetMaxEventNumber(int max) { iMaxEvent = max; };


  // method to dump geometry configuration file
  void CheckCRPConfig(string file, string token);

  // method to fill event header
  void FillEventHeader();

  // method to fill run header
  void FillRunHeader(const char *fname);
  void WipeEvent();

  EventHeader* theEvHeader(void){ return theEventHeader; }
  RunHeader*   theRHeader(void){ return theRunHeader; }
  BeamInfo*    theBmInfo(void) { return theBeamInfo; }
  GeomConfig*  theGeoCfg(void) { return theGeomConfig; }

  //protected:

 private:

  // the root tree output
  TFile *fOut;

  // event tree
  TTree *treeEV;
 
  // run header tree
  TTree *treeRH;

  int iEvent;
  int iMaxEvent;

  // VMC data handler
  RawDataVirtualMC *DataVMC;

  //
  // Data Classes
  //
  RunHeader*   theRunHeader;
  BeamInfo*    theBeamInfo;
  GeomConfig*  theGeomConfig;
  EventHeader* theEventHeader;

  LArQReadMC::DLArCRP* CrpData;

  LArLReadMC::DLArLRO* LroData;



  //
  // field map and charge drift
  //
  ChargeDrift* Qdrift;
  EFieldMap*   Efield;

};

#endif
