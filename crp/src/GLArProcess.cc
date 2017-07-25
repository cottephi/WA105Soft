/***********
 *
 *
 *
 *
 *
 *
 *
 *
***********/




//
#include <iostream>
#include <stdio.h>
#include <vector>

//
#include <TString.h>
#include <TMath.h>
#include <TH1F.h>

#include <TRandom3.h>
//

//
#include "HEPSystemOfUnits.h"

//#include "NoElProcConfiguration.h"
#include "GLArProcDB.h"

#include "GLArProcess.h"


using std::cout;
using std::endl;


//
// -------------------------------------
//


GLArProc::GLArProc() {
  /**
   *
   */


  Init();



}


GLArProc::GLArProc(const GLArProc&) {


}


GLArProc::~GLArProc() {

}

void GLArProc::Print() const {
/**


 */

    using std::cout;
    using std::endl;

    cout<< endl <<"  ----- GLArProc::Print ----- "<<endl;

    cout<<" IsApplicable "<<fIsApplicable<<endl;
    cout<<" Material ";
    if(fElement==kArgon) cout<<" Argon ";
    if(fPhase==kLiquid) cout<<" Liquid ";
    if(fPhase==kGas) cout<<" Gas ";
    cout<<endl;

    cout<<" Z: "<<fMaterialZ<<" A: "<<fMaterialA<<endl;

    cout<<" density "<<fDensity/(g/cm3)<<" [g/cm3]"<<" ref: "<<fRefDensity/(g/cm3)<<" [g/cm3]"<<endl;

    cout<<" Particle: "<<fParticleName<<" Z: "<<fPartZ<<" A: "<<fPartA<<endl;

    cout<<" Deposited Energy: "<<fdE/MeV<<" [MeV]"<<endl;
    cout<<" Track Length: "<<fdx/cm<<" [cm]"<<endl;
    if(fdx>0){
	cout<<" dE/dx: "<<fdE/fdx/(MeV/cm)<<" [MeV/cm]"<<endl;
	cout<<" LET: "<<fdE/fdx/fDensity/(MeV*cm2/g)<<" [MeV*cm2/g]"<<endl;
    }
    cout<<" Kinetic Energy: "<<fKin/MeV<<" [MeV]"<<endl;

    cout<<" ElectricField: "<<fElectricField/(kilovolt/cm)<<" [kV/cm]"<<endl;

    cout<<" IsModelDokeBirks: "<<fModelDokeBirks<<" UseLArRecombData: "<<fUseLArRecombData<<endl;
    cout<<" IsModelThomasImel: "<<fModelThomasImel<<endl;

    cout<<" YieldFactor: "<<fYieldFactor<<endl;
    cout<<" ExcitationRatio "<<fExcitationRatio<<endl;
    cout<<" DokeBirks: "<<fDokeBirks_A<<" "<<fDokeBirks_B<<" "<<fDokeBirks_C<<endl;
    cout<<" ThomasImel: "<<fThomasImel<<endl;
    cout<<" RecombProb: "<<fRecombProb<<" R factor: "<<1-fRecombProb<<endl;

    cout<<" "<<endl;
    cout<<" MeanNumOfQuanta "<<fMeanNumOfQuanta<<endl;
    cout<<" MeanNumOfExcit "<<fMeanNumOfExcit<<" MeanNumOfIons "<<fMeanNumOfIon<<endl;
    cout<<" MeanNumOfPhotons: "<<fMeanNumOfPhot<<" MeanNumOfElectrons: "<<fMeanNumOfEle<<endl;

    cout<<" NumOfQuanta: "<<fNumOfQuanta<<endl;
    cout<<" NumOfExcitions: "<<fNumOfExcit<<" NumOfIons: "<<fNumOfIon<<endl;
    cout<<" NumOfPhotons: "<<fNumOfPhot<<" NumOfElectrons: "<<fNumOfEle<<endl;

    cout<<" "<<endl;
    cout<<" SingletToTrilet Ratio, Excit: "<<fSingletToTripletX<<" Recomb: "<<fSingletToTripletR<<endl;
    cout<<" Tau Singlet "<<fTauSinglet/ns<<" triplet: "<<fTauTriplet/ns<<" recomb "<<fTauRecomb/ns<<" rise time: "<<fTauRise<<" [ns]"<<endl;


    cout<<"  ---------- "<<endl;



}//Print




