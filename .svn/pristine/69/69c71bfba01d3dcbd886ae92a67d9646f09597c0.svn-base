#include <cassert>

#include <TROOT.h>
#include <TInterpreter.h>
#include <TDatabasePDG.h>
#include <TVirtualMC.h>

#include "LiquidArgonMedium.h"
#include "RawDataVirtualMC.h"

//specific detector geometires
#include "GeomMapping6x6x6m3.h"
#include "GeomMapping1x1x3m3.h"


#include "EFieldAndDriftMap.h"
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>

using namespace std;


// ctor
RawDataVirtualMC::RawDataVirtualMC(std::string detgeom, std::string vmctransport)
{
  InitDetGeom(detgeom);
  InitVMC(vmctransport);

  genieInput = new RawDataGenie();
  
  isOpenInputFile = 0;
  isEOF           = 1;
  RunNumber       = -1;
  EventNumber     = 0;
  runVMC          = true;

  PtrQdrift      = NULL;
  PtrEfield      = NULL;
  PtrCRPData     = NULL;

  PtrLROData     = NULL;

  m_isGenieNu    = false;
  m_isGenieNd    = false;

  m_Primaries    = new TObjArray();
  //m_Primaries->SetOwner(true);
}

// ctor
RawDataVirtualMC::RawDataVirtualMC( std::string detgeom, std::string vmctransport,
				    ChargeDrift* qdrift, EFieldMap* efld )
{



  InitDetGeom(detgeom);
  InitVMC(vmctransport);

  genieInput = new RawDataGenie();

  isOpenInputFile = 0;
  isEOF           = 0;
  RunNumber       = -1;
  EventNumber     = 0;
  runVMC          = true;

  PtrQdrift      = qdrift;
  PtrEfield      = efld;
  PtrCRPData     = NULL;

  m_isGenieNu    = false;
  m_isGenieNd    = false;

  m_Primaries    = new TObjArray();
  m_Primaries->SetOwner(true);
}

// dtor
RawDataVirtualMC::~RawDataVirtualMC()
{
  delete genieInput;
  delete detGeom;
  delete appVMC;
  
  delete m_Primaries;
}  
  

// build detector geometry
void RawDataVirtualMC::InitDetGeom(std::string detgeom)
{
  if(detgeom.compare("WA105_6x6x6") == 0)
    {
      // WA105 6x6x6 m3 
      GeomMapping6x6x6m3* myDet = new GeomMapping6x6x6m3();
      // construct geometry
      myDet->CreateGeometry();
      detGeom = myDet; 
    }
  else if(detgeom.compare("WA105_3x1x1") == 0)
    {
      // WA105 3x1x1 m3 proto
      GeomMapping1x1x3m3* myDet = new GeomMapping1x1x3m3();
      // construct geometry
      myDet->CreateGeometry();
      detGeom = myDet; 
    }
  else
    {
      cerr<<"ERROR: detector geometry for "<<detgeom<<" is not defined "<<endl;
      exit(0);
    }
}

// build vmc 
void RawDataVirtualMC::InitVMC(std::string vmctransport)
{
  string geant="g3";
  string libFile;
  string libFunc;
  string confFile;
  
  cout <<"RawDataVirtualMC::LoadVMC() "<<endl;
  
  if      (!strcmp(vmctransport.c_str(),"G3")) geant = "g3";
  else if (!strcmp(vmctransport.c_str(),"G4")) geant = "g4";
  else if (!strcmp(vmctransport.c_str(),"G4.9.4")) geant = "g4";
  else if (!strcmp(vmctransport.c_str(),"G4.9.5")) geant = "g4";
  else 
    {
      cerr << "RawDataVirtualMC::InitVMC ERROR: Only VMC_TRANSPORTCODE = G3 or G4 are allowed! Exit." << endl;
      exit(0);
    }

  if (!strcmp(vmctransport.c_str(),"G4.9.5"))
    libFile = geant + "libsNew.C";
  else 
    libFile = geant + "libs.C";
  
  libFunc  = geant + "libs()";
  confFile = geant + "tgeoConfig.C";

  //load geant and mc libraries
  gROOT->LoadMacro(libFile.c_str());
  gInterpreter->ProcessLine(libFunc.c_str());

  //load MC application
  appVMC = new VMCApplication("VMCAPP", "The VMC application");
  appVMC->SetRawDataVirtualMC(this);

  //initialize geant3/geant4 vmc
  appVMC->InitMC(confFile.c_str());
  
  // set grandom seed for gRandom
  gMC->GetRandom()->SetSeed(myPrefs->iGRandomSeed);
}




