#ifndef __BENCH_H__
#define __BENCH_H__

#include "RunHeader.h" 
#include "GeomConfig.h" 
#include "RecoConfig.h" 

#include "EventHeader.h" 
#include "DLArCRP.h" 
#include "ChargeDrift.h"
#include "LiquidArgonMedium.h"
#include "FieldMap.h"
#include "QProjector.h"
#include "HitRecon.h"
#include "Track2d.h"
 
void ReadTrees(string fname, string ofile, int maxevt);
void DumpRH(TFile *file, ofstream& out) ; 
int CheckFile(TFile *file, ofstream& out) ;   
void PrintHelp();
void SaveHisto(int ipass);
void DefineHisto(int ipass);
void ReadEH(EventHeader *evt_header,ofstream& out);
void ReadEHonline(EventHeader *evt_header,ofstream& out);
void ReadRawData(LArRawData::DataCRP *dat_crp,ofstream& out );
void ReadVoxel(LArVoxel::VoxelInfoCRP *vxl_crp,ofstream& out); 
void ReadHits(TObjArray *hits_V0,TObjArray *hits_V1,EventHeader *evt_header, ofstream& out);
void ReadTrks(TObjArray *trks_V0,TObjArray *trks_V1,TObjArray *hits_V0,TObjArray *hits_V1,EventHeader *evt_header, ofstream& out);
 
  

float x_CRM[MAXCRM],y_CRM[MAXCRM],z_CRM[MAXCRM];
float DIMx_CRM[MAXCRM],DIMy_CRM[MAXCRM];
float pitchx_CRM[MAXCRM],pitchy_CRM[MAXCRM];
UInt_t nchax_CRM[MAXCRM],nchay_CRM[MAXCRM], nsample_CRM[MAXCRM];
float pitchT;
float pedvalue; 
int crmnum;
 
 

  
Bool_t isRunHeader,isGeom,isEventTree; //trees
Bool_t istheRunHeader,istheGeomConfig,istheBeamInfo,istheEventHeader,isCRPRawData,isCRPVoxelData,isLRORawData; //branches
Bool_t istheHitReco,istheTrkReco;
Bool_t isHitView0,isHitView1;
Bool_t isTrkView0,isTrkView1;

 
float AdcToCharge = 1/(11.8*4.44/0.806);
float Wei=23.6*0.000001; 
float ele_fC= 1/1.60217646e-4; 

//Define output root file
     TFile *fout;
 
//histos booking
 
//event Header 
TH1F *heve;
TH1F *hmomemtumc;
TH1F *hmomemtum;
TH1F *htheta;
TH1F *hphi; 
TH1F *hxcoord;
TH1F *hycoord;
TH1F *hzcoord;  
TH1F *hene;  
TH1F *heneq;  
TH1F *hemene; 
TH1F *hemeneq; 
TH1F *hhene; 
TH1F *hheneq;  
TH1F *hlost;
TH1F *heneCRT1;
TH1F *heneCRT2; 

//real data
TH1F *hraw_run;
TH1F *hraw_seq;
TH1F *hraw_nev;
TH1F *hraw_nevsize;
      
//RawData      

TH1F *rawdatawire0[MAXCRM];
TH1F *rawdatawire1[MAXCRM];

TH1F  *hintegral0[MAXCRM]; 
TH1F  *hintegral1[MAXCRM]; 

TH1F  *htotcharge0[MAXCRM];  
TH1F  *htotcharge1[MAXCRM];    

 
 
 
//HitReconstruction

 
TH1F *hitnumber0[MAXCRM];
TH1F *hitnumber1[MAXCRM];

TH1F *hittedwire0[MAXCRM];
TH1F *hittedwire1[MAXCRM];

TH1F *hitperwire0[MAXCRM];
TH1F *hitperwire1[MAXCRM];

TH1F *hhitcharge0[MAXCRM];
TH1F *hhitcharge1[MAXCRM];

TH1F *hittedwire0T0[MAXCRM];
TH1F *hittedwire1T0[MAXCRM];

TH1F *htotreccharge0[MAXCRM];
TH1F *htotreccharge1[MAXCRM];

//TrackReconstruction

TH1F *trknumber0[MAXCRM];
TH1F *trknumber1[MAXCRM];  

TH1F *hSlopeI_0[MAXCRM];  
TH1F *hSlopeIErr_0[MAXCRM];  
TH1F *hSlopeF_0[MAXCRM];  
TH1F *hSlopeFErr_0[MAXCRM];  
TH1F *hpathX0[MAXCRM];
TH1F *hpathY0[MAXCRM];
TH1F *hdq0[MAXCRM];
TH1F *nhit_trk0[MAXCRM];
TH1F *hcheckhitQ0[MAXCRM];
TH1F *hcheckhitT0[MAXCRM];
TH1F *hcheckhitX0[MAXCRM]; 
TH1F *hdeltahitQ0[MAXCRM];
TH1F *hdeltahitT0[MAXCRM];
TH1F *hdeltahitX0[MAXCRM];
TH1F *hdeltahitch0[MAXCRM];

TH1F *hSlopeI_1[MAXCRM];  
TH1F *hSlopeIErr_1[MAXCRM];  
TH1F *hSlopeF_1[MAXCRM];  
TH1F *hSlopeFErr_1[MAXCRM];   
TH1F *hpathX1[MAXCRM];
TH1F *hpathY1[MAXCRM];
TH1F *hdq1[MAXCRM];
TH1F *nhit_trk1[MAXCRM];
TH1F *hcheckhitQ1[MAXCRM];
TH1F *hcheckhitT1[MAXCRM];
TH1F *hcheckhitX1[MAXCRM]; 
TH1F *hdeltahitQ1[MAXCRM];
TH1F *hdeltahitT1[MAXCRM];
TH1F *hdeltahitX1[MAXCRM]; 


#endif


