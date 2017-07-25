#ifndef PARTICLE_H /* ci_typedef */
#define PARTICLE_H /* ci_typedef */

#define NPARTICLE 51 /* ci_typedef */
typedef enum {       /* ci_typedef */
  Unknown  = 0,      /* ci_typedef */
  Photon   ,         /* ci_typedef */
  Positron ,         /* ci_typedef */
  Electron ,         /* ci_typedef */
  Neutrino ,         /* ci_typedef */
  Muplus   ,         /* ci_typedef */
  Muminus  ,         /* ci_typedef */
  Pi0      ,         /* ci_typedef */
  Piplus   ,         /* ci_typedef */
  Piminus  ,         /* ci_typedef */
  K0long   ,         /* ci_typedef */
  Kplus    ,         /* ci_typedef */
  Kminus   ,         /* ci_typedef */
  Neutron  ,         /* ci_typedef */
  Proton   ,         /* ci_typedef */
  Antiproton ,       /* ci_typedef */
  K0short  ,         /* ci_typedef */
  Eta      ,         /* ci_typedef */
  Lambda   ,         /* ci_typedef */
  Sigmaplus,         /* ci_typedef */
  Sigma0   ,         /* ci_typedef */
  Sigmaminus,        /* ci_typedef */
  Xi0      ,         /* ci_typedef */
  Ximinus  ,         /* ci_typedef */
  Omega    ,         /* ci_typedef */
  Antineutron,       /* ci_typedef */
  Antilambda,        /* ci_typedef */
  Antisigmaminus,    /* ci_typedef */
  Antisigma0,        /* ci_typedef */
  Antisigmaplus,     /* ci_typedef */
  Antixi0   ,        /* ci_typedef */
  Antixiplus,        /* ci_typedef */
  Antiomegaplus,     /* ci_typedef */
  Tauplus   ,        /* ci_typedef */
  Tauminus  ,        /* ci_typedef */
  Dplus     ,        /* ci_typedef */
  Dminus    ,        /* ci_typedef */
  D0        ,        /* ci_typedef */
  Antid0    ,        /* ci_typedef */
  Dsplus    ,        /* ci_typedef */
  Dsminus   ,        /* ci_typedef */
  Lambdacplus,       /* ci_typedef */
  Wplus     ,        /* ci_typedef */
  Wminus    ,        /* ci_typedef */
  Z0        ,        /* ci_typedef */
  Deuteron  ,        /* ci_typedef */
  Tritium   ,        /* ci_typedef */
  Alpha     ,        /* ci_typedef */
  Geantino  ,        /* ci_typedef */
  He3       ,        /* ci_typedef */
  LastParticle       /* ci_typedef */
} _ParticleType;     /* ci_typedef */

/* ++++++++++++++++++++++ public funtion prototyping ++++++++++++++++++++++ */
#ifdef _CPLUSPLUS_
extern "C"
{
#endif

double  ParticleMass           (_ParticleType Particle);/*fortran*/ 
int     ParticleCharge         (_ParticleType Particle);/*fortran*/
int     ParticleColor          (_ParticleType Particle);/*fortran*/
char*   ParticleName           (_ParticleType Particle);/*fortran*/
int     ToGeantCode(int code); 

#ifdef _CPLUSPLUS_
}
#endif

#endif /* ci_typedef */