// store data
void RawDataVirtualMC::StoreTrackStep( int pdg, double *pos, double t, 
				       double edep, double step )
{

  //dummy
  double gain = 1;

  //Record events only in the active volume
  std::string volname = gMC->CurrentVolName();

  //For light production that should be better defined
  double tmp_EdepCRT1 = 0.;
  double tmp_EdepCRT2 = 0.;

  if(strcmp("CRT1 VOLUME",  gMC->CurrentVolName()) == 0)
    {
      tmp_EdepCRT1 = edep;
      m_EdepCRT1 += tmp_EdepCRT1;
    }
  if(strcmp("CRT2 VOLUME",  gMC->CurrentVolName()) == 0)
    {
      tmp_EdepCRT2 = edep;
      m_EdepCRT2 += tmp_EdepCRT2;
    }

  if(strcmp("ACTIVE VOLUME",  gMC->CurrentVolName()) != 0) //node->GetVolume()->GetName()))
    return;
  
  //Keep only primaries that enter active volume (TPC volume with electric field)
  if(m_KeepPrimary.back() == 0) {m_KeepPrimary.back() = 1;}
  
  //Energy leaked out of active volume when particle exiting (it does not take into account the energy lost before enetring the active region)
  if (gMC->IsTrackExiting()) 
    {
      TLorentzVector momentum;
      gMC->TrackMomentum(momentum);
      m_Elost += 1.0E+3*(momentum.T() - gMC->TrackMass());
      //Equivalent formula:                                                                                                                                  
      //m_Elost += 1.0E+3*sqrt( pow(momentum.Px(),2)+pow(momentum.Py(),2)+pow(momentum.Pz(),2)+pow(gMC->TrackMass(),2) );       
    }

  gGeoManager->SetCurrentPoint(pos);
  TGeoNode *node = gGeoManager->FindNode(kTRUE);
  
  if (!node )
    return;

  // negative step : round off bug?
  if( step < 0 ) step = 0;

  double tmp_Edep, tmp_EdepQ;
  double tmp_EdepE, tmp_EdepEQ;
  double tmp_EdepH, tmp_EdepHQ;
  tmp_Edep  = tmp_EdepQ  = 0.0;
  tmp_EdepE = tmp_EdepEQ = 0.0;
  tmp_EdepH = tmp_EdepHQ = 0.0;

  //transform to "ACTIVE VOLUME" coordinates   
  double chargeXYZ[3];
  gGeoManager->MasterToLocal(pos,chargeXYZ); 

  // get Efield and calculate quenching
  double R      = 0;
  double efield = 0;
  if(PtrQdrift && PtrEfield)
    {

      efield = PtrEfield->EField(chargeXYZ[0], chargeXYZ[1], chargeXYZ[2]);
      R      = PtrQdrift->GetQuenchingFactor(edep/step, efield); // !!! returns 0 if step == 0
	
    }
  
  //quenced energy
  double edepq  = R*edep; 
  // edep --> qdep
  double qdep   = LiquidArgonMedium::Instance()->ConvertEdepToQ(edepq);

  // save deposited energy for the event header
  //m_Edep  += edep;
  //m_EdepQ += edepq;
  tmp_Edep  = edep;
  tmp_EdepQ = edepq;

  // electronic part
  if (pdg==11 || pdg==-11 || pdg==22 || pdg==111) 
    {
      //m_EdepE  += edep;
      //m_EdepEQ += edepq;
      tmp_EdepE  = edep;
      tmp_EdepEQ = edepq;
    }
  else // hadronoc part?
    {
      //m_EdepH  += edep;
      //m_EdepHQ += edepq;
      tmp_EdepH  = edep;
      tmp_EdepHQ = edepq;
    }
  //add beam energy, energy from cosmic bkg, lost energy ???
  
  
  if (!qdep) return;


  double dt=-1, xcrp=0, ycrp=0, qcrp=0; //us, cm, cm, fC

  double sigmaDt = 0, sigmaDl = 0; // diffusion in cm, us

  double zcrp = PtrCRPData->GetZ();

  // fill CRP data structures
  if(PtrCRPData && PtrQdrift)
    {
      // drift charge

      if(PtrCRPData->GetZ() < chargeXYZ[2] ) 
	{
	  cout<<" Problem in geant's volume assignment ?"<<endl;
	  cout<< pos[2] <<"  "<<chargeXYZ[2] <<" "
	      << node->GetVolume()->GetName() <<" " << volname  << endl;
	}
      
      PtrQdrift->Drift( chargeXYZ[0], chargeXYZ[1], chargeXYZ[2], qdep, 
			PtrCRPData->GetZ(), PtrEfield, dt, xcrp, ycrp, qcrp,
			sigmaDl, sigmaDt);
      
      //note time should be in us and distance in cm
      PtrCRPData->Fill( pdg, edep, edepq, chargeXYZ[0], chargeXYZ[1], chargeXYZ[2], 
			1.0E+6*t, dt, xcrp, ycrp, qcrp, sigmaDl, sigmaDt ); 

      // add energy deposits only for samples inside drift window
      if( PtrCRPData->IsInDriftWin( 1.0E+6*t + dt ) )
	{
	  m_Edep   += tmp_Edep;
	  m_EdepQ  += tmp_EdepQ;
	  m_EdepE  += tmp_EdepE;
	  m_EdepEQ += tmp_EdepEQ;
	  m_EdepH  += tmp_EdepH;
	  m_EdepHQ += tmp_EdepHQ;
	}
    }
  else  // if no output CRP is defind just save the energy depositions
    {
      m_Edep   += tmp_Edep;
      m_EdepQ  += tmp_EdepQ;
      m_EdepE  += tmp_EdepE;
      m_EdepEQ += tmp_EdepEQ;
      m_EdepH  += tmp_EdepH;
      m_EdepHQ += tmp_EdepHQ;
    }


  //
  //Fill LRO Data Structure
  //

  
  if( PtrLROData && PtrQdrift) {

    double pos_step[3]={chargeXYZ[0], chargeXYZ[1], chargeXYZ[2]};

    double pos_crp[3]={xcrp,ycrp,zcrp};

    // times in ns
    PtrLROData->Fill(pdg, edep,step,efield,R,pos_step,pos_crp, 1.0E+9*t, 1.0E+3*dt, gain);
    

  }//LROData


}
 

