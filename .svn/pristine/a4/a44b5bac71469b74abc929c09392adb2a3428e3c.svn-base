#include "GeomConfig.h"  

using namespace std;

ClassImp(GeomConfig) 

GeomConfig::GeomConfig() {
  GC_GeomType = "not set";
  GC_RDType = "not set";
  GC_ElecModel = "not set";
  GC_NoiseName = "not set";
  GC_CRMnum=0;
  GC_SRxdimension=0;
  GC_SRydimension=0;
  GC_SRzdimension=0;  
  GC_quanta=-1;
  GC_ped=-999; 
  GC_ped_rms=0; 
  GC_maxbit=0;
  GC_samplingfreq=0;
  GC_NoiseFreq=0; 
  GC_NoiseRMS=0; 
  GC_cohe=-1;
  
  GC_CRMpitchT=0;  
  
  for (int i=0; i<MAXCRM; i++) {
    GC_CRMxpos[i]=0;
    GC_CRMypos[i]=0;
    GC_CRMzpos[i]=0;
    GC_CRMxdim[i]=0;
    GC_CRMydim[i]=0; 
    GC_CRMpitchX[i]=0;
    GC_CRMpitchY[i]=0;
    GC_CRMnchaX[i]=0;
    GC_CRMnchaY[i]=0;
    GC_CRMgain[i]=0;
    GC_CRMnsample[i]=0;
  }
}
/*----------------------------------------------------------------------------------------*/
void GeomConfig::DumpGeomConfig() const
{
  
  cout <<"-----------GEOMETRY CONFIGURATION FILE ----------------"<<endl;
  cout <<" detector geometry "<<GC_GeomType << endl;
  cout <<" number of CRP: "<<GC_CRMnum<<endl;
  
  for (int i=0; i<GC_CRMnum; i++) {
    cout <<"--------------------------------------------------------"<<endl;
    cout <<"  CRP number  "<<i<<":"<<endl;
    cout <<"    position    "<<GC_CRMxpos[i]<<"  "<<GC_CRMypos[i]<<"  "<<GC_CRMzpos[i]<<endl;
    cout <<"    dimension   "<<GC_CRMxdim[i]<<"  "<<GC_CRMydim[i]<<endl;
    cout <<"    pitch       "<<GC_CRMpitchX[i]<<"  "<<GC_CRMpitchY[i]<<endl;
    cout <<"    time bin    "<<GC_CRMpitchT<<" (us)"<<endl;
    cout <<"    ncha        "<<GC_CRMnchaX[i]<<"  "<<GC_CRMnchaY[i]<<endl;
    cout <<"    gain        "<<GC_CRMgain[i]<<endl;
    cout <<"    sample      "<<GC_CRMnsample[i]<<endl;
  }
}
/*----------------------------------------------------------------------------------------*/

