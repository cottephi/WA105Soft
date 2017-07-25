////////////////////////////////////////////////////////////////////////////////
//
// Different noise models
//
////////////////////////////////////////////////////////////////////////////////

#include "NoiseGen.h"

//
BasicNoise::BasicNoise()
{
  // default init of random number generator from time seed
  m_rand      = gRandom; ////new TRandom3(time(NULL));
  m_rms       = 0.0;
  m_freq      = 0.0;  //MHz
  m_coher     = false;
  m_newevent  = true;
  m_shaperesp = false;
  ElecModule  = NULL;

  m_name      = "BasicNoise";
}

BasicNoise::~BasicNoise()
{;}

void BasicNoise::AddNoise(std::vector<float> &data)
{
  //std::vector<float> noise(data.size(), 0);

  if(m_noise.empty())
    m_noise.resize(data.size(), 0);

  if(m_noise.size() != data.size())
    {
      std::cout<<"WARNING: in AddNoise the data size does not match the size of noise vector"<<std::endl;
      m_noise.resize(data.size(), 0);
    }
  // get noise (implemented in derived classes)
  // for coherent noise keep the old vector unless new event is true
  if( m_coher )
    {
      if( m_newevent )
	{
	  GetNoise( m_noise );
	  //std::cout << " gen noise 1 "<< std::endl;
	}
    }
  else
    {
      GetNoise( m_noise );
      //std::cout << " gen noise 2 "<< std::endl;
    }

  // add noise to data
  for(size_t i=0;i<m_noise.size();i++)
    data[i] += m_noise[i];
}

// white noise ADC
ADCWhiteNoise::ADCWhiteNoise()
{
  m_name = "ADC";
}

ADCWhiteNoise::~ADCWhiteNoise()
{;}

void ADCWhiteNoise::GetNoise(std::vector<float> &noise)
{
  if(m_rms == 0.0) return;
  
  for(size_t i=0;i<noise.size();i++) //add quantized noise
    {
      noise[i] = floor(m_rand->Gaus(0.0, m_rms) + 0.5);
    }
}

// cable capacitance noise 
CapaWhiteNoise::CapaWhiteNoise()
{
  m_name = "CAPA";
}

CapaWhiteNoise::~CapaWhiteNoise()
{;}

void CapaWhiteNoise::GetNoise(std::vector<float> &noise)
{
  // set to zero
  for(size_t i=0;i<noise.size();i++)
    noise[i] = 0.0;

  if(m_resp.empty())
    {
      if(!ElecModule)
	{
	  std::cerr<<"ERROR: CapaWhiteNoise electronics model is not defined"
		   <<std::endl;
	  return;
	}
      ElecModule->ComputeRespVector();
      ElecModule->GetRespVector(m_resp);
    }
  
  if(m_rms == 0.0) return;
  
  // generate noise & apply convolution
  for(size_t i=0;i<noise.size();i++)
    {
      float qval = m_rand->Gaus(0.0, m_rms);

      if(m_shaperesp) // shape with response function 
	{
	  for(size_t j=i;j<noise.size();j++)
	    {
	      size_t indx = j-i;
	      if(indx >= m_resp.size())
		break;
	      double fval = qval * m_resp[indx]; 
	      noise[j] += fval;
	    }
	}
      else
	noise[i] = qval;
    }
}
