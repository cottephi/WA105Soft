/**
 * \file VMCPrimaryGenerator.cc
 * \author Devis Lussi
 * \date 28-Jul-2011
 */

/* fullreco -- Full reconstruction software
 * (C) 2000 - 2011,  The fullreco Team
 *
 * fullreco is mainly intended for academic purposes. Proper
 * credit must be given if you use fullreco or parts of it. Please
 * read the section 'Credit' in the README file.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#include <math.h>
#include "TDatabasePDG.h"
#include "TFile.h"

#include "VMCPrimaryGenerator.h"
#include "Preferences.h"

//#include "GENIE.h"
#include "kinutil.h"

//ClassImp(VMCPrimaryGenerator)

using namespace std;

//_____________________________________________________________________________
VMCPrimaryGenerator::VMCPrimaryGenerator(TVirtualMCStack *stack) 
{

  fStack = stack;
  

  vmc_seed = myPrefs->iPrimarySeed;   // default seed value --> change via SetVMCSeed
  vmc_rndm = new TRandom3(vmc_seed);  // random number generator
  
  //cout<<" ------------> gRandom  "<< gRandom->GetSeed() << " " << gRandom << endl;
  //cout<<" ------------> vmc_rndm "<< vmc_rndm->GetSeed() << " " << vmc_rndm << endl;

  //default values (pencil 1 GeV/c electron beam from the origin in x direction)
  fType       = kSector;
  fNPrimaries = 1;
  fPdgCode    = 11;
  fPMin=fPMax = 1.0;
  fPSigma     = 0;
  fThetaMin  = fThetaMax  = TMath::Pi()/2;
  fVThetaMin = fVThetaMax = TMath::Pi()/2;
  fRadiusMin = fRadiusMax = 0.;
  fPhiMin = fPhiMax = 0.;  
  fVxMin = fVxMax = 0;
  fVyMin = fVyMax = 0;
  fVzMin = fVzMax = 0;
  
  inputGeniePtr = NULL;

  // WA105 cosmic background
  isWa105BeamOn  = false;
  isWa105CRBkgOn = false;
  wa105CRYflux   = new WA105CRYFlux();
  fWA105CRT0     = 0.0;
  fWA105CRTWin   = 0.0;

  wa105BHflux = new bkgsrc::WA105HaloFlux();
  isWa105BHBkgOn = false;
  fWA105BHT0     = 0;
  fWA105BHTWin   = 0;
}

//_____________________________________________________________________________
VMCPrimaryGenerator::~VMCPrimaryGenerator() 
{
  //
  // Destructor  
  //

  
  vmc_rndm->Delete();
  
  delete wa105CRYflux;
}
//_____________________________________________________________________________


//_____________________________________________________________________________
void VMCPrimaryGenerator::GeneratePrimary_Sector()
{
  TParticlePDG *aParticlePDG=NULL;
  if (!(aParticlePDG = TDatabasePDG::Instance()->GetParticle(fPdgCode))) {
    cerr << "VMCPrimaryGenerator::GeneratePrimary() ERROR: Particle does not exit in pdg database! Exit." << endl;
    return;
  }
  
  // Track ID (filled by stack)
  Int_t ntr;
  
  // Option: to be tracked
  Int_t toBeDone = 1; 
 
  // PDG
  pdgPri = fPdgCode;
  
  // Polarization
  Double_t polx = 0.; 
  Double_t poly = 0.; 
  Double_t polz = 0.; 

  // Position
  vxPri = vmc_rndm->Uniform(fVxMin,fVxMax);
  vyPri = vmc_rndm->Uniform(fVyMin,fVyMax);
  vzPri = vmc_rndm->Uniform(fVzMin,fVzMax);
  tof = 0.;

  // Particle momentum
  Double_t cosTheta = cos(fThetaMin);
  phiPri = fPhiMin;
  
  Double_t pAbs = fPMin;
  if (fThetaMin!=fThetaMax) 
    cosTheta = vmc_rndm->Uniform(cos(fThetaMax),cos(fThetaMin));
  if (fPhiMin != fPhiMax) 
    phiPri = vmc_rndm->Uniform(fPhiMin,fPhiMax);
  if (fPMin!=fPMax) 
    pAbs = vmc_rndm->Uniform(fPMin,fPMax);
  
  thetaPri = acos(cosTheta);

  pPri  = pAbs;
  pxPri = pAbs * cos(phiPri) * sqrt(1-cosTheta*cosTheta);
  pyPri = pAbs * sin(phiPri) * sqrt(1-cosTheta*cosTheta);
  pzPri = pAbs * cosTheta;
  
  // Energy (in GeV)
  Double_t mass = aParticlePDG->Mass();
  Double_t e    = sqrt( pAbs*pAbs + mass*mass );
  
  // Add particle to stack 
  fStack->PushTrack(toBeDone, -1, pdgPri, pxPri, pyPri, pzPri, e, 
		    vxPri, vyPri, vzPri, tof, polx, poly, polz, 
		    kPPrimary, ntr, 1., 0);

}//_____________________________________________________________________________
void VMCPrimaryGenerator::GenerateGENIEPrimary()
{
  //
  // Fill the user stack (derived from TVirtualMCStack) with GENIE primary particles.
  //

  if(!inputGeniePtr)
    {
      cerr<<"ERROR: Input to RawDataGenie has to be defined "<<endl;
      return;
    }


  inputGeniePtr->fill_PO_table(myPrefs->reinteraction);
  inputGeniePtr->DumpGENIEEvent();
  int mNPO = inputGeniePtr->getNPO();
  microPO* mPO = inputGeniePtr->getPO();

 // Neutrino momentum direction
  Double_t cosTheta = cos(fThetaMin);
  phiPri = fPhiMin;
  
  if (fThetaMin!=fThetaMax) 
    cosTheta = vmc_rndm->Uniform(cos(fThetaMax),cos(fThetaMin));
  if (fPhiMin!=fPhiMax) 
    phiPri = vmc_rndm->Uniform(fPhiMin,fPhiMax);

  thetaPri = acos(cosTheta);

  // primary neutrino information
  pdgPri = inputGeniePtr->GENIEv.brNeutrino; // nu pdg code
  pPri  = inputGeniePtr->GENIEv.brEv;        // nu energy
  pxPri = pPri * cos(phiPri) * sqrt(1 - cosTheta*cosTheta);
  pyPri = pPri * sin(phiPri) * sqrt(1 - cosTheta*cosTheta);
  pzPri = pPri * cosTheta;

  // Track ID (filled by stack)
  Int_t ntr;
  
  // Option: to be tracked
  Int_t toBeDone = 1; 
  
  // Polarization
  Double_t polx = 0.; 
  Double_t poly = 0.; 
  Double_t polz = 0.; 
  
  tof = 0.;
/*  
  // vertex position
  Double_t vr = vmc_rndm->Uniform(fRadiusMin*fRadiusMin,fRadiusMax*fRadiusMax);
  vr = sqrt(vr);
  Double_t vTheta = vmc_rndm->Uniform(fVThetaMin,fVThetaMax);
  
  vxPri = vr * cos(vTheta);
  vyPri = vr * sin(vTheta);
  vzPri = vmc_rndm->Uniform(fVzMin, fVzMax); */
 
  
  vxPri  = vmc_rndm->Uniform(fVxMin,fVxMax);
  vyPri  = vmc_rndm->Uniform(fVyMin,fVyMax);
  vzPri  = vmc_rndm->Uniform(fVzMin,fVzMax);
   

  for(int i=0;i<mNPO;i++) 
    {
      Int_t pdg     = mPO[i].iCode;
      Double_t px_g = mPO[i].p[0];
      Double_t py_g = mPO[i].p[1];
      Double_t pz_g = mPO[i].p[2];
      Double_t e    = mPO[i].Emeasured;
      
      TVector3 p(px_g, py_g, pz_g);
      
      //rotation (GENIE momenta is along the x axis)
      p.RotateY(thetaPri);
      p.RotateZ(phiPri);
      cout << "1: px, py, pz = " << p.X() << " " << p.Y() << " " << p.Z() << endl;
      cout << "1: vx, vy, vz = " << vxPri/100 << " " << vyPri/100 << " " << vzPri/100 << endl;
      
      //toBeDone = (mPO[i].UseIt == 1) ? 1 : 0;
      toBeDone = 1;
      
      fStack->PushTrack( toBeDone, -1, pdg, p.x(), p.y(), p.z(), e, 
			 vxPri, vyPri, vzPri, tof, 
			 polx, poly, polz, kPPrimary, ntr, 1., 0 );
      
      cout<<"iPrimary "<<ntr<<" -> toBeDone,pdg,e: "<<toBeDone<<" "<<pdg<<" "<<e<<" "<<endl;       
    }
}
//_____________________________________________________________________________
void VMCPrimaryGenerator::GenerateGENIE_ND_Primary()
{
  //
  // Fill the user stack (derived from TVirtualMCStack) with GENIE primary particles.
  //

  if(!inputGeniePtr)
    {
      cerr<<"ERROR: Input to RawDataGenie has to be defined "<<endl;
      return;
    }
  
  inputGeniePtr->fill_PO_ND_table(myPrefs->reinteraction);
  inputGeniePtr->DumpGENIE_ND_Event();
  
  int mNPO = inputGeniePtr->getNPO();
  microPO* mPO = inputGeniePtr->getPO();

  //inputGeniePtr->DumpGENIE_ND_Event();

  // Track ID (filled by stack)
  Int_t ntr;
  
  // Option: to be tracked
  Int_t toBeDone = 1; 
  
  // Geantino
  //Int_t pdg  = 0;
  
  // Polarization
  Double_t polx = 0.; 
  Double_t poly = 0.; 
  Double_t polz = 0.; 
  
  // vertex position
  tof = 0.;
  vxPri  = vmc_rndm->Uniform(fVxMin,fVxMax);
  vyPri  = vmc_rndm->Uniform(fVyMin,fVyMax);
  vzPri  = vmc_rndm->Uniform(fVzMin,fVzMax);
  
   
 
  //theta phi
  Double_t cosTheta = cos(fThetaMin);
  phiPri = fPhiMin;
  
  if (fThetaMin!=fThetaMax) 
    cosTheta = vmc_rndm->Uniform(cos(fThetaMax),cos(fThetaMin));
  if (fPhiMin!=fPhiMax) 
    phiPri = vmc_rndm->Uniform(fPhiMin,fPhiMax);
  
  thetaPri = acos(cosTheta);

  for(int i=0;i<mNPO;i++) 
    {
      Int_t pdg     = mPO[i].iCode;
      Double_t px_g = mPO[i].p[0];
      Double_t py_g = mPO[i].p[1];
      Double_t pz_g = mPO[i].p[2];
      Double_t e    = mPO[i].Emeasured;
      
      TVector3 p(px_g, py_g, pz_g);

      //rotation (GENIE momenta is along the x axis)
      p.RotateY(thetaPri);
      p.RotateZ(phiPri);
      
      cout << "1: px, py, pz = " << p.X() << " " << p.Y() << " " << p.Z() << endl;
      cout << "1: vx, vy, vz = " << vxPri/100 << " " << vyPri/100 << " " << vzPri/100 << endl;
      
      //toBeDone = (mPO[i].UseIt == 1) ? 1 : 0;
      toBeDone = 1;
      fStack->PushTrack( toBeDone, -1, pdg, p.x(), p.y(), p.z(), e, 
			 vxPri, vyPri, vzPri, tof, 
			 polx, poly, polz, kPPrimary, ntr, 1., 0 );
    }   
  
}

 
//_____________________________________________________________________________
void VMCPrimaryGenerator::ConfigWA105CRBackground( TString fname, float t0, 
						   float twin, float ratenorm )
{
  fWA105CRT0   = t0/1000.0;   //in s
  fWA105CRTWin = twin/1000.0; //in s
  wa105CRYflux->OpenFile( fname.Data() );
  wa105CRYflux->SetNormRate( ratenorm );
  
  //wa105CRYflux->SetSeed( seed );
}

