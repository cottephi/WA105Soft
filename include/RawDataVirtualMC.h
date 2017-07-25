////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////
#ifndef __RAWDATAVIRTUAL_MC__
#define __RAWDATAVIRTUAL_MC__

#include <iostream>
#include <string>

#include <TParticle.h>
#include <TObjArray.h>

#include "VMCApplication.h"
#include "Preferences.h"

// charge projection and field maps
#include "ChargeDrift.h"
#include "FieldMap.h"

#include "BasicGeomMapping.h"
#include "RawDataGenie.h"

// data structures
#include "RunHeader.h"
#include "BeamInfo.h"
#include "EventHeader.h"
#include "DLArCRP.h"

#include "DLArLRO.h"


class RawDataVirtualMC
{

 public:
  RawDataVirtualMC( std::string detgeom, std::string vmctransport );
  RawDataVirtualMC( std::string detgeom, std::string vmctransport,
		    ChargeDrift *qdrift, EFieldMap *efld );
  
  ~RawDataVirtualMC();

  int OpenInputFile(const char *fname);
  int CloseInputFile();

  // GENIE inputs
  RawDataGenie *GetGenieInput(){ return genieInput; }

  void SetQdrift(ChargeDrift *qdrft){ PtrQdrift = qdrft; }
  void SetEfield(EFieldMap *efld){ PtrEfield = efld; }
  void SetCRPData(LArQReadMC::DLArCRP *crpdata){ PtrCRPData = crpdata; }

  void SetLROData(LArLReadMC::DLArLRO *lrodata){ PtrLROData = lrodata; }


  // load event
  int LoadEvent();

  // step data
  void StoreTrackStep(int pdg, double *pos, double t, double edep, double step);
  
  // output 
  LArQReadMC::DLArCRP *PtrCRPData;       //pointer to CRP Data
   
  //
  LArLReadMC::DLArLRO *PtrLROData;       //pointer to LRO Data


  // charge projection and drift class
  ChargeDrift *PtrQdrift;
  EFieldMap   *PtrEfield;

  void SetEdep(double val = 0){ m_Edep = val; }
  void SetEdepQ(double val = 0){ m_EdepQ = val; }
  void SetEdepE(double val = 0){ m_EdepE = val; }
  void SetEdepEQ(double val = 0){ m_EdepEQ = val; }
  void SetEdepH(double val = 0){ m_EdepH = val; }
  void SetEdepHQ(double val = 0){ m_EdepHQ = val; }
  void SetEdepBeam(double val = 0){ m_EdepBeam = val; }
  void SetEdepCR(double val = 0){ m_EdepCR = val; }
  void SetElost(double val = 0){ m_Elost = val; }
  void SetEdepCRT1(double val = 0){ m_EdepCRT1 = val; }
  void SetEdepCRT2(double val = 0){ m_EdepCRT2 = val; }

  void SetTruePhi(double val){ m_TruePhi = val; }
  void SetTrueTht(double val){ m_TrueTht = val; }
  void SetTrueVx(double val){ m_TrueVx = val; }
  void SetTrueVy(double val){ m_TrueVy = val; }
  void SetTrueVz(double val){ m_TrueVz = val; }
  void SetTrueP(double val){ m_TrueP = val; }
  void SetTruePx(double val){ m_TruePx = val; }
  void SetTruePy(double val){ m_TruePy = val; }
  void SetTruePz(double val){ m_TruePz = val; }

  double GetEdep(){ return m_Edep; }
  double GetEdepQ(){ return m_EdepQ; }
  double GetEdepE(){ return m_EdepE; }
  double GetEdepEQ(){ return m_EdepEQ; }
  double GetEdepH(){ return m_EdepH; }
  double GetEdepHQ(){ return m_EdepHQ; }
  double GetEdepBeam(){ return m_EdepBeam; }
  double GetEdepCR(){ return m_EdepCR; }
  double GetElost(){ return m_Elost; }
  double GetEdepCRT1(){ return m_EdepCRT1; }
  double GetEdepCRT2(){ return m_EdepCRT2; }

  double GetTruePhi(){ return m_TruePhi; }
  double GetTrueTht(){ return m_TrueTht; }
  double GetTrueVx(){ return m_TrueVx; }
  double GetTrueVy(){ return m_TrueVy; }
  double GetTrueVz(){ return m_TrueVz; }
  double GetTrueP(){ return m_TrueP; }
  double GetTruePx(){ return m_TruePx; }
  double GetTruePy(){ return m_TruePy; }
  double GetTruePz(){ return m_TruePz; }

  void ClearPrimaries();

  TObjArray* GetPrimaries(){ return m_Primaries; }

  // add primary 
  void AddPrimary( TParticle *p ) 
  { m_Primaries->Add(p); 
    // flag to keep it
    m_KeepPrimary.push_back(0);
  }

  bool IsGenieNu(){ return m_isGenieNu; }
  bool IsGenieNd(){ return m_isGenieNd; }
  
  TGeoManager* GetGeoManager(){ return detGeom->GetGeoManager(); }
    
 protected:
  bool IsTrigger() { return true; }
  //bool IsTrigger() { return (m_Edep != 0); }
  bool runVMC;

 private:

  void InitDetGeom(std::string detgeom);
  void InitVMC(std::string vmctransport);
  
  RawDataGenie *genieInput;
  
  // detector geometry : see InitDetGeom
  BasicGeomMapping   *detGeom;
  
  // VMC application : see InitVMC
  VMCApplication *appVMC;

  int RunNumber;
  int EventNumber; 
  int isEOF;
  int nEve; 
  int lastEve;
  int EvtOk;
  int isOpenInputFile;
  TString inputFileName;

  // energy depositions
  double m_Edep, m_EdepQ;   //total energy true and quenched
  double m_EdepE, m_EdepEQ; //total energy electronic
  double m_EdepH, m_EdepHQ; //total energy hadronic
  double m_EdepCR;          //quenched energy from CR
  double m_EdepBeam;        //quenched energy from beam
  double m_Elost;           //lost energy
  double m_EdepCRT1;
  double m_EdepCRT2;

  // primary particle
  double m_TruePhi;
  double m_TrueTht;
  double m_TrueVx;
  double m_TrueVy;
  double m_TrueVz;

  double m_TruePx;
  double m_TruePy;
  double m_TruePz;
  double m_TrueP;

  //
  bool m_isGenieNu;
  bool m_isGenieNd;

  // primary particles
  TObjArray *m_Primaries;

  // flag to keep in list
  std::vector<int> m_KeepPrimary;
  
};

#endif
