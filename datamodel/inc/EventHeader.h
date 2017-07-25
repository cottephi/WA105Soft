#ifndef EventHeader_h
#define EventHeader_h


#include <iostream>
#include <vector>
#include <TROOT.h>
#include <TTimeStamp.h>
#include <TObjArray.h>
#include <TParticle.h>


#define MAXCRM 50   

class EventHeader {

public:

  EventHeader();
  virtual ~EventHeader();
  
  void DumpEventHeader_True(); 

  /* methods to access member */

  Int_t GetEventNumber() {return EH_EventNumber;}
  TTimeStamp& GetEventTime(){ return EH_EventTime; }
  Bool_t  GetDQFlag() { return EH_DQFlag; }
  Float_t GetTrueP() {return EH_TrueP;}
  Float_t GetTrueMomentum(int i) {return EH_TrueMomentum[i];}
  Float_t GetTrueTheta() {return EH_TrueTheta;}
  Float_t GetTruePhi()   {return EH_TruePhi;}
  Float_t GetTrueProdX() {return EH_TrueProdX;}  
  Float_t GetTrueProdY() {return EH_TrueProdY;}  
  Float_t GetTrueProdZ() {return EH_TrueProdZ;}  
  Float_t GetTrueTotDepEne() {return EH_TrueTotDepEne;}
  Float_t GetTrueTotDepEneQ() {return EH_TrueTotDepEneQ;}   
  Float_t GetTrueTotDepEEne() {return EH_TrueTotDepEEne;}   
  Float_t GetTrueTotDepEEneQ() {return EH_TrueTotDepEEneQ;} 
  Float_t GetTrueTotDepHEne() {return EH_TrueTotDepHEne;}   
  Float_t GetTrueTotDepHEneQ() {return EH_TrueTotDepHEneQ;} 
  Float_t GetTrueEneFromBeam()  {return EH_TrueEneFromBeam;}
  Float_t GetTrueEneFromCRBack() {return EH_TrueEneFromCRBack;}  
  Float_t GetTrueEneLost() {return EH_TrueEneLost;}  
  Float_t GetTrueTotDepEneCRT1() {return EH_TrueTotDepEneCRT1;}
  Float_t GetTrueTotDepEneCRT2() {return EH_TrueTotDepEneCRT2;}

  Float_t GetRecP() {return EH_RecP;}
  Float_t GetRecMomentum(int i) {return EH_RecMomentum[i];}
  Float_t GetRecTheta() {return EH_RecTheta;}
  Float_t GetRecPhi()   {return EH_RecPhi;}
  Float_t GetRecProdX() {return EH_RecProdX;}  
  Float_t GetRecProdY() {return EH_RecProdY;}  
  Float_t GetRecProdZ() {return EH_RecProdZ;}  
  Float_t GetRecTotDepEne() {return EH_RecTotDepEne;}
  Float_t GetRecTotDepEneQ() {return EH_RecTotDepEneQ;}   
  Float_t GetRecTotDepEEne() {return EH_RecTotDepEEne;}   
  Float_t GetRecTotDepEEneQ() {return EH_RecTotDepEEneQ;} 
  Float_t GetRecTotDepHEne() {return EH_RecTotDepHEne;}   
  Float_t GetRecTotDepHEneQ() {return EH_RecTotDepHEneQ;}
  


  TObjArray* GetPrimaryArray(){ return EH_Primaries; }
 
  /*  beam */

  Float_t GetBCh1 (int i)  {return  EH_BCh1[i];} 
  Float_t GetBCh2 (int i) {return  EH_BCh2[i];} 
  Float_t GetEvTof()  {return  EH_EvTof;}        
  Float_t GetEvCherenkov (int i) {return EH_EvCherenkov[i];}  

 /* reconstruction results */
 
  Int_t GetNumRecHit0(int i) {return EH_nhitv0[i];}
  Int_t GetNumRecHit1(int i) {return EH_nhitv1[i];}

  // total number of reconstructed tracks in each view
  Int_t GetNumTrkCRP0() { return EH_crptrks0; }
  Int_t GetNumTrkCRP1() { return EH_crptrks1; }


  /* methods to set member */
 
