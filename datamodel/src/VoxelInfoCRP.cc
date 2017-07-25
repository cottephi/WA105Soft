#include "VoxelInfoCRP.h"

ClassImp(LArVoxel::VoxelInfoCRP)

// ctor
LArVoxel::VoxelInfoCRP::VoxelInfoCRP()
{
  NumCRM = 0;
  vCRM   = new TObjArray();
  vCRM->SetOwner(true);
}

// dtor
LArVoxel::VoxelInfoCRP::~VoxelInfoCRP()
{
  Reset();
}

// Clear
void LArVoxel::VoxelInfoCRP::Clear(Option_t *option)
{
  vCRM ->Clear(option);
  NumCRM = 0;
}

// Reset
void LArVoxel::VoxelInfoCRP::Reset(Option_t *option)
{
  Clear(option);
  delete vCRM;
  vCRM = 0;
}

// AddCRM
LArVoxel::VoxelInfoCRM* LArVoxel::VoxelInfoCRP::AddCRM()
{
  if(!vCRM) 
    {
      vCRM   = new TObjArray();
      vCRM->SetOwner(true);
    }

  //TClonesArray &vcrms = *vCRM;
  //LArVoxel::VoxelInfoCRM *crm = new(vcrms[NumCRM++]) LArVoxel::VoxelInfoCRM();
  LArVoxel::VoxelInfoCRM *crm = new LArVoxel::VoxelInfoCRM();
  vCRM->Add(crm);
  NumCRM++;

  return crm;
}

// AddCRM
LArVoxel::VoxelInfoCRM* LArVoxel::VoxelInfoCRP::AddCRM(Int_t crm_id)
{
  //TClonesArray &vcrms = *vCRM;
  //LArVoxel::VoxelInfoCRM *crm = new(vcrms[NumCRM++]) LArVoxel::VoxelInfoCRM(crm_id);

  if(!vCRM) 
    {
      vCRM   = new TObjArray();
      vCRM->SetOwner(true);
    }
  
  LArVoxel::VoxelInfoCRM *crm = new LArVoxel::VoxelInfoCRM(crm_id);
  vCRM->Add(crm);
  NumCRM++;

  return crm;
}

// Print
void LArVoxel::VoxelInfoCRP::Print()
{
  if(!vCRM || NumCRM == 0) return;

  std::cout<<"Printing voxels in CRP"<<std::endl;
  LArVoxel::VoxelInfoCRM* vxlcrm;
  TIter next(vCRM);
  while( (vxlcrm = (LArVoxel::VoxelInfoCRM*)next()) )
    vxlcrm->Print();
}
