/* DST analyze */

/* Utilities for analysis based on the DST */
/* Send comments to A. Rubbia, 1/11/95 */

#ifndef _NEGUTIL_H
#define _NEGUTIL_H

/***************************************************************************/
/*                             K I N E M A T I C S                         */
/***************************************************************************/
struct KINEMATICREC {
  double Ptot[4];
  double PLep[4];
  double PHad[4];
  double Enu;               /* Neutrino energy */
  double Elep;              /* Lepton energy */
  double Thetalep, Ptlep;   /* angle and pt of lepton */
  double Thetahad;          /* Angle of the jet */
  double Pthad;             /* pt of hadron jet */
  double q2;                /* Q2==4*enu*elep*sin**2(thetalep/2) */
  double w2,sqrtw2;         /* W2==-Q2+2Mnu+M**2 */
  double nu;                /* nu==enu-elep */
  double xbj,ybj;           /* scaling variables x=q2/2m*nu; y=elep/enu */
  
  KINEMATICREC() {
    Ptot[0]=Ptot[1]=Ptot[2]=Ptot[3]=0;
    PLep[0]=PLep[1]=PLep[2]=PLep[3]=0;
    PHad[0]=PHad[1]=PHad[2]=PHad[3]=0;
  }
};

/***********************************************************************************/
/*     G E N E R A L   S I M U L A T  E D   E V E N T    I N F O R M A T I O N     */
/***********************************************************************************/

struct NEGEVENTREC {

  /* Lepto Quantities (in neutrino frame if px,py,pz)*/
  int    Lepto_NCharged;
  int    Lepto_GNeutral;
  int    Lepto_HNeutral;
  double Lepto_Enu;
  double Lepto_Evis;
  double Lepto_Charged[4];         /* charged */
  double Lepto_G_Neutral[4];       /* gamma */
  double Lepto_H_Neutral[4];       /* neutral hadrons */
  double Lepto_Tot[4];             /* Total */
  double Lepto_Rec_PflowDif[4];    /* the Eflow difference */
  double Lepto_Rec_Ptres;          /* the Pt resolution */

  int    Lepto_Ev_Classify;        /* Quick classification event */
  /* Nprot = protons in final state */
  /* Nneut = neutrons in final state */
  /* NK    = K0L or K0S in final state */
  /* Nc    = other charged (pi,k,..but also MU!) */
  /* Ng    = gammas */
  /* If NK==0 :  */
  /*   If Nprot=1,Nneut=0 --> Lepto_Ev_Classify = Min(Nc+Ng,19) */
  /*   If Nprot=0,Nneut=1 --> Lepto_Ev_Classify = 20+Min(nc+ng,19) */
  /*   If nbaryons>1      --> Lepto_Ev_Classify = 40+Min(nc+ng,19) */
  /* If NK>0              -->                   = 60+Min(nc+ng,19) */
};

typedef struct NEGEVENTREC _NegEventR;

extern _NegEventR NegEv;


/***************************************************************************/
/*         T A U    D I S C R I M I N A T I N G    V A R I A B L E S       */
/***************************************************************************/
struct TAUVARIABLES {
  double ptot[4];          /* TOTAL EVENT MOMENTUM USED */
  double ptau[4];          /* TAU MOMENTUM USED */
  double phad[4];
  double pttau, pthad;     /* PT of the tau, of the hadronic system */
  double ptmiss;           /* Missing pt */
  double pt_ass;           /* Pt assymetry */
  double pt_ratio;         /* so-called Pt ratio */
  double mt;               /* Transverse mass */
  double qt;               /* Qt w.r.t. total momentum */
  double qlep;             /* Qt w.r.t. hadron jet vector */
  double fith,fimh;        /* Albright angles */
  double rhol,rhoh;        /* Normalized Dalitz variables */
  double ptt, ptl;         /* so called Ptt, Ptl */
  double mass_isol;        /* Mass isolation */
  double theta;            /* angle between ptau and ptot in space */
  int sim_flag;            /* it is 1 if MC information is used, 
                              and -1 then MC track for tau is not found */
};
typedef struct TAUVARIABLES _RecTauVars;

/************************************************************************************/
/*                    N U C L E A R      R E I N T E R A C T I O N                  */
/************************************************************************************/

struct SIMREINTREC {
  /* nuclear reinteractions */
  int    Lepto_NReinteract;            /* number of particles reinteracted */
  double Lepto_Tot_NoReinteract[4];    /* Total Ptot BEFORE reinteractions !*/
  int    Lepto_Ncharged_NoReinteract;  /* total charged multiplcity before reint */
  double Lepto_Diff_Reinteract[4];     /* Pt vector diff introduced by reinteractions */

  struct KINEMATICREC Kine;            /* The kinematics in the lab BEFORE reinteractions */

  _RecTauVars TauSimEv;                /* The tau variables from reinteractions */

};

extern struct SIMREINTREC SimReint;

/***********************************************************************************/
/*    F L A G S   F O R   F A K E   N E U T R A L    C U R R E N T,  E T C.        */
/***********************************************************************************/

/* following global variables are input on data cards */
extern int fake_numunc,  /* 1=fake NC from nu_mu CC, with mu Veto; 2=fake, no mu V; 0=normal job */
    fake_nuecc,   /* 1=fake nu_e CC from nu_mu CC; 0=normal job */
    fake_nutaucc, /* 1=fake taue->e from nu_mu CC with wt; 2=fake, no wt; 0=normal job */
    lepton_choice, /* 0=electron, 1=muon */
    global_dump_flag; /* 0=no big dumps; non-zero for various dumps */

extern int candidate_selection;

/***********************************************************************************/
/*                             G E N E R A L     F L A G S                         */
/***********************************************************************************/
extern int idumpGeVMeV;

/************************************************************************************/
/*                              P R O T O T Y P E S                                 */
/************************************************************************************/

#ifdef __cplusplus
extern "C"
{
#endif

void transform_to_nu(double s[3],double d[3]);
void transform_from_nu(double s[3],double d[3]);
void zboost(double prom[4], double p[4], double pb[4]);

void compute_Lepto_ptot(void);
int count_lepto_reinteractions(void);
int is_lepto_reinteraction_parent(int ilepto);
void check_lepto_reinteractions(void);
void dump_lepto_reinteractions(void);
void dump_lepto_full(FILE *f);
void dump_lepto_full_stdout(void);  /* for debugger */
void dump_lepto_linear(FILE *f);

void compute_kinematic_rec(struct KINEMATICREC *k);
_RecTauVars tau_variables_struct(double ptot[4], double ptau[4]);

int ToGeantCode(int code);
int is_lepto_charged(int i);


#ifdef IT_IS_ONLY_CI_PROTO
void compute_Lepto_ptot();               /* ci_proto */
void dump_lepto_reinteractions();        /* ci_proto */
#endif

#ifdef __cplusplus
}
#endif

#endif
