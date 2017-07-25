#include <iostream>


#include "EFieldAndDriftMap.h"


#include "ChargeDrift.h"

// ctor
ChargeDrift::ChargeDrift()
{
  m_etau = -1.0; //infinite lifetime

  // transverse and longitudinal diffusion parameters
  m_diffcDl = 0; // cm2/s
  m_diffcDt = 0; // cm2/s

  m_recombfact = -1.0; // use birk's law (ICARUS)
}

// dtor
ChargeDrift::~ChargeDrift()
{;}

// get drift time and return drift velocity by reference (in cm/us)
double ChargeDrift::GetDriftTime(double d, double efield, double &vd) // d in cm, efield in V/cm
{
  LiquidArgonMedium* lar = LiquidArgonMedium::Instance();
  
  // get drift velocity in mm / us
  //double vd = (double)(lar->DriftVelocity(efield*1.0E-3)); //efield kV/cm
  //double dt = d / (0.1 * vd); // cm / cm/us

  // cm / us
  vd = 0.1*(double)(lar->DriftVelocity(efield*1.0E-3)); //efield kV/cm
  
  double dt = d / vd; // cm / cm/us
  return dt; // in us
}

// charge recombination
double ChargeDrift::GetQuenchingFactor(double dEdx, double efield)
{
  if(dEdx != dEdx) return 0;

  if(m_recombfact >= 0)
    return m_recombfact;

  // otherwise use birk's law
  LiquidArgonMedium* lar = LiquidArgonMedium::Instance();
  double R = lar->QuenchingFactor(dEdx, efield*1.0E-3); // MeV/cm kV/cm
  
  return R;
}


// drift 
void ChargeDrift::Drift( double x_dep, double y_dep, double z_dep, double q_dep, 
			 double z_crp, EFieldMap *efield, 
			 double &dt, double &x_crp, double &y_crp, double &q_crp,
			 double &sigmaDl, double &sigmaDt)
{

  double dx = 0;
  double dy = 0;

  // no diffusion by default
  sigmaDl = 0.0;
  sigmaDt = 0.0;

  //basic projection
  q_crp = 0.0;
  x_crp = x_dep;
  y_crp = y_dep;

  
  // distance to the anode
  double dist = z_crp - z_dep;


  if(dist < 0.0)
    {
      std::cerr<<"ERROR: distance to CRP is negative"<<std::endl;
      std::cerr<<"       z_crp = "<<z_crp<<std::endl;
      std::cerr<<"       z_dep = "<<z_dep<<std::endl;
      std::cerr<<"       return qdep = 0 "<<std::endl;
      return;
    }


  //
  //
  //
  if( EFieldAndDriftMap *map = dynamic_cast<EFieldAndDriftMap*>(efield) ) 
    {
      
      //get drift time and position at the crp
      
	 
      double xp = x_dep*10;
      double yp = y_dep*10;
      double zp = z_dep*10;
      
      dx = map->GetEndPointDX(xp,yp,zp);
      dy = map->GetEndPointDY(xp,yp,zp);
      
      
      x_crp = xp + dx ;
      y_crp = yp + dy;
      
      // mm to cm
      x_crp *= 0.1; 
      y_crp *= 0.1;
      
      dt = map->GetDriftTime(xp,yp,zp);
      
      // ms to us
      dt *= 1e3;	
      
      //std::cout<<__FUNCTION__<<" "<<xp<<" "<<yp<<" "<<zp<<" "<<dt<<" "<<x_crp<<" "<<y_crp<<std::endl;
    } 
  else 
    {
    
      // calculate drift time to anode
      double Efld = efield->EField(x_dep, y_dep, z_dep);
      double vd;
      dt = GetDriftTime(dist, Efld, vd);

      // calculate diffusion at the anode (if any)
      
      // if parametrization (-1), compute constants once for uniform field
      if(m_diffcDl < 0 || m_diffcDt < 0) 
	{
	  LiquidArgonMedium* lar = LiquidArgonMedium::Instance();
	  if(m_diffcDl < 0)
	    m_diffcDl = lar->GetDiffusionDl( Efld * 1.0E-3 );    //ok compute once for uniform field
	  if(m_diffcDt < 0)
	    m_diffcDt = lar->GetDiffusionDt( Efld * 1.0E-3 );    //ok compute once for uniform field
	}
      
      if(m_diffcDl > 0)
	{
	  sigmaDl = sqrt(2 * m_diffcDl * 1.0E-6 * dt ) / vd; // --> in us
	}
      if(m_diffcDt > 0)
	{
	  sigmaDt = sqrt(2 * m_diffcDt * 1.0E-6 * dt ); // --> in cm
	}

    }//EFieldAndDrift


  if(dt != dt)
    {
      std::cerr<<"ERROR: Drift time is infinite"<<std::endl;
      return;
    }


  // lifetime attenuation
  q_crp = q_dep * DriftEloss(dt*1.0E-3); //us -> ms
}




