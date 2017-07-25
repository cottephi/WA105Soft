/************************************************************************************/
/*                                  C O N S T A N T S                               */
/************************************************************************************/
#define PIGREEK 3.14159265359
#define NEUTRONMASS 0.93956533
#define PROTONMASS 0.93827231
#define PROTONMASS2 0.880354928
#define PIZEMASS  0.13498
#define PICHMASS2 0.01948 /* mass of charged pion squared */
#define LAMBDAMASS 1.115684
#define ELECMASS 0.511e-3
#define MUONMASS 0.1057
#define TAUMASS 1.777

#define K0SMASS  0.49767
#define KCHMASS  0.49368
#define PICHMASS 0.13957

#define K0SPIPI  0.6861 /*  Probability   K0s -> pi+ pi-   */
#define K0SP0P0  0.3139 /*  Probability   K0s -> pi0 pi0   */

#define JETSET_TAU    15
#define JETSET_PHOTON 22
#define JETSET_NEUTRON 2112
#define JETSET_ELECTRON 11
#define JETSET_MUON 13
#define JETSET_PROTON 2212
#define JETSET_KCHARGED 321
#define JETSET_K0S 310
#define JETSET_K0L 130
#define JETSET_PIZERO 111
#define JETSET_PICHARGED 211
#define JETSET_LAMBDA    3122

#define GEANT_PHOTON       1
#define GEANT_ELECTRON     3
#define GEANT_POSITRON     2
#define GEANT_MUPLUS       5
#define GEANT_MUMINUS      6
#define GEANT_PIZERO       7
#define GEANT_PIPLUS       8
#define GEANT_PIMINUS      9
#define GEANT_K0L         10
#define GEANT_KPLUS       11
#define GEANT_KMINUS      12
#define GEANT_NEUTRON     13
#define GEANT_PROTON      14
#define GEANT_ANTIPROTON  15
#define GEANT_K0S         16
#define GEANT_LAMBDA      18
#define GEANT_ANTINEUTRON 25

/************************************************************************************/
/*                                      A L G E B R A                               */
/************************************************************************************/
#define SQR(a) ((a)*(a))
#define PTCALC(a) (sqrt(SQR(a[0])+SQR(a[1])))
#define DIST3(a,b) (sqrt(SQR(a[0]-b[0])+SQR(a[1]-b[1])+SQR(a[2]-b[2])))
#define DIST2(a,b) (sqrt(SQR(a[0]-b[0])+SQR(a[1]-b[1])))
#define INVMASS(a) (sqrt(SQR(a[3])-SQR(a[0])-SQR(a[1])-SQR(a[2])))
#define SCALAR2(a,b) (a[0]*b[0]+a[1]*b[1])
#define SCALAR3(a,b) (a[0]*b[0]+a[1]*b[1]+a[2]*b[2])

/************************************************************************************/
/*                               V E C T O R    A L G E B R A                       */
/************************************************************************************/
#define ENERGY(p,m)  (sqrt(SQR(p[0])+SQR(p[1])+SQR(p[2])+SQR(m)))
#define VUMOD3(d)    (sqrt(SQR(d[0])+SQR(d[1])+SQR(d[2])))
#define VUMOD2(d)    (sqrt(SQR(d[0])+SQR(d[1])))
#define VUZERO3(d)   d[0]=d[1]=d[2]=0;
#define VUZERO4(d)   d[0]=d[1]=d[2]=d[3]=0;
#define VUCOPY3(d,s) d[0]=s[0];d[1]=s[1];d[2]=s[2];
#define VUCOPY4(d,s) d[0]=s[0];d[1]=s[1];d[2]=s[2];d[3]=s[3];
#define VUSUM4(d,a,b) d[0]=a[0]+b[0];d[1]=a[1]+b[1];d[2]=a[2]+b[2];d[3]=a[3]+b[3];
#define VUSUM3(d,a,b) d[0]=a[0]+b[0];d[1]=a[1]+b[1];d[2]=a[2]+b[2];
#define VUSUB3(d,a,b) d[0]=a[0]-b[0];d[1]=a[1]-b[1];d[2]=a[2]-b[2];
#define VUSUB4(d,a,b) d[0]=a[0]-b[0];d[1]=a[1]-b[1];d[2]=a[2]-b[2];d[3]=a[3]-b[3];

#ifndef MIN
#  define MIN(a,b) ((a)<(b) ? (a) : (b))
#endif
#ifndef MAX
# define MAX(a,b) ((a)>(b) ? (a) : (b))
#endif
