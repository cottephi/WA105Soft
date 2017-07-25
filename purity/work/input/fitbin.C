#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TString.h>


 
void fitbin() {

char name[256] ; 
 
 
    ofstream outfilemv;
    outfilemv.open("tbin_mean.list"); 
    ofstream outfilefit;
    outfilefit.open("tbin_fit.list"); 
    ofstream forfit_mop;
    forfit_mop.open("mop.input");
    
    
   
    
TFile f("tbin.root"); 

if (f.GetListOfKeys()->Contains("ntbin")) {
TIter next(f.GetListOfKeys());
TKey *key;
TH1F *h;


int nstart=1;
int ncount=0;
int ncount1=0;
int looper=0; 
int itowrite; 
   
 
     
    int nh=ntbin->GetBinContent(1);
    float timebin=ntbin->GetBinContent(3);
     
 
//  
    vector <float> x0;
    vector <float> x1;
  
 
    float b_inf,b_sup;
 
    
    int iused=0;
 
 while ((key=(TKey*)next())) {
    looper=looper+1; 
    sprintf(name,key->GetName()); 
    (TH1F*) h = (TH1F*)f.Get(key->GetName());   
    iused=0;
      int nent=h->Integral(); 
       itowrite=0;
       if (looper>1) {
       if (ncount<nh && iused==0) {
       iused=1;
       
        x0.push_back((timebin)/2+(timebin*ncount));
        outfilemv<<"0  "<<ncount<<"  "<<nent<<"  "<<x0[ncount]<<"  "<<h->GetMean()<<"   "<<h->GetMeanError()<<"  "<<h->GetRMS()<<"  "<<h->GetRMSError()<<endl;  
        b_inf=(h->GetMean())-(h->GetRMS());
        b_sup=(h->GetMean())+(h->GetRMS());  
  
 
        if (nent>99) { 
          h->Fit("gaus","Q","",b_inf,b_sup);
 	  TString fitresult=gMinuit->fCstatu;
 	  if (fitresult.BeginsWith("CONVE")) itowrite=1;  
	}
	if (ncount==0) {
	 outfilefit<<"0  "<<ncount<<"  "<<nent<<"  "<<x0[ncount]<<"  1 1 1 1"<<endl; 	
         forfit_mop<<"x0["<<ncount1<<"]= "<<x0[ncount]<<";"<<endl;
         forfit_mop<<"y0["<<ncount1<<"]= 1;"<<endl; 
	 forfit_mop<<"dy0["<<ncount1<<"]= 0;"<<endl;}
	 
	if (itowrite==1 && ncount>0) 
	{
         outfilefit<<"0  "<<ncount<<"  "<<nent<<"  "<<x0[ncount]<<"  "<<gaus->GetParameter(1)<<" "<<gaus->GetParError(1)<<"   "<<gaus->GetParameter(2)<<" "<<gaus->GetParError(2)<<endl; 	
         forfit_mop<<"x0["<<ncount<<"]= "<<x0[ncount]<<";"<<endl;
         forfit_mop<<"y0["<<ncount<<"]= "<<gaus->GetParameter(1)<<";"<<endl; 
	 forfit_mop<<"dy0["<<ncount<<"]= "<<gaus->GetParError(1)<<";"<<endl;
	 } 
       if (itowrite==0 && ncount>0) {
        outfilefit<<"0  "<<ncount<<"  "<<nent<<"  "<<x0[ncount]<<"  0 0 0 0"<<endl; 	
        forfit_mop<<"x0["<<ncount<<"]= "<<x0[ncount]<<";"<<endl;
        forfit_mop<<"y0["<<ncount<<"]=  0;"<<endl; 
        forfit_mop<<"dy0["<<ncount<<"]= 0;"<<endl; 
       }  
        ncount++;  
       } //if (ncount<nh)
       
 
       if (ncount>=nh && ncount<2*nh && iused==0) {
       iused=1; 
           
        x1.push_back((timebin)/2+(timebin*ncount1)); 
        outfilemv<<"1  "<<ncount1<<"  "<<nent<<"  "<<x1[ncount1]<<"   "<<h->GetMean()<<"   "<<h->GetMeanError()<<"  "<<h->GetRMS()<<"  "<<h->GetRMSError()<<endl; 
        
        b_inf=(h->GetMean())-(h->GetRMS());
       b_sup=(h->GetMean())+(h->GetRMS());  
 
        if (nent>99) {
        h->Fit("gaus","Q","",b_inf,b_sup);
	 TString fitresult=gMinuit->fCstatu;
 	  if (fitresult.BeginsWith("CONVE")) itowrite=1;  
	}
	if (ncount1==0) {
	 outfilefit<<"1  "<<ncount1<<"  "<<nent<<"  "<<x1[ncount1]<<"  1 1 1 1"<<endl; 
	 forfit_mop<<"x1["<<ncount1<<"]= "<<x1[ncount1]<<";"<<endl;
         forfit_mop<<"y1["<<ncount1<<"]= 1;"<<endl; 
	 forfit_mop<<"dy1["<<ncount1<<"]= 0;"<<endl;}
	if (itowrite==1 && ncount1>0) { 
         outfilefit<<"1  "<<ncount1<<"  "<<nent<<"  "<<x1[ncount1]<<"   "<<gaus->GetParameter(1)<<" "<<gaus->GetParError(1)<<"   "<<gaus->GetParameter(2)<<" "<<gaus->GetParError(2)<<endl; 
	 forfit_mop<<"x1["<<ncount1<<"]= "<<x1[ncount1]<<";"<<endl;
         forfit_mop<<"y1["<<ncount1<<"]= "<<gaus->GetParameter(1)<<";"<<endl; 
	 forfit_mop<<"dy1["<<ncount1<<"]= "<<gaus->GetParError(1)<<";"<<endl;
       }
         if (itowrite==0 && ncount1>0) {
         outfilefit<<"1  "<<ncount1<<"  "<<nent<<"   "<<x1[ncount1]<<"  0 0 0 0"<<endl; 	
         forfit_mop<<"x1["<<ncount1<<"]= "<<x1[ncount1]<<";"<<endl;
         forfit_mop<<"y1["<<ncount1<<"]=  0;"<<endl; 
	 forfit_mop<<"dy1["<<ncount1<<"]= 0;"<<endl; 
       }  
        ncount1++;
        ncount++;  
       } //if (ncount<nh)
        
     }//looper>1  
   } //while ((key=(TKey*)next()))  
 
 
 }
 
  outfilemv.close();
  outfilefit.close(); 
  forfit_mop.close(); 
 
 
  
  }
 
 
 
 
 
 
 
