// A class used to store information on LAr properties
// A.Meregaglia - A.Rubbia (Oct 2004)

// Singleton class : LiquidARgonMedium::Instance()

//
// should update temperature and density for LAr
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "LiquidArgonMedium.h"

using namespace std;


// singleton instance
LiquidArgonMedium* LiquidArgonMedium::pInstance = NULL;

LiquidArgonMedium* LiquidArgonMedium::Instance()
{
  if(! pInstance ) pInstance = new LiquidArgonMedium();
  return pInstance;
}

void LiquidArgonMedium::DestroyInstance()
{
  if(pInstance)
    {
      delete pInstance;
      pInstance = NULL;
    }
}

LiquidArgonMedium::LiquidArgonMedium()
{ 

  LArT = 88.0;   // * kelvin;
  dyne = 1.e-5;  // * newton; // compressibility uses this CGS unit
  LambdaE = 2.0 * 3.14159265358979323846 * 1.973269602e-16; // * m * GeV; 
  GArRho  = 0.001784; // *g/cm3 o  1.784 * kg/m3; // GAr (gas) mass density
  LArRho = 1.390;     // * g/cm3; // LAr (liquid) mass density
  k = 1.380658e-23;   // * joule/kelvin; // the Boltzmann constant
  LArKT = 2.18e-10;   // * cm2/dyne; // LAr isothermal compressibility
  LArScintillationYield = 4000.0;         //  / MeV;//40000
  LArScintillationResolutionScale = 1.0;
  LArScintillationFastTimeConstant = 6.0; // * ns; // 27%
  LArScintillationSlowTimeConstant = 1.4; // * microsecond; // 73%
  LArScintillationYieldRatio = 0.27;      // fast component / slow component

  Wion = 23.6; //eV
}

LiquidArgonMedium::~LiquidArgonMedium()
{;}

// Calculates the refractive index of LAr
double LiquidArgonMedium::RefIndex(const double lambda) //input in nm!!!
{
  return ( sqrt(Epsilon(lambda)) ); // square root of dielectric constant
}

//
double LiquidArgonMedium::Epsilon(const double lambda)
{

  // Calculates the dielectric constant of LAr from the Bideau-Sellmeier formula.
  // See : A. Bideau-Mehu et al., "Measurement of refractive indices of Ne, Ar,
  // Kr and Xe ...", J. Quant. Spectrosc. Radiat. Transfer, Vol. 25 (1981), 395
  
  
  double epsilon;
  if (lambda <= (107.05)) return 1.0e4; // lambda MUST be > 107.05 nm
  epsilon = lambda / 1000.; // switch to micrometers
  epsilon = 1.0 / (epsilon * epsilon); // 1 / (lambda)^2
  epsilon = 1.2055e-2 * ( 0.2075 / (91.012 - epsilon) +
			  0.0415 / (87.892 - epsilon) +
			  4.3330 / (214.02 - epsilon) );
  epsilon *= (8./12.); // Bideau-Sellmeier -> Clausius-Mossotti
  epsilon *= (LArRho / GArRho); // density correction (Ar gas -> LAr liquid)
  if ((epsilon < 0.0) || (epsilon > 0.999999)) return 4.0e6;
  epsilon = (1.0 + 2.0 * epsilon) / (1.0 - epsilon); // solve Clausius-Mossotti
  return epsilon;
}

// Reyleigh scattering length
// input in nm e kelvin
double LiquidArgonMedium::RayLength(const double lambda,const double temperature)
{

  //output in mm

  // Calculates the Rayleigh scattering length using equations given in
  // G. M. Seidel at al., "Rayleigh scattering in rare-gas liquids",
  // arXiv:hep-ex/0111054 v2 22 Apr 2002

  double h;
  h = Epsilon(lambda);
  if (h < 1.00000001) h = 1.00000001; // just a precaution
  h = (h - 1.0) * (h + 2.0); // the "dielectric constant" dependance
  h *= h; // take the square
  h *= LArKT * temperature * k; // compressibility * temperature * Boltzmann constant
  h /= lambda * lambda * lambda * lambda; // (lambda)^4
  h *= 9.18704494231105429; // (2 * Pi / 3)^3

  return ( (1.0 / h)/(1e34) );
}

