////////////////////////////////////////////////////////////////////////////
//
//   Truth information of the charge mapped to the anode
//  
//
////////////////////////////////////////////////////////////////////////////

#include "ChanTrueQ.h"

#ifndef __ANODETRUEQ_H__
#define __ANODETRUEQ_H__

namespace LArMCTruth
{
  class AnodeTrueQ : public TObject
  {
  public:
    AnodeTrueQ();
    ~AnodeTrueQ();
    
    void Clear();
    void Print() const;
    
    AnodeTrueQ& operator=(const AnodeTrueQ& rhs);

    Float_t GetQView0() const { return fQView0; }
    Float_t GetQView1() const { return fQView1; }
    size_t GetNChView0() const { return fAnodeQView0.size(); }
    size_t GetNChView1() const { return fAnodeQView1.size(); }
    
    bool IsValid() const;

    void AddChanData(ChanTrueQ &ChQ, int iView);
    
    // return truth information for channels
    const std::vector<LArMCTruth::ChanTrueQ*>& View0() const { return fAnodeQView0; }
    const std::vector<LArMCTruth::ChanTrueQ*>& View1() const { return fAnodeQView1; }

  private:
    // channel assigend to anode
    std::vector<LArMCTruth::ChanTrueQ*> fAnodeQView0;
    std::vector<LArMCTruth::ChanTrueQ*> fAnodeQView1;
    
    // 
    Float_t fQView0;
    Float_t fQView1;

    ClassDef(AnodeTrueQ,1);
  };
}


#endif
