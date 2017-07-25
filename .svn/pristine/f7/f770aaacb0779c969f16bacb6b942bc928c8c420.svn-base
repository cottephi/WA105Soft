
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <string>


#include <TFile.h>
#include <TGeoManager.h>

#include "EFieldAndDriftMap.h"

#include "EventLooper.h"
#include "TSystem.h"
#include "Geometry.h"
#include "LiquidArgonMedium.h"

using namespace std;

/*--------------------------------------------------------------------------------------------*/
/*                                   C L A S S    M E T H O D S                               */
/*--------------------------------------------------------------------------------------------*/

EventLooper::EventLooper(void)
{
  iEvent    = -1;
  iMaxEvent = -1;
  theRunHeader   = new RunHeader();
  theGeomConfig   = new GeomConfig();
  theBeamInfo    = new BeamInfo();
  theEventHeader = new EventHeader();

  //
  LroData = new LArLReadMC::DLArLRO();

  //
  CrpData = new LArQReadMC::DLArCRP();
  Qdrift  = new ChargeDrift();

  Efield  = NULL;
  DataVMC = NULL;

  fOut   = NULL;
  treeEV = NULL;
  treeRH = NULL;

  ageom = new Geometry();

}
/*--------------------------------------------------------------------------------------------*/
EventLooper::~EventLooper()
{

  if(Qdrift)
    delete Qdrift;

  if(CrpData)
    delete CrpData;


  if(LroData)
    delete LroData;

  if(Efield)
    delete Efield;

  if(DataVMC)
    delete DataVMC;

  if(theEventHeader)
    delete theEventHeader ;

  if(theBeamInfo)
    delete theBeamInfo;

  if(theGeomConfig)
    delete theGeomConfig;

  if(theRunHeader)
    delete theRunHeader;
}

/*--------------------------------------------------------------------------------------------*/
int EventLooper::LoadTree(const char* filename)
{

  fOut = new TFile(filename, "RECREATE");


  //
  if (myPrefs->iSaveTGeo)
    {
      TGeoManager *geo = DataVMC->GetGeoManager();
      if (geo)
	{
	  geo->Write();
	}
    }


  //run header tree  CreateAndFill_RunHeader();
  treeRH = new TTree("RunHeader ","RunHeader");
  treeRH->Branch("theRunHeader","RunHeader", &theRunHeader); //, 32000, 0);
  treeRH->Branch("theGeomConfig","GeomConfig", &theGeomConfig); //, 32000, 0);
  treeRH->Branch("theBeamInfo","BeamInfo", &theBeamInfo); //, 32000, 0);


  //event tree
  treeEV = new TTree("EventTree","EventTree");
  treeEV->Branch("theEventHeader","EventHeader", &theEventHeader);


  if(CrpData)
    CrpData->RegisterBranches(treeEV);

  if(LroData)
    LroData->RegisterBranches(treeEV);



  return 1;
}

/*--------------------------------------------------------------------------------------------*/
void EventLooper::SaveEvent()
{

  if (!fOut)
    return;

  if (iEvent==0)
    {
      treeRH->Fill();
    }

  //cout<<"Fill event tree"<<endl;
  treeEV->Fill();

}


/*--------------------------------------------------------------------------------------------*/
void EventLooper::CloseTree()
{
  if(!fOut)
    return;

  fOut->cd();
  treeRH->Write(0,TObject::kOverwrite);
  treeEV->Write(0,TObject::kOverwrite);

  if (myPrefs->iVerboseLevel>5)
    {
      treeRH->Print();
      treeEV->Print();
    }

  fOut->Close();
}

/*--------------------------------------------------------------------------------------------*/
void EventLooper::FileClose(void)
{

  if(DataVMC)
    {
      delete DataVMC;
      DataVMC = 0;
    }

  if (theRunHeader)
    {
      delete theRunHeader;
      theRunHeader = 0;
    }

  if (theGeomConfig)
    {
      delete theGeomConfig;
      theGeomConfig = 0;
    }

  if (theBeamInfo)
    {
      delete theBeamInfo;
      theBeamInfo = 0;
    }

  if(theEventHeader)
    {
      delete theEventHeader;
      theEventHeader = 0;
    }

  if(CrpData)
    {
      delete CrpData;
      CrpData = 0;
    }


  if(LroData)
    {
      delete LroData;
      LroData = 0;
    }



  if(Qdrift)
    {
      delete Qdrift;
      Qdrift = 0;
    }

  if(Efield)
    {
      delete Efield;
      Efield = 0;
    }

}