// energy loss with particle id as input
double LiquidArgonMedium::BetheBloch(double KE,int pid)
{
  double dEdX;
  double M;
  
  switch(pid)
    {
    case 13: //muon-
      M=105.658; //MeV/c2
      break;
    case -13: //muon+
      M=105.658; //MeV/c2
      break; 
    case 211: //pi+
      M=139.570; //MeV/c2
      break; 
    case -211: //pi+
      M=139.570; //MeV/c2
      break; 
    case 2212: //proton
      M=938.272; //MeV/c2
      break;  
    case 321: //k+
      M=493.677; //MeV/c2
      break; 
    
    default:
      M=100;// not good!
      break;
    }
  
  
  double Z=18;
  double A=39.948;
  double z=1;//charge of incident particle in unit of e
  double beta;
  beta=CalculateBeta(KE,M);
  double gamma;
  gamma=1/sqrt(1-beta*beta);
  double I=0.000188;//MeV
  double me=0.511;//Mev/c2
  double Wmax;
  Wmax=(2*me*beta*beta*gamma*gamma) / (1 + 2*gamma*me/M + (me/M)*(me/M));
  double C1=0.1535;//MeV cm2/g
  
  double X,X0,X1;
  X0=0.201;
  X1=3;
  X=log10(beta*gamma);
  double delta;
  double C,a;
  C=-5.217;
  a=0.196;
  
  if(X<=X0)
    delta=0;
  else if(X<=X1)
    delta=2*log(10.)*X+C+a*(X1-X)*(X1-X)*(X1-X);
  else
    delta=2*log(10.)*X+C;
  
  dEdX= ( C1*(Z/A) * LArRho  * z*z /(beta*beta)) * (log(2*me*beta*beta*gamma*gamma*Wmax / (I*I)) - 2*beta*beta - delta); //-Dshell??

  return dEdX;
  
  
}

// energy loss with mass as input
double LiquidArgonMedium::BetheBloch(double KE, double Mass)
{
  double dEdX;
  double M;
  M=Mass;
  
  
  double Z=18;
  double A=39.948;
  double z=1;//charge of incident particle in unit of e
  double beta;
  beta=CalculateBeta(KE,M);
  double gamma;
  gamma=1/sqrt(1-beta*beta);
  double I=0.000188;//MeV
  double me=0.511;//Mev/c2
  double Wmax;
  Wmax=(2*me*beta*beta*gamma*gamma) / (1 + 2*gamma*me/M + (me/M)*(me/M));
  double C1=0.1535;//MeV cm2/g
  
  double X,X0,X1;
  X0=0.201;
  X1=3;
  X=log10(beta*gamma);
  double delta;
  double C,a;
  C=-5.217;
  a=0.196;
  
  if(X<=X0)
    delta=0;
  else if(X<=X1)
    delta=2*log(10.)*X+C+a*(X1-X)*(X1-X)*(X1-X);
  else
    delta=2*log(10.)*X+C;
  
  dEdX= ( C1*(Z/A) * LArRho  * z*z /(beta*beta)) * (log(2*me*beta*beta*gamma*gamma*Wmax / (I*I)) - 2*beta*beta - delta); //-Dshell??
  
  return dEdX;
  
  
}

double LiquidArgonMedium::CalculateBeta(double KE, double M)
{
  
  //calculate beta given the kinetic energy of a particle

  double Ene=M+KE;
  double pc=sqrt(Ene*Ene - M*M);

  double beta=pc/Ene;

  return beta;

}

// Range
double LiquidArgonMedium::BetheBlochRange(double Range,int pid)// cm
{
  double r(0);
  double kin(0);
  double en;
  while(r<Range)
    {
      kin+=0.1; //step di 0.1 MeV
      en=BetheBloch(kin,pid);
	r+=0.1/en;
    }
  return en;
}

