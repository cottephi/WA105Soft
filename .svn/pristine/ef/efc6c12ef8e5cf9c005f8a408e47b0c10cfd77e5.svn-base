#include <iostream>
#include <algorithm>
#include <sstream>
#include <cmath>


#include "CRPViewPlotting.h"


using namespace std;

//
// ctor
//
CRPViewPlotting::CRPViewPlotting( std::string viewname, int iview, int maxtdc, 
				  int chlow, int chhigh )
{
  fName  = viewname;
  fView  = iview;
  
  stringstream ss;
  ss<<"Ch in View "<<fView;
  fChAxisName  = ss.str();
  fTdcAxisName = "tdc";
  fAdcAxisName = "ADC";

  // set these to 0 
  fNumTdc = 0;
  fMinTdc = 0;
  fMaxTdc = 0;
  fNumCh  = 0;
  fChLo   = 0;
  fChHi   = 0;
  
  //
  fADCWfThresh = 10;
  fMaxWfRange  = 20;
  
  //
  fViewChTdc = NULL;
  fHAxis     = NULL;
  fMaxAdc   = pow(2, 12) - 1;
  fMaxChAdc = 0;
  
  fMaxScale = 4095;

  // define a rough color scheme for the waveforms
  fLineColors.push_back( kBlue + 4 );
  fLineColors.push_back( kBlue + 3 );
  fLineColors.push_back( kBlue + 2 );
  fLineColors.push_back( kBlue + 1 );
  fLineColors.push_back( kBlue + 0 );
  fLineColors.push_back( kRed + 4 );
  fLineColors.push_back( kRed + 3 );
  fLineColors.push_back( kRed + 2 );
  fLineColors.push_back( kRed + 1 );
  fLineColors.push_back( kRed + 0 );
  fNcols = fLineColors.size()-1;

  SetNewDataFrame( maxtdc, chlow, chhigh );
}


//
// dtor
//
CRPViewPlotting::~CRPViewPlotting()
{
  Clear();
}

//
// clear
//
void CRPViewPlotting::Clear()
{
  if(fViewChTdc) fViewChTdc->Delete();
  fViewChTdc = NULL;

  if(fHAxis) fHAxis->Delete();
  fHAxis = NULL;
  
  while(!fChWfs.empty())
    {
      delete fChWfs.back();
      fChWfs.pop_back();
    }

  fChMaxVals.clear();  
}


//
// reset
//
void CRPViewPlotting::Reset()
{
  if(fViewChTdc) 
    {
      fViewChTdc->Reset();
      fViewChTdc->GetXaxis()->UnZoom();
      fViewChTdc->GetYaxis()->UnZoom();
    }

  if(fHAxis)
    {
      fHAxis->GetXaxis()->UnZoom();
      fHAxis->GetYaxis()->UnZoom();
    }
  
  for(size_t i=0;i<fChWfs.size();i++)
    {
      fChWfs[i]->Reset();
      fChMaxVals[i] = 0;
      fChMinVals[i] = 10000;
    }
    
  fIndxSorted.clear();
  fChMaxValsSorted.clear();
  fMaxChAdc = 0;
}

//
// new data frame
//
void CRPViewPlotting::SetNewDataFrame( int maxtdc, int chlow, int chhigh )
{
  
  // always call reset
  Reset();

  // and can rebuild to new dimensions
  bool rebuild = false;
    
  if(maxtdc != fMaxTdc)
    {
      fMaxTdc = maxtdc;
      rebuild = true;
    }

  if(chlow != fChLo)
    {
      fChLo   = chlow;
      rebuild = true;
    }

  if(chhigh != fChHi)
    {
      fChHi   = chhigh;
      rebuild = true;
    }
  
  if(rebuild) NewDataFrame();
}


