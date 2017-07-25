#ifndef __CRPVIEWPLOTTING_H__
#define __CRPVIEWPLOTTING_H__

#include <string>
#include <vector>

#include <TH1F.h>
#include <TH2F.h>
#include <TH1S.h>
#include <TH2S.h>
#include <TCanvas.h>

#include "DataFragment.h"

// index sorter 
template<typename T>
class index_sorter
{
public:
 index_sorter(const std::vector<T> &v) : v(v) {}
  bool operator()(const int &lhs, const int &rhs) const { return ( v[lhs] < v[rhs] ); }
 private:
  const std::vector<T> &v;
};

//
class CRPViewPlotting
{
 public:
  CRPViewPlotting( std::string viewname, int iview, 
		   int maxtdc, int chlow, int chhigh );
  ~CRPViewPlotting();
  
  // fill wf form 
  void Fill(int ch, LArRawData::DataFragment<UShort_t>* wf_adc, float ped = 0);
  
  // reset to call at the beginning of each event
  void Reset();

  // frees all memory
  void Clear();
  
  void SetMaxAdc(float val){ fMaxAdc = val; }
  
  // new frame dimensions
  void SetNewDataFrame( int maxtdc, int chlow, int chhigh );

  void SelectWFInADCRange(int nwf, int adcth);
  void SelectWFInChans(std::string &input);
  void DrawWF(TCanvas *cnvs, int tdcmin=-1, int tdcmax=-1);
  void Draw2D(TCanvas *cnvs, float scale2max = 1.0);
  
 protected:
  void GetWFInADCRange(std::vector<int> &wftodraw);
  
  void NewDataFrame();
  void SortMaxVals();

  // color wheel
  int GetLineColor(float f)
  {
    int icolor = (int)(floor(f * fNcols + 0.5));
    return fLineColors[icolor];
  }

  size_t fNcols;
  std::vector<int> fLineColors;
  
  int fView;
  std::string fName;
  std::string fTdcAxisName;
  std::string fChAxisName;
  std::string fAdcAxisName;

  // dimiensions of our frame
  int fNumCh;
  int fChLo;
  int fChHi;
  int fNumTdc;
  int fMinTdc;
  int fMaxTdc;

  //
  TH1F *fHAxis;
  std::vector<TH1F*> fChWfs;
  TH2F *fViewChTdc;

  //
  float fMaxAdc;     // e.g., 4095 for 12 bit  
  float fMaxChAdc;   // max value for the channel
  float fMaxScale; 
  
  // number of ADC to display above the threshold
  int fADCWfThresh;   // threshold for ADC WF to display
  int fMaxWfRange;    // max number of WF to display in a given range
  
  // max ADC values in each channel 
  std::vector<float> fChMaxVals;
  std::vector<float> fChMinVals;

  //
  std::vector<int> fWfSelect;
  
  std::vector<int> fIndxSorted;
  std::vector<float> fChMaxValsSorted;
};

#endif
