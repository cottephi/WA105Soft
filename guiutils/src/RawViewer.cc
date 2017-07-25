//////////////////////////////////////////////////////////////////////////////////////////////
//
//  Viewer for waveforms in two views (the most basic event display)
//
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <ctime>
#include <sstream>
#include <string>

#include <TApplication.h>
#include <TGClient.h>
#include <TColor.h>
#include <TGFileDialog.h>
#include <TStyle.h>

#include "EventDataManager.h"

#include "RawViewer.h"

// for pedestals
#include "calidaq.h"

////////////////////////////////////////////////////////////////
// ctor (build overal gui layout)
RawViewer::RawViewer(const TGWindow *p, UInt_t w, UInt_t h)
{
  //set some defaults
  InitDrawStyle();

  //Gui builder
  fMain = new TGMainFrame(p, w, h);
  
  //fMain->SetCleanup(kDeepCleanup);

  fMain->Connect("CloseWindow()","RawViewer", this, "CloseWindow()");
  fMain->SetIconPixmap("$WA105SOFT/guiutils/icons/wa105icon.png");

  // Composite frames
  TGCompositeFrame *top = new TGCompositeFrame(fMain, 0, 0, kVerticalFrame);
  
  //Some global controls
  TGCompositeFrame *hcon  = new TGCompositeFrame(top, 0, 0, kHorizontalFrame);
  
  fTEDirPath = new TGTextEntry(hcon, new TGTextBuffer(256));
  fTEDirPath->SetToolTipText("Set input directory or file name");
  fTEDirPath->Resize(300, fTEDirPath->GetDefaultHeight());
  fTEDirPath->Connect("ReturnPressed()","RawViewer", this, "InputDirectory()");
  hcon->AddFrame(fTEDirPath, new TGLayoutHints(kLHintsTop | kLHintsLeft, 5, 5, 5, 5));

  // dir and file names
  fDir = "./";
  fFileName = "";
  
  
  // browse button
  fBBrowse = new TGTextButton(hcon,"Browse");
  fBBrowse->SetToolTipText("Browse directory for input files");
  fBBrowse->Connect("Clicked()","RawViewer",this,"BrowseFiles()");
  hcon->AddFrame(fBBrowse, new TGLayoutHints(kLHintsLeft|kLHintsTop,0,5,5,5));
  
  // exit button
  fBExit = new TGTextButton(hcon,"Exit");
  fBExit->SetToolTipText("Click to close");
  fBExit->Connect("Clicked()","RawViewer",this,"CloseWindow()");
  fBExit->SetBackgroundColor(TColor::Number2Pixel(2));
  hcon->AddFrame(fBExit, new TGLayoutHints(kLHintsLeft|kLHintsTop, 10,5,5,5));

  // horizontal frame for data viewer
  TGCompositeFrame *hdf   = new TGCompositeFrame(top, 0, 0, kHorizontalFrame);

  // side frame
  TGCompositeFrame *vdis  = new TGCompositeFrame(hdf, 0, 0, kVerticalFrame);
  
  // our event browsing buttons
  fSWGrp = new TGButtonGroup(vdis, "Event browsing controls", kHorizontalFrame);
  
  // set all bits to 0
  fBtnState.reset();
  fBPrev = new TGPictureButton(fSWGrp, "$WA105SOFT/guiutils/icons/go-previous.xpm");
  fBPrev->SetState(kButtonDisabled);
  fBPrev->SetToolTipText("Go to previous event");
  fBPrev->Connect("Clicked()","RawViewer",this,"PrevEvent()");
  
  fBPlay  = new TGPictureButton(fSWGrp, "$WA105SOFT/guiutils/icons/player_play.xpm");
  fBPlay->SetState(kButtonDisabled);
  fBPlay->SetToolTipText("Autoplay events");
  fBPlay->Connect("Clicked()", "RawViewer",this,"PlayEvents()");
  fBPlay->SetBackgroundColor(TColor::Number2Pixel(8));
  fPlayOn = false;

  fBNext = new TGPictureButton(fSWGrp, "$WA105SOFT/guiutils/icons/go-next.xpm");
  fBNext->SetState(kButtonDisabled);
  fBNext->SetToolTipText("Go to next event");
  fBNext->Connect("Clicked()","RawViewer",this,"NextEvent()");


  fBReload = new TGPictureButton(fSWGrp, "$WA105SOFT/guiutils/icons/reload.xpm");
  fBReload->SetState(kButtonDisabled);
  fBReload->SetToolTipText("Do canvas redraw");
  fBReload->Connect("Clicked()","RawViewer",this,"RedrawEvent()");

  //
  vdis->AddFrame( fSWGrp, new TGLayoutHints(kLHintsLeft | kLHintsTop, 5, 5, 5, 5));
  
  // crm selector
  TGHorizontalFrame *hcrm = new TGHorizontalFrame(vdis, 200, 30);
  fMaxCRMs = 0;
  fSelCRMId = 0;
  fCRMSelector = new TGNumberEntry( hcrm,  fSelCRMId, 9, -1, TGNumberFormat::kNESInteger, 
				    TGNumberFormat::kNEANonNegative, 
				    TGNumberFormat::kNELLimitMinMax, 0, fMaxCRMs );
  fCRMSelector->Connect("ValueSet(Long_t)","RawViewer", this, "SelectCRM()");
  fCRMSelector->SetLogStep(kFALSE);
  fCRMSelector->SetState(kFALSE);
  fCRMSelLabel = new TGLabel( hcrm, Form("CRM out of total %d", fMaxCRMs));
  hcrm->AddFrame( fCRMSelector, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 0, 0, 0, 0) );
  hcrm->AddFrame( fCRMSelLabel, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 5, 0, 0, 0) );
  vdis->AddFrame(hcrm, new TGLayoutHints(kLHintsLeft, 5, 5, 5, 5));

  //
  // playback rate
  TGHorizontalFrame *hpbk = new TGHorizontalFrame(vdis, 200, 30);
  fRatePlayback  = 10;
  fNEPlaybackRate = new TGNumberEntry( hpbk,  fRatePlayback, 9, -1, TGNumberFormat::kNESInteger, 
				       TGNumberFormat::kNEANonNegative, 
				       TGNumberFormat::kNELLimitMinMax, 1, 100);
  fNEPlaybackRate->Connect("ValueSet(Long_t)","RawViewer", this, "SetPlaybackRate()");
  fNEPlaybackRate->SetLogStep(kFALSE);
  hpbk->AddFrame( fNEPlaybackRate, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 0, 0, 0, 0) );
  hpbk->AddFrame( new TGLabel(hpbk, "Event playback rate"), 
		  new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 5, 0, 0, 0) );
  vdis->AddFrame(hpbk, new TGLayoutHints(kLHintsLeft, 5, 5, 5, 5));
  
  //
  // time refresh in autoplay
  TGHorizontalFrame *hrnw = new TGHorizontalFrame(vdis, 200, 30);
  fTRefresh = 3;
  fNETimeRefresh = new TGNumberEntry( hrnw,  fTRefresh, 9, -1, TGNumberFormat::kNESRealOne, 
				      TGNumberFormat::kNEANonNegative, 
				      TGNumberFormat::kNELLimitMinMax, 1, 20);
  fNETimeRefresh->Connect("ValueSet(Long_t)","RawViewer", this, "SetRefreshTime()");
  fNETimeRefresh->SetLogStep(kFALSE);
  hrnw->AddFrame( fNETimeRefresh, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 0, 0, 0, 0) );
  hrnw->AddFrame( new TGLabel(hrnw, "Refresh time [s]"), 
		  new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 5, 0, 0, 0) );
  vdis->AddFrame(hrnw, new TGLayoutHints(kLHintsLeft, 5, 5, 5, 5));
  
  //
  // threshold to display ADC WFs
  TGHorizontalFrame *hmaxadc = new TGHorizontalFrame(vdis, 200, 30);
  fADCWFThresh   = 20;
  fNEADCWFThresh = new TGNumberEntry( hmaxadc, fADCWFThresh , 9, -1, TGNumberFormat::kNESInteger, 
				      TGNumberFormat::kNEANonNegative, 
				      TGNumberFormat::kNELLimitMinMax, 0, 4096);
  fNEADCWFThresh->Connect("ValueSet(Long_t)","RawViewer", this, "SetWFThreshADC()");
  fNEADCWFThresh->SetLogStep(kFALSE);
  hmaxadc->AddFrame( fNETimeRefresh, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 0, 0, 0, 0) );
  hmaxadc->AddFrame( new TGLabel(hmaxadc, "> WF ADC Threshold"), 
		     new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 5, 0, 0, 0) );
  vdis->AddFrame(hmaxadc, new TGLayoutHints(kLHintsLeft, 5, 5, 5, 5));

  //
  // Max number of WFs to show
  TGHorizontalFrame *hmaxwf = new TGHorizontalFrame(vdis, 200, 30);
  fMaxWF   = 20;
  fNEMaxWF = new TGNumberEntry( hmaxwf, fMaxWF , 9, -1, TGNumberFormat::kNESInteger, 
				      TGNumberFormat::kNEANonNegative, 
				      TGNumberFormat::kNELLimitMinMax, 1, 960);
  fNEMaxWF->Connect("ValueSet(Long_t)","RawViewer", this, "SetWFMaxNum()");
  fNEMaxWF->SetLogStep(kFALSE);
  hmaxwf->AddFrame( fNETimeRefresh, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 0, 0, 0, 0) );
  hmaxwf->AddFrame( new TGLabel(hmaxwf, "Max number of WFs to show"), 
		    new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 5, 0, 0, 0) );
  vdis->AddFrame(hmaxwf, new TGLayoutHints(kLHintsLeft, 5, 5, 5, 5));
  
  //
  TGHorizontalFrame *hselch0 = new TGHorizontalFrame(vdis, 200, 30);
  fSelChView0 = new TGTextEntry(hselch0, new TGTextBuffer(256), 0);
  fSelChView0->SetToolTipText("E.g., 1,2,3 or 1-3 or 1,2-3");
  fSelChView0->Connect("ReturnPressed()","RawViewer", this, "SetChans()");
  fSelChView0->Resize(100, fSelChView0->GetDefaultHeight());
  hselch0->AddFrame( fSelChView0, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 0, 0, 0, 0) );
  hselch0->AddFrame( new TGLabel(hselch0, "Ch to show in view 0"), 
		     new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 5, 0, 0, 0) );
  vdis->AddFrame(hselch0, new TGLayoutHints(kLHintsTop | kLHintsLeft, 5, 5, 5, 5));
  
  //
  TGHorizontalFrame *hselch1 = new TGHorizontalFrame(vdis, 200, 30);
  fSelChView1 = new TGTextEntry(hselch1, new TGTextBuffer(256), 1);
  fSelChView1->SetToolTipText("E.g., 1,2,3 or 1-3 or 1,2-3");
  fSelChView1->Connect("ReturnPressed()","RawViewer", this, "SetChans()");
  fSelChView1->Resize(100, fSelChView1->GetDefaultHeight());
  hselch1->AddFrame( fSelChView1, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 0, 0, 0, 0) );
  hselch1->AddFrame( new TGLabel(hselch1, "Ch to show in view 1"), 
		     new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 5, 0, 0, 0) );
  vdis->AddFrame(hselch1, new TGLayoutHints(kLHintsTop | kLHintsLeft, 5, 5, 5, 5));
   
  // contrast adjustment
  fSliderMinPos = 1;
  fSliderMaxPos = 20;
  fSliderX0 = log10( 0.01 );
  fSliderX1 = log10( 1.00 );
  fSliderSlope = ( fSliderX1 - fSliderX0 )/(fSliderMaxPos - fSliderMinPos );
  Int_t sliderPos = fSliderMaxPos;
  //fSetScale = 1.0;
  fSetScale = GetSliderFactor( sliderPos );
  vdis->AddFrame(new TGLabel(vdis, "Adjust constrast"),
		 new TGLayoutHints(kLHintsLeft, 25, 5, 15, 0));
  fContScaleSlider = new TGHSlider(vdis, 200, kSlider1 | kScaleDownRight);
  fContScaleSlider->SetRange(fSliderMinPos, fSliderMaxPos);
  fContScaleSlider->SetPosition( sliderPos );
  fContScaleSlider->Connect("PositionChanged(Int_t)", "RawViewer",
			    this, "SetContrast(Int_t)");
  vdis->AddFrame(fContScaleSlider, new TGLayoutHints(kLHintsLeft, 5, 5, 0, 5));
  

  // add more options here ...
  
  

  
  // canvas frame
  TGCompositeFrame *vcan  = new TGCompositeFrame(hdf, 0, 0, kVerticalFrame);
  
  // upper frame with canvases
  TGCompositeFrame *hcan_up  = new TGCompositeFrame(vcan, 0, 0, kHorizontalFrame);
  
  fCnvsViewXT0 = new TRootEmbeddedCanvas("XTview0", hcan_up, 600, 500);
  hcan_up->AddFrame(fCnvsViewXT0, new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX | 
					       kLHintsExpandY, 2, 2, 2, 2));
  fCnvsViewXT1 = new TRootEmbeddedCanvas("XTview1", hcan_up, 600, 500);
  hcan_up->AddFrame(fCnvsViewXT1, new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX | 
					       kLHintsExpandY, 2, 2, 2, 2));

  // lower frame with canvases
  TGCompositeFrame *hcan_lo  = new TGCompositeFrame(vcan, 0, 0, kHorizontalFrame);
  
  fCnvsViewTADC0 = new TRootEmbeddedCanvas("TADCview0", hcan_lo, 600, 500);
  hcan_lo->AddFrame(fCnvsViewTADC0, new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX | 
					       kLHintsExpandY, 2, 2, 2, 2));
  fCnvsViewTADC1 = new TRootEmbeddedCanvas("TADCview1", hcan_lo, 600, 500);
  hcan_lo->AddFrame(fCnvsViewTADC1, new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX | 
					       kLHintsExpandY, 2, 2, 2, 2));
  
  
  // map it
  std::string tmpstr = "Run NULL: Event %d / %d, NULL";
  fInfoLabel  = new TGLabel( vcan, Form(tmpstr.c_str(), 0, 0) );
  fInfoLabel->SetTextFont("*-courier-bold-r-normal-*-18-*");
  vcan->AddFrame(fInfoLabel, new TGLayoutHints( kLHintsLeft | kLHintsExpandX ,0,5,5,0));

  vcan->AddFrame(hcan_up, new TGLayoutHints( kLHintsLeft | kLHintsTop | kLHintsExpandX | 
					     kLHintsExpandY,0,5,0,5));
  
  vcan->AddFrame(hcan_lo, new TGLayoutHints( kLHintsLeft | kLHintsTop | kLHintsExpandX | 
					     kLHintsExpandY,0,5,0,5));
  
  
  hdf->AddFrame(vdis, new TGLayoutHints( kLHintsLeft ,0,0,0,0));
  hdf->AddFrame(vcan, new TGLayoutHints( kLHintsLeft | kLHintsExpandX | 
					 kLHintsExpandY,0,0,0,0));

  
  // map everything to the top
  top->AddFrame(hcon, new TGLayoutHints( kLHintsLeft | kLHintsTop,0,0,0,0));
  top->AddFrame(hdf, new TGLayoutHints( kLHintsLeft | kLHintsTop | kLHintsExpandX | 
					 kLHintsExpandY,0,0,0,0));
  		  
  // map to the main
  fMain->AddFrame(top, new TGLayoutHints(kLHintsTop | kLHintsExpandX | kLHintsExpandY, 
					   0, 0, 0, 0));
  
  // our playback timer
  fPlayTimer = new TTimer();
  fPlayTimer->Connect("Timeout()", "RawViewer", this, "FetchEvent()");
  
  // event manager 
  fEvMgr = new EventDataManager();

  // Set a name to the main frame
  fMain->SetWindowName("WA105 Event Display");
  fMain->MapSubwindows();
  fMain->Resize(fMain->GetDefaultSize());
  fMain->MapWindow();

  fWinUp = kTRUE;

  fCurEvent   = 0;
  fEntries    = 0;
  fSkipEvents = 1;

  //
  fOnlineMode = kFALSE;
}


