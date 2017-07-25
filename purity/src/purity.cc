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
#include <TNtuple.h> 
#include "TROOT.h"
#include "TSystem.h"   
#include "purity.h"
 
 
 
 
 

 


/*=======================================================================================*/  

void ReadTrees(string fname, int maxevt)

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
    cout <<"----->file "<<inputfile<<"  will be analized "<<endl;
    
  
   
//get filename from fullpath   
  string filename=fullfilename.substr(fullfilename.find_last_of("/\\") + 1); 
  int pos = filename.find_first_of('.');
  

  string aname=filename.substr(0,pos);
   
  TString thename=aname;
   
    
//Define output files
  
  

    TString outfile = "purity_"+thename+".listing";  
    out.open(outfile);
    string temp;
    temp=outfile;
    
    
    
    

//root     
    TString outrootname="purity_"+thename+".root"; 
     fout = new TFile (outrootname,"recreate");
    
  
 
//select branches to analyse
    
    out<<"######################################################### \n\n ";  	
    out<<"      file  "<<inputfile<<"   will be analized \n \n ";  	 
    out<<"######################################################### \n\n ";  	     
    
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
   
    out<<"  ==================   EVENT HEADER DUMP: =================================="<<endl; 
   
    out<<"Total number of events:  "<<nentries<<endl; 
 

//set the number of events to be read 
//neve --> set by -e option (default =-1, read full tree)
//nenetries --> number of events in the EventTree
   
    int neve;
    if (maxevt==-1) neve=nentries;
    if (maxevt>-1)   neve=maxevt;
    if (maxevt>nentries) neve=nentries;   
    
     out<<" ----> "<< neve<<" events will be analyzed "<<endl; 
 
//branches
   
   EventHeader *evt_header = new EventHeader(); 
   theTree->SetBranchAddress("theEventHeader", &evt_header);  
   
   
    
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
  
    
    
      float bw=600./ntbin_drift; //bin width in cm
      ntbin= ceil(float(ldrift)/bw);  //number of histograms to be booked
      timebin=floor(4000/ntbin_drift);  //bin width (in time)
        
   
    DefineHisto();

          
     for (int nn=0; nn<neve; nn++) {  
     theTree->GetEvent(nn);  
  
 
     if (nn==0) { 
      hntbin->Fill(1,ntbin);
      hntbin->Fill(2,ldrift); 
      hntbin->Fill(3,timebin); 
     }
  
        

//Hit reconstruction results:   

      if ((!isHitView0) && (!isHitView1)) out<<"NO HITS INFORMATION AVAILABLE "<<endl; 
      
      if ((!isTrkView0) && (!isTrkView1))  {
            out<<"    NO 2D Tracks INFORMATION AVAILABLE "<<endl;
           cerr <<"    NO 2D Tracks INFORMATION AVAILABLE "<<endl;
	   return;
	   
	}
      
      if (isTrkView0 || isTrkView1) ReadTrks(trks_V0,trks_V1,hits_V0,hits_V1,evt_header,out);  
      
	   
   
      } //loop on events   

//write output root file   
     fout->cd();  
     SaveHisto();
     fout->Write();
    
     
//     
 
 
  
     delete trks_V0;
     delete trks_V1;
     
   

}

 

/*=======================================================================================*/  

void ReadTrks(TObjArray *trks_V0,TObjArray *trks_V1,TObjArray *hits_V0,TObjArray *hits_V1,EventHeader *evt_header, ofstream& out) 

/*=======================================================================================*/  