// input beam particle source
int RawDataVirtualMC::OpenInputFile(const char *fname)
{ 
  int needhelp = 0;
  
  inputFileName = fname;
     
  if(inputFileName.BeginsWith("pdg:")) 
    {// user defined pencil beam                                                                                                                                                              
      std::cout << "OpenInputFile() INFO: define primary particles" << std::endl;
      std::string pdgCodeString;
      TString pdgName;
      bool pdgCode = false;
      for(int i=4; i<inputFileName.Sizeof(); i++){
	if(isdigit(inputFileName[i])) 
	  {
	    pdgCodeString += inputFileName[i];
	    pdgCode = true;
	  }
      }
      //if pdg in datacard is a number 
      if(pdgCode == true)
	{
	  const char *pdgCodeChar;
	  pdgCodeChar = pdgCodeString.c_str();
	  int pdgCodeNum = std::atoi(pdgCodeChar);
	  TParticlePDG * aParticlePDG;
	  if(!(aParticlePDG = TDatabasePDG::Instance()->GetParticle(pdgCodeNum)))
	    {
	      std::cerr << " could not understand particle PDG code " << pdgName << std::endl;
	      needhelp++;
	    }
	  
	  else 
	    {
	    // define particle generator (using datacards)                                                                                                                                       
	    std::cerr << "pdg code (same as in GEANT4): " << aParticlePDG->PdgCode() 
			<< ", pdg name: " << aParticlePDG->GetTitle() 
			<< ", particle class: " << aParticlePDG->ParticleClass() 
			<< ", particle charge (in units of |e|/3): " << aParticlePDG->Charge() 
			<< ", particle mass: " << aParticlePDG->Mass()*1000 << " Mev"  
			<< std::endl;
	    VMCPrimaryGenerator *primary = appVMC->GetPrimaryGenerator();
	    primary->SetType(VMCPrimaryGenerator::kSector);
	    primary->SetPdgCode(aParticlePDG->PdgCode());
	    primary->SetPMin(myPrefs->fPrimaryPabs[0]);
	    primary->SetPMax(myPrefs->fPrimaryPabs[1]);
	    primary->SetThetaMinMax(myPrefs->fPrimaryTheta[0],
                                  myPrefs->fPrimaryTheta[1]);
	    primary->SetPhiMinMax(myPrefs->fPrimaryPhi[0],myPrefs->fPrimaryPhi[1]);
	    primary->SetVxMinMax(myPrefs->fPrimaryVx[0],myPrefs->fPrimaryVx[1]);
	    primary->SetVyMinMax(myPrefs->fPrimaryVy[0],myPrefs->fPrimaryVy[1]);
	    primary->SetVzMinMax(myPrefs->fPrimaryVz[0],myPrefs->fPrimaryVz[1]);
	    primary->SetVMCSeed(myPrefs->iPrimarySeed);
	  }
	}
      //if pdg in datacard is a string
      else
	{
	  TString pdgName(inputFileName(4,inputFileName.Sizeof()));
	  TParticlePDG * aParticlePDG;
	  if(!(aParticlePDG = TDatabasePDG::Instance()->GetParticle(pdgName)))
	  {
	    std::cerr << " could not understand particle PDG code " << pdgName << std::endl;
	    needhelp++;
	  }
          else 
	    {
	      // define particle generator (using datacards)                                                                                                                                        
	      std::cerr << "pdg code (same as in GEANT4): " << aParticlePDG->PdgCode() 
			<< ", pdg name: " << aParticlePDG->GetTitle() 
			<< ", particle class: " << aParticlePDG->ParticleClass() 
			<< ", particle mass: " << aParticlePDG->Mass()*1000 << " Mev"   
			<< ", particle charge (in units of |e|/3): " << aParticlePDG->Charge() 
			<< std::endl;
	      VMCPrimaryGenerator *primary = appVMC->GetPrimaryGenerator();
	      primary->SetType(VMCPrimaryGenerator::kSector);
	      primary->SetPdgCode(aParticlePDG->PdgCode());
	      primary->SetPMin(myPrefs->fPrimaryPabs[0]);
	      primary->SetPMax(myPrefs->fPrimaryPabs[1]);
	      primary->SetThetaMinMax(myPrefs->fPrimaryTheta[0],
				    myPrefs->fPrimaryTheta[1]);
	      primary->SetPhiMinMax(myPrefs->fPrimaryPhi[0],myPrefs->fPrimaryPhi[1]);
	      primary->SetVxMinMax(myPrefs->fPrimaryVx[0],myPrefs->fPrimaryVx[1]);
	      primary->SetVyMinMax(myPrefs->fPrimaryVy[0],myPrefs->fPrimaryVy[1]);
	      primary->SetVzMinMax(myPrefs->fPrimaryVz[0],myPrefs->fPrimaryVz[1]);
	      primary->SetVMCSeed(myPrefs->iPrimarySeed);
	  }
	}
    }
  
  else if(inputFileName.BeginsWith("gst:")) // GENIE root file
    { 
      m_isGenieNu = true;

      nEve = genieInput->doOpenGENIE(inputFileName(4,inputFileName.Sizeof()).Data());
      lastEve = 0; // start from 0 added initialization for lastEve

      // define particle generator
      VMCPrimaryGenerator *primary = appVMC->GetPrimaryGenerator();
      primary->SetType(VMCPrimaryGenerator::kGENIE);
      
      primary->SetRawDataGeniePtr(genieInput);

      primary->SetPMin(myPrefs->fPrimaryPabs[0]);
      primary->SetPMax(myPrefs->fPrimaryPabs[1]);
      primary->SetThetaMinMax(myPrefs->fPrimaryTheta[0],myPrefs->fPrimaryTheta[1]);
      primary->SetPhiMinMax(myPrefs->fPrimaryPhi[0],myPrefs->fPrimaryPhi[1]);
      primary->SetVxMinMax(myPrefs->fPrimaryVx[0],myPrefs->fPrimaryVx[1]);
      primary->SetVyMinMax(myPrefs->fPrimaryVy[0],myPrefs->fPrimaryVy[1]);
      primary->SetVzMinMax(myPrefs->fPrimaryVz[0],myPrefs->fPrimaryVz[1]);
      primary->SetVMCSeed(myPrefs->iPrimarySeed);
    }
  else if (inputFileName.BeginsWith("WA105_MCEVENT")) //WA105 MC event
    {

      // define particle generator (using datacards)
      VMCPrimaryGenerator *primary = appVMC->GetPrimaryGenerator();
      primary->SetType(VMCPrimaryGenerator::kWA105);

      if(myPrefs->iWa105Beam)
	{
	  TParticlePDG *aParticlePDG;
	  if ((aParticlePDG = TDatabasePDG::Instance()->GetParticle(myPrefs->sWa105BeamPdgName)))
	    {
	      primary->SetWA105BeamOn(true);

	      //kinematics
	      primary->SetPdgCode(aParticlePDG->PdgCode());
	      primary->SetPMin(myPrefs->fPrimaryPabs[0]);
	      primary->SetPMax(myPrefs->fPrimaryPabs[1]);
	      primary->SetThetaMinMax(myPrefs->fPrimaryTheta[0],
				      myPrefs->fPrimaryTheta[1]);
	      primary->SetPhiMinMax(myPrefs->fPrimaryPhi[0],
				    myPrefs->fPrimaryPhi[1]);
	      primary->SetVxMinMax(myPrefs->fPrimaryVx[0],myPrefs->fPrimaryVx[1]);
	      primary->SetVyMinMax(myPrefs->fPrimaryVy[0],myPrefs->fPrimaryVy[1]);
	      primary->SetVzMinMax(myPrefs->fPrimaryVz[0],myPrefs->fPrimaryVz[1]);
	      primary->SetVMCSeed(myPrefs->iPrimarySeed);
	    }
	}
      if(myPrefs->iWa105CosmicBkg)
	{
	  primary->SetWA105CRBackgroundOn(true);
	  primary->ConfigWA105CRBackground( myPrefs->sWa105CosmicBkgFile, 
					    myPrefs->fWa105CosmicT0, 
					    myPrefs->fWa105CosmicWin,
					    myPrefs->fWa105CosmicRate );
	}

      if(myPrefs->iWa105BHBkg)
	{
	  primary->SetWA105BHBackgroundOn(true);
	  primary->ConfigWA105BHBackground( myPrefs->fWa105BHT0, myPrefs->fWa105BHWin, myPrefs->fWa105BHRate,
					    myPrefs->fWa105BHProfile[0], myPrefs->fWa105BHProfile[1], 
					    myPrefs->fWa105BHProfile[2], myPrefs->fWa105BHProfile[3],
					    myPrefs->fWa105BHProfile[4], myPrefs->fWa105BHProfile[5],
					    myPrefs->fWa105BHProfile[6], myPrefs->fWa105BHProfile[7],
					    myPrefs->fWa105BHProfile[8], myPrefs->fWa105BHProfile[9] );
	}
      
    }
  else if (inputFileName.BeginsWith("NDgst:")) { // GENIE nucleon decay root file
    
    m_isGenieNd = true;

    nEve = genieInput->doOpen_ND_GENIE(inputFileName(6,inputFileName.Sizeof()).Data());
    lastEve = 0; //start from 0 added initialization for lastEve
    
    // define particle generator
    VMCPrimaryGenerator *primary = appVMC->GetPrimaryGenerator();
    
    primary->SetType(VMCPrimaryGenerator::kGENIE_nd);
    primary->SetRawDataGeniePtr(genieInput);

    primary->SetVxMinMax(myPrefs->fPrimaryVx[0],myPrefs->fPrimaryVx[1]);
    primary->SetVyMinMax(myPrefs->fPrimaryVy[0],myPrefs->fPrimaryVy[1]);
    primary->SetVzMinMax(myPrefs->fPrimaryVz[0],myPrefs->fPrimaryVz[1]);
    primary->SetThetaMinMax(myPrefs->fPrimaryTheta[0],myPrefs->fPrimaryTheta[1]);
    
    primary->SetPhiMinMax(myPrefs->fPrimaryPhi[0], myPrefs->fPrimaryPhi[1]);
    primary->SetVMCSeed(myPrefs->iPrimarySeed);
  }
  else 
    {
      std::cerr << "RawDataVirtualMC: unknown input!" << std::endl;
      needhelp++;
    }
  
  if(needhelp) {
    std::cerr << " valid examples: " << std::endl;
    std::cerr << "     pdg:e+,1.0           for 1 GeV/c positrons" << std::endl;
    std::cerr << "     pdg:proton,1.7       for 1.7 GeV/c proton, ..." << std::endl;
    exit(1);
  }
  
  isOpenInputFile = 1;
  isEOF = 0;
  
  return 1;
}


