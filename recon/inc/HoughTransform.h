/////////////////////////////////////////////////////////////////////////////////
//
//  Hough transform accumulator to search for track segments
// 
//  Created: Fri Aug 28 16:03:34 CEST 2015  
//
//  Performs a transform of two dimensional coordinates x, y into HT space 
//  of alpha, rho to find linear segments and associated delta ray clusters
//
//  Transform:
//      rho = x cos(alpha) + y sin(alpha)  
// 
//  The slope of the line and intercept are given by 
//        -cos(alpha) / sin(alpha) (slope)
//         rho / sin(alpha)        (intercept)      
//
//
/////////////////////////////////////////////////////////////////////////////////


#ifndef __HTACCUMULATOR_H__
#define __HTACCUMULATOR_H__

#include <utility>
#include <vector>
#include <deque>
//#include <map>

#include <TAxis.h>

#include "HitCollection.h"

// max number of HT bins
#define MAXHTACCUBINS 10000000

// max number of points for 2 byte index
// change if need more than 2 bytes
typedef unsigned short htindex;

//
typedef unsigned short ushort;


//
//
//
class HoughTransform
{
 public:
  
  HoughTransform( float xmax, float ymax, float xpitch, float ypitch, 
		  int sbinx = 1, int sbiny = 1 );
  ~HoughTransform();
  
  void BuildClusters(const HitCollection* hcoll, std::vector<HitCollection*> &hitclus);

  // 
  void SetAlphaBinWidth(float dalpha_rad);
  void SetRhoBinWidth(float drho_mm);

  void SetSumAlphaBins(int val){ fSumalpbin = (val>0)?val:0; }
  void SetSumRhoBins(int val){ fSumrhobin = (val>0)?val:0; }
  
  void SetMultiHitBlock(int val){ fMultiHitBlock = val; }
  void SetDRayMaxTDC(int val){ fDRayMaxTdcDist = val; }
  void SetUseHitPos(bool val){ fUseHitPos = val; }
  void SetHitOverlapRation(float val){ fHitOverlapRatio = val; }
  
  // min number of hits in track segment
  void SetMinTrackSegHits(int val){ fMinTrackSegHits = (val>0)?val:1; }
  
  // max ch gap in a track to stop searching
  void SetMaxChGap(int val){ fMaxChGap = (val>0)?val:0; }

  
 private:

  // get sqrt ( mean squared error )
  float GetLinFitMSE(float p0, float p1, std::deque<htindex> &dpnts);
  
  // calculate HT transform
  void Transform();

  // get global maximum from HT
  int  GetGlobalMax( float &alpha, float &rho, 
		     std::deque<htindex> &linepts );
  //
  int GetMaxBin(int &maxval);
  
  
  void GetAlphaRho(int bina, int binrho, float &alpha, float &rho);
  void GetAlphaRhoBins(int binn, int &bina, int &binrho);

  
  //
  void SubDataPts(std::deque<htindex> &datapts);

  //
  // convert HT to cartisian
  void ConvertToCartisian(float &alpha, float &rho)
  {
    float a = -cos(alpha)/sin(alpha);
    float b = rho / sin(alpha);
    alpha = a;
    rho   = b;
    return;
  }

  //
  // build HT bins
  void BuildAxes();
  
  // get slope from lsq fit (more presice than from HT parameters)
  // returns slope in tdc/ch if UsePosInfo is false
  // otherwise position in mm is used
  int   GetLinePoints(std::deque<htindex> &datapts, float &a, float &b, float &resval);
  float GetLinePara(std::deque<htindex> &datapts, float &a, float &b, int &maxseg, int itrack_cut = -1 );

  // find ch hits in a given range
  float  FindChHit(htindex startidx, htindex endidx, int ch, float tval, int &idx);
  
 
  //
  void AddBins( int binn, int halfnx, int halfny, float alpha, float rho, 
		int mincut, std::deque<htindex> &datapts, bool binmask );

  //
  void Reset();

  //
  void AddHTIndexPoint(std::deque<htindex>& datapts, htindex val);
  
  //
  //
  // variables
  float fXPitch, fYPitch; // pitch along x and y (i.e., time) axis in mm

  int   fAlphaNb;         // number of alpha bins
  float fAlphaMin;        // min alpha = -pi (default)
  float fAlphaMax;        // max alpha =  pi (default)

  int   fRhoNb;           // number of rho bins
  float fRhoMin;          // min rho = 0 (default)
  float fRhoMax;          // max rho
  
  int   fTotBins;         // total number of bins

  // TAxis for bin searching purposes
  TAxis *fAlphaxis;
  TAxis *fRhoaxis;

  // flag to enable using position information coming with each ht          
  bool  fUseHitPos;       
  
  // sum number of bins in max search
  int   fSumalpbin;
  int   fSumrhobin;
  
  // timing cuts
  int   fMultiHitBlock;   // ticks to merge multiple hits into a block 
  int   fDRayMaxTdcDist;  // max time bins for delta ray 
  
  // max gap between consecutive ch to detect track break
  int fMaxChGap;
  
  // min independent track segment
  int fMinTrackSegHits;
  float fHitOverlapRatio;

  float fTolNsgm;        // tolerance in sigmas
  
  struct HT_Accum
  {
    bool BinMask;
    htindex PointsUsed;
    std::vector<htindex> Points;
  };
  
  //
  //
  struct HT_HitPoint
  {
    int ch;        // hit channel
    int chhitidx;  // index of the hit in channel
    int starttdc;  // ADC time bin of the hit start
    int endtdc;    // ADC time bin of the hit end
    float x;       // in mm
    float y;       // in mm
    int itracks;   // number of associated tracks
  };

  size_t GetBinValue(HT_Accum &htbin);

  // hit collection
  //const HitCollection *fHColl; 

  // internal data vector
  std::vector<HT_HitPoint> fDataVec; 

  // HT bins
  //std::vector< std::vector<int> > fHTbins;
  //std::vector< HTAccumBin* > fHTbins;
  std::vector< HT_Accum > fHTbins;
  
  std::vector< std::vector<int> > fHTbinmap;
};

#endif
