#ifndef _HITRECO_H_
#define _HITRECO_H_

#include <cmath>
#include <cstdlib>
#include <string>
#include <sstream>
#include <iostream>
#include "TString.h" 

using namespace std;

#define MAXHITRFLVL 50  
#define MAXHITRINTVL 50 
#define MAXHITRSTRVL 50  
 
class HitReco {

 public: 
 
  HitReco();
  virtual ~HitReco(){;}
  void DumpHitReco(); 


  /* getters */
  
  UInt_t   GetHITRecRunTime() {return HITRec_RunTime;} 
  TString  GetHITRecInFlname() {return HITRec_InFlname;}
  
  Int_t   GetHITRecFNum()  {return HITRec_Float;}
  Int_t   GetHITRecINum() {return HITRec_Int;}
  Int_t   GetHITRecSNum() {return HITRec_String;}
  Float_t GetHITRecF(int i) {return HITRec_PFloat[i];}
  Float_t GetHITRecI(int i) {return HITRec_PInt[i];}
  std::string  GetHITRecS(int i) {return HITRec_PString[i];} 
  /* setters */
  
   void SetHITRecTime(UInt_t RunTime)     {HITRec_RunTime=RunTime;}  
   void SetHITRecInFlname(TString finname) {HITRec_InFlname=finname;}
   
  
   void SetHITRecFNum (Int_t HITFNum)
   {
     CheckHitFNum(HITFNum);
     HITRec_Float=HITFNum;
    }  
   
   void SetHITRecINum (Int_t HITINum)
   {
     CheckHitINum(HITINum);
     HITRec_Int=HITINum;
    } 
    void SetHITRecSNum (Int_t HITSNum)
    {
     CheckHitSNum(HITSNum);
     HITRec_String=HITSNum;
    } 
    void SetHITRecF (int i, Float_t HitF) 
    {
     CheckHitFNum(i);
     HITRec_PFloat[i]=HitF;
    }
    void SetHITRecI (int i, Int_t HitI) 
    {
     CheckHitINum(i);
     HITRec_PInt[i]=HitI;
    } 
    void SetHITRecS (int i, std::string HitS) 
    {
     CheckHitSNum(i);
     HITRec_PString[i]=HitS;
    } 
  
  
   
  
private: 
   
   UInt_t  HITRec_RunTime;
   TString HITRec_InFlname;
   
   Int_t HITRec_Float;
   Int_t HITRec_Int;
   Int_t HITRec_String;
   Float_t HITRec_PFloat[MAXHITRFLVL];
   Int_t HITRec_PInt[MAXHITRINTVL];
   std::string HITRec_PString[MAXHITRSTRVL];
   
   void CheckHitFNum(int fnum);
   void CheckHitINum(int inum);
   void CheckHitSNum(int snum);
  
  
  ClassDef(HitReco,1);
    
};
 

#endif
