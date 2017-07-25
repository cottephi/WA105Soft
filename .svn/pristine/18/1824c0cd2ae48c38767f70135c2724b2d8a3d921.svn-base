////////////////////////////////////////////////////////////////////////////////////
//
//  Object to book quantities to analyze charge collection in a CRP
//  
//  
//
//
////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <algorithm>

#include "CRPQAnaStore.h"
#include "RecoUtils.h"
#include "RootUtils.h"

using namespace std;


//
//
//
CRPQAnaStore::CRPQAnaStore( std::string name, double x0, double y0,
			    double x1, double y1 )
{
  fName = name;
  
  fX0 = x0;
  fX1 = x1;
  fY0 = y0;
  fY1 = y1;

  if(fX1 < fX0) std::swap(fX0, fX1);
  if(fY1 < fY0) std::swap(fY0, fY1);

  fRlo = 0;
  fRhi = 0.3; //remove 30% from the high Q end

  // default binning
  vector<double> xbins(2), ybins(2);
  xbins[0] = fX0;
  xbins[1] = fX1;
  
  ybins[0] = fY0;
  ybins[1] = fY1;

  fCRPArea = NULL;
  InitAreaSegs( xbins, ybins );
}

//
//
//
CRPQAnaStore::~CRPQAnaStore()
{
  fCRPArea->Delete();
}


//
//
//
void CRPQAnaStore::InitAreaSegs(std::vector<double> &xbins, 
				std::vector<double> &ybins)
{
  fAreaBins.clear();
  
  for(size_t i=0;i<ybins.size()-1;i++)
    {
      double y0 = ybins[i];
      double y1 = ybins[i+1];
      for(size_t j=0;j<xbins.size()-1;j++)
	{
	  double x0 = xbins[j];
	  double x1 = xbins[j+1];
	  CRPArea_t crpseg;
	  crpseg.xc0 = x0;
	  crpseg.xc1 = x1;
	  crpseg.yc0 = y0;
	  crpseg.yc1 = y1;
	  	  
	  fAreaBins.push_back( crpseg );
	}
    }

  // remove histograms
  if( fCRPArea ) fCRPArea->Delete();
  const char *hname = Form("%s_crpmap", fName.c_str());
  fCRPArea = new TH2F( hname, hname, 
		       xbins.size()-1, &xbins[0], 
		       ybins.size()-1, &ybins[0] );
  fCRPArea->SetDirectory(0);
  fCRPArea->GetXaxis()->SetTitle("X");
  fCRPArea->GetYaxis()->SetTitle("Y");
  fCRPArea->GetZaxis()->SetTitle("Tr<dQ/dx> (fC/mm)");
}

//
// break area into segments
//
void CRPQAnaStore::BreakIntoSegments(double dx, double dy)
{
  int nx = (int)((fX1 - fX0) / dx);
  int ny = (int)((fY1 - fY0) / dy);
  
  dx = (fX1 - fX0)/nx;
  dy = (fY1 - fY0)/ny;

  vector<double> xbins(nx+1);
  vector<double> ybins(ny+1);
  
  for(int i=0;i<=nx;i++)
    xbins[i] = fX0 + i*dx;

  for(int i=0;i<=ny;i++)
    ybins[i] = fY0 + i*dy;
  
  InitAreaSegs( xbins, ybins );
}


