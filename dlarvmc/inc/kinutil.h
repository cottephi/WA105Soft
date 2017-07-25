
/* NEGLIB analyze */

/* Utilities for analysis based on the Neglib output file */
/* Send comments to A. Rubbia, 21/8/98 */
/* */

#ifndef _KINUTIL_H_
#define _KINUTIL_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "negutil.h"

#define BETHE 1
#define IONIZATION 0.2 /* GeV/m */
#define HALLANGLE 122.5 // Angle between north and CERN-LNGS line (in degrees)


/* To simulate liquid-solid events */
//#define ICANOE

extern int ismearPO; // Decide whether you want to smear primary particles 
             //  according to fast simulation                
extern int event_simul_type;
extern float  n_neutron, n_K0l;
extern int igammaconv;
extern float thetahadmc;
extern double PrimCoor[3];
extern double range_mu,range_proton;

extern struct KINEMATICREC RecKine;            /* The reconstructed kinematics */

extern struct KINEMATICREC Rot_RecKine;        /* The reconstructed kinematics */
extern _RecTauVars         Rot_TauKine;


#define NMAX_PART_IN_JET 100
extern struct HADRONICJET_REC {
  int ntracks;
  int nvistracks;
  double Ptotmc[4];
  double Ptot[4];
  double E;
  double Emc;

  int icodepart[NMAX_PART_IN_JET];   /* geant code of the particles */
  float xmcppart[NMAX_PART_IN_JET];  /* generated momentum of the particles */
  float xmcepart[NMAX_PART_IN_JET];  /* generated energy of the particles */
  float ppart[NMAX_PART_IN_JET];     /* measured momentum of the particles */
  float epart[NMAX_PART_IN_JET];     /* measured energy of the particles */
  int   interaction[NMAX_PART_IN_JET]; /* type of interaction of the particle */
  float angres[NMAX_PART_IN_JET];      /* angular resolution of the particle */
} HadronicJet;

/************************************************************************************/
/*                           P O     D E F I N I T I O N                            */
/************************************************************************************/
typedef enum {
  POUndefType, 

  POCharged, 
  PONeutral,
  POElectron, 
  POMuon
} _PObjectType; 

#define MAXPO 100

typedef struct MICROPOREC microPO;
struct MICROPOREC {
  int          iCode;
  int          GeantId;
  _PObjectType POtype;
  double       p[4];
  double       depp[4];
  double       Emeasured;   /* Measured total energy */
  double       MassMC;      /* monte-carlo mass */

  double       preso; /* Momentum resolution of reconstructed track */
  double       angular_res;   /* angular resolution of reconstructed track */
  double       pmc[4];      /* generator level momentum of particle */

  int          UseIt;
  int          qtype;       /* charge */
  int          HasInteracted;
  double       WallDistance;
  double       Length;
  double       qtmc;
  double       range;
  double       endvertex[3]; /* end vertex of the track */

  /* stuff filled only when reading full simulation */
  int          iTrack;      /* the track number */
  double       EdepAr;      /* deposited energy in argon */
  double       Tmc;         /* generator level kinetic energy */
};

extern int NPO;
extern microPO PO[MAXPO];

void compute_event_ptot(int);
void compute_event_rotation(void);
void dump_PO_table(void);
void smear_PO(void );
void smear_PO_liquid_solid(void );
double angular_resolution(double p[4]);
double pfermi(void );
void compute_qt_mc(void );
double ionization_loss(double p, double dist);

#define ICARUS_4MODULES 1
#define ICARUS_SUPERI   2
#define ICARUS_600T     3
#define ICARUS_1KT      4
#define ICANOE1         5
#define ICANOE2         6
#define FINAL_ICANOE    7

void initialize_geometry(int ichoice);

int get_tau_decay_mode(void);
int get_lepto_tau_decay_mode(void);
void get_lepto_tau_child_kine(double child[4]);
void get_lepto_tau_pi_child_kine(double child[4]);
void get_lepto_tau_pi0_child_kine(double child[4]);
void change_to_atm_coordinates(double target[3], double source[3]);
void change_from_geographical_to_atm(double target[3], double source[3]);
int is_lepto_tau_parent(int ilepto);
void simulate_photon_conversion(void );
float muons_from_decay(void);


#endif
