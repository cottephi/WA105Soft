//////////////////////////////////////////////////////////////////////////////////////////////
//
//  Simple viewer for reconstructed information for two plane views
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <ctime>

#include <TApplication.h>

#include "RecoViewer.h"

#include "QHit.h"
#include "Track2d.h"
#include "EventDataManager.h"


////////////////////////////////////////////////////////////////
// ctor
RecoViewer::RecoViewer(const TGWindow *p, UInt_t w, UInt_t h)
{
  //set some defaults

  //Gui builder
  fMain = new TGMainFrame(p, w, h);
  
  //fMain->SetCleanup(kDeepCleanup);

  fMain->Connect("CloseWindow()","RecoViewer", this, "CloseWindow()");
  fMain->SetIconPixmap("$WA105SOFT/guiutils/icons/wa105icon.png");

  // Composite frames
  TGCompositeFrame *top = new TGCompositeFrame(fMain, 0, 0, kVerticalFrame);
  
  //Some global controls
  TGCompositeFrame *hcon  = new TGCompositeFrame(top, 0, 0, kHorizontalFrame);
  
  fSWGrp = new TGButtonGroup(hcon, "Event browsing", kHorizontalFrame);
  
  fBPrev = new TGTextButton(fSWGrp,"Prev");
  fBPrev->SetState(kButtonDisabled);
  fBPrev->SetToolTipText("Go to previous event");
  fBPrev->Connect("Clicked()","RecoViewer",this,"PrevEvent()");
  //hcon->AddFrame(fBPrev, new TGLayoutHints(kLHintsLeft|kLHintsTop,5,5,5,5));
  
  fBNext = new TGTextButton(fSWGrp,"Next");
  fBNext->SetState(kButtonDisabled);
  fBNext->SetToolTipText("Go to next event");
  fBNext->Connect("Clicked()","RecoViewer",this,"NextEvent()");
  //hcon->AddFrame(fBNext, new TGLayoutHints(kLHintsLeft|kLHintsTop,5,5,5,5));
  hcon->AddFrame( fSWGrp, new TGLayoutHints(kLHintsLeft|kLHintsTop,2,2,2,2));


  fBExit = new TGTextButton(hcon,"Exit");
  fBExit->SetToolTipText("Click to close");
  fBExit->Connect("Clicked()","RecoViewer",this,"CloseWindow()");
  hcon->AddFrame(fBExit, new TGLayoutHints(kLHintsLeft|kLHintsTop,5,5,5,5));
  
  
  //
  fCmbBoxCRPView0 = new TGComboBox( hcon,  kCRPSel0 );
  fCmbBoxCRPView0->Resize(150, 20);
  fCmbBoxCRPView0->Connect("Selected(Int_t)", "RecoViewer", this, "CRP_Selector(Int_t)");
  hcon->AddFrame(fCmbBoxCRPView0, new TGLayoutHints(kLHintsLeft|kLHintsTop,5,5,5,5));

  fCmbBoxCRPView1 = new TGComboBox( hcon,  kCRPSel1 );
  fCmbBoxCRPView1->Resize(150, 20);
  fCmbBoxCRPView1->Connect("Selected(Int_t)", "RecoViewer", this, "CRP_Selector(Int_t)");
  hcon->AddFrame(fCmbBoxCRPView1, new TGLayoutHints(kLHintsLeft|kLHintsTop,5,5,5,5));


  // lower frame with canvases
  TGCompositeFrame *hcan  = new TGCompositeFrame(top, 0, 0, kHorizontalFrame);
  
  fCnvsView0 = new TRootEmbeddedCanvas("view0", hcan, 600, 500);
  hcan->AddFrame(fCnvsView0, new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX | 
					       kLHintsExpandY, 2, 2, 2, 2));
  fCnvsView1 = new TRootEmbeddedCanvas("view1", hcan, 600, 500);
  hcan->AddFrame(fCnvsView1, new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX | 
					       kLHintsExpandY, 2, 2, 2, 2));

  
  // map to the top
  top->AddFrame(hcon, new TGLayoutHints( kLHintsLeft | kLHintsTop,0,0,0,0));
  top->AddFrame(hcan, new TGLayoutHints( kLHintsLeft | kLHintsTop | kLHintsExpandX | 
					 kLHintsExpandY,0,0,0,0));
  		  
  // map to the main
  fMain->AddFrame(top, new TGLayoutHints(kLHintsTop | kLHintsExpandX | kLHintsExpandY, 
					   0, 0, 0, 0));
  
  
  // event manager 
  fEvMgr = new EventDataManager();
  //
  fHitBrNames.push_back("RecoHitsView0_Data");
  fHitBrNames.push_back("RecoHitsView1_Data");

  //
  fTrkBrNames.push_back("RecoCRTracks_View0");
  fTrkBrNames.push_back("RecoCRTracks_View1");
  
  
  // Set a name to the main frame
  fMain->SetWindowName("Simple Reco Viewer");
  fMain->MapSubwindows();
  fMain->Resize(fMain->GetDefaultSize());
  fMain->MapWindow();

  fWinUp = kTRUE;

  // for histogram axis in cm
  fAxisBinSize = 5.0;
}