bool GLArProc::SetPhase(Phase p) const {

      if(  p == kLiquid || p == kGas ){

        fPhase = p;

    }else{

        fIsApplicable = false;

        return false;
    }

    fIsApplicable = true;

    return true;

}


bool GLArProc::SetElement(Element e, Phase p) const {
/**

 **/

    if( e != kArgon ){

	fIsApplicable = false;
	return false;

    }

    fElement = e;

    if(  p == kLiquid || p == kGas ){

	fPhase = p;

    }else{

	fIsApplicable = false;

	return false;
    }



    if( fElement == kArgon ){

	fMaterialZ = GLArDB::Argon::Z;
	fMaterialA = GLArDB::Argon::A;

	if( fPhase == kLiquid ){

	    fRefDensity = GLArDB::Argon::Liquid::density;
	    fDensity = GLArDB::Argon::Liquid::density;

	}else if (fPhase == kGas ){

	    fRefDensity = GLArDB::Argon::Gas::density;
	    fDensity = GLArDB::Argon::Gas::density;
	}

    }



    fIsApplicable = true;

    return true;


}

void GLArProc::SetDensity(float d) const {
/*
 */
    fDensity = d;

}


bool GLArProc::SetElectricField(float EField) const {
  /*
   *
   */


  fElectricField = EField;

  fIsZeroField = false;

  if(fElectricField == 0.){
    fIsZeroField = true;
  }

}


bool GLArProc::SetEnergy(float deposited, float step, float kinetic) const {
  /*
   *
   *
   */



  fKin = kinetic;
  fdE = deposited;
  fdx = step;

  if(deposited == 0. || step == 0.) {
    fIsApplicable = false;
    return false;
  }

  if( kinetic > fHighEnergyLowerLimit && !fIsNeutron ){
    fIsHighEnergy = true;
  }else fIsHighEnergy = false;


  fIsShortTrack = false;


  return true;

}

void GLArProc::UseLArRecombData(bool b) const {
/**
 **/
    fUseLArRecombData = b;

}


//bool GLArProc::SetParticle(TString particle, int Z, float A ) const {
bool GLArProc::SetParticle(int pdg) const {
  /**
   *
   * //TODO: deal with neutral particles
   *
   */
  float A = 0;
  int Z;

  ClearParticle();

  fIsApplicable = false;
  TString particle = "";

  //Check Particles

  //  if( particle == "opticalphoton" ){
  if(pdg==0){ // includes opticalphoton and other weird particles
    return false;
  }

  /* //LZ - what is it??
  if( particle == "thermalelectron" ){
    return false;
  }
  */

  //  if( particle.Contains("nu_e") || particle.Contains("nu_mu") || particle.Contains("nu_tau") ){
  if(fabs(pdg) == 12 || fabs(pdg) == 14 || fabs(pdg) == 16){
    return false;
  }

  fIsApplicable = true;


  //  if( particle == "e+"  || particle == "e-"){
  if(fabs(pdg) == 11){
    fIsElectron = true;
    particle = (pdg==11) ? "e-" : "e+";      
    A = 0;
    Z = (pdg==11) ? -1 : 1;
  }
  else fIsElectron = false;


  //  if( particle == "mu+"  || particle == "mu-"){
  if(fabs(pdg)==13){
    fIsMuon = true;
    particle = (pdg==13) ? "mu-" : "mu+";
    A = 0;
    Z = (pdg==13) ? -1 : 1;
  }
  else fIsMuon = false;


  //  if(particle == "nucleus"){
  if(pdg > 1000000000 && pdg != 1000020040){
    fIsNucleus = true;
    particle = "nucleus";
    // pdg numbering for nucleus is a 10-digit number : 10LZZZAAAI
    //L = number of strange quarks, should be zero
    //I = excited state
    int tmp = pdg - 1000000000; // only the ZZZAAAI should remains
    if(tmp > 1.E8)
      while(tmp>1E8)
	tmp -= 10000000; // should remove the L
    tmp = floor(tmp/10); // remove the excited state - ZZZAAA remains
    A=tmp%1000;
    tmp -= tmp%1000; //only ZZZ000 remains
    Z = (tmp%1000000)/1000;

  }
  else fIsNucleus = false;



  fPartZ = Z;
  fPartA = A;

  fIsAlpha = false;

  //  if(particle == "alpha" || particle == "Alpha"){
  if(pdg == 1000020040){
    fIsAlpha = true;
    fPartZ = 2;
    fPartA = 4;
    particle = "alpha";
  }

  if( Z == 2 && A == 4){
    fIsAlpha = true;
  }


  if( fIsAlpha || fIsNeutron ) {
    fPartA = fMaterialA; // LZ : Why ??? 
  }



  fIsNuclearRecoil = false;

//  if( particle == "neutron"  || particle == "antineutron"){
  if(fabs(pdg) == 2112){
    fIsNeutron = true;
    fIsNuclearRecoil = true;
    particle = (pdg==2112) ? "neutron" : "antineutron";
    A = 1;
    Z = 0;
  }
  else fIsNeutron = false;


  if( fPartZ == fMaterialZ && fIsNucleus) {
    fIsNuclearRecoil = true;
  }

  fParticleName = particle;

  return true;

}//SetParticle



