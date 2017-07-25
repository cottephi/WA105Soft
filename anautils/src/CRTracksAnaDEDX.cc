////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//  Analysis module to look at cosmic ray charge loss for calibration of CRP gain
//  The calculation is performed for each view separetely to allow to study charge 
//  sharing uniformity between views
//
//
////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cmath>

#include "CRTracksAnaDEDX.h"
#include "LiquidArgonMedium.h"

using namespace std;

//
//
//
CRTracksAnaDEDX::CRTracksAnaDEDX(const RunHeader *rhead, const GeomConfig *ghead)
{
  // get geo info from global header
  // pitch is now in mm
  fPitch0 = 10*ghead->GetCRMpitchX(0); 
  fPitch1 = 10*ghead->GetCRMpitchY(0); 
  fPitchT = ghead->GetCRMpitchT();
  
  // field value in V/cm
  fEfield = rhead->GetEfieldValue(); 
  
  // drift velocity in cm/us
  fVdrift = 0.1*LiquidArgonMedium::Instance()->DriftVelocity(1.0E-3*fEfield); 

  fLifetime = -1;
  fEleAtten = -1;
  
  // pick up for MC the truth information by default
  if( rhead->GetDataType() > 1)
    {
      // electron lifetime in us
      double etau = 1000*rhead->GetLifeTime();
      if(etau>0)
	SetLifetime( etau );
      //cout<<"Electron lifetime "<<etau<<" "<<fEleAtten<<" "<<fVdrift<<endl;  
    }
}

//
//
//
CRTracksAnaDEDX::~CRTracksAnaDEDX()
{;}


//
// Assumes the two view tracks have been matched already 
//
int CRTracksAnaDEDX::Process( const LArReco::Track2d* trk0, const LArReco::Track2d *trk1 )
{
  fPath3dTrk0.clear();
  fDqTrk0.clear();

  fPath3dTrk1.clear();
  fDqTrk1.clear();
  
  int np0 = trk0->GetNpts();
  int np1 = trk1->GetNpts();

  fMean_dQdx0 = 0;
  fMean_dQdx1 = 0;

  // require some reasonable long tracks
  if( !(np0 > 10 && np1 > 10) )  return -1;
  
  
  //
  Init( trk0, 0, fPath3dTrk0, fDqTrk0, fLn0 );
  Init( trk1, 1, fPath3dTrk1, fDqTrk1, fLn1 );

  // view 0
  fPathLen0 = Build( fPitch0, 0, fTotal_Q0, fPath3dTrk0, fDqTrk0, fStep0, fLn0, fLn1 );

  // view 1
  fPathLen1 = Build( fPitch1, 1, fTotal_Q1, fPath3dTrk1, fDqTrk1, fStep1, fLn1, fLn0 );

  // calculate mean values for energy loss
  fMean_dQdx0 = fTotal_Q0 / fPathLen0; //path length is in units of pitch
  fMean_dQdx1 = fTotal_Q1 / fPathLen1; //path length is in units of pitch
  
  bool bad = false;
  if( fPathLen0 != fPathLen0 || fPathLen1 != fPathLen1 )
    {
      cout<<"Bad path length "<<"NAN"<<endl;
      //return -1;
      bad = true;
    }

  if( isinf(fPathLen0) || isinf(fPathLen1) )
    {
      cout<<"Bad path length "<<"INF"<<endl;
      //return -1;
      bad = true;
    }

  if( fPathLen0 < 50 || fPathLen1 < 50) // in mm !!!
    {
      cout<<"Path length is too short "<<fPathLen0<<" "<<fPathLen1<<endl;
      bad = true;
    }
  
  if(bad)
    {
      /*
      double ravg = 0;
      for(size_t i=0;i<fPath3dTrk0.size();i++)
	{
	  ravg += fStep0[i];
	  cout<<setw(5)<<i
	      <<setw(15)<<fPath3dTrk0[i].X()
	      <<setw(15)<<fPath3dTrk0[i].Y()
	      <<setw(15)<<fPath3dTrk0[i].Z()
	      <<setw(15)<<fDqTrk0[i]
	      <<setw(15)<<fStep0[i]<<endl;
	}
      ravg /= fStep0.size();
      cout<<" Path length "<<fPathLen0<<" path step "<<ravg<<endl;

      cout<<endl;
      ravg = 0;
      for(size_t i=0;i<fPath3dTrk1.size();i++)
	{
	  ravg += fStep1[i];
	  cout<<setw(5)<<i
	      <<setw(15)<<fPath3dTrk1[i].X()
	      <<setw(15)<<fPath3dTrk1[i].Y()
	      <<setw(15)<<fPath3dTrk1[i].Z()
	      <<setw(15)<<fDqTrk1[i]
	      <<setw(15)<<fStep1[i]<<endl;
	}
      ravg /= fStep1.size();
      cout<<" Path length "<<fPathLen1<<" path step "<<ravg<<endl;
      */
      return -1;
    }
  // apply corrections

  return 0;
}


