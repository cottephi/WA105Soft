#include "BeamInfo.h"

using namespace std;

ClassImp(BeamInfo)


BeamInfo::BeamInfo(){

RH_PBeam=0;
RH_TargetType=-1;
for (int i=0; i<2; i++) {
RH_Tof[i]=0;
RH_Slit1[i]=0;
RH_Slit2[i]=0;
}
for (int i=0; i<3; i++) 
{
RH_CerenkovThre[i]=0;
}
}




void BeamInfo::DumpBeamInfo() 

{

cout <<"========================================="<<endl;
cout <<"               BEAM SETUP DUMP           "<<endl;
cout <<"========================================="<<endl;


cout <<"Beam Momenta: "<<RH_PBeam<<endl; 
cout <<"Target Type:   "<< RH_TargetType<<endl;
cout <<"TOF : "<<RH_Tof[0]<<"  "<<RH_Tof[1]<<endl;  
cout <<"Cerenkov: " <<RH_CerenkovThre[0]<<" "<<RH_CerenkovThre[1]<<"  "<<RH_CerenkovThre[2]<<endl; 
cout <<"Slit1 : "<<RH_Slit1[0]<<"   "<<RH_Slit1[1]<<endl; 
cout <<"Slit2 : "<<RH_Slit2[0]<<"   "<<RH_Slit2[1]<<endl; 
 
} 




 