// open file
int EventLooper::FileOpen(const char *fname, const char *evtype)
{
  int ierr=1;

  string evtyp(evtype);

  cout<<"#####################################################################"<<endl<<endl;;
  cout<<"                      RUN SET UP                             "<<endl<<endl;;

  if(ierr)
    {
      cout << "  You are analyzing file -->" << fname << endl;

    }
  else
    {
      cout << " Error opening file -->" << fname << endl;

    }


  cout <<"  CRP detector configuration file  --> "<<myPrefs->sDLArConfFile<<endl;
  //

  string splitter ="_";
  string splitter1 =".";
  size_t pos=(myPrefs->sDLArConfFile).find(splitter);
  string token = (myPrefs->sDLArConfFile).substr(pos+1,string::npos);
  size_t pos1=token.find(splitter1);
  string token1 = token.substr(0,pos1);


  CheckCRPConfig(myPrefs->sDLArConfFile, token1);


  if(myPrefs->iVerboseLevel>0)
    {
      cout <<"=================================================== "<<endl;

      myPrefs->Dump();

      cout <<"=================================================== "<<endl;
    }


  // crp data and register output branches
  if(CrpData)
    {
      CrpData->DefineCRP(myPrefs->sDLArConfFile.c_str());
      
      if( myPrefs->sLemEffMapFile.Length())
	{
	  TString tmpstr = myPrefs->sLemEffMapFile;
	  gSystem->ExpandPathName( tmpstr );
	  CrpData->SetCollEffMap( tmpstr.Data() );
	}
      
      // sampling time
      theGeomConfig->SetCRMpitchT(1.0/CrpData->GetElecModel()->GetSampleFreq());
    }

  // fill run header
  FillRunHeader(fname);

  // set lifetime & recombination factors
  Qdrift->SetLifetime(myPrefs->fForceLifetime);   //in ms
  Qdrift->SetQuenchFactor(myPrefs->fForceRecomb); //should be <= 1

  // longitudinal diffusion & transverse diffusion constants
  Qdrift->SetDiffDl(myPrefs->fDiffusionDL);       //cm2/s
  Qdrift->SetDiffDt(myPrefs->fDiffusionDT);       //cm2/s




  // different field maps
  if( myPrefs->fEFieldValue > 0 ) {//uniform field

    Efield = new UniformEField(myPrefs->fEFieldValue); } // V/cm


  else // field map file
  {

    cout<<" "<<endl;
    std::cout<<" Opening EField Map file: "<<myPrefs->sEFieldMapFile<<std::endl;
    std::cout<<" Tags: "<<myPrefs->sEFieldMapTags<<std::endl;
    std::cout<<" Histos: "<<myPrefs->sEFieldMapHistos<<std::endl;


    //
    // to Check system of reference and units !!!
    //
    Efield = new EFieldAndDriftMap(  myPrefs->sEFieldMapFile
				    ,myPrefs->sEFieldMapTags
				    ,myPrefs->sEFieldMapHistos
				    );
    //test
    double ef = Efield->EField(0,0,0);

    std::cout<<" Ef "<<ef<<std::endl;

    std::cout<<dynamic_cast< EFieldAndDriftMap*>(Efield)->GetDriftTime(0,0,0)<<std::endl;

    //

  }


  //LRO
  if( LroData && myPrefs->LRO_NPMTS >0 ) {

    std::cout<<" "<<std::endl;

    std::cout<<" Light ReadOut "<<std::endl;
    std::cout<<" Maps "<<std::endl;

    LroData->SetNPMTs(myPrefs->LRO_NPMTS);

    std::cout<<" NPMTs: "<<  myPrefs->LRO_NPMTS <<std::endl;


    if(myPrefs->LRO_SAMPLING_TIME >0) {

     LroData->SetSamplingTime(myPrefs->LRO_SAMPLING_TIME);
     std::cout<<" Sampling Time: "<<myPrefs->LRO_SAMPLING_TIME<<" ns "<<std::endl;
     std::cout<<" "<<std::endl;
    }


    if(myPrefs->LRO_TIME_WIN_NBINS >0) {

      LroData->SetTimeWindow(
	myPrefs->LRO_TIME_WIN_NBINS,
	myPrefs->LRO_TIME_WIN_MIN,
	myPrefs->LRO_TIME_WIN_MAX
      );

      std::cout<<" Time Window: "<<
	myPrefs->LRO_TIME_WIN_NBINS<<" "<<
	myPrefs->LRO_TIME_WIN_MIN<<" "<<
	myPrefs->LRO_TIME_WIN_MAX
       <<std::endl;
       std::cout<<" "<<std::endl;
    }


    LroData->SetPMTQuantumEff( myPrefs->LRO_PMTS_QEFF );
    LroData->SetElectronLifeTime( myPrefs->fForceLifetime );//must be ms



    std::cout<<" PMTs Quantum Efficiency: "<<LroData->GetPMTQuantumEff()<<std::endl<<std::endl;
    std::cout<<" Electron Life Time: "<<LroData->GetElectronLifeTime()<<std::endl<<std::endl;


    std::cout<<"  "<<myPrefs->sLightMapTags_LAr<<std::endl;
    std::cout<<"  "<<myPrefs->sLightMapHistos_LAr<<std::endl;
    std::cout<<"  "<<myPrefs->sLightMapHIndex_LAr[0]<<" "<<myPrefs->sLightMapHIndex_LAr[1]<<std::endl;

    std::cout<<" GAr File: "<<myPrefs->sLightMapFile_GAr<<std::endl;
    std::cout<<"  "<<myPrefs->sLightMapTags_GAr<<std::endl;
    std::cout<<"  "<<myPrefs->sLightMapHistos_GAr<<std::endl;
    std::cout<<"  "<<myPrefs->sLightMapHIndex_GAr[0]<<" "<<myPrefs->sLightMapHIndex_GAr[1]<<std::endl;


    LroData->ImportSet( "LAr"
			,myPrefs->sLightMapFile_LAr
			,myPrefs->sLightMapTags_LAr
			,myPrefs->sLightMapHistos_LAr
			,myPrefs->sLightMapHIndex_LAr[0]
			,myPrefs->sLightMapHIndex_LAr[1]
			);


    LroData->ImportSet( "GAr"
			,myPrefs->sLightMapFile_GAr
			,myPrefs->sLightMapTags_GAr
			,myPrefs->sLightMapHistos_GAr
			,myPrefs->sLightMapHIndex_GAr[0]
			,myPrefs->sLightMapHIndex_GAr[1]
			);



    std::cout<<" --- "<<std::endl;

  }

  // define our VMC data handler
  DataVMC = new RawDataVirtualMC(theGeomConfig->GetGC_GeomType(), myPrefs->sVMCTransportCode);

  // set pointer to field and drift classes
  DataVMC->SetQdrift(Qdrift);
  DataVMC->SetEfield(Efield);

  //
  // set pointer to data containers
  //
  if(CrpData) DataVMC->SetCRPData(CrpData);

  //
  if(LroData) DataVMC->SetLROData(LroData);




  // beam type
  ierr = DataVMC->OpenInputFile(fname);

  if(ierr)
    {
      cout << " You are analyzing file -->" << fname << endl;

    }
  else
    {
      cout << " Error opening file -->" << fname << endl;
      delete DataVMC;
      DataVMC = NULL;
    }

  return ierr;
}