//
// -- dtor
//
RecoViewer::~RecoViewer()
{  
  fMain->Cleanup();
  delete fMain;
  delete fEvMgr;

  clear( fHitsView0 );
  clear( fHitsView1 );
  clear( fTrksView0 );
  clear( fTrksView1 );
  clear( fAxisView0 );
  clear( fAxisView1 );

  for(int i=0;i<fAllCrp;i++)
    {
      clear( fTrkHitsView0[i] );
      clear( fTrkDrHitsView0[i] );
      clear( fTrkHitsView1[i] );
      clear( fTrkDrHitsView1[i] );
    }
}


//
// exit slot
//
void RecoViewer::Exit()
{
  //sleep(0.5); //give some time to other processes
  
  std::cout<<"Terminating... "<<std::endl;
  gApplication->Terminate();
}

//
//
//
void RecoViewer::OpenFile(std::string &fname)
{
  fBPrev->SetState(kButtonDisabled);
  fBNext->SetState(kButtonDisabled);
  
  
  fEvMgr->Open(fname);
  //fEvMgr->PrintRunConfig();
  
  fHasHits   = fEvMgr->HasRecoBranch(fHitBrNames[0]) && fEvMgr->HasRecoBranch(fHitBrNames[0]);
  fHasTracks = fEvMgr->HasRecoBranch(fTrkBrNames[0]) && fEvMgr->HasRecoBranch(fTrkBrNames[0]);
  
  if(!fHasHits) 
    {
      std::cerr<<"ERROR: reco hit information is mandatory"<<std::endl;
      return;
    }

  //if( !fHasTracks )
  //{
  // disable track graph controls
  //}


  NewFileInit();
  
  FetchEvent();

  // refresh all
  Refresh(-1);
  
}



