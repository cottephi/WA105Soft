#include <iostream>
#include <fstream>
#include <sstream>
#include <vector> 
#include <string>
#include <iomanip> 
#include <cstdlib> 
#include <unistd.h> 
#include <time.h> 


#include <TFile.h>
#include <TTree.h>
#include <TList.h>
#include <TString.h>
#include <TH1F.h>
#include <TH2F.h> 
#include <TNtuple.h> 
#include "TROOT.h"
#include "TSystem.h"  
#include "TObjectTable.h"  
#include "bench.h"
#include "EventDecoder.h"
#include "EventDataManager.h" 
 
 
 

 


/*=======================================================================================*/  

void ReadTrees(string fname, string ofile, int ipass, int maxevt)

/*=======================================================================================*/  

{ 
  
  
  
    ofstream out;
    stringstream ss;
    string fullfilename;
    ss<<fname;
    ss>>fullfilename; 
    
    TString inputfile = fullfilename; 
   
    
// check if the file exists
    ifstream fok(inputfile);
    if (!fok.good()) {
    cerr << "cannot find  file " << inputfile << endl;
    exit(1);
  }    
    cout <<"----->file "<<inputfile<<"  will be read "<<endl;
    
    //check  ipass value  
  
   switch (ipass) {
    
     case 0:  cout <<  "you select ipass=0:  only RunHeader content will be dumped \n";   break;
     case 1:  cout <<  "you select ipass=1:  histograms related to  events generation will be filled\n";   break;
     case 2:  cout <<  "you select ipass=2:  histograms related to  RawData will be filled\n";  break;
     case 11: cout << "you select ipass=11: histograms related to  Hit reconstruction  will be filled\n";   break;
     case 12: cout << "you select ipass=12: histograms related to 2DTrack reconstruction will be filled\n";  break;
     case 13: cout << "you select ipass=13: histograms for online monitoring will be filled\n";  break;
     default : {
        cout<<" the choosen value of ipass is not valid, please re-run with -h \n ";  
	return; }
    } 
     
   
//get filename from fullpath   
  string filename=fullfilename.substr(fullfilename.find_last_of("/\\") + 1); 
  int pos = filename.find_first_of('.');
  
   
   string aname=filename.substr(0,pos);
   string ftype=filename.substr(filename.length() - 4 ); 
  
 
   int itype;
   if (ftype=="root") {
    itype=1;
   } 
   else
   {
    itype=2;
    }
    
//  cout << "file: "<<filename<<" is of type "<<itype<<"  "<<ftype<<"  "<<ftype<<endl;
   TString thename=aname;
   
    
//Define output files
  
    
  ostringstream  passlevel;    
  passlevel << ipass;
  TString outfile;
  TString outrootname;

   if (ofile=="") {
    outfile = "bench_"+thename+"_pass"+passlevel.str()+".listing";  
    outrootname="bench_"+thename+"_pass"+passlevel.str()+".root";  
   } 
   else
   {
    outfile = ofile+".listing";
    outrootname=ofile+".root"; 
     } 
   
    
    out.open(outfile);
    string temp;
    temp=outfile;
    
    
    
    

//root     
//    TString outrootname="bench_"+thename+"_pass"+passlevel.str()+".root"; 
    if (ipass>0) fout = new TFile (outrootname,"recreate");
    
  
 
//select branches to analyse
    
    out<<"######################################################### \n\n ";  	
    out<<"      file  "<<inputfile<<"   will be analized \n \n ";  	 
    out<<"######################################################### \n\n ";  	     
    
    if (itype==2) {
      
      hraw_run= new TH1F("run number","run number",10000,-0.5,9999.5);
      hraw_seq= new TH1F("seq id","seq id",50,-0.5,49.5);
      hraw_nev= new TH1F("number of events","number of events",400,-0.5,399.5);
      hraw_nevsize= new TH1F("event size (MB) ","event size (MB)",50,0,10);
           
      
      size_t runpos = aname.find("-"); 
       
      int therun=atoi(aname.substr(0,runpos).c_str());
      int theseq=atoi(aname.substr(runpos+1).c_str());
      
      hraw_run->Fill(therun);
      hraw_seq->Fill(theseq);

      if (ipass>0) out<<" run "<< therun <<" sequence "<< theseq <<endl;  
      
     
      dlardaq::EventDecoder DaqDecoder( 1280, 1667 ); 
 
      DaqDecoder.Open(filename);
      size_t nev = DaqDecoder.GetTotEvents();   
      
      hraw_nev->Fill(nev);
      
      if (ipass>0) out<<" total number of events "<< nev <<endl;  
      
//set the number of events to be read 
//maxevt --> set by -e option (default =-1, read full tree)
//nev --> number of events in the input binary file
 
  
    int neve;
    if (maxevt==-1) neve=nev;
    if (maxevt>-1)   neve=maxevt;
    if (maxevt>nev) neve=nev;   
    
   if (ipass>0) out<<" ----> "<< neve<<" events will be analyzed "<<endl;  
    
      for(size_t i=0;i<neve;i++) {

      dlardaq::evheader_t evh; 
       std::vector<dlardaq::adc16_t> adc;
       DaqDecoder.GetEvent( i, evh, adc);
//       cout<<"Event number "<<evh.ev_num<<endl;
//       cout<<"Event size "<<evh.ev_size<<endl;
//      cout<<"Event quality flag "<<bitset<8>(evh.dq_flag)<<endl;
//        cout<<"Trigger number "<<evh.trig_info.num<<endl;
//        cout<<"Event timestamp:"<<evh.trig_info.ts.tv_sec <<" s "<<evh.trig_info.ts.tv_nsec<<" ns"<<endl;
 
       hraw_nevsize->Fill(evh.ev_size/(1024*1024));
      }  
      
     fout->cd();  
     hraw_run->Write();
     hraw_seq->Write();
     hraw_nev->Write();
     hraw_nevsize->Write();
//     fout->Write();
       }
     else
     {
  
    TFile *fin  = TFile::Open(inputfile); 

//   check  which trees and branches exist in input root file

    int iok=CheckFile(fin,out); 
    
    if (iok==0) 
     exit(1);
 
//Read and Dump RunHeader 
    
    DumpRH(fin,out); 
  
//Start reading  EventTree 

   TTree* theTree = (TTree*) fin->Get("EventTree"); 
   Long64_t nentries = theTree->GetEntries();  
   if (ipass>0) {
    out<<"  ==================   EVENT HEADER DUMP: =================================="<<endl; 
   
    out<<"Total number of events:  "<<nentries<<endl; 
  }

//set the number of events to be read 
//maxevt --> set by -e option (default =-1, read full tree)
//nenetries --> number of events in the EventTree
   
    int neve;
    if (maxevt==-1) neve=nentries;
    if (maxevt>-1)   neve=maxevt;
    if (maxevt>nentries) neve=nentries;   
    
   if (ipass>0) out<<" ----> "<< neve<<" events will be analyzed "<<endl; 
 
//branches
   
   EventHeader *evt_header = new EventHeader(); 
   theTree->SetBranchAddress("theEventHeader", &evt_header);  
   
   LArRawData::DataCRP *dat_crp = new LArRawData::DataCRP();  
   if (isCRPRawData) {
    theTree->SetBranchAddress("CRPRawData", &dat_crp); 
    }
   else
   {
     out <<"NO RAW DATA! "<<endl;
     } 
    
    
//hit 
 
      TObjArray *hits_V0 = new TObjArray();
      TObjArray *hits_V1 = new TObjArray();

     if (isHitView0) {
      theTree->SetBranchAddress("RecoHitsView0_Data",&hits_V0);
      }   
     if (isHitView1) {
      theTree->SetBranchAddress("RecoHitsView1_Data",&hits_V1);
      }   
      
//track
 
      TObjArray *trks_V0 = new TObjArray();
      TObjArray *trks_V1 = new TObjArray();

     if (isTrkView0) {
      theTree->SetBranchAddress("RecoCRTracks_View0",&trks_V0);
      }   
     if (isTrkView1) {
      theTree->SetBranchAddress("RecoCRTracks_View1",&trks_V1);
      }         
    if (ipass<1)  return;
    
    
    
    DefineHisto(ipass);

          
     for (int nn=0; nn<neve; nn++) {  
     theTree->GetEvent(nn);  
 
      if (nn%10==0) out<<" reading event " <<nn<<endl; 
//Read theEventHeader:     
      
     if (ipass>0 && ipass != 13) ReadEH(evt_header,out);
     if (ipass == 13) ReadEHonline(evt_header,out);

//Read CRPRawData:  
    if (ipass==2) { 
       if (!isCRPRawData) {
          out<<"NO RAW DATA "<<endl; 
	}
       if (isCRPRawData) ReadRawData(dat_crp,out);     
        
   }
   

//Hit reconstruction results:  

    if (ipass==11 || ipass==13 ){
       if (isHitView0 || isHitView1) ReadHits(hits_V0,hits_V1,evt_header,out);
//     if (isHitView0 || isHitView1) ReadHits(hits_V0,hits_V1, out);
      if ((!isHitView0) && (!isHitView1)) out<<"NO HITS INFORMATION AVAILABLE "<<endl;
   }
   
    if (ipass==12 || ipass==13) {
        if (isTrkView0 || isTrkView1)  ReadTrks(trks_V0,trks_V1,hits_V0,hits_V1,evt_header,out);   
       if ((!isTrkView0) && (!isTrkView1))  out<<"NO 2D Tracks INFORMATION AVAILABLE "<<endl;
   }
      } //loop on events   

 
//write output root file    

     fout->cd();  
     SaveHisto(ipass);
     fout->Write();
    
     
//     
 
     delete evt_header;
     delete dat_crp;
     delete hits_V0;
     delete hits_V1;
     delete trks_V0;
     delete trks_V1;
}   //if itype==1  
   

}

 

