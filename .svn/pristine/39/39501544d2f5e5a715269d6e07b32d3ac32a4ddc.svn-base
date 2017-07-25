////////////////////////////////////////////////////////////////////////////////
//
//   Class to hold raw data from all CRMs 
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __VOXELINFO_CRP_H__
#define __VOXELINFO_CRP_H__

#include <TROOT.h>
#include <TObject.h>
#include <TObjArray.h>

#include "VoxelInfoCRM.h"

namespace LArVoxel
{
  class VoxelInfoCRP : public TObject
  {
  public:
    VoxelInfoCRP();
    ~VoxelInfoCRP();
    
    VoxelInfoCRM* AddCRM();
    VoxelInfoCRM* AddCRM(Int_t crm_id);
    TObjArray *GetVoxelCRMs() const { return vCRM; }
    
    UInt_t GetNumCRM() const { return NumCRM; } 
    
    //
    void Clear(Option_t *option = "");
    void Reset(Option_t *option = "");
    void Print();

  private:
    UInt_t NumCRM;
    TObjArray *vCRM;

  public:
    ClassDef(VoxelInfoCRP, 1);
  };
}

#endif

