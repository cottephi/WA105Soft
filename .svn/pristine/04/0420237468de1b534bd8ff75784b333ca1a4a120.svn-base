#include <iostream>
#include <iomanip>

#include "LogMsg.h"
#include "RootUtils.h"

#include "CaliAnaPed.h"

using namespace std;

//
// ctor
//
caliana::CaliAnaPed::CaliAnaPed(size_t chid) : caliana::CaliAnaDaqCh( chid )
{
  m_nres = 2;
  m_keys.resize( m_nres );
  m_evres.resize( m_nres );

  m_keys[PED]    = "pedval";
  m_keys[PEDRMS] = "pedrms";

  Reset();

  // chi2 cut to remove outliers
  m_chi2cut = 9.0;  

  //
  m_histo = NULL;
}


//
// dtor
//
caliana::CaliAnaPed::~CaliAnaPed()
{
  if(m_histo) m_histo->Delete();
}


//
//
//
void caliana::CaliAnaPed::ShowData(TCanvas *cnvs, TDirectory *dirout)
{
  if(m_data.empty()) return;
  
  if(m_histo) m_histo->Delete();
  
  stringstream ss;
  ss<<"chdata"<<m_chid<<"_evid"<<m_evid;
  m_histo = new TH1F(ss.str().c_str(), ss.str().c_str(), m_data.size(), 0, m_data.size());
  m_histo->SetDirectory(0);
  m_histo->GetXaxis()->SetTitle("tdc");
  m_histo->GetYaxis()->SetTitle("adc");
  
  for(size_t i=0;i<m_data.size();i++)
    {
      m_histo->SetBinContent(i+1, m_data[i]);
    }
  
  if( cnvs )
    {
      cnvs->cd();
      m_histo->Draw();
      cnvs->Modified();
      cnvs->Update();
    }

  if(dirout)
    {
      dirout->cd();
      m_histo->Write();
    }
}


//
//
//
void caliana::CaliAnaPed::ShowPedDist(TCanvas *cnvs, TDirectory *dirout)
{
  if(m_histo)  m_histo->Delete();
  
  stringstream ss;
  ss<<"chdata"<<m_chid<<"ped";
  m_histo = new TH1F(ss.str().c_str(), ss.str().c_str(), 100, 0, 200);
  m_histo->SetDirectory(0);
  m_histo->GetXaxis()->SetTitle( "Ped (ADC)" );

  for(size_t i=0;i<m_evres[PED].size();i++)
    {
      //cout<<m_evres[PED][i] <<endl;
      RootUtils::FillConstantBinWidth( *m_histo, m_evres[PED][i] );
    }
  //float mean, rms;
  //FindMeanAndRms( m_evres[PED], mean, rms );
  //cout<<mean<<" "<<rms<<endl;

  if(cnvs)
    {
      cnvs->cd();
      m_histo->Draw();
      cnvs->Modified();
      cnvs->Update();
    }

  if( dirout )
    {
      dirout->cd();
      m_histo->Write();
    }
}



//
//
//
void caliana::CaliAnaPed::ShowPedRmsDist(TCanvas *cnvs, TDirectory *dirout)
{
  if(m_histo)  m_histo->Delete();
  
  stringstream ss;
  ss<<"chdata"<<m_chid<<"pedrms";
  m_histo = new TH1F(ss.str().c_str(), ss.str().c_str(), 100, 0, 200);
  m_histo->SetDirectory(0);
  m_histo->GetXaxis()->SetTitle( "Ped RMS (ADC)" );
  for(size_t i=0;i<m_evres[PEDRMS].size();i++)
    RootUtils::FillConstantBinWidth( *m_histo, m_evres[PEDRMS][i] );
  
  if( cnvs )
    {
      cnvs->cd();
      m_histo->Draw();
      cnvs->Modified();
      cnvs->Update();
    }

  if( dirout )
    {
      dirout->cd();
      m_histo->Write();
    }
}


//
//
//
void caliana::CaliAnaPed::Fill( dlardaq::adc16_t* adc, size_t nsz)
{
  m_data.clear();
  for(size_t i=0;i<nsz;i++)
    {
      // skip first samples which are 0 in 3x1x1
      if( i<3 ) continue; 
      float val = (float)(adc[i]);
      m_data.push_back(val);
    }
  
  float ped, pedrms;
  FindMeanAndRms( m_data, ped, pedrms );
  
  // skip 0 (most likely no data)
  if( ped == 0 && pedrms == 0 )
    {
      return;
    }

  // save computed pedestal
  m_evres[PED].push_back( ped );
  m_evres[PEDRMS].push_back( pedrms );
}

//
//
//
void caliana::CaliAnaPed::GetResults( std::map< std::string, std::pair< double, double > > &res )
{
  res.clear();
  
  for(size_t i=0;i<m_nres;i++)
    {
      float mean, rms;
      FindMeanAndRms( m_evres[i], mean, rms, m_chi2cut );
      res[ m_keys[i] ] = std::make_pair( mean, rms );
    }
}


//
//
//
void caliana::CaliAnaPed::Show( TCanvas *cnvs, TDirectory *dirout, std::string opt )
{
  if( opt.compare("pedval") == 0 )
    ShowPedDist( cnvs, dirout );
  else if( opt.compare("pedrms") == 0 )
    ShowPedRmsDist( cnvs, dirout );
  else
    ShowData( cnvs, dirout );
}
