/////////////////////////////////////////////////////////////////////////////////////
//
//  Primary function is to assign the channel / time bin number
//  Also model longitudinal and transverse diffusion with projection onto 2D plane
//
//
//  Created: Fri Oct 16 15:09:34 CEST 2015
//
/////////////////////////////////////////////////////////////////////////////////////

#include <iomanip>
#include <fstream>
#include <sstream>

#include <TROOT.h>

#include "QProjector.h"

using namespace std;


//////////////////////////////////////////////////////////////
// Class to handle gaussian integrals
//

// GIntegral ctor
GIntegral::GIntegral(double nsgm, double step)
{
   fStep = step;
   fMaxX = nsgm;

   fSpline = NULL;
   ComputeSpline();

}

// GIntegral dtor
GIntegral::~GIntegral()
{
   delete fSpline;
}

// reset the range
void GIntegral::SetRange(int nsgm)
{
   fMaxX = nsgm;
   ComputeSpline();
}

// compute new spline
void GIntegral::ComputeSpline()
{
   if(fSpline) delete fSpline;
   // compute the spline points
   std::vector<double> xd, yd;
   double xval = 0.0;
   while(xval < fMaxX )
   {
      xd.push_back(xval);
      yd.push_back(0.5*TMath::Erf(xval/TMath::Sqrt(2.0)));
      xval += fStep;
   }

   fSpline = new TSpline3("GIngegralTSpline3", &xd[0], &yd[0], xd.size());
}

// check integration range
bool  GIntegral::CheckRange(double &val)
{
   if(fabs(val) > fMaxX)
   {
      //std::cerr<<"ERROR ["<<__FILE__<<", "<<__LINE__<<"]: "
      //<<"Integration limit "<< val <<" exceeds range "<<fMaxX<<std::endl;
      val = val/fabs(val)*fMaxX;

      return false;
   }
   return true;
}

// do integral (private method)
double GIntegral::Integral(double x1, double x2)
{
   double y1 = 0;
   double y2 = 0;
   if(x1 != 0)
   y1 = fSpline->Eval(x1);

   y2 = fSpline->Eval(x2);

   return (y2 - y1);
}


// get integral value (public method)
double GIntegral::GetIntegral(double x1, double x2)
{
   double fval = 0;

   // check if the range is within max limit
   CheckRange(x1);
   CheckRange(x2);

   if(x1 == x2)
   return 0.0;

   if(x1 < 0 && x2 < 0)
   {
      if(x1 < x2 )
      {
         fval = Integral( fabs(x2), fabs(x1) );
         return fval;
      }
      else
      {
         fval = Integral( fabs(x1), fabs(x2) );
         return fval;
      }
   }
   else if (x1 < 0 && x2 >= 0)
   {
      fval  = Integral(0, fabs(x1));
      fval += Integral(0, x2);
      return fval;
   }
   else if(x1 >= 0 && x2 < 0)
   {
      fval  = Integral(0, x1);
      fval += Integral(0, fabs(x2));
      return fval;
   }
   else
   {
      if(x1 > x2)
      {
         fval = Integral(x2, x1);
         return fval;
      }
      else
      {
         fval = Integral(x1, x2);
         return fval;
      }
   }

   return fval;
}

// perform bin integrals for a given range
void GIntegral::BinIntegral(double mu, double sigma,
   double x1, double x2, double step,
   double norm, double epszero,
   std::vector< std::pair<double, double> > &res)
   {
      res.clear();
      //double tmp1 =  (x1 - mu) / sigma;
      //double tmp2 =  (x2 - mu) / sigma;
      //if(!CheckRange(tmp1) || !CheckRange(tmp2))
      //cout << " Mean = "<<mu<<" sigma = "<<sigma<<" x1 = "<<x1<<" x2 = "<<x2<<" step = "<<step<<endl;

      if(x1 == x2)
      {
         res.push_back( std::make_pair(x1, norm) );
         return;
      }

      double startx = x1;
      double endx   = x2;

      if(x1 > x2)
      {
         startx = x2;
         endx   = x1;
      }


      while(startx < endx)
      {
         // integration range nomalized to
         double xb1 = startx - mu;
         double xb2 = xb1 + 0.999999*step;

         double w = GetIntegral(xb1/sigma, xb2/sigma);
         double y = norm * w;


         if(y < epszero)
         {
            if(!res.empty()) //on the other side of Gaussian
            break;

            startx += step;
            continue;
         }

         //std::cout<<setw(10)<<xb1<<setw(10)<<xb2<<setw(15)<<y<<endl;

         res.push_back( std::make_pair(startx, y) );
         startx += step;
      }
   }