//
//
//
void CRTracksAnaDEDX::Init( const LArReco::Track2d *atrk, int iview,
			    std::vector<TVector3> &path, std::vector<double> &charge, 
			    RecoUtils::LineSegment &lseg )
{
  path.clear();
  charge.clear();
  
  const std::vector<TVector2> vecp = atrk->GetPath();
  const std::vector<double>   vecq = atrk->GetDQ();

  //bool toreverse = false;

  // Since CR's always comre from the top take the track start to be 
  // the end closest to anode (shortest drift)
  bool toreverse = (vecp[0].Y() > vecp.back().Y());

  //cout<<vecp.size()<<" "<<vecq.size()<<" "<<toreverse<<endl;
  
  std::vector<double> xtmp, ytmp;
  
  // alternatively could have copied first and reversed later
  // but since we are already looping ...
  if( toreverse )
    {
      // ok could insert into q but since we already looping...
      size_t rindx = vecq.size() - 1;
      std::vector<TVector2>::const_reverse_iterator rit;
      for(rit=vecp.rbegin(); rit!=vecp.rend(); rit++)
	{
	  double xv = (*rit).X();
	  double yv = (*rit).Y();
	  double qv = vecq[rindx--]; //should be the same size as path vector!!!

	  xtmp.push_back( xv );
	  ytmp.push_back( yv );
	  
	  path.push_back( TVector3() );
	  if(iview == 0) path.back().SetXYZ(xv, 0, yv);
	  else path.back().SetXYZ(0, xv, yv);
      
	  charge.push_back( qv );
	}
    }
  else
    {
      size_t indx = 0;
      std::vector<TVector2>::const_iterator it;
      for(it=vecp.begin(); it!=vecp.end(); it++)
	{
	  double xv = (*it).X();
	  double yv = (*it).Y();
	  double qv = vecq[indx++]; //should be the same size as path vector!!!

	  xtmp.push_back( xv );
	  ytmp.push_back( yv );
	  
	  path.push_back( TVector3() );
	  if(iview == 0) path.back().SetXYZ(xv, 0, yv);
	  else path.back().SetXYZ(0, xv, yv);
      
	  charge.push_back( qv );
	}

    }

  lseg.SetPoints( xtmp, ytmp );
}

//
//
//
double CRTracksAnaDEDX::Build( double pitch, int iview, 
			      double &qtot, 
			      std::vector<TVector3> &path, 
			      std::vector<double> &charge, 
			      std::vector<double> &dstep,
			      RecoUtils::LineSegment &lseg, 
			      RecoUtils::LineSegment &lseg_other )
{
  double totlen = 0;

  dstep.clear();

  int npavg = 2; //average slope over nearby 2*npavg points ( -npavg -> +npavg )

  int laststep = -1;
  
  qtot = 0;
  //double a0avg = 0;
  //double a1avg = 0;
  for(size_t i=0;i<path.size();i++)
    {
      // slope in this view 
      double a0 = lseg.GetTangent( i, npavg );

      double z = path[i].Z();
      
      // get slope and other coordinate from the other view
      double y, a1;
      lseg_other.GetXandTan(z, npavg, laststep, y, a1);
      
      //cout<< i <<setw(10)<<a0<<setw(10)<<a1<<endl;
      //a0avg += a0;
      //a1avg += a1;
      // the units of dr are the same as those of the pitch !
      double dr = pitch;
      if( a1 == 0) dr *= sqrt(1 + a0*a0);
      else if( a0 == 0) dr *= sqrt(1 + a1*a1);
      else dr *= sqrt(1 + (a0*a0)*(1.0/(a1*a1) + 1.0) );
	
      dstep.push_back(dr);

      // apply lifetime correction
      double fcorr = LifetimeCorrection(z);
      //cout<<z<<" "<<fcorr<<endl;
      
      charge[i] *= fcorr;
      
      // save total q
      qtot += charge[i];
      
      // normalize by the step size
      charge[i] /= dr;
      
      totlen += dr;
      
      // set the point
      if( iview == 0 ) path[i].SetY( y );
      else path[i].SetX( y );
    }
  
  //cout<<"Average slope : "<<a0avg/path.size()<<" "<<a1avg/path.size()<<endl;

  return totlen;
}


//
// lifetime correction factor
//
double CRTracksAnaDEDX::LifetimeCorrection(double zdrift)
{
  if( fEleAtten < 0 ) return 1.0;

  return exp( zdrift/fEleAtten );
}