//
// -- dtor
//
RawViewer::~RawViewer()
{  
  fMain->Cleanup();
  delete fMain;
  delete fEvMgr;
  delete fPlayTimer;

  //
  while( !fPlotView0.empty() )
    {
      delete fPlotView0.back();
      fPlotView0.pop_back();     
    }
  //
  while( !fPlotView1.empty() )
    {
      delete fPlotView1.back();
      fPlotView1.pop_back();     
    }
  
}

//
// make your style choices here
//
void RawViewer::InitDrawStyle()
{
  // use palette
  gStyle->SetPalette(55);

  
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);

  gStyle->SetPadLeftMargin(0.12);
  gStyle->SetPadBottomMargin(0.12);

  gStyle->SetTextFont(132);
  gStyle->SetTextSize(0.1);
  gStyle->SetLabelFont(132,"x");
  gStyle->SetLabelFont(132,"y");
  gStyle->SetLabelFont(132,"z");
  gStyle->SetLabelSize(0.05,"x"); //0.09
  gStyle->SetTitleSize(0.06,"x"); //0.1
  gStyle->SetLabelSize(0.05,"y"); //0.09
  gStyle->SetTitleSize(0.06,"y"); //0.1
  gStyle->SetLabelSize(0.05,"z"); //0.09
  gStyle->SetTitleSize(0.06,"z"); //0.1
  gStyle->SetLabelFont(132,"t");
  gStyle->SetTitleFont(132,"x");
  gStyle->SetTitleFont(132,"y");
  gStyle->SetTitleFont(132,"z");
  gStyle->SetTitleFont(132,"t"); 
  gStyle->SetLegendFont(132);
  gStyle->SetTitleFillColor(0);
  gStyle->SetTitleX(0.25);
  gStyle->SetTitleFontSize(0.08);
  gStyle->SetTitleFont(132,"pad");

  gStyle->SetFrameBorderMode(0);
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetPadBorderMode(0);
  gStyle->SetPadColor(0);
  gStyle->SetCanvasColor(0);
  gStyle->SetStatColor(0);

  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
}


