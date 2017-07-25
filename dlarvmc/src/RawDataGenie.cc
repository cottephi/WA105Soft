#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath> 
#include <string>

#include <TFile.h>
#include <TTree.h> 
#include <TROOT.h>
#include <TRandom3.h>

#include "dstconst.h"
#include "particle.h"
#include "kinutil.h"

#include "RawDataGenie.h"

using namespace std;

RawDataGenie::RawDataGenie()
{
  cout <<"RawDataGenie "<<endl;

  rootfile = NULL;
  s_tree   = NULL;
}

/*--------------------------------------------------------------------------------------------*/
RawDataGenie::~RawDataGenie()
{
  if(rootfile) rootfile->Close();
}

/*--------------------------------------------------------------------------------------------*/
void RawDataGenie::fill_PO_table(int reinter)
{
 int i,code,igeant;
#define nR  2
  int na=nR;
  float a[nR];

  mNPO=0;
  VUZERO4(Ptot_mc);
  VUZERO4(Phad_mc);

   cout <<"RawDataGenie::fill_PO_table(int reinter)  "<<endl; 

  mPO[mNPO].iCode=GENIEv.brFSPrimLept;
  if(abs(GENIEv.brFSPrimLept)==12 || abs(GENIEv.brFSPrimLept)==14 || abs(GENIEv.brFSPrimLept)==16){
    mPO[mNPO].UseIt = -1;
  } else {
    mPO[mNPO].UseIt = 1;
  }
  mPO[mNPO].p[0]=GENIEv.brPxl;
  mPO[mNPO].p[1]=GENIEv.brPyl;
  mPO[mNPO].p[2]=GENIEv.brPzl;
  mPO[mNPO].p[3]=VUMOD3(mPO[mNPO].p);
  mPO[mNPO].Emeasured = GENIEv.brEl;
  mNPO++;
  cout << "Reinteraction = " << reinter << endl;

  // insert hadrons
  if(reinter)
    for(i=0;i<GENIEv.brNf;i++)
      {
        code  = GENIEv.brPdgf[i];
        /* skip neutrinos */
        mPO[mNPO].iCode=code;
        mPO[mNPO].p[0]=GENIEv.brPxf[i];
        mPO[mNPO].p[1]=GENIEv.brPyf[i];
        mPO[mNPO].p[2]=GENIEv.brPzf[i];
        mPO[mNPO].p[3]=VUMOD3(mPO[mNPO].p);
        mPO[mNPO].Emeasured = GENIEv.brEf[i];
        mPO[mNPO].UseIt= 1;
        mNPO++;
      }
  else
    for(i=0;i<GENIEv.brNi;i++)
      {
        code  = GENIEv.brPdgi[i];
        /* skip neutrinos */
        mPO[mNPO].iCode=code;
        mPO[mNPO].p[0]=GENIEv.brPxi[i];
        mPO[mNPO].p[1]=GENIEv.brPyi[i];
        mPO[mNPO].p[2]=GENIEv.brPzi[i];
        mPO[mNPO].p[3]=VUMOD3(mPO[mNPO].p);
        mPO[mNPO].Emeasured = GENIEv.brEi[i];
        mPO[mNPO].UseIt = 1;
        mNPO++;
      }

  for(int iPO=0;iPO<mNPO;iPO++)
    {
      code = mPO[iPO].iCode;
      mPO[iPO].MassMC = SQR(mPO[iPO].Emeasured)-SQR(mPO[iPO].p[3]);
      if(mPO[iPO].MassMC<0)mPO[iPO].MassMC=0;
      mPO[iPO].MassMC = sqrt( mPO[iPO].MassMC);
      mPO[iPO].Tmc = mPO[iPO].Emeasured-mPO[iPO].MassMC;
      igeant=ToGeantCode(code);
      mPO[iPO].GeantId = igeant;
      mPO[iPO].HasInteracted = -1;
      mPO[iPO].angular_res = 0;

      // neutrinos
      if((abs(code)==12)||(abs(code)==14)||(abs(code)==16))
        mPO[iPO].UseIt = -1;

      if(ParticleCharge((_ParticleType)igeant)!=0)
        {
          mPO[iPO].POtype = POCharged;
#ifdef _USE_TPYTHIA6_
          mPO[iPO].qtype = aPythia->Pychge(code);
#endif
          if(abs(code)==JETSET_ELECTRON)
            {
              mPO[iPO].POtype = POElectron;
              //              mPO[iPO].HasInteracted = electron_shower(mPO[iPO].p);
            }         
          else if(abs(code)==JETSET_MUON)
            {
              mPO[iPO].POtype = POMuon;
              /* Decaying muon */
              //              mPO[iPO].HasInteracted = muon_decays(mPO[iPO].p); 
              /* Stopping muon */
              //              if(!mPO[iPO].HasInteracted) 
              //                mPO[iPO].HasInteracted = muon_stops(mPO[iPO].p);
            }
          else
            {
              //              mPO[iPO].HasInteracted = hadron_interact(mPO[iPO].p);
              /***                if(!mPO[iPO].HasInteracted)
                                  printf("Hadron has escaped ? \n");***/
            }
        }
      else
        {
          mPO[iPO].POtype = PONeutral;
          mPO[iPO].qtype = 0;
        }
      
      gRandom->RndmArray(na,a);

      /*

      std::cout << "*****************************->>>>>> Neutron simulation: a[0] = " << a[0] << "  a[1] = " << a[1] << "  n_neutron = " << n_neutron << std::endl;

      int id=igeant;
      if(id == GEANT_MUPLUS || id == GEANT_MUMINUS){
        if(mPO[iPO].p[3]<0.020) 
          mPO[iPO].UseIt = -2;
      }
      else if(id == GEANT_PIPLUS || id ==GEANT_PIMINUS){
        if(mPO[iPO].p[3]<0.020) 
          mPO[iPO].UseIt = -2;
      }
      else if(id == GEANT_PIZERO){
        if(mPO[iPO].p[3]<0.020) 
          mPO[iPO].UseIt = -2;
      }
      else if(id == GEANT_PROTON){
        double tpro = SQR(mPO[iPO].p[3])/(2.*PROTONMASS);  // proton kinetic energy 
        if(tpro<0.050)
          mPO[iPO].UseIt = -2;
      }
      else if(id ==GEANT_KPLUS || id ==GEANT_KMINUS){
        if(mPO[iPO].p[3]<0.030)
          mPO[iPO].UseIt = -2;
      }
      else if(id == GEANT_K0L){
        if(mPO[iPO].p[3]<0.030) 
          mPO[iPO].UseIt = -2;
      }
      else if(id == GEANT_K0S){
        if(mPO[iPO].p[3]<0.030) 
          mPO[iPO].UseIt = -2;
      }
      else if((id ==GEANT_POSITRON || id==GEANT_ELECTRON)) {
      }
      else if(id==GEANT_PHOTON){
	if(mPO[iPO].p[3] < 0.010)
	  mPO[iPO].UseIt = -2;
      }
      else if(abs(code)==JETSET_NEUTRON && a[0]>n_neutron) 
	mPO[iPO].UseIt = -4; 
      else if(abs(code)==JETSET_K0L && a[1]>n_K0l) 
	mPO[iPO].UseIt = -5; 
 
      */


     
      if(mPO[iPO].UseIt>0)
	{
	  // Compute the total generated momentum
	  Ptot_mc[0] += mPO[iPO].p[0];
	  Ptot_mc[1] += mPO[iPO].p[1];
	  Ptot_mc[2] += mPO[iPO].p[2];
	  if(i>1 || GENIEv.brIsNC)
	    {
	      Phad_mc[0] += mPO[iPO].p[0];
	      Phad_mc[1] += mPO[iPO].p[1];
	      Phad_mc[2] += mPO[iPO].p[2];
	      Phad_mc[3] += mPO[iPO].p[3];
	    }
	}
    }
  
  /* Total generated momentum modulus */
  Ptot_mc[3] = VUMOD3(Ptot_mc);
  
  /* Hadronic System angle at generation level */
  thetahadmc = atan2(PTCALC(Phad_mc), Phad_mc[2]);
  
  /* Do we want to convert photons into electron-positron pairs???*/
  //  if(igammaconv) 
  //    simulate_photon_conversion();
  
  /* Do we want to desintegrate the K0s into a pair of pions ? */
  //  if(iK0sdecay)
  //    K0s_decay_simula();
  
  /* Before smearing we compute QT_MC */
  //  compute_qt_mc();
  
  /* save MC values */
  for(int i=0;i<mNPO;i++)
    for(int j=0;j<4;j++)
      mPO[i].pmc[j]=mPO[i].p[j];

}
/*--------------------------------------------------------------------------------------------*/
int RawDataGenie::doOpenGENIE(const char *ROOTfileName)
{
  //   char myfile[100];
//     system("source /Users/sebastienmurphy/QscanPandora/Qscan/test.sh");
// //    setenv("GST_FILE","/Users/sebastienmurphy/Genie_data/numu_10k_argon_sbl.gst.root",true);
//     cout<<" the file is:"<<getenv("GST_FILE")<<endl;;
//     exit(0);
//     sprintf(myfile,"%s",getenv("GST_FILE"));
// //    sprintf(myfile,"%s","/Users/sebastienmurphy/Genie_data/numu_10k_argon_sbl.gst.root");
    std::cout << "/////////RawDataGenie////////// Opening file ... " << ROOTfileName << " ... "<<endl;

  rootfile= new TFile(ROOTfileName);
//    rootfile= new TFile(myfile);
  
  s_tree=(TTree*)rootfile->Get("gst");
  
  //-- create tree branches
  //
  s_tree->SetBranchAddress("iev",       &GENIEv.brIev);
  s_tree->SetBranchAddress("neu",       &GENIEv.brNeutrino);
  s_tree->SetBranchAddress("fspl",      &GENIEv.brFSPrimLept);
  s_tree->SetBranchAddress("tgt",       &GENIEv.brTarget);
  s_tree->SetBranchAddress("Z",         &GENIEv.brZ);
  s_tree->SetBranchAddress("A",         &GENIEv.brA);
  s_tree->SetBranchAddress("hitnuc",    &GENIEv.brHitNuc);
  s_tree->SetBranchAddress("hitqrk",    &GENIEv.brHitQrk);
  s_tree->SetBranchAddress("sea",       &GENIEv.brFromSea);
  s_tree->SetBranchAddress("resid",     &GENIEv.brResId);
  s_tree->SetBranchAddress("qel",       &GENIEv.brIsQel);
  s_tree->SetBranchAddress("res",       &GENIEv.brIsRes);
  s_tree->SetBranchAddress("dis",       &GENIEv.brIsDis);
  s_tree->SetBranchAddress("coh",       &GENIEv.brIsCoh);
  s_tree->SetBranchAddress("dfr",       &GENIEv.brIsDfr);
  s_tree->SetBranchAddress("imd",       &GENIEv.brIsImd);
  s_tree->SetBranchAddress("nuel",      &GENIEv.brIsNuEL);
  s_tree->SetBranchAddress("em",        &GENIEv.brIsEm);
  s_tree->SetBranchAddress("cc",        &GENIEv.brIsCC);
  s_tree->SetBranchAddress("nc",        &GENIEv.brIsNC);
  s_tree->SetBranchAddress("charm",     &GENIEv.brIsCharmPro);
  s_tree->SetBranchAddress("neut_code", &GENIEv.brNeut_code);
  s_tree->SetBranchAddress("nuance_code",&GENIEv.brNuance_code);
  s_tree->SetBranchAddress("wght",      &GENIEv.brWeight);
  s_tree->SetBranchAddress("xs",        &GENIEv.brKineXs);
  s_tree->SetBranchAddress("ys",        &GENIEv.brKineYs);
  s_tree->SetBranchAddress("ts",        &GENIEv.brKineTs);
  s_tree->SetBranchAddress("Q2s",       &GENIEv.brKineQ2s);
  s_tree->SetBranchAddress("Ws",        &GENIEv.brKineWs);
  s_tree->SetBranchAddress("x",         &GENIEv.brKineX);
  s_tree->SetBranchAddress("y",         &GENIEv.brKineY);
  s_tree->SetBranchAddress("t",         &GENIEv.brKineT);
  s_tree->SetBranchAddress("Q2",        &GENIEv.brKineQ2);
  s_tree->SetBranchAddress("W",         &GENIEv.brKineW);
  s_tree->SetBranchAddress("Ev",        &GENIEv.brEv);
  s_tree->SetBranchAddress("pxv",       &GENIEv.brPxv);
  s_tree->SetBranchAddress("pyv",       &GENIEv.brPyv);
  s_tree->SetBranchAddress("pzv",       &GENIEv.brPzv);
  s_tree->SetBranchAddress("En",        &GENIEv.brEn);
  s_tree->SetBranchAddress("pxn",       &GENIEv.brPxn);
  s_tree->SetBranchAddress("pyn",       &GENIEv.brPyn);
  s_tree->SetBranchAddress("pzn",       &GENIEv.brPzn);
  s_tree->SetBranchAddress("El",        &GENIEv.brEl);
  s_tree->SetBranchAddress("pxl",       &GENIEv.brPxl);
  s_tree->SetBranchAddress("pyl",       &GENIEv.brPyl);
  s_tree->SetBranchAddress("pzl",       &GENIEv.brPzl);
  s_tree->SetBranchAddress("pl",        &GENIEv.brPl);
  s_tree->SetBranchAddress("cthl",      &GENIEv.brCthl);
  s_tree->SetBranchAddress("nfp",       &GENIEv.brNfP);
  s_tree->SetBranchAddress("nfn",       &GENIEv.brNfN);
  s_tree->SetBranchAddress("nfpip",     &GENIEv.brNfPip);
  s_tree->SetBranchAddress("nfpim",     &GENIEv.brNfPim);
  s_tree->SetBranchAddress("nfpi0",     &GENIEv.brNfPi0);
  s_tree->SetBranchAddress("nfkp",      &GENIEv.brNfKp);
  s_tree->SetBranchAddress("nfkm",      &GENIEv.brNfKm);
  s_tree->SetBranchAddress("nfk0",      &GENIEv.brNfK0);
  s_tree->SetBranchAddress("nfem",      &GENIEv.brNfEM);
  s_tree->SetBranchAddress("nfother",   &GENIEv.brNfOther);
  s_tree->SetBranchAddress("nip",       &GENIEv.brNiP);
  s_tree->SetBranchAddress("nin",       &GENIEv.brNiN);
  s_tree->SetBranchAddress("nipip",     &GENIEv.brNiPip);
  s_tree->SetBranchAddress("nipim",     &GENIEv.brNiPim);
  s_tree->SetBranchAddress("nipi0",     &GENIEv.brNiPi0);
  s_tree->SetBranchAddress("nikp",      &GENIEv.brNiKp);
  s_tree->SetBranchAddress("nikm",      &GENIEv.brNiKm);
  s_tree->SetBranchAddress("nik0",      &GENIEv.brNiK0);
  s_tree->SetBranchAddress("niem",      &GENIEv.brNiEM);
  s_tree->SetBranchAddress("niother",   &GENIEv.brNiOther);
  s_tree->SetBranchAddress("ni",        &GENIEv.brNi);
  s_tree->SetBranchAddress("pdgi",      &GENIEv.brPdgi);
  s_tree->SetBranchAddress("resc",      &GENIEv.brResc);
  s_tree->SetBranchAddress("Ei",        &GENIEv.brEi);
  s_tree->SetBranchAddress("pxi",       &GENIEv.brPxi);
  s_tree->SetBranchAddress("pyi",       &GENIEv.brPyi);
  s_tree->SetBranchAddress("pzi",       &GENIEv.brPzi);
  s_tree->SetBranchAddress("nf",        &GENIEv.brNf);
  s_tree->SetBranchAddress("pdgf",      &GENIEv.brPdgf);
  s_tree->SetBranchAddress("Ef",        &GENIEv.brEf);
  s_tree->SetBranchAddress("pxf",       &GENIEv.brPxf);
  s_tree->SetBranchAddress("pyf",       &GENIEv.brPyf);
  s_tree->SetBranchAddress("pzf",       &GENIEv.brPzf);
  s_tree->SetBranchAddress("pf",        &GENIEv.brPf);
  s_tree->SetBranchAddress("cthf",      &GENIEv.brCthf);
  s_tree->SetBranchAddress("vtxx",      &GENIEv.brVtxx);
  s_tree->SetBranchAddress("vtxy",      &GENIEv.brVtxy);
  s_tree->SetBranchAddress("vtxz",      &GENIEv.brVtxz);
  s_tree->SetBranchAddress("vtxt",      &GENIEv.brVtxt);
  s_tree->SetBranchAddress("calresp0",  &GENIEv.brCalresp0);
  
  std::cout << s_tree->GetEntries() << " events found ... looping... "<< std::endl;
  return (s_tree->GetEntries());
}
/*--------------------------------------------------------------------------------------------*/
microPO* RawDataGenie::getPO() { 

  return mPO; 

}
/*--------------------------------------------------------------------------------------------*/
int RawDataGenie::getNPO( void ) { 

  return mNPO;

}
/*--------------------------------------------------------------------------------------------*/
void RawDataGenie::DumpPOs(void)
{
  FILE *f = stdout;
  double phi,theta;
  char *name;
  double p[4];
  int    i;
  int    itype,code;

  fprintf(f,"--------------RawDataGenie--------------------------------------------\n");
  fprintf(f,"------------ EVENT %10d  ----- NU ENERGY %10.2f GeV ---------------\n",GENIEv.brIev, GENIEv.brEv);
  if(idumpGeVMeV)
    fprintf(f,"------------------------ ENERGIES IN MeV ------------------------\n");
  fprintf(f,"-----------------------------------------------------------------\n");
  fprintf(f,"    itype     Code                   Px        Py       Pz         P        Theta    Phi\n");
  for(i=0;i<mNPO;i++)
    {
      itype = i;
      code  = mPO[i].iCode;
      p[0]  = mPO[i].p[0];
      p[1]  = mPO[i].p[1];
      p[2]  = mPO[i].p[2];
      p[3]  = sqrt(SQR(p[0])+SQR(p[1])+SQR(p[2])); /* energy with mass neglected */
      phi   = atan2(p[0],p[1])*180.0/PIGREEK;
      theta = atan2(sqrt(SQR(p[0])+SQR(p[1])),p[2])*180.0/PIGREEK;
      name = ParticleName((_ParticleType)ToGeantCode(code));
      if(idumpGeVMeV)
        {
          p[0]*=1000.0;
          p[1]*=1000.0;
          p[2]*=1000.0;
          p[3]*=1000.0;
        }
      if(mPO[i].UseIt<1)fprintf(f,"(");
      fprintf(f,"  %7d  %7d(%10s)   %9.3f %9.3f %9.3f %9.3f %9.3f %9.3f",
              itype,code,name,p[0],p[1],p[2],p[3],theta,phi);
      if(mPO[i].UseIt<1)fprintf(f,")");
      fprintf(f,"\n");
      p[0]  = mPO[i].pmc[0];
      p[1]  = mPO[i].pmc[1];
      p[2]  = mPO[i].pmc[2];
      p[3]  = sqrt(SQR(p[0])+SQR(p[1])+SQR(p[2])); /* energy with mass neglected */
      phi   = atan2(p[0],p[1])*180.0/PIGREEK;
      theta = atan2(sqrt(SQR(p[0])+SQR(p[1])),p[2])*180.0/PIGREEK;
      if(idumpGeVMeV)
        {
          p[0]*=1000.0;
          p[1]*=1000.0;
          p[2]*=1000.0;
          p[3]*=1000.0;
        }
      fprintf(f,"  %7s  %7s %10s    %9.3f %9.3f %9.3f %9.3f %9.3f %9.3f",
              "MC","","",p[0],p[1],p[2],p[3],theta,phi);
      fprintf(f,"\n");
      cout<<"iTrack,emeas,edep "<<mPO[i].iTrack<<" "<<mPO[i].Emeasured<<" "<<mPO[i].EdepAr<<" "<<endl;
    }
  fprintf(f,"-----------------------------------------------------------------\n");
  phi   = atan2(RecKine.Ptot[0],RecKine.Ptot[1])*180.0/PIGREEK;
  theta = atan2(sqrt(SQR(RecKine.Ptot[0])+SQR(RecKine.Ptot[1])),RecKine.Ptot[2])*180.0/PIGREEK;
  fprintf(f,"  %7s  %7s %10s    %9.3f %9.3f %9.3f %9.3f %9.3f %9.3f\n",
          "Tot","vis","",RecKine.Ptot[0],RecKine.Ptot[1],RecKine.Ptot[2],RecKine.Ptot[3],theta,phi);
  fprintf(f,"-----------------------------------------------------------------\n");
}
/*--------------------------------------------------------------------------------------------*/
void RawDataGenie::Dump_ND_POs(void)
{ 
  FILE *f = stdout;
  
  char *name;
  double p[4];
  int    i;
  int    itype,code;
   
  
  if(idumpGeVMeV)
    fprintf(f,"---------RawDataGenie-------- ENERGIES IN MeV ------------------------\n");
  fprintf(f,"-----------------------------------------------------------------\n");
  fprintf(f,"    itype     Code                   Px        Py       Pz         P        Theta    Phi\n");
  for(i=0;i<mNPO;i++)
    {
      itype = i;
      code  = mPO[i].iCode;
      p[0]  = mPO[i].p[0];
      p[1]  = mPO[i].p[1];
      p[2]  = mPO[i].p[2];
      p[3]  = sqrt(SQR(p[0])+SQR(p[1])+SQR(p[2])); /* energy with mass neglected */
      name = ParticleName((_ParticleType)ToGeantCode(code));
      if(idumpGeVMeV)
        {
          p[0]*=1000.0;
          p[1]*=1000.0;
          p[2]*=1000.0;
          p[3]*=1000.0;
        }
      if(mPO[i].UseIt<1)fprintf(f,"(");
      fprintf(f,"  %7d  %7d(%10s)   %9.3f %9.3f %9.3f %9.3f ",
              itype,code,name,p[0],p[1],p[2],p[3]);
      if(mPO[i].UseIt<1)fprintf(f,")");
      fprintf(f,"\n");
      p[0]  = mPO[i].pmc[0];
      p[1]  = mPO[i].pmc[1];
      p[2]  = mPO[i].pmc[2];
      p[3]  = sqrt(SQR(p[0])+SQR(p[1])+SQR(p[2])); /* energy with mass neglected */
//      phi   = atan2(p[0],p[1])*180.0/PIGREEK;
//      theta = atan2(sqrt(SQR(p[0])+SQR(p[1])),p[2])*180.0/PIGREEK;
      if(idumpGeVMeV)
        {
          p[0]*=1000.0;
          p[1]*=1000.0;
          p[2]*=1000.0;
          p[3]*=1000.0;
        }
      fprintf(f,"  %7s  %7s %10s    %9.3f %9.3f %9.3f %9.3f ",
              "MC","","",p[0],p[1],p[2],p[3]);
      fprintf(f,"\n");
      cout<<"iTrack,emeas,edep "<<mPO[i].iTrack<<" "<<mPO[i].Emeasured<<" "<<mPO[i].EdepAr<<" "<<endl;
    }
  fprintf(f,"-----------------------------------------------------------------\n");
//  phi   = atan2(RecKine.Ptot[0],RecKine.Ptot[1])*180.0/PIGREEK;
//  theta = atan2(sqrt(SQR(RecKine.Ptot[0])+SQR(RecKine.Ptot[1])),RecKine.Ptot[2])*180.0/PIGREEK;
//  fprintf(f,"  %7s  %7s %10s    %9.3f %9.3f %9.3f %9.3f %9.3f %9.3f\n",
//          "Tot","vis","",RecKine.Ptot[0],RecKine.Ptot[1],RecKine.Ptot[2],RecKine.Ptot[3],theta,phi);
  fprintf(f,"-----------------------------------------------------------------\n");
}
/*--------------------------------------------------------------------------------------------*/
int RawDataGenie::DumpGENIEEvent()
{
  DumpPOs();
  return 0;
}
/*--------------------------------------------------------------------------------------------*/
int RawDataGenie::DumpGENIE_ND_Event()
{
  Dump_ND_POs();
  return 0;
}

