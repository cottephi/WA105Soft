/////////////////////////////////////////////////////////////////////////////////
//
//  Defines CRP and handles filling data classes
//
//
////////////////////////////////////////////////////////////////////////////////
#ifndef __DLARCRP_H__
#define __DLARCRP_H__

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include <TTree.h>

#include "ElecSim.h"
#include "DLArCRM.h"
#include "DataCRP.h"
#include "VoxelInfoCRP.h"
#include "NoiseGen.h"

namespace LArQReadMC
{
  class DLArCRP
  {
  public:
    DLArCRP();
    ~DLArCRP();

    void DefineCRP(std::string config);
    void SetCollEffMap(std::string fname);
    
    void RegisterBranches(TTree *tree);

    BasicDigitizer* GetElecModel(){ return ElecSim; }
    size_t GetNumCRMs(){ return ncrms; }

    void BeginEvent();
    void EndEvent();

    // Fill CRMs with data
    void   Fill( int pid, double edep, double edepq,
		 double xvox, double yvox, double zvox,
		 double tvox, double dt, double x, double y,
		 double q, double sigmaL, double sigmaT );

    double DistanceToCRP(double x, double y, double z);
    double GetZ();
    float  GetCRMGain(double x, double y);

    bool   IsInDriftWin(double dt);

  private:
    void Clean();
    void ReadConfig(const char *filename);
    void ReadDatacard(std::istringstream &line, std::string &keyword);

    void SetRawDataType(std::string tname, int qntz);
    void SetElecSimType(std::string tname, int bits, float ped, float ped_rms, float sampling);
    bool Overlaps(DLArCRM *crm);

    bool quant;  //quantize data
    size_t ncrms;
    BasicDigitizer *ElecSim;
    std::vector<DLArCRM*> CRMs;

    // pointer to the raw data
    LArRawData::DataCRP *DatCRP;
    LArRawData::Dtype     dtype;

    // voxel information
    LArVoxel::VoxelInfoCRP *VxlCRP;

    std::vector<BasicNoise*> Noise;
    // add LRO data structure

  };
}

#endif
