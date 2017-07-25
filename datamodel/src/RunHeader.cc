#include "RunHeader.h"



using namespace std;

ClassImp(RunHeader)

RunHeader::RunHeader(){

RH_RunNumber=0;
RH_RunTime=0; 
RH_DataType=-1;
RH_Geometry=0;
RH_BeamType=-1;
RH_PBeam=-999;
RH_LifeTime=-999;
RH_DiffusionL=-999;
RH_DiffusionT=-999;
RH_EFieldMap="";
RH_EFieldValue=-999; 
RH_IsCompressed=-1;
RH_VGrid=0;
RH_VLEMBottom=0;
RH_VLEMTop=0;
RH_TransportCode=-999;
RH_PhysicList=-999;
RH_StartSeed=-999;
RH_ConfigFile="";
RH_GENIEfn="";
RH_IsReinteraction=-1;
RH_LMapLAR="";
RH_LMapGAR="";
RH_LSTime=-1;
RH_PMTsQEff=1;
TString  RH_CRBeam="";
TString  RH_CRCRY=""; 
Float_t  RH_CRTO=0.;  
Float_t  RH_CRWIN=0.;  
Int_t    RH_CREVNT=0; 
Int_t    RH_CRRATE=-1; 

 

}

void RunHeader::DumpRunHeader() {

   struct tm  tstruct;
   char       buf[80];
   time_t  tonow=RH_RunTime;
   tstruct = *localtime(&tonow);
   strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
   std::cout << "currentDateTime()=" << buf << std::endl;     


cout <<"========================================="<<endl;
cout <<"               RUN HEADER DUMP           "<<endl;
cout <<"========================================="<<endl;
cout <<"RunNumber: "<<RH_RunNumber<<endl; 
cout <<"RunTime:   "<<buf<<endl; 
cout <<"DataType: " <<RH_DataType<<endl; 
cout <<"Geometry : "<<RH_Geometry<<endl; 
cout <<"Configuration file "<<RH_ConfigFile<<endl;
cout <<"Light map     "<<RH_LMapLAR<<endl; 
cout <<"              "<<RH_LMapGAR<<endl; 
cout <<"Sampling Time "<<RH_LSTime<<endl;
cout <<"Beam     : "<<RH_BeamType<<endl;
cout <<"P Beam   : "<<RH_PBeam<<endl;
cout <<"LifeTime: "<<RH_LifeTime<<endl; 
cout <<"EFieldMap: "<<RH_EFieldMap<<endl; 
cout <<"EFieldValue(V/cm): "<<RH_EFieldValue<<endl;  
cout <<"IsCompressed: "<<RH_IsCompressed<<endl; 
cout <<"V Grid : "<<RH_VGrid<<endl; 
cout <<"V LEM bottom : "<<RH_VLEMBottom<<"  top  "<< RH_VLEMTop<<endl; 
cout <<"TransportCode: "<<RH_TransportCode<<endl; 
cout <<"PhysicList: "<<RH_PhysicList<<endl; 
cout <<"StartSeed: "<<RH_StartSeed<<endl;

}
 

