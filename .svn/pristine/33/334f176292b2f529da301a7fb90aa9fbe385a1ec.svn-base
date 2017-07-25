/*
 * RootUtils.cc
 *
 *  Created on:
 *      Author: Silvestro di Luise
 *      Silvestro.Di.Luise@cern.ch
 *
 *
 *
 *
 */
#include <iostream>
#include <vector>
#include <cmath>

#include <TROOT.h>
#include <TSystemDirectory.h>
#include <TSystemFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TH2Poly.h>
#include <TTree.h>
#include <TEventList.h>
#include <TList.h>

#include "RootUtils.h"
#include "StringUtils.h"


#include <TMath.h>


//TString RootUtils::STR_ERROR = " ######### ";


void RootUtils::AddMarginBin(TH1F&h, double width, TString opt){


 //add a bin (at the beginning or a the end) keeping axis range limits


 if(width<=0) return;

 int margin = -1;

 if(opt=="first") margin = 1;
 else if (opt=="last") margin =2;
 else return;


 //if last bin-width smaller than width return;

 if( margin==1 && width >= h.GetXaxis()->GetBinWidth(1) ) return;
 if( margin==2 && width >= h.GetXaxis()->GetBinWidth(h.GetNbinsX()) ) return;


 int Nx = h.GetNbinsX();

 double *b = new double[Nx+2];

 if( margin==2 ) {

   for(int i=1;i<=Nx;++i){
    b[i-1]= h.GetXaxis()->GetBinLowEdge(i);
   }	

   b[Nx]=h.GetXaxis()->GetBinLowEdge(Nx+1)-width;
   b[Nx+1]=h.GetXaxis()->GetBinLowEdge(Nx+1);

}


 if( margin==1 ) {

  b[0]=h.GetXaxis()->GetBinLowEdge(1);
  b[1]=h.GetXaxis()->GetBinLowEdge(1)+width;
  
  for(int i=2;i<=Nx+1;++i){
    b[i]=h.GetXaxis()->GetBinLowEdge(i);
  }

  
 }



 h.SetBins(Nx+1,b);

 delete [] b;

 return;

}



TGraph* RootUtils::BuildGraph(TString name, TString tit, double xmin,double xmax,double ymin,double ymax){

	/* *************************
	xmin,ymax	     xmax,ymax


	xmin,ymin        xmax,ymin

	Important the vertex ordering
	if this graph is passed to
	the method IsInsideRectangular

	************************* */


	double x[4]={xmin,xmax,xmax,xmin};
	double y[4]={ymin,ymin,ymax,ymax};

	return BuildGraph(name,tit,4,x,y,1);

}


TGraph* RootUtils::BuildGraph(TString name,TString tit,int N, double *x, double *y, int closed=0){

	TGraph *g=new TGraph();

	g->SetName(name);
	g->SetTitle(tit);

	for(int i=0;i<N;++i){
		g->SetPoint(i,x[i],y[i]);
	}

	if(closed) g->SetPoint(N,x[0],y[0]);

	return g;
}




TGraph* RootUtils::BuildGraphCircle(TString name, TString tit, double xc, double yc, double rad, int npts) {

    TGraph* g = new TGraph();

    g->SetName(name);
    g->SetTitle(tit);

    if(npts<=1) return g;

    double dAngle = TMath::TwoPi()/(npts-1);

    for(int i=0; i<npts; ++i){

            double angle = dAngle*i;

            double x = xc + TMath::Abs(rad)*TMath::Cos(angle);
            double y  = yc + TMath::Abs(rad)*TMath::Sin(angle);

            g->SetPoint(g->GetN(),x,y);

            //cout<<__FUNCTION__<<" "<<i<<"; "<<xc<<" "<<yc<<"; "<<x<<" "<<y<<" ; "<<angle<<" "<<rad<<endl;
    }

    return g;
}


TH2Poly* RootUtils::CopyTH2Poly(TH2Poly* hp, TString name, TString title) {
/**

Hand made deep copy of the binning (no content)

**/

    TH2Poly *h = new TH2Poly();
    h->SetName(name);
    h->SetTitle(title);

    if(!hp) return h;

    h->GetXaxis()->SetLimits( hp->GetXaxis()->GetXmin(), hp->GetXaxis()->GetXmax() );
    h->GetYaxis()->SetLimits( hp->GetYaxis()->GetXmin(), hp->GetYaxis()->GetXmax() );

    TList *binList = hp->GetBins();

    int Nbins = binList->GetSize();

    for(int i=0; i<Nbins;++i){

        TH2PolyBin* bin = (TH2PolyBin*)binList->At(i);

        TGraph *gp = (TGraph*)bin->GetPolygon();

        TGraph *g = new TGraph();

        double *x = gp->GetX();
        double *y = gp->GetY();

        for(int j=0; j<gp->GetN(); ++j) {
                g->SetPoint(j,x[j],y[j]);
        }

        g->SetName( gp->GetName() );
        g->SetTitle( gp->GetTitle() );

        h->AddBin(g);
    }



    return h;
}