//
// exit slot
//
void RawViewer::Exit()
{
  //sleep(0.5); //give some time to other processes
  
  std::cout<<"Terminating... "<<std::endl;
  gApplication->Terminate();
}

//
//
//
void RawViewer::ConnectToServer(unsigned int port, const char *hostip)
{
  fSrvInfo = Form("%s : %d", hostip, port);

  // disable most of the controls
  fBtnState.reset(0);
  NewEventButtonState();
  
  //
  fBBrowse->SetState(kButtonDisabled);
  
  // time to poll for new events
  fNETimeRefresh->SetLimits(TGNumberFormat::kNELLimitMinMax, 1, 10);
  fTRefresh = 1.0;
  fNETimeRefresh->SetNumber(fTRefresh);

  //
  fTEDirPath->SetState( kFALSE );
  fTEDirPath->SetToolTipText("Status of server connection");
  fNEPlaybackRate->SetState( kFALSE );

  // open connection to online server
  fEvMgr->Open( port, hostip );
  ShowConnectionStatus( fEvMgr->ConnectionStatus() );
  InitViews();

  //
  fBtnState.set(0);  
  fBtnState.set(3);  // enable button play
  NewEventButtonState();
  
  fOnlineMode = kTRUE;
  PlayEvents();
}

//
// show the state of the connection
//
void RawViewer::ShowConnectionStatus(bool state)
{
  const char *str = NULL;
  if(state)
    {
      str = Form("Connected to %s",fSrvInfo.Data());
      fTEDirPath->SetTextColor(gROOT->GetColor(8));
    }
  else
    {
      str = Form("Not connected to %s",fSrvInfo.Data());
      fTEDirPath->SetTextColor(gROOT->GetColor(2));
    }
  fTEDirPath->SetText(str);
}



