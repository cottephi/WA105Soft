// GENIE event structure
// A.Rubbia Feb08

#ifndef __GENIE_H__
#define __GENIE_H__

const int kNPmax = 100; 

struct GENIEVREC {
  int    brIev;             // "iev"      Event number 
  int    brNeutrino;        // "neu"      Neutrino pdg code
  int    brFSPrimLept;      // "fspl"     Final state lepton
  int    brTarget;          // "tgt"      Nuclear target pdg code (10LZZZAAAI)
  int    brZ;               // "Z"        Nuclear target Z
  int    brA;               // "A"        Nuclear target A
  int    brHitNuc;          // "hitnuc"   Hit nucleon pdg code      (not set for COH,IMD and NuEL events)
  int    brHitQrk;          // "hitqrk"   Hit quark pdg code        (set for DIS events only)
  bool   brFromSea;         // "sea"      Hit quark is from sea     (set for DIS events only)
  int    brResId;           // "resid"    Produced baryon resonance (set for resonance events only)
  bool   brIsQel;           // "qel"      Is QEL?
  bool   brIsRes;           // "res"      Is RES?
  bool   brIsDis;           // "dis"      Is DIS?
  bool   brIsCoh;           // "coh"      Is COH?
  bool   brIsDfr;           // "dfr"      Is DFR?
  bool   brIsImd;           // "imd"      Is IMD?
  bool   brIsNuEL;          // "nuel"     Is NUEL?
  bool   brIsEm;            // "em"
  bool   brIsCC;            // "cc" Is CC?
  bool   brIsNC;            // "nc" Is NC?
  bool   brIsCharmPro;      // "charm" Produces charm?
  int    brNeut_code;       // "neut_code"
  int    brNuance_code;     // "nuance_code"
  double brWeight;          // "wght" Event weight
  double brKineXs;          // "xs" Bjorken x (selected)
  double brKineYs;          // "ys" Inelasticity y (selected)
  double brKineTs;          // "ts" Energy transfer to nucleus at COHPi events (selected)
  double brKineQ2s;         // "Q2s" Momentum transfer Q^2 (selected)
  double brKineWs;          // "Ws" Hadronic invariant mass W (selected)
  double brKineX;           // "x" Bjorken x  (computed from the event record)
  double brKineY;           // "y" Inelasticity y (computed from the event record)
  double brKineT;           // "t" Energy transfer to nucleus at COHPi events (computed from the event record)
  double brKineQ2;          // "Q2" Momentum transfer Q^2 (computed from the event record)
  double brKineW;           // "W" Hadronic invariant mass W (computed from the event record)
  double brEv;              // "Ev" Neutrino energy (neutrino assumed in +z direction)
  double brPxv;             // "pxv" Neutrino momentum
  double brPyv;             // "pyv" Neutrino momentum
  double brPzv;             // "pzv" Neutrino momentum
  double brEn;              // "En" Initial state hit nucleon energy
  double brPxn;             // "pxn" Initial state hit nucleon px
  double brPyn;             // "pyn" Initial state hit nucleon py
  double brPzn;             // "pzn" Initial state hit nucleon pz
  double brEl;              // "El" Final state primary lepton energy
  double brPxl;             // "pxl" Final state primary lepton px
  double brPyl;             // "pyl" Final state primary lepton py
  double brPzl;             // "pzl" Final state primary lepton pz 
  double brPl;              // "pl"
  double brCthl;            // "cthl"
  int    brNfP;             // "nfp" Number of final state p's + \bar{p}'s (after intranuclear rescattering)
  int    brNfN;             // "nfn" Number of final state n's + \bar{n}'s
  int    brNfPip;           // "nfpip" Number of final state pi+'s
  int    brNfPim;           // "nfpim" Number of final state pi-'s
  int    brNfPi0;           // "nfpi0" Number of 'final state' pi0's (
  int    brNfKp;            // "nfkp" Number of final state K+'s
  int    brNfKm;            // "nfkm" Number of final state K-'s
  int    brNfK0;            // "nfk0" Number of final state K0's + \bar{K0}'s
  int    brNfEM;            // "nfem" Number of final state gammas and e-/e+ (excluding pi0 decay products)
  int    brNfOther;         // "nfother" Number of heavier final state hadrons (D+,D-,D0,Ds+,Ds-,Lamda,Sigma,Lamda_c,Sigma_c,...)
  int    brNiP;             // "nip" Number of 'primary' (: before intranuclear rescattering) p's + \bar{p}'s  
  int    brNiN;             // "nin" Number of 'primary' n's + \bar{n}'s  
  int    brNiPip;           // "nipip" Number of 'primary' pi+'s 
  int    brNiPim;           // "nipim" Number of 'primary' pi-'s 
  int    brNiPi0;           // "nipi0" Number of 'primary' pi0's 
  int    brNiKp;            // "nikp" Number of 'primary' K+'s  
  int    brNiKm;            // "nikm" Number of 'primary' K-'s  
  int    brNiK0;            // "nik0" Number of 'primary' K0's + \bar{K0}'s 
  int    brNiEM;            // "niem" Number of 'primary' gammas and e-/e+ (eg from resonance decays)
  int    brNiOther;         // "niother" Number of 'primary' hadron shower particles
  int    brNi;              // "ni" Number of particles in 'primary' hadronic system (before intranuclear rescattering)
  int    brPdgi[kNPmax];    // "pdgi[ni]" Pdg code of i^th particle in 'primary' hadronic system 
  int    brResc[kNPmax];    // "resc[ni]"
  double brEi  [kNPmax];    // "Ei  [ni]" Energy of i^th particle in 'primary' hadronic system 
  double brPxi [kNPmax];    // "pxi [ni]" Px of i^th particle in 'primary' hadronic system 
  double brPyi [kNPmax];    // "pyi [ni]" Py of i^th particle in 'primary' hadronic system 
  double brPzi [kNPmax];    // "pzi [ni]" Pz of i^th particle in 'primary' hadronic system
  int    brNf;              // "nf" Number of final state particles in hadronic system
  int    brPdgf[kNPmax];    // "pdgf[nf]" Pdg code of i^th final state particle in hadronic system
  double brEf  [kNPmax];    // "Ef  [nf]" Energy   of i^th final state particle in hadronic system
  double brPxf [kNPmax];    // "pxf [nf]" Px of i^th final state particle in hadronic system
  double brPyf [kNPmax];    // "pyf [nf]" Py of i^th final state particle in hadronic system
  double brPzf [kNPmax];    // "pzf [nf]" Pz of i^th final state particle in hadronic system
  double brPf  [kNPmax];    // "pf  [nf]" P of i^th final state particle in hadronic system  
  double brCthf[kNPmax];    // "cthf[nf]"
  double brVtxx;            // "vtxx" vertex x coord
  double brVtxy;            // "vtxy" vertex y coord
  double brVtxz;            // "vtxz" vertex z coord
  double brVtxt;            // "vtxt" vertex time coord
  double brCalresp0;        // "calresp0" approximate calorimetric response
};

//struct GENIEVREC GENIEv; 

const int kNPmaxnd = 31; 

struct GENIENDREC {

   int  brIev ;      // Event number 
   int  brCha ;      // decay channel
   int  brnout ;  
    int brStatus [kNPmaxnd];   // statut code   
   int  brPdg   [kNPmaxnd];   // PDG code      
   double brEf  [kNPmaxnd];   // Energy        
   double brPxf [kNPmaxnd];   // Px            
   double brPyf [kNPmaxnd];   // Py            
   double brPzf [kNPmaxnd];   // Pz            
   double brvx  [kNPmaxnd];   // production x  
   double brvy  [kNPmaxnd];   // production y  
   double brvz  [kNPmaxnd];   // production z  
   double brvt  [kNPmaxnd];   // production t   
};
 
//struct GENIENDREC GENIEnd;

#endif