bool RootUtils::CompareGraphs(TGraph &g1, TGraph &g2){


	int N1=g1.GetN();

	if(N1==0 || N1 != g2.GetN() ){
		return 0;
	}

	double *x1=g1.GetX();
	double *y1=g1.GetY();
	double *x2=g2.GetX();
	double *y2=g2.GetY();

	//should sort them first

	for(int i=0; i<N1; ++i){

		//if( !(x1[i]==x2[i] && y1[i]==y2[i]) ){
	    if( !( TMath::AreEqualRel(x1[i],x2[i],1e-5) && TMath::AreEqualRel(y1[i],y2[i],1e-5) ) ){
			return 0;
		}
	}

	return 1;
}


double RootUtils::ComputeEffEntries(TH1 &h, TString opt){

	int Nx = h.GetNbinsX();
	int Ny = h.GetNbinsY();

	int bin_start=1;

	if(opt.Contains("overflows")){
		bin_start=0;
		Nx+=1;
		Ny+=1;
	}

	double sum=0;

	for(int ix=bin_start;ix<=Nx;++ix){

		sum += h.GetBinContent(ix,bin_start);

		for(int iy=2;iy<=Ny;++iy){
			sum += h.GetBinContent(ix,iy);
		}
	}


	return sum;
}

double RootUtils::ComputeIntegral(TH1 &h, TString opt){

	int Nx = h.GetNbinsX();
	int Ny = h.GetNbinsY();

	double sum=0;

	for(int ix=1;ix<=Nx;++ix){

		double DX = h.GetXaxis()->GetBinWidth(ix);

		sum += h.GetBinContent(ix,1)*DX*h.GetYaxis()->GetBinWidth(1);

		for(int iy=2;iy<=Ny;++iy){
			sum += h.GetBinContent(ix,iy)*DX*h.GetYaxis()->GetBinWidth(iy);
		}
	}


	return sum;
}


double* RootUtils::CreateBinsArray(int n, double xmin, double xmax) {

	if(n==0) return 0;

	if(xmin==xmax) return 0;


	double *array=new double[n+1];

	for(int i=0; i<=n; ++i){

		array[i]=xmin+(xmax-xmin)*i/n;
	}

	return array;

}



std::vector<TString> RootUtils::DirFileList(TString path, TString ext, TString opt){


	std::vector<TString> list;

	TSystemDirectory dir(path, path);

	TList *files = dir.GetListOfFiles();

	if (files) {
	      TSystemFile *file;
	      TString fname;
	      TIter next(files);
	      while ((file=(TSystemFile*)next())) {

	    	 fname="";
	    	 if(opt=="full-path"){
	    		 fname = path;
	    		 if(!fname.EndsWith("/")) fname+="/";
	    	 }
	         fname += file->GetName();

	         if (!file->IsDirectory()){

	        	 if(ext.IsWhitespace()){
	        		 //cout << fname.Data() << endl;
	        		 list.push_back(fname);
	        	 }
	        	 else if( fname.EndsWith(ext) ) {
	        		 //cout << fname.Data() << endl;
	        		 list.push_back(fname);
	        	 }
	         }
	      }
	}

	return list;
}



int RootUtils::FillHistoFast(TH1F &h, double x, double w)
{

	int bin = h.GetXaxis()->FindFixBin(x);

	float *array = h.GetArray();

	array[bin] += w;

    return bin;
}



int RootUtils::FillHistoFast(TH2F &h, double x, double y, double w)
{
/* From TH1.cxx
int_t TH1::FindFixBin(Double_t x, Double_t y, Double_t z) const

if (GetDimension() < 2) {
      return fXaxis.FindFixBin(x);
}
if (GetDimension() < 3) {
      Int_t nx   = fXaxis.GetNbins()+2;
      Int_t binx = fXaxis.FindFixBin(x);
      Int_t biny = fYaxis.FindFixBin(y);
      return  binx + nx*biny;
}
*/

	//int bin = h.FindFixBin(x,y);

	int nx = h.GetNbinsX()+2;
	int binx = h.GetXaxis()->FindFixBin(x);
	int biny = h.GetYaxis()->FindFixBin(y);

	int bin = binx + nx*biny;

	float *array = h.GetArray();

	array[bin] += w;

    return bin;
}



