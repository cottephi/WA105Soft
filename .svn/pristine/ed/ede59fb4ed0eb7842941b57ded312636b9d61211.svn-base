/**




**/



#ifndef __DATALRO_H__
#define __DATALRO_H__

#include <TROOT.h>
#include <TObject.h>
#include <TClonesArray.h>


#include <TH1F.h>
#include <TList.h>


namespace LArRawData
{
  class DataLRO : public TObject
  {
  public:
    DataLRO();
    ~DataLRO();
    
    void Clear(Option_t *option = "");
    void Reset(Option_t *option = "");

    TList hList;

  private:

    UInt_t NumPMTs;
    double sampleTime;

    TClonesArray *dLRO;
	
	
  public:
    ClassDef(DataLRO, 1);

  };

}

#endif





