#include <iostream>
#include <vector>
#include <string>
#include <cstdio>  
#include <algorithm>

#include <TObjArray.h>
#include <TClonesArray.h>

#include "LiquidArgonMedium.h"
#include "RunHeader.h"
#include "BeamInfo.h"
#include "GeomConfig.h"
#include "EventHeader.h"
#include "HitReco.h"
#include "DLArCRP.h"
#include "HitReco.h"
#include "HitReconConfig.h"
#include "HitRecon.h"

const char *treerun = "RunHeader";
const char *treecrm = "EventTree";

vector<int> recint;
vector<float> recfl;
vector<string> recstr;

float x_CRM[MAXCRM],y_CRM[MAXCRM],z_CRM[MAXCRM];
float DIMx_CRM[MAXCRM],DIMy_CRM[MAXCRM];
float pitchx_CRM[MAXCRM],pitchy_CRM[MAXCRM];
int nchax_CRM[MAXCRM],nchay_CRM[MAXCRM], nsample_CRM[MAXCRM]; 
float pitchT;

int  ncrm,chXMax,chYMax; 

float MC_HIT0 [4][960] [20] [7];  //O=ntot hit, 1=first time bin, 2=charge from muon, 3 charge from e, 4 charge from gamma, 5 charge from pi0, 6: charge from charged pions, 7 charge from others  
float MC_HIT1 [4][960] [20] [7]; 

                                  

Long64_t sentry ;
Long64_t  maxevt ; 

using namespace std;

void ReadConfig(string fname);
void ReadVoxelInfo(LArVoxel::VoxelInfoCRP *vxl_crp);


