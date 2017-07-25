/////////////////////////////////////////////////////////////////////////////////////////
//
//   Monitoring task for the benchmark reconstructed distributions
//  
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <sstream>

#include <TAxis.h>
#include <TFile.h>


#include "RootUtils.h"
#include "MonBenchDist.h"

using namespace std;


//
// ctor
//
MonBenchDist::MonBenchDist() : MonTask()
{
  fName  = "benchmark";         // unique name to be associated with files
  fTKey  = "Recon";             // tree entry under in the monitor tasks
  fSEnv  = "toshow_benchmark";  // enviroment for latest directory
  fFExt  = ".root";             // extension of the files we will pick up

  fCnvsWinWsz = 1200;
  fCnvsWinHsz =  700;


  fItemList.push_back("CRP_RecoHits");
  fHitsChView0  = NULL;
  fHitsChView1  = NULL;
  fHitsChQView0 = NULL; 
  fHitsChQView1 = NULL; 
  
  fItemList.push_back("CRP_RecoTracks");
  fTrkStartView0 = NULL;
  fTrkStartView1 = NULL;
  fTrkEndView0   = NULL;
  fTrkEndView1   = NULL;
}

//
// dtor
//
MonBenchDist::~MonBenchDist()
{
  Clear();
}

void MonBenchDist::Clear()
{
  if( fHitsChView0 ) fHitsChView0->Delete();
  if( fHitsChView1 ) fHitsChView1->Delete();
  if( fHitsChQView0 ) fHitsChQView0->Delete();
  if( fHitsChQView1 ) fHitsChQView1->Delete();

  if( fTrkStartView0 ) fTrkStartView0->Delete();
  if( fTrkStartView1 ) fTrkStartView1->Delete();
  if( fTrkEndView0 ) fTrkEndView0->Delete();
  if( fTrkEndView1 ) fTrkEndView1->Delete();
}

//
//
//
void MonBenchDist::DoDraw( size_t idx, TCanvas *cnvs )
{
  bool redraw = NewCanvas( fItemList[idx], cnvs );
  if( redraw ) cnvs->Divide(2,2);
  
  // new canvas or new data 
  redraw |= fGotData;
  
  if(redraw)
    {
      if( idx == 0 )
	{
	  cnvs->cd(1);
	  if(fHitsChView0) fHitsChView0->Draw();
	  
	  cnvs->cd(2);
	   if(fHitsChView1) fHitsChView1->Draw();
	  
	  cnvs->cd(3);
	  if(fHitsChQView0) fHitsChQView0->Draw();
	  
	  cnvs->cd(4);
	  if(fHitsChQView1) fHitsChQView1->Draw();
	}
      else if( idx == 1 )
	{
	  cnvs->cd(1);
	  if(fTrkStartView0) fTrkStartView0->Draw();
	  
	  cnvs->cd(3);
	  if(fTrkStartView1) fTrkStartView1->Draw();
	  
	  cnvs->cd(2);
	  if(fTrkEndView0) fTrkEndView0->Draw();

	  cnvs->cd(4);
	  if(fTrkEndView1) fTrkEndView1->Draw();
	}
      cnvs->cd();
    }	
}



//
//
//
void MonBenchDist::DoFileRead( std::string &fname )
{
  TFile *fin = TFile::Open( fname.c_str() );
  if( !fin->IsOpen() ) return;
  
  TH1F *h;
  fGotData = false;

  //
  h = (TH1F*)fin->Get("hittedwire#_v0_CRM0");
  fGotData |= Copy( h, fHitsChView0, "ChHitsView0", "view0 ch");

  //
  h = (TH1F*)fin->Get("hittedwire#_v1_CRM0");
  fGotData |=  Copy( h, fHitsChView1, "ChHitsView1", "view1 ch");
  
  //
  h = (TH1F*)fin->Get("tothitcharge_v0_CRM0");
  fGotData |= Copy( h, fHitsChQView0, "QHitsView0", "Q (fC)");

  //
  h = (TH1F*)fin->Get("tothitcharge_v1_CRM0");
  fGotData |= Copy( h, fHitsChQView1, "QHitsView1", "Q (fC)");

  
  //
  h = (TH1F*)fin->Get("trkfirstpoint_v0_CRM0");
  fGotData |= Copy( h, fTrkStartView0, "TrkStartX", "x (cm)");
  
  //
  h = (TH1F*)fin->Get("trkfirstpoint_v1_CRM0");
  fGotData |= Copy( h, fTrkStartView1, "TrkStartY", "y (cm)");
  
  //
  h = (TH1F*)fin->Get("trklastpoint_v0_CRM0");
  fGotData |= Copy( h, fTrkEndView0, "TrkEndX", "x (cm)");
  
  //
  h = (TH1F*)fin->Get("trklastpoint_v1_CRM0");
  fGotData |= Copy( h, fTrkEndView1, "TrkEndY", "y (cm)");

  fin->Close();
}


//
//
//
bool MonBenchDist::Copy( TH1F* from, TH1F* &to, const char *name, const char *xtitle )
{
  if( !from ) return false;

  if( to ) to->Delete();
  to = NULL;
  
  to = (TH1F*)from->Clone(name);
  to->SetTitle( name );
  to->SetDirectory(0); // make memory resident
  to->SetLineColor(2);
  to->SetLineWidth(2);
  to->GetXaxis()->SetTitle(xtitle);
  to->SetStats(0);
  return true;
}
