///////////////////////////////////////////////////////////////////////////////
//
//  Classes to define how charge is drifted in Efield
//
//
//  Created: Thu May 28 10:12:18 CEST 2015
//
///////////////////////////////////////////////////////////////////////////////
#ifndef __CHARGEDRIFT_H__
#define __CHARGEDRIFT_H__

#include <cmath>
#include <vector>

#include "FieldMap.h"
#include "LiquidArgonMedium.h"

class ChargeDrift
{
 public:
  ChargeDrift();
  virtual ~ChargeDrift();
  
  // drift the charge under the influence of electric field distance z_crp - z_dep
  virtual void Drift( double x_dep, double y_dep, double z_dep, double q_dep, 
		      double z_crp, EFieldMap *efield, 
		      double &dt, double &x_crp, double &y_crp, double &q_crp,
		      double &sigmaDl, double &sigmaDt );

  //
  double GetDriftTime(double d, double efield, double &vd);
  double GetQuenchingFactor(double dEdx, double efield);
  double GetLifetime() const { return m_etau; }
  
  // get diffusion constants
  double GetDiffDl() const { return m_diffcDl; }
  double GetDiffDt() const { return m_diffcDt; }
  
  //
  void SetLifetime(double val){ m_etau = val; }          // e lifetime in ms
  void SetQuenchFactor(double val){ m_recombfact = val; }
  void SetDiffDl(double val){ m_diffcDl = val; }
  void SetDiffDt(double val){ m_diffcDt = val; }


  // eloss due to drift
  double DriftEloss(double dt) // e-loss due to drift for (dt in ms)
  {
    if(m_etau > 0)
      return exp( -dt / m_etau);
    
    return 1.0;
  }

 protected:
  double m_etau;          // e lifetime in ms
  double m_recombfact;    // recombination factor -1 use birk's law (ICARUS)

  double m_diffcDl;       // longitudianl diffusion contant
  double m_diffcDt;       // transverse diffusion constant
};

#endif