///////////////////////////////////////////////////////////////////////
//
// main
//
///////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
  string finname     = "none";
  string fconfigname = "none";
  string suffix      = "";
  string hitAlgoName = "";  
  int selCRM         = -1; // -1 = process all CRMs
 

  if(argc == 1) 
    {
      cout<<"Usage: -i <input file> "<<endl;
      return 0;
    }

  char cc;
  while((cc = getopt(argc, argv, "i:j:m:c:s:")) != -1)
    {
      switch(cc)
	{
		  
	case 'i': 
	  finname = optarg;
	  break;

	case 's':
	  suffix = optarg;
	  break;	  

	case 'm':
	  selCRM = atoi(optarg);
	  break;
 
	case 'c':
	 fconfigname= optarg; 
         break; 
 
	default:
	  cerr<<"ERROR: unrecognized option"<<endl;
	  break;
	}
    }
  
  
  if(finname.compare("none") == 0) 
    {
      cerr<<"ERROR: no input file provided"<<endl;
      return 0;
    }
    
  if(fconfigname.compare("none") == 0) 
    {
      cerr<<"ERROR: no reco configuration file provided"<<endl;
      return 0;} 
  else
    {
      std::cout<<"reco configuration file "<<fconfigname.c_str()<<endl; 
      ReadConfig(fconfigname.c_str()); 
    }
     
  
  cout<<"Hit reconstruction with "<<hitAlgoName<<" algorithm "<<endl;
 
 
  // open input file
  TFile *fin    = TFile::Open(finname.c_str());
  TTree *treeRH = (TTree*)fin->Get(treerun);
  TTree *treeEV = (TTree*)fin->Get(treecrm);

  // run header
  RunHeader *run_head = new RunHeader();
  BeamInfo *beam_head = new BeamInfo(); 
  
  treeRH->SetBranchAddress("theRunHeader", &run_head); 
  treeRH->SetBranchAddress("theBeamInfo", &beam_head);
  
  // get event branches
  EventHeader *evt_head = new EventHeader();
  bool hasEvtHead = (treeEV->SetBranchAddress("theEventHeader", &evt_head)  == 0);
  
  LArRawData::DataCRP *dat_crp = new LArRawData::DataCRP();  
  bool hasRawData = (treeEV->SetBranchAddress("CRPRawData", &dat_crp)  == 0);
  
  LArVoxel::VoxelInfoCRP *vxl_crp = new LArVoxel::VoxelInfoCRP();
  bool hasVoxelInfo = (treeEV->SetBranchAddress("CRPVoxelData", &vxl_crp) == 0);

  if(!hasRawData) //need raw data
    {
      cerr << "ERROR: Raw data branch is not found" <<endl;
      TObjArray *myBranches = treeEV->GetListOfBranches();
      
      TIter next(myBranches);
      TObject *obj;
      cerr << "       List of possible branches in "
	   <<treeEV->GetName()<<":"<<endl;
      while( (obj = next()) )
	cout << "         " << obj->GetName() << endl;

      delete evt_head;
      delete dat_crp;
      delete vxl_crp;
      delete run_head;
      delete beam_head;
      fin->Close();
      return 0;
    }

  // output file
  string fbase = finname;
  fbase = fbase.substr(0, fbase.find_last_of(".root")-4);
  const char *foutname;
  if(suffix.empty())
    foutname = Form("%s_hitreco.root", fbase.c_str());
  else
    foutname = Form("%s_hitreco_%s.root", fbase.c_str(), suffix.c_str());
  
  TFile *fout = TFile::Open(foutname, "RECREATE");
  
  // write run header to the output
  TTree *treeRH_New = treeRH->CloneTree(0);
  treeRH_New->CopyEntries(treeRH);

  //add new branch
  HitReco* theHitReco;
  theHitReco = new HitReco();
  treeRH_New->Branch("theHitReco","HitReco", &theHitReco);
  //and fill it
  
  
  //store input file name and processing date

  UInt_t gTime   = time(0);
  
  theHitReco->SetHITRecTime(gTime);
  theHitReco->SetHITRecInFlname(finname);
  theHitReco->SetHITRecFNum(recfl.size());
  theHitReco->SetHITRecINum(recint.size());
  theHitReco->SetHITRecSNum(recstr.size());
  
  
  for (int i=0; i<int(recfl.size()); i++)  theHitReco->SetHITRecF(i,recfl[i]);
  for (int i=0; i<int(recstr.size()); i++) theHitReco->SetHITRecS(i,recstr[i]);
  for (int i=0; i<int(recint.size()); i++) theHitReco-> SetHITRecI(i,recint[i]); 
  
  
  //   theHitReco->DumpHitReco(); 
  
   
  treeRH_New->Fill();
  treeRH_New->Write();
  
  //////////////////////////////////////////////////////////////////////////////
  // hit reconstruction
  
  HitReconConfig *hitRecConf = HitReconConfig::Instance(); 
  hitAlgoName=theHitReco->GetHITRecS(0);
  hitRecConf->hitAlgo = hitAlgoName;
  
  //get Elec model from run header  
  GeomConfig *geo_config= new GeomConfig(); 
  treeRH->SetBranchAddress("theGeomConfig", &geo_config);  
  treeRH->GetEvent(0);
  
  
  // acquisition electronics model  
  hitRecConf->SetElecModule(geo_config->GetGC_ElecModel()); 
 
 

  // parameters for multihit finder

 
 
  hitRecConf->MultiHit_relTh1   = theHitReco->GetHITRecF(0);
  hitRecConf->MultiHit_relTh2   = theHitReco->GetHITRecF(1);
  hitRecConf->MultiHit_absTh1   = theHitReco->GetHITRecF(2);
  hitRecConf->MultiHit_absTh2   = theHitReco->GetHITRecF(3);
  hitRecConf->MultiHit_aboveT   = theHitReco->GetHITRecF(4);  
  hitRecConf->MultiHit_padLeft  = theHitReco->GetHITRecI(0);
  hitRecConf->MultiHit_padRight = theHitReco->GetHITRecI(1);
  
  hitRecConf->ROI_adcTh    = theHitReco->GetHITRecF(5);
  hitRecConf->ROI_nSigTh   = theHitReco->GetHITRecF(6); 
  hitRecConf->ROI_nSigLow1 = theHitReco->GetHITRecF(7);
  hitRecConf->ROI_nSigLow2 = theHitReco->GetHITRecF(8);
  hitRecConf->ROI_padLeft  = theHitReco->GetHITRecI(2);
  hitRecConf->ROI_padRight = theHitReco->GetHITRecI(3); 
  

  if (hitAlgoName.compare("AlgoTDeconHit")==0) 
    {
      hitRecConf->TDecon_hitTh   = theHitReco->GetHITRecF(9);
    }
  
   
   cout<<"========================================== "<<endl;
   //   theHitReco->DumpHitReco();
   

   //read CRM geom configuration 
   
    
   ncrm= geo_config->GetCRMnum();

   for (int nn=0; nn<geo_config->GetCRMnum(); nn++) {
     x_CRM[nn]=geo_config->GetCRMcX(nn);
     y_CRM[nn]=geo_config->GetCRMcY(nn);
     z_CRM[nn]=geo_config->GetCRMcZ(nn);
     DIMx_CRM[nn]=geo_config->GetCRMxdim(nn);
     DIMy_CRM[nn]=geo_config->GetCRMydim(nn); 
     pitchx_CRM[nn]=geo_config->GetCRMpitchX(nn);
     pitchy_CRM[nn]=geo_config->GetCRMpitchY(nn);
     nchax_CRM[nn]=geo_config->GetCRMnchaX(nn);
     if (nchax_CRM[nn]>chXMax) chXMax=nchax_CRM[nn]; 
     nchay_CRM[nn]=geo_config->GetCRMnchaY(nn);
     if (nchay_CRM[nn]>chYMax) chYMax=nchay_CRM[nn];
     nsample_CRM[nn]=geo_config->GetCRMnsample(nn);
   }  

   

   pitchT=geo_config->GetCRMpitchT();
   
 
   // hit recon object
   HitRecon hitRec;
  
   // output data structure
   TObjArray hitsView0; 
   hitsView0.SetOwner(true);
  
   TObjArray hitsView1; 
   hitsView1.SetOwner(true);
   
   // output tree
   TTree *treeEV_New = new TTree(treecrm, treecrm);
   treeEV_New->Branch("theEventHeader","EventHeader", &evt_head);
   treeEV_New->Branch("RecoHitsView0_Data", hitsView0.ClassName(), &hitsView0);
   treeEV_New->Branch("RecoHitsView1_Data", hitsView1.ClassName(), &hitsView1);
   //treeEV_New->Branch("RecoHitsData", CrpQhit->ClassName(), &CrpQhit);
   
   // now loop over entries
   Long64_t nentries = treeEV->GetEntries();
   cout<<"Total number of events "<<nentries<<endl;
   /*  if(sentry >= 0)
       cout<<"Selected event "<<sentry<<endl; */
 
   
   //int nHitsView0 = 0;
   //int nHitsView1 = 0;
   
   // drift velocity for nominal field
   Double_t Efield = run_head->GetEfieldValue(); // in V/cm
   Double_t vDrift = 0.1*LiquidArgonMedium::Instance()->DriftVelocity(1.0E-3*Efield); //in cm/us
    
   Long64_t ev_start = 0;
   Long64_t ev_end   = nentries;    
    
   if(sentry >= 0 && sentry < (int)ev_end && maxevt<0 && maxevt<0)
    {
      ev_start = sentry;
      ev_end   = sentry+1;
    } 
   if(sentry >= (int)ev_end)
     {
       cout<<"event requested larger that total number of entries in input file. The last event is processed"<<endl; 
       ev_start =  nentries-1;
       ev_end   =  ev_start+1 ;
     }  
     
   if (maxevt==-1) ev_end=nentries; 
   
   if ( maxevt>-1) { 
     Long64_t neve;
     if (maxevt<=nentries) {
       cout<<" ----> "<< maxevt <<" events will be analyzed "<<endl; 
       neve=maxevt-1;
       ev_start=0;
       ev_end=maxevt;
       if (sentry>-1) {
         ev_start= sentry;
         ev_end   = neve+sentry+1 ; 
	 if (ev_end>nentries) ev_end= nentries;
       }
     }  
     if (maxevt>=nentries){
       neve= nentries-1 ;  
       cout<<" ----> all events will be analyzed "<<endl;   
       ev_end   =  nentries; 
       ev_start=0;
       if (sentry>-1) ev_start= sentry;
     }
   }
   


   for (Long64_t nentry=ev_start;nentry<ev_end; nentry++) 
     {
       cout<<"Processing entry "<<nentry<<endl;
       
       treeEV->GetEvent(nentry);
       
       // dump header information
       //if(hasEvtHead)
      //evt_head->DumpEventHeader_True();
       
      
       //Read CRPVoxelData:   
       if (hasVoxelInfo)   ReadVoxelInfo(vxl_crp);      
  
       
       cout<<"Processing CRMs ... "<<endl;
      
       int crmcounter0=0; 
       int crmcounter1=0; 
       int currcrm=-1;
       TIter next_crm(dat_crp->GetCRMs());
       LArRawData::DataCRM *crm;
       while ( (crm = (LArRawData::DataCRM*) next_crm()) ) //loop over CRMs
	 {
	   if(selCRM != -1 && selCRM != crm->GetCRMId()) continue;
	   
	   if (crm->GetCRMId()>currcrm) { 
	     crmcounter0=hitsView0.GetEntries();
	     crmcounter1=hitsView1.GetEntries(); 
	     currcrm=crm->GetCRMId();
	   } 
	   cout<<endl<<" CRM : "<<crm->GetCRMId()<<endl;
	   cout<< "  Num channels in view 0 "<< crm->GetNChanView0() <<endl;
	   cout<< "  Num channels in view 1 "<< crm->GetNChanView1() <<endl;
	   

	   // loop over channels in each view
	   TClonesArray *vData = NULL;
	   int iview, hitcounter;
	   
	   // view 0
	   hitcounter = 0;
	   iview = 0;
	   vData = crm->GetChanView0();

	   LArRawData::DataChan *ch = NULL;  
	   TIter next0(vData); 
	   while ( (ch = (LArRawData::DataChan*)(next0())) )
	     {
	       hitRec.FindHits(ch); 
	       std::vector<LArReco::QHit> foundhits = hitRec.GetHits();
	       // copy hits into TObjArray
	       for(size_t ah=0;ah<foundhits.size();ah++)
		 {
		   foundhits[ah].SetCRM( crm->GetCRMId() );
		   foundhits[ah].SetView( iview );
		   foundhits[ah].SetCh( ch->GetChanId() );
		   // convert in us
		   foundhits[ah].SetT( geo_config->GetDriftCoord(foundhits[ah].GetTbStart()) );
		   // get spatial coordinate for this channel
		   foundhits[ah].SetX( geo_config->GetCrpCoord(foundhits[ah].GetCRM(),
							       foundhits[ah].GetView(),
							       foundhits[ah].GetCh()) );
		   foundhits[ah].SetD( vDrift * foundhits[ah].GetT() );
		   // object index (hit index)
		   foundhits[ah].SetObjIndex( hitsView0.GetEntries() );
		   
		   hitsView0.Add( new LArReco::QHit(foundhits[ah]) ) ;
		   hitcounter++;
		   
		 }
	     }
 
	     cout  <<"    Total hits in view 0 "<<hitsView0.GetEntries()<<endl;
	   
	   float difft=0;
	      
	   //fill event header	      
	   if(hasEvtHead) evt_head->SetNumRecHit0(crm->GetCRMId(), hitcounter) ;
	    
 	      
	   // assign HitIndex and associate and set   MC true
	   int nas;
	   float qtot; 
	   float qdiff, currqdiff;
	   float qmchit;
	   int idx;
	   float toprint; 
	   
	   for (int ah=crmcounter0; ah<hitsView0.GetEntries(); ah++) {
	     LArReco::QHit* ahit = (LArReco::QHit*)hitsView0.At(ah); 
	     nas=0;
	     qdiff=99999;
	     qmchit=0;
	     //ahit->SetHitIndex(ah);  
             
 
	     //loop on MC_hits
	     for (int mch=0; mch<=MC_HIT0  [ahit->GetCRM()] [ahit->GetCh()][0] [0]; mch++) {  
	       difft= ahit->GetTbStart()- MC_HIT0 [ahit->GetCRM()] [ahit->GetCh()][mch] [1];  
	       qtot=0;
	       
	       if (abs(difft) <10) {  
		 nas++; 
		 for (int e=2; e<8; e++) qtot+=MC_HIT0 [ahit->GetCRM()] [ahit->GetCh()][mch] [e];
		 
		 currqdiff=abs(qtot-ahit->GetQ())/qtot; 
		 
		 if (currqdiff <qdiff) { 
		   qdiff=currqdiff;
		   idx= mch; 
		   qmchit=qtot; 
		 }
		 
	       } 	// if (abs(difft) <10) {    
	       //		   
	     }//int nn=1; nn<=MC_HIT0 [ahit->GetCRM()] [ahit->GetCh()][0] [0]; nn++)   
	     
	     //		 if (nas>0) {
	     if (qmchit>0){
	       toprint= (qmchit-ahit->GetQ())/qmchit; 
	       
	     }
	     if (qmchit==0) {
	       toprint= -999; 
	     }
	     
	     
	     MC_HIT0 [ahit->GetCRM()] [ahit->GetCh()][idx] [1]=-99999;   ////MC_hit already associated
	     
	     for (int nq=0; nq<6; nq++) {
	       
	       ahit->SetHitMCCharge(nq,MC_HIT0 [ahit->GetCRM()] [ahit->GetCh()][idx] [2+nq]);
	       
	     }
	   } //  for (int ah=crmcounter0; ah<hitsView0.GetEntries(); ah++) {  
	      
  
	   // view 1
	   hitcounter = 0;
	   iview = 1;
	   vData = crm->GetChanView1();
	   
	   ch = NULL;  
	   TIter next1(vData); 
	   while ( (ch = (LArRawData::DataChan*)(next1())) )
	     {
	       hitRec.FindHits(ch); 
	       std::vector<LArReco::QHit> foundhits = hitRec.GetHits();
	       // copy hits into TObjArray
	       for(size_t ah=0;ah<foundhits.size();ah++)
		 {
		   foundhits[ah].SetCRM( crm->GetCRMId() );
		   foundhits[ah].SetView( iview );
		   foundhits[ah].SetCh( ch->GetChanId() );
		   // convert in us
		   foundhits[ah].SetT( geo_config->GetDriftCoord(foundhits[ah].GetTbStart()) );
		   // get spatial coordinate for this channel
		   foundhits[ah].SetX( geo_config->GetCrpCoord( foundhits[ah].GetCRM(),
								foundhits[ah].GetView(),
								foundhits[ah].GetCh()) );
		   foundhits[ah].SetD( vDrift * foundhits[ah].GetT() );
		   // object index (hit index)
		   foundhits[ah].SetObjIndex( hitsView1.GetEntries() );
		   //
		   hitsView1.Add( new LArReco::QHit(foundhits[ah]) ) ;
		   hitcounter++;
		 }
	     }
	   
	   cout <<"    Total hits in view 1 "<<hitsView1.GetEntries()<<endl;

	   //fill event header	      
	   if(hasEvtHead) evt_head->SetNumRecHit1(crm->GetCRMId(), hitcounter) ;

	   // assign HitIndex and associate and set   MC true
                   
	   
	   for (int ah=crmcounter1; ah<hitsView1.GetEntries(); ah++) {
	     LArReco::QHit* ahit = (LArReco::QHit*)hitsView1.At(ah); 
	     nas=0;
	     qdiff=99999;
	     qmchit=0;
	     //ahit->SetHitIndex(ah);  
	     //		 cout<<"============================================================"<<endl;  
	     //		 cout<<"HIT: "<<ah<<" CRM "<< ahit->GetCRM()<<"  channel "<< ahit->GetCh()<<endl;
	     
	     // and  MC true	  
	     //loop on MC_hits
	     for (int mch=0; mch<=MC_HIT1  [ahit->GetCRM()] [ahit->GetCh()][0] [0]; mch++) {  
	       difft= ahit->GetTbStart()- MC_HIT1 [ahit->GetCRM()] [ahit->GetCh()][mch] [1];  
	       qtot=0;
	       
	       if (abs(difft) <10) {  
		 nas++; 
		 for (int e=2; e<8; e++) qtot+=MC_HIT1 [ahit->GetCRM()] [ahit->GetCh()][mch] [e];
		 
		 currqdiff=abs(qtot-ahit->GetQ())/qtot; 
		  
		 if (currqdiff <qdiff) { 
		   qdiff=currqdiff;
		   idx= mch; 
		   qmchit=qtot; 
		 }
		 
	       } 	// if (abs(difft) <10) {    
	       //		   
	     }//int nn=1; nn<=MC_HIT1 [ahit->GetCRM()] [ahit->GetCh()][0] [0]; nn++)   
	      
	     
	     if (qmchit>0) {
	       toprint= (qmchit-ahit->GetQ())/qmchit; 
	       
	     }
	     if (qmchit==0) {
	       
	     }
	     
	     MC_HIT1 [ahit->GetCRM()] [ahit->GetCh()][idx] [1]=-99999;   ////MC_hit already associated
	     for (int nq=0; nq<6; nq++) {
	       ahit->SetHitMCCharge(nq,MC_HIT1 [ahit->GetCRM()] [ahit->GetCh()][idx] [2+nq]);
	     } 
	     
	     
	   }//  for (int ah=crmcounter1; ah<hitsView0.GetEntries(); ah++) {  
	   
	 } 
       
       // fill and prepare for next event
       treeEV_New->Fill();
       hitsView0.Clear();  //C
       hitsView1.Clear();  //C
     }
   
   

   fin->Close();

   // write event tree with our hits