/*=======================================================================================*/  

void ReadTrks(TObjArray *trks_V0,TObjArray *trks_V1,TObjArray *hits_V0,TObjArray *hits_V1,EventHeader *evt_header, ofstream& out) 

/*=======================================================================================*/  

//read 2DTracks 
{

      int nsize=0;  
      vector<int>  ntrk0(crmnum),ntrk1(crmnum);
      for (int n=0; n<crmnum; n++) {
       ntrk0[n]=0;
       ntrk1[n]=0;
      }  
    
    for(int i=0;i<trks_V0->GetEntries();i++)
    {
      LArReco::Track2d* atrk = (LArReco::Track2d*)trks_V0->At(i);  
       ntrk0[atrk->GetCRM()]++;  
          
       hSlopeI_0[atrk->GetCRM()]->Fill(atrk->GetSlopeInit());
       hSlopeIErr_0[atrk->GetCRM()]->Fill(atrk->GetSlopeInitErr());
       hSlopeF_0[atrk->GetCRM()]->Fill(atrk->GetSlopeFinal());
       hSlopeFErr_0[atrk->GetCRM()]->Fill(atrk->GetSlopeFinalErr());
      
       vector<TVector2> apath   = atrk->GetPath();
       vector<Double_t> adq     =  atrk->GetDQ(); 

//get track points    

         nsize=apath.size();
        for (int np=0; np<nsize; np++) {
	   hpathX0[atrk->GetCRM()]->Fill(apath[np].X());
	   hpathY0[atrk->GetCRM()]->Fill(apath[np].Y());
 	 }
//get charge	

          nsize=adq.size();
	  for (int np=0; np<nsize; np++) {
	   hdq0[atrk->GetCRM()]->Fill(adq[np]);
	  }  

//get hits associated to the trk 
        vector<LArReco::RecoObjLink> atrkhits = atrk->GetHits();  
        int nhit=atrkhits.size(); 
	nhit_trk0[atrk->GetCRM()]->Fill(atrkhits.size());  
        for (int nh=0; nh<nhit; nh++) { 
	 LArReco::QHit *ahit = (LArReco::QHit*)hits_V0->At( atrkhits[nh].ObjIdx );
	  hcheckhitQ0[atrk->GetCRM()]->Fill(ahit->GetQ());
	  hcheckhitT0[atrk->GetCRM()]->Fill(ahit->GetT());
	  hcheckhitX0[atrk->GetCRM()]->Fill(ahit->GetX());
	 } 
	 
//get hits tagged as delta ray
  
        vector<LArReco::RecoObjLink> atrkdelta = atrk->GetDRays();
         int ndelta=atrkdelta.size();
 
	 for (int nd=0; nd<ndelta; nd++) {
           LArReco::QHit *adhit = (LArReco::QHit*)hits_V0->At( atrkdelta[nd].ObjIdx ); 
	   hdeltahitQ0[atrk->GetCRM()]->Fill(adhit->GetQ());
	   hdeltahitT0[atrk->GetCRM()]->Fill(adhit->GetT());
	   hdeltahitX0[atrk->GetCRM()]->Fill(adhit->GetX()); 
	 }  
         
	}  //for(int i=0;i<trks_V0->GetEntries();i++)
	
 
     
    for(int i=0;i<trks_V1->GetEntries();i++)
    {
      LArReco::Track2d* btrk = (LArReco::Track2d*)trks_V1->At(i);  
      ntrk1[btrk->GetCRM()]++;   
      
      
       hSlopeI_1[btrk->GetCRM()]->Fill(btrk->GetSlopeInit());
       hSlopeIErr_1[btrk->GetCRM()]->Fill(btrk->GetSlopeInitErr());
       hSlopeF_1[btrk->GetCRM()]->Fill(btrk->GetSlopeFinal());
       hSlopeFErr_1[btrk->GetCRM()]->Fill(btrk->GetSlopeFinalErr());
       
       vector<TVector2> bpath   = btrk->GetPath();
       vector<Double_t> bdq     =  btrk->GetDQ(); 

//get track points   
         nsize=bpath.size();
 
        for (int np=0; np<nsize; np++) {
	   hpathX1[btrk->GetCRM()]->Fill(bpath[np].X());
	   hpathY1[btrk->GetCRM()]->Fill(bpath[np].Y());
 	 }
//get charge	 
          nsize=bdq.size();
	  for (int np=0; np<nsize; np++) {
	   hdq1[btrk->GetCRM()]->Fill(bdq[np]);
	  }  

//get hits associated to the trk 
        vector<LArReco::RecoObjLink> btrkhits = btrk->GetHits();  
        int nhit=btrkhits.size(); 
	nhit_trk1[btrk->GetCRM()]->Fill(btrkhits.size());  
        for (int nh=0; nh<nhit; nh++) { 
	  LArReco::QHit *bhit = (LArReco::QHit*)hits_V1->At( btrkhits[nh].ObjIdx );
	  hcheckhitQ1[btrk->GetCRM()]->Fill(bhit->GetQ());
	  hcheckhitT1[btrk->GetCRM()]->Fill(bhit->GetT());
	  hcheckhitX1[btrk->GetCRM()]->Fill(bhit->GetX());
	 } 
	 
//get hits tagged as delta ray
  
        vector<LArReco::RecoObjLink> btrkdelta = btrk->GetDRays();
         int ndelta=btrkdelta.size();
 
	 for (int nd=0; nd<ndelta; nd++) {
           LArReco::QHit *bdhit = (LArReco::QHit*)hits_V1->At( btrkdelta[nd].ObjIdx ); 
	   hdeltahitQ1[btrk->GetCRM()]->Fill(bdhit->GetQ());
	   hdeltahitT1[btrk->GetCRM()]->Fill(bdhit->GetT());
	   hdeltahitX1[btrk->GetCRM()]->Fill(bdhit->GetX()); 
	 }  
         
	}  //for(int i=0;i<trks_V1->GetEntries();i++)
	
       
   
     for (int n=0; n<crmnum; n++) {
      trknumber0[n]->Fill(ntrk0[n]);  
      trknumber1[n]->Fill(ntrk1[n]);  
     }  
        
 
      
} 
/*=======================================================================================*/  

