////////////////////////////////////////////////////////////////////////////////
//
//  Template class definition for a data fragment
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __DATAFRAGMENT_H__
#define __DATAFRAGMENT_H__

#include <vector>
#include <TROOT.h>
#include <TObject.h>

namespace LArRawData
{
  template<typename T>
  class DataFragment : public TObject
  {
  public:
    DataFragment(); 
    ~DataFragment();
    
    // Fill gradually
    void  InitFragment(UInt_t fstart, UInt_t fsize);
    void  AddDatum(UInt_t at, T val, Bool_t overwrite);
    void  AddData(UInt_t at, std::vector<T> data, Bool_t overwrite);
    
    T GetDatum(UInt_t at);
    UInt_t  GetData(std::vector<T> &data);
    UInt_t  GetDataFloat(std::vector<float> &data);

    // Fill fragment with a vector in one go
    void   SetData(UInt_t fstart, std::vector<T> &data);
    void   Clear(Option_t *option = "")
    { 
      fStart = fEnd = 0;
      fData.clear();
    }
    //
    UInt_t GetFragStart(){ return fStart; }
    UInt_t GetFragEnd(){ return fEnd; }
    UInt_t GetFragSize(){ return fData.size(); }
    
  private:
    UInt_t fStart;               //start of the data fragment sequence
    UInt_t fEnd;                 //end of the data fragment sequence
    std::vector<T> fData;        //fragment data
    
  public:
    ClassDef(DataFragment,1);
  };
}

#endif