//
// (re)build histograms
//
void CRPViewPlotting::NewDataFrame()
{
  Clear();

  fNumTdc = fMaxTdc - fMinTdc + 1;
  fNumCh  = fChHi - fChLo + 1;
  
  fChMaxVals.resize( fNumCh );
  fChMinVals.resize( fNumCh );
  
  const char *hname2d = Form("%s_chtdc", fName.c_str());
  
  //
  fViewChTdc = new TH2F(hname2d, hname2d, fNumCh, fChLo, fChHi+1, fNumTdc, fMinTdc, fMaxTdc+1);
  fViewChTdc->SetDirectory(0);
  fViewChTdc->GetXaxis()->SetTitle(fChAxisName.c_str());
  fViewChTdc->GetYaxis()->SetTitle(fTdcAxisName.c_str());
  
  
  //
  for(int i=0;i<fNumCh;i++)
    {
      int ch = fChLo + i;
      const char *hname; 
      
      if(i==0) //make aixs
	{
	  hname = Form("%s_axis", fName.c_str());
	  fHAxis = new TH1F(hname, hname, fNumTdc, fMinTdc, fMaxTdc+1);
	  fHAxis->SetDirectory(0);
	  fHAxis->GetXaxis()->SetTitle(fTdcAxisName.c_str());
	  fHAxis->GetYaxis()->SetTitle(fAdcAxisName.c_str());
      
	}
      
      hname = Form("%s_ch%d", fName.c_str(), ch);
      TH1F *h = new TH1F(hname, hname, fNumTdc, fMinTdc, fMaxTdc);
      h->SetDirectory(0);
      h->GetXaxis()->SetTitle(fTdcAxisName.c_str());
      h->GetYaxis()->SetTitle(fAdcAxisName.c_str());
      fChWfs.push_back( h );
    }
}



//
//
//
void CRPViewPlotting::Fill(int ch, LArRawData::DataFragment<UShort_t>* wf_adc, float ped)
{
  if(ch < fChLo || ch > fChHi || fNumCh <= 0)
    {
      cerr<<"ERROR: bad ch number "<<ch<<" range [ "
	  <<fChLo<<", "<<fChHi<<"]"<<endl;
      return;
    }
  int id_x = ch - fChLo;
  
  // fill data
  for(UInt_t i=wf_adc->GetFragStart();i<wf_adc->GetFragEnd();i++)
    {
      
      if(i > (UInt_t)fMaxTdc)  //should not happen (in principle)
	{
	  cerr<<"ERROR: bad dimensions detected "<<i
	      <<" tdc exceeds max "<<fMaxTdc<<" tdc"<<endl;
	  break;
	}
      
      float val = (float)(wf_adc->GetDatum(i));
      
      // bad fix for the problem with the first two samples being 0 in 3x1x1
      if( i < 2 && val == 0) val = val; 
      else val -= ped;

      if(val > fMaxChAdc) fMaxChAdc = val;


      int id_y = i - fMinTdc;
      
      // max for this channel
      if(val > fChMaxVals[id_x]) fChMaxVals[id_x] = val;

      // min for this channel
      if(val < fChMinVals[id_x]) fChMinVals[id_x] = val;
            
      fViewChTdc->SetBinContent(id_x+1, id_y+1, val);
      fChWfs[id_x]->SetBinContent(id_y+1, val);
    }
  
}

//
// sort array with max vals
//
void CRPViewPlotting::SortMaxVals()
{
  fIndxSorted.resize( fChMaxVals.size(), 0 );
  for( size_t i=0;i<fChMaxVals.size();i++)
    {
      fIndxSorted[i] = i;
    }
  
  // sort index array
  sort( fIndxSorted.begin(), fIndxSorted.end(), index_sorter<float>(fChMaxVals) );

  // sorted max values
  fChMaxValsSorted.resize( fChMaxVals.size(), 0 );
  for(size_t i=0;i<fIndxSorted.size();i++)
    fChMaxValsSorted[i] = fChMaxVals[ fIndxSorted[i] ];
}


//
// select some fraction of waveforms in a given range
//
void CRPViewPlotting::SelectWFInADCRange(int nwf, int adcth)
{
  fADCWfThresh = adcth;
  fMaxWfRange  = nwf;
}

//
//
//
void CRPViewPlotting::GetWFInADCRange(std::vector<int> &wftodraw)
{
  int adcth = fADCWfThresh;
  int nwf   = fMaxWfRange;

  if(fIndxSorted.empty())
    SortMaxVals();
  
  size_t startpos = 0;
  size_t endpos   = fChWfs.size()-1;
  
  if(adcth < fChMaxVals[ fIndxSorted.back() ] )
    {
      // find lower bound
      std::vector<float>::iterator low;
      low = lower_bound( fChMaxValsSorted.begin(), fChMaxValsSorted.end(), (float)adcth);
      startpos = low - fChMaxValsSorted.begin();
    }
  
  size_t step = (size_t)( (float)( endpos - startpos ) / nwf );
  if(step == 0) step = 1;
  
  wftodraw.clear();

  //  start with the largest
  for(int i=(int)endpos;i>=(int)startpos;i=i-step)
    {
      wftodraw.push_back( fIndxSorted[i] );
    }
}

