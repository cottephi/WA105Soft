#include <iostream>
#include <vector>
#include <TVector2.h>

void GrayPalette()
{  
  int ncol=50;
  double dcol = 1./float(ncol);
  double gray = 1;
  TColor **theCols = new TColor*[ncol];
  for (int i=0;i<ncol;i++) theCols[i] = new TColor(999-i,0.0,0.7,0.7);
  for (int j = 0; j < ncol; j++) {
    theCols[j]->SetRGB(gray,gray,gray);
    gray -= dcol;
  }
  int ColJul[100];
  for  (int i=0; i<100; i++) ColJul[i]=999-i;
  gStyle->SetPalette(ncol, ColJul);
}

void DrawHits(const char *fname, int iView, int crm, int entry = 0)
{
  gSystem->Load("libdatamodel.so");
  
  TGraph *grhits = new TGraph();
 

  TFile *fin  = TFile::Open(fname);
  TTree *tree = (TTree*)fin->Get("EventTree");
  
  TObjArray *hitsView = new TObjArray();
  //TClonesArray *hitsView = new TClonesArray("LArReco::QHit");
  
  if(iView == 0)
    {
      if(tree->SetBranchAddress("RecoHitsView0_Data", &hitsView) != 0)
	return;
    }
  else
    {
      if(tree->SetBranchAddress("RecoHitsView1_Data", &hitsView) != 0)
	return;
    }

  tree->GetEntry(entry);
 
  cout<<"Number of hits in view "<<iView
      <<" for all CRMs "<<hitsView->GetEntries()<<endl;

  for(int i=0;i<hitsView->GetEntries();i++)
    {
      LArReco::QHit* hit = (LArReco::QHit*)hitsView->At(i);
      if(crm != -1 && hit->GetCRM() != crm) continue;
      //hit->Print();
      
      //{
      //cout<<hit->GetD()<<endl;
      grhits->SetPoint(i, hit->GetX(), hit->GetD());
    }
  
  fin->Close();
  grhits->SetMarkerStyle(6);
  grhits->Draw("ap");
}

// example function to read list of hits from a file
void GraphicHits(const char *fname, std::vector<std::vector<TGraph*>> &grHits, 
		 int ncrm, int iView, int entry = 0)
{
  gSystem->Load("libdatamodel.so");
  
  
  for(size_t ii=0;ii<grHits.size();ii++)
    {
      while(!grHits[ii].empty())
	{
	  delete grHits[ii].back();
	  grHitsp[ii].pop_back();
	}
    }
  grHits.clear();
  

  TFile *fin  = TFile::Open(fname);
  TTree *tree = (TTree*)fin->Get("EventTree");
  
  TObjArray *hitsView = new TObjArray();
  //TClonesArray *hitsView = new TClonesArray("LArReco::QHit");
  
  if(iView == 0)
    {
      if(tree->SetBranchAddress("RecoHitsView0_Data", &hitsView) != 0)
	return;
    }
  else
    {
      if(tree->SetBranchAddress("RecoHitsView1_Data", &hitsView) != 0)
	return;
    }

  tree->GetEntry(entry);
 
  cout<<"Number of hits in view "<<iView
      <<" for all CRMs "<<hitsView->GetEntries()<<endl;

  grHits.resize(ncrm);

  int lastcrm = -1;
  int hitcounts = 0;
  std::vector<TGraph*> hitBoxes;
  for(int i=0;i<hitsView->GetEntries();i++)
    {
      LArReco::QHit* hit = (LArReco::QHit*)hitsView->At(i);
      //hit->Print();
      
      if(lastcrm != hit->GetCRM())
	{
	  if(lastcrm != -1)
	    cout << " CRM "<< lastcrm <<" has hits "<<hitcounts<<endl;
	  if(lastcrm != -1)
	    {
	      
	      grHits[lastcrm] = hitBoxes;
	      //grHits.push_back(hitBoxes);
	    }

	  lastcrm = hit->GetCRM();
	  hitBoxes.clear();

	  hitcounts = 0;
	}
      hitcounts++;
      
      //if(crm == hit->GetCRM())
      //{
      TGraph *gr = new TGraph(5);
      gr->SetPoint(0, hit->GetCh(), hit->GetTbStart());
      gr->SetPoint(1, hit->GetCh()+1, hit->GetTbStart());
      gr->SetPoint(2, hit->GetCh()+1, hit->GetTbEnd());
      gr->SetPoint(3, hit->GetCh(), hit->GetTbEnd());
      gr->SetPoint(4, hit->GetCh(), hit->GetTbStart());
      gr->SetLineColor(2);
      hitBoxes.push_back(gr);
      //}
    }
  
  if(lastcrm != -1)
    cout << " CRM "<< lastcrm <<" has hits "<<hitcounts<<endl;
  
  // last crm
  //grHits.push_back(hitBoxes);
  grHits[lastcrm] = hitBoxes;

  fin->Close();
}

