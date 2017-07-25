//////////////////////////////////////////////////////////////////////////////////////////////
//
//  Simple viewer for reconstructed information for two plane views
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __RECOVIEWER_H__
#define __RECOVIEWER_H__

//ROOT GUI classes
#include <TGFrame.h>
#include <TGLayout.h>
#include <TGMenu.h>
#include <TGButton.h>
#include <TGButtonGroup.h>
#include <TGComboBox.h>
#include <TGTextEntry.h>
#include <TGNumberEntry.h>
#include <TRootEmbeddedCanvas.h>
#include <TGListTree.h>
#include <TGCanvas.h>
#include <TPad.h>
#include <TFrame.h>
#include <TCanvas.h>
#include <TROOT.h>

#include <TH2F.h>
#include <TGraph.h>

#include <vector>
#include <string>

//#include "EventDataManager.h"

class Event;
class EventDataManager;

enum {kCRPSel0 = 600, kCRPSel1 = 700};

class RecoViewer : public TGMainFrame
{
 private:
  TGMainFrame        *fMain;
  
  
  //
  //TGTextEntry       *fTEDirPath;
  
  //text buttons
  //TGTextButton      *fBConnect;
  //TGTextButton      *fBNewCanvas;
  
  // move between events
  TGButtonGroup     *fSWGrp;
  TGTextButton      *fBPrev;
  TGTextButton      *fBNext;

  // exit
  TGTextButton      *fBExit;
  
  //
  TGComboBox        *fCmbBoxCRPView0;
  TGComboBox        *fCmbBoxCRPView1;
  
  TRootEmbeddedCanvas *fCnvsView0;
  TRootEmbeddedCanvas *fCnvsView1;


  //
  Bool_t fWinUp;
  
  // event manager
  EventDataManager *fEvMgr; 
  
  bool fHasHits;
  bool fHasTracks;
  
  std::vector<std::string> fHitBrNames;
  std::vector<std::string> fTrkBrNames;

  // 
  Long64_t fCurEvent;
  Long64_t fEntries;

  Int_t fAllCrp;
  Int_t fSelCRMView0;
  Int_t fSelCRMView1;
  
  // 
  float fAxisBinSize; 
  std::vector< TH2F* > fAxisView0;
  std::vector< TH2F* > fAxisView1;


  // hits in all CRMs
  std::vector< TGraph* > fHitsView0;
  std::vector< TGraph* > fHitsView1;

  // trks in CRP
  std::vector< TGraph* > fTrksView0;
  std::vector< TGraph* > fTrksView1;

  // track hits
  std::vector< std::vector< TGraph* > > fTrkHitsView0;
  std::vector< std::vector< TGraph* > > fTrkHitsView1;
  
  // delta ray tagged hits
  std::vector< std::vector< TGraph* > > fTrkDrHitsView0;
  std::vector< std::vector< TGraph* > > fTrkDrHitsView1;

  void Refresh( int iview );
  void RefreshView0();
  void RefreshView1();
  
  //void Clear();
  void Exit();
  
  void NewFileInit();
  
  void GraphHits( Event* event, int iview, std::vector< TGraph* > &objs );
  void GraphTracks( Event* event, int iview, std::vector< TGraph* > &objs );
  void GraphTrackHits( Event *event, int iview,
		       std::vector< std::vector< TGraph* > > &objs1,
		       std::vector< std::vector< TGraph* > > &objs2 );
  // build new axis
  struct Rectangle
  {
    float x1;
    float x2;
    float y1;
    float y2;
  };

  // axis builder
  void BuildAxis( std::vector< Rectangle > dims, std::vector< TH2F* > &axis, int iview);
  
  // clear memory of objects in vector
  template<typename T> void clear(std::vector<T*> &vec)
    {
      while(!vec.empty())
	{
	  delete vec.back();
	  vec.pop_back();
	}
    }


 public:
  RecoViewer(const TGWindow *p, UInt_t w, UInt_t h);
  virtual ~RecoViewer();
  
  void CRP_Selector(Int_t id);
  void FetchEvent();
  void NextEvent();
  void PrevEvent();
  
  Bool_t GetWindowState() const {return fWinUp;};
  Bool_t GetState() const { return fWinUp; }
  
  // slots
  void CloseWindow() {fWinUp = kFALSE; Exit();}; 
  
  void OpenFile(std::string &fname);
  //void Open()
  //void InputDirectory();
  //void DirectoryEntered();
  //void DoNewCanvas();
  
  ClassDef(RecoViewer,0);
};

#endif
