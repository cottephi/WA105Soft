/////////////////////////////////////////////////////////////////////////////////
//
//  2D track in a given view
//
//
//
/////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <iomanip>
#include <cmath>

#include "Track2d.h"

ClassImp(LArReco::Track2d)

using namespace std;
//
//
//
LArReco::Track2d::Track2d()
{
  fThisObj = LArReco::RecoObjLink(LArReco::kTrack2d, -1);  
  
  SetDefaults();
}

//
//
//
void LArReco::Track2d::SetDefaults()
{
  fCrm  = -1; //all view
  fViewType = kBadView;
  
  fTotQ   = 0;  
  fT0     = 0;
  fLength = 0;

  fChi2 = 999;
  
  fMom  = -999;
  fPid  = -999;
  
  fTanThInit     = -999;
  fTanThFinal    = -999;
  
  fTanThInitErr   = 0;
  fTanThFinalErr  = 0;
  fDPosInitErr    = 0;
  fDPosFinalErr   = 0;
  fCorrCoeffInit  = 0;
  fCorrCoeffFinal = 0;
}

//
//
//
LArReco::Track2d::~Track2d()
{;}


//
//
//
LArReco::Track2d::Track2d( std::vector<TVector2> &xz, std::vector<double> dqdx, 
			   std::vector<const LArReco::QHit*> &hits )
{
  fThisObj = LArReco::RecoObjLink(LArReco::kTrack2d, -1);  
 
  SetDefaults();

  fPath  = xz;
  fQdX   = dqdx;

  //
  Init( hits );
  
  fStart = fPath[0];
  fEnd   = fPath.back();
}


//
//
//
LArReco::Track2d::Track2d(std::vector<const LArReco::QHit*> &hits)
{
  fThisObj = LArReco::RecoObjLink(LArReco::kTrack2d, -1);  
 
  //
  SetDefaults();
  
  MakePath( hits );

  //
  Init( hits );

  fStart = fPath[0];
  fEnd   = fPath.back();
}

//
//
//
void LArReco::Track2d::Init(std::vector<const LArReco::QHit*> &hits)
{
  fViewType = GetRecoObjView(hits[0]->GetView());
  fCrm      = hits[0]->GetCRM();

  fTotQ = 0;
  for(size_t i=0;i<hits.size();i++)
    {
      // add numeric index reference
      AddHit( hits[i] );

      // add charge
      Double_t qval = hits[i]->GetQ();
      fTotQ += qval;
    }

  // calculate total legnth
  fLength = 0;
  for( size_t i=1;i<fPath.size();i++ )
    {
      double dx = ( fPath[i].X() - fPath[i-1].X() );
      double dy = ( fPath[i].Y() - fPath[i-1].Y() );
      fLength += sqrt( dx*dx + dy*dy );
      
      if(i == 1) fTanThInit = dy/dx;
      fTanThFinal = dy/dx;
    }
}


//
//
//
void LArReco::Track2d::MakePath(std::vector<const LArReco::QHit*> &hits)
{
  int lastch = hits[0]->GetCh();
  double q   = hits[0]->GetQ();
  double x   = hits[0]->GetX();
  double y   = hits[0]->GetD(); //hits[0]->GetT();
  fPath.push_back(TVector2( x, y ));
  fQdX.push_back(q);

  fTotQ = q;
  for(size_t i=1;i<hits.size();i++)
    {
      if(lastch == hits[i]->GetCh())
	{
	  fQdX.back() += hits[i]->GetQ(); 
	  fTotQ += hits[i]->GetQ();
	  continue;
	}
      
      q = hits[i]->GetQ();
      x = hits[i]->GetX();
      y = hits[i]->GetD();
      lastch = hits[i]->GetCh();

      fTotQ += q;
      fPath.push_back(TVector2( x, y ));
      fQdX.push_back(q);
    }
}