//
//
//
bool RawViewer::OpenNewFile(std::string fpath)
{
  fFileName = fpath;
  // update entry string
  fTEDirPath->SetText(fpath.c_str());
  //cout<<"File search path  : "<<fDir<<endl;
  cout<<"Current file name : "<<fFileName<<endl;

  //
  fEvMgr->Open(fpath);

  //
  fEntries  = fEvMgr->GetEntries();
  fCurEvent = 0;
  fPlayEnd  = false;
  
  if(fEntries == 0) return false;

  MakeEventLoopStep();
  
  // time string
  //time_t run_time = (time_t)rconf.GetRunHeader()->GetRunTime();
  //struct tm *timeinfo = localtime(&run_time);
  //char buffer[256];
  //strftime(buffer,sizeof(buffer),"%c",timeinfo);
  //string rtimestr(buffer);
  //cout<<"Run started at "<<rtimestr<<endl;
  RunConfig rconf;
  fEvMgr->GetRunConfig( rconf );
  Long64_t run_nb = rconf.GetRunHeader()->GetRunNumber();

  stringstream ss;
  ss<<"Run "<<run_nb<<": Event %d / "<<fEntries<<", %s";
  fInfoString = ss.str();

  if(!InitViews()) return false;

  // show first event
  FetchEvent();

  return true;
}

