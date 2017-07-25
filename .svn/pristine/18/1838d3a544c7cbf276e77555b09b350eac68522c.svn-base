////////////////////////////////////////////////////////////////////////////////////
//
//
//   FFT utils: bascially a copy of the LArSoft LArFFT service
//
//
//
////////////////////////////////////////////////////////////////////////////////////
//#include <iostream>
#include <cmath>

#include "FFTUtils.h"


//
//
//
FFTUtils::FFTUtils(int dsize)
{
  fSize   = dsize; //size of data array
  fOption = "";

  InitializeFFT();
}


//
//
//
FFTUtils::~FFTUtils() 
{
  delete fFFT;
  delete fInverseFFT;
}

//
//
//
void FFTUtils::ReinitializeFFT(int size, std::string option)
{

  delete fFFT;
  delete fInverseFFT;

  //set members
  fSize   = size;
  fOption = option;

  //now initialize
  InitializeFFT();
}

//
//
//
void FFTUtils::InitializeFFT()
{

  // make it a pow of 2
  fSize = (int)( pow(2, ceil(log(fSize)/log(2))) );
  fFreqSize = fSize/2+1;  

  // transform objects
  fFFT        = new TFFTRealComplex(fSize, false);  
  fInverseFFT = new TFFTComplexReal(fSize, false);  

  int dummy[1] = {0}; 
  fFFT->Init(fOption.c_str(),-1,dummy);  
  fInverseFFT->Init(fOption.c_str(),1,dummy);  
  
  fCompTemp.resize(fFreqSize);  
  fKern.resize(fFreqSize);
}