//////////////////////////////////////////////////////////////
// class for projection diffuse cloud on 2D anode

QProjectorCRM::QProjectorCRM( double posX0, double posY0,
			      double pitchX, double pitchY, double pitchT,
			      unsigned int nchx, unsigned int nchy,
			      unsigned int nsamples )
{
  fPosX0 = posX0;
  fPosY0 = posY0;
  
  fPitchX = pitchX;
  fPitchY = pitchY;
  fPitchT = pitchT;
  
  fNChX   = nchx;
  fNChY   = nchy;
  fNSamT  = nsamples;
  
  // active volume
  fWidthX = fPitchX * fNChX;
  fWidthY = fPitchY * fNChY;
  fMaxT   = fPitchT * fNSamT;
  
  fNSgm  = 5.0;    //number of standard deviations
  fQzero = 0.00005; //in fC
  
  fGausIntegrator = NULL;
  BuildGausIntegrator( 2*fNSgm );
  
  fCollAnode = new LArMCTruth::AnodeTrueQ();
  
  
  //
  fAnodeEffMap = NULL;
}

//
QProjectorCRM::~QProjectorCRM()
{
  if( fGausIntegrator )
    delete fGausIntegrator;

  if( fAnodeEffMap )
    delete fAnodeEffMap;
  
  delete fCollAnode;
}

//
void QProjectorCRM::BuildGausIntegrator(int nsgm)
{
  if( fGausIntegrator )
    fGausIntegrator->SetRange(nsgm);
  
  fGausIntegrator = new GIntegral( nsgm );
}

//
int QProjectorCRM::GetTimeBin(double tval) const
{
  float val = (float)(tval / fPitchT);
  if (val < 0)
    return -2;
  
  if(val > (float) fNSamT)
    return -1;
  
  return (int)(val);
}


//
int QProjectorCRM::GetChNum(double pos, int iview) const
{
  double pitch = 0;
  double pos0  = 0;
  UInt_t nch   = 0;

  if(iview == 0)
    {
      pitch = fPitchX;
      pos0  = fPosX0;
      nch   = fNChX;
    }
  else if(iview == 1)
    {
      pitch = fPitchY;
      pos0  = fPosY0;
      nch   = fNChY;
    }
  else
    {
      std::cerr<<"ERROR: view "<<iview<<" is not defined"<<std::endl;
      return -1;
    }

  double tmp = (pos - pos0)/pitch + nch / 2.0;

  if(tmp < 0)
    {
      //cerr<<"ERROR: channel assignemnt failed for CRM at "<<pos0<<endl
      //<<"       and charge deposition at "<<pos<<endl;
      return -1;
    }

  int chnum = (int)(floor(tmp));
  if(chnum >= (int)nch)
    {
      //cerr<<"ERROR: channel assignment failed for CRM at "<<pos0<<endl
      //<<"       and charge deposition at "<<pos<<endl
      //<<"       Channel count exceeds maximum declared"<<nch<<endl;
      return -1;
    }

  return chnum;
}

//
//
//
double QProjectorCRM::GetEff_2D(int channelx, int channely )
{
  double eff = 1;
  if( fAnodeEffMap )
    {
      int distx = channelx % 160 + 1;
      int disty = channely % 160 + 1;
      if( distx <= fAnodeEffMap->GetNbinsX() && disty <= fAnodeEffMap->GetNbinsY() )
	eff = fAnodeEffMap->GetBinContent(distx, disty);
    }
  return eff;
}

