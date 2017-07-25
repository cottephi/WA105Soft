#ifndef __CALIANAPED_H__
#define __CALIANAPED_H__

#include <vector>

#include "CaliAnaDaqCh.h"

#include <TH1F.h>
#include <TCanvas.h>
#include <TDirectory.h>

namespace caliana
{
  class CaliAnaPed : public CaliAnaDaqCh
  {
  public: 
    CaliAnaPed( size_t ch_id );
    ~CaliAnaPed();
    
    // show what we got
    void Show( TCanvas *cnvs, TDirectory *dirout, std::string opt );

    // various function (for debugging mainly)
    void ShowData(TCanvas* cnvs, TDirectory *dirout = NULL);
    void ShowPedDist(TCanvas* cnvs, TDirectory *dirout = NULL);
    void ShowPedRmsDist(TCanvas *cnvs, TDirectory *dirout = NULL);
    
    void GetResults( std::map< std::string, std::pair< double, double > > &res );
    
  private:
    // fill with data
    void Fill(dlardaq::adc16_t* adc, size_t nsz); 
    
    float m_chi2cut;

    TH1F *m_histo;
  };
}

#endif
