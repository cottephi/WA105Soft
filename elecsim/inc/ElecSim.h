////////////////////////////////////////////////////////////////////////////////
//
//   classes to define pre-amp response and backend digitizer
//
//   Created: Wed Apr  1 16:27:34 CEST 2015
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __ELECSIM_HH__
#define __ELECSIM_HH__

#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cmath>


// skeleton class to describe front-end pre-amp and back-end digitizer
class BasicDigitizer 
{
public:
  BasicDigitizer();
  virtual ~BasicDigitizer();

  virtual double Response(double *t, double *par) = 0;
    
  std::string GetName(){ return m_name; }
  double GetCalibConst(){ return m_Calib; }
  double GetSampleFreq(){ return m_freq; }
  double GetPulseWidth(){ return m_pwidth; }
  double GetPedestal(){ return m_ped; }
  double GetPedRMS(){ return m_pedrms; }
  double GetmVtoADC(){ return m_mV2ADC; }
  int    GetBitsADC(){ return m_bitsadc; }

  
  void SetBitsADC(int val){ m_bitsadc = val; }
  void SetPedestal(double val){ m_ped = val; }
  void SetPedRMS(double val){ m_pedrms = val; }
  void SetCalibConst(double val){ m_Calib = val; }
  void SetSampleFreq(double val){ m_freq  = val; }
  void SetPulseWidth(double val){ m_pwidth = val; }
  void SetmVtoADC(double val){ m_mV2ADC = val; }

  void GetRespVector(std::vector<float> &vec){ vec = m_respvec; }
  void ComputeRespVector();
  void Quantize(std::vector<float> &data);
  float QuantizeValue(float val);

protected:
  std::string m_name;
  double m_Calib;    // fC --> ADC
  double m_freq;     // in MHz
  double m_pwidth;   // pulse width in us
  double m_ped;
  double m_pedrms;   
  double m_mV2ADC;
  int    m_bitsadc;
  std::vector<float> m_respvec;
};

//
// class for ETHZ pre-amp and CAEN digitizer
class DigitizerETHZ : public BasicDigitizer
{
public:
  DigitizerETHZ();
  ~DigitizerETHZ();
  // elecronics reponse
  double Response(double *t, double *par);
  void   SetCutOff(double val);
private:
  double responseFunction(double x, double *par);

  double m_dT;
  double m_T1;
  double m_T2;
};

#endif