//_____________________________________________________________________________
void VMCPrimaryGenerator::ConfigWA105BHBackground(  float t0, float twin, float rate,
						    double x0, double thx0, double sgmx, 
						    double epsx, double alphx, 
						    double y0, double thy0, double sgmy, 
						    double epsy, double alphy )
{
  fWA105BHT0   = t0/1000.0;   //in s
  fWA105BHTWin = twin/1000.0; //in s
  wa105BHflux->SetRate( rate );
  wa105BHflux->SetProfile( x0, thx0, sgmx, epsx, alphx, y0, thy0, sgmy, epsy, alphy );
}

//_____________________________________________________________________________
void VMCPrimaryGenerator::GenerateWA105BkgPrimary(particle_fld &cryvec)
{
  TParticlePDG *aParticlePDG = NULL;
  if (!(aParticlePDG = TDatabasePDG::Instance()->GetParticle(cryvec.pdg))) 
    {
      cerr << "GenerateWA105CryPrimary() ERROR: Particle does not exit in pdg database! Exit." << endl;
      return;
    }
  
  // Track ID (filled by stack)
  Int_t ntr;
  
  // Option: to be tracked
  Int_t toBeDone = 1; 
 
  // PDG
  Int_t pdg = cryvec.pdg;
  
  // Polarization
  Double_t polx = 0.; 
  Double_t poly = 0.; 
  Double_t polz = 0.; 

  // need to implement some geometrical cross-checks 
  double vx_cry  = cryvec.x * 100.0; //cm
  double vy_cry  = cryvec.y * 100.0; //cm
  double vz_cry  = cryvec.z * 100.0; //cm
  double tof_cry = cryvec.time;      //s
  
  Double_t ekin = cryvec.eneKin / 1000.0; //GeV
  Double_t mass = aParticlePDG->Mass();
  Double_t e    = ekin + mass;
  Double_t pAbs = sqrt(e*e - mass*mass);

  double px_cry = pAbs * cryvec.dirx;
  double py_cry = pAbs * cryvec.diry;
  double pz_cry = pAbs * cryvec.dirz;
  
  // cout<<"Cosmic ray primary : "<<pdg<<" "<<ekin<<" "<<mass<<" "<<vx_cry
  //     <<" "<<vy_cry<<" "<<vz_cry<<endl;
  // cout<<"                     "<<px_cry<<" "<<py_cry<<" "<<pz_cry
  //     <<" "<<tof_cry<<endl;

  // Add particle to stack 
  fStack->PushTrack( toBeDone, -1, pdg, px_cry, py_cry, pz_cry, e, 
		     vx_cry, vy_cry, vz_cry, tof_cry, polx, poly, polz, 
		     kPPrimary, ntr, 1., 0 );

}