//
const LArMCTruth::AnodeTrueQ*  QProjectorCRM::Projection( double posx, double posy,
							  double tarr, double Q, 
							  double sigmaL, double sigmaT )
{
  // clear any stuff in the vector
  fCollAnode->Clear();
  
  // longitudinal diffusion sigmaL in us
  // transverse diffusion sigmaT in mm

  std::map<int, double> mapQtime;
  std::map<int, double> mapChX;
  std::map<int, double> mapChY;

  int bint0, chx0, chy0;
  double timeLimLow, timeLimHigh;
  double xLimLow, xLimHigh;
  double yLimLow, yLimHigh;

  double vw0_Qint = 1;
  double vw1_Qint = 1;

  // get integration limits
  if(!GetLimits(posx, sigmaT, chx0, xLimLow, xLimHigh, 0))
    return NULL;
  if(!GetLimits(posy, sigmaT, chy0, yLimLow, yLimHigh, 1))
    return NULL;
  if(!GetLimits(tarr, sigmaL, bint0, timeLimLow, timeLimHigh, 2))
    return NULL;


  if( sigmaL <= 0 )
    {
      if( bint0 < 0 )
	return NULL;
      else
	mapQtime[bint0] = 1;
    }
  else if(sigmaL > 0) // calculate integrals in time
    {
      if( fPitchT / sigmaL >= 2 * (fNSgm - 1) ) // the pitch is much larger than diffusion
	{
	  if( bint0 < 0 )
	    return NULL;

	  mapQtime[bint0] = 1;
	}
      else
	{
	  std::vector< std::pair<double, double> > res;
	  fGausIntegrator->BinIntegral(tarr, sigmaL, timeLimLow, timeLimHigh, fPitchT, 1, fQzero, res);

	  // iterator over the results
	  for(std::vector< std::pair<double, double> >::iterator it = res.begin();
	      it != res.end(); it++)
	    {
	      // time bin
	      int tbin = it->first / fPitchT;
	      mapQtime[tbin] = it->second;
	    }
	}
    }

  if( sigmaT <= 0 )
    {
      if( chx0 < 0 || chy0 < 0 )
	return NULL;

      mapChX[chx0] = 1;
      mapChY[chy0] = 1;
    }
  else if(sigmaT > 0) // do integrals
    {
      std::vector< std::pair<double, double> > res;

      if( fPitchX / sigmaT >= 2 * (fNSgm - 1) ) // the pitch is much larger than diffusion
	{
	  if( chx0 < 0 )
	    return NULL;

	  mapChX[chx0] = 1;
	}
      else
	{

	  // -- view 0
	  fGausIntegrator->BinIntegral(posx, sigmaT, xLimLow, xLimHigh, fPitchX, 1, fQzero, res);

	  // total integral in view 0
	  vw0_Qint = fGausIntegrator->GetIntegral((xLimLow-posx)/sigmaT, (xLimHigh-posx)/sigmaT);
	  // iterator over the results
	  for(std::vector< std::pair<double, double> >::iterator it = res.begin();
	      it != res.end(); it++)
	    {
	      //
	      int ch = GetChNum(it->first, 0);
	      if(ch < 0 ) //should not be
		{
		  std::cerr<<"ERROR: "<<__FILE__<<" channel number was not assigened properly"<<std::endl;
		  //std::cerr<<"        "<<ch<<" "<<it->first<<" "<<posx<<" "<<sigmaT<<std::endl;
		  //std::cerr<<"        "<<xLimLow<<" "<<xLimHigh<<std::endl;
		  continue;
		}
	      mapChX[ch] = it->second;
	    }
	}

      //res.clear();

      if( fPitchY / sigmaT >= 2 * (fNSgm - 1) ) // the pitch is much larger than diffusion
	{
	  if( chy0 < 0 )
	    return NULL;

	  mapChY[chy0] = 1;
	}
      else
	{
	  // -- view 1
	  fGausIntegrator->BinIntegral(posy, sigmaT, yLimLow, yLimHigh, fPitchY, 1, fQzero, res);
	  // total integral in view 1
	  vw1_Qint = fGausIntegrator->GetIntegral((yLimLow-posy)/sigmaT, (yLimHigh-posy)/sigmaT);
	  // iterator over the results
	  for(std::vector< std::pair<double, double> >::iterator it = res.begin();
	      it != res.end(); it++)
	    {
	      //
	      int ch = GetChNum(it->first, 1);
	      if(ch < 0 ) //should not be
		{
		  std::cerr<<"ERROR: "<<__FILE__<<" channel number was not assigened properly"<<std::endl;
		  //std::cerr<<"        "<<ch<<" "<<it->first<<" "<<posy<<" "<<sigmaT<<std::endl;
		  //std::cerr<<"        "<<yLimLow<<" "<<yLimHigh<<std::endl;
		  continue;
		}
	      mapChY[ch] = it->second;
	    }
	}
    }


  //now do the main loops

  /////////
  // view 0
  for(std::map<int, double>::iterator it = mapChX.begin(); it!=mapChX.end();it++){
    LArMCTruth::ChanTrueQ chq((UInt_t)it->first);
    double qnorm_1=0;
    // normalization charge : Q times integral over Y of each strip x diffusion part
    qnorm_1 = Q * it->second;
    double eff=1;

    std::vector<float> tdata;
    UInt_t tstart = 0;

    for(std::map<int, double>::iterator itt = mapQtime.begin(); itt != mapQtime.end();itt++){
      // get the time index
      if(tdata.empty()){ tstart = (UInt_t)itt->first;}
      double qnorm=0;

      if((int)tstart > itt->first){
	std::cerr<<"ERROR: "<<__FILE__<<" looks like time indexes are not ordered"<<std::endl;
      }
      for(std::map<int, double>::iterator jt = mapChY.begin(); jt!=mapChY.end();jt++){
	eff = GetEff_2D(it->first,jt->first);
	qnorm+=eff*qnorm_1*jt->second;
      }
      tdata.push_back( (float)(qnorm * itt->second) );
    }
    chq.SetData( tstart, tdata);

    fCollAnode->AddChanData(chq, 0);
  }

  /////////
  // view 1
  for(std::map<int, double>::iterator it = mapChY.begin(); it!=mapChY.end();it++){
    LArMCTruth::ChanTrueQ chq((UInt_t)it->first);

    // normalization charge : our charge x integral over X of each strip x diffusion part
    double qnorm_1 = Q * it->second;
    double eff=1;

    std::vector<float> tdata;
    UInt_t tstart = 0;
    for(std::map<int, double>::iterator itt = mapQtime.begin(); itt != mapQtime.end();itt++){
      // get the time index
      if(tdata.empty()) {tstart = itt->first;}
      double qnorm=0;

      if((int)tstart > itt->first) {
	std::cerr<<"ERROR: "<<__FILE__<<" looks like time indexes are not ordered"<<std::endl;
      }
      for(std::map<int, double>::iterator jt = mapChY.begin(); jt!=mapChY.end();jt++){
	eff = GetEff_2D(it->first,jt->first);
	qnorm+=eff*qnorm_1*jt->second;
      }
      tdata.push_back( (float)(qnorm * itt->second) );
    }

    chq.SetData( tstart, tdata );

    fCollAnode->AddChanData(chq, 1);
  }

  //  fCollAnode->Print();
  //cout << timeLimLow <<" " <<timeLimHigh << endl;
  //cout << xLimLow <<" "<<xLimHigh<<" "<<fPosX0<<endl;
  //cout << yLimLow <<" "<<yLimHigh<<" "<<fPosY0<<endl;
  //cout <<sigmaT<<" "<<sigmaL<<endl;
  //cout <<" "<<(xLimLow-posx)/sigmaT <<" "<< (xLimHigh-posx)/sigmaT<<endl;
  //cout <<" "<<(yLimLow-posy)/sigmaT <<" "<< (yLimHigh-posy)/sigmaT<<endl;
  //cout <<" integrals in x and y "<<vw0_Qint<<" "<<vw1_Qint<<endl;

  // that is it!
  return fCollAnode;
}