void ReadHits(TObjArray *hits_V0,TObjArray *hits_V1,EventHeader *evt_header, ofstream& out) 

/*=======================================================================================*/  

//read hits
 
{
      int currcrm;
      int currcha,nhitcha;
      float totchastrip;

 

	 for (int n=0; n<crmnum; n++) {
 	  hitnumber0[n]->Fill(evt_header->GetNumRecHit0(n));
 	  hitnumber1[n]->Fill(evt_header->GetNumRecHit1(n));   
     } 
     
   
   
  
       
      for(int i=0;i<hits_V0->GetEntries();i++)
      { 
       LArReco::QHit* ahit = (LArReco::QHit*)hits_V0->At(i);  
       if (i==0 ) {
        currcrm=ahit->GetCRM();
        currcha=ahit->GetCh();
        totchastrip=0;
        nhitcha=0;
       }
        hhitcharge0[ahit->GetCRM()]->Fill(ahit->GetQ());
        hittedwire0[ahit->GetCRM()]->Fill(ahit->GetCh());  
        hittedwire0T0[ahit->GetCRM()]->Fill(ahit->GetT());
	if (ahit->GetCRM()==currcrm && ahit->GetCh()==currcha  ) {
	   nhitcha++;
	   totchastrip+=ahit->GetQ();
	} 
	  if (ahit->GetCRM()> currcrm || ahit->GetCh()>currcha  ) {
	   hitperwire0[ahit->GetCRM()]->Fill(nhitcha);
	   htotreccharge0[ahit->GetCRM()]->Fill(totchastrip);
	   totchastrip=0;
	   totchastrip=ahit->GetQ();
	   nhitcha=1;
	   currcha=ahit->GetCh();   
	   currcrm=ahit->GetCRM();
	} 
       } 
    
    
     for(int i=0;i<hits_V1->GetEntries();i++)
     {
       LArReco::QHit* ahit = (LArReco::QHit*)hits_V1->At(i); 
       if (i==0 ) {
        currcrm=ahit->GetCRM();
        currcha=ahit->GetCh();
        totchastrip=0;
        nhitcha=0;
       }
       hhitcharge1[ahit->GetCRM()]->Fill(ahit->GetQ());
       hittedwire1[ahit->GetCRM()]->Fill(ahit->GetCh());  
       hittedwire1T0[ahit->GetCRM()]->Fill(ahit->GetT());
	if (ahit->GetCRM()==currcrm && ahit->GetCh()==currcha  ) {
	   nhitcha++;
	   totchastrip+=ahit->GetQ();
	} 
	   if (ahit->GetCRM()> currcrm || ahit->GetCh()>currcha  ) {
	   hitperwire1[ahit->GetCRM()]->Fill(nhitcha);
	   htotreccharge1[ahit->GetCRM()]->Fill(totchastrip);
	   totchastrip=0;
	   totchastrip=ahit->GetQ();
	   nhitcha=1;
	   currcha=ahit->GetCh();  
	   currcrm=ahit->GetCRM(); 
	} 
       
    }   
    
 }   



/*=======================================================================================*/  

void ReadRawData(LArRawData::DataCRP *dat_crp, ofstream& out)

/*=======================================================================================*/  