// next event : step over events
int EventLooper::NextEvent(void)
{
  int evt_status = 0;

  ++iEvent;

  //check if max number of events reached or iMaxEvent is not initialized
  if((iEvent>=iMaxEvent && iMaxEvent!=-1) || iMaxEvent == -1)
    return 0;

  cout<<"\n -------------- Event Number:"<<iEvent<<" --------------------------"<<endl;

  //makespace...
  WipeEvent();

  // initialize storage for next event
  if(CrpData)
    CrpData->BeginEvent();

  if(LroData)
    LroData->BeginEvent();


  if (myPrefs->iVerboseLevel>0)
    cout << "Event " << iEvent << " (status = " << evt_status << ")" << endl;

  // process event
  evt_status = DataVMC->LoadEvent();

  if (myPrefs->iVerboseLevel>0)
    cout << "Event " << iEvent << " (status = " << evt_status << ")" << endl;

  //cout<<"=============================================="<<endl;
  //cout <<"      here you can process event  "<<endl;
  //cout<<"=============================================="<<endl;

  // fill event header
  FillEventHeader();

  if (myPrefs->iVerboseLevel>0)
    theEventHeader->DumpEventHeader_True();

  // process raw data
  if(CrpData)
    CrpData->EndEvent();


  if(LroData)
    LroData->EndEvent();

  return evt_status;
}

