#ifndef RunHeader_h
#define RunHeader_h


#include <iostream>
#include <vector>
#include <TROOT.h>

class RunHeader { 

public :
  
  RunHeader();
  virtual ~RunHeader(){;}
  void DumpRunHeader();


/* methods to access member */

Long64_t GetRunNumber() const {return RH_RunNumber;}
UInt_t   GetRunTime() const {return RH_RunTime;} 
Int_t    GetDataType() const{return RH_DataType;}  
Int_t    GetGeometry() const {return RH_Geometry;}
Int_t    GetBeamType() const {return RH_BeamType;} 
Int_t    GetPBeam() const {return RH_PBeam;} 
Float_t  GetLifeTime()  const {return RH_LifeTime;}  
Float_t  GetDiffusionL() const {return RH_DiffusionL;}   
Float_t  GetDiffusionT() const {return RH_DiffusionT;}   
TString  GetEfieldMap() const {return RH_EFieldMap;}
Float_t  GetEfieldValue() const {return RH_EFieldValue;} 
Int_t    GetCompression() const {return RH_IsCompressed;}  
Float_t  GetVGrid() const {return RH_VGrid;}
Float_t  GetVLEMBottom() const {return RH_VLEMBottom;}
Float_t  GetVLEMTop() const {return RH_VLEMTop;}
Int_t    GetTransportCode() const {return RH_TransportCode;}  
Int_t    GetPhysicList() const {return RH_PhysicList;}  
Int_t    GetStartSeed()  {return RH_StartSeed;}  
TString  GetConfigFile() const {return RH_ConfigFile;}
Int_t    GetReinteraction() const {return  RH_IsReinteraction;}
TString  GetGENIEfn() const {return RH_GENIEfn;}
TString  GetLMapLAR() const {return RH_LMapLAR;}
TString  GetLMapGAR() const {return RH_LMapGAR;}
Float_t  GetLSTime() const {return RH_LSTime;}  
Float_t  GetPMTsQEff() const {return RH_PMTsQEff;} 
TString  GetCRBeam() const {return RH_CRBeam;}
TString  GetCRCRY() const {return RH_CRCRY;}
Float_t  GetCRTO () const {return RH_CRTO;}
Float_t  GetCRWIN() const {return RH_CRWIN;}
Int_t    GetCREVNT() const {return RH_CREVNT;}
Int_t    GetCRRATE() const {return RH_CRRATE;}




/* methods to set member */

void SetRunNumber   (Long64_t RunNumber) {RH_RunNumber=RunNumber;}
void SetRunTime     (UInt_t RunTime)     {RH_RunTime=RunTime;}  
void SetDataType    (Int_t DataType)     {RH_DataType=DataType;}
void SetGeometry    (Int_t Geometry)     {RH_Geometry=Geometry;}
void SetBeamType    (Int_t BeamType)     {RH_BeamType=BeamType;}
void SetPBeam       (Float_t PBeam)      {RH_PBeam=PBeam;}
void SetLifeTime    (Float_t LifeTime)   {RH_LifeTime=LifeTime;}
void SetDiffusionL  (Float_t DiffL)      {RH_DiffusionL=DiffL;}
void SetDiffusionT  (Float_t DiffT)      {RH_DiffusionT=DiffT;}
void SetEfieldMap   (TString EFieldMap)  {RH_EFieldMap=EFieldMap;}
void SetEfieldValue (Float_t EFieldValue){RH_EFieldValue=EFieldValue;} 
void SetCompression (Int_t IsCompressed) {RH_IsCompressed= IsCompressed;}
void SetVGrid       (Float_t VGrid)      {RH_VGrid=VGrid;}
void SetVLEMBottom  (Float_t VLEMBottom) {RH_VLEMBottom=VLEMBottom;}
void SetVLEMTop     (Float_t VLEMTop)    {RH_VLEMTop=VLEMTop;}
void SetTransportCode(Int_t TransportCode){RH_TransportCode=TransportCode;}
void SetPhysicList  (Int_t PhysicList)   {RH_PhysicList=PhysicList;}
void SetStartSeed   (Int_t StartSeed)    {RH_StartSeed=StartSeed;}
void SetConfigFile  (TString  ConfigFile){RH_ConfigFile=ConfigFile;}
void SetReinteraction (Int_t IsReint  )  {RH_IsReinteraction= IsReint;}
void SetGENIEfn     (TString  GENIEfn)   {RH_GENIEfn=GENIEfn;}
void SetLMapLAR     (TString  LMapLAR)   {RH_LMapLAR=LMapLAR;}
void SetLMapGAR     (TString  LMapGAR)   {RH_LMapGAR=LMapGAR;}
void SetLSTime      (Float_t  LSTime)    {RH_LSTime=LSTime;}
void SetPMTsQEff    (Float_t  QEff)      {RH_PMTsQEff=QEff;}  
void SetCRBeam      (TString  CRBeam)    {RH_CRBeam=CRBeam;}
void SetCRCRY       (TString  CRCRY)     {RH_CRCRY=CRCRY;}
void SetCRTO        (Float_t  CRTO)      {RH_CRTO=CRTO;}
void SetCRWIN       (Float_t  CRWIN)     {RH_CRWIN=CRWIN;}
void SetCREVNT      (Int_t CREVNT)       {RH_CREVNT=CREVNT;}
void SetCRRATE      (Int_t CRRATE)       {RH_CRRATE=CRRATE;}



private:

Long64_t RH_RunNumber; /* Run Number */
UInt_t   RH_RunTime; /* Run Time/ Generation Time */ 
Int_t  RH_DataType;   /* 1 if Data, 2 if MC gun 3 if MC external file */
Int_t  RH_Geometry;   /* 3 if 3x1x1, 6 if 6x6x6 */
Int_t  RH_BeamType;   /* should we use geantID? should we define enum with 1=pi+, 2=pi-....to be decided */
Float_t RH_PBeam;  /* beam  energy */
Float_t  RH_LifeTime; /* -999 if Data */
Float_t  RH_EFieldValue; /* -999 if Map is used*/
TString  RH_EFieldMap; /* map name*/
Float_t RH_DiffusionL;
Float_t RH_DiffusionT; 
Int_t  RH_IsCompressed; /* O uncompressed, 1 compressed */ 
Float_t  RH_VGrid;   /* Charge readout configuration*/
Float_t  RH_VLEMBottom ;   
Float_t  RH_VLEMTop ;   
TString  RH_ConfigFile; /* Geomentry configuration files**/
TString  RH_GENIEfn; /* name of genie input file */
Int_t  RH_IsReinteraction ;
TString  RH_LMapLAR; /* Light Map */
TString  RH_LMapGAR; /* Light Map */
Float_t RH_LSTime;   /*sampling time*/
Float_t RH_PMTsQEff; /*PMTs Quantum efficiency, same for all PMTs*/  

Int_t  RH_TransportCode;  /* 3 if Geant3, 4 if Geant4, -999 if Data */
Int_t  RH_PhysicList; /* enum? -999 if Data*/
Int_t  RH_StartSeed;  /* -999 if Data*/   

TString  RH_CRBeam; /* beam particle if Cosmic Background is generated */
TString  RH_CRCRY; /* CRY cosmic file */
Float_t  RH_CRTO;  /* start time */
Float_t  RH_CRWIN; /*duration */
Int_t    RH_CREVNT; /* number of events from cry file */
Int_t    RH_CRRATE; /* rate */



ClassDef(RunHeader,1);

};

#endif