//read RawData
{


  UInt_t avalue; 
  float totcha0,totcha1;
  int chnum;
  float pedcut=pedvalue; 
  float toadd;
  
   
      TIter next_crm(dat_crp->GetCRMs());
      LArRawData::DataCRM *crm;
      
      while ( (crm = (LArRawData::DataCRM*) next_crm()) ) //loop over CRMs
      {
    
	 
         LArRawData::DataChan *ch = NULL;
	 
	 totcha0=0;
	 totcha1=0;
 
// process channels in view 0
       TIter next_chv0(crm->GetChanView0()); //
       while ( (ch = (LArRawData::DataChan*)(next_chv0())) )
	{  
	 chnum=ch->GetChanId(); 
	  TIter next(ch->GetDataFragments()); 
	  TObject *obj;
	  while( (obj = next()) )
	    {
	      if(ch->GetDataType() == LArRawData::DTID_USHORT)
		{  		
		  totcha0=0;
		  LArRawData::DataFragment<UShort_t> *dFrag = (LArRawData::DataFragment<UShort_t>*)obj;  
                   for(UInt_t i=0;i<nsample_CRM[crm->GetCRMId()];i++) {  
                      avalue=dFrag->GetDatum(i);  
		       if (avalue>pedcut)  {
		         toadd=avalue-pedcut; 
		         totcha0+=toadd;   
			  rawdatawire0[crm->GetCRMId()]->Fill(chnum); 
			  hintegral0[crm->GetCRMId()]->Fill(toadd*pitchT*AdcToCharge);   
			 
		       }
		    } //for 
		}   //if 
		
             if (totcha0>0) htotcharge0[crm->GetCRMId()]->Fill((totcha0*pitchT)* AdcToCharge);     
		
	    } //while ( (obj = next()) ) 
 
	} // while ( (ch = (LArRawData::DataChan*)(next_chv0())) )
	
	 
// process channels in view 1
      TIter next_chv1(crm->GetChanView1()); //
      while ( (ch = (LArRawData::DataChan*)(next_chv1())) )
	{  
	  chnum=ch->GetChanId(); 	 
	  TIter next(ch->GetDataFragments()); 
	  TObject *obj;
	  while( (obj = next()) )
	    {
	      if(ch->GetDataType() == LArRawData::DTID_USHORT)
		{ 
		  totcha1=0;
		  LArRawData::DataFragment<UShort_t> *dFrag = (LArRawData::DataFragment<UShort_t>*)obj;  
                   for(UInt_t i=0;i<nsample_CRM[crm->GetCRMId()];i++) {  
                      avalue=dFrag->GetDatum(i);  
		       if (avalue>pedcut)  {
		         toadd=avalue-pedcut; 
		         totcha1+=toadd;  
			   hintegral1[crm->GetCRMId()]->Fill(toadd*pitchT*AdcToCharge);    
			   rawdatawire1[crm->GetCRMId()]->Fill(chnum); 
			   
		       }
		    } //for 
		}   //if 
		
		 if (totcha1>0) htotcharge1[crm->GetCRMId()]->Fill((totcha1*pitchT)* AdcToCharge);   
		
		
	    } //while ( (obj = next()) )
 
	} // while ( (ch = (LArRawData::DataChan*)(next_chv1())) ) 
		   
		  
       } //loop CRM   
       
   
    }



/*=======================================================================================*/  

void ReadEH(EventHeader *evt_header,ofstream& out )

/*=======================================================================================*/  

//read theEventHeader


 {
 
//event generation
     
         heve->Fill(evt_header->GetEventNumber());
	 for (int ii=0; ii<3; ii++) {
         hmomemtumc->Fill(evt_header->GetTrueMomentum(ii));} 
         hxcoord->Fill(evt_header->GetTrueProdX());
         hycoord->Fill(evt_header->GetTrueProdY());
         hzcoord->Fill(evt_header->GetTrueProdZ());
	 hmomemtum->Fill(evt_header->GetTrueP());
	 htheta->Fill(evt_header->GetTrueTheta());
	 hphi->Fill(evt_header->GetTruePhi());
	 hene->Fill(evt_header->GetTrueTotDepEne());  
         heneq->Fill(evt_header->GetTrueTotDepEneQ());
	 hemene->Fill(evt_header->GetTrueTotDepEEne()); 
	 hemeneq->Fill(evt_header->GetTrueTotDepEEneQ());
         hhene->Fill(evt_header->GetTrueTotDepHEne());   
         hheneq->Fill(evt_header->GetTrueTotDepHEneQ());
         hlost->Fill(evt_header->GetTrueEneLost());  
	 heneCRT1->Fill(evt_header->GetTrueTotDepEneCRT1());
	 heneCRT2->Fill(evt_header->GetTrueTotDepEneCRT2());
 }  
     

/*=======================================================================================*/  

void ReadEHonline(EventHeader *evt_header,ofstream& out )

/*=======================================================================================*/  

//read theEventHeader


 {
 
//event generation
     
         heve->Fill(evt_header->GetEventNumber());
}	
     
 
/*=======================================================================================*/  
 
void   DumpRH(TFile *file, ofstream& outfile)  

