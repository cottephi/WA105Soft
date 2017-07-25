#include "WaveformGenerator.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
//
// WaveformGenerator class
//
WaveformGenerator::WaveformGenerator(BasicDigitizer *elecMod)
{
  ElecModule = elecMod;

  hPulse = NULL;
  hQvals = NULL;
  
  UInt_t seed = time(NULL);
  cout << "Seed for random number is "<<seed<<endl;
  m_Rand = new TRandom3(seed);
}

// dtor
WaveformGenerator::~WaveformGenerator()
{
  if(hPulse)
    hPulse->Delete();
  if(hQvals)
    hQvals->Delete();

  delete m_Rand;
}

// get waveforms for a given charge deposition historgram
TH1F* WaveformGenerator::MakeResponseHisto(TH1F *hQ, double ped, double noisepost) 
{
  if(hPulse)
    hPulse->Delete();
  
  hPulse = (TH1F*)hQ->Clone(Form("rpulse_%s", ElecModule->GetName().c_str()));
  hPulse->SetDirectory(0);
  hPulse->GetXaxis()->SetTitle("us");
  hPulse->GetYaxis()->SetTitle("ADC");
  hPulse->Reset();
  
  //double par[3];
  std::vector<float> respvec;
  ElecModule->ComputeRespVector();
  ElecModule->GetRespVector(respvec);

  int N = hQ->GetNbinsX();
  for(int i=1;i<N;i++)
    {
      double q  = hQ->GetBinContent(i);  //in fC 

      // skip q = 0 contributions
      if(q == 0.0) continue; 
      for(int j=i;j<=N;j++)
	{
	  int indx = j-i;
	  if(indx >= (int)respvec.size())
	    break;
	  double fval = q * respvec[indx]; 
	  double bval = hPulse->GetBinContent(j);
	  hPulse->SetBinContent(j, bval + fval);
	}
      
    }
  
  // add pedestal + noise term
  //double tmax = hPulse->GetBinCenter(hPulse->GetNbinsX());
  for(int i=1;i<=hPulse->GetNbinsX();i++)
    {
      double pval = ped;
      if(noisepost>0) 
	{
	  pval += m_Rand->Gaus(0, noisepost);
	  pval = floor(pval + 0.5);
	}
      //double tval = hPulse->GetBinCenter(i);
      //if((tmax - tval) < ElecModule->GetPulseWidth()) 
      //break;
      double val = hPulse->GetBinContent(i);
      val = ElecModule->QuantizeValue(val) + pval;
      hPulse->SetBinContent(i, val);
    }

  return hPulse;
}

//
TH1F* WaveformGenerator::MakeWaveform( double tmin, double tmax, 
				       double pedestal, 
				       double noisepre, double noisepost,
				       std::vector<double> &tpulse, 
				       std::vector<double> &qpulse )
{
  // Arguments:
  //   tmin      -- start time in us
  //   tmax      -- finish time in us
  //   pedestal  -- pedestal in ADC
  //   noisepre  -- noise rms in fC (before pre-amp)
  //   noisepost -- noise rms in ADC (after pre-amp)
  //   tpulse    -- vector with timings for q pulses
  //   qpulse    -- vector with charge for q pulses

  if(hQvals)
    hQvals->Delete();
  
  double binw = 1.0/ElecModule->GetSampleFreq();
  int nbins = (int)((tmax - tmin) / binw);
  const char *hname = Form("qpulse_%s", ElecModule->GetName().c_str());
  hQvals = new TH1F(hname, hname, nbins, tmin, tmax);
  hQvals->SetDirectory(0);
  hQvals->GetXaxis()->SetTitle("us");
  hQvals->GetYaxis()->SetTitle("fC");
  hQvals->Reset();

  // generate noise (in fC)
  if(noisepre > 0)
    {
      for(int i=1;i<=hQvals->GetNbinsX();i++)
	{
	  hQvals->SetBinContent(i, m_Rand->Gaus(0, noisepre));
	  //double tval = hQvals->GetBinCenter(i);
	  //Set to 0 the last pulse width usec (needed for FFT)
	  //if((tmax - tval) < ElecModule->GetPulseWidth()) 
	  //hQvals->SetBinContent(i, 0);
	}
      
    }

  // add charge depositions
  for(size_t i=0;i<tpulse.size();i++)
    {
      double tval = tpulse[i];
      double qval = qpulse[i];
      int binn = hQvals->FindBin(tval);
      hQvals->SetBinContent(binn, qval + hQvals->GetBinContent(binn));
      //pad with zeros at the for the pulse width length
      //if((tmax - tval) < ElecModule->GetPulseWidth())
      //hQvals->SetBinContent(binn, 0);
    }

  // generate the response histogram
  return MakeResponseHisto(hQvals, pedestal, noisepost);
}

