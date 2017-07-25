#ifndef __VOXELINFO_CRM_H__
#define __VOXELINFO_CRM_H__

#include <TROOT.h>
#include <TObject.h>
#include <TObjArray.h>

#include "VoxelInfo.h"

namespace LArVoxel
{
  class VoxelInfoCRM : public TObject
  {
  public:
    VoxelInfoCRM();
    VoxelInfoCRM(int id);
    ~VoxelInfoCRM();
    
    VoxelInfo* AddVoxel( int pid, float edep, float edepq, 
			 float xvox, float yvox, float zvox, float tvox,
			 float qcrp, float xcrp, float ycrp, float tarr,
			 float sigmaL, float sigmaT );

    TObjArray *GetVoxels() const { return Voxels; }
    int    GetCRMId(){ return CRMId; }
    UInt_t GetNVoxels(){ return nVoxels; }

    void SetCRMId(int val){ CRMId = val; }
    
    //
    void Clear(Option_t *option = "");
    void Reset(Option_t *option = "");
    void Print();

  private:
    int CRMId;
    UInt_t nVoxels;
    TObjArray *Voxels;
    
  public:
    ClassDef(VoxelInfoCRM,1);
  };
}


#endif
