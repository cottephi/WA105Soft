/////////////////////////////////////////////////////////////////////////
// 
//  Basic GUI for data quality monitoring tasks
//  
//  Created:  vyacheslav.galymov@cern.ch
//  Date:     Mon Feb  1 10:38:13 CET 2016
//  Modified: 
// 
/////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <ctime>

//ROOT GUI classes
#include <TGLayout.h>
#include <TGButton.h>
#include <TGListTree.h>
#include <TGPicture.h>
#include <TGCanvas.h>

// ROOT basic classes
#include <TPad.h>
#include <TROOT.h>
#include <TCanvas.h>
#include <TColor.h>
#include <TSeqCollection.h>
#include <TApplication.h>

#include "MonTask.h"
#include "DQMonViewer.h"

using namespace std;

////////////////////////////////////////////////////////////////
// ctor
DQMonViewer::DQMonViewer(const TGWindow *p, UInt_t w, UInt_t h)
{
  //set some defaults

  //Gui builder
  fMain = new TGMainFrame(p, w, h);
  
  fMain->Connect("CloseWindow()","DQMonViewer", this, "CloseWindow()");
  fMain->SetIconPixmap("$WA105SOFT/guiutils/icons/wa105icon.png");
  
  // Composite frames
  TGCompositeFrame *top = new TGCompositeFrame(fMain, 0, 0, kVerticalFrame);
  
  //Some global controls
  TGCompositeFrame *hcon  = new TGCompositeFrame(top, 0, 0, kHorizontalFrame);
  
  fBExit = new TGTextButton(hcon,"Exit");
  fBExit->SetToolTipText("Click to close");
  fBExit->Connect("Clicked()","DQMonViewer",this,"CloseWindow()");
  fBExit->SetBackgroundColor(TColor::Number2Pixel(2));
  hcon->AddFrame(fBExit, new TGLayoutHints(kLHintsLeft|kLHintsTop,5,5,5,5));
  
  //fBNewCanvas = new TGTextButton(hcon, "Add Canvas");
  //fBNewCanvas->Connect("Clicked()", "DQMonViewer", this, "DoNewCanvas()");
  //hcon->AddFrame(fBNewCanvas, new TGLayoutHints(kLHintsLeft|kLHintsTop,5,5,5,5));
  
  // lower frame with list
  TGCompositeFrame *hcan  = new TGCompositeFrame(top, 0, 0, kHorizontalFrame);
  fCanvView = new TGCanvas(hcan, 400, 300, kSunkenFrame | kDoubleBorder);
  fListTree = new TGListTree(fCanvView, kHorizontalFrame);

  fBase = fListTree->AddItem(NULL, "Monitoring Tasks");

  //fCanvView->GetViewPort()->AddFrame(fListTree);
  fCanvView->SetContainer(fListTree);

  fListTree->Connect("DoubleClicked(TGListTreeItem*, Int_t)", "DQMonViewer", this, 
		     "SelectedItem(TGListTreeItem*, Int_t)");
  
  hcan->AddFrame(fCanvView, new TGLayoutHints(kLHintsLeft | kLHintsExpandY, 5, 10, 10, 10));
  
  //fBNewCanvas = new TGTextButton(hcan, "Add Canvas");
  //fBNewCanvas->Connect("Clicked()", "DQMonViewer", this, "DoNewCanvas()");
  //hcan->AddFrame(fBNewCanvas, new TGLayoutHints(kLHintsLeft, 5, 5, 10));
  
  // map to the top
  top->AddFrame(hcon, new TGLayoutHints( kLHintsLeft | kLHintsTop,0,0,0,0));
  top->AddFrame(hcan, new TGLayoutHints( kLHintsLeft | kLHintsTop,0,0,0,0));
  		  
  // map to the main
  fMain->AddFrame(top, new TGLayoutHints(kLHintsTop | kLHintsExpandX | kLHintsExpandY, 
					   0, 0, 0, 0));

  // Set a name to the main frame
  fMain->SetWindowName("Data Quality Monitor");
  fMain->MapSubwindows();
  fMain->Resize(fMain->GetDefaultSize());
  fMain->MapWindow();

  fWinUp = kTRUE;


  // our refresh timer
  fRefreshTimer = new TTimer();
  fRefreshTimer->Connect("Timeout()", "DQMonViewer", this, "RefreshView()");
  
  fActiveViewItem = NULL;
}