//
//
//
void RecoViewer::NewFileInit()
{
  RunConfig rconf;
  fEvMgr->GetRunConfig( rconf );

  fEntries  = fEvMgr->GetEntries();
  fCurEvent = 0;
  
  int ncrms    = rconf.GetCRPConfig()->GetCRMnum();

  // each CRM + total
  fAllCrp = ncrms + 1;
    
  //Get CRP dimensions
  float xmin =  1.0E+9;
  float xmax = -1.0E+9;
  float ymin =  1.0E+9;
  float ymax = -1.0E+9;

  std::vector< Rectangle > crpdim_0(ncrms+1);
  std::vector< Rectangle > crpdim_1(ncrms+1);

  //float pitchT = rconf.GetCRPConfig()->GetCRMpitchT();
  float dlo = 0;
  float dhi = rconf.GetCRPConfig()->GetGC_SRzdimension(); // should be ok
  float xlo, xhi;
  float ylo, yhi;

  for(int i=0;i<ncrms;i++)
    {
      float xc = rconf.GetCRPConfig()->GetCRMcX(i);
      float xd = rconf.GetCRPConfig()->GetCRMxdim(i);
      xlo = xc - 0.5*xd;
      xhi = xc + 0.5*xd;
      
      if( xlo < xmin ) xmin = xlo;
      if( xhi > xmax ) xmax = xhi;

      crpdim_0[i].x1 = xlo;
      crpdim_0[i].x2 = xhi;
      crpdim_0[i].y1 = dlo;
      crpdim_0[i].y2 = dhi;

      float yc = rconf.GetCRPConfig()->GetCRMcY(i);
      float yd = rconf.GetCRPConfig()->GetCRMydim(i);
      ylo = yc - 0.5*yd;
      yhi = yc + 0.5*yd;
      
      if( ylo < ymin ) ymin = ylo;
      if( yhi > ymax ) ymax = yhi;
	
      crpdim_1[i].x1 = ylo;
      crpdim_1[i].x2 = yhi;
      crpdim_1[i].y1 = dlo;
      crpdim_1[i].y2 = dhi;
    }
  
  crpdim_0.back().x1 = xmin;
  crpdim_0.back().x2 = xmax;
  crpdim_0.back().y1 = dlo;
  crpdim_0.back().y2 = dhi;
  
  crpdim_1.back().x1 = ymin;
  crpdim_1.back().x2 = ymax;
  crpdim_1.back().y1 = dlo;
  crpdim_1.back().y2 = dhi;

  // build axis
  BuildAxis( crpdim_0, fAxisView0, 0 );
  BuildAxis( crpdim_1, fAxisView1, 1 );

  //
  // remove all previous entries
  fCmbBoxCRPView0->RemoveAll();
  fCmbBoxCRPView1->RemoveAll();
  
  // make a new combo box
  for(int i=0;i<ncrms;i++)
    {
      fCmbBoxCRPView0->AddEntry(Form("View 0 CRP %02d", i), i);
      fCmbBoxCRPView1->AddEntry(Form("View 1 CRP %02d", i), i);
    }
  
  fCmbBoxCRPView0->AddEntry("View 0 CRP All", ncrms);
  fCmbBoxCRPView1->AddEntry("View 1 CRP All", ncrms);

  fSelCRMView0 = ncrms;
  // select total by default (but do not call slot function)
  fCmbBoxCRPView0->Select(ncrms, false);

  fSelCRMView1 = ncrms;
  // select total by default (but do not call slot function)
  fCmbBoxCRPView1->Select(ncrms, false);


  // prepare arrays to draw track hits
  for(size_t i=0;i<fTrkHitsView0.size();i++)
    {
      clear( fTrkHitsView0[i] );
      clear( fTrkDrHitsView0[i] );
    }
  for(size_t i=0;i<fTrkHitsView1.size();i++)
    {
      clear( fTrkHitsView1[i] );
      clear( fTrkDrHitsView1[i] );
    }
  
  fTrkHitsView0.resize( fAllCrp );
  fTrkDrHitsView0.resize( fAllCrp );
  fTrkHitsView1.resize( fAllCrp );
  fTrkDrHitsView1.resize( fAllCrp );
}


//
//
//
void RecoViewer::CRP_Selector(Int_t id)
{
  TGComboBox *btn = (TGComboBox*)gTQSender;
  int wid = btn->WidgetId();
  
  if( wid == kCRPSel0 )
    {
      fSelCRMView0 = fCmbBoxCRPView0->GetSelected() ;
      //fSelCRMView0 = id;
      Refresh(0);
    }
  else if( wid == kCRPSel1 )
    {
      fSelCRMView1 = fCmbBoxCRPView1->GetSelected();
      //fSelCRMView1 = id;
      Refresh(1);
    }  
}