// fill run header
void EventLooper::FillRunHeader(const char *fname)
{

  Long64_t runid = 123456;
  Float_t PBeam  = myPrefs->fPrimaryPabs[0];
  UInt_t gTime   = time(0);
  Int_t TrCd     = -999;
  if (!strcmp(myPrefs->sVMCTransportCode.c_str(),"G3")) TrCd = 3;
  if (!strcmp(myPrefs->sVMCTransportCode.c_str(),"G4")) TrCd = 4;
  Int_t St_seed = myPrefs->iPrimarySeed;
  Float_t lifetime=myPrefs->fForceLifetime;

  // diffusion constants
  Float_t Dl = myPrefs->fDiffusionDL;
  Float_t Dt = myPrefs->fDiffusionDT;

  theRunHeader->SetRunNumber(runid);
  theRunHeader->SetPBeam(PBeam);
  theRunHeader->SetRunTime(gTime);

  theRunHeader->SetTransportCode(TrCd);
  theRunHeader->SetStartSeed(St_seed);
  theRunHeader->SetLifeTime(lifetime);

  theRunHeader->SetDiffusionL(Dl);
  theRunHeader->SetDiffusionT(Dt);

// configuration file
  theRunHeader->SetConfigFile(myPrefs->sDLArConfFile);
//field map
   if(myPrefs->sEFieldType.compare("UNIFORM") == 0)  {
    theRunHeader->SetEfieldMap("UNIFORM");
    theRunHeader->SetEfieldValue(myPrefs->fEFieldValue);
  }
  if(myPrefs->sEFieldType.compare("EMAPFILE") == 0) {
     theRunHeader->SetEfieldMap(myPrefs->sEFieldMapFile);
     TString fullmapname = myPrefs->sEFieldMapFile;
     TString tokmap;
     Ssiz_t from = 0 ;
     while (fullmapname.Tokenize(tokmap,from,"[/]")){
       if (tokmap.BeginsWith("map")) {
        TObjArray *tvalue = tokmap.Tokenize("_");
        TString value=((TObjString*)(tvalue->At(2)))->String();
	theRunHeader->SetEfieldValue(value.Atof()*1000);
     }
    }
  }


//LightMAp

   theRunHeader->SetLMapLAR(myPrefs->sLightMapFile_LAr);
   theRunHeader->SetLMapGAR(myPrefs->sLightMapFile_GAr);
   theRunHeader->SetLSTime(myPrefs->LRO_SAMPLING_TIME);
   theRunHeader->SetPMTsQEff(myPrefs->LRO_PMTS_QEFF);


   cout <<"verifica "<<myPrefs->LRO_SAMPLING_TIME<<endl;

  TString tobeunpacked = fname;
  if (tobeunpacked.BeginsWith("pdg:")) {
    theRunHeader->SetDataType(1);
    TObjArray *tx = tobeunpacked.Tokenize(":");
    TString particle=((TObjString*)(tx->At(1)))->String();
    if (!strcmp(particle,"mu-")) theRunHeader->SetBeamType(6);
    if (!strcmp(particle,"mu+")) theRunHeader->SetBeamType(5);
    if (!strcmp(particle,"pi-")) theRunHeader->SetBeamType(9);
    if (!strcmp(particle,"pi+")) theRunHeader->SetBeamType(8);
    if (!strcmp(particle,"e-"))  theRunHeader->SetBeamType(3);
    if (!strcmp(particle,"geantino"))  theRunHeader->SetBeamType(0);
  }

  if (tobeunpacked.BeginsWith("gst:")) {
      theRunHeader->SetDataType(2);
      theRunHeader->SetGENIEfn(tobeunpacked);
      theRunHeader->SetReinteraction(myPrefs->reinteraction);
   }
  if (tobeunpacked.BeginsWith("NDgst:")) {
      theRunHeader->SetDataType(3);
      theRunHeader->SetGENIEfn(tobeunpacked);
   }
  if (tobeunpacked.BeginsWith("WA105_MCEVENT")) {
     theRunHeader->SetDataType(4);
     theRunHeader->SetCRBeam(myPrefs->sWa105BeamPdgName);
     theRunHeader->SetCRCRY(myPrefs->sWa105CosmicBkgFile);
     theRunHeader->SetCRTO(myPrefs->fWa105CosmicT0);
     theRunHeader->SetCRWIN(myPrefs->fWa105CosmicWin);
     theRunHeader->SetCREVNT(myPrefs->iWa105SingleCREvent);
     theRunHeader->SetCRRATE(myPrefs->fWa105CosmicRate);
   }

}

