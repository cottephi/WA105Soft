//////////////////////////////////////////////////////////////////////////
//
//  Abstract base class for hit finding algorithms
//
//  Created: Fri Jul  3 14:34:53 CEST 2015  
//
//////////////////////////////////////////////////////////////////////////
#include <iostream>

#include "AlgoHitFinder.h"

// ctor
AlgoHitFinder::AlgoHitFinder()
{
  dT     = 0;
  Ped    = 0;
  PedRms = 0;
  tStart = 0;
  AlgoName = "AlgoHitFinder";
}

// dtor
AlgoHitFinder::~AlgoHitFinder()
{
  Data.clear();
  ClearHits();
}

// Clear
void AlgoHitFinder::ClearHits()
{
  while(!Hits.empty())
    {
      delete Hits.back();
      Hits.pop_back();
    }
}

// do pedestal subtraction
void AlgoHitFinder::SubPedestal()
{
  if(Ped == 0) return;
  
  for(size_t i=0;i<Data.size();i++)
    Data[i] -= Ped;
}

// print information
void AlgoHitFinder::Print()
{
  std::cout << "Hits found with algorithm '" << AlgoName <<"'"<< std::endl;
  for(size_t i=0;i<Hits.size();i++)
    {
      std::cout<< ">>>> Hit " << i <<std::endl;
      Hits[i]->Print();
    }
}
  

void AlgoHitFinder::SaveHists(const char *hbasename, TDirectory *dirout)
{
  if(Data.empty()) return;

  const char *h1name = Form("%s_data_tbin", hbasename);
  TH1F *h1 = new TH1F(h1name, h1name, Data.size(), tStart, tStart + Data.size());
  h1->SetDirectory(0);
  h1->GetXaxis()->SetTitle("Time bin");
  h1->GetYaxis()->SetTitle("ADC");
  
  for(size_t i=0;i<Data.size();i++)
    {
      h1->SetBinContent(i+1, Data[i]);
    }

  // write and delete
  dirout->cd();
  h1->Write();
  h1->Delete();
}
  