  void SetEventNumber (Int_t EventNumber) {EH_EventNumber=EventNumber;}
  void SetEventTime( TTimeStamp &ts ){ EH_EventTime = ts; }
  void SetEventTime(Int_t sec, Int_t nsec){ EH_EventTime.SetSec(sec); EH_EventTime.SetNanoSec(nsec); }
  void SetEventTime(const timespec_t &ts){ EH_EventTime = TTimeStamp( ts ); }
  void SetDQFlag(Bool_t val){ EH_DQFlag = val; }
  void SetTrueP (Float_t tp ) {EH_TrueP=tp;}
  void SetTrueMomentum (int i, Float_t tmom) {EH_TrueMomentum[i]=tmom;}
  void SetTrueTheta    (Float_t tTheta) { EH_TrueTheta= tTheta;}
  void SetTruePhi   (Float_t tPhi) {EH_TruePhi=tPhi;}
  void SetTrueProdX (Float_t tProdX) {EH_TrueProdX=tProdX;}
  void SetTrueProdY (Float_t tProdY) {EH_TrueProdY=tProdY;}
  void SetTrueProdZ (Float_t tProdZ) {EH_TrueProdZ=tProdZ;}
  void SetTrueTotDepEne (Float_t tTotDepEne) { EH_TrueTotDepEne= tTotDepEne;}
  void SetTrueTotDepEneQ (Float_t tTotDepEneQ) { EH_TrueTotDepEneQ= tTotDepEneQ;}
  void SetTrueTotDepEEne (Float_t tTotDepEEne) { EH_TrueTotDepEEne= tTotDepEEne;}
  void SetTrueTotDepEEneQ (Float_t tTotDepEEneQ) { EH_TrueTotDepEEneQ= tTotDepEEneQ;}
  void SetTrueTotDepHEne (Float_t tTotDepHEne) { EH_TrueTotDepHEne= tTotDepHEne;}
  void SetTrueTotDepHEneQ (Float_t tTotDepHEneQ) { EH_TrueTotDepHEneQ= tTotDepHEneQ;}
  void SetTrueEneFromBeam(Float_t tEneFromBeam) {EH_TrueEneFromBeam=tEneFromBeam;}
  void SetTrueEneFromCRBack (Float_t tEneFromCRBack) {EH_TrueEneFromCRBack=tEneFromCRBack;}
  void SetTrueEneLost (Float_t tval) {EH_TrueEneLost = tval;}
  void SetTrueTotDepEneCRT1 (Float_t tTotDepEneCRT1) { EH_TrueTotDepEneCRT1= tTotDepEneCRT1;}
  void SetTrueTotDepEneCRT2 (Float_t tTotDepEneCRT2) { EH_TrueTotDepEneCRT2= tTotDepEneCRT2;}

  void SetRecP (Float_t rp ) {EH_RecP=rp;}
  void SetRecMomentum (int i, Float_t rmom) {EH_RecMomentum[i]=rmom;}
  void SetRecTheta	(Float_t rTheta) { EH_RecTheta= rTheta;}
  void SetRecPhi   (Float_t rPhi) {EH_RecPhi=rPhi;}
  void SetRecProdX (Float_t rProdX) {EH_RecProdX=rProdX;}
  void SetRecProdY (Float_t rProdY) {EH_RecProdY=rProdY;}
  void SetRecProdZ (Float_t rProdZ) {EH_RecProdZ=rProdZ;}
  void SetRecTotDepEne (Float_t rTotDepEne) { EH_RecTotDepEne= rTotDepEne;}
  void SetRecTotDepEneQ (Float_t rTotDepEneQ) { EH_RecTotDepEneQ= rTotDepEneQ;}
  void SetRecTotDepEEne (Float_t rTotDepEEne) { EH_RecTotDepEEne= rTotDepEEne;}
  void SetRecTotDepEEneQ (Float_t rTotDepEEneQ) { EH_RecTotDepEEneQ= rTotDepEEneQ;}
  void SetRecTotDepHEne (Float_t rTotDepHEne) { EH_RecTotDepHEne= rTotDepHEne;}
  void SetRecTotDepHEneQ (Float_t rTotDepHEneQ) { EH_RecTotDepHEneQ= rTotDepHEneQ;}

  
  void SetPrimary( TParticle *p );
  void SetPrimaryArray( TObjArray *arr );

  /* CR background */
 
  /* beam */
  void SetBCh1 (int i, Float_t  BCh1 ) {EH_BCh1[i]=BCh1;}
  void SetBCh2 (int i, Float_t  BCh2 ) {EH_BCh2[i]=BCh2;}
  void SetEvTof (Float_t Ev_Tof) {EH_EvTof=Ev_Tof;}
  void SetEvCherenkov (int i, Float_t EvCherenkov) {EH_EvCherenkov[i]=EvCherenkov;}


/* reconstruction results */
 
