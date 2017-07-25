#include "VoxelInfoCRM.h"

ClassImp(LArVoxel::VoxelInfoCRM)


// ctor
LArVoxel::VoxelInfoCRM::VoxelInfoCRM()
{
  CRMId   = 0;
  
  nVoxels = 0;
  Voxels  = new TObjArray();
  Voxels->SetOwner(true);
}

// ctor
LArVoxel::VoxelInfoCRM::VoxelInfoCRM(int id)
{
  CRMId   = id;
  
  nVoxels = 0;
  Voxels  = new TObjArray();
  Voxels->SetOwner(true);
}


// dtor
LArVoxel::VoxelInfoCRM::~VoxelInfoCRM()
{
  Reset();
}

// Clear
void LArVoxel::VoxelInfoCRM::Clear(Option_t *option)
{
  if(nVoxels)
    Voxels->Clear(option);
  nVoxels = 0;
}

// Reset
void LArVoxel::VoxelInfoCRM::Reset(Option_t *option)
{
  Clear(option);
  
  //if(Voxels)
  delete Voxels;
  
  Voxels = 0;
}


// add voxel
LArVoxel::VoxelInfo* LArVoxel::VoxelInfoCRM::AddVoxel( int pid, float edep, float edepq, 
						       float xvox, float yvox, float zvox, float tvox,
						       float qcrp, float xcrp, float ycrp, float tarr,
						       float sigmaL, float sigmaT )
{
  if(!Voxels)
    {
      Voxels  = new TObjArray();
      Voxels->SetOwner(true);
    }

  //TClonesArray &vxls = *Voxels;
  
  LArVoxel::VoxelInfo *vxl = NULL;
  
  vxl = new LArVoxel::VoxelInfo( pid, edep, edepq, xvox, yvox, zvox, tvox,
				 qcrp, xcrp, ycrp, tarr, sigmaL, sigmaT );
  Voxels->Add(vxl);
  nVoxels++;

  return vxl;
}


//
void LArVoxel::VoxelInfoCRM::Print()
{
  if(!Voxels || nVoxels == 0) return;
  
  std::cout<<std::endl<<" Begin voxel info in CRM "<<CRMId<<std::endl;
  LArVoxel::VoxelInfo* vxl;
  TIter next(Voxels);
  while( (vxl = (LArVoxel::VoxelInfo*)next()) )
    vxl->Print();
  std::cout<<" End voxel info in CRM "<<CRMId<<std::endl<<std::endl;
}