// display charge data and hits
void DrawCRM(const char *fhisto, const char *fhits = NULL, int view = 0) 
{
  if(view > 1) view = 0;

  //GrayPalette();
  
  int ncrm = 4;

  float aspect = 2.0; //aspect ratio is 2:1
  int c_width  = 700;
  TCanvas *c1 = new TCanvas("c1","c1", c_width, aspect*c_width);
  
  c1->Divide(2,2,0,0);
  
  TFile *fin = TFile::Open(fhisto);
  TH2F *h1 = (TH2F*)fin->Get(Form("h2_view%d_trueq_crm0",view));
  h1->GetXaxis()->SetNdivisions(505);
  h1->GetYaxis()->SetNdivisions(505);
  h1->GetXaxis()->SetLabelSize(0.05);
  h1->GetYaxis()->SetLabelSize(0.05);
  TH2F *h2 = (TH2F*)fin->Get(Form("h2_view%d_trueq_crm1",view));
  h2->GetXaxis()->SetNdivisions(505);
  h2->GetYaxis()->SetNdivisions(505);
  h2->GetXaxis()->SetLabelSize(0.05);
  h2->GetYaxis()->SetLabelSize(0.05);
  TH2F *h3 = (TH2F*)fin->Get(Form("h2_view%d_trueq_crm2",view));
  h3->GetXaxis()->SetNdivisions(505);
  h3->GetYaxis()->SetNdivisions(505);
  h3->GetXaxis()->SetLabelSize(0.05);
  h3->GetYaxis()->SetLabelSize(0.05);
  TH2F *h4 = (TH2F*)fin->Get(Form("h2_view%d_trueq_crm3",view));
  h4->GetXaxis()->SetNdivisions(505);
  h4->GetYaxis()->SetNdivisions(505);
  h4->GetXaxis()->SetLabelSize(0.05);
  h4->GetYaxis()->SetLabelSize(0.05);

  std::vector<std::vector<TGraph*>> grHits;
  if(fhits)
    GraphicHits(fhits, grHits, ncrm, view, 0);
  
  c1->cd(3);
  gPad->SetTickx(2);
  h1->Draw("col");
  if(!grHits.empty())
    {
      if(!grHits[0].empty())
	for(size_t ii=0;ii<grHits[0].size();ii++)
	  grHits[0][ii]->Draw("lsame");
    }
  
  
  c1->cd(4);
  gPad->SetTickx(2);
  gPad->SetTicky(2);
  h2->GetYaxis()->SetLabelOffset(0.01);
  h2->Draw("col");
  if(!grHits.empty())
    {
      if(!grHits[1].empty())
	for(size_t ii=0;ii<grHits[1].size();ii++)
	  grHits[1][ii]->Draw("lsame");
    }
  
  
  c1->cd(1);
  h3->Draw("col");
  if(!grHits.empty())
    {
      if(!grHits[2].empty())
	for(size_t ii=0;ii<grHits[2].size();ii++)
	  grHits[2][ii]->Draw("lsame");
    }
  
    
  c1->cd(2);
  gPad->SetTicky(2);
  h4->Draw("col");
  if(!grHits.empty())
    {
      if(!grHits[3].empty())
	for(size_t ii=0;ii<grHits[3].size();ii++)
	  grHits[3][ii]->Draw("lsame");
    }
}      
