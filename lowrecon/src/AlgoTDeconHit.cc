//////////////////////////////////////////////////////////////////////////
//
//  Hit finder based on deconvolution in time using NNLS fitter
//  
//  Created: Fri Jul  3 17:12:54 CEST 2015
//
//////////////////////////////////////////////////////////////////////////

#include "AlgoTDeconHit.h"

// ctor
AlgoTDeconHit::AlgoTDeconHit(std::vector<float> &eresp, float hitth)
{ 
  AlgoName = "AlgoTDeconHit";
  
  hitTh = hitth; // threshold for hit finding in fC
  nnlsSolver = new NNLS();
  nnlsSolver->SetResponse(eresp);
}

// dtor
AlgoTDeconHit::~AlgoTDeconHit()
{
  delete nnlsSolver;
}


// find hits
void AlgoTDeconHit::FindHits(float dt, UInt_t tstart, std::vector<float> &datafrag, 
			     float ped, float pedrms, bool subped)
{
  dT     = dt;
  tStart = tstart;
  Data   = datafrag;
  Ped    = ped;
  PedRms = pedrms;
  
  if(subped) // perform pedestal subtraction
    SubPedestal(); 

  //if(!append) // clear hit list
  ClearHits(); 
  
  // find hits
  Qvals.clear();

  //std::cout << " Data size "<<Data.size() << std::endl;
  //if(Data.size() > 200) return;

  if(Data.size() < NNLSMAXBINS ) //process the entire array
    {
      nnlsSolver->Solve(Data, Data.size(), Qvals);
    }
  else // try to subdivide further
    {
      std::vector< std::pair<size_t, size_t> > subsamples;
      SubDivide( subsamples );
      for(size_t i=0;i<subsamples.size();i++)
	{
	  //std::cout<<subsamples[i].first<<", "<<subsamples[i].second<<std::endl;
	  std::vector<float> qtemp;
	  std::vector<float> dtemp(&Data[subsamples[i].first], &Data[subsamples[i].second]);
	  
	  nnlsSolver->Solve(dtemp, dtemp.size(), qtemp); 
	  size_t toffset = subsamples[i].first;
	  
	  for(size_t j=0;j<toffset;j++)
	    Qvals.push_back(0);
	  
	  for(size_t j=0;j<qtemp.size();j++)
	    Qvals.push_back(qtemp[j]);
	  
	}
    }
  
  Algorithm();
  
  // finalize
  FinalizeHits();
}

// process data to find hits
void AlgoTDeconHit::Algorithm()
{
  // no groupt Qvals into hits
  LArReco::QHit *hit = NULL;
  UInt_t Nbins = Qvals.size();
  for(size_t i=0;i<Nbins;i++)
    {
      if(Qvals[i] > hitTh) 
	{
	  hit = new LArReco::QHit();

	  UInt_t MoveFrom = i;
	  UInt_t MoveTo   = 0;

	  if(!Hits.empty()) 
	    MoveTo = Hits.back()->GetTbEnd()+1;
	  
	  hit->SetTbEnd( i );
	  hit->SetTbMax( i );
	  hit->SetAmplitude( Qvals[i] );

	  // add all below treshold deposits
	  for(size_t j=MoveFrom;j>=MoveTo;j--)
	    {
	      if(Qvals[j] <= QZERO) break;
	      
	      hit->SetTbStart( j );
	      hit->SetQ( hit->GetQ() + Qvals[j]);
	    }
	  
	  // go forward
	  MoveFrom = i+1;
	  MoveTo   = Nbins;
	  //bool nexthit = false;
	  int counter  = 0;
	  for(size_t j = MoveFrom;j<MoveTo;j++)
	    {
	      if( Qvals[j] <= QZERO )
		counter++; 
	      
	      if(counter == 1) //break after nth time bin
		break;
	      
	      //if(!nexthit)
	      //{
	      //nexthit = ( Qvals[j] < hitTh );
	      //}
	      
	      //cout << j <<" "<< ydata[j] << endl;
	      hit->SetTbEnd( j );
	      hit->SetQ( hit->GetQ() + Qvals[j]);
	      if(Qvals[j] > hit->GetAmplitude())
		{
		  hit->SetAmplitude( Qvals[j] );
		  hit->SetTbMax( j );
		}
	      i = j;
	    }
	  Hits.push_back(hit);
	}
    }
}