/*--------------------------------------------------------------------------------------------*/
int RawDataGenie::doOpen_ND_GENIE(const char *ROOTfileName)
{
  
    std::cout << "//////////// doOpen_ND_GENIE "<<endl;
    std::cout << "///////////////// Opening file ... " << ROOTfileName << " ... "<<endl;

  rootfile= new TFile(ROOTfileName);
//    rootfile= new TFile(myfile);
  
  s_tree=(TTree*)rootfile->Get("gst");
   
   
  //-- create tree branches
     
   
      s_tree->SetBranchAddress("iev",      &GENIEnd.brIev);
      s_tree->SetBranchAddress("ichannel", &GENIEnd.brCha);
      s_tree->SetBranchAddress("nout",     &GENIEnd.brnout); 
      s_tree->SetBranchAddress("status",&GENIEnd.brStatus);
      s_tree->SetBranchAddress("pdg",	 &GENIEnd.brPdg);
      s_tree->SetBranchAddress("energy",&GENIEnd.brEf);
      s_tree->SetBranchAddress("px",	 &GENIEnd.brPxf);
      s_tree->SetBranchAddress("py",	 &GENIEnd.brPyf);
      s_tree->SetBranchAddress("pz",	 &GENIEnd.brPzf);
      s_tree->SetBranchAddress("vx",	 &GENIEnd.brvx);
      s_tree->SetBranchAddress("vy",	 &GENIEnd.brvy);
      s_tree->SetBranchAddress("vz",	 &GENIEnd.brvz); 
      s_tree->SetBranchAddress("vt",	 &GENIEnd.brvt);     
      
  
	
      std::cout << s_tree->GetEntries() << " events found ... looping... "<< std::endl;
      
     
      return (s_tree->GetEntries());
}
  