void RootUtils::FillWithBinVolumeSize(TH1 &h ){

  
  for(int ix=1;ix<=h.GetNbinsX();++ix){
    for(int iy=1;iy<=h.GetNbinsY();++iy){
      for(int iz=1;iz<=h.GetNbinsZ();++iz){
	
	
	double wx = h.GetXaxis()->GetBinWidth(ix);
	double wy = h.GetYaxis()->GetBinWidth(iy);
	double wz = h.GetZaxis()->GetBinWidth(iz);
	
	double w = wx*wy*wz;

	if(w>0)
	  h.SetBinContent(ix,iy,iz,TMath::Log(1/w) );

      } 
    }    
  }


}


int RootUtils::FillConstantBinWidth(TH1F &h, double x, bool FixFirstBin) {
/**


**/

    if( h.GetDimension() !=1 ) return -1;

    int N = h.GetNbinsX();

    if(N<1) return -1;

    double fBinW = h.GetXaxis()->GetBinWidth(1);


    if( FixFirstBin && h.GetNbinsX()==1 && h.GetEntries()==0  ){

        h.SetBins( 3, x-fBinW, x+2*fBinW );

        return h.Fill(x+h.GetXaxis()->GetBinWidth(1)/2.);
    }


    int bin = h.FindBin(x);

    if(bin>=1 && bin<=N) {

        return h.Fill(x);

    }



    double xmin = h.GetXaxis()->GetXmin();
    double xmax = h.GetXaxis()->GetXmax();

    double x0 = h.GetXaxis()->GetBinCenter(1);


    bool isOverFlow;

    double dx = 0;


    if( x> xmax ){

        isOverFlow = true;
        dx = fabs(x - xmax);
    }

    if( x < xmin ){

        isOverFlow = false;
        dx = fabs(xmin - x);
    }

    int Nadd = dx/fBinW+1;


    if( isOverFlow ) xmax += Nadd*fBinW;
    else xmin -= Nadd*fBinW;

    int Nnew = N+Nadd;



    std::vector<float> conts;
    //std::vector<float> ws;

    for(int i=1; i<=N; ++i){
        conts.push_back( h.GetBinContent(i) );

    }


    h.Reset();

    //put two bins of margin each side
    h.SetBins(Nnew+4,xmin-2*fBinW,xmax+2*fBinW);


    int bin0 = h.FindBin(x0);

    float Nent = 0;

    for(int i=0; i<N;++i) {

        h.SetBinContent(i+bin0,conts[i]);
        Nent+=conts[i];
    }

    h.SetEntries(Nent);

    return h.Fill(x);

}


int RootUtils::FindBin(TAxis *a, double val, int opt) {
	/**
	 * Find the bin containing the coordinate val in the bin range
	 *   Underflow: -1
	 *   Overflow: -2
	 *   If opt ==1 then a coordinate falling
	 *   in the first half of the first bin
	 *   or in the second half of the last bin
	 *   is considered an underflow/overflow
	 *
	 *
	 */


	if( !a ) return -1;

	int N = a->GetNbins();

	int ib = a->FindBin(val);

	if(opt==0){

		if(ib==0) return -1;
		if(ib==N+1) return -2;

		return ib;
	}


	if(opt==1) {

		if( val <= a->GetBinCenter(1) ) return -1;
		if( val > a->GetBinCenter(N) ) return -2;

		return ib;

	}

	return -1;

}

int RootUtils::GetBin(int nx,int binx){

	/**
	 *
	 * Check the 2D case method
	 *
	 */

	nx += 2;

	if (binx < 0)   binx = 0;
	else if (binx >= nx) binx = nx-1;

	return binx;

}



int RootUtils::GetBin(int nx,int binx,int ny, int biny){

	/** For the 2D case
	 *
	 * TH1F::GetBin(....)

	 Int_t nx, ny, nz;
	   if (GetDimension() < 2) {
	      nx  = fXaxis.GetNbins()+2;
	      if (binx < 0)   binx = 0;
	      if (binx >= nx) binx = nx-1;
	      return binx;
	   }
	   if (GetDimension() < 3) {
	      nx  = fXaxis.GetNbins()+2;
	      if (binx < 0)   binx = 0;
	      if (binx >= nx) binx = nx-1;
	      ny  = fYaxis.GetNbins()+2;
	      if (biny < 0)   biny = 0;
	      if (biny >= ny) biny = ny-1;
	      return  binx + nx*biny;
	   }

	*/

	nx+=2;
	ny+=2;

	if(binx<0) binx = 0;
	else if(binx >= nx) binx = nx-1;

	if (biny < 0)   biny = 0;
	else if (biny >= ny) biny = ny-1;

	return  binx + nx*biny;


}