//
// based on the present accumulated statistics
//
void CRPQAnaStore::ComputeAreaMap()
{
  
  for(size_t i=0;i<fAreaBins.size();i++)
    {
      fAreaBins[i].dq0     = 0;
      fAreaBins[i].dq0_err = 0;
      fAreaBins[i].dq1     = 0;
      fAreaBins[i].dq1_err = 0;
      fAreaBins[i].dq_tot  = 0;
      
      // requires at least some statistics in each view
      if( fAreaBins[i].dqdx0.size() < 10 || fAreaBins[i].dqdx1.size() < 10 ) 
	continue;
      
      // calculate truncated means
      double v0 = RecoUtils::TruncatedMean( fAreaBins[i].dqdx0, fRlo, fRhi );
      double v1 = RecoUtils::TruncatedMean( fAreaBins[i].dqdx1, fRlo, fRhi );
      fAreaBins[i].dq_tot = v0 + v1;
      
      if(fAreaBins[i].dq_tot < 0) 
	{
	  fAreaBins[i].dq_tot = 0;
	  continue;
	}

      // 
      fAreaBins[i].dq0 = v0;
      fAreaBins[i].dq1 = v1;

      // better estimation of uncertainty TO DO
      fAreaBins[i].dq0_err = fAreaBins[i].dq0 / sqrt( fAreaBins[i].dqdx0.size() );
      fAreaBins[i].dq1_err = fAreaBins[i].dq0 / sqrt( fAreaBins[i].dqdx1.size() );
    }
  
  // no fill fCRPArea
  int nx = fCRPArea->GetNbinsX();
  int ny = fCRPArea->GetNbinsY();
  
  // clear previous bin content
  fCRPArea->Reset();
  
  for( int i=1;i<=ny;i++)
    {
      for(int j=1;j<=nx;j++)
	{
	  int idx = (j-1) + (i-1)*nx;
	  if( fAreaBins[idx].dq_tot == 0 ) continue;
	  fCRPArea->SetBinContent(j,i,fAreaBins[idx].dq_tot);
	}
    }
}
 
//
// clear for the next round
//
void CRPQAnaStore::Reset()
{
  fCRPArea->Reset();
  
  for(size_t i=0;i<fAreaBins.size();i++)
    {
      fAreaBins[i].dqdx0.clear();
      fAreaBins[i].dqdx1.clear();
    }
}


  //
//
//
void CRPQAnaStore::Fill( std::vector<double> &dqdx0, std::vector<TVector3> &path0,
			 std::vector<double> &dqdx1, std::vector<TVector3> &path1 )
{
  for(size_t p=0;p<path0.size();p++)
    {
      double x = path0[p].X();
      double y = path0[p].Y();

      int binn = FindBin(x, y, fCRPArea);
      if(binn < 0 || binn >= (int) fAreaBins.size()) continue;
      fAreaBins[binn].dqdx0.push_back( dqdx0[p] );
    }

  for(size_t p=0;p<path1.size();p++)
    {
      double x = path1[p].X();
      double y = path1[p].Y();

      int binn = FindBin(x, y, fCRPArea);
      if(binn < 0 || binn >= (int) fAreaBins.size()) continue;
      fAreaBins[binn].dqdx1.push_back( dqdx1[p] );
    }
}


//
//
//
ssize_t CRPQAnaStore::GetBinContent( size_t bin, int iview, double &v, double &ve ) const
{
  if( bin >= fAreaBins.size() ) return -1;
  
  if(iview == 0)
    {
      v  = fAreaBins[bin].dq0;
      ve = fAreaBins[bin].dq0_err;
    }
  else if(iview == 1)
    {
      v  = fAreaBins[bin].dq1;
      ve = fAreaBins[bin].dq1_err;
    }
  else
    {
      v  = fAreaBins[bin].dq_tot;
      ve = 0; // TO DO
    }
  
  return bin;
}


//
//
//
void CRPQAnaStore::Write(TDirectory *dirout)
{
  dirout->cd();
  fCRPArea->Write();

  for(size_t i=0;i<fAreaBins.size();i++)
    {
      TH1F htmp0(Form("%s_view0_bin%d", fName.c_str(), (int)i),
		 Form("%s_view0_bin%d", fName.c_str(), (int)i), 100, 0, 50); 
      htmp0.SetDirectory(0);
      htmp0.GetXaxis()->SetTitle("View 0 dQ/ds (fC/mm)");
      for(size_t j=0;j<fAreaBins[i].dqdx0.size();j++)
	RootUtils::FillConstantBinWidth( htmp0, fAreaBins[i].dqdx0[j] );
      
      TH1F htmp1(Form("%s_view1_bin%d", fName.c_str(), (int)i),
		 Form("%s_view1_bin%d", fName.c_str(), (int)i), 100, 0, 50); 
      htmp1.SetDirectory(0);
      htmp1.GetXaxis()->SetTitle("View 1 dQ/ds (fC/mm)");
      for(size_t j=0;j<fAreaBins[i].dqdx1.size();j++)
	RootUtils::FillConstantBinWidth( htmp1, fAreaBins[i].dqdx1[j] );
      
      dirout->cd();
      htmp0.Write();
      htmp1.Write();
    }
}
