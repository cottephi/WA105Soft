/*
 * SpaceMap3.cc
 *
 *  Created on:
 *      Author:
 */

//
#include <cstddef>
#include <iostream>

//
#include <TMath.h>
#include <TAxis.h>
#include <TGraph.h>
#include <TGraph2D.h>
#include <TMultiGraph.h>
#include <TH2F.h>

//
#include "SpaceMap3.h"


//
using std::cout;
using std::cerr;
using std::endl;


//


//ClassImp(Map::SpaceMap3);


namespace Map {

SpaceMap3::SpaceMap3() {
/**
 *
 *
 */
	Init();
}

SpaceMap3::SpaceMap3(const TString & name, const TString & tit
			,int nx, double xmin, double xmax
			,int ny, double ymin, double ymax
			,int nz, double zmin, double zmax)
:TH3F(name,tit,nx,xmin,xmax,ny,ymin,ymax,nz,zmin,zmax)
{
/**
 *
 */
	Init();


}


SpaceMap3::SpaceMap3(const SpaceMap3& m)
  :TH3F(m)
{

	Init();

	SetGlobalNativeScale(m.GetGlobalNativeScale());
	SetGlobalScale(m.GetGlobalScale());
	SetGlobalOffset(m.GetGlobalOffset());

	if( m.HasTranslation() ) {
	  SetTranslation( m.GetTranslation(kAxisX),m.GetTranslation(kAxisY),m.GetTranslation(kAxisZ) );
	}

	if( m.HasRotation() ) {

	  double phi, theta, psi;

	  m.GetRotationMatrix().GetAngles(phi,theta,psi);

	  SetRotation(phi,theta, psi);
	}

}


SpaceMap3::SpaceMap3(const TH3F& h)
: TH3F(h)
{

	Init();

}

SpaceMap3::~SpaceMap3() {

}


void SpaceMap3::Import(TH3F &h) {
  /**
   *
   *
   */



  ImportBinning(h);

  Add( &h );
}





void SpaceMap3::PrintRanges() {

  using std::cout;
  using std::endl;

 cout<<__FUNCTION__<<" "<<GetName()<<" "<<GetTitle()<<endl;

 cout<<" - x - "<<GetAxis(kAxisX)->GetXmin()<<" "<<GetAxis(kAxisX)->GetXmax()<<endl;
 cout<<" - y - "<<GetAxis(kAxisY)->GetXmin()<<" "<<GetAxis(kAxisY)->GetXmax()<<endl;
 cout<<" - z - "<<GetAxis(kAxisZ)->GetXmin()<<" "<<GetAxis(kAxisZ)->GetXmax()<<endl;


}

void SpaceMap3::PrintCenters() {

 cout<<" - local - "<<GetCenter(kAxisX,kLocal)<<" "<<GetCenter(kAxisY,kLocal)<<" "<<GetCenter(kAxisZ,kLocal)<<endl;
 cout<<" - master - "<<GetCenter(kAxisX,kMaster)<<" "<<GetCenter(kAxisY,kMaster)<<" "<<GetCenter(kAxisZ,kMaster)<<endl;
 cout<<" - global - "<<GetCenter(kAxisX,kGlobal)<<" "<<GetCenter(kAxisY,kGlobal)<<" "<<GetCenter(kAxisZ,kGlobal)<<endl;

}




void SpaceMap3::ImportBinning(TH3F &h) {
  /**
   *
   *
   */

  if( !h.GetXaxis() || !h.GetYaxis() || !h.GetZaxis()) {
    return;
  }

  SetName( h.GetName() );
  SetTitle( h.GetTitle() );

  GetXaxis()->SetTitle( h.GetXaxis()->GetTitle() );
  GetYaxis()->SetTitle( h.GetYaxis()->GetTitle() );
  GetZaxis()->SetTitle( h.GetZaxis()->GetTitle() );




  int Nx = h.GetNbinsX();
  int Ny = h.GetNbinsY();
  int Nz = h.GetNbinsZ();



  if( !h.GetXaxis()->GetXbins()->GetArray()
      || !h.GetYaxis()->GetXbins()->GetArray()
      || !h.GetZaxis()->GetXbins()->GetArray() ) {



    double range_min[3];
    double range_max[3];

    range_min[0]=h.GetXaxis()->GetXmin();
    range_min[1]=h.GetYaxis()->GetXmin();
    range_min[2]=h.GetZaxis()->GetXmin();

    range_max[0]=h.GetXaxis()->GetXmax();
    range_max[1]=h.GetYaxis()->GetXmax();
    range_max[2]=h.GetZaxis()->GetXmax();

    this->SetBins(Nx,range_min[0],range_max[0]
		  ,Ny,range_min[1],range_max[1]
		  ,Nz,range_min[2],range_max[2]
		  );

  }else{


    //this->SetBins( Nx,h.GetXaxis()->GetXbins()->GetArray()
    //		    ,Ny,h.GetYaxis()->GetXbins()->GetArray()
    //		    ,Nz,h.GetZaxis()->GetXbins()->GetArray());


    double *binsx=new double[Nx+1];

    for(int i=0;i<=Nx;++i){
      binsx[i]=h.GetXaxis()->GetBinLowEdge(i+1);
    }

    double *binsy=new double[Ny+1];

    for(int i=0;i<=Ny;++i){
      binsy[i]=h.GetYaxis()->GetBinLowEdge(i+1);
    }

    double *binsz=new double[Nz+1];

    for(int i=0;i<=Nz;++i){
      binsz[i]=h.GetZaxis()->GetBinLowEdge(i+1);
    }

    this->SetBins(Nx,binsx,Ny,binsy,Nz,binsz);


    delete [] binsx;
    delete [] binsy;
    delete [] binsz;

  }

}


float SpaceMap3::Evaluate(double x, double y, double z) {
/**
 *
 */

  SetPoint(x,y,z);

  return Evaluate();


}


float SpaceMap3::Evaluate() {
    /**
     *
     *
     */

    double x = fMaster[kAxisX];
    double y = fMaster[kAxisY];
    double z = fMaster[kAxisZ];


    float val = 0;

    if( !IsInRange(x,y,z) ) {

      // Use closest bin ...

      return 0;

    }


    if( IsBorder(x,y,z) ) {



      int bin = FindBin(x,y,z);

      val = GetBinContent(bin);

#if 1
      // use "extrapolation" to treat discontinuities of maps in edge bins 
      //      val = Extrapolate (x,y,z);
      //      val = InterpolateAtEdge(x,y,z); // LZ
      val = ExtrapolateAtEdge(x,y,z);
#endif      

#if 0
      int binx = GetXaxis()->FindBin(x);
      int biny = GetYaxis()->FindBin(y);
      int binz = GetZaxis()->FindBin(z);

      int Nx = GetNbinsX();
      int Ny = GetNbinsY();
      int Nz = GetNbinsZ();

 
      double xc = GetXaxis()->GetBinCenter(binx);
      double yc = GetYaxis()->GetBinCenter(biny);
      double zc = GetZaxis()->GetBinCenter(binz);


      double dx = x-xc;
      double dy = y-yc;
      double dz = z-zc;

      double x2 = xc - dx;
      double y2 = yc - dy;
      double z2 = zc - dz;

      bool isBordX = IsBorderX(x);
      bool isBordY = IsBorderY(y);
      bool isBordZ = IsBorderZ(z);


      bool is_edge_xy = false;

      if(binx == 1 && biny ==1) is_edge_xy = true;
      if(binx == Nx && biny ==1) is_edge_xy = true;
      if(binx == 1 && biny ==Ny) is_edge_xy = true;
      if(binx == Nx && biny == Ny) is_edge_xy = true;

      
      if(is_edge_xy) {
        if(!isBordX) {x2 = x;}
        if(!isBordY) {y2 = y;}
        //if(!isBordZ) {z2 = z;}
      }


      int bin2 = FindBin(x2,y2,z2);

      int binx2 = GetXaxis()->FindBin(x2);
      int biny2 = GetYaxis()->FindBin(y2);
      int binz2 = GetZaxis()->FindBin(z2);


      bool isBorder2 = IsBorder(x2,y2,z2);



      double val2 = val;
      if( !isBorder2 ) val2 = Interpolate(x2,y2,z2);
      
      double dval = val2 - val;

      double val3 = val - dval;

      val = val3;

#endif


    }else{


      val = Interpolate(x,y,z);

      //cout<<" Interpolate "<<x<<" "<<y<<" "<<z<<" "<<val<<endl;

    }


    return val*fGlobNativeScale*fGlobScale + fGlobOffset;

  }


double SpaceMap3::Extrapolate(double x, double y, double z) {

      int bin = FindBin(x,y,z);
      double val = GetBinContent(bin);

      int binx = GetXaxis()->FindBin(x);
      int biny = GetYaxis()->FindBin(y);
      int binz = GetZaxis()->FindBin(z);

      int Nx = GetNbinsX();
      int Ny = GetNbinsY();
      int Nz = GetNbinsZ();

      if (Nz==1){ // when the GAr map is used (only one Z bin at the moment)
	val = InterpolateAtEdge(x,y,z);// LZ 
	return val;
      }

      int binx2 = binx; 
      int biny2 = biny; 
      int binz2 = binz;

      // find the previous or next bin, away from edges
      if (binx==1) binx2=2;
      if (biny==1) biny2=2;
      if (binz==1) binz2=2;
      if (binx==Nx) binx2=Nx-1;
      if (biny==Ny) biny2=Ny-1;
      if (binz==Nz) binz2=Nz-1;

      // get center of current bin and of previous/next bin
      double xc = GetXaxis()->GetBinCenter(binx);
      double yc = GetYaxis()->GetBinCenter(biny);
      double zc = GetZaxis()->GetBinCenter(binz);
      double xc2 = GetXaxis()->GetBinCenter(binx2);
      double yc2 = GetYaxis()->GetBinCenter(biny2);
      double zc2 = GetZaxis()->GetBinCenter(binz2);

      // interpolate to point in the previous/next bin with the other coordinate(s) set the same
      double x2 = x + (xc2-xc);
      double y2 = y + (yc2-yc);
      double z2 = z + (zc2-zc);
      if (x2==x && y2==y && z2==z) return val;

      double val2;
      val2 = Interpolate(x2,y2,z2);
      int bin2 = FindBin(x2,y2,z2);
      // go back by difference of values at the center of the two bins
      double val1;
      val1 = GetBinContent(bin2);
      double Delta = val-val1;

      val = val2+Delta;
      return val;
}

bool SpaceMap3::IsBorder(double x, double y, double z) {
	/**
	 *
	 */

  if( IsBorderX(x) ) return true;
  if( IsBorderY(y) ) return true;
  if( IsBorderZ(z) ) return true;

  return false;
}

bool SpaceMap3::IsBorder(const Axis &ax, double v) {

  /*
   In the external half of the first/last bin or outside the range
   */

  TAxis *a = GetAxis(ax);


  if( a ) {
    if( a->GetNbins()==1) return true;
    if( v<= a->GetBinCenter(1) ) return true;
    //    if( v>= a->GetBinCenter(GetXaxis()->GetNbins()) ) return true; // BUG
    if( v>= a->GetBinCenter(a->GetNbins()) ) return true;
  }

  return false;
}

bool SpaceMap3::IsBorderX(double v) {

  return IsBorder(kAxisX,v);
}


bool SpaceMap3::IsBorderY(double v) {
  return IsBorder(kAxisY,v);
}


bool SpaceMap3::IsBorderZ(double v) {

  return IsBorder(kAxisZ,v);
}


bool SpaceMap3::IsInRange(double x, double y, double z) {

  if( !IsInRangeX(x) ) return false;
  if( !IsInRangeY(y) ) return false;
  if( !IsInRangeZ(z) ) return false;

  return true;
}

bool SpaceMap3::IsInRangeX(double v) {

  return IsInRange(kAxisX,v);

}

bool SpaceMap3::IsInRangeY(double v) {

  return IsInRange(kAxisY,v);

}

bool SpaceMap3::IsInRangeZ(double v) {

  return IsInRange(kAxisZ,v);

}

bool SpaceMap3::IsInRange(const Axis& ax, double v) {

  TAxis *a = GetAxis(ax);

  if(a){
    int bin = a->FindBin(v);
    if(bin >= 1 && bin <= a->GetNbins()) return true;
    return false;
  }

  return false;

}


double SpaceMap3::GetBinCenterMin(const Axis& a) {

    TAxis *ax = GetAxis(a);

    if(ax)
        return ax->GetBinCenter(1);

    return 0;

}

double SpaceMap3::GetBinCenterMax(const Axis& a) {

    TAxis *ax = GetAxis(a);

    if(ax)
        return ax->GetBinCenter(ax->GetNbins());

    return 0;
}


TAxis* SpaceMap3::GetAxis(const Axis& a) {

  if( a == kAxisX ) return GetXaxis();
  if( a == kAxisY ) return GetYaxis();
  if( a == kAxisZ ) return GetZaxis();

  return nullptr;
}




void SpaceMap3::SetGlobalNativeScale(float v) {

	fGlobNativeScale = v;

}

void SpaceMap3::SetGlobalScale(float v) {

	fGlobScale = v;

}

void SpaceMap3::SetGlobalOffset(float v) {

	fGlobOffset = v;

}

double SpaceMap3::GetCenter(const Axis& ax) {

   return fCenter[ax];
}





void SpaceMap3::SetCenter(double xc, double yc, double zc) {


  fCenter[kAxisX] = xc;
  fCenter[kAxisY] = yc;
  fCenter[kAxisZ] = zc;

  fTranslation[kAxisX]= fCenter[kAxisX] - GetCenter(kAxisX,kMaster);
  fTranslation[kAxisY]= fCenter[kAxisY] - GetCenter(kAxisY,kMaster);
  fTranslation[kAxisZ]= fCenter[kAxisZ] - GetCenter(kAxisZ,kMaster);

  if( fTranslation[kAxisX] == 0. && fTranslation[kAxisX] == 0. && fTranslation[kAxisX] == 0. ) {
    fHasTranslation = false;
  }else{
    fHasTranslation = true;
  }


}

void SpaceMap3::SetTranslation(double dx, double dy, double dz) {


  fCenter[kAxisX] = GetCenter(kAxisX,kMaster) + dx;
  fCenter[kAxisY] = GetCenter(kAxisX,kMaster) + dy;
  fCenter[kAxisZ] = GetCenter(kAxisX,kMaster) + dz;

  fTranslation[kAxisX] = dx;
  fTranslation[kAxisY] = dy;
  fTranslation[kAxisZ] = dz;

  if( dx ==0. && dy == 0. && dz ==0.) {
    fHasTranslation = false;
  }else{
    fHasTranslation = true;
  }

}


