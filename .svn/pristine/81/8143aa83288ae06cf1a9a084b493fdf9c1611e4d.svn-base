#include <iostream>
#include <iomanip>

#include <TRandom3.h>
#include <TVector3.h>

#include "BeamHalo.h"


using namespace std;


//
//
//
bkgsrc::MultivarNormal::MultivarNormal(TVectorD &parms, TMatrixDSym &covm)
{  
  pvals = NULL;
  covar = NULL;
  chold = NULL;
  npars = parms.GetNrows();
  pvals = new TVectorD(npars);
  covar = new TMatrixDSym(npars);
  //parms.Print();
  (*pvals) = parms;
  (*covar) = covm;
  TDecompChol chdcmp(*covar);
  if(!chdcmp.Decompose())
    {
      std::cerr<<"Cholesky decomposition failed in MultivarNormal"<<std::endl;
      exit(-1);
    }
  chold = new TMatrixD(chdcmp.GetU());

  half_pars = npars/2+npars%2;
}


//
//
//
bkgsrc::MultivarNormal::~MultivarNormal()
{
  if(pvals) pvals->Delete();
  if(covar) covar->Delete();
  if(chold) chold->Delete();
}


//
//
//
void bkgsrc::MultivarNormal::StdNormRand(double *z)
{
  
  double u = 2.*gRandom->Rndm()-1.0;
  double v = 2.*gRandom->Rndm()-1.0;
  
  double s = u*u+v*v;
  
  while(s==0 || s>=1.){
    u = 2.*gRandom->Rndm()-1.0;
    v = 2.*gRandom->Rndm()-1.0;
    s = u*u+v*v;
  }
  
  z[0] = u*sqrt(-2.*TMath::Log(s)/s);
  z[1] = v*sqrt(-2.*TMath::Log(s)/s);
}



//
// beam profile characterized in terms of two bivariate normals
//    x0, y0   - beam position on the reference plane in cm
//    nx0, ny0 - beam angle in reference plane in mrad
//    epsy, epsy - emisttance in cm x mrad
//    alphx, alpy - Twiss alpha parameter
//
bkgsrc::BeamProfile::BeamProfile( double x0, double thx0, double sgmx, double epsx, double alphx,
				    double y0, double thy0, double sgmy, double epsy, double alphy )
{
  fMeanx[0] = x0;
  fMeanx[1] = thx0;
  
  fMeany[0] = y0;
  fMeany[1] = thy0;

  TVectorD vec(4);
  vec(0) = x0;
  vec(1) = thx0; 
  vec(2) = y0;
  vec(3) = thy0;
  
  //vec.Print();

  //
  fEps[0]   = epsx;
  fEps[1]   = epsy;
  fAlpha[0] = alphx;
  fAlpha[1] = alphy;

  //
  fParsx[0] = sgmx;
  fParsx[1] = epsx * sqrt( 1 + alphx*alphx ) / (4 * sgmx );
  fParsx[2] = -alphx / sqrt( 1 + alphx*alphx );
  

  //
  fParsy[0] = sgmy;
  fParsy[1] = epsy * sqrt( 1 + alphy*alphy ) / (4 * sgmy );
  fParsy[2] = -alphy / sqrt( 1 + alphy*alphy );
  
  
  //
  TMatrixDSym cov(4);
  
  cov(0,0) = fParsx[0] * fParsx[0];
  cov(0,1) = fParsx[0] * fParsx[1] * fParsx[2];
  cov(0,2) = 0;
  cov(0,3) = 0;
  
  cov(1,0) = cov(0,1);
  cov(1,1) = fParsx[1] * fParsx[1];
  cov(1,2) = 0;
  cov(1,3) = 0;


  cov(2,0) = 0;
  cov(2,1) = 0;
  cov(2,2) = fParsy[0] * fParsy[0];
  cov(2,3) = fParsy[0] * fParsy[1] * fParsy[2];
  
  cov(3,0) = 0;
  cov(3,1) = 0;
  cov(3,2) = cov(2,3); 
  cov(3,3) = fParsy[1] * fParsy[1];
  
  //cov.Print();
  
  fMvNorm = NULL;
  fMvNorm = new bkgsrc::MultivarNormal(vec, cov);
}


//
//
//
bkgsrc::BeamProfile::~BeamProfile()
{
  if(fMvNorm) delete fMvNorm;
}


//
//
//
double bkgsrc::BeamProfile::GetProbX( double x, double thx )
{
  return Bivariate( x, thx, fMeanx, fParsx );
}


//
//
//
double bkgsrc::BeamProfile::GetProbY( double y, double thy )
{
  return Bivariate( y, thy, fMeany, fParsy );
}


//
// bivariate normal
//
double bkgsrc::BeamProfile::Bivariate(double pos, double tht,
					double *parMean,  double *parErr)
{
  double xx = pos;
  double yy = tht;
  
  double mx  = parMean[0];
  double my  = parMean[1];
  double sx  = parErr[0];
  double sy  = parErr[1];
  double rho = parErr[2];
  
  double norm = 1.0/(2*TMath::Pi()*sx*sy*TMath::Sqrt(1.0-rho*rho));
  double xterm = (xx-mx)*(xx-mx)/sx/sx;
  double yterm = (yy-my)*(yy-my)/sy/sy;
  double cterm = -2*rho*(xx-mx)*(yy-my)/sx/sy;

  double f = norm*TMath::Exp(-0.5/(1.0-rho*rho)*(xterm+yterm+cterm));
  return f;
}


//
//
//
void bkgsrc::BeamProfile::GetRandom( std::vector<double> &vec )
{
  fMvNorm->DrawRandom( vec );
}