// wipe event
void EventLooper::WipeEvent(void)
{
  if(DataVMC)
    {
      if (DataVMC->GetGeoManager())
	DataVMC->GetGeoManager()->ClearTracks();
    }

  if (theEventHeader)
    delete theEventHeader;
  theEventHeader = NULL;

  theEventHeader = new EventHeader;
}
////////////////////////////////////////////////////////////////////////////
// Check geom config and fill branch geometry in RunHeader
////////////////////////////////////////////////////////////////////////////

void EventLooper::CheckCRPConfig(string geoconfig,string token1)
{

  ifstream f(geoconfig.c_str());

  if (!f)
    {
      cout <<"  detector configuration file "<<geoconfig<< "  does not exist  STOP "<<endl;
      exit(1);
    }

  // read file
  char *filename = new char [geoconfig.size()+1];
  std::copy(geoconfig.begin(), geoconfig.end(), filename);
  ifstream inFile;
  inFile.open(filename,ios::in);
  string aLine="";
  int ncrm = 0;
  string check1 = "DETECTOR";
  string check2 = "RAWDATATYPE";
  string check3 = "ELECMODEL";
  string check4 = "CRM";
  string check5 = "NOISE";

  int ped,maxbits,ifquanta,coherent;
  float pedrms;
  float freq,rms;
  float sampling;
  int id,nchax,nchay,nsample;
  float x,y,z,dx,dy,gain,pitchx,pitchy;


  while (!inFile.eof())
    {
      getline(inFile,aLine);
      istringstream ss;
      if (aLine.compare(0, check1.length(), check1)==0)
	{
	  string astring="";
	  string aname = "";
	  ss.str(aLine);
	  ss>>astring>>aname;
	  theGeomConfig->SetGC_GeomType( aname );
	}
//select RAWDATATYPE
       if (aLine.compare(0, check2.length(), check2)==0)
       {
	  string astring="";
	  string aname="";
	  ss.str(aLine);
	  ss>>astring>>aname>>ifquanta;
	  theGeomConfig->SetGC_RDType( aname );
	  theGeomConfig->SetGC_quanta( ifquanta );

       	}
      //      select ELECMODEL
      if  (aLine.compare(0,check3.length(),check3)==0)
	{

	  string astring="";
	  string aname="";
	  ss.str(aLine);
	  ss>>astring>>aname>>ped>>pedrms>>maxbits>>sampling;
	  theGeomConfig->SetGC_ElecModel(aname);
	  theGeomConfig->SetGC_Ped(ped);
	  theGeomConfig->SetGC_PedRMS(pedrms);
	  theGeomConfig->SetGC_maxbit(maxbits);
	  theGeomConfig->SetGC_samplingfreq(sampling);

	}
      if  (aLine.compare(0,check4.length(),check4)==0)
	{
	  string aname="";
	  string bname="";
	  ss.str(aLine);
	  ss>>aname>>id>>x>>y>>z>>dx>>dy>>gain>>pitchx>>pitchy>>nchax>>nchay>>nsample;

	  theGeomConfig->SetCRMcX(ncrm,x);
	  theGeomConfig->SetCRMcY(ncrm,y);
	  theGeomConfig->SetCRMcZ(ncrm,z);
	  theGeomConfig->SetCRMxdim(ncrm,dx);
	  theGeomConfig->SetCRMydim(ncrm,dy);
	  theGeomConfig->SetCRMpitchX(ncrm,pitchx);
	  theGeomConfig->SetCRMpitchY(ncrm,pitchy);
	  theGeomConfig->SetCRMnchaX(ncrm,nchax);
	  theGeomConfig->SetCRMnchaY(ncrm,nchay);
	  theGeomConfig->SetCRMgain(ncrm,gain);
	  theGeomConfig->SetCRMnsample(ncrm,nsample);

	  ncrm++;  //# CRM
	} // if  (aLine.compare(0,check4.length(),check4)==0)

	 if  (aLine.compare(0,check5.length(),check5)==0)
	{
	  string astring="";
	  string aname="";
	  ss.str(aLine);
	  ss>>astring>>aname>>freq>>rms>>coherent;
	  theGeomConfig->SetGC_NoiseName(aname);
	  theGeomConfig->SetGC_NoiseFreq(freq);
	  theGeomConfig->SetGC_NoiseRMS(rms);
	  theGeomConfig->SetGC_cohe(coherent);

	}




    } //eof

  theGeomConfig->SetCRMnum(ncrm);

  if (myPrefs->iVerboseLevel>5)
    theGeomConfig->DumpGeomConfig();

  // do crp geometry check
  theGeomConfig->CheckGeomConfig();

//usefull values

  ageom->SetGG_SRxdimension(theGeomConfig->GetGC_SRxdimension());
  ageom->SetGG_SRydimension(theGeomConfig->GetGC_SRydimension());
  ageom->SetGG_SRzdimension(theGeomConfig->GetGC_SRzdimension());


  int idgeo=0;

  if (token1.compare("6x6x6")==0) idgeo=6000+ncrm;
  if (token1.compare("3x1x1")==0) idgeo=3000+ncrm;

  theRunHeader->SetGeometry(idgeo);

}

