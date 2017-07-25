//////////////////////////////////////////////////////////////////////
//
// Hit finder based on MultiHit algorithm from Qscan
// see D. Lussi, Diss.-No. ETH 21314    
//
//////////////////////////////////////////////////////////////////////
#include <iostream>

#include "AlgoMultiHit.h"

AlgoMultiHit::AlgoMultiHit( float calibc, float relth1, float relth2, 
			    float absth1, float absth2, 
			    float abovetbin, UInt_t padleft, UInt_t padright )
{
  AlgoName = "AlgoMultiHit";
  
  //dT       = dt;
  calibC   = calibc; // calibration constant
  relTh1   = relth1;
  relTh2   = relth2;
  absTh1   = absth1;
  absTh2   = absth2;
  aboveTb  = abovetbin;
  
  // add bins on left and right of the hit 
  // if possible to avoid bias due to threshold
  padLeft  = padleft;
  padRight = padright;
}


AlgoMultiHit::~AlgoMultiHit()
{;}


void AlgoMultiHit::FindHits( float dt, UInt_t tstart, std::vector<float> &datafrag, 
			     float ped, float pedrms, bool subped )
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
  Algorithm();

  // finalize
  FinalizeHits();
}


// Algorithm (converted from Qscan with some modifications)
void AlgoMultiHit::Algorithm()
{
  
  // Hit finding parameters
  float relThresh1 = relTh1;
  float relThresh2 = relTh2;
  float absThresh1 = absTh1;
  float absThresh2 = absTh2;
  UInt_t minWidth  = (UInt_t)(aboveTb);
  double rms       = PedRms;
  float threshold1;
  float threshold2;
  
  LArReco::QHit *hit = NULL;
  
  
  if( rms < 0 ) return;
  if( rms == 0.0 ) 
    {
      threshold1=1.;
      threshold2=1.;
    }
  else 
    {
      threshold1 = absThresh1; 
      if ( relThresh1*rms<threshold1 ) 
	threshold1 = relThresh1*rms;
      threshold2 = absThresh2; 
      if (relThresh2*rms<threshold2) 
	threshold2 = relThresh2*rms;
    }

  if(threshold1 < 1.0) threshold1 = 1.0;
  if(threshold2 < 1.0) threshold2 = 1.0;

  //reset variables
  int hitFlag = 0;
  int singleHitFlag = 1;
  float minimum     = 1000;
  int minimumSample = -1;
  
  // loop over the drift samples
  for(size_t ii=0;ii<Data.size();ii++) 
    {
      if(Data[ii]>threshold1) {// we're within a hit or a hit-group
	
	if (hitFlag==0) { // initialize a new single hit / hit-group
	  //std::cout << " new hit flag "<<std::endl;
	  hitFlag = 1;
	  singleHitFlag = 1;

	  // new hit
	  hit = new LArReco::QHit();
	  hit->SetTbStart( ii );
	  hit->SetTbMax( ii );
	  hit->SetAmplitude( Data[ii] );

	  minimumSample = -1;

	}
      
	// update minimum and finDrift of current hit
	if( ii > hit->GetTbMax() && 
	    Data[ii]<hit->GetAmplitude()-threshold2 && 
	    (minimumSample==-1 || minimum>=Data[ii])) 
	  {
	    minimumSample = (int)ii;
	    minimum = Data[ii];
	  }
	
	// check for adjacent hit
	if (minimumSample>=0 && (int)ii>minimumSample && Data[ii]>minimum+threshold2) 
	{
	  hit->SetTbEnd( (UInt_t)(minimumSample-1) );
	  Hits.push_back(hit);
	  
	  //init new hit
	  hitFlag = 1;
	  singleHitFlag = 0;

	  // new hit
	  hit = new LArReco::QHit();
	  hit->SetTbStart( (UInt_t)minimumSample );
	  hit->SetTbMax( ii );
	  hit->SetAmplitude( Data[ii] );
	  
	  minimumSample = -1;
	}
      
      //update amplitude of current hit
      if (hit->GetTbEnd() == 0 && Data[ii]>hit->GetAmplitude()) 
	{
	  hit->SetTbMax( ii );
	  hit->SetAmplitude( Data[ii] );
	}
      }
      else if ( hitFlag == 1 ) // below threshold
	{
	  hitFlag = 0;
	  hit->SetTbEnd( ii-1 );	
	  if ((singleHitFlag && (ii - hit->GetTbStart()) >= minWidth) ||  // valid single hit
	      !singleHitFlag) 
	    {
	      Hits.push_back(hit);
	    }
	  else // not a valid hit
	    delete hit;
	}
    }
}


void AlgoMultiHit::FinalizeHits()
{
  for(size_t i=0;i<Hits.size();i++)
    {
      UInt_t iwindow, fwindow;
      if(((int)Hits[i]->GetTbStart() - (int)padLeft) > 0)
	iwindow =  Hits[i]->GetTbStart() - padLeft; 
      else
	iwindow = 0;
      
      fwindow = Hits[i]->GetTbEnd() + padRight;
      if(fwindow >= Data.size()) fwindow = Data.size()-1;
      
      // check for everlaps with the nearby hits
      if(i > 0) 
	{
	  if(iwindow <= (Hits[i-1]->GetTbEnd()+padRight))
	    iwindow = (UInt_t)(0.5*(Hits[i-1]->GetTbEnd() + Hits[i]->GetTbStart())+1); 
	}
      if( i != (Hits.size() - 1) )
	{
	  if(fwindow >= (Hits[i+1]->GetTbStart()-padLeft))
	    fwindow = (UInt_t)(0.5*(Hits[i]->GetTbEnd() + Hits[i+1]->GetTbStart())+1);
	}
      
      if(iwindow >= fwindow )
	{
	  std::cerr<<"Oh no, I screwed up the limits"<<std::endl;
	  iwindow = Hits[i]->GetTbStart();
	  fwindow = Hits[i]->GetTbEnd();
	  //continue;
	}
      
      //std::cout << " Integrating window " << iwindow <<", "<<fwindow<<std::endl;
      ComputeSignalIntegral(Hits[i], iwindow, fwindow);
    }
  

  for(size_t i=0;i<Hits.size();i++)
    {
      Hits[i]->ShiftHitTb(tStart);
      Hits[i]->SetT(Hits[i]->GetTbStart() * dT);
      Hits[i]->ValidateHitParam();
    }
}

// compute integral
void AlgoMultiHit::ComputeSignalIntegral(LArReco::QHit *hit, UInt_t tbins, UInt_t tbine)
{
  //
  Double_t integral = 0;
  for(size_t i=tbins;i<=tbine;i++)
    {
      integral += Data[i] * dT;
    }
  
  integral /= calibC;
  
  hit->SetQ(integral);
}