/*--------------------------------------------------------------------------------------------*/
void RawDataGenie::fill_PO_ND_table(int reinter)
{
 
  mNPO=0;
 
      
   //loop to select finalstate =1 particles

 

    for(int i=0;i<GENIEnd.brnout;i++)
      {
         if (GENIEnd.brStatus[i]==1) {
        mPO[mNPO].iCode=GENIEnd.brPdg[i];
        mPO[mNPO].p[0]=GENIEnd.brPxf[i];
        mPO[mNPO].p[1]=GENIEnd.brPyf[i];
        mPO[mNPO].p[2]=GENIEnd.brPzf[i];
        mPO[mNPO].p[3]=VUMOD3(mPO[mNPO].p);
        mPO[mNPO].Emeasured = GENIEnd.brEf[i];
        mPO[mNPO].UseIt= 1; 
	
	mPO[mNPO].GeantId=ToGeantCode(mPO[mNPO].iCode);
	
	
// 	cout<<"  "<<GENIEnd.brPdg[i]<<"  "<<GENIEnd.brPxf[i]<<"  "<<GENIEnd.brPyf[i]<<" "<<GENIEnd.brPzf[i]<<"  "<<GENIEnd.brEf[i]<<endl;
// 	cout<<"  "<<mPO[mNPO].iCode<<"   "<<mPO[mNPO].p[0]<<"  "<< mPO[mNPO].p[1]<<"  "<< mPO[mNPO].p[2]<<"  "<<mPO[mNPO].p[3]<<"  "<<mPO[mNPO].Emeasured<<"  "<<mPO[mNPO].GeantId<<endl;
	
	 mNPO++;
      }
}
   
     

    
  
 
  /* save MC values */
  for(int i=0;i<mNPO;i++)
    for(int j=0;j<4;j++)
      mPO[i].pmc[j]=mPO[i].p[j];

}