TString RootUtils::GetBinRangeLabel(TH1 &h, int bin, TString axis){

	TString label = "";

	int ax = 1;
	int nb = h.GetNbinsX();
	TAxis *TAx = h.GetXaxis();


	if( axis == "Y" || axis == "y"){
		ax = 2;
		nb = h.GetNbinsY();
		TAx = h.GetYaxis();
	}

	if(TAx == 0 ){
		return label;
	}

	if( bin<1 && bin>nb ){
		return label;
	}


	TString bin_low = StringUtils::ToString(TAx->GetBinLowEdge(bin));
	TString bin_up = StringUtils::ToString(TAx->GetBinUpEdge(bin));

	label = StringUtils::Name("[", bin_low,",",bin_up ,"]");

	return label;
}



TObject* RootUtils::GetObjFromFile(TString file, TString obj){

	//base ROOT dir
  //TDirectory *dir0 = gROOT->GetDirectory(0);

	//current dir
	TDirectory *dir = gDirectory;

	//cout<<"Current Dir "<<dir->GetPath()<<endl;

	TFile *f = TFile::Open(file);

	if(f){
		TObject *o = f->Get(obj);

		if(!o){
			cout<<__FILE__<<"::"<<__FUNCTION__<<" object "<<obj<<" not found in TFile "<<file<<endl;
			f->Close();
			return 0;
		}

		cout<<"Opened file Dir: "<<gDirectory->GetPath()<<endl;

		//move ownership to the previous directory (only for obj owned by default by TFile)
		if (o->InheritsFrom(TH1::Class())) ((TH1*)o)->SetDirectory(dir);
		if (o->InheritsFrom(TTree::Class())) ((TTree*)o)->SetDirectory(dir);
		if (o->InheritsFrom(TEventList::Class())) ((TEventList*)o)->SetDirectory(dir);

		//
		f->Close();

		//move back to parent dir
		dir->cd();

		return o;

	}else{

		dir->cd();

		cout<<__FILE__<<"::"<<__FUNCTION__<<" TFile "<<file<<" not found "<<endl;
		return 0;
	}

}


TGraphAsymmErrors* RootUtils::GetPoissonErrors(TH1 &h, TString opt){

	/**
	 *
	 *

	http://www-cdf.fnal.gov/physics/statistics/notes/pois_eb.txt

	*/

	TGraphAsymmErrors *g = new TGraphAsymmErrors();

	TString gname = "gErrors_"; gname += h.GetName();

	g->SetName(gname);

	for(int i=1; i<=h.GetNbinsX();++i){


		float cont = (float)h.GetBinContent(i);

		if(cont<0) cont = 0;

		int ip = g->GetN();

		g->SetPoint(ip,h.GetBinCenter(i),cont);

		float errlow= -0.5 + sqrt(cont+0.25);
		float errup = 0.5 + sqrt(cont+0.25);

		float errx = 0;

		if(opt != "NoBinWidth") errx = 0.5*h.GetXaxis()->GetBinWidth(i);



		g->SetPointError(ip,errx,errx,errlow,errup);

	}


	return g;
}


float RootUtils::GetOutliers(TH1& h){

	/*
	int nx=h.GetXaxis()->GetNBins();
	int ny=h.GetYaxis()->GetNBins();

	float nx_low= h.GetBinContent(0);
	float nx_up= h.GetBinContent(nx+1);

    float n = nx_low+nx_up;

    return n;
	 */

	return 0;

}

void RootUtils::ImportBinning(TH2F *h2, TH1F *hx, TH1F *hy){

	/// If hx or hy is NULL the corresponding axis binning in h2 is not changed
	/// it means h2 axis binning is first copied and then reassigned to itself since
	/// SetBins method for TH2F allows only simultaneous
	/// changing of both bin axis
	///
	/// hx->GetXasis()->GetXbins()->GetArray() is not working ???
	/// ok --> The array exists only if the axis has variable bin sizes
	/// otherwise the axis is defined only by Nbins,min and max
	///
	///

	if( !h2 ) return;

	if(!hx && !hy) return;

	int NX=0, NY=0;
	double *binsx;
	double *binsy;

	if( hx ){

	   NX=hx->GetNbinsX();
	   binsx=new double[NX+1];

	   for(int i=0;i<=NX;++i){
	   		binsx[i]=hx->GetXaxis()->GetBinLowEdge(i+1);
	   	}

	}else{

		NX=h2->GetNbinsX();
		binsx=new double[NX+1];

		for(int i=0;i<=NX;++i){
			binsx[i]=h2->GetXaxis()->GetBinLowEdge(i+1);
		}
	}



	if( hy ){

		NY=hy->GetNbinsX();
		binsy=new double[NY+1];

		for(int i=0;i<=NY;++i){
			binsy[i]=hy->GetXaxis()->GetBinLowEdge(i+1);
		}

	}else{

		NY=h2->GetNbinsX();
		binsy=new double[NY+1];

		for(int i=0;i<=NY;++i){
			binsy[i]=h2->GetXaxis()->GetBinLowEdge(i+1);
		}
	}


	h2->SetBins(NX,binsx,NY,binsy);

	delete [] binsx;
	delete [] binsy;

}



