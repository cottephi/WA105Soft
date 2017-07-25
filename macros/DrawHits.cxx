#include <iostream>
#include <vector>
#include <TVector2.h>

// example macro to draw hits and hits from trackas

void DrawHits(const char *fname, int iView, int crm, int entry = 0)
{
  gSystem->Load("libdatamodel.so");
  
  TGraph *grhits = new TGraph();
 

  TFile *fin  = TFile::Open(fname);
  TTree *tree = (TTree*)fin->Get("EventTree");
  
  TObjArray *hits = new TObjArray();
  TObjArray *trks = new TObjArray();
  
  bool hastrks = false;

  if(iView == 0)
    {
      if(tree->SetBranchAddress("RecoHitsView0_Data", &hits) != 0)
	return;
      hastrks = (tree->SetBranchAddress("RecoCRTracks_View0", &trks) == 0);
    }
  else
    {
      if(tree->SetBranchAddress("RecoHitsView1_Data", &hits) != 0)
	return;
      hastrks = (tree->SetBranchAddress("RecoCRTracks_View1", &trks) == 0);
    }
  
  
  tree->GetEntry(entry);
 
  cout<<"Number of hits in view "<<iView
      <<" for all CRMs "<<hits->GetEntries()<<endl;
  if(hastrks)
    {
      cout<<"Number of trks in view "<<iView
	  <<" for all CRMs "<<trks->GetEntries()<<endl;
    }
  
  // draw hits
  TGraph *grhits = new TGraph();
  for(int i=0;i<hits->GetEntries();i++)
    {
      LArReco::QHit* hit = (LArReco::QHit*)hits->At(i);
      if(crm != -1 && hit->GetCRM() != crm) continue;
      //hit->Print();
      
      grhits->SetPoint(i, hit->GetX(), hit->GetD());
    }
  
  
  // plot hits associated with track and delta rays
  std::vector< TGraph* > grtrkpnts;
  std::vector< TGraph* > grtrkpntsdr; 
  if(hastrks)
    {
      for(int i=0;i<trks->GetEntries();i++)
	{
      
	  LArReco::Track2d* trk = (LArReco::Track2d*)trks->At(i);
	  
	  if(crm!=-1 && trk->GetCRM() != crm) continue;
      
	  // main track hits
	  TGraph *grtrk   = new TGraph();
	  std::vector<LArReco::RecoObjLink> trkhits = trk->GetHits();
	  for(size_t j=0;j<trkhits.size();j++)
	    {
	      LArReco::QHit *hit = (LArReco::QHit*)hits->At( trkhits[j].ObjIdx );
	      grtrk->SetPoint(j, hit->GetX(), hit->GetD());
	    }
      
	  grtrk->SetMarkerColor(2);
	  grtrk->SetMarkerStyle(6);
	  grtrkpnts.push_back( grtrk );
      
	  TGraph *grtrkdr = new TGraph();
	  std::vector<LArReco::RecoObjLink> trkdrhits = trk->GetDRays();
	  for(size_t j=0;j<trkdrhits.size();j++)
	    {
	      LArReco::QHit *hit = (LArReco::QHit*)hits->At( trkdrhits[j].ObjIdx );
	      grtrk->SetPoint(j, hit->GetX(), hit->GetD());
	    }
	  grtrkdr->SetMarkerColor(4);
	  grtrkdr->SetMarkerStyle(6);
	  if(grtrkdr->GetN() != 0)
	    grtrkpntsdr.push_back( grtrkdr );
	}
    }
    
  fin->Close();
  grhits->SetMarkerStyle(6);
  grhits->Draw("ap");

  for(size_t j=0;j<grtrkpnts.size();j++)
    grtrkpnts[j]->Draw("psame");
  for(size_t j=0;j<grtrkpntsdr.size();j++)
    grtrkpntsdr[j]->Draw("psame");
}


