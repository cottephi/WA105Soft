#ifndef _RAWDATAGENIE_H_
#define _RAWDATAGENIE_H_

#include <stdio.h>
#include <time.h>
#include <vector>

#include "TString.h"

#include "kinutil.h"

#include "DLArCRP.h"
#include "ChargeDrift.h"
#include "FieldMap.h"
#include "GENIE.h"

#define MAXPO 100

class RawDataGenie
{
 public:
  RawDataGenie();
  virtual ~RawDataGenie();
  
  virtual int doOpenGENIE(const char *ROOTfileName);
  virtual int doOpen_ND_GENIE(const char *ROOTfileName);
  
  //GENIE interface
  TFile *rootfile;
  TTree *s_tree;   
  int mNPO;
  microPO mPO[MAXPO];

  microPO* getPO();
  int getNPO(void);
  virtual void fill_PO_table(int reinter);
  virtual void fill_PO_ND_table(int reinter);

  double Ptot_mc[4];
  double Phad_mc[4];
  float  n_neutron;
  float n_K0l;
  float thetahadmc;
  int idumpGeVMeV;
  
  // genie structures
  struct GENIEVREC GENIEv;
  struct GENIENDREC GENIEnd;

  // The reconstructed kinematics
  struct KINEMATICREC RecKine; 

  virtual void DumpPOs(void);
  virtual void Dump_ND_POs(void);
  virtual int  DumpGENIEEvent(void);
  virtual int  DumpGENIE_ND_Event(void);

};

#endif