void RootUtils::ImportBinning(TH1F *h, TH1F *hx){

	/*
	 *
	 *  Assign hx binning to h
	 *
	 * hx->GetXasis()->GetXbins()->GetArray() is not working ???
	 *
	 */

	if( !h || !hx) return;

	int NX=hx->GetNbinsX();

	double *binsx=new double[NX+1];

	for(int i=0;i<=NX;++i){
		binsx[i]=hx->GetXaxis()->GetBinLowEdge(i+1);
	}


	h->SetBins(NX,binsx);

	delete [] binsx;
}


bool RootUtils::IsInsideRectangular(TGraph *g, double xp, double yp) {

	/* *************************
		xmin,ymax	     xmax,ymax


		xmin,ymin        xmax,ymin

		Important the vertex ordering

        The selection logic is

        xp:[xmin,xmax[ && yp:[ymin,ymax[


		************************* */

   if( !g ) return false;

   if( g->GetN()<5 ) return false;

   double *x=g->GetX();
   double *y=g->GetY();

   double xmin = x[0];
   double xmax = x[1];

   double ymin = y[4];
   double ymax = y[3];


   if( xp>=xmin && xp<xmax && yp>=ymin && yp<ymax){

	   return true;
   }

   return false;

}


bool RootUtils::IsTH2Poly(const TObject& obj) {

	 return obj.InheritsFrom("TH2Poly");

}

bool RootUtils::IsTH2(const TObject& obj) {

	return obj.InheritsFrom("TH2") && !IsTH2Poly(obj);

}

bool RootUtils::IsTH1(const TObject& obj) {

	return obj.InheritsFrom("TH1") && !IsTH2(obj) && !IsTH2Poly(obj);

}


bool RootUtils::IsTH2F(const TObject& obj) {

	return obj.InheritsFrom("TH2F") && !IsTH2Poly(obj);

}

bool RootUtils::IsTH1F(const TObject& obj) {

	return obj.InheritsFrom("TH1F") && !IsTH2F(obj) && !IsTH2Poly(obj);

}

TH2F* RootUtils::InvertAxis(TH2& hin,int opt)
{


	int Nx = hin.GetNbinsX();
	int Ny = hin.GetNbinsY();

	TAxis *ax = hin.GetXaxis();
	TAxis *ay = hin.GetYaxis();

	double *binsx=(double*)ax->GetXbins()->GetArray();
	double *binsy=(double*)ay->GetXbins()->GetArray();

	double xmin = ax->GetXmin();
	double xmax = ax->GetXmax();

	double ymin = ay->GetXmin();
	double ymax = ay->GetXmax();

	TString name=hin.GetName();
	name+="_yVsx";

	TH2F *hinv;
	if(ax->IsVariableBinSize() && ay->IsVariableBinSize()) hinv= new TH2F(name,"",Ny,binsy,Nx,binsx);
	else if(ax->IsVariableBinSize() && !ay->IsVariableBinSize()) hinv= new TH2F(name,"",Ny,ymin,ymax,Nx,binsx);
	else if(!ax->IsVariableBinSize() && ay->IsVariableBinSize()) hinv= new TH2F(name,"",Ny,binsy,Nx,xmin,xmax);

	hinv->SetTitle(hin.GetTitle());

	hinv->GetXaxis()->SetTitle(ay->GetTitle());
	hinv->GetYaxis()->SetTitle(ax->GetTitle());


	for(int ix=1; ix<=Nx;++ix){
		for(int iy=1; iy<=Ny;++iy){

			hinv->SetBinContent(iy,ix,hin.GetBinContent(ix,iy));
		}
	}


	return hinv;

}

double* RootUtils::UnifSpaced(int npts, double vmin, double vmax) {

  //NB: binwidth = (vmax-vmin)/(npts-1)
  //To get N bins in [vmin,vmax] you call : N+1,vmin,vmax
  
  if(vmax<=vmin || npts<=1) {
    cout<<__FILE__<<" "<<__FUNCTION__<<" ERROR:  Invalid arguments: "<<vmin<<" "<<vmax<<" "<<npts<<endl;
    return 0;
  }


  double *points = new double[npts];

  for(int i=0; i<npts;++i) {
    points[i] = vmin + (vmax-vmin)*i/(npts-1);
  }

  return points;
}



