/////////////////////////////////////////////////////////////////////////////////
//
//  Basic 2D hit
//
//  
//  
//
/////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <iomanip>

#include "QHit.h"
using namespace std;

ClassImp(LArReco::QHit)


// ctor 1
LArReco::QHit::QHit()
{
  fThisObj = LArReco::RecoObjLink(LArReco::kQHit, -1);  
  Reset();
}

// ctor 2
LArReco::QHit::QHit( Int_t crmid, Int_t iview, Int_t chnum)
{
  fThisObj = LArReco::RecoObjLink(LArReco::kQHit, -1);
  Reset();

  crmId  = crmid;
  iView  = iview;
  chId   = chnum;
}

// ctor 3
LArReco::QHit::QHit( Int_t crmid, Int_t iview, Int_t chnum, 
		     UInt_t tstart, UInt_t tend, UInt_t tmax,
		     Double_t qmax, Double_t qtot )
{
  fThisObj = LArReco::RecoObjLink(LArReco::kQHit, -1);

  Reset();

  crmId   = crmid;
  iView   = iview;
  chId    = chnum;
  tbStart = tstart;
  tbEnd   = tend;
  tbMax   = tmax;
  Ampl    = qmax;
  Qtot    = qtot;

  ValidateHitParam();
}

// dtor
LArReco::QHit::~QHit()
{;}

// check if hit parameters are valid
Bool_t LArReco::QHit::ValidateHitParam()
{
  // perform some basic checks
  isValid = true;

  if( Ampl < 0 )
    {
      cerr<<"ERROR: negative amplitude = "<<Ampl<<endl;
      isValid = false;
      Print();

      return isValid;
    }
  
  //if( Qtot > MAXQHIT || Qtot < MINQHIT || Qtot != Qtot)
  if( Qtot != Qtot)
    {
      cerr<<"ERROR: charge for the hit appears to be bad "<<Qtot<<endl;
      isValid = false;
      Print();

      return isValid;
    }
    
  if( (tbStart > tbEnd) || (tbMax > tbEnd ) || (tbMax < tbStart) )
    {
      cerr<<"ERROR: issue with hit timing "<<endl;
      isValid = false;
      Print();

      return isValid;
    }

  return isValid; // hit is ok
}

void LArReco::QHit::ShiftHitTb(UInt_t t0)
{
  tbStart += t0;
  tbEnd   += t0;
  tbMax   += t0;
}

void LArReco::QHit::Reset()
{
  crmId  = -1;
  iView  = -1;
  chId   = -1;
  tbStart = tbEnd = tbMax = 0;
  Ampl = Qtot = 0;
  
  isValid = false;

  Ht = Hx = Hd = 0.0;
}

//
// total true charge
//
Double_t LArReco::QHit::GetQTotMC()  const 
{
  Double_t rval = 0.0;
  for (int nc=0; nc<MCHITINFO; nc++) rval+=HitMCQ[nc];
  return rval;
}

// print basic information
void LArReco::QHit::Print() const
{
  //Float_t  MCCharge;  
  cout << "<============= Basic Hit information =============>" << endl;
  cout << "     Hit index : "<< GetHitIndex() << endl;
  cout << "     Hit status (1-ok / 0-not ok) : "<< isValid << endl;
  cout << "     CRM module     : "<< crmId << endl;
  cout << "     View type      : "<< iView << endl;
  cout << "     Hit channel    : "<< chId << endl; 
  cout << "     First time bin : "<< tbStart << endl;
  cout << "     Last time bin  : "<< tbEnd << endl;
  cout << "     Total charge   : "<< Qtot << endl;
  cout << "     Peak value     : "<< Ampl << endl;
  cout << "     Peak time bin  : "<< tbMax << endl;
  cout << "     Position (cm)  : "<< Hx << endl;
  cout << "     Time (us)      : "<< Ht <<endl;
  cout << "     Distance to CRP : "<< Hd <<endl;
  //cout << "     PDG code       : "<< pdgCode << endl; 
  //for (int nc=0; nc<MCHITINFO; nc++) MCCharge+=HitMCQ[nc];
  //cout << "     MC true charge : "<< MCCharge<<endl;  
  cout << "     MC true charge : "<< GetQTotMC()<<endl;  
  cout << "Different contributions: "<<endl;
  cout<<"  muon "<<   HitMCQ[0]<<endl; 
  cout<<"(e- and e+), gammas "<< HitMCQ[1]<<"  "<<HitMCQ[2]<<endl;   
  cout<<"   pi0  "<< HitMCQ[3]<<endl; 
  cout<<" pi+ and  pi- "<< HitMCQ[4]<<endl; 
  cout<<" others "<< HitMCQ[5]<<endl; 
              
}

//
// function to enforce linking hierarchy
//
bool LArReco::QHit::CheckUplinkPolicy(RecoObjLink &ref)
{
  // can only link up to track, shower segments or 3d point
  bool rval = ( ref.ObjType == LArReco::kTrack2d  || 
		ref.ObjType == LArReco::kShower2d ||
		ref.ObjType == LArReco::kPoint3d );
  return rval;
}

//
// function to enforce linking hierarchy
//
bool LArReco::QHit::CheckDnlinkPolicy(RecoObjLink &ref)
{
  // there is nothing lower than a hit 
  // in the reco obj hierarchy
  return false;
}


