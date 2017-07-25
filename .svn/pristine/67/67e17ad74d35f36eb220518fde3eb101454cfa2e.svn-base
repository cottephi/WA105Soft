#include "VoxelInfo.h"

ClassImp(LArVoxel::VoxelInfo)


// ctor
LArVoxel::VoxelInfo::VoxelInfo()
{
  
  Pid   = 0;
  Edep  = -999.0;
  EdepQ = -999.0;
  xVox  = yVox = zVox = 0;
  tVox  = tCrp = -999.0;
  xCrp  = yCrp = 0.0;
  qCrp  = 0.0;
  qSigmaL = 0;
  qSigmaT = 0;
}

LArVoxel::VoxelInfo::VoxelInfo(int pid, float edep, float edepq, 
			       float xvox, float yvox, float zvox, float tvox,
			       float qcrp, float xcrp, float ycrp, float tarr,
			       float sigmaL, float sigmaT)
{
  
  Pid     = pid;
  Edep    = edep;
  EdepQ   = edepq;
  xVox    = xvox;
  yVox    = yvox;
  zVox    = zvox;
  tVox    = tvox;
  
  qCrp    = qcrp;
  xCrp    = xcrp;
  yCrp    = ycrp;
  tCrp    = tarr;
  
  qSigmaL = sigmaL;
  qSigmaT = sigmaT;
}

void LArVoxel::VoxelInfo::Clear(Option_t*)
{
  
  Pid   = 0;
  Edep  = -999.0;
  EdepQ = -999.0;
  xVox  = yVox = zVox = 0;
  tVox  = tCrp = -999.0;
  xCrp  = yCrp = 0.0;
  qCrp  = 0.0;
  qSigmaL = qSigmaT = 0;
}

// dtor
LArVoxel::VoxelInfo::~VoxelInfo()
{
  Clear();
}

void LArVoxel::VoxelInfo::Print()
{
  
  std::cout<<"   Voxel information"<<std::endl;
  std::cout<<"     Particle      : "<<Pid<<std::endl;
  std::cout<<"     Edep (MeV)    : "<<Edep<<std::endl;
  std::cout<<"     EdepQ (MeV)   : "<<EdepQ<<std::endl;
  std::cout<<"     Position (cm) : ("<<xVox<<", "<<yVox<<", "<<zVox<<")"<<std::endl;
  std::cout<<"     Time (us)     : "<<tVox<<std::endl;
  
  std::cout<<"     Charge at CRP (fC)          : "<<qCrp<<std::endl;
  std::cout<<"     Time of arrival at CRP (us) : "<<tCrp<<std::endl;
  std::cout<<"     Position at CRP (cm)        : ("<<xCrp<<", "<<yCrp<<")"<<std::endl;

  std::cout<<"     Diffusion longtitudinal (us) : "<<qSigmaL<<std::endl;
  std::cout<<"     Diffusion transverse (cm)    : "<<qSigmaT<<std::endl;
}
