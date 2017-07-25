#ifndef __RECOUTILS_H__
#define __RECOUTILS_H__

#include <utility>
#include <vector>

#include <TSpline.h>

#include <TObjArray.h>

#include <TVector2.h>
#include <TVector3.h>

namespace RecoUtils
{
  // truncated mean 
  //   rmlow  - fraction of events to remove at the low part of the distribution
  //   rmhigh - fraction of events to remove at the high part of the distribution 
  //
  double TruncatedMean( const std::vector<double> &vec, double rmlow, double rmhigh );
    


  // fit line LSQ 
  // returns value of correlation coefficient: the closer to 1 the better
  // params[0] +/- params_err[0] - constant 
  // params[1] +/- params_err[1] - tangent
  // serr - estimator for variance
  double FitLineLSQ(double *xdata, double *ydata, int np, 
		    std::vector<double> &params, 
		    std::vector<double> &params_err,
		    double &serr);
  
  

  // calculate tangent to line using defined number of points
  double LineTangent( const std::vector<TVector2> &vec, size_t nstart, size_t np, 
		      double &tanth, double &tanth_err);
  
  // basic function to match reonstructed tracks between views
  size_t ViewTrackMatch( const TObjArray *trks0, const TObjArray *trks1,
			 std::vector< std::pair<int, int> > &mpairs, 
			 double maxdist, double maxdelq = 0.1 );

  // find view and polar angles from track slopes
  void FindThetaAndPhi( const double slp_view0, const double slp_view1, 
			double &theta, double &phi );
  
  // find mean and rms of the values in the vector
  // using Welford's method to ensure numerical stability
  template< class T >
    void FindMeanAndRMS( std::vector<T> &data, double &mean, double &rms )
    {
      
      mean = 0;
      rms  = 0;
      
      if(data.size()<=1)
	{
	  mean = data[0];
	  rms  = 0;
	  return;
	}
      
      double A = 0;
      double Q = 0;
      for(size_t i=0;i<data.size();i++)
	{
	  //cout<<data[i]<<endl;
	  double d  = (double)data[i];
	  double Ak = A + (d - A)/(i+1);
	  double Qk = Q + (d - Ak)*(d-A);
	  A = Ak;
	  Q = Qk;
	}
      mean = A;
      rms  = sqrt( Q/(data.size()-1) );
    }

  // 
  // 2D line segment
  //
  class LineSegment
  {
  public:
    LineSegment();
    ~LineSegment();
    
    // assiming sorted array
    void SetPoints( std::vector<TVector2> &vec );
    void SetPoints( std::vector<double> &x, std::vector<double> &y );
    int GetXandTan(double y, int np, int &startpnt, double &x, double &slope);
    double GetTangent(size_t idx, int np);
    
    // linear fit mx + k
    double LinearFit(size_t ifirst, size_t ilast, double &a, double &b,
		     double &a_err, double &b_err);
    //void GetResiduals(double m, double k, std::vector<double> &res);
    
    size_t GetN(){ return fNp; }
    
  protected:
    double Interpolate( double y, int &startpnt );

    double fMinX, fMaxX;
    double fMinY, fMaxY;
      
    int fSlope;
    size_t fNp;
    std::vector<double> fAbcissa;
    std::vector<double> fOrdinata;
    std::vector<double> fTangent;
    //TSpline3 *fSpline;
  };



}
#endif