int GLArProc::Compute() const {
/**


*/

    return ComputeArgon();

}

int GLArProc::ComputeArgon() const {
    /**
     **
     **/


  ClearResults();


  if( !fIsApplicable ) {

    return 0;

  }


//
//
//


  fModelLindhard = false;

  if( fIsNuclearRecoil || fIsNeutron ) {

    fModelLindhard = true;
  }



  if(fPhase == kLiquid){


    fModelDokeBirks = true;
    fModelThomasImel = false;

    if( (fIsShortTrack && !fIsHighEnergy) || fIsAlpha || fIsNuclearRecoil) {
      fModelThomasImel = true;
    }
    
  }
  

  if(fPhase == kGas){

    fIsShortTrack = false; //forced

    fModelDokeBirks = true;
    fModelThomasImel = false;

    if( fIsAlpha ) {
      fModelThomasImel = true;
    }

  }

//
//
//

  fYieldFactor = 1;


  fThomasImel=0;
  fDokeBirks_A=0, fDokeBirks_B=0, fDokeBirks_C=0;

  if( fModelLindhard ) {

      if(fPhase == kLiquid){
	  fYieldFactor = GLArDB::Argon::Liquid::Lindhard::YieldFactor(fdE/keV);
      }


      fExcitationRatio = GLArDB::Argon::Liquid::ExcitationRatio::NuclearRecoil::Ratio( fElectricField/(volt/cm) );

  } else {

      fExcitationRatio = GLArDB::Argon::Liquid::ExcitationRatio::Ratio();

  }



  if( fModelDokeBirks ) {

      fDokeBirks_A = GLArDB::Argon::Liquid::DokeBirks::A(fElectricField/(kilovolt/cm));
      fDokeBirks_B = GLArDB::Argon::Liquid::DokeBirks::B(fElectricField/(kilovolt/cm));
      fDokeBirks_C = GLArDB::Argon::Liquid::DokeBirks::C(fElectricField/(kilovolt/cm));

      //cout<<" fModelDokeBirks "<<fElectricField<<" "<<fElectricField/(kilovolt/cm)<<endl; 
      //cout<<" fModelDokeBirks Pars: "<<fDokeBirks_A<<" "<<fDokeBirks_B<<" "<<fDokeBirks_C<<endl; 
  }


  if( fModelThomasImel ){

      fThomasImel = GLArDB::Argon::Liquid::ThomasImel::Factor(fElectricField/(volt/cm));

      if( fModelLindhard && fPhase == kLiquid ){
	  fThomasImel = GLArDB::Argon::Liquid::ThomasImel::NuclearRecoil::Factor(fElectricField/(volt/cm));
      }
   }


  //
  //
  //

  if( fYieldFactor > 1) fYieldFactor = 1;
  if( fYieldFactor < 0) fYieldFactor = 0;

  //
  //
  //

  if(GLArDB::Argon::Wg < 0 ) {

      return 0;
  }

  fMeanNumOfQuanta = fdE/GLArDB::Argon::Wg;

  float ExcitationRatioResolutionScale = GLArDB::Argon::Liquid::ExcitationRatio::ResolutionScale;

  float sigma = sqrt(ExcitationRatioResolutionScale*fMeanNumOfQuanta);


  fNumOfQuanta = int( floor( gRandom->Gaus(fMeanNumOfQuanta,sigma) + 0.5 ) );

  if( fIsNuclearRecoil || fIsNeutron) {

    float Lvar = gRandom->Gaus( fYieldFactor, 0.25*fYieldFactor );
    if(Lvar >1) Lvar = 1;
    if(Lvar <0) Lvar = 0;

    fNumOfQuanta = BinomFluct(fNumOfQuanta, Lvar);
  }

  if( fdE < GLArDB::Argon::Wg || fNumOfQuanta <0 ){
      fNumOfQuanta = 0;
  }


  float excitProb = fExcitationRatio/(1+fExcitationRatio);

  fMeanNumOfExcit = fMeanNumOfQuanta*excitProb;
  fMeanNumOfIon = fMeanNumOfQuanta - fMeanNumOfExcit;

  fNumOfExcit = BinomFluct(fNumOfQuanta, excitProb);

  fNumOfIon = fNumOfQuanta - fNumOfExcit;

/*
  cout<<"compute "<<endl;
  cout<<"compute "<<fMeanNumOfQuanta<<endl;
  cout<<"compute: "<<fMeanNumOfExcit<<" "<<fMeanNumOfIon<<" sum: "<<fMeanNumOfExcit+fMeanNumOfIon<<endl;
  cout<<"compute "<<fNumOfQuanta<<endl;
  cout<<"compute: "<<fNumOfExcit<<" "<<fNumOfIon<<" sum: "<<fNumOfExcit+fNumOfIon<<endl;
*/


  //
  //  Recombination
  //

  float recombProbDB = 0;

  // MeV/cm
  float dEdx = fdE/fdx/(MeV/cm);

  float rho = fDensity/(g/cm3);

  // MeV*cm2/g
  float LET = dEdx/rho;

  //
  float electronLET = GLArDB::Argon::ElectronLET::LET(fdE/keV);


  float letRatio = electronLET/LET;

  if( fParticleName == "e-" && letRatio < 0.7 ){
      LET = electronLET;
  }

  // B=A/(1-C)
  fDokeBirks_B = fDokeBirks_A/(1-fDokeBirks_C);

  recombProbDB = fDokeBirks_A*LET/(1+fDokeBirks_B*LET) + fDokeBirks_C;

  if(fPhase == kLiquid){
      recombProbDB *= fDensity/fRefDensity;
  }


  fLET = LET;

  //
  //
  //
  if(  !fIsZeroField && fPhase == kLiquid && fModelDokeBirks && fUseLArRecombData  ) {


      float Wi = GLArDB::Argon::Wi;

      
    //
    //if(Wi>0) {
    // fNumOfIon = fdE/Wi;
    // fNumOfExcit = fNumOfQuanta  - fNumOfIon;
    //}

      float Recomb_A = GLArDB::Argon::Liquid::RecombData::A;

      float Recomb_K = GLArDB::Argon::Liquid::RecombData::K;


      //cout<<" **** fUseRecombData **** A: "<<Recomb_A<<" K: "<<Recomb_K<<" rho: "<<rho<<endl;

      Recomb_K /= ((kilovolt/cm)*(g/cm2)/MeV);

      float R = Recomb_A/( 1 + Recomb_K*LET/(fElectricField/(kilovolt/cm)) );

      recombProbDB = 1 - R;

      //cout<<" **** fUseRecombData **** A2: "<<Recomb_A<<" K2: "<<Recomb_K<<"; R: "<<R<<" recomb: "<<recombProbDB<<endl;

  }

  //
  // --
  //


  float recombProbTI = 0;

  float Xi = (fNumOfIon/4.)*fThomasImel;

  if(Xi>0) {
      recombProbTI = 1 - log(1+Xi)/Xi;
  }

  if(fPhase == kLiquid){
      recombProbTI *= pow(fDensity/fRefDensity,0.3);
  }

  //

  //
  //
  //


  //


  fRecombProb = 0;

  if( fModelDokeBirks ) {

      fRecombProb = recombProbDB;

  }else if( fModelThomasImel ) {

      fRecombProb = recombProbTI;

  }else{

      ClearYields();
      return 0;
  }


  if(fRecombProb >1) fRecombProb =1;
  if(fRecombProb <0) fRecombProb =0;


  //
  //
  //


  fMeanNumOfEle = fMeanNumOfIon*(1-fRecombProb);
  fMeanNumOfPhot = fMeanNumOfQuanta - fMeanNumOfEle;


  int fNumOfRecombIon = BinomFluct(fNumOfIon, fRecombProb);



  fNumOfPhot = fNumOfExcit + fNumOfRecombIon;

  fNumOfEle = fNumOfIon - fNumOfRecombIon;

/*
    cout<<"compute q: "<<fNumOfQuanta<<" ex: "<<fNumOfExcit<<" ion: "<<fNumOfIon<<" sum: "<<fNumOfExcit+fNumOfIon<<endl;
    cout<<"compute g: "<<fNumOfPhot<<" ex: "<<fNumOfEle<<" e: "<<"sum: "<<fNumOfPhot+fNumOfEle<<endl;
    cout<<"compute rec: "<<fNumOfIon<<" "<<fNumOfRecombIon<<endl;
*/

  double OpPhEneMean = GLArDB::Argon::Liquid::Scintillation::OpticalPhoton::EneMean;
  double OpPhEneWidth = GLArDB::Argon::Liquid::Scintillation::OpticalPhoton::EneWidth;


  //double sampledPhEne = NoElProcRnd::Gaus(OpPhEneMean,OpPhEneWidth);


  //
  //  Triplet Singlet
  //

  fSingletToTripletX = 0;
  fSingletToTripletR = 0;

  if( fIsNuclearRecoil ){

    fSingletToTripletX = GLArDB::Argon::Liquid::Scintillation::NuclearRecoil::SingletToTriplet::RatioExcit(fdE/keV);
    fSingletToTripletR = GLArDB::Argon::Liquid::Scintillation::NuclearRecoil::SingletToTriplet::RatioRecomb(fdE/keV);

  }else if( fIsAlpha ){


    fSingletToTripletX = GLArDB::Argon::Liquid::Scintillation::Alpha::SingletToTriplet::RatioExcit(fdE/MeV);
    fSingletToTripletR = GLArDB::Argon::Liquid::Scintillation::Alpha::SingletToTriplet::RatioRecomb(fdE/MeV);

  }else{

    fSingletToTripletX = GLArDB::Argon::Liquid::Scintillation::ElectronRecoil::SingletToTriplet::RatioExcit(fLET);
    fSingletToTripletR = GLArDB::Argon::Liquid::Scintillation::ElectronRecoil::SingletToTriplet::RatioRecomb(fLET);

  }


 if( fPhase == kLiquid ) {

  fTauSinglet = GLArDB::Argon::Liquid::Scintillation::tauSingl;
  fTauTriplet = GLArDB::Argon::Liquid::Scintillation::tauTripl;
  fTauRecomb = GLArDB::Argon::Liquid::Scintillation::tauRecomb;
  fTauRise = GLArDB::Argon::Liquid::Scintillation::tauRise;

   fOPMeanEne =GLArDB::Argon::Liquid::Scintillation::OpticalPhoton::EneMean;
   fOPEneWidth = GLArDB::Argon::Liquid::Scintillation::OpticalPhoton::EneWidth;

 }

 if( fPhase == kGas ) {

  fTauSinglet = GLArDB::Argon::Gas::Scintillation::tauSingl;
  fTauTriplet = GLArDB::Argon::Gas::Scintillation::tauTripl;
  fTauRecomb = GLArDB::Argon::Gas::Scintillation::tauRecomb;
  fTauRise = GLArDB::Argon::Gas::Scintillation::tauRise;

  fOPMeanEne = GLArDB::Argon::Gas::Scintillation::OpticalPhoton::EneMean;
  fOPEneWidth = GLArDB::Argon::Gas::Scintillation::OpticalPhoton::EneWidth;

 }





  //
  //
  //

  return 1;

}//Compute Argon



