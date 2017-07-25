#ifndef _GEOMCONFIG_H_
#define _GEOMCONFIG_H_

#include <cmath>
#include <cstdlib>
#include <string>
#include <sstream>
#include <iostream>
#include "TString.h" 

using namespace std;

#define MAXCRM 50   
 
class GeomConfig {

 public: 
 
  GeomConfig();
  virtual ~GeomConfig(){;}
  void DumpGeomConfig() const;
  void CheckGeomConfig();


  /* getters */
  Float_t GetDriftCoord(int tdc) const;
  Float_t GetCrpCoord(int crm, int iview, int ch) const;

  Int_t   GetCRMnum() const {return GC_CRMnum;}
  Float_t GetCRMcX(int i) const {return GC_CRMxpos[i];}
  Float_t GetCRMcY(int i) const {return GC_CRMypos[i];}
  Float_t GetCRMcZ(int i) const {return GC_CRMzpos[i];}
  Float_t GetCRMxdim(int i) const {return GC_CRMxdim[i];}
  Float_t GetCRMydim(int i) const {return GC_CRMydim[i];} 
  Float_t GetCRMpitchX(int i) const {return GC_CRMpitchX[i];}
  Float_t GetCRMpitchY(int i) const {return GC_CRMpitchY[i];}
  Float_t GetCRMpitchT() const {return GC_CRMpitchT;}
  Int_t   GetCRMnchaX(int i) const {return GC_CRMnchaX[i];}
  Int_t   GetCRMnchaY(int i) const {return GC_CRMnchaY[i];}
  Float_t GetCRMgain(int i) const {return GC_CRMgain[i];}
  Int_t   GetCRMnsample(int i) const {return GC_CRMnsample[i];}

  Float_t GetGC_SRxdimension() const {return GC_SRxdimension;}
  Float_t GetGC_SRydimension() const {return GC_SRydimension;}
  Float_t GetGC_SRzdimension() const {return GC_SRzdimension;}  

  std::string GetGC_GeomType()  const { return GC_GeomType; }
  std::string GetGC_RDType() const { return GC_RDType; }
  std::string GetGC_ElecModel() const { return GC_ElecModel; }
  std::string GetGC_NoiseName() const { return GC_NoiseName; }
  
  Int_t GetGC_quanta() const  {return GC_quanta;}
  Float_t GetGC_Ped() const {return GC_ped;}
  Float_t GetGC_PedRMS() const { return GC_ped_rms; }
  Int_t GetGC_maxbit() const {return GC_maxbit;}
  Float_t GetGC_samplingfreq() const {return GC_samplingfreq;}
  Float_t GetGC_NoiseFreq() const {return GC_NoiseFreq;}
  Float_t GetGC_NoiseRMS() const {return GC_NoiseRMS;}
  Int_t GetGC_cohe() const {return GC_cohe;}

  /* setters */
  void SetCRMnum(Int_t CRMnum) 
  {
    CheckCRMNum(CRMnum);
    GC_CRMnum=CRMnum;
  }
  void SetCRMcX(int i, Float_t CRMxpos) 
  {
    CheckCRMNum(i);
    GC_CRMxpos[i]=CRMxpos;
  }
  void SetCRMcY(int i, Float_t CRMypos) 
  {
    CheckCRMNum(i);
    GC_CRMypos[i]=CRMypos;
  }
  void SetCRMcZ(int i, Float_t CRMzpos) 
  {
    CheckCRMNum(i);
    GC_CRMzpos[i]=CRMzpos;
  } 
  void SetCRMxdim(int i, Float_t CRMxdim) 
  {
    CheckCRMNum(i);
    GC_CRMxdim[i]=CRMxdim;
  }
  void SetCRMydim(int i, Float_t CRMydim) 
  {
    CheckCRMNum(i);
    GC_CRMydim[i]=CRMydim;
  } 
  void SetCRMpitchX(int i,Float_t CRMpitchX) 
  {
    CheckCRMNum(i);
    GC_CRMpitchX[i]=CRMpitchX;
  }
  void SetCRMpitchY(int i, Float_t CRMpitchY) 
  {
    CheckCRMNum(i);
    GC_CRMpitchY[i]=CRMpitchY;
  }
  void SetCRMpitchT(Float_t CRMpitchT) 
  {
    GC_CRMpitchT = CRMpitchT;
  }
  void SetCRMnchaX(int i, Int_t CRMnchaX) 
  {
    CheckCRMNum(i);
    GC_CRMnchaX[i]=CRMnchaX;
  }
  void SetCRMnchaY(int i, Int_t CRMnchaY) 
  {
    CheckCRMNum(i);
    GC_CRMnchaY[i]=CRMnchaY;
  }
  void SetCRMgain(int i, Float_t CRMgain) 
  {
    CheckCRMNum(i);
    GC_CRMgain[i]=CRMgain;
  }
  void SetCRMnsample(int i, Int_t CRMnsample) 
  {
    CheckCRMNum(i);
    GC_CRMnsample[i]=CRMnsample;
  }
  void SetGC_GeomType(std::string val){GC_GeomType = val; } 
  void SetGC_RDType(std::string val){GC_RDType= val; } 
  void SetGC_ElecModel(std::string val){GC_ElecModel= val; } 
  void SetGC_NoiseName(std::string val){GC_NoiseName= val; } 
  
  void SetGC_quanta(Int_t quanta) {GC_quanta=quanta;}
  void SetGC_Ped(Float_t ped) {GC_ped=ped;}    
  void SetGC_PedRMS(Float_t pedrms) {GC_ped_rms=pedrms;}    
  void SetGC_maxbit(Int_t bit ){GC_maxbit=bit;}
  void SetGC_samplingfreq (Float_t sampling) {GC_samplingfreq=sampling;};
  void SetGC_NoiseFreq(Float_t nfreq )   {GC_NoiseFreq = nfreq;}
  void SetGC_NoiseRMS(Float_t nrms)  {GC_NoiseRMS=nrms;}
  void SetGC_cohe(Int_t cohe)   {GC_cohe=cohe;}
  
  
private: 
 
  Int_t GC_CRMnum;
  Float_t GC_CRMxpos[MAXCRM];
  Float_t GC_CRMypos[MAXCRM];
  Float_t GC_CRMzpos[MAXCRM];
  Float_t GC_CRMxdim[MAXCRM];
  Float_t GC_CRMydim[MAXCRM];  
  Float_t GC_CRMpitchX[MAXCRM];
  Float_t GC_CRMpitchY[MAXCRM];
  Float_t GC_CRMpitchT; 
  Int_t GC_CRMnchaX[MAXCRM];
  Int_t GC_CRMnchaY[MAXCRM];
  Float_t GC_CRMgain[MAXCRM];
  Int_t GC_CRMnsample[MAXCRM];

  Float_t GC_SRxdimension;
  Float_t GC_SRydimension;
  Float_t GC_SRzdimension;  
  std::string GC_GeomType;
  std::string GC_RDType;
  std::string GC_ElecModel;
  std::string GC_NoiseName;
  Int_t GC_quanta; 
  Float_t GC_ped, GC_ped_rms; 
  Int_t GC_maxbit; 
  Float_t GC_samplingfreq;
  Float_t GC_NoiseFreq; 
  Float_t GC_NoiseRMS; 
  Int_t  GC_cohe;
 
  
  void CheckCRMNum(int crm);
  
  ClassDef(GeomConfig,2);
    
};
 

#endif