//read 2DTracks 

 
{
      vector<int>  ntrk0(crmnum),ntrk1(crmnum); 
   
  
       float charge[ntbin];
       
       float tmin, tmax;
       
       float tfirst,tlast;
       
       
       
       float vdrift=.1577; 
      
 
      for (int n=0; n<crmnum; n++) {
       ntrk0[n]=0;
       ntrk1[n]=0;
      } 
       
     
//view 0 loop on 2D tracks: 

    for(int i=0;i<trks_V0->GetEntries();i++)
    {
      LArReco::Track2d* atrk = (LArReco::Track2d*)trks_V0->At(i);  
       ntrk0[atrk->GetCRM()]++;  
       
       if (atrk->GetCRM()==0) {    
       
	 vector<TVector2> apath   = atrk->GetPath();
	 vector<Double_t> adq     =  atrk->GetDQ();  
           
	 tfirst=(apath[0].Y());
	 tlast=(apath[apath.size()-1].Y());
 
	
 //buidt charge vector (each component corresponds to an inteval in drift time )
  
      for (int nb=0; nb<ntbin; nb++) charge[nb]=0;
  
        int nstart,nstop; 
	int nfound=0;
       
        int asize=adq.size();

//to reduce the size of inner loop    
        for (int nh=0; nh<asize; nh++)  
        {  
 	  if (nh==0) {
	    nstart=0;
	    nstop=ntbin;
	   }
	   if (nh>0) {
	    nstart=max(0,nfound-5); 
	    nstop=min(nfound+5,ntbin); 
	    } 
 
           for (int nb=nstart; nb<nstop; nb++) {  
	    tmin=timebin*nb;
	    tmax=timebin*(nb+1);   
	    if ((apath[nh].Y()/vdrift)>=tmin && (apath[nh].Y()/vdrift)<tmax)  
	     {
	     charge[nb]+=adq[nh]; 
	     nfound=nb;
  
	     }
	  } //nb   
        } //nh
 	
      
 
// fill histograms for fit 
//select first and last time interval with charge deposition >0        
             int ilast=-1000;
	     int ifirst=1000; 
 
             for (int nb=0; nb<ntbin; nb++) { 
	       if (charge[nb]>0) {
	        if (nb<ifirst) ifirst=nb;
		if (nb>ilast)  ilast=nb;
	       } 
	      } 
   
	   for (int nb=0; nb<ilast-ifirst-1; nb++)  {
	     htbin0[nb]->Fill(charge[nb+ifirst+1]/charge[ifirst+1]);  
	   }   
       
     } //  if (atrk->GetCRM()==0) {   
       
     } // for(int i=0;i<trks_V0->GetEntries();i++)
     
//the same for View1  
     
    for(int i=0;i<trks_V1->GetEntries();i++)
    {
      LArReco::Track2d* btrk = (LArReco::Track2d*)trks_V1->At(i);  
       ntrk1[btrk->GetCRM()]++;    
     
        if (btrk->GetCRM()==0) {   
       
          vector<TVector2> bpath   = btrk->GetPath();
	  vector<Double_t> bdq     =  btrk->GetDQ();  
          tfirst=(bpath[0].Y());
	  tlast=(bpath[bpath.size()-1].Y()); 
      
 
	
      
        for (int nb=0; nb<ntbin; nb++) charge[nb]=0;
  
            int nstart,nstop; 
	int nfound=0;
       
         int bsize=bdq.size();
 
        for (int nh=0; nh<bsize; nh++)  
         {  
 	  if (nh==0) {
	    nstart=0;
	    nstop=ntbin;
	   }
	   if (nh>0) {
	    nstart=max(0,nfound-5); 
	    nstop=min(nfound+5,ntbin); 
	    } 
 
           for (int nb=nstart; nb<nstop; nb++) {  
	    tmin=timebin*nb;
	    tmax=timebin*(nb+1);    
            if ((bpath[nh].Y()/vdrift)>=tmin && (bpath[nh].Y()/vdrift)<tmax)  
	       
	    {
	     charge[nb]+=bdq[nh]; 
	      nfound=nb;
  
	     }
	  } //nb   
        } //nh  
     
     
// fill histograms for fit 
//select first and last time interval with charge depostion >0        
             int ilast=-1000;
	     int ifirst=1000;
	    
 
             for (int nb=0; nb<ntbin; nb++) { 
	       if (charge[nb]>0) {
	        if (nb<ifirst) ifirst=nb;
		if (nb>ilast)  ilast=nb;
	       } 
	      } 
   
	   
	   for (int nb=0; nb<ilast-ifirst-1; nb++)  {
  	     htbin1[nb]->Fill(charge[nb+ifirst+1]/charge[ifirst+1]);  
  	   }   
   
     } //   if (btrk->GetCRM()==0) {
   
     } //ntrk in view 1
    
        
    
      
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
   geotype= run_header->GetConfigFile(); 
   if (geotype.BeginsWith("WA105_1x1x3")) ldrift=100;
   if (geotype.BeginsWith("WA105_6x6x6")) ldrift=600;  
   outfile <<"     Configuration File  "<<geotype<<endl; 
   
 
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
      outfile<<"Hit reconstruction parameters : "<<endl;
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
     
    
     
     return(iout);
    
 }  

/*=======================================================================================*/  
 
void PrintHelp(void)

/*=======================================================================================*/  


{
   cout  <<"analysis program to compute purity value from  reconstruction results (recotask) "<<endl;
   cout  <<"Syntax: "<<endl;
   cout <<"           purity.exe -f afile.root [-e nev] "<<endl<<endl<<endl;  
   cout <<"  Options : "<<endl;
   cout <<"              -f input filename : afile.root "<<endl; 
   cout <<"              [] optional argument "<<endl<<endl; 
   cout <<"                -e :  number of events to process "<<endl;
   cout <<"                      (optional, default: process all events) "<<endl<<endl; 
   cout <<" 2 output files are produced: "<<endl;
   cout <<"    1) purity_afile.root: histograms file  "<<endl;
   cout <<"    2) purity_afile.listing  dump of RunHeader    "<<endl<<endl; 
  
   
   
}           

/*=======================================================================================*/  

void DefineHisto( )  

/*=======================================================================================*/  

{
 char name[256];

 
  
    

    hntbin=new TH1F ("ntbin", "ntbin",10,0.5,10.5);
   
 
    for (int nb=0; nb<ntbin; nb++) {
	   sprintf(name,"tbin0_%d",nb);
 
//             htbin0[nb]= new TH1F (name, name, 100,0,2000);
//        htbin0[nb]= new TH1F (name, name, 100,0.,2);
           htbin0[nb]= new TH1F (name, name, 60,0.,1.2);
	   sprintf(name,"tbin1_%d",nb);
//           htbin1[nb]= new TH1F (name, name, 100,0,2000);
//	 htbin1[nb]= new TH1F (name, name, 100,0.,2);
          htbin1[nb]= new TH1F (name, name, 60,0.,1.2);
   
	}   
 
   
 
	 
	 
      }
      

/*=======================================================================================*/  

void SaveHisto()  

/*=======================================================================================*/  

{

  hntbin->Write();
  for (int nb=0; nb<ntbin; nb++)   htbin0[nb]->Write();
  for (int nb=0; nb<ntbin; nb++)   htbin1[nb]->Write();
   
   
   
}     



/*=======================================================================================*/   
int main(int argc, char **argv)

/*=======================================================================================*/  

{ 
  
  string fname=""; 
  char cc;  
  int maxevt=-1; 

  while((cc = getopt(argc, argv, "f:e:h")) != -1) 
    {  
      switch(cc)
	{ 
          case 'f': 
	  fname =  optarg;
	  break;  
	  
	  
	   
	  case 'e':
	  maxevt = atoi(optarg);  
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
   
      ReadTrees(fname,  maxevt); 

  return 0;
}


 