//   treeEV_New->Write(); 
     treeEV_New->Write("",TObject::kOverwrite); 
   fout->Close();
   
   
   delete evt_head;
   delete dat_crp;
   delete vxl_crp;
   delete run_head;
   delete beam_head;
   HitReconConfig::DestroyInstance();
   
   return 0;
}



/*=======================================================================================*/  

void ReadVoxelInfo(LArVoxel::VoxelInfoCRP *vxl_crp)

/*=======================================================================================*/  

//read Voxels 

{
       TObjArray *vCRM;
       TObjArray *Voxels;  
       
   
       
 
         int nmaxhit=500;
  
        float truech0 [chXMax]  [nmaxhit];
 	int   timebin0  [chXMax]  [nmaxhit]; 
	int   pid0 [chXMax][nmaxhit];  
	int   touchedch0 [chXMax];
	
	
        float truech1 [chYMax]  [nmaxhit];
 	int   timebin1  [chYMax]  [nmaxhit]; 
	int   pid1 [chYMax] [nmaxhit];  
	int   touchedch1 [chYMax];
	
	 
//	int gap=4;
        int gap=6;
//	int gap=2;
//      int gap=8;
//          int gap=10;
	
       int icrm=-1; 
  
    
  
        LArVoxel::VoxelInfoCRM* vxlcrm=0;
        vCRM=vxl_crp->GetVoxelCRMs(); 
 
	
        TIter next(vCRM);
	
	
        while( (vxlcrm = (LArVoxel::VoxelInfoCRM*)next()) )
	{  
          int crmid = vxlcrm->GetCRMId();
	  if (crmid>icrm) {
	      icrm=crmid;
	      for (int nc=0; nc<chXMax; nc++) {
 	        touchedch0[nc]=0; 
	       for (int nh=0; nh<nmaxhit; nh++) {
	           truech0 [nc] [nh]=0;
 	           timebin0 [nc] [nh]=0;  
	           pid0 [nc] [nh]=0;  
	       }}  
	         for (int nc=0; nc<chYMax; nc++) { 
 		touchedch1[nc]=0;
	       for (int nh=0; nh<nmaxhit; nh++) { 
		   truech1[nc] [nh]=0;
 	           timebin1 [nc]  [nh]=0; 
	           pid1 [nc][nh]=0;   
	       }}  
	       
	       
	       
	     } 
	  
	 
	     
          QProjectorCRM qproj( x_CRM[crmid], y_CRM[crmid], 
			        pitchx_CRM[crmid], pitchy_CRM[crmid],pitchT, 
			        (UInt_t) nchax_CRM[crmid], (UInt_t) nchay_CRM[crmid],(UInt_t) nsample_CRM[crmid]);     
	  LArVoxel::VoxelInfo* vxl=0;
	  Voxels=vxlcrm->GetVoxels(); 
          TIter next(Voxels);
//voxel information

          int currCHX=-1;
	  int currCHY=-1;
	  int currpidx=-9999;
	  int currpidy=-9999;
	  int currtimex=-9999;
	  int currtimey=-9999;
	  float chargex=0;
	  float chargey=0;
	  int diffx=-999;
	  int diffy=-999;

           

          while( (vxl = (LArVoxel::VoxelInfo*)next()) ){
            
  
// calculate projection of charge on the anode
	       qproj.Projection( vxl->GetXCrp(), vxl->GetYCrp(), vxl->GetTCrp(), vxl->GetQCrp(),
			 	vxl->GetSigmaL(), vxl->GetSigmaT() );

   	 
	  
	    if (currCHX==-1) {
 		      chargex=0;
		      currCHX=qproj.GetChNum(vxl->GetXCrp(),0); 
  		      currtimex=qproj.GetTimeBin(vxl->GetTCrp());
		      currpidx=vxl->GetPid(); 
	                      }   
		   
	     if (currCHY==-1) {
 		      chargey=0;
		      currCHY=qproj.GetChNum(vxl->GetYCrp(),1); 
  		      currtimey=qproj.GetTimeBin(vxl->GetTCrp());
		      currpidy=vxl->GetPid(); 
	                       }  

	 	
		
	     diffx=abs (qproj.GetTimeBin(vxl->GetTCrp()) - currtimex); 
	     diffy=abs (qproj.GetTimeBin(vxl->GetTCrp()) - currtimey); 	
	     
	      
	  
//view 0 
  
               if (qproj.GetChNum(vxl->GetXCrp(),0) == currCHX    && (diffx==0) &&  vxl->GetPid()==currpidx) {
		    chargex+=vxl->GetQCrp();  
		  }    	 //if
	      else  
		{   
                     truech0 [currCHX] [touchedch0 [currCHX]] = chargex;
		     timebin0 [currCHX] [touchedch0[currCHX]] = currtimex;
		     pid0  [currCHX]  [touchedch0[currCHX]] = currpidx;
		     touchedch0 [currCHX]+=1; //number of "hits" for this channel 
                     if( touchedch0 [currCHX] >=nmaxhit) {
		       cout<<"===========PROBLEM on  hit number  "<<crmid<<"  "<<currCHX<<"  "<< touchedch0 [currCHX]<<endl;
		      } // if( touchedch0 [currCHX] >=nmaxhit) { 
 		     chargex=vxl->GetQCrp();
		     currCHX=qproj.GetChNum(vxl->GetXCrp(),0); 
  		     currtimex=qproj.GetTimeBin(vxl->GetTCrp());
		     currpidx=vxl->GetPid();    
		  }   //else    
		    
		   
//view 1
 
                if (qproj.GetChNum(vxl->GetYCrp(),1) == currCHY    && (diffy==0) &&  vxl->GetPid()==currpidy) {
		    chargey+=vxl->GetQCrp();   
		  }    	 //if
	       else  
	 	{    
                     truech1 [currCHY] [touchedch1 [currCHY]] = chargey; 
		     timebin1 [currCHY]  [touchedch1[currCHY]] = currtimey; 
		     pid1  [currCHY]  [touchedch1[currCHY]] = currpidy; 
		     touchedch1 [currCHY]+=1; //number of "hits" for this channel   
                     if( touchedch1 [currCHY] >=nmaxhit) {
		      cout<<"===========PROBLEM on  hit number "<<crmid<<"  "<<currCHY<<"  "<< touchedch1 [currCHY]<<endl;
		      } // if( touchedch1 [currCHY] >=nmaxhit) { 
 		     chargey=vxl->GetQCrp();
		     currCHY=qproj.GetChNum(vxl->GetYCrp(),1); 
  		     currtimey=qproj.GetTimeBin(vxl->GetTCrp());
		     currpidy=vxl->GetPid();     
		  }   //else   		    
		    
		    
		
	} //while( (vxl = (LArVoxel::VoxelInfo*)next()) ) */


       
  
         for(int nc=0; nc<chXMax; nc++) {
	   for (int nf=0; nf<20; nf++) {
	      for (int nv=0; nv<8; nv++) {
 	         MC_HIT0 [crmid][nc] [nf] [nv]=0;  
        }}}     
	 for(int nc=0; nc<chYMax; nc++) {
	   for (int nf=0; nf<20; nf++) {
	      for (int nv=0; nv<8; nv++) { 
 		 MC_HIT1 [crmid][nc] [nf] [nv]=0; 
        }}}        
    
		
// now build MC_hits for view0   
 
                  int ntot0=0;
		  int ntot1=0;
    
   
	     for (int ch=0; ch<chXMax; ch++) {  //loop on channels
  	       int r0time[touchedch0[ch]];  
	        
 		    for (int nh=0; nh<touchedch0[ch]; nh++)  r0time[nh]=timebin0  [ch]  [nh];  
//time sorting	     
	     
  	            std::sort(r0time, r0time+touchedch0[ch]);  
		    
 		    int nf=0; 
		    int TBin_ref=-1;  
	     
	           for (int rt=0; rt<touchedch0[ch]; rt++) { 
		     int tocompare=r0time[rt];  
		     for (int nh=0; nh<touchedch0[ch]; nh++) { 
	               if (timebin0 [ch][nh]==tocompare) { 
 
//found element in the list, check TBin
                           if ((timebin0 [ch] [nh]- TBin_ref)<=gap ){	  
			     if  ( abs (pid0 [ch] [nh]) == 13 ) { MC_HIT0[crmid][ch]  [nf] [2]+=truech0 [ch] [nh] ;} 
			     else if  ( abs (pid0 [ch] [nh]) ==  11) { MC_HIT0 [crmid][ch]  [nf] [3]+=truech0 [ch] [nh] ;}
			     else if  (  pid0 [ch]  [nh]  ==  22) { MC_HIT0 [crmid][ch] [nf] [4]+=truech0 [ch]  [nh] ;}
			     else if  (  pid0 [ch]  [nh]  == 111)  {MC_HIT0 [crmid][ch] [nf] [5]+=truech0 [ch]  [nh] ;}
			     else if  (  abs (pid0 [ch] [nh])   == 211) { MC_HIT0 [crmid][ch] [nf] [6]+=truech0 [ch]  [nh] ;}
			     else { MC_HIT0 [crmid][ch] [nf] [7]=truech0 [ch]  [nh] ;}
			     
			     
			      TBin_ref=timebin0 [ch]  [nh] ; 
			      timebin0  [ch] [nh]=-1; 
			    }  
			   else
			   {  
			      nf++;	
			     MC_HIT0 [crmid][ch] [0] [0]=nf;
			     
			     MC_HIT0 [crmid][ch] [nf] [1]=timebin0 [ch]  [nh] ; 
			     if  ( abs (pid0 [ch] [nh]) == 13 ) { MC_HIT0[crmid][ch][nf] [2]=truech0 [ch] [nh] ;} 
			     else if  ( abs (pid0 [ch] [nh]) ==  11) { MC_HIT0 [crmid][ch] [nf] [3]=truech0 [ch] [nh] ;}
			     else if  (  pid0 [ch] [nh]  ==  22) { MC_HIT0 [crmid][ch] [nf] [4]=truech0 [ch] [nh] ;}
			     else if  (  pid0 [ch] [nh]  == 111)  {MC_HIT0 [crmid][ch] [nf] [5]=truech0 [ch]  [nh] ;}
			     else if  (  abs (pid0 [ch] [nh])   == 211) { MC_HIT0 [crmid][ch] [nf] [6]=truech0 [ch] [nh] ;}
			     else { MC_HIT0 [crmid][ch] [nf] [7]=truech0 [ch] [nh] ;}
                             TBin_ref=timebin0 [ch] [nh] ;
			     timebin0  [ch] [nh]=-1;   
 			     
			    }  //else 
		 	 }//tocompare
		        
		        } //nh
	     
	           } //rt */
		   
		                 ntot0+=MC_HIT0 [crmid][ch] [0] [0];  
 		      
	
  
 
		}  //  for(int nc=0; nc<chXMax; nc++)
	
// now build MC_hits for view1   



	     for (int ch=0; ch<chYMax; ch++) {  //loop on channels 
	       int r1time[touchedch1[ch]];    
 	 
		    for (int nh=0; nh<touchedch1[ch]; nh++)  r1time[nh]=timebin1 [ch]  [nh]; 
//time sorting	     
 
		    std::sort(r1time, r1time+touchedch1[ch]); 
 
 	            int nf1=0; 
	 	    int TBin1_ref=-1;  
	  
	            for (int rt=0; rt<touchedch1[ch]; rt++) {
		     
		     int tocompare=r1time[rt];  
		     
		     for (int nh=0; nh<touchedch1[ch]; nh++) {
 
	               if (timebin1 [ch] [nh]==tocompare) { 
 
//found element in the list, check TBin
                           if ((timebin1 [ch] [nh]- TBin1_ref)<=gap ){	  
			     
			     if  ( abs (pid1 [ch]  [nh]) == 13 ) { MC_HIT1 [crmid][ch] [nf1] [2]+=truech1 [ch] [nh] ;} 
			     else if  ( abs (pid1 [ch] [nh]) ==  11) { MC_HIT1 [crmid][ch] [nf1] [3]+=truech1 [ch] [nh] ;}
			     else if  (  pid1 [ch]  [nh]  ==  22) { MC_HIT1 [crmid][ch] [nf1] [4]+=truech1 [ch] [nh] ;}
			     else if  (  pid1 [ch]  [nh]  == 111)  {MC_HIT1 [crmid][ch] [nf1] [5]+=truech1 [ch] [nh] ;}
			     else if  (  abs (pid1 [ch]  [nh])   == 211) { MC_HIT1 [crmid][ch] [nf1] [6]+=truech1 [ch] [nh] ;}
			     else { MC_HIT1 [crmid][ch] [nf1] [7]=truech1 [ch] [nh] ;}
			     
			     
			      TBin1_ref=timebin1 [ch] [nh] ; 
			      timebin1  [ch]  [nh]=-1; 
			    }  
			   else
			   {  
			      nf1++;	
			     MC_HIT1 [crmid][ch]  [0] [0]=nf1;
			     
			     MC_HIT1 [crmid][ch]  [nf1] [1]=timebin1 [ch] [nh] ; 
			     if  ( abs (pid1[ch]  [nh]) == 13 ) { MC_HIT1 [crmid][ch] [nf1] [2]=truech1 [ch]  [nh] ;} 
			     else if  ( abs (pid1 [ch] [nh]) ==  11) { MC_HIT1 [crmid][ch] [nf1] [3]=truech1 [ch] [nh] ;}
			     else if  (  pid1 [ch] [nh]  ==  22) { MC_HIT1 [crmid][ch] [nf1] [4]=truech1 [ch] [nh] ;}
			     else if  (  pid1 [ch] [nh]  == 111)  {MC_HIT1 [crmid][ch] [nf1] [5]=truech1 [ch]  [nh] ;}
			     else if  (  abs (pid1 [ch] [nh])   == 211) { MC_HIT1 [crmid][ch] [nf1] [6]=truech1 [ch] [nh] ;}
			     else { MC_HIT1 [crmid][ch] [nf1] [7]=truech1 [ch]  [nh] ;}
                             TBin1_ref=timebin1 [ch] [nh] ;
			     timebin1  [ch]  [nh]=-1;   
//			     
			    }  //else 
		 	 }//tocompare
		        
		        } //nh
	     
	           } //rt	  
		   
		       ntot1+=MC_HIT1 [crmid][ch] [0] [0]; 
 
 	 
		   
		}  //for (int ch=0; ch<chYMax; ch++)

	
 
       } //while( (vxlcrm = (LArVoxel::VoxelInfoCRM*)next()) ) 	  
       
       
  
       
  }     




 






