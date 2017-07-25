////////////////////////////////////////////////////////////////////////////////////
//
//  Class for hit reconstuction with support for POSIX threads
//
//  Created: Tue Jul  7 11:56:23 CEST 2015
//
////////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "HitRecon.h"

using namespace std;

// ctor
HitRecon::HitRecon()
{
  m_fout = NULL;
  hitFinder = NULL;
}


// dtor
HitRecon::~HitRecon()
{
  if(hitFinder) 
    delete hitFinder;
}


// find hits
void HitRecon::FindHits( LArRawData::DataChan *chdata )
{
  // clear the internal list of hits
  Hits.clear();

  // get the instance of hit recon config class
  HitReconConfig *hitRecConf = HitReconConfig::Instance();

  RawChDataROI roifind;
  roifind.SetNSigTh(hitRecConf->ROI_nSigTh);
  roifind.SetNSigLow1(hitRecConf->ROI_nSigLow1);
  roifind.SetNSigLow2(hitRecConf->ROI_nSigLow2);
//  roifind.SetNPadRight(hitRecConf->ROI_padLeft);
  roifind.SetNPadRight(hitRecConf->ROI_padRight);
  roifind.SetNPadLeft(hitRecConf->ROI_padLeft);

  // get data framgents
  size_t nfrags = 0;
  std::vector< LArRawData::DataFragment<Float_t> > myROIs;
  // get pedestal and ROIs
  if( chdata->GetChanStatus() == LArRawData::FULLDATA )
    {
      nfrags = roifind.FindROI(chdata, myROIs, hitRecConf->ROI_adcTh, true);
    }
  else if( chdata->GetChanStatus() == LArRawData::ZSDATA ) 
    {
      TClonesArray *frgs = chdata->GetDataFragments();
      nfrags = chdata->GetNFrags();
      // push it to the vector (should change at some point)
      for(size_t i=0;i<nfrags;i++)
	{
	  myROIs.push_back( *((LArRawData::DataFragment<Float_t>*)frgs->At(i)) );
	}
    }
  else
    {
      cerr << "ERROR : cannot treat this datatype to find hits" << endl;
    }
  
  if( nfrags == 0)
    {
      return;
    }
  
  float ped_ch  = chdata->GetPed();
  float ped_rms = chdata->GetPedRMS();
  
  //cout << endl;
  //cout <<" Processing channel    : "<<chdata->GetChanId()<<endl;
  //cout <<" Computed pedestal     : "<<ped_ch<<endl;
  //cout <<" Computed pedestal rms : "<<ped_rms<<endl;
  
  // now calculate deconvolution for each ROI
  if(!hitFinder) 
    hitFinder = hitRecConf->CreateHitAlgo();
  
  // loop over fragments
  for(size_t i=0;i<myROIs.size();i++)
    {
      LArRawData::DataFragment<Float_t> *Frag = &myROIs[i];

      std::vector<float> roidata;
      UInt_t fStrt = Frag->GetData(roidata);
      
      // find hits
      hitFinder->FindHits(hitRecConf->dT, fStrt, roidata, ped_ch, ped_rms, false);
      
      if(m_fout) //for debug only!
	hitFinder->SaveHists(Form("%s_ch%d_f%d", hitFinder->GetName().c_str(),
				  chdata->GetChanId(),(int)fStrt), m_fout);
      
      std::vector<LArReco::QHit*> htemp = hitFinder->GetHits();
      for(size_t i=0;i<htemp.size();i++)
	{
	  Hits.push_back(*htemp[i]);
	  Hits.back().SetCh(chdata->GetChanId());
	}
    }
  
  myROIs.clear();
  return;
}
