/********************************








*******************************/

#ifndef __DLARLRO_H__
#define __DLARLRO_H__

//
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

//
#include <TTree.h>


//
#include "DataLRO.h"

//
#include "SpaceMap.h"
#include "SpaceMapSet.h"
#include "SpaceMapFolder.h"
#include "SpaceMapMgr.h"

#include "GLArProcess.h"


namespace LArLReadMC
{
  class DLArLRO {

  public:
    DLArLRO();
    ~DLArLRO();
    
    void BeginEvent();
    void EndEvent();

    void RegisterBranches(TTree *tree);

    bool ImportSet(std::string type, std::string file, std::string tags, std::string histos, int idx0, int idx1);
  

    //times are [ns]
    void Fill( int pdg, double edep, double step, double efield, double R, double* pos_step, double* pos_crp, double time, double driftTime, double gain);


    void SetNPMTs(int);
    void SetSamplingTime(double);
    void SetTimeWindow(int,double,double);
    void SetPMTQuantumEff(float); 
    void SetElectronLifeTime(float tau /*ms*/);

    int     GetNPMTs() const { return NPMTs; }
    double  GetSamplingTime() const { return sampTime; }
    int     GetTimeWinNumOfBins() const {return timeWinNbins;}
    double     GetTimeWinMin() const {return timeWinMin;}
    double     GetTimeWinMax() const {return timeWinMax;}
    float      GetPMTQuantumEff() const {return quantumEff;}
    float      GetElectronLifeTime() const {return electronLifeTime;}

    bool TimeHistoIsAuto() const {return h_is_auto;}

    Map::SpaceMapFolder spaceMapFld_LAr;

    Map::SpaceMapFolder spaceMapFld_GAr;

    

  private:
     void Clear();
     
     LArRawData::DataLRO *dataLRO;
     
     GLArProc GLArProcess;

     void buildHistos();
     void rebinHistos();

     int NPMTs;  
     bool h_is_auto;
     double sampTime;
     int timeWinNbins;
     double timeWinMin;
     double timeWinMax;

     float quantumEff;
     float electronLifeTime;
     double electroLumFactor;

  };

}//



#endif