/*=======================================================================================*/  

  
//
 {

   TTree* theTreeRH = (TTree*) file->Get("RunHeader"); 
   RunHeader *run_header = new RunHeader();  
   
// read theRunHeader  
   theTreeRH->SetBranchAddress("theRunHeader", &run_header); 
   theTreeRH->GetEvent(0);
  
   time_t now = run_header->GetRunTime();
   
// convert now to string form
   char* dt = ctime(&now);
 
  outfile <<"  =================== RUN HEADER DUMP: "<<endl<<endl; 
  outfile << " --> file produced on: " << dt << endl; 
  outfile << " --> run Number "<<run_header->GetRunNumber()<<endl<<endl;
  
   outfile <<" --> Detector specification:"<< endl;  
   outfile <<"     Configuration File  "<<run_header->GetConfigFile()<<endl;
  
   int igeo=run_header->GetGeometry(); 
   if ((igeo/1000 % 10) ==6) 
   outfile <<"     Geometry:  6x6x6,  "<<igeo%10<<" CRM" <<endl;
   
   if ((igeo/1000 % 10) ==3)  
   outfile <<"     Geometry   3x1x1   "<<igeo%10<<" CRM" <<endl;
  
   outfile <<"     purity(ms):  "<<run_header->GetLifeTime()<<endl<<endl;  
   
   outfile <<"     longitudinal diffusion (L)  "<<run_header->GetDiffusionL()<<endl;  
   outfile <<"     transerse diffusion (T)     " <<run_header->GetDiffusionT()<<endl<<endl;  
  
   outfile <<" --> Electric field configuration: "<<endl; 
    
    if (run_header->GetEfieldMap()=="UNIFORM") 
    {
      outfile <<"     uniform EField, value    "<<run_header->GetEfieldValue()<<"  (V/cm)"<<endl<<endl; 
    }
    else
    {
      outfile <<"     selected field map: "<<run_header->GetEfieldMap()<<endl;
      outfile <<"      nominal EField value "<< run_header->GetEfieldValue()<<"  (V/cm)"<<endl<<endl;
    }
    
     if (run_header->GetLMapLAR()=="") 
      outfile <<" --> No Light Simulation "<<endl<<endl; 
     if (run_header->GetLMapLAR()!="")
     { 
       outfile <<" --> Light Simulation "<<endl; 
       outfile <<"     Light map   "<<run_header->GetLMapLAR()<<endl;
       outfile <<"                 "<<run_header->GetLMapGAR()<<endl;
       outfile <<"     Sampling(ns): "<<run_header->GetLSTime()<<endl<<endl; 
      }
   
  
   outfile <<" --> Particle generation: "<<endl; 
   outfile <<"     initial seed "<<run_header->GetStartSeed()<<endl; 
   
   int idt=run_header->GetDataType();
 
   outfile <<"     input file selection:" ;
 
   switch (idt) 
   {
      case 1:{
       int ipart=run_header->GetBeamType(); 
       if (ipart==3) outfile<<"  e- ,  gun mode"<<endl<<endl;
       if (ipart==5) outfile<<"  mu+,  gun mode"<<endl<<endl;
       if (ipart==6) outfile<<"  mu-,  gun mode"<<endl<<endl;
       if (ipart==8) outfile<<"  pi+,  gun mode"<<endl<<endl;
       if (ipart==9) outfile<<"  pi-,  gun mode"<<endl<<endl;  
       break;
       }
      case 2: {
        outfile <<"   GENIE neutrino events  "<<run_header->GetGENIEfn()<<endl; 
	outfile <<" Reinteraction "<<run_header->GetReinteraction() <<endl<<endl; 
	break;
	}
      case 3: outfile <<"   GENIE nucleon decay events  "<<run_header->GetGENIEfn()<< endl<<endl; break;
      case 4: {
       outfile <<" events with Cosmic backgroud "<<endl; 
       outfile <<"       beam Particle "<<run_header->GetCRBeam()<<endl;
       outfile <<"       CRY input file "<<run_header->GetCRCRY()<<endl;
       outfile <<"       start time and duration  "<<run_header->GetCRTO()<<"  "<<run_header->GetCRWIN()<<endl;
       outfile <<"       event number from CRY file "<<run_header->GetCREVNT()<<endl;
       outfile <<"       rate  "<<run_header->GetCRRATE()<<endl<<endl;;
       break;
      } 
   }
   
   
   int tcd=run_header->GetTransportCode();
   
   switch (tcd) 
   {
     case 3:  outfile <<"     Geant 3 has been selected "<<endl<<endl; break;
     case 4:  outfile <<"     Geant 4 has been selected "<<endl<<endl; break;
   }  
      

// read theGeomConfig
 
   GeomConfig *geo_config= new GeomConfig(); 
   theTreeRH->SetBranchAddress("theGeomConfig", &geo_config);  
   theTreeRH->GetEvent(0);
    
   
    outfile <<" ========== GEOMETRY CONFIGURATION: "<< run_header->GetConfigFile()<<endl<<endl;
    outfile <<"    DETECTOR "<< geo_config->GetGC_GeomType()<<endl;
    outfile <<"    RAWDATATYPE  "<<geo_config->GetGC_RDType()<<"  "<<geo_config->GetGC_quanta()<<endl;
    outfile <<"    ELECMODEL "<<geo_config->GetGC_ElecModel()<<"  "<<geo_config->GetGC_Ped()<<"  "<<geo_config->GetGC_maxbit()<<endl;
    outfile <<"    CRM number "<<geo_config->GetCRMnum()<<endl; 
    
    pedvalue=geo_config->GetGC_Ped();
    crmnum=geo_config->GetCRMnum();
    
    for (int nn=0; nn<geo_config->GetCRMnum(); nn++) {
    
     outfile <<"      CRM "<<nn<<"  "<<geo_config->GetCRMcX(nn)<<"  "<<geo_config->GetCRMcY(nn)<<"  "<<geo_config->GetCRMcZ(nn)<<"  "<< geo_config->GetCRMxdim(nn);
     outfile<<"  "<<geo_config->GetCRMydim(nn)<<"  "<<geo_config->GetCRMgain(nn)<<"  "<<geo_config->GetCRMpitchX(nn)<<" "<<geo_config->GetCRMpitchY(nn);
     outfile <<"  "<<geo_config->GetCRMnchaX(nn)<<"  "<<geo_config->GetCRMnchaY(nn)<<"  "<<geo_config->GetCRMnsample(nn)<<endl;;

     x_CRM[nn]=geo_config->GetCRMcX(nn);
     y_CRM[nn]=geo_config->GetCRMcY(nn);
     z_CRM[nn]=geo_config->GetCRMcZ(nn);
     DIMx_CRM[nn]=geo_config->GetCRMxdim(nn);
     DIMy_CRM[nn]=geo_config->GetCRMydim(nn); 
     pitchx_CRM[nn]=geo_config->GetCRMpitchX(nn);
     pitchy_CRM[nn]=geo_config->GetCRMpitchY(nn);
     nchax_CRM[nn]=geo_config->GetCRMnchaX(nn);
     nchay_CRM[nn]=geo_config->GetCRMnchaY(nn);
     nsample_CRM[nn]=geo_config->GetCRMnsample(nn);
    }  
  
     pitchT=geo_config->GetCRMpitchT();
  
     outfile<<"    time sampling "<<pitchT<<endl;
  
     outfile <<"    NOISE "<< geo_config->GetGC_NoiseName()<<"  "<<geo_config->GetGC_NoiseFreq() <<"  "<<geo_config->GetGC_NoiseRMS()<<"  "<<geo_config->GetGC_cohe()<<endl<<endl;
    
     if (istheHitReco) {
       
       RecoConfig *hreco_config= new RecoConfig(); 
       theTreeRH->SetBranchAddress("theRecoConfig_HIT", &hreco_config);  
       theTreeRH->GetEvent(0);
  
       outfile<<"============================  HIT RECONSTRUCTION ==================================="<<endl<<endl; 
 
  
       outfile<<" input file: "<<hreco_config->GetInFileName()<<endl; 
       outfile<<" Algorithm selected  "<<hreco_config->GetAlgoName()<<endl;
       
       
       time_t hitnow = hreco_config->GetRunTime(); 
// convert now to string form 
       char* dthit = ctime(&hitnow);
       
       outfile<<" hit reconstruction run on :"<<dthit<<endl; 
      
 
      vector<float> par_htirec =hreco_config->GetValue("PARA_HIT");
      outfile<<"Hit reconstruction parameters: (please see rectasks config file for comments)"<<endl;
      for (int n=0; n<int(par_htirec.size()); n++){
       outfile<<"  "<<par_htirec[n];
      } 
      outfile<<"  "<<endl;
      vector<float> par_roi =hreco_config->GetValue("PARA_ROI");
      outfile<<"ROI definition   "<<endl;
      for (int n=0; n<int(par_roi.size()); n++){
      outfile<<"  "<<par_roi[n];
      } 
      outfile<<"  "<<endl;
      
      
      delete hreco_config;
 
   }
   
       if (istheTrkReco) {
       
       RecoConfig *treco_config= new RecoConfig(); 
       theTreeRH->SetBranchAddress("theRecoConfig_CRTRACK", &treco_config);  
       theTreeRH->GetEvent(0);
  
       outfile<<"============================  TRACKING  ==================================="<<endl<<endl; 
 
  
       outfile<<" input file: "<<treco_config->GetInFileName()<<endl; 
       outfile<<" Algorithm selected  "<<treco_config->GetAlgoName()<<endl;
       
       
       time_t trknow = treco_config->GetRunTime(); 
// convert now to string form 
       char* dttrk = ctime(&trknow);
       
       outfile<<" track reconstruction run on :"<<dttrk<<endl; 
      
 
      vector<float> par_atrkrec =treco_config->GetValue("PARA_CLUS");
      outfile<<"Track reconstruction parameters: (please see rectasks config file for comments)"<<endl;
      for (int n=0; n<int(par_atrkrec.size()); n++){
       outfile<<"minimum number of hits in cluster:  "<<par_atrkrec[n];
      } 
       outfile<<endl;
       vector<float> par_mtrkrec =treco_config->GetValue("PARA_MULTITRK");
       outfile<<" PARA_MULTITRK: ";
       for (int n=0; n<int(par_mtrkrec.size()); n++){
       outfile<<"  "<<par_mtrkrec[n];
       }
       outfile<<endl;
       vector<float> par_ttrkrec =treco_config->GetValue("PARA_TRKBUILD");
       outfile<<" PARA_TRKBUILD: ";
       for (int n=0; n<int(par_ttrkrec.size()); n++){
       outfile<<"  "<<par_ttrkrec[n];
       }
       outfile<<endl;
       
      delete treco_config;
 
   }
   
       delete run_header;
       delete geo_config;
        
  
}
 