void GLArProc::GenerateScintTime(float &time, bool& isSinglet, bool &isExcit) {



    float fracExcit = fNumOfExcit*1./(  fNumOfPhot );

    isExcit = gRandom->Uniform() < fracExcit;

    float ratioX = fSingletToTripletX / (1+fSingletToTripletX );
    float ratioR = fSingletToTripletR / (1+fSingletToTripletR );

    if( isExcit ) isSinglet = gRandom->Uniform() < ratioX;
    else isSinglet = gRandom->Uniform() < ratioR;

    float tau1 = fTauRise; //fTauRecomb;

    float tau2;

    if(isSinglet) tau2 = fTauSinglet;
    else tau2 = fTauTriplet;

    time = SampleScintTime(tau1,tau2);


}

float GLArProc::SampleScintTime(float tau1, float tau2) const {
/**

*/

     while(1){
    //2 random numbers
       double ran1 = gRandom->Uniform();
       double ran2 = gRandom->Uniform();

    //
    //exponential distribution as envelope function: more efficient
    //
      double d = (tau1+tau2)/tau2;
    //make sure the envelope function is
    //always larger than the bi-exponential
    double t = -1.0*tau2*std::log(1-ran1);
    double g = d*Scint_single_exp(t,tau2);
    if(ran2 <= Scint_bi_exp(t,tau1,tau2)/g){
        return t;
    }

  }

  return -1.0;


}