void ReadConfig(string fname) 
{    
  
  ifstream f(fname.c_str());
  
  if (!f) 
    { 
      cout <<"  reconstruction setup file  "<<fname<< "  does not exist  STOP "<<endl;
      exit(1);
    } 
  
  // read file 
   
  char *filename = new char [fname.size()+1];
 
  strcpy(filename,fname.c_str()); 
  ifstream inFile; 
  inFile.open(filename,ios::in);
  string aLine="";
  
  string  tostart="RECO_HIT";
  string  tocheck="HIT_ALGORITHM"; 
  string  tostore1="PARAMETER1"; 
  string  tostore2="PARAMETER2"; 
  string  tostore3="PARAMETER3"; 
  string  toexit="END_ALGO";
  string  toend="END_RECO_HIT";
  string  toprocess="EVENTS";
  
  int avalue;
  string aname=""; 
  string astring="";   

      
   int is_selected=0;   
      
   while (!inFile.eof())
    {
     getline(inFile,aLine); 
     istringstream ss; 
      if (aLine.compare(0, tostart.length(), tostart)==0) { 
       cout<<" start reading configuration file"<<endl;
      } 
       if (aLine.compare(0, toprocess.length(), toprocess)==0) { 
        ss.str(aLine);
        ss>>astring>>sentry>>maxevt; 
	
      }
      if (aLine.compare(0, tocheck.length(), tocheck)==0) { 
        ss.str(aLine);
        ss>>astring>>avalue>>aname;
        if (avalue>0) {    
 	 cout <<"algo  "<<aname<<"  has been chosen   "<<endl;
	 recstr.push_back(aname);
	 is_selected=1; 
        }
       }
        if (aLine.compare(0, tostore1.length(), tostore1)==0) {
	 if (is_selected==1) { 
	 ss.str(aLine); 
	  int i1,i2;
	  float f1,f2,f3,f4,f5;
	  ss>>astring>>f1>>f2>>f3>>f4>>f5>>i1>>i2;   
	  recint.push_back(i1);
	  recint.push_back(i2); 
	  recfl.push_back(f1);
	  recfl.push_back(f2);
	  recfl.push_back(f3); 
	  recfl.push_back(f4);
	  recfl.push_back(f5);  
        }
	}
	if (aLine.compare(0, tostore2.length(), tostore2)==0) { 
	 if (is_selected==1) { 
	  ss.str(aLine);
	  int i3,i4;
	  float  f6,f7,f8,f9;
	  ss>>astring>>f6>>f7>>f8>>f9>>i3>>i4;
	  recint.push_back(i3);
	  recint.push_back(i4);  
	  recfl.push_back(f6);
	  recfl.push_back(f7); 
	  recfl.push_back(f8);
	  recfl.push_back(f9);
	 } 
	}
  	if (aLine.compare(0, tostore3.length(), tostore3)==0) { 
	 if (is_selected==1) { 
	  ss.str(aLine);
	  float f10; 
	  ss>>astring>>f10; 
	  recfl.push_back(f10); 
	 } 
	}
	
	if (aLine.compare(0, toexit.length(), toexit)==0) {
          if (is_selected==1)  {
	      cout<<"set up has been read "<<endl;
	     return;} 
        }
	if (aLine.compare(0, toend.length(), toend)==0) { 
	 if (is_selected ==0 ) {
	      cout <<" no algorithm for hit reconstruction has been selected   STOP "<<endl;
             exit(1);
	 } 
         return;
       }
      }  //eof
      
      
      
      }