/*=======================================================================================*/  
  
int  CheckFile(TFile *file, ofstream& outfile)  

/*=======================================================================================*/  
 
 {
//to check if RunHeader and EventHeader exist, and verify which branches are available
  
     isRunHeader=file->GetListOfKeys()->Contains("RunHeader");  
     isGeom=file->GetListOfKeys()->Contains("TGeo");  
     isEventTree=file->GetListOfKeys()->Contains("EventTree");  
   
     if (isRunHeader) {
         TTree* theTree = (TTree*) file->Get("RunHeader"); 
	 istheRunHeader=theTree->GetBranch("theRunHeader");
	 istheGeomConfig=theTree->GetBranch("theGeomConfig");
	 istheBeamInfo=theTree->GetBranch("theBeamInfo"); 
	 istheHitReco=theTree->GetBranch("theRecoConfig_HIT"); 
	 istheTrkReco=theTree->GetBranch("theRecoConfig_CRTRACK"); 
	} 
	
     if (isEventTree) {
         TTree* theTree = (TTree*) file->Get("EventTree"); 
	 istheEventHeader=theTree->GetBranch("theEventHeader");
	 isCRPRawData=theTree->GetBranch("CRPRawData"); 
	 isCRPVoxelData=theTree->GetBranch("CRPVoxelData"); 
	 isLRORawData=theTree->GetBranch("LRORawData");  
	 isHitView0=theTree->GetBranch("RecoHitsView0_Data");   
         isHitView1=theTree->GetBranch("RecoHitsView1_Data");   
	 isTrkView0=theTree->GetBranch("RecoCRTracks_View0");   
         isTrkView1=theTree->GetBranch("RecoCRTracks_View1");   
        }
    
    
     if (!isRunHeader) { 
       cout <<"RUN HEADER IS MISSING.... exit "<<endl;
       outfile <<"RUN HEADER IS MISSING.... exit "<<endl;
       int iout=0; 
       return(iout);
      }
     
     if (!isEventTree) { 
       cout <<"EVENT TREE   IS MISSING.... exit "<<endl;
       outfile <<"EVENT TREE IS MISSING.... exit "<<endl;
       int iout=0; 
       return(iout);
      }    
      
  
     if (!istheEventHeader) {
       cout <<"EVENT HEADER    IS MISSING.... exit "<<endl;
       outfile <<"EVENT HEADER IS MISSING.... exit "<<endl;
       int iout=0; 
       return(iout);
      }    
  
  
      int iout=1;
     
      outfile<<"Folders, Trees, branches available: "<<endl;
      outfile<<"  TGeo  "<<isGeom<<endl;  
      outfile<<"  RunHeader "<< isRunHeader<<endl;
      outfile<<"     -->theRunHeader  "<< istheRunHeader<<endl; 
      outfile<<"     -->theGeomConfig "<< istheGeomConfig<<endl; 
      outfile<<"     -->theBeamInfo "<< istheBeamInfo<<endl;  
      outfile<<"     -->theHitReco "<< istheHitReco<<endl;  
      outfile<<"  EventTree "<<isEventTree<<endl;   
      outfile<<"     -->theEventHeader " <<istheEventHeader<<endl;
      outfile<<"     -->CRPRawData  "<<isCRPRawData<<endl;
      outfile<<"     -->CRPVoxelData "<<isCRPVoxelData<<endl; 
      outfile<<"     -->LRORawData "<<isLRORawData<<endl;
      outfile<<"     -->Hits  "<<isHitView0<<"  "<<isHitView1<<endl;
      outfile<<"     -->Tracks "<<isTrkView0<<"  "<<isTrkView1<<endl<<endl<<endl;
 
     
     return(iout);
    
 }  

/*=======================================================================================*/  
 
void PrintHelp(void)

/*=======================================================================================*/  


{
   cout  <<"analysis program to read root files produced with Qscan and root files obtained running  reconstruction (recotask) "<<endl;
   cout  <<"Syntax: "<<endl;
   cout <<"           bench.exe -f afile.root -p ipass [-e nev] [-o out.root]"<<endl<<endl<<endl;  
   cout <<"  Options : "<<endl;
   cout <<"              -f input filename : afile.root "<<endl;
   cout<<"               -p ipass is an integer, to select  different levels of analysis "<<endl;
   cout<<"                  ipass=0 only a dump of the run header is produced   "<<endl;
   cout<<"                  ipass=1 histograms related to event generation are filled "<<endl;
   cout<<"                  ipass=2 histograms related to RawData are produced "<<endl;
   cout<<"                  ipass=11 histograms related to Hit Reconstruction are filled "<<endl;
   cout<<"                  ipass=12 histograms related fo 2DTrack Reconstruction are filled  "<<endl;    
   cout <<"              [] denotes an optional argument "<<endl<<endl; 
   cout <<"                -e :  number of events to process "<<endl;
   cout <<"                      (optional, default: process all events) "<<endl<<endl; 
   cout <<"                -o     specify name of output files (without extension) "<<endl<<endl; 
   cout <<" 2 output files are obtained : (example if  filename for outpus files is not specified)"<<endl;
   cout <<"    1) bench_afile_ipass.root: histograms file (only if pass>0)"<<endl;
   cout <<"    2) bench_afile_ipass.listing  dump of RunHeader and of some events info "<<endl<<endl; 
  
   
   
}           

/*=======================================================================================*/  

void DefineHisto(int ipass)  

/*=======================================================================================*/  

