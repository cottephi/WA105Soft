////////////////////////////////////////////////////////////////////////////////////
//
//
//   FFT utils: bascially a copy of the LArSoft LArFFT service
//
//
//
////////////////////////////////////////////////////////////////////////////////////

#ifndef __FFTUTILS_H__
#define __FFTUTILS_H__

#include <vector>
#include <string>

#include <TComplex.h>
#include <TFFTRealComplex.h>
#include <TFFTComplexReal.h>

class FFTUtils
{
 public:
  
  FFTUtils(int dsize);
  ~FFTUtils();
  
  // perform FFT
  template <class T> void DoFFT( std::vector<T> & input,
				     std::vector<TComplex> & output ); 
  
  // perofrm inverse FFT
  template <class T> void DoInvFFT( std::vector<TComplex> & input,
				    std::vector<T> & output );
  
  // perform convlolution with a given response function
  template <class T> void Convolute( std::vector<T> & input,
				     std::vector<T> & respFunc );

  // perform convolution with a given kernel
  template <class T> void Convolute( std::vector<T> & input,
				     std::vector<TComplex> & kern );
  
  // perform deconvolution with a given response function 
  template <class T> void Deconvolute( std::vector<T> & input,
				       std::vector<T> & respFunc );

  // perform deconvolution with a given kernel
  template <class T> void Deconvolute( std::vector<T> & input,
				       std::vector<TComplex> & kern );
  
  int  FFTSize()     const { return fSize; }
  int  FreqSize()    const { return fFreqSize; }
  std::string FFTOptions() const { return fOption; }
  void ReinitializeFFT(int, std::string);
  
 private:
  int                   fSize;
  int                   fFreqSize;
  std::string           fOption;

  // internal buffer to store input FFT
  std::vector<TComplex> fCompTemp; 

  // interlan buffer to store kernel FFT
  std::vector<TComplex> fKern;
  
  // FFT engines
  TFFTRealComplex       *fFFT;
  TFFTComplexReal       *fInverseFFT;

  void InitializeFFT();

};

//
// FFT
//
template <class T> inline void FFTUtils::DoFFT( std::vector<T> & input,		   
						std::vector<TComplex> & output )
{  
  double real      = 0.;  
  double imaginary = 0.;  

  // set the points
  size_t nsz = input.size();
  for(size_t p = 0; p < (size_t)fSize ; ++p)
    {
      if( p < nsz )
	fFFT->SetPoint(p, input[p]);
      else // zero pad
	fFFT->SetPoint(p, 0);
    }
  
  fFFT->Transform();    
  
  if( (int)output.size() < fFreqSize) output.resize( fFreqSize );

  for(int i = 0; i < fFreqSize; ++i){    
    fFFT->GetPointComplex(i, real, imaginary);    
    output[i] = TComplex(real, imaginary);  
  }  
  
  return;
}


//
// Inverse FFT
//
template <class T> inline void FFTUtils::DoInvFFT( std::vector<TComplex> & input,		      
						   std::vector<T> & output )
{  
  for(int i = 0; i < fFreqSize; ++i)    
    fInverseFFT->SetPointComplex(i, input[i]);   

  fInverseFFT->Transform();  
  double factor = 1.0/(double) fSize;  
  
  for(int i = 0; i < fSize; ++i)    
    output[i] = factor*fInverseFFT->GetPointReal(i,false);  

  return;
}


//
// deconvolution with response function
//
template <class T> inline void FFTUtils::Deconvolute(std::vector<T> & input,			 
						     std::vector<T> & respFunction)
{  
  DoFFT(respFunction, fKern);  
  DoFFT(input, fCompTemp);  

  for(int i = 0; i < fFreqSize; i++) 
    fCompTemp[i] /= fKern[i];  

  DoInvFFT(fCompTemp, input);  

  return;
}


//
// deconvolution with existing reponse kernal
//
template <class T> inline void FFTUtils::Deconvolute( std::vector<T> & input,			 
						      std::vector<TComplex> & kern )
{    
  DoFFT(input, fCompTemp);  
  
  for(int i = 0; i < fFreqSize; i++) 
    fCompTemp[i] *= kern[i];   // multiply by deconvolution kernel!!!

  DoInvFFT(fCompTemp, input);  
  
  return;
}


//
// convolution with the given shape
//
template <class T> inline void FFTUtils::Convolute( std::vector<T> & input,
						    std::vector<T> & respFunc )
{  
  DoFFT(input, fCompTemp);
  DoFFT(respFunc, fKern);  


  for(int i = 0; i < fFreqSize; i++) 
    fCompTemp[i] *= fKern[i];  

  DoInvFFT(fCompTemp, input);  

  return;
}


//
// convolution with a given kernel function
//
template <class T> inline void FFTUtils::Convolute( std::vector<T> & input,
						    std::vector<TComplex> & kern )
{  
  DoFFT(input, fCompTemp);  
  
  for(int i = 0; i < fFreqSize; i++) 
    fCompTemp[i] *= kern[i];  
  
  DoInvFFT(fCompTemp, input);    
  
  return;
}

#endif