double* RootUtils::LogSpaced(int npts, double vmin, double vmax, bool forceFirst) {
  
  //To get N bins in [vmin,vmax] you call : N+1,vmin,vmax,true

  if(vmax<=vmin || npts<=1) {
    cout<<__FILE__<<" "<<__FUNCTION__<<" ERROR:  Invalid arguments: "<<vmin<<" "<<vmax<<" "<<npts<<" "<<forceFirst<<endl;
    return 0;
  }


  //if range goes into <=1 region shift it to >=1 first then shift it back

  double v1 = vmin;
  double v2 = vmax;
  bool shift = false;

  if(vmin<=0) {
    shift = true;
    forceFirst = true;

    v1 = 1;
    v2 = 1+ vmax-vmin;

  }

  double *points = new double[npts];

  double s = TMath::Log10(v2/v1)/npts;

  for(int i=0; i<npts;++i){
    
    points[i]=v1*TMath::Power(10,s*(i+1));
    //cout<<" log: "<<i<<" "<<points[i]<<endl;

  }
  
  if(forceFirst){
    double dx = points[0]-v1;

    for(int i=0; i<npts-1;++i) {
      points[i] -= dx;
      //cout<<" forced: "<<i<<" "<<points[i]<<endl;
    }  
    //cout<<" forced: "<<npts-1<<" "<<points[npts-1]<<endl;
  }


  if(shift){
    for(int i=0; i<npts;++i) {
      points[i] -= (1-vmin);
      //cout<<" shifted: "<<i<<" "<<points[i]<<endl;
    }
  }


  return points;
}




double* RootUtils::LogSpacedMulti(int npts_low, int npts_up, double vmin, double vmiddle, double vmax) {

  double *pup = RootUtils::LogSpaced(npts_up,vmiddle,vmax);
    
  double *plow = RootUtils::LogSpaced(npts_low,-vmiddle,-vmin);

  
  for(int i=0; i<npts_low;++i) plow[i]*=-1;

  int npts=npts_low+npts_up;

  double *points = new double[npts];

  for(int i=0; i<npts_low;++i) points[i]=plow[npts_low-1-i];
  for(int i=1; i<npts_up;++i)  points[npts_low-1+i]=pup[i];

  delete [] plow;
  delete [] pup;

  return points;
}



void RootUtils::RebinWithBinWidthLimits(TH1F &h, double wmin) {
  

  if(wmin<=0) return;

  //cout<<"wmin "<<wmin<<endl;

  std::vector<double> wlist;

  int Nx = h.GetNbinsX();

  wlist.push_back(h.GetXaxis()->GetBinWidth(1));

  //cout<<" w 1 "<<wlist[0]<<endl;

  for(int i=2; i<=Nx; ++i){

    double bw = h.GetXaxis()->GetBinWidth(i);

    double bw0 = wlist[wlist.size()-1];

    //cout<<" w "<<i<<" "<<bw<<endl;

    if(bw0<wmin){
      wlist[wlist.size()-1] = bw0+bw;
      //cout<<"merge "<<i<<" "<<bw0<<" "<<bw<<" "<<wlist[wlist.size()-1]<<endl;
    }else{
      wlist.push_back(bw);
    }
 
  }//i

  

  int Nrx=wlist.size();

  double *bx = new double[wlist.size()+1];

  double x = h.GetXaxis()->GetBinLowEdge(1);

  bx[0]=x;
  
  //cout<<bx[0]<<endl;

  for(int i=0;i<(int)wlist.size();++i){

    bx[i+1]=bx[i]+wlist[i];

    //cout<<i<<" "<<bx[i+1]<<endl;

  }


  h.SetBins(Nrx,bx);

  delete [] bx;


}


void RootUtils::MeanAndRMS(std::vector<double> *val, std::vector<double> *ws, double &mean, double &rms){
	/**
	 *  Mean and RMS, one pass
	 *  Optional a vector of weights
	 *
	 */

	mean=0;
	rms=0;

	if(!val){
		cout<<__FILE__<<"::"<<__FUNCTION__<<" ERROR: null input vector"<<endl;
		return;
	}


	int N = (int)(val->size());

	if(N==0) return;

	if(ws && (int)(ws->size())!=N){
		cout<<__FILE__<<"::"<<__FUNCTION__<<" ERROR different dimension for input vectors"<<endl;
		return;
	}

	double sumw=0;
	double sumwx=0;
	double sumwx2=0;

	for(int i=0; i<N;++i){

		double w=1;
		if(ws) w= ws->at(i);

		double v=val->at(i);

		if( TMath::IsNaN(v)){
			cout<<"!!!ERROR!!!! "<<__FILE__<<"::"<<__FUNCTION__<<" NaN element"<<endl;
			continue;
		}

		sumw+=w;
		sumwx+=w*v;
		sumwx2+=w*v*v;

	}


	if(sumw==0) return;

	mean = sumwx/sumw;

	rms= (sumwx2*sumw-sumwx*sumwx)/(sumw*sumw);

	if(rms>0) rms=TMath::Sqrt(rms);
	else rms=0;

}