float GLArProc::Scint_bi_exp(float t, float tau1, float tau2) const{
  /**

  */

  return std::exp(-1.0*t/tau2)*(1-std::exp(-1.0*t/tau1))/tau2/tau2*(tau1+tau2);

}

float GLArProc::Scint_single_exp(float t, float tau) const{
  /**

  */

  return std::exp(-1.0*t/tau)/tau;


}


void GLArProc::SetHighEnergyLowerLimit(float E) const {
  /**
   *
   */

  fHighEnergyLowerLimit = E;

}


void GLArProc::Init() const {
  /**
   *
   */

  Clear();

  SetElement(kArgon,kLiquid);

  SetHighEnergyLowerLimit(5*MeV);

}


void GLArProc::Clear() const {


  SetEnergy(0.,0.,0.);
  SetElectricField(0.);


  fIsApplicable = false;

  ClearSettings();

  ClearResults();

  ClearParticle();

}


void GLArProc::ClearResults() const {

  fModelDokeBirks = false;
  fModelThomasImel = false;
  fModelLindhard = false;

  fRecombProb = 0;
  fYieldFactor = 0;
  fExcitationRatio = 0;
  fLET = 0;
  fElectronLET = 0;

  fDokeBirks_A = 0;
  fDokeBirks_B = 0;
  fDokeBirks_C = 0;

  fThomasImel = 0;

  fSingletToTripletX = 0;
  fSingletToTripletR = 0;

  fTauSinglet = 0;
  fTauTriplet = 0;
  fTauRecomb = 0;
  fTauRise = 0;


  fOPMeanEne = 0;
  fOPEneWidth = 0;

  ClearYields();

}