  void SpaceMap3::SetRotation(double phi, double theta, double psi) {


    fRotMatrix.SetAngles(phi,theta,psi);

  }



  void SpaceMap3::ResetCenter() {
    /**
     *
     */

    SetCenter(0.5*(GetXaxis()->GetXmin()+
		 GetXaxis()->GetXmax()),
	    0.5*(GetYaxis()->GetXmin()+
		 GetYaxis()->GetXmax()),
	    0.5*(GetZaxis()->GetXmin()+
		 GetZaxis()->GetXmax())
	    );

  }



  void SpaceMap3::ResetRotation() {

    SetRotation(0,0,0);
  }


  void SpaceMap3::ResetRefSystem() {

    SetRotation(0.,0.,0.);
    SetTranslation(0.,0.,0.);

  }




  bool SpaceMap3::HasTranslation() const {

    return fHasTranslation;
  }


  bool SpaceMap3::HasRotation() const {

    return ! fRotMatrix.IsIdentity();

  }

  bool SpaceMap3::HasTransformation() const {

    return  HasTranslation() || HasRotation() ? true : false;

  }

  double SpaceMap3::GetTranslation(const Axis &a ) const {

    return fTranslation[a];

  }

  void SpaceMap3::SetPoint(double x, double y, double z) {


    if( refSyst == kGlobal) {

      /* Default Reference System */
      /* Master Ref eventually Rotated and Translated */

      if( !HasTransformation() ) {

	fMaster[kAxisX] = x;
	fMaster[kAxisY] = y;
	fMaster[kAxisZ] = z;

	return;

      }


      if( !HasRotation() ) {

	fMaster[kAxisX] = x - fTranslation[kAxisX];
	fMaster[kAxisY] = y - fTranslation[kAxisY];
	fMaster[kAxisZ] = z - fTranslation[kAxisZ];
	return;

      }


      cout<<"Compute Rotation "<<endl;

      fRotMatrix.Print();

      double xloc_rot[3]={
	 x - GetCenter(kAxisX,kGlobal)
	,y - GetCenter(kAxisY,kGlobal)
	,z - GetCenter(kAxisZ,kGlobal)
      };

      cout<<" Global "<<x<<" "<<y<<" "<<z<<endl;
      cout<<" CenterMaster: "<<GetCenter(kAxisX,kMaster)<<" "<<GetCenter(kAxisY,kMaster)<<" "<<GetCenter(kAxisZ,kMaster)<<endl;
      cout<<" CenterGlobal: "<<GetCenter(kAxisX,kGlobal)<<" "<<GetCenter(kAxisY,kGlobal)<<" "<<GetCenter(kAxisZ,kGlobal)<<endl;
      cout<<" Local Rotated "<<xloc_rot[0]<<" "<<xloc_rot[1]<<" "<<xloc_rot[2]<<endl;

      double xloc[3];


      /**
       In TGeo:
       Local To Master use the Matrix: Rotate and Translate
       ->It transform from Local (TH3) to Global
       Master To Local use the inverse Matrix: inverse Rotate and Translate
       ->It transform from Glonal to Local (TH3)

       In this setup:
        Local to Master = Master to Global
     */

      fRotMatrix.MasterToLocal(xloc_rot,xloc);

      fMaster[kAxisX] = xloc[0] + GetCenter(kAxisX,kMaster);
      fMaster[kAxisY] = xloc[1] + GetCenter(kAxisY,kMaster);
      fMaster[kAxisZ] = xloc[2] + GetCenter(kAxisZ,kMaster);


    } else if ( refSyst == kMaster ) {


      /* Original TH3 Ref system */

      fMaster[kAxisX] = x;
      fMaster[kAxisY] = y;
      fMaster[kAxisZ] = z;

      return;

    } else if ( refSyst == kLocal ) {

      /* Master Ref centered in (0,0,0) */

      if( !HasRotation() ) {

       fMaster[kAxisX] = x + GetCenter(kAxisX,kMaster);
       fMaster[kAxisY] = y + GetCenter(kAxisY,kMaster);
       fMaster[kAxisZ] = z + GetCenter(kAxisZ,kMaster);

       return;

      }

      if( HasRotation() ){

	double xloc_rot[3] = {x,y,z};
	double xloc[3];


	fRotMatrix.MasterToLocal(xloc_rot,xloc);

	fMaster[kAxisX] = xloc[0] + GetCenter(kAxisX,kMaster);
	fMaster[kAxisY] = xloc[1] + GetCenter(kAxisY,kMaster);
	fMaster[kAxisZ] = xloc[2] + GetCenter(kAxisZ,kMaster);

      }


    }



  }//SetPoint