// Range
double LiquidArgonMedium::BetheBlochRange(double Range,float Mass)// cm
{
  double r(0);
  double kin(0);
  double en;
  while(r<Range)
    {
      kin+=0.1; //step di 0.1 MeV
      en=BetheBloch(kin,Mass);
	r+=0.1/en;
    }
  return en;
}

double LiquidArgonMedium::QuenchingFactor(double dEdX,double EField)// MeV/cm kV/cm
{
  //check "Study of electron recombination in liquid Argon with the ICARUS TPC"
 
  double R;          //QuenchingFactor
  double A=0.8;      //Normalization
  double k=0.0486;   //kV/cm * g/cm2 *1/MeV
  double Eps=EField; //Electric field kV/cm   default = 0.5
  double dedx=dEdX;  //MeV/cm
  
  if (Eps==0) 
    R=0.;
  else
    R=A/(1+((dedx * k)/(Eps*LArRho)));
    
  //cout<<" --- Quenching-- dEdx: "<<dEdX<<" [MeV/cm], EField:"<<EField<<" R: "<<R<<endl;
  return R;
}

double LiquidArgonMedium::DriftVelocity(double EField, double temperature)// kV/cm
{

  double vd = 0.;
  double e  = EField;
  double t  = temperature;
  
  //Walkowiak Function (E > 0.5 kV/cm)
  double t0 = 90.371;
  double p1 = -0.01481;
  double p2 = -0.0075;
  double p3 =  0.141;
  double p4 =  12.4;
  double p5 =  1.627;
  double p6 =  0.317;

  // ICARUS Polynomial fit (E <= 0.5 kV/cm)
  double po[6];
  po[0] = -0.03229;
  po[1] = 6.231;
  po[2] = -10.62;
  po[3] = 12.74;
  po[4] = -9.112;
  po[5] = 2.83;
  
  if (e>0.5)
    {
      vd = (p1*(t-t0)+1)*(p3*e*log(1+p4/e)+p5*pow(e,p6))+p2*(t-t0);
    }

  if (e<=0.5){
    for (int i=0;i<6;i++) vd = vd + po[i]*pow(e,i);
    float etmp = 0.5;
    float tmp1 = 0;
    for (int i=0;i<6;i++) tmp1 = tmp1 + po[i]*pow(etmp,i);
    float tmp2 = (p1*(t-t0)+1)*(p3*etmp*log(1+p4/etmp)+p5*pow(etmp,p6))+p2*(t-t0);
    vd = vd * tmp2 / tmp1;  // continuous function at E=0.5kV/cm
  }
  return (vd); // mm/us
}

// convert energy deposition in MeV to Q in fC
double LiquidArgonMedium::ConvertEdepToQ(double edep)
{
  double e = 1.60217646e-4;
  double qval = edep*1.0E+6 / Wion * e; //MeV --> fC
  return qval;
}

// convert energy deposition in MeV to Q in fC
double LiquidArgonMedium::ConvertEdepToNe(double edep)
{
  double Ne = edep*1.0E+6 / Wion; //MeV --> Ne
  return Ne;
}


// diffusion constants as a function of E-field strength in kV/cm (to be completed)
void LiquidArgonMedium::GetDiffusionConsts(const double Efield, double &Dl, double &Dt)
{
  Dl = Dt = 0.0; //cm2/s
  // to implement e.g., Atrazhev IEEE Transaction Vol5 450 (1998)
  return;
}

// longitudinal diffusion constant
double LiquidArgonMedium::GetDiffusionDl(const double Efield)
{
  double Dl, Dt;
  GetDiffusionConsts(Efield, Dl, Dt);
  return Dl;  //cm2/s
}

// transverse diffusion constant
double LiquidArgonMedium::GetDiffusionDt(const double Efield)
{
  double Dl, Dt;
  GetDiffusionConsts(Efield, Dl, Dt);
  return Dt; //cm2/s
}