void RootUtils::MeanAndRMSOld(std::vector<double> *val, std::vector<double> *ws, double &mean, double &rms){

	/**
	 *  Mean and RMS
	 *  Optional a vector of weights
	 *
	 */

	mean=0;
	rms=0;


	if(!val){
		cout<<__FILE__<<"::"<<__FUNCTION__<<" ERROR: null input vector"<<endl;
		return;
	}


	int N = (int)(val->size());

	if(N==0) return;


	if(ws && (int)(ws->size())!=N){
		cout<<__FILE__<<"::"<<__FUNCTION__<<" ERROR different dimension for input vectors"<<endl;
		return;
	}

	double sum=0;
	double sumw=0;

	for(int i=0; i<N;++i){

		double w=1;
		if(ws) w= ws->at(i);

		double v=val->at(i);

		if( TMath::IsNaN(v)){
			cout<<__FILE__<<"::"<<__FUNCTION__<<" NaN element"<<endl;
			continue;
		}

		sum+=w*v;
		sumw+=w;

	}


	if(sumw>0)	mean = sum/sumw;
	else return;


	sum=0;

	for(int i=0; i<N;++i){

		double w=1;
		if(ws) w= ws->at(i);

		double v=val->at(i);

		if(TMath::IsNaN(v)){
			continue;
		}

		sum+= w*(v-mean)*(v-mean);

	}


	rms = sum/sumw;

	if(rms) rms=TMath::Sqrt(rms);


}



double RootUtils::Normalize(TH1 &h, double norm, TString opt){

	double sum = ComputeEffEntries(h,opt);

	if(sum>0)
		h.Scale(norm/sum);

	return sum;

}

void RootUtils::Poisson(TH1 &h, int opt)
{

	int Nx = h.GetNbinsX();
	int Ny = h.GetNbinsY();

	double cont;

	for(int ix=1; ix<=Nx; ++ix){

		cont = h.GetBinContent(ix,1);

		h.SetBinContent(ix,1,fRandom.PoissonD(cont));

		for(int iy=2; iy<=Ny; ++iy){

			cont = h.GetBinContent(ix,iy);

			h.SetBinContent(ix,iy,fRandom.PoissonD(cont));

		}


	}

}

void RootUtils::Poisson(TH1 &hIn, TH1 &hOut, int opt)
{

	int Nx = hIn.GetNbinsX();
	int Ny = hIn.GetNbinsY();

	double cont;

	for(int ix=1; ix<=Nx; ++ix){

		cont = hIn.GetBinContent(ix,1);

		hOut.SetBinContent(ix,1,fRandom.PoissonD(cont));

		for(int iy=2; iy<=Ny; ++iy){

			cont = hIn.GetBinContent(ix,iy);

			hOut.SetBinContent(ix,iy,fRandom.PoissonD(cont));

		}


	}

}


TH1F* RootUtils::newTH1F(TString name, TString title, int nbins, double low, double up){

	/**
	 *
	 * Workaround to avid Root Warning
	 * on Potential Memory Leak, when creting two
	 * Histos with the same name
	 *
	 * One solution is to create it with a Dummy and the
	 * change the name with SetName();
	 * Otherwise use Clone()
	 *
	 */

	/*
	TH1F htemp("h",title,nbins,low,up);

	return (TH1F*)htemp.Clone(name);
	 */

	TH1F* h=new TH1F("h",title,nbins,low,up);

	h->SetName(name);

	return h;

}


TH2F* RootUtils::newTH2F(TString name, TString title, int nbinsx, double lowx, double upx
									,int nbinsy, double lowy, double upy		){

	/**
	 *
	 * Workaround to avid Root Warning
	 * on Potential Memory Leak, when creting two
	 * Histos with the same name
	 *
	 * A solution is to create it with a dummy name and the
	 * change the name with SetName();
	 * Otherwise use the Clone()
	 */

	/*
	TH2F htemp("h",title,nbinsx,lowx,upx,nbinsy,lowy,upy);

	return (TH2F*)htemp.Clone(name);
    */

	TH2F *h = new TH2F("h",title,nbinsx,lowx,upx,nbinsy,lowy,upy);
	h->SetName(name);

	return h;

}