{
 char name[256];

if (ipass>0) {
   heve= new TH1F("event","event",1000,0.,1000);
   if (ipass != 13 ) {
    hmomemtumc= new TH1F("momentum_component","momentum_component",10,-0.5,9.5);
    hmomemtum= new TH1F("momentum","momentum",10,-0.5,9.5);
    htheta= new TH1F("theta","theta",50,0.,4.);
    hphi= new TH1F("hphi","hphi",50,0.,3.14); 
    hxcoord= new TH1F("Xcoord","Xcoord",700,-350.,350);
    hycoord= new TH1F("Ycoord","Ycoord",700,-350.,350);
    hzcoord= new TH1F("Zcoord","Zcoord",700,-350.,350);  
    hene=  new TH1F("ene","ene",300,0.,3000);  
    heneq=  new TH1F("eneq","eneq",300,0.,3000);  
    hemene= new TH1F("emene","emene",300,0.,3000); 
    hemeneq= new TH1F("emeneq","emeneq",300,0.,3000); 
    hhene= new TH1F("hene","hene",300,0.,3000); 
    hheneq=  new TH1F("heneq","heneq",300,0.,3000);  
    hlost= new TH1F("elost","elost",1000,0.,10000);
    heneCRT1=  new TH1F("eneCRT1","eneCRT1",300,0.,3000);
    heneCRT2=  new TH1F("eneCRT2","eneCRT2",300,0.,3000);
   }
 }     
   
 if (ipass==2){   
   for (int nn=0; nn<crmnum; nn++) {
       sprintf(name,"hittedwire#_v0_CRM%d",nn);
       rawdatawire0[nn]=new TH1F(name,name,1001,-.5,1000.5);  
       sprintf(name,"hittedwire#_v1_CRM%d",nn);
       rawdatawire1[nn]=new TH1F(name,name,1001,-.5,1000.5);  
       sprintf(name,"single_charge_deposition_v0_CRM%d",nn); 
       hintegral0[nn]= new TH1F(name,name,200,0.,100.);
       sprintf(name,"total_charge_perstrip_v0_CRM%d",nn);  
       htotcharge0[nn]= new TH1F(name,name,400,0.,200.); 
       sprintf(name,"single_charge_deposition_v1_CRM%d",nn); 
       hintegral1[nn]= new TH1F(name,name,200,0.,100.);
       sprintf(name,"total_charge_perstrip_v1_CRM%d",nn);  
       htotcharge1[nn]= new TH1F(name,name,400,0.,200.);  
   }
   

  }  
  
 if (ipass==11 || ipass==13) {
     for (int nn=0; nn<crmnum; nn++) {  
      sprintf(name,"numberofhits_v0_CRM%d",nn); 
      hitnumber0[nn]=new TH1F(name,name,2001,-0.5,2000.5);   
      sprintf(name,"hittedwire#_v0_CRM%d",nn); 
      hittedwire0[nn]=new TH1F(name,name,1001,-.5,1000.5);  
      sprintf(name,"hitsperwire_v0_CRM%d",nn); 
      hitperwire0[nn]=new TH1F(name,name,11,-.5, 10.5); 
      sprintf(name,"hitcharge_v0_CRM%d",nn);  
      hhitcharge0[nn]=new TH1F(name,name,200,0.,100.);   
      sprintf(name,"hitt0_v0_CRM%d",nn); 
      hittedwire0T0[nn]=new TH1F(name,name,1000,0,4000);
      sprintf(name,"tothitcharge_v0_CRM%d",nn); 
       htotreccharge0[nn]=new TH1F(name,name,400,0.,200.);  
      sprintf(name,"numberofhits_v1_CRM%d",nn); 
      hitnumber1[nn]=new TH1F(name,name,2001,-0.5,2000.5);    
      sprintf(name,"hittedwire#_v1_CRM%d",nn); 
      hittedwire1[nn]=new TH1F(name,name,1001,-.5,1000.5);  
      sprintf(name,"hitsperwire_v1_CRM%d",nn); 
      hitperwire1[nn]=new TH1F(name,name,11,-.5, 10.5); 
      sprintf(name,"hitcharge_v1_CRM%d",nn); 
      hhitcharge1[nn]=new TH1F(name,name,200,0.,100.);   
       sprintf(name,"hitt0_v1_CRM%d",nn); 
      hittedwire1T0[nn]=new TH1F(name,name,1000,0,4000);
      sprintf(name,"tothitcharge_v1_CRM%d",nn); 
      htotreccharge1[nn]=new TH1F(name,name,400,0.,200.);    
     }
 }

 if (ipass==12 || ipass==13) {
      for (int nn=0; nn<crmnum; nn++) {
       sprintf(name,"numberof2Dtracks_v0_CRM%d",nn); 
       trknumber0[nn]=new TH1F(name,name,21,-0.5,20.5);  
       sprintf(name,"numberof2Dtracks_v1_CRM%d",nn);  
       trknumber1[nn]=new TH1F(name,name,21,-0.5,20.5);  
       
       sprintf(name,"track_initial_slope_v0_CRM%d",nn);  
       hSlopeI_0[nn]=new TH1F(name,name,100,-3,3);  
       sprintf(name,"track_initial_slopeError_v0_CRM%d",nn);  
       hSlopeIErr_0[nn]=new TH1F(name,name,100,-3,3);
       sprintf(name,"track_final_slope_v0_CRM%d",nn);  
       hSlopeF_0[nn]=new TH1F(name,name,100,-3,3); 
       sprintf(name,"track_final_slopeError_v0_CRM%d",nn);   
       hSlopeFErr_0[nn]=new TH1F(name,name,100,-3,3); 
       sprintf(name,"numberofhit/trk_v0_CRM%d",nn); 
       nhit_trk0[nn]=new TH1F(name,name,100,-0.5, 1999.5);
       sprintf(name,"numberofhit/trk_v1_CRM%d",nn); 
       nhit_trk1[nn]=new TH1F(name,name,100,-0.5, 1999.5);
      
       sprintf(name,"trkpoint_x_v0_CRM%d",nn); 
       hpathX0[nn]=new TH1F(name,name,600,-300,300);
       sprintf(name,"trkpoint_z_v0_CRM%d",nn); 
       hpathY0[nn]=new TH1F(name,name,310,0,620);
       sprintf(name,"trkpoint_charge_v0_CRM%d",nn); 
       hdq0[nn]=new TH1F(name,name,400,0,200);
       sprintf(name,"trk_hitcharge_v0_CRM%d",nn); 
       hcheckhitQ0[nn]=new TH1F(name,name,400,0,200);
       sprintf(name,"trk_hitt0_v0_CRM%d",nn); 
       hcheckhitT0[nn]=new TH1F(name,name,1000,0,4000);
       sprintf(name,"trk_hitx_v0_CRM%d",nn); 
       hcheckhitX0[nn]=new TH1F(name,name,1800,-300,300);  
       sprintf(name,"trk_deltahitcharge_v0_CRM%d",nn); 
       hdeltahitQ0[nn]=new TH1F(name,name,400,0,200);
       sprintf(name,"trk_deltahitt0_v0_CRM%d",nn); 
       hdeltahitT0[nn]=new TH1F(name,name,1000,0,4000);
       sprintf(name,"trk_deltahitx_v0_CRM%d",nn); 
       hdeltahitX0[nn]=new TH1F(name,name,1800,-300,300); 
       
       sprintf(name,"track_initial_slope_v1_CRM%d",nn);  
       hSlopeI_1[nn]=new TH1F(name,name,100,-3,3);  
       sprintf(name,"track_initial_slopeError_v1_CRM%d",nn);  
       hSlopeIErr_1[nn]=new TH1F(name,name,100,-3,3);
       sprintf(name,"track_final_slope_v1_CRM%d",nn);  
       hSlopeF_1[nn]=new TH1F(name,name,100,-3,3); 
       sprintf(name,"track_final_slopeError_v1_CRM%d",nn);   
       hSlopeFErr_1[nn]=new TH1F(name,name,100,-3,3); 
       sprintf(name,"trkpoint_x_v1_CRM%d",nn); 
       hpathX1[nn]=new TH1F(name,name,600,-300,300);
       sprintf(name,"trkpoint_z_v1_CRM%d",nn); 
       hpathY1[nn]=new TH1F(name,name,310,0,620);
       sprintf(name,"trkpoint_charge_v1_CRM%d",nn); 
       hdq1[nn]=new TH1F(name,name,400,0,200);
       sprintf(name,"trk_hitcharge_v1_CRM%d",nn); 
       hcheckhitQ1[nn]=new TH1F(name,name,400,0,200);
       sprintf(name,"trk_hitt0_v1_CRM%d",nn); 
       hcheckhitT1[nn]=new TH1F(name,name,1000,0,4000);
       sprintf(name,"trk_hitx_v1_CRM%d",nn); 
       hcheckhitX1[nn]=new TH1F(name,name,1800,-300,300);  
       sprintf(name,"trk_deltahitcharge_v1_CRM%d",nn); 
       hdeltahitQ1[nn]=new TH1F(name,name,400,0,200);
       sprintf(name,"trk_deltahitt0_v1_CRM%d",nn); 
       hdeltahitT1[nn]=new TH1F(name,name,1000,0,4000);
       sprintf(name,"trk_deltahitx_v1_CRM%d",nn); 
       hdeltahitX1[nn]=new TH1F(name,name,1800,-300,300); 
       
      }
   }   
 }     