// fill even header
void EventLooper::FillEventHeader()
{
  theEventHeader->SetEventNumber( iEvent );

  if(DataVMC)
    {
      theEventHeader->SetTrueTheta( DataVMC->GetTrueTht() );
      theEventHeader->SetTruePhi( DataVMC->GetTruePhi() );
      theEventHeader->SetTrueProdX( DataVMC->GetTrueVx() );
      theEventHeader->SetTrueProdY( DataVMC->GetTrueVy() );
      theEventHeader->SetTrueProdZ( DataVMC->GetTrueVz() );
      theEventHeader->SetTrueP( DataVMC->GetTrueP() );
      theEventHeader->SetTrueMomentum( 0, DataVMC->GetTruePx() );
      theEventHeader->SetTrueMomentum( 1, DataVMC->GetTruePy() );
      theEventHeader->SetTrueMomentum( 2, DataVMC->GetTruePz() );
      theEventHeader->SetTrueTotDepEne( DataVMC->GetEdep() );
      theEventHeader->SetTrueTotDepEneQ( DataVMC->GetEdepQ() );
      theEventHeader->SetTrueTotDepEEne( DataVMC->GetEdepE() );
      theEventHeader->SetTrueTotDepEEneQ( DataVMC->GetEdepEQ() );
      theEventHeader->SetTrueTotDepHEne( DataVMC->GetEdepH() );
      theEventHeader->SetTrueTotDepHEneQ( DataVMC->GetEdepHQ() );
      theEventHeader->SetTrueEneLost( DataVMC->GetElost() );
      theEventHeader->SetTrueTotDepEneCRT1( DataVMC->GetEdepCRT1() );
      theEventHeader->SetTrueTotDepEneCRT2( DataVMC->GetEdepCRT2() );
      theEventHeader->SetPrimaryArray( DataVMC->GetPrimaries() );
    }
}
