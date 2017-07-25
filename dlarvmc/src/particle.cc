#ifdef __cplusplus
extern "C"{
#endif

/* ++++++++++++++++++++++ C Include Files ++++++++++++++++++++++ */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "particle.h"


/*\docF*/
double ParticleMass(_ParticleType Particle)
/*returns the mass of Particle.*/
{static double Mass[NPARTICLE] = {0.1395 /* mass of Unknow particle set to Pion mass */,
                           0.0, 0.000511, 0.000511, 0.0, 0.105, 0.105, 0.135, 
                           0.1395, 0.1395, 0.49767, 0.49364, 0.49364, 0.939566, 0.9383, /*proton */
                           0.9383, 0.49767, 0.5488 , 1.11563, 1.18937, 1.19255,1.19743,
                           1.3149 , 1.32132, 1.67243, 0.939566, 1.11563, 1.18937, 1.19255,
                           1.19743, 1.3149, 1.32132, 1.67243, 1.7841 , 1.7841, 
                           1.8693, 1.8693, 1.8645, 1.8645, 1.9693, 1.9693,
                           2.2849, 81.0 /* ! */, 81.0 , 91.2, 1.875613,
                           2.81448, 3.727417, 0.0, 2.81448  };
if ( (int) Particle < (int) LastParticle) return Mass[(int) Particle]; 
   else printf(" ParticleMass requested for particle id %d \n",Particle);return 0.0;
}

/*\docF*/
int ParticleCharge(_ParticleType Particle)
/*returns the charge of Particle.*/
{ static int charge[NPARTICLE] = {1,0,1,-1,0,1,-1,0,1,-1,0,1,-1,0,1,-1, /* 15: antiproton */
                           0,0,0,1,0,-1,0,-1,-1,0,0,-1,0,1,0,1, /* 31 : anti_xi+ */
                           1,1,1,1,-1,0,0,1,-1,1,1,-1,0,1,1,2,0,2};
if ((int) Particle < (int) LastParticle ) return charge[(int) Particle]; 
  else {printf(" ParticleCharge requested for particle id %d \n",Particle); return 0;}
}

/*\docF*/
int ParticleColor(_ParticleType Particle)
/*returns a standard color for drawing*/
{ static int color[NPARTICLE] = {8,3,45,45,62,23,23,3,47,47,62,47,47,62,47,47, /* 15: antiproton */
                           62,62,62,47,62,47,62,47,47,62,62,47,62,47,62,47, /* 31 : anti_xi+ */
                           47,47,47,47,47,62,62,47,47,47,47,47,62,47,47,62,62,47};
if ((int) Particle < (int) LastParticle) return color[(int) Particle]; 
  else {printf(" ParticleColor requested for particle id %d \n",Particle); return 45;}
}

/*\docF*/
char *ParticleName(_ParticleType Particle)
/*returns the name of Particle as a string.*/
{ static char name[][15] = {
  "Unknown  ",  "Photon   ",  "Positron ",  "Electron ",  "Neutrino ",  "Muplus   ",
  "Muminus  ",  "Pi0      ",   "Piplus   ",   "Piminus  ",  "K0long   ",  "Kplus    ",
  "Kminus   ",  "Neutron  ",   "Proton   ",  "Antiproton ",  "K0short  ",  "Eta      ",
  "Lambda   ",  "Sigmaplus",  "Sigma0   ",  "Sigmaminus",  "Xi0      ",  "Ximinus  ",
  "Omega    ",  "Antineutron",  "Antilambda",  "Antisigmaminus",  "Antisigma0",
  "Antisigmaplus",  "Antixi0   ",  "Antixiplus",  "Antiomegaplus",  "Tauplus   ",
  "Tauminus  ",  "Dplus     ",  "Dminus    ",  "D0        ",  "Antid0    ",  "Dsplus    ",
  "Dsminus   ",  "Lambdacplus",  "Wplus     ",  "Wminus    ",  "Z0        ",  
  "Deuteron  ",  "Tritium   ",  "Alpha     ",  "Geantino  ",  "He3       ","Undefined "};
if ((int) Particle < (int) LastParticle) return name[(int) Particle]; 
  else {printf(" ParticleName requested for particle id %d \n",Particle); return name[0];}
}


/************************************************************************************/
/*                  J E T S E T   T O  G E A N T    C O D E                         */
/************************************************************************************/
/*\docF*/
int ToGeantCode(int code)
{
  int i;
  int Jetset[] = {
/* GAMMA */  22,
/* POSITRON, ELECTRON */
     -11 , 11,
/* MUON+, MUON- */
     -13, 13,
/* TAU+, TAU- */
     -15, 15,
/* PI+, PI-, ETA, PI0 */
     211, -211, 221, 111,
/* K+, K-, KS, KL */
     321,-321, 310, 130,
/* NEUTRINO NUE, NUMU, NUTAU */
      12,-12,14,-14,16,-16,
/* PROTON */
    2212,-2212,
/* NEUTRON */
    2112,-2112,
/* SIGMA+, ANTI(SIGMA+), SIGMA-, ANTI(SIGMA-),SIGMA0, ANTISIGMA0, */
    3222, -3222, 3112, -3112, 3212, -3212,
/* XI 0, ANTIXI 0 */
     3322, -3322,
/* XI-, ANTI(XI-), */
     3312, -3312,
/* LAMBDA, ANTILAMBDA */
      3122, -3122,
/* OMEGA- , ANTI-(OMEGA-) */
    3334, -3334};

/*
   GEANT 3.21 PARTICLE DEFINITION
*/
  int Geant[] = {
/* GAMMA */
      1,
/* POSITRON, ELECTRON */
      2, 3,
/* MUON+, MUON- */
      5, 6,
/* TAU+, TAU- */
     33, 34,
/* PI+, PI-, ETA, PI0 */
     8, 9, 17, 7,
/* K+, K-, KS, KL */
     11, 12, 16, 10,
/* NEUTRINO NUE, NUMU, NUTAU */
      4, 4, 4, 4, 4, 4,
/* PROTON */
     14, 15,
/* NEUTRON */
     13, 25,
/* SIGMA+, ANTI(SIGMA+), SIGMA-, ANTI(SIGMA-),SIGMA0, ANTISIGMA0, */
     19, 27, 21, 29, 20, 28,
/* XI 0, ANTIXI 0, XI-, ANTI(XI-) */
     22, 30, 23, 31,
/* LAMBDA, ANTILAMBDA */
      18, 26,
/* OMEGA- , ANTI-(OMEGA-) */
     24, 32};
  
  for(i=0;i<sizeof(Jetset)/sizeof(int);i++)
    {
      if(code==Jetset[i])
	return Geant[i];
    };
  return -1;
}

#ifdef __cplusplus
}
#endif