// close input file
int RawDataVirtualMC::CloseInputFile()
{
  return 1;
}


// load event
int RawDataVirtualMC::LoadEvent()
{
  while (runVMC) 
    {  // produce MC tracks until the trigger condition is fulfilled
      
      if(inputFileName.BeginsWith("gst:")) // GENIE event loader
	{
	  if(lastEve>nEve) {
	    isEOF = 1;
	    return 1;
	  }
	  genieInput->s_tree->GetEntry(lastEve++);
	}
      if(inputFileName.BeginsWith("NDgst:")) // GENIE event loader
	{
	  if(lastEve>nEve) {
	    isEOF = 1;
	    return 1;
	  } 
	  //cout<<" getting NDgst entry "<<lastEve<<endl;
	  genieInput->s_tree->GetEntry(lastEve++);
	}
  
      appVMC->RunMC(1); // particle tracking with g3/g4
      if(IsTrigger()) 
	break;
      else
	gGeoManager->ClearTracks();
    }
  
  // loop over all primaries and remove those that didn't enter TPC active volume
  if( (int)m_KeepPrimary.size() == m_Primaries->GetEntries() )
    {
      TObjArray pricopy(*m_Primaries);
      m_Primaries->Clear();
      for(size_t i=0;i<m_KeepPrimary.size();i++)
	{
	  if( m_KeepPrimary[i] ) 
	    m_Primaries->Add( (TParticle*)pricopy.At(i) );
	}

    }
  else // do nothing
    {
      std::cerr<<"ERROR: "<<__FILE__<<", "<<__LINE__
	       <<" flag to keep primaries is out of step"<<std::endl;
    }


  EvtOk = 1;
  EventNumber++;
  
  return -1;
}


//
//
//
void RawDataVirtualMC::ClearPrimaries()
{
  m_KeepPrimary.clear();
  m_Primaries->Clear();
}