//
//
//
bool RawViewer::InitViews()
{
  // get run config
  RunConfig rconf;
  fEvMgr->GetRunConfig( rconf );

  fMaxCRMs = rconf.GetCRPConfig()->GetCRMnum();
  fCRMSelLabel->SetText(Form("CRM out of total %d", fMaxCRMs));  
  if( fMaxCRMs == 0) return false;
  
  fCRMSelector->SetState(kTRUE);
  fCRMSelector->SetLimitValues(0, fMaxCRMs-1);
  fSelCRMId = 0;
  fCRMSelector->GetNumberEntry()->SetNumber( fSelCRMId );

  std::string name0 = "view0_crm";
  std::string name1 = "view1_crm";
  
  // initialize histos
  for(int i=0;i<fMaxCRMs;i++)
    {
      int nchx = rconf.GetCRPConfig()->GetCRMnchaX(i);
      int nchy = rconf.GetCRPConfig()->GetCRMnchaY(i);
      int ntdc = rconf.GetCRPConfig()->GetCRMnsample(i);

      if( i < (int)fPlotView0.size() )
	{
	  fPlotView0[i]->SetNewDataFrame( ntdc-1, 0, nchx-1 );
	  fPlotView1[i]->SetNewDataFrame( ntdc-1, 0, nchy-1 );
	}
      else
	{
	  stringstream ss1, ss2;
	  ss1<<name0<<i;
	  ss2<<name1<<i;
	  
	  fPlotView0.push_back( new CRPViewPlotting( ss1.str(), 0, ntdc-1, 0, nchx-1 ) );
	  fPlotView1.push_back( new CRPViewPlotting( ss2.str(), 1, ntdc-1, 0, nchy-1 ) );
	}
      
      fPlotView0.back()->SelectWFInADCRange(fMaxWF, fADCWFThresh);
      fPlotView1.back()->SelectWFInADCRange(fMaxWF, fADCWFThresh);      
    }

  return true;
}

