//////////////////////////////////////////////////////////////////////////////
//
//  utility to handle a spare matrix as list of lists
// 
//
//
//////////////////////////////////////////////////////////////////////////////
#ifndef __LILUTIL_H__
#define __LILUTIL_H__

#include <vector>
#include <utility>

//namespace MathUtils
//{
class LILUtil
{
 public:
  LILUtil();
  ~LILUtil();
  
  static bool CompFunc(const std::pair<int, double> &lhs, 
		       const std::pair<int, double> &rhs )
  {
    return (lhs.second < rhs.second);
  }
  

  void Init( double x0, double xstep, 
	     int xidx_start, int xidx_end );
  
  // we assume the data is sorted in increasing x 
  // and for each x in increasing y
  int Build( std::vector<double> xdata, std::vector<double> ydata );

  int GetN( double x );
  int GetN( int xbin );
  
  int Find( double x, double y, 
	    double ytol, std::vector<int> &res );

  int Find( int xbin, double y, double ytol,
	    std::vector<int> &res );
  int FindClosest( int xbin, double y, double ytol );

  void Print();

 private:

  int FindFromBin( int bin, double y, double ytol,
		   std::vector<int> &res );
  
  int FindBinNum( double x );

  std::vector< std::vector< std::pair<int, double> > > fLIL;
  bool fLocked;
  double fXFirst;
  double fXLast;
  double fXPitch;
  int    fXBinFirst;
  int    fXBinLast;
  int    fNbins;
}; 


//};


#endif