//
bool QProjectorCRM::GetLimits(double pos, double sigma, int &binv, double &x1, double &x2, int iType)
{
  double minPos = 0;
  double maxPos = 0;
  double step   = 0;

  binv = 0;
  x1 = x2 = 0;


  if(sigma < 0) sigma = 0;   // in case it is negative

  if(iType == 0) // view 0
    {
      //
      minPos = fPosX0 - 0.5*fWidthX;
      maxPos = fPosX0 + 0.5*fWidthX;
      step   = fPitchX;
      binv   = GetChNum(pos, 0);
    }
  else if(iType == 1) // view 1
    {
      //
      minPos = fPosY0 - 0.5*fWidthY;
      maxPos = fPosY0 + 0.5*fWidthY;
      step   = fPitchY;
      binv   = GetChNum(pos, 1);

    }
  else if(iType == 2) // time view
    {
      //
      minPos = 0;
      maxPos = fMaxT;
      step   = fPitchT;
      binv   = GetTimeBin(pos);
    }
  else
    return false;

  //cout << iType <<" "<<pos<<" "<<sigma << endl;
  //cout << fPosX0<<" "<<fWidthX<<endl;
  //cout << minPos <<" "<< maxPos <<" "<<step<<" "<<binv<<endl;


  double posbin  = ((int)(pos/step)) * step;
  double distbin = ((int)((fNSgm * sigma)/step)) * step;
  //append step to bin distance until required Nsigma is reached
  if(distbin < fNSgm * sigma)
    distbin += step;

  // lower limit
  x1 = posbin - distbin;
  if(x1 < minPos)
    x1 = minPos;
  else if( x1 >= maxPos )
    return false;

  // upper limit
  x2 = posbin + distbin;
  if(x2 > maxPos)
    x2 = maxPos;
  if(x2 <= minPos)
    return false;

  return true;
}