//
//
//
void RawViewer::FetchEvent()
{
  // disable all event buttons while we are getting new event
  fBtnState.reset(0);
  NewEventButtonState();
  
  if( !fOnlineMode )
    {
      if( fPlayOn ) 
	{
	  if(fCurEvent >= fEntries ) // end events
	    {
	      UpdateInfoString("Waiting for next file ...");
	      fCurEvent = fEntries;
	      UpdateEventButtonState();
	      return;
	    }
	}
  
      Event *event = fEvMgr->GetEvent(fCurEvent);  
      fStrTStamp = event->GetEventHeader()->GetEventTime().AsString();
      Fill( event );
      Redraw( kDrawAll );
      UpdateInfoString();
  
      if(fPlayOn) //if autoplay increment event counter
	{
	  fCurEvent += fSkipEvents;
	  if( fCurEvent >= fEntries && !fPlayEnd ) 
	    {
	      fCurEvent  = fEntries-1;
	      fPlayEnd   = true;
	    }
	  else
	    fPlayEnd = false;
	}
    }
  else // try to see if we get new event
    {
      Event *event = fEvMgr->GetOnlineEvent();  
      if(event) // got new event
	{
	  fStrTStamp = event->GetEventHeader()->GetEventTime().AsString();
	  std::stringstream ss;
	  ss<<"Run "<<event->GetRunHeader()->GetRunNumber()<<": Event %d, %s";
	  fInfoString = ss.str();
	  fCurEvent = event->GetEventHeader()->GetEventNumber();
	  Fill( event );
	  Redraw( kDrawAll );
	  UpdateInfoString();
	}
      //else
      ShowConnectionStatus( fEvMgr->ConnectionStatus() );
    }
  
  UpdateEventButtonState();
}

//
//
//
void RawViewer::NewEventButtonState()
{
  if(!fBtnState.test(0))
    {
      fBPrev->SetState(kButtonDisabled);
      fBNext->SetState(kButtonDisabled);
      fBPlay->SetState(kButtonDisabled);
      fBReload->SetState(kButtonDisabled);
    }
  else
    {
      fBPrev->SetEnabled( fBtnState.test(1) );
      fBNext->SetEnabled( fBtnState.test(2) );
      fBPlay->SetEnabled( fBtnState.test(3) );
      fBReload->SetEnabled( fBtnState.test(4) );
    }
}


//
//
//
void RawViewer::UpdateEventButtonState()
{
  // set first bit to enable
  fBtnState.set();
  if( fPlayOn )
    {
      fBtnState.reset(1);
      fBtnState.reset(2);
      fBtnState.reset(4);
    }
  else
    {
      if(!fOnlineMode)
	{
	  if( fCurEvent < 1 ) 
	    fBtnState.reset(1);
	  if( (int)fCurEvent >= (int)fEntries - 1 )
	    fBtnState.reset(2);
	}
      else
	{
	  // enable only reload button
	  fBtnState.reset(1);
	  fBtnState.reset(2);
	  fBtnState.set(4);
	}
    }

  NewEventButtonState();
}


//
//
//
void RawViewer::SelectCRM()
{
  int crm_id = fCRMSelector->GetNumberEntry()->GetIntNumber();
  
  if(crm_id >= fMaxCRMs) return;
  
  //cout<<"Picked CRM = "<<crm_id<<endl;
  fSelCRMId = crm_id;
  
}


//
// clicked on play events ...
//
void RawViewer::PlayEvents()
{
  fPlayOn = !fPlayOn;
  if( fPlayOn )
    {
      fPlayTimer->Start(fTRefresh*1.0E+3, kFALSE);
      if(!fOnlineMode)
	{
	  // go to next event
	  fCurEvent++;
	  fBPlay->SetPicture(gClient->GetPicture("$WA105SOFT/guiutils/icons/player_stop.xpm"));
	  fBPlay->SetBackgroundColor(TColor::Number2Pixel(2));
	}
      else
	{
	  fBPlay->SetPicture(gClient->GetPicture("$WA105SOFT/guiutils/icons/player_pause.xpm"));
	  fBPlay->SetBackgroundColor(TColor::Number2Pixel(2));
	}
    }
  else
    {
      fPlayTimer->Stop();
      if(!fOnlineMode)
	{
	  // reached the end ?
	  if(fCurEvent >= fEntries) 
	    {
	      fCurEvent = fEntries - 1;
	      UpdateInfoString();
	    }
	}
      
      fBPlay->SetPicture(gClient->GetPicture("$WA105SOFT/guiutils/icons/player_play.xpm"));
      fBPlay->SetBackgroundColor(TColor::Number2Pixel(8));
    }

  UpdateEventButtonState();
}

