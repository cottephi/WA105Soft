#include "ElecSim.h"  

using namespace std;

// ctor for basic class to define pre-amp and digitizer
BasicDigitizer::BasicDigitizer()
{
  m_name = "BasicDigitzier";
 
  // our basic digitizer does not do anything
  m_Calib   = 0.0;
  m_mV2ADC  = 0.0;
  m_freq    = 0.0;
  m_pwidth  = 0.0;
  m_ped     = 0.0;
  m_pedrms  = 0.0;
  m_bitsadc = 0;
}

// dtor for basic digitizer
BasicDigitizer::~BasicDigitizer()
{;}

// compute pre-amp response vector
void BasicDigitizer::ComputeRespVector()
{
  m_respvec.clear();
  if(m_freq == 0) return;

  double dt = 1.0 / m_freq;

  double par[2];
  par[0] = 1.0; // 
  par[1] = 0.0; // 

  double tval = par[1] + 0.5*dt;
  while(tval < m_pwidth)
    {
      double fval = Response(&tval, par);
      //cout<<tval<<" "<<fval<<endl;
      m_respvec.push_back(fval);
      tval += dt;

    }
}


// Quantize
float BasicDigitizer::QuantizeValue(float val)
{
  float fval = val;
  if(m_mV2ADC > 0) fval /= m_mV2ADC;
  fval = floor(fval + 0.5);
  
  return fval;
}

//
void BasicDigitizer::Quantize(std::vector<float> &data)
{
  for(size_t i=0;i<data.size();i++)
    {
      data[i] = QuantizeValue(data[i]);
    }
}

// ETHZ pre-amplifier & CAEN digitizer
DigitizerETHZ::DigitizerETHZ()
{
  // calibration constant 
  // calibration constant adjusted for 1mip = 100 ADC
  //m_Calib   = 100./370.* 11.8*4.44;  // fC -- > mV x us

  //
  m_Calib   = 11.8*4.44;  // fC -- > mV x us
  m_mV2ADC  = 0.805664;   // mV --> ADC 
  
/*  // basic parameters
  m_freq    = 2.;  //MHz
  m_pwidth  = 50.0; //in us
  m_bitsadc = 16;   // number of adc bits
  m_ped     = 50.0; // some pedestal value    */
  
 // basic parameters
  m_freq    = 2.5;  //MHz
  m_pwidth  = 50.0; //in us
  m_bitsadc = 12;   // number of adc bits
  m_ped     = 5.0; // some pedestal value    

  m_dT = 3.5;       //
  m_T1 = 2.83;      //in us
  m_T2 = 0.47;      //in us

  m_name = "ethz_caen";
}

DigitizerETHZ::~DigitizerETHZ()
{;}


// responseFunction from Qscan
double DigitizerETHZ::responseFunction(double x, double *par)
{
  double value = 0;
  double q     = par[0]; //
  double t0    = par[1]; //t0 
  double dt    = m_dT;   //p[3]; 
  double T1    = m_T1;   //p[4];
  double T2    = m_T2;   //p[5];
  
  q *= m_Calib; //fC --> mV

  if (x-t0>0)
    value+=q/(dt*(T1-T2)*(T1-T2)) *
      (exp(-(x-t0)/T2)*( (x-t0)*(T1-T2)+(2*T1-T2)*T2-
			 ((x-t0-dt>=0)?1.:0.) * exp(dt/T2)*((x-t0)*(T1-T2)+2*T1*T2-T2*T2+dt*(T2-T1)) )+
       exp(-(x-t0)/T1)*( ((dt-x+t0> 0)?1.:0.) * exp((x-t0)/T1)*(T1-T2)*(T1-T2)+
			 T1*T1*(exp(dt/T1) * ((x-t0-dt>=0)?1:0) -1)));

  return value;
}

// implementation of the virutal method from BasicDigitizer
double DigitizerETHZ::Response(double *t, double *par)
{ return responseFunction(t[0], par); }




