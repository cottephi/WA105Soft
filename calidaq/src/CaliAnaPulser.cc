//////////////////////////////////////////////////////////////////////////////
// 
//  Analysis of the pulser calibration data to get gain calibration 
//  for each ch
//
//
//  Created: Fri Nov 18 16:15:55 CET 2016
//
//////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <iomanip>

#include "LogMsg.h"
#include "RootUtils.h"

#include "CaliAnaPulser.h"

using namespace std;

//
//
//
caliana::CaliAnaPulser::CaliAnaPulser(size_t chid, float norm) : caliana::CaliAnaDaqCh( chid )
{
  m_nres = 5;
  m_keys.resize( m_nres );
  m_evres.resize( m_nres );

  m_keys[PED]    = "pedval";
  m_keys[PEDRMS] = "pedrms";
  m_keys[AMPL]   = "pheight";
  m_keys[PSUM]   = "psum";
  m_keys[TMAX]   = "tmax";

  Reset();
  
  //
  m_thresh = 200; //ADC

  //
  m_tdclo = 0;
  m_tdchi = 0;
  
  // normalization constant: should be in ADC/fC units
  m_norm = norm; 

  //
  m_roifind.SetNSigTh(4);   
  m_roifind.SetNSigLow1(0.5); 
  m_roifind.SetNSigLow2(-0.1); 
  m_roifind.SetNPadRight(10);
  m_roifind.SetNPadLeft(10); 
    
  //
  m_pedext = false;
  m_ped    = 0;
  m_pedrms = 0;
  
  //
  m_histo1 = NULL;
  m_histo2 = NULL;
}


//
//
//
caliana::CaliAnaPulser::~CaliAnaPulser()
{
  if(m_histo1) m_histo1->Delete();
  if(m_histo2) m_histo2->Delete();
}


//
// show what we have made so far
//
void caliana::CaliAnaPulser::Show( TCanvas *cnvs, TDirectory *dirout, std::string opt )
{
  if( opt.compare("pedval") == 0 )
    ShowPedDist( cnvs, dirout );
  else if( opt.compare("pedrms") == 0 )
    ShowPedRmsDist( cnvs, dirout );
  else if( opt.compare("pedsamples") == 0 )
    ShowPedSamples( cnvs, dirout );
  else if( opt.compare("pheight") == 0 )
    ShowPulsePeaks( cnvs, dirout );
  else
    ShowData( cnvs, dirout );
}

//
//
//
void caliana::CaliAnaPulser::Fill(dlardaq::adc16_t* adc, size_t nsz)
{
  
  m_data.clear();
  m_roi.clear();

  //
  bool hasPulse = false;
  for(size_t i=0;i<nsz;i++)
    {
      float datum = adc[i];
      
      //
      if( m_pedext ) datum -= m_ped;
      m_data.push_back(datum);

      if(datum>m_thresh) 
	hasPulse = true;
    }
  
  if(!hasPulse)  // do nothing
    return;

  float ped    = 0;
  float pedrms = 0;
  if( m_tdclo != m_tdchi && m_tdchi > 0 )
    {
      m_roi.push_back( make_pair( m_tdclo, m_tdchi ) );
      std::vector<float> pedsamples;
      for(size_t i=0;i<nsz;i++)
	{
	  if( i<m_tdclo || i > m_tdchi )
	    pedsamples.push_back( m_data[i] );
	}
      FindMeanAndRms( pedsamples, ped, pedrms );
    }
  else
    {
      m_roifind.FindROI( m_data, m_thresh, m_roi, true );
      ped     = m_roifind.GetPed();
      pedrms  = m_roifind.GetPedRMS();
    }

  // save computed pedestal
  m_evres[PED].push_back( ped );
  m_evres[PEDRMS].push_back( pedrms );

  //
  for(size_t i=0;i<m_roi.size();i++)
    {
      ssize_t tdc   = -1;
      float maxval  = 0;
      float sumval  = 0;
      size_t start  = m_roi[i].first;
      size_t end    = m_roi[i].second;
      if( (end - start) > 200 )
	{
	  dlardaq::msg_warn<<"Pulse ROI appears to be to large in event "<<m_evid
			   <<". Skipping... "<<start<<" - "<<end<<std::endl;
	  continue;
	}
      for(size_t j=start;j<=end;j++)
	{
	  float val = m_data[j]; 
	  
	  // subtract pedestal if not external
	  if( !m_pedext ) val -= ped;
	  
	  //
	  if(val > maxval) 
	    {
	      tdc    = j;
	      maxval = val;
	    }
	  sumval += val;
	}
      if( PassCuts( maxval, tdc ) )
	{
	  m_evres[AMPL].push_back( maxval/m_norm );
	  m_evres[PSUM].push_back( sumval/m_norm );
	  m_evres[TMAX].push_back( tdc );
	}
    }
}


//
//
//
bool caliana::CaliAnaPulser::PassCuts(float height, float tval)
{
  bool rval = true;
  
  rval = (m_thresh > 0) && (height > m_thresh);
  
  if( m_tdclo != m_tdchi && (m_tdclo >= 0 && m_tdchi > 0) )
    rval = rval && ( tval > m_tdclo && tval < m_tdchi );
  
  

  return rval;
}



