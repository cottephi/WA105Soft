
#ifndef BeamInfo_h
#define BeamInfo_h


#include <iostream>
#include <vector>
#include <TROOT.h>




class BeamInfo { 

public :

  BeamInfo();
  virtual ~BeamInfo(){;}
  void DumpBeamInfo();
  

/* methods to access member */

Float_t  GetPBeam() {return RH_PBeam;} 
Int_t    GetTargetType() {return RH_TargetType;} 
Float_t  GetTof(int i) {return RH_Tof[i];}
Float_t  GetCerenkovThre (int i) {return RH_CerenkovThre[i];}
Float_t  GetSlit1(int i) {return RH_Slit1[i];}
Float_t  GetSlit2(int i) {return RH_Slit2[i];}

/* methods to set member */

void SetPBeam (Float_t PBeam) {RH_PBeam=PBeam;} 
void SetTargetType (Int_t TargetType) {RH_TargetType=TargetType;} 
void SetTof (int i, Float_t  Tof ) {RH_Tof[i]=Tof;}
void SetCerenkovThre (int i, Float_t CerenkovThre) {RH_CerenkovThre[i]=CerenkovThre;}
void SetSlit1(int i, Float_t Slit1) {RH_Slit1[i]=Slit1;}
void SetSlit2(int i, Float_t Slit2) {RH_Slit2[i]=Slit2;}



private:

Float_t RH_PBeam;  /* beam momentum */
Int_t RH_TargetType; /* target type enum to be defined */
Float_t RH_Tof[2]; /*contatori scintillazione */
Float_t RH_CerenkovThre[3]; /* cerenkov threshold pion, k proton*/
Float_t RH_Slit1[2];  /*momentum spread */
Float_t RH_Slit2[2];   /* momentum spread */

ClassDef(BeamInfo,1);

};

#endif
