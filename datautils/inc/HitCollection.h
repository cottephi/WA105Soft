/////////////////////////////////////////////////////////////////////////////////
//
//  Hit collection objects
//
//  Author: VG
//  Created: Wed Mar  2 20:58:52 CET 2016
//
/////////////////////////////////////////////////////////////////////////////////

#ifndef __HITCOLLECTION_H__
#define __HITCOLLECTION_H__

#include <string>
#include <vector>
#include <deque>
#include <utility>
#include <map>

#include "QHit.h"
#include "Track2d.h"

#include <TPad.h>
#include <TGraph.h>
#include <TH2F.h>

//////////////////////////////////////////////////////////////////////
//
// A collection of hits on a given channel
// It is ordered in time: from early to late time tick
//
//
// TODO: replace const LArReco::QHit* with a smart ptr
//
class ChHitCollection
{
 public:

  ChHitCollection();
  ~ChHitCollection();
  
  // comparison
  static bool ChCompPredicate(const LArReco::QHit* lhs, const LArReco::QHit* rhs)
  {
    return (lhs->GetTbStart() < rhs->GetTbStart());
  }

  void Reset();  
  bool AddHit(const LArReco::QHit* hit);
  void AddHitVector(const std::vector<const LArReco::QHit*> &hits);

  size_t GetNhits() const { return fHits.size(); }
  double GetQtot() const { return fQtot; }
  double GetQtotMC() const { return fQtotMC; }
  
  
  int GetCRM() const { return fCrmId; }
  int GetView() const { return fView; }
  int GetChId() const { return fChId; }
  
  // return physical position of the hit ch in cm
  double GetChPos() const
  {
    if(fHits.empty()) return -999.0;
    double pos = fHits[0]->GetX();
    return pos;
  }

  int GetFirstTdc() const 
  { 
    if( fHits.empty() ) return -1;
    return (int)fHits[0]->GetTbEnd(); 
  }

  int GetLastTdc() const 
  { 
    if( fHits.empty() ) return -1;
    return (int)fHits.back()->GetTbEnd(); 
  }
  

  void Print();

  // Get hit list
  const std::vector<const LArReco::QHit*>& GetHits() const
  { return fHits; }

  // Get closest hit based on time tick within a tolerance
  const LArReco::QHit* GetClosestHit(double ctick, double &dtick);
  const LArReco::QHit* GetHit(int tbin);

  void GetChTbin(std::vector< std::pair<int, int> >&coord, bool mknew = false);
  void GetPosTime(std::vector< std::pair<double, double> >&coord, bool mknew = false);
  void GetTMaxWindow(double &tlow, double &thigh, bool tus = false);
  void GetTWindow(double &tlow, double &thigh, bool tus = false);
  int  GetMaxTdcDiff();
  bool IsInCollection(const LArReco::QHit* hit);
  
 private:
  
  //
  bool ChMatch(const LArReco::QHit *hit)
  {
    if(fCrmId != hit->GetCRM()) return false;
    if(fView != hit->GetView()) return false;
    if(fChId != hit->GetCh()) return false;
    
    return true;
  }

  int fCrmId;    //CRM id
  int fView;     //view id
  int fChId;     //channel id
  
  double fQtot;
  double fQtotMC;

  // the ch hit list is maintained in the increasing time order
  std::vector<const LArReco::QHit*> fHits;

  std::map<int, const LArReco::QHit*> fHitsMap;
  std::map<int, const LArReco::QHit*>::iterator fHitsMapIt;
};


//////////////////////////////////////////////////////////////////////
//
// Hit collection for a continuous block of channels
// Note: could have several view / CRMs in the same 
// collection, provided that ch number is unique
// The list is maintained in the increased ch order
//
/*
struct BasicHitInfo
{
  int Ch;
  int Tbin;
  int HitIdx;
};
*/
class HitCollection
{
 public:

  HitCollection();
  ~HitCollection();

  void Reset();  
  void AddHit(const LArReco::QHit* hit);
  void SetHitVector(std::vector<const LArReco::QHit*> &hits);
  void Print();
  
  void GetTMaxWindow(double &tlow, double &thigh, bool tus = false);

  const std::deque<ChHitCollection*>& GetHits() const
  { return fHits; }

  void GetHitList(std::vector<const LArReco::QHit*>& hitlist, int fromch = -1) const;
  std::vector< std::pair<int, int> >& GetHitChCoord() { return fHitChCoord; };
  

  //
  const ChHitCollection* GetChHits(int ch) const;
  const ChHitCollection* GetChHitsFromChCoord(int idx) const;

  int GetNch() const { return fNch; }
  int GetChStart() const { return fChStart; }
  int GetChEnd() const { return fChEnd; }
  int GetNhits() const { return fNhits; }

  double GetChPosStart() const
  {
    if (fHits.empty()) return -999.0;
    return fHits[0]->GetChPos();
  }

  double GetChPosEnd() const
  {
    if (fHits.empty()) return -999.0;
    return fHits.back()->GetChPos();
  }

 private:
  
  //
  ChHitCollection* GetChHitCollection(int ch);
  
  std::vector< std::pair<int, int> > fHitChCoord;

  // collection hit channels using double-ended queue
  std::deque<ChHitCollection*> fHits;
  
  int fNch;         //total channels
  int fChStart;     //first channels
  int fChEnd;       //last channel
  int fNhits;       //total number of hits in this collection
};


//
//
// A class to draw a given hit collection
// 
//
class GraphHits
{
 public:
  GraphHits();
  ~GraphHits();
  
  void Clear();
  void SetRange(float xmin, float xmax, 
		float ymin, float ymax);
  void Draw(HitCollection &hit, TPad *pad, int colour = 1, 
	    std::string opt= "");

  void BuildBoxPrimitive(const LArReco::QHit* ahit, TGraph* &hitgraph);
  
 private:
  std::vector<TGraph*> fGraphs; //
  TH2F* fHistAxis;
};

#endif
