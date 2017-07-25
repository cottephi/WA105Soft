//////////////////////////////////////////////////////////////////////////////
// 
//  Analysis of the pulser calibration data to get gain calibration 
//  for each ch
//
//
//  Created: Fri Nov 18 16:15:55 CET 2016
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __CALIANAPULSER_H__
#define __CALIANAPULSER_H__

#include <vector>

#include "CaliAnaDaqCh.h"
#include "RawChDataProcess.h"

#include <TH1F.h>

//
namespace caliana
{
  class CaliAnaPulser : public CaliAnaDaqCh
  {
  public: 
    CaliAnaPulser( size_t ch_id, float norm );
    ~CaliAnaPulser();
    
    //
    float GetNorm() const { return m_norm; }

    // show what we got
    void Show( TCanvas *cnvs, TDirectory *dirout, std::string opt );
    
    // various function (for debugging mainly)
    void ShowData(TCanvas* cnvs, TDirectory *dirout = NULL);
    void ShowPedDist(TCanvas* cnvs, TDirectory *dirout = NULL);
    void ShowPedSamples(TCanvas* cnvs, TDirectory *dirout = NULL);
    void ShowPedRmsDist(TCanvas *cnvs, TDirectory *dirout = NULL);
    void ShowPulsePeaks(TCanvas *cnvs, TDirectory *dirout = NULL);
    
    // external ped values to use
    void SetPedestal( float ped, float pedrms )
    {
      m_ped    = ped;
      m_pedrms = pedrms;
      m_pedext = !((m_ped == 0) && (m_pedrms == 0)); 
    }

    //
    void SetSearchTimeWindow(float low, float high)
    {
      m_tdclo = low;
      m_tdchi = high;
      if( m_tdclo > m_tdchi ) 
	{
	  m_tdclo = high;
	  m_tdchi = low;
	}
    }
    //
    void SetSearchThresh( float val ){ m_thresh = val; }
    
  private:
    // fill with data
    void Fill(dlardaq::adc16_t* adc, size_t nsz); 
    
    bool PassCuts(float height, float tval);
    
    // search for region of interest
    RawChDataROI m_roifind;
    std::vector< std::pair<size_t, size_t> > m_roi;
    
    TH1F *m_histo1;
    TH1F *m_histo2;

    //
    float m_thresh;
    
    //
    float m_tdclo;
    float m_tdchi;

    //
    float m_norm;

    //
    bool  m_pedext;
    float m_ped;
    float m_pedrms;
  };
};



#endif
