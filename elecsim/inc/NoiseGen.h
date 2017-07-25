////////////////////////////////////////////////////////////////////////////////
//
// Different noise models
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __NOISEGEN_H__
#define __NOISEGEN_H__

#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cmath>

#include <TH1F.h>
#include <TFile.h>
#include <TRandom3.h>

#include "ElecSim.h"

///////////////////////////////////////////////////////////////////////
//abstract class which does nothing
class BasicNoise 
{
 public:
  BasicNoise();
  virtual ~BasicNoise();
  
  //use this to add noise to the existing data
  virtual void AddNoise(std::vector<float> &data); 

  //use this to generate noise sample to be implemented in derived class
  virtual void GetNoise(std::vector<float> &noise) = 0;
  
  bool GetCoherent(){ return m_coher; }
  
  void SetSeed(UInt_t seed){ m_rand->SetSeed(seed); }
  void SetNoiseRMS(float val){ m_rms = fabs(val); }
  void SetNoiseFreq(float val){ m_freq = val; }
  void SetCoherent(bool val){ m_coher = val; }
  void SetRespShaping(bool val){ m_shaperesp = val; }
  void SetElecModule(BasicDigitizer *elec){ ElecModule = elec; }
  void SetNewEvent(bool val){ m_newevent = val; }
  void SetName(std::string val){ m_name = val; }
  std::string GetName(){ return m_name; }
  
 protected:
  TRandom        *m_rand;     // random number generator
  BasicDigitizer *ElecModule; // electronics module
  float           m_rms;      // noise RMS
  float           m_freq;     // noise freq
  float           m_coher;    // coherent noise
  bool            m_newevent; // new event (for coherent noise)
  std::vector<float> m_noise; // noise vector
  std::string     m_name;
  bool            m_shaperesp; //response shaping
};



///////////////////////////////////////////////////////////////////////
// simplest noise consisting of randomly fluctuating ADC values
class ADCWhiteNoise : public BasicNoise
{
 public:
  
  ADCWhiteNoise();
  ~ADCWhiteNoise();
  //void AddNoise(std::vector<float> data);
  void GetNoise(std::vector<float> &noise);
};

///////////////////////////////////////////////////////////////////////
// white noise due to channel capacitance before pre-amp
class CapaWhiteNoise : public BasicNoise
{
 public:
  
  CapaWhiteNoise();
  ~CapaWhiteNoise();
  //void AddNoise(std::vector<float> data);
  void GetNoise(std::vector<float> &noise);

 private:
  std::vector<float> m_resp;
};

///////////////////////////////////////////////////////////////////////
// coherent noise at a given frequency



#endif
