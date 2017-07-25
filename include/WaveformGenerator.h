////////////////////////////////////////////////////////////////////////
// useful class to test response of given electronics modules

#ifndef __WAVEFORMGEN_HH__
#define __WAVEFORMGEN_HH__

#include <vector>
#include <string>
#include <ctime>
#include <cmath>

#include <TH1F.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TRandom3.h>

#include "ElecSim.h"

class WaveformGenerator
{
public:
  WaveformGenerator(BasicDigitizer *elecMod);
  ~WaveformGenerator();
  
  // digitized charge deposition (q in fC)
  TH1F* MakeResponseHisto(TH1F *hQ, double ped, double noisepost);
  TH1F* MakeWaveform( double tmin, double tmax, 
		      double pedestal, double noisepre, double noisepost, 
		      std::vector<double> &tpulse, 
		      std::vector<double> &qpulse );
  TH1F* GetChargeHisto(){ return hQvals; }
  TH1F* GetResponseHisto(){ return hPulse; }

  BasicDigitizer* ElecModule;
  
private:
  TH1F     *hPulse;
  TH1F     *hQvals;
  TRandom3 *m_Rand;
};
#endif
