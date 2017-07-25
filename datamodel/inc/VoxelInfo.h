#ifndef __VOXELINFO_H__
#define __VOXELINFO_H__

#include <iostream>
#include <algorithm>
#include <iterator>
#include <map>

#include <TROOT.h>
#include <TObject.h>

namespace LArVoxel
{
  class VoxelInfo : public TObject
  {
  public:
    VoxelInfo();
    VoxelInfo( int pid, float edep, float edepq, 
	       float xvox, float yvox, float zvox, float tvox,
	       float qcrp, float xcrp, float ycrp, float tarr,
	       float sigmaL, float sigmaT );
    ~VoxelInfo();

    void Clear(Option_t *option = "");
    
    
    void Print();
    
    int GetPid() const { return Pid; }
    float GetEdep() const { return Edep; }
    float GetEdepQ() const { return EdepQ; }
    
    float GetXVox() const { return xVox; }
    float GetYVox() const { return yVox; }
    float GetZVox() const { return zVox; }
    float GetTVox() const { return tVox; }

    float GetQCrp() const { return qCrp; }
    float GetXCrp() const { return xCrp; }
    float GetYCrp() const { return yCrp; }
    float GetTCrp() const { return tCrp; }

    float GetSigmaL() const { return qSigmaL; }
    float GetSigmaT() const { return qSigmaT; }
    
  protected:
    
    // truth in voxel
    int Pid;                  // particle id
    float Edep;               // energy deposited in voxel
    float EdepQ;              // quenched deposited energy
    float xVox, yVox, zVox;   // position of the voxel
    float tVox;               // time of the deposition

    // for CRP projection
    float qCrp;               // charge arriving at crp
    float xCrp;               // position on crp
    float yCrp;               // position on crp
    float tCrp;               // time of arrival at crp
    float qSigmaL;            // longitudinal charge spread
    float qSigmaT;            // tranverse charge spread

    ClassDef(VoxelInfo,2);
  };
}

#endif