// finalize hits
void AlgoTDeconHit::FinalizeHits()
{
  for(size_t i=0;i<Hits.size();i++)
    {
      Hits[i]->ShiftHitTb(tStart);
      Hits[i]->SetT(Hits[i]->GetTbStart() * dT);
      Hits[i]->ValidateHitParam();
    }
}

// SaveHistos -- useful for debugging / making plots
void AlgoTDeconHit::SaveHists(const char *hbasename, TDirectory *dirout)
{
  //cout<<"Saving histograms to"<<endl;
  //dout->pwd();

  TH1F *h_data    = NULL;
  TH1F *h_dcvdata = NULL;

  MakeHisto(h_data, tStart, Form("%s_time_data", hbasename), Data);
  MakeHisto(h_dcvdata, tStart, Form("%s_time_dcnvdata", hbasename), Qvals);

  //Save histograms and delete
  dirout->cd();
  if(h_data)
    {
      h_data->Write();
      h_data->Delete();
    }
  if(h_dcvdata)
    {
      h_dcvdata->Write();
      h_dcvdata->Delete();
    }
}

// histogram creator: time series
void AlgoTDeconHit::MakeHisto(TH1F* &h, UInt_t tstart, const char *hname, std::vector<float> &data)
{
  if(h) h->Delete();
  h = NULL;
  
  if(data.empty())
    return;
  
  std::vector<double> bins;
  double dt = 1.0 ; 
  for(size_t i=0;i<=data.size();i++)
    {
      bins.push_back( (tstart + i) * dt);
    }
  
  h = new TH1F(hname, hname, bins.size()-1, &bins[0]);
  h->SetDirectory(0); // memory only
  h->GetXaxis()->SetTitle("time bin");

  for(size_t i=0;i<data.size();i++)
    h->SetBinContent(i+1, data[i]);
}


// try to sub-divide into more subsamples
void AlgoTDeconHit::SubDivide(std::vector< std::pair<size_t, size_t> > &subsamples)
{
  subsamples.clear();

  float thresh1 = 3.0 * PedRms;
  float thresh2 = 0.5 * PedRms;
  //int   padSize = 5;

  for(size_t i=0;i<Data.size();i++)
    {
      if(Data[i] > thresh1)
	{
	  size_t strt_idx = i;
	  
	  // step forward
	  size_t end_idx = strt_idx;
	  int counter = 0;
	  for(;end_idx<Data.size();end_idx++)
	    {
	      if(Data[end_idx] < thresh2) break;
	      //if(Data[end_idx] < thresh2) 
	      //counter++;
	      //if(counter > padSize);
	      //break;
	    }
	  
	  // update loop counter
	  i = end_idx;

	  //now step back
	  counter = 0;
	  for(;strt_idx!=0;strt_idx--)
	    {
	      if(Data[strt_idx] < thresh2) break;
	      //if(Data[strt_idx] < thresh2) 
	      //counter++;
	      //if(counter > padSize)
	      //break;
	    }
	  
	  if(strt_idx < end_idx)
	    subsamples.push_back(std::make_pair(strt_idx, end_idx));
	  
	}
    }

  if(subsamples.empty())
    subsamples.push_back(std::make_pair(0, Data.size()));
  
  //for(size_t i=0;i<subsamples.size();i++)
  //std::cout<<subsamples[i].first<<", "<<subsamples[i].second<<std::endl;
  

  // failed to subdivide intelegently --> divide at minimum value
  //if(subsamples.size() == 1 && subsamples[0].first  = 0;) 
  //{
  //subsamples[0].first  = 0;
  //subsamples[0].second = std::min_element(Data.begin(), Data.end()) - Data.begin();
  //subsamples.push_back( std::make_pair(subsamples[0].second+1, Data.size()) );
  //}
}