//
//
//
void caliana::CaliAnaPulser::ShowData(TCanvas *cnvs, TDirectory *dirout)
{
  if(m_data.empty()) return;
  
  if(m_histo1) m_histo1->Delete();
  if(m_histo2)  m_histo2->Delete();
  
  stringstream ss;
  ss<<"chdata"<<m_chid<<"_event"<<m_evid;
  m_histo1 = new TH1F(ss.str().c_str(), ss.str().c_str(), m_data.size(), 0, m_data.size());
  m_histo1->SetDirectory(0);
  m_histo1->GetXaxis()->SetTitle("tdc");
  m_histo1->GetYaxis()->SetTitle("adc");
  
  ss<<"roi";
  m_histo2 = new TH1F(ss.str().c_str(), ss.str().c_str(), m_data.size(), 0, m_data.size());
  m_histo2->SetDirectory(0);
  m_histo2->SetLineColor(2);
  m_histo2->GetXaxis()->SetTitle("tdc");
  m_histo2->GetYaxis()->SetTitle("adc");
  
  for(size_t i=0;i<m_data.size();i++)
    {
      m_histo1->SetBinContent(i+1, m_data[i]);
    }
  
  for(size_t i=0;i<m_roi.size();i++)
    {
      size_t start = m_roi[i].first;
      size_t end   = m_roi[i].second;
      for(size_t j=start;j<=end;j++)
	{
	  m_histo2->SetBinContent(j+1, m_data[j]);
	}
    }

  if( cnvs )
    {
      cnvs->cd();
      m_histo1->Draw();
      m_histo2->Draw("same");
      cnvs->Modified();
      cnvs->Update();
    }

  if(dirout)
    {
      dirout->cd();
      m_histo1->Write();
      m_histo2->Write();      
    }
}


//
//
//
void caliana::CaliAnaPulser::ShowPedDist(TCanvas *cnvs, TDirectory *dirout)
{
  if(m_histo1)  m_histo1->Delete();
  
  stringstream ss;
  ss<<"chdata"<<m_chid<<"ped";
  m_histo1 = new TH1F(ss.str().c_str(), ss.str().c_str(), 100, 0, 200);
  m_histo1->SetDirectory(0);
  m_histo1->GetXaxis()->SetTitle( "Ped (ADC)" );

  for(size_t i=0;i<m_evres[PED].size();i++)
    RootUtils::FillConstantBinWidth( *m_histo1, m_evres[PED][i] );
  
  if(cnvs)
    {
      cnvs->cd();
      m_histo1->Draw();
      cnvs->Modified();
      cnvs->Update();
    }

  if( dirout )
    {
      dirout->cd();
      m_histo1->Write();
    }
}


//
//
//
void caliana::CaliAnaPulser::ShowPedSamples(TCanvas *cnvs, TDirectory *dirout)
{
  if(m_histo1)  m_histo1->Delete();
  
  stringstream ss;
  ss<<"chdata"<<m_chid<<"pedsamples_event"<<m_evid;
  m_histo1 = new TH1F(ss.str().c_str(), ss.str().c_str(), 100, 0, 200);
  m_histo1->SetDirectory(0);
  m_histo1->GetXaxis()->SetTitle( "Ped Samples (ADC)" );
  
  size_t start = 0;
  size_t end   = 0;
  for(size_t i=0;i<m_roi.size();i++)
    {
      end = m_roi[i].first;
      for(size_t j=start;j<=end;j++)
	RootUtils::FillConstantBinWidth( *m_histo1, m_data[j]);
      
      i = m_roi[i].second + 1;
      start = i; 
    }
  
  if( cnvs )
    {
      cnvs->cd();
      m_histo1->Draw();
      cnvs->Modified();
      cnvs->Update();
    }


  if( dirout )
    {
      dirout->cd();
      m_histo1->Write();
    }
}


//
//
//
void caliana::CaliAnaPulser::ShowPedRmsDist(TCanvas *cnvs, TDirectory *dirout)
{
  if(m_histo1)  m_histo1->Delete();
  
  stringstream ss;
  ss<<"chdata"<<m_chid<<"pedrms";
  m_histo1 = new TH1F(ss.str().c_str(), ss.str().c_str(), 100, 0, 200);
  m_histo1->SetDirectory(0);
  m_histo1->GetXaxis()->SetTitle( "Ped RMS (ADC)" );
  for(size_t i=0;i<m_evres[PEDRMS].size();i++)
    RootUtils::FillConstantBinWidth( *m_histo1, m_evres[PEDRMS][i] );
  
  if( cnvs )
    {
      cnvs->cd();
      m_histo1->Draw();
      cnvs->Modified();
      cnvs->Update();
    }

  if( dirout )
    {
      dirout->cd();
      m_histo1->Write();
    }
}




//
//
//
void caliana::CaliAnaPulser::ShowPulsePeaks(TCanvas *cnvs, TDirectory *dirout)
{
  if(m_histo1)  m_histo1->Delete();
  
  stringstream ss;
  ss<<"chdata"<<m_chid<<"ampl";
  m_histo1 = new TH1F(ss.str().c_str(), ss.str().c_str(), 2000, m_thresh, 4096);
  m_histo1->SetDirectory(0);
  m_histo1->GetXaxis()->SetTitle( "Amplitude (ADC)" );

  for(size_t i=0;i<m_evres[AMPL].size();i++)
    RootUtils::FillConstantBinWidth( *m_histo1, m_evres[AMPL][i] );
  
  if( cnvs )
    {
      cnvs->cd();
      m_histo1->Draw();
      cnvs->Modified();
      cnvs->Update();
    }

  if( dirout )
    {
      dirout->cd();
      m_histo1->Write();
    }
}
    