  void SetNumRecHit0(int i, int hnum) { EH_nhitv0[i]=hnum;}
  void SetNumRecHit1(int i, int hnum) { EH_nhitv1[i]=hnum;}

  void SetNumTrkCRP0(int val) { EH_crptrks0 = val; }
  void SetNumTrkCRP1(int val) { EH_crptrks1 = val; }

 private:

  Int_t    EH_EventNumber;     /* event number */
  TTimeStamp EH_EventTime;     /* time stamp   */
  Bool_t   EH_DQFlag;          /* event data quality flag */
  Float_t  EH_TrueP;           /*particle momentum    MC true value  */ 
  Float_t  EH_TrueMomentum[3]; /*  MC true value px, py, pz*/ 
  Float_t  EH_TrueTheta;  /*theta    MC true value */ 
  Float_t  EH_TruePhi;  /* phi   MC true value */ 
  /*these 3 values have been added */
  Float_t  EH_TrueProdX;  /* x vertex production   MC true value */
  Float_t  EH_TrueProdY;  /* y vertex production   MC true value  */
  Float_t  EH_TrueProdZ;   /* z vertex production   MC true value  */

  Float_t  EH_TrueTotDepEne;   /* total deposited energy  MC true value  */
  Float_t  EH_TrueTotDepEneQ;   /* total deposited energy  (quenched) */
  Float_t  EH_TrueTotDepEEne;   /* total deposited em energy   MC true value  */
  Float_t  EH_TrueTotDepEEneQ;   /* total deposited em energy (quenched)  MC true value  */
  Float_t  EH_TrueTotDepHEne;   /* total deposited had energy  MC true value  */
  Float_t  EH_TrueTotDepHEneQ;  /* total deposited had energy (quenched)  MC true value  */ 

  Float_t  EH_TrueEneFromBeam;   /* energy deposited by beam particle  MC true value  */
  Float_t  EH_TrueEneFromCRBack;  /* energy deposited by CR backgroud  MC true value  */
  Float_t  EH_TrueEneLost;          // lost energy out of fiducial volume
  Float_t  EH_TrueTotDepEneCRT1;   /*  energy deposited in CRT1 */
  Float_t  EH_TrueTotDepEneCRT2;   /*  energy deposited in CRT2 */

  Float_t  EH_RecP; /*particle momentum   reconstrcuted  value  */ 
  Float_t  EH_RecMomentum[3]; /* rec. value px, py, pz*/ 
  Float_t  EH_RecTheta;  /*theta   Rec value */ 
  Float_t  EH_RecPhi;  /* phi  Rec value */ 
  
  Float_t  EH_RecProdX;  /* x vertex production  reconstructed value */
  Float_t  EH_RecProdY;  /* y vertex production   reconstructed value  */
  Float_t  EH_RecProdZ;   /* z vertex production  reconstructed value  */

  Float_t  EH_RecTotDepEne;   /* total deposited energy reconstructed value */
  Float_t  EH_RecTotDepEneQ;   /* total deposited energy  reconstructed value(quenched) */
  Float_t  EH_RecTotDepEEne;   /* total deposited em energy  reconstructed value  */
  Float_t  EH_RecTotDepEEneQ;	/* total deposited em energy (quenched) reconstructed value  */
  Float_t  EH_RecTotDepHEne;   /* total deposited had energy  reconstructed value  */
  Float_t  EH_RecTotDepHEneQ;  /* total deposited had energy (quenched) reconstructed value  */ 

   
 
  
 
  /* beam */
  Float_t EH_BCh1[2];  /* x y coordinate form beam chamber 1 */
  Float_t EH_BCh2[2];  /* x y coordinate form beam chamber 1 */
  Float_t EH_EvTof;    /* Time of flight value */
  Float_t EH_EvCherenkov[3];  /* Cherenkov */

/* reconstruction results */

  Int_t  EH_nhitv0[MAXCRM];
  Int_t  EH_nhitv1[MAXCRM];

  // tracks reconstructed in CRP
  Int_t  EH_crptrks0;
  Int_t  EH_crptrks1;

  TObjArray* EH_Primaries;
  
  ClassDef(EventHeader,3);


};

#endif