//_____________________________________________________________________________
void VMCPrimaryGenerator::GenerateWA105EventPrimary()
{
  //if beam event is required generate beam event
  if(isWa105BeamOn)
    GeneratePrimary_Sector();
  
  //if cr backround is required generate events
  if(isWa105CRBkgOn)
    {
      if(myPrefs->iWa105SingleCREvent>=0) // get a single event from file
	{
	  particle_fld crevent;
	  if(wa105CRYflux->GetEntry(myPrefs->iWa105SingleCREvent, crevent))
	    {
	      crevent.time = fWA105CRT0;
	      GenerateWA105BkgPrimary(crevent);
	    }
	}
      else //generate a vector of events
	{
	  vector<particle_fld> crflux;
	  wa105CRYflux->GetCosmics(fWA105CRT0, fWA105CRTWin, crflux);
	  for(size_t i=0;i<crflux.size();i++)
	    GenerateWA105BkgPrimary(crflux[i]);
	}
    }
  
  if( isWa105BHBkgOn ) 
    {
      vector<particle_fld> crflux;
      wa105BHflux->GetHalo( fWA105BHT0, fWA105BHTWin, crflux );
      for(size_t i=0;i<crflux.size();i++)
	GenerateWA105BkgPrimary(crflux[i]); 
    }
}

//_____________________________________________________________________________
void VMCPrimaryGenerator::GeneratePrimaries()
{
  switch (fType) 
    {
    case kGENIE:
      for (Int_t ii=0; ii<fNPrimaries; ii++) GenerateGENIEPrimary();
      return;
      
    case kGENIE_nd:
      for (Int_t ii=0; ii<fNPrimaries; ii++) GenerateGENIE_ND_Primary();
      return;
      
    case kWA105:
      GenerateWA105EventPrimary();
      return;
      
    default:
      for (Int_t ii=0; ii<fNPrimaries; ii++) GeneratePrimary_Sector();
      return;
    }
}
