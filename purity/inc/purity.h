#ifndef __PURITY_H__
#define __PURITY_H__

#include "RunHeader.h" 
#include "GeomConfig.h" 
#include "RecoConfig.h" 

#include "EventHeader.h"  
#include "HitRecon.h"
#include "Track2d.h"

#define  ntbin_drift 60  /*number on bins in drift coordinate,  bin width in time is given by 4000/ntbin */
#define nmaxdrift 120  /* max number of drift intervals, 120  means 6m/120 --> 5cm bin size*/
  
 
void ReadTrees(string fname, int maxevt);
void DumpRH(TFile *file, ofstream& out) ; 
int CheckFile(TFile *file, ofstream& out) ;   
void PrintHelp();
void SaveHisto();
void DefineHisto(); 

void ReadTrks(TObjArray *trks_V0,TObjArray *trks_V1,TObjArray *hits_V0,TObjArray *hits_V1,EventHeader *evt_header, ofstream& out);
 
  

float x_CRM[MAXCRM],y_CRM[MAXCRM],z_CRM[MAXCRM];
float DIMx_CRM[MAXCRM],DIMy_CRM[MAXCRM];
float pitchx_CRM[MAXCRM],pitchy_CRM[MAXCRM];
UInt_t nchax_CRM[MAXCRM],nchay_CRM[MAXCRM], nsample_CRM[MAXCRM];
float pitchT;
float pedvalue; 
int crmnum;
TString geotype; 
int ldrift, ntbin;
float timebin;

 
 
 

  
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

TH1F *hntbin;
TH1F *htbin0[nmaxdrift];
TH1F *htbin1[nmaxdrift];

#endif