//
//
//
void RecoViewer::NextEvent()
{
  fCurEvent++;
  FetchEvent();
  Refresh(-1);
}

//
//
//
void RecoViewer::PrevEvent()
{
  fCurEvent--;
  FetchEvent();
  Refresh(-1);
}


//
//
//
void RecoViewer::FetchEvent()
{
  if(fCurEvent < 1) 
    {
      fCurEvent = 0;
      fBPrev->SetState(kButtonDisabled);
      // first event?
      if(fBNext->GetState() == kButtonDisabled && fEntries > 1)
	fBNext->SetEnabled();
    }
  else if(fCurEvent >=  fEntries-1)
    {
      fCurEvent = fEntries - 1;
      fBNext->SetState(kButtonDisabled);
    }
  else
    {
      if(fBPrev->GetState() == kButtonDisabled)
	fBPrev->SetEnabled();
      if(fBNext->GetState() == kButtonDisabled)
	fBNext->SetEnabled();
    }

  Event *event = fEvMgr->GetEvent(fCurEvent);
  
  GraphHits( event, 0, fHitsView0 );
  GraphHits( event, 1, fHitsView1 );
  
  if( fHasTracks )
    {
      GraphTracks( event, 0, fTrksView0 );
      GraphTrackHits( event, 0, fTrkHitsView0, fTrkDrHitsView0 );
      
      GraphTracks( event, 1, fTrksView1 );
      GraphTrackHits( event, 1, fTrkHitsView1, fTrkDrHitsView1 );
    }
}


//
//
//
void RecoViewer::GraphHits( Event *event, int iview, std::vector< TGraph* > &objs )
{
  // clean up from previous event
  clear( objs );
  
  const TObjArray *hits = event->Get(fHitBrNames[iview]);
  if(!hits) return;
  
  for(int i=0;i<fAllCrp;i++)
    {
      objs.push_back( new TGraph() );
      objs.back()->SetMarkerStyle(7);
      //objs.back()->SetMarkerSize(1.0);
    }

  for(int i=0;i<hits->GetEntries();i++)
    {
      LArReco::QHit* hit = (LArReco::QHit*)hits->At(i);
      objs[hit->GetCRM()]->SetPoint(i, hit->GetX(), hit->GetD());
      objs.back()->SetPoint(i, hit->GetX(), hit->GetD());
    }
}


//
//
//
void RecoViewer::GraphTracks( Event *event, int iview, std::vector< TGraph* > &objs)
{
  // clean up from previous event
  clear( objs );

  // all trks
  const TObjArray *trks = event->Get(fTrkBrNames[iview]);

  if(!trks) return;
  
  for(int i=0;i<trks->GetEntries();i++)
    {
      LArReco::Track2d* trk = (LArReco::Track2d*)trks->At(i);
      //std::cout<<"view "<<iview<<" trk "<<i<<" "<<trk->GetCRM()<<std::endl;

      std::vector<TVector2> trpath = trk->GetPath();
      
      TGraph *gr = new TGraph();

      for(size_t j=0;j<trpath.size();j++)
	gr->SetPoint(j, trpath[j].X(), trpath[j].Y());
      
      gr->SetLineColor(2);
      objs.push_back(gr);
    }
}