  void SpaceMap3::PrintPoint() {

    cout<<__FUNCTION__<<endl;
    cout<<" Master: "<<fMaster[kAxisX]<<" "<<fMaster[kAxisY]<<" "<<fMaster[kAxisZ]<<endl;

  }

  double SpaceMap3::GetCenter(const Axis& a, const Ref& R) {

    if( R == kMaster ) {

      return 0.5*( GetAxis(a)->GetXmin() + GetAxis(a)->GetXmax() );

    }else if( R == kGlobal ) {

      //return fCenter[a]; //GetCenter(a,kMaster) + GetTranslation(a,kMaster,kGlobal);
      return GetCenter(a,kMaster) + fTranslation[a];

    }else if(R == kLocal ) {

      return 0;
    }


    return 0;

  }


  //
  //
  //

  void SpaceMap3::SetRefSystem(const Ref& r) {

    refSyst = r;

  }

  void SpaceMap3::Init() {
	/**
	 *
	 */



    fCalibMap = nullptr;

    fGlobNativeScale = 1;
    fGlobScale = 1;
    fGlobOffset = 0;

    SetRefSystem(kGlobal);
    ResetRefSystem();
  }

  SpaceMap3* SpaceMap3::map() const {

	return const_cast<SpaceMap3*>(this);
  }


