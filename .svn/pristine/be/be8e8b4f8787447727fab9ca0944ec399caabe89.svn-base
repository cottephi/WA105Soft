/////////////////////////////////////////////////////////////////////////
// 
//  Basic GUI for data quality monitoring tasks
//  
//  Created:  vyacheslav.galymov@cern.ch
//  Date:     Mon Feb  1 10:38:13 CET 2016
//  Modified: 
// 
/////////////////////////////////////////////////////////////////////////

#ifndef __DQMONVIEWER_H__
#define __DQMONVIEWER_H__

#include <vector>
#include <string>

#include <TGFrame.h>
#include <TTimer.h>

#define DQREFRESH 3 // refresh view in seconds

class TGFrame;
class TGButton;
class TGCanvas;
class TGListTree;
class TGListTreeItem;

class MonTask;

//
class DQMonViewer : public TGMainFrame
{
 private:
  TGMainFrame        *fMain;
  
  
  //text buttons
  TGTextButton      *fBNewCanvas;
  TGTextButton      *fBExit;
  
  // list tree
  TGCanvas          *fCanvView;
  TGListTree        *fListTree;
  TGListTreeItem    *fBase;
  TGListTreeItem    *fActiveViewItem;
  
  //
  TTimer *fRefreshTimer;
  
  //
  Bool_t fWinUp;
  
  //
  void Exit();
  
  // list of tasks to plot
  // std::vector<MonTask*> fMonTasks;
  
 public:
  
  //
  void SelectedItem(TGListTreeItem*, Int_t);

  //
  void RefreshView();


 public:
  DQMonViewer(const TGWindow *p, UInt_t w, UInt_t h);
  virtual ~DQMonViewer();
  
  // monitoring tasks for viewing
  // void AddToViewTree(const char* taskgroup, const char *mondata);
  void AddTaskToTree(MonTask *montask);

  Bool_t GetWindowState() const {return fWinUp;};
  Bool_t GetState() const { return fWinUp; }
    
  // slots
  void CloseWindow() {fWinUp = kFALSE; Exit();}; 
  void DoNewCanvas();
  
  ClassDef(DQMonViewer,0);
};



#endif