void GeomConfig::CheckGeomConfig(){
/*-----------------------------------------------------------------------------------------
 
 CRM numbering convention to be used in the configuration input file
  __________
  |    |    |
  |CRM2|CRM3|
  |____|____|   
  |    |    | 
  |CRM0|CRM1|
  |____|____|

                                                       

corner numbering c3________c2
                   |      |
                   | CMR  |
		   |______| 
		  c0      c1 
		   
---------------------------------------------------------------------------------------*/

  float CornerX[GC_CRMnum][4]; 
  float CornerY[GC_CRMnum][4];
  
  float ZValue[GC_CRMnum];
  float Zref;
  float sumx=0;
  float sumy=0;
 
  //find corners position */
  
  for (int i=0; i<GC_CRMnum; i++) {
    CornerX[i][0]=GC_CRMxpos[i]-(GC_CRMxdim[i]/2);
    CornerX[i][1]=GC_CRMxpos[i]+(GC_CRMxdim[i]/2);
    CornerX[i][2]=CornerX[i][1];
    CornerX[i][3]=CornerX[i][0];
    CornerY[i][0]=GC_CRMypos[i]-(GC_CRMydim[i]/2);
    CornerY[i][1]=CornerY[i][0];
    CornerY[i][2]=GC_CRMypos[i]+(GC_CRMydim[i]/2);
    CornerY[i][3]=CornerY[i][2];
    
    ZValue[i]=GC_CRMzpos[i];
    if (i==0) Zref=ZValue[i];
    if (ZValue[i]!=Zref ) 
      {
	cerr <<" CRM has different Z value ----> please check configuration file"<<endl;  //all CRM must have same Z position
	exit(1);
      }  
    
    sumx +=GC_CRMxdim[i];
    sumy +=GC_CRMydim[i];
  }
  
  /*
    for (int i=0; i<GC_CRMnum; i++) {
    cout <<"CRM  "<<i<<endl; 
    for (int c=0; c<4; c++) { 
    cout <<"---> Corner  "<<c<<" corner "<<CornerX[i][c]<<"  "<<CornerY[i][c]<<endl;
    }
    }   */
  
  
  //find dimensions of total sensitive regione 
 
  float totx=abs(CornerX[0][0])+abs(CornerX[GC_CRMnum-1][2]);
  float toty=abs(CornerY[0][0])+abs(CornerY[GC_CRMnum-1][2]);
  
   
  GC_SRxdimension=totx;
  GC_SRydimension=toty;
  GC_SRzdimension=Zref*2; 
  
  //and its center 
  
  float centerx=CornerX[GC_CRMnum-1][2]+CornerX[0][0];
  float centery=CornerY[GC_CRMnum-1][2]+CornerY[0][0];
  
  //  cout <<CornerX[0][0]<<" "<<CornerY[0][0]<<endl;
  //  cout <<CornerX[GC_CRMnum-1][2]<<"  "<<CornerY[GC_CRMnum-1][2]<<endl;
  cout <<"##################################################   "<<endl<<endl;
  cout <<"     GEOMETRY CONFIGURATION CHECK    "<<endl<<endl;
   
  cout <<"  sensitive area dimensions :  x "<<GC_SRxdimension<<" (cm), y "<<GC_SRydimension<<"  (cm), z "<<GC_SRzdimension<<endl;
  cout <<"  sensitive area is centered  in "<<centerx<<"  "<<centery<<endl; 
   
  //!!!

  if (centerx!=0 || centery!=0) {
    cerr <<" sensitive area is not centered in 0 0"<<endl;
    exit(1);
  }   
 
  //check for overlaps (comparison between sensitive area dimension and sum of CRM dimension 
/*  
  if (sumx<totx )  {
    cout <<"---------> overlaps between at least 2 CRM (x coordinate) "<<endl;}
  
  else  if (sumy<toty ){
    cout <<"---------> overlaps between at least 2 CRM (y coordinate) "<<endl; }
  
  else 
    cout <<"  no overlaps have been detected "<<endl;  */
  
  cout <<"#############################################   "<<endl;
}

// check CRM number 
void GeomConfig::CheckCRMNum(int crm)
{
  if( crm >= MAXCRM )
    {
      cerr<<"ERROR: number of CRMs is greater than max number suported by GeomConfig "<<MAXCRM<<endl;
      exit(-1);
    }
}
 


//
// return time coordinate of adc tick in us
//
Float_t GeomConfig::GetDriftCoord(int tdc) const
{
  return tdc * GC_CRMpitchT;
}


//
// return position of a given channel number in CRP in cm
// to be done with a channel map in the future
//
Float_t GeomConfig::GetCrpCoord(int crm, int iview, int ch) const
{
  if(crm >= GC_CRMnum)
    {
      cerr<<"ERROR: No CRM greater than "<<GC_CRMnum<<" found"<<endl;
      DumpGeomConfig();
      return -999;
    }
  
  Int_t nch; 
  Float_t pitch; 
  Float_t pos;
  Float_t dim;
  
  switch(iview)
    {
    case 0:
      nch   = GC_CRMnchaX[crm];
      pitch = GC_CRMpitchX[crm];
      pos   = GC_CRMxpos[crm];
      dim   = GC_CRMxdim[crm];
      break;
    case 1:
      nch   = GC_CRMnchaY[crm];
      pitch = GC_CRMpitchY[crm];
      pos   = GC_CRMypos[crm];
      dim   = GC_CRMydim[crm];
      break;

    default:
      cerr<<"ERROR: uknown view"<<endl;
      return -999;
    }
  
  Float_t xch = (ch*pitch - 0.5*nch*pitch) + pos;
  return xch;
}