//
//
//
void RecoViewer::GraphTrackHits( Event *event, int iview, std::vector< std::vector< TGraph* > > &objs1,
				 std::vector< std::vector< TGraph* > > &objs2 )
{
  for(size_t i=0;i<objs1.size();i++)
    {
      clear( objs1[i] );
      clear( objs2[i] );
    }

  // all hits
  const TObjArray *hits = event->Get(fHitBrNames[iview]);  

  // all trks
  const TObjArray *trks = event->Get(fTrkBrNames[iview]);

  if(!trks) return;
  int lastcrm;
  // loop over tracks
  for(int i=0;i<trks->GetEntries();i++)
    {
      LArReco::Track2d* trk = (LArReco::Track2d*)trks->At(i);
      
      // all CRPs
      objs1.back().push_back( new TGraph() );
      objs1.back().back()->SetMarkerStyle(25);
      objs1.back().back()->SetMarkerSize(0.4);
      objs1.back().back()->SetMarkerColor(4);
      
      
      // get all track hits
      std::vector<LArReco::RecoObjLink> trkhits = trk->GetHits();
      
      lastcrm = -1;
      for(size_t j=0;j<trkhits.size();j++)
	{
	  LArReco::QHit *hit = (LArReco::QHit*)hits->At( trkhits[j].ObjIdx );
	  if(lastcrm != hit->GetCRM()) 
	    {
	      lastcrm = hit->GetCRM();
	      objs1[lastcrm].push_back( new TGraph() );
	      objs1[lastcrm].back()->SetMarkerStyle(25);
	      objs1[lastcrm].back()->SetMarkerSize(0.4);
	      objs1[lastcrm].back()->SetMarkerColor(4);
	    }
	  double xval = hit->GetX();
	  double yval = hit->GetD();
	  
	  // fill last graphs added
	  // all CRPs
	  objs1.back().back()->SetPoint(objs1.back().back()->GetN(), xval, yval);
	  // each CRM
	  objs1[lastcrm].back()->SetPoint(objs1[lastcrm].back()->GetN(), xval, yval);

	} // end loop over track hits
      
      // do the same for delta ray hits if any
      std::vector<LArReco::RecoObjLink> trkdrhits = trk->GetDRays();
      if(trkdrhits.empty()) continue;
      
      objs2.back().push_back( new TGraph() );
      objs2.back().back()->SetMarkerStyle(24);
      objs2.back().back()->SetMarkerSize(0.4);
      objs2.back().back()->SetMarkerColor(3);
      
      lastcrm = -1;
      for(size_t j=0;j<trkdrhits.size();j++)
	{
	  LArReco::QHit *hit = (LArReco::QHit*)hits->At( trkdrhits[j].ObjIdx );
	  if(lastcrm != hit->GetCRM()) 
	    {
	      lastcrm = hit->GetCRM();
	      objs2[lastcrm].push_back( new TGraph() );
	      objs2[lastcrm].back()->SetMarkerStyle(24);
	      objs2[lastcrm].back()->SetMarkerSize(0.4);
	      objs2[lastcrm].back()->SetMarkerColor(3);
	    }
	  double xval = hit->GetX();
	  double yval = hit->GetD();
	  
	  // fill last graphs added
	  // all CRPs
	  objs2.back().back()->SetPoint(objs2.back().back()->GetN(), xval, yval);
	  // each CRM
	  objs2[lastcrm].back()->SetPoint(objs2[lastcrm].back()->GetN(), xval, yval);

	} // end loop over track hits
      
    } // end loop over all tracks
}

//
//
//
void RecoViewer::Refresh(int iview)
{
  if(iview == 0)
    RefreshView0();
  else if(iview == 1)
    RefreshView1();
  else
    {
      RefreshView0();
      RefreshView1();
    }
}