bool RootUtils::OverlappingRectangulars(TGraph &g1, TGraph &g2){

	/**
	 * Check if a vertex of the first polygon is inside the perymeter of the second
	 *
	 * Function RootUtils::IsTotalInside, adapted from TMath::IsInside, is used here
	 * in order to skip the case where the two polygons having one side in common
	 * are considered as overlapping: histogram bins are normally side by side
	 *
	 * Both Polygon  order is inverted in case the second polygon is totally inside the first
	 *
	*/

	int N=g1.GetN();

	if( N !=5 || N != g2.GetN() ){
		return false;
	}


	double *x1=g1.GetX();
	double *y1=g1.GetY();

	double *x2=g2.GetX();
	double *y2=g2.GetY();




	for(int i=0; i<N; ++i){

        //cout<<x1[i]<<" "<<x2[i]<<"; "<<y1[i]<<" "<<y2[i]<<endl;

		//bool point_inside=TMath::IsInside(x1[i],y1[i],5,x2,y2);

		bool point_tot_inside=RootUtils::IsTotalInside<double>(x1[i],y1[i],5,x2,y2);


		if(point_tot_inside) return true;
	}


	// Invert order in case the second polygon is totally inside the first

	for(int i=0; i<N; ++i){

		//cout<<x1[i]<<" "<<x2[i]<<"; "<<y1[i]<<" "<<y2[i]<<endl;

		//bool point_inside=TMath::IsInside(x2[i],y2[i],5,x1,y1);

		bool point_tot_inside=RootUtils::IsTotalInside<double>(x2[i],y2[i],5,x1,y1);


		if(point_tot_inside) return true;
	}


	return false;
}


void RootUtils::PrintHistoInfo(TObject *o){


	cout<<o->GetName();

	if( o->InheritsFrom("TH1F") || o->InheritsFrom("TH2F") ){

		cout<<" #bins x= "
				<<((TH1F*)o)->GetNbinsX()
				<<": ["
				<<((TH1F*)o)->GetXaxis()->GetXmin()<<","
				<<((TH1F*)o)->GetXaxis()->GetXmax()
				<<"]";
	}


	if( o->InheritsFrom("TH2F") ){

			cout<<" #bins y= "
					<<((TH2F*)o)->GetNbinsY()
					<<": ["
					<<((TH2F*)o)->GetYaxis()->GetXmin()<<","
					<<((TH2F*)o)->GetYaxis()->GetXmax()
					<<"]";
		}


	cout<<endl;

}


TH1F* RootUtils::RatioHist1D(TString name,TString tit, TH1F &hnum, TH1F& hden, double hmin, double hmax,TString opt)
{

	TH1F *hr = new TH1F(hnum);

	hr->Divide(&hden);

	TString xtit = hnum.GetXaxis()->GetTitle();
	if(xtit.IsWhitespace()) xtit = hden.GetXaxis()->GetTitle();

	TString title="";
	TString ytit="";

	std::vector<TString> vtit = StringUtils::Tokenize(tit,";");


	if(vtit.size()>=1){
		title = vtit[0];
	}

	if(vtit.size()>=2){
		if(!vtit[1].IsNull()) xtit = vtit[1];
	}

	if(vtit.size()==3){
		if(!vtit[2].IsNull()) ytit = vtit[2];
	}

	hr->SetName(name);

	hr->SetTitle(title);
	hr->GetXaxis()->SetTitle(xtit);
	hr->GetYaxis()->SetTitle(ytit);



	hr->SetMinimum(hmin);
	hr->SetMaximum(hmax);

	return hr;

}



bool RootUtils::SameBinning(TH1 &h1, TH1 &h2)
{

	if( h1.GetDimension() != h2.GetDimension() ) return false;

	TAxis *axis1 = 0;
	TAxis *axis2 = 0;

	for(int ia=1; ia<=h1.GetDimension(); ++ia){

		if(ia==1){
			axis1 = h1.GetXaxis();
			axis2 = h2.GetXaxis();
		}
		else
		if(ia==2){
			axis1 = h1.GetYaxis();
			axis2 = h2.GetYaxis();
		}
		else
		if(ia==3){
			axis1 = h1.GetZaxis();
			axis2 = h2.GetZaxis();
		}


		if( !axis1 || !axis2 ){
			cout<<STR_ERROR<<__FILE__<<"::"<<__FUNCTION__<<" null axis ptr for dimension: "<<ia<<endl;
			return false;
		}

		if( axis1->GetNbins() != axis2->GetNbins() ){
			return false;
		}

		if(!TMath::AreEqualRel(axis1->GetXmax(), axis2->GetXmax(), 1.E-15)){
			return false;
		}

		for(int ib = 1; ib <= axis1->GetNbins() + 1; ++ib)
			if(!TMath::AreEqualRel(axis1->GetBinLowEdge(ib), axis2->GetBinLowEdge(ib), 1.E-15)) {
				return false;
			}

	}//axis



	return true;
}