// fill histo
void QProjectorCRM::FillHisto(TH2F *histo, int iview)
{
  if(!histo) return;

  if(iview == 0)
    {
      const std::vector<LArMCTruth::ChanTrueQ*> viewCh = fCollAnode->View0();
      for(size_t i=0;i<viewCh.size();i++)
	{
	  const std::vector<Float_t> tdata = viewCh[i]->GetData();
	  UInt_t tbin = viewCh[i]->GetTstart();
	  UInt_t ch   = viewCh[i]->GetChan();

	  for(size_t j=0;j<tdata.size();j++)
	    {
	      int xbin = ch + 1;
	      int ybin = tbin + 1 + j;
	      if( (xbin >= 1 && xbin <= histo->GetNbinsX()) &&
		  (ybin >= 1 && ybin <= histo->GetNbinsY()) )
		histo->Fill(xbin-0.5, ybin-0.5, tdata[j]);
	    }
	}
    }
  else if(iview == 1)
    {
      const std::vector<LArMCTruth::ChanTrueQ*> viewCh = fCollAnode->View1();
      for(size_t i=0;i<viewCh.size();i++)
	{
	  const std::vector<Float_t> tdata = viewCh[i]->GetData();
	  UInt_t tbin = viewCh[i]->GetTstart();
	  UInt_t ch   = viewCh[i]->GetChan();

	  for(size_t j=0;j<tdata.size();j++)
	    {
	      int xbin = ch+1;
	      int ybin = tbin + 1 + j;
	      if( (xbin >= 1 && xbin <= histo->GetNbinsX()) &&
		  (ybin >= 1 && ybin <= histo->GetNbinsY()) )
		histo->Fill(xbin-0.5, ybin-0.5, tdata[j]);
	    }
	}
    }

}


//
//
//
void QProjectorCRM::LoadEffFile(std::string fname)
{
  if( fAnodeEffMap ) fAnodeEffMap->Delete();
  fAnodeEffMap = NULL;

  if( fname.empty() ) return;
  
  
  //
  ifstream in( fname.c_str() );
  if( !in.is_open() ) return;
  
  std::string hname = "anode_eff_map";
  int cycle = 0;
  std::stringstream oss;
  do
    {
      oss<<hname<<cycle;
      cycle++;
    } while(gROOT->FindObject( oss.str().c_str() ));
  
  hname = oss.str();

  fAnodeEffMap = new TH2D(hname.c_str(),hname.c_str(),
			  160, 0, 159, 160, 0, 159);
  fAnodeEffMap->SetDirectory(0);
  
  //
  int nx = fAnodeEffMap->GetNbinsX();
  int ny = fAnodeEffMap->GetNbinsY();
  
  //
  for(int i=1;i<=nx;i++)
    for(int j=1;j<=ny;j++)
      fAnodeEffMap->SetBinContent(i, j, 1);
  
  // read file
  std::string line;                                                             
  while( getline(in, line ) )                                                   
    {                                                                           
      if(line.empty()) continue;                                                
                                                                                
      // strip leading 0 if any                                                 
      size_t pos = line.find_first_not_of(' ');                                 
      if( pos == std::string::npos )                                            
        continue;                                                               
      
      std::stringstream ss(line);
      
      int x,y;
      double eff;
      ss>>x>>y>>eff;
      
      fAnodeEffMap->SetBinContent(x+1, y+1, eff);
    }
  in.close();
  
  // 
  for(int i=1;i<=nx;i++)
    {
      for(int j=1;j<=ny;j++)
	{
	  if( i == 1 || j == 1 || i == nx || j == ny )
	    fAnodeEffMap->SetBinContent(i, j, 0);
	  else if( i == 2 || j == 2 || i == nx - 1 || j == ny - 1 )
	    {
	      double tmp = fAnodeEffMap->GetBinContent( i, j );
	      fAnodeEffMap->SetBinContent(i, j, 0.5*tmp);
	      if( (i == 2 && j == 2) || (i == nx-1 && j == ny - 1) ||
		  (i == 2 && j == ny - 1) || (i == nx-1 && j == 2 ) )
		fAnodeEffMap->SetBinContent(i,j, 0.2);
	    }
	  
	}
    }
}