/*=======================================================================================*/  

void SaveHisto(int ipass)  

/*=======================================================================================*/  

{

  if (ipass>0) {
    heve->Write();
    if (ipass != 13) {   
      hmomemtumc->Write(); 
      hxcoord->Write();
      hycoord->Write();
      hzcoord->Write();
      hmomemtum->Write();
      htheta->Write();
      hphi->Write();
      hene->Write();
      heneq->Write();
      hemene->Write();      
      hemeneq->Write();
      hhene->Write();   
      hheneq->Write();
      hlost ->Write();
      heneCRT1->Write();
      heneCRT2->Write();
    }  
    
  }     

  if (ipass==2) {   
    for (int nn=0; nn<crmnum; nn++) {
    
      rawdatawire0[nn]->Write();
      rawdatawire1[nn]->Write();
      hintegral0[nn]->Write();
      hintegral1[nn]->Write();
      htotcharge0[nn]->Write();
      htotcharge1[nn]->Write();  
     } 
    
       
  }
  
  if (ipass==11 || ipass==13 ) {
    for (int nn=0; nn<crmnum; nn++) {
      hitnumber0[nn]->Write(); 
      hitnumber1[nn]->Write(); 
      hittedwire0[nn]->Write(); 
      hittedwire1[nn]->Write(); 
      hitperwire0[nn]->Write();
      hitperwire1[nn]->Write();
      hhitcharge0[nn]->Write();
      hhitcharge1[nn]->Write();
      hittedwire0T0[nn]->Write();
      hittedwire1T0[nn]->Write();
      htotreccharge0[nn]->Write();
      htotreccharge1[nn]->Write();
    }
  }
  if (ipass==12 || ipass==13) {
    for (int nn=0; nn<crmnum; nn++) {
     trknumber0[nn]->Write();   
     trknumber1[nn]->Write();
     hSlopeI_0[nn]->Write();
     hSlopeIErr_0[nn]->Write();
     hSlopeF_0[nn]->Write();
     hSlopeFErr_0[nn]->Write();  
     hpathX0[nn]->Write(); 
     hpathY0[nn]->Write(); 
     hdq0[nn]->Write(); 
     nhit_trk0[nn]->Write();  
     hcheckhitQ0[nn]->Write(); 
     hcheckhitT0[nn]->Write(); 
     hcheckhitX0[nn]->Write();  
     hdeltahitQ0[nn]->Write(); 
     hdeltahitT0[nn]->Write(); 
     hdeltahitX0[nn]->Write();   
     
     hSlopeI_1[nn]->Write();
     hSlopeIErr_1[nn]->Write();
     hSlopeF_1[nn]->Write();
     hSlopeFErr_1[nn]->Write();
     hpathX1[nn]->Write(); 
     hpathY1[nn]->Write(); 
     hdq1[nn]->Write(); 
     nhit_trk1[nn]->Write();
     hcheckhitQ1[nn]->Write(); 
     hcheckhitT1[nn]->Write(); 
     hcheckhitX1[nn]->Write();  
     hdeltahitQ1[nn]->Write(); 
     hdeltahitT1[nn]->Write(); 
     hdeltahitX1[nn]->Write();  
     
     
    }
   }  
}     



/*=======================================================================================*/   
int main(int argc, char **argv)

/*=======================================================================================*/  

{ 
  
  string fname=""; 
  string ofile="";
  char cc;  
  int ipass=0;
  int maxevt=-1; 

  while((cc = getopt(argc, argv, "f:p:e:o:h")) != -1) 
    {  
      switch(cc)
	{ 
          case 'f': 
	  fname =  optarg;
	  break;  
	  
	  case 'p':
	  ipass = atoi(optarg); 
	  break;
	   
	  case 'e':
	  maxevt = atoi(optarg);  
	  break; 
	  
	  case 'o':
	  ofile = optarg;
	  break; 
	  
	  case 'h':
	  PrintHelp();  
	  exit(1);  

	}  
    }
    if(argc == 1) 
    { 
      cerr << "ERROR: please run with -h option" << endl;
      return 1;
     }  
     
     if (fname.empty()) {
     
        cerr<<" please provide input file "<<endl;
	return 1;
     }	
     
  
   
      ReadTrees(fname, ofile, ipass, maxevt); 

  return 0;
}


 