///
// beam halo for WA105
//
//
bkgsrc::WA105HaloFlux::WA105HaloFlux()
{
  fProfile = NULL;

  fRate    = 0;    // muons /s 
  fDetSzX  = 600;  // cm
  fDetSzY  = 600;  // cm
  fDetSzZ  = 600;  // cm
  fDiaSz   = TMath::Sqrt( fDetSzX*fDetSzX + fDetSzY*fDetSzY );
  
  fPlaneDist = 800; // cm
  fRotAngle  = 0.25*TMath::Pi();
}


//
//
//
bkgsrc::WA105HaloFlux::~WA105HaloFlux()
{
  if(fProfile) delete fProfile;
  fProfile = NULL;
}


//
//
//
void bkgsrc::WA105HaloFlux::SetProfile( double x0, double thx0, double sgmx, double epsx, double alphx, 
					  double y0, double thy0, double sgmy, double epsy, double alphy )
{
  if(fProfile) delete fProfile;

  fProfile = new bkgsrc::BeamProfile( x0, thx0, sgmx, epsx, alphx, y0, thy0, sgmy, epsy, alphy );
}


//
//
//
int bkgsrc::WA105HaloFlux::GetPid()
{
  int pid = 13; //mu-
  if(gRandom->Rndm() > 0.5) pid = -13; //mu+
  return pid;
}


//
//
//
double bkgsrc::WA105HaloFlux::GetEkin()
{
  return 10.0; //GeV
}


//
// assume constant rate 
//
size_t bkgsrc::WA105HaloFlux::GetTiming(double t0, double twin, std::vector<float> &evtimes)
{
  evtimes.clear();
  
  if(fRate <= 0 ) return 0;
  
  // generate time distribution over a slightly larger time window
  float tstart    =  1.1*t0;          //in s
  float tend      =  1.1*(t0 + twin); //in s
  
  if(tstart >= tend ) // make just a single event at a given time t0
    {
      evtimes.push_back(t0);
    }
  else
    {
      float mytime = tstart;
      //cout<<"Generating event timing with "<<m_rate<<" muon/s"<<endl;
      
      while(mytime <= tend)
	{
	  float tevent = gRandom->Exp(1.0/fRate); //per s
	  
	  mytime += tevent;
	  if(mytime > tend)
	    break;
	  
	  if(mytime >= t0 && mytime <= (t0+twin))
	    {
	      evtimes.push_back(mytime);
	    }
	}
    }
  
  cout<<"Generated "<<evtimes.size()<<" events in time window ["
      <<t0<<", "<<t0+twin<<"] s"<<endl;
  
  
  return evtimes.size();
}


//
//
//
size_t bkgsrc::WA105HaloFlux::GetHalo(double t0, double twin, std::vector<particle_fld> &flux)
{
  flux.clear();
  
  if( fRate <= 0 || !fProfile ) return 0;
  
  std::vector<float> evtimes;
  if( !GetTiming(t0, twin, evtimes) ) return 0;
 
  vector<double> pristart; 
  //
  for(size_t i=0;i<evtimes.size();i++)
    {
      fProfile->GetRandom( pristart );

      // !!! coordinates in the beam halo plane !!!
      // the z axis is aligned with the detector axis
      // x and y are rotated by the pi/4
      
      // primary vertex
      double vx = 0.0;
      double vy = pristart[0];
      double vz = pristart[2];
      
      // primary direction (beam moves in +x direciton)
      double ny = TMath::Sin( pristart[1] ); // <== cos( pi/2 - tht )
      double nz = TMath::Sin( pristart[3] ); // <== cos( pi/2 - tht )
      double nx = TMath::Sqrt( 1.0 - ny*ny - nz*nz );
      
      if( nx != nx ) 
	{
	  //cerr<<"Bad angular cosine "<<nx<<" "<<ny<<" "<<nz<<" "
	  //<<sqrt(nx*nx+ny*ny+nz*nz)<<endl;
	  continue;
	}
      
      double x, y, z;
      if( !DetectorIntercept(nx, ny, nz, vx, vy, vz, x, y, z) ) continue;
      
      // convert to the coordinate system of the detector
      TVector3 vertex( vx - fPlaneDist, vy, vz );
      TVector3 dircos( nx, ny, nz );
      vertex.RotateZ( fRotAngle );
      dircos.RotateZ( fRotAngle );
      
      particle_fld aMuon;
      aMuon.time   = evtimes[i]; // in s
      aMuon.pdg    = GetPid();
      aMuon.eneKin = GetEkin() * 1.0E+3; // in MeV
      aMuon.x      = vertex.X()*1.0E-2;  //x; in m
      aMuon.y      = vertex.Y()*1.0E-2;  //y; in m
      aMuon.z      = vertex.Z()*1.0E-2;  //z; in m
      aMuon.dirx   = dircos.X(); 
      aMuon.diry   = dircos.Y(); 
      aMuon.dirz   = dircos.Z(); 
      
      flux.push_back( aMuon );
    }

  cout<<"Number of events in the detector "<<flux.size()<<endl;  
  cout<<"Rate of beam halo in the detector "<<flux.size()/twin*1.0E-3<<" kHz "<<endl;
  return flux.size();
}


//
//
//
bool bkgsrc::WA105HaloFlux::DetectorIntercept(  double nx, double ny, double nz,
						  double vx, double vy, double vz,
						  double &x, double &y, double &z )
{
  if( nx <= 0 ) return false;
  
  double d = fPlaneDist / nx;
  
  x = nx * d + vx;
  y = ny * d + vy;
  z = nz * d + vz;
  
  
  bool rval = ( (fabs( y ) < 0.5*fDiaSz) && 
		(fabs( z ) < 0.5*fDetSzZ) );
  
  return rval;
}