//
//
//
void RawViewer::NextEvent()
{
  //cout<<"Next event pressed "<<endl;
  fCurEvent++;
  FetchEvent();
}

//
// clicked on refresh button
//
void RawViewer::RedrawEvent()
{
  Redraw( kDrawAll );
}

//
//
//
void RawViewer::PrevEvent()
{
  //cout<<"Prev event pressed "<<endl; 
  fCurEvent--;
  FetchEvent();
}


//
// input directory slot (needs some work...)
//
void RawViewer::InputDirectory()
{
  std::string inputdir = fTEDirPath->GetText();
  if(inputdir.empty())
    return;
  
  size_t found = inputdir.find_last_of(".root"); 
  if(found != std::string::npos)
    {
      fFileName = inputdir;
      fDir = inputdir.substr(0, inputdir.find_last_of("/"));
      if(fDir.empty()) fDir = "./";
    }
  else
    {
      fDir = inputdir;
      std::string ftmp = FindLatestFile(fDir, ".root");
      if(ftmp != fFileName) // already played
	fFileName = ftmp;
      else
	fFileName = "";
    }
  
  if(!fFileName.empty())
    {
      if(!OpenNewFile( fFileName ))
	{
	  cerr<<"ERROR: file format could not be read"<<endl;
	}
      //if(OpenNewFile( fFileName ))
      //{
      //fTEDirPath->SetText(fFileName.c_str());
      //}
      //else //error
    }
}


//
// input directory slot
//
void RawViewer::BrowseFiles()
{
  TGFileInfo fi;
  
  const char *filetypes[] = {"ROOT files",    "*.root",
			     "Raw data files",    "*.dat",
			     0,                0};
  
  
  fi.fFileTypes = filetypes;
  fi.fIniDir    = StrDup(fDir.c_str());

  new TGFileDialog(gClient->GetRoot(), fMain, kFDOpen, &fi);
  if(!fi.fFilename)
    {
      return;
    }
  else
    {
      fDir      = fi.fIniDir;
      fFileName = fi.fFilename;

      if(!OpenNewFile( fFileName ))
	{
	  cerr<<"ERROR: file format could not be read"<<endl;
	}
      //if()
      //{
      //}
      //else // popup error dialog
      
    }
}

 
//
// to implement maybe ...
//
std::string RawViewer::FindLatestFile( std::string dir, std::string ext)
{
  return "";
}



//
//
//
void RawViewer::SetPlaybackRate()
{
  fRatePlayback = fNEPlaybackRate->GetNumberEntry()->GetNumber();
  MakeEventLoopStep();
  //cout<<"Skip events "<<fSkipEvents<<" "<<fRatePlayback<<" "<<fTRefresh<<endl;
}

//
//
//
void RawViewer::SetRefreshTime()
{
  fTRefresh = fNETimeRefresh->GetNumberEntry()->GetNumber();
  MakeEventLoopStep();
  // timer takes time in ms
  fPlayTimer->SetTime( fTRefresh * 1.0E+3 );
}

//
//
//
void RawViewer::SetContrast(Int_t pos)
{
  //cout<<"Slider position "<<pos<<endl;
  //fSetScale = (float)(pos)/fSliderMaxPos;
  fSetScale = GetSliderFactor( pos );
}

//
//
//
void RawViewer::SetWFThreshADC()
{
  fADCWFThresh = fNEADCWFThresh->GetNumberEntry()->GetIntNumber();
  fPlotView0[fSelCRMId]->SelectWFInADCRange(fMaxWF, fADCWFThresh);
  fPlotView1[fSelCRMId]->SelectWFInADCRange(fMaxWF, fADCWFThresh);
}


//
//
//
void RawViewer::SetWFMaxNum()
{  
  fMaxWF = fNEMaxWF->GetNumberEntry()->GetIntNumber();
  fPlotView0[fSelCRMId]->SelectWFInADCRange(fMaxWF, fADCWFThresh);
  fPlotView1[fSelCRMId]->SelectWFInADCRange(fMaxWF, fADCWFThresh);
}

//
//
//
void RawViewer::SetChans()
{
  TGTextEntry *txtentry = (TGTextEntry *)gTQSender;
  int id = txtentry->WidgetId();
  std::string input = txtentry->GetText();
  if( id == 0)
    fPlotView0[fSelCRMId]->SelectWFInChans(input);
  else if( id == 1 )
    fPlotView1[fSelCRMId]->SelectWFInChans(input);
}