  ///////////////////////////////////////////////////////
  //
  //
  //
  //
  //
  //
  ////////////////////////////////////////////////////////

TGraph2D* SpaceMap3::BuildGraph2D(const Axis& afirst, Axis asecond, const TString& name, const TString& tit
		       ,double third
		       ,int n1, double min1, double max1
		       ,int n2, double min2, double max2)
{


  TGraph2D *g = new TGraph2D();
  g->SetName(name);
  g->SetTitle(tit);


  Axis athird;

  if( (afirst == kAxisX && asecond == kAxisY)
      || (afirst == kAxisY && asecond == kAxisX)){

    athird = kAxisZ;
  }

  if( (afirst == kAxisX && asecond == kAxisZ)
      || (afirst == kAxisZ && asecond == kAxisX)){

    athird = kAxisY;
  }


  if( (afirst == kAxisZ && asecond == kAxisY)
      || (afirst == kAxisY && asecond == kAxisZ)){

    athird = kAxisX;
  }


  if( !IsInRange(athird,third) ) return g;

  double var[3] = {0,0,0};

  double range_min[2];
  double range_max[2];

  range_min[afirst] = TMath::Max(min1, GetBinCenterMin(afirst) );
  range_max[afirst] = TMath::Min(max1, GetBinCenterMax(afirst) );

  range_min[asecond] = TMath::Max(min2, GetBinCenterMin(asecond) );
  range_max[asecond] = TMath::Min(max2, GetBinCenterMax(asecond) );


  var[athird]=third;


  if( n1<2 || n1<2) return g;


  for(int ip1=0; ip1<n1; ++ip1){
    for(int ip2=0; ip2<n2; ++ip2){

      var[afirst] = range_min[afirst] + (range_max[afirst]-range_min[afirst])*ip1/(n1-1);

      var[asecond] = range_min[asecond] + (range_max[asecond]-range_min[asecond])*ip2/(n2-1);


      double val = Evaluate(var[0],var[1],var[2]);

      g->SetPoint(g->GetN(),var[afirst],var[asecond],val);

    }
  }

  return g;


}



TGraph2D* SpaceMap3::BuildGraphXY(const TString& name, const TString& tit
		       ,double z
		       ,int ny, double ymin, double ymax
		       ,int nx, double xmin, double xmax)
{


  TGraph2D *g = new TGraph2D();
  g->SetName(name);
  g->SetTitle(tit);


  if( !IsInRangeZ(z) ) return g;

  double x_min = TMath::Max(xmin, GetBinCenterMin(kAxisX) );
  double x_max = TMath::Min(xmax, GetBinCenterMax(kAxisX) );

  double y_min = TMath::Max(ymin, GetBinCenterMin(kAxisY) );
  double y_max = TMath::Min(ymax, GetBinCenterMax(kAxisY) );

  if( nx<2 || ny<2) return g;


  for(int ipx=0; ipx<nx; ++ipx){
    for(int ipy=0; ipy<ny; ++ipy){

      double xp = x_min + (x_max-x_min)*ipx/(nx-1);
      double yp = y_min + (y_max-y_min)*ipy/(ny-1);

      double val = Evaluate(xp,yp,z);

      g->SetPoint(g->GetN(),xp,yp,val);

    }
  }


  return g;

}

TGraph* SpaceMap3::BuildGraphX(const TString& name, const TString& tit
			       , double y, double z, int nbins, double xmin, double xmax){
  /**
   *
   *
   */

  TGraph *g = new TGraph();
  g->SetName(name);
  g->SetTitle(tit);


  if( !IsInRangeY(y) ) return g;
  if( !IsInRangeZ(z) ) return g;

  double x_min = TMath::Max(xmin, GetBinCenterMin(kAxisX) );
  double x_max = TMath::Min(xmax, GetBinCenterMax(kAxisX) );

  if( nbins < 2) {
    return g;
  }

  for(int ip=0; ip<nbins; ++ip){

    double xp = x_min + (x_max-x_min)*ip/(nbins-1);

    double val = Evaluate(xp,y,z);

    g->SetPoint(g->GetN(),xp,val);
  }



  return g;

}

TGraph* SpaceMap3::BuildGraphY(const TString& name, const TString& tit
			       , double second, double third, int nbins, double vmin, double vmax){

  return BuildGraph(kAxisX,name,tit,second,third,nbins,vmin,vmax);

}


TGraph* SpaceMap3::BuildGraphZ(const TString& name, const TString& tit
			       , double second, double third, int nbins, double vmin, double vmax){

  return BuildGraph(kAxisZ,name,tit,second,third,nbins,vmin,vmax);

}


TGraph* SpaceMap3::BuildGraph(const Axis& ax, const TString& name, const TString& tit
			       , double second, double third, int nbins, double vmin, double vmax){
  /**
   *   first, second, third
   *   x y z
   *   y z x
   *   z x y
   */

  TGraph *g = new TGraph();
  g->SetName(name);
  g->SetTitle(tit);

  double var[3] = {0.,0.,0.};

  if( ax == kAxisX ){

    if( !IsInRange(kAxisY,second)) return g;
    if( !IsInRange(kAxisZ,third)) return g;

    var[kAxisY] = second;
    var[kAxisZ] = third;

  }else if(ax==kAxisY){

    if( !IsInRange(kAxisZ,second)) return g;
    if( !IsInRange(kAxisX,third)) return g;

    var[kAxisZ] = second;
    var[kAxisX] = third;


  }else if(ax==kAxisZ){

    if( !IsInRange(kAxisX,second)) return g;
    if( !IsInRange(kAxisY,third)) return g;

    var[kAxisX] = second;
    var[kAxisY] = third;

  }else{

    return g;
  }



  double v_min = TMath::Max(vmin, GetBinCenterMin(ax) );
  double v_max = TMath::Min(vmax, GetBinCenterMax(ax) );


  if( nbins < 2) {
    return g;
  }

  for(int ip=0; ip<nbins; ++ip){

    double vp = v_min + (v_max-v_min)*ip/(nbins-1);

    var[ax]=vp;

    double val = Evaluate(var[0],var[1],var[2]);

    g->SetPoint(g->GetN(),vp,val);
  }

  return g;

}

TMultiGraph* SpaceMap3::BuildMultiGraph(const Axis& first, Axis scan, const TString& name, const TString& tit
			     ,double third
			     ,int n1, double min1, double max1
			     ,int n2, double min2, double max2)
{


  TMultiGraph* mg = new TMultiGraph();
  mg->SetName(name);
  mg->SetTitle(tit);


  if( n1< 2 || n2 < 2) return mg;

  for(int i=0; i<n2; ++i){

    double v = min2 + (min2-min1)*i/(n2-1);

    TGraph *g = BuildGraph(first,"","",v,third,n1,min1,max1);

    mg->Add(g);
  }



  return mg;

}


double SpaceMap3::InterpolateAtEdge(double x, double y, double z)
{
  int binx = GetXaxis()->FindBin(x);
  int biny = GetYaxis()->FindBin(y);
  int binz = GetZaxis()->FindBin(z);

  //int Nx = GetXaxis()->GetNbins();
  //int Ny = GetYaxis()->GetNbins();
  //int Nz = GetZaxis()->GetNbins();
  
  double xc = GetXaxis()->GetBinCenter(binx);
  double yc = GetYaxis()->GetBinCenter(biny);
  double zc = GetZaxis()->GetBinCenter(binz);
  
  double val =  GetBinContent(binx,biny,binz);

  double xd, yd, zd;
  double x1, y1, z1;
  double x2, y2, z2;

  x1 = x2 = xc;
  y1 = y2 = yc;
  z1 = z2 = zc;

  if(IsBorderX(x))
    xd = 1;
  else{
    if( x <= xc) //on left side
      x1 = GetXaxis()->GetBinCenter(binx-1);
    else //right side
      x2 = GetXaxis()->GetBinCenter(binx+1);
    xd = (x-x1)/(x2-x1);
  }
 
  if(IsBorderY(y))
    yd = 1;
  else{
    if( y <= yc) //on left side
      y1 = GetYaxis()->GetBinCenter(biny-1);
    else //right side
      y2 = GetYaxis()->GetBinCenter(biny+1);
    yd = (y-y1)/(y2-y1);
  }
  if(IsBorderZ(z))
    zd = 1;
  else{
    if( z <= zc) //on left side
      z1 = GetZaxis()->GetBinCenter(binz-1);
    else //right side
      z2 = GetZaxis()->GetBinCenter(binz+1);
    zd = (z-z1)/(z2-z1);
  }
  
  double F111 = GetBinContent(FindBin(x1,y1,z1));
  double F112 = GetBinContent(FindBin(x1,y1,z2));
  double F121 = GetBinContent(FindBin(x1,y2,z1));
  double F122 = GetBinContent(FindBin(x1,y2,z2));
  double F211 = GetBinContent(FindBin(x2,y1,z1));
  double F212 = GetBinContent(FindBin(x2,y1,z2));
  double F221 = GetBinContent(FindBin(x2,y2,z1));
  double F222 = GetBinContent(FindBin(x2,y2,z2));
  
  val = F111*(1.-xd)*(1.-yd)*(1.-zd)+ F211*xd*(1.-yd)*(1.-zd)
      + F121*(1.-xd)*yd*(1.-zd)     + F221*xd*yd*(1.-zd)
      + F112*(1.-xd)*(1.-yd)*zd     + F212*xd*(1.-yd)*zd
      + F122*(1.-xd)*yd*zd          + F222*xd*yd*zd;
  return val;


}

double SpaceMap3::ExtrapolateAtEdge(double x, double y, double z){
  /*
    Want to get the value at x, close to the wall
    |----+----+--+--+---->
         x    c  a  b
    c is the center of the voxel (interpolation limit)
    b is the symetric of x wrt to c
    a is between c and b
    ->make linear interpolation from B, A to X
    
   */
  
  int binx = GetXaxis()->FindBin(x);
  int biny = GetYaxis()->FindBin(y);
  int binz = GetZaxis()->FindBin(z);


  //get coordinate of the center of the voxel
  double xcenter = IsBorderX(x) ? GetXaxis()->GetBinCenter(binx) : x;
  double ycenter = IsBorderY(y) ? GetYaxis()->GetBinCenter(biny) : y;
  double zcenter = IsBorderZ(z) ? GetZaxis()->GetBinCenter(binz) : z;

  int Nx = GetXaxis()->GetNbins();
  int Ny = GetYaxis()->GetNbins();
  int Nz = GetZaxis()->GetNbins();


  if (Nz==1){ // when the GAr map is used (only one Z bin at the moment)
    return InterpolateAtEdge(x,y,z);// LZ 
  }

  //special case when the starting point is at the center of the voxel
  if(x==xcenter && IsBorderX(x)){
    int binx2 = (binx==Nx) ? binx-1 : 2;
    xcenter =  0.5*(GetXaxis()->GetBinCenter(binx2)+x);
  }
  if(y==ycenter&& IsBorderY(y)){
    int biny2 = (biny==Ny) ? biny-1 : 2;
    ycenter =  0.5*(GetYaxis()->GetBinCenter(biny2)+y);
  }
  if(z==zcenter&& IsBorderZ(z)){

    int binz2 = (binz==Nz) ? binz-1 : 2;
    zcenter =  0.5*(GetZaxis()->GetBinCenter(binz2)+z);
  }
    

  //Get the symmetric of edgy point wrt to center
  double xb = IsBorderX(x) ? GetSymmetric(x,xcenter) : x;
  double yb = IsBorderY(y) ? GetSymmetric(y,ycenter) : y;
  double zb = IsBorderZ(z) ? GetSymmetric(z,zcenter) : z;

  //Get middle point of [symmetric, center]
  double xa = IsBorderX(x) ?  0.5*(xcenter+xb) : x;
  double ya = IsBorderY(y) ?  0.5*(ycenter+yb) : y;
  double za = IsBorderZ(z) ?  0.5*(zcenter+zb) : z;
 
  double valb = Interpolate(xb, yb, zb); //symmetric point value
  double vala =  Interpolate(xa, ya, za);
  double val = 0.;

  double Nedges = 0.;
  if(IsBorderX(x)) Nedges = Nedges+1.;
  if(IsBorderY(y)) Nedges = Nedges+1.;
  if(IsBorderZ(z)) Nedges = Nedges+1.;
    


  double delta = vala - valb;//

  double dx = IsBorderX(x) ? xa-xb : 1;
  double dy = IsBorderY(y) ? ya-yb : 1;
  double dz = IsBorderZ(z) ? za-zb : 1;

  val = x*delta/dx + (xa*valb - xb*vala)/dx
      +  y*delta/dy + (ya*valb - yb*vala)/dy
      +  z*delta/dz + (za*valb - zb*vala)/dz;


  val = val/Nedges;
  return val;
  
}
double SpaceMap3::GetSymmetric(double x, double xc){
  return xc + (xc-x); 
}



} /* namespace Map */