//
// -- dtor
//
DQMonViewer::~DQMonViewer()
{  
  fMain->Cleanup();
  delete fMain;
  delete fRefreshTimer;
}


//
// exit slot
//
void DQMonViewer::Exit()
{
  
  std::cout<<"Terminating... "<<std::endl;
  gApplication->Terminate();
}

//
// pop up a new canvas
//
void DQMonViewer::DoNewCanvas()
{
  // open a root canvas
  gROOT->MakeDefCanvas();
}


//
//
//
// void DQMonViewer::AddToViewTree(const char* taskname, const char *mondata)
// {
//   TGListTreeItem *parent = fListTree->FindChildByName(fBase, taskname);
//   TGListTreeItem *child = NULL;
//   if( parent )
//     {
//       //cout<<"Parent "<<taskname<<" already exsits"<<endl;
//       child = fListTree->FindChildByName(parent, mondata);
//       if(child)
// 	{
// 	  cerr<<"Cannot add "<<mondata<<" the data monitoring task exists already"<<endl;
// 	  return;
// 	}
//     }
//   else
//     {
//       parent = fListTree->AddItem(fBase, taskname);
//     }
  
//   //
//   child = fListTree->AddItem(parent, mondata);
//   child->SetPictures( gClient->GetPicture("ofolder_t.xpm"),
// 		      gClient->GetPicture("return_object_t.xpm") );

// }


//
//
//
void DQMonViewer::AddTaskToTree(MonTask *montask)
{
  const char* taskname = montask->GetTaskKey();
  TGListTreeItem *parent = fListTree->FindChildByName(fBase, taskname);
  if(!parent) parent = fListTree->AddItem(fBase, taskname);

  const vector<string> plts = montask->GetItems();
  for(size_t i=0;i<plts.size();i++)
    {
      const char *mondata = plts[i].c_str();
      TGListTreeItem* child = fListTree->FindChildByName(parent, mondata);
      if(child)
	{
	  cerr<<"Cannot add "<<mondata<<" the data monitoring task exists already"<<endl;
	  continue;
	}
      
      //
      child = fListTree->AddItem(parent, mondata);
      child->SetPictures( gClient->GetPicture("ofolder_t.xpm"),
			  gClient->GetPicture("return_object_t.xpm") );
      child->SetUserData( montask );
      montask->StartPolling();
    }
  
}


//
//
//
void DQMonViewer::SelectedItem(TGListTreeItem* item, Int_t btn)
{
  // clicked on parent
  if(!item->GetParent()) return;

  if( item->GetParent() == fBase ) return;

  // close the view item
  if(fActiveViewItem && fActiveViewItem != item)
    {
      fActiveViewItem->SetOpen(false);
      //MonTask *aTask = (MonTask*)fActiveViewItem->GetUserData();
      //aTask->StopPolling();
    }
  
  fActiveViewItem = item;
  //MonTask *aTask = (MonTask*)fActiveViewItem->GetUserData();
    
  // if we double clicked on the item is open 
  // we will stop the timer
  if(!fActiveViewItem->IsOpen())
    {
      fRefreshTimer->Stop();
      //aTask->StopPolling();
      return;
    }
  //aTask->StartPolling();
  fRefreshTimer->Start(DQREFRESH*1.0E+3, false);

  // call it immediately and then on timer timeouts
  RefreshView();
}


//
//
//
void DQMonViewer::RefreshView()
{
  // 
  //cout<<"Refresh view "<<fActiveViewItem->GetText()<<endl;

  TSeqCollection *clist = gROOT->GetListOfCanvases();
  if( clist->GetEntries() == 0 ) // no canvases
    DoNewCanvas();

  TCanvas* cnvs = (TCanvas*)clist->Last();
  
  MonTask *aTask = (MonTask*)fActiveViewItem->GetUserData();
  aTask->Draw( fActiveViewItem->GetText(), cnvs );

  //cout<<"Number of canvases "<<clist->GetEntries()<<endl;
  //cout<<clist->First()<<" "<<clist->Last()<<endl;
  // pass last canvas to the client for draw ...
  
  //cnvs->SetTitle(fActiveViewItem->GetText());
  //cnvs->SetWindowSize(200, 100);
  //cout<<gPad<<endl;
}