//
// use this function to combine several tracks
//
LArReco::Track2d& LArReco::Track2d::operator+=(const Track2d &rhs)
{
  if(fViewType != rhs.GetView())
    {
      cerr<<"ERROR: cannot append tracks from different views. "<<endl;
      return *this;	
    }

  //make sure we are not adding tracks at different times
  if(fT0 != fT0) 
    {
      cerr<<"ERROR: cannot merge two track segments with different time of origin"<<endl;
      return *this;      
    }
  
  // are we combining between different readout modules?
  if( rhs.GetCRM() != fCrm ) fCrm = -1;

  fTotQ += rhs.fTotQ;
  const std::vector<TVector2>& path   = rhs.GetPath();
  const std::vector<Double_t>& dq     = rhs.GetDQ();
  const std::vector<LArReco::RecoObjLink> drays = rhs.GetDRays();
  const std::vector<LArReco::RecoObjLink> hits  = rhs.GetHits();
  
  // add hits
  for(size_t i=0;i<hits.size();i++)
    LinkToLowerNode(hits[i]);

  for(size_t i=0;i<drays.size();i++)
    fDeltaRays.push_back(drays[i]);
  
  // total length
  fLength += ( rhs.fLength + sqrt((path[0].X()-fPath.back().X())*(path[0].X()-fPath.back().X())+
				  (path[0].Y()-fPath.back().X())*(path[0].Y()-fPath.back().Y())) );
  
    
  // add path and charge per step
  for(size_t i=0;i<path.size();i++)
    {
      fPath.push_back( path[i] );
      fQdX.push_back( dq[i] );
    }

  // add end point
  fEnd   = fPath.back();
  
  // final taken from the last track
  fTanThFinal     = rhs.fTanThFinal;
  fTanThFinalErr  = rhs.fTanThFinalErr;
  fDPosFinalErr   = rhs.fDPosFinalErr;
  fCorrCoeffFinal = rhs.fCorrCoeffFinal;
  
  return *this;
}


//
//
//
void LArReco::Track2d::AddDeltaRays(std::vector<const LArReco::QHit*> &drays)
{ 
  if(!fDeltaRays.empty()) 
    {
      std::cerr<<"ERROR: delta rays have already been added to this track"<<std::endl;
      return;
    }
  
  //fDrayQ = 0;
  for(size_t i=0;i<drays.size();i++)
    {
      // add delta ray charge to total
      fTotQ  += drays[i]->GetQ();
      //fDrayQ += drays[i]->GetQ();
      fDeltaRays.push_back( drays[i]->GetRecoObjLink() );
    }
}



//
//
//
void LArReco::Track2d::Print() const
{
  std::cout<<"Track chi2 fit    : "<<fChi2<<std::endl;
  std::cout<<"Total charge      : "<<fTotQ<<" fC"<<std::endl;
  std::cout<<"Particle id       : "<<fPid<<std::endl;
  std::cout<<"Particle momentum : "<<fMom<<std::endl;
  std::cout<<"Initial direction : "<<fTanThInit<<" +/- "<<fTanThInitErr<<std::endl;
  std::cout<<"Final direction   : "<<fTanThFinal<<" +/- "<<fTanThFinalErr<<std::endl;
  std::cout<<"Track beginnning at : "<<fStart.X()<<" (cm), "<<fStart.Y()<<" (cm)"<<std::endl;
  std::cout<<"Track ending at     : "<<fEnd.X()<<" (cm), "<<fEnd.Y()<<" (cm)"<<std::endl;
  std::cout<<"Number of track points : "<<fPath.size()<<std::endl;
  std::cout<<"Track total length     : "<<fLength<<" (cm)"<<std::endl;
  for(size_t i=0;i<fPath.size();i++)
    {
      cout<<setw(10)<<fPath[i].X()<<setw(10)<<fPath[i].Y()<<setw(10)<<fQdX[i]<<endl;
    }
}