//
// fill 
//
void RawViewer::Fill( Event *event )
{
  const LArRawData::DataCRP* dat_crp = event->GetCRPData();

  if(!dat_crp)
    {
      cerr<<"ERROR: no CRP raw data found "<<endl;
      return;
    }
  
  // pedestals read from external file (if any)
  calidaq::ChPedestal *calidaqPeds = &calidaq::ChPedestal::Instance();
  
  // loop over CRMs
  TIter next_crm(dat_crp->GetCRMs());
  LArRawData::DataCRM *crm;
  while ( (crm = (LArRawData::DataCRM*) next_crm()) ) 
    {
      int icrm = crm->GetCRMId();

      // reset 
      fPlotView0[icrm]->Reset();
      fPlotView1[icrm]->Reset();

      LArRawData::DataChan *ch = NULL;

      // process channels in view 0
      TIter next_chv0(crm->GetChanView0()); //
      while ( (ch = (LArRawData::DataChan*)(next_chv0())) )
	{
	  int chnum = ch->GetChanId(); 
	  float ped = calidaqPeds->Pedestal( icrm, 0, chnum );
	  
	  // now get the segments
	  TIter next(ch->GetDataFragments());
	  
	  TObject *obj;
	  while( (obj = next()) )
	    {
	      if(ch->GetDataType() == LArRawData::DTID_USHORT)
		{
		  LArRawData::DataFragment<UShort_t> *adc = (LArRawData::DataFragment<UShort_t>*)obj;
		  fPlotView0[icrm]->Fill( chnum, adc, ped ); 
		}
	      else // didn't bother with float ...
		{
		  cerr<<"ERROR: cannot handle this datatype "<<endl;
		}
	    } // end loop over segments
	}//loop over channels in view 0

      // process channels in view 1
      TIter next_chv1(crm->GetChanView1());
      while ( (ch = (LArRawData::DataChan*) next_chv1()) )
	{
	  int chnum = ch->GetChanId(); 
	  float ped = calidaqPeds->Pedestal( icrm, 1, chnum );	  

	  // now get the segments
	  TIter next(ch->GetDataFragments());
	  TObject *obj;
	  while( (obj = next()) )
	    {
	      if(ch->GetDataType() == LArRawData::DTID_USHORT)
		{
		  LArRawData::DataFragment<UShort_t> *adc = (LArRawData::DataFragment<UShort_t>*)obj;
		  fPlotView1[icrm]->Fill( chnum, adc, ped ); 
		}
	      else // didn't bother with float ...
		{
		  cerr<<"ERROR: cannot handle this datatype "<<endl;
		}
	    } // end loop over segments
	}// loop over channels in vew 1

    } // loop over CRMs
}


//
// redraw on canvases
//
void RawViewer::Redraw(int type)
{
  switch(type)
    {
    case kDrawAll:
      fPlotView0[fSelCRMId]->Draw2D(fCnvsViewXT0->GetCanvas(), fSetScale);
      //fPlotView0[fSelCRMId]->SelectWFInADCRange(fMaxWF, fADCWFThresh);
      fPlotView0[fSelCRMId]->DrawWF(fCnvsViewTADC0->GetCanvas());
      
      fPlotView1[fSelCRMId]->Draw2D(fCnvsViewXT1->GetCanvas(), fSetScale);
      //fPlotView1[fSelCRMId]->SelectWFInADCRange(fMaxWF, fADCWFThresh);
      fPlotView1[fSelCRMId]->DrawWF(fCnvsViewTADC1->GetCanvas());
      break;
      
    case kDraw2DAll:
      fPlotView0[fSelCRMId]->Draw2D(fCnvsViewXT0->GetCanvas(), fSetScale);
      fPlotView1[fSelCRMId]->Draw2D(fCnvsViewXT1->GetCanvas(), fSetScale);
      break;
      
    case kDraw1DAll:
      //fPlotView0[fSelCRMId]->SelectWFInADCRange(fMaxWF, fADCWFThresh);
      fPlotView0[fSelCRMId]->DrawWF(fCnvsViewTADC0->GetCanvas());
      
      //fPlotView1[fSelCRMId]->SelectWFInADCRange(fMaxWF, fADCWFThresh);
      fPlotView1[fSelCRMId]->DrawWF(fCnvsViewTADC1->GetCanvas());
      break;
      
    case kDraw1D0: // under construction ...
      break; 

    case kDraw1D1: // under construction ...
      break;
      
    default:
      break;
    }
}

