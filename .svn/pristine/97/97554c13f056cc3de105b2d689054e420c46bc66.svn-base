//////////////////////////////////////////////////////////////////////////////////////////////
//
//  Viewer for waveforms in two views (the most basic event display)
//
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __RAWVIEWER_H__
#define __RAWVIEWER_H__
#include <iostream>

//ROOT GUI classes
#include <TTimer.h>
#include <TGFrame.h>
#include <TGLayout.h>
#include <TGMenu.h>
#include <TGButton.h>
#include <TGButtonGroup.h>
#include <TGLabel.h>
#include <TGComboBox.h>
#include <TGTextEntry.h>
#include <TGNumberEntry.h>
#include <TRootEmbeddedCanvas.h>
#include <TGListTree.h>
#include <TGCanvas.h>
#include <TGSlider.h>
#include <TPad.h>
#include <TFrame.h>
#include <TCanvas.h>
#include <TROOT.h>

#include <bitset>
#include <vector>
#include <string>


#include "CRPViewPlotting.h"

class Event;
class EventDataManager;

class RawViewer : public TGMainFrame
{
 private:
  //
  TGMainFrame        *fMain;
  
  //
  TGTextEntry       *fTEDirPath;
  std::string fDir;
  std::string fFileName;
  
  // directory browser
  TGTextButton      *fBBrowse;

  // exit
  TGTextButton      *fBExit;
  
  // move between events
  TGButtonGroup     *fSWGrp;

  // bits to tag button states

  // button
  std::bitset<5>    fBtnState;
  TGPictureButton   *fBPrev;
  TGPictureButton   *fBNext;
  TGPictureButton   *fBPlay;
  TGPictureButton   *fBReload;

  void NewEventButtonState();
  void UpdateEventButtonState();
 
  Bool_t fPlayOn;
  Bool_t fPlayEnd;

  //
  std::string fInfoString;
  TGLabel *fInfoLabel;

  // timer for event playback
  TTimer *fPlayTimer;

  // view 0
  // canvas to display ch vs drift
  TRootEmbeddedCanvas *fCnvsViewXT0;
  // canvas to display waveforms
  TRootEmbeddedCanvas *fCnvsViewTADC0;

  // view 1
  // canvas to display ch vs drift
  TRootEmbeddedCanvas *fCnvsViewXT1;
  // canvas to display waveforms
  TRootEmbeddedCanvas *fCnvsViewTADC1;
  
  //
  Bool_t fWinUp;
  
  // event manager
  EventDataManager *fEvMgr; 

  //
  Long64_t fCurEvent;
  Long64_t fEntries;
  Long64_t fSkipEvents;
  
  TString  fStrTStamp;
  
  //
  TString  fSrvInfo;
  Bool_t   fOnlineMode;

  // CRM slector
  TGNumberEntry* fCRMSelector;
  TGLabel *fCRMSelLabel;
  Int_t fMaxCRMs;
  Int_t fSelCRMId;
  
  // time refresh in autoplay in s
  TGNumberEntry* fNETimeRefresh;
  Double_t fTRefresh;   // refresh time in s  
  TGNumberEntry* fNEPlaybackRate;
  Double_t fRatePlayback;  // playback rate in Hz
 
  //
  TGNumberEntry* fNEADCWFThresh;
  Int_t fADCWFThresh;   // threshold for ADC WF to display
  
  //
  TGNumberEntry* fNEMaxWF;
  Int_t fMaxWF;   // max number of WF to display
  
  //
  TGTextEntry  *fSelChView0;
  TGTextEntry  *fSelChView1;

  //
  Float_t fSetScale;
  Int_t fSliderMaxPos;
  Int_t fSliderMinPos;
  TGHSlider *fContScaleSlider;
  
  // redraw actions
  enum DrawType{ kDrawAll   = 0,
		 kDraw2DAll = 1,
		 kDraw1DAll = 2,
		 kDraw1D0   = 3,
		 kDraw1D1   = 4 };

  void Redraw(int type);
  
  void Exit();
  
  // how many events to skip during playback loop
  void MakeEventLoopStep()
  {
    Long64_t rval =  (Long64_t)( fRatePlayback ); // * fTRefresh );
    if(rval == 0) rval = 1;
    fSkipEvents = rval;
  }
  
  //
  // update info string
  void UpdateInfoString(std::string str = "")
  {
    std::string myinfo = Form( fInfoString.c_str(),  fCurEvent, fStrTStamp.Data());
    if( !str.empty() ) myinfo = str;
    
    fInfoLabel->SetText(myinfo.c_str());
  }

  // find latest file in a given directory
  std::string FindLatestFile(std::string dir, 
			     std::string ext = ".root");


  // 
  void EnableEventBrowsing(bool state);

  // define how we want things to appear
  void InitDrawStyle();

  // fill graphics with raw ADC data
  void Fill(Event *event);

  void ShowConnectionStatus(bool state);
  
  bool InitViews();

 public:
  RawViewer(const TGWindow *p, UInt_t w, UInt_t h);
  virtual ~RawViewer();
  
  // open new file
  bool OpenNewFile(std::string fpath);

  // connect to online event server
  void ConnectToServer(unsigned int port, const char *hostip);

  void SelectCRM();
  void FetchEvent();
  void NextEvent();
  void PlayEvents();
  void PrevEvent();
  void RedrawEvent();

  void SetPlaybackRate();
  void SetRefreshTime();
  void SetWFThreshADC();
  void SetWFMaxNum();
  void SetChans();
  void SetContrast(Int_t pos);


  //
  void InputDirectory();
  void BrowseFiles();
  
  // use log scale for slider pos
  float fSliderX0, fSliderX1;
  float fSliderSlope;
  float GetSliderFactor(Int_t pos)
  {
    float x = fSliderSlope * ( pos - fSliderMinPos ) + fSliderX0;
    return pow(10, x);
  }
  
  Bool_t GetWindowState() const {return fWinUp;};
  Bool_t GetState() const { return fWinUp; }
  
  // slots
  void CloseWindow() {fWinUp = kFALSE; Exit();}; 
  
  std::vector<CRPViewPlotting*> fPlotView0;
  std::vector<CRPViewPlotting*> fPlotView1;

  ClassDef(RawViewer,1);
};

#endif