void GLArProc::ClearSettings() const {
/*
 */

  UseLArRecombData(false);

}

void GLArProc::ClearYields() const {

  fMeanNumOfQuanta = 0;
  fMeanNumOfEle = 0;
  fMeanNumOfPhot = 0;
  fMeanNumOfExcit = 0;
  fMeanNumOfIon = 0;

  fNumOfQuanta = 0;
  fNumOfEle = 0;
  fNumOfPhot = 0;
  fNumOfExcit = 0;
  fNumOfIon = 0;
}


void GLArProc::ClearParticle() const {

 fParticleName="";

 fIsApplicable = false;
 fIsElectron = false;
 fIsMuon = false;
 fIsNeutron = false;
 fIsAlpha = false;
 fIsNucleus = false;

 fIsHighEnergy = false;
 fIsNuclearRecoil = false;
 fIsExcitedNucleaus = false;
 fIsShortTrack = false;

}

int GLArProc::BinomFluct(int N0, double prob) const {

  double mean = N0*prob;

  double sigma = sqrt(N0*prob*(1-prob));

  int N1 = 0;

  if(prob <= 0.) return N1;
  if(prob >= 1.) return N0;

  if( N0<10 ){
    for(int i=0; i<N0; ++i){
      if(gRandom->Uniform(0,1)<prob) N1++;
    }
  }else{
    N1 = int( floor( gRandom->Gaus(mean,sigma)+0.5) );
  }

  if(N1>N0) N1 = N0;
  else if(N1<0) N1 = 0;

  return N1;
}