//
//
//
void RecoViewer::RefreshView0()
{
  TGraph *grhits = fHitsView0[fSelCRMView0];
  
  TCanvas *cnvs = fCnvsView0->GetCanvas();
  cnvs->cd();
  cnvs->SetLeftMargin(0.12);
  fAxisView0[fSelCRMView0]->Draw("axis");
  fAxisView0[fSelCRMView0]->GetYaxis()->SetTitleOffset(1.4);
  fAxisView0[fSelCRMView0]->GetXaxis()->UnZoom();
  fAxisView0[fSelCRMView0]->GetYaxis()->UnZoom();


  // no points?
  if(grhits->GetN() == 0) 
    {
      //cnvs->Clear();
      cnvs->Modified();
      cnvs->Update();
      return;
    }

  grhits->Draw("psame");
  grhits->GetXaxis()->SetTitle("X (cm)");
  grhits->GetYaxis()->SetTitle("Drift distance (cm)");

  
  // draw track hits
  for(size_t j=0;j<fTrkHitsView0[fSelCRMView0].size();j++)
    {
      fTrkHitsView0[fSelCRMView0][j]->Draw("psame");
    }

  // draw track delta-ray hits
  for(size_t j=0;j<fTrkDrHitsView0[fSelCRMView0].size();j++)
    {
      fTrkDrHitsView0[fSelCRMView0][j]->Draw("psame");
    }

  // draw track lines
  for(size_t j=0;j<fTrksView0.size();j++)
    fTrksView0[j]->Draw("lsame");
  
  cnvs->Modified();
  cnvs->Update();
}


//
//
//
void RecoViewer::RefreshView1()
{
  TGraph *grhits = fHitsView1[fSelCRMView1];
  
  TCanvas *cnvs = fCnvsView1->GetCanvas();
  cnvs->cd();
  cnvs->SetLeftMargin(0.12);
  fAxisView1[fSelCRMView1]->Draw("axis");
  fAxisView1[fSelCRMView1]->GetYaxis()->SetTitleOffset(1.4);
  fAxisView1[fSelCRMView1]->GetXaxis()->UnZoom();
  fAxisView1[fSelCRMView1]->GetYaxis()->UnZoom();

  // no points?
  if(grhits->GetN() == 0) 
    {
      //cnvs->Clear();
      cnvs->Modified();
      cnvs->Update();
      return;
    }
  grhits->Draw("psame");
  grhits->GetXaxis()->SetTitle("X (cm)");
  grhits->GetYaxis()->SetTitle("Drift distance (cm)");


  // draw track hits
  for(size_t j=0;j<fTrkHitsView1[fSelCRMView1].size();j++)
    {
      fTrkHitsView1[fSelCRMView1][j]->Draw("psame");
    }

  // draw track delta-ray hits
  for(size_t j=0;j<fTrkDrHitsView1[fSelCRMView1].size();j++)
    {
      fTrkDrHitsView1[fSelCRMView1][j]->Draw("psame");
    }

  // draw track lines
  for(size_t j=0;j<fTrksView1.size();j++)
    fTrksView1[j]->Draw("lsame");


  cnvs->Modified();
  cnvs->Update();
}


//
//
//
void RecoViewer::BuildAxis( std::vector< Rectangle > dims, std::vector< TH2F* > &axis, int iview )
{
  clear( axis );
  
  for(size_t i=0;i<dims.size();i++)
    {
      const char* hname = Form("axis_view%d_crm%d", iview, (int)i);
      int nx = (dims[i].x2 - dims[i].x1)/fAxisBinSize;
      int ny = (dims[i].y2 - dims[i].y1)/fAxisBinSize;
      
      if(ny == 0)
	cout<<i<<" "<<dims[i].y1 <<", "<<dims[i].y2 << std::endl;
      
      TH2F *haxis = new TH2F(hname, hname, nx, dims[i].x1, dims[i].x2, ny, dims[i].y1, dims[i].y2);
      haxis->SetDirectory(0);
      haxis->SetStats(0);
      haxis->SetTitle(Form("View %d", iview));
      haxis->GetXaxis()->SetTitle(Form("View%d X (cm)",iview));
      haxis->GetYaxis()->SetTitle("Drift D (cm)");
      axis.push_back( haxis );
    }

  //std::cout<< dims.back().x1 <<", "<<dims.back().x2 << std::endl;
  //std::cout<< dims.back().y1 <<", "<<dims.back().y2 << std::endl;
  //exit(-1);
}
