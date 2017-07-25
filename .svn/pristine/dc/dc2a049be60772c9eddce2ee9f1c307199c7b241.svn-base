////////////////////////////////////////////////////////////////////////////////////
//
//  Object to book quantities to analyze charge collection in a CRP
//  
//  
//
//
////////////////////////////////////////////////////////////////////////////////////

#ifndef __CRPQANASTORE_H__
#define __CRPQANASTORE_H__

#include <string>
#include <vector>

#include <TVector3.h>
#include <TDirectory.h>
#include <TH2F.h>

//
//
//
class CRPQAnaStore
{
 public:
  // in cm
  CRPQAnaStore(std::string name, 
	       double x0, double y0, 
	       double x1, double y1);
  
  ~CRPQAnaStore();

  void Fill( std::vector<double> &dqdx0, std::vector<TVector3> &path0,
	     std::vector<double> &dqdx1, std::vector<TVector3> &path1 );

  // compute the result after filling
  void ComputeAreaMap();
  size_t  GetNbins() const { return fAreaBins.size(); }
  ssize_t GetBinContent( size_t bin, int iview, 
			 double &v, double &ve ) const;

  // segment CRP (in cm)
  void BreakIntoSegments(double dx, double dy);
  
  //
  void SetTrMeanParameter(double flo, double fhi)
  { fRlo = flo; fRhi = fhi; }
  
  //
  void Write(TDirectory *dirout);
  
  // reset
  void Reset();

 protected:
  
  // find bin (use TH2 axis to do binary search)
  int FindBin(double x, double y, TH2F *histo)
  {
    int nx = histo->GetNbinsX();
    int bx = histo->GetXaxis()->FindBin(x)-1;
    int by = histo->GetYaxis()->FindBin(y)-1;
    return bx + nx*by;
  }

  void InitAreaSegs( std::vector<double> &xbins, 
		     std::vector<double> &ybins );
  void InitQHists();
  

  struct CRPArea_t
  {
    // coord of the corners
    double xc0, xc1;
    double yc0, yc1;
    // accumulated vector
    std::vector<double> dqdx0; // accumulator array
    std::vector<double> dqdx1; // accumulator array
    float dq0;       // dqds from view 0
    float dq0_err;   // dqds error from view 0
    float dq1;       // dqds from view 1
    float dq1_err;   // dqds error from view 1
    float dq_tot;    // dqds total
  };

  //
  double fX0, fX1;
  double fY0, fY1;
  std::string fName;
  std::vector< CRPArea_t > fAreaBins;
  
  //
  TH2F *fCRPArea;
  
  // for truncated mean
  double fRlo, fRhi;
};


#endif