//
// draw waveforms for channels
//
void CRPViewPlotting::SelectWFInChans(std::string &input)
{
  fWfSelect.clear();
  
  if(input.empty()) 
    return;
  
  int strpos = 0;
  int endpos = (int)fChWfs.size();

  std::istringstream iss(input);
  std::string token;
  
  while( getline(iss, token, ',') )
    {
      size_t pos = token.find('-');
      if( pos == std::string::npos ) // single value
	{
	  int tmp;
	  std::stringstream ss(token);
	  ss >> tmp;
	  tmp -= fChLo;
	  if( tmp >=strpos && tmp < endpos ) 
	    fWfSelect.push_back( tmp );
	}
      else
	{
	  std::stringstream ss1(token.substr(0, pos));
	  std::stringstream ss2(token.substr(pos+1));
	  int tmp1, tmp2, tmp;
	  ss1 >> tmp1;
	  ss2 >> tmp2;
	  if( tmp1 > tmp2 ) std::swap( tmp1, tmp2 );
	  tmp1 -= fChLo; tmp2 -= fChLo;
	  for(tmp=tmp1; tmp<=tmp2; tmp++)
	    {
	      if( tmp >=strpos && tmp < endpos ) 
		fWfSelect.push_back( tmp );
	    }
	}
    }
  
  if( fWfSelect.empty() ) 
    return;
  
  // erase if there are duplicates
  std::sort(fWfSelect.begin(), fWfSelect.end());

  // sort in the increasing order
  fWfSelect.erase( std::unique(fWfSelect.begin(), fWfSelect.end()), fWfSelect.end() );
}


//
// draw total frame on the canvas
//
void CRPViewPlotting::Draw2D(TCanvas *cnvs, float scale2max)
{
  if( !fViewChTdc ) return;

  float maxval = (scale2max) * fMaxScale; //fMaxChAdc;
  fViewChTdc->SetMaximum( maxval );
  
  cnvs->cd();
  fViewChTdc->Draw("col");
  cnvs->Modified();
  cnvs->Update();
}


//
// draw waveforms on the canvas
//
void CRPViewPlotting::DrawWF(TCanvas *cnvs, int tdcmin, int tdcmax)
{
  if( fChWfs.empty() ) return;
  
  std::vector<int> wftodraw;

  // use selected wfs
  if( !fWfSelect.empty() )
    wftodraw = fWfSelect;
  else
    GetWFInADCRange(wftodraw);
  
  //
  if( wftodraw.empty() ) return;

  //
  cnvs->cd();

  // we get the max and min val from histograms themselves
  //// float maxval = fChMaxVals[ fWfSelected[0] ];
  
  float minval =  1000;
  float maxval = -1000;

  for(size_t i=0;i<wftodraw.size();i++)
    {
      int indx = wftodraw[i];
      if(minval > fChMinVals[indx]) 
	minval = fChMinVals[indx];

      if(maxval < fChMaxVals[indx]) 
	maxval = fChMaxVals[indx];

      // do draw
      //if( i == 0 ) fChWfs[indx]->Draw();
      //else 
      //cout<<"Selected "<<indx<<endl;
      //fChWfs[indx]->Draw("same");
    }

  //
  if(minval < 0 ) minval *= 1.1;
  else minval *= 0.9;

  if(maxval < 0 ) maxval *= 0.9;
  else maxval *= 1.1;
  
  // this does not work after zoom out though  
  fHAxis->SetMinimum(minval);
  fHAxis->SetMaximum(maxval);
  fHAxis->GetYaxis()->SetLimits(minval, maxval);
  fHAxis->Draw("AXIS");    
  for(size_t i=0;i<wftodraw.size();i++)
    {
      int indx = wftodraw[i];
      float f  = fChMaxVals[indx]/maxval;
      if(f!=f) f = 0;
      fChWfs[indx]->SetLineColor( GetLineColor(f) );
      fChWfs[indx]->SetLineWidth(2);

      fChWfs[ indx ]->Draw("same");
    }
    
  cnvs->Modified();
  cnvs->Update();
}
